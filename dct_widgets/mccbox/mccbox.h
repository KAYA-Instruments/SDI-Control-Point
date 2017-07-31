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
 * @file    mccbox.h
 *
 * @brief   Class definition of a multi color control box 
 *
 *****************************************************************************/
#ifndef __MCC_BOX_H__
#define __MCC_BOX_H__

#include <dct_widgets_base.h>

/******************************************************************************
 * MCC General
 *****************************************************************************/
#define MCC_DEFAULT_OPERATION_MODE              ( 2 )
#define MCC_DEFAULT_COLOR_PHASES                ( 24 ) 
#define MCC_MAX_COLOR_PHASES                    ( 32 ) 

/******************************************************************************
 * Saturation Definitions
 *****************************************************************************/
#define MCC_SATURATION_MIN                      (        0 )
#define MCC_SATURATION_MAX                      (    65535 )
#define MCC_SATURATION_COMMA_POSITION           (       14 )
#define MCC_SATURATION_BASE                     (        2 )
#define MCC_SATURATION_DISPLAY_MULTIPLIER       (        1 )
#define MCC_SATURATION_DISPLAY_MASK             (  "%1.3f" )

/******************************************************************************
 * Hue Definitions
 *****************************************************************************/
#define MCC_HUE_MIN                             (   -32768 )
#define MCC_HUE_MAX                             (    32767 )
#define MCC_HUE_COMMA_POSITION                  (       14 )
#define MCC_HUE_BASE                            (        2 )
#define MCC_HUE_DISPLAY_MULTIPLIER              (       90 )
#define MCC_HUE_DISPLAY_MASK                    (  "%3.3f" )

/******************************************************************************
 * Multi Color Controller Widget
 *****************************************************************************/
class MccBox : public DctWidgetBox
{
    Q_OBJECT

public:
    explicit MccBox( QWidget * parent = 0 );
    ~MccBox();

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

    void setMccPhase( const int id, const int saturation, const int hue );

protected:
    void prepareMode( const Mode ) Q_DECL_OVERRIDE;
    
    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

signals:
    void MccEnableChanged( int flag );
    void MccOperationModeChanged( int mode, int no_phases );

    void MccPhaseIndexChanged( int id );
    void MccPhaseChanged( int id, int saturation, int hue );

public slots:
    void onMccEnableChange( const int flag );
    void onMccOperationModeChange( int mode, int no_phases );
    void onMccPhaseChange( int id, int saturation, int hue );

protected:
    void enterEvent(QEvent * ) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent * ) Q_DECL_OVERRIDE;

private slots:
    void onOpModeChange( int );
    void onPhaseChange( int );
    void onHueChange( int );
    void onSaturationChange( int );
    void onLoadProfileClick();
    void onSaveProfileClick();

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __MCC_BOX_H__

