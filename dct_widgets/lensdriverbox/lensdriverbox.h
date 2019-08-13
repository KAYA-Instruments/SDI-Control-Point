/******************************************************************************
 * Copyright (C) 2019 Dream Chip Technologies GmbH
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
 * @file    lensdriverbox.h
 *
 * @brief   Class definition of lens driver box
 *
 *****************************************************************************/
#ifndef __LENS_DRIVER_BOX_H__
#define __LENS_DRIVER_BOX_H__

#include <dct_widgets_base.h>
#include <QItemSelection>
#include "defines.h"

typedef struct lens_settings_s {
    int address;
    int chipID;
    int controllerFeatures;
    int focusMotorNr;
    int zoomMotorNr;
    int irisMotorNr;
    int filterMotorNr;
    int focusMotorFeatures;
    int zoomMotorFeatures;
    int irisMotorFeatures;
    int filterMotorFeatures;
} lens_settings_t;

typedef struct lens_iris_position_s {
    QVector<int> fStops;
    QVector<int> fStopsPos;
} lens_iris_position_t;

typedef struct lens_iris_position_template_s {
    QVector<int> fStops;
    QVector<int> fStopPos;
    QString lensName;
    QVector<int> compatibleID;
} lens_iris_position_template_t;

/******************************************************************************
 * Lens Driver Box Widget
 *****************************************************************************/
class LensDriverBox : public DctWidgetBox
{
    Q_OBJECT

public:
    explicit LensDriverBox( QWidget * parent = nullptr );
    ~LensDriverBox() Q_DECL_OVERRIDE;

    QString LensProfile() const;
    QString LensInvert() const;
    int LensFocusPosition() const;
    QString LensFocusSettings() const;
    int LensZoomPosition() const;
    QString LensZoomSettings() const;
    int LensIrisPosition() const;
    QString LensIrisSettings() const;
    QString LensIrisTable() const;
    int LensFilterPosition() const;
    QString LensFilterSettings() const;

    void setLensProfile( const QString mode );
    void setLensInvert( const QString mode );
    void setLensFocusPosition ( int );
    void setLensFocusSettings( const QString settings );
    void setLensZoomPosition ( int );
    void setLensZoomSettings( const QString settings );
    void setLensIrisPosition ( int );
    void setLensIrisSettings( const QString settings );
    void setLensIrisTable( const QString settings );
    void setLensFilterPosition ( int );
    void setLensFilterSettings( const QString settings );

protected:
    void prepareMode( const Mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

    void addLensProfile( QString name, int id );
    void addLensIrisAperture( QString name, int id );
    void addLensIrisTemplate( QString name, int id );

private:
    lens_settings_t profileToSettings( enum LensProfile profile );
    enum LensProfile settingsToProfile( lens_settings_t settings );
    int showLensProfilBoxes(enum LensProfile);
    int enableLensMotorSettings(enum LensFeatues features,int motorSettings);


signals:
    void LensSettingsChanged( QVector<int> values );
    void LensSettingsChanged(bool);
    void LensActiveChanged( bool active );
    void LensInvertChanged( QVector<int> values );

    void SmallResyncRequest(void);

    void LensFocusPositionChanged( int pos );
    void LensFocusFineChanged( bool en );
    void LensZoomPositionChanged( int pos );
    void LensIrisPositionChanged( int pos );
    void LensFilterPositionChanged( int pos );
    void LensFocusSettingsChanged( QVector<int>  );
    void LensZoomSettingsChanged(  QVector<int>  );
    void LensIrisSettingsChanged(  QVector<int>  );
    void LensIrisApertureChanged( int apt );
    void LensIrisAperturePosChanged( int pos );
    void LensIrisSetupChanged( QVector<int> );
    void LensFilterSettingsChanged(  QVector<int>  );

public slots:
    void onLensSettingsChange( QVector<int> values );
    void onLensActiveChange( bool active );
    void onLensInvertChange( QVector<int> );
    void onLensFocusPositionChange( int pos);
    void onLensFocusFineChange( bool en );
    void onLensZoomPositionChange( int pos);
    void onLensZoomInvertChange( bool en );
    void onLensIrisPositionChange( int pos);
    void onLensIrisInvertChange( bool en );
    void onLensFilterPositionChange( int pos);
    void onLensFilterInvertChange( bool en );
    void onLensFocusSettingsChange( QVector<int>  );
    void onLensZoomSettingsChange(  QVector<int>  );
    void onLensIrisSettingsChange(  QVector<int>  );
    void onLensIrisSetupChange( QVector<int> values );
    void onLensIrisAperturePosChange( int pos );
    void onLensFilterSettingsChange( QVector<int>  );

private slots:
    void onCbxLensProfileChange( int index );
    void onBtnLensActiveChange( void );
    void onCbxLensEnableAdvancedSettings(int check);

    void onSbxLensFocusPositionChanged(int pos);
    void onSbxLensFocusSpeedChanged(int speed);
    void onSbxLensFocusStepModeChanged(int steMode);
    void onSbxLensFocusTorqueChanged(int torque);
    void onCbxLensFocusFineChanged( int en);
    void onCbxLensFocusInvertChanged( int enable);


    void onSbxLensZoomPositionChanged(int pos);
    void onSbxLensZoomSpeedChanged(int speed);
    void onSbxLensZoomStepModeChanged(int steMode);
    void onSbxLensZoomTorqueChanged(int torque);
    void onCbxLensZoomInvertChanged( int enable);

    void onSbxLensIrisPositionChanged(int pos);
    void onSbxLensIrisSpeedChanged(int speed);
    void onSbxLensIrisStepModeChanged(int steMode);
    void onSbxLensIrisTorqueChanged(int torque);
    void onCbxLensIrisInvertChanged( int enable);
    void onCbxLensIrisApertureChanged( int index);
    void onBtnLensIrisAperturePlusChanged( void );
    void onBtnLensIrisApertureMinusChanged( void );
    void onBtnLensIrisTableTransmitChanged( void );
    void onBtnLensIrisTableRemoveColumnChanged( void );
    void onCbxLensIrisTemplateChanged( int index );

    void onSbxLensFilterPositionChanged(int pos);
    void onSbxLensFilterSpeedChanged(int speed);
    void onSbxLensFilterStepModeChanged(int steMode);
    void onSbxLensFilterTorqueChanged(int torque);
    void onCbxLensFilterInvertChanged( int enable);


private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __LENS_DRIVER_BOX_H__
