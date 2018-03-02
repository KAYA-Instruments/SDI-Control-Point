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
 * @file    dpccbox.cpp
 *
 * @brief   Implementation of dpcc setting box
 *
 *****************************************************************************/
#include <QtDebug>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QFileInfo>
#include <QLineEdit>
#include <QItemDelegate>
#include <QDir>
#include <QFileDialog>
#include <QToolTip>
#include <QMessageBox>

#include <csvwrapper/csvwrapper.h>

#include "defines.h"
#include "dpccbox.h"
#include "ui_dpccbox.h"

#include <ctrl_protocol/ctrl_protocol_dpcc.h>

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_DpccBox;
}

/******************************************************************************
 * Settings
 *****************************************************************************/
#define DPCC_SETTINGS_SECTION_NAME          ( "DPCC" )
#define DPCC_SETTINGS_ENABLE                ( "enable" )
#define DPCC_SETTINGS_MODE                  ( "mode" )
#define DPCC_SETTINGS_LEVEL                 ( "level" )
#define DPCC_SETTINGS_TEST_MODE             ( "test_mode" )
#define DPCC_SETTINGS_STORAGE               ( "storage" )
#define DPCC_SETTINGS_Y_POSITIONS           ( "yPositions" )
#define DPCC_SETTINGS_X_POSITIONS           ( "xPositions" )
#define DPCC_TABLE_MAX_NO_ROWS              ( (int)MAX_DPCC_NO_PIXEL )    // See <ctrl_protocol/ctrl_protocol_dpcc.h>
#define DPCC_TABLE_MAX_NO_COLUMNS           ( 2 )

/******************************************************************************
 * fileExists
 *****************************************************************************/
static bool fileExists( QString & path )
{
    QFileInfo check_file( path );

    // check if file exists and if yes: Is it really a file and no directory?
    return ( check_file.exists() && check_file.isFile() );
}

/******************************************************************************
 * pairSort
 * @brief Sorts a List of pairs given by a and b components first by a and then
 *        by b value. Duplicates are removed and the sorted / cleaned vectors
 *        are returned. Both vectors must have the same lenght!
 * @param a Vector of the first component of the pairs
 * @param b Vector of the second component of the paris
 * @returns False if an error occured (e.g. length missmatch), true otherwise
 *****************************************************************************/
static bool pairSort( QVector<int> &a, QVector<int> &b)
{
    if ( a.length() != b.length())
    {
        return false;
    }

    QList<QPair<int,int>> pointList;
    for ( int  i= 0; i < a.length(); i++ )
    {
        // Do not add duplicates to the List
        QPair<int,int> newPair(a.at(i), b.at(i));
        if (!pointList.contains(newPair))
        {
            pointList.append(newPair);
        }
    }

    // Sort it (will sort first for x, than for y, see QPair
    qSort(pointList.begin(), pointList.end());

    // Write the results back into the table
    a.clear();
    b.clear();
    for ( int i = 0; i < pointList.length(); i++)
    {
        a.append(pointList.at(i).first);
        b.append(pointList.at(i).second);
    }

    return true;
}

/******************************************************************************
 * Delegate
 *****************************************************************************/
class DpccDelegate : public QItemDelegate
{
public:
    // create a single editable table-cell
    QWidget* createEditor( QWidget * parent, const QStyleOptionViewItem &, const QModelIndex & index) const
    {
        QLineEdit * edit = new QLineEdit( parent );

        // set validator
        int upperBound;
        if (index.column() == 0)    // Set the upper bound of the first column
        {
            upperBound = m_firstColBound;
        }
        else                        // Set the upper bound of the second (and all other) columns
        {
            upperBound = m_secondColBound;
        }
        QIntValidator *valid = new QIntValidator( 0, upperBound, edit );
        edit->setValidator( valid );

        return ( edit );
    }

    // transfer value from data-model into line-edit
    void setEditorData( QWidget * editor, const QModelIndex & idx ) const
    {
        int value = idx.model()->data( idx, Qt::EditRole ).toInt();
        QLineEdit * edt = static_cast< QLineEdit * >( editor );
        edt->setText( QString().setNum(value) );
    }    

    // transfer value from line_edit into data-model
    void setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex &idx ) const
    {
        QLineEdit * edt = static_cast< QLineEdit * >( editor );
        QString value = edt->text();
        model->setData( idx, value, Qt::EditRole );
    }

    // set geometry of line-edit
    void updateEditorGeometry( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & ) const
    {
        editor->setGeometry( option.rect );
    }

    // Set the upper bound for first end second column
    void setBounds(int firstColBound, int secondColBound)
    {
        m_firstColBound = firstColBound;
        m_secondColBound = secondColBound;
    }

    // Get the upper bound of the first column
    int getFirstColBound()
    {
        return m_firstColBound;
    }

    // Get the upper bound of the second column
    int getSecondColBound()
    {
        return m_secondColBound;
    }

private:
    int m_firstColBound;
    int m_secondColBound;
};

/******************************************************************************
 * DpccBox::PrivateData
 *****************************************************************************/
class DpccBox::PrivateData
{
public:
    PrivateData( QWidget * parent )
        : m_ui( new Ui::UI_DpccBox )
        , m_delegate( new DpccDelegate )
    {
        // initialize UI
        m_ui->setupUi( parent );

        initDataModel();

        m_ui->tblPositions->setItemDelegate( m_delegate );
        m_ui->tblPositions->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
        m_ui->tblPositions->setSelectionBehavior( QAbstractItemView::SelectRows );
        setDataModel();
    }

    ~PrivateData()
    {
        delete m_ui;
        delete m_delegate;
        delete m_model;
    };
    
    // initialize data model
    void initDataModel()
    {
        m_model = new QStandardItemModel( 0, DPCC_TABLE_MAX_NO_COLUMNS, NULL );
        m_model->setHorizontalHeaderItem( 0, new QStandardItem(QString("Row (Y)")) );
        m_model->setHorizontalHeaderItem( 1, new QStandardItem(QString("Col (X)")) );
    }
    
    // replace data model of the table view widget with our custom model
    void setDataModel()
    {
        // see http://doc.qt.io/qt-5.7/qabstractitemview.html#setModel
        // get pointer to old selection model
        QItemSelectionModel *m = m_ui->tblPositions->selectionModel();
        // set new data model
        m_ui->tblPositions->setModel( m_model );
        // delete old selection model
        delete m;
    }

    // set positions in tableview widget
    void fillTable( QVector<int> &yPos, QVector<int> &xPos )
    {
        Q_ASSERT( yPos.count() == xPos.count() );
        Q_ASSERT( m_model != NULL );
        
        // update data model (add or remove rows)
        int numRowsModel = m_model->rowCount();
        int numRowsInput = yPos.count();
        if ( numRowsModel < numRowsInput )
        {
            m_model->insertRows( 0, numRowsInput - numRowsModel );
        }
        else
        {
            m_model->removeRows( 0, numRowsModel - numRowsInput );
        }

        // set values
        for ( int i=0; i < yPos.count(); ++i )
        {
            QModelIndex yPos_i = m_model->index( i, 0, QModelIndex() );
            QModelIndex xPos_i = m_model->index( i, 1, QModelIndex() );

            // set used or clear unsed sample values
            m_model->setData( yPos_i, yPos[i] );
            m_model->setData( xPos_i, xPos[i] );

            // set alignment
            m_model->setData( yPos_i, QVariant(Qt::AlignVCenter | Qt::AlignRight), Qt::TextAlignmentRole );
            m_model->setData( xPos_i, QVariant(Qt::AlignVCenter | Qt::AlignRight), Qt::TextAlignmentRole );
        }
    }

    void getDataFromModel( QVector<int> &yPos, QVector<int> &xPos  )
    {
        yPos.clear();
        xPos.clear();

        for ( int i=0; i<m_model->rowCount(); i ++ )
        {
            yPos.append( m_model->data( m_model->index(i, 0), Qt::DisplayRole ).toInt() );
            xPos.append( m_model->data( m_model->index(i, 1), Qt::DisplayRole ).toInt() );
        }
    }

    Ui::UI_DpccBox *        m_ui;                           /**< ui handle */
    DpccDelegate *          m_delegate;                     /**< delegation class */
    QStandardItemModel *    m_model;                        /**< data model */
    QString                 m_filename;
};

/******************************************************************************
 * DpccBox::DpccBox
 *****************************************************************************/
DpccBox::DpccBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData( this );

    // connect internal signals
    // Camera Interaction
    connect ( d_data->m_ui->btnTransmitTable, SIGNAL(clicked()), this, SLOT(onTransmittTableClicked()) );
    connect ( d_data->m_ui->btnLoadTable, SIGNAL(clicked()), this, SLOT(onLoadTableClicked()) );
    connect ( d_data->m_ui->btnAutoLoadTable, SIGNAL(clicked()), this, SLOT(onAutoLoadTableClicked()) );

    // Settings
    connect ( d_data->m_ui->cbxEnable, SIGNAL(stateChanged(int)), this, SLOT(onCbxEnableChanged( int )) );
    connect ( d_data->m_ui->cbxCorrectionMode, SIGNAL(currentIndexChanged( int )), this, SLOT(onCbxCorrectionModeChanged( int )) );
    connect ( d_data->m_ui->sbxLevel, SIGNAL(valueChanged( int )), this, SLOT(onSbxDetectionLevelChanged(int)) );
    connect ( d_data->m_ui->cbxTestMode, SIGNAL(currentIndexChanged( int )), this, SLOT(onCbxTestModeChanged(int)) );

    // Pixel Position Table
    connect( d_data->m_ui->btnAdd, SIGNAL(clicked()), this, SLOT(onAddClicked()) );
    connect( d_data->m_ui->btnRemove, SIGNAL(clicked()), this, SLOT(onRemoveClicked()) );
    connect( d_data->m_ui->btnSort, SIGNAL(clicked()), this, SLOT(onSortClicked()) );
    connect( d_data->m_ui->btnClear, SIGNAL(clicked()), this, SLOT(onClearClicked()) );
    connect( d_data->m_ui->btnImport, SIGNAL(clicked()), this, SLOT(onImportClicked()) );
    connect( d_data->m_ui->btnExport, SIGNAL(clicked()), this, SLOT(onExportClicked()) );
}

/******************************************************************************
 * DpccBox::~DpccBox
 *****************************************************************************/
DpccBox::~DpccBox()
{
    delete d_data;
}

/******************************************************************************
 * DpccBox::prepareMode
 *****************************************************************************/
void DpccBox::prepareMode( const Mode )
{
    // @TODO do nothing here
}

/******************************************************************************
 * DpccBox::loadSettings
 *****************************************************************************/
void DpccBox::loadSettings( QSettings & s )
{
    QVector<int> yPos;
    QVector<int> xPos;

    s.beginGroup( DPCC_SETTINGS_SECTION_NAME );
    setDpccEnabled( s.value( DPCC_SETTINGS_ENABLE ).toBool() );
    setDpccMode( s.value( DPCC_SETTINGS_MODE ).toInt() );
    setDpccLevel( s.value( DPCC_SETTINGS_LEVEL ).toInt() );
    setDpccTestMode( s.value( DPCC_SETTINGS_TEST_MODE ).toInt() );

    /* Note: The defect pixel table is not loaded, because it is device specific.
     * The user may use the separate DPCC save / load mechanism to handle table
     * import and export */
//    // NOTE: call qRegisterMetaTypeStreamOperators<QVector<int> >("QVector<int>"); in main.cpp
//    yPos = s.value( DPCC_SETTINGS_Y_POSITIONS ).value<QVector<int> >();
//    xPos = s.value( DPCC_SETTINGS_X_POSITIONS ).value<QVector<int> >();
//    d_data->fillTable( yPos, xPos );

    // Call on onWriteTableToRamClicked to transmitt loaded data to the device
//    onSaveTableClicked();
    s.endGroup();

}

/******************************************************************************
 * DpccBox::saveSettings
 *****************************************************************************/
void DpccBox::saveSettings( QSettings & s )
{
    // Get data and sort it before storing
    QVector<int> yPos;
    QVector<int> xPos;
    d_data->getDataFromModel( yPos, xPos );
    pairSort( yPos, xPos );
    d_data->fillTable( yPos, xPos );

    // Write to file
    s.beginGroup( DPCC_SETTINGS_SECTION_NAME );
    s.setValue( DPCC_SETTINGS_ENABLE, getDpccEnabled() );
    s.setValue( DPCC_SETTINGS_MODE, getDpccMode() );
    s.setValue( DPCC_SETTINGS_LEVEL, getDpccLevel() );
    s.setValue( DPCC_SETTINGS_TEST_MODE, getDpccTestMode() );

    /* Note: The defect pixel table is not saved, because it is device specific.
     * The user may use the separate DPCC save / load mechanism to handle table
     * import and export */
//    // NOTE: call qRegisterMetaTypeStreamOperators<QVector<int> >("QVector<int>"); in main.cpp
//    s.setValue( DPCC_SETTINGS_Y_POSITIONS, QVariant::fromValue<QVector<int>>(yPos) );
//    s.setValue( DPCC_SETTINGS_X_POSITIONS, QVariant::fromValue<QVector<int>>(xPos) );
    s.endGroup();
}

/******************************************************************************
 * DpccBox::applySettings
 *****************************************************************************/
void DpccBox::applySettings( void )
{
    emit DpccEnableChanged( getDpccEnabled() );
    emit DpccCorrectionModeChanged( getDpccMode() );
    emit DpccDetectionLevelChanged( getDpccLevel() );

    /* Note: The defect pixel table is not applied, because it is device specific.
     * The user may use the separate DPCC save / load mechanism to handle table
     * import and export */
}

/******************************************************************************
 * DpccBox::getDpccEnabled
 *****************************************************************************/
bool DpccBox::getDpccEnabled() const
{
    return ( (d_data->m_ui->cbxEnable->checkState() == Qt::Checked) ? true : false );
}

/******************************************************************************
 * DpccBox::setDpccEnabled
 *****************************************************************************/
void DpccBox::setDpccEnabled( const bool enable )
{
    d_data->m_ui->cbxEnable->blockSignals( true );
    d_data->m_ui->cbxEnable->setCheckState( enable ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxEnable->blockSignals( false );

    emit DpccEnableChanged( enable ? 1 : 0 );
}

/******************************************************************************
 * DpccBox::getDpccMode
 *****************************************************************************/
int DpccBox::getDpccMode() const
{
    return ( d_data->m_ui->cbxCorrectionMode->currentIndex() );
}

/******************************************************************************
 * DpccBox::setDpccMode
 *****************************************************************************/
void DpccBox::setDpccMode( const int mode )
{
    d_data->m_ui->cbxCorrectionMode->blockSignals( true );
    d_data->m_ui->cbxCorrectionMode->setCurrentIndex( mode );
    d_data->m_ui->cbxCorrectionMode->blockSignals( false );

    emit DpccCorrectionModeChanged( mode );
}

/******************************************************************************
 * DpccBox::getDpccLevel
 *****************************************************************************/
int DpccBox::getDpccLevel() const
{
    return ( d_data->m_ui->sbxLevel->value() );
}

/******************************************************************************
 * DpccBox::setDpccLevel
 *****************************************************************************/
void DpccBox::setDpccLevel( const int level )
{
    d_data->m_ui->sbxLevel->blockSignals( true );
    d_data->m_ui->sbxLevel->setValue( level );
    d_data->m_ui->sbxLevel->blockSignals( false );

    emit DpccDetectionLevelChanged( level );
}

/******************************************************************************
 * DpccBox::getDpccTestMode
 *****************************************************************************/
int DpccBox::getDpccTestMode() const
{
    return ( d_data->m_ui->cbxTestMode->currentIndex() );
}

/******************************************************************************
 * DpccBox::setDpccTestMode
 *****************************************************************************/
void DpccBox::setDpccTestMode( const int mode )
{
    d_data->m_ui->cbxTestMode->blockSignals( true );
    d_data->m_ui->cbxTestMode->setCurrentIndex( mode );
    d_data->m_ui->cbxTestMode->blockSignals( false );

    emit DpccCorrectionModeChanged( mode );
}


/******************************************************************************
 * DpccBox::setUserEditableTableVisible
 *****************************************************************************/
void DpccBox::setFullDpccFeatureSetVisible( bool visible )
{
    /* Note: Some devices only have a reduced DPCC module which only allows
     * auto load and changing the test mode. */

    // Show / hide position table
    d_data->m_ui->gbxPositionTable->setVisible( visible );

    // Show / hide data transmission box
    d_data->m_ui->gbxDataTransmission->setVisible( visible );

    // Show / hide correction mode elements
    d_data->m_ui->lblCorrectionMode->setVisible( visible );
    d_data->m_ui->cbxCorrectionMode->setVisible( visible );

    // Show / hide detection level elements
    d_data->m_ui->lblLevel->setVisible( visible );
    d_data->m_ui->sbxLevel->setVisible( visible );

    // Show / hide information label
    d_data->m_ui->lblInformation->setVisible( visible );
}

/******************************************************************************
 * DpccBox::clearDpccTable
 *****************************************************************************/
void DpccBox::clearDpccTable()
{
    onClearClicked();
}

/******************************************************************************
 * DpccBox::onAddClicked
 *****************************************************************************/
void DpccBox::onAddClicked()
{
    // Insert new row at the bottom of the table, if the row count is not at maximum
    int rowCount = d_data->m_ui->tblPositions->model()->rowCount();
    if ( rowCount >= DPCC_TABLE_MAX_NO_ROWS )
    {
        // Show a tooltip
        QToolTip::showText( QCursor::pos(), QString ("You can not add more than %1 Positions.").arg(DPCC_TABLE_MAX_NO_ROWS) );
        return;
    }
    d_data->m_ui->tblPositions->model()->insertRow(rowCount);

    // Set style of the new row
    QModelIndex rowIndex = d_data->m_ui->tblPositions->model()->index( rowCount, 0, QModelIndex() );
    QModelIndex colIndex = d_data->m_ui->tblPositions->model()->index( rowCount, 1, QModelIndex() );
    d_data->m_ui->tblPositions->model()->setData( rowIndex, QVariant(Qt::AlignVCenter | Qt::AlignRight), Qt::TextAlignmentRole );
    d_data->m_ui->tblPositions->model()->setData( colIndex, QVariant(Qt::AlignVCenter | Qt::AlignRight), Qt::TextAlignmentRole );

    // Select the newly created row
     d_data->m_ui->tblPositions->setCurrentIndex( rowIndex );
}

/******************************************************************************
 * DpccBox::onRemoveClicked
 *****************************************************************************/
void DpccBox::onRemoveClicked()
{
    QItemSelectionModel * select = (QItemSelectionModel *)d_data->m_ui->tblPositions->selectionModel();

    if ( select->hasSelection() )
    {
        QModelIndexList list = select->selectedRows();
        foreach ( QModelIndex index, list )
        {
            d_data->m_ui->tblPositions->model()->removeRow( index.row() );
        }

        // Select next row (if there is one)
        int rowCount = d_data->m_ui->tblPositions->model()->rowCount();
        if (list.last().row() < rowCount)
        {
            d_data->m_ui->tblPositions->setCurrentIndex(list.last());
        }
        // If this is the last row, select the previous row (if there is one)
        else if (rowCount > 0)
        {
            d_data->m_ui->tblPositions->setCurrentIndex(d_data->m_ui->tblPositions->model()->index(rowCount - 1, 0));
        }
    }
}

/******************************************************************************
 * DpccBox::onSortClicked
 *****************************************************************************/
void DpccBox::onSortClicked()
{
    // Create a list of pairs with the defect pixel coordinates
    setWaitCursor();
    QVector<int> yPos;
    QVector<int> xPos;
    d_data->getDataFromModel( yPos, xPos );

    // Sort it and remove duplicates
    pairSort( yPos, xPos );

    // Write the results into the table
    d_data->fillTable( yPos, xPos );
    setNormalCursor();
}

/******************************************************************************
 * DpccBox::onClearClicked
 *****************************************************************************/
void DpccBox::onClearClicked()
{
    // Create a new data model and set it (this deletes the old model)
    d_data->initDataModel();
    d_data->setDataModel();
}

/******************************************************************************
 * DpccBox::onImportClicked
 *****************************************************************************/
void DpccBox::onImportClicked()
{
    QString directory = QDir::currentPath();

    // NOTE: It can fail on gtk-systems when an empty filename is given
    //       in the native dialog-box, because GTK sends a SIGSEGV-signal
    //       to process and this is not handled by Qt.
    QFileDialog dialog( this );
    dialog.setDefaultSuffix( "csv" );
    d_data->m_filename = dialog.getOpenFileName(
        this, tr("Load Defect Pixel Table"),
        directory,
        "Comma Seperated Values (CSV) File (*.csv);;All files (*.*)"
    );

    if ( NULL != d_data->m_filename )
    {
        QFileInfo file( d_data->m_filename );
        if ( file.suffix().isEmpty() )
        {
            d_data->m_filename += ".csv";
        }

        if ( fileExists(d_data->m_filename) )
        {
            // Read CSV file
            QVector<QVector<int>> table(2);
            QVector<QPair<int, int>> boundaries( { QPair<int, int>(0, d_data->m_delegate->getFirstColBound()),
                                                   QPair<int, int>(0, d_data->m_delegate->getSecondColBound()) });

            // If no error occurred, paste the data into the table
            if ( loadTableCsv( d_data->m_filename, DPCC_TABLE_MAX_NO_ROWS, boundaries, table) == 0)
            {
                d_data->fillTable(table[0], table[1]);
            }
        }
    }
}

/******************************************************************************
 * DpccBox::onExportClicked
 *****************************************************************************/
void DpccBox::onExportClicked()
{
    QString directory = QDir::currentPath();

    // NOTE: It can fail on gtk-systems when an empty filename is given
    //       in the native dialog-box, because GTK sends a SIGSEGV-signal
    //       to process and this is not handled by Qt.
    QFileDialog dialog( this );
    dialog.setDefaultSuffix( "csv" );
    d_data->m_filename = dialog.getSaveFileName(
        this, tr("Save Defect Pixel Table"),
        directory,
        "Comma Seperated Values (CSV) File (*.csv);;All files (*.*)"
    );

    if ( NULL != d_data->m_filename )
    {
        QFileInfo file( d_data->m_filename );
        if ( file.suffix().isEmpty() )
        {
            d_data->m_filename += ".csv";
        }

        // Get data and sort it before storing
        QVector<QVector<int>> table(2);
        d_data->getDataFromModel( table[0], table[1] );
        pairSort( table[0], table[1] );
        d_data->fillTable( table[0], table[1] );

        // Write to CSV file
        saveTableCsv(d_data->m_filename, table);
    }
}

/******************************************************************************
 * DpccBox::onTransmittTableClicked
 *****************************************************************************/
void DpccBox::onTransmittTableClicked()
{
    setWaitCursor();

    // Get data and sort it before storing
    QVector<int> yPos, xPos;
    d_data->getDataFromModel( yPos, xPos );
    pairSort( yPos, xPos );
    d_data->fillTable( yPos, xPos );

    // Write table to camera Ram
    emit DpccWriteTableToRam(xPos, yPos);

    // If "save permanently" checkbox is checked, save table in camera Flash
    if ( d_data->m_ui->cbxSaveTablePermanently->isChecked() )
    {
        emit DpccStoreTableInFlash();
    }

    setNormalCursor();
}

/******************************************************************************
 * DpccBox::onLoadTableClicked
 *****************************************************************************/
void DpccBox::onLoadTableClicked()
{
    setWaitCursor();

    // Load table from camera Flash to Ram
    emit DpccRestoreTableFromFlash();

    // Load table from Ram to GUI
    emit DpccLoadTableFromRam();

    setNormalCursor();
}

/******************************************************************************
 * DpccBox::onAutoLoadTableClicked
 *****************************************************************************/
void DpccBox::onAutoLoadTableClicked()
{
    setWaitCursor();

    // Automatically load / fill table
    emit DpccAutoLoadTable();

    // Load table from Ram to GUI
    emit DpccLoadTableFromRam();

    setNormalCursor();

    // Check amount of pixels loaded
    int rowCount = d_data->m_ui->tblPositions->model()->rowCount();
    if ( rowCount >= DPCC_TABLE_MAX_NO_ROWS )
    {
        // Show a message box, that the auto loader found too many pixels
        QString msgText = QString( "The auto detection found %1 defect pixels, which is the maximum that can be handled.\n\n"
                                   "There might be more pixels found, which can not be displayed here. "
                                   "To decrease the amount of pixels found, choose a lower auto-detection level, or change your test setup." )
                                  .arg( DPCC_TABLE_MAX_NO_ROWS );
        QMessageBox msgBox;
        msgBox.setWindowTitle("The autoload function found too many pixels");
        msgBox.setText( msgText );
        msgBox.exec();
    }
}

/******************************************************************************
 * DpccBox::onCbxEnableChanged
 *****************************************************************************/
void DpccBox::onCbxEnableChanged( int value )
{
    bool enable = ( value == Qt::Checked ) ? true : false;

    // Emit signal
    setWaitCursor();
    emit DpccEnableChanged( enable ? 1u : 0u );
    setNormalCursor();
}


/******************************************************************************
 * DpccBox::onCbxCorrectionModeChanged
 *****************************************************************************/
void DpccBox::onCbxCorrectionModeChanged( int mode )
{
    // Emit signal
    setWaitCursor();
    emit DpccCorrectionModeChanged( mode );
    setNormalCursor();
}

/******************************************************************************
 * DpccBox::onSbxDetectionLevelChanged
 *****************************************************************************/
void DpccBox::onSbxDetectionLevelChanged( int level )
{
    // Emit signal
    setWaitCursor();
    emit DpccDetectionLevelChanged( level );
    setNormalCursor();
}

/******************************************************************************
 * DpccBox::onCbxTestModeChanged
 *****************************************************************************/
void DpccBox::onCbxTestModeChanged( int mode )
{
    // Emit signal
    setWaitCursor();
    emit DpccTestModeChanged( mode );
    setNormalCursor();
}

/******************************************************************************
 * DpccBox::onDpccEnableChanged
 *****************************************************************************/
void DpccBox::onDpccEnableChanged( int enable )
{
    d_data->m_ui->cbxEnable->blockSignals( true );
    d_data->m_ui->cbxEnable->setCheckState( (enable == 1) ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxEnable->blockSignals( false );
}

/******************************************************************************
 * DpccBox::onDpccCorrectionModeChanged
 *****************************************************************************/
void DpccBox::onDpccCorrectionModeChanged( int mode )
{
    d_data->m_ui->cbxCorrectionMode->blockSignals( true );
    d_data->m_ui->cbxCorrectionMode->setCurrentIndex( mode );
    d_data->m_ui->cbxCorrectionMode->blockSignals( false );
}

/******************************************************************************
 * DpccBox::onDpccDetectionLevelChanged
 *****************************************************************************/
void DpccBox::onDpccDetectionLevelChanged( int level )
{
    d_data->m_ui->sbxLevel->blockSignals( true );
    d_data->m_ui->sbxLevel->setValue( level );
    d_data->m_ui->sbxLevel->blockSignals( false );
}

/******************************************************************************
 * DpccBox::onDpccTestModeChanged
 *****************************************************************************/
void DpccBox::onDpccTestModeChanged( int mode )
{
    d_data->m_ui->cbxTestMode->blockSignals( true );
    d_data->m_ui->cbxTestMode->setCurrentIndex( mode );
    d_data->m_ui->cbxTestMode->blockSignals( false );
}

/******************************************************************************
 * DpccBox::onDpccTableFromCameraLoaded
 *****************************************************************************/
void DpccBox::onDpccTableFromCameraLoaded( QVector<int> xPos, QVector<int> yPos )
{
    d_data->fillTable( yPos, xPos );
}

/******************************************************************************
 * DpccBox::onDpccVideoModeChanged
 *****************************************************************************/
void DpccBox::onDpccVideoModeChanged( int mode )
{
    // @TODO: Maybe it is better to not save and clear the table as soon as the mode
    // is changed, but instead set a flag which is evaluated the next time that the widget
    // is shown and then ask the user whether he wants to save his old table.

    // Get the resolution corresponding to the video mode
    int xRes, yRes;
    GetVideoModeResolution((VideoMode)mode, xRes, yRes);

    // If the model is empty, just apply the new boundaries
    if ( d_data->m_ui->tblPositions->model()->rowCount() == 0 )
    {
        d_data->m_delegate->setBounds(yRes - 1, xRes - 1);  // valid values: 0 ... (yRes - 1), 0 ... (xRes - 1)
    }
    // If the resolution has changed, the data in the table is not valid anymore
    else if ( yRes != (d_data->m_delegate->getFirstColBound() + 1) || xRes != (d_data->m_delegate->getSecondColBound() + 1) )
    {

        // Show a Dialog to save the current table
        setNormalCursor();
        QMessageBox msgBox;
        msgBox.setWindowTitle("Defect Pixel Correction");
        msgBox.setText("The camera resolution has been changed.");
        msgBox.setInformativeText("Your defect pixel table does not match the new resolution and has to be cleared.\n\nDo you want to save your old table?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setIcon(QMessageBox::Question);

        int ret = msgBox.exec();
        switch (ret)
        {
          case QMessageBox::Yes:    // Save was clicked, show save dialog
              onExportClicked();
              break;
          case QMessageBox::No:     // Don't Save was clicked, do nothing
              break;
          default:
              // should never be reached
              break;

        }
        setWaitCursor();

        // Clear the table, apply the new bounds
        onClearClicked();
        d_data->m_delegate->setBounds(yRes - 1, xRes - 1);  // valid values: 0 ... (yRes - 1), 0 ... (xRes - 1)

        // Also clear the table on the device, as the data is not valid anymore
        // @TODO Maybe this will be done directly in the camera if the resolution changes,
        // or the camera will have different dpcc storages for each resolution. For now, this
        // is not a use case.

    }
}
