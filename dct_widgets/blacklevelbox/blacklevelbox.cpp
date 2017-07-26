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
 * @file    blacklevelbox.cpp
 *
 * @brief   Implementation of a blacklevel box 
 *
 *****************************************************************************/
#include <QtDebug>

#include "skinneddial.h"
#include "blacklevelbox.h"
#include "ui_blacklevelbox.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_BlackLevelBox;
}

/******************************************************************************
 * local definitions
 *****************************************************************************/
#define BL_SETTINGS_SECTION_NAME        ( "BL" )

#define BL_SETTINGS_RED_BLACK_LEVEL     ( "sensor-red" )
#define BL_SETTINGS_GREEN_BLACK_LEVEL   ( "sensor-green" )
#define BL_SETTINGS_BLUE_BLACK_LEVEL    ( "sensor-blue" )

#define BL_SETTINGS_RED_FLARE_LEVEL     ( "flare-red" )
#define BL_SETTINGS_GREEN_FLARE_LEVEL   ( "flare-green" )
#define BL_SETTINGS_BLUE_FLARE_LEVEL    ( "flare-blue" )

#define BL_SETTINGS_RED_MASTER_LEVEL     ( "master-red" )
#define BL_SETTINGS_GREEN_MASTER_LEVEL   ( "master-green" )
#define BL_SETTINGS_BLUE_MASTER_LEVEL    ( "master-blue" )

/******************************************************************************
 * local helper functions
 *****************************************************************************/

/******************************************************************************
 * Minimum - compute minimum of a 3 component tuple
 *****************************************************************************/
static int Minimum( int a, int b, int c)
{
    int v = a;
    
    if ( b < v )
    {
        v=b;
    }

    if ( c < v )
    {
        v = c;
    }

    return ( v );
}

/******************************************************************************
 * BlackLevelBox::PrivateData
 *****************************************************************************/
class BlackLevelBox::PrivateData
{
public:
    PrivateData()
        : m_ui( new Ui::UI_BlackLevelBox )
    {
        // do nothing
    };

    ~PrivateData()
    {
        delete m_ui;
    };

    Ui::UI_BlackLevelBox *  m_ui;   /**< ui handle */
};


/******************************************************************************
 * BlackLevelBox::BlackLevelBox
 *****************************************************************************/
BlackLevelBox::BlackLevelBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData;

    // initialize UI
    d_data->m_ui->setupUi( this );

    ////////////////////
    // sensor black
    ////////////////////

    // configure sensor black
    d_data->m_ui->SensorBlack->setRange( BL_SENSOR_BLACK_MIN, BL_SENSOR_BLACK_MAX );
    d_data->m_ui->SensorBlack->setCommaPos( BL_SENSOR_BLACK_COMMA_POSITION );
    d_data->m_ui->SensorBlack->setDisplayMultiplier( BL_SENSOR_BLACK_DISPLAY_MULTIPLIER );
    d_data->m_ui->SensorBlack->setBase( BL_SENSOR_BLACK_BASE );
    d_data->m_ui->SensorBlack->setMaxAngle( 180 );
    d_data->m_ui->SensorBlack->setMaxRounds( 4 );
    d_data->m_ui->SensorBlack->setFmt( BL_SENSOR_BLACK_DISPLAY_MASK );

    // configure sensor black master
    d_data->m_ui->SensorBlackMaster->setRange( BL_SENSOR_BLACK_MIN, BL_SENSOR_BLACK_MAX );
    d_data->m_ui->SensorBlackMaster->setCommaPos( BL_SENSOR_BLACK_COMMA_POSITION );
    d_data->m_ui->SensorBlackMaster->setDisplayMultiplier( BL_SENSOR_BLACK_DISPLAY_MULTIPLIER );
    d_data->m_ui->SensorBlackMaster->setBase( BL_SENSOR_BLACK_BASE );
    d_data->m_ui->SensorBlackMaster->setMaxAngle( 180 );
    d_data->m_ui->SensorBlackMaster->setMaxRounds( 4 );
    d_data->m_ui->SensorBlackMaster->setFmt( BL_SENSOR_BLACK_DISPLAY_MASK );

    ////////////////////
    // flare level
    ////////////////////

    // configure flare level
    d_data->m_ui->FlareCompensation->setRange( BL_FLARE_LEVEL_MIN, BL_FLARE_LEVEL_MAX );
    d_data->m_ui->FlareCompensation->setCommaPos( BL_FLARE_LEVEL_COMMA_POSITION );
    d_data->m_ui->FlareCompensation->setDisplayMultiplier( BL_FLARE_LEVEL_DISPLAY_MULTIPLIER );
    d_data->m_ui->FlareCompensation->setBase( BL_FLARE_LEVEL_BASE );
    d_data->m_ui->FlareCompensation->setMaxAngle( 180 );
    d_data->m_ui->FlareCompensation->setMaxRounds( 4 );
    d_data->m_ui->FlareCompensation->setFmt( BL_FLARE_LEVEL_DISPLAY_MASK );

    // configure sensor black master
    d_data->m_ui->FlareCompensationMaster->setRange( BL_FLARE_LEVEL_MIN, BL_FLARE_LEVEL_MAX );
    d_data->m_ui->FlareCompensationMaster->setCommaPos( BL_FLARE_LEVEL_COMMA_POSITION );
    d_data->m_ui->FlareCompensationMaster->setDisplayMultiplier( BL_FLARE_LEVEL_DISPLAY_MULTIPLIER );
    d_data->m_ui->FlareCompensationMaster->setBase( BL_FLARE_LEVEL_BASE );
    d_data->m_ui->FlareCompensationMaster->setMaxAngle( 180 );
    d_data->m_ui->FlareCompensationMaster->setMaxRounds( 4 );
    d_data->m_ui->FlareCompensationMaster->setFmt( BL_FLARE_LEVEL_DISPLAY_MASK );
    
    ////////////////////
    // master black
    ////////////////////

    // configure sensor black
    d_data->m_ui->MasterBlack->setRange( BL_MASTER_BLACK_MIN, BL_MASTER_BLACK_MAX );
    d_data->m_ui->MasterBlack->setCommaPos( BL_MASTER_BLACK_COMMA_POSITION );
    d_data->m_ui->MasterBlack->setDisplayMultiplier( BL_MASTER_BLACK_DISPLAY_MULTIPLIER );
    d_data->m_ui->MasterBlack->setBase( BL_MASTER_BLACK_BASE );
    d_data->m_ui->MasterBlack->setMaxAngle( 180 );
    d_data->m_ui->MasterBlack->setMaxRounds( 4 );
    d_data->m_ui->MasterBlack->setFmt( BL_MASTER_BLACK_DISPLAY_MASK );

    // configure sensor black master
    d_data->m_ui->MasterBlackMaster->setRange( BL_MASTER_BLACK_MIN, BL_MASTER_BLACK_MAX );
    d_data->m_ui->MasterBlackMaster->setCommaPos( BL_MASTER_BLACK_COMMA_POSITION );
    d_data->m_ui->MasterBlackMaster->setDisplayMultiplier( BL_MASTER_BLACK_DISPLAY_MULTIPLIER );
    d_data->m_ui->MasterBlackMaster->setBase( BL_MASTER_BLACK_BASE );
    d_data->m_ui->MasterBlackMaster->setMaxAngle( 180 );
    d_data->m_ui->MasterBlackMaster->setMaxRounds( 4 );
    d_data->m_ui->MasterBlackMaster->setFmt( BL_MASTER_BLACK_DISPLAY_MASK );

    ////////////////////
    // connect signals
    ////////////////////

    // connect sensor black signals
    connect( d_data->m_ui->SensorBlack, SIGNAL(Value0Changed(int)), this, SLOT(onSensorBlackRedChange(int)) );
    connect( d_data->m_ui->SensorBlack, SIGNAL(Value1Changed(int)), this, SLOT(onSensorBlackGreenChange(int)) );
    connect( d_data->m_ui->SensorBlack, SIGNAL(Value2Changed(int)), this, SLOT(onSensorBlackBlueChange(int)) );
    connect( d_data->m_ui->SensorBlackMaster, SIGNAL(ValueChanged(int)), this, SLOT(onSensorBlackMasterChange(int)) );

    // connect flare compensation signals
    connect( d_data->m_ui->FlareCompensation, SIGNAL(ValuesChanged(int,int,int)), this, SLOT(onFlareCompensationChange(int,int,int)) );
    connect( d_data->m_ui->FlareCompensationMaster, SIGNAL(ValueChanged(int)), this, SLOT(onFlareCompensationMasterChange(int)) );
    connect ( d_data->m_ui->btnFlareLevelReset, SIGNAL(clicked()), this, SLOT(onFlareLevelResetClicked()) );

    // connect master black signals
    connect( d_data->m_ui->MasterBlack, SIGNAL(ValuesChanged(int,int,int)), this, SLOT(onMasterBlackChange(int,int,int)) );
    connect( d_data->m_ui->MasterBlackMaster, SIGNAL(ValueChanged(int)), this, SLOT(onMasterBlackMasterChange(int)) );
    connect ( d_data->m_ui->btnMasterBlackReset, SIGNAL(clicked()), this, SLOT(onMasterBlackResetClicked()) );

    ////////////////////
    // operation mode
    ////////////////////
    prepareMode( mode() ); 
}

/******************************************************************************
 * BlackLevelBox::~BlackLevelBox
 *****************************************************************************/
BlackLevelBox::~BlackLevelBox()
{
    delete d_data;
}

/******************************************************************************
 * BlackLevelBox::RedBlackLevel
 *****************************************************************************/
int BlackLevelBox::RedBlackLevel() const
{
    int value = d_data->m_ui->SensorBlack->value0();
    value += d_data->m_ui->SensorBlackMaster->value();
    return ( value );
}

/******************************************************************************
 * BlackLevelBox::setRedBlackLevel
 *****************************************************************************/
void BlackLevelBox::setRedBlackLevel( const int red )
{
    int green = d_data->m_ui->SensorBlack->value1();
    int blue  = d_data->m_ui->SensorBlack->value2();
    int min   = Minimum( red, green, blue );

    d_data->m_ui->SensorBlackMaster->setValue( min );
    d_data->m_ui->SensorBlack->setRange( (BL_SENSOR_BLACK_MIN - min), (BL_SENSOR_BLACK_MAX - min) );
    d_data->m_ui->SensorBlack->setValue0( red   - min );
    d_data->m_ui->SensorBlack->setValue1( green - min );
    d_data->m_ui->SensorBlack->setValue2( blue  - min );
    
    emit RedBlackLevelChanged( red );
}

/******************************************************************************
 * BlackLevelBox::GreenBlackLevel
 *****************************************************************************/
int BlackLevelBox::GreenBlackLevel() const
{
    int value = d_data->m_ui->SensorBlack->value1();
    value += d_data->m_ui->SensorBlackMaster->value();
    return ( value );
}

/******************************************************************************
 * BlackLevelBox::setGreenBlackLevel
 *****************************************************************************/
void BlackLevelBox::setGreenBlackLevel( const int green )
{
    int red   = d_data->m_ui->SensorBlack->value0();
    int blue  = d_data->m_ui->SensorBlack->value2();
    int min   = Minimum( red, green, blue );

    d_data->m_ui->SensorBlackMaster->setValue( min );
    d_data->m_ui->SensorBlack->setRange( (BL_SENSOR_BLACK_MIN - min), (BL_SENSOR_BLACK_MAX - min) );
    d_data->m_ui->SensorBlack->setValue0( red   - min );
    d_data->m_ui->SensorBlack->setValue1( green - min );
    d_data->m_ui->SensorBlack->setValue2( blue  - min );
    
    emit GreenBlackLevelChanged( green );
}

/******************************************************************************
 * BlackLevelBox::BlueBlackLevel
 *****************************************************************************/
int BlackLevelBox::BlueBlackLevel() const
{
    int value = d_data->m_ui->SensorBlack->value2();
    value += d_data->m_ui->SensorBlackMaster->value();
    return ( value );
}

/******************************************************************************
 * BlackLevelBox::setBlueBlackLevel
 *****************************************************************************/
void BlackLevelBox::setBlueBlackLevel( const int blue )
{
    int red   = d_data->m_ui->SensorBlack->value0();
    int green = d_data->m_ui->SensorBlack->value1();
    int min   = Minimum( red, green, blue );

    d_data->m_ui->SensorBlackMaster->setValue( min );
    d_data->m_ui->SensorBlack->setRange( (BL_SENSOR_BLACK_MIN - min), (BL_SENSOR_BLACK_MAX - min) );
    d_data->m_ui->SensorBlack->setValue0( red   - min );
    d_data->m_ui->SensorBlack->setValue1( green - min );
    d_data->m_ui->SensorBlack->setValue2( blue  - min );
    
    emit BlueBlackLevelChanged( blue );
}

/******************************************************************************
 * BlackLevelBox::RedFlareLevel
 *****************************************************************************/
int BlackLevelBox::RedFlareLevel() const
{
    int value = d_data->m_ui->FlareCompensation->value0();
    value += d_data->m_ui->FlareCompensationMaster->value();
    return ( value );
}

/******************************************************************************
 * BlackLevelBox::GreenFlareLevel
 *****************************************************************************/
int BlackLevelBox::GreenFlareLevel() const
{
    int value = d_data->m_ui->FlareCompensation->value1();
    value += d_data->m_ui->FlareCompensationMaster->value();
    return ( value );
}

/******************************************************************************
 * BlackLevelBox::BlueFlareLevel
 *****************************************************************************/
int BlackLevelBox::BlueFlareLevel() const
{
    int value = d_data->m_ui->FlareCompensation->value2();
    value += d_data->m_ui->FlareCompensationMaster->value();
    return ( value );
}

/******************************************************************************
 * BlackLevelBox::setFlareLevel
 *****************************************************************************/
void BlackLevelBox::setFlareLevel( const int red, const int green, const int blue )
{
    int min = Minimum( red, green, blue );
    d_data->m_ui->FlareCompensationMaster->setValue( min );
    d_data->m_ui->FlareCompensation->setRange( (BL_FLARE_LEVEL_MIN - min), (BL_FLARE_LEVEL_MAX - min) );
    d_data->m_ui->FlareCompensation->onValue0Change( red   - min );
    d_data->m_ui->FlareCompensation->onValue1Change( green - min );
    d_data->m_ui->FlareCompensation->onValue2Change( blue  - min );

    emit FlareLevelChanged( red, green, blue ); 
}

/******************************************************************************
 * BlackLevelBox::RedMasterBlack
 *****************************************************************************/
int BlackLevelBox::RedMasterBlack() const
{
    int value = d_data->m_ui->MasterBlack->value0();
    value += d_data->m_ui->MasterBlackMaster->value();
    return ( value );
}

/******************************************************************************
 * BlackLevelBox::GreenMasterBlack
 *****************************************************************************/
int BlackLevelBox::GreenMasterBlack() const
{
    int value = d_data->m_ui->MasterBlack->value1();
    value += d_data->m_ui->MasterBlackMaster->value();
    return ( value );
}

/******************************************************************************
 * BlackLevelBox::BlueMasterBlack
 *****************************************************************************/
int BlackLevelBox::BlueMasterBlack() const
{
    int value = d_data->m_ui->MasterBlack->value2();
    value += d_data->m_ui->MasterBlackMaster->value();
    return ( value );
}

/******************************************************************************
 * BlackLevelBox::setMasterBlackLevel
 *****************************************************************************/
void BlackLevelBox::setMasterBlackLevel( const int red, const int green, const int blue )
{
    int min = Minimum( red, green, blue );
    d_data->m_ui->MasterBlackMaster->setValue( min );
    d_data->m_ui->MasterBlack->setRange( (BL_MASTER_BLACK_MIN - min), (BL_MASTER_BLACK_MAX - min) );
    d_data->m_ui->MasterBlack->onValue0Change( red   - min );
    d_data->m_ui->MasterBlack->onValue1Change( green - min );
    d_data->m_ui->MasterBlack->onValue2Change( blue  - min );
    
    emit MasterBlackLevelChanged( red, green, blue ); 
}

/******************************************************************************
 * BlackLevelBox::setFlareLevelVisible
 *****************************************************************************/
void BlackLevelBox::setFlareLevelVisible( const bool visible )
{
    d_data->m_ui->gpxFlareLevel->setVisible( visible );
}

/******************************************************************************
 * BlackLevelBox::setMasterBlackLevelVisible
 *****************************************************************************/
void BlackLevelBox::setMasterBlackLevelVisible( const bool visible )
{
    d_data->m_ui->gpxMasterBlack->setVisible( visible );
}

/******************************************************************************
 * BlackLevelBox::prepareMode
 *****************************************************************************/
void BlackLevelBox::prepareMode( const Mode mode )
{
    d_data->m_ui->gpxSensorBlack->setVisible( (DctWidgetBox::Normal == mode) ? false : true );
}

/******************************************************************************
 * BlackLevelBox::loadSettings
 *****************************************************************************/
void BlackLevelBox::loadSettings( QSettings & s )
{
    int red, green, blue;

    s.beginGroup( BL_SETTINGS_SECTION_NAME );

    setRedBlackLevel  ( s.value( BL_SETTINGS_RED_BLACK_LEVEL ).toInt() );
    setGreenBlackLevel( s.value( BL_SETTINGS_GREEN_BLACK_LEVEL ).toInt() );
    setBlueBlackLevel ( s.value( BL_SETTINGS_BLUE_BLACK_LEVEL ).toInt() );

    red   = s.value( BL_SETTINGS_RED_FLARE_LEVEL ).toInt();
    green = s.value( BL_SETTINGS_GREEN_FLARE_LEVEL ).toInt();
    blue  = s.value( BL_SETTINGS_BLUE_FLARE_LEVEL ).toInt();
    setFlareLevel( red, green, blue );

    red   = s.value( BL_SETTINGS_RED_MASTER_LEVEL ).toInt();
    green = s.value( BL_SETTINGS_GREEN_MASTER_LEVEL ).toInt();
    blue  = s.value( BL_SETTINGS_BLUE_MASTER_LEVEL ).toInt();
    setMasterBlackLevel( red, green, blue );
  
    s.endGroup();
}

/******************************************************************************
 * BlackLevelBox::saveSettings
 *****************************************************************************/
void BlackLevelBox::saveSettings( QSettings & s )
{
    s.beginGroup( BL_SETTINGS_SECTION_NAME );
  
    s.setValue( BL_SETTINGS_RED_BLACK_LEVEL  , RedBlackLevel() );
    s.setValue( BL_SETTINGS_GREEN_BLACK_LEVEL, GreenBlackLevel() );
    s.setValue( BL_SETTINGS_BLUE_BLACK_LEVEL , BlueBlackLevel() );
    
    s.setValue( BL_SETTINGS_RED_FLARE_LEVEL  , RedFlareLevel() );
    s.setValue( BL_SETTINGS_GREEN_FLARE_LEVEL, GreenFlareLevel() );
    s.setValue( BL_SETTINGS_BLUE_FLARE_LEVEL , BlueFlareLevel() );
    
    s.setValue( BL_SETTINGS_RED_MASTER_LEVEL  , RedMasterBlack() );
    s.setValue( BL_SETTINGS_GREEN_MASTER_LEVEL, GreenMasterBlack() );
    s.setValue( BL_SETTINGS_BLUE_MASTER_LEVEL , BlueMasterBlack() );

    s.endGroup();
}

/******************************************************************************
 * BlackLevelBox::applySettings
 *****************************************************************************/
void BlackLevelBox::applySettings( void )
{
    emit RedBlackLevelChanged( RedBlackLevel() );
    emit GreenBlackLevelChanged( GreenBlackLevel() );
    emit BlueBlackLevelChanged( BlueBlackLevel() );

    emit FlareLevelChanged( RedFlareLevel(), GreenFlareLevel(), BlueFlareLevel() );
    emit MasterBlackLevelChanged( RedMasterBlack(), GreenMasterBlack(), BlueMasterBlack() );
}

/******************************************************************************
 * BlackLevelBox::onRedBlackLevelChange
 *****************************************************************************/
void BlackLevelBox::onRedBlackLevelChange( int value )
{
    int green = d_data->m_ui->SensorBlack->value1();
    int blue  = d_data->m_ui->SensorBlack->value2();
    int min   = Minimum( value, green, blue );
    d_data->m_ui->SensorBlackMaster->setValue( min );
    d_data->m_ui->SensorBlack->setRange( (BL_SENSOR_BLACK_MIN - min), (BL_SENSOR_BLACK_MAX - min) );
    d_data->m_ui->SensorBlack->setValue0( value - min );
    d_data->m_ui->SensorBlack->setValue1( green - min );
    d_data->m_ui->SensorBlack->setValue2( blue  - min );
}

/******************************************************************************
 * BlackLevelBox::onGreenBlackLevelChange
 *****************************************************************************/
void BlackLevelBox::onGreenBlackLevelChange( int value )
{
    int red   = d_data->m_ui->SensorBlack->value0();
    int blue  = d_data->m_ui->SensorBlack->value2();
    int min   = Minimum( red, value, blue );
    d_data->m_ui->SensorBlackMaster->setValue( min );
    d_data->m_ui->SensorBlack->setRange( (BL_SENSOR_BLACK_MIN - min), (BL_SENSOR_BLACK_MAX - min) );
    d_data->m_ui->SensorBlack->setValue0( red   - min );
    d_data->m_ui->SensorBlack->setValue1( value - min );
    d_data->m_ui->SensorBlack->setValue2( blue  - min );
}

/******************************************************************************
 * BlackLevelBox::onBlueBlackLevelChange
 *****************************************************************************/
void BlackLevelBox::onBlueBlackLevelChange( int value )
{
    int red   = d_data->m_ui->SensorBlack->value0();
    int green = d_data->m_ui->SensorBlack->value1();
    int min   = Minimum( red, green, value );
    d_data->m_ui->SensorBlackMaster->setValue( min );
    d_data->m_ui->SensorBlack->setRange( (BL_SENSOR_BLACK_MIN - min), (BL_SENSOR_BLACK_MAX - min) );
    d_data->m_ui->SensorBlack->setValue0( red   - min );
    d_data->m_ui->SensorBlack->setValue1( green - min );
    d_data->m_ui->SensorBlack->setValue2( value - min );
}

/******************************************************************************
 * BlackLevelBox::onBlackLevelChange
 *****************************************************************************/
void BlackLevelBox::onBlackLevelChange( int red, int green, int blue )
{
    int min = Minimum( red, green, blue );
    d_data->m_ui->SensorBlackMaster->setValue( min );
    d_data->m_ui->SensorBlack->setRange( (BL_SENSOR_BLACK_MIN - min), (BL_SENSOR_BLACK_MAX - min) );
    d_data->m_ui->SensorBlack->setValue0( red   - min );
    d_data->m_ui->SensorBlack->setValue1( green - min );
    d_data->m_ui->SensorBlack->setValue2( blue  - min );
}

/******************************************************************************
 * BlackLevelBox::onMasterBlackLevelChange
 *****************************************************************************/
void BlackLevelBox::onMasterBlackLevelChange( int red, int green, int blue )
{
    int min = Minimum( red, green, blue );
    d_data->m_ui->MasterBlackMaster->setValue( min );
    d_data->m_ui->MasterBlack->setRange( (BL_MASTER_BLACK_MIN - min), (BL_MASTER_BLACK_MAX - min) );
    d_data->m_ui->MasterBlack->onValue0Change( red   - min );
    d_data->m_ui->MasterBlack->onValue1Change( green - min );
    d_data->m_ui->MasterBlack->onValue2Change( blue  - min );
}

/******************************************************************************
 * BlackLevelBox::onFlareLevelChange
 *****************************************************************************/
void BlackLevelBox::onFlareLevelChange( int red, int green, int blue )
{
    int min = Minimum( red, green, blue );
    d_data->m_ui->FlareCompensationMaster->setValue( min );
    d_data->m_ui->FlareCompensation->setRange( (BL_FLARE_LEVEL_MIN - min), (BL_FLARE_LEVEL_MAX - min) );
    d_data->m_ui->FlareCompensation->onValue0Change( red   - min );
    d_data->m_ui->FlareCompensation->onValue1Change( green - min );
    d_data->m_ui->FlareCompensation->onValue2Change( blue  - min );
}

/******************************************************************************
 * BlackLevelBox::onSensorBlackRedChange
 *****************************************************************************/
void BlackLevelBox::onSensorBlackRedChange( int value )
{
    value += d_data->m_ui->SensorBlackMaster->value();
    emit RedBlackLevelChanged( value );
}

/******************************************************************************
 * BlackLevelBox::onSensorBlackGreenChange
 *****************************************************************************/
void BlackLevelBox::onSensorBlackGreenChange( int value )
{
    value += d_data->m_ui->SensorBlackMaster->value();
    emit GreenBlackLevelChanged( value );
}

/******************************************************************************
 * BlackLevelBox::onSensorBlackBlueChange
 *****************************************************************************/
void BlackLevelBox::onSensorBlackBlueChange( int value )
{
    value += d_data->m_ui->SensorBlackMaster->value();
    emit BlueBlackLevelChanged( value );
}

/******************************************************************************
 * BlackLevelBox::onSensorBlackMasterChange
 *****************************************************************************/
void BlackLevelBox::onSensorBlackMasterChange( int value )
{
    d_data->m_ui->SensorBlack->setRange( (BL_SENSOR_BLACK_MIN - value), (BL_SENSOR_BLACK_MAX - value) );

    emit RedBlackLevelChanged( value + d_data->m_ui->SensorBlack->value0() );
    emit GreenBlackLevelChanged( value + d_data->m_ui->SensorBlack->value1() );
    emit BlueBlackLevelChanged( value + d_data->m_ui->SensorBlack->value2() );
}

/******************************************************************************
 * BlackLevelBox::onMasterBlackChange
 *****************************************************************************/
void BlackLevelBox::onMasterBlackChange( int red, int green, int blue )
{
    red   += d_data->m_ui->MasterBlackMaster->value();
    green += d_data->m_ui->MasterBlackMaster->value();
    blue  += d_data->m_ui->MasterBlackMaster->value();
    
    emit MasterBlackLevelChanged( red, green, blue );
}

/******************************************************************************
 * BlackLevelBox::onMasterBlackMasterChange
 *****************************************************************************/
void BlackLevelBox::onMasterBlackMasterChange( int value )
{
    int red, green, blue;

    d_data->m_ui->MasterBlack->setRange( (BL_MASTER_BLACK_MIN - value), (BL_MASTER_BLACK_MAX - value) );
    
    red   = d_data->m_ui->MasterBlack->value0() + value;
    green = d_data->m_ui->MasterBlack->value1() + value;
    blue  = d_data->m_ui->MasterBlack->value2() + value;

    emit MasterBlackLevelChanged( red, green, blue );
}

/******************************************************************************
 * BlackLevelBox::onMasterBlackResetClicked
 *****************************************************************************/
void BlackLevelBox::onMasterBlackResetClicked()
{
    // Reset master black level widgets to default
    setMasterBlackLevel( BL_MASTER_BLACK_DEFAULT, BL_MASTER_BLACK_DEFAULT, BL_MASTER_BLACK_DEFAULT );
}

/******************************************************************************
 * BlackLevelBox::onFlareCompensationChange
 *****************************************************************************/
void BlackLevelBox::onFlareCompensationChange( int red, int green, int blue )
{
    red   += d_data->m_ui->FlareCompensationMaster->value();
    green += d_data->m_ui->FlareCompensationMaster->value();
    blue  += d_data->m_ui->FlareCompensationMaster->value();

    emit FlareLevelChanged( red, green, blue );
}

/******************************************************************************
 * BlackLevelBox::onFlareCompensationMasterChange
 *****************************************************************************/
void BlackLevelBox::onFlareCompensationMasterChange( int value )
{
    int red, green, blue;

    d_data->m_ui->FlareCompensation->setRange( (BL_FLARE_LEVEL_MIN - value), (BL_FLARE_LEVEL_MAX - value) );

    red   = d_data->m_ui->FlareCompensation->value0() + value;
    green = d_data->m_ui->FlareCompensation->value1() + value;
    blue  = d_data->m_ui->FlareCompensation->value2() + value;
    
    emit FlareLevelChanged( red, green, blue );
}

/******************************************************************************
 * BlackLevelBox::onFlareLevelResetClicked
 *****************************************************************************/
void BlackLevelBox::onFlareLevelResetClicked()
{
    // Reset flare level widgets to default
    setFlareLevel( BL_FLARE_LEVEL_DEFAULT, BL_FLARE_LEVEL_DEFAULT, BL_FLARE_LEVEL_DEFAULT );
}


