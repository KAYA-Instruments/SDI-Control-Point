/******************************************************************************
 *
 * Copyright 2016, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
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
    int CameraGain() const;
    void setCameraGain( const int value );
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
    void setTimeCodeVisible(const bool value);
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

    void IrisAptChanged( int );

    void ResyncRequest( void );

public slots:

    void onBayerPatternChange( int value );

    void onCameraInfoChange( int min_gain, int max_gain, int min_exposure, int max_exposure );
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

    void onIrisAptChange( int value );
    void onIrisAptError( void );
    void onIrisSetupChange( QVector<int> values );

private slots:
    void onCbxBayerPatternChange( int value );

    void onSldAnalogueGainChange( int value );
    void onSldAnalogueGainReleased();
    void onSbxAnalogueGainChange( int value );

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

};

#endif // __IN_OUT_BOX_H__

