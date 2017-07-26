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
 * @file    fltbox.cpp
 *
 * @brief   Implementation of filter configuration box
 *
 *****************************************************************************/
#include "defines.h"
#include "playbackbox.h"
#include "ui_playbackbox.h"

#include <QtDebug>
#include <QSignalMapper>
#include <QTimer>

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_PlayBackBox;
}

/******************************************************************************
 * local definitions
 *****************************************************************************/
#define MAX_BANKS                       ( 4 )

#define BANK_CONFIGURATION_DEFAULT_1    ( 1 )
#define BANK_CONFIGURATION_DEFAULT_2    ( 2 )
#define BANK_CONFIGURATION_DEFAULT_3    ( 3 )
#define BANK_CONFIGURATION_DEFAULT_4    ( 4 )
#define BANK_CONFIGURATION_USER         ( 5 )

/******************************************************************************
 * class Bank
 *****************************************************************************/
class Bank
{

private:
    void setSpinBoxValue( QSpinBox * sbx, int value )
    {
        sbx->blockSignals( true );
        sbx->setValue( value );
        sbx->blockSignals( false );
    }

    void setLineEditValue( QLineEdit * let, double value )
    {
        QString s;
        s.sprintf( "%4.2fs", value );

        let->blockSignals( true );
        let->setText( s );
        let->blockSignals( false );
    }

    void updRecordDuration()
    {
        setLineEditValue( m_letRecordDuration , ((double)size()) / m_cam_fps );
    }

    void updPlayOutDuration()
    {
        setLineEditValue( m_letPlayOutDuration, ((double)(size() * 1000)) / m_out_fps );
    }

public:
    enum State
    {
        FirstState = 0,
        Recording  = 0,
        Playing,
        Live,
        Paused,
        Locked,
        Free,
        Invalid,
        LastState
    };

    Bank
    (
        int             id,
        QLabel *        lblBankId,
        QSpinBox *      sbxStartFrame,
        QSpinBox *      sbxEndFrame,
        QSpinBox *      sbxSize,
        QLineEdit *     letRecordDuration,
        QLineEdit *     letPlayOutDuration,
        QToolButton *   btnBank,
        QSpacerItem *   hspBank
    )
    {
        m_lblBankId         = lblBankId;
        m_sbxStartFrame     = sbxStartFrame;
        m_sbxEndFrame       = sbxEndFrame;
        m_sbxSize           = sbxSize;
        m_letRecordDuration = letRecordDuration;
        m_letPlayOutDuration= letPlayOutDuration;
        m_btnBank           = btnBank;
        m_hspBank           = hspBank;

        m_id                = id;
        m_cam_fps           = 300;
        m_out_fps           = 50;

        m_clicked           = 0;
        
        m_btnBank->setStyleSheet(
                "QToolButton {"
                    "color: silver;"
                    "background-color: #302F2F;"
                    "border-width: 1px;"
                    "border-color: #4A4949;"
                    "border-style: solid;"
                    "padding-top: 5px;"
                    "padding-bottom: 5px;"
                    "padding-left: 5px;"
                    "padding-right: 5px;"
                    "border-radius: 2px;"
                    "outline: none;"
                "}"

                "QToolButton:hover {"
                    "border: 1px solid #78879b;"
                    "color: silver;"
                "}"

                "QToolButton:checked {"
                    "background-color: #4A4949;"
                    "border-color: #6A6969;"
                "}"

                "QToolButton:pressed {"
                    "background-color: #484846;"
                "}"
            );

        QString s;
        s.sprintf( "Bank %d", (id+1) );
        m_btnBank->setText( s );

        setState( Invalid );
    }

    int startFrame() const
    {
        return ( m_sbxStartFrame->value() );
    }

    int endFrame() const
    {
        return ( m_sbxEndFrame->value() );
    }

    int size() const
    {
        return ( m_sbxSize->value() );
    }

    int clickCnt() const
    {
        return ( m_clicked );
    }

    bool isChecked() const
    {
        return ( m_btnBank->isChecked() );
    }

    void setChecked( bool checked )
    {
        m_btnBank->setChecked( checked );
        if ( !checked )
        {
            m_clicked = 0;
        }
        else
        {
            m_clicked++;
        }
    }

    void setEnable( bool enable, bool readonly = true )
    {
        // bank configuration
        m_lblBankId         ->setVisible( enable );
        m_sbxStartFrame     ->setVisible( enable );
        m_sbxStartFrame     ->setReadOnly( readonly );
        m_sbxEndFrame       ->setVisible( enable );
        m_sbxEndFrame       ->setReadOnly( readonly );
        m_sbxSize           ->setVisible( enable );
        m_sbxSize           ->setReadOnly( readonly );
        m_letRecordDuration ->setVisible( enable );
        m_letPlayOutDuration->setVisible( enable );

        // bank selection
        if ( enable )
        {
            m_hspBank->changeSize( 40, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
        }
        else
        {
            m_hspBank->changeSize( 0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed );
        }

        m_btnBank->setVisible( enable );

        // update bank state
        setState( enable ? Free : Invalid );
    }

    int getId() const
    {
        return ( m_id );
    }

    State getState() const
    {
        return ( m_state );
    }

    void setState( State s )
    {
        switch ( s ) 
        {
            case Recording:
                m_btnBank->setIcon(QIcon(":/images/banks/recording.png"));
                break;

            case Live:
                m_btnBank->setIcon(QIcon(":/images/banks/live.png"));
                break;
            
            case Playing:
                m_btnBank->setIcon(QIcon(":/images/banks/playing.png"));
                break;

            case Paused:
                m_btnBank->setIcon(QIcon(":/images/banks/paused.png"));
                break;
            
            case Locked:
                m_btnBank->setIcon(QIcon(":/images/banks/locked.png"));
                break;
            
            case Free:
            case Invalid:
                m_btnBank->setIcon(QIcon(":/images/banks/unused.png"));
                break;

            default:
                break;
        }
        m_state = s;
    }

    void setCameraFps( int value )
    {
        if ( value > 0 )
        {
            m_cam_fps = value;
            updRecordDuration();
        }
    }

    void setVideoModeSpeed( int value )
    {
        if ( value > 0 )
        {
            m_out_fps = GetVideoModeSpeed( (VideoMode)value );
            updPlayOutDuration();
        }
    }

    void setMaxFrames( int value )
    {
        m_sbxStartFrame->setMaximum( value );
        m_sbxEndFrame  ->setMaximum( value );
        m_sbxSize      ->setMaximum( value + 1 );
        m_max = value;
    }
 
    void setValues( int start, int end )
    {
        if ( (0 <= start) && (start < end) && (end < (m_max + 1)) )
        {
            int size = (end - start + 1);

            setSpinBoxValue( m_sbxStartFrame, start );
            setSpinBoxValue( m_sbxEndFrame  , end );
            setSpinBoxValue( m_sbxSize      , size );

            updRecordDuration();
            updPlayOutDuration();
        }
    }

    bool operator==( const Bank & rhs ) const
    {
        return ( (startFrame() == rhs.startFrame()) && 
                 (endFrame()   == rhs.endFrame()  ) && 
                 (size()       == rhs.size()      ) );
    }

    bool operator<( const Bank & rhs ) const
    {
        return ( (startFrame()  != rhs.startFrame()) && 
                 (endFrame()    != rhs.endFrame()  ) &&
                 (endFrame()+1  == rhs.startFrame()) &&
                 (size()        == rhs.size()      ) );
    }

private:
    int             m_id;                   /**< identifier */
    int             m_max;                  /**< max number of available frames */
    int             m_cam_fps;              /**< camera framerate */
    int             m_out_fps;              /**< playout speed */

    State           m_state;
    int             m_clicked;

    QLabel *        m_lblBankId;            /**< points to bank-id label */
    QSpinBox *      m_sbxStartFrame;        /**< points to start-frame spin-box */
    QSpinBox *      m_sbxEndFrame;          /**< points to end-frame spin-box */
    QSpinBox *      m_sbxSize;              /**< points to size spin-box */
    QLineEdit *     m_letRecordDuration;    /**< points to record duration line-edit */
    QLineEdit *     m_letPlayOutDuration;   /**< points to playout duration line-edit */
    QToolButton *   m_btnBank;              /**< points to bank selection push-button */
    QSpacerItem *   m_hspBank;              /**< points to bank spacer */
};

/******************************************************************************
 * PlayBackBox::PrivateData
 *****************************************************************************/
class PlayBackBox::PrivateData
{
private:
    void setComboBoxBankConfigurationMode( int mode )
    {
        int index = m_ui->cbxBankSetup->findData( mode );
        if ( index != -1 )
        {
            m_ui->cbxBankSetup->blockSignals( true );
            m_ui->cbxBankSetup->setCurrentIndex( index );
            m_ui->cbxBankSetup->blockSignals( false );
        }

        m_mode = mode;
    }

    void evaluateBanks()
    {
        if ( (*m_banks[0] < *m_banks[1]) && (*m_banks[1] < *m_banks[2]) && (*m_banks[2] < *m_banks[3]) )
        {
            // Bank Configuration: Default 4
            setComboBoxBankConfigurationMode( BANK_CONFIGURATION_DEFAULT_4 );
            foreach( Bank * b, m_banks )
            {
                b->setEnable( true );
            }
        }
        else if ( (*m_banks[0] < *m_banks[1]) && (*m_banks[1] < *m_banks[2]) )
        {
            // Bank Configuration: Default 4
            setComboBoxBankConfigurationMode( BANK_CONFIGURATION_DEFAULT_3 );
            m_banks[0]->setEnable( true ); 
            m_banks[1]->setEnable( true ); 
            m_banks[2]->setEnable( true ); 
            m_banks[3]->setEnable( false ); 
        }
        else if ( *m_banks[0] < *m_banks[1] )
        {
            // Bank Configuration: Default 2
            setComboBoxBankConfigurationMode( BANK_CONFIGURATION_DEFAULT_2 );
            m_banks[0]->setEnable( true  ); 
            m_banks[1]->setEnable( true  ); 
            m_banks[2]->setEnable( false ); 
            m_banks[3]->setEnable( false ); 
        }
        else if ( (*m_banks[0] == *m_banks[1]) && (*m_banks[2] == *m_banks[3]) && (*m_banks[0] == *m_banks[3]) )
        {
            // Bank Configuration: Default 1
            setComboBoxBankConfigurationMode( BANK_CONFIGURATION_DEFAULT_1 );
            m_banks[0]->setEnable( true  ); 
            m_banks[1]->setEnable( false ); 
            m_banks[2]->setEnable( false ); 
            m_banks[3]->setEnable( false ); 
        }
        else
        {
            // Bank Configuration: User defined
            setComboBoxBankConfigurationMode( BANK_CONFIGURATION_USER );
            foreach( Bank * b, m_banks )
            {
                b->setEnable( true, false );
            }
        }
    }
 
 public:
    PrivateData( QWidget * parent )
        : m_ui( new Ui::UI_PlayBackBox )
        , m_rec_bank( -1 )
        , m_play_bank( -1 )
        , m_sel_bank( 0 )
        , m_init_sync( true )
        , m_dial_value( 25 )
        , m_dial_delta( 0 )
    {
        // initialize UI
        m_ui->setupUi( parent );
        
        m_mapper   = new QSignalMapper( parent );

        m_banks.resize( MAX_BANKS );

        // create bank button 0
        m_banks[0] = new Bank( 0, m_ui->lblBankId0, 
                            m_ui->sbxStartFrame0, m_ui->sbxEndFrame0, m_ui->sbxSize0,
                            m_ui->letRecordDuration0, m_ui->letPlayOutDuration0,
                            m_ui->btnBank0, m_ui->hspBank0 );
        m_banks[0]->setChecked( true );
        QObject::connect( m_ui->btnBank0, SIGNAL(clicked()), m_mapper, SLOT(map()) );
        m_mapper->setMapping( m_ui->btnBank0, 0 );

        // create bank button 1
        m_banks[1] = new Bank( 1, m_ui->lblBankId1, 
                            m_ui->sbxStartFrame1, m_ui->sbxEndFrame1, m_ui->sbxSize1,
                            m_ui->letRecordDuration1, m_ui->letPlayOutDuration1,
                            m_ui->btnBank1, m_ui->hspBank1 );
        m_banks[1]->setChecked( false );
        QObject::connect( m_ui->btnBank1, SIGNAL(clicked()), m_mapper, SLOT(map()) );
        m_mapper->setMapping( m_ui->btnBank1, 1 );

        // create bank button 2
        m_banks[2] = new Bank( 2, m_ui->lblBankId2, 
                            m_ui->sbxStartFrame2, m_ui->sbxEndFrame2, m_ui->sbxSize2,
                            m_ui->letRecordDuration2, m_ui->letPlayOutDuration2,
                            m_ui->btnBank2, m_ui->hspBank2 );
        m_banks[2]->setChecked( false );
        QObject::connect( m_ui->btnBank2, SIGNAL(clicked()), m_mapper, SLOT(map()) );
        m_mapper->setMapping( m_ui->btnBank2, 2 );
        
        // create bank button 3
        m_banks[3] = new Bank( 3, m_ui->lblBankId3, 
                            m_ui->sbxStartFrame3, m_ui->sbxEndFrame3, m_ui->sbxSize3,
                            m_ui->letRecordDuration3, m_ui->letPlayOutDuration3,
                            m_ui->btnBank3, m_ui->hspBank3 );
        m_banks[3]->setChecked( false );
        QObject::connect( m_ui->btnBank3, SIGNAL(clicked()), m_mapper, SLOT(map()) );
        m_mapper->setMapping( m_ui->btnBank3, 3 );

        // add configuration modes
        m_ui->cbxBankSetup->addItem( "Default 1"   , BANK_CONFIGURATION_DEFAULT_1 );
        m_ui->cbxBankSetup->addItem( "Default 2"   , BANK_CONFIGURATION_DEFAULT_2 );
        m_ui->cbxBankSetup->addItem( "Default 3"   , BANK_CONFIGURATION_DEFAULT_3 );
        m_ui->cbxBankSetup->addItem( "Default 4"   , BANK_CONFIGURATION_DEFAULT_4 );
        m_ui->cbxBankSetup->addItem( "User defined", BANK_CONFIGURATION_USER );

        // set position dial to value=max/2 to avoid triggering the wrap-around corner case
        // positive  effect is that the notch points to north
        m_ui->Position->blockSignals( true );
        m_ui->Position->setValue( (m_ui->Position->maximum() >> 1) );
        m_ui->Position->blockSignals( false );

        m_pos_timer = new QTimer();
    };

    ~PrivateData()
    {
        delete m_pos_timer;

        foreach( Bank * b, m_banks )
        {
            delete b;    
        }

        delete m_mapper;
        delete m_ui;
    };

    void setCameraFps( int value )
    {
        foreach( Bank * b, m_banks )
        {
            b->setCameraFps( value );
        }
    }

    void setVideoModeSpeed( int value )
    {
        foreach( Bank * b, m_banks )
        {
            b->setVideoModeSpeed( value );
        }

        // wind up position timer to once per playout second
        if ( value > 0 )
        {
            m_pos_timer->setInterval( GetVideoModeSpeed( (VideoMode)value ) / 1000 );
        }
    }

    void setMaxFrames( int value )
    {
        foreach( Bank * b, m_banks )
        {
            b->setMaxFrames( value );
        }
    }

    void setBank( int id, int start, int end )
    {
        if ( (id >= 0) && (id < MAX_BANKS) ) 
        {
            m_banks[id]->setValues( start, end );
            evaluateBanks();
        }
    }

    int getFreeBank()
    {
        foreach( Bank * b, m_banks )
        {
            if ( b->getState() == Bank::Free )
            {
                return ( b->getId() );
            }
        }
        return ( -1 );
    }

    void setBankUserDefined( )
    {
        foreach( Bank * b, m_banks )
        {
            b->setEnable( true, false );
        }
    }

    Ui::UI_PlayBackBox *  m_ui;                 /**< ui handle */
   
    int                   m_rec_bank;           /**< index of current recording bank */
    int                   m_play_bank;          /**< index of current play-back bank */
    int                   m_sel_bank;           /**< currently selected bank */
    bool                  m_init_sync;          /**< initial sync/start-up */

    // position knob handling
    int                   m_dial_value;         /**< current dial value to compute delta */
    int                   m_dial_delta;         /**< current dial value to compute delta */

    // combobox bank configuration mode
    int                   m_mode;               /**< current bank configuration mode */

    QVector<Bank *>       m_banks;              /**< bank array */
    QSignalMapper *       m_mapper;             /**< signal mapper for bank-buttons */
    QTimer *              m_pos_timer;          /**< get-position timer */
};

/******************************************************************************
 * PlayBackBox::PlayBackBox
 *****************************************************************************/
PlayBackBox::PlayBackBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData( this );

    connect( d_data->m_ui->cbxBankSetup,  SIGNAL(activated(int)), this, SLOT(onBankDefaultChange(int)) );
    connect( d_data->m_mapper, SIGNAL(mapped(int)), this, SLOT(onBankClick(int)) );

    connect( d_data->m_ui->btnRecord , SIGNAL(clicked()), this, SLOT(onRecordClick()) );
    connect( d_data->m_ui->btnLive   , SIGNAL(clicked()), this, SLOT(onLiveClick()) );
    connect( d_data->m_ui->btnPlay   , SIGNAL(clicked()), this, SLOT(onPlayClick()) );
    connect( d_data->m_ui->btnStop   , SIGNAL(clicked()), this, SLOT(onStopClick()) );
    connect( d_data->m_ui->btnFirst  , SIGNAL(clicked()), this, SLOT(onFirstClick()) );
    connect( d_data->m_ui->btnLast   , SIGNAL(clicked()), this, SLOT(onLastClick()) );
    connect( d_data->m_ui->btnForward, SIGNAL(clicked()), this, SLOT(onForwardClick()) );
    connect( d_data->m_ui->btnRewind , SIGNAL(clicked()), this, SLOT(onRewindClick()) );

    connect( d_data->m_ui->Position  , SIGNAL(valueChanged(int)), this, SLOT(onRotationChange(int)) );
    connect( d_data->m_ui->Position  , SIGNAL(sliderMoved(int)) , this, SLOT(calcRotationDirection(int)) );
    connect( d_data->m_pos_timer     , SIGNAL(timeout())        , this, SLOT(onPosTimeOut()) );
}

/******************************************************************************
 * PlayBackBox::~PlayBackBox
 *****************************************************************************/
PlayBackBox::~PlayBackBox()
{
    delete d_data;
}

/******************************************************************************
 * PlayBackBox::prepareMode
 *****************************************************************************/
void PlayBackBox::prepareMode( const Mode )
{
    // do nothing here
}

/******************************************************************************
 * PlayBackBox::loadSettings
 *****************************************************************************/
void PlayBackBox::loadSettings( QSettings & )
{
    // do nothing here
}

/******************************************************************************
 * PlayBackBox::saveSettings
 *****************************************************************************/
void PlayBackBox::saveSettings( QSettings & )
{
    // do nothing here
}

/******************************************************************************
 * PlayBackBox::applySettings
 *****************************************************************************/
void PlayBackBox::applySettings( void )
{
    // do nothing here
}

/******************************************************************************
 * PlayBackBox::onCameraFramerateChange
 *****************************************************************************/
void PlayBackBox::onCameraFramerateChange( int value )
{
    d_data->setCameraFps( value );
}

/******************************************************************************
 * PlayBackBox::onChainVideoModeChange
 *****************************************************************************/
void PlayBackBox::onChainVideoModeChange( int value )
{
    d_data->setVideoModeSpeed( value );
}

/******************************************************************************
 * PlayBackBox::onMaxFramesChange
 *****************************************************************************/
void PlayBackBox::onMaxFramesChange( int value )
{
    d_data->setMaxFrames( value );
}

/******************************************************************************
 * PlayBackBox::onBankChange
 *****************************************************************************/
void PlayBackBox::onBankChange(int id, int start, int end )
{
    d_data->setBank( id, start, end );
}

/******************************************************************************
 * PlayBackBox::onAutoLiveModeChange
 *****************************************************************************/
void PlayBackBox::onAutoLiveModeChange( int flag )
{
    // set value of checkbox
    d_data->m_ui->cbxAutoLive->blockSignals( true );
    d_data->m_ui->cbxAutoLive->setCheckState( flag ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxAutoLive->blockSignals( false );
}

/******************************************************************************
 * PlayBackBox::onPlayChange
 *****************************************************************************/
void PlayBackBox::onPlayChange( int id )
{
    // set initial selected bank (first synchronization with device)
    if ( d_data->m_init_sync )
        {
        // set currently selected bank
        if ( (id >= 0) && (id < MAX_BANKS) )
        {
            // set initally selected bank to deselect state
            d_data->m_banks[d_data->m_sel_bank]->setChecked( false );
            
            // set current play-back bank to selected state
            d_data->m_banks[id]->setChecked( true );
            d_data->m_sel_bank = id;
        }
        d_data->m_init_sync = false;
    }

    // stop position timer
    d_data->m_pos_timer->stop();

    // handle old playback bank
    if ( (d_data->m_play_bank >= 0) && (d_data->m_play_bank < MAX_BANKS) && // range
         (d_data->m_play_bank != id)                                     && // old != new play-back bank
         (d_data->m_play_bank != d_data->m_rec_bank)                     )  // old != rec bank
    {
        // set the bank in locked state
        d_data->m_banks[d_data->m_play_bank]->setState( Bank::Locked );
    }

    // change play-back bank state
    if ( (id >= 0) && (id < MAX_BANKS) )
    {
        if (  (d_data->m_banks[id]->getState() == Bank::Recording) ||
              (d_data->m_banks[id]->getState() == Bank::Live)      )
        {
            d_data->m_banks[id]->setState( Bank::Live );
        }
        else
        {
            d_data->m_banks[id]->setState( Bank::Playing );

            // start position timer
            d_data->m_pos_timer->start();
        }
    }

    // update play-back bank 
    // Note: -1 in case of no play-back is also allowed here
    d_data->m_play_bank = id;
}

/******************************************************************************
 * PlayBackBox::onRecordChange
 *****************************************************************************/
void PlayBackBox::onRecordChange( int id )
{
    // handle old recording bank
    if ( (d_data->m_rec_bank >= 0) && (d_data->m_rec_bank < MAX_BANKS) &&
         (d_data->m_rec_bank != id)                                    )
    {
        // set the bank in locked state
        d_data->m_banks[d_data->m_rec_bank]->setState( Bank::Locked );
    }

    // handle new recording bank
    if ( (id >= 0) && (id < MAX_BANKS) )
    {
        d_data->m_banks[id]->setState( Bank::Recording );
    }

    // update record bank
    // Note: -1 in case of no record is also allowed here
    d_data->m_rec_bank = id;
}

/******************************************************************************
 * PlayBackBox::onPausedChange
 *****************************************************************************/
void PlayBackBox::onPausedChange( bool flag )
{
    if ( (d_data->m_play_bank >= 0) && (d_data->m_play_bank < MAX_BANKS) )
    {
        if ( d_data->m_banks[d_data->m_play_bank]->getState() != Bank::Live )
        {
            if ( flag )
            {
                d_data->m_pos_timer->stop();
                d_data->m_banks[d_data->m_play_bank]->setState( Bank::Paused );
            }
            else
            {
                d_data->m_banks[d_data->m_play_bank]->setState( Bank::Playing );
            }
        }
    }
}

/******************************************************************************
 * PlayBackBox::onBankDefaultChange
 *****************************************************************************/
void PlayBackBox::onBankDefaultChange( int index )
{
    int mode = d_data->m_ui->cbxBankSetup->itemData( index ).toInt();

    if ( mode !=  d_data->m_mode )
    {   
        if ( (mode >= BANK_CONFIGURATION_DEFAULT_1) && (mode < BANK_CONFIGURATION_USER) )
        {
            emit BankDefaultChanged( mode );

            // reset to default recording
            d_data->m_init_sync = true;

            d_data->m_rec_bank  = -1;
            d_data->m_play_bank = -1;
           
            emit PlayStopped();
            emit RecordChanged( 0 );
            emit LiveChanged();
        }
        else
        {
            d_data->setBankUserDefined();
        }
    }
}

/******************************************************************************
 * PlayBackBox::onPositionChange
 *****************************************************************************/
void PlayBackBox::onPositionChange( int pos )
{
    QString s;
    s.sprintf("%4d", pos );
    d_data->m_ui->letPosition->setText( s );
}

/******************************************************************************
 * PlayBackBox::onBankClick
 *****************************************************************************/
void PlayBackBox::onBankClick( int id )
{
    if ( d_data->m_banks[id]->clickCnt() > 0 )
    {
        if ( d_data->m_banks[id]->getState() == Bank::Locked )
        {
            d_data->m_banks[id]->setState( Bank::Free );
        }
        else if ( d_data->m_banks[id]->getState() == Bank::Free )
        {
            d_data->m_banks[id]->setState( Bank::Locked );
        }
    }

    foreach( Bank * b, d_data->m_banks )
    {
        b->setChecked( (d_data->m_banks[id] != b) ? false : true );
    }

    d_data->m_sel_bank = id;
}

/******************************************************************************
 * PlayBackBox::onRecordClick
 *****************************************************************************/
void PlayBackBox::onRecordClick()
{
    // get a free bank for new recording
    int id = d_data->getFreeBank();

    if ( id != -1 )
    {
        // found a free bank => switch recording to new bank
        emit RecordChanged( id );
    }
    else
    {
        // no free bank => stop recording
        if ( (d_data->m_rec_bank >= 0) && (d_data->m_rec_bank < MAX_BANKS )  &&
             (d_data->m_banks[d_data->m_rec_bank]->getState() == Bank::Live) )
        {
            // old bank is in live state
            //  => stop recording 
            //  => set bank to play-state
            //  => pause bank
            emit RecordStoppedPlayPause( d_data->m_rec_bank );
        }
        else
        {
            // stop recording
            emit RecordStopped();
        }
    }
}

/******************************************************************************
 * PlayBackBox::onLiveClick
 *****************************************************************************/
void PlayBackBox::onLiveClick()
{
    // case 1: no play-back
    // case 2: play-back and not live
    if (  (d_data->m_play_bank < 0) ||
          ((d_data->m_play_bank >= 0) && (d_data->m_play_bank < MAX_BANKS)  &&
           (d_data->m_banks[d_data->m_play_bank]->getState() != Bank::Live) ) 
       )
    {
        emit LiveChanged();
    }
}

/******************************************************************************
 * PlayBackBox::onPlayClick
 *****************************************************************************/
void PlayBackBox::onPlayClick()
{
    // selected bank in range
    if ( (d_data->m_sel_bank >= 0) && (d_data->m_sel_bank < MAX_BANKS ) )
    {
        // I.1: selected bank != playback bank
        if ( d_data->m_sel_bank != d_data->m_play_bank )
        {
            emit PlayChanged( d_data->m_sel_bank );
        }

        // II.1: selected bank == playback bank && selected bank == paused
        // II.2: selected bank == playback bank && selected bank == live 
        else if ( d_data->m_sel_bank == d_data->m_play_bank )
        {
            if (  (d_data->m_banks[d_data->m_sel_bank]->getState() == Bank::Live)   ||
                  (d_data->m_banks[d_data->m_sel_bank]->getState() == Bank::Paused) )
            {
                emit PlayChanged( d_data->m_sel_bank );
            }
        }
    }
}

/******************************************************************************
 * PlayBackBox::onStopClick
 *****************************************************************************/
void PlayBackBox::onStopClick()
{
    // play running 
    if ( (d_data->m_play_bank >= 0) && (d_data->m_play_bank < MAX_BANKS) ) 
    {
        // play is live or paused ?
        if ( (d_data->m_banks[d_data->m_play_bank]->getState() == Bank::Live)   ||
             (d_data->m_banks[d_data->m_play_bank]->getState() == Bank::Paused) )
        {
            emit PlayStopped();
        }
        else
        {
            emit PlayPaused();
        }
    }
}

/******************************************************************************
 * PlayBackBox::onFirstClick
 *****************************************************************************/
void PlayBackBox::onFirstClick()
{
    // if play-back running and not live
    if ( (d_data->m_play_bank >= 0) && (d_data->m_play_bank < MAX_BANKS)    && 
         (d_data->m_banks[d_data->m_play_bank]->getState() != Bank::Live)   )
    {
        emit SeekChanged( 1 );
    }
}

/******************************************************************************
 * PlayBackBox::onLastClick
 *****************************************************************************/
void PlayBackBox::onLastClick()
{
    // if play-back running and not live
    if ( (d_data->m_play_bank >= 0) && (d_data->m_play_bank < MAX_BANKS)    && 
         (d_data->m_banks[d_data->m_play_bank]->getState() != Bank::Live)   )
    {
        emit SeekChanged( d_data->m_banks[d_data->m_play_bank]->endFrame() );
    }
}

/******************************************************************************
 * PlayBackBox::onForwardClick
 *****************************************************************************/
void PlayBackBox::onForwardClick()
{
    // if play-back running and not live
    if ( (d_data->m_play_bank >= 0) && (d_data->m_play_bank < MAX_BANKS)    && 
         (d_data->m_banks[d_data->m_play_bank]->getState() != Bank::Live)   )
    {
        emit ForwardChanged( 1 );
    }
}

/******************************************************************************
 * PlayBackBox::onRewindClick
 *****************************************************************************/
void PlayBackBox::onRewindClick()
{
    // if play-back running and not live
    if ( (d_data->m_play_bank >= 0) && (d_data->m_play_bank < MAX_BANKS)    && 
         (d_data->m_banks[d_data->m_play_bank]->getState() != Bank::Live)   )
    {
        emit RewindChanged( 1 );
    }
}

/******************************************************************************
 * PlayBackBox::onRotationChange
 *****************************************************************************/
void PlayBackBox::onRotationChange( int value )
{
    (void) value; // all calculation are done 
    
    // if play-back running and not live
    if ( (d_data->m_play_bank >= 0) && (d_data->m_play_bank < MAX_BANKS)    && 
         (d_data->m_banks[d_data->m_play_bank]->getState() != Bank::Live)   )
    {
        if ( d_data->m_dial_delta > 0 )
        {
            emit ForwardChanged( d_data->m_dial_delta );
        }
        else
        {
            emit RewindChanged( d_data->m_dial_delta );
        }
    }
}

/******************************************************************************
 * PlayBackBox::calcRotationDirection
 *****************************************************************************/
void PlayBackBox::calcRotationDirection( int value )
{
    int min = d_data->m_ui->Position->minimum();
    int max = d_data->m_ui->Position->maximum();
    int direction;

    // I. compute difference to previous position
    int difference = d_data->m_dial_value - value;
    if ( difference < -(max / 2) )
    {
        difference += max;
    }
    else if ( difference > (max / 2) )
    {
        difference -= max;
    }

    if ( difference == 0 )
    {
        return;
    }

    // II. check for clock-wise wrap arround
    if ( value == min )
    {
        if ( d_data->m_dial_value == max )
        {
            direction = 1;
        }
        else
        {
            direction = -1;
        }
    }
    
    // III. check for counter clock-wise wrap arround
    else if ( value == max )
    {
        if ( d_data->m_dial_value == min )
        {
            direction = -1;
        }
        else
        {
            direction = 1;
        }
    }
    
    // IV. normal case
    else
    {
        if ( difference > 0 )
        {
            direction = -1;
        }
        else
        {
            direction = 1;
        }
    }
    
    d_data->m_dial_delta = qAbs( difference ) * direction; 
    d_data->m_dial_value = value;
}

/******************************************************************************
 * PlayBackBox::onPosTimeOut
 *****************************************************************************/
void PlayBackBox::onPosTimeOut()
{
    if ( (d_data->m_play_bank >= 0) && (d_data->m_play_bank<MAX_BANKS)       &&
         (d_data->m_banks[d_data->m_play_bank]->getState() == Bank::Playing) )
    {
        emit PositionUpdate();
    }
}

/******************************************************************************
 * PlayBackBox::enterEvent
 *****************************************************************************/
void PlayBackBox::enterEvent( QEvent * )
{
    d_data->m_pos_timer->start();
} 

/******************************************************************************
 * PlayBackBox::leaveEvent
 *****************************************************************************/
void PlayBackBox::leaveEvent( QEvent * )
{
    d_data->m_pos_timer->stop();
} 


