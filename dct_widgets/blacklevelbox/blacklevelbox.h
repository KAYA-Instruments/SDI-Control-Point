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
 * @file    blacklevelbox.h
 *
 * @brief   Class definition of a blacklevel box 
 *
 *****************************************************************************/
#ifndef __BLACK_LEVEL_BOX_H__
#define __BLACK_LEVEL_BOX_H__

#include <dct_widgets_base.h>

/******************************************************************************
 * Sensor Black Definitions
 *****************************************************************************/
#define BL_SENSOR_BLACK_MIN                 (    -4096 )
#define BL_SENSOR_BLACK_MAX                 (     4095 )
#define BL_SENSOR_BLACK_COMMA_POSITION      (        0 )
#define BL_SENSOR_BLACK_BASE                (       10 )
#define BL_SENSOR_BLACK_DISPLAY_MULTIPLIER  (        1 )
#define BL_SENSOR_BLACK_DISPLAY_MASK        ( "%-4.0f" )
#define BL_SENSOR_BLACK_DEFAULT             (        0 )

/******************************************************************************
 * Flare Compensation Definitions
 *****************************************************************************/
#define BL_FLARE_LEVEL_MIN                  (        0 )
#define BL_FLARE_LEVEL_MAX                  (    65535 )
#define BL_FLARE_LEVEL_COMMA_POSITION       (       16 )
#define BL_FLARE_LEVEL_BASE                 (        2 )
#define BL_FLARE_LEVEL_DISPLAY_MULTIPLIER   (      100 )    // percentage display
#define BL_FLARE_LEVEL_DISPLAY_MASK         (  "%3.3f" )
#define BL_FLARE_LEVEL_DEFAULT              (        0 )

/******************************************************************************
 * Master Black (RGB-Black) Definitions
 *****************************************************************************/
#define BL_MASTER_BLACK_MIN                 (    -2048 )
#define BL_MASTER_BLACK_MAX                 (     2047 )
#define BL_MASTER_BLACK_COMMA_POSITION      (        0 )
#define BL_MASTER_BLACK_BASE                (       10 )
#define BL_MASTER_BLACK_DISPLAY_MULTIPLIER  (        1 )
#define BL_MASTER_BLACK_DISPLAY_MASK        ( "%-4.0f" )
#define BL_MASTER_BLACK_DEFAULT             (        0 )

/******************************************************************************
 * Black Level Box Widget
 *****************************************************************************/
class BlackLevelBox : public DctWidgetBox
{
    Q_OBJECT

public:
    explicit BlackLevelBox( QWidget * parent = 0 );
    ~BlackLevelBox();

    int RedBlackLevel() const;
    void setRedBlackLevel( const int red );
    int GreenBlackLevel() const;
    void setGreenBlackLevel( const int green );
    int BlueBlackLevel() const;
    void setBlueBlackLevel( const int blue );
    void setBlackLevel( const int red, const int green, const int blue );
    
    int RedFlareLevel() const;
    int GreenFlareLevel() const;
    int BlueFlareLevel() const;
    void setFlareLevel( const int red, const int green, const int blue );
    
    int RedMasterBlack() const;
    int GreenMasterBlack() const;
    int BlueMasterBlack() const;
    void setMasterBlackLevel( const int red, const int green, const int blue );

    void setFlareLevelVisible( const bool visible );
    void setMasterBlackLevelVisible( const bool visible );

protected:
    void prepareMode( const Mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

signals:
    // sensor black level changed
    void RedBlackLevelChanged( int value );
    void GreenBlackLevelChanged( int value );
    void BlueBlackLevelChanged( int value );

    // flare level
    void FlareLevelChanged( int red, int green, int blue );

    // master black level
    void MasterBlackLevelChanged( int red, int green, int blue );
 
public slots:
    // sensor black 
    void onRedBlackLevelChange( int value );
    void onGreenBlackLevelChange( int value );
    void onBlueBlackLevelChange( int value );
    void onBlackLevelChange( int red, int green, int blue );

    // flare level
    void onFlareLevelChange( int red, int green, int blue );

    // master black level
    void onMasterBlackLevelChange( int red, int green, int blue );

private slots:
    // sensor black 
    void onSensorBlackRedChange( int value );
    void onSensorBlackGreenChange( int value );
    void onSensorBlackBlueChange( int value );
    void onSensorBlackMasterChange( int value );
    void onSensorBlackResetClicked();

    // master black 
    void onMasterBlackChange( int red, int green, int blue );
    void onMasterBlackMasterChange( int value );
    void onMasterBlackResetClicked();
    
    // flare level
    void onFlareCompensationChange( int red, int green, int blue );
    void onFlareCompensationMasterChange( int value );
    void onFlareLevelResetClicked();

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __BLACK_LEVEL_BOX_H__

