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

#include <QNetworkReply>
#include <QMimeData>

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
        RebootState  =  3,
        ErrorState   =  4,
    };

    explicit UpdateBox( QWidget * parent = 0 );
    ~UpdateBox() Q_DECL_OVERRIDE;


    quint32 Baudrate() const;
    void setBaudrate( const quint32 baudrate );

    QString Portname() const;
    void setPortname( const QString& portname );

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

    void prepareMode( const Mode mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

private:
    /******************************************************************************
     * Firmware version structure
     *
     * @brief Represents the version of a firmware. Example: V1.2.3 where major
     *        release is 1, minor release is 2 and patch level is 3.
     *****************************************************************************/
    typedef struct version_s
    {
        int major_release;
        int minor_release;
        int patch_level;
    } version_t;

    QString mimeDataToUpdateFilePath( const QMimeData * mimeData );

    SystemStates getSystemState( void );
    void setSystemState( SystemStates state , bool force = false );

    unsigned int getTotalNumUpdates();
    void setUpdateCounter( unsigned int updCnt );
    void incrementUpdateCounter();
    void getFirstUpdateIndex();
    void getNextUpdateIndex();

    bool isNewVersion( version_t server_version, version_t current_version );
    void checkUpdateDirectory( QString updateDirectory );
    bool downloadFile( QUrl url );
    void downloadUpdate();

signals:
    void BootIntoUpdateMode();
    void CloseSerialConnection();
    void ReopenSerialConnection();
    void LockCurrentTabPage( bool );
    void DownloadProgress( int progressPercent );

public slots:
    void onPromptChange( uint8_t flag );
    void onSystemPlatformChange( QString name );
    void onApplicationVersionChange( QString version );

private slots:
    void onDownloadProgress( qint64 bytesReceived, qint64 bytesTotal );
    void onCheckFirmwareUpdateClicked();
    void onCheckGuiUpdateClicked();

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

