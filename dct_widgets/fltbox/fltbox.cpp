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
 * @file    fltbox.cpp
 *
 * @brief   Implementation of filter configuration box
 *
 *****************************************************************************/
#include <QtDebug>
#include <QFileDialog>

#include "fltbox.h"
#include "ui_fltbox.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_FltBox;
}

/******************************************************************************
 * local defines
 *****************************************************************************/
#define FLT_SETTINGS_SECTION_NAME       ( "FLT" )
#define FLT_SETTINGS_2D_ENABLE          ( "enable_2d" )
#define FLT_SETTINGS_2D_DETAIL_LEVEL    ( "detail_2d" )
#define FLT_SETTINGS_2D_DENOISE_LEVEL   ( "denoise_2d" )
#define FLT_SETTINGS_3D_ENABLE          ( "enable_3d" )
#define FLT_SETTINGS_3D_DETAIL_LEVEL    ( "detail_3d" )
#define FLT_SETTINGS_3D_DENOISE_LEVEL   ( "denoise_3d" )

/******************************************************************************
 * FltBox::PrivateData
 *****************************************************************************/
class FltBox::PrivateData
{
public:
    PrivateData()
        : m_ui( new Ui::UI_FltBox )
    {
        // do nothing
    };

    ~PrivateData()
    {
        delete m_ui;
    };

    Ui::UI_FltBox *  m_ui;   /**< ui handle */
};

/******************************************************************************
 * FltBox::FltBox
 *****************************************************************************/
FltBox::FltBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData;

    // initialize UI
    d_data->m_ui->setupUi( this );

    ////////////////////
    // 2d denoise level
    ////////////////////

    // configure 2d denoise level 
    d_data->m_ui->kbxDenoiseLevel2d->setRange( FLT_2D_DENOISE_LEVEL_MIN, FLT_2D_DENOISE_LEVEL_MAX );
    d_data->m_ui->kbxDenoiseLevel2d->setCommaPos( FLT_2D_DENOISE_LEVEL_COMMA_POSITION );
    d_data->m_ui->kbxDenoiseLevel2d->setDisplayMultiplier( FLT_2D_DENOISE_LEVEL_DISPLAY_MULTIPLIER );
    d_data->m_ui->kbxDenoiseLevel2d->setBase( FLT_2D_DENOISE_LEVEL_BASE );
    d_data->m_ui->kbxDenoiseLevel2d->setMaxAngle( 150 );
    d_data->m_ui->kbxDenoiseLevel2d->setMaxRounds( 1 );
    d_data->m_ui->kbxDenoiseLevel2d->setFmt( FLT_2D_DENOISE_LEVEL_DISPLAY_MASK );

    ////////////////////
    // 2d detail level
    ////////////////////

    // configure 2d detail level 
    d_data->m_ui->kbxDetailLevel2d->setRange( FLT_2D_DETAIL_LEVEL_MIN, FLT_2D_DETAIL_LEVEL_MAX );
    d_data->m_ui->kbxDetailLevel2d->setCommaPos( FLT_2D_DETAIL_LEVEL_COMMA_POSITION );
    d_data->m_ui->kbxDetailLevel2d->setDisplayMultiplier( FLT_2D_DETAIL_LEVEL_DISPLAY_MULTIPLIER );
    d_data->m_ui->kbxDetailLevel2d->setBase( FLT_2D_DETAIL_LEVEL_BASE );
    d_data->m_ui->kbxDetailLevel2d->setMaxAngle( 150 );
    d_data->m_ui->kbxDetailLevel2d->setMaxRounds( 1 );
    d_data->m_ui->kbxDetailLevel2d->setFmt( FLT_2D_DETAIL_LEVEL_DISPLAY_MASK );

    ////////////////////
    // 3d denoise level
    ////////////////////

    // configure 3d denoise level 
    d_data->m_ui->kbxDenoiseLevel3d->setRange( FLT_3D_DENOISE_LEVEL_MIN, FLT_3D_DENOISE_LEVEL_MAX );
    d_data->m_ui->kbxDenoiseLevel3d->setCommaPos( FLT_3D_DENOISE_LEVEL_COMMA_POSITION );
    d_data->m_ui->kbxDenoiseLevel3d->setDisplayMultiplier( FLT_3D_DENOISE_LEVEL_DISPLAY_MULTIPLIER );
    d_data->m_ui->kbxDenoiseLevel3d->setBase( FLT_3D_DENOISE_LEVEL_BASE );
    d_data->m_ui->kbxDenoiseLevel3d->setMaxAngle( 150 );
    d_data->m_ui->kbxDenoiseLevel3d->setMaxRounds( 1 );
    d_data->m_ui->kbxDenoiseLevel3d->setFmt( FLT_3D_DENOISE_LEVEL_DISPLAY_MASK );

    ////////////////////
    // 3d detail level
    ////////////////////

    // configure 3d detail level 
    d_data->m_ui->kbxDetailLevel3d->setRange( FLT_3D_DETAIL_LEVEL_MIN, FLT_3D_DETAIL_LEVEL_MAX );
    d_data->m_ui->kbxDetailLevel3d->setCommaPos( FLT_3D_DETAIL_LEVEL_COMMA_POSITION );
    d_data->m_ui->kbxDetailLevel3d->setDisplayMultiplier( FLT_3D_DETAIL_LEVEL_DISPLAY_MULTIPLIER );
    d_data->m_ui->kbxDetailLevel3d->setBase( FLT_3D_DETAIL_LEVEL_BASE );
    d_data->m_ui->kbxDetailLevel3d->setMaxAngle( 150 );
    d_data->m_ui->kbxDetailLevel3d->setMaxRounds( 1 );
    d_data->m_ui->kbxDetailLevel3d->setFmt( FLT_3D_DETAIL_LEVEL_DISPLAY_MASK );

    
    // 2d ISP filter stage
    connect( d_data->m_ui->cbxEnable, SIGNAL(stateChanged(int)), this, SLOT(on2dEnableChange(int)) );
    connect( d_data->m_ui->kbxDetailLevel2d, SIGNAL(ValueChanged(int)), this, SIGNAL(FilterDetailLevelChanged(int)) );
    connect( d_data->m_ui->kbxDenoiseLevel2d, SIGNAL(ValueChanged(int)), this, SIGNAL(FilterDenoiseLevelChanged(int)) );
    
    // 3d TFLT stage
    connect( d_data->m_ui->cbxTfltEnable, SIGNAL(stateChanged(int)), this, SLOT(on3dEnableChange(int)) );
    connect( d_data->m_ui->kbxDenoiseLevel3d, SIGNAL(ValueChanged(int)), this, SIGNAL(TfltDenoiseLevelChanged(int)) );
    connect( d_data->m_ui->kbxDetailLevel3d, SIGNAL(ValueChanged(int)), this, SLOT(onObjectSpeedChange(int)) );

    d_data->m_ui->gpb3dDenoising->hide();
}

/******************************************************************************
 * FltBox::~FltBox
 *****************************************************************************/
FltBox::~FltBox()
{
    delete d_data;
}

/******************************************************************************
 * FltBox::Enable2d
 *****************************************************************************/
bool FltBox::Enable2d() const
{
    return ( (d_data->m_ui->cbxEnable->checkState() == Qt::Checked) ? true : false );
}

/******************************************************************************
 * FltBox::setEnable2d
 *****************************************************************************/
void FltBox::setEnable2d( const bool enable )
{
    d_data->m_ui->cbxEnable->blockSignals( true );
    d_data->m_ui->cbxEnable->setCheckState( enable ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxEnable->blockSignals( false );

    emit FilterEnableChanged( enable ? 1 : 0 );
}

/******************************************************************************
 * FltBox::DetailLevel2d
 *****************************************************************************/
int FltBox::DetailLevel2d() const
{
    return ( d_data->m_ui->kbxDetailLevel2d->value() );
}

/******************************************************************************
 * FltBox::setDetailLevel2d
 *****************************************************************************/
void FltBox::setDetailLevel2d( const int level )
{
    d_data->m_ui->kbxDetailLevel2d->blockSignals( true );
    d_data->m_ui->kbxDetailLevel2d->setValue( level );
    d_data->m_ui->kbxDetailLevel2d->blockSignals( false );

    emit FilterDetailLevelChanged( level );
}

/******************************************************************************
 * FltBox::DenoiseLevel2d
 *****************************************************************************/
int FltBox::DenoiseLevel2d() const
{
    return ( d_data->m_ui->kbxDenoiseLevel2d->value() );
}

/******************************************************************************
 * FltBox::setDenoiseLevel2d
 *****************************************************************************/
void FltBox::setDenoiseLevel2d( const int level )
{
    d_data->m_ui->kbxDenoiseLevel2d->blockSignals( true );
    d_data->m_ui->kbxDenoiseLevel2d->setValue( level );
    d_data->m_ui->kbxDenoiseLevel2d->blockSignals( false );

    emit FilterDenoiseLevelChanged( level );
}

/******************************************************************************
 * FltBox::Enable3d
 *****************************************************************************/
bool FltBox::Enable3d() const
{
    return ( (d_data->m_ui->cbxTfltEnable->checkState() == Qt::Checked) ? true : false );
}

/******************************************************************************
 * FltBox::setEnable3d
 *****************************************************************************/
void FltBox::setEnable3d( const bool enable )
{
    d_data->m_ui->cbxTfltEnable->blockSignals( true );
    d_data->m_ui->cbxTfltEnable->setCheckState( enable ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxTfltEnable->blockSignals( false );
    
    emit TfltEnableChanged( enable ? 1 : 0 );
}

/******************************************************************************
 * FltBox::DetailLevel3d
 *****************************************************************************/
int FltBox::DetailLevel3d() const
{
    return ( d_data->m_ui->kbxDetailLevel3d->value() );
}

/******************************************************************************
 * FltBox::setDetailLevel3d
 *****************************************************************************/
void FltBox::setDetailLevel3d( const int level )
{
    int max = FLT_3D_DETAIL_LEVEL_MAX;

    d_data->m_ui->kbxDetailLevel3d->blockSignals( true );
    d_data->m_ui->kbxDetailLevel3d->setValue( level );
    d_data->m_ui->kbxDetailLevel3d->blockSignals( false );
        
    if ( (level>=1) && (level <= FLT_3D_DETAIL_LEVEL_MAX) )
    {
        max -= level;
    }

    emit TfltMinMaxChanged( 0, max );
}

/******************************************************************************
 * FltBox::DenoiseLevel3d
 *****************************************************************************/
int FltBox::DenoiseLevel3d() const
{
    return ( d_data->m_ui->kbxDenoiseLevel3d->value() );
}

/******************************************************************************
 * FltBox::setDenoiseLevel3d
 *****************************************************************************/
void FltBox::setDenoiseLevel3d( const int level )
{
    d_data->m_ui->kbxDenoiseLevel2d->blockSignals( true );
    d_data->m_ui->kbxDenoiseLevel2d->setValue( level );
    d_data->m_ui->kbxDenoiseLevel2d->blockSignals( false );

    emit TfltDenoiseLevelChanged( level );
}

/******************************************************************************
 * FltBox::prepareMode
 *****************************************************************************/
void FltBox::prepareMode( const Mode )
{
    // do nothing here
}

/******************************************************************************
 * FltBox::loadSettings
 *****************************************************************************/
void FltBox::loadSettings( QSettings & s )
{
    s.beginGroup( FLT_SETTINGS_SECTION_NAME );

    setEnable2d( s.value( FLT_SETTINGS_2D_ENABLE ).toBool() );
    setDetailLevel2d( s.value( FLT_SETTINGS_2D_DETAIL_LEVEL ).toInt() );
    setDenoiseLevel2d( s.value( FLT_SETTINGS_2D_DENOISE_LEVEL ).toInt() );

    setEnable3d( s.value( FLT_SETTINGS_2D_ENABLE ).toBool() );
    setDetailLevel3d( s.value( FLT_SETTINGS_3D_DETAIL_LEVEL ).toInt() );
    setDenoiseLevel3d( s.value( FLT_SETTINGS_3D_DENOISE_LEVEL ).toInt() );

    s.endGroup();
}

/******************************************************************************
 * FltBox::saveSettings
 *****************************************************************************/
void FltBox::saveSettings( QSettings & s )
{
    s.beginGroup( FLT_SETTINGS_SECTION_NAME );

    s.setValue( FLT_SETTINGS_2D_ENABLE       , Enable2d() );
    s.setValue( FLT_SETTINGS_2D_DETAIL_LEVEL , DetailLevel2d() );
    s.setValue( FLT_SETTINGS_2D_DENOISE_LEVEL, DenoiseLevel2d() );

    s.setValue( FLT_SETTINGS_3D_ENABLE       , Enable3d() );
    s.setValue( FLT_SETTINGS_3D_DETAIL_LEVEL , DetailLevel3d() );
    s.setValue( FLT_SETTINGS_3D_DENOISE_LEVEL, DenoiseLevel3d() );

    s.endGroup();
}

/******************************************************************************
 * FltBox::applySettings
 *****************************************************************************/
void FltBox::applySettings( void )
{
    emit FilterEnableChanged( Enable2d() );
    emit FilterDetailLevelChanged( DetailLevel2d() );
    emit FilterDenoiseLevelChanged( DenoiseLevel2d() );

    emit TfltEnableChanged( Enable3d() );
    setDetailLevel3d( DetailLevel3d() );    // Call set method which will emit the signal after performing needed calculations
    emit TfltDenoiseLevelChanged( DetailLevel3d() );
}

/******************************************************************************
 * FltBox::onFilterEnableChange
 *****************************************************************************/
void FltBox::onFilterEnableChange( int value )
{
    d_data->m_ui->cbxEnable->blockSignals( true );
    d_data->m_ui->cbxEnable->setCheckState( value ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxEnable->blockSignals( false );
}

/******************************************************************************
 * FltBox::onFilterDetailLevelChange
 *****************************************************************************/
void FltBox::onFilterDetailLevelChange( int value )
{
    d_data->m_ui->kbxDetailLevel2d->blockSignals( true );
    d_data->m_ui->kbxDetailLevel2d->setValue( value );
    d_data->m_ui->kbxDetailLevel2d->blockSignals( false );
}

/******************************************************************************
 * FltBox::onFilterDenoiseLevelChange
 *****************************************************************************/
void FltBox::onFilterDenoiseLevelChange( int value )
{
    d_data->m_ui->kbxDenoiseLevel2d->blockSignals( true );
    d_data->m_ui->kbxDenoiseLevel2d->setValue( value );
    d_data->m_ui->kbxDenoiseLevel2d->blockSignals( false );
}

/******************************************************************************
 * FltBox::onFilterChange
 *****************************************************************************/
void FltBox::onFilterChange( int enable, int detail, int denoise )
{
    onFilterEnableChange( enable );
    onFilterDetailLevelChange( detail );
    onFilterDenoiseLevelChange( denoise );
}

/******************************************************************************
 * FltBox::onTfltEnableChange
 *****************************************************************************/
void FltBox::onTfltEnableChange( int value )
{
    d_data->m_ui->cbxTfltEnable->blockSignals( true );
    d_data->m_ui->cbxTfltEnable->setCheckState( value ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxTfltEnable->blockSignals( false );
}

/******************************************************************************
 * FltBox::onTfltDenoiseLevelChange
 *****************************************************************************/
void FltBox::onTfltDenoiseLevelChange( int value )
{
    d_data->m_ui->kbxDenoiseLevel3d->blockSignals( true );
    d_data->m_ui->kbxDenoiseLevel3d->setValue( value );
    d_data->m_ui->kbxDenoiseLevel3d->blockSignals( false );
}

/******************************************************************************
 * FltBox::onTfltMinMaxChange
 *****************************************************************************/
void FltBox::onTfltMinMaxChange( int min, int max )
{
    (void)min;
    d_data->m_ui->kbxDetailLevel3d->blockSignals( true );
    d_data->m_ui->kbxDetailLevel3d->setValue( FLT_3D_DETAIL_LEVEL_MAX - max );
    d_data->m_ui->kbxDetailLevel3d->blockSignals( false );
}

/******************************************************************************
 * FltBox::on2dEnableChange
 *****************************************************************************/
void FltBox::on2dEnableChange( int value )
{
    setWaitCursor();
    emit FilterEnableChanged( (Qt::Unchecked == value) ? 0 : 1 );
    setNormalCursor();
}

/******************************************************************************
 * FltBox::on3dEnableChange
 *****************************************************************************/
void FltBox::on3dEnableChange( int value )
{
    setWaitCursor();
    emit TfltEnableChanged( (Qt::Unchecked == value) ? 0 : 1 );
    setNormalCursor();
}

/******************************************************************************
 * FltBox::onObjectSpeedChange
 *****************************************************************************/
void FltBox::onObjectSpeedChange( int value )
{
    int max = FLT_3D_DETAIL_LEVEL_MAX;

    // inverted appearance
    if ( (value>=1) && (value <= FLT_3D_DETAIL_LEVEL_MAX) )
    {
        max -= value;    
    }

    setWaitCursor();
    emit TfltMinMaxChanged( 0, max );
    setNormalCursor();
}

