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
    enum SystemStates { InvalidState = -1, CommandState = 0, UpdateState = 1, FlashState = 2,  };

    explicit UpdateBox( QWidget * parent = 0 );
    ~UpdateBox();

    quint32 Baudrate() const;
    void setBaudrate( const quint32 baudrate );

    QString Portname() const;
    void setPortname( const QString& portname );

protected:
    void prepareMode( const Mode mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

private:
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

