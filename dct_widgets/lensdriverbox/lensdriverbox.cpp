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
 * @file    LensDriverBox.cpp
 *
 * @brief   Implementation of lens driver box
 *
 *****************************************************************************/
#include <QtDebug>
#include <QProxyStyle>

#include "lensdriverbox.h"
#include "ui_lensdriverbox.h"
#include "defines.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_LensDriverBox;
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
    SpinBoxStyle( QStyle *style = nullptr ) : QProxyStyle(style) { }

    int styleHint
    (
        StyleHint hint,
        const QStyleOption * option = nullptr,
        const QWidget * widget = nullptr,
        QStyleHintReturn * returnData = nullptr
    ) const Q_DECL_OVERRIDE
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
#define LENSDRIVER_SETTINGS_SECTION_NAME            ( "LENSDRIVER" )

#define LENSDRIVER_SETTINGS_SETTINGS                ( "settings" )

#define LENSDRIVER_SETTINGS_FOCUS_POSITION          ( "focus_position" )
#define LENSDRIVER_SETTINGS_ZOOM_POSITION           ( "zoom_position" )
#define LENSDRIVER_SETTINGS_IRIS_POSITION           ( "iris_position" )
#define LENSDRIVER_SETTINGS_FILTER_POSITION         ( "filter_position" )

#define LENSDRIVER_SETTINGS_FOCUS_SETTINGS          ( "focus_settings" )
#define LENSDRIVER_SETTINGS_ZOOM_SETTINGS           ( "zoom_settings" )
#define LENSDRIVER_SETTINGS_IRIS_SETTINGS           ( "iris_settings" )
#define LENSDRIVER_SETTINGS_FILTER_SETTINGS         ( "filter_settings" )

const lens_settings_t settingsUnkown {
    /* .address =             */ 0,
    /* .chipID =              */ 0,
    /* .controllerFeatures =  */ 0,
    /* .focusMotorNr =        */ 0,
    /* .zoomMotorNr =         */ 0,
    /* .irisMotorNr =         */ 0,
    /* .filterMotorNr =       */ 0,
    /* .focusMotorFeatures =  */ 0,
    /* .zoomMotorFeatures =   */ 0,
    /* .irisMotorFeatures =   */ 0,
    /* .filterMotorFeatures = */ 0,
};

const lens_settings_t settingsICS {
    /* .address =             */ 32,
    /* .chipID =              */ 13,
    /* .controllerFeatures =  */ 7,
    /* .focusMotorNr =        */ 0,
    /* .zoomMotorNr =         */ 1,
    /* .irisMotorNr =         */ 2,
    /* .filterMotorNr =       */ 3,
    /* .focusMotorFeatures =  */ 3,
    /* .zoomMotorFeatures =   */ 3,
    /* .irisMotorFeatures =   */ 3,
    /* .filterMotorFeatures = */ 0,
};

const lens_settings_t settingsDctKit {
    /* .address =             */ 8,
    /* .chipID =              */ 18,
    /* .controllerFeatures =  */ 1,
    /* .focusMotorNr =        */ 1,
    /* .zoomMotorNr =         */ 2,
    /* .irisMotorNr =         */ 0,
    /* .filterMotorNr =       */ 3,
    /* .focusMotorFeatures =  */ 0,
    /* .zoomMotorFeatures =   */ 0,
    /* .irisMotorFeatures =   */ 11,
    /* .filterMotorFeatures = */ 0,
};

const lens_settings_t lensSettingProfiles[LensProfileMax] =
{
    settingsUnkown,
    settingsICS,
    settingsDctKit
};

/******************************************************************************
 * LensDriverBox::PrivateData
 *****************************************************************************/
class LensDriverBox::PrivateData
{
public:
    PrivateData()
        : m_ui( new Ui::UI_LensDriverBox )
        , m_cntEvents( 0 )
        , m_maxEvents( 5 )
        , m_sbxStyle( new SpinBoxStyle() )
        , m_LensSettings{}
    {
        // do nothing
    }

    ~PrivateData()
    {
        delete m_ui;
        delete m_sbxStyle;
    }

    Ui::UI_LensDriverBox *  m_ui;           /**< ui handle */
    int                     m_cntEvents;    /**< ignore move-events if slider moving */
    int                     m_maxEvents;    /**< number of ignored move-events */
    SpinBoxStyle *          m_sbxStyle;     /**< proxy style to overrule not implemented spinbox properties */

    lens_settings_t         m_LensSettings;
};

/******************************************************************************
 * LensDriverBox::LensDriverBox
 *****************************************************************************/
LensDriverBox::LensDriverBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData;

    // initialize UI
    d_data->m_ui->setupUi( this );

    // overrule auto-repeat threshold
    d_data->m_ui->sbxFocusPosition->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxFocusSpeed->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxFocusStepMode->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxFocusTorque->setStyle( d_data->m_sbxStyle );

    d_data->m_ui->sbxZoomPosition->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxZoomSpeed->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxZoomStepMode->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxZoomTorque->setStyle( d_data->m_sbxStyle );

    d_data->m_ui->sbxIrisPosition->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxIrisSpeed->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxIrisStepMode->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxIrisTorque->setStyle( d_data->m_sbxStyle );

    d_data->m_ui->sbxFilterPosition->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxFilterSpeed->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxFilterStepMode->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxFilterTorque->setStyle( d_data->m_sbxStyle );

    // fill settings combo box
    for ( int i = LensProfileFirst; i < LensProfileMax; i++ )
    {
        addLensProfile( GetLensProfileName( static_cast<enum LensProfile>(i) ), i );
    }

    // connect internal signals

    ////////////////////
    // operation mode
    ////////////////////
    prepareMode( mode() ); 
}

/******************************************************************************
 * LensDriverBox::~LensDriverBox
 *****************************************************************************/
LensDriverBox::~LensDriverBox()
{
    delete d_data;
}

/******************************************************************************
 * LensDriverBox::prepareMode
 *****************************************************************************/
void LensDriverBox::prepareMode( const Mode mode )
{
    (void)mode;
}

/******************************************************************************
 * LensDriverBox::loadSettings
 *****************************************************************************/
void LensDriverBox::loadSettings( QSettings & s )
{
    s.beginGroup( LENSDRIVER_SETTINGS_SECTION_NAME );
    s.endGroup();
}

/******************************************************************************
 * LensDriverBox::saveSettings
 *****************************************************************************/
void LensDriverBox::saveSettings( QSettings & s )
{
    s.beginGroup( LENSDRIVER_SETTINGS_SECTION_NAME );
    s.endGroup();
}

/******************************************************************************
 * LensDriverBox::applySettings
 *****************************************************************************/
void LensDriverBox::applySettings( void )
{
}

/******************************************************************************
 * LensDriverBox::profileToSettings
 *****************************************************************************/
lens_settings_t LensDriverBox::profileToSettings( enum LensProfile profile )
{
    return lensSettingProfiles[profile];
}

/******************************************************************************
 * LensDriverBox::settingsToProfile
 *****************************************************************************/
enum LensProfile LensDriverBox::settingsToProfile( lens_settings_t settings )
{
    for ( int i = 0; i < LensProfileMax; i++ )
    {
        if ( settings.address == lensSettingProfiles[i].address &&
             settings.chipID == lensSettingProfiles[i].address &&
             settings.controllerFeatures == lensSettingProfiles[i].address &&
             settings.focusMotorNr == lensSettingProfiles[i].address &&
             settings.zoomMotorNr == lensSettingProfiles[i].zoomMotorNr &&
             settings.irisMotorNr == lensSettingProfiles[i].irisMotorNr &&
             settings.filterMotorNr == lensSettingProfiles[i].filterMotorNr &&
             settings.focusMotorFeatures == lensSettingProfiles[i].focusMotorFeatures &&
             settings.zoomMotorFeatures == lensSettingProfiles[i].zoomMotorFeatures &&
             settings.irisMotorFeatures == lensSettingProfiles[i].irisMotorFeatures &&
             settings.filterMotorFeatures == lensSettingProfiles[i].filterMotorFeatures )
        {
            return static_cast<enum LensProfile>(i);
        }
    }

    return LensProfileUnknown;
}

/******************************************************************************
 * LensDriverBox::addLensProfile
 *****************************************************************************/
void LensDriverBox::addLensProfile( QString name, int id )
{
    d_data->m_ui->cbxLensProfile->blockSignals( true );
    d_data->m_ui->cbxLensProfile->addItem( name, id );
    d_data->m_ui->cbxLensProfile->blockSignals( false );
}

/******************************************************************************
 * LensDriverBox::LensProfile
 *****************************************************************************/
QString LensDriverBox::LensProfile() const
{
    return ( d_data->m_ui->cbxLensProfile->currentText() );
}

/******************************************************************************
 * LensDriverBox::setLensProfile
 *****************************************************************************/
void LensDriverBox::setLensProfile( const QString mode )
{
    int index = d_data->m_ui->cbxLensProfile->findText( mode );
    if ( index != -1 )
    {
        d_data->m_ui->cbxLensProfile->blockSignals( true );
        d_data->m_ui->cbxLensProfile->setCurrentIndex( index );
        d_data->m_ui->cbxLensProfile->blockSignals( false );

        QVector<int> values;

        lens_settings_t settings = profileToSettings(static_cast<enum LensProfile>(index));

        values.append(settings.address);
        values.append(settings.chipID);
        values.append(settings.controllerFeatures);
        values.append(settings.focusMotorNr);
        values.append(settings.zoomMotorNr);
        values.append(settings.irisMotorNr);
        values.append(settings.filterMotorNr);
        values.append(settings.focusMotorFeatures);
        values.append(settings.zoomMotorFeatures);
        values.append(settings.irisMotorFeatures);
        values.append(settings.filterMotorFeatures);

        emit LensSettingsChanged( values );
    }
}

/******************************************************************************
 * LensDriverBox::onLensSettingsChange
 *****************************************************************************/
void LensDriverBox::onLensSettingsChange( QVector<int> values )
{
    lens_settings_t settings;
    settings.address             = values[0];
    settings.chipID              = values[1];
    settings.controllerFeatures  = values[2];
    settings.focusMotorNr        = values[3];
    settings.zoomMotorNr         = values[4];
    settings.irisMotorNr         = values[5];
    settings.filterMotorNr       = values[6];
    settings.focusMotorFeatures  = values[7];
    settings.zoomMotorFeatures   = values[8];
    settings.irisMotorFeatures   = values[9];
    settings.filterMotorFeatures = values[10];

    d_data->m_ui->cbxLensProfile->blockSignals( true );
    d_data->m_ui->cbxLensProfile->setCurrentIndex( settingsToProfile(settings) );
    d_data->m_ui->cbxLensProfile->blockSignals( false );
}

/******************************************************************************
 * LensDriverBox::onCbxLensSettingsChange
 *****************************************************************************/
void LensDriverBox::onCbxLensProfileChange( int index )
{
    if (index != LensProfileUnknown)
    {
        QVector<int> values;

        lens_settings_t settings = profileToSettings(static_cast<enum LensProfile>(index));

        values.append(settings.address);
        values.append(settings.chipID);
        values.append(settings.controllerFeatures);
        values.append(settings.focusMotorNr);
        values.append(settings.zoomMotorNr);
        values.append(settings.irisMotorNr);
        values.append(settings.filterMotorNr);
        values.append(settings.focusMotorFeatures);
        values.append(settings.zoomMotorFeatures);
        values.append(settings.irisMotorFeatures);
        values.append(settings.filterMotorFeatures);

        setWaitCursor();
        emit LensSettingsChanged( values );
        // TODO sent active command
        // emit LensActiveChanged( 1 );
        setNormalCursor();
    }
}

