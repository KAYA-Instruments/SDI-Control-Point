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
    void setFanSettingsVisible( const bool value );
    void setNumTempSensors( const unsigned int tempSensorCount );

protected:
    void prepareMode( const Mode mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

signals:
    void GetTempRequest( uint8_t id );
    void GetMaxTempRequest();
    void GetFanSpeedRequest();
    void FanTargetChanged( uint8_t target );
    void GetOverTempCountRequest();
    void MaxTempReset();

public slots:
    // system interface slots
    void onDeviceNameChange( QString name );
    void onSystemPlatformChange( QString platform );
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
    void onTempChange( uint8_t id, float temp , QString name );
    void onMaxTempChange( int32_t max_temp_logged_user, int32_t max_temp_logged_persistent, int32_t max_temp_allowed );
    void onFanSpeedChange( uint8_t speed );
    void onFanTargetChange( uint8_t target );
    void onOverTempCountChange( uint32_t count );

private slots:
    void onRefreshTempClicked();
    void onSbxFanTargetChanged( int target );
    void onResetMaxTempClicked();
    void onShowLicenseClicked();
    void onShowThirdPartyLicensesClicked();

private:
    class PrivateData;
    PrivateData * d_data;

    void showEvent( QShowEvent* event );
};

#endif // __INFO_BOX_H__

