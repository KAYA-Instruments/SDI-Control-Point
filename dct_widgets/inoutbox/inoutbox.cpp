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

#include "inoutbox.h"
#include "ui_inoutbox.h"
#include "defines.h"
#include <QTime>

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_InOutBox;
}

#define AUTO_REPEAT_THRESHOLD    ( 5000 )

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
    SpinBoxStyle( QStyle *style = 0 ) : QProxyStyle(style) { }

    int styleHint
    (
        StyleHint hint,
        const QStyleOption * option = 0,
        const QWidget * widget = 0,
        QStyleHintReturn * returnData = 0
    ) const
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
#define INOUT_SETTINGS_CAMERA_GAIN                  ( "iso" )
#define INOUT_SETTINGS_CAMERA_EXPOSURE              ( "shutter" )

#define INOUT_SETTINGS_AEC_ENABLE                   ( "aec_enable" )
#define INOUT_SETTINGS_AEC_SETPOINT                 ( "aec_setpoint" )
#define INOUT_SETTINGS_AEC_SPEED                    ( "aec_speed" )
#define INOUT_SETTINGS_AEC_FLICKER                  ( "aec_flicker" )

#define INOUT_SETTINGS_LSC_ENABLE                   ( "lsc_enable" )
#define INOUT_SETTINGS_LSC_SEGMENTATION_MODE        ( "lsc_segmentation_mode" )
#define INOUT_SETTINGS_LSC_K                        ( "lsc_k" )
#define INOUT_SETTINGS_LSC_OFFSET                   ( "lsc_offset" )
#define INOUT_SETTINGS_LSC_SLOPE                    ( "lsc_slope" )

#define INOUT_SETTINGS_VIDEO_MODE                   ( "video_mode" )
#define INOUT_SETTINGS_SDI2_MODE                    ( "sdi2_mode" )
#define INOUT_SETTINGS_FLIP_MODE                    ( "flip_mode" )
#define INOUT_SETTINGS_TEST_PATTERN                 ( "test_pattern" )

#define INOUT_SETTINGS_GENLOCK_MODE                 ( "genlock_mode" )
#define INOUT_SETTINGS_GENLOCK_OFFSET_VERTICAL      ( "genlock_offset_vertical" )
#define INOUT_SETTINGS_GENLOCK_OFFSET_HORIZONTAL    ( "genlock_offset_horizontal" )
#define INOUT_SETTINGS_GENLOCK_TERMINATION          ( "genlock_termination" )

typedef struct aec_setup_t {
    int run;
    int setPoint;
    int speed;
    int ClmTolerance;
    int cost_gain;
    int cost_t_int;
    int cost_aperture;
    int t_af;
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
        , m_maxEvents( 10 )
        , m_sbxStyle( new SpinBoxStyle() )
    {
        // do nothing
        m_AptMin    = 125;
        m_AptMax    = 16000;
        m_AptEnable = true;
    };

    ~PrivateData()
    {
        delete m_ui;
        delete m_sbxStyle;
    };

    Ui::UI_InOutBox *   m_ui;           /**< ui handle */
    int                 m_cntEvents;    /**< ignore move-events if slider moving */
    int                 m_maxEvents;    /**< number of ignored move-events */
    SpinBoxStyle *      m_sbxStyle;     /**< proxy style to overrule not implemented spinbox properties */

    aec_setup_t         m_AecSetup;

    lsc_setup_t         m_LscSetup;

    int                 m_AptMin;
    int                 m_AptMax;
    bool                m_AptEnable;
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

    d_data->m_ui->sbxAnalogueGain->setRange( 100, 1600 );
    d_data->m_ui->sldAnalogueGain->setRange( 100, 1600 );
    
    d_data->m_ui->sbxExposure->setRange( 100, 20000 );
    d_data->m_ui->sldExposure->setRange( 100, 20000 );

    d_data->m_ui->sbxSetPoint->setRange( 50, 3000 );
    d_data->m_ui->sldSetPoint->setRange( 50, 3000 );
    
    d_data->m_ui->sbxControlSpeed->setRange( 3, 30 );
    d_data->m_ui->sldControlSpeed->setRange( 3, 30 );

    d_data->m_ui->sbxK->setRange( 0.0f, 2.0f );
    d_data->m_ui->sbxK->setSingleStep( 0.01f );
    d_data->m_ui->sbxK->setKeyboardTracking( false );
    d_data->m_ui->sldK->setRange( 0, 200 );
    d_data->m_ui->sldK->setPageStep( 10 );

    d_data->m_ui->sbxOffset->setRange( 0.0f, 1.0f );
    d_data->m_ui->sbxOffset->setSingleStep( 0.01f );
    d_data->m_ui->sbxOffset->setKeyboardTracking( false );
    d_data->m_ui->sldOffset->setRange( 0, 100 );
    d_data->m_ui->sldOffset->setPageStep( 5 );

    d_data->m_ui->sbxSlope->setRange( 0.0f, 2.0f );
    d_data->m_ui->sbxSlope->setSingleStep( 0.01f );
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

    // fill lens shading correction segmentation mode combo box
    for ( int i=LscSegmentationModeFirst; i<LscSegmentationMax; i++ )
    {
        addLscSegmentationMode( GetLscSegmentationModeName( (LscSegmentationMode)i ), i );
    }

    // fill bayer pattern combo box
    for ( int i=BayerPatternFirst; i<BayerPatternMax; i++ )
    {
        addBayerPattern( GetBayerPatternName( (enum BayerPattern)i ), i );
    }

    // fill sdi2-mode combo box
    for ( int i=Sdi2ModeFirst; i<Sdi2ModeMax; i++ )
    {
        addSdi2Mode( GetSdi2ModeName( (enum Sdi2Mode)i ), i );
    }

    // fill flip-mode combo box
    for ( int i=FlipModeFirst; i<FlipModeMax; i++ )
    {
        addFlipMode( GetFlipModeName( (enum FlipMode)i ), i );
    }

    // fill genlock-mode combo box
    for ( int i=GenLockModeFirst; i<GenLockModeMax; i++ )
    {
        addGenlockMode( GetGenlockModeName( (enum GenLockMode)i ), i );
    }

    // overrule auto-repeat threshold
    d_data->m_ui->sbxAnalogueGain           ->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxExposure               ->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxGenLockOffsetVertical  ->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxGenlockOffsetHorizontal->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxSetPoint               ->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxAperture               ->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxK                      ->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxOffset                 ->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxSlope                  ->setStyle( d_data->m_sbxStyle );

    // connect internal signals
    // bayer pattern
    connect( d_data->m_ui->cbxBayerPattern, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxBayerPatternChange(int)) );

    // gain
    connect( d_data->m_ui->sldAnalogueGain, SIGNAL(valueChanged(int)), this, SLOT(onSldAnalogueGainChange(int)) );
    connect( d_data->m_ui->sldAnalogueGain, SIGNAL(sliderReleased()), this, SLOT(onSldAnalogueGainReleased()) );
    connect( d_data->m_ui->sbxAnalogueGain, SIGNAL(valueChanged(int)), this, SLOT(onSbxAnalogueGainChange(int)) );

    // exposure
    connect( d_data->m_ui->sldExposure    , SIGNAL(valueChanged(int)), this, SLOT(onSldExposureChange(int)) );
    connect( d_data->m_ui->sldExposure    , SIGNAL(sliderReleased()), this, SLOT(onSldExposureReleased()) );
    connect( d_data->m_ui->sbxExposure    , SIGNAL(valueChanged(int)), this, SLOT(onSbxExposureChange(int)) );

    // video
    connect( d_data->m_ui->cbxVideoMode   , SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxVideoModeChange(int)) );
    connect( d_data->m_ui->cbxSdi2Mode    , SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxSdi2ModeChange(int)) );
    connect( d_data->m_ui->cbxFlipMode    , SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxFlipModeChange(int)) );
    connect( d_data->m_ui->cbxTestPattern , SIGNAL(stateChanged(int)), this, SLOT(onCbxTestPatternChange(int)) );
    
    // genlock
    connect( d_data->m_ui->cbxGenLockMode , SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxGenlockModeChange(int)) );
    connect( d_data->m_ui->sbxGenLockOffsetVertical, SIGNAL(valueChanged(int)), this, SLOT(onSbxGenlockOffsetVerticalChange(int)) );
    connect( d_data->m_ui->sbxGenlockOffsetHorizontal, SIGNAL(valueChanged(int)), this, SLOT(onSbxGenlockOffsetHorizontalChange(int)) );
    connect( d_data->m_ui->cbxGenLockTermination, SIGNAL(stateChanged(int)), this, SLOT(onCbxGenlockTerminationChange(int)) );

    // auto exposure
    connect( d_data->m_ui->cbxAecEnable   , SIGNAL(stateChanged(int)), this, SLOT(onCbxAecEnableChange(int)) );

    connect( d_data->m_ui->sldSetPoint    , SIGNAL(valueChanged(int)), this, SLOT(onSldSetPointChange(int)) );
    connect( d_data->m_ui->sldSetPoint    , SIGNAL(sliderReleased()), this, SLOT(onSldSetPointReleased()) );
    connect( d_data->m_ui->sbxSetPoint    , SIGNAL(valueChanged(int)), this, SLOT(onSbxSetPointChange(int)) );

    connect( d_data->m_ui->sldControlSpeed, SIGNAL(valueChanged(int)), this, SLOT(onSldControlSpeedChange(int)) );
    connect( d_data->m_ui->sldControlSpeed, SIGNAL(sliderReleased()), this, SLOT(onSldControlSpeedReleased()) );
    connect( d_data->m_ui->sbxControlSpeed, SIGNAL(valueChanged(int)), this, SLOT(onSbxControlSpeedChange(int)) );

    connect( d_data->m_ui->rdbTaf50Hz, SIGNAL( toggled(bool) ), this, SLOT(onTafToggle(bool)) );
    connect( d_data->m_ui->rdbTaf60Hz, SIGNAL( toggled(bool) ), this, SLOT(onTafToggle(bool)) );

    connect( d_data->m_ui->sbxAperture    , SIGNAL(valueChanged(int)), this, SLOT(onSbxIrisAptChange(int)) );
    connect( d_data->m_ui->sldAperture    , SIGNAL(valueChanged(int)), this, SLOT(onSldIrisAptChange(int)) );
    connect( d_data->m_ui->sldSetPoint    , SIGNAL(sliderReleased()), this, SLOT(onSldIrisAptReleased()) );

    // timecode
    connect( d_data->m_ui->btnSetTimecode , SIGNAL(clicked()), this, SLOT(onBtnTimecodeSetClicked()) );
    connect( d_data->m_ui->btnGetTimecode , SIGNAL(clicked()), this, SLOT(onBtnTimecodeGetClicked()) );

    // lense shading correction
    connect( d_data->m_ui->cbxLscEnable   , SIGNAL(stateChanged(int)), this, SLOT(onCbxLscEnableChange(int)) );

    connect( d_data->m_ui->cbxSegmentationMode, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxSegmentationModeChange(int)) );

    connect( d_data->m_ui->sbxK         , SIGNAL(valueChanged(double)), this, SLOT(onSbxKChange(double)) );
    connect( d_data->m_ui->sldK         , SIGNAL(valueChanged(int)), this, SLOT(onSldKChange(int)) );
    connect( d_data->m_ui->sldK         , SIGNAL(sliderReleased()), this, SLOT(onSldKReleased()) );

    connect( d_data->m_ui->sbxOffset    , SIGNAL(valueChanged(double)), this, SLOT(onSbxOffsetChange(double)) );
    connect( d_data->m_ui->sldOffset    , SIGNAL(valueChanged(int)), this, SLOT(onSldOffsetChange(int)) );
    connect( d_data->m_ui->sldOffset    , SIGNAL(sliderReleased()), this, SLOT(onSldOffsetReleased()) );

    connect( d_data->m_ui->sbxSlope     , SIGNAL(valueChanged(double)), this, SLOT(onSbxSlopeChange(double)) );
    connect( d_data->m_ui->sldSlope     , SIGNAL(valueChanged(int)), this, SLOT(onSldSlopeChange(int)) );
    connect( d_data->m_ui->sldSlope     , SIGNAL(sliderReleased()), this, SLOT(onSldSlopeReleased()) );

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
 * InOutBox::CameraGain
 *****************************************************************************/
int InOutBox::CameraGain() const
{
    return ( d_data->m_ui->sbxAnalogueGain->value() );
}

/******************************************************************************
 * InOutBox::setCameraGain
 *****************************************************************************/
void InOutBox::setCameraGain( const int value )
{
    d_data->m_ui->sldAnalogueGain->blockSignals( true );
    d_data->m_ui->sldAnalogueGain->setValue( value );
    d_data->m_ui->sldAnalogueGain->blockSignals( false );

    d_data->m_ui->sbxAnalogueGain->blockSignals( true );
    d_data->m_ui->sbxAnalogueGain->setValue( value );
    d_data->m_ui->sbxAnalogueGain->blockSignals( false );

    setWaitCursor();
    emit CameraGainChanged( value * 10 );
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
    return ( (bool)d_data->m_AecSetup.run );
}

/******************************************************************************
 * InOutBox::AecEnable
 *****************************************************************************/
void InOutBox::setAecEnable( const bool value )
{
    enableAecWidgets( value );
    enableCamConfWidgets( !value );
    
    d_data->m_ui->cbxAecEnable->blockSignals( true );
    d_data->m_ui->cbxAecEnable->setCheckState( value ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxAecEnable->blockSignals( false );

    d_data->m_AecSetup.run = (int)value;

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
    return ( d_data->m_AecSetup.t_af );
}

/******************************************************************************
 * InOutBox::setAecFlickerFrequency
 *****************************************************************************/
void InOutBox::setAecFlickerFrequency( const int value )
{
    if ( value == 8333 || value == 10000 )
    {
        d_data->m_AecSetup.t_af = value;
    }
    else
    {
        d_data->m_AecSetup.t_af = 10000;
    }

    d_data->m_ui->rdbTaf50Hz->blockSignals( true );
    d_data->m_ui->rdbTaf50Hz->setChecked( d_data->m_AecSetup.t_af == 10000 );
    d_data->m_ui->rdbTaf50Hz->blockSignals( false );
    
    d_data->m_ui->rdbTaf60Hz->blockSignals( true );
    d_data->m_ui->rdbTaf60Hz->setChecked( d_data->m_AecSetup.t_af == 8333 );
    d_data->m_ui->rdbTaf60Hz->blockSignals( false );

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
 * InOutBox::LscMode
 *****************************************************************************/
LscSegmentationMode InOutBox::LscMode() const
{
    return ( d_data->m_LscSetup.mode );
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
 * InOutBox::VideoMode
 *****************************************************************************/
void InOutBox::setVideoMode( const QString mode )
{
    int index = d_data->m_ui->cbxVideoMode->findText( mode );
    if ( index != -1 )
    {
        d_data->m_ui->cbxVideoMode->blockSignals( true );
        d_data->m_ui->cbxVideoMode->setCurrentIndex( index );
        d_data->m_ui->cbxVideoMode->blockSignals( false );

        emit ChainVideoModeChanged( d_data->m_ui->cbxVideoMode->itemData( index ).toInt() );
    }
}

/******************************************************************************
 * InOutBox::VideoMode
 *****************************************************************************/
QString InOutBox::Sdi2Mode() const
{
    return ( d_data->m_ui->cbxSdi2Mode->currentText() );
}

/******************************************************************************
 * InOutBox::VideoMode
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
 * InOutBox::VideoMode
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
    d_data->m_ui->cbxTestPattern->setChecked( value ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxTestPattern->blockSignals( false );

    emit OsdTestPatternChanged( value ? 1 : 0 );
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
    d_data->m_ui->cbxGenLockTermination->setChecked( value ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxGenLockTermination->blockSignals( false );

    emit ChainGenlockTerminationChanged( value ? 1 : 0 );
}

/******************************************************************************
 * InOutBox::prepareMode
 *****************************************************************************/
void InOutBox::prepareMode( const Mode mode )
{
    bool v = (DctWidgetBox::Normal == mode) ? false : true;
    d_data->m_ui->lblBayerPattern->setVisible( v );
    d_data->m_ui->cbxBayerPattern->setVisible( v );
}

/******************************************************************************
 * InOutBox::loadSettings
 *****************************************************************************/
void InOutBox::loadSettings( QSettings & s )
{
    s.beginGroup( INOUT_SETTINGS_SECTION_NAME );
    
    // Note: Set video mode first, otherwise gain and exposure will not be correctly set
    setVideoMode( s.value( INOUT_SETTINGS_VIDEO_MODE ).toString() );
    setSdi2Mode( s.value( INOUT_SETTINGS_SDI2_MODE ).toString() );
    setFlipMode( s.value( INOUT_SETTINGS_FLIP_MODE ).toString() );
    setTestPattern( s.value( INOUT_SETTINGS_TEST_PATTERN ).toBool() );

    setBayerPattern( s.value( INOUT_SETTINGS_BAYER_PATTERN ).toInt() );
    setCameraGain( s.value( INOUT_SETTINGS_CAMERA_GAIN ).toInt() );
    setCameraExposure( s.value( INOUT_SETTINGS_CAMERA_EXPOSURE ).toInt() );

    setAecSetPoint( s.value( INOUT_SETTINGS_AEC_SETPOINT ).toInt() );
    setAecControlSpeed( s.value( INOUT_SETTINGS_AEC_SPEED ).toInt() );
    setAecFlickerFrequency( s.value( INOUT_SETTINGS_AEC_FLICKER ).toInt() );
    setAecEnable( s.value( INOUT_SETTINGS_AEC_ENABLE ).toBool() );

    lsc_setup_t lscSetup;
    lscSetup.enable = s.value( INOUT_SETTINGS_LSC_ENABLE ).toBool();
    lscSetup.mode = (LscSegmentationMode)s.value( INOUT_SETTINGS_LSC_SEGMENTATION_MODE ).toUInt();
    lscSetup.k = s.value( INOUT_SETTINGS_LSC_K ).toUInt();
    lscSetup.offset = s.value( INOUT_SETTINGS_LSC_OFFSET ).toUInt();
    lscSetup.slope = s.value( INOUT_SETTINGS_LSC_SLOPE ).toUInt();

    setLsc( lscSetup );

    setGenLockMode( s.value( INOUT_SETTINGS_GENLOCK_MODE ).toString() );
    setGenLockOffsetVertical( s.value( INOUT_SETTINGS_GENLOCK_OFFSET_VERTICAL ).toInt() );
    setGenLockOffsetHorizontal( s.value( INOUT_SETTINGS_GENLOCK_OFFSET_HORIZONTAL ).toInt() );
    setGenLockTermination( s.value( INOUT_SETTINGS_GENLOCK_TERMINATION ).toBool() );

    s.endGroup();
}

/******************************************************************************
 * InOutBox::saveSettings
 *****************************************************************************/
void InOutBox::saveSettings( QSettings & s )
{
    s.beginGroup( INOUT_SETTINGS_SECTION_NAME );

    s.setValue( INOUT_SETTINGS_BAYER_PATTERN                , BayerPattern() );
    s.setValue( INOUT_SETTINGS_CAMERA_GAIN                  , CameraGain() );
    s.setValue( INOUT_SETTINGS_CAMERA_EXPOSURE              , CameraExposure() );
    
    s.setValue( INOUT_SETTINGS_AEC_ENABLE                   , AecEnable() );
    s.setValue( INOUT_SETTINGS_AEC_SETPOINT                 , AecSetPoint() );
    s.setValue( INOUT_SETTINGS_AEC_SPEED                    , AecControlSpeed() );
    s.setValue( INOUT_SETTINGS_AEC_FLICKER                  , AecFlickerFrequency() );

    s.setValue ( INOUT_SETTINGS_LSC_ENABLE                  , LscEnable() );
    s.setValue ( INOUT_SETTINGS_LSC_SEGMENTATION_MODE       , LscMode() );
    s.setValue ( INOUT_SETTINGS_LSC_K                       , LscK() );
    s.setValue ( INOUT_SETTINGS_LSC_OFFSET                  , LscOffset() );
    s.setValue ( INOUT_SETTINGS_LSC_SLOPE                   , LscSlope() );

    s.setValue( INOUT_SETTINGS_VIDEO_MODE                   , VideoMode() );
    s.setValue( INOUT_SETTINGS_SDI2_MODE                    , Sdi2Mode() );
    s.setValue( INOUT_SETTINGS_FLIP_MODE                    , FlipMode() );
    s.setValue( INOUT_SETTINGS_TEST_PATTERN                 , TestPattern() );

    s.setValue( INOUT_SETTINGS_GENLOCK_MODE                 , GenLockMode() );
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
    // Note: Set video mode first, otherwise gain and exposure will not be correctly set
    emit ChainVideoModeChanged( d_data->m_ui->cbxVideoMode->currentData().toInt() );
    emit ChainSdi2ModeChanged( d_data->m_ui->cbxSdi2Mode->currentData().toInt() );
    emit ChainFlipModeChanged( d_data->m_ui->cbxFlipMode->currentData().toInt() );
    emit OsdTestPatternChanged( TestPattern() );

    emit BayerPatternChanged( BayerPattern() );
    emit CameraGainChanged( CameraGain() * 10 );
    emit CameraExposureChanged( CameraExposure() );

    emit AecSetupChanged( createAecVector() );

    emit ChainGenlockModeChanged( d_data->m_ui->cbxGenLockMode->currentData().toInt() );
    emit ChainGenlockOffsetChanged( GenLockOffsetVertical(), GenLockOffsetHorizontal() );
    emit ChainGenlockTerminationChanged( GenLockTermination() );
}

/******************************************************************************
 * InOutBox::addLscSegmentationMode
 *****************************************************************************/
void InOutBox::addLscSegmentationMode( QString name, int id )
{
    d_data->m_ui->cbxSegmentationMode->blockSignals( true );
    d_data->m_ui->cbxSegmentationMode->addItem( name, id );
    d_data->m_ui->cbxSegmentationMode->blockSignals( false );
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
 * InOutBox::clearAllVideoModes
 *****************************************************************************/
void InOutBox::clearAllVideoModes()
{
    d_data->m_ui->cbxVideoMode->blockSignals( true );
    d_data->m_ui->cbxVideoMode->clear();
    d_data->m_ui->cbxVideoMode->blockSignals( false );
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
 * InOutBox::addGenlockMode
 *****************************************************************************/
void InOutBox::addGenlockMode( QString name, int id )
{
    d_data->m_ui->cbxGenLockMode->blockSignals( true );
    d_data->m_ui->cbxGenLockMode->addItem( name, id );
    d_data->m_ui->cbxGenLockMode->blockSignals( false );
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
 * InOutBox::addFlipMode
 *****************************************************************************/
void InOutBox::addFlipMode( QString name, int id )
{
    d_data->m_ui->cbxFlipMode->blockSignals( true );
    d_data->m_ui->cbxFlipMode->addItem( name, id );
    d_data->m_ui->cbxFlipMode->blockSignals( false );
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
void InOutBox::setTimeCodeVisible(const bool value)
{
    d_data->m_ui->gbxTimeCodeSettings->setVisible(value);
}

/******************************************************************************
 * InOutBox::setTimeCodeVisible
 *****************************************************************************/
void InOutBox::setSdi2ModeVisible(const bool value)
{
    d_data->m_ui->lblSdi2Mode->setVisible(value);
    d_data->m_ui->cbxSdi2Mode->setVisible(value);
}

/******************************************************************************
 * InOutBox::setFlipModeVisible
 *****************************************************************************/
void InOutBox::setFlipModeVisible(const bool value)
{
    d_data->m_ui->lblFlip->setVisible(value);
    d_data->m_ui->cbxFlipMode->setVisible(value);
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
void InOutBox::onCameraInfoChange( int min_gain, int max_gain, int min_exposure, int max_exposure )
{
    d_data->m_ui->sbxAnalogueGain->blockSignals( true );
    d_data->m_ui->sbxAnalogueGain->setRange( min_gain / 10, max_gain / 10 );
    d_data->m_ui->sldAnalogueGain->setRange( min_gain / 10, max_gain / 10 );
    d_data->m_ui->sbxAnalogueGain->blockSignals( false );

    d_data->m_ui->sbxExposure->blockSignals( true );
    d_data->m_ui->sbxExposure->setRange( min_exposure, max_exposure );
    d_data->m_ui->sldExposure->setRange( min_exposure, max_exposure );
    d_data->m_ui->sbxExposure->blockSignals( false );
}

/******************************************************************************
 * InOutBox::onCameraGainChange
 *****************************************************************************/
void InOutBox::onCameraGainChange( int value )
{
    d_data->m_ui->sbxAnalogueGain->blockSignals( true );
    d_data->m_ui->sbxAnalogueGain->setValue( value / 10 );
    d_data->m_ui->sbxAnalogueGain->blockSignals( false );

    d_data->m_ui->sldAnalogueGain->blockSignals( true );
    d_data->m_ui->sldAnalogueGain->setValue( value / 10 );
    d_data->m_ui->sldAnalogueGain->blockSignals( false );
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
 * InOutBox::onCbxBayerPatternChange
 *****************************************************************************/
void InOutBox::onCbxBayerPatternChange( int index )
{
    setWaitCursor();
    emit BayerPatternChanged( d_data->m_ui->cbxBayerPattern->itemData( index ).toInt() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSldAnalogueGainChange
 *****************************************************************************/
void InOutBox::onSldAnalogueGainChange( int value )
{
    d_data->m_ui->sbxAnalogueGain->blockSignals( true );
    d_data->m_ui->sbxAnalogueGain->setValue( value );
    d_data->m_ui->sbxAnalogueGain->blockSignals( false );

    if ( (d_data->m_ui->sldAnalogueGain->isSliderDown()  ) &&
         (d_data->m_cntEvents++ > d_data->m_maxEvents) )
    {
        d_data->m_cntEvents = 0;
        
        setWaitCursor();
        emit CameraGainChanged( value * 10 );
        setNormalCursor();
    }
}

/******************************************************************************
 * InOutBox::onSldAnalogueGainReleased
 *****************************************************************************/
void InOutBox::onSldAnalogueGainReleased()
{
    d_data->m_cntEvents = 0;
        
    setWaitCursor();
    emit CameraGainChanged( d_data->m_ui->sldAnalogueGain->value() * 10 );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxAnalogueGainChange
 *****************************************************************************/
void InOutBox::onSbxAnalogueGainChange( int value )
{
    d_data->m_ui->sldAnalogueGain->blockSignals( true );
    d_data->m_ui->sldAnalogueGain->setValue( value );
    d_data->m_ui->sldAnalogueGain->blockSignals( false );

    setWaitCursor();
    emit CameraGainChanged( value * 10 );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSldExposureChange
 *****************************************************************************/
void InOutBox::onSldExposureChange( int value )
{
    d_data->m_ui->sbxExposure->blockSignals( true );
    d_data->m_ui->sbxExposure->setValue( value );
    d_data->m_ui->sbxExposure->blockSignals( false );

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
    
    setWaitCursor();
    emit CameraExposureChanged( d_data->m_ui->sldExposure->value() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSbxExposureChange
 *****************************************************************************/
void InOutBox::onSbxExposureChange( int value )
{
    d_data->m_ui->sldExposure->blockSignals( true );
    d_data->m_ui->sldExposure->setValue( value );
    d_data->m_ui->sldExposure->blockSignals( false );

    setWaitCursor();
    emit CameraExposureChanged( value );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxVideoModeChange
 *****************************************************************************/
void InOutBox::onCbxVideoModeChange( int index )
{
    setWaitCursor();
    emit ChainVideoModeChanged( d_data->m_ui->cbxVideoMode->itemData( index ).toInt() );
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
 * InOutBox::onCbxFlipModeChange
 *****************************************************************************/
void InOutBox::onCbxFlipModeChange( int index )
{
    setWaitCursor();
    emit ChainFlipModeChanged( d_data->m_ui->cbxFlipMode->itemData( index ).toInt() );
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
 * InOutBox::onCbxGenlockModeChange
 *****************************************************************************/
void InOutBox::onCbxGenlockModeChange( int index )
{
    setWaitCursor();
    emit ChainGenlockModeChanged( d_data->m_ui->cbxGenLockMode->itemData( index ).toInt() );
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
    setWaitCursor();
    emit ChainGenlockTerminationChanged( (value == Qt::Checked) ? 1 : 0 );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxAecEnableChange
 *****************************************************************************/
void InOutBox::onCbxAecEnableChange( int value )
{
    bool enable = (value == Qt::Checked) ? true : false;

    enableAecWidgets( enable );
    enableCamConfWidgets( !enable );

    d_data->m_AecSetup.run = (int)enable;

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
 * InOutBox::onAecEnableChange
 *****************************************************************************/
void InOutBox::onAecEnableChange( int enable )
{
    enableAecWidgets( enable ? true : false );
    enableCamConfWidgets( enable ? false : true );

    d_data->m_ui->cbxAecEnable->blockSignals( true );
    d_data->m_ui->cbxAecEnable->setCheckState( enable ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxAecEnable->blockSignals( false );
}

/******************************************************************************
 * InOutBox::onAecSetupChange
 *****************************************************************************/
void InOutBox::onAecSetupChange( QVector<int> values )
{
    if ( values.count() == 8 )
    {
        d_data->m_AecSetup.run           = values[0];
        d_data->m_AecSetup.setPoint      = values[1];
        d_data->m_AecSetup.speed         = values[2];
        d_data->m_AecSetup.ClmTolerance  = values[3];
        d_data->m_AecSetup.cost_gain     = values[4];
        d_data->m_AecSetup.cost_t_int    = values[5];
        d_data->m_AecSetup.cost_aperture = values[6];
        d_data->m_AecSetup.t_af          = values[7];

        updateAecSetupWidgets();
    }
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
 * InOutBox::updateAecSetupWidgets
 *****************************************************************************/
void InOutBox::updateAecSetupWidgets( void )
{
    // Set point
    d_data->m_ui->sbxSetPoint->blockSignals( true );
    d_data->m_ui->sbxSetPoint->setValue( d_data->m_AecSetup.setPoint );
    d_data->m_ui->sbxSetPoint->blockSignals( false );
    d_data->m_ui->sldSetPoint->blockSignals( true );
    d_data->m_ui->sldSetPoint->setValue( d_data->m_AecSetup.setPoint );
    d_data->m_ui->sldSetPoint->blockSignals( false );

    // Control Speed
    d_data->m_ui->sbxControlSpeed->blockSignals( true );
    d_data->m_ui->sbxControlSpeed->setValue( d_data->m_AecSetup.speed );
    d_data->m_ui->sbxControlSpeed->blockSignals( false );
    d_data->m_ui->sldControlSpeed->blockSignals( true );
    d_data->m_ui->sldControlSpeed->setValue( d_data->m_AecSetup.speed );
    d_data->m_ui->sldControlSpeed->blockSignals( false );

    // flicker
    d_data->m_ui->rdbTaf50Hz->blockSignals( true );
    d_data->m_ui->rdbTaf50Hz->setChecked( d_data->m_AecSetup.t_af == 10000 );
    d_data->m_ui->rdbTaf50Hz->blockSignals( false );

    d_data->m_ui->rdbTaf60Hz->blockSignals( true );
    d_data->m_ui->rdbTaf60Hz->setChecked( d_data->m_AecSetup.t_af == 8333 );
    d_data->m_ui->rdbTaf60Hz->blockSignals( false );
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

    // mode
    int index = d_data->m_ui->cbxSegmentationMode->findData( d_data->m_LscSetup.mode );
    if ( index != -1 )
    {
        d_data->m_ui->cbxSegmentationMode->blockSignals( true );
        d_data->m_ui->cbxSegmentationMode->setCurrentIndex( index );
        d_data->m_ui->cbxSegmentationMode->blockSignals( false );
    }

    // k
    d_data->m_ui->sbxK->blockSignals( true );
    d_data->m_ui->sbxK->setValue( d_data->m_LscSetup.k );
    d_data->m_ui->sbxK->blockSignals( false );
    d_data->m_ui->sldK->blockSignals( true );
    d_data->m_ui->sldK->setValue( (int)(d_data->m_LscSetup.k * 100.0f) );
    d_data->m_ui->sldK->blockSignals( false );

    // offset
    d_data->m_ui->sbxOffset->blockSignals( true );
    d_data->m_ui->sbxOffset->setValue( d_data->m_LscSetup.offset );
    d_data->m_ui->sbxOffset->blockSignals( false );
    d_data->m_ui->sldOffset->blockSignals( true );
    d_data->m_ui->sldOffset->setValue( (int)(d_data->m_LscSetup.offset * 100.0f) );
    d_data->m_ui->sldOffset->blockSignals( false );

    // slope
    d_data->m_ui->sbxSlope->blockSignals( true );
    d_data->m_ui->sbxSlope->setValue( d_data->m_LscSetup.slope );
    d_data->m_ui->sbxSlope->blockSignals( false );
    d_data->m_ui->sldSlope->blockSignals( true );
    d_data->m_ui->sldSlope->setValue( (int)(d_data->m_LscSetup.slope * 100.0f) );
    d_data->m_ui->sldSlope->blockSignals( false );
}

/******************************************************************************
 * InOutBox::enableAecWidgets
 *****************************************************************************/
void InOutBox::enableAecWidgets( bool enable )
{
    d_data->m_ui->sbxSetPoint->setEnabled( enable );
    d_data->m_ui->sldSetPoint->setEnabled( enable );
    d_data->m_ui->sbxControlSpeed->setEnabled( enable );
    d_data->m_ui->sldControlSpeed->setEnabled( enable );
    d_data->m_ui->rdbTaf50Hz->setEnabled( enable );
    d_data->m_ui->rdbTaf60Hz->setEnabled( enable );
}

/******************************************************************************
 * InOutBox::enableCamConfWidgets
 *****************************************************************************/
void InOutBox::enableCamConfWidgets( bool enable )
{
    d_data->m_ui->sbxAnalogueGain->setEnabled( enable );
    d_data->m_ui->sldAnalogueGain->setEnabled( enable );
    
    d_data->m_ui->sbxExposure->setEnabled( enable );
    d_data->m_ui->sldExposure->setEnabled( enable );

    d_data->m_ui->sbxAperture->setEnabled( enable && d_data->m_AptEnable );
    d_data->m_ui->sldAperture->setEnabled( enable && d_data->m_AptEnable );
}

/******************************************************************************
 * InOutBox::createAecVector
 *****************************************************************************/
QVector<int> InOutBox::createAecVector( void )
{
    // pack aec setup values into vector
    QVector<int> values(8);
    values[0] = d_data->m_AecSetup.run;
    values[1] = d_data->m_AecSetup.setPoint;
    values[2] = d_data->m_AecSetup.speed;
    values[3] = d_data->m_AecSetup.ClmTolerance;
    values[4] = d_data->m_AecSetup.cost_gain;
    values[5] = d_data->m_AecSetup.cost_t_int;
    values[6] = d_data->m_AecSetup.cost_aperture;
    values[7] = d_data->m_AecSetup.t_af;
    return ( values );
}

/******************************************************************************
 * InOutBox::createLscVector
 *****************************************************************************/
QVector<uint> InOutBox::createLscVector( void )
{
    // pack LSC setup values into vector
    QVector<uint> values(5);
    values[0] = d_data->m_LscSetup.enable;
    values[1] = d_data->m_LscSetup.mode;

    /* The parameters k, offset and slope have to be converted from
     * float to Q2.30 fixed point format */
    values[2] = (uint)(d_data->m_LscSetup.k * (float)(1u << 30u));
    values[3] = (uint)(d_data->m_LscSetup.offset * (float)(1u << 30u));
    values[4] = (uint)(d_data->m_LscSetup.slope * (float)(1u << 30u));
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
    emit TimecodeSet( v_time );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onBtnTimecodeGetClicked
 *****************************************************************************/
void InOutBox::onBtnTimecodeGetClicked()
{
    setWaitCursor();
    emit TimecodeGet();
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onTimecodeChange
 *****************************************************************************/
void InOutBox::onTimecodeChange( QVector<int> time )
{
    d_data->m_ui->tmeTimecode->setTime( QTime(time[0], time[1], time[2]) );
}

/******************************************************************************
 * InOutBox::onTafToggle
 *****************************************************************************/
void InOutBox::onTafToggle( bool )
{
    if( d_data->m_ui->rdbTaf50Hz->isChecked() )
    {
        // 50 Hz
        d_data->m_AecSetup.t_af = 10000;
    }
    else
    {
        // 60 Hz
        d_data->m_AecSetup.t_af = 8333;
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
    d_data->m_ui->sbxAperture->setEnabled( false );
    d_data->m_ui->sldAperture->setEnabled( false );

    d_data->m_AptEnable = false;
}

/******************************************************************************
 * InOutBox::onLscChange
 *****************************************************************************/
void InOutBox::onLscChange( QVector<uint> values )
{
    if ( values.count() == 5 )
    {
        d_data->m_LscSetup.enable = (bool)values[0];
        d_data->m_LscSetup.mode   = (LscSegmentationMode)values[1];

        /* The parameters k, offset and slope have to be converted from
         * Q2.30 fixed point format to float */
        d_data->m_LscSetup.k      = (float)values[2] / (float)(1u << 30u);
        d_data->m_LscSetup.offset = (float)values[3] / (float)(1u << 30u);
        d_data->m_LscSetup.slope  = (float)values[4] / (float)(1u << 30u);

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

    setWaitCursor();
    emit LscChanged( createLscVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onCbxSegmentationModeChange
 *****************************************************************************/
void InOutBox::onCbxSegmentationModeChange( int index )
{
    d_data->m_LscSetup.mode = (LscSegmentationMode)d_data->m_ui->cbxSegmentationMode->itemData( index ).toUInt();

    setWaitCursor();
    emit LscChanged( createLscVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSldKChange
 *****************************************************************************/
void InOutBox::onSldKChange( int value )
{
    float sbxValue = (float)value / 100.0f;

    d_data->m_ui->sbxK->blockSignals( true );
    d_data->m_ui->sbxK->setValue( sbxValue );
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
    int sldValue = (int)(value * 100.0f);

    d_data->m_ui->sldK->blockSignals( true );
    d_data->m_ui->sldK->setValue( sldValue );
    d_data->m_ui->sldK->blockSignals( false );

    d_data->m_LscSetup.k = value;

    setWaitCursor();
    emit LscChanged( createLscVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSldOffsetChange
 *****************************************************************************/
void InOutBox::onSldOffsetChange( int value )
{
    float sbxValue = (float)value / 100.0f;

    d_data->m_ui->sbxOffset->blockSignals( true );
    d_data->m_ui->sbxOffset->setValue( sbxValue );
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
    int sldValue = (int)(value * 100.0f);

    d_data->m_ui->sldOffset->blockSignals( true );
    d_data->m_ui->sldOffset->setValue( sldValue );
    d_data->m_ui->sldOffset->blockSignals( false );

    d_data->m_LscSetup.offset = value;

    setWaitCursor();
    emit LscChanged( createLscVector() );
    setNormalCursor();
}

/******************************************************************************
 * InOutBox::onSldSlopeChange
 *****************************************************************************/
void InOutBox::onSldSlopeChange( int value )
{
    float sbxValue = (float)value / 100.0f;

    d_data->m_ui->sbxSlope->blockSignals( true );
    d_data->m_ui->sbxSlope->setValue( sbxValue );
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
    int sldValue = (int)(value * 100.0f);

    d_data->m_ui->sldSlope->blockSignals( true );
    d_data->m_ui->sldSlope->setValue( sldValue );
    d_data->m_ui->sldSlope->blockSignals( false );

    d_data->m_LscSetup.slope = value;

    setWaitCursor();
    emit LscChanged( createLscVector() );
    setNormalCursor();
}
