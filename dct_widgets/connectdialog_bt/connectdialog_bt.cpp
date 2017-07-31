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
 * @file    connectdialog_bt.cpp
 *
 * @brief   Implementation of connection dialog to a proVideo device
 *
 *****************************************************************************/
#include <cerrno>
#include <QtDebug>

#include <com_ctrl/ComChannelBluetooth.h>

#include "connectdialog_bt.h"
#include "ui_connectdialog_bt.h"

// from http://developer.android.com/guide/topics/connectivity/bluetooth.html
static const QLatin1String rfcomm_uuid("00001101-0000-1000-8000-00805F9B34FB");

/******************************************************************************
 * ConnectDialogBt::ConnectDialogBt
 *****************************************************************************/
ConnectDialogBt::ConnectDialogBt( QWidget * parent )
    : QDialog( parent )
    , m_ui( new Ui::dlgConnectBt )
{
    // initialize UI
    m_ui->setupUi( this );

    // add scan button
    m_rescan = new QPushButton( tr("Re&scan") );
    connect( m_rescan, SIGNAL(clicked()), this, SLOT(scanClicked()) );
    m_ui->btnBoxBlueDevice->addButton( m_rescan, QDialogButtonBox::ActionRole );

    // add connect button
    m_connect = new QPushButton( tr("&Connect") );
    connect( m_connect, SIGNAL(clicked()), this, SLOT(connectClicked()) );
    m_ui->btnBoxBlueDevice->addButton( m_connect, QDialogButtonBox::ActionRole );

    // setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowTitleHint );

    // disable all buttons, reenabled by scan-finished
    m_rescan->setEnabled( false );
    m_connect->setEnabled( false );

    // initialize bluetooth stuff
    m_local     = new QBluetoothLocalDevice();

    m_discovery = new QBluetoothServiceDiscoveryAgent( m_local->address() );

    connect( m_discovery, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
             this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));
    connect( m_discovery, SIGNAL(finished()), this, SLOT(discoveryFinished()));
    connect( m_discovery, SIGNAL(canceled()), this, SLOT(discoveryFinished()));

    // clear service list
    m_services.clear();

    // start bluetooth service scan
    m_discovery->setUuidFilter( QBluetoothUuid(rfcomm_uuid) );
    m_discovery->start( QBluetoothServiceDiscoveryAgent::FullDiscovery );
}

/******************************************************************************
 * ConnectDialogBt::~ConnectDialogBt
 *****************************************************************************/
ConnectDialogBt::~ConnectDialogBt()
{
    delete m_discovery;
    delete m_local;
    delete m_ui;
}

/******************************************************************************
 * ConnectDialogBt::scanClicked
 *****************************************************************************/
void ConnectDialogBt::scanClicked()
{
    if ( m_discovery->isActive() )
    {
        m_discovery->stop();
    }

    m_discovery->clear();

    // clear device list
    m_ui->cbxDeviceBt->clear();

    // disable scan and connect button
    m_rescan->setEnabled( false );
    m_connect->setEnabled( false );

    // clear service list
    m_services.clear();

    // start bluetooth service scan
    m_discovery->setUuidFilter( QBluetoothUuid(rfcomm_uuid) );
    m_discovery->start( QBluetoothServiceDiscoveryAgent::FullDiscovery );
}

/******************************************************************************
 * ConnectDialogBt::connectClicked
 *****************************************************************************/
void ConnectDialogBt::connectClicked()
{
    ctrl_channel_generic_config_t open_cfg;

    // get selected service/device
    m_service = m_services.value( m_ui->cbxDeviceBt->currentIndex() );

    // stop discovery
    if ( m_discovery->isActive() )
    {
        m_discovery->stop();
    }

    qDebug() << m_ui->cbxDeviceBt->currentIndex() << ":" <<  m_service.device().name();

    // open channel
    if ( ctrl_channel_open( m_bt->GetInstance(), &open_cfg, sizeof(open_cfg) ) < 0 )
    {
        return; 
    }

    // connect socket
    connect( m_bt->getSocket(), SIGNAL(connected()), this, SLOT(connected()));
    m_bt->getSocket()->connectToService( m_service );
}

/******************************************************************************
 * ConnectDialogBt::connected
 *****************************************************************************/
void ConnectDialogBt::connected()
{
    disconnect( m_bt->getSocket(), SIGNAL(connected()), this, SLOT(connected()));

    // socket is connected so close dialog now
    QDialog::accept();
}

/******************************************************************************
 * ConnectDialogBt::addDevice
 *****************************************************************************/
void ConnectDialogBt::serviceDiscovered( const QBluetoothServiceInfo &serviceInfo )
{
    QString remoteName;
    if ( serviceInfo.device().name().isEmpty() )
    {
        remoteName = serviceInfo.device().address().toString();
    }
    else
    {
        remoteName = serviceInfo.device().name();
    }

    qDebug() << m_ui->cbxDeviceBt->count() << ":" << remoteName << ":" << serviceInfo.device().name() << ":"  << serviceInfo.device().address().toString();

    m_services.insert( m_ui->cbxDeviceBt->count(), serviceInfo );
    m_ui->cbxDeviceBt->addItem( remoteName );
}


/******************************************************************************
 * ConnectDialogBt::scanFinished
 *****************************************************************************/
void ConnectDialogBt::discoveryFinished()
{
    m_rescan->setEnabled( true );
    m_connect->setEnabled( true );
}

/******************************************************************************
 * ConnectDialog::setChannel
 *****************************************************************************/
void ConnectDialogBt::setChannel(ComChannelBluetooth *c )
{
    if ( c )
    {
        m_bt = c;
    }
}

/******************************************************************************
 * ConnectDialogBt::loadSettings
 *****************************************************************************/
bool ConnectDialogBt::loadSettings( QSettings &s )
{
    (void) s;

    return ( false );
}

/******************************************************************************
 * ConnectDialogBt::saveSettings
 *****************************************************************************/
void ConnectDialogBt::saveSettings( QSettings &s )
{
    (void) s;
}

/******************************************************************************
 * ConnectDialogBt::openInterface
 *****************************************************************************/
int ConnectDialogBt::openInterface()
{
    return ( -EFAULT );
}

