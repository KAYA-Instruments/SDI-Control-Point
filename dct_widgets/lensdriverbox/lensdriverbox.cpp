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
    settingsDctKit,
    settingsICS
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

    d_data->m_ui->sbxFocusPosition->setRange(0,100);
    d_data->m_ui->sbxFocusSpeed->setRange(0,100);
    d_data->m_ui->sbxFocusStepMode->setRange(0,100);
    d_data->m_ui->sbxFocusTorque->setRange(0,100);

    d_data->m_ui->sbxZoomPosition->setRange(0,100);
    d_data->m_ui->sbxZoomSpeed->setRange(0,100);
    d_data->m_ui->sbxZoomStepMode->setRange(0,100);
    d_data->m_ui->sbxZoomTorque->setRange(0,100);

    d_data->m_ui->sbxIrisPosition->setRange(0,100);
    d_data->m_ui->sbxIrisSpeed->setRange(0,100);
    d_data->m_ui->sbxIrisStepMode->setRange(0,100);
    d_data->m_ui->sbxIrisTorque->setRange(0,100);

    d_data->m_ui->sbxFilterPosition->setRange(0,100);
    d_data->m_ui->sbxFilterSpeed->setRange(0,100);
    d_data->m_ui->sbxFilterStepMode->setRange(0,100);
    d_data->m_ui->sbxFilterTorque->setRange(0,100);

    d_data->m_ui->sldFocusPosition->setRange(0,100);
    d_data->m_ui->sldZoomPosition->setRange(0,100);
    d_data->m_ui->sldIrisPosition->setRange(0,100);
    d_data->m_ui->sldFilterPosition->setRange(0,100);

    d_data->m_ui->btnActivateLens->clearFocus();
    d_data->m_ui->cbxEnableAdvancedSettings->setChecked(false);

    d_data->m_ui->lblFocusSpeed->setVisible(false);
    d_data->m_ui->lblFocusStepMode->setVisible(false);
    d_data->m_ui->lblFocusTorque->setVisible(false);

    d_data->m_ui->sbxFocusSpeed->setVisible(false);
    d_data->m_ui->sbxFocusStepMode->setVisible(false);
    d_data->m_ui->sbxFocusTorque->setVisible(false);

    d_data->m_ui->lblZoomSpeed->setVisible(false);
    d_data->m_ui->lblZoomStepMode->setVisible(false);
    d_data->m_ui->lblZoomTorque->setVisible(false);

    d_data->m_ui->sbxZoomSpeed->setVisible(false);
    d_data->m_ui->sbxZoomStepMode->setVisible(false);
    d_data->m_ui->sbxZoomTorque->setVisible(false);

    d_data->m_ui->lblIrisSpeed->setVisible(false);
    d_data->m_ui->lblIrisStepMode->setVisible(false);
    d_data->m_ui->lblIrisTorque->setVisible(false);

    d_data->m_ui->sbxIrisSpeed->setVisible(false);
    d_data->m_ui->sbxIrisStepMode->setVisible(false);
    d_data->m_ui->sbxIrisTorque->setVisible(false);

    d_data->m_ui->lblFilterSpeed->setVisible(false);
    d_data->m_ui->lblFilterStepMode->setVisible(false);
    d_data->m_ui->lblFilterTorque->setVisible(false);

    d_data->m_ui->sbxFilterSpeed->setVisible(false);
    d_data->m_ui->sbxFilterStepMode->setVisible(false);
    d_data->m_ui->sbxFilterTorque->setVisible(false);


    // fill settings combo box
    for ( int i = LensProfileFirst; i < LensProfileMax; i++ )
    {
        addLensProfile( GetLensProfileName( static_cast<enum LensProfile>(i) ), i );
    }

    // connect internal signals

    connect( d_data->m_ui->cbxLensProfile, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxLensProfileChange(int)) );
    connect( d_data->m_ui->btnActivateLens , SIGNAL(clicked()), this, SLOT(onBtnLensActiveChange()) );
    connect( d_data->m_ui->cbxEnableAdvancedSettings , SIGNAL(stateChanged(int)), this, SLOT(onCbxLensEnableAdvancedSettings(int)) );

    connect( this , SIGNAL(LensSettingsChanged(bool)), this, SLOT(onLensActiveChange(bool)) );


    // Focus Elements
    connect( d_data->m_ui->sbxFocusPosition, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensFocusPositionChanged(int)));
    connect( d_data->m_ui->sldFocusPosition,SIGNAL(valueChanged(int)), d_data->m_ui->sbxFocusPosition, SLOT(setValue(int)));
    connect( d_data->m_ui->sbxFocusPosition, SIGNAL(valueChanged(int)),d_data->m_ui->sldFocusPosition,SLOT(setValue(int)));
    connect( d_data->m_ui->sbxFocusSpeed, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensFocusSpeedChanged(int)));
    connect( d_data->m_ui->sbxFocusStepMode, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensFocusStepModeChanged(int)));
    connect( d_data->m_ui->sbxFocusTorque, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensFocusTorqueChanged(int)));

    // Zoom Elements
    connect( d_data->m_ui->sbxZoomPosition, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensZoomPositionChanged(int)));
    connect( d_data->m_ui->sldZoomPosition,SIGNAL(valueChanged(int)), d_data->m_ui->sbxZoomPosition, SLOT(setValue(int)));
    connect( d_data->m_ui->sbxZoomPosition, SIGNAL(valueChanged(int)),d_data->m_ui->sldZoomPosition,SLOT(setValue(int)));
    connect( d_data->m_ui->sbxZoomSpeed, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensZoomSpeedChanged(int)));
    connect( d_data->m_ui->sbxZoomStepMode, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensZoomStepModeChanged(int)));
    connect( d_data->m_ui->sbxZoomTorque, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensZoomTorqueChanged(int)));

    // Iris Elements
    connect( d_data->m_ui->sbxIrisPosition, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensIrisPositionChanged(int)));
    connect( d_data->m_ui->sldIrisPosition,SIGNAL(valueChanged(int)), d_data->m_ui->sbxIrisPosition, SLOT(setValue(int)));
    connect( d_data->m_ui->sbxIrisPosition, SIGNAL(valueChanged(int)),d_data->m_ui->sldIrisPosition,SLOT(setValue(int)));
    connect( d_data->m_ui->sbxIrisSpeed, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensIrisSpeedChanged(int)));
    connect( d_data->m_ui->sbxIrisStepMode, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensIrisStepModeChanged(int)));
    connect( d_data->m_ui->sbxIrisTorque, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensIrisTorqueChanged(int)));

    // Filter Elements
    connect( d_data->m_ui->sbxFilterPosition, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensFilterPositionChanged(int)));
    connect( d_data->m_ui->sldFilterPosition,SIGNAL(valueChanged(int)), d_data->m_ui->sbxFilterPosition, SLOT(setValue(int)));
    connect( d_data->m_ui->sbxFilterPosition, SIGNAL(valueChanged(int)),d_data->m_ui->sldFilterPosition,SLOT(setValue(int)));
    connect( d_data->m_ui->sbxFilterSpeed, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensFilterSpeedChanged(int)));
    connect( d_data->m_ui->sbxFilterStepMode, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensFilterStepModeChanged(int)));
    connect( d_data->m_ui->sbxFilterTorque, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensFilterTorqueChanged(int)));




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
    setLensProfile(s.value(LENSDRIVER_SETTINGS_SETTINGS).toString() );
    setLensFocusPosition(s.value(LENSDRIVER_SETTINGS_FOCUS_POSITION).toInt());
    setLensFocusSettings(s.value(LENSDRIVER_SETTINGS_FOCUS_SETTINGS).toString());
    setLensZoomPosition(s.value(LENSDRIVER_SETTINGS_ZOOM_POSITION).toInt());
    setLensZoomSettings(s.value(LENSDRIVER_SETTINGS_ZOOM_SETTINGS).toString());
    setLensIrisPosition(s.value(LENSDRIVER_SETTINGS_IRIS_POSITION).toInt());
    setLensIrisSettings(s.value(LENSDRIVER_SETTINGS_IRIS_SETTINGS).toString());
    setLensFilterPosition(s.value(LENSDRIVER_SETTINGS_FILTER_POSITION).toInt());
    setLensFilterSettings(s.value(LENSDRIVER_SETTINGS_FILTER_SETTINGS).toString());
    s.endGroup();
}

/******************************************************************************
 * LensDriverBox::saveSettings
 *****************************************************************************/
void LensDriverBox::saveSettings( QSettings & s )
{
    s.beginGroup( LENSDRIVER_SETTINGS_SECTION_NAME );
    s.setValue(LENSDRIVER_SETTINGS_SETTINGS, LensProfile());
    s.setValue(LENSDRIVER_SETTINGS_FOCUS_POSITION, LensFocusPosition());
    s.setValue(LENSDRIVER_SETTINGS_FOCUS_SETTINGS, LensFocusSettings());
    s.setValue(LENSDRIVER_SETTINGS_ZOOM_POSITION, LensZoomPosition());
    s.setValue(LENSDRIVER_SETTINGS_ZOOM_SETTINGS, LensZoomSettings());
    s.setValue(LENSDRIVER_SETTINGS_IRIS_POSITION, LensIrisPosition());
    s.setValue(LENSDRIVER_SETTINGS_IRIS_SETTINGS, LensIrisSettings());
    s.setValue(LENSDRIVER_SETTINGS_FILTER_POSITION, LensFilterPosition());
    s.setValue(LENSDRIVER_SETTINGS_FILTER_SETTINGS, LensFilterSettings());
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
             settings.chipID == lensSettingProfiles[i].chipID &&
             settings.controllerFeatures == lensSettingProfiles[i].controllerFeatures &&
             settings.focusMotorNr == lensSettingProfiles[i].focusMotorNr &&
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
 * LensDriverBox::LensFocusPosition
 *****************************************************************************/
int LensDriverBox::LensFocusPosition() const
{
    return ( d_data->m_ui->sbxFocusPosition->value() );
}

/******************************************************************************
 * LensDriverBox::LensFocusSettings
 *****************************************************************************/
QString LensDriverBox::LensFocusSettings() const
{
    QString settings;
    settings.append(d_data->m_ui->sbxFocusSpeed->cleanText() );
    settings.append(" ");
    settings.append(d_data->m_ui->sbxFocusStepMode->cleanText() );
    settings.append(" ");
    settings.append(d_data->m_ui->sbxFocusTorque->cleanText() );
    return ( settings );
}

/******************************************************************************
 * LensDriverBox::LensZoomPosition
 *****************************************************************************/
int LensDriverBox::LensZoomPosition() const
{
    return ( d_data->m_ui->sbxZoomPosition->value() );
}

/******************************************************************************
 * LensDriverBox::LensZoomSettings
 *****************************************************************************/
QString LensDriverBox::LensZoomSettings() const
{
    QString settings;
    settings.append(d_data->m_ui->sbxZoomSpeed->cleanText() );
    settings.append(" ");
    settings.append(d_data->m_ui->sbxZoomStepMode->cleanText() );
    settings.append(" ");
    settings.append(d_data->m_ui->sbxZoomTorque->cleanText() );
    return ( settings );
}

/******************************************************************************
 * LensDriverBox::LensIrisPosition
 *****************************************************************************/
int LensDriverBox::LensIrisPosition() const
{
    return ( d_data->m_ui->sbxIrisPosition->value() );
}

/******************************************************************************
 * LensDriverBox::LensIrisSettings
 *****************************************************************************/
QString LensDriverBox::LensIrisSettings() const
{
    QString settings;
    settings.append(d_data->m_ui->sbxIrisSpeed->cleanText() );
    settings.append(" ");
    settings.append(d_data->m_ui->sbxIrisStepMode->cleanText() );
    settings.append(" ");
    settings.append(d_data->m_ui->sbxIrisTorque->cleanText() );
    return ( settings );
}

/******************************************************************************
 * LensDriverBox::LensFilterPosition
 *****************************************************************************/
int LensDriverBox::LensFilterPosition() const
{
    return ( d_data->m_ui->sbxFilterPosition->value() );
}

/******************************************************************************
 * LensDriverBox::LensFilterSettings
 *****************************************************************************/
QString LensDriverBox::LensFilterSettings() const
{
    QString settings;
    settings.append(d_data->m_ui->sbxFilterSpeed->cleanText() );
    settings.append(" ");
    settings.append(d_data->m_ui->sbxFilterStepMode->cleanText() );
    settings.append(" ");
    settings.append(d_data->m_ui->sbxFilterTorque->cleanText() );
    return ( settings );
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
 * LensDriverBox::setLensFocusPosition
 *****************************************************************************/
void LensDriverBox::setLensFocusPosition( int focus )
{

        d_data->m_ui->sbxFocusPosition->blockSignals( true );
        d_data->m_ui->sbxFocusPosition->setValue(focus);
        d_data->m_ui->sbxFocusPosition->blockSignals( false );

        emit LensFocusPositionChanged( focus );
}

/******************************************************************************
 * LensDriverBox::setLensZoomPosition
 *****************************************************************************/
void LensDriverBox::setLensZoomPosition( int zoom )
{

        d_data->m_ui->sbxZoomPosition->blockSignals( true );
        d_data->m_ui->sbxZoomPosition->setValue(zoom);
        d_data->m_ui->sbxZoomPosition->blockSignals( false );

        emit LensZoomPositionChanged( zoom );
}

/******************************************************************************
 * LensDriverBox::setLensIrisPosition
 *****************************************************************************/
void LensDriverBox::setLensIrisPosition( int iris )
{

        d_data->m_ui->sbxIrisPosition->blockSignals( true );
        d_data->m_ui->sbxIrisPosition->setValue(iris);
        d_data->m_ui->sbxIrisPosition->blockSignals( false );

        emit LensIrisPositionChanged( iris );
}

/******************************************************************************
 * LensDriverBox::setLensFilterPosition
 *****************************************************************************/
void LensDriverBox::setLensFilterPosition( int filter )
{

        d_data->m_ui->sbxFilterPosition->blockSignals( true );
        d_data->m_ui->sbxFilterPosition->setValue(filter);
        d_data->m_ui->sbxFilterPosition->blockSignals( false );

        emit LensFilterPositionChanged( filter );
}

/******************************************************************************
 * LensDriverBox::setLensFocusSettings
 *****************************************************************************/
void LensDriverBox::setLensFocusSettings( const QString settings )
{
        QStringList values;
        values = settings.split(QRegularExpression("\\s+"));

        int speed = values.first().toInt();
        values.removeFirst();
        int stepMode = values.first().toInt();
        values.removeFirst();
        int torque = values.first().toInt();

        d_data->m_ui->sbxFocusSpeed->blockSignals( true );
        d_data->m_ui->sbxFocusSpeed->setValue(speed);
        d_data->m_ui->sbxFocusSpeed->blockSignals( false );

        d_data->m_ui->sbxFocusStepMode->blockSignals( true );
        d_data->m_ui->sbxFocusStepMode->setValue(stepMode);
        d_data->m_ui->sbxFocusStepMode->blockSignals( false );

        d_data->m_ui->sbxFocusTorque->blockSignals( true );
        d_data->m_ui->sbxFocusTorque->setValue(torque);
        d_data->m_ui->sbxFocusTorque->blockSignals( false );

        QVector<int> valueVector;

        valueVector.append(speed);
        valueVector.append(stepMode);
        valueVector.append(torque);

        emit LensFocusSettingsChanged(valueVector);


}

/******************************************************************************
 * LensDriverBox::setLensZoomSettings
 *****************************************************************************/
void LensDriverBox::setLensZoomSettings( const QString settings )
{
        QStringList values;
        values = settings.split(QRegularExpression("\\s+"));

        int speed = values.first().toInt();
        values.removeFirst();
        int stepMode = values.first().toInt();
        values.removeFirst();
        int torque = values.first().toInt();

        d_data->m_ui->sbxZoomSpeed->blockSignals( true );
        d_data->m_ui->sbxZoomSpeed->setValue(speed);
        d_data->m_ui->sbxZoomSpeed->blockSignals( false );

        d_data->m_ui->sbxZoomStepMode->blockSignals( true );
        d_data->m_ui->sbxZoomStepMode->setValue(stepMode);
        d_data->m_ui->sbxZoomStepMode->blockSignals( false );

        d_data->m_ui->sbxZoomTorque->blockSignals( true );
        d_data->m_ui->sbxZoomTorque->setValue(torque);
        d_data->m_ui->sbxZoomTorque->blockSignals( false );

        QVector<int> valueVector;

        valueVector.append(speed);
        valueVector.append(stepMode);
        valueVector.append(torque);

        emit LensZoomSettingsChanged(valueVector);


}

/******************************************************************************
 * LensDriverBox::setLensIrisSettings
 *****************************************************************************/
void LensDriverBox::setLensIrisSettings( const QString settings )
{
        QStringList values;
        values = settings.split(QRegularExpression("\\s+"));

        int speed = values.first().toInt();
        values.removeFirst();
        int stepMode = values.first().toInt();
        values.removeFirst();
        int torque = values.first().toInt();

        d_data->m_ui->sbxIrisSpeed->blockSignals( true );
        d_data->m_ui->sbxIrisSpeed->setValue(speed);
        d_data->m_ui->sbxIrisSpeed->blockSignals( false );

        d_data->m_ui->sbxIrisStepMode->blockSignals( true );
        d_data->m_ui->sbxIrisStepMode->setValue(stepMode);
        d_data->m_ui->sbxIrisStepMode->blockSignals( false );

        d_data->m_ui->sbxIrisTorque->blockSignals( true );
        d_data->m_ui->sbxIrisTorque->setValue(torque);
        d_data->m_ui->sbxIrisTorque->blockSignals( false );

        QVector<int> valueVector;

        valueVector.append(speed);
        valueVector.append(stepMode);
        valueVector.append(torque);

        emit LensIrisSettingsChanged(valueVector);


}

/******************************************************************************
 * LensDriverBox::setLensFilterSettings
 *****************************************************************************/
void LensDriverBox::setLensFilterSettings( const QString settings )
{
        QStringList values;
        values = settings.split(QRegularExpression("\\s+"));

        int speed = values.first().toInt();
        values.removeFirst();
        int stepMode = values.first().toInt();
        values.removeFirst();
        int torque = values.first().toInt();

        d_data->m_ui->sbxFilterSpeed->blockSignals( true );
        d_data->m_ui->sbxFilterSpeed->setValue(speed);
        d_data->m_ui->sbxFilterSpeed->blockSignals( false );

        d_data->m_ui->sbxFilterStepMode->blockSignals( true );
        d_data->m_ui->sbxFilterStepMode->setValue(stepMode);
        d_data->m_ui->sbxFilterStepMode->blockSignals( false );

        d_data->m_ui->sbxFilterTorque->blockSignals( true );
        d_data->m_ui->sbxFilterTorque->setValue(torque);
        d_data->m_ui->sbxFilterTorque->blockSignals( false );

        QVector<int> valueVector;

        valueVector.append(speed);
        valueVector.append(stepMode);
        valueVector.append(torque);

        emit LensFilterSettingsChanged(valueVector);


}

/******************************************************************************
 * LensDriverBox::showLensProfilBoxes
 *****************************************************************************/
int LensDriverBox::showLensProfilBoxes(enum LensProfile currentProfile)
{
    int res = 0;
    bool focusAvailable = false;
    bool zoomAvailable = false;
    bool irisAvailable = false;
    bool filterAvailable = false;

    lens_settings_t lensfeatures;
    lensfeatures  = profileToSettings( currentProfile );

    ( ( lensfeatures.controllerFeatures & ( 1 << lensfeatures.focusMotorNr ) ) > 0) ? focusAvailable = true : focusAvailable = false;
    ( ( lensfeatures.controllerFeatures & ( 1 << lensfeatures.zoomMotorNr ) ) > 0) ? zoomAvailable = true : zoomAvailable = false;
    ( ( lensfeatures.controllerFeatures & ( 1 << lensfeatures.irisMotorNr ) ) > 0) ? irisAvailable = true : irisAvailable = false;
    ( ( lensfeatures.controllerFeatures & ( 1 << lensfeatures.filterMotorNr ) ) > 0) ? filterAvailable = true : filterAvailable = false;

    d_data->m_ui->gbxFocusControl->setVisible(focusAvailable);
    d_data->m_ui->gbxZoomControl->setVisible(zoomAvailable);
    d_data->m_ui->gbxIrisControl->setVisible(irisAvailable);
    d_data->m_ui->gbxFilterControl->setVisible(filterAvailable);

    enableLensMotorSettings(LensFeatuesFocus,lensfeatures.focusMotorFeatures);
    enableLensMotorSettings(LensFeatuesZoom,lensfeatures.zoomMotorFeatures);
    enableLensMotorSettings(LensFeatuesIris,lensfeatures.irisMotorFeatures);
    enableLensMotorSettings(LensFeatuesFilter,lensfeatures.filterMotorFeatures);


   return res;
}

/******************************************************************************
 * LensDriverBox::enableLensMotorSettings
 *****************************************************************************/
int LensDriverBox::enableLensMotorSettings(enum LensFeatues features,int motorSettings)
{
    int res = 0;
    bool speedEnable = false;
    bool stepModeEnable = false;
    bool torqueEnable = false;

    ( ( motorSettings & ( 1 << 1 ) ) > 0 ) ? speedEnable = true : speedEnable = false;
    ( ( motorSettings & ( 1 << 2 ) ) > 0 ) ? stepModeEnable = true : stepModeEnable = false;
    ( ( motorSettings & ( 1 << 3 ) ) > 0 ) ? torqueEnable = true : torqueEnable = false;

    switch ( static_cast<int>(features) )
    {
        case static_cast<int>( LensFeatuesFocus):
        {
            d_data->m_ui->sbxFocusSpeed->setEnabled(speedEnable);
            d_data->m_ui->sbxFocusStepMode->setEnabled(stepModeEnable);
            d_data->m_ui->sbxFocusTorque->setEnabled(torqueEnable);
            break;
        }
        case static_cast<int>( LensFeatuesZoom ):
        {
            d_data->m_ui->sbxZoomSpeed->setEnabled(speedEnable);
            d_data->m_ui->sbxZoomStepMode->setEnabled(stepModeEnable);
            d_data->m_ui->sbxZoomTorque->setEnabled(torqueEnable);
            break;
        }
        case static_cast<int>( LensFeatuesIris ):
        {
            d_data->m_ui->sbxIrisSpeed->setEnabled(speedEnable);
            d_data->m_ui->sbxIrisStepMode->setEnabled(stepModeEnable);
            d_data->m_ui->sbxIrisTorque->setEnabled(torqueEnable);
            break;
        }
        case static_cast<int>( LensFeatuesFilter ):
        {
            d_data->m_ui->sbxFilterSpeed->setEnabled(speedEnable);
            d_data->m_ui->sbxFilterStepMode->setEnabled(stepModeEnable);
            d_data->m_ui->sbxFilterTorque->setEnabled(torqueEnable);
            break;
        }
    }

    return res;
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

    showLensProfilBoxes(settingsToProfile(settings));
    d_data->m_LensSettings = settings;

    d_data->m_ui->cbxLensProfile->blockSignals( true );
    d_data->m_ui->cbxLensProfile->setCurrentIndex( settingsToProfile(settings) );
    d_data->m_ui->cbxLensProfile->blockSignals( false );
}



/******************************************************************************
 * LensDriverBox::onLensActiveChange
 *****************************************************************************/
void LensDriverBox::onLensActiveChange( bool active )
{
    static bool last_state = false;

    if( active == true)
    {
        d_data->m_ui->lblLensActivated->setText("Active");
    }
    else
    {
         d_data->m_ui->lblLensActivated->setText("not Active");
    }

    d_data->m_ui->gbxFocusControl->setEnabled(active);
    d_data->m_ui->gbxZoomControl->setEnabled(active);
    d_data->m_ui->gbxIrisControl->setEnabled(active);
    d_data->m_ui->gbxFilterControl->setEnabled(active);

    if(active == true && last_state == false)
    {
        emit SmallResyncRequest();
    }
    last_state = active;

}

/******************************************************************************
 * LensDriverBox::onLensFocusPositionChange
 *****************************************************************************/
void LensDriverBox::onLensFocusPositionChange( int pos )
{
    d_data->m_ui->sbxFocusPosition->blockSignals( true );
    d_data->m_ui->sbxFocusPosition->setValue(pos);
    d_data->m_ui->sbxFocusPosition->blockSignals( false );

    d_data->m_ui->sldFocusPosition->blockSignals( true );
    d_data->m_ui->sldFocusPosition->setValue(pos);
    d_data->m_ui->sldFocusPosition->blockSignals( false );
}

/******************************************************************************
 * LensDriverBox::onLensZoomPositionChange
 *****************************************************************************/
void LensDriverBox::onLensZoomPositionChange( int pos )
{
    d_data->m_ui->sbxZoomPosition->blockSignals( true );
    d_data->m_ui->sbxZoomPosition->setValue(pos);
    d_data->m_ui->sbxZoomPosition->blockSignals( false );

    d_data->m_ui->sldZoomPosition->blockSignals( true );
    d_data->m_ui->sldZoomPosition->setValue(pos);
    d_data->m_ui->sldZoomPosition->blockSignals( false );
}

/******************************************************************************
 * LensDriverBox::onLensIrisPositionChange
 *****************************************************************************/
void LensDriverBox::onLensIrisPositionChange( int pos )
{
    d_data->m_ui->sbxIrisPosition->blockSignals( true );
    d_data->m_ui->sbxIrisPosition->setValue(pos);
    d_data->m_ui->sbxIrisPosition->blockSignals( false );

    d_data->m_ui->sldIrisPosition->blockSignals( true );
    d_data->m_ui->sldIrisPosition->setValue(pos);
    d_data->m_ui->sldIrisPosition->blockSignals( false );
}

/******************************************************************************
 * LensDriverBox::onLensFilterPositionChange
 *****************************************************************************/
void LensDriverBox::onLensFilterPositionChange( int pos )
{
    d_data->m_ui->sbxFilterPosition->blockSignals( true );
    d_data->m_ui->sbxFilterPosition->setValue(pos);
    d_data->m_ui->sbxFilterPosition->blockSignals( false );

    d_data->m_ui->sldFilterPosition->blockSignals( true );
    d_data->m_ui->sldFilterPosition->setValue(pos);
    d_data->m_ui->sldFilterPosition->blockSignals( false );
}

/******************************************************************************
 * LensDriverBox::onLensFocusSettingsChange
 *****************************************************************************/
void LensDriverBox::onLensFocusSettingsChange( QVector<int> values  )
{
    d_data->m_ui->sbxFocusSpeed->blockSignals( true );
    d_data->m_ui->sbxFocusSpeed->setValue(values[0]);
    d_data->m_ui->sbxFocusSpeed->blockSignals( false );

    d_data->m_ui->sbxFocusStepMode->blockSignals( true );
    d_data->m_ui->sbxFocusStepMode->setValue(values[1]);
    d_data->m_ui->sbxFocusStepMode->blockSignals( false );

    d_data->m_ui->sbxFocusTorque->blockSignals( true );
    d_data->m_ui->sbxFocusTorque->setValue(values[2]);
    d_data->m_ui->sbxFocusTorque->blockSignals( false );

}

/******************************************************************************
 * LensDriverBox::onLensZoomSettingsChange
 *****************************************************************************/
void LensDriverBox::onLensZoomSettingsChange( QVector<int> values )
{
    d_data->m_ui->sbxZoomSpeed->blockSignals( true );
    d_data->m_ui->sbxZoomSpeed->setValue(values[0]);
    d_data->m_ui->sbxZoomSpeed->blockSignals( false );

    d_data->m_ui->sbxZoomStepMode->blockSignals( true );
    d_data->m_ui->sbxZoomStepMode->setValue(values[1]);
    d_data->m_ui->sbxZoomStepMode->blockSignals( false );

    d_data->m_ui->sbxZoomTorque->blockSignals( true );
    d_data->m_ui->sbxZoomTorque->setValue(values[2]);
    d_data->m_ui->sbxZoomTorque->blockSignals( false );

}

/******************************************************************************
 * LensDriverBox::onLensIrisSettingsChange
 *****************************************************************************/
void LensDriverBox::onLensIrisSettingsChange( QVector<int> values  )
{
    d_data->m_ui->sbxIrisSpeed->blockSignals( true );
    d_data->m_ui->sbxIrisSpeed->setValue(values[0]);
    d_data->m_ui->sbxIrisSpeed->blockSignals( false );

    d_data->m_ui->sbxIrisStepMode->blockSignals( true );
    d_data->m_ui->sbxIrisStepMode->setValue(values[1]);
    d_data->m_ui->sbxIrisStepMode->blockSignals( false );

    d_data->m_ui->sbxIrisTorque->blockSignals( true );
    d_data->m_ui->sbxIrisTorque->setValue(values[2]);
    d_data->m_ui->sbxIrisTorque->blockSignals( false );

}

/******************************************************************************
 * LensDriverBox::onLensFilterSettingsChange
 *****************************************************************************/
void LensDriverBox::onLensFilterSettingsChange(  QVector<int> values  )
{
    d_data->m_ui->sbxFilterSpeed->blockSignals( true );
    d_data->m_ui->sbxFilterSpeed->setValue(values[0]);
    d_data->m_ui->sbxFilterSpeed->blockSignals( false );

    d_data->m_ui->sbxFilterStepMode->blockSignals( true );
    d_data->m_ui->sbxFilterStepMode->setValue(values[1]);
    d_data->m_ui->sbxFilterStepMode->blockSignals( false );

    d_data->m_ui->sbxFilterTorque->blockSignals( true );
    d_data->m_ui->sbxFilterTorque->setValue(values[2]);
    d_data->m_ui->sbxFilterTorque->blockSignals( false );

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

        d_data->m_LensSettings = settings;
        setWaitCursor();
        showLensProfilBoxes(static_cast<enum LensProfile>(index));
        emit LensSettingsChanged( values );
        emit LensSettingsChanged( false );

        setNormalCursor();
    }
}

/******************************************************************************
 * LensDriverBox::onCbxLensSettingsChange
 *****************************************************************************/
void LensDriverBox::onBtnLensActiveChange( void )
{
        d_data->m_ui->btnActivateLens->clearFocus();
        setWaitCursor();
        emit LensActiveChanged( true );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onCbxLensActivateAdvancedSettings
 *****************************************************************************/
void LensDriverBox::onCbxLensEnableAdvancedSettings(int state)
{
    bool check = static_cast<bool>(state);
    d_data->m_ui->lblFocusSpeed->setVisible(check);
    d_data->m_ui->lblFocusStepMode->setVisible(check);
    d_data->m_ui->lblFocusTorque->setVisible(check);

    d_data->m_ui->sbxFocusSpeed->setVisible(check);
    d_data->m_ui->sbxFocusStepMode->setVisible(check);
    d_data->m_ui->sbxFocusTorque->setVisible(check);

    d_data->m_ui->lblZoomSpeed->setVisible(check);
    d_data->m_ui->lblZoomStepMode->setVisible(check);
    d_data->m_ui->lblZoomTorque->setVisible(check);

    d_data->m_ui->sbxZoomSpeed->setVisible(check);
    d_data->m_ui->sbxZoomStepMode->setVisible(check);
    d_data->m_ui->sbxZoomTorque->setVisible(check);

    d_data->m_ui->lblIrisSpeed->setVisible(check);
    d_data->m_ui->lblIrisStepMode->setVisible(check);
    d_data->m_ui->lblIrisTorque->setVisible(check);

    d_data->m_ui->sbxIrisSpeed->setVisible(check);
    d_data->m_ui->sbxIrisStepMode->setVisible(check);
    d_data->m_ui->sbxIrisTorque->setVisible(check);

    d_data->m_ui->lblFilterSpeed->setVisible(check);
    d_data->m_ui->lblFilterStepMode->setVisible(check);
    d_data->m_ui->lblFilterTorque->setVisible(check);

    d_data->m_ui->sbxFilterSpeed->setVisible(check);
    d_data->m_ui->sbxFilterStepMode->setVisible(check);
    d_data->m_ui->sbxFilterTorque->setVisible(check);
}

/******************************************************************************
 * LensDriverBox::onSbxLensFocusPositionChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensFocusPositionChanged( int pos )
{

        setWaitCursor();
        emit LensFocusPositionChanged( pos );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensFocusSpeedChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensFocusSpeedChanged( int speed )
{
        QVector<int> values;

        values.append(speed);
        values.append(d_data->m_ui->sbxFocusStepMode->value());
        values.append(d_data->m_ui->sbxFocusTorque->value());

        setWaitCursor();
        emit LensFocusSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensFocusStepModeChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensFocusStepModeChanged( int stepMode )
{
        QVector<int> values;

        values.append(d_data->m_ui->sbxFocusSpeed->value());
        values.append(stepMode);
        values.append(d_data->m_ui->sbxFocusTorque->value());

        setWaitCursor();
        emit LensFocusSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensFocusTorqueChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensFocusTorqueChanged( int torque )
{
        QVector<int> values;

        values.append(d_data->m_ui->sbxFocusSpeed->value());
        values.append(d_data->m_ui->sbxFocusStepMode->value());
        values.append(torque);

        setWaitCursor();
        emit LensFocusSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensZoomPositionChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensZoomPositionChanged( int pos )
{

        setWaitCursor();
        emit LensZoomPositionChanged( pos );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensZoomSpeedChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensZoomSpeedChanged( int speed )
{
        QVector<int> values;

        values.append(speed);
        values.append(d_data->m_ui->sbxZoomStepMode->value());
        values.append(d_data->m_ui->sbxZoomTorque->value());

        setWaitCursor();
        emit LensZoomSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensZoomStepModeChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensZoomStepModeChanged( int stepMode )
{
        QVector<int> values;

        values.append(d_data->m_ui->sbxZoomSpeed->value());
        values.append(stepMode);
        values.append(d_data->m_ui->sbxZoomTorque->value());

        setWaitCursor();
        emit LensZoomSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensZoomTorqueChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensZoomTorqueChanged( int torque )
{
        QVector<int> values;

        values.append(d_data->m_ui->sbxZoomSpeed->value());
        values.append(d_data->m_ui->sbxZoomStepMode->value());
        values.append(torque);

        setWaitCursor();
        emit LensZoomSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensIrisPositionChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensIrisPositionChanged( int pos )
{

        setWaitCursor();
        emit LensIrisPositionChanged( pos );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensIrisSpeedChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensIrisSpeedChanged( int speed )
{
        QVector<int> values;

        values.append(speed);
        values.append(d_data->m_ui->sbxIrisStepMode->value());
        values.append(d_data->m_ui->sbxIrisTorque->value());

        setWaitCursor();
        emit LensIrisSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensIrisStepModeChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensIrisStepModeChanged( int stepMode )
{
        QVector<int> values;

        values.append(d_data->m_ui->sbxIrisSpeed->value());
        values.append(stepMode);
        values.append(d_data->m_ui->sbxIrisTorque->value());

        setWaitCursor();
        emit LensIrisSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensIrisTorqueChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensIrisTorqueChanged( int torque )
{
        QVector<int> values;

        values.append(d_data->m_ui->sbxIrisSpeed->value());
        values.append(d_data->m_ui->sbxIrisStepMode->value());
        values.append(torque);

        setWaitCursor();
        emit LensIrisSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensFilterPositionChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensFilterPositionChanged( int pos )
{

        setWaitCursor();
        emit LensFilterPositionChanged( pos );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensFilterSpeedChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensFilterSpeedChanged( int speed )
{
        QVector<int> values;

        values.append(speed);
        values.append(d_data->m_ui->sbxFilterStepMode->value());
        values.append(d_data->m_ui->sbxFilterTorque->value());

        setWaitCursor();
        emit LensFilterSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensFilterStepModeChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensFilterStepModeChanged( int stepMode )
{
        QVector<int> values;

        values.append(d_data->m_ui->sbxFilterSpeed->value());
        values.append(stepMode);
        values.append(d_data->m_ui->sbxFilterTorque->value());

        setWaitCursor();
        emit LensFilterSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensFilterTorqueChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensFilterTorqueChanged( int torque )
{
        QVector<int> values;

        values.append(d_data->m_ui->sbxFilterSpeed->value());
        values.append(d_data->m_ui->sbxFilterStepMode->value());
        values.append(torque);

        setWaitCursor();
        emit LensFilterSettingsChanged( values );
        setNormalCursor();
}



