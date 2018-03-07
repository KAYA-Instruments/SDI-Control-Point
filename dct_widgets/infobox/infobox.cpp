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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/
/**
 * @file    infobox.cpp
 *
 * @brief   Implementation of system information box 
 *
 *****************************************************************************/
#include <QtDebug>
#include <QMessageBox>

#include <textviewer.h>

#include <com_ctrl/ComChannelRSxxx.h>

#include "infobox.h"
#include "ui_infobox.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_InfoBox;
}

/******************************************************************************
 * InfoBox::PrivateData
 *****************************************************************************/
class InfoBox::PrivateData
{
public:
    PrivateData()
        : m_ui( new Ui::UI_InfoBox ),
          m_numTempSensors( 0 )
    {
        // do nothing
    };

    ~PrivateData()
    {
        delete m_ui;
    };

    Ui::UI_InfoBox *    m_ui;                   /**< ui handle */
    unsigned int        m_numTempSensors;       /**< number of Temperature sensors which are available */
};

/******************************************************************************
 * InfoBox::InfoBox
 *****************************************************************************/
InfoBox::InfoBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData;

    // initialize UI
    d_data->m_ui->setupUi( this );

    // fill GUI version line edit
    d_data->m_ui->letGuiVersion->setText(VERSION_STRING);

    // hide UI elements for sw and hw mask, they are of no interest for the user
    d_data->m_ui->lblFeatureMaskSw->setVisible( false );
    d_data->m_ui->letFeatureMaskSw->setVisible( false );
    d_data->m_ui->lblFeatureMaskHw->setVisible( false );
    d_data->m_ui->letFeatureMaskHw->setVisible( false );
    d_data->m_ui->lstFeatureMaskHw->setVisible( false );

    // fill temperature line edits
    d_data->m_ui->letTemp0->setText("N/A");
    d_data->m_ui->letTemp1->setText("N/A");
    d_data->m_ui->letMaxTemp->setText("N/A");
    d_data->m_ui->letOverTemp->setText("N/A");

    // connect temperature refresh and reset buttons
    connect( d_data->m_ui->btnRefreshTemp, SIGNAL(clicked(bool)), this, SLOT(onRefreshTempClicked()) );
    connect( d_data->m_ui->btnResetMaxTemp, SIGNAL(clicked(bool)), this, SLOT(onResetMaxTempClicked()) );

    // connect software license dialogs
    connect( d_data->m_ui->btnShowLicense, SIGNAL(clicked(bool)), this, SLOT(onShowLicenseClicked()) );
    connect( d_data->m_ui->btnShowThirdPartyLicenses, SIGNAL(clicked(bool)), this, SLOT(onShowThirdPartyLicensesClicked()) );
}

/******************************************************************************
 * InfoBox::~InfoBox
 *****************************************************************************/
InfoBox::~InfoBox()
{
    delete d_data;
}

/******************************************************************************
 * InfoBox::showEvent
 *****************************************************************************/
void InfoBox::showEvent( QShowEvent* event )
{
    // Call inherited function
    DctWidgetBox::showEvent( event );

    // Refresh temperature readouts
    onRefreshTempClicked();
}

/******************************************************************************
 * InfoBox::prepareMode
 *****************************************************************************/
void InfoBox::prepareMode( const Mode )
{
    // do nothing here
}

/******************************************************************************
 * InfoBox::loadSettings
 *****************************************************************************/
void InfoBox::loadSettings( QSettings & )
{
    // do nothing here
}

/******************************************************************************
 * InfoBox::saveSettings
 *****************************************************************************/
void InfoBox::saveSettings( QSettings & )
{
    // do nothing here
}

/******************************************************************************
 * InfoBox::applySettings
 *****************************************************************************/
void InfoBox::applySettings( void )
{
    // do nothing here
}

/******************************************************************************
 * InfoBox::setRuntimeVisibility
 *****************************************************************************/
void InfoBox::setRuntimeVisible( const bool value )
{
    d_data->m_ui->lblRuntime->setVisible(value);
    d_data->m_ui->tetRuntime->setVisible(value);
}

/******************************************************************************
 * InfoBox::setNumTempSensors
 *****************************************************************************/
void InfoBox::setNumTempSensors( const unsigned int tempSensorCount )
{
    d_data->m_numTempSensors = tempSensorCount;

    /* If no temp sensor is available, hide the temperature eval group box */
    if ( tempSensorCount == 0 )
    {
        d_data->m_ui->gpxTemperature->setVisible( false );
    }
    else
    {
        d_data->m_ui->gpxTemperature->setVisible( true );
    }

    /* If only one sensor is available, hide the second sensor readout label */
    if ( tempSensorCount == 1 )
    {
        d_data->m_ui->letTemp1->setVisible( false );
    }
    else if ( tempSensorCount >= 2 )
    {
        d_data->m_ui->letTemp1->setVisible( true );
    }
}

/******************************************************************************
 * InfoBox::onDeviceNameChange
 *****************************************************************************/
void InfoBox::onDeviceNameChange( QString name )
{
    d_data->m_ui->letDeviceName->setText( name );
}

/******************************************************************************
 * InfoBox::onSystemPlatformChange
 *****************************************************************************/
void InfoBox::onSystemPlatformChange( QString platform )
{
    d_data->m_ui->letSystemPlatform->setText( platform );
}

/******************************************************************************
 * InfoBox::onDeviceIdChange
 *****************************************************************************/
void InfoBox::onDeviceIdChange( uint32_t id0, uint32_t id1, uint32_t id2, uint32_t id3 )
{
    QString s;
    s.sprintf( "%08X-%08X-%08X-%08X", id0, id1, id2, id3 );
    d_data->m_ui->letSystemId->setText( s );
}

/******************************************************************************
 * InfoBox::onSystemValidityChange
 *****************************************************************************/
void InfoBox::onSystemValidityChange( QString validity )
{
    d_data->m_ui->letSystemValidity->setText( validity );
}

/******************************************************************************
 * InfoBox::onBitStreamVersionChange
 *****************************************************************************/
void InfoBox::onBitStreamVersionChange( uint32_t version )
{
    QString s;
    s.sprintf( "0x%x", version );
    d_data->m_ui->letBitstreamId->setText( s );
}

/******************************************************************************
 * InfoBox::onApplicationVersionChange
 *****************************************************************************/
void InfoBox::onApplicationVersionChange( QString version )
{
    d_data->m_ui->letSoftwareId->setText( version );
}

/******************************************************************************
 * InfoBox::onApplicationReleaseDateChange
 *****************************************************************************/
void InfoBox::onApplicationReleaseDateChange( QString date )
{
    d_data->m_ui->letSoftwareReleaseDate->setText( date );
}

/******************************************************************************
 * InfoBox::onApplicationBuildDateChange
 *****************************************************************************/
void InfoBox::onApplicationBuildDateChange( QString date )
{
    d_data->m_ui->letSoftwareBuildDate->setText( date );
}

/******************************************************************************
 * InfoBox::onBootloaderVersionChange
 *****************************************************************************/
void InfoBox::onBootloaderVersionChange( uint32_t version )
{
    QString s;
    s.sprintf( "%d", version );
    d_data->m_ui->letLoaderId->setText( s );
}

/******************************************************************************
 * InfoBox::onFeatureMaskHwChange
 *****************************************************************************/
void InfoBox::onFeatureMaskHwChange( uint32_t mask )
{
    QString s;
    s.sprintf( "0x%08x", mask );
    d_data->m_ui->letFeatureMaskHw->setText( s );
}

/******************************************************************************
 * InfoBox::onFeatureMaskHwListChange
 *****************************************************************************/
void InfoBox::onFeatureMaskHwListChange( QStringList features )
{
    d_data->m_ui->lstFeatureMaskHw->clear();
    d_data->m_ui->lstFeatureMaskHw->addItems( features );
}

/******************************************************************************
 * InfoBox::onFeatureMaskSwChange
 *****************************************************************************/
void InfoBox::onFeatureMaskSwChange( uint32_t mask )
{
    QString s;
    s.sprintf( "0x%08x", mask );
    d_data->m_ui->letFeatureMaskSw->setText( s );
}

/******************************************************************************
 * InfoBox::onRunTimeChange
 *****************************************************************************/
void InfoBox::onRunTimeChange( uint32_t seconds )
{
    QTime t( 0, 0 , 0 );
    d_data->m_ui->tetRuntime->setTime( t.addSecs( seconds ) );
}

/******************************************************************************
 * InfoBox::onTempChange
 *****************************************************************************/
void InfoBox::onTempChange( uint8_t id, float temp, QString name )
{
    QString tempString;
    tempString.sprintf( "%.1f °C", temp );

    if ( id == 0u )
    {
        d_data->m_ui->letTemp0->setText( QStringLiteral("%1 (%2)").arg(tempString).arg(name) );
    }
    else if ( id == 1u )
    {
        d_data->m_ui->letTemp1->setText( QStringLiteral("%1 (%2)").arg(tempString).arg(name) );
    }
}

/******************************************************************************
 * InfoBox::onMaxTempChange
 *****************************************************************************/
void InfoBox::onMaxTempChange( int32_t max_temp )
{
    d_data->m_ui->letMaxTemp->setText( QStringLiteral("%1.0 °C").arg(max_temp) );
}

/******************************************************************************
 * InfoBox::onOverTempCountChange
 *****************************************************************************/
void InfoBox::onOverTempCountChange( uint32_t count )
{
    d_data->m_ui->letOverTemp->setText( QString::number(count) );
}

/******************************************************************************
 * InfoBox::onRefreshTempClicked
 *****************************************************************************/
void InfoBox::onRefreshTempClicked()
{
    if ( d_data->m_numTempSensors > 0 )
    {
        emit GetTempRequest( 0u );

        if ( d_data->m_numTempSensors >= 2 )
        {
            emit GetTempRequest( 1u );
        }

        emit GetMaxTempRequest();
        emit GetOverTempCountRequest();
    }
}

/******************************************************************************
 * InfoBox::onResetMaxTempClicked
 *****************************************************************************/
void InfoBox::onResetMaxTempClicked()
{
    emit MaxTempReset();
    emit GetMaxTempRequest();
}

/******************************************************************************
 * InfoBox::onShowLicenseClicked
 *****************************************************************************/
void InfoBox::onShowLicenseClicked()
{
    TextViewer * licenseView = new TextViewer( ":/doc/license.txt", "License" );
    licenseView->show();
}

/******************************************************************************
 * InfoBox::onShowThirdPartyLicensesClicked
 *****************************************************************************/
void InfoBox::onShowThirdPartyLicensesClicked()
{
    TextViewer * licenseView = new TextViewer( ":/doc/third-party-licenses.txt", "Third-Party Licenses" );
    licenseView->show();
}

