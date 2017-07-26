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
 * @file    mcceqbox.h
 *
 * @brief   Class definition of a multi color control box 
 *
 *****************************************************************************/
#ifndef __MCC_EQ_BOX_H__
#define __MCC_EQ_BOX_H__

#include <dct_widgets_base.h>

/******************************************************************************
 * MCC General
 *****************************************************************************/
#define MCC_DEFAULT_OPERATION_MODE              ( 2 )
#define MCC_DEFAULT_COLOR_PHASES                ( 24 )
#define MCC_DEFAULT_ACTIVE_SEGMENT              ( -1 )  // default: no phase selected
#define MCC_MAX_COLOR_PHASES                    ( 32 ) 

/******************************************************************************
 * Saturation Definitions
 *****************************************************************************/
#define MCC_SATURATION_MIN                      (       0 )
#define MCC_SATURATION_MAX                      (   65535 )
#define MCC_SATURATION_DEFAULT                  ( 1 << 14 ) // 1 in Q2.14 format
#define MCC_SATURATION_DIVISOR                  ( 1 << 14 ) // defines the value that equals "1"
#define MCC_SATURATION_DEFAULT_RANGE            (     100 )

/******************************************************************************
 * Hue Definitions
 *****************************************************************************/
#define MCC_HUE_MIN                             ( -32768 )
#define MCC_HUE_MAX                             (  32767 )
#define MCC_HUE_DEFAULT                         (      0 )
#define MCC_HUE_DIVISOR                         (  32768 ) // defines the value that equals "1"
#define MCC_HUE_DEFAULT_RANGE                   (    100 )

/******************************************************************************
 * Multi Color Controller Widget
 *****************************************************************************/
class MccEqBox : public DctWidgetBox
{
    Q_OBJECT

public:
    explicit MccEqBox( QWidget * parent = 0 );
    ~MccEqBox();

    void addOperationMode( int mode, int no_phases );

    bool MccEnable() const;
    void setMccEnable( const bool enable );

    int MccOpMode() const;
    int MccNoPhases() const;
    void setMccOpMode( const int mode, const int no_phases );

    int MccHue( const int id ) const;
    void setMccHue( const int id, const int hue );

    int MccSaturation( const int id ) const;
    void setMccSaturation( const int id, const int saturation );

    int HueRange() const;
    void setHueRange( const int range );

    int SatRange() const;
    void setSatRange( const int range );

    void setMccPhase( const int id, const int saturation, const int hue );

protected:
    void prepareMode( const Mode ) Q_DECL_OVERRIDE;
    
    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

signals:
    // signals to mcc interface to indicate changed settings / values
    void MccEnableChanged( int flag );
    void MccOperationModeChanged( int mode, int no_phases );

    void MccPhaseIndexChanged( int id );
    void MccPhaseChanged( int id, int saturation, int hue );

    // internal signals
    void HueChanged( int id, int hue );
    void SatChanged( int id, int sat );
    void BlinkChanged( int id, bool blink );
    void HueRangeChanged( int range );
    void SatRangeChanged( int range );
    void ResetPhase( int index );
    void SegmentSelected( int index );

public slots:
    void onMccEnableChange( const int flag );
    void onMccOperationModeChange( int mode, int no_phases );
    void onMccPhaseChange( int id, int saturation, int hue );

private slots:
    // mcc settings
    void onEnableChange( int enable );
    void onOpModeChange( int mode );
    void onLoadProfileClick();
    void onSaveProfileClick();
    // slider changes
    void onHueChange( int id, int hue );
    void onSatChange( int id, int sat );
    void onBlinkChange( int id, bool blink );
    // eq settings
    void onHueRangeChange( int range );
    void onSatRangeChange( int range );
    void onResetEqualizerClick();
    // phase settings
    void onHueSpinBoxChange( double value );
    void onSatSpinBoxChange( double value );
    void onResetPhaseClick();

private:
    class PrivateData;
    PrivateData * d_data;

    // add sliders to equalizer
    void setupEqualizer();
    // deselect current phase to return "selected phase" window to default state (e.g. on mode change or load settings)
    void deselectActiveSegment();
};

#endif // __MCC_EQ_BOX_H__

