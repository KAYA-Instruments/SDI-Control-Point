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
    LscSegmentationMode mode;
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
    explicit InOutBox( QWidget * parent = 0 );
    ~InOutBox();

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
    int AecControlSpeed() const;
    void setAecControlSpeed( const int value );
    int AecFlickerFrequency() const;
    void setAecFlickerFrequency( const int value );

    bool LscEnable() const;
    LscSegmentationMode LscMode() const;
    float LscK() const;
    float LscOffset() const;
    float LscSlope() const;
    void setLsc(lsc_setup_t setup );

    QString VideoMode() const;
    void setVideoMode( const QString mode );
    QString Sdi2Mode() const;
    void setSdi2Mode( const QString mode );
    QString FlipMode() const;
    void setFlipMode( const QString mode );
    bool TestPattern() const;
    void setTestPattern( const bool value );

    QString GenLockMode() const;
    void setGenLockMode( const QString mode );
    int GenLockOffsetVertical() const;
    void setGenLockOffsetVertical( const int value );
    int GenLockOffsetHorizontal() const;
    void setGenLockOffsetHorizontal( const int value );
    bool GenLockTermination() const;
    void setGenLockTermination( const bool value );

    void addLscSegmentationMode( QString name, int id );
    void addBayerPattern( QString name, int id );
    void clearAllVideoModes();
    void addVideoMode( QString name, int id );
    void addGenlockMode( QString name, int id );
    void addSdi2Mode( QString name, int id );
    void addFlipMode( QString name, int id );

    // Show or hide UI elements
    void setCameraSettingsVisible(const bool value);
    void setAutoExposureSettingsVisible(const bool value);
    void setLensChadingCorrectionSettingsVisible(const bool value);
    void setApartureVisible(const bool value);
    void setGenLockVisible(const bool value);
    void setTimeCodeVisible(const bool groupbox_visible, const bool hold_visible);
    void setSdi2ModeVisible(const bool value);
    void setFlipModeVisible(const bool value);
    void setTestPatternVisible(const bool value);

protected:
    void prepareMode( const Mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

    void updateAecSetupWidgets( void );
    void updateLscWidgets( void );
    void enableAecWidgets( bool enable );
    void enableCamConfWidgets( bool enable );

    QVector<int> createAecVector( void );
    QVector<uint> createLscVector( void );

signals:
    void LscChanged( QVector<uint> values );

    void BayerPatternChanged( int value );

    void CameraGainChanged( int value );
    void CameraExposureChanged( int value );

    void ChainVideoModeChanged( int value );
    void ChainSdi2ModeChanged( int value );
    void ChainFlipModeChanged( int value );
    void OsdTestPatternChanged( int value );
    
    void ChainGenlockModeChanged( int value );
    void ChainGenlockOffsetChanged( int vertical, int horizontal );
    void ChainGenlockTerminationChanged( int value );

    void AecEnableChanged( int value );
    void AecSetupChanged( QVector<int> values );

    void TimecodeGet( void );
    void TimecodeSet( QVector<int> );
    void TimecodeHold( bool );

    void IrisAptChanged( int );

    void ResyncRequest( void );

public slots:
    void onBayerPatternChange( int value );

    void onCameraInfoChange( int min_gain, int max_gain, int min_exposure, int max_exposure , int min_iso );
    void onCameraGainChange( int value );
    void onCameraExposureChange( int value );

    void onChainVideoModeChange( int value );
    void onChainSdi2ModeChange( int value );
    void onChainFlipModeChange( int value );
    void onOsdTestPatternChange( int value );

    void onChainGenlockModeChange( int value );
    void onChainGenlockOffsetChange( int vertical , int horizontal );
    void onChainGenlockTerminationChange( int value );

    void onAecEnableChange( int enable );
    void onAecSetupChange( QVector<int> values );

    void onAecStatChange( QVector<int> values );

    void onLscChange( QVector<uint> values );

    void onTimecodeChange( QVector<int> time );
    void onTimecodeHoldChange( bool enable );

    void onIrisAptChange( int value );
    void onIrisAptError( void );
    void onIrisSetupChange( QVector<int> values );

private slots:
    void onCbxBayerPatternChange( int value );

    void onSldIsoChange( int value );
    void onSldIsoReleased();
    void onSbxIsoChange( int value );

    void onSldExposureChange( int value );
    void onSldExposureReleased();
    void onSbxExposureChange( int value );
    
    void onCbxVideoModeChange( int value );
    void onCbxSdi2ModeChange( int value );
    void onCbxFlipModeChange( int value );
    void onCbxTestPatternChange( int value );
    
    void onCbxGenlockModeChange( int index );

    void onSbxGenlockOffsetVerticalChange( int value );
    void onSbxGenlockOffsetHorizontalChange( int value );
    
    void onCbxGenlockTerminationChange( int value );

    void onCbxAecEnableChange( int value );
    
    void onSldSetPointChange( int value );
    void onSldSetPointReleased();
    void onSbxSetPointChange( int value );
    
    void onSldControlSpeedChange( int value );
    void onSldControlSpeedReleased();
    void onSbxControlSpeedChange( int value );

    void onBtnTimecodeSetClicked( );
    void onBtnTimecodeGetClicked( );
    void onBtnTimecodeHoldClicked( bool checked );

    void onTafToggle( bool checked );

    void onSldIrisAptChange( int value );
    void onSldIrisAptReleased();

    void onSbxIrisAptChange( int value );

    void onCbxLscEnableChange( int value );
    void onCbxSegmentationModeChange( int index );

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

    int gainToIso( int gain );
    int isoToGain( int iso );

};

#endif // __IN_OUT_BOX_H__

