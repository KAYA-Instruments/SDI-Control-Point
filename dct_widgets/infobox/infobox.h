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

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __INFO_BOX_H__

