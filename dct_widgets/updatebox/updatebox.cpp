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
 * @file    updatebox.cpp
 *
 * @brief   Implementation of system update box
 *
 *****************************************************************************/
#include <QtDebug>
#include <QProcess>
#include <QFileDialog>
#include <QDirIterator>
#include <QMessageBox>
#include <QTimer>
#include <QThread>
#include <QSemaphore>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QTemporaryDir>
#include <QCryptographicHash>
#include <QDesktopServices>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>

#include <common.h>

#include <infodialog.h>

#include "id.h"
#include "flashloader.h"
#include "updatebox.h"
#include "ui_updatebox.h"

/******************************************************************************
 * namespaces
 *****************************************************************************/
namespace Ui {
    class UI_UpdateBox;
}

/******************************************************************************
 * definitions
 *****************************************************************************/
#define SYSTEM_STATE_COMMAND        ( "Device in operation." )
#define SYSTEM_STATE_UPDATE         ( "Device is waiting for update." )
#define SYSTEM_STATE_FLASHING       ( "Device is being updated." )
#define SYSTEM_STATE_REBOOT         ( "Device is rebooting." )

#define VERSION_DOWNLOAD_SYNC       ( "version: " )
#define VERSION_DOWNLOAD_PARSE      ( "version: V%d_%d_%d\n" )
#define FILE0_DOWNLOAD_SYNC         ( "file0: ")
#define FILE0_DOWNLOAD_PARSE        ( "file0: %s %s\n" )
#define FILE1_DOWNLOAD_SYNC         ( "file1: ")
#define FILE1_DOWNLOAD_PARSE        ( "file1: %s %s\n" )
#define DOWNLOAD_SERVER             ( "https://gitlab.com/dreamchip/provideo-downloads/raw/master/auto_update/" )
#define GUI_DOWNLOAD_PAGE           ( "https://gitlab.com/dreamchip/provideo-downloads/wikis/provideo-gui" )

/******************************************************************************
 * update configuration structure
 *****************************************************************************/
enum updateType
{
    xbow,
    condor4k_fw, condor4k_bs,
    cooper_fw, cooper_bs,
    invalid
};

typedef struct update_config_s
{
    updateType type;           // update type (camera and partition)
    quint32    baudrate;       // communication baudrate to device
    quint32    sector;         // flash start sector
    QString    extension;      // file extension
    bool       content;        // check file content do clarify what to update
    bool       reversal;       // bit reversal
    QString    file;           // binary file that is flashed during update
    QString    description;    // description of the update type
} update_config_t;

// NOTE: xbow, bitstream contains firmware (2in1 update)
#define PLATFORM_XBOW     ( "xbow" )    // platform-name
const update_config_t xbow_update =
{
    /* .type        = */ xbow,
    /* .baudrate    = */ 0u,        // use automatic baudrate detection of flashloader
    /* .sector      = */ 128u,
    /* .extension   = */ "rpd",
    /* .content     = */ false,     // not needed it's clear
    /* .reversal    = */ true,
    /* .file        = */ "",
    /* .description = */ "Combined Bitstream and Software Update File"
};

// NOTE: condor4k and condor4k_mini, bitstream and firmware are seperate
#define PLATFORM_CONDOR_4K          ( "condor4k" )      // platform-name
#define PLATFORM_CONDOR_4K_MINI     ( "condor4k_mini" ) // platform-name
#define PLATFORM_CONDOR_4K_SP       ( 0x2001C000u )     // stack-pointer
#define PLATFORM_CONDOR_4K_PC       ( 0x08010000u )     // minimum program counter

// firmware update
const update_config_t condor4k_fw_update =
{
    /* .type        = */ condor4k_fw,
    /* .baudrate    = */ 0u,        // use automatic baudrate detection of flashloader
    /* .sector      = */ 1u,
    /* .extension   = */ "bin",
    /* .content     = */ true,      // needed to distinguish between firmware and bitstream
    /* .reversal    = */ false,
    /* .file        = */ QString::null,
    /* .description = */ "Software Update File"
};

// bitstream update
const update_config_t condor4k_bs_update =
{
    /* .type        = */ condor4k_bs,
    /* .baudrate    = */ 0u,        // use automatic baudrate detection of flashloader
    /* .sector      = */ 144u,
    /* .extension   = */ "bin",
    /* .content     = */ true,      // needed to distinguish between firmware and bitstream
    /* .reversal    = */ false,
    /* .file        = */ QString::null,
    /* .description = */ "Bitstream Update File"
};

// NOTE: cooper, bitstream and firmware are seperate
#define PLATFORM_COOPER     ( "cooper" )        // platform-name
#define PLATFORM_COOPER_SP  ( 0x2001C000u )     // stack-pointer
#define PLATFORM_COOPER_PC  ( 0x08010000u )     // minimum program counter

// firmware update
const update_config_t cooper_fw_update =
{
    /* .type        = */ cooper_fw,
    /* .baudrate    = */ 0u,        // use automatic baudrate detection of flashloader
    /* .sector      = */ 1u,
    /* .extension   = */ "bin",
    /* .content     = */ true,      // needed to distinguish between firmware and bitstream
    /* .reversal    = */ false,
    /* .file        = */ QString::null,
    /* .description = */ "Software Update File"
};

// bitstream update
const update_config_t cooper_bs_update =
{
    /* .type        = */ cooper_bs,
    /* .baudrate    = */ 0u,        // use automatic baudrate detection of flashloader
    /* .sector      = */ 4u,
    /* .extension   = */ "bin",
    /* .content     = */ true,      // needed to distinguish between firmware and bitstream
    /* .reversal    = */ false,
    /* .file        = */ QString::null,
    /* .description = */ "Bitstream Update File"
};

/******************************************************************************
 * fileExists
 *****************************************************************************/
static bool fileExists( const QString & path )
{
    QFileInfo check_file( path );

    // check if file exists and if yes: Is it really a file and no directory?
    return ( check_file.exists() && check_file.isFile() );
}

/******************************************************************************
 * fileTypeMatches
 *****************************************************************************/
static bool fileTypeMatches( const updateType type, const QString & fn )
{
    QFile f( fn );

    if ( !f.open( QIODevice::ReadOnly ) )
    {
        return ( false );
    }

    quint32 d[2];
    f.read( (char *)d, sizeof(quint32) * 2 );
    f.close();

    // Condor 4k
    if ( type == condor4k_fw || type == condor4k_bs )
    {
        // if stack-pointer matches and program-pointer is bigger than min, this is a firmware binary
        if ( type == condor4k_fw && ((d[0] == PLATFORM_CONDOR_4K_SP) && (d[1] >= PLATFORM_CONDOR_4K_PC)) )
        {
            return true;
        }
        // else this is a bitstream binary
        else if ( type == condor4k_bs && !((d[0] == PLATFORM_CONDOR_4K_SP) && (d[1] >= PLATFORM_CONDOR_4K_PC)) )
        {
            return true;
        }
    }

    // Cooper
    if ( type == cooper_fw || type == cooper_bs )
    {
        // if stack-pointer matches and program-pointer is bigger than min, this is a firmware binary
        if ( type == cooper_fw && ((d[0] == PLATFORM_COOPER_SP) && (d[1] >= PLATFORM_COOPER_PC)) )
        {
            return true;
        }
        // else this is a bitstream binary
        else if ( type == cooper_bs && !((d[0] == PLATFORM_COOPER_SP) && (d[1] >= PLATFORM_COOPER_PC)) )
        {
            return true;
        }
    }

    return false;
}

/******************************************************************************
 * UpdateBox::PrivateData
 *****************************************************************************/
class UpdateBox::PrivateData
{
public:

    PrivateData( QWidget * parent )
        : m_ui( new Ui::UI_UpdateBox )
        , m_FsmTimer( new QTimer( parent ) )
        , m_application ( new FlashLoader() )
        , m_state( InvalidState )
        , m_state_sema(1)
        , m_current_fw_version({0, 0, 0})
        , m_server_fw_version({0, 0, 0})
        , m_current_gui_version({0, 0, 0})
        , m_server_gui_version({0, 0, 0})
        , m_network_manager( new QNetworkAccessManager( parent) )
        , m_download_dir( NULL )
    {
        // setup ui
        m_ui->setupUi( parent );

        // set version labels to default
        m_ui->letCurrentFirmwareVersion->setText( "Not checked yet" );
        m_ui->letServerFirmwareVersion->setText( "Not checked yet" );
        m_ui->letCurrentGuiVersion->setText( "v" + QString(VERSION_STRING) );
        m_ui->letServerGuiVersion->setText( "Not checked yet" );

        // get current gui version
        sscanf( VERSION_STRING, "%d.%d.%d",
                      &m_current_gui_version.major_release,
                      &m_current_gui_version.minor_release,
                      &m_current_gui_version.patch_level );

        // set progress bar to 0%
        m_ui->progressBar->setValue( 0u );

        // disable update button as long as no filename selected
        m_ui->btnRun->setEnabled( false );

        m_ui->cbxVerify->blockSignals( true );
        m_ui->cbxVerify->setCheckState( m_application->Verify() ? Qt::Checked : Qt::Unchecked );
        m_ui->cbxVerify->blockSignals( false );

        // configure reboot timer
        m_FsmTimer->setSingleShot( true );
        QObject::connect( m_FsmTimer, SIGNAL(timeout()), parent, SLOT(onFsmTimer()) );

        // connect application events
        QObject::connect( m_application, SIGNAL(FlashLoaderVersion(quint32,quint32)), parent, SLOT(onFlashLoaderVersion(quint32,quint32)) );
        QObject::connect( m_application, SIGNAL(SystemId(qint32)), parent, SLOT(onSystemId(qint32)) );
        QObject::connect( m_application, SIGNAL(SystemName(QString)), parent, SLOT(onSystemName(QString)) );
        QObject::connect( m_application, SIGNAL(SystemVersion(quint32,quint32,quint32)), parent, SLOT(onSystemVersion(quint32,quint32,quint32)) );
        QObject::connect( m_application, SIGNAL(FlashBlockNo(quint32)), parent, SLOT(onFlashBlockNo(quint32)) );
        QObject::connect( m_application, SIGNAL(FlashBlockSize(quint32)), parent, SLOT(onFlashBlockSize(quint32)) );
        QObject::connect( m_application, SIGNAL(FirstFlashBlock(quint32)), parent, SLOT(onFirstFlashBlock(quint32)) );
        QObject::connect( m_application, SIGNAL(ReconfCond(quint32)), parent, SLOT(onReconfCond(quint32)) );
        QObject::connect( m_application, SIGNAL(FlashLoaderError(int)), parent, SLOT(onFlashLoaderError(int)) );
        QObject::connect( m_application, SIGNAL(ReadbackProgress(quint32)), parent, SLOT(onReadbackProgress(quint32)) );
        QObject::connect( m_application, SIGNAL(EraseProgress(quint32)), parent, SLOT(onEraseProgress(quint32)) );
        QObject::connect( m_application, SIGNAL(ProgramProgress(quint32)), parent, SLOT(onProgramProgress(quint32)) );
        QObject::connect( m_application, SIGNAL(VerifyProgress(quint32)), parent, SLOT(onVerifyProgress(quint32)) );
        QObject::connect( m_application, SIGNAL(UpdateFinished()), parent, SLOT(onUpdateFinished()) );

        // connect buttons and checkboxes
        QObject::connect( m_ui->btnCheckFirmwareUpdate, SIGNAL(clicked()), parent, SLOT(onCheckFirmwareUpdateClicked()) );
        QObject::connect( m_ui->btnCheckGuiUpdate, SIGNAL(clicked()), parent, SLOT(onCheckGuiUpdateClicked()) );
        QObject::connect( m_ui->btnFilename, SIGNAL(clicked()), parent, SLOT(onFileNameClicked()) );
        QObject::connect( m_ui->btnRun, SIGNAL(clicked()), parent, SLOT(onRunClicked()) );
        QObject::connect( m_ui->cbxVerify, SIGNAL(stateChanged(int)), parent, SLOT(onVerifyChanged(int)) );
    }

    ~PrivateData()
    {
        delete m_network_manager;
        delete m_application;
        delete m_FsmTimer;
        delete m_ui;

        // Delete the temporary download directory
        if ( m_download_dir )
        {
            delete m_download_dir;
        }
    }

    Ui::UI_UpdateBox *          m_ui;                   /**< ui handle */

    QTimer *                    m_FsmTimer;             /**< FSM timer */
    FlashLoader *               m_application;          /**< flashloader application process */
    UpdateBox::SystemStates     m_state;                /**< current system state */

    qint32                      m_id;                   /**< detected system identifier */
    quint32                     m_no_sec;               /**< number of flash sectors */
    quint32                     m_first_sec;            /**< id of first writeable sector */
    quint32                     m_size_sec;             /**< sector size */
    quint32                     m_reconf_cond;          /**< sector size */

    quint32                     m_erase_cnt;
    quint32                     m_program_cnt;
    quint32                     m_verify_cnt;
    quint32                     m_upd_cnt;              /**< number of the update currently being flashed (like: "1 of 2") */
    qint32                      m_upd_idx;              /**< index in m_upd_config of the update that is currently being flashed */
    QVector<update_config_t>    m_upd_config;           /**< vector of update configurations */
    QString                     m_upd_dir;              /**< path to the directory that contains binary files */

    QSemaphore                  m_state_sema;           /**< semaphore to protect the access to get / set state */

    version_t                   m_current_fw_version;    /**< firmware version which is currently installed on the device */
    version_t                   m_server_fw_version;     /**< firmware version which is available online */
    version_t                   m_current_gui_version;   /**< GUI version which is currently being used */
    version_t                   m_server_gui_version;    /**< GUI version which is available online */

    QNetworkAccessManager *     m_network_manager;      /**< network access manager used to download updates from the internet */
    QByteArray                  m_downloaded_data;      /**< data which has been downloaded by the network manager */
    QList<QString>              m_download_files;       /**< file names of the update files */
    QList<QString>              m_download_md5_sums;    /**< md5 checksums of the update files stored in m_download_files */

    QString                     m_system_platform;      /**< system platform string */
    QTemporaryDir *             m_download_dir;         /**< temporary download directory */
};

/******************************************************************************
 * UpdateBox::UpdateBox
 *****************************************************************************/
UpdateBox::UpdateBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData( this );

    // enable dragging update files from the explorer and dropping them on this widget
    setAcceptDrops( true );

    // set initial state
    setSystemState( CommandState );
}

/******************************************************************************
 * UpdateBox::~UpdateBox
 *****************************************************************************/
UpdateBox::~UpdateBox()
{
    delete d_data;
}

/******************************************************************************
 * UpdateBox::dragEnterEvent
 *****************************************************************************/
void UpdateBox::dragEnterEvent(QDragEnterEvent *event)
{
    // Check if a valid file was dragged
    if ( mimeDataToUpdateFilePath( event->mimeData() ).isEmpty() == false )
    {
        event->acceptProposedAction();
    }
}

/******************************************************************************
 * UpdateBox::dropEvent
 *****************************************************************************/
void UpdateBox::dropEvent(QDropEvent *event)
{
    // Check if a valid vile was dropped
    QString filePath = mimeDataToUpdateFilePath( event->mimeData() );
    if ( filePath.isEmpty() == false )
    {
        // Get file info
        QFileInfo updateFile( filePath );

        // This is a single file, there is no update directory
        d_data->m_upd_dir = updateFile.path();

        // reset data structures
        for ( int i = 0; i < d_data->m_upd_config.count(); i++ )
        {
            d_data->m_upd_config[i].file = QString::null;
        }

        // loop over all update configs and try to find a matching binary file
        for ( int i = 0; i < d_data->m_upd_config.count(); i++ )
        {
            if ( updateFile.isFile() &&
                 updateFile.suffix() == d_data->m_upd_config[i].extension )
            {
                // if content has to be checked
                if ( d_data->m_upd_config[i].content )
                {
                    // check file type matches, if it does not, continue with next file
                    if ( !fileTypeMatches(d_data->m_upd_config[i].type, filePath) )
                    {
                        continue;
                    }
                }

                // Update file found
                d_data->m_upd_config[i].file = filePath;

                // Enable ui elements to start update
                /*  counter will be incremented in the onUpdateFinished() slot,
                 * which will then restart the timer, if more updates are pending */
                setUpdateCounter( 1 );
                d_data->m_ui->btnRun->setEnabled( true );
                d_data->m_ui->cbxVerify->setEnabled( true );
            }
        }
    }

    // Call getFirstUpdateIndex() to setup line edits with the update path (or clear them if no update found)
    getFirstUpdateIndex();

    event->acceptProposedAction();
}

/******************************************************************************
 * UpdateBox::~UpdateBox
 *****************************************************************************/
quint32 UpdateBox::Baudrate() const
{
    return ( d_data->m_application->Baudrate() );
}

/******************************************************************************
 * UpdateBox::~UpdateBox
 *****************************************************************************/
void UpdateBox::setBaudrate( const quint32 baudrate )
{
    d_data->m_application->setBaudrate( baudrate );
}

/******************************************************************************
 * UpdateBox::~UpdateBox
 *****************************************************************************/
QString UpdateBox::Portname() const
{
    return ( d_data->m_application->Portname() );
}

/******************************************************************************
 * UpdateBox::~UpdateBox
 *****************************************************************************/
void UpdateBox::setPortname( const QString& portname )
{
    d_data->m_application->setPortname( portname );
}

/******************************************************************************
 * UpdateBox::prepareMode
 *****************************************************************************/
void UpdateBox::prepareMode( const Mode )
{
    // do nothing here
}

/******************************************************************************
 * UpdateBox::loadSettings
 *****************************************************************************/
void UpdateBox::loadSettings( QSettings & )
{
    // do nothing here
}

/******************************************************************************
 * UpdateBox::saveSettings
 *****************************************************************************/
void UpdateBox::saveSettings( QSettings & )
{
    // do nothing here
}

/******************************************************************************
 * UpdateBox::applySettings
 *****************************************************************************/
void UpdateBox::applySettings( void )
{
    // do nothing here
}

/******************************************************************************
 * UpdateBox::mimeDataToUpdateFilePath
 *****************************************************************************/
QString UpdateBox::mimeDataToUpdateFilePath( const QMimeData * mimeData )
{
    if ( mimeData->hasUrls() &&  mimeData->urls().count() == 1 )
    {
        QString path = mimeData->urls().first().toLocalFile();
        if ( path.isEmpty() == false )
        {
            // loop over all update configs and try to find a matching binary file
            for ( int i = 0; i < d_data->m_upd_config.count(); i++ )
            {
                if ( QFileInfo( path ).isFile() &&
                     QFileInfo( path ).suffix() == d_data->m_upd_config[i].extension )
                {
                    return path;
                }
            }
        }
    }
    return QString();
}

/******************************************************************************
 * UpdateBox::getSystemState
 *****************************************************************************/
UpdateBox::SystemStates UpdateBox::getSystemState( void )
{
    // Aquire semaphore
    d_data->m_state_sema.acquire();

    // Get current state
    SystemStates state = d_data->m_state;

    // Release semaphore
    d_data->m_state_sema.release();

    // Return system state
    return state;
}

/******************************************************************************
 * UpdateBox::setSystemState
 *****************************************************************************/
void UpdateBox::setSystemState( SystemStates state, bool force )
{
    // Aquire semaphore
    d_data->m_state_sema.acquire();

    // Configure system state
    if ( (state != d_data->m_state || force) && (state > InvalidState) )
    {
        bool enable = true;

        // check if update index is in range, if it is, get file path
        QString fn = QString::null;
        if ( d_data->m_upd_idx >= 0 && d_data->m_upd_idx < d_data->m_upd_config.count() )
        {
            fn = d_data->m_upd_config[d_data->m_upd_idx].file;
        }

        if ( state == CommandState )
        {
            // command state (prepared to execute commands from console)
            d_data->m_ui->letSystemMode->setText( SYSTEM_STATE_COMMAND );
            d_data->m_ui->btnRun->setText( "Start" );
            d_data->m_ui->btnRun->setEnabled( (!fn.isEmpty()) && (fileExists(fn)) ? true : false );
            d_data->m_ui->cbxVerify->setEnabled( (!fn.isEmpty()) && (fileExists(fn)) ? true : false );
            d_data->m_ui->btnFilename->setEnabled( true );
            d_data->m_ui->progressBar->setFormat( "%p%" );
            d_data->m_ui->progressBar->setValue( 0 );
            d_data->m_ui->btnCheckFirmwareUpdate->setEnabled( true );
            d_data->m_ui->btnCheckGuiUpdate->setEnabled( true );
            enable = false;
        }
        else if ( state == UpdateState )
        {
            // update state or error retry state (waits for update data)
            d_data->m_ui->letSystemMode->setText( SYSTEM_STATE_UPDATE );
            d_data->m_ui->btnRun->setText( "Start" );
            // When coming from command state, disable the run button during update state
            if ( d_data->m_state == CommandState )
            {
                d_data->m_ui->btnRun->setEnabled( false );
            }
            // Else (e.g. when coming from error state) enable it, if a valid update path is present
            else
            {
                d_data->m_ui->btnRun->setEnabled( (!fn.isEmpty()) && (fileExists(fn)) ? true : false );
            }
            d_data->m_ui->cbxVerify->setEnabled( false );
            d_data->m_ui->btnFilename->setEnabled( false );
            d_data->m_ui->btnCheckFirmwareUpdate->setEnabled( false );
            d_data->m_ui->btnCheckGuiUpdate->setEnabled( false );
            enable = true;
        }
        else if ( state == FlashState )
        {
            // flash state, set run button text to abort, so that user can stop the update
            d_data->m_ui->letSystemMode->setText( SYSTEM_STATE_FLASHING );
            d_data->m_ui->btnRun->setEnabled( true );
            d_data->m_ui->btnRun->setText( "Abort" );
        }
        else if ( state == RebootState )
        {
            // reboot state, disable abort button / run button
            d_data->m_ui->letSystemMode->setText( SYSTEM_STATE_REBOOT);
            d_data->m_ui->btnRun->setEnabled( false );
        }
        else if ( state == ErrorState )
        {
            // error state, change run botton text to start, so that user can restart the update
            d_data->m_ui->btnRun->setEnabled( true );
            d_data->m_ui->btnRun->setText( "Start" );
        }

        // Show / hide flashloader tool information depending on enable flag
        d_data->m_ui->lblSystemName->setEnabled( enable );
        d_data->m_ui->letSystemName->setEnabled( enable );
        d_data->m_ui->lblSystemVersion->setEnabled( enable );
        d_data->m_ui->letSystemVersion->setEnabled( enable );
        d_data->m_ui->lblFlashLoaderVersion->setEnabled( enable );
        d_data->m_ui->letFlashLoaderVersion->setEnabled( enable );

        if ( !enable )
        {
            d_data->m_ui->letSystemName->clear();
            d_data->m_ui->letSystemVersion->clear();
            d_data->m_ui->letFlashLoaderVersion->clear();
        }

        // set the new system state
        d_data->m_state = state;
    }

    // Release semaphore
    d_data->m_state_sema.release();
}

/******************************************************************************
 * UpdateBox::getNumUpdates
 *****************************************************************************/
unsigned int UpdateBox::getTotalNumUpdates()
{
    int numUpdates = 0;

    /* Loop over all update configs, if a file path exits, this update has
     * to be performed */
    for ( int i = 0; i < d_data->m_upd_config.count(); i++ )
    {
        if ( d_data->m_upd_config[i].file != QString::null )
        {
            numUpdates++;
        }
    }

    return numUpdates;
}

/******************************************************************************
 * UpdateBox::setUpdateCounter
 *****************************************************************************/
void UpdateBox::setUpdateCounter( unsigned int updCnt )
{
    // copy value to class variable
    d_data->m_upd_cnt = updCnt;

    // set update counter label
    // If count is 0, clear the label
    if ( updCnt == 0 )
    {
        d_data->m_ui->lblUpdateCount->clear();
    }
    // Else show amount of updates processed
    else
    {
        QString labelText = QString("File %1 of %2").arg(updCnt).arg(getTotalNumUpdates());
        d_data->m_ui->lblUpdateCount->setText( labelText );
    }
}

/******************************************************************************
 * UpdateBox::incrementUpdateCounter
 *****************************************************************************/
void UpdateBox::incrementUpdateCounter()
{
    // increment counter
    d_data->m_upd_cnt++;

    // set update counter label
    QString labelText = QString("File %1 of %2").arg(d_data->m_upd_cnt).arg(getTotalNumUpdates());
    d_data->m_ui->lblUpdateCount->setText( labelText );
}

/******************************************************************************
 * UpdateBox::getFirstUpdateIndex
 *****************************************************************************/
void UpdateBox::getFirstUpdateIndex()
{
    // default -1 (no update availbale)
    int firstIndex = -1;

    // loop over all pending updates and check if a file was specified
    for ( int i = 0; i < d_data->m_upd_config.count(); i++ )
    {
        if (d_data->m_upd_config[i].file != QString::null )
        {
            firstIndex = i;
            break;
        }
    }

    d_data->m_upd_idx = firstIndex;

    // if index found, show file path and description in UI
    if ( firstIndex != -1 )
    {
        d_data->m_ui->letFilename->setText( d_data->m_upd_config[firstIndex].file );
        d_data->m_ui->letFiletype->setText( d_data->m_upd_config[firstIndex].description );
    }
    // else clear file path and description
    else
    {
        d_data->m_ui->letFilename->clear();
        d_data->m_ui->letFiletype->clear();
    }
}

/******************************************************************************
 * UpdateBox::getNextUpdateIndex
 *****************************************************************************/
void UpdateBox::getNextUpdateIndex()
{
    // default -1 (no next index availbale)
    int nextIndex = -1;

    // loop over all pending updates and check if a file was specified
    for ( int i = d_data->m_upd_idx + 1; i < d_data->m_upd_config.count(); i++ )
    {
        if (d_data->m_upd_config[i].file != QString::null )
        {
            nextIndex = i;
            break;
        }
    }

    d_data->m_upd_idx = nextIndex;

    // if index found, show file path and description in UI
    if ( nextIndex != -1 )
    {
        d_data->m_ui->letFilename->setText( d_data->m_upd_config[nextIndex].file );
        d_data->m_ui->letFiletype->setText( d_data->m_upd_config[nextIndex].description );
    }
    // else clear file path and description
    else
    {
        d_data->m_ui->letFilename->clear();
        d_data->m_ui->letFiletype->clear();
    }

}

/******************************************************************************
 * UpdateBox::isNewVersion
 *****************************************************************************/
bool UpdateBox::isNewVersion( version_t server_version, version_t current_version )
{
    // Check if major version is bigger
    if ( server_version.major_release > current_version.major_release )
    {
        return true;
    }
    // If major version is equal
    else if ( server_version.major_release == current_version.major_release )
    {
        // Check if minor verison is bigger
        if ( server_version.minor_release > current_version.minor_release )
        {
            return true;
        }
        // If minor version is equal
        else if ( server_version.minor_release == current_version.minor_release )
        {
            // Check if patch level is bigger
            if ( server_version.patch_level > current_version.patch_level )
            {
                return true;
            }
        }
    }

    return false;
}

/******************************************************************************
 * UpdateBox::onDownloadProgress
 *****************************************************************************/
void UpdateBox::onDownloadProgress( qint64 bytesReceived, qint64 bytesTotal )
{
    // Calculate progress
    int progress = 0;
    if ( bytesTotal > 0 )
    {
         progress = (int)( (bytesReceived * 100) / bytesTotal );
    }

    // Emit download progress signal
    emit DownloadProgress( progress );
}

/******************************************************************************
 * UpdateBox::onCheckFirmwareUpdateClicked
 *****************************************************************************/
bool UpdateBox::downloadFile( QUrl url )
{
    // Return value
    bool downloadSuccessful = false;

    // Create URL request for file download
    QNetworkRequest request( url );

    // Create a timer which will be used to timeout the dowload
    QTimer timer;

    // Create event loop which will quit as soon as the timer expires
    QEventLoop loop;
    connect( &timer, SIGNAL(timeout()), &loop, SLOT(quit()) );

    // Start the timer, download request and event loop
    /* Note: If no data is received within 10 seconds the connection is considered broken
     * and the download is aborted. */
    timer.start( 10000 );
    d_data->m_network_manager->get( request );
    QNetworkReply * reply = d_data->m_network_manager->get( request );

    // On download process, call onDownloadProgress() which will in turn emit DownloadProgress()
    connect( reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64,qint64)) );
    connect( this, SIGNAL(DownloadProgress(int)), &timer, SLOT(start()) );

    // If the download finishes stop the event loop
    connect( reply, SIGNAL(finished()), &loop, SLOT(quit()) );

    // If the timer expires, stop the download
    connect( &timer, SIGNAL(timeout()), reply, SLOT(abort()) );

    // Loop while download in progress
    loop.exec();

    // Check if download was successful
    if ( reply->error() )
    {
        // Show error message
        QMessageBox::information( this, QString("Download Failed"), QString("The download via HTTP failed. The following error was reported:\n\n%1").arg(reply->errorString()) );
    }
    else
    {
        // Copy downloaded data
        d_data->m_downloaded_data = reply->readAll();

        // Flag download as successful
        downloadSuccessful = true;
    }

    // Delete the reply later
    reply->deleteLater();

    // Return error state
    return downloadSuccessful;
}

/******************************************************************************
 * UpdateBox::onCheckFirmwareUpdateClicked
 *****************************************************************************/
void UpdateBox::onCheckFirmwareUpdateClicked()
{
    // Show a info dialog to indicate update server is contacted
    InfoDialog infoDlg( QString(":/images/tab/update.png"), QString("Update Dialog"), QString("Contacting Update Server..."), this->parentWidget() );
    infoDlg.show();

    // Create URL request for file download
    QUrl url = QString( QString(DOWNLOAD_SERVER) + d_data->m_system_platform + "/update_info.txt" );

    // Download the file
    bool downloadSuccessful = downloadFile(url);

    // Close info dialog
    infoDlg.close();

    // Check if a valid file was downloaded
    if( downloadSuccessful && d_data->m_downloaded_data.length() > 0 )
    {
        /* Get the version and check for new update */
        // Read server version from downloaded data
        version_t server_version;
        const char * data = d_data->m_downloaded_data.constData();

        data = strstr( data, VERSION_DOWNLOAD_SYNC );
        int res = 0;
        if ( data )
        {
            res = sscanf( data, VERSION_DOWNLOAD_PARSE,
                          &server_version.major_release,
                          &server_version.minor_release,
                          &server_version.patch_level );
        }
        if ( res != 3 )
        {
            d_data->m_ui->letServerFirmwareVersion->setText( "No valid Version found" );
            server_version.major_release = 0;
            server_version.minor_release = 0;
            server_version.patch_level = 0;

            QMessageBox::warning( this, QString("Update Check failed"), QString("No valid update file was found on the server. Update is not possible.") );
        }
        else
        {
            // Store server version
            d_data->m_server_fw_version = server_version;

            // Set version in GUI
            d_data->m_ui->letServerFirmwareVersion->setText( QString("v%1.%2.%3").arg(server_version.major_release)
                                                                                 .arg(server_version.minor_release)
                                                                                 .arg(server_version.patch_level) );

            /* Get the update file names */
            // Clear list
            d_data->m_download_files.clear();
            d_data->m_download_md5_sums.clear();

            // File 0
            data = strstr( data, FILE0_DOWNLOAD_SYNC );
            if ( data )
            {
                char fileName[128];
                char md5Sum[33];
                res = sscanf( data, FILE0_DOWNLOAD_PARSE, fileName, md5Sum );

                if ( res == 2 )
                {
                    d_data->m_download_files.append( QString(fileName) );
                    d_data->m_download_md5_sums.append( QString(md5Sum) );
                }
            }

            // File 1
            data = strstr( data, FILE1_DOWNLOAD_SYNC );
            if ( data )
            {
                char fileName[128];
                char md5Sum[33];
                res = sscanf( data, FILE1_DOWNLOAD_PARSE, fileName, md5Sum );

                if ( res == 2 )
                {
                    d_data->m_download_files.append( QString(fileName) );
                    d_data->m_download_md5_sums.append( QString(md5Sum) );
                }
            }

            /* Check if update is valid and ask user for download */
            if ( d_data->m_download_files.count() > 0)
            {
                // Check if the server version is newer than the current version
                QMessageBox::StandardButton reply;
                if ( isNewVersion(d_data->m_server_fw_version, d_data->m_current_fw_version) )
                {
                    reply = QMessageBox::question( this,
                                                   "New Firmware available",
                                                   "A new firmware update is available on the download server.\n\n"
                                                   "Do you want to download it now?\n\n"
                                                   "The update file(s) will be stored in a temporary download folder which is deleted after the upate is completed.",
                                                   QMessageBox::Yes|QMessageBox::No );
                }
                else
                {
                    reply = QMessageBox::question( this,
                                                   "No Update available",
                                                   "The latest firmware is already installed, an update is not required.\n\n"
                                                   "Do you want to to download and reinstall the latest update?\n\n"
                                                   "The update file(s) will be stored in a temporary download folder which is deleted after the upate is completed.",
                                                   QMessageBox::Yes|QMessageBox::No );
                }

                if (reply == QMessageBox::Yes)
                {
                    // Call download function
                    downloadUpdate();
                }
            }
            else
            {
                QMessageBox::warning( this, QString("Update Check failed"), QString("No valid update file was found on the server. Update is not possible.") );
            }
        }
    }
    else
    {
        QMessageBox::warning( this, QString("Update Check failed"), QString("No valid update file was found on the server. Update is not possible.") );
    }

    // If update is invalid, cleanup
    if ( d_data->m_downloaded_data.length() == 0 ||
         d_data->m_download_files.count() == 0 )
    {
        // Reset server version
        d_data->m_ui->letServerFirmwareVersion->setText( "No valid Version found" );
        d_data->m_server_fw_version.major_release = 0;
        d_data->m_server_fw_version.minor_release = 0;
        d_data->m_server_fw_version.patch_level = 0;
    }
}

/******************************************************************************
 * UpdateBox::downloadUpdate
 *****************************************************************************/
void UpdateBox::downloadUpdate()
{
    // Create temporary download directory
    if ( d_data->m_download_dir )
    {
        delete d_data->m_download_dir;
    }
    d_data->m_download_dir = new QTemporaryDir();

    // If temporary directory can not be created, abort
    if ( !d_data->m_download_dir->isValid() )
    {
        QMessageBox::warning( this,
                              "Can not create Download Directory",
                              "The temporary download directory can not be created, a download is not possible." );
        return;
    }

    // Show a info dialog to indicate download is ongoing
    InfoDialog infoDlg( QString(":/icons/save2.png"), QString("Update Dialog"), QString("Downloading Update..."), this->parentWidget() );
    infoDlg.show();

    // Download update files to temporary directory
    bool downloadError = false;
    bool checksumError = false;
    for ( int i = 0; i < d_data->m_download_files.count(); i++ )
    {
        // Download file, break if not succesful
        QUrl url = QString( QString(DOWNLOAD_SERVER) + d_data->m_system_platform + "/" + d_data->m_download_files.at(i) );
        if ( !downloadFile( url ) )
        {
            downloadError = true;
            break;
        }

        // Check if a valid file was downloaded by evaluating its md5 checksum
        if( d_data->m_downloaded_data.length() > 0 )
        {
            QCryptographicHash hash( QCryptographicHash::Md5 );
            hash.addData( d_data->m_downloaded_data );

            // If checksum matches
            if ( QString::compare(QString(hash.result().toHex()), d_data->m_download_md5_sums.at(i)) == 0 )
            {
                // Save byte array to file
                QFile file( d_data->m_download_dir->path() + "/" + d_data->m_download_files.at(i) );
                file.open( QIODevice::WriteOnly );
                file.write( d_data->m_downloaded_data );
                file.close();
            }
            else
            {
                checksumError = true;
                break;
            }
        }
    }

    // close info dialog
    infoDlg.close();

    // If the files were downloaded correctly
    bool updateValid = false;
    if ( !checksumError && !downloadError )
    {
        // check update files in temp directory
        /* Note: The  checkUpdateDirectory() function will print an error message if the directory does
         * not contain valid files, so we do not need to print an error message here */
        checkUpdateDirectory( d_data->m_download_dir->path() );

        // If the update button is now enabled, the update is valid. Ask user if he wants to update now
        if ( d_data->m_ui->btnRun->isEnabled() )
        {
            updateValid = true;
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question( this,
                                           "Update is Ready",
                                           "The firmware update was downloaded successully.\n\nDo you want to run the update now?",
                                           QMessageBox::Yes|QMessageBox::No );

            // User choose to abort, so return
            if (reply == QMessageBox::Yes)
            {
                // execute clicked function of the run button
                onRunClicked();
            }
        }
    }
    // If a checksum error occured, print a message
    else if ( checksumError )
    {
        QMessageBox::information( this,
                                  "Checksum Error",
                                  "The checksum of the download does not match the reference, which means the file is corrupt.\n\n"
                                  "Please retry the update check to download the files again." );
    }

    // If an error occured, cleanup
    if ( checksumError || !updateValid )
    {
        delete d_data->m_download_dir;
        d_data->m_download_dir = NULL;

        // Reset server version
        d_data->m_ui->letServerFirmwareVersion->setText( "No valid Version found" );
        d_data->m_server_fw_version.major_release = 0;
        d_data->m_server_fw_version.minor_release = 0;
        d_data->m_server_fw_version.patch_level = 0;

        // reset update counter and index
        setUpdateCounter( 0 );
        d_data->m_upd_idx = -1;
        getNextUpdateIndex();   // this will not find a index, thus clear the file path line edit
    }
}

/******************************************************************************
 * UpdateBox::onCheckGuiUpdateClicked
 *****************************************************************************/
void UpdateBox::onCheckGuiUpdateClicked()
{
    // Show a info dialog to indicate update server is contacted
    InfoDialog infoDlg( QString(":/images/tab/update.png"), QString("Update Dialog"), QString("Contacting Update Server..."), this->parentWidget() );
    infoDlg.show();

    // Create URL request for file download
    QUrl url = QString( QString(DOWNLOAD_SERVER) + "provideo_gui/update_info.txt" );

    // Download the file
    bool downloadSuccessful = downloadFile( url );

    // Close info dialog
    infoDlg.close();

    // Check if a valid file was downloaded
    if( downloadSuccessful && d_data->m_downloaded_data.length() > 0 )
    {
        /* Get the version and check for new update */
        // Read server version from downloaded data
        version_t server_version;
        const char * data = d_data->m_downloaded_data.constData();

        data = strstr( data, VERSION_DOWNLOAD_SYNC );
        int res = 0;
        if ( data )
        {
            res = sscanf( data, VERSION_DOWNLOAD_PARSE,
                          &server_version.major_release,
                          &server_version.minor_release,
                          &server_version.patch_level );
        }
        if ( res != 3 )
        {
            d_data->m_ui->letServerGuiVersion->setText( "No valid Version found" );
            server_version.major_release = 0;
            server_version.minor_release = 0;
            server_version.patch_level = 0;

            QMessageBox::warning( this, QString("Update Check failed"), QString("No valid version file was found on the server. Update check is not possible.") );
        }
        else
        {
            // Store server version
            d_data->m_server_gui_version = server_version;

            // Set version in GUI
            d_data->m_ui->letServerGuiVersion->setText( QString("v%1.%2.%3").arg(server_version.major_release)
                                                                            .arg(server_version.minor_release)
                                                                            .arg(server_version.patch_level) );

            // Check if the server version is newer than the current version
            if ( isNewVersion(d_data->m_server_gui_version, d_data->m_current_gui_version) )
            {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question( this,
                                               "New GUI available",
                                               "A new GUI version is available on the download server.\n\n"
                                               "Would you like to visit the download page now?",
                                               QMessageBox::Yes|QMessageBox::No );

                if (reply == QMessageBox::Yes)
                {
                    if ( !QDesktopServices::openUrl(QUrl(GUI_DOWNLOAD_PAGE)) )
                    {
                        QMessageBox::information( this,
                                                  "Can not open Browser",
                                                  "The web browser can not be opened.\n\nPlease visit " + QString(GUI_DOWNLOAD_PAGE) + "to download the latest GUI version." );
                    }
                }
            }
            else
            {
                QMessageBox::information( this,
                                          "No Update avialable",
                                          "Your GUI version is already up-to-date, there is no newer version available." );
            }
        }
    }
    else
    {
        d_data->m_ui->letServerGuiVersion->setText( "No valid Version found" );
        QMessageBox::warning( this, QString("Update Check failed"), QString("No valid version file was found on the server. Update check is not possible.") );
    }
}

/******************************************************************************
 * UpdateBox::onFsmTimer
 *****************************************************************************/
void UpdateBox::onFsmTimer( )
{
    int res = 0;
    int updateIndex = d_data->m_upd_idx;

    if ( updateIndex < 0 )
    {
        return;
    }

    SystemStates state = getSystemState();

    // I. CommandState: not connected with flashloader
    if ( state == CommandState )
    {
        // set baudrate and reversal
        d_data->m_application->setBaudrate( d_data->m_upd_config[updateIndex].baudrate );
        d_data->m_application->setReverse( d_data->m_upd_config[updateIndex].reversal );

        // run asynchronous command
        res = d_data->m_application->runCommand(
                FlashLoader::FLASHLOAD_CMD_SYSTEM_INFO );

        // restart reconnect timer
        d_data->m_FsmTimer->start( 2000 );
    }

    // II. UpdateState: initiate update run
    else if ( state == UpdateState )
    {
        // set baudrate and reversal
        d_data->m_application->setBaudrate( d_data->m_upd_config[updateIndex].baudrate );
        d_data->m_application->setReverse( d_data->m_upd_config[updateIndex].reversal );

        d_data->m_erase_cnt     = 0u;
        d_data->m_program_cnt   = 0u;
        d_data->m_verify_cnt    = 0u;

        // start flashing
        res = d_data->m_application->runCommand(
                FlashLoader::FLASHLOAD_CMD_PROGRAM, d_data->m_upd_config[updateIndex].sector, 0, d_data->m_upd_config[updateIndex].file );
    }

    // III. ErrorStateRetry: An error occured while updating
    else if ( state == ErrorState )
    {
        // switch to update state, so the device stays in fw update mode and the user can try again
        setSystemState( UpdateState );
    }

    // IV. update running or error state
    else
    {
    }

    // Handle errors
    if ( res == -EBUSY )
    {
        QMessageBox::warning( this,
                              "Updated Failed",
                              "An invalid command was sent to the Flashloader Tool.\n\n"
                              "This error should not occur, please send a bug report to the developers." );
        setSystemState( ErrorState );

        // set normal cursor
        setNormalCursor();
    }
    else if ( res == -ENOENT )
    {
        QMessageBox::warning( this,
                              "Flashloader Tool not found",
                              "The Flashloader Tool could not be found. Please make sure that the file 'flashloader.exe' is "
                              "placed in a folder named 'tools' which is placed in the same folder that 'ProVideo.exe' is placed.\n\n"
                              "Please restart the Update after copying the Flashloader." );
        setSystemState( ErrorState );

        // set normal cursor
        setNormalCursor();
    }
}

/******************************************************************************
 * UpdateBox::checkUpdateDirectory
 *****************************************************************************/
void UpdateBox::checkUpdateDirectory( QString updateDirectory  )
{
    // reset data structures
    for ( int i = 0; i < d_data->m_upd_config.count(); i++ )
    {
        d_data->m_upd_config[i].file = QString::null;
    }

    // Error flags
    bool noFileFoundError = true;
    bool multipleMatchError = false;

    // check if user selected a directory
    if ( !updateDirectory.isEmpty() )
    {
        // store new update directory path
        d_data->m_upd_dir = updateDirectory;

        // loop over all update configs and try to find a matching binary file
        for ( int i = 0; i < d_data->m_upd_config.count(); i++ )
        {
            // loop over all files in the directory and search for a matching binary file
            QDirIterator binaryFiles( updateDirectory );
            while ( binaryFiles.hasNext() )
            {
                binaryFiles.next();

                if ( binaryFiles.fileInfo().isFile() &&
                     binaryFiles.fileInfo().suffix() == d_data->m_upd_config[i].extension )
                {
                    // if content has to be checked
                    if ( d_data->m_upd_config[i].content )
                    {
                        // check file type matches, if it does not, continue with next file
                        if ( !fileTypeMatches(d_data->m_upd_config[i].type, binaryFiles.filePath()) )
                        {
                            continue;
                        }
                    }

                    // If the file path is still empty, copy the file path and increment update counter
                    if ( d_data->m_upd_config[i].file == QString::null )
                    {
                        d_data->m_upd_config[i].file = binaryFiles.filePath();
                        noFileFoundError = false;
                    }
                    // Abort, because more than one matching file was found
                    else
                    {
                        multipleMatchError = true;
                        break;
                    }
                }

                // Stop searching for files if an error occured
                if ( multipleMatchError )
                {
                    break;
                }
            }
        }

        // Handle error multiple matches found
        if ( multipleMatchError )
        {
            // show message for download failed
            if ( d_data->m_download_dir && d_data->m_download_dir->path() == updateDirectory )
            {
                QMessageBox::information( this,
                                          "Download invalid",
                                          "The downloaded update is invalid. Update is not possible." );
            }
            // show message for error with selected folder
            else
            {
                QMessageBox::information( this,
                                          "Two many files found",
                                          "In the given update folder more than one matching update file was found. Please delete duplicate files or specify a different folder." );
            }
        }

        // Handle no file found error
        else if ( noFileFoundError )
        {
            // show message for download failed
            if ( d_data->m_download_dir && d_data->m_download_dir->path() == updateDirectory )
            {
                QMessageBox::information( this,
                                          "Download invalid",
                                          "The downloaded update is invalid. Update is not possible." );
            }
            // show message for error with selected folder
            else
            {
                QMessageBox::information( this,
                                          "No file found",
                                          "The given folder does not contain valid update files, please select a different update folder." );
            }
        }
    }

    // User cancelled directory selection or an error occurd
    if ( noFileFoundError || multipleMatchError )
    {
        // reset data structures
        for ( int i = 0; i < d_data->m_upd_config.count(); i++ )
        {
            d_data->m_upd_config[i].file = QString::null;
        }

        // and disable UI elements
        d_data->m_ui->btnRun->setEnabled( false );
        d_data->m_ui->cbxVerify->setEnabled( false );
    }
    // If no error occured, enable ui elements to start update
    else
    {
        // Set update counter and index
        /*  counter will be incremented in the onUpdateFinished() slot,
         * which will then restart the timer, if more updates are pending */
        setUpdateCounter( 1 );
        d_data->m_ui->btnRun->setEnabled( true );
        d_data->m_ui->cbxVerify->setEnabled( true );
    }

    // Call getFirstUpdateIndex() to setup line edits with the update path (or clear them if no update found)
    getFirstUpdateIndex();
}

/******************************************************************************
 * UpdateBox::onFileNameClicked
 *****************************************************************************/
void UpdateBox::onFileNameClicked()
{
    if ( !d_data->m_upd_config.empty() )
    {
        QString updateDirectory = d_data->m_upd_dir;
        QString directory;

        // get update directory
        if ( updateDirectory != QString::null && QDir(d_data->m_upd_dir).exists() )
        {
            directory = QDir(updateDirectory).absolutePath();
        }
        else
        {
            directory = QDir::currentPath();
        }

        // NOTE: It can fail on gtk-systems when an empty filename is given
        //       in the native dialog-box, because GTK sends a SIGSEGV-signal
        //       to process and this is not handled by Qt.
        updateDirectory = QFileDialog::getExistingDirectory(
            this, tr("Choose Update Directory"), directory, QFileDialog::ShowDirsOnly
        );

        checkUpdateDirectory( updateDirectory );
    }
}

/******************************************************************************
 * UpdateBox::onRunClicked
 *****************************************************************************/
void UpdateBox::onRunClicked()
{
    // Check if updates available
    if ( getTotalNumUpdates() > 0 )
    {
        SystemStates state = getSystemState();

        // CommandState, UpdateState, ErrorState -> Start update
        if ( (state == CommandState) ||
             (state == UpdateState)  ||
             (state == ErrorState) )
        {
            int dT = 1; // 1 ms = 1 tick

            // I. set wait cursor and lock GUI to current tab page, disable run button
            setWaitCursor();
            emit LockCurrentTabPage( true );

            // II. boot into update state if needed
            if ( state == CommandState )
            {
                // restart camera in update (bootloader) mode
                emit BootIntoUpdateMode();
                QApplication::processEvents();

                // Close serial connection to device, otherwise the flashloader can not communicate with it
                emit CloseSerialConnection();

                // reboot wait delay 5s
                dT = 5000;
            }

            // III. reset progress bar
            d_data->m_ui->progressBar->setFormat( "%p%" );
            d_data->m_ui->progressBar->setValue( 0 );

            // IV: if in error state, switch to update state
            setSystemState( UpdateState );

            // V. start FSM timer
            d_data->m_FsmTimer->start( dT );
        }

        // FlashState -> Cancel update
        else
        {
            // Change cursor to normal
            setNormalCursor();

            // Ask user if he really wants to cancel the update
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question( this,
                                           "Cancel Update?",
                                           "If the update is aborted, the camera will stay in update mode. "
                                           "You can than choose to flash a different update, but you can not restart the"
                                           "camera until it has been succesfuly updated!\n\n"
                                           "Do you really want to cancel the update?",
                                           QMessageBox::Yes|QMessageBox::No );

            // User choose yes
            if ( reply == QMessageBox::Yes)
            {
                /* Check if still in flash state, otherwise update was completed
                 * while user was reading the message and we do not need to cancel */
                if ( getSystemState() == FlashState )
                {
                    // I. send stop command
                    int res = d_data->m_application->stopCommand();
                    HANDLE_ERROR( res );

                    // II. update system state
                    setSystemState( UpdateState );

                    // III. reset progress bar and update counter / index
                    d_data->m_ui->progressBar->setFormat( "%p%" );
                    d_data->m_ui->progressBar->setValue( 0 );

                    setUpdateCounter( 1 );
                    getFirstUpdateIndex();
                }
            }
            // User choose no
            else
            {
              // Set cursor to busy again
              setWaitCursor();
            }
        }
    }
}

/******************************************************************************
 * UpdateBox::onVerifyChanged
 *****************************************************************************/
void UpdateBox::onVerifyChanged( int state )
{
    d_data->m_application->setVerify( (state == Qt::Checked) ? true : false );
}

/******************************************************************************
 * UpdateBox::onPromptChange
 *****************************************************************************/
void UpdateBox::onPromptChange( uint8_t )
{
    // successfull prompt command
    // => command connection is established
    setSystemState( CommandState );
}

/******************************************************************************
 * UpdateBox::onSystemPlatformChange
 *****************************************************************************/
void UpdateBox::onSystemPlatformChange( QString name )
{
    // If the platform has changed
    if ( name != d_data->m_system_platform )
    {
        // Reset server version numbers for the firmware, as it is not correct for new platform
        d_data->m_ui->letServerFirmwareVersion->setText( "Not checked yet" );

        // Clear list of update configs
        d_data->m_upd_config.clear();

        if ( name == PLATFORM_XBOW )
        {
            d_data->m_upd_config.append( xbow_update );
        }

        else if ( name == PLATFORM_CONDOR_4K || name.contains(QString(PLATFORM_CONDOR_4K_MINI)) )
        {
            // first file is firmware update
            d_data->m_upd_config.append( condor4k_fw_update );

            // second file is bitstream update
            d_data->m_upd_config.append( condor4k_bs_update );
        }

        else if ( name == PLATFORM_COOPER )
        {
            // first file is firmware update
            d_data->m_upd_config.append( cooper_fw_update );

            // second file is bitstream update
            d_data->m_upd_config.append( cooper_bs_update );
        }
    }

    // Save platform name
    d_data->m_system_platform = name;

    // Reset update path by calling getFirstUpdateIndex()
    getFirstUpdateIndex();

    // Make sure we are in command state
    setSystemState( CommandState, true );
}

/******************************************************************************
 * UpdateBox::onApplicationVersionChange
 *****************************************************************************/
void UpdateBox::onApplicationVersionChange( QString version )
{
    version_t currentVersion;
    int res = sscanf( version.toStdString().c_str(), "v%d.%d.%d",
                      &currentVersion.major_release,
                      &currentVersion.minor_release,
                      &currentVersion.patch_level );

    if ( res != 3 )
    {
        d_data->m_ui->letCurrentFirmwareVersion->setText( "Unrecognized Version (" + version + ")"  );
        currentVersion.major_release = 0;
        currentVersion.minor_release = 0;
        currentVersion.patch_level = 0;
    }
    else
    {
        d_data->m_ui->letCurrentFirmwareVersion->setText( version );
    }

    d_data->m_current_fw_version = currentVersion;
}

/******************************************************************************
 * UpdateBox::onFlashLoaderVersion
 *****************************************************************************/
void UpdateBox::onFlashLoaderVersion( quint32 m, quint32 n )
{
    QString s;
    s.sprintf( "%d.%d", m, n );
    d_data->m_ui->letFlashLoaderVersion->setText( s );
}

/******************************************************************************
 * UpdateBox::onSystemId
 *****************************************************************************/
void UpdateBox::onSystemId( qint32 id )
{
    // is system detected
    if ( ( id > SYSTEM_ID_INVALID) && ( id < SYSTEM_ID_MAX ) )
    {
        // go to update state
        setSystemState( UpdateState );
        d_data->m_id = id;
    }
    else
    {
        // call onFlashLoaderError function with version mismatch error
        onFlashLoaderError( -EPROTO );
    }
}

/******************************************************************************
 * UpdateBox::onSystemName
 *****************************************************************************/
void UpdateBox::onSystemName( QString name )
{
    d_data->m_ui->letSystemName->setText( name );
}

/******************************************************************************
 * UpdateBox::onSystemVersion
 *****************************************************************************/
void UpdateBox::onSystemVersion( quint32 m, quint32 n, quint32 o )
{
    QString s;
    s.sprintf( "%d.%d.%d", m, n, o );
    d_data->m_ui->letSystemVersion->setText( s );
}

/******************************************************************************
 * UpdateBox::onFlashBlockNo
 *****************************************************************************/
void UpdateBox::onFlashBlockNo( quint32 n )
{
    d_data->m_no_sec = n;
}

/******************************************************************************
 * UpdateBox::onFlashBlockSize
 *****************************************************************************/
void UpdateBox::onFlashBlockSize( quint32 sz )
{
    d_data->m_size_sec = sz;
}

/******************************************************************************
 * UpdateBox::onFirstFlashBlock
 *****************************************************************************/
void UpdateBox::onFirstFlashBlock( quint32 id )
{
    d_data->m_first_sec = id;
}

/******************************************************************************
 * UpdateBox::onReconfCond
 *****************************************************************************/
void UpdateBox::onReconfCond( quint32 mask )
{
    d_data->m_reconf_cond = mask;
}

/******************************************************************************
 * UpdateBox::onFlashLoaderError
 *****************************************************************************/
void UpdateBox::onFlashLoaderError( int error )
{
    // Set error state
    setSystemState( ErrorState );

    // Build error message
    QMessageBox errorMessage( this );
    errorMessage.setIcon( QMessageBox::Warning );
    errorMessage.setWindowTitle( "An Error Occured");

    switch ( error )
    {
        case -ETIMEDOUT:
            errorMessage.setText( "Communication with device timed out. Please retry." );
            break;
        case -ENODEV:
            errorMessage.setText( "No device found, try power-cycling the device and restarting the GUI." );
            break;
        case -EIO:
            errorMessage.setText( "Can not communicate with device, please retry." );
            break;
        case -EPROTO:
            errorMessage.setText( "Version mismatch, device can not be flashed." );
            break;
        default:
            errorMessage.setText( "An unknown error occured during flashing, please retry." );
            break;
    }

    // set normal cursor
    setNormalCursor();

    // Show error message
    errorMessage.exec();

    // Return to Update state, so that user can retry
    // reset progress bar and update counter / index
    d_data->m_ui->progressBar->setFormat( "%p%" );
    d_data->m_ui->progressBar->setValue( 0 );

    setUpdateCounter( 1 );
    getFirstUpdateIndex();
}

/******************************************************************************
 * UpdateBox::onReadbackProgress
 *****************************************************************************/
void UpdateBox::onReadbackProgress( quint32 progress )
{
    d_data->m_ui->progressBar->setFormat( "Read %p%" );
    d_data->m_ui->progressBar->setValue( progress );
}

/******************************************************************************
 * UpdateBox::onEraseProgress
 *****************************************************************************/
void UpdateBox::onEraseProgress( quint32 progress )
{
    d_data->m_ui->progressBar->setFormat( "Erase %p%" );
    d_data->m_ui->progressBar->setValue( progress );
    d_data->m_erase_cnt = progress;
    setSystemState( FlashState );
}

/******************************************************************************
 * UpdateBox::onProgramProgress
 *****************************************************************************/
void UpdateBox::onProgramProgress( quint32 progress )
{
    d_data->m_ui->progressBar->setFormat( "Program %p%" );
    d_data->m_ui->progressBar->setValue( progress );
    d_data->m_program_cnt = progress;
    setSystemState( FlashState );
}

/******************************************************************************
 * UpdateBox::onVerifyProgress
 *****************************************************************************/
void UpdateBox::onVerifyProgress( quint32 progress )
{
    d_data->m_ui->progressBar->setFormat( "Verify %p%" );
    d_data->m_ui->progressBar->setValue( progress );
    d_data->m_verify_cnt = progress;
    setSystemState( FlashState );
}

/******************************************************************************
 * UpdateBox::onUpdateFinished
 *****************************************************************************/
void UpdateBox::onUpdateFinished()
{
    if ( FlashState == getSystemState() )
    {
        if ( ( d_data->m_erase_cnt   == 100 ) &&
             ( d_data->m_program_cnt == 100 ) &&
             ( (d_data->m_verify_cnt  == 100) || (d_data->m_verify_cnt  == 0) ) )
        {
            // check if this was the last update
            if ( d_data->m_upd_cnt == getTotalNumUpdates() )
            {
                // reset update counter and index
                setUpdateCounter( 1 );
                getFirstUpdateIndex();

                // Switch to reboot state
                setSystemState( RebootState );

                // send restart command to camera
                int res = d_data->m_application->runCommand(
                            FlashLoader::FLASHLOAD_CMD_REBOOT );
                HANDLE_ERROR( res );

                // Wait 1s for command to be transmitted
                QThread::sleep( 1 );

                // process events, otherwise the last event of the flashloader is not processed
                QApplication::processEvents( QEventLoop::WaitForMoreEvents );

                // unlock GUI
                emit LockCurrentTabPage( false );

                /* reconnect with camera, the connect dialog will either reconnect, or
                 * show the dialog if no automatic reconnect is possible */
                emit ReopenSerialConnection();
            }
            // if more updates are pending, continue with the next update
            else
            {
                // increment update counter and index
                incrementUpdateCounter();
                getNextUpdateIndex();

                // update process for this update sucessfully finished, switch back to command state
                setSystemState( CommandState );

                // restart FSM timer
                d_data->m_FsmTimer->start( 1 );
            }
        }
    }
}
