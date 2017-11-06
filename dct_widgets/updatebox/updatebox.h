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
 * @file    updatebox.h
 *
 * @brief   Class definition of a system update box 
 *
 *****************************************************************************/
#ifndef __UPDATE_BOX_H__
#define __UPDATE_BOX_H__

#include <dct_widgets_base.h>

/******************************************************************************
 * System Update Box Widget
 *****************************************************************************/
class UpdateBox : public DctWidgetBox
{
    Q_OBJECT

public:
    enum SystemStates
    {
        InvalidState = -1,
        CommandState =  0,
        UpdateState  =  1,
        FlashState   =  2,
        ErrorState   =  3,
    };

    explicit UpdateBox( QWidget * parent = 0 );
    ~UpdateBox();

protected:
    void prepareMode( const Mode mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

private:
    SystemStates getSystemState( void );
    void setSystemState( SystemStates state );
    unsigned int getTotalNumUpdates();
    void setUpdateCounter( unsigned int updCnt );
    void incrementUpdateCounter();
    void getFirstUpdateIndex();
    void getNextUpdateIndex();

signals:
    void BootIntoUpdateMode();
    void CloseSerialConnection();
    void ReopenSerialConnection();
    void LockCurrentTabPage( bool );

public slots:
    void onPromptChange( uint8_t flag );
    void onSystemPlatformChange( QString name );

private slots:
    void onFsmTimer();
    
    void onFileNameClicked();
    void onRunClicked();
    void onVerifyChanged( int );

    void onFlashLoaderVersion( quint32 m, quint32 n );
    void onSystemId( qint32 id );
    void onSystemName( QString name );
    void onSystemVersion( quint32 m, quint32 n, quint32 o );
    void onFlashBlockNo( quint32 n );
    void onFlashBlockSize( quint32 sz );
    void onFirstFlashBlock( quint32 id );
    void onReconfCond( quint32 mask );
    void onFlashLoaderError( int no );
    
    void onReadbackProgress( quint32 progress );
    void onEraseProgress( quint32 progress );
    void onProgramProgress( quint32 progress );
    void onVerifyProgress( quint32 progress );
    void onUpdateFinished();

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __UPDATE_BOX_H__

