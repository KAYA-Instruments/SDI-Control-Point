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
 * @file    infobox.h
 *
 * @brief   Class definition of a system information box 
 *
 *****************************************************************************/
#ifndef __INFO_BOX_H__
#define __INFO_BOX_H__

#include <dct_widgets_base.h>

/******************************************************************************
 * System Information Box Widget
 *****************************************************************************/
class InfoBox : public DctWidgetBox
{
    Q_OBJECT

public:
    explicit InfoBox( QWidget * parent = 0 );
    ~InfoBox();

    // Show or hide UI elements
    void setRuntimeVisible( const bool value );
    void setBroadcastSettingsVisible( const bool value );

protected:
    void prepareMode( const Mode mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

signals:
    void CopyFlagChanged( bool flag );
    void ResetToDefaultsClicked();
    void SystemSettingsChanged( int rs232Baudrate, int rs485Baudrate, int rs485Address, int rs485BroadcastAddress );
    void EngineeringModeChanged( bool flag );

    void ResyncRequest( void );

public slots:
    // system interface slots
    void onDeviceNameChange( QString name );
    void onDeviceIdChange( uint32_t id0, uint32_t id1, uint32_t id2, uint32_t id3 );
    void onSystemValidityChange( QString version );
    void onBitStreamVersionChange( uint32_t version );
    void onApplicationVersionChange( QString version );
    void onApplicationReleaseDateChange( QString date );
    void onApplicationBuildDateChange( QString date );
    void onBootloaderVersionChange( uint32_t version );
    void onFeatureMaskHwChange( uint32_t mask );
    void onFeatureMaskHwListChange( QStringList features );
    void onFeatureMaskSwChange( uint32_t mask );
    void onRunTimeChange( uint32_t seconds );
    void onRS232BaudrateChange( uint32_t baudrate );
    void onRS485BaudrateChange( uint32_t baudrate );
    void onRS485AddressChange( uint32_t address );
    void onRS485BroadcastAddressChange( uint32_t address );
    void onBroadcastChange( bool enable );

private slots:
    void onBtnResetToDefaultsClicked();
    void onBtnApplySerialPortSettingsClicked();
    void onCbxEngineeringModeChange( int value );
    void onShowLicenseClicked();
    void onShowThirdPartyLicensesClicked();

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __INFO_BOX_H__

