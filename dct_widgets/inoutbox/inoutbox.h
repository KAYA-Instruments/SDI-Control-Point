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
 * @file    inoutbox.h
 *
 * @brief   Class definition of input/output box 
 *
 *****************************************************************************/
#ifndef __IN_OUT_BOX_H__
#define __IN_OUT_BOX_H__

#include <dct_widgets_base.h>
#include "defines.h"

/******************************************************************************
 * global definitions
 *****************************************************************************/
typedef struct lsc_setup_t {
    bool                enable;
    float               k;
    float               offset;
    float               slope;
} lsc_setup_t;

/******************************************************************************
 * In-/Output Box Widget
 *****************************************************************************/
class InOutBox : public DctWidgetBox
{
    Q_OBJECT

public:
    explicit InOutBox( QWidget * parent = nullptr );
    ~InOutBox() Q_DECL_OVERRIDE;

    int BayerPattern() const;
    void setBayerPattern( const int );
    int CameraIso() const;
    void setCameraIso( const int value );
    int CameraExposure() const;
    void setCameraExposure( const int value );

    bool AecEnable() const;
    void setAecEnable( const bool value );
    int AecSetPoint() const;
    void setAecSetPoint( const int value );
    int AecMaxIso() const;
    void setAecMaxIso( const int value );
    int AecControlSpeed() const;
    void setAecControlSpeed( const int value );
    int AecFlickerFrequency() const;
    void setAecFlickerFrequency( const int value );
    bool AecCustomWeights() const;
    void setAecCustomWeights( const bool value );

    bool LscEnable() const;
    float LscK() const;
    float LscOffset() const;
    float LscSlope() const;
    void setLsc(lsc_setup_t setup );

    QString VideoMode() const;
    void setVideoMode( const QString mode );
    QString Sdi2Mode() const;
    void setSdi2Mode( const QString mode );
    QString Sdi1Downscaler() const;
    void setSdi1Downscaler( const QString mode );
    QString Sdi2Downscaler() const;
    void setSdi2Downscaler( const QString mode );
    QString FlipMode() const;
    void setFlipMode( const QString mode );
    QString LogMode() const;
    void setLogMode( const QString mode);
    int PQMaxBrightness() const;
    void setPQMaxBrightness( const int value );
    QString ColorSpace() const;
    void setColorSpace( const QString mode);
    bool TestPattern() const;
    void setTestPattern( const bool value );
    bool AudioEnable() const;
    void setAudioEnable( const bool value );
    double AudioGain() const;
    void setAudioGain( const double gain );

    QString GenLockMode() const;
    void setGenLockMode( const QString mode );
    QString GenLockCrosslockEnable() const;
    QString GenLockCrosslockVmode() const;
    void setGenLockCrosslock( const QString enable, const QString vmode );
    int GenLockOffsetVertical() const;
    void setGenLockOffsetVertical( const int value );
    int GenLockOffsetHorizontal() const;
    void setGenLockOffsetHorizontal( const int value );
    bool GenLockTermination() const;
    void setGenLockTermination( const bool value );
    void setGenLockLOLFilter( const int value );

    // Show or hide UI elements
    void setCameraSettingsVisible(const bool value);
    void setAutoExposureSettingsVisible(const bool value);
    void setLensChadingCorrectionSettingsVisible(const bool value);
    void setApartureVisible(const bool value);
    void setGenLockVisible(const bool value);
    void setTimeCodeVisible(const bool groupbox_visible, const bool hold_visible);
    void setSdi2ModeVisible(const bool value);
    void setDownscaleModeVisible(const bool value);
    void setFlipModeVisible(const bool vertical, const bool horizontal);
    void setLogModeVisible(const bool value);
    void setTestPatternVisible(const bool value);
    void setAudioVisible(const bool value);

    void addVideoMode( QString name, int id );
    void clearAllVideoModes();

    void addGenlockCrosslockVideoMode( QString name, int id );
    void clearAllGenlockCrosslockVideoModes();

protected:
    void prepareMode( const Mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

    bool eventFilter(QObject *obj, QEvent *evt) Q_DECL_OVERRIDE;

    void addBayerPattern( QString name, int id );
    void addGenlockMode( QString name, int id );
    void addGenlockCrosslockEnable( QString name, int id);
    void addSdi2Mode( QString name, int id );
    void addDownscaleMode( QString name, int id );
    void addFlipMode( QString name, int id );
    void addLogMode( QString name, int id );
    void addColorSpace( QString name, int id );

    void UpdateIsoPlusMinusButtons();
    void UpdateExposurePlusMinusButtons();
    void UpdateIsoComboBox( int iso );
    void UpdateExposureComboBox( int exposure );

    void updateAecSetupWidgets( void );
    void updateLscWidgets( void );
    void enableAecWidgets( bool enable );
    void enableCamConfWidgets( bool enable );
    void enableGainConfWidgets( bool enable );
    void enableMaxExposureWidgets( bool enable );
    void enableMaxAnalogGainWidgets( bool enable );

    void updateEnableOfGenlockSettings( int genlockMode, int crosslockMode );

    QVector<int> createAecVector( void );
    QVector<uint> createLscVector( void );

signals:
    void LscChanged( QVector<uint> values );

    void BayerPatternChanged( int value );

    void CameraGainChanged( int value );
    void CameraExposureChanged( int value );

    void CameraRoiOffsetChanged(int offset_x, int offset_y);
    void CameraDownscalerChange();

    void ChainVideoModeChanged( int value );
    void ChainSdi2ModeChanged( int value );
    void ChainDownscaleModeChanged( int sdi_out_idx, bool downscale, bool interlace );
    void ChainFlipModeChanged( int value );
    void LogModeChanged( int value );
    void PQMaxBrightnessChanged( int value );
    void ColorSpaceChanged( int value );
    void OsdTestPatternChanged( int value );
    void ChainAudioEnableChanged( bool enable );
    void ChainAudioGainChanged( double gain );
    
    void ChainGenlockModeChanged( int value );
    void ChainGenLockStatusRefresh();
    void ChainGenlockCrosslockChanged( int enable, int vmode );
    void ChainGenlockOffsetChanged( int vertical, int horizontal );
    void ChainGenlockTerminationChanged( int value );
    void ChainGenlockLOLFilterChanged( int value );

    void AecEnableChanged( int value );
    void AecSetupChanged( QVector<int> values );
    void AecWeightChanged( int index, int weight );
    void WeightDialogAecWeightsChanged( QVector<int> weights );

    void ChainTimecodeGetRequested( void );
    void ChainTimecodeSetChanged( QVector<int> );
    void ChainTimecodeHoldChanged( bool );

    void IrisAptChanged( int );

    void ResyncRequest( void );

    void ROIVideoModeChanged();

public slots:
    void onBayerPatternChange( int value );

    void onCameraInfoChange( int min_gain, int max_gain, int min_exposure, int max_exposure , int min_iso );
    void onCameraGainChange( int value );
    void onCameraExposureChange( int value );
    void onCameraRoiOffsetInfoChange( int offset_x_max, int offset_y_max, int offset_x_step, int offset_y_step );
    void onCameraRoiOffsetChange( int offset_x, int offset_y );

    void onChainVideoModeChange( int value );
    void onChainSdi2ModeChange( int value );
    void onChainDownscaleModeChange( int sdi_out_idx, bool downscale, bool interlace );
    void onChainFlipModeChange( int value );
    void onLogModeChange( int value );
    void onPQMaxBrightnessChange( int percent );
    void onColorSpaceChange( int value );
    void onOsdTestPatternChange( int value );
    void onChainAudioEnableChange( bool enable );
    void onChainAudioGainChange( double gain );

    void onChainGenlockModeChange( int value );
    void onChainGenlockStatusChange( int value );
    void onChainGenlockCrosslockChange( int enable, int vmode );
    void onChainGenlockOffsetChange( int vertical , int horizontal );
    void onChainGenlockOffsetMaxChange( int verticalMax, int horizontalMax );
    void onChainGenlockTerminationChange( int value );
    void onChainGenlockLOLFilterChange( int value );

    void onAecEnableChange( int enable );
    void onAecSetupChange( QVector<int> values );
    void onAecWeightsChange( QVector<int> weights );

    void onAecStatChange( QVector<int> values );

    void onLscChange( QVector<uint> values );

    void onChainTimecodeChange( QVector<int> time );
    void onChainTimecodeHoldChange( bool enable );

    void onIrisAptChange( int value );
    void onIrisAptError( void );
    void onIrisSetupChange( QVector<int> values );

private slots:
    void onCbxBayerPatternChange( int value );

    void onSldIsoChange( int value );
    void onSldIsoReleased();
    void onSbxIsoChange( double value );
    void onCbxIsoChange( int index );
    void onBtnIsoMinusClicked( );
    void onBtnIsoPlusClicked( );

    void onSldExposureChange( int value );
    void onSldExposureReleased();
    void onSbxExposureChange( int value );
    void onCbxExposureChange( int index );
    void onBtnExposureMinusClicked( );
    void onBtnExposurePlusClicked( );

    void onSbxRoiOffsetXChange( int index );
    void onSbxRoiOffsetYChange( int index );
    void onSldRoiOffsetXChange( int index );
    void onSldRoiOffsetYChange( int index );

    void onCbxVideoModeChange( int index );
    void onCbxSdi2ModeChange( int index );
    void onCbxSdi1DownscalerChange( int index );
    void onCbxSdi2DownscalerChange( int index );
    void onCbxFlipModeChange( int index );
    void onCbxLogModeChange( int index );
    void onSbxPQMaxBrightnessChange( int value );
    void onCbxColorSpaceChange( int index );
    void onCbxTestPatternChange( int value );
    void onCbxAudioEnableChange( int value );
    void onSbxAudioGainChange( double gain );
    
    void onCbxGenlockModeChange( int index );
    void onBtnGenlockStatusRefresh();
    void onCbxGenlockCrosslockEnableChange( int index );
    void onCbxGenlockCrosslockVmodeChange( int index );
    void onSbxGenlockOffsetVerticalChange( int value );
    void onSbxGenlockOffsetHorizontalChange( int value );
    void onCbxGenlockTerminationChange( int value );
    void onSbxGenlockLOLFilterValueChange(int value);

    void onCbxAecEnableChange( int value );
    void onCbxMaxAnalogGainEnableChange( int value );
    void onCbxMaxExposureEnableChange( int value );
    void onCbxAecWeightChange( int value );
    
    void onSldSetPointChange( int value );
    void onSldSetPointReleased();
    void onSbxSetPointChange( int value );

    void onSldMaxAnalogGainChange( int value );
    void onSldMaxAnalogGainReleased();
    void onSbxMaxAnalogGainChange( double value );

    void onSldMaxExposureChange( int value );
    void onSldMaxExposureReleased();
    void onSbxMaxExposureChange( int value );

    void onSldControlSpeedChange( int value );
    void onSldControlSpeedReleased();
    void onSbxControlSpeedChange( int value );

    void onSldToleranceChange( int value );
    void onSldToleranceReleased();
    void onSbxToleranceChange( double value );

    void onBtnTimecodeSetClicked( );
    void onBtnTimecodeGetClicked( );
    void onBtnTimecodeHoldClicked( bool checked );

    void onTafToggle( bool checked );

    void onSldIrisAptChange( int value );
    void onSldIrisAptReleased();

    void onSbxIrisAptChange( int value );

    void onCbxLscEnableChange( int value );

    void onSldKChange( int value );
    void onSldKReleased();
    void onSbxKChange( double value );

    void onSldOffsetChange( int value );
    void onSldOffsetReleased();
    void onSbxOffsetChange( double value );

    void onSldSlopeChange( int value );
    void onSldSlopeReleased();
    void onSbxSlopeChange( double value );

private:
    class PrivateData;
    PrivateData * d_data;

    int gainToIso( int gain ) const;
    int isoToGain( int iso ) const;
    int multiplyBy1000( int value ) const;
    void show4kGenlockNote( int mode );
    void EmitDownscaleChanged( int sdi_out_idx, int combo_box_idx );
};

#endif // __IN_OUT_BOX_H__

