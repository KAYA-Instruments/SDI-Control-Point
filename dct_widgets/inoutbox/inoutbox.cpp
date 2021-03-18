/******************************************************************************
 * Copyright (C) 2017 Dream Chip Technologies GmbH
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/
/**
 * @file    inoutbox.cpp
 *
 * @brief   Implementation of input/output setting box
 *
 *****************************************************************************/
#include <QtDebug>
#include <QProxyStyle>
#include <QThread>
#include <QTime>
#include <QMessageBox>

#include "inoutbox.h"
#include "ui_inoutbox.h"
#include "defines.h"

#include <aecweightsdialog.h>

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_InOutBox;
}

#define AUTO_REPEAT_THRESHOLD    ( 10000 )

/******************************************************************************
 * Overrule unconfigureable timer implementation in qt, there's  no property
 * in spinbox class to do this => use proxy-style (Arrggghhh!!!)
 *
 * Problem: Due to a long processing time of some provideo commands the 
 *          internal repeat-timer of spin-box gets expired. This leads to an 
 *          repetation of setValue event in the spin-box instance by button
 *          releases and finally the spinbox-value is decreased or increased 
 *          twice by clicking up- or down-button once.
 *
 * Hope my qt-patch will be accepted to make it configurable. 
 *****************************************************************************/
class SpinBoxStyle : public QProxyStyle
{
public:
    SpinBoxStyle( QStyle *style = nullptr ) : QProxyStyle(style) { }

    int styleHint
    (
        StyleHint hint,
        const QStyleOption * option = nullptr,
        const QWidget * widget = nullptr,
        QStyleHintReturn * returnData = nullptr
    ) const Q_DECL_OVERRIDE
    {
        if ( hint == QStyle::SH_SpinBox_ClickAutoRepeatThreshold )
        {
            return ( AUTO_REPEAT_THRESHOLD );
        }

        return ( QProxyStyle::styleHint(hint, option, widget, returnData) );
    }
};

/******************************************************************************
 * local definitions
 *****************************************************************************/
#define INOUT_SETTINGS_SECTION_NAME                 ( "INOUT" )

#define INOUT_SETTINGS_BAYER_PATTERN                ( "bayer" )
#define INOUT_SETTINGS_CAMERA_ISO                   ( "iso" )
#define INOUT_SETTINGS_CAMERA_EXPOSURE              ( "shutter" )

#define INOUT_SETTINGS_AEC_ENABLE                   ( "aec_enable" )
#define INOUT_SETTINGS_AEC_USE_CUSTOM_WEIGHTS       ( "aec_use_custom_weights" )
#define INOUT_SETTINGS_AEC_WEIGHTS                  ( "aec_weights" )
#define INOUT_SETTINGS_AEC_SETPOINT                 ( "aec_setpoint" )
#define INOUT_SETTINGS_AEC_MAX_ISO                  ( "aec_max_iso" )
#define INOUT_SETTINGS_AEC_SPEED                    ( "aec_speed" )
#define INOUT_SETTINGS_AEC_FLICKER                  ( "aec_flicker" )

#define INOUT_SETTINGS_LSC_ENABLE                   ( "lsc_enable" )
#define INOUT_SETTINGS_LSC_K                        ( "lsc_k" )
#define INOUT_SETTINGS_LSC_OFFSET                   ( "lsc_offset" )
#define INOUT_SETTINGS_LSC_SLOPE                    ( "lsc_slope" )

#define INOUT_SETTINGS_VIDEO_MODE                   ( "video_mode" )
#define INOUT_SETTINGS_SDI2_MODE                    ( "sdi2_mode" )
#define INOUT_SETTINGS_SDI1_DOWNSCALER              ( "sdi1_downscaler" )
#define INOUT_SETTINGS_SDI2_DOWNSCALER              ( "sdi2_downscaler" )
#define INOUT_SETTINGS_FLIP_MODE                    ( "flip_mode" )
#define INOUT_SETTINGS_LOG_MODE                     ( "log_mode" )
#define INOUT_SETTINGS_COLOR_SPACE                  ( "color_space" )
#define INOUT_SETTINGS_PQ_MAX_BRIGHTNESS            ( "pq_max_brightness" )
#define INOUT_SETTINGS_TEST_PATTERN                 ( "test_pattern" )
#define INOUT_SETTINGS_AUDIO_ENABLE                 ( "audio_enable" )
#define INOUT_SETTINGS_AUDIO_GAIN                   ( "audio_gain" )

#define INOUT_SETTINGS_GENLOCK_MODE                 ( "genlock_mode" )
#define INOUT_SETTINGS_GENLOCK_CROSSLOCK_ENABLE     ( "genlock_crosslock_enable" )
#define INOUT_SETTINGS_GENLOCK_CROSSLOCK_VMODE      ( "genlock_crosslock_vmode" )
#define INOUT_SETTINGS_GENLOCK_OFFSET_VERTICAL      ( "genlock_offset_vertical" )
#define INOUT_SETTINGS_GENLOCK_OFFSET_HORIZONTAL    ( "genlock_offset_horizontal" )
#define INOUT_SETTINGS_GENLOCK_TERMINATION          ( "genlock_termination" )
#define INOUT_SETTINGS_GENLOCK_LOL_FILTER           ( "genlock_lol_filter" )

typedef struct aec_setup_t {
    bool run;
    int setPoint;
    int speed;
    int clmTolerance;
    int activeGain;
    int activeExposure;
    int activeAperture;
    int maxExposure;
    int maxGain;
    bool useCustomWeighting;
    uint8_t weights[25];
} aec_setup_t;

/******************************************************************************
 * InOutBox::PrivateData
 *****************************************************************************/
class InOutBox::PrivateData
{
public:
    PrivateData()
        : m_ui( new Ui::UI_InOutBox )
        , m_cntEvents( 0 )
        , m_maxEvents( -1 )
        , m_sbxStyle( new SpinBoxStyle() )
        , m_AecSetup{}
        , m_LscSetup{}
        , m_AptMin( 125 )
        , m_AptMax( 16000 )
        , m_AptEnable( true )
        , m_minIso( 1 )
        , m_weightDialog( new AecWeightsDialog() )
        , offset_x_step ( 1 )
        , offset_y_step ( 1 )
        , roi_offset_x ( 0 )
        , roi_offset_y ( 0 )
    {
        // do nothing
    }

    ~PrivateData()
    {
        delete m_ui;
        delete m_sbxStyle;
        delete m_weightDialog;
    }

    Ui::UI_InOutBox *   m_ui;           /**< ui handle */
    int                 m_cntEvents;    /**< ignore move-events if slider moving */
    int                 m_maxEvents;    /**< number of ignored move-events */
    SpinBoxStyle *      m_sbxStyle;     /**< proxy style to overrule not implemented spinbox properties */

    aec_setup_t         m_AecSetup;

    lsc_setup_t         m_LscSetup;

    int                 m_AptMin;
    int                 m_AptMax;
    bool                m_AptEnable;

    int                 m_minIso;       /**< iso value of the device at gain 1 (1000) */

    AecWeightsDialog *  m_weightDialog;

    int                 offset_x_step;
    int                 offset_y_step;
    int                 roi_offset_x;
    int                 roi_offset_y;
};

/******************************************************************************
 * InOutBox::InOutBox
 *****************************************************************************/
InOutBox::InOutBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData;

    // initialize UI
    d_data->m_ui->setupUi( this );

    d_data->m_weightDialog->setParent( this, Qt::Dialog);

    d_data->m_ui->sbxAnalogGain->setRange( 0, 252 );
    d_data->m_ui->sldAnalogGain->setRange( 0, 252 );
    
    d_data->m_ui->sbxExposure->setRange( 100, 20000 );
    d_data->m_ui->sldExposure->setRange( 100, 20000 );

    d_data->m_ui->sbxSetPoint->setRange( 0, 4095 );
    d_data->m_ui->sldSetPoint->setRange( 0, 4095 );

    d_data->m_ui->sbxMaxAnalogGain->setRange( 0, 252 );
    d_data->m_ui->sldMaxAnalogGain->setRange( 0, 252 );

    d_data->m_ui->sbxMaxExposure->setRange( 100, 20000 );
    d_data->m_ui->sldMaxExposure->setRange( 100, 20000 );
    
    d_data->m_ui->sbxControlSpeed->setRange( 0, 1000 );
    d_data->m_ui->sldControlSpeed->setRange( 0, 1000 );

    d_data->m_ui->sbxTolerance->setRange( 0, 100 );
    d_data->m_ui->sldTolerance->setRange( 0, 1000 );

    d_data->m_ui->sbxK->setRange( 0.0, 2.0 );
    d_data->m_ui->sbxK->setSingleStep( 0.01 );
    d_data->m_ui->sbxK->setKeyboardTracking( false );
    d_data->m_ui->sldK->setRange( 0, 200 );
    d_data->m_ui->sldK->setPageStep( 10 );

    d_data->m_ui->sbxOffset->setRange( 0.0, 1.0 );
    d_data->m_ui->sbxOffset->setSingleStep( 0.01 );
    d_data->m_ui->sbxOffset->setKeyboardTracking( false );
    d_data->m_ui->sldOffset->setRange( 0, 100 );
    d_data->m_ui->sldOffset->setPageStep( 5 );

    d_data->m_ui->sbxSlope->setRange( 0.0, 2.0 );
    d_data->m_ui->sbxSlope->setSingleStep( 0.01 );
    d_data->m_ui->sbxSlope->setKeyboardTracking( false );
    d_data->m_ui->sldSlope->setRange( 0, 200 );
    d_data->m_ui->sldSlope->setPageStep( 10 );

    d_data->m_ui->tmeTimecode->setTime( QTime(0,0,0) );

    d_data->m_ui->sbxAperture->setRange( 0, 100 );
    d_data->m_ui->sldAperture->setRange( 0, 100 );
    
    d_data->m_ui->sbxGenLockOffsetVertical->setRange( -4095, 4095 );
    d_data->m_ui->sbxGenLockOffsetVertical->setKeyboardTracking( false );
    d_data->m_ui->sbxGenlockOffsetHorizontal->setRange( -4095, 4095 );
    d_data->m_ui->sbxGenlockOffsetHorizontal->setKeyboardTracking( false );

    d_data->m_ui->sbxRoiOffsetX->setRange( 0, 100 );
    d_data->m_ui->sldRoiOffsetX->setRange( 0, 100 );
    d_data->m_ui->sbxRoiOffsetY->setRange( 0, 100 );
    d_data->m_ui->sldRoiOffsetY->setRange( 0, 100 );

    // fill bayer pattern combo box
    for ( int i=BayerPatternFirst; i<BayerPatternMax; i++ )
    {
        addBayerPattern( GetBayerPatternName( static_cast<enum BayerPattern>(i) ), i );
    }

    // fill sdi2-mode combo box
    for ( int i=Sdi2ModeFirst; i<Sdi2ModeMax; i++ )
    {
        addSdi2Mode( GetSdi2ModeName( static_cast<enum Sdi2Mode>(i) ), i );
    }

    // fill downscale mode combo boxes
    for ( int i=DownscaleModeFirst; i<DownscaleModeMax; i++ )
    {
        addDownscaleMode( GetDownscaleModeName( static_cast<enum DownscaleMode>(i) ), i );
    }

    // Note: Flip modes depend on the device and are added in "setFlipModeVisible()"

    // fill log mode combo box
    for ( int i=LogModeFirst; i<LogModeMax; i++ )
    {
        addLogMode( GetLogModeName( static_cast<enum LogMode>(i) ), i );
    }

    // fill color space combo box
    for ( int i=ColorSpaceFirst; i<ColorSpaceMax; i++ )
    {
        addColorSpace( GetColorSpaceName( static_cast<enum ColorSpace>(i) ), i );
    }

    // fill genlock-mode combo box
    for ( int i=GenLockModeFirst; i<GenLockModeMax; i++ )
    {
        addGenlockMode( GetGenlockModeName( static_cast<enum GenLockMode>(i) ), i );
    }

    // fill genlock-crosslock combo boxes
    for ( int i=GenLockCrosslockEnableFirst; i<GenLockCrosslockEnableMax; i++ )
    {
        addGenlockCrosslockEnable( GetGenlockCrosslockEnableName( static_cast<enum GenLockCrosslockEnable>(i) ), i );
    }

    // overrule auto-repeat threshold
    d_data->m_ui->sbxAnalogGain             ->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxExposure               ->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxGenLockOffsetVertical  ->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxGenlockOffsetHorizontal->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxSetPoint               ->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxMaxAnalogGain          ->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxAperture               ->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxK                      ->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxOffset                 ->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxSlope                  ->setStyle( d_data->m_sbxStyle );

    // connect internal signals
    // bayer pattern
    connect( d_data->m_ui->cbxBayerPattern, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxBayerPatternChange(int)) );

    // gain
    connect( d_data->m_ui->sldAnalogGain, SIGNAL(valueChanged(int)), this, SLOT(onSldIsoChange(int)) );
    connect( d_data->m_ui->sldAnalogGain, SIGNAL(sliderReleased()), this, SLOT(onSldIsoReleased()) );
    connect( d_data->m_ui->sbxAnalogGain, SIGNAL(valueChanged(double)), this, SLOT(onSbxIsoChange(double)) );
    connect( d_data->m_ui->cbxIso, SIGNAL(activated(int)), this, SLOT(onCbxIsoChange(int)) );

    // exposure
    connect( d_data->m_ui->sldExposure, SIGNAL(valueChanged(int)), this, SLOT(onSldExposureChange(int)) );
    connect( d_data->m_ui->sldExposure, SIGNAL(sliderReleased()), this, SLOT(onSldExposureReleased()) );
    connect( d_data->m_ui->sbxExposure, SIGNAL(valueChanged(int)), this, SLOT(onSbxExposureChange(int)) );
    connect( d_data->m_ui->cbxExposure, SIGNAL(activated(int)), this, SLOT(onCbxExposureChange(int)) );
    connect( d_data->m_ui->btnExposureMinus, SIGNAL(clicked()), this, SLOT(onBtnExposureMinusClicked()) );
    connect( d_data->m_ui->btnExposurePlus, SIGNAL(clicked()), this, SLOT(onBtnExposurePlusClicked()) );

    // auto exposure & gain
    connect( d_data->m_ui->cbxAecEnable, SIGNAL(stateChanged(int)), this, SLOT(onCbxAecEnableChange(int)) );
    connect( d_data->m_ui->cbxAecWeight, SIGNAL(stateChanged(int)), this, SLOT(onCbxAecWeightChange(int)) );
    connect( d_data->m_ui->cbxMaxAnalogGainEnable, SIGNAL(stateChanged(int)), this, SLOT(onCbxMaxAnalogGainEnableChange(int)) );
    connect( d_data->m_ui->cbxMaxExposureEnable, SIGNAL(stateChanged(int)), this, SLOT(onCbxMaxExposureEnableChange(int)) );

    connect( this, SIGNAL(WeightDialogAecWeightsChanged(QVector<int>)), d_data->m_weightDialog, SLOT(onAecWeightsChange(QVector<int>)) );
    connect( d_data->m_weightDialog, SIGNAL(AecWeightChanged(int,int)), this, SIGNAL(AecWeightChanged(int,int)) );

    connect( d_data->m_ui->btnAecWeight, SIGNAL(clicked()), d_data->m_weightDialog, SLOT(show()) );

    connect( d_data->m_ui->sldSetPoint, SIGNAL(valueChanged(int)), this, SLOT(onSldSetPointChange(int)) );
    connect( d_data->m_ui->sldSetPoint, SIGNAL(sliderReleased()), this, SLOT(onSldSetPointReleased()) );
    connect( d_data->m_ui->sbxSetPoint, SIGNAL(valueChanged(int)), this, SLOT(onSbxSetPointChange(int)) );

    connect( d_data->m_ui->sldMaxAnalogGain, SIGNAL(valueChanged(int)), this, SLOT(onSldMaxAnalogGainChange(int)) );
    connect( d_data->m_ui->sldMaxAnalogGain, SIGNAL(sliderReleased()), this, SLOT(onSldMaxAnalogGainReleased()) );
    connect( d_data->m_ui->sbxMaxAnalogGain, SIGNAL(valueChanged(double)), this, SLOT(onSbxMaxAnalogGainChange(double)) );

    connect( d_data->m_ui->sldMaxExposure, SIGNAL(valueChanged(int)), this, SLOT(onSldMaxExposureChange(int)) );
    connect( d_data->m_ui->sldMaxExposure, SIGNAL(sliderReleased()), this, SLOT(onSldMaxExposureReleased()) );
    connect( d_data->m_ui->sbxMaxExposure, SIGNAL(valueChanged(int)), this, SLOT(onSbxMaxExposureChange(int)) );

    connect( d_data->m_ui->sldControlSpeed, SIGNAL(valueChanged(int)), this, SLOT(onSldControlSpeedChange(int)) );
    connect( d_data->m_ui->sldControlSpeed, SIGNAL(sliderReleased()), this, SLOT(onSldControlSpeedReleased()) );
    connect( d_data->m_ui->sbxControlSpeed, SIGNAL(valueChanged(int)), this, SLOT(onSbxControlSpeedChange(int)) );

    connect( d_data->m_ui->rdbTaf50Hz, SIGNAL( toggled(bool) ), this, SLOT(onTafToggle(bool)) );
    connect( d_data->m_ui->rdbTaf60Hz, SIGNAL( toggled(bool) ), this, SLOT(onTafToggle(bool)) );

    connect( d_data->m_ui->sldAperture, SIGNAL(valueChanged(int)), this, SLOT(onSldIrisAptChange(int)) );
    connect( d_data->m_ui->sldAperture, SIGNAL(sliderReleased()), this, SLOT(onSldIrisAptReleased()) );
    connect( d_data->m_ui->sbxAperture, SIGNAL(valueChanged(int)), this, SLOT(onSbxIrisAptChange(int)) );

    connect( d_data->m_ui->sldTolerance, SIGNAL(valueChanged(int)), this, SLOT(onSldToleranceChange(int)) );
    connect( d_data->m_ui->sldTolerance, SIGNAL(sliderReleased()), this, SLOT(onSldToleranceReleased()) );
    connect( d_data->m_ui->sbxTolerance, SIGNAL(valueChanged(double)), this, SLOT(onSbxToleranceChange(double)) );

    // video
    connect( d_data->m_ui->cbxVideoMode, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxVideoModeChange(int)) );
    connect( d_data->m_ui->cbxSdi2Mode, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxSdi2ModeChange(int)) );
    connect( d_data->m_ui->cbxSdi1Downscaler, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxSdi1DownscalerChange(int)) );
    connect( d_data->m_ui->cbxSdi2Downscaler, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxSdi2DownscalerChange(int)) );
    connect( d_data->m_ui->cbxFlipMode, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxFlipModeChange(int)) );
    connect( d_data->m_ui->cbxLogMode, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxLogModeChange(int)) );
    connect( d_data->m_ui->sbxPQMaxBrightness, SIGNAL(valueChanged(int)), this, SLOT(onSbxPQMaxBrightnessChange(int)) );
    connect( d_data->m_ui->cbxColorSpace, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxColorSpaceChange(int)) );
    connect( d_data->m_ui->cbxTestPattern, SIGNAL(stateChanged(int)), this, SLOT(onCbxTestPatternChange(int)) );
    connect( d_data->m_ui->btnIsoMinus, SIGNAL(clicked()), this, SLOT(onBtnIsoMinusClicked()) );
    connect( d_data->m_ui->btnIsoPlus, SIGNAL(clicked()), this, SLOT(onBtnIsoPlusClicked()) );

    // audio
    connect( d_data->m_ui->cbxAudioEnable , SIGNAL(stateChanged(int)), this, SLOT(onCbxAudioEnableChange(int)) );
    connect( d_data->m_ui->sbxAudioGain, SIGNAL(valueChanged(double)), this, SLOT(onSbxAudioGainChange(double)) );
    
    // genlock
    connect( d_data->m_ui->cbxGenLockMode , SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxGenlockModeChange(int)) );
    connect( d_data->m_ui->btnGenLockRefreshState , SIGNAL(clicked()), this, SLOT(onBtnGenlockStatusRefresh()) );
    connect( d_data->m_ui->cbxGenlockCrosslockEnable , SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxGenlockCrosslockEnableChange(int)) );
    connect( d_data->m_ui->cbxGenlockCrosslockVmode , SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxGenlockCrosslockVmodeChange(int)) );
    connect( d_data->m_ui->sbxGenLockOffsetVertical, SIGNAL(valueChanged(int)), this, SLOT(onSbxGenlockOffsetVerticalChange(int)) );
    connect( d_data->m_ui->sbxGenlockOffsetHorizontal, SIGNAL(valueChanged(int)), this, SLOT(onSbxGenlockOffsetHorizontalChange(int)) );
    connect( d_data->m_ui->cbxGenLockTermination, SIGNAL(stateChanged(int)), this, SLOT(onCbxGenlockTerminationChange(int)) );
    connect( d_data->m_ui->sbxGenLockLOLFilter, SIGNAL(valueChanged(int)), this, SLOT(onSbxGenlockLOLFilterValueChange(int)) );

    // ROI
    connect( d_data->m_ui->sbxRoiOffsetX, SIGNAL(valueChanged(int)), this, SLOT(onSbxRoiOffsetXChange(int)) );
    connect( d_data->m_ui->sbxRoiOffsetY, SIGNAL(valueChanged(int)), this, SLOT(onSbxRoiOffsetYChange(int)) );
    connect( d_data->m_ui->sldRoiOffsetX, SIGNAL(valueChanged(int)), this, SLOT(onSldRoiOffsetXChange(int)) );
    connect( d_data->m_ui->sldRoiOffsetY, SIGNAL(valueChanged(int)), this, SLOT(onSldRoiOffsetYChange(int)) );

    // timecode
    connect( d_data->m_ui->btnSetTimecode, SIGNAL(clicked()), this, SLOT(onBtnTimecodeSetClicked()) );
    connect( d_data->m_ui->btnGetTimecode, SIGNAL(clicked()), this, SLOT(onBtnTimecodeGetClicked()) );
    connect( d_data->m_ui->btnHoldTimecode, SIGNAL(clicked(bool)), this, SLOT(onBtnTimecodeHoldClicked(bool)) );

    // lense shading correction
    connect( d_data->m_ui->cbxLscEnable, SIGNAL(stateChanged(int)), this, SLOT(onCbxLscEnableChange(int)) );

    connect( d_data->m_ui->sbxK, SIGNAL(valueChanged(double)), this, SLOT(onSbxKChange(double)) );
    connect( d_data->m_ui->sldK, SIGNAL(valueChanged(int)), this, SLOT(onSldKChange(int)) );
    connect( d_data->m_ui->sldK, SIGNAL(sliderReleased()), this, SLOT(onSldKReleased()) );

    connect( d_data->m_ui->sbxOffset, SIGNAL(valueChanged(double)), this, SLOT(onSbxOffsetChange(double)) );
    connect( d_data->m_ui->sldOffset, SIGNAL(valueChanged(int)), this, SLOT(onSldOffsetChange(int)) );
    connect( d_data->m_ui->sldOffset, SIGNAL(sliderReleased()), this, SLOT(onSldOffsetReleased()) );

    connect( d_data->m_ui->sbxSlope, SIGNAL(valueChanged(double)), this, SLOT(onSbxSlopeChange(double)) );
    connect( d_data->m_ui->sldSlope, SIGNAL(valueChanged(int)), this, SLOT(onSldSlopeChange(int)) );
    connect( d_data->m_ui->sldSlope, SIGNAL(sliderReleased()), this, SLOT(onSldSlopeReleased()) );

    /*
     * Manulal hiding the following features.
     * This features are currently not available the IronSDI
     */
    d_data->m_ui->lblAecWeights->hide();
    d_data->m_ui->cbxAecWeight->hide();
    d_data->m_ui->btnAecWeight->hide();
    d_data->m_ui->horizontalLayout_4->layout()->removeItem(d_data->m_ui->horizontalSpacer_5);

    d_data->m_ui->lblTaf->hide();
    d_data->m_ui->rdbTaf50Hz->hide();
    d_data->m_ui->rdbTaf60Hz->hide();

    d_data->m_ui->lblColorSpace->hide();
    d_data->m_ui->cbxColorSpace->hide();

    // Temporary hide unimplemented GenLock features
    d_data->m_ui->cbxGenLockTermination->hide();
    d_data->m_ui->lblGenLockTermination->hide();
    d_data->m_ui->lblGenlockCrosslock->hide();
    d_data->m_ui->cbxGenlockCrosslockEnable->hide();
    d_data->m_ui->cbxGenlockCrosslockVmode->hide();
    d_data->m_ui->gbxGenLockSettings->layout()->removeItem(d_data->m_ui->horizontalSpacer);
    d_data->m_ui->sbxGenLockOffsetVertical->hide();
    d_data->m_ui->lblGenLockOffsetVertical->hide();
    d_data->m_ui->sbxGenlockOffsetHorizontal->hide();
    d_data->m_ui->lblGenLockOffsetHorizontal->hide();
    //*******************************************

    ////////////////////
    // operation mode
    ////////////////////
    prepareMode( mode() ); 
}

/******************************************************************************
 * InOutBox::~InOutBox
 *****************************************************************************/
InOutBox::~InOutBox()
{
    delete d_data;
}

/******************************************************************************
 * gain_to_iso
 *****************************************************************************/
int InOutBox::gainToIso( int gain ) const
{
    return ( ((gain * d_data->m_minIso) + 500) / 1000 );
}

/******************************************************************************
 * isoToGain
 *****************************************************************************/
int InOutBox::isoToGain( int iso ) const
{
    return ( (iso * 1000) / d_data->m_minIso );
}

/******************************************************************************
 * multiplyBy1000
 *****************************************************************************/
int InOutBox::multiplyBy1000( int value ) const
{
    return ( value * 1000 );
}

/******************************************************************************
 * show4kGenlockNote
 *****************************************************************************/
void InOutBox::show4kGenlockNote( int mode )
{
    /* If this is a 4K / UHD video mode, show the note on genlock and 4k */
    if ( mode >= VideoModeFirstUHD  && mode <= VideoModeLast4K )
    {
        d_data->m_ui->lblGenlock4kNote->setVisible( true );
    }
    else
    {
        d_data->m_ui->lblGenlock4kNote->setVisible( false );
    }
}

/******************************************************************************
 * isoToGain
 *****************************************************************************/
void InOutBox::EmitDownscaleChanged( int sdi_out_idx, int combo_box_idx )
{
    switch ( combo_box_idx )
    {
    case 0: // Disable downscaler
        emit ChainDownscaleModeChanged( sdi_out_idx, 0, 0 );
        break;
    case 1: // Enable downscaler
        emit ChainDownscaleModeChanged( sdi_out_idx, 1, 0 );
        break;
    case 2: // Enable downscaler and interlacer
        emit ChainDownscaleModeChanged( sdi_out_idx, 1, 1 );
        break;
    }
}

/******************************************************************************
 * InOutBox::BayerPattern
 *****************************************************************************/
int InOutBox::BayerPattern() const
{
    return ( d_data->m_ui->cbxBayerPattern->currentData().toInt() );
}

/******************************************************************************
 * InOutBox::setBayerPattern
 *****************************************************************************/
void InOutBox::setBayerPattern( const int value )
{
    int index = d_data->m_ui->cbxBayerPattern->findData( value );
    if ( index != -1 )
    {
        d_data->m_ui->cbxBayerPattern->blockSignals( true );
        d_data->m_ui->cbxBayerPattern->setCurrentIndex( index );
        d_data->m_ui->cbxBayerPattern->blockSignals( false );

        emit BayerPatternChanged( value );
    } 
}

/******************************************************************************
 * InOutBox::CameraIso
 *****************************************************************************/
int InOutBox::CameraIso() const
{
    return ( d_data->m_ui->sbxAnalogGain->value() );
}

/******************************************************************************
 * InOutBox::setCameraGain
 *****************************************************************************/
void InOutBox::setCameraIso( const int value )
{
    d_data->m_ui->sldAnalogGain->blockSignals( true );
    d_data->m_ui->sldAnalogGain->setValue( value );
    d_data->m_ui->sldAnalogGain->blockSignals( false );

    d_data->m_ui->sbxAnalogGain->blockSignals( true );
    d_data->m_ui->sbxAnalogGain->setValue( value );
    d_data->m_ui->sbxAnalogGain->blockSignals( false );

    setWaitCursor();
    emit CameraGainChanged( isoToGain(value) );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::CameraExposure
 *****************************************************************************/
int InOutBox::CameraExposure() const
{
    return ( d_data->m_ui->sbxExposure->value() );
}

/******************************************************************************
 * InOutBox::setCameraExposure
 *****************************************************************************/
void InOutBox::setCameraExposure( const int value )
{
    d_data->m_ui->sldExposure->blockSignals( true );
    d_data->m_ui->sldExposure->setValue( value );
    d_data->m_ui->sldExposure->blockSignals( false );

    d_data->m_ui->sbxExposure->blockSignals( true );
    d_data->m_ui->sbxExposure->setValue( value );
    d_data->m_ui->sbxExposure->blockSignals( false );

    setWaitCursor();
    emit CameraExposureChanged( value );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::AecEnable
 *****************************************************************************/
bool InOutBox::AecEnable() const
{
    return ( d_data->m_AecSetup.run );
}

/******************************************************************************
 * InOutBox::AecEnable
 *****************************************************************************/
void InOutBox::setAecEnable( const bool value )
{
    enableAecWidgets( value );
    d_data->m_ui->cbxAecEnable->blockSignals( true );
    d_data->m_ui->cbxAecEnable->setCheckState( value ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxAecEnable->blockSignals( false );

    d_data->m_AecSetup.run = value;

    setWaitCursor();
    emit AecEnableChanged( d_data->m_AecSetup.run );
    setNormalCursor();
    
    // aec disabled, update exposure, gain, aperture widgets
    if ( !value )
    {
        setWaitCursor();
        emit ResyncRequest();
        setNormalCursor();
    }
}

/******************************************************************************
 * InOutBox::AecSetPoint
 *****************************************************************************/
int InOutBox::AecSetPoint() const
{
    return ( d_data->m_AecSetup.setPoint );
}

/******************************************************************************
 * InOutBox::setAecSetPoint
 *****************************************************************************/
void InOutBox::setAecSetPoint( const int value )
{
    d_data->m_ui->sbxSetPoint->blockSignals( true );
    d_data->m_ui->sbxSetPoint->setValue( value );
    d_data->m_ui->sbxSetPoint->blockSignals( false );

    d_data->m_ui->sldSetPoint->blockSignals( true );
    d_data->m_ui->sldSetPoint->setValue( value );
    d_data->m_ui->sldSetPoint->blockSignals( false );

    d_data->m_AecSetup.setPoint = value;
    
    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::AecMaxIso
 *****************************************************************************/
int InOutBox::AecMaxIso() const
{
    return ( gainToIso(d_data->m_AecSetup.maxGain) );
}

/******************************************************************************
 * InOutBox::setAecMaxIso
 *****************************************************************************/
void InOutBox::setAecMaxIso( const int value )
{
    d_data->m_ui->sbxMaxAnalogGain->blockSignals( true );
    d_data->m_ui->sbxMaxAnalogGain->setValue( value );
    d_data->m_ui->sbxMaxAnalogGain->blockSignals( false );

    d_data->m_ui->sldMaxAnalogGain->blockSignals( true );
    d_data->m_ui->sldMaxAnalogGain->setValue( value );
    d_data->m_ui->sldMaxAnalogGain->blockSignals( false );

    d_data->m_AecSetup.maxGain = isoToGain( value );

    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::AecControlSpeed
 *****************************************************************************/
int InOutBox::AecControlSpeed() const
{
    return ( d_data->m_AecSetup.speed );
}

/******************************************************************************
 * InOutBox::setAecControlSpeed
 *****************************************************************************/
void InOutBox::setAecControlSpeed( const int value )
{
    d_data->m_ui->sbxControlSpeed->blockSignals( true );
    d_data->m_ui->sbxControlSpeed->setValue( value );
    d_data->m_ui->sbxControlSpeed->blockSignals( false );

    d_data->m_ui->sldControlSpeed->blockSignals( true );
    d_data->m_ui->sldControlSpeed->setValue( value );
    d_data->m_ui->sldControlSpeed->blockSignals( false );

    d_data->m_AecSetup.speed = value;
    
    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::AecFlickerFrequency
 *****************************************************************************/
int InOutBox::AecFlickerFrequency() const
{
    return ( d_data->m_AecSetup.maxExposure );
}

/******************************************************************************
 * InOutBox::setAecFlickerFrequency
 *****************************************************************************/
void InOutBox::setAecFlickerFrequency( const int value )
{
    if ( value == 8333 || value == 10000 )
    {
        //d_data->m_AecSetup.maxExposure = value;
    }
    else
    {
        //d_data->m_AecSetup.maxExposure = 10000;
    }

    d_data->m_ui->rdbTaf50Hz->blockSignals( true );
    d_data->m_ui->rdbTaf50Hz->setChecked( d_data->m_AecSetup.maxExposure == 10000 );
    d_data->m_ui->rdbTaf50Hz->blockSignals( false );
    
    d_data->m_ui->rdbTaf60Hz->blockSignals( true );
    d_data->m_ui->rdbTaf60Hz->setChecked( d_data->m_AecSetup.maxExposure == 8333 );
    d_data->m_ui->rdbTaf60Hz->blockSignals( false );

    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::AecCustomWeights
 *****************************************************************************/
bool InOutBox::AecCustomWeights() const
{
    return ( d_data->m_AecSetup.useCustomWeighting );
}

/******************************************************************************
 * InOutBox::setAecCustomWeights
 *****************************************************************************/
void InOutBox::setAecCustomWeights( const bool value )
{
    d_data->m_ui->cbxAecWeight->blockSignals( true );
    d_data->m_ui->cbxAecWeight->setCheckState( value ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxAecWeight->blockSignals( false );

    d_data->m_AecSetup.useCustomWeighting = value;

    // Run enable aec widgets to enable / disable the set weighting button
    enableAecWidgets( d_data->m_AecSetup.run );

    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::LscEnable
 *****************************************************************************/
bool InOutBox::LscEnable() const
{
    return ( d_data->m_LscSetup.enable );
}

/******************************************************************************
 * InOutBox::LscK
 *****************************************************************************/
float InOutBox::LscK() const
{
    return ( d_data->m_LscSetup.k );
}

/******************************************************************************
 * InOutBox::LscOffset
 *****************************************************************************/
float InOutBox::LscOffset() const
{
    return ( d_data->m_LscSetup.offset );
}

/******************************************************************************
 * InOutBox::LscSlope
 *****************************************************************************/
float InOutBox::LscSlope() const
{
    return ( d_data->m_LscSetup.slope );
}

/******************************************************************************
 * InOutBox::setLsc
 *****************************************************************************/
void InOutBox::setLsc(lsc_setup_t setup )
{
    d_data->m_LscSetup = setup;

    updateLscWidgets();

    emit LscChanged( createLscVector() );
}

/******************************************************************************
 * InOutBox::VideoMode
 *****************************************************************************/
QString InOutBox::VideoMode() const
{
    return ( d_data->m_ui->cbxVideoMode->currentText() );
}

/******************************************************************************
 * InOutBox::setVideoMode
 *****************************************************************************/
void InOutBox::setVideoMode( const QString mode )
{
    int index = d_data->m_ui->cbxVideoMode->findText( mode );
    if ( index != -1 )
    {
        d_data->m_ui->cbxVideoMode->blockSignals( true );
        d_data->m_ui->cbxVideoMode->setCurrentIndex( index );
        show4kGenlockNote( d_data->m_ui->cbxVideoMode->itemData( index ).toInt() );
        d_data->m_ui->cbxVideoMode->blockSignals( false );

        emit ChainVideoModeChanged( d_data->m_ui->cbxVideoMode->itemData( index ).toInt() );
    }
}

/******************************************************************************
 * InOutBox::Sdi2Mode
 *****************************************************************************/
QString InOutBox::Sdi2Mode() const
{
    return ( d_data->m_ui->cbxSdi2Mode->currentText() );
}

/******************************************************************************
 * InOutBox::setSdi2Mode
 *****************************************************************************/
void InOutBox::setSdi2Mode( const QString mode )
{
    int index = d_data->m_ui->cbxSdi2Mode->findText( mode );
    if ( index != -1 )
    {
        d_data->m_ui->cbxSdi2Mode->blockSignals( true );
        d_data->m_ui->cbxSdi2Mode->setCurrentIndex( index );
        d_data->m_ui->cbxSdi2Mode->blockSignals( false );

        emit ChainSdi2ModeChanged( d_data->m_ui->cbxSdi2Mode->itemData( index ).toInt() );
    }
}

/******************************************************************************
 * InOutBox::Sdi1Downscaler
 *****************************************************************************/
QString InOutBox::Sdi1Downscaler() const
{
    return ( d_data->m_ui->cbxSdi1Downscaler->currentText() );
}

/******************************************************************************
 * InOutBox::setSdi1Downscaler
 *****************************************************************************/
void InOutBox::setSdi1Downscaler( const QString mode )
{
    int index = d_data->m_ui->cbxSdi1Downscaler->findText( mode );
    if ( index != -1 )
    {
        d_data->m_ui->cbxSdi1Downscaler->blockSignals( true );
        d_data->m_ui->cbxSdi1Downscaler->setCurrentIndex( index );
        d_data->m_ui->cbxSdi1Downscaler->blockSignals( false );

        EmitDownscaleChanged( 1, index );
    }
}

/******************************************************************************
 * InOutBox::Sdi2Downscaler
 *****************************************************************************/
QString InOutBox::Sdi2Downscaler() const
{
    return ( d_data->m_ui->cbxSdi2Downscaler->currentText() );
}

/******************************************************************************
 * InOutBox::setSdi2Downscaler
 *****************************************************************************/
void InOutBox::setSdi2Downscaler( const QString mode )
{
    int index = d_data->m_ui->cbxSdi2Downscaler->findText( mode );
    if ( index != -1 )
    {
        d_data->m_ui->cbxSdi2Downscaler->blockSignals( true );
        d_data->m_ui->cbxSdi2Downscaler->setCurrentIndex( index );
        d_data->m_ui->cbxSdi2Downscaler->blockSignals( false );

        EmitDownscaleChanged( 2, index );
    }
}

/******************************************************************************
 * InOutBox::FlipMode
 *****************************************************************************/
QString InOutBox::FlipMode() const
{
    return ( d_data->m_ui->cbxFlipMode->currentText() );
}

/******************************************************************************
 * InOutBox::setFlipMode
 *****************************************************************************/
void InOutBox::setFlipMode( const QString mode )
{
    int index = d_data->m_ui->cbxFlipMode->findText( mode );
    if ( index != -1 )
    {
        d_data->m_ui->cbxFlipMode->blockSignals( true );
        d_data->m_ui->cbxFlipMode->setCurrentIndex( index );
        d_data->m_ui->cbxFlipMode->blockSignals( false );

        emit ChainFlipModeChanged( d_data->m_ui->cbxFlipMode->itemData( index ).toInt() );
    }
}

/******************************************************************************
 * InOutBox::LogMode
 *****************************************************************************/
QString InOutBox::LogMode() const
{
    return ( d_data->m_ui->cbxLogMode->currentText() );
}

/******************************************************************************
 * InOutBox::setLogMode
 *****************************************************************************/
void InOutBox::setLogMode( const QString mode )
{
    int index = d_data->m_ui->cbxLogMode->findText( mode );
    if ( index != -1 )
    {
        d_data->m_ui->cbxLogMode->blockSignals( true );
        d_data->m_ui->cbxLogMode->setCurrentIndex( index );
        d_data->m_ui->cbxLogMode->blockSignals( false );

        emit LogModeChanged( d_data->m_ui->cbxLogMode->itemData( index ).toInt() );
    }
}

/******************************************************************************
 * InOutBox::PQMaxBrightness
 *****************************************************************************/
int InOutBox::PQMaxBrightness() const
{
    return ( d_data->m_ui->sbxPQMaxBrightness->value() );
}

/******************************************************************************
 * InOutBox::setPQMaxBrightness
 *****************************************************************************/
void InOutBox::setPQMaxBrightness( const int value )
{
    d_data->m_ui->sbxPQMaxBrightness->blockSignals( true );
    d_data->m_ui->sbxPQMaxBrightness->setValue( value );
    d_data->m_ui->sbxPQMaxBrightness->blockSignals( false );

    emit PQMaxBrightnessChanged( value );
}

/******************************************************************************
 * InOutBox::ColorSpace
 *****************************************************************************/
QString InOutBox::ColorSpace() const
{
    return ( d_data->m_ui->cbxLogMode->currentText() );
}

/******************************************************************************
 * InOutBox::setColorSpace
 *****************************************************************************/
void InOutBox::setColorSpace( const QString mode )
{
    int index = d_data->m_ui->cbxColorSpace->findText( mode );
    if ( index != -1 )
    {
        d_data->m_ui->cbxColorSpace->blockSignals( true );
        d_data->m_ui->cbxColorSpace->setCurrentIndex( index );
        d_data->m_ui->cbxColorSpace->blockSignals( false );

        emit ColorSpaceChanged( d_data->m_ui->cbxColorSpace->itemData( index ).toInt() );
    }
}

/******************************************************************************
 * InOutBox::TestPattern
 *****************************************************************************/
bool InOutBox::TestPattern() const
{
    return ( d_data->m_ui->cbxTestPattern->isChecked() );
}

/******************************************************************************
 * InOutBox::setTestPattern
 *****************************************************************************/
void InOutBox::setTestPattern( const bool value )
{
    d_data->m_ui->cbxTestPattern->blockSignals( true );
    d_data->m_ui->cbxTestPattern->setChecked( value );
    d_data->m_ui->cbxTestPattern->blockSignals( false );

    emit OsdTestPatternChanged( value ? 1 : 0 );
}

/******************************************************************************
 * InOutBox::AudioEnable
 *****************************************************************************/
bool InOutBox::AudioEnable() const
{
    return ( d_data->m_ui->cbxAudioEnable->isChecked() );
}

/******************************************************************************
 * InOutBox::setAudioEnable
 *****************************************************************************/
void InOutBox::setAudioEnable( const bool value )
{
    d_data->m_ui->cbxAudioEnable->blockSignals( true );
    d_data->m_ui->cbxAudioEnable->setChecked( value );
    d_data->m_ui->cbxAudioEnable->blockSignals( false );

    emit ChainAudioEnableChanged( value ? 1 : 0 );
}

/******************************************************************************
 * InOutBox::AudioGain
 *****************************************************************************/
double InOutBox::AudioGain() const
{
    return ( d_data->m_ui->sbxAudioGain->value() );
}

/******************************************************************************
 * InOutBox::setAudioGain
 *****************************************************************************/
void InOutBox::setAudioGain( const double gain )
{
    d_data->m_ui->sbxAudioGain->blockSignals( true );
    d_data->m_ui->sbxAudioGain->setValue( gain );
    d_data->m_ui->sbxAudioGain->blockSignals( false );

    emit ChainAudioGainChanged( gain );
}

/******************************************************************************
 * InOutBox::GenLockMode
 *****************************************************************************/
QString InOutBox::GenLockMode() const
{
    return ( d_data->m_ui->cbxGenLockMode->currentText() );
}

/******************************************************************************
 * InOutBox::setGenLockMode
 *****************************************************************************/
void InOutBox::setGenLockMode( const QString mode )
{
    int index = d_data->m_ui->cbxGenLockMode->findText( mode );
    if ( index != -1 )
    {
        d_data->m_ui->cbxGenLockMode->blockSignals( true );
        d_data->m_ui->cbxGenLockMode->setCurrentIndex( index );
        d_data->m_ui->cbxGenLockMode->blockSignals( false );

        emit ChainGenlockModeChanged( d_data->m_ui->cbxGenLockMode->itemData( index ).toInt() );
    }
}

/******************************************************************************
 * InOutBox::GenLockCrosslockEnable
 *****************************************************************************/
QString InOutBox::GenLockCrosslockEnable() const
{
    return ( d_data->m_ui->cbxGenlockCrosslockEnable->currentText() );
}

/******************************************************************************
 * InOutBox::GenLockCrosslockVmode
 *****************************************************************************/
QString InOutBox::GenLockCrosslockVmode() const
{
    return ( d_data->m_ui->cbxGenlockCrosslockVmode->currentText() );
}

/******************************************************************************
 * InOutBox::setGenLockCrosslock
 *****************************************************************************/
void InOutBox::setGenLockCrosslock( const QString enable, const QString vmode )
{
    int index = d_data->m_ui->cbxGenlockCrosslockEnable->findText( enable );
    if ( index != -1 )
    {
        d_data->m_ui->cbxGenlockCrosslockEnable->blockSignals( true );
        d_data->m_ui->cbxGenlockCrosslockEnable->setCurrentIndex( index );
        d_data->m_ui->cbxGenlockCrosslockEnable->blockSignals( false );
    }

    index = d_data->m_ui->cbxGenlockCrosslockVmode->findText( vmode );
    if ( index != -1 )
    {
        d_data->m_ui->cbxGenlockCrosslockVmode->blockSignals( true );
        d_data->m_ui->cbxGenlockCrosslockVmode->setCurrentIndex( index );
        d_data->m_ui->cbxGenlockCrosslockVmode->blockSignals( false );
    }

    emit ChainGenlockCrosslockChanged( d_data->m_ui->cbxGenlockCrosslockEnable->currentData().toInt(),
                                       d_data->m_ui->cbxGenlockCrosslockVmode->currentData().toInt() );
}

/******************************************************************************
 * InOutBox::GenLockOffsetVertical
 *****************************************************************************/
int InOutBox::GenLockOffsetVertical() const
{
    return ( d_data->m_ui->sbxGenLockOffsetVertical->value() );
}

/******************************************************************************
 * InOutBox::setGenLockOffsetVertical
 *****************************************************************************/
void InOutBox::setGenLockOffsetVertical( const int value )
{
    d_data->m_ui->sbxGenLockOffsetVertical->blockSignals( true );
    d_data->m_ui->sbxGenLockOffsetVertical->setValue( value );
    d_data->m_ui->sbxGenLockOffsetVertical->blockSignals( false );

    emit ChainGenlockOffsetChanged( value, GenLockOffsetHorizontal() );
}

/******************************************************************************
 * InOutBox::GenLockOffsetHorizontal
 *****************************************************************************/
int InOutBox::GenLockOffsetHorizontal() const
{
    return ( d_data->m_ui->sbxGenlockOffsetHorizontal->value() );
}

/******************************************************************************
 * InOutBox::setGenLockOffsetVertical
 *****************************************************************************/
void InOutBox::setGenLockOffsetHorizontal( const int value )
{
    d_data->m_ui->sbxGenlockOffsetHorizontal->blockSignals( true );
    d_data->m_ui->sbxGenlockOffsetHorizontal->setValue( value );
    d_data->m_ui->sbxGenlockOffsetHorizontal->blockSignals( false );

    emit ChainGenlockOffsetChanged( GenLockOffsetVertical(), value );
}

/******************************************************************************
 * InOutBox::GenLockTermination
 *****************************************************************************/
bool InOutBox::GenLockTermination() const
{
    return ( d_data->m_ui->cbxGenLockTermination->isChecked() );
}

/******************************************************************************
 * InOutBox::setGenLockTermination
 *****************************************************************************/
void InOutBox::setGenLockTermination( const bool value )
{
    d_data->m_ui->cbxGenLockTermination->blockSignals( true );
    d_data->m_ui->cbxGenLockTermination->setChecked( value );
    d_data->m_ui->cbxGenLockTermination->blockSignals( false );

    emit ChainGenlockTerminationChanged( value ? 1 : 0 );
}

void setGenLockLOLFilter( const int value );
/******************************************************************************
 * InOutBox::setGenLockLOLFilter
 *****************************************************************************/
void InOutBox::setGenLockLOLFilter( const int value )
{
    d_data->m_ui->sbxGenLockLOLFilter->blockSignals( true );
    d_data->m_ui->sbxGenLockLOLFilter->setValue( value );
    d_data->m_ui->sbxGenLockLOLFilter->blockSignals( false );

    emit ChainGenlockLOLFilterChanged( value ? 1 : 0 );
}

/******************************************************************************
 * InOutBox::prepareMode
 *****************************************************************************/
void InOutBox::prepareMode( const Mode mode )
{
    //bool v = (DctWidgetBox::Normal == mode) ? false : true;
    Q_UNUSED(mode);
    // Not available in IronSDI
    d_data->m_ui->lblBayerPattern->setVisible( false );
    d_data->m_ui->cbxBayerPattern->setVisible( false );
}

/******************************************************************************
 * InOutBox::loadSettings
 *****************************************************************************/
void InOutBox::loadSettings( QSettings & s )
{
    s.beginGroup( INOUT_SETTINGS_SECTION_NAME );
    
    /* Disable genlock before making changes to the settings, otherwise the user might see
     * multiple warnings if no sync signal is available in slave mode and the process takes
     * very long because the device will run into the genlock lock timeout multiple times. */
    emit ChainGenlockModeChanged( GenLockModeDisabled );

    // Note: Set video mode first, otherwise gain and exposure will not be correctly set
    setVideoMode( s.value( INOUT_SETTINGS_VIDEO_MODE ).toString() );
    setSdi2Mode( s.value( INOUT_SETTINGS_SDI2_MODE ).toString() );
    setSdi1Downscaler( s.value( INOUT_SETTINGS_SDI1_DOWNSCALER ).toString() );
    setSdi2Downscaler( s.value( INOUT_SETTINGS_SDI2_DOWNSCALER ).toString() );
    setFlipMode( s.value( INOUT_SETTINGS_FLIP_MODE ).toString() );
    setLogMode( s.value( INOUT_SETTINGS_LOG_MODE ).toString() );
    setPQMaxBrightness( s.value( INOUT_SETTINGS_PQ_MAX_BRIGHTNESS ).toInt() );
    setColorSpace( s.value( INOUT_SETTINGS_COLOR_SPACE ).toString() );
    setTestPattern( s.value( INOUT_SETTINGS_TEST_PATTERN ).toBool() );
    setAudioEnable( s.value( INOUT_SETTINGS_AUDIO_ENABLE ).toBool() );
    setAudioGain( s.value( INOUT_SETTINGS_AUDIO_GAIN ).toDouble() );

    setBayerPattern( s.value( INOUT_SETTINGS_BAYER_PATTERN ).toInt() );
    setCameraIso( s.value( INOUT_SETTINGS_CAMERA_ISO ).toInt() );
    setCameraExposure( s.value( INOUT_SETTINGS_CAMERA_EXPOSURE ).toInt() );

    setAecSetPoint( s.value( INOUT_SETTINGS_AEC_SETPOINT ).toInt() );
    setAecMaxIso( s.value( INOUT_SETTINGS_AEC_MAX_ISO ).toInt() );
    setAecControlSpeed( s.value( INOUT_SETTINGS_AEC_SPEED ).toInt() );
    setAecFlickerFrequency( s.value( INOUT_SETTINGS_AEC_FLICKER ).toInt() );
    setAecEnable( s.value( INOUT_SETTINGS_AEC_ENABLE ).toBool() );
    setAecCustomWeights( s.value( INOUT_SETTINGS_AEC_USE_CUSTOM_WEIGHTS ).toBool() );
    // NOTE: call qRegisterMetaTypeStreamOperators<QVector<int> >("QVector<int>"); in main.cpp
    d_data->m_weightDialog->setAecWeights( s.value( INOUT_SETTINGS_AEC_WEIGHTS ).value<QVector<int> >() );

    lsc_setup_t lscSetup;
    lscSetup.enable = s.value( INOUT_SETTINGS_LSC_ENABLE ).toBool();
    lscSetup.k = s.value( INOUT_SETTINGS_LSC_K ).toUInt();
    lscSetup.offset = s.value( INOUT_SETTINGS_LSC_OFFSET ).toUInt();
    lscSetup.slope = s.value( INOUT_SETTINGS_LSC_SLOPE ).toUInt();

    setLsc( lscSetup );

    setGenLockOffsetVertical( s.value( INOUT_SETTINGS_GENLOCK_OFFSET_VERTICAL ).toInt() );
    setGenLockOffsetHorizontal( s.value( INOUT_SETTINGS_GENLOCK_OFFSET_HORIZONTAL ).toInt() );
    setGenLockTermination( s.value( INOUT_SETTINGS_GENLOCK_TERMINATION ).toBool() );
    setGenLockLOLFilter(s.value(INOUT_SETTINGS_GENLOCK_LOL_FILTER).toInt());
    setGenLockCrosslock( s.value( INOUT_SETTINGS_GENLOCK_CROSSLOCK_ENABLE ).toString(),
                         s.value( INOUT_SETTINGS_GENLOCK_CROSSLOCK_VMODE ).toString() );
    setGenLockMode( s.value( INOUT_SETTINGS_GENLOCK_MODE ).toString() );

    s.endGroup();
}

/******************************************************************************
 * InOutBox::saveSettings
 *****************************************************************************/
void InOutBox::saveSettings( QSettings & s )
{
    s.beginGroup( INOUT_SETTINGS_SECTION_NAME );

    s.setValue( INOUT_SETTINGS_BAYER_PATTERN                , BayerPattern() );
    s.setValue( INOUT_SETTINGS_CAMERA_ISO                   , CameraIso() );
    s.setValue( INOUT_SETTINGS_CAMERA_EXPOSURE              , CameraExposure() );
    
    s.setValue( INOUT_SETTINGS_AEC_ENABLE                   , AecEnable() );
    s.setValue( INOUT_SETTINGS_AEC_USE_CUSTOM_WEIGHTS       , AecCustomWeights() );
    // NOTE: call qRegisterMetaTypeStreamOperators<QVector<int> >("QVector<int>"); in main.cpp
    s.setValue( INOUT_SETTINGS_AEC_WEIGHTS                  , QVariant::fromValue<QVector<int>>(d_data->m_weightDialog->getAecWeights()) );
    s.setValue( INOUT_SETTINGS_AEC_SETPOINT                 , AecSetPoint() );
    s.setValue( INOUT_SETTINGS_AEC_MAX_ISO                  , AecMaxIso() );
    s.setValue( INOUT_SETTINGS_AEC_SPEED                    , AecControlSpeed() );
    s.setValue( INOUT_SETTINGS_AEC_FLICKER                  , AecFlickerFrequency() );

    s.setValue ( INOUT_SETTINGS_LSC_ENABLE                  , LscEnable() );
    s.setValue ( INOUT_SETTINGS_LSC_K                       , LscK() );
    s.setValue ( INOUT_SETTINGS_LSC_OFFSET                  , LscOffset() );
    s.setValue ( INOUT_SETTINGS_LSC_SLOPE                   , LscSlope() );

    s.setValue( INOUT_SETTINGS_VIDEO_MODE                   , VideoMode() );
    s.setValue( INOUT_SETTINGS_SDI2_MODE                    , Sdi2Mode() );
    s.setValue( INOUT_SETTINGS_SDI1_DOWNSCALER              , Sdi1Downscaler() );
    s.setValue( INOUT_SETTINGS_SDI2_DOWNSCALER              , Sdi2Downscaler() );
    s.setValue( INOUT_SETTINGS_FLIP_MODE                    , FlipMode() );
    s.setValue( INOUT_SETTINGS_LOG_MODE                     , LogMode() );
    s.setValue( INOUT_SETTINGS_PQ_MAX_BRIGHTNESS            , PQMaxBrightness() );
    s.setValue( INOUT_SETTINGS_COLOR_SPACE                  , ColorSpace() );
    s.setValue( INOUT_SETTINGS_TEST_PATTERN                 , TestPattern() );
    s.setValue( INOUT_SETTINGS_AUDIO_ENABLE                 , AudioEnable() );
    s.setValue( INOUT_SETTINGS_AUDIO_GAIN                   , AudioGain() );

    s.setValue( INOUT_SETTINGS_GENLOCK_MODE                 , GenLockMode() );
    s.setValue( INOUT_SETTINGS_GENLOCK_CROSSLOCK_ENABLE     , GenLockCrosslockEnable() );
    s.setValue( INOUT_SETTINGS_GENLOCK_CROSSLOCK_VMODE      , GenLockCrosslockVmode() );
    s.setValue( INOUT_SETTINGS_GENLOCK_OFFSET_VERTICAL      , GenLockOffsetVertical() );
    s.setValue( INOUT_SETTINGS_GENLOCK_OFFSET_HORIZONTAL    , GenLockOffsetHorizontal() );
    s.setValue( INOUT_SETTINGS_GENLOCK_TERMINATION          , GenLockTermination() );

    s.endGroup();
}

/******************************************************************************
 * InOutBox::applySettings
 *****************************************************************************/
void InOutBox::applySettings( void )
{
    /* Disable genlock before making changes to the settings, otherwise the user might see
     * multiple warnings if no sync signal is available in slave mode and the process takes
     * very long because the device will run into the genlock lock timeout multiple times. */
    emit ChainGenlockModeChanged( GenLockModeDisabled );

    // Note: Set video mode first, otherwise gain and exposure will not be correctly set
    emit ChainVideoModeChanged( d_data->m_ui->cbxVideoMode->currentData().toInt() );
    emit ChainSdi2ModeChanged( d_data->m_ui->cbxSdi2Mode->currentData().toInt() );
    EmitDownscaleChanged( 1,  d_data->m_ui->cbxSdi1Downscaler->currentData().toInt() );
    EmitDownscaleChanged( 2,  d_data->m_ui->cbxSdi2Downscaler->currentData().toInt() );
    emit ChainFlipModeChanged( d_data->m_ui->cbxFlipMode->currentData().toInt() );
    emit LogModeChanged( d_data->m_ui->cbxLogMode->currentData().toInt() );
    emit ColorSpaceChanged( d_data->m_ui->cbxColorSpace->currentData().toInt() );
    emit OsdTestPatternChanged( TestPattern() ? 1 : 0 );
    emit ChainAudioEnableChanged( AudioEnable() );
    emit ChainAudioGainChanged( AudioGain() );

    emit BayerPatternChanged( BayerPattern() );
    emit CameraGainChanged( gainToIso(CameraIso()) );
    emit CameraExposureChanged( CameraExposure() );

    emit AecSetupChanged( createAecVector() );
    d_data->m_weightDialog->setAecWeights( d_data->m_weightDialog->getAecWeights() );

    emit ChainGenlockOffsetChanged( GenLockOffsetVertical(), GenLockOffsetHorizontal() );
    emit ChainGenlockTerminationChanged( GenLockTermination() );
    emit ChainGenlockCrosslockChanged( d_data->m_ui->cbxGenlockCrosslockEnable->currentData().toInt(),
                                       d_data->m_ui->cbxGenlockCrosslockVmode->currentData().toInt() );
    emit ChainGenlockModeChanged( d_data->m_ui->cbxGenLockMode->currentData().toInt() );
    emit ChainGenlockLOLFilterChanged(d_data->m_ui->sbxGenLockLOLFilter->value());
}

/******************************************************************************
 * InOutBox::addBayerPattern
 *****************************************************************************/
void InOutBox::addBayerPattern( QString name, int id )
{
    d_data->m_ui->cbxBayerPattern->blockSignals( true );
    d_data->m_ui->cbxBayerPattern->addItem( name, id );
    d_data->m_ui->cbxBayerPattern->blockSignals( false );
}

/******************************************************************************
 * InOutBox::addVideoMode
 *****************************************************************************/
void InOutBox::addVideoMode( QString name, int id )
{
    d_data->m_ui->cbxVideoMode->blockSignals( true );
    d_data->m_ui->cbxVideoMode->addItem( name, id );
    d_data->m_ui->cbxVideoMode->blockSignals( false );
}

/******************************************************************************
 * InOutBox::clearAllVideoModes
 *****************************************************************************/
void InOutBox::clearAllVideoModes()
{
    d_data->m_ui->cbxVideoMode->blockSignals( true );
    d_data->m_ui->cbxVideoMode->clear();
    d_data->m_ui->cbxVideoMode->blockSignals( false );
}

/******************************************************************************
 * InOutBox::addGenlockCrosslockVideoMode
 *****************************************************************************/
void InOutBox::addGenlockCrosslockVideoMode( QString name, int id )
{
    d_data->m_ui->cbxGenlockCrosslockVmode->blockSignals( true );
    d_data->m_ui->cbxGenlockCrosslockVmode->addItem( name, id );
    d_data->m_ui->cbxGenlockCrosslockVmode->blockSignals( false );
}

/******************************************************************************
 * InOutBox::clearAllGenlockCrosslockVideoModes
 *****************************************************************************/
void InOutBox::clearAllGenlockCrosslockVideoModes()
{
    d_data->m_ui->cbxGenlockCrosslockVmode->blockSignals( true );
    d_data->m_ui->cbxGenlockCrosslockVmode->clear();
    d_data->m_ui->cbxGenlockCrosslockVmode->blockSignals( false );
}

/******************************************************************************
 * InOutBox::addGenlockMode
 *****************************************************************************/
void InOutBox::addGenlockMode( QString name, int id )
{
    d_data->m_ui->cbxGenLockMode->blockSignals( true );
    d_data->m_ui->cbxGenLockMode->addItem( name, id );
    d_data->m_ui->cbxGenLockMode->blockSignals( false );
}

/******************************************************************************
 * InOutBox::addGenlockCrosslockEnable
 *****************************************************************************/
void InOutBox::addGenlockCrosslockEnable( QString name, int id )
{
    d_data->m_ui->cbxGenlockCrosslockEnable->blockSignals( true );
    d_data->m_ui->cbxGenlockCrosslockEnable->addItem( name, id );
    d_data->m_ui->cbxGenlockCrosslockEnable->blockSignals( false );
}

/******************************************************************************
 * InOutBox::addSdi2Mode
 *****************************************************************************/
void InOutBox::addSdi2Mode( QString name, int id )
{
    d_data->m_ui->cbxSdi2Mode->blockSignals( true );
    d_data->m_ui->cbxSdi2Mode->addItem( name, id );
    d_data->m_ui->cbxSdi2Mode->blockSignals( false );
}

/******************************************************************************
 * InOutBox::addDownscaleMode
 *****************************************************************************/
void InOutBox::addDownscaleMode( QString name, int id )
{
    d_data->m_ui->cbxSdi1Downscaler->blockSignals( true );
    d_data->m_ui->cbxSdi1Downscaler->addItem( name, id );
    d_data->m_ui->cbxSdi1Downscaler->blockSignals( false );

    d_data->m_ui->cbxSdi2Downscaler->blockSignals( true );
    d_data->m_ui->cbxSdi2Downscaler->addItem( name, id );
    d_data->m_ui->cbxSdi2Downscaler->blockSignals( false );
}

/******************************************************************************
 * InOutBox::addFlipMode
 *****************************************************************************/
void InOutBox::addFlipMode( QString name, int id )
{
    d_data->m_ui->cbxFlipMode->blockSignals( true );
    d_data->m_ui->cbxFlipMode->addItem( name, id );
    d_data->m_ui->cbxFlipMode->blockSignals( false );
}

/******************************************************************************
 * InOutBox::addLogMode
 *****************************************************************************/
void InOutBox::addLogMode( QString name, int id )
{
    d_data->m_ui->cbxLogMode->blockSignals( true );
    d_data->m_ui->cbxLogMode->addItem( name, id );
    d_data->m_ui->cbxLogMode->blockSignals( false );
}

/******************************************************************************
 * InOutBox::addColorSpace
 *****************************************************************************/
void InOutBox::addColorSpace( QString name, int id )
{
    d_data->m_ui->cbxColorSpace->blockSignals( true );
    d_data->m_ui->cbxColorSpace->addItem( name, id );
    d_data->m_ui->cbxColorSpace->blockSignals( false );
}

/******************************************************************************
 * InOutBox::setCameraSettingsVisible
 *****************************************************************************/
void InOutBox::setCameraSettingsVisible(const bool value)
{
    d_data->m_ui->gbxCameraSettings->setVisible(value);
}

/******************************************************************************
 * InOutBox::setAutoExposureSettingsVisible
 *****************************************************************************/
void InOutBox::setAutoExposureSettingsVisible(const bool value)
{
    d_data->m_ui->gbxAutoExposureSettings->setVisible(value);
}

/******************************************************************************
 * InOutBox::setLensChadingCorrectionSettingsVisible
 *****************************************************************************/
void InOutBox::setLensChadingCorrectionSettingsVisible(const bool value)
{
    d_data->m_ui->gbxLensShadingCorrectionSettings->setVisible(value);
}


/******************************************************************************
 * InOutBox::setApartureVisible
 *****************************************************************************/
void InOutBox::setApartureVisible(const bool value)
{
    d_data->m_ui->lblAperture->setVisible(value);
    d_data->m_ui->sbxAperture->setVisible(value);
    d_data->m_ui->sldAperture->setVisible(value);
}

/******************************************************************************
 * InOutBox::setGenLockVisible
 *****************************************************************************/
void InOutBox::setGenLockVisible(const bool value)
{
    d_data->m_ui->gbxGenLockSettings->setVisible(value);
}

/******************************************************************************
 * InOutBox::setTimeCodeVisible
 *****************************************************************************/
void InOutBox::setTimeCodeVisible(const bool groupbox_visible, const bool hold_visible)
{
    d_data->m_ui->gbxTimeCodeSettings->setVisible(groupbox_visible);
    d_data->m_ui->btnHoldTimecode->setVisible(hold_visible);
}

/******************************************************************************
 * InOutBox::setSdi2ModeVisible
 *****************************************************************************/
void InOutBox::setSdi2ModeVisible(const bool value)
{
    d_data->m_ui->lblSdi2Mode->setVisible(value);
    d_data->m_ui->cbxSdi2Mode->setVisible(value);
}

/******************************************************************************
 * InOutBox::setDownscaleModeVisible
 *****************************************************************************/
void InOutBox::setDownscaleModeVisible(const bool value)
{
    d_data->m_ui->lblSdi1Downscaler->setVisible(value);
    d_data->m_ui->cbxSdi1Downscaler->setVisible(value);

    //d_data->m_ui->lblSdi2Downscaler->setVisible(value);
    //d_data->m_ui->cbxSdi2Downscaler->setVisible(value);
    // There is only one SDI out
    d_data->m_ui->lblSdi2Downscaler->setVisible(false);
    d_data->m_ui->cbxSdi2Downscaler->setVisible(false);
}

/******************************************************************************
 * InOutBox::setFlipModeVisible
 *****************************************************************************/
void InOutBox::setFlipModeVisible(const bool vertical, const bool horizontal)
{
    // clear flip-mode combo box
    d_data->m_ui->cbxFlipMode->blockSignals( true );
    d_data->m_ui->cbxFlipMode->clear();
    d_data->m_ui->cbxFlipMode->blockSignals( false );

    // if at least one flip mode is available
    if ( vertical || horizontal )
    {
        // add "off" mode to combo box
        addFlipMode( GetFlipModeName(FlipModeOff), FlipModeOff );

        // if vertical flip is available, add it
        if ( vertical )
        {
            addFlipMode( GetFlipModeName(FlipModeVertical), FlipModeVertical );
        }

        // if vertical flip is available, add it
        if ( horizontal )
        {
            addFlipMode( GetFlipModeName(FlipModeHorizontal), FlipModeHorizontal );
        }

        // if both are available, add rotation mode
        if ( vertical && horizontal )
        {
            addFlipMode( GetFlipModeName(FlipModeRotated), FlipModeRotated );
        }

        // show combo box and label
        d_data->m_ui->lblFlipMode->setVisible(true);
        d_data->m_ui->cbxFlipMode->setVisible(true);
    }
    // else if flip is not available
    else
    {
        // hide combo box and label
        d_data->m_ui->lblFlipMode->setVisible(false);
        d_data->m_ui->cbxFlipMode->setVisible(false);
    }
}

/******************************************************************************
 * InOutBox::setLogModeVisible
 *****************************************************************************/
void InOutBox::setLogModeVisible(const bool value)
{
    d_data->m_ui->lblLogMode->setVisible(value);
    d_data->m_ui->cbxLogMode->setVisible(value);

    /* PQ max brightness is only setable in PQ log mode, so if log mode is
     * not availbe there is no need to show those settings. */
    d_data->m_ui->lblPQMaxBrightness->setVisible(value);
    d_data->m_ui->sbxPQMaxBrightness->setVisible(value);
}

/******************************************************************************
 * InOutBox::setTestPatternVisible
 *****************************************************************************/
void InOutBox::setTestPatternVisible(const bool value)
{
    d_data->m_ui->lblTestPattern->setVisible(value);
    d_data->m_ui->cbxTestPattern->setVisible(value);
}

/******************************************************************************
 * InOutBox::setAudioEnableVisible
 *****************************************************************************/
void InOutBox::setAudioVisible(const bool value)
{
    d_data->m_ui->lblAudio->setVisible(value);
    d_data->m_ui->cbxAudioEnable->setVisible(value);
    d_data->m_ui->sbxAudioGain->setVisible(value);
}


/******************************************************************************
 * InOutBox::onBayerPatternChange
 *****************************************************************************/
void InOutBox::onBayerPatternChange( int value )
{
    int index = d_data->m_ui->cbxBayerPattern->findData( value );
    if ( index != -1 )
    {
        d_data->m_ui->cbxBayerPattern->blockSignals( true );
        d_data->m_ui->cbxBayerPattern->setCurrentIndex( index );
        d_data->m_ui->cbxBayerPattern->blockSignals( false );
    }    
}

/******************************************************************************
 * InOutBox::onCameraInfoChange
 *****************************************************************************/
void InOutBox::onCameraInfoChange( int min_gain, int max_gain, int min_exposure, int max_exposure, int min_iso )
{
    // Set min iso in class variable
    d_data->m_minIso = min_iso;

    /* Setup Sliders and Spin Boxes */
    // Set gain / iso range
    d_data->m_ui->sbxAnalogGain->blockSignals( true );
    d_data->m_ui->sbxAnalogGain->setRange( min_gain / 1000, max_gain / 1000 );
    d_data->m_ui->sbxAnalogGain->blockSignals( false );

    d_data->m_ui->sldAnalogGain->blockSignals( true );
    d_data->m_ui->sldAnalogGain->setRange( min_gain / 1000, max_gain / 1000 );
    d_data->m_ui->sldAnalogGain->blockSignals( false );

    // Set max iso range for auto exposure control
    d_data->m_ui->sbxMaxAnalogGain->blockSignals( true );
    d_data->m_ui->sbxMaxAnalogGain->setRange( min_gain / 1000, max_gain / 1000 );
    d_data->m_ui->sbxMaxAnalogGain->blockSignals( false );

    d_data->m_ui->sldMaxAnalogGain->blockSignals( true );
    d_data->m_ui->sldMaxAnalogGain->setRange( min_gain / 1000, max_gain / 1000 );
    d_data->m_ui->sldMaxAnalogGain->blockSignals( false );

    d_data->m_ui->sbxMaxExposure->blockSignals( true );
    d_data->m_ui->sbxMaxExposure->setRange( min_exposure, max_exposure );
    d_data->m_ui->sbxMaxExposure->blockSignals( false );

    d_data->m_ui->sldMaxExposure->blockSignals( true );
    d_data->m_ui->sldMaxExposure->setRange( min_exposure, max_exposure );
    d_data->m_ui->sldMaxExposure->blockSignals( false );

    // Set exposure range
    d_data->m_ui->sbxExposure->blockSignals( true );
    d_data->m_ui->sbxExposure->setRange( min_exposure, max_exposure );
    d_data->m_ui->sbxExposure->blockSignals( false );

    d_data->m_ui->sldExposure->blockSignals( true );
    d_data->m_ui->sldExposure->setRange( min_exposure, max_exposure );
    d_data->m_ui->sldExposure->blockSignals( false );

    /* Setup the ISO combo box */
    // Clear combo box
    d_data->m_ui->cbxIso->clear();

    // Add the first item
    d_data->m_ui->cbxIso->addItem( QString("Select Gain"), 0);

    // Add iso values
    for ( int i = IsoValueFirst; i < IsoValueMax; i++ )
    {
        // Get the iso value
        int iso = GetIsoValue( static_cast<GainValue>(i) );
        //int gain = isoToGain( iso );
        int gain = iso * 1000;

        // Check if iso value is in valid iso range for this video mode
        if ( gain < min_gain )
        {
            // Gain is below minimum, try next value
            continue;
        }
        else if ( gain > max_gain )
        {
            // Gain is above maximum, abort
            break;
        }
        else
        {
            // Gain is within range, add iso value to combo box
            d_data->m_ui->cbxIso->addItem( "x" + QString::number(iso), iso );
        }
    }

    // Select the first entry in the list (the "Select ISO" entry)
    d_data->m_ui->cbxIso->setCurrentIndex( 0 );

    /* Setup the exposure combo box */
    // Clear combo box
    d_data->m_ui->cbxExposure->clear();

    // Add the first item
    d_data->m_ui->cbxExposure->addItem( QString("Select Shutter"), 0);

    // Add exposure times
    for ( int i = ExposureTimeFirst; i < ExposureTimeMax; i++ )
    {
        // Get the iso value
        int exposure = GetExposureTime( static_cast<ExposureTime>(i) );

        // Check if iso value is in valid iso range for this video mode
        if ( exposure < min_exposure )
        {
            // Exposure is below minimum, try next value
            continue;
        }
        else if ( exposure > max_exposure )
        {
            // Exposure is above maximum, abort
            break;
        }
        else
        {
            // Gain is within range, add to combo box
            d_data->m_ui->cbxExposure->addItem( GetExposureTimeString( static_cast<ExposureTime>(i)), exposure );
        }
    }

    // Select the first entry in the list (the "Select ISO" entry)
    d_data->m_ui->cbxExposure->setCurrentIndex( 0 );
}

/******************************************************************************
 * InOutBox::onCameraGainChange
 *****************************************************************************/
void InOutBox::onCameraGainChange( int value )
{
    d_data->m_ui->sbxAnalogGain->blockSignals( true );
    d_data->m_ui->sbxAnalogGain->setValue( gainToIso(value) );
    d_data->m_ui->sbxAnalogGain->blockSignals( false );

    d_data->m_ui->sldAnalogGain->blockSignals( true );
    d_data->m_ui->sldAnalogGain->setValue( gainToIso(value) );
    d_data->m_ui->sldAnalogGain->blockSignals( false );

    UpdateIsoComboBox( gainToIso(value) );
}

/******************************************************************************
 * InOutBox::onCameraExposureChange
 *****************************************************************************/
void InOutBox::onCameraExposureChange( int value )
{
    d_data->m_ui->sbxExposure->blockSignals( true );
    d_data->m_ui->sbxExposure->setValue( value );
    d_data->m_ui->sbxExposure->blockSignals( false );

    d_data->m_ui->sldExposure->blockSignals( true );
    d_data->m_ui->sldExposure->setValue( value );
    d_data->m_ui->sldExposure->blockSignals( false );

    UpdateExposureComboBox( value );
}

/******************************************************************************
 * InOutBox::onCameraRoiOffsetInfoChange
 *****************************************************************************/
void InOutBox::onCameraRoiOffsetInfoChange( int offset_x_max, int offset_y_max, int offset_x_step, int offset_y_step )
{
    d_data->offset_x_step = offset_x_step;
    d_data->offset_y_step = offset_y_step;

    d_data->m_ui->sbxRoiOffsetX->blockSignals( true );
    d_data->m_ui->sbxRoiOffsetX->setMaximum( offset_x_max);
    d_data->m_ui->sbxRoiOffsetX->setSingleStep( offset_x_step);
    d_data->m_ui->sbxRoiOffsetX->blockSignals( false );

    d_data->m_ui->sldRoiOffsetX->blockSignals( true );
    d_data->m_ui->sldRoiOffsetX->setMaximum( offset_x_max );
    d_data->m_ui->sldRoiOffsetX->blockSignals( false );

    d_data->m_ui->sbxRoiOffsetY->blockSignals( true );
    d_data->m_ui->sbxRoiOffsetY->setMaximum( offset_y_max );
    d_data->m_ui->sbxRoiOffsetY->setSingleStep( offset_y_step );
    d_data->m_ui->sbxRoiOffsetY->blockSignals( false );

    d_data->m_ui->sldRoiOffsetY->blockSignals( true );
    d_data->m_ui->sldRoiOffsetY->setMaximum( offset_y_max );
    d_data->m_ui->sldRoiOffsetY->blockSignals( false );
}

/******************************************************************************
 * InOutBox::onCameraRoiOffsetChange
 *****************************************************************************/
void InOutBox::onCameraRoiOffsetChange( int offset_x, int offset_y )
{
    d_data->roi_offset_x = offset_x;
    d_data->m_ui->sbxRoiOffsetX->blockSignals( true );
    d_data->m_ui->sbxRoiOffsetX->setValue( offset_x );
    d_data->m_ui->sbxRoiOffsetX->blockSignals( false );
    d_data->m_ui->sldRoiOffsetX->blockSignals( true );
    d_data->m_ui->sldRoiOffsetX->setValue( offset_x );
    d_data->m_ui->sldRoiOffsetX->blockSignals( false );

    d_data->roi_offset_y = offset_y;
    d_data->m_ui->sbxRoiOffsetY->blockSignals( true );
    d_data->m_ui->sbxRoiOffsetY->setValue( offset_y );
    d_data->m_ui->sbxRoiOffsetY->blockSignals( false );
    d_data->m_ui->sldRoiOffsetY->blockSignals( true );
    d_data->m_ui->sldRoiOffsetY->setValue( offset_y );
    d_data->m_ui->sldRoiOffsetY->blockSignals( false );
}

/******************************************************************************
 * InOutBox::onChainVideoModeChange
 *****************************************************************************/
void InOutBox::onChainVideoModeChange( int value )
{
    int index = d_data->m_ui->cbxVideoMode->findData( value );
    if ( index != -1 )
    {
        d_data->m_ui->cbxVideoMode->blockSignals( true );
        d_data->m_ui->cbxVideoMode->setCurrentIndex( index );
        show4kGenlockNote( d_data->m_ui->cbxVideoMode->itemData( index ).toInt() );
        d_data->m_ui->cbxVideoMode->blockSignals( false );
    }
}

/******************************************************************************
 * InOutBox::onChainSdi2ModeChange
 *****************************************************************************/
void InOutBox::onChainSdi2ModeChange( int value )
{
    int index = d_data->m_ui->cbxSdi2Mode->findData( value );
    if ( index != -1 )
    {
        d_data->m_ui->cbxSdi2Mode->blockSignals( true );
        d_data->m_ui->cbxSdi2Mode->setCurrentIndex( index );
        d_data->m_ui->cbxSdi2Mode->blockSignals( false );
    }
}

/******************************************************************************
 * InOutBox::onChainDownscaleModeChange
 *****************************************************************************/
void InOutBox::onChainDownscaleModeChange( int sdi_out_idx, bool downscale, bool interlace )
{
    // Get index for combo box (1 = downscale, 2 = downscale and interlace)
    int index = 0;
    if ( downscale )
    {
        index = 1;

        if ( interlace )
        {
            index = 2;
        }
    }

    if ( sdi_out_idx == 1 )
    {
        d_data->m_ui->cbxSdi1Downscaler->blockSignals( true );
        d_data->m_ui->cbxSdi1Downscaler->setCurrentIndex( index );
        d_data->m_ui->cbxSdi1Downscaler->blockSignals( false );
    }
    else if ( sdi_out_idx == 2 )
    {
        d_data->m_ui->cbxSdi2Downscaler->blockSignals( true );
        d_data->m_ui->cbxSdi2Downscaler->setCurrentIndex( index );
        d_data->m_ui->cbxSdi2Downscaler->blockSignals( false );
    }
}

/******************************************************************************
 * InOutBox::onChainFlipModeChange
 *****************************************************************************/
void InOutBox::onChainFlipModeChange( int value )
{
    int index = d_data->m_ui->cbxFlipMode->findData( value );
    if ( index != -1 )
    {
        d_data->m_ui->cbxFlipMode->blockSignals( true );
        d_data->m_ui->cbxFlipMode->setCurrentIndex( index );
        d_data->m_ui->cbxFlipMode->blockSignals( false );
    }
}

/******************************************************************************
 * InOutBox::onLogModeChange
 *****************************************************************************/
void InOutBox::onLogModeChange( int value )
{
    int index = d_data->m_ui->cbxLogMode->findData( value );
    if ( index != -1 )
    {
        d_data->m_ui->cbxLogMode->blockSignals( true );
        d_data->m_ui->cbxLogMode->setCurrentIndex( index );
        d_data->m_ui->cbxLogMode->blockSignals( false );

        // Show max PQ brightness settings if log mode is PQ
        if ( index == LogModePQ )
        {
            d_data->m_ui->lblPQMaxBrightness->setEnabled( true );
            d_data->m_ui->sbxPQMaxBrightness->setEnabled( true );
        }
        else
        {
            d_data->m_ui->lblPQMaxBrightness->setEnabled( false );
            d_data->m_ui->sbxPQMaxBrightness->setEnabled( false );
        }
    }
}

/******************************************************************************
 * InOutBox::onPQMaxBrightnessChange
 *****************************************************************************/
void InOutBox::onPQMaxBrightnessChange( int percent )
{
    d_data->m_ui->sbxPQMaxBrightness->blockSignals( true );
    d_data->m_ui->sbxPQMaxBrightness->setValue( percent );
    d_data->m_ui->sbxPQMaxBrightness->blockSignals( false );
}

/******************************************************************************
 * InOutBox::onColorSpaceChange
 *****************************************************************************/
void InOutBox::onColorSpaceChange( int value )
{
    int index = d_data->m_ui->cbxColorSpace->findData( value );
    if ( index != -1 )
    {
        d_data->m_ui->cbxColorSpace->blockSignals( true );
        d_data->m_ui->cbxColorSpace->setCurrentIndex( index );
        d_data->m_ui->cbxColorSpace->blockSignals( false );
    }
}

/******************************************************************************
 * InOutBox::onOsdTestPatternChange
 *****************************************************************************/
void InOutBox::onOsdTestPatternChange( int value )
{
    // set value of checkbox
    d_data->m_ui->cbxTestPattern->blockSignals( true );
    d_data->m_ui->cbxTestPattern->setCheckState( value ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxTestPattern->blockSignals( false );
}

/******************************************************************************
 * InOutBox::onChainAudioEnableChange
 *****************************************************************************/
void InOutBox::onChainAudioEnableChange( bool enable )
{
    // set value of checkbox
    d_data->m_ui->cbxAudioEnable->blockSignals( true );
    d_data->m_ui->cbxAudioEnable->setCheckState( enable ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxAudioEnable->blockSignals( false );

    // Disable audio gain spin box if audio is disabled
    d_data->m_ui->sbxAudioGain->setEnabled( enable );
}

/******************************************************************************
 * InOutBox::onChainAudioGainChange
 *****************************************************************************/
void InOutBox::onChainAudioGainChange( double gain )
{
    // set value of spin box
    d_data->m_ui->sbxAudioGain->blockSignals( true );
    d_data->m_ui->sbxAudioGain->setValue( gain );
    d_data->m_ui->sbxAudioGain->blockSignals( false );
}

/******************************************************************************
 * InOutBox::onChainGenlockModeChange
 *****************************************************************************/
void InOutBox::onChainGenlockModeChange( int value )
{
    int index = d_data->m_ui->cbxGenLockMode->findData( value );
    if ( index != -1 )
    {
        d_data->m_ui->cbxGenLockMode->blockSignals( true );
        d_data->m_ui->cbxGenLockMode->setCurrentIndex( index );
        d_data->m_ui->cbxGenLockMode->blockSignals( false );

        updateEnableOfGenlockSettings( value, d_data->m_ui->cbxGenlockCrosslockEnable->currentIndex() );
    }

}

/******************************************************************************
 * InOutBox::onChainGenlockStatusChange
 *****************************************************************************/
void InOutBox::onChainGenlockStatusChange( int index )
{
    d_data->m_ui->letGenLockStatus->blockSignals( true );
    d_data->m_ui->letGenLockStatus->setText(GetGenlockStatusName(static_cast<enum GenLockStatus>(index)));
    d_data->m_ui->letGenLockStatus->blockSignals( false );
}

/******************************************************************************
 * InOutBox::onChainGenlockCrosslockChange
 *****************************************************************************/
void InOutBox::onChainGenlockCrosslockChange( int enable, int vmode )
{
    int index = d_data->m_ui->cbxGenlockCrosslockEnable->findData( enable );
    if ( index != -1 )
    {
        d_data->m_ui->cbxGenlockCrosslockEnable->blockSignals( true );
        d_data->m_ui->cbxGenlockCrosslockEnable->setCurrentIndex( index );
        d_data->m_ui->cbxGenlockCrosslockEnable->blockSignals( false );

        updateEnableOfGenlockSettings( d_data->m_ui->cbxGenLockMode->currentIndex(), index);
    }

    index = d_data->m_ui->cbxGenlockCrosslockVmode->findData( vmode );
    if ( index != -1 )
    {
        d_data->m_ui->cbxGenlockCrosslockVmode->blockSignals( true );
        d_data->m_ui->cbxGenlockCrosslockVmode->setCurrentIndex( index );
        d_data->m_ui->cbxGenlockCrosslockVmode->blockSignals( false );
    }

}

/******************************************************************************
 * InOutBox::onChainGenlockOffsetChange
 *****************************************************************************/
void InOutBox::onChainGenlockOffsetChange( int vertical, int horizontal )
{
    d_data->m_ui->sbxGenLockOffsetVertical->blockSignals( true );
    d_data->m_ui->sbxGenLockOffsetVertical->setValue( vertical );
    d_data->m_ui->sbxGenLockOffsetVertical->blockSignals( false );
    
    d_data->m_ui->sbxGenlockOffsetHorizontal->blockSignals( true );
    d_data->m_ui->sbxGenlockOffsetHorizontal->setValue( horizontal );
    d_data->m_ui->sbxGenlockOffsetHorizontal->blockSignals( false );
}

/******************************************************************************
 * InOutBox::onChainGenlockOffsetMaxChange
 *****************************************************************************/
void InOutBox::onChainGenlockOffsetMaxChange( int verticalMax, int horizontalMax )
{
    d_data->m_ui->sbxGenLockOffsetVertical->blockSignals( true );
    d_data->m_ui->sbxGenLockOffsetVertical->setMaximum( verticalMax );
    d_data->m_ui->sbxGenLockOffsetVertical->setMinimum( - verticalMax );
    d_data->m_ui->sbxGenLockOffsetVertical->blockSignals( false );

    d_data->m_ui->sbxGenlockOffsetHorizontal->blockSignals( true );
    d_data->m_ui->sbxGenlockOffsetHorizontal->setMaximum( horizontalMax );
    d_data->m_ui->sbxGenlockOffsetHorizontal->setMinimum( - horizontalMax );
    d_data->m_ui->sbxGenlockOffsetHorizontal->blockSignals( false );
}

/******************************************************************************
 * InOutBox::onChainGenlockTerminationChange
 *****************************************************************************/
void InOutBox::onChainGenlockTerminationChange( int value )
{
    // set value of checkbox
    d_data->m_ui->cbxGenLockTermination->blockSignals( true );
    d_data->m_ui->cbxGenLockTermination->setCheckState( value ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxGenLockTermination->blockSignals( false );
}

/******************************************************************************
 * InOutBox::onChainGenlockLOLFilterChange
 *****************************************************************************/
void InOutBox::onChainGenlockLOLFilterChange( int value )
{
    d_data->m_ui->sbxGenLockLOLFilter->blockSignals( true );
    d_data->m_ui->sbxGenLockLOLFilter->setValue( value );
    d_data->m_ui->sbxGenLockLOLFilter->blockSignals( false );
}

/******************************************************************************
 * InOutBox::onCbxBayerPatternChange
 *****************************************************************************/
void InOutBox::onCbxBayerPatternChange( int index )
{
    setWaitCursor();
    emit BayerPatternChanged( d_data->m_ui->cbxBayerPattern->itemData( index ).toInt() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSldIsoChange
 *****************************************************************************/
void InOutBox::onSldIsoChange( int value )
{
    // Set the spin box to new value
    d_data->m_ui->sbxAnalogGain->blockSignals( true );
    d_data->m_ui->sbxAnalogGain->setValue( value );
    d_data->m_ui->sbxAnalogGain->blockSignals( false );

    // Note: the combo box is set with the released() event of the slider

    if ( (d_data->m_ui->sldAnalogGain->isSliderDown()  ) &&
         (d_data->m_cntEvents++ > d_data->m_maxEvents) )
    {
        d_data->m_cntEvents = 0;
        
        setWaitCursor();
        emit CameraGainChanged( multiplyBy1000(value) );
        setNormalCursor();
    }
}

/******************************************************************************
 * InOutBox::onSldIsoReleased
 *****************************************************************************/
void InOutBox::onSldIsoReleased()
{
    d_data->m_cntEvents = 0;

    /* Set the combo box to new value, blocking signals is not needed, as we only use
     * the activated() event of the combo box which is only triggerd by user interaction */
    UpdateIsoComboBox( d_data->m_ui->sldAnalogGain->value() );
        
    setWaitCursor();
    emit CameraGainChanged( multiplyBy1000(d_data->m_ui->sldAnalogGain->value()) );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxIsoChange
 *****************************************************************************/
void InOutBox::onSbxIsoChange( double value )
{
    // Set the slider to new value
    d_data->m_ui->sldAnalogGain->blockSignals( true );
    d_data->m_ui->sldAnalogGain->setValue( int(value) );
    d_data->m_ui->sldAnalogGain->blockSignals( false );

    /* Set the combo box to new value, blocking signals is not needed, as we only use
     * the activated() event of the combo box which is only triggerd by user interaction */
    UpdateIsoComboBox( int(value) );

    setWaitCursor();
    emit CameraGainChanged( int(value * 1000.0) );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxIsoChange
 *****************************************************************************/
void InOutBox::onCbxIsoChange( int index )
{
    // Check if a valid entry was selected (not "Select ISO")
    if ( index == 0)
    {
        return;
    }

    // Get iso value from combo box
    int iso = d_data->m_ui->cbxIso->currentData().toInt();

    // Set spin box to new iso value
    d_data->m_ui->sbxAnalogGain->blockSignals( true );
    d_data->m_ui->sbxAnalogGain->setValue( iso );
    d_data->m_ui->sbxAnalogGain->blockSignals( false );

    // Set slider to new iso value
    d_data->m_ui->sldAnalogGain->blockSignals( true );
    d_data->m_ui->sldAnalogGain->setValue( iso );
    d_data->m_ui->sldAnalogGain->blockSignals( false );

    // Emit gain changed event
    setWaitCursor();
    emit CameraGainChanged( multiplyBy1000(iso) );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onBtnIsoMinusClicked
 *****************************************************************************/
void InOutBox::onBtnIsoMinusClicked( )
{
    // Get current ISO value
    int currentIso = d_data->m_ui->sbxAnalogGain->value();

    // Loop over all available ISO values until the next smaller ISO is found
    int i;
    for ( i = d_data->m_ui->cbxIso->count() - 1; i >= 1; i-- )
    {
        // Get ISO for this index
        int iso = d_data->m_ui->cbxIso->itemData(i).toInt();

        // If ISO is smaller than the current ISO, this is the new ISO
        if ( iso < currentIso )
        {
            break;
        }
    }

    // Clip to smallest possible ISO (0 is "Select ISO" text)
    if ( i <= 0 )
    {
        i = 1;
    }

    // Set new ISO value
    d_data->m_ui->cbxIso->setCurrentIndex( i );

    // Manually call combo box change event
    onCbxIsoChange( i );

    // Enable / disable the plus minus buttons
    UpdateIsoPlusMinusButtons();

    // If minus button is now disabled, change focus to the plus button
    if ( !d_data->m_ui->btnIsoMinus->isEnabled() )
    {
         d_data->m_ui->btnIsoPlus->setFocus();
    }
}

/******************************************************************************
 * InOutBox::onBtnIsoPlusClicked
 *****************************************************************************/
void InOutBox::onBtnIsoPlusClicked( )
{
    // Get current ISO value
    int currentIso = d_data->m_ui->sbxAnalogGain->value();

    // Loop over all available ISO values until the next bigger ISO is found
    int i;
    for ( i = 1; i < d_data->m_ui->cbxIso->count(); i++ )
    {
        // Get ISO for this index
        int iso = d_data->m_ui->cbxIso->itemData(i).toInt();

        // If ISO is bigger than the current ISO, this is the new ISO
        if ( iso > currentIso )
        {
            break;
        }
    }

    // Clip to smallest possible ISO (0 is "Select ISO" text)
    if ( i >= d_data->m_ui->cbxIso->count() )
    {
        i = d_data->m_ui->cbxIso->count() - 1;
    }

    // Set new ISO value
    d_data->m_ui->cbxIso->setCurrentIndex( i );

    // Manually call combo box change event
    onCbxIsoChange( i );

    // Enable / disable the plus minus buttons
    UpdateIsoPlusMinusButtons();

    // If plus button is now disabled, change focus to the minus button
    if ( !d_data->m_ui->btnIsoPlus->isEnabled() )
    {
         d_data->m_ui->btnIsoMinus->setFocus();
    }
}

/******************************************************************************
 * InOutBox::onSldExposureChange
 *****************************************************************************/
void InOutBox::onSldExposureChange( int value )
{
    // Set the spin box to new value
    d_data->m_ui->sbxExposure->blockSignals( true );
    d_data->m_ui->sbxExposure->setValue( value );
    d_data->m_ui->sbxExposure->blockSignals( false );

    // Note: the combo box is set with the released() event of the slider

    if ( (d_data->m_ui->sldExposure->isSliderDown()  ) &&
         (d_data->m_cntEvents++ > d_data->m_maxEvents) )
    {
        d_data->m_cntEvents = 0;
    
        setWaitCursor();
        emit CameraExposureChanged( value );
        setNormalCursor();
    }
}

/******************************************************************************
 * InOutBox::onSldExposureReleased
 *****************************************************************************/
void InOutBox::onSldExposureReleased()
{
    d_data->m_cntEvents = 0;

    /* Set the combo box to new value, blocking signals is not needed, as we only use
     * the activated() event of the combo box which is only triggerd by user interaction */
    UpdateExposureComboBox( d_data->m_ui->sldExposure->value() );
    
    setWaitCursor();
    emit CameraExposureChanged( d_data->m_ui->sldExposure->value() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxExposureChange
 *****************************************************************************/
void InOutBox::onSbxExposureChange( int value )
{
    // Set the slider to new value
    d_data->m_ui->sldExposure->blockSignals( true );
    d_data->m_ui->sldExposure->setValue( value );
    d_data->m_ui->sldExposure->blockSignals( false );

    /* Set the combo box to new value, blocking signals is not needed, as we only use
     * the activated() event of the combo box which is only triggerd by user interaction */
    UpdateExposureComboBox( value );

    setWaitCursor();
    emit CameraExposureChanged( value );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxExposureChange
 *****************************************************************************/
void InOutBox::onCbxExposureChange( int index )
{
    // Check if a valid entry was selected (not "Select Shutter")
    if ( index == 0)
    {
        return;
    }

    // Get exposure value from combo box
    int exposure = d_data->m_ui->cbxExposure->currentData().toInt();

    // Set spin box to new exposure value
    d_data->m_ui->sbxExposure->blockSignals( true );
    d_data->m_ui->sbxExposure->setValue( exposure );
    d_data->m_ui->sbxExposure->blockSignals( false );

    // Set slider to new exposure value
    d_data->m_ui->sldExposure->blockSignals( true );
    d_data->m_ui->sldExposure->setValue( exposure );
    d_data->m_ui->sldExposure->blockSignals( false );

    // Emit exposure changed event
    setWaitCursor();
    emit CameraExposureChanged( exposure );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onBtnExposureMinusClicked
 *****************************************************************************/
void InOutBox::onBtnExposureMinusClicked( )
{
    // Get current exposure value
    int currentExposure = d_data->m_ui->sbxExposure->value();

    // Loop over all available exposures until the next smaller exposure is found
    int i;
    for ( i = d_data->m_ui->cbxExposure->count() - 1; i >= 1; i-- )
    {
        // Get exposure for this index
        int exposure = d_data->m_ui->cbxExposure->itemData(i).toInt();

        // If exposure is smaller than the current exposure, this is the new exposure
        if ( exposure < currentExposure )
        {
            break;
        }
    }

    // Clip to smallest possible exposure (0 is "Select Shutter" text)
    if ( i <= 0 )
    {
        i = 1;
    }

    // Set new exposure
    d_data->m_ui->cbxExposure->setCurrentIndex( i );

    // Manually call combo box change event
    onCbxExposureChange( i );

    // Enable / disable the plus minus buttons
    UpdateExposurePlusMinusButtons();

    // If minus button is now disabled, change focus to the plus button
    if ( !d_data->m_ui->btnExposureMinus->isEnabled() )
    {
         d_data->m_ui->btnExposurePlus->setFocus();
    }
}

/******************************************************************************
 * InOutBox::onBtnExposurePlusClicked
 *****************************************************************************/
void InOutBox::onBtnExposurePlusClicked( )
{
    // Get current exposure value
    int currentExposure = d_data->m_ui->sbxExposure->value();

    // Loop over all available exposures until the next bigger exposure is found
    int i;
    for ( i = 1; i < d_data->m_ui->cbxExposure->count(); i++ )
    {
        // Get exposure for this index
        int exposure = d_data->m_ui->cbxExposure->itemData(i).toInt();

        // If exposure is bigger than the current exposure, this is the new exposure
        if ( exposure > currentExposure )
        {
            break;
        }
    }

    // Clip to smallest possible exposure (0 is "Select Shutter" text)
    if ( i >= d_data->m_ui->cbxExposure->count() )
    {
        i = d_data->m_ui->cbxExposure->count() - 1;
    }

    // Set new exposure
    d_data->m_ui->cbxExposure->setCurrentIndex( i );

    // Manually call combo box change event
    onCbxExposureChange( i );

    // Enable / disable the plus minus buttons
    UpdateExposurePlusMinusButtons();

    // If plus button is now disabled, change focus to the minus button
    if ( !d_data->m_ui->btnExposurePlus->isEnabled() )
    {
         d_data->m_ui->btnExposureMinus->setFocus();
    }
}

/******************************************************************************
 * InOutBox::onSbxRoiOffsetXChange
 *****************************************************************************/
void InOutBox::onSbxRoiOffsetXChange( int index )
{
    d_data->roi_offset_x = index;

    // Set slider to new ROI value
    d_data->m_ui->sldRoiOffsetX->blockSignals( true );
    d_data->m_ui->sldRoiOffsetX->setValue( index );
    d_data->m_ui->sldRoiOffsetX->blockSignals( false );

    // Emit ROI changed event
    setWaitCursor();
    emit CameraRoiOffsetChanged( d_data->roi_offset_x , d_data->roi_offset_y);
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxRoiOffsetYChange
 *****************************************************************************/
void InOutBox::onSbxRoiOffsetYChange( int index )
{
    d_data->roi_offset_y = index;

    // Set slider to new ROI value
    d_data->m_ui->sldRoiOffsetY->blockSignals( true );
    d_data->m_ui->sldRoiOffsetY->setValue( index );
    d_data->m_ui->sldRoiOffsetY->blockSignals( false );

    // Emit ROI changed event
    setWaitCursor();
    emit CameraRoiOffsetChanged( d_data->roi_offset_x, d_data->roi_offset_y);
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSldRoiOffsetXChange
 *****************************************************************************/
void InOutBox::onSldRoiOffsetXChange( int index )
{
    d_data->roi_offset_x = index / d_data->offset_x_step * d_data->offset_x_step;

    // Set slider to new ROI value (reimplement slider value change)
    d_data->m_ui->sldRoiOffsetX->blockSignals( true );
    d_data->m_ui->sldRoiOffsetX->setValue( d_data->roi_offset_x );
    d_data->m_ui->sldRoiOffsetX->blockSignals( false );

    // Set spin box to new ROI value
    d_data->m_ui->sbxRoiOffsetX->blockSignals( true );
    d_data->m_ui->sbxRoiOffsetX->setValue( d_data->roi_offset_x );
    d_data->m_ui->sbxRoiOffsetX->blockSignals( false );

    // Emit ROI changed event
    setWaitCursor();
    emit CameraRoiOffsetChanged(d_data->roi_offset_x , d_data->roi_offset_y);
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSldRoiOffsetYChange
 *****************************************************************************/
void InOutBox::onSldRoiOffsetYChange( int index )
{
    d_data->roi_offset_y = index / d_data->offset_y_step * d_data->offset_y_step;

    // Set slider to new ROI value (reimplement value change)
    d_data->m_ui->sldRoiOffsetY->blockSignals( true );
    d_data->m_ui->sldRoiOffsetY->setValue( d_data->roi_offset_y );
    d_data->m_ui->sldRoiOffsetY->blockSignals( false );

    // Set spin box to new ROI value
    d_data->m_ui->sbxRoiOffsetY->blockSignals( true );
    d_data->m_ui->sbxRoiOffsetY->setValue(d_data->roi_offset_y);
    d_data->m_ui->sbxRoiOffsetY->blockSignals( false );

    // Emit ROI changed event
    setWaitCursor();
    emit CameraRoiOffsetChanged( d_data->roi_offset_x, d_data->roi_offset_y);
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxVideoModeChange
 *****************************************************************************/
void InOutBox::onCbxVideoModeChange( int index )
{
    setWaitCursor();
    emit ChainVideoModeChanged( d_data->m_ui->cbxVideoMode->itemData( index ).toInt() );
    show4kGenlockNote( d_data->m_ui->cbxVideoMode->itemData( index ).toInt() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxSdi2ModeChange
 *****************************************************************************/
void InOutBox::onCbxSdi2ModeChange( int index )
{
    setWaitCursor();
    emit ChainSdi2ModeChanged( d_data->m_ui->cbxSdi2Mode->itemData( index ).toInt() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxSdi1DownscalerChange
 *****************************************************************************/
void InOutBox::onCbxSdi1DownscalerChange( int index )
{
    setWaitCursor();

    EmitDownscaleChanged( 1 , index );

    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxSdi2DownscalerChange
 *****************************************************************************/
void InOutBox::onCbxSdi2DownscalerChange( int index )
{
    setWaitCursor();

    EmitDownscaleChanged( 2 , index );

    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxFlipModeChange
 *****************************************************************************/
void InOutBox::onCbxFlipModeChange( int index )
{
    setWaitCursor();
    emit ChainFlipModeChanged( d_data->m_ui->cbxFlipMode->itemData( index ).toInt() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxLogModeChange
 *****************************************************************************/
void InOutBox::onCbxLogModeChange( int index )
{
    setWaitCursor();
    emit LogModeChanged( d_data->m_ui->cbxLogMode->itemData( index ).toInt() );
    setNormalCursor();

    // Show max PQ brightness settings if log mode is PQ
    if ( index == LogModePQ )
    {
        d_data->m_ui->lblPQMaxBrightness->setEnabled( true );
        d_data->m_ui->sbxPQMaxBrightness->setEnabled( true );
    }
    else
    {
        d_data->m_ui->lblPQMaxBrightness->setEnabled( false );
        d_data->m_ui->sbxPQMaxBrightness->setEnabled( false );
    }
}

/******************************************************************************
 * InOutBox::onSbxPQMaxBrightnessChange
 *****************************************************************************/
void InOutBox::onSbxPQMaxBrightnessChange( int value )
{
    setWaitCursor();
    emit PQMaxBrightnessChanged( value );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxColorSpaceChange
 *****************************************************************************/
void InOutBox::onCbxColorSpaceChange( int index )
{
    setWaitCursor();
    emit ColorSpaceChanged( d_data->m_ui->cbxColorSpace->itemData( index ).toInt() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxTestPatternChange
 *****************************************************************************/
void InOutBox::onCbxTestPatternChange( int value )
{
    setWaitCursor();
    emit OsdTestPatternChanged( (value == Qt::Checked) ? 1 : 0 );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxAudioEnableChange
 *****************************************************************************/
void InOutBox::onCbxAudioEnableChange( int value )
{
    setWaitCursor();

    emit ChainAudioEnableChanged( (value == Qt::Checked) ? true : false );

    // Disable audio gain spin box if audio is disabled
    d_data->m_ui->sbxAudioGain->setEnabled( (value == Qt::Checked) ? true : false  );

    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxAudioGainChange
 *****************************************************************************/
void InOutBox::onSbxAudioGainChange( double gain )
{
    setWaitCursor();
    emit ChainAudioGainChanged( gain );
    setNormalCursor();
}


/******************************************************************************
 * InOutBox::onCbxGenlockModeChange
 *****************************************************************************/
void InOutBox::onCbxGenlockModeChange( int index )
{
    setWaitCursor();
    emit ChainGenlockModeChanged( d_data->m_ui->cbxGenLockMode->itemData( index ).toInt() );
    updateEnableOfGenlockSettings( index, d_data->m_ui->cbxGenlockCrosslockEnable->currentIndex() );
    setNormalCursor();

}

/******************************************************************************
 * InOutBox::onBtnGenLockRefreshStateClicked
 *****************************************************************************/
void InOutBox::onBtnGenlockStatusRefresh()
{
    setWaitCursor();
    emit ChainGenLockStatusRefresh();
    setNormalCursor();

}

/******************************************************************************
 * InOutBox::onCbxGenlockCrosslockEnableChange
 *****************************************************************************/
void InOutBox::onCbxGenlockCrosslockEnableChange( int index )
{
    setWaitCursor();
    emit ChainGenlockCrosslockChanged( d_data->m_ui->cbxGenlockCrosslockEnable->itemData( index ).toInt(),
                                       d_data->m_ui->cbxGenlockCrosslockVmode->currentData().toInt() );
    updateEnableOfGenlockSettings( d_data->m_ui->cbxGenLockMode->currentIndex(), index );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxGenlockCrosslockVmodeChange
 *****************************************************************************/
void InOutBox::onCbxGenlockCrosslockVmodeChange( int index )
{
    setWaitCursor();
    emit ChainGenlockCrosslockChanged( d_data->m_ui->cbxGenlockCrosslockEnable->currentData().toInt(),
                                       d_data->m_ui->cbxGenlockCrosslockVmode->itemData( index ).toInt() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxGenlockOffsetVerticalChange
 *****************************************************************************/
void InOutBox::onSbxGenlockOffsetVerticalChange( int value )
{
    setWaitCursor();
    emit ChainGenlockOffsetChanged( value, GenLockOffsetHorizontal() );

    /* Setting the genlock offset might take a while (if no lock is possible)
     * this can cause a timer event of the spinbox to trigger, which will then
     * fire the value changed event of the spinbox again.
     * See: https://github.com/MRtrix3/mrtrix3/issues/306
     *      https://bugreports.qt.io/browse/QTBUG-33128
     * To avoid this we block all signals from the spinbox and then allow the
     * application to process all pending events. */
    d_data->m_ui->sbxGenLockOffsetVertical->blockSignals( true );
    QApplication::processEvents();
    d_data->m_ui->sbxGenLockOffsetVertical->blockSignals( false );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxGenlockOffsetHorizontalChange
 *****************************************************************************/
void InOutBox::onSbxGenlockOffsetHorizontalChange( int value )
{
    setWaitCursor();
    emit ChainGenlockOffsetChanged( GenLockOffsetVertical(), value );

    /* Setting the genlock offset might take a while (if no lock is possible)
     * this can cause a timer event of the spinbox to trigger, which will then
     * fire the value changed event of the spinbox again.
     * See: https://github.com/MRtrix3/mrtrix3/issues/306
     *      https://bugreports.qt.io/browse/QTBUG-33128
     * To avoid this we block all signals from the spinbox and then allow the
     * application to process all pending events. */
    d_data->m_ui->sbxGenLockOffsetVertical->blockSignals( true );
    QApplication::processEvents();
    d_data->m_ui->sbxGenLockOffsetVertical->blockSignals( false );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxGenlockTerminationChange
 *****************************************************************************/
void InOutBox::onCbxGenlockTerminationChange( int value )
{
    if ( GenLockMode() == GetGenlockModeName( GenLockModeMaster ) && value )
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Termination enabled in Master Mode");
        msgBox.setText("You have enabled Genlock Termination while the device is in Genlock Master Mode.\n\n"
                       "In Master Mode the Termination is always disabled, regardless of the Genlock "
                       "Termination setting. It will get enabled as soon as you switch to Slave Mode.");
        msgBox.exec();
    }

    setWaitCursor();
    emit ChainGenlockTerminationChanged( (value == Qt::Checked) ? 1 : 0 );
    setNormalCursor();
}

void InOutBox::onSbxGenlockLOLFilterValueChange(int value)
{
    setWaitCursor();
    emit ChainGenlockLOLFilterChanged( value );

    // Set the genlock loss fo link Filter
    //block all signals from the spinbox then allow application to process all pending events
    d_data->m_ui->sbxGenLockLOLFilter->blockSignals( true );
    QApplication::processEvents();
    d_data->m_ui->sbxGenLockLOLFilter->blockSignals( false );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxAecEnableChange
 *****************************************************************************/
void InOutBox::onCbxAecEnableChange( int value )
{
    bool enable = (value == Qt::Checked) ? true : false;

    enableAecWidgets( enable );

    d_data->m_AecSetup.run = enable;

    setWaitCursor();
    emit AecEnableChanged( d_data->m_AecSetup.run );
    setNormalCursor();
    
    if ( value == 0 )
    {
        // aec disabled, update exposure, gain, aperture widgets
        setWaitCursor();
        emit ResyncRequest();
        setNormalCursor();
    }
}

/******************************************************************************
 * InOutBox::onCbxMaxAnalogGainEnableChange
 *****************************************************************************/
void InOutBox::onCbxMaxAnalogGainEnableChange( int value )
{
    bool enable = (value == Qt::Checked) ? true : false;

    d_data->m_AecSetup.activeGain = enable;
    d_data->m_AecSetup.maxGain = multiplyBy1000(d_data->m_ui->sldMaxAnalogGain->value());

    enableGainConfWidgets( !enable );
    enableMaxAnalogGainWidgets( enable );

    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxMaxExposureEnableChange
 *****************************************************************************/
void InOutBox::onCbxMaxExposureEnableChange( int value )
{
    bool enable = (value == Qt::Checked) ? true : false;

    d_data->m_AecSetup.activeExposure = enable;

    enableCamConfWidgets( !enable );
    enableMaxExposureWidgets( enable );

    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxAecWeightChange
 *****************************************************************************/
void InOutBox::onCbxAecWeightChange( int value )
{
    bool enable = (value == Qt::Checked) ? true : false;

    d_data->m_AecSetup.useCustomWeighting = enable;

    // Run enable aec widgets to enable / disable the set weighting button
    enableAecWidgets( d_data->m_AecSetup.run );

    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onAecEnableChange
 *****************************************************************************/
void InOutBox::onAecEnableChange( int enable )
{
    enableAecWidgets( enable ? true : false );

    d_data->m_ui->cbxAecEnable->blockSignals( true );
    d_data->m_ui->cbxAecEnable->setCheckState( enable ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxAecEnable->blockSignals( false );

    d_data->m_AecSetup.run = enable ? true : false;
    createAecVector();
}

/******************************************************************************
 * InOutBox::onAecSetupChange
 *****************************************************************************/
void InOutBox::onAecSetupChange( QVector<int> values )
{
    if ( values.count() == 10 )
    {
        d_data->m_AecSetup.run                = values[0];
        d_data->m_AecSetup.setPoint           = values[1];
        d_data->m_AecSetup.speed              = values[2];
        d_data->m_AecSetup.clmTolerance       = values[3];
        d_data->m_AecSetup.activeGain         = values[4];
        d_data->m_AecSetup.activeExposure     = values[5];
        d_data->m_AecSetup.activeAperture     = values[6];
        d_data->m_AecSetup.maxExposure        = values[7];
        d_data->m_AecSetup.maxGain            = values[8];
        d_data->m_AecSetup.useCustomWeighting = values[9];

        // Run enable aec widgets to enable / disable the set weighting button
        enableAecWidgets( d_data->m_AecSetup.run );

        updateAecSetupWidgets();
    }
}

/******************************************************************************
 * InOutBox::onAecWeightsChange
 *****************************************************************************/
void InOutBox::onAecWeightsChange( QVector<int> weights )
{
    // Change weights in weight dialog
    emit WeightDialogAecWeightsChanged( weights );
}

/******************************************************************************
 * InOutBox::onSldSetPointChange
 *****************************************************************************/
void InOutBox::onSldSetPointChange( int value )
{
    d_data->m_ui->sbxSetPoint->blockSignals( true );
    d_data->m_ui->sbxSetPoint->setValue( value );
    d_data->m_ui->sbxSetPoint->blockSignals( false );

    d_data->m_AecSetup.setPoint = value;

    if ( (d_data->m_ui->sldSetPoint->isSliderDown()  ) &&
         (d_data->m_cntEvents++ > d_data->m_maxEvents) )
    {
        d_data->m_cntEvents = 0;
        
        setWaitCursor();
        emit AecSetupChanged( createAecVector() );
        setNormalCursor();
    }
}

/******************************************************************************
 * InOutBox::onSldSetPointReleased
 *****************************************************************************/
void InOutBox::onSldSetPointReleased()
{
    d_data->m_cntEvents = 0;

    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxSetPointChange
 *****************************************************************************/
void InOutBox::onSbxSetPointChange( int value )
{
    d_data->m_ui->sldSetPoint->blockSignals( true );
    d_data->m_ui->sldSetPoint->setValue( value );
    d_data->m_ui->sldSetPoint->blockSignals( false );

    d_data->m_AecSetup.setPoint = value;
    
    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSldMaxAnalogGainChange
 *****************************************************************************/
void InOutBox::onSldMaxAnalogGainChange( int value )
{
    d_data->m_ui->sbxMaxAnalogGain->blockSignals( true );
    d_data->m_ui->sbxMaxAnalogGain->setValue( value );
    d_data->m_ui->sbxMaxAnalogGain->blockSignals( false );

    d_data->m_AecSetup.maxGain = multiplyBy1000( value );

    if ( (d_data->m_ui->sldMaxAnalogGain->isSliderDown()  ) &&
         (d_data->m_cntEvents++ > d_data->m_maxEvents) )
    {
        d_data->m_cntEvents = 0;

        setWaitCursor();
        emit AecSetupChanged( createAecVector() );
        setNormalCursor();
    }
}

/******************************************************************************
 * InOutBox::onSldMaxAnalogGainReleased
 *****************************************************************************/
void InOutBox::onSldMaxAnalogGainReleased()
{
    d_data->m_cntEvents = 0;

    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxMaxAnalogGainChange
 *****************************************************************************/
void InOutBox::onSbxMaxAnalogGainChange( double value )
{
    d_data->m_ui->sldMaxAnalogGain->blockSignals( true );
    d_data->m_ui->sldMaxAnalogGain->setValue( int(value) );
    d_data->m_ui->sldMaxAnalogGain->blockSignals( false );

    d_data->m_AecSetup.maxGain = int(value * 1000.0);

    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSldMaxExposureChange
 *****************************************************************************/
void InOutBox::onSldMaxExposureChange( int value )
{
    d_data->m_ui->sbxMaxExposure->blockSignals( true );
    d_data->m_ui->sbxMaxExposure->setValue( value );
    d_data->m_ui->sbxMaxExposure->blockSignals( false );

    d_data->m_AecSetup.maxExposure = value;

    if ( (d_data->m_ui->sldMaxExposure->isSliderDown()  ) &&
         (d_data->m_cntEvents++ > d_data->m_maxEvents) )
    {
        d_data->m_cntEvents = 0;

        setWaitCursor();
        emit AecSetupChanged( createAecVector() );
        setNormalCursor();
    }
}

/******************************************************************************
 * InOutBox::onSldMaxExposureReleased
 *****************************************************************************/
void InOutBox::onSldMaxExposureReleased()
{
    d_data->m_cntEvents = 0;

    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxMaxExposureChange
 *****************************************************************************/
void InOutBox::onSbxMaxExposureChange( int value )
{
    d_data->m_ui->sldMaxExposure->blockSignals( true );
    d_data->m_ui->sldMaxExposure->setValue( value );
    d_data->m_ui->sldMaxExposure->blockSignals( false );

    d_data->m_AecSetup.maxExposure = value;

    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSldControlSpeedChange
 *****************************************************************************/
void InOutBox::onSldControlSpeedChange( int value )
{
    d_data->m_ui->sbxControlSpeed->blockSignals( true );
    d_data->m_ui->sbxControlSpeed->setValue( value );
    d_data->m_ui->sbxControlSpeed->blockSignals( false );

    d_data->m_AecSetup.speed = value;

    if ( (d_data->m_ui->sldControlSpeed->isSliderDown() ) &&
         (d_data->m_cntEvents++ > d_data->m_maxEvents)    )
    {
        d_data->m_cntEvents = 0;
        
        setWaitCursor();
        emit AecSetupChanged( createAecVector() );
        setNormalCursor();
    }
}

/******************************************************************************
 * InOutBox::onSldControlSpeedReleased
 *****************************************************************************/
void InOutBox::onSldControlSpeedReleased()
{
    d_data->m_cntEvents = 0;

    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxControlSpeedChange
 *****************************************************************************/
void InOutBox::onSbxControlSpeedChange( int value )
{
    d_data->m_ui->sldControlSpeed->blockSignals( true );
    d_data->m_ui->sldControlSpeed->setValue( value );
    d_data->m_ui->sldControlSpeed->blockSignals( false );

    d_data->m_AecSetup.speed = value;
    
    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSldMaxIsoChange
 *****************************************************************************/
void InOutBox::onSldToleranceChange( int value )
{
    d_data->m_ui->sbxTolerance->blockSignals( true );
    d_data->m_ui->sbxTolerance->setValue( double(value) / 10.0);
    d_data->m_ui->sbxTolerance->blockSignals( false );

    d_data->m_AecSetup.clmTolerance = value;

    if ( (d_data->m_ui->sldTolerance->isSliderDown()  ) &&
         (d_data->m_cntEvents++ > d_data->m_maxEvents) )
    {
        d_data->m_cntEvents = 0;

        setWaitCursor();
        emit AecSetupChanged( createAecVector() );
        setNormalCursor();
    }
}

/******************************************************************************
 * InOutBox::onSldMaxIsoReleased
 *****************************************************************************/
void InOutBox::onSldToleranceReleased()
{
    d_data->m_cntEvents = 0;

    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxMaxIsoChange
 *****************************************************************************/
void InOutBox::onSbxToleranceChange( double value )
{
    d_data->m_ui->sldTolerance->blockSignals( true );
    d_data->m_ui->sldTolerance->setValue( int(value * 10.0) );
    d_data->m_ui->sldTolerance->blockSignals( false );

    d_data->m_AecSetup.clmTolerance = int(value * 10.0);

    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onAecStatChange
 *****************************************************************************/
void InOutBox::onAecStatChange( QVector<int> values )
{
    (void)values;
    /*
    15 Parameter:
    z
    lumaDeviation   
    semSetPoint
    meanLuma
    meanLumaObject                
    meanHistogram
    clmHistogramSize
    sumHistogram 
    maxExposure
    newExposure
    targetExposure
    realExposure                 
    gain
    t_int                
    aperture
    */
}

/******************************************************************************
 * InOutBox::UpdateIsoPlusMinusButtons
 *****************************************************************************/
void InOutBox::UpdateIsoPlusMinusButtons()
{
    // Get current ISO value
    int iso = d_data->m_ui->sbxAnalogGain->value();

    /* Disable minus button, if the value is smaller than the value of the first combo box item
     * or the iso spin box is diabled (aec enabled) */
    if ( iso <= d_data->m_ui->cbxIso->itemData(1).toInt() ||
         !d_data->m_ui->sbxAnalogGain->isEnabled() )
    {
        d_data->m_ui->btnIsoMinus->setEnabled( false );
    }
    // Else enable it
    else
    {
        d_data->m_ui->btnIsoMinus->setEnabled( true );
    }

    /* Disable plus button, if this the value is bigger than the value of the last combo box item
     * or the iso spin box is disabled (aec enabled) */
    if ( iso >= d_data->m_ui->cbxIso->itemData(d_data->m_ui->cbxIso->count() - 1).toInt() ||
         !d_data->m_ui->sbxAnalogGain->isEnabled() )
    {
        d_data->m_ui->btnIsoPlus->setEnabled( false );
    }
    // Else enable it
    else
    {
        d_data->m_ui->btnIsoPlus->setEnabled( true );
    }
}

/******************************************************************************
 * InOutBox::UpdateExposurePlusMinusButtons
 *****************************************************************************/
void InOutBox::UpdateExposurePlusMinusButtons()
{
    // Get current exposure value
    int exposure = d_data->m_ui->sbxExposure->value();

    int currentExposure = d_data->m_ui->cbxExposure->itemData(1).toInt();

    /* Disable minus button, if the value is smaller than the value of the first combo box item
     * or the exposure spin box is diabled (aec enabled) */
    if ( exposure <= currentExposure ||
         !d_data->m_ui->sbxExposure->isEnabled() )
    {
        d_data->m_ui->btnExposureMinus->setEnabled( false );
    }
    // Else enable it
    else
    {
        d_data->m_ui->btnExposureMinus->setEnabled( true );
    }

    /* Disable plus button, if this the value is bigger than the value of the last combo box item
     * or the exposure spin box is diabled (aec enabled) */
    if ( exposure >= d_data->m_ui->cbxExposure->itemData(d_data->m_ui->cbxExposure->count() - 1).toInt() ||
         !d_data->m_ui->sbxExposure->isEnabled() )
    {
        d_data->m_ui->btnExposurePlus->setEnabled( false );
    }
    // Else enable it
    else
    {
        d_data->m_ui->btnExposurePlus->setEnabled( true );
    }
}

/******************************************************************************
 * InOutBox::UpdateIsoComboBox
 *****************************************************************************/
void InOutBox::UpdateIsoComboBox( int iso )
{
    // Check if the selected ISO value is one of the fixed ISO values
    int index = d_data->m_ui->cbxIso->findData( iso );

    /* In case nothing was found, findData() returns -1, in this case set index to 0
     * to show the default "Select ISO" text in the combo box */
    if ( index == -1 )
    {
        index = 0;
    }

    // Set the current combo box index
    d_data->m_ui->cbxIso->setCurrentIndex( index );

    // Enable / disable the plus minus buttons
    UpdateIsoPlusMinusButtons();
}

/******************************************************************************
 * InOutBox::UpdateExposureComboBox
 *****************************************************************************/
void InOutBox::UpdateExposureComboBox( int exposure )
{
    // Check if the selected exposure value is one of the fixed exposure values
    int index = d_data->m_ui->cbxExposure->findData( exposure );

    /* In case nothing was found, findData() returns -1, in this case set index to 0
     * to show the default "Select Shutter" text in the combo box */
    if ( index == -1 )
    {
        index = 0;
    }

    // Set the current combo box index
    d_data->m_ui->cbxExposure->setCurrentIndex( index );

    // Enable / disable the plus minus buttons
    UpdateExposurePlusMinusButtons();
}

/******************************************************************************
 * InOutBox::updateAecSetupWidgets
 *****************************************************************************/
void InOutBox::updateAecSetupWidgets( void )
{
    // Custom weighting
    d_data->m_ui->cbxAecWeight->blockSignals( true );
    d_data->m_ui->cbxAecWeight->setChecked( d_data->m_AecSetup.useCustomWeighting );
    d_data->m_ui->cbxAecWeight->blockSignals( false );

    // Set point
    d_data->m_ui->sbxSetPoint->blockSignals( true );
    d_data->m_ui->sbxSetPoint->setValue( d_data->m_AecSetup.setPoint );
    d_data->m_ui->sbxSetPoint->blockSignals( false );
    d_data->m_ui->sldSetPoint->blockSignals( true );
    d_data->m_ui->sldSetPoint->setValue( d_data->m_AecSetup.setPoint );
    d_data->m_ui->sldSetPoint->blockSignals( false );

    // Max Analog Gain
    d_data->m_ui->cbxMaxAnalogGainEnable->blockSignals( true );
    d_data->m_ui->cbxMaxAnalogGainEnable->setChecked(d_data->m_AecSetup.activeGain);
    d_data->m_ui->cbxMaxAnalogGainEnable->blockSignals( false );
    d_data->m_ui->sbxMaxAnalogGain->blockSignals( true );
    d_data->m_ui->sbxMaxAnalogGain->setValue( gainToIso(d_data->m_AecSetup.maxGain) );
    d_data->m_ui->sbxMaxAnalogGain->blockSignals( false );
    d_data->m_ui->sldMaxAnalogGain->blockSignals( true );
    d_data->m_ui->sldMaxAnalogGain->setValue( gainToIso(d_data->m_AecSetup.maxGain) );
    d_data->m_ui->sldMaxAnalogGain->blockSignals( false );

    // Max Exposure
    d_data->m_ui->cbxMaxExposureEnable->blockSignals( true );
    d_data->m_ui->cbxMaxExposureEnable->setChecked(d_data->m_AecSetup.activeExposure);
    d_data->m_ui->cbxMaxExposureEnable->blockSignals( false );
    d_data->m_ui->sbxMaxExposure->blockSignals( true );
    d_data->m_ui->sbxMaxExposure->setValue( gainToIso(d_data->m_AecSetup.maxExposure) );
    d_data->m_ui->sbxMaxExposure->blockSignals( false );
    d_data->m_ui->sldMaxExposure->blockSignals( true );
    d_data->m_ui->sldMaxExposure->setValue( gainToIso(d_data->m_AecSetup.maxExposure) );
    d_data->m_ui->sldMaxExposure->blockSignals( false );

    // Control Speed
    d_data->m_ui->sbxControlSpeed->blockSignals( true );
    d_data->m_ui->sbxControlSpeed->setValue( d_data->m_AecSetup.speed );
    d_data->m_ui->sbxControlSpeed->blockSignals( false );
    d_data->m_ui->sldControlSpeed->blockSignals( true );
    d_data->m_ui->sldControlSpeed->setValue( d_data->m_AecSetup.speed );
    d_data->m_ui->sldControlSpeed->blockSignals( false );

    // flicker
    d_data->m_ui->rdbTaf50Hz->blockSignals( true );
    d_data->m_ui->rdbTaf50Hz->setChecked( d_data->m_AecSetup.maxExposure == 10000 );
    d_data->m_ui->rdbTaf50Hz->blockSignals( false );

    d_data->m_ui->rdbTaf60Hz->blockSignals( true );
    d_data->m_ui->rdbTaf60Hz->setChecked( d_data->m_AecSetup.maxExposure == 8333 );
    d_data->m_ui->rdbTaf60Hz->blockSignals( false );

    // Tolerance
    d_data->m_ui->sbxTolerance->blockSignals( true );
    d_data->m_ui->sbxTolerance->setValue( double(d_data->m_AecSetup.clmTolerance) / 10.0);
    d_data->m_ui->sbxTolerance->blockSignals( false );
    d_data->m_ui->sldTolerance->blockSignals( true );
    d_data->m_ui->sldTolerance->setValue( d_data->m_AecSetup.clmTolerance );
    d_data->m_ui->sldTolerance->blockSignals( false );
}

/******************************************************************************
 * InOutBox::updateLscWidgets
 *****************************************************************************/
void InOutBox::updateLscWidgets( void )
{
    // enable
    d_data->m_ui->cbxLscEnable->blockSignals( true );
    d_data->m_ui->cbxLscEnable->setChecked( d_data->m_LscSetup.enable );
    d_data->m_ui->cbxLscEnable->blockSignals( false );

    // k
    d_data->m_ui->sbxK->blockSignals( true );
    d_data->m_ui->sbxK->setValue( static_cast<double>(d_data->m_LscSetup.k) );
    d_data->m_ui->sbxK->setEnabled( d_data->m_LscSetup.enable );
    d_data->m_ui->sbxK->blockSignals( false );
    d_data->m_ui->sldK->blockSignals( true );
    d_data->m_ui->sldK->setValue( static_cast<int>(d_data->m_LscSetup.k * 100.0f) );
    d_data->m_ui->sldK->setEnabled( d_data->m_LscSetup.enable );
    d_data->m_ui->sldK->blockSignals( false );

    // offset
    d_data->m_ui->sbxOffset->blockSignals( true );
    d_data->m_ui->sbxOffset->setValue( static_cast<double>(d_data->m_LscSetup.offset) );
    d_data->m_ui->sbxOffset->setEnabled( d_data->m_LscSetup.enable );
    d_data->m_ui->sbxOffset->blockSignals( false );
    d_data->m_ui->sldOffset->blockSignals( true );
    d_data->m_ui->sldOffset->setValue( static_cast<int>(d_data->m_LscSetup.offset * 100.0f) );
    d_data->m_ui->sldOffset->setEnabled( d_data->m_LscSetup.enable );
    d_data->m_ui->sldOffset->blockSignals( false );

    // slope
    d_data->m_ui->sbxSlope->blockSignals( true );
    d_data->m_ui->sbxSlope->setValue( static_cast<double>(d_data->m_LscSetup.slope) );
    d_data->m_ui->sbxSlope->setEnabled( d_data->m_LscSetup.enable );
    d_data->m_ui->sbxSlope->blockSignals( false );
    d_data->m_ui->sldSlope->blockSignals( true );
    d_data->m_ui->sldSlope->setValue( static_cast<int>(d_data->m_LscSetup.slope * 100.0f) );
    d_data->m_ui->sldSlope->setEnabled( d_data->m_LscSetup.enable );
    d_data->m_ui->sldSlope->blockSignals( false );
}

/******************************************************************************
 * InOutBox::enableAecWidgets
 *****************************************************************************/
void InOutBox::enableAecWidgets( bool enable )
{
    d_data->m_ui->cbxAecWeight->setEnabled( enable );

    /* The configure weight button shall only be enabled, if custom weighting
     * is enabled */
    if ( d_data->m_AecSetup.useCustomWeighting && enable )
    {
        d_data->m_ui->btnAecWeight->setEnabled( true );
    }
    else
    {
        d_data->m_ui->btnAecWeight->setEnabled( false );

        // Make sure the weights dialog is closed
        d_data->m_weightDialog->close();
    }

    d_data->m_ui->sbxSetPoint->setEnabled( enable );
    d_data->m_ui->sldSetPoint->setEnabled( enable );

    d_data->m_ui->cbxMaxAnalogGainEnable->setEnabled( enable );
    d_data->m_ui->sbxMaxAnalogGain->setEnabled( enable ? d_data->m_AecSetup.activeGain : enable );
    d_data->m_ui->sldMaxAnalogGain->setEnabled( enable ? d_data->m_AecSetup.activeGain : enable );

    d_data->m_ui->cbxMaxExposureEnable->setEnabled( enable );
    d_data->m_ui->sbxMaxExposure->setEnabled(enable ? d_data->m_AecSetup.activeExposure : enable );
    d_data->m_ui->sldMaxExposure->setEnabled(enable ? d_data->m_AecSetup.activeExposure : enable );

    enableGainConfWidgets(enable ? !d_data->m_AecSetup.activeGain : true );
    enableCamConfWidgets(enable ? !d_data->m_AecSetup.activeExposure : true );

    d_data->m_ui->sbxControlSpeed->setEnabled( enable );
    d_data->m_ui->sldControlSpeed->setEnabled( enable );

    d_data->m_ui->rdbTaf50Hz->setEnabled( enable );
    d_data->m_ui->rdbTaf60Hz->setEnabled( enable );

    d_data->m_ui->sbxTolerance->setEnabled( enable );
    d_data->m_ui->sldTolerance->setEnabled( enable );
}

/******************************************************************************
 * InOutBox::enableCamConfWidgets
 *****************************************************************************/
void InOutBox::enableCamConfWidgets( bool enable )
{
    d_data->m_ui->sbxExposure->setEnabled( enable );
    d_data->m_ui->sldExposure->setEnabled( enable );
    d_data->m_ui->cbxExposure->setEnabled( enable );
    UpdateExposurePlusMinusButtons();

    d_data->m_ui->sbxAperture->setEnabled( enable && d_data->m_AptEnable );
    d_data->m_ui->sldAperture->setEnabled( enable && d_data->m_AptEnable );
}

/******************************************************************************
 * InOutBox::enableIsoConfWidgets
 *****************************************************************************/
void InOutBox::enableGainConfWidgets( bool enable )
{
    d_data->m_ui->sbxAnalogGain->setEnabled( enable );
    d_data->m_ui->sldAnalogGain->setEnabled( enable );
    d_data->m_ui->cbxIso->setEnabled( enable );
    UpdateIsoPlusMinusButtons();
}

/******************************************************************************
 * InOutBox::enableMaxAnalogGainWidgets
 *****************************************************************************/
void InOutBox::enableMaxAnalogGainWidgets( bool enable )
{
    d_data->m_ui->sbxMaxAnalogGain->setEnabled( enable );
    d_data->m_ui->sldMaxAnalogGain->setEnabled( enable );
}

/******************************************************************************
 * InOutBox::enableMaxExposureWidgets
 *****************************************************************************/
void InOutBox::enableMaxExposureWidgets( bool enable )
{
    d_data->m_ui->sbxMaxExposure->setEnabled( enable );
    d_data->m_ui->sldMaxExposure->setEnabled( enable );
}

/******************************************************************************
 * InOutBox::updateEnableOfGenlockSettings
 *****************************************************************************/
void InOutBox::updateEnableOfGenlockSettings( int genlockMode, int crosslockMode )
{
    if ( genlockMode == GenLockModeMaster )
    {
        d_data->m_ui->cbxGenlockCrosslockEnable->setEnabled( false );
        d_data->m_ui->cbxGenlockCrosslockVmode->setEnabled( false );
        //d_data->m_ui->sbxGenLockOffsetVertical->setEnabled( false );
        //d_data->m_ui->sbxGenlockOffsetHorizontal->setEnabled( false );
        d_data->m_ui->cbxGenLockTermination->setEnabled( false );
    }
    else
    {
        d_data->m_ui->cbxGenlockCrosslockEnable->setEnabled( true );

        if ( crosslockMode == GenLockCrosslockEnableOtherHDMode )
        {
            d_data->m_ui->cbxGenlockCrosslockVmode->setEnabled( true );
        }
        else
        {
            d_data->m_ui->cbxGenlockCrosslockVmode->setEnabled( false );
        }

        d_data->m_ui->sbxGenLockOffsetVertical->setEnabled( true );
        d_data->m_ui->sbxGenlockOffsetHorizontal->setEnabled( true );
        d_data->m_ui->cbxGenLockTermination->setEnabled( true );
    }
}

/******************************************************************************
 * InOutBox::createAecVector
 *****************************************************************************/
QVector<int> InOutBox::createAecVector( void )
{
    // pack aec setup values into vector
    QVector<int> values(10);
    values[0] = static_cast<int>(d_data->m_AecSetup.run);
    values[1] = d_data->m_AecSetup.setPoint;
    values[2] = d_data->m_AecSetup.speed;
    values[3] = d_data->m_AecSetup.clmTolerance;
    values[4] = d_data->m_AecSetup.activeGain;
    values[5] = d_data->m_AecSetup.activeExposure;
    values[6] = d_data->m_AecSetup.activeAperture;
    values[7] = d_data->m_AecSetup.maxExposure;
    values[8] = d_data->m_AecSetup.maxGain;
    values[9] = static_cast<int>(d_data->m_AecSetup.useCustomWeighting);
    return ( values );
}

/******************************************************************************
 * InOutBox::createLscVector
 *****************************************************************************/
QVector<uint> InOutBox::createLscVector( void )
{
    // pack LSC setup values into vector
    QVector<uint> values(4);
    values[0] = d_data->m_LscSetup.enable;

    /* The parameters k, offset and slope have to be converted from
     * float to Q2.30 fixed point format */
    values[1] = static_cast<uint>(d_data->m_LscSetup.k * (1u << 30u));
    values[2] = static_cast<uint>(d_data->m_LscSetup.offset * (1u << 30u));
    values[3] = static_cast<uint>(d_data->m_LscSetup.slope * (1u << 30u));
    return ( values );
}

/******************************************************************************
 * InOutBox::onBtnTimecodeSetClicked
 *****************************************************************************/
void InOutBox::onBtnTimecodeSetClicked()
{
    QVector<int> v_time(3);
    
    const QTime& time = d_data->m_ui->tmeTimecode->time();
    v_time[0] = time.hour();
    v_time[1] = time.minute();
    v_time[2] = time.second();

    setWaitCursor();
    emit ChainTimecodeSetChanged( v_time );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onBtnTimecodeGetClicked
 *****************************************************************************/
void InOutBox::onBtnTimecodeGetClicked()
{
    setWaitCursor();
    emit ChainTimecodeGetRequested();
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onBtnTimecodeHoldClicked
 *****************************************************************************/
void InOutBox::onBtnTimecodeHoldClicked( bool checked )
{
    setWaitCursor();
    emit ChainTimecodeHoldChanged( checked );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onChainTimecodeChange
 *****************************************************************************/
void InOutBox::onChainTimecodeChange( QVector<int> time )
{
    d_data->m_ui->tmeTimecode->setTime( QTime(time[0], time[1], time[2]) );
}

/******************************************************************************
 * InOutBox::onChainTimecodeHoldChange
 *****************************************************************************/
void InOutBox::onChainTimecodeHoldChange( bool enable )
{
    d_data->m_ui->btnHoldTimecode->blockSignals( true );
    d_data->m_ui->btnHoldTimecode->setChecked( enable );
    d_data->m_ui->btnHoldTimecode->blockSignals( false );
}

/******************************************************************************
 * InOutBox::onTafToggle
 *****************************************************************************/
void InOutBox::onTafToggle( bool )
{
    if( d_data->m_ui->rdbTaf50Hz->isChecked() )
    {
        // 50 Hz
        d_data->m_AecSetup.maxExposure = 10000;
    }
    else
    {
        // 60 Hz
        d_data->m_AecSetup.maxExposure = 8333;
    }

    setWaitCursor();
    emit AecSetupChanged( createAecVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onIrisAptChange
 *****************************************************************************/
void InOutBox::onIrisAptChange( int value )
{
    // Protocol layer signals aperture change
    value = 100 * ( value - d_data->m_AptMin ) / ( d_data->m_AptMax - d_data->m_AptMin );

    d_data->m_ui->sldAperture->blockSignals( true );
    d_data->m_ui->sldAperture->setValue( value );
    d_data->m_ui->sldAperture->blockSignals( false );

    d_data->m_ui->sbxAperture->blockSignals( true );
    d_data->m_ui->sbxAperture->setValue( value );
    d_data->m_ui->sbxAperture->blockSignals( false );
}

/******************************************************************************
 * InOutBox::onSldIrisAptChange
 *****************************************************************************/
void InOutBox::onSldIrisAptChange( int value )
{
    d_data->m_ui->sbxAperture->blockSignals( true );
    d_data->m_ui->sbxAperture->setValue( value );
    d_data->m_ui->sbxAperture->blockSignals( false );

    if ( (d_data->m_ui->sldAperture->isSliderDown()  ) &&
         (d_data->m_cntEvents++ > d_data->m_maxEvents) )
    {
        // User changes state of widgets
        // widget as value range from 0..100
        // convert from percentage to aperture
        int aperture = d_data->m_AptMin + ( ( d_data->m_AptMax - d_data->m_AptMin ) * value ) / 100;

        d_data->m_cntEvents = 0;

        setWaitCursor();
        emit IrisAptChanged( aperture );
        setNormalCursor();
    }
}

/******************************************************************************
 * InOutBox::onSldIrisAptReleased
 *****************************************************************************/
void InOutBox::onSldIrisAptReleased()
{
    // User changes state of widgets
    // widget as value range from 0..100
    // convert from percentage to aperture
    int aperture = d_data->m_AptMin + ( ( d_data->m_AptMax - d_data->m_AptMin ) * d_data->m_ui->sldAperture->value() ) / 100;

    setWaitCursor();
    emit IrisAptChanged( aperture );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxIrisAptChange
 *****************************************************************************/
void InOutBox::onSbxIrisAptChange( int value )
{
    d_data->m_ui->sldAperture->blockSignals( true );
    d_data->m_ui->sldAperture->setValue( value );
    d_data->m_ui->sldAperture->blockSignals( false );

    // User changes state of widgets
    // widget as value range from 0..100
    // convert from percentage to aperture
    int aperture = d_data->m_AptMin + ( ( d_data->m_AptMax - d_data->m_AptMin ) * value ) / 100;
    
    setWaitCursor();
    emit IrisAptChanged( aperture );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onIrisSetupChange
 *****************************************************************************/
void InOutBox::onIrisSetupChange( QVector<int> values )
{
    // determine cameras min and max aperture
    d_data->m_AptMin = values[8];   // min aperture
    d_data->m_AptMax = values[9];   // max aperture
}

/******************************************************************************
 * InOutBox::onIrisAptError
 *****************************************************************************/
void InOutBox::onIrisAptError( void )
{
    // Iris Aperture error reported
    // means there is no stepper motor for aperture control present.
    // disabling aperture control
    d_data->m_ui->lblAperture->setVisible( false );
    d_data->m_ui->sbxAperture->setVisible( false );
    d_data->m_ui->sldAperture->setVisible( false );

    d_data->m_AptEnable = false;
}

/******************************************************************************
 * InOutBox::onLscChange
 *****************************************************************************/
void InOutBox::onLscChange( QVector<uint> values )
{
    if ( values.count() == 4 )
    {
        d_data->m_LscSetup.enable = static_cast<bool>(values[0]);

        /* The parameters k, offset and slope have to be converted from
         * Q2.30 fixed point format to float */
        d_data->m_LscSetup.k      = static_cast<float>(values[1]) / static_cast<float>(1u << 30u);
        d_data->m_LscSetup.offset = static_cast<float>(values[2]) / static_cast<float>(1u << 30u);
        d_data->m_LscSetup.slope  = static_cast<float>(values[3]) / static_cast<float>(1u << 30u);

        updateLscWidgets();
    }
}

/******************************************************************************
 * InOutBox::cbxLscEnableChange
 *****************************************************************************/
void InOutBox::onCbxLscEnableChange( int value )
{
    bool enable = (value == Qt::Checked) ? true : false;

    d_data->m_LscSetup.enable = enable;

    updateLscWidgets();

    setWaitCursor();
    emit LscChanged( createLscVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSldKChange
 *****************************************************************************/
void InOutBox::onSldKChange( int value )
{
    float sbxValue = static_cast<float>(value) / 100.0f;

    d_data->m_ui->sbxK->blockSignals( true );
    d_data->m_ui->sbxK->setValue( static_cast<double>(sbxValue) );
    d_data->m_ui->sbxK->blockSignals( false );

    d_data->m_LscSetup.k = sbxValue;

    if ( !d_data->m_ui->sldK->isSliderDown() ||
         (d_data->m_cntEvents++ > d_data->m_maxEvents) )
    {
        d_data->m_cntEvents = 0;

        setWaitCursor();
        emit LscChanged( createLscVector() );
        setNormalCursor();
    }
}

/******************************************************************************
 * InOutBox::onSldKReleased
 *****************************************************************************/
void InOutBox::onSldKReleased()
{
    d_data->m_cntEvents = 0;

    setWaitCursor();
    emit LscChanged( createLscVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxKChange
 *****************************************************************************/
void InOutBox::onSbxKChange( double value )
{
    int sldValue = static_cast<int>(value * 100.0);

    d_data->m_ui->sldK->blockSignals( true );
    d_data->m_ui->sldK->setValue( sldValue );
    d_data->m_ui->sldK->blockSignals( false );

    d_data->m_LscSetup.k = static_cast<float>(value);

    setWaitCursor();
    emit LscChanged( createLscVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSldOffsetChange
 *****************************************************************************/
void InOutBox::onSldOffsetChange( int value )
{
    float sbxValue = static_cast<float>(value / 100.0);

    d_data->m_ui->sbxOffset->blockSignals( true );
    d_data->m_ui->sbxOffset->setValue( static_cast<double>(sbxValue) );
    d_data->m_ui->sbxOffset->blockSignals( false );

    d_data->m_LscSetup.offset = sbxValue;

    if ( !d_data->m_ui->sldOffset->isSliderDown() ||
         (d_data->m_cntEvents++ > d_data->m_maxEvents) )
    {
        d_data->m_cntEvents = 0;

        setWaitCursor();
        emit LscChanged( createLscVector() );
        setNormalCursor();
    }
}

/******************************************************************************
 * InOutBox::onSldOffsetReleased
 *****************************************************************************/
void InOutBox::onSldOffsetReleased()
{
    d_data->m_cntEvents = 0;

    setWaitCursor();
    emit LscChanged( createLscVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxOffsetChange
 *****************************************************************************/
void InOutBox::onSbxOffsetChange( double value )
{
    int sldValue = static_cast<int>(value * 100.0);

    d_data->m_ui->sldOffset->blockSignals( true );
    d_data->m_ui->sldOffset->setValue( sldValue );
    d_data->m_ui->sldOffset->blockSignals( false );

    d_data->m_LscSetup.offset = static_cast<float>(value);

    setWaitCursor();
    emit LscChanged( createLscVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSldSlopeChange
 *****************************************************************************/
void InOutBox::onSldSlopeChange( int value )
{
    float sbxValue = static_cast<float>(value / 100.0);

    d_data->m_ui->sbxSlope->blockSignals( true );
    d_data->m_ui->sbxSlope->setValue( static_cast<double>(sbxValue) );
    d_data->m_ui->sbxSlope->blockSignals( false );

    d_data->m_LscSetup.slope = sbxValue;

    if ( !d_data->m_ui->sldOffset->isSliderDown() ||
         (d_data->m_cntEvents++ > d_data->m_maxEvents) )
    {
        d_data->m_cntEvents = 0;

        setWaitCursor();
        emit LscChanged( createLscVector() );
        setNormalCursor();
    }
}

/******************************************************************************
 * InOutBox::onSldSlopeReleased
 *****************************************************************************/
void InOutBox::onSldSlopeReleased()
{
    d_data->m_cntEvents = 0;

    setWaitCursor();
    emit LscChanged( createLscVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxSlopeChange
 *****************************************************************************/
void InOutBox::onSbxSlopeChange( double value )
{
    int sldValue = static_cast<int>(value * 100.0);

    d_data->m_ui->sldSlope->blockSignals( true );
    d_data->m_ui->sldSlope->setValue( sldValue );
    d_data->m_ui->sldSlope->blockSignals( false );

    d_data->m_LscSetup.slope = static_cast<float>(value);

    setWaitCursor();
    emit LscChanged( createLscVector() );
    setNormalCursor();
}
