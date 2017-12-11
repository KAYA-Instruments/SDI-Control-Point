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
 * @file    lutbox.cpp
 *
 * @brief   Implementation of lut setting box
 *
 *****************************************************************************/
#include <QtDebug>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QFileInfo>
#include <QItemDelegate>
#include <QLineEdit>
#include <QDir>
#include <QFileDialog>

#include <csvwrapper.h>
#include <simple_math/gamma.h>

#include "cubic_interpolation.h"
#include "lutbox.h"
#include "ui_lutbox.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_LutBox;
}

/******************************************************************************
 * general
 *****************************************************************************/
enum LutMode
{
    LUT_MODE_INVALID = -1,      /**< for range check */
    LUT_MODE_FIRST   = 0,       /**< Table based interpolation mode (load table, do cubic spline interpolate) */
    LUT_MODE_INTERPOLATE = 0,   /**< Table based interpolation mode (load table, do cubic spline interpolate) */
    LUT_MODE_FAST_GAMMA = 1,    /**< Fast gamma function with direct computation and without interpolation */
    LUT_MODE_FIXED = 2,         /**< Fixed gamma table that can not be edited */
    LUT_MODE_MAX                /**< number of available components */
};

enum LutFixedMode
{
    LUT_FIXED_INVALID = -1,     /**< for range check */
    LUT_FIXED_FIRST   = 0,      /**< Default Rec.709 gamma curve */
    LUT_FIXED_REC709 = 0,       /**< Default Rec.709 gamma curve */
    LUT_FIXED_PQ = 1,           /**< PQ gamma curve function specified in Rec. ITU-R BT.2100 used for HDR content */
    LUT_FIXED_HLG = 2,          /**< Hybrid Log-Gamma (HLG) function specified in Rec. ITU-R BT.2100 used for HDR content */
    LUT_FIXED_MAX               /**< number of available components */
};

/******************************************************************************
 * unit-scale
 *****************************************************************************/
#define SCALING_FACTOR                      ( 1000.0f )
#define TO_FLOAT(x)                         (((float)x) / SCALING_FACTOR)
#define TO_DOUBLE(x)                        (((double)x) / SCALING_FACTOR)
#define TO_INT(x)                           ((int)(x * SCALING_FACTOR))
#define DEFAULT_BIT_WIDTH                   ( 12 )

/******************************************************************************
 * local definitions
 *****************************************************************************/
#define LINEAR_CURVE_ID                     ( 0 )
#define INTERPOLATION_CURVE_ID              ( 1 )
#define FINAL_CURVE_ID                      ( 2 )
#define SAMPLE_CURVE_ID                     ( 3 )
#define HIGHLIGHT_CURVE_ID                  ( 4 )

/******************************************************************************
 * STRAIGHT
 *****************************************************************************/
#define STRAIGHT_THRESHOLD                  ( 1000 )
#define STRAIGHT_LINEAR_CONTRAST            ( 1000 )
#define STRAIGHT_LINEAR_BRIGHTNESS          ( 0 )
#define STRAIGHT_CONTRAST                   ( 0 )
#define STRAIGHT_GAMMA                      ( 0 )
#define STRAIGHT_BRIGHTNESS                 ( 0 )

/******************************************************************************
 * REC.709
 *****************************************************************************/
#define REC709_THRESHOLD                    ( 18 )
#define REC709_LINEAR_CONTRAST              ( 4500 )
#define REC709_LINEAR_BRIGHTNESS            ( 0 )
#define REC709_CONTRAST                     ( 1099 )
#define REC709_GAMMA                        ( 2222 )
#define REC709_BRIGHTNESS                   ( -99 )

/******************************************************************************
 * BT.601
 *****************************************************************************/
#define BT601_THRESHOLD                     ( 0 )
#define BT601_LINEAR_CONTRAST               ( 0 )
#define BT601_LINEAR_BRIGHTNESS             ( 0 )
#define BT601_CONTRAST                      ( 1000 )
#define BT601_GAMMA                         ( 450 )
#define BT601_BRIGHTNESS                    ( 0 )

/******************************************************************************
 * Settings
 *****************************************************************************/
#define LUT_SETTINGS_SECTION_NAME           ( "LUT" )
#define LUT_SETTINGS_ENABLE                 ( "enable" )
#define LUT_SETTINGS_MODE                   ( "mode" )
#define LUT_SETTINGS_FIXED_MODE             ( "fixed_mode" )
#define LUT_SETTINGS_STORAGE                ( "storage" )
#define LUT_SETTINGS_SAMPLE_X               ( "sample_x" )
#define LUT_SETTINGS_SAMPLE_Y               ( "sample_y" )
#define LUT_SETTINGS_SAMPLE_RED_X           ( "sample_red_x" )
#define LUT_SETTINGS_SAMPLE_RED_Y           ( "sample_red_y" )
#define LUT_SETTINGS_SAMPLE_GREEN_X         ( "sample_green_x" )
#define LUT_SETTINGS_SAMPLE_GREEN_Y         ( "sample_green_y" )
#define LUT_SETTINGS_SAMPLE_BLUE_X          ( "sample_blue_x" )
#define LUT_SETTINGS_SAMPLE_BLUE_Y          ( "sample_blue_y" )
#define LUT_SETTINGS_REC709_LCONTRAST       ( "rec709_lcontrast" )
#define LUT_SETTINGS_REC709_LBRIGHTNESS     ( "rec709_lbrightness" )
#define LUT_SETTINGS_REC709_THRESHOLD       ( "rec709_threshold" )
#define LUT_SETTINGS_REC709_CONTRAST        ( "rec709_contrast" )
#define LUT_SETTINGS_REC709_GAMMA           ( "rec709_gamma" )
#define LUT_SETTINGS_REC709_BRIGHTNESS      ( "rec709_brightness" )
#define LUT_SETTINGS_FAST_GAMMA             ( "fast_gamma" )
#define LUT_TABLE_NO_ROWS                   ( 24 )
#define LUT_TABLE_NO_COLUMNS                ( 2 )
#define LUT_NO_PRESETS                      ( 5 )

/******************************************************************************
 * Colors
 *****************************************************************************/
const QColor InterpolColor[LutBox::LutChannelMax] =
{
    Qt::yellow,
    Qt::red,
    Qt::green,
    Qt::blue
};

const QColor FinalColor[LutBox::LutChannelMax] =
{
    Qt::darkYellow,
    Qt::darkRed,
    Qt::darkGreen,
    Qt::darkBlue
};

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
 * Delegate
 *****************************************************************************/
class LutDelegate : public QItemDelegate
{
private:
    int min;
    int max;

public:
    LutDelegate( int min, int max ) : QItemDelegate()
    {
        this->min = min;
        this->max = max;
    }

    // create a single editable table-cell
    QWidget* createEditor( QWidget * parent, const QStyleOptionViewItem &, const QModelIndex & ) const
    {
        QLineEdit * edit = new QLineEdit( parent );

        // set validator
        QIntValidator *valid = new QIntValidator( min, max, edit );
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
};

/******************************************************************************
 * LutBox::PrivateData
 *****************************************************************************/
class LutBox::PrivateData
{
public:
    PrivateData( QWidget * parent )
        : m_ui( new Ui::UI_LutBox )
        , m_delegate( new LutDelegate( 0, ((1 << DEFAULT_BIT_WIDTH) - 1)) )
        , m_enable( true )
        , m_mode( LUT_MODE_INVALID )
        , m_fixed_mode( LUT_FIXED_INVALID )
        , m_preset( 0 )
        , m_ch( Master )
        , m_bit_width( DEFAULT_BIT_WIDTH )
    {
        // initialize UI
        m_ui->setupUi( parent );

        // configure REC.709 default
        m_ui->sbxRec709LinContrast->setRange( 
                TO_DOUBLE(LUT_LINEAR_CONTRAST_MIN), TO_DOUBLE(LUT_LINEAR_CONTRAST_MAX) );
        m_ui->sbxRec709LinContrast->setDecimals( LUT_LINEAR_CONTRAST_DECIMALS );
        m_ui->sbxRec709LinContrast->setSingleStep( TO_DOUBLE(LUT_LINEAR_CONTRAST_STEP) );
        
        m_ui->sbxRec709LinOffset->setRange( 
                TO_DOUBLE(LUT_LINEAR_BRIGHTNESS_MIN), TO_DOUBLE(LUT_LINEAR_BRIGHTNESS_MAX) );
        m_ui->sbxRec709LinOffset->setDecimals( LUT_LINEAR_BRIGHTNESS_DECIMALS );
        m_ui->sbxRec709LinOffset->setSingleStep( TO_DOUBLE(LUT_LINEAR_BRIGHTNESS_STEP) );

        m_ui->sbxRec709Threshold->setRange( 
                TO_DOUBLE(LUT_THRESHOLD_MIN), TO_DOUBLE(LUT_THRESHOLD_MAX) );
        m_ui->sbxRec709Threshold->setDecimals( LUT_THRESHOLD_DECIMALS );
        m_ui->sbxRec709Threshold->setSingleStep( TO_DOUBLE(LUT_THRESHOLD_STEP) );

        m_ui->sbxRec709Contrast->setRange( 
                TO_DOUBLE(LUT_CONTRAST_MIN), TO_DOUBLE(LUT_CONTRAST_MAX) );
        m_ui->sbxRec709Contrast->setDecimals( LUT_CONTRAST_DECIMALS );
        m_ui->sbxRec709Contrast->setSingleStep( TO_DOUBLE(LUT_CONTRAST_STEP) );

        m_ui->sbxRec709Gamma->setRange( 
                TO_DOUBLE(LUT_GAMMA_MIN), TO_DOUBLE(LUT_GAMMA_MAX) );
        m_ui->sbxRec709Gamma->setDecimals( LUT_GAMMA_DECIMALS );
        m_ui->sbxRec709Gamma->setSingleStep( TO_DOUBLE(LUT_GAMMA_STEP) );

        m_ui->sbxRec709Offset->setRange( 
                TO_DOUBLE(LUT_BRIGHTNESS_MIN), TO_DOUBLE(LUT_BRIGHTNESS_MAX) );
        m_ui->sbxRec709Offset->setDecimals( LUT_BRIGHTNESS_DECIMALS );
        m_ui->sbxRec709Offset->setSingleStep( TO_DOUBLE(LUT_BRIGHTNESS_STEP) );

        m_ui->grpPreset->setId( m_ui->cbxPreset0, 0 ); 
        m_ui->grpPreset->setId( m_ui->cbxPreset1, 1 ); 
        m_ui->grpPreset->setId( m_ui->cbxPreset2, 2 ); 
        m_ui->grpPreset->setId( m_ui->cbxPreset3, 3 ); 
        m_ui->grpPreset->setId( m_ui->cbxPreset4, 4 ); 

        initRec709Default();
        
        initDataModel( Master );
        initDataModel( Red );
        initDataModel( Green );
        initDataModel( Blue );

        initLutPlot( Master, m_ui->LutPlot );
        initLutPlot( Red   , m_ui->LutPlotRed );
        initLutPlot( Green , m_ui->LutPlotGreen );
        initLutPlot( Blue  , m_ui->LutPlotBlue );

        m_ui->tblSamples->setItemDelegate( m_delegate );
        m_ui->tblSamples->resizeColumnsToContents();
        m_ui->tblSamples->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
        m_ui->tblSamples->setSelectionBehavior( QAbstractItemView::SelectRows );
        setDataModel( Master );
        
        drawHelperLine( Master );
        drawHelperLine( Red );
        drawHelperLine( Green );
        drawHelperLine( Blue );

        // vertical color gradient in plot
        QLinearGradient axisRectGradient;
        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(120, 120, 120));
        axisRectGradient.setColorAt(1, QColor(48, 48, 48));
        m_ui->LutPlot->axisRect()->setBackground(axisRectGradient);

        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(120, 48, 48));
        axisRectGradient.setColorAt(1, QColor(48, 48, 48));
        m_ui->LutPlotRed->axisRect()->setBackground(axisRectGradient);
 
        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(48, 120, 48));
        axisRectGradient.setColorAt(1, QColor(48, 48, 48));
        m_ui->LutPlotGreen->axisRect()->setBackground(axisRectGradient);

        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(48, 48, 120));
        axisRectGradient.setColorAt(1, QColor(48, 48, 48));
        m_ui->LutPlotBlue->axisRect()->setBackground(axisRectGradient);
    }

    ~PrivateData()
    {
        delete m_ui;
        delete m_delegate;
        
        delete m_final_interpolate[Blue];
        delete m_final_interpolate[Green];
        delete m_final_interpolate[Red];
        delete m_final_interpolate[Master];

        delete m_interpolate[Blue];
        delete m_interpolate[Green];
        delete m_interpolate[Red];
        delete m_interpolate[Master];

        delete m_model[Blue];
        delete m_model[Green];
        delete m_model[Red];
        delete m_model[Master];
    };
    
    // set Rec.709 
    void initRec709Default()
    {
        m_ui->sbxRec709LinContrast->blockSignals( true );
        m_ui->sbxRec709LinContrast->setValue( TO_DOUBLE(REC709_LINEAR_CONTRAST) );
        m_ui->sbxRec709LinContrast->blockSignals( false );

        m_ui->sbxRec709LinOffset->blockSignals( true );
        m_ui->sbxRec709LinOffset->setValue( TO_DOUBLE(REC709_LINEAR_BRIGHTNESS) );
        m_ui->sbxRec709LinOffset->blockSignals( false );

        m_ui->sbxRec709Threshold->blockSignals( true );
        m_ui->sbxRec709Threshold->setValue( TO_DOUBLE(REC709_THRESHOLD) );
        m_ui->sbxRec709Threshold->blockSignals( false );

        m_ui->sbxRec709Contrast->blockSignals( true );
        m_ui->sbxRec709Contrast->setValue( TO_DOUBLE(REC709_CONTRAST) );
        m_ui->sbxRec709Contrast->blockSignals( false );

        m_ui->sbxRec709Gamma->blockSignals( true );
        m_ui->sbxRec709Gamma->setValue( TO_DOUBLE(REC709_GAMMA) );
        m_ui->sbxRec709Gamma->blockSignals( false );

        m_ui->sbxRec709Offset->blockSignals( true );
        m_ui->sbxRec709Offset->setValue( TO_DOUBLE(REC709_BRIGHTNESS) );
        m_ui->sbxRec709Offset->blockSignals( false );
    }

    // initialize plot widget
    void initLutPlot( LutChannel ch, QCustomPlot * plot )
    {
        // threshold curve
        QPen penLinear( QColor(177, 177, 177) );
        plot->addGraph();
        plot->graph( LINEAR_CURVE_ID )->setPen( penLinear );

        // interpolated curve
        QPen penInterpolation( InterpolColor[ch] );
        plot->addGraph();
        plot->graph( INTERPOLATION_CURVE_ID )->setPen( penInterpolation );
        
        // final interpolated curve
        QPen penFinal( FinalColor[ch] );
        plot->addGraph();
        plot->graph( FINAL_CURVE_ID )->setPen( penFinal );

        // sample curve
        QPen penSamples( Qt::yellow );
        plot->addGraph();
        plot->graph( SAMPLE_CURVE_ID )->setPen( penSamples );
        plot->graph( SAMPLE_CURVE_ID )->setLineStyle(QCPGraph::lsNone);
        plot->graph( SAMPLE_CURVE_ID )->setScatterStyle( QCPScatterStyle(QCPScatterStyle::ssCircle, 4) );

        // highlight curve
        QPen penHighlight( Qt::magenta );
        plot->addGraph();
        plot->graph( HIGHLIGHT_CURVE_ID )->setPen( penHighlight );
        plot->graph( HIGHLIGHT_CURVE_ID )->setLineStyle(QCPGraph::lsNone);
        plot->graph( HIGHLIGHT_CURVE_ID )->setScatterStyle( QCPScatterStyle(QCPScatterStyle::ssCircle, 4) );

        plot->setBackground( QBrush(QColor(48,47,47)) );
        plot->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom );
        
        plot->xAxis->setLabel( "" );
        plot->xAxis->setLabelColor( QColor(177, 177, 177) );
        plot->yAxis->setLabel( "" );
        plot->yAxis->setLabelColor( QColor(177, 177, 177) );
        plot->xAxis->setRange( 0, 1 );
        plot->yAxis->setRange( 0, 1 );
        plot->xAxis->setBasePen( QPen(QColor(177, 177, 177), 1) );
        plot->yAxis->setBasePen( QPen(QColor(177, 177, 177), 1) );
        plot->xAxis->setTickPen( QPen(QColor(177, 177, 177), 1) );
        plot->yAxis->setTickPen( QPen(QColor(177, 177, 177), 1) );
        plot->xAxis->setSubTickPen( QPen( QColor(177, 177, 177), 1) );
        plot->yAxis->setSubTickPen( QPen( QColor(177, 177, 177), 1) );
        plot->xAxis->setTickLabelColor( QColor(177, 177, 177) );
        plot->yAxis->setTickLabelColor( QColor(177, 177, 177) );
        plot->axisRect()->setupFullAxesBox( true );

        plot->xAxis2->setBasePen( QPen(QColor(177, 177, 177), 1) );
        plot->yAxis2->setBasePen( QPen(QColor(177, 177, 177), 1) );
        plot->xAxis2->setTickPen( QPen(QColor(177, 177, 177), 1) );
        plot->yAxis2->setTickPen( QPen(QColor(177, 177, 177), 1) );
        plot->xAxis2->setSubTickPen( QPen( QColor(177, 177, 177), 1) );
        plot->yAxis2->setSubTickPen( QPen( QColor(177, 177, 177), 1) );
        plot->xAxis2->setTickLabelColor( QColor(177, 177, 177) );
        plot->yAxis2->setTickLabelColor( QColor(177, 177, 177) );
    }

    // initialize data model and interpolation context
    void initDataModel( LutChannel ch )
    {
        m_model[ch] =  new QStandardItemModel( 0, LUT_TABLE_NO_COLUMNS, NULL );
        m_model[ch]->setHorizontalHeaderItem( 0, new QStandardItem(QString("X")) );
        m_model[ch]->setHorizontalHeaderItem( 1, new QStandardItem(QString("Y")) );

        m_model[ch]->insertRows( 0, 2 );

        // add 2 rows (start and end-point)
        m_model[ch]->blockSignals( true );
        for ( int i=0; i < m_model[ch]->rowCount(); ++i )
        {
            QModelIndex x_i = m_model[ch]->index( i, 0, QModelIndex() );
            QModelIndex y_i = m_model[ch]->index( i, 1, QModelIndex() );

            // set used or clear unsed sample values
            m_model[ch]->setData( x_i, (i==0) ? 0 : ((1 << m_bit_width) - 1) );
            m_model[ch]->setData( y_i, (i==0) ? 0 : ((1 << m_bit_width) - 1) );

            // set alignment 
            m_model[ch]->setData( x_i, QVariant(Qt::AlignVCenter | Qt::AlignRight), Qt::TextAlignmentRole );
            m_model[ch]->setData( y_i, QVariant(Qt::AlignVCenter | Qt::AlignRight), Qt::TextAlignmentRole );
        }
        m_model[ch]->blockSignals( false );

        // x-coordinate of start-point is fix (0)
        m_model[ch]->item( 0, 0 )->setFlags( m_model[ch]->item( 0, 0 )->flags() ^ Qt::ItemIsEditable );
        m_model[ch]->item( 0, 0 )->setBackground( QColor(48, 48, 48) );

        // x-coordinate of end-point is fix ((1 << LUT_BIT_WIDTH) - 1)
        m_model[ch]->item( 1, 0 )->setFlags( m_model[ch]->item( 1, 0 )->flags() ^ Qt::ItemIsEditable );
        m_model[ch]->item( 1, 0 )->setBackground( QColor(48, 48, 48) );
        
        // initialize interpolation context
        m_interpolate[ch]       = new CubicInterpolation();
        m_final_interpolate[ch] = new CubicInterpolation();
    }
    
    // set data model
    void setDataModel( LutChannel ch )
    {
        // see http://doc.qt.io/qt-5.7/qabstractitemview.html#setModel
        //
        // get pointer to old selection model
        QItemSelectionModel *m = m_ui->tblSamples->selectionModel();
        // set new data model
        m_ui->tblSamples->setModel( m_model[ch] );
        // delete old selection model
        delete m;
    }

    // set samples in tableview widget
    void setSamples( LutChannel ch, QVector<int> &x, QVector<int> &y )
    {
        Q_ASSERT( x.count() == y.count() );
        Q_ASSERT( m_model[ch] != NULL );
        
        // update data model (add or remove rows)
        int rc   = m_model[ch]->rowCount();
        int rows = x.count();
        if ( rc < rows )
        {
            // add rows after row=0
            m_model[ch]->insertRows( 1, rows - rc );
        }
        else
        {
            m_model[ch]->removeRows( 1, rc - rows );
        }

        // set values
        m_model[ch]->blockSignals( true );
        for ( int i=0; i < x.count(); ++i )
        {
            QModelIndex x_i = m_model[ch]->index( i, 0, QModelIndex() );
            QModelIndex y_i = m_model[ch]->index( i, 1, QModelIndex() );

            // set used or clear unsed sample values
            m_model[ch]->setData( x_i, x[i] );
            m_model[ch]->setData( y_i, y[i] );

            // set alignment 
            m_model[ch]->setData( x_i, QVariant(Qt::AlignVCenter | Qt::AlignRight), Qt::TextAlignmentRole );
            m_model[ch]->setData( y_i, QVariant(Qt::AlignVCenter | Qt::AlignRight), Qt::TextAlignmentRole );
        }
        m_model[ch]->blockSignals( false );

        // update plot if in interpolation mode
        if ( m_mode == LUT_MODE_INTERPOLATE )
        {
            computeLutLine( ch, x, y );
        }
    }

    // return channel plot
    QCustomPlot * getLutPlot( LutChannel ch )
    {
        switch ( ch )
        {
            case Red:
                return ( m_ui->LutPlotRed );
            case Green:
                return ( m_ui->LutPlotGreen );
            case Blue:
                return ( m_ui->LutPlotBlue );
            default:
                return ( m_ui->LutPlot );
        }
    }

    void getDataFromModel( LutChannel ch, QVector<int> &x, QVector<int> &y  )
    {
        // sort samples and remove X-duplicates (last X wins)
        QMap<int, int> map;
        int x_, y_;

        for ( int i=0; i<m_model[ch]->rowCount(); i ++ )
        {
            x_ = m_model[ch]->data( m_model[ch]->index(i, 0), Qt::DisplayRole ).toInt();
            y_ = m_model[ch]->data( m_model[ch]->index(i, 1), Qt::DisplayRole ).toInt();
            map.insert( x_, y_ );
        }

        QList<int> xl = map.keys();
        QList<int> yl = map.values();

        x = QVector<int>::fromList( xl );
        y = QVector<int>::fromList( yl );
    }

    void getDataFromModel( LutChannel ch, QList<int> &x, QList<int> &y  )
    {
        // sort samples and remove X-duplicates (last X wins)
        QMap<int, int> map;
        int x_, y_;

        for ( int i=0; i<m_model[ch]->rowCount(); i ++ )
        {
            x_ = m_model[ch]->data( m_model[ch]->index(i, 0), Qt::DisplayRole ).toInt();
            y_ = m_model[ch]->data( m_model[ch]->index(i, 1), Qt::DisplayRole ).toInt();
            map.insert( x_, y_ );
        }

        x = map.keys();
        y = map.values();
    }

    void drawHelperLine( LutChannel ch )
    {
        QVector<double> x(2);
        QVector<double> y(2);

        QCustomPlot * plot = getLutPlot( ch );

        plot->graph( LINEAR_CURVE_ID )->clearData();

        x[0] = y[0] = 0.0f;
        x[1] = y[1] = 1.0f;

        plot->graph( LINEAR_CURVE_ID )->setData( x, y );
        
        plot->replot();
    }

    void computeLutLine( LutChannel ch, QVector<int> &x, QVector<int> &y )
    {
        QCustomPlot * plot = getLutPlot( ch );

        // plot 1024 values
        const int num_samples = 1024;
        const int scale_factor = (1 << m_bit_width) / num_samples;

        // clear all graphs 
        plot->graph( INTERPOLATION_CURVE_ID )->clearData();
        plot->graph( FINAL_CURVE_ID )->clearData();
        plot->graph( SAMPLE_CURVE_ID )->clearData();
        plot->graph( HIGHLIGHT_CURVE_ID )->clearData();

        // interpolated curve
        QVector<double> x1( num_samples );
        QVector<double> y1( num_samples );
        m_interpolate[ch]->setSamples( x, y );
        for ( int i=0; i<num_samples; i+=1 )
        {
            x1[i] = ((double)(i*scale_factor)) / (double)(1 << m_bit_width);
            y1[i] = ((double)m_interpolate[ch]->interpolate( (i*scale_factor) )) / (double)(1 << m_bit_width);
        }
        plot->graph( INTERPOLATION_CURVE_ID )->setData( x1, y1 );

        // final curve
        if ( ch != Master )
        {
            QVector<int> x2;
            QVector<int> y2;
            getDataFromModel( Master, x2, y2  );

            QVector<int> y3( y2.count() );
            for ( int i=0; i<y2.count(); i+=1 )
            {
                y3[i] = m_interpolate[ch]->interpolate( y2[i] );
            }
            m_final_interpolate[ch]->setSamples( x2, y3 );
            
            QVector<double> x4( num_samples );
            QVector<double> y4( num_samples );
            for ( int i=0; i<num_samples; i+=1 )
            {
                x4[i] = ((double)(i*scale_factor)) / (double)(1 << m_bit_width);
                y4[i] = ((double)m_final_interpolate[ch]->interpolate( (i*scale_factor) )) / (double)(1 << m_bit_width);
            }
            
            plot->graph( FINAL_CURVE_ID )->setData( x4, y4 );
        }
        // master curve and interpolation mode
        else if ( m_mode == LUT_MODE_INTERPOLATE )
        {
            QVector<int> x2;
            QVector<int> y2;

            getDataFromModel( Red, x2, y2  );
            computeLutLine( Red, x2, y2 );
            x2.clear();
            y2.clear();
            
            getDataFromModel( Green, x2, y2  );
            computeLutLine( Green, x2, y2 );
            x2.clear();
            y2.clear();
            
            getDataFromModel( Blue, x2, y2  );
            computeLutLine( Blue, x2, y2 );
            x2.clear();
            y2.clear();
        }

        // sample points 
        QVector<double> x5( x.count() );
        QVector<double> y5( y.count() );
        for ( int i=0; i<x.count(); i ++ )
        {
            x5[i] = ((double)x[i]) / (double)(1 << m_bit_width);
            y5[i] = ((double)y[i]) / (double)(1 << m_bit_width);
        }
        plot->graph( SAMPLE_CURVE_ID )->setData( x5, y5 );

        plot->replot();
    }

    void highlightSamples( LutChannel ch, QVector<int> &x, QVector<int> &y )
    {
        QCustomPlot * plot = getLutPlot( ch );

        plot->graph( HIGHLIGHT_CURVE_ID )->clearData();

        // sample points 
        QVector<double> x2( x.count() );
        QVector<double> y2( y.count() );
        for ( int i=0; i<x.count(); i ++ )
        {
            x2[i] = ((double)x[i]) / (double)(1 << m_bit_width);
            y2[i] = ((double)y[i]) / (double)(1 << m_bit_width);
        }

        plot->graph( HIGHLIGHT_CURVE_ID )->setData( x2, y2 );

        plot->replot();
    }

    void drawFastGammaPlot( LutChannel ch, int gamma_int )
    {
        float kink;
        float lcontrast;
        float lbrightness;
        float contrast;
        float gamma;
        float brightness;

        float x_i, y_i;
        float inverse_gamma;

        QVector<int> x;
        QVector<int> y;

        // I. Calculate intersection of linear section and gamma curve
        /* Note:
         * The gamma curve has a linear part at the beginning, because otherwise the curve
         * would not go trough the point of origin. The linear function will be called g(x)
         * while the gamma function will be called f(x) in the following text. They are
         * described as follows:
         *
         *    g(x) = lcontrast * x + lbrightness
         * I) f(x) = contrast * x^(1/gamma) + brightness
         *
         * Since we always want the the linear part to go through the point of origin we set:
         *
         * lbrightness = 0
         *
         * The point of the intersection is defined by x_i = kink and y_i. Since it goes
         * trough the origin the linear part can now also be described as:
         *
         * II) g(x) = y_i/x_i * x
         *
         * To calculate the intersection point (x_i|y_i) we calculate the derivatives of g(x)
         * and f(x). Both have to be equal in the intersection point. This way we find the
         * point where the curve points towards the origin.
         *
         * g'(x) = y_i/x_i
         * f'(x) = contrast/g * x^((1/gamma) - 1)
         *
         * Setting those equal and solving for x_i results in:
         *
         * III) x_i = (y_i * gamma / contrast)^gamma
         *
         * Which still includes y_i as an unknown.
         * Inserting III) into I) and solving for y_i results in:
         *
         * IV) y_i = brightness / (1-gamma)
         *
         * Inserting IV) into III) gives us the formula for x_i:
         *
         * V) x_i = ((brightness / (1-gamma) * gamma) / contrast)^gamma
         *
         * x_i and y_i can now be calculated for any contrast brightness and gamma. Please
         * note that for some settings, there will be no solution to the formula. Which means
         * those input values are illegal.
         *
         * Here we fix contrast and brightness of the curve to the REC.709 defaults:
         *
         * contrast = 1.099
         * brightness = -0.099
         */

        // convert gamma to float
        gamma = TO_FLOAT( gamma_int );

        // contrast and brightness are fixed to REC.709 defaults
        contrast = TO_FLOAT( REC709_CONTRAST );
        brightness = TO_FLOAT( REC709_BRIGHTNESS );

        // calculate intersection point of linear part and gamma curve (see note above)
        y_i = brightness / ( 1.0f - gamma);
        x_i = powf(((y_i * gamma) / contrast), gamma);

        // kink is x_i
        kink = x_i;

        // calculate linear contrast
        lcontrast = y_i / x_i;

        // linear brightness is fixed to 0
        lbrightness = 0.0f;

        // compute inverse gamma
        inverse_gamma = 1.0f / gamma;

        // II. Calculate samples
        int const bit_width = m_bit_width;
        int const num_samples = 128;
        int i;
        for ( i = 0; i < (1 << bit_width); i += (1 << bit_width) / num_samples )
        {
            x.append( i );
            y.append( sm_gamma_float(i, kink, lcontrast, lbrightness, contrast, inverse_gamma, brightness, bit_width, bit_width) );

            // Add a sample at the intersection of linear part and gamma curve
            int x_i_int = (int)(x_i * (1 << bit_width));
            if ( x_i_int > i && x_i_int < (i + (1 << bit_width) / num_samples ) )
            {
                x.append( x_i_int );
                y.append( sm_gamma_float(x_i_int, kink, lcontrast, lbrightness, contrast, inverse_gamma, brightness, bit_width, bit_width) );
            }
        }

        // III. Draw the lut line
        computeLutLine( ch, x, y );
    }

    void drawFixedModePlot( LutChannel ch, int mode )
    {
        QVector<int> x;
        QVector<int> y;

        // Set constants for Rec. 709 curve calculation
        float const kink = TO_FLOAT( REC709_THRESHOLD );
        float const lcontrast = TO_FLOAT( REC709_LINEAR_CONTRAST );
        float const lbrightness = TO_FLOAT( REC709_LINEAR_BRIGHTNESS );
        float const contrast = TO_FLOAT( REC709_CONTRAST );
        float const inverse_gamma = 1.0f / TO_FLOAT( REC709_GAMMA );
        float const brightness = TO_FLOAT( REC709_BRIGHTNESS );

        // I. Calculate samples
        int const bit_width = m_bit_width;
        int const num_samples = 512;
        int i;
        for ( i = 0; i < (1 << bit_width); i += (1 << bit_width) / num_samples )
        {
            x.append( i );
            switch ( mode )
            {
            case LUT_FIXED_REC709: // Rec. 709
                y.append( sm_gamma_float(i, kink, lcontrast, lbrightness, contrast, inverse_gamma, brightness, bit_width, bit_width) );
                break;
            case LUT_FIXED_PQ: // Rec. 2100 - PQ
                y.append( sm_pq(i, bit_width, bit_width) );
                break;
            case LUT_FIXED_HLG: // Rec. 2100 - HLG
                y.append( sm_hlg(i, bit_width, bit_width) );
                break;
            default:
                return;
            }
        }

        // II. Draw the lut line
        computeLutLine( ch, x, y );
    }

    Ui::UI_LutBox *         m_ui;                           /**< ui handle */
    LutDelegate *           m_delegate;                     /**< delegation class */
    bool                    m_enable;                       /**< enable status */
    int                     m_mode;                         /**< operational mode */
    int                     m_fixed_mode;                   /**< fixed gamma curve mode */
    int                     m_preset;                       /**< current preset */
    LutChannel              m_ch;                           /**< currently selected channel */
    int                     m_bit_width;                    /**< width of the lut module (depends on device) */
    
    CubicInterpolation *    m_interpolate[LutChannelMax];       /**< interpolation class */
    CubicInterpolation *    m_final_interpolate[LutChannelMax]; /**< final interpolation class */
    QStandardItemModel *    m_model[LutChannelMax];             /**< data model */

    QString                 m_filename;
};

/******************************************************************************
 * LutBox::LutBox
 *****************************************************************************/
LutBox::LutBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData( this );

    // Setup fast gamma knob box
    d_data->m_ui->sckbFastGamma->setRange( FAST_GAMMA_MIN, FAST_GAMMA_MAX );
    d_data->m_ui->sckbFastGamma->setCommaPos( FAST_GAMMA_COMMA_POSITION );
    d_data->m_ui->sckbFastGamma->setDisplayMultiplier( FAST_GAMMA_DISPLAY_MULTIPLIER);
    d_data->m_ui->sckbFastGamma->setBase( FAST_GAMMA_BASE );
    d_data->m_ui->sckbFastGamma->setMaxAngle( 180 );
    d_data->m_ui->sckbFastGamma->setMaxRounds( 3 );
    d_data->m_ui->sckbFastGamma->setFmt( FAST_GAMMA_DISPLAY_MASK );
    d_data->m_ui->sckbFastGamma->setMaxEvent( 2 );

    // connect internal signals

    // lut operational mode
    connect( d_data->m_ui->rbInterpolate, SIGNAL(clicked(bool)), this, SLOT(onLutModeInterpolateClicked(bool)));
    connect( d_data->m_ui->rbFastGamma, SIGNAL(clicked(bool)), this, SLOT(onLutModeFastGammaClicked(bool)));
    connect( d_data->m_ui->rbFixedRec709, SIGNAL(clicked(bool)), this, SLOT(onLutFixedModeRec709Clicked(bool)));
    connect( d_data->m_ui->rbFixedPQ, SIGNAL(clicked(bool)), this, SLOT(onLutFixedModePQClicked(bool)));
    connect( d_data->m_ui->rbFixedHLG, SIGNAL(clicked(bool)), this, SLOT(onLutFixedModeHLGClicked(bool)));

    // lut plot tab-widget
    connect( d_data->m_ui->tabColorSelect, SIGNAL(currentChanged(int)), this, SLOT(onColorSelectChange(int)) );
    connect( d_data->m_ui->LutPlot->xAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onMasterXRangeChanged(QCPRange)));
    connect( d_data->m_ui->LutPlot->yAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onMasterYRangeChanged(QCPRange)));
    connect( d_data->m_ui->LutPlotRed->xAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onRedXRangeChanged(QCPRange)));
    connect( d_data->m_ui->LutPlotRed->yAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onRedYRangeChanged(QCPRange)));
    connect( d_data->m_ui->LutPlotGreen->xAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onGreenXRangeChanged(QCPRange)));
    connect( d_data->m_ui->LutPlotGreen->yAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onGreenYRangeChanged(QCPRange)));
    connect( d_data->m_ui->LutPlotBlue->xAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onBlueXRangeChanged(QCPRange)));
    connect( d_data->m_ui->LutPlotBlue->yAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onBlueYRangeChanged(QCPRange)));
    
    // lut storage select
    connect( d_data->m_ui->grpPreset, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(onLutPresetClicked(QAbstractButton *)) );

    // sample 
    connect( d_data->m_ui->btnRemoveSample, SIGNAL(clicked()), this, SLOT(onRemoveSampleClicked()) );
    connect( d_data->m_ui->btnReset, SIGNAL(clicked()), this, SLOT(onResetClicked()) );
    connect( d_data->m_ui->btnInterpolate, SIGNAL(clicked()), this, SLOT(onInterpolateClicked()) );
    connect( d_data->m_ui->btnImport, SIGNAL(clicked()), this, SLOT(onImportClicked()) );
    connect( d_data->m_ui->btnExport, SIGNAL(clicked()), this, SLOT(onExportClicked()) );

    connect( d_data->m_model[Master], SIGNAL(dataChanged(const QModelIndex&,const QModelIndex&)), this, SLOT(onMasterSampleChanged(const QModelIndex&,const QModelIndex&)) );
    connect( d_data->m_model[Red]   , SIGNAL(dataChanged(const QModelIndex&,const QModelIndex&)), this, SLOT(onRedSampleChanged(const QModelIndex&,const QModelIndex&)) );
    connect( d_data->m_model[Green] , SIGNAL(dataChanged(const QModelIndex&,const QModelIndex&)), this, SLOT(onGreenSampleChanged(const QModelIndex&,const QModelIndex&)) );
    connect( d_data->m_model[Blue]  , SIGNAL(dataChanged(const QModelIndex&,const QModelIndex&)), this, SLOT(onBlueSampleChanged(const QModelIndex&,const QModelIndex&)) );

    connect( d_data->m_model[Master], SIGNAL(rowsRemoved(const QModelIndex&,int,int)), this, SLOT(onMasterSampleRemoved(const QModelIndex&,int,int)) );
    connect( d_data->m_model[Red]   , SIGNAL(rowsRemoved(const QModelIndex&,int,int)), this, SLOT(onRedSampleRemoved(const QModelIndex&,int,int)) );
    connect( d_data->m_model[Green] , SIGNAL(rowsRemoved(const QModelIndex&,int,int)), this, SLOT(onGreenSampleRemoved(const QModelIndex&,int,int)) );
    connect( d_data->m_model[Blue]  , SIGNAL(rowsRemoved(const QModelIndex&,int,int)), this, SLOT(onBlueSampleRemoved(const QModelIndex&,int,int)) );
    
    connect( d_data->m_ui->LutPlot      , SIGNAL(mousePress(QMouseEvent*)), this, SLOT(onMasterPlotClicked(QMouseEvent*)) );
    connect( d_data->m_ui->LutPlotRed   , SIGNAL(mousePress(QMouseEvent*)), this, SLOT(onRedPlotClicked(QMouseEvent*)) );
    connect( d_data->m_ui->LutPlotGreen , SIGNAL(mousePress(QMouseEvent*)), this, SLOT(onGreenPlotClicked(QMouseEvent*)) );
    connect( d_data->m_ui->LutPlotBlue  , SIGNAL(mousePress(QMouseEvent*)), this, SLOT(onBluePlotClicked(QMouseEvent*)) );

    connect( d_data->m_ui->tblSamples->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
             this, SLOT(onSelectionChange(const QItemSelection &, const QItemSelection &)) );

    // REC.709, ignore value, it's directly read from component
    connect( d_data->m_ui->btnDefaultRec709, SIGNAL(clicked()), this, SLOT(onDefaultRec709Clicked()));
    connect( d_data->m_ui->btnSetRec709 , SIGNAL(clicked()), this, SLOT(onSetRec709Clicked()));

    // lut fast gamma
    connect( d_data->m_ui->sckbFastGamma, SIGNAL(ValueChanged(int)), this, SLOT(onFastGammaChanged(int)) );
    connect( d_data->m_ui->btnDefaultFastGamma, SIGNAL(clicked()), this, SLOT(onDefaultFastGammaClicked()) );

    // draw initial (master) lut line
    QVector<int> x;
    QVector<int> y;

    d_data->getDataFromModel( Master, x, y  );
    d_data->setSamples( Master, x, y );
}

/******************************************************************************
 * LutBox::~LutBox
 *****************************************************************************/
LutBox::~LutBox()
{
    delete d_data;
}

/******************************************************************************
 * LutBox::LutEnable
 *****************************************************************************/
bool LutBox::LutEnable() const
{
    return ( d_data->m_enable );
}

/******************************************************************************
 * LutBox::setLutEnable
 *****************************************************************************/
void LutBox::setLutEnable( const bool value )
{
    d_data->m_enable = value;
}

/******************************************************************************
 * LutBox::LutMode
 *****************************************************************************/
int LutBox::LutMode() const
{
    return ( d_data->m_mode );
}

/******************************************************************************
 * LutBox::setLutMode
 *****************************************************************************/
void LutBox::setLutMode( const int mode )
{
    // Setup lut mode
    onLutModeChange( mode );

    // Emit lut mode changed signal
    emit LutModeChanged( mode );
}

/******************************************************************************
 * LutBox::LutFixedMode
 *****************************************************************************/
int LutBox::LutFixedMode() const
{
    return ( d_data->m_fixed_mode );
}

/******************************************************************************
 * LutBox::setLutFixedMode
 *****************************************************************************/
void LutBox::setLutFixedMode( const int mode )
{
    // Setup lut mode
    onLutFixedModeChange( mode );

    // Emit lut mode changed signal
    emit LutFixedModeChanged( mode );
}

/******************************************************************************
 * LutBox::LutPresetStorage
 *****************************************************************************/
int LutBox::LutPresetStorage() const
{
    return ( d_data->m_preset );
}

/******************************************************************************
 * LutBox::LutPresetStorage
 *****************************************************************************/
void LutBox::setLutPresetStorage( const int value )
{
    // Set preset
    onLutPresetChange( value );

    // Emit lut preset changed signal
    emit LutPresetChanged( value );
}

/******************************************************************************
 * LutBox::LutPresetStorage
 *****************************************************************************/
int LutBox::LutFastGamma() const
{
    return ( d_data->m_ui->sckbFastGamma->value() );
}

/******************************************************************************
 * LutBox::setLutFastGamma
 *****************************************************************************/
void LutBox::setLutFastGamma( const int gamma )
{
    // Set Value of knob box
    onLutFastGammaChange( gamma );

    // Emit fast gamma value changed signal
    emit LutFastGammaChanged( gamma );
}

/******************************************************************************
 * LutBox::LutBitWidth
 *****************************************************************************/
int LutBox::LutBitWidth() const
{
    return ( d_data->m_bit_width );
}

/******************************************************************************
 * LutBox::setLutBidWidth
 *****************************************************************************/
void LutBox::setLutBitWidth( const int width )
{
    d_data->m_bit_width = width;

    // Reinitialize data model with new bit width
    d_data->initDataModel( Master );
    d_data->initDataModel( Red );
    d_data->initDataModel( Green );
    d_data->initDataModel( Blue );

    // Create new item delegate for the table (so that it accapts the new ranges)
    delete d_data->m_delegate;
    d_data->m_delegate = new LutDelegate( 0, ((1 << width) - 1));
    d_data->m_ui->tblSamples->setItemDelegate( d_data->m_delegate );
    d_data->setDataModel( d_data->m_ch );
}

/******************************************************************************
 * LutBox::prepareMode
 *****************************************************************************/
void LutBox::prepareMode( const Mode )
{
    // @TODO do nothing here
}

/******************************************************************************
 * LutBox::loadSettings
 *****************************************************************************/
void LutBox::loadSettings( QSettings & s )
{
    QVector<int> x;
    QVector<int> y;
    
    s.beginGroup( LUT_SETTINGS_SECTION_NAME);

    // set lut enable
    setLutEnable( s.value( LUT_SETTINGS_ENABLE).toBool() );

    // set lut mode
    setLutMode( s.value( LUT_SETTINGS_MODE).toInt() );

    // set fixed mode
    setLutFixedMode( s.value( LUT_SETTINGS_FIXED_MODE).toInt() );

    // set lut fast gamma
    setLutFastGamma( s.value( LUT_SETTINGS_FAST_GAMMA).toInt() );

    // get lut preset number and set it
    int presetStorage = s.value( LUT_SETTINGS_STORAGE).toInt();
    setLutPresetStorage( presetStorage );

    // disable updates to the view, otherwise the user can see the iterations
    d_data->m_ui->LutPlot->setUpdatesEnabled( false );
    d_data->m_ui->LutPlotRed->setUpdatesEnabled( false );
    d_data->m_ui->LutPlotGreen->setUpdatesEnabled( false );
    d_data->m_ui->LutPlotBlue->setUpdatesEnabled( false );
    d_data->m_ui->tblSamples->setUpdatesEnabled( false );

    // load all presets
    for ( int preset = 0; preset < LUT_NO_PRESETS; preset++ )
    {
        // switch camera to preset
        emit LutPresetChanged( preset );

        // load preset from file
        for ( int ch = Master; ch < LutChannelMax; ch++ )
        {
            QString name_x;
            QString name_y;

            switch ( ch )
            {
                case Red:
                    name_x = QString( "%1_%2" ).arg( LUT_SETTINGS_SAMPLE_RED_X).arg( preset );
                    name_y = QString( "%1_%2" ).arg( LUT_SETTINGS_SAMPLE_RED_Y).arg( preset );
                    break;

                case Green:
                    name_x = QString( "%1_%2" ).arg( LUT_SETTINGS_SAMPLE_GREEN_X).arg( preset );;
                    name_y = QString( "%1_%2" ).arg( LUT_SETTINGS_SAMPLE_GREEN_Y).arg( preset );;
                    break;

                case Blue:
                    name_x = QString( "%1_%2" ).arg( LUT_SETTINGS_SAMPLE_BLUE_X).arg( preset );;
                    name_y = QString( "%1_%2" ).arg( LUT_SETTINGS_SAMPLE_BLUE_Y).arg( preset );;
                    break;

                default:
                    name_x = QString( "%1_%2" ).arg( LUT_SETTINGS_SAMPLE_X).arg( preset );;
                    name_y = QString( "%1_%2" ).arg( LUT_SETTINGS_SAMPLE_Y).arg( preset );;
                    break;
            }

            // @TODO: load list from readable text
            // NOTE: call qRegisterMetaTypeStreamOperators<QList<int> >("QList<int>");
            x = QVector<int>::fromList( s.value(name_x).value<QList<int> >() );
            y = QVector<int>::fromList( s.value(name_y).value<QList<int> >() );

            // Transfer the new settings to the device
            switch ( ch )
            {
                case Red:
                    emit LutSampleValuesRedChanged( x,y );
                    break;

                case Green:
                    emit LutSampleValuesGreenChanged( x,y );
                    break;

                case Blue:
                    emit LutSampleValuesBlueChanged( x,y );
                    break;

                default:
                    emit LutSampleValuesMasterChanged( x,y );
                    break;
            }
        }
    }

    // Renable updates
    d_data->m_ui->LutPlot->setUpdatesEnabled( true );
    d_data->m_ui->LutPlotRed->setUpdatesEnabled( true );
    d_data->m_ui->LutPlotGreen->setUpdatesEnabled( true );
    d_data->m_ui->LutPlotBlue->setUpdatesEnabled( true );
    d_data->m_ui->tblSamples->setUpdatesEnabled( true );

    // switch to loaded preset storage and interpolate to apply lut on device
    emit LutPresetChanged( presetStorage );
    emit LutInterpolateChanged();
    emit LutInterpolateRedChanged();
    emit LutInterpolateGreenChanged();
    emit LutInterpolateBlueChanged();

    // REC.709 settings
    d_data->m_ui->sbxRec709LinContrast->setValue( s.value(LUT_SETTINGS_REC709_LCONTRAST).toDouble() );
    d_data->m_ui->sbxRec709LinOffset  ->setValue( s.value(LUT_SETTINGS_REC709_LBRIGHTNESS).toDouble() );
    d_data->m_ui->sbxRec709Threshold  ->setValue( s.value(LUT_SETTINGS_REC709_THRESHOLD).toDouble() );
    d_data->m_ui->sbxRec709Contrast   ->setValue( s.value(LUT_SETTINGS_REC709_CONTRAST).toDouble() );
    d_data->m_ui->sbxRec709Gamma      ->setValue( s.value(LUT_SETTINGS_REC709_GAMMA).toDouble() );
    d_data->m_ui->sbxRec709Offset     ->setValue( s.value(LUT_SETTINGS_REC709_BRIGHTNESS).toDouble() );

    s.endGroup();
}

/******************************************************************************
 * LutBox::saveSettings
 *****************************************************************************/
void LutBox::saveSettings( QSettings & s )
{
    QList<int> x;
    QList<int> y;

    // before storing, get current preset storage number, because it will be iterated in the save loop
    int currentPreset = LutPresetStorage();

    s.beginGroup( LUT_SETTINGS_SECTION_NAME );

    // Store general lut settings
    s.setValue( LUT_SETTINGS_ENABLE    , LutEnable() );
    s.setValue( LUT_SETTINGS_MODE      , LutMode() );
    s.setValue( LUT_SETTINGS_FIXED_MODE, LutFixedMode() );
    s.setValue( LUT_SETTINGS_STORAGE   , LutPresetStorage() );
    s.setValue( LUT_SETTINGS_FAST_GAMMA, LutFastGamma() );

    // disable updates to the view, otherwise the user can see the iterations
    d_data->m_ui->LutPlot->setUpdatesEnabled( false );
    d_data->m_ui->LutPlotRed->setUpdatesEnabled( false );
    d_data->m_ui->LutPlotGreen->setUpdatesEnabled( false );
    d_data->m_ui->LutPlotBlue->setUpdatesEnabled( false );
    d_data->m_ui->tblSamples->setUpdatesEnabled( false );

    // Store all lut tables
    for ( int preset = 0; preset < LUT_NO_PRESETS; preset++ )
    {
        // trigger event that loads the table from the device
        emit LutPresetChanged( preset );

        // store preset in file
        for ( int ch = Master; ch < LutChannelMax; ch++ )
        {
            QString name_x;
            QString name_y;

            switch ( ch )
            {
                case Red:
                    name_x = QString( "%1_%2" ).arg( LUT_SETTINGS_SAMPLE_RED_X).arg( preset );
                    name_y = QString( "%1_%2" ).arg( LUT_SETTINGS_SAMPLE_RED_Y).arg( preset );
                    break;

                case Green:
                    name_x = QString( "%1_%2" ).arg( LUT_SETTINGS_SAMPLE_GREEN_X).arg( preset );;
                    name_y = QString( "%1_%2" ).arg( LUT_SETTINGS_SAMPLE_GREEN_Y).arg( preset );;
                    break;

                case Blue:
                    name_x = QString( "%1_%2" ).arg( LUT_SETTINGS_SAMPLE_BLUE_X).arg( preset );;
                    name_y = QString( "%1_%2" ).arg( LUT_SETTINGS_SAMPLE_BLUE_Y).arg( preset );;
                    break;

                default:
                    name_x = QString( "%1_%2" ).arg( LUT_SETTINGS_SAMPLE_X).arg( preset );;
                    name_y = QString( "%1_%2" ).arg( LUT_SETTINGS_SAMPLE_Y).arg( preset );;
                    break;
            }

            d_data->getDataFromModel( (LutChannel)ch, x, y );
            // @TODO: save list as readable text
            // NOTE: call qRegisterMetaTypeStreamOperators<QList<int> >("QList<int>");
            s.setValue( name_x, QVariant::fromValue<QList<int>>(x) );
            s.setValue( name_y, QVariant::fromValue<QList<int>>(y) );
        }
    }

    // REC.709 settings
    s.setValue( LUT_SETTINGS_REC709_LCONTRAST  , d_data->m_ui->sbxRec709LinContrast->value() );
    s.setValue( LUT_SETTINGS_REC709_LBRIGHTNESS, d_data->m_ui->sbxRec709LinOffset  ->value() );
    s.setValue( LUT_SETTINGS_REC709_THRESHOLD  , d_data->m_ui->sbxRec709Threshold  ->value() );
    s.setValue( LUT_SETTINGS_REC709_CONTRAST   , d_data->m_ui->sbxRec709Contrast   ->value() );
    s.setValue( LUT_SETTINGS_REC709_GAMMA      , d_data->m_ui->sbxRec709Gamma      ->value() );
    s.setValue( LUT_SETTINGS_REC709_BRIGHTNESS , d_data->m_ui->sbxRec709Offset     ->value() );

    s.endGroup();

    // Switch back to original preset
    emit LutPresetChanged( currentPreset );

    // Renable updates
    d_data->m_ui->LutPlot->setUpdatesEnabled( true );
    d_data->m_ui->LutPlotRed->setUpdatesEnabled( true );
    d_data->m_ui->LutPlotGreen->setUpdatesEnabled( true );
    d_data->m_ui->LutPlotBlue->setUpdatesEnabled( true );
    d_data->m_ui->tblSamples->setUpdatesEnabled( true );
}

/******************************************************************************
 * LutBox::applySettings
 *****************************************************************************/
void LutBox::applySettings( void )
{
    QVector<int> x;
    QVector<int> y;

    // general lut settins
    emit LutModeChanged( LutMode() );
    emit LutFixedModeChanged( LutFixedMode() );
    emit LutFastGammaChanged( LutFastGamma() );

    // disable updates to the view, otherwise the user can see the iterations
    d_data->m_ui->LutPlot->setUpdatesEnabled( false );
    d_data->m_ui->LutPlotRed->setUpdatesEnabled( false );
    d_data->m_ui->LutPlotGreen->setUpdatesEnabled( false );
    d_data->m_ui->LutPlotBlue->setUpdatesEnabled( false );
    d_data->m_ui->tblSamples->setUpdatesEnabled( false );

    // apply all presets
    for ( int preset = 0; preset < LUT_NO_PRESETS; preset++ )
    {
        // switch camera to preset
        emit LutPresetChanged( preset );

        QVector<int> x;
        QVector<int> y;

        // transmit tables for all channels
        for ( int ch = Master; ch < LutChannelMax; ch++ )
        {
            d_data->getDataFromModel( (LutChannel)ch, x, y );

            // Transfer the settings to the device
            switch ( ch )
            {
                case Red:
                    emit LutSampleValuesRedChanged( x,y );
                    break;

                case Green:
                    emit LutSampleValuesGreenChanged( x,y );
                    break;

                case Blue:
                    emit LutSampleValuesBlueChanged( x,y );
                    break;

                default:
                    emit LutSampleValuesMasterChanged( x,y );
                    break;
            }
        }
    }

    // Renable updates
    d_data->m_ui->LutPlot->setUpdatesEnabled( true );
    d_data->m_ui->LutPlotRed->setUpdatesEnabled( true );
    d_data->m_ui->LutPlotGreen->setUpdatesEnabled( true );
    d_data->m_ui->LutPlotBlue->setUpdatesEnabled( true );
    d_data->m_ui->tblSamples->setUpdatesEnabled( true );

    // switch back to current preset storage and interpolate to apply lut on device
    emit LutPresetChanged( LutPresetStorage() );
    emit LutInterpolateChanged();
    emit LutInterpolateRedChanged();
    emit LutInterpolateGreenChanged();
    emit LutInterpolateBlueChanged();
}

/******************************************************************************
 * LutBox::loadProfile (only load a sample table to current sample table)
 *****************************************************************************/
void LutBox::loadProfile( QSettings & s )
{
    QVector<int> x;
    QVector<int> y;
    
    LutChannel ch = (LutChannel)d_data->m_ui->tabColorSelect->currentIndex();

    s.beginGroup( LUT_SETTINGS_SECTION_NAME);
    x = QVector<int>::fromList( s.value(LUT_SETTINGS_SAMPLE_X).value<QList<int> >() );
    y = QVector<int>::fromList( s.value(LUT_SETTINGS_SAMPLE_Y).value<QList<int> >() );
    d_data->setSamples( ch, x, y );
    s.endGroup();
}

/******************************************************************************
 * LutBox::saveSettings (only save the current sample table)
 *****************************************************************************/
void LutBox::saveProfile( QSettings & s )
{
    QList<int> x;
    QList<int> y;

    LutChannel ch = (LutChannel)d_data->m_ui->tabColorSelect->currentIndex();
    d_data->getDataFromModel( ch, x, y );

    s.beginGroup( LUT_SETTINGS_SECTION_NAME );
    s.setValue( LUT_SETTINGS_SAMPLE_X, QVariant::fromValue<QList<int>>(x) );
    s.setValue( LUT_SETTINGS_SAMPLE_Y, QVariant::fromValue<QList<int>>(y) );
    s.endGroup();
}

/******************************************************************************
 * LutBox::onLutEnableChange
 *****************************************************************************/
void LutBox::onLutEnableChange( int, int value )
{
    d_data->m_enable = value ? true : false;
}

/******************************************************************************
 * LutBox::onLutModeChange
 *****************************************************************************/
void LutBox::onLutModeChange( int mode )
{
    // Check if mode got changed
    if ( mode == LutMode() )
    {
        return;
    }

    // Block signals to ui elements
    d_data->m_ui->rbInterpolate->blockSignals( true );
    d_data->m_ui->rbFastGamma->blockSignals( true );
    d_data->m_ui->rbFixedRec709->blockSignals( true );
    d_data->m_ui->rbFixedPQ->blockSignals( true );
    d_data->m_ui->rbFixedHLG->blockSignals( true );

    // Get Master lut plot
    QCustomPlot * masterPlot = d_data->getLutPlot( Master );

    // If mode = interpolate, activate table based lut configuration
    if ( mode == LUT_MODE_INTERPOLATE )
    {
        // Enable table based ui elements, add RGB plots
        d_data->m_ui->rbInterpolate->setChecked( true );
        d_data->m_ui->gbxPreset->setEnabled( true );
        d_data->m_ui->gbxSamplePoints->setEnabled( true );
        d_data->m_ui->gbxDefaults->setEnabled( true );
        d_data->m_ui->LutPlot->setEnabled( true );
        d_data->m_ui->tabColorSelect->addTab( d_data->m_ui->tabRedChannel, QIcon(":/images/tab/red.png"), "Red" );
        d_data->m_ui->tabColorSelect->addTab( d_data->m_ui->tabGreenChannel, QIcon(":/images/tab/green.png"), "Green" );
        d_data->m_ui->tabColorSelect->addTab( d_data->m_ui->tabBlueChannel, QIcon(":/images/tab/blue.png"), "Blue" );

        // Use scatter symbols in interpolation mode
        masterPlot->graph( SAMPLE_CURVE_ID )->setScatterStyle( QCPScatterStyle(QCPScatterStyle::ssCircle, 4 ) );

        // Disable fast gamma ui elements
        d_data->m_ui->rbFastGamma->setChecked( false );
        d_data->m_ui->sckbFastGamma->setEnabled( false );
        d_data->m_ui->btnDefaultFastGamma->setEnabled( false );

        // Uncheck fixed mode radio buttons
        d_data->m_ui->rbFixedRec709->setChecked( false );
        d_data->m_ui->rbFixedPQ->setChecked( false );
        d_data->m_ui->rbFixedHLG->setChecked( false );
    }

    // Else if mode = fast gamma or fixed curve, activate simple configuration
    else if ( mode == LUT_MODE_FAST_GAMMA || mode == LUT_MODE_FIXED )
    {
        // Disable table based ui elements, remove RGB plots (show only master plot)
        d_data->m_ui->rbInterpolate->setChecked( false );
        d_data->m_ui->gbxPreset->setEnabled( false );
        d_data->m_ui->gbxSamplePoints->setEnabled( false );
        d_data->m_ui->gbxDefaults->setEnabled( false );
        d_data->m_ui->tabColorSelect->setCurrentIndex( Master );
        d_data->m_ui->LutPlot->setEnabled( false );
        d_data->m_ui->tabColorSelect->removeTab( Blue );
        d_data->m_ui->tabColorSelect->removeTab( Green );
        d_data->m_ui->tabColorSelect->removeTab( Red );

        // Do not use scatter symbols in fat gamma or fixed mode
        masterPlot->graph( SAMPLE_CURVE_ID )->setScatterStyle( QCPScatterStyle(QCPScatterStyle::ssNone ) );

        // If mode = fast gamma, enable fast gamma UI elements
        if ( mode == LUT_MODE_FAST_GAMMA )
        {
            // Uncheck fixed mode radio buttons
            d_data->m_ui->rbFixedRec709->setChecked( false );
            d_data->m_ui->rbFixedPQ->setChecked( false );
            d_data->m_ui->rbFixedHLG->setChecked( false );

            // Enable fast gamma ui elements
            d_data->m_ui->rbFastGamma->setChecked( true );
            d_data->m_ui->sckbFastGamma->setEnabled( true );
            d_data->m_ui->btnDefaultFastGamma->setEnabled( true );

            // Draw the fast gamma curve in the master plot
            d_data->drawFastGammaPlot( Master, LutFastGamma() );
        }

        // If mode = fixed gamma, disable fast gamma UI elements
        if ( mode == LUT_MODE_FIXED )
        {
            // Disable fast gamma ui elements
            d_data->m_ui->rbFastGamma->setChecked( false );
            d_data->m_ui->sckbFastGamma->setEnabled( false );
            d_data->m_ui->btnDefaultFastGamma->setEnabled( false );

            /* Note: We check the correct radio button in onLutFixedModeChange() */
        }
    }

    // Enable signals to ui elements
    d_data->m_ui->rbInterpolate->blockSignals( false );
    d_data->m_ui->rbFastGamma->blockSignals( false );
    d_data->m_ui->rbFixedRec709->blockSignals( false );
    d_data->m_ui->rbFixedPQ->blockSignals( false );
    d_data->m_ui->rbFixedHLG->blockSignals( false );

    // Store mode
    d_data->m_mode = mode;
}

/******************************************************************************
 * LutBox::onLutFixedModeChange
 *****************************************************************************/
void LutBox::onLutFixedModeChange( int mode )
{
    // Check if lut is in fixed mode
    if ( LutMode() != LUT_MODE_FIXED )
    {
        return;
    }

    // Uncheck all radio boxes
    d_data->m_ui->rbFixedRec709->setChecked( false );
    d_data->m_ui->rbFixedPQ->setChecked( false );
    d_data->m_ui->rbFixedHLG->setChecked( false );

    // Check boxes according to selected mode
    if ( mode == LUT_FIXED_REC709 )
    {
        d_data->m_ui->rbFixedRec709->setChecked( true );
    }
    else if ( mode == LUT_FIXED_PQ )
    {
        d_data->m_ui->rbFixedPQ->setChecked( true );
    }
    else if ( mode == LUT_FIXED_HLG )
    {
        d_data->m_ui->rbFixedHLG->setChecked( true );
    }

    // Store mode
    d_data->m_fixed_mode = mode;

    // Draw the plot
    d_data->drawFixedModePlot( Master, LutFixedMode() );
}

/******************************************************************************
 * LutBox::onLutPresetChange
 *****************************************************************************/
void LutBox::onLutPresetChange( int value )
{
    QCheckBox * cbx = qobject_cast<QCheckBox *>( d_data->m_ui->grpPreset->button( value ) );
    cbx->setChecked( true );
    d_data->m_preset = value;
}

/******************************************************************************
 * LutBox::onRemoveSampleClicked
 *****************************************************************************/
void LutBox::onRemoveSampleClicked()
{
    QItemSelectionModel * select = (QItemSelectionModel *)d_data->m_ui->tblSamples->selectionModel();

    if ( select->hasSelection() && (d_data->m_ui->tblSamples->model()->rowCount() > 2) )
    {
        QModelIndexList list = select->selectedRows();
        // sort so that rows are removed from highest index
        qSort( list.begin(), list.end(), qGreater<QModelIndex>() );
        foreach ( QModelIndex index, list )
        {
            int x = d_data->m_ui->tblSamples->model()->data( index, Qt::DisplayRole ).toInt();
            if ( (index.row() != 0) && (x != ((1 << LutBitWidth()) - 1)) )
            {
                d_data->m_ui->tblSamples->model()->removeRow( index.row() );
            }
        }
    }
}

/******************************************************************************
 * LutBox::onResetClicked
 *****************************************************************************/
void LutBox::onResetClicked()
{
    setWaitCursor();
    switch ( (LutChannel)d_data->m_ui->tabColorSelect->currentIndex() )
    {
        case Red:
            emit LutResetRedChanged();
            break;

        case Green:
            emit LutResetGreenChanged();
            break;
        
        case Blue:
            emit LutResetBlueChanged();
            break;
            
        case Master:
            emit LutResetMasterChanged();
            break;

        default:
            emit LutResetChanged();
            break;
    }
    setNormalCursor();
}

/******************************************************************************
 * LutBox::onInterpolateClicked
 *****************************************************************************/
void LutBox::onInterpolateClicked()
{
    QVector<int> x;
    QVector<int> y;

    LutChannel ch = (LutChannel)d_data->m_ui->tabColorSelect->currentIndex();

    // sort samples and remove X-duplicates (last X wins)
    d_data->getDataFromModel( ch, x, y );

    // apply samples and call interpolate on device
    setWaitCursor();
    switch ( ch )
    {
        case Red:
            emit LutSampleValuesRedChanged( x,y );
            emit LutInterpolateRedChanged();
            break;

        case Green:
            emit LutSampleValuesGreenChanged( x,y );
            emit LutInterpolateGreenChanged();
            break;
        
        case Blue:
            emit LutSampleValuesBlueChanged( x,y );
            emit LutInterpolateBlueChanged();
            break;
            
        case Master:
            emit LutSampleValuesMasterChanged( x,y );
            emit LutInterpolateChanged();
            break;
        
        default:
            emit LutSampleValuesChanged( x,y );
            emit LutInterpolateChanged();
            break;
    }
    setNormalCursor();

    // set reordered and reduced sample list
    d_data->setSamples( ch, x, y );
}

/******************************************************************************
 * LutBox::onImportClicked
 *****************************************************************************/
void LutBox::onImportClicked()
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
            QVector<QPair<int, int>> boundaries( { QPair<int, int>(0, ((1 << LutBitWidth()) - 1)),
                                                   QPair<int, int>(0, ((1 << LutBitWidth()) - 1)) });

            // If no error occurred, paste the data into the table
            if ( loadTableCsv( d_data->m_filename, LUT_TABLE_NO_ROWS, boundaries, table) == 0)
            {
                LutChannel ch = (LutChannel)d_data->m_ui->tabColorSelect->currentIndex();
                d_data->setSamples( ch, table[0], table[1] );
            }
        }
    }
}

/******************************************************************************
 * LutBox::onExportClicked
 *****************************************************************************/
void LutBox::onExportClicked()
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
        LutChannel ch = (LutChannel)d_data->m_ui->tabColorSelect->currentIndex();
        d_data->getDataFromModel( ch, table[0], table[1] );

        // Write to CSV file
        saveTableCsv(d_data->m_filename, table);
    }
}

/******************************************************************************
 * LutBox::SampleChanged
 *****************************************************************************/
void LutBox::SampleChanged( LutChannel ch )
{
    // redraw sample-and interpolated graph
    QVector<int> x;
    QVector<int> y;

    d_data->getDataFromModel( ch, x, y );
    d_data->computeLutLine( ch, x, y );

    // redraw selection graph 
    QItemSelectionModel * select = (QItemSelectionModel *)d_data->m_ui->tblSamples->selectionModel();
    if ( select->hasSelection() && (d_data->m_ui->tblSamples->model()->rowCount() > 2) )
    {
        QModelIndexList list = select->selectedRows();

        QMap<int, int> map;

        // sort so that rows are removed from highest index
        qSort( list.begin(), list.end(), qLess<QModelIndex>() );
        
        foreach ( QModelIndex index, list )
        {
            int x = d_data->m_ui->tblSamples->model()->data( index, Qt::DisplayRole ).toInt();
            int y = d_data->m_ui->tblSamples->model()->data( d_data->m_ui->tblSamples->model()->index(index.row(), 1), Qt::DisplayRole ).toInt();
            map.insert( x, y );
        }

        QList<int> xl = map.keys();
        QList<int> yl = map.values();

        QVector<int> xv = QVector<int>::fromList( xl );
        QVector<int> yv = QVector<int>::fromList( yl );
       
        d_data->highlightSamples( (LutChannel)d_data->m_ui->tabColorSelect->currentIndex(), xv, yv );
    }
}

/******************************************************************************
 * LutBox::onMasterSampleChanged
 *****************************************************************************/
void LutBox::onMasterSampleChanged( const QModelIndex &, const QModelIndex & )
{
    SampleChanged( Master );
}

/******************************************************************************
 * LutBox::onRedSampleChanged
 *****************************************************************************/
void LutBox::onRedSampleChanged( const QModelIndex &, const QModelIndex & )
{
    SampleChanged( Red );
}

/******************************************************************************
 * LutBox::onGreenSampleChanged
 *****************************************************************************/
void LutBox::onGreenSampleChanged( const QModelIndex &, const QModelIndex & )
{
    SampleChanged( Green );
}

/******************************************************************************
 * LutBox::onBlueSampleChanged
 *****************************************************************************/
void LutBox::onBlueSampleChanged( const QModelIndex &, const QModelIndex & )
{
    SampleChanged( Blue );
}

/******************************************************************************
 * LutBox::onMasterSampleRemoved
 *****************************************************************************/
void LutBox::onMasterSampleRemoved( const QModelIndex &, int, int )
{
    SampleChanged( Master );
}

/******************************************************************************
 * LutBox::onRedSampleRemoved
 *****************************************************************************/
void LutBox::onRedSampleRemoved( const QModelIndex &, int, int )
{
    SampleChanged( Red );
}

/******************************************************************************
 * LutBox::onGreenSampleRemoved
 *****************************************************************************/
void LutBox::onGreenSampleRemoved( const QModelIndex &, int, int )
{
    SampleChanged( Green );
}

/******************************************************************************
 * LutBox::onBlueSampleRemoved
 *****************************************************************************/
void LutBox::onBlueSampleRemoved( const QModelIndex &, int, int )
{
    SampleChanged( Blue );
}

/******************************************************************************
 * LutBox::onLutSampleValuesMasterChange
 *****************************************************************************/
void LutBox::onLutSampleValuesMasterChange( QVector<int> x, QVector<int> y )
{
    d_data->setSamples( Master, x, y );
}

/******************************************************************************
 * LutBox::onLutSampleValuesRedChange
 *****************************************************************************/
void LutBox::onLutSampleValuesRedChange( QVector<int> x, QVector<int> y )
{
    d_data->setSamples( Red, x, y );
}

/******************************************************************************
 * LutBox::onLutSampleValuesGreenChange
 *****************************************************************************/
void LutBox::onLutSampleValuesGreenChange( QVector<int> x, QVector<int> y )
{
    d_data->setSamples( Green, x, y );
}

/******************************************************************************
 * LutBox::onLutSampleValuesBlueChange
 *****************************************************************************/
void LutBox::onLutSampleValuesBlueChange( QVector<int> x, QVector<int> y )
{
    d_data->setSamples( Blue, x, y );
}

/******************************************************************************
 * LutBox::onLutFastGammaChange
 *****************************************************************************/
void LutBox::onLutFastGammaChange( int gamma )
{
    // Set Value of knob box
    d_data->m_ui->sckbFastGamma->blockSignals( true );
    d_data->m_ui->sckbFastGamma->setValue( gamma );
    d_data->m_ui->sckbFastGamma->blockSignals( false );

    // If device is in fast gamma mode, display fast gamma plot
    if ( d_data->m_mode == LUT_MODE_FAST_GAMMA )
    {
        // Draw the fast gamma curve in the master plot
        d_data->drawFastGammaPlot( Master, LutFastGamma() );
    }
}

/******************************************************************************
 * LutBox::onLutModeInterpolateClicked
 *****************************************************************************/
void LutBox::onLutModeInterpolateClicked( bool checked )
{
    // Set interpolation mode
    if ( checked )
    {
        setWaitCursor();
        setLutMode( LUT_MODE_INTERPOLATE );
        setNormalCursor();
    }

    // If button is being "unchecked" by clicking on it again, make it checked again
    else
    {
        d_data->m_ui->rbInterpolate->setChecked( true );
    }
}

/******************************************************************************
 * LutBox::onLutModeFastGammaClicked
 *****************************************************************************/
void LutBox::onLutModeFastGammaClicked( bool checked )
{
    // Set fast gamma mode
    if ( checked )
    {
        setWaitCursor();
        setLutMode( LUT_MODE_FAST_GAMMA );
        setNormalCursor();
    }

    // If button is being "unchecked" by clicking on it again, make it checked again
    else
    {
        d_data->m_ui->rbFastGamma->setChecked( true );
    }
}

/******************************************************************************
 * LutBox::onLutFixedModeRec709Clicked
 *****************************************************************************/
void LutBox::onLutFixedModeRec709Clicked( bool checked )
{
    // Set fixed lut mode to Rec. 709
    if ( checked )
    {
        setWaitCursor();
        setLutMode( LUT_MODE_FIXED );
        setLutFixedMode( LUT_FIXED_REC709 );
        setNormalCursor();
    }

    // If button is being "unchecked" by clicking on it again, make it checked again
    else
    {
        d_data->m_ui->rbFixedRec709->setChecked( true );
    }
}

/******************************************************************************
 * LutBox::onLutFixedModePQClicked
 *****************************************************************************/
void LutBox::onLutFixedModePQClicked( bool checked )
{
    // Set fixed lut mode to PQ
    if ( checked )
    {
        setWaitCursor();
        setLutMode( LUT_MODE_FIXED );
        setLutFixedMode( LUT_FIXED_PQ );
        setNormalCursor();
    }

    // If button is being "unchecked" by clicking on it again, make it checked again
    else
    {
        d_data->m_ui->rbFixedPQ->setChecked( true );
    }
}

/******************************************************************************
 * LutBox::onLutFixedModeHLGClicked
 *****************************************************************************/
void LutBox::onLutFixedModeHLGClicked( bool checked )
{
    // Set fixed lut mode to HLG
    if ( checked )
    {
        setWaitCursor();
        setLutMode( LUT_MODE_FIXED );
        setLutFixedMode( LUT_FIXED_HLG );
        setNormalCursor();
    }

    // If button is being "unchecked" by clicking on it again, make it checked again
    else
    {
        d_data->m_ui->rbFixedHLG->setChecked( true );
    }
}

/******************************************************************************
 * LutBox::oonLutPresetClicked
 *****************************************************************************/
void LutBox::onLutPresetClicked( QAbstractButton * btn )
{
    int preset = d_data->m_ui->grpPreset->id( btn );
    if ( d_data->m_preset != preset )
    {
        // Transmit data of current channel to device before preset switch
        QVector<int> x;
        QVector<int> y;

        d_data->getDataFromModel( d_data->m_ch, x, y  );
        switch ( d_data->m_ch )
        {
            case Red:
                emit LutSampleValuesRedChanged( x,y );
                break;

            case Green:
                emit LutSampleValuesGreenChanged( x,y );
                break;

            case Blue:
                emit LutSampleValuesBlueChanged( x,y );
                break;

            case Master:
                emit LutSampleValuesMasterChanged( x,y );
                break;

            default:
                emit LutSampleValuesChanged( x,y );
                break;
        }

        // Switch to new preset
        d_data->m_preset = preset;
        
        setWaitCursor();
        emit LutPresetChanged( preset );
        setNormalCursor();

        /* Repaint table, otherwise it is not refreshed in those cases where
         * we switch between two tables with the same row-count but different data */
        d_data->m_ui->tblSamples->repaint();
    }
}

/******************************************************************************
 * LutBox::onColorSelectChange
 *****************************************************************************/
void LutBox::onColorSelectChange( int value )
{
    if ( ( value >= Master ) && ( value < LutChannelMax ) )
    {
        QVector<int> x;
        QVector<int> y;

        d_data->getDataFromModel( d_data->m_ch, x, y  );
        switch ( d_data->m_ch )
        {
            case Red:
                emit LutSampleValuesRedChanged( x,y );
                break;

            case Green:
                emit LutSampleValuesGreenChanged( x,y );
                break;
        
            case Blue:
                emit LutSampleValuesBlueChanged( x,y );
                break;
            
            case Master:
                emit LutSampleValuesMasterChanged( x,y );
                break;
        
            default:
                emit LutSampleValuesChanged( x,y );
                break;
        }
        
        d_data->setDataModel( (LutChannel)value );

        // old selection model is deleted => create a new connection
        connect( d_data->m_ui->tblSamples->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                 this, SLOT(onSelectionChange(const QItemSelection &, const QItemSelection &)) );

        d_data->m_ch = (LutChannel)value ;
    }
}

/******************************************************************************
 * LutBox::onDefaultRec709Clicked
 *****************************************************************************/
void LutBox::onDefaultRec709Clicked()
{
    d_data->initRec709Default();
}

/******************************************************************************
 * LutBox::onSetRec709Click
 *****************************************************************************/
void LutBox::onSetRec709Clicked()
{
    int threshold   = TO_INT( d_data->m_ui->sbxRec709Threshold->value() );
    int lcontrast   = TO_INT( d_data->m_ui->sbxRec709LinContrast->value() );
    int lbrightness = TO_INT( d_data->m_ui->sbxRec709LinOffset->value() );
    int contrast    = TO_INT( d_data->m_ui->sbxRec709Contrast->value() );
    int gamma       = TO_INT( d_data->m_ui->sbxRec709Gamma->value() );
    int brightness  = TO_INT( d_data->m_ui->sbxRec709Offset->value() );

    setWaitCursor();
    emit LutRec709Changed( threshold, lcontrast, lbrightness, contrast, gamma, brightness );
    emit LutInterpolateChanged();
    setNormalCursor();
}

/******************************************************************************
 * LutBox::onFastGammaChanged
 *****************************************************************************/
void LutBox::onFastGammaChanged( int gamma )
{
    // Draw the fast gamma curve in the master plot
    d_data->drawFastGammaPlot( Master, LutFastGamma() );

    // Emit fast gamma changed signal
    setWaitCursor();
    emit LutFastGammaChanged( gamma );
    setNormalCursor();
}

/******************************************************************************
 * LutBox::onDefaultFastGammaClick
 *****************************************************************************/
void LutBox::onDefaultFastGammaClicked()
{
    setWaitCursor();
    setLutFastGamma( REC709_GAMMA );
    setNormalCursor();
}

/******************************************************************************
 * LutBox::SetXRangeChanged
 *****************************************************************************/
void LutBox::SetXRangeChanged( const QCPRange &range, QCustomPlot * plot )
{
    QCPRange boundedRange = range;
    double lowerRangeBound = 0;
    double upperRangeBound = 1;
    if ( boundedRange.size() > upperRangeBound-lowerRangeBound )
    {
        boundedRange = QCPRange(lowerRangeBound, upperRangeBound);
    }
    else
    {
        double oldSize = boundedRange.size();
        if ( boundedRange.lower < lowerRangeBound )
        {
            boundedRange.lower = lowerRangeBound;
            boundedRange.upper = lowerRangeBound+oldSize;
        }
        if ( boundedRange.upper > upperRangeBound )
        {
            boundedRange.lower = upperRangeBound-oldSize;
            boundedRange.upper = upperRangeBound;
        }
    }

    plot->xAxis->setRange( boundedRange );
}

/******************************************************************************
 * LutBox::SetYRangeChanged
 *****************************************************************************/
void LutBox::SetYRangeChanged( const QCPRange &range, QCustomPlot * plot )
{
    QCPRange boundedRange = range;
    double lowerRangeBound = 0;
    double upperRangeBound = 1;
    if ( boundedRange.size() > upperRangeBound-lowerRangeBound )
    {
        boundedRange = QCPRange(lowerRangeBound, upperRangeBound);
    }
    else
    {
        double oldSize = boundedRange.size();
        if ( boundedRange.lower < lowerRangeBound )
        {
            boundedRange.lower = lowerRangeBound;
            boundedRange.upper = lowerRangeBound+oldSize;
        }
        if ( boundedRange.upper > upperRangeBound )
        {
            boundedRange.lower = upperRangeBound-oldSize;
            boundedRange.upper = upperRangeBound;
        }
    }

    plot->yAxis->setRange( boundedRange );
}

/******************************************************************************
 * LutBox::onMasterXRangeChanged
 *****************************************************************************/
void LutBox::onMasterXRangeChanged( const QCPRange &range )
{
    SetXRangeChanged( range, d_data->m_ui->LutPlot );
}

/******************************************************************************
 * LutBox::onMasterYRangeChanged
 *****************************************************************************/
void LutBox::onMasterYRangeChanged( const QCPRange &range )
{
    SetYRangeChanged( range, d_data->m_ui->LutPlot );
}

/******************************************************************************
 * LutBox::onRedXRangeChanged
 *****************************************************************************/
void LutBox::onRedXRangeChanged( const QCPRange &range )
{
    SetXRangeChanged( range, d_data->m_ui->LutPlotRed );
}

/******************************************************************************
 * LutBox::onRedYRangeChanged
 *****************************************************************************/
void LutBox::onRedYRangeChanged( const QCPRange &range )
{
    SetYRangeChanged( range, d_data->m_ui->LutPlotRed );
}

/******************************************************************************
 * LutBox::onGreenXRangeChanged
 *****************************************************************************/
void LutBox::onGreenXRangeChanged( const QCPRange &range )
{
    SetXRangeChanged( range, d_data->m_ui->LutPlotGreen );
}

/******************************************************************************
 * LutBox::onGreenYRangeChanged
 *****************************************************************************/
void LutBox::onGreenYRangeChanged( const QCPRange &range )
{
    SetYRangeChanged( range, d_data->m_ui->LutPlotGreen );
}

/******************************************************************************
 * LutBox::onBlueXRangeChanged
 *****************************************************************************/
void LutBox::onBlueXRangeChanged( const QCPRange &range )
{
    SetXRangeChanged( range, d_data->m_ui->LutPlotBlue );
}

/******************************************************************************
 * LutBox::onBlueYRangeChanged
 *****************************************************************************/
void LutBox::onBlueYRangeChanged( const QCPRange &range )
{
    SetYRangeChanged( range, d_data->m_ui->LutPlotBlue );
}

/******************************************************************************
 * LutBox::PlotClicked
 *****************************************************************************/
void LutBox::PlotClicked( QMouseEvent * evt, LutChannel ch )
{
    if ( d_data->m_ui->tblSamples->model()->rowCount() < LUT_TABLE_NO_ROWS ) 
    {
        QCustomPlot * plot = d_data->getLutPlot( ch );
        
        // I. Compute new sample coordinates

        // get pixel coordinate
        QPoint p = evt->pos();
        
        // compute coordinates 
        double x = plot->xAxis->pixelToCoord( (double)p.x() );
        double y = plot->yAxis->pixelToCoord( (double)p.y() );

        // range check
        if ( (x < 0.0f) || (x > 1.0f) || (y < 0.0f) || (y>1.0f) )
        {
            // invalid coordinates (due to plot margins) => ignore these clicks
            return;
        }

        // II. Add new sample to List
        
        // sort samples and remove X-duplicates (last X wins)
        QMap<int, int> map;
        int x_, y_;

        for ( int i=0; i<d_data->m_model[ch]->rowCount(); i ++ )
        {
            x_ = d_data->m_model[ch]->data( d_data->m_model[ch]->index(i, 0), Qt::DisplayRole ).toInt();
            y_ = d_data->m_model[ch]->data( d_data->m_model[ch]->index(i, 1), Qt::DisplayRole ).toInt();

            map.insert( x_, y_ );
        }

        map.insert( (int)(x*(1 << LutBitWidth())), (int)(y*(1 << LutBitWidth())) );

        QList<int> xl = map.keys();
        QList<int> yl = map.values();

        QVector<int> xv = QVector<int>::fromList( xl );
        QVector<int> yv = QVector<int>::fromList( yl );

        // III. Overwrite old samples

        // set reordered and reduced sample list
        d_data->setSamples( ch, xv, yv );
    }
}

/******************************************************************************
 * LutBox::onMasterPlotClicked
 *****************************************************************************/
void LutBox::onMasterPlotClicked( QMouseEvent * evt )
{
    PlotClicked( evt, Master );
}

/******************************************************************************
 * LutBox::onRedPlotClicked
 *****************************************************************************/
void LutBox::onRedPlotClicked( QMouseEvent * evt )
{
    PlotClicked( evt, Red );
}

/******************************************************************************
 * LutBox::onGreenPlotClicked
 *****************************************************************************/
void LutBox::onGreenPlotClicked( QMouseEvent * evt )
{
    PlotClicked( evt, Green );
}

/******************************************************************************
 * LutBox::onBluePlotClicked
 *****************************************************************************/
void LutBox::onBluePlotClicked( QMouseEvent * evt )
{
    PlotClicked( evt, Blue );
}

/******************************************************************************
 * LutBox::onSelectionChange
 *****************************************************************************/
void LutBox::onSelectionChange( const QItemSelection &, const QItemSelection & )
{
    QItemSelectionModel * select = (QItemSelectionModel *)d_data->m_ui->tblSamples->selectionModel();

    if ( select->hasSelection() && (d_data->m_ui->tblSamples->model()->rowCount() > 2) )
    {
        QModelIndexList list = select->selectedRows();

        QMap<int, int> map;

        // sort so that rows are removed from highest index
        qSort( list.begin(), list.end(), qLess<QModelIndex>() );
        
        foreach ( QModelIndex index, list )
        {
            int x = d_data->m_ui->tblSamples->model()->data( index, Qt::DisplayRole ).toInt();
            int y = d_data->m_ui->tblSamples->model()->data( d_data->m_ui->tblSamples->model()->index(index.row(), 1), Qt::DisplayRole ).toInt();
            map.insert( x, y );
        }

        QList<int> xl = map.keys();
        QList<int> yl = map.values();

        QVector<int> xv = QVector<int>::fromList( xl );
        QVector<int> yv = QVector<int>::fromList( yl );
        
        d_data->highlightSamples( (LutChannel)d_data->m_ui->tabColorSelect->currentIndex(), xv, yv );
    }
}

