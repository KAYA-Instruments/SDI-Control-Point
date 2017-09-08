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
 * @file    wbbox.cpp
 *
 * @brief   Implementation of white balance setting box
 *
 *****************************************************************************/
#include <QtDebug>
#include <QTimer>

#include <simple_math/xyz2ct.h>
#include <common.h>

#include "wbbox.h"
#include "ui_wbbox.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_WbBox;
}

/******************************************************************************
 * local definitions
 *****************************************************************************/
#define FIX_PRECISION_S0216             ( 65536.0f )

#define WB_SETTINGS_SECTION_NAME        ( "WB" )

#define WB_SETTINGS_AWB_ENABLE          ( "awb" )
#define WB_SETTINGS_AWB_SPEED           ( "awb_speed" )

#define WB_SETTINGS_RED_GAIN            ( "red" )
#define WB_SETTINGS_GREEN_GAIN          ( "green" )
#define WB_SETTINGS_BLUE_GAIN           ( "blue" )
#define WB_SETTINGS_COLOR_TEMPERATURE   ( "ct" )

#define WB_SETTINGS_HUE                 ( "hue" )
#define WB_SETTINGS_SATURATION          ( "saturation" )
#define WB_SETTINGS_BRIGHTNESS          ( "brighness" )
#define WB_SETTINGS_CONTRAST            ( "contrast" )

/******************************************************************************
 * WbBox::PrivateData
 *****************************************************************************/
class WbBox::PrivateData
{
public:
    PrivateData()
        : m_ui( new Ui::UI_WbBox )
        , m_awb_enable( false )
        , m_awb_speed( 0 )
        , m_green_gain( 0 )
    {
        m_XYZ.resize( 9 );

        m_wb_timer =  new QTimer();
        m_wb_timer->setInterval( 2000 );
        m_wb_timer->setSingleShot( false );
    };

    ~PrivateData()
    {
        delete m_wb_timer;
        delete m_ui;
    };
    
    Ui::UI_WbBox *  m_ui;               /**< ui handle */
    
    bool            m_awb_enable;
    int             m_awb_speed;

    int             m_green_gain;       /**< green gain, normally not changeable by GUI,
                                             because of normalization to '1' by awb */
    QVector<float>  m_XYZ;
    QTimer *        m_wb_timer;         /**< white-balance timer to update gains*/
};

/******************************************************************************
 * WbBox::WbBox
 *****************************************************************************/
WbBox::WbBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData;

    // initialize UI
    d_data->m_ui->setupUi( this );

    ////////////////////
    // red gain
    ////////////////////

    // configure red gain
    d_data->m_ui->RedGain->setRange( WB_RED_GAIN_MIN, WB_RED_GAIN_MAX );
    d_data->m_ui->RedGain->setCommaPos( WB_RED_GAIN_COMMA_POSITION );
    d_data->m_ui->RedGain->setDisplayMultiplier( WB_RED_GAIN_DISPLAY_MULTIPLIER );
    d_data->m_ui->RedGain->setBase( WB_RED_GAIN_BASE );
    d_data->m_ui->RedGain->setMaxAngle( 180 );
    d_data->m_ui->RedGain->setMaxRounds( 4 );
    d_data->m_ui->RedGain->setFmt( WB_RED_GAIN_DISPLAY_MASK );

    ////////////////////
    // green gain
    ////////////////////

    // configure red gain
    d_data->m_ui->GreenGain->setRange( WB_GREEN_GAIN_MIN, WB_GREEN_GAIN_MAX );
    d_data->m_ui->GreenGain->setCommaPos( WB_GREEN_GAIN_COMMA_POSITION );
    d_data->m_ui->GreenGain->setDisplayMultiplier( WB_GREEN_GAIN_DISPLAY_MULTIPLIER );
    d_data->m_ui->GreenGain->setBase( WB_GREEN_GAIN_BASE );
    d_data->m_ui->GreenGain->setMaxAngle( 180 );
    d_data->m_ui->GreenGain->setMaxRounds( 4 );
    d_data->m_ui->GreenGain->setFmt( WB_GREEN_GAIN_DISPLAY_MASK );

    ////////////////////
    // blue gain
    ////////////////////

    // configure blue gain
    d_data->m_ui->BlueGain->setRange( WB_BLUE_GAIN_MIN, WB_BLUE_GAIN_MAX );
    d_data->m_ui->BlueGain->setCommaPos( WB_BLUE_GAIN_COMMA_POSITION );
    d_data->m_ui->BlueGain->setDisplayMultiplier( WB_BLUE_GAIN_DISPLAY_MULTIPLIER );
    d_data->m_ui->BlueGain->setBase( WB_BLUE_GAIN_BASE );
    d_data->m_ui->BlueGain->setMaxAngle( 180 );
    d_data->m_ui->BlueGain->setMaxRounds( 4 );
    d_data->m_ui->BlueGain->setFmt( WB_BLUE_GAIN_DISPLAY_MASK );

    ////////////////////
    // awb speed
    ////////////////////

    d_data->m_ui->sldAwbSpeed->setRange( 0, 2 );
    d_data->m_ui->sldAwbSpeed->setPageStep( 1 );

    /* NOTE: Color Temperature is not implemented yet */
    ////////////////////
    // color temperature
    ////////////////////

    // configure color temperature
//    d_data->m_ui->ColorTemperature->setRange( WB_COLOR_TEMPERATURE_MIN, WB_COLOR_TEMPERATURE_MAX );
//    d_data->m_ui->ColorTemperature->setCommaPos( WB_COLOR_TEMPERATURE_COMMA_POSITION );
//    d_data->m_ui->ColorTemperature->setDisplayMultiplier( WB_COLOR_TEMPERATURE_DISPLAY_MULTIPLIER );
//    d_data->m_ui->ColorTemperature->setBase( WB_COLOR_TEMPERATURE_BASE );
//    d_data->m_ui->ColorTemperature->setMaxAngle( 180 );
//    d_data->m_ui->ColorTemperature->setMaxRounds( 4 );
//    d_data->m_ui->ColorTemperature->setFmt( WB_COLOR_TEMPERATURE_DISPLAY_MASK );
//    d_data->m_ui->ColorTemperature->setVisible( false );
//    d_data->m_ui->ColorTemperature->setDisabled( true );

    ////////////////////
    // hue
    ////////////////////

    // configure hue 
    d_data->m_ui->Hue->setRange( WB_HUE_MIN, WB_HUE_MAX );
    d_data->m_ui->Hue->setCommaPos( WB_HUE_COMMA_POSITION );
    d_data->m_ui->Hue->setDisplayMultiplier( WB_HUE_DISPLAY_MULTIPLIER );
    d_data->m_ui->Hue->setBase( WB_HUE_BASE );
    d_data->m_ui->Hue->setMaxAngle( 150 );
    d_data->m_ui->Hue->setMaxRounds( 1 );
    d_data->m_ui->Hue->setFmt( WB_HUE_DISPLAY_MASK );

    ////////////////////
    // saturation
    ////////////////////

    // configure saturation
    d_data->m_ui->Saturation->setRange( WB_SATURATION_MIN, WB_SATURATION_MAX );
    d_data->m_ui->Saturation->setCommaPos( WB_SATURATION_COMMA_POSITION );
    d_data->m_ui->Saturation->setDisplayMultiplier( WB_SATURATION_DISPLAY_MULTIPLIER );
    d_data->m_ui->Saturation->setBase( WB_SATURATION_BASE );
    d_data->m_ui->Saturation->setMaxAngle( 150 );
    d_data->m_ui->Saturation->setMaxRounds( 1 );
    d_data->m_ui->Saturation->setFmt( WB_SATURATION_DISPLAY_MASK );

    ////////////////////
    // brightness
    ////////////////////

    // configure brightness
    d_data->m_ui->Brightness->setRange( WB_BRIGHTNESS_MIN, WB_BRIGHTNESS_MAX );
    d_data->m_ui->Brightness->setCommaPos( WB_BRIGHTNESS_COMMA_POSITION );
    d_data->m_ui->Brightness->setDisplayMultiplier( WB_BRIGHTNESS_DISPLAY_MULTIPLIER );
    d_data->m_ui->Brightness->setBase( WB_BRIGHTNESS_BASE );
    d_data->m_ui->Brightness->setMaxAngle( 150 );
    d_data->m_ui->Brightness->setMaxRounds( 1 );
    d_data->m_ui->Brightness->setFmt( WB_BRIGHTNESS_DISPLAY_MASK );

    ////////////////////
    // contrast 
    ////////////////////

    // configure contrast
    d_data->m_ui->Contrast->setRange( WB_CONTRAST_MIN, WB_CONTRAST_MAX );
    d_data->m_ui->Contrast->setCommaPos( WB_CONTRAST_COMMA_POSITION );
    d_data->m_ui->Contrast->setDisplayMultiplier( WB_CONTRAST_DISPLAY_MULTIPLIER );
    d_data->m_ui->Contrast->setBase( WB_CONTRAST_BASE );
    d_data->m_ui->Contrast->setMaxAngle( 150 );
    d_data->m_ui->Contrast->setMaxRounds( 1 );
    d_data->m_ui->Contrast->setFmt( WB_CONTRAST_DISPLAY_MASK );

    ////////////////////
    // connect signals
    ////////////////////
    
    connect( d_data->m_ui->WbButtonBox, SIGNAL(buttonClicked(int)), this, SLOT(onWbButtonClick(int)) );

    // connect cintinnouse white balance
    connect( d_data->m_ui->cbxAwbEnable, SIGNAL(stateChanged(int)), this, SLOT(onAwbEnableClick(int)) );
    connect( d_data->m_ui->sldAwbSpeed, SIGNAL(valueChanged(int)), this, SLOT(onAwbSpeedSliderChange(int)));

    // connect gain signals
    connect( d_data->m_ui->RedGain   , SIGNAL(ValueChanged(int)), this, SIGNAL(RedGainChanged(int)) );
    connect( d_data->m_ui->GreenGain , SIGNAL(ValueChanged(int)), this, SIGNAL(GreenGainChanged(int)) );
    connect( d_data->m_ui->BlueGain  , SIGNAL(ValueChanged(int)), this, SIGNAL(BlueGainChanged(int)) );

    // connect color processing signals
    connect( d_data->m_ui->Hue       , SIGNAL(ValueChanged(int)), this, SIGNAL(HueChanged(int)) );
    connect( d_data->m_ui->Saturation, SIGNAL(ValueChanged(int)), this, SIGNAL(SaturationChanged(int)) );
    connect( d_data->m_ui->Brightness, SIGNAL(ValueChanged(int)), this, SIGNAL(BrightnessChanged(int)) );
    connect( d_data->m_ui->Contrast  , SIGNAL(ValueChanged(int)), this, SIGNAL(ContrastChanged(int)) );

    ////////////////////
    // statistic timer
    ////////////////////
    connect( d_data->m_wb_timer, SIGNAL(timeout()), this, SLOT(onWbUpdate()) );

    ////////////////////
    // operation mode
    ////////////////////
    prepareMode( mode() ); 
}

/******************************************************************************
 * WbBox::~WbBox
 *****************************************************************************/
WbBox::~WbBox()
{
    delete d_data;
}

/******************************************************************************
 * WbBox::RedGain
 *****************************************************************************/
int WbBox::RedGain() const
{
    return ( d_data->m_ui->RedGain->value() );
}

/******************************************************************************
 * WbBox::setRedGain
 *****************************************************************************/
void WbBox::setRedGain( const int gain )
{
    // set red gain (event change event is blocked internally)
    d_data->m_ui->RedGain->onValueChange( gain );

    // emmit value change event
    emit RedGainChanged( gain );
}

/******************************************************************************
 * WbBox::GreenGain
 *****************************************************************************/
int WbBox::GreenGain() const
{
    if ( d_data->m_ui->GreenGain->isEnabled() )
    {
        return ( d_data->m_ui->GreenGain->value() );
    }
    else
    {
        return ( d_data->m_green_gain );
    }
}

/******************************************************************************
 * WbBox::setGreenGain
 *****************************************************************************/
void WbBox::setGreenGain( const int gain )
{
    if ( d_data->m_ui->GreenGain->isEnabled() )
    {
        // set red gain (event change event is blocked internally)
        d_data->m_ui->GreenGain->onValueChange( gain );

        // emmit value change event
        emit GreenGainChanged( gain );
    }
    else
    {
        d_data->m_green_gain = gain;
        emit GreenGainChanged( gain );
    }
}

/******************************************************************************
 * WbBox::BlueGain
 *****************************************************************************/
int WbBox::BlueGain() const
{
    return ( d_data->m_ui->BlueGain->value() );
}

/******************************************************************************
 * WbBox::setBlueGain
 *****************************************************************************/
void WbBox::setBlueGain( const int gain )
{
    // set blue gain (event change event is blocked internally)
    d_data->m_ui->BlueGain->onValueChange( gain );

    // emmit value change event
    emit BlueGainChanged( gain );
}

/******************************************************************************
 * WbBox::AwbEnable
 *****************************************************************************/
bool WbBox::AwbEnable() const
{
    return ( d_data->m_ui->cbxAwbEnable->isChecked() );
}

/******************************************************************************
 * WbBox::setAwbEnable
 *****************************************************************************/
void WbBox::setAwbEnable( const bool enable )
{
    // set enable value of checkbox (creates a change event => block it)
    d_data->m_ui->cbxAwbEnable->blockSignals( true );
    d_data->m_ui->cbxAwbEnable->setCheckState( enable ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxAwbEnable->blockSignals( false );

    // raise the change event
    emit AwbEnableChanged( enable );
}

/******************************************************************************
 * WbBox::AwbSpeed
 *****************************************************************************/
int WbBox::AwbSpeed() const
{
    return ( d_data->m_ui->sldAwbSpeed->value() );
}

/******************************************************************************
 * WbBox::setAwbSpeed
 *****************************************************************************/
void WbBox::setAwbSpeed( const int value )
{
    QString s;
    s.sprintf( "%d", value );
    d_data->m_ui->letAwbSpeed->setText( s );

    // set speed value of slider (creates a change event => block it)
    d_data->m_ui->sldAwbSpeed->blockSignals( true );
    d_data->m_ui->sldAwbSpeed->setValue( value );
    d_data->m_ui->sldAwbSpeed->blockSignals( false );

    // raise the change event
    emit AwbSpeedChanged( value );
}

/******************************************************************************
 * WbBox::ColorTemperature
 *****************************************************************************/
//int WbBox::ColorTemperature() const
//{
//    return ( d_data->m_ui->ColorTemperature->value() );
//}

/******************************************************************************
 * WbBox::Hue
 *****************************************************************************/
int WbBox::Hue() const
{
    return ( d_data->m_ui->Hue->value() );
}

/******************************************************************************
 * WbBox::setHue
 *****************************************************************************/
void WbBox::setHue( const int hue )
{
    d_data->m_ui->Hue->onValueChange( hue );
    emit HueChanged( hue );
}

/******************************************************************************
 * WbBox::Saturation
 *****************************************************************************/
int WbBox::Saturation() const
{
    return ( d_data->m_ui->Saturation->value() );
}

/******************************************************************************
 * WbBox::setSaturation
 *****************************************************************************/
void WbBox::setSaturation( const int saturation )
{
    d_data->m_ui->Saturation->onValueChange( saturation );
    emit SaturationChanged( saturation );
}

/******************************************************************************
 * WbBox::Brightness
 *****************************************************************************/
int WbBox::Brightness() const
{
    return ( d_data->m_ui->Brightness->value() );
}

/******************************************************************************
 * WbBox::setBrightness
 *****************************************************************************/
void WbBox::setBrightness( const int brightness )
{
    d_data->m_ui->Brightness->onValueChange( brightness );
    emit BrightnessChanged( brightness );
}

/******************************************************************************
 * WbBox::Contrast
 *****************************************************************************/
int WbBox::Contrast() const
{
    return ( d_data->m_ui->Contrast->value() );
}

/******************************************************************************
 * WbBox::setContrast
 *****************************************************************************/
void WbBox::setContrast( const int contrast )
{
    d_data->m_ui->Contrast->onValueChange( contrast );
    emit ContrastChanged( contrast );
}

/******************************************************************************
 * WbBox::prepareMode
 *****************************************************************************/
void WbBox::prepareMode( const Mode )
{
    // do nothing here
}

/******************************************************************************
 * WbBox::loadSettings
 *****************************************************************************/
void WbBox::loadSettings( QSettings & s )
{
    s.beginGroup( WB_SETTINGS_SECTION_NAME );

    // continnouse white-balancing enable
    setAwbEnable( s.value( WB_SETTINGS_AWB_ENABLE ).toBool() );
    setAwbSpeed( s.value( WB_SETTINGS_AWB_SPEED ).toInt() );

    setRedGain( s.value( WB_SETTINGS_RED_GAIN ).toInt() );
    setGreenGain( s.value( WB_SETTINGS_GREEN_GAIN ).toInt() );
    setBlueGain( s.value( WB_SETTINGS_BLUE_GAIN ).toInt() );
    
    setHue( s.value( WB_SETTINGS_HUE ).toInt() );
    setSaturation( s.value( WB_SETTINGS_SATURATION ).toInt() );
    setBrightness( s.value( WB_SETTINGS_BRIGHTNESS ).toInt() );
    setContrast( s.value( WB_SETTINGS_CONTRAST ).toInt() );
   
    s.endGroup();
}

/******************************************************************************
 * WbBox::saveSettings
 *****************************************************************************/
void WbBox::saveSettings( QSettings & s )
{
    s.beginGroup( WB_SETTINGS_SECTION_NAME );

    s.setValue( WB_SETTINGS_AWB_ENABLE       , AwbEnable() );
    s.setValue( WB_SETTINGS_AWB_SPEED        , AwbSpeed() );
    s.setValue( WB_SETTINGS_RED_GAIN         , RedGain() );
    s.setValue( WB_SETTINGS_GREEN_GAIN       , GreenGain() );
    s.setValue( WB_SETTINGS_BLUE_GAIN        , BlueGain() );
//    s.setValue( WB_SETTINGS_COLOR_TEMPERATURE, ColorTemperature() );
    s.setValue( WB_SETTINGS_HUE              , Hue() );
    s.setValue( WB_SETTINGS_SATURATION       , Saturation() );
    s.setValue( WB_SETTINGS_BRIGHTNESS       , Brightness() );
    s.setValue( WB_SETTINGS_CONTRAST         , Contrast() );

    s.endGroup();
}

/******************************************************************************
 * WbBox::applySettings
 *****************************************************************************/
void WbBox::applySettings( void )
{
    emit AwbEnableChanged( AwbEnable() );
    emit AwbSpeedChanged( AwbSpeed() );

    emit RedGainChanged( RedGain() );
    emit GreenGainChanged( GreenGain() );
    emit BlueGainChanged( BlueGain() );

    emit HueChanged( Hue() );
    emit SaturationChanged( Saturation() );
    emit BrightnessChanged( Brightness() );
    emit ContrastChanged( Contrast() );
}

/******************************************************************************
 * WbBox::setWhiteBalanceSettingsVisible
 *****************************************************************************/
void WbBox::setWhiteBalanceSettingsVisible(const bool awbVisible, const bool gainVisible, const bool greenGainVisible )
{
    // Enable WB Button Box (contains WB Presets), if awb settings shall be visible
    d_data->m_ui->WbButtonBox->setVisible(awbVisible);

    // Enable WB Settings Group box, if either awb or gain shall be visible
    d_data->m_ui->WbSettings->setVisible(awbVisible || gainVisible);

    // Enable AWB Settings and Color Temperature if awb shall be visible
    d_data->m_ui->gbxAwb->setVisible(awbVisible);
//    d_data->m_ui->ColorTemperature->setVisible(awbVisible);

    // Enable Red and Blue Gain Knobs, if gain shall be visible
    d_data->m_ui->RedGain->setVisible(gainVisible);
    d_data->m_ui->GreenGain->setVisible(greenGainVisible);
    d_data->m_ui->BlueGain->setVisible(gainVisible);
}

/******************************************************************************
 * WbBox::setColorProcessingSettingsVisible
 *****************************************************************************/
void WbBox::setColorProcessingSettingsVisible(const bool hueVisible )
{
    // Set Hue knob visibility
    d_data->m_ui->Hue->setVisible( hueVisible );

    /* Note: At the moment only Hue can be hidden, since all devices support the
     * other color processing settings */
}

/******************************************************************************
 * WbBox::onNoWbPresetsChange
 *****************************************************************************/
void WbBox::onNoWbPresetsChange( int value )
{
    (void)value;
}

/******************************************************************************
 * WbBox::onNoWbPresetsChange
 *****************************************************************************/
void WbBox::onWbPresetsChange( int id, QString name, int ct )
{
    if ( id == 0 )
    {
        d_data->m_ui->WbButtonBox->deleteButtons();
        d_data->m_ui->WbButtonBox->addButton( "Oneshot", QPixmap(":/images/wb/wb.png") );
    }

    QString s;
    s.sprintf( "%s(%dK)", name.toStdString().c_str(), ct );

    switch ( ct )
    {
        case 2200:
            d_data->m_ui->WbButtonBox->addButton( s, QPixmap(":/images/wb/sunset.png") );
            break;

        case 2700:
            d_data->m_ui->WbButtonBox->addButton( s, QPixmap(":/images/wb/candle.png") );
            break;

        case 3700:
            d_data->m_ui->WbButtonBox->addButton( s, QPixmap(":/images/wb/tungsten.png") );
            break;

        case 4000:
            d_data->m_ui->WbButtonBox->addButton( s, QPixmap(":/images/wb/fluorescent.png") );
            break;
        
        case 5000:
            d_data->m_ui->WbButtonBox->addButton( s, QPixmap(":/images/wb/sun.png") );
            break;
        
        case 6200:
            d_data->m_ui->WbButtonBox->addButton( s, QPixmap(":/images/wb/cloudy.png") );
            break;
        
        case 6500:
            d_data->m_ui->WbButtonBox->addButton( s, QPixmap(":/images/wb/cloudy.png") );
            break;

        case 7500:
            d_data->m_ui->WbButtonBox->addButton( s, QPixmap(":/images/wb/shadow.png") );
            break;

        default:
            d_data->m_ui->WbButtonBox->addButton( s );
            break;
    }
}

/******************************************************************************
 * WbBox::onAwbEnableChange
 *****************************************************************************/
void WbBox::onAwbEnableChange( const int value )
{
    // set internal state
    d_data->m_awb_enable = ( value ) ? true : false;

    // set value of checkbox
    d_data->m_ui->cbxAwbEnable->blockSignals( true );
    d_data->m_ui->cbxAwbEnable->setCheckState( value ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxAwbEnable->blockSignals( false );
}

/******************************************************************************
 * WbBox::onAwbSpeedChange
 *****************************************************************************/
void WbBox::onAwbSpeedChange( int value )
{
    QString s;
    s.sprintf( "%d", value );
    d_data->m_ui->letAwbSpeed->setText( s );

    d_data->m_ui->sldAwbSpeed->blockSignals( true );
    d_data->m_ui->sldAwbSpeed->setValue( value );
    d_data->m_ui->sldAwbSpeed->blockSignals( false );
}

/******************************************************************************
 * WbBox::onRedGainChange
 *****************************************************************************/
void WbBox::onRedGainChange( int value )
{
    d_data->m_ui->RedGain->blockSignals( true );
    d_data->m_ui->RedGain->onValueChange( value );
    d_data->m_ui->RedGain->blockSignals( false );
}

/******************************************************************************
 * WbBox::onGreenGainChange
 *****************************************************************************/
void WbBox::onGreenGainChange( int value )
{
    if ( d_data->m_ui->GreenGain->isEnabled() )
    {
        d_data->m_ui->GreenGain->blockSignals( true );
        d_data->m_ui->GreenGain->onValueChange( value );
        d_data->m_ui->GreenGain->blockSignals( false );
    }
    else
    {
        d_data->m_green_gain = value;
    }
}

/******************************************************************************
 * WbBox::onBlueGainChange
 *****************************************************************************/
void WbBox::onBlueGainChange( int value )
{
    d_data->m_ui->BlueGain->blockSignals( true );
    d_data->m_ui->BlueGain->onValueChange( value );
    d_data->m_ui->BlueGain->blockSignals( false );
}

/******************************************************************************
 * WbBox::onColorTemperatureChange
 *****************************************************************************/
//void WbBox::onColorTemperatureChange( int value )
//{
//    d_data->m_ui->ColorTemperature->blockSignals( true );
//    d_data->m_ui->ColorTemperature->onValueChange( value );
//    d_data->m_ui->ColorTemperature->blockSignals( false );
//}

/******************************************************************************
 * WbBox::onHueChange
 *****************************************************************************/
void WbBox::onHueChange( int value )
{
    d_data->m_ui->Hue->blockSignals( true );
    d_data->m_ui->Hue->onValueChange( value );
    d_data->m_ui->Hue->blockSignals( false );
}

/******************************************************************************
 * WbBox::onSaturationChange
 *****************************************************************************/
void WbBox::onSaturationChange( int value )
{
    d_data->m_ui->Saturation->blockSignals( true );
    d_data->m_ui->Saturation->onValueChange( value );
    d_data->m_ui->Saturation->blockSignals( false );
}

/******************************************************************************
 * WbBox::onBrightnessChange
 *****************************************************************************/
void WbBox::onBrightnessChange( int value )
{
    d_data->m_ui->Brightness->blockSignals( true );
    d_data->m_ui->Brightness->onValueChange( value );
    d_data->m_ui->Brightness->blockSignals( false );
}

/******************************************************************************
 * WbBox::onContrastChange
 *****************************************************************************/
void WbBox::onContrastChange( int value )
{
    d_data->m_ui->Contrast->blockSignals( true );
    d_data->m_ui->Contrast->onValueChange( value );
    d_data->m_ui->Contrast->blockSignals( false );
}

/******************************************************************************
 * WbBox::onStatRGBChange
 *****************************************************************************/
void WbBox::onStatRGBChange( int red, int green, int blue )
{
    (void) red;
    (void) green;
    (void) blue;
    // qDebug( "red=%d, green=%d, blue=%d", red, green, blue );
}

/******************************************************************************
 * WbBox::onStatXYZChange
 *****************************************************************************/
void WbBox::onStatXYZChange( int x, int y, int z )
{
    double XYZ[3];
    double ct;
    int res;

    XYZ[0] = (double)x;
    XYZ[1] = (double)y;
    XYZ[2] = (double)z;
    ct = 0.0f;
    res = XYZtoCorColorTemp_Robertson( XYZ, &ct );
    if ( ! res )
    {
//        d_data->m_ui->ColorTemperature->onValueChange( (int)ct );
    }
    // qDebug( "ct=%d, X=%d, Y=%d, Z=%d", (int)ct, x, y, z );
}

/******************************************************************************
 * WbBox::onColorXYZChange
 *****************************************************************************/
void WbBox::onColorXYZChange( int c0, int c1, int c2,
                              int c3, int c4, int c5,
                              int c6, int c7, int c8 )
{
    d_data->m_XYZ[0] = ((float)c0) / FIX_PRECISION_S0216;
    d_data->m_XYZ[1] = ((float)c1) / FIX_PRECISION_S0216;
    d_data->m_XYZ[2] = ((float)c2) / FIX_PRECISION_S0216;
    d_data->m_XYZ[3] = ((float)c3) / FIX_PRECISION_S0216;
    d_data->m_XYZ[4] = ((float)c4) / FIX_PRECISION_S0216;
    d_data->m_XYZ[5] = ((float)c5) / FIX_PRECISION_S0216;
    d_data->m_XYZ[6] = ((float)c6) / FIX_PRECISION_S0216;
    d_data->m_XYZ[7] = ((float)c7) / FIX_PRECISION_S0216;
    d_data->m_XYZ[8] = ((float)c8) / FIX_PRECISION_S0216;

    // qDebug( "c0=%6.2f, c1=%6.2f, c2=%6.2f", d_data->m_XYZ[0], d_data->m_XYZ[1], d_data->m_XYZ[2] );
}

/******************************************************************************
 * WbBox::onWbButtonClick
 *****************************************************************************/
void WbBox::onWbButtonClick( int value )
{
    if ( value == 0 )
    {
        emit WbChanged();
    }
    else
    {
        emit WbPresetChanged( (value - 1) );
    }
}

/******************************************************************************
 * WbBox::onAwbSpeedSliderChange
 *****************************************************************************/
void WbBox::onAwbEnableClick( int value )
{
    if ( value )
    {
        d_data->m_awb_enable = true;
        d_data->m_wb_timer->start();
    }
    else
    {
        d_data->m_awb_enable = false;
        d_data->m_wb_timer->stop();
    }

    emit AwbEnableChanged( value );
}

/******************************************************************************
 * WbBox::onAwbSpeedSliderChange
 *****************************************************************************/
void WbBox::onAwbSpeedSliderChange( int value )
{
    QString s;
    s.sprintf( "%d", value );
    d_data->m_ui->letAwbSpeed->setText( s );

    d_data->m_awb_speed = value;

    emit AwbSpeedChanged( value );
}

/******************************************************************************
 * WbBox::onStatisticUpdate
 *****************************************************************************/
void WbBox::onWbUpdate()
{
    emit WbUpdate();
}

/******************************************************************************
 * WbBox::enterEvent
 *****************************************************************************/
void WbBox::enterEvent( QEvent * )
{
    if ( d_data->m_awb_enable )
    {
        d_data->m_wb_timer->start();
    }
} 

/******************************************************************************
 * WbBox::leaveEvent
 *****************************************************************************/
void WbBox::leaveEvent( QEvent * )
{
    d_data->m_wb_timer->stop();
} 


