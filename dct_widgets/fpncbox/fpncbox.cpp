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
 * @file    fltbox.cpp
 *
 * @brief   Implementation of filter configuration box
 *
 *****************************************************************************/
#include <QtDebug>
#include <QFileDialog>

#include <FpncData.h>

#include "fpncbox.h"
#include "ui_fpncbox.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_FpncBox;
}

/******************************************************************************
 * local definitions
 *****************************************************************************/
#define FPNC_SAMPLE_X0       (     0 )
#define FPNC_SAMPLE_X1       (   128 )
#define FPNC_SAMPLE_X2       (   256 )
#define FPNC_SAMPLE_X3       (   384 )
#define FPNC_SAMPLE_X4       (   512 )
#define FPNC_SAMPLE_X5       (   768 )
#define FPNC_SAMPLE_X6       (  1024 )
#define FPNC_SAMPLE_X7       (  1536 )
#define FPNC_SAMPLE_X8       (  2048 )
#define FPNC_SAMPLE_X9       (  3072 )
#define FPNC_SAMPLE_X10      (  4096 )
#define FPNC_SAMPLE_X11      (  6144 )
#define FPNC_SAMPLE_X12      (  8192 )
#define FPNC_SAMPLE_X13      ( 16384 )
#define FPNC_SAMPLE_X14      ( 32768 )
#define FPNC_SAMPLE_X15      ( 65536 )
#define FPNC_SAMPLE_X_SCALE  ( 65536 )

#define SAMPLE(x)            ( ((double)x)/FPNC_SAMPLE_X_SCALE )

/******************************************************************************
 * curve names/identifiers
 *****************************************************************************/
#define CURVE_EVEN_LINES_ID         ( 0 )    // curve identifier for even lines
#define CURVE_ODD_LINES_ID          ( 1 )    // curve identifier for odd lines

#define CURVE_EVEN_LINES_LOADED_ID  ( 2 )    // curve identifier for loaded values in even lines
#define CURVE_ODD_LINES_LOADED_ID   ( 3 )    // curve identifier for loaded values in odd lines

/******************************************************************************
 * load/save definitions
 *****************************************************************************/
#define FPNC_SETTINGS_SECTION_NAME          ( "FPNC" )

#define FPNC_SETTINGS_FILENAME_0            ( "file-0" )
#define FPNC_SETTINGS_FILENAME_1            ( "file-1" )

#define FPNC_SETTINGS_RED_INVERSE_GAIN      ( "inv-red" )
#define FPNC_SETTINGS_GREEN_INVERSE_GAIN    ( "inv-green" )
#define FPNC_SETTINGS_BLUE_INVERSE_GAIN     ( "inv-blue" )

#define FPNC_SETTINGS_RED_GAIN              ( "red" )
#define FPNC_SETTINGS_GREEN_GAIN            ( "green" )
#define FPNC_SETTINGS_BLUE_GAIN             ( "blue" )

/******************************************************************************
 * x-axis for X-sample points (fix)
 *****************************************************************************/
const QVector<double>
x_axis( QVector<double>()
        << SAMPLE( FPNC_SAMPLE_X0) << SAMPLE( FPNC_SAMPLE_X1) << SAMPLE( FPNC_SAMPLE_X2) <<  SAMPLE( FPNC_SAMPLE_X3)
        << SAMPLE( FPNC_SAMPLE_X4) << SAMPLE( FPNC_SAMPLE_X5) << SAMPLE( FPNC_SAMPLE_X6) <<  SAMPLE( FPNC_SAMPLE_X7)
        << SAMPLE( FPNC_SAMPLE_X8) << SAMPLE( FPNC_SAMPLE_X9) << SAMPLE(FPNC_SAMPLE_X10) <<  SAMPLE(FPNC_SAMPLE_X11)
        << SAMPLE(FPNC_SAMPLE_X12) << SAMPLE(FPNC_SAMPLE_X13) << SAMPLE(FPNC_SAMPLE_X14) <<  SAMPLE(FPNC_SAMPLE_X15) );

/******************************************************************************
 * FpncBox::PrivateData
 *****************************************************************************/
class FpncBox::PrivateData
{
public:
    PrivateData()
        : m_ui( new Ui::UI_FpncBox )
        , m_bayer_pattern( 0 )
    {
        // do nothing
        m_ab_data = new FpncData( 1920, 16 );
        m_cd_data = new FpncData( 1920, 16 );
        
        m_ab_synced = false;    // currently not loaded to device
        m_cd_synced = false;    // currently not loaded to device 
    };

    ~PrivateData()
    {
        delete m_ab_data;
        delete m_cd_data;
        delete m_ui;
    };

    void initFpncPlot( QFont font )
    {
        // add data point curves
        QPen pen( Qt::red );
        
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph( CURVE_EVEN_LINES_ID )->setPen( pen );
        m_ui->FpncPlot->graph( CURVE_EVEN_LINES_ID )->setName( QString("Even lines") );
        
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph( CURVE_ODD_LINES_ID )->setPen( pen );
        m_ui->FpncPlot->graph( CURVE_ODD_LINES_ID )->setName( QString("Odd lines") );
        
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph( CURVE_EVEN_LINES_LOADED_ID )->setPen( pen );
        m_ui->FpncPlot->graph( CURVE_EVEN_LINES_LOADED_ID )->setName( QString("Even lines (Loaded)") );
        m_ui->FpncPlot->graph( CURVE_EVEN_LINES_LOADED_ID )->setVisible( false );
        m_ui->FpncPlot->graph( CURVE_EVEN_LINES_LOADED_ID )->removeFromLegend();

        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph( CURVE_ODD_LINES_LOADED_ID )->setPen( pen );
        m_ui->FpncPlot->graph( CURVE_ODD_LINES_LOADED_ID )->setName( QString("Odd lines (Loaded)") );
        m_ui->FpncPlot->graph( CURVE_ODD_LINES_LOADED_ID )->setVisible( false );
        m_ui->FpncPlot->graph( CURVE_ODD_LINES_LOADED_ID )->removeFromLegend();

        // add sample point lines
        QPen penSample( QColor( 255, 255, 0, 128 ) );
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph(  4 )->setPen( penSample );   // 3
        m_ui->FpncPlot->graph(  4 )->removeFromLegend();
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph(  5 )->setPen( penSample );   // 4
        m_ui->FpncPlot->graph(  5 )->removeFromLegend();
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph(  6 )->setPen( penSample );   // 5
        m_ui->FpncPlot->graph(  6 )->removeFromLegend();
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph(  7 )->setPen( penSample );   // 6
        m_ui->FpncPlot->graph(  7 )->removeFromLegend();
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph(  8 )->setPen( penSample );   // 7
        m_ui->FpncPlot->graph(  8 )->removeFromLegend();
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph(  9 )->setPen( penSample );   // 8
        m_ui->FpncPlot->graph(  9 )->removeFromLegend();
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph( 10 )->setPen( penSample );   // 9
        m_ui->FpncPlot->graph( 10 )->removeFromLegend();
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph( 11 )->setPen( penSample );   // 10
        m_ui->FpncPlot->graph( 11 )->removeFromLegend();
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph( 12 )->setPen( penSample );   // 11
        m_ui->FpncPlot->graph( 12 )->removeFromLegend();
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph( 13 )->setPen( penSample );   // 12
        m_ui->FpncPlot->graph( 13 )->removeFromLegend();
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph( 14 )->setPen( penSample );   // 13
        m_ui->FpncPlot->graph( 14 )->removeFromLegend();
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph( 15 )->setPen( penSample );   // 14
        m_ui->FpncPlot->graph( 15 )->removeFromLegend();
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph( 16 )->setPen( penSample );   // 1
        m_ui->FpncPlot->graph( 16 )->removeFromLegend();
        m_ui->FpncPlot->addGraph();
        m_ui->FpncPlot->graph( 17 )->setPen( penSample );   // 2
        m_ui->FpncPlot->graph( 17 )->removeFromLegend();

        m_ui->FpncPlot->setBackground( QBrush(QColor(48,47,47)) );
        m_ui->FpncPlot->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom );
        m_ui->FpncPlot->axisRect()->setupFullAxesBox(true);
        m_ui->FpncPlot->xAxis->setLabel( "" );
        m_ui->FpncPlot->xAxis->setLabelColor( QColor(177, 177, 177) );
        m_ui->FpncPlot->yAxis->setLabel( "" );
        m_ui->FpncPlot->yAxis->setLabelColor( QColor(177, 177, 177) );
        m_ui->FpncPlot->xAxis->setRange( 0, 1 );
        m_ui->FpncPlot->yAxis->setRange( -300, 300 );
        m_ui->FpncPlot->xAxis->setBasePen( QPen(QColor(177, 177, 177), 1) );
        m_ui->FpncPlot->yAxis->setBasePen( QPen(QColor(177, 177, 177), 1) );
        m_ui->FpncPlot->xAxis->setTickPen( QPen(QColor(177, 177, 177), 1) );
        m_ui->FpncPlot->yAxis->setTickPen( QPen(QColor(177, 177, 177), 1) );
        m_ui->FpncPlot->xAxis->setSubTickPen( QPen( QColor(177, 177, 177), 1) );
        m_ui->FpncPlot->yAxis->setSubTickPen( QPen( QColor(177, 177, 177), 1) );
        m_ui->FpncPlot->xAxis->setTickLabelColor( QColor(177, 177, 177) );
        m_ui->FpncPlot->yAxis->setTickLabelColor( QColor(177, 177, 177) );
        m_ui->FpncPlot->xAxis->setScaleType(QCPAxis::stLogarithmic);

        m_ui->FpncPlot->xAxis2->setBasePen( QPen(QColor(177, 177, 177), 1) );
        m_ui->FpncPlot->yAxis2->setBasePen( QPen(QColor(177, 177, 177), 1) );
        m_ui->FpncPlot->xAxis2->setTickPen( QPen(QColor(177, 177, 177), 1) );
        m_ui->FpncPlot->yAxis2->setTickPen( QPen(QColor(177, 177, 177), 1) );
        m_ui->FpncPlot->xAxis2->setSubTickPen( QPen( QColor(177, 177, 177), 1) );
        m_ui->FpncPlot->yAxis2->setSubTickPen( QPen( QColor(177, 177, 177), 1) );
        m_ui->FpncPlot->xAxis2->setTickLabelColor( QColor(177, 177, 177) );
        m_ui->FpncPlot->yAxis2->setTickLabelColor( QColor(177, 177, 177) );

        QLinearGradient axisRectGradient;
        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(120, 120, 120));
        axisRectGradient.setColorAt(1, QColor(48, 48, 48));
        m_ui->FpncPlot->axisRect()->setBackground(axisRectGradient);

        QFont legendFont = font;
        legendFont.setPointSize(10);
        m_ui->FpncPlot->legend->setVisible( true );
        m_ui->FpncPlot->legend->setFont( legendFont );
        m_ui->FpncPlot->legend->setSelectedFont( legendFont );
        m_ui->FpncPlot->legend->setSelectableParts( QCPLegend::spItems ); // legend box shall not be selectable, only legend items
        
        QBrush brush( QColor(177, 177, 177) );
        m_ui->FpncPlot->legend->setBrush( brush );
        m_ui->FpncPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
        
        computeSampleLines();
    }
    
    void computeSampleLines( )
    {
        QVector<double> y(2);
        QVector<double> x(2);
        
        unsigned i;

        y[0] = -300.0f;
        y[1] =  300.0f;

        for ( i=0; i<14; i++ )
        {
            x[0] = x[1] = x_axis[i+1];
            
            m_ui->FpncPlot->graph( i+4 )->data()->clear();
            m_ui->FpncPlot->graph( i+4 )->setData( x, y );
        }
    }

    void computeColors( int column )
    {
        QColor c0;
        QColor c1;
        QColor c2;
        QColor c3;

        if ( column & 0x1 )
        {
            // odd columns
            switch ( m_bayer_pattern )
            {
                case 0:
                    c0 = Qt::green;
                    c1 = Qt::blue;
                    c2 = QColor(   0, 128,   0, 192 );
                    c3 = QColor(   0,   0, 128, 192 );
                    break;

                case 1:
                    c0 = Qt::red;
                    c1 = Qt::green;
                    c2 = QColor( 128,   0,   0, 192 );
                    c3 = QColor(   0, 128,   0, 192 );
                    break;

                case 2:
                    c0 = Qt::blue;
                    c1 = Qt::green;
                    c2 = QColor(   0,   0, 128, 192 );
                    c3 = QColor(   0, 128,   0, 192 );
                    break;
                
                case 3:
                    c0 = Qt::green;
                    c1 = Qt::red;
                    c2 = QColor(   0, 128,   0, 192 );
                    c3 = QColor( 128,   0,   0, 192 );
                    break;

                default:
                    c0 = Qt::yellow;
                    c1 = Qt::yellow;
                    c2 = Qt::yellow;
                    c3 = Qt::yellow;
                    break;
            }
        }
        else
        {
            // even columns
            switch ( m_bayer_pattern )
            {
                case 0:
                    c0 = Qt::red;
                    c1 = Qt::green;
                    c2 = QColor( 128,   0,   0, 192 );
                    c3 = QColor(   0, 128,   0, 192 );
                    break;                         
                                                   
                case 1:                            
                    c0 = Qt::green;                
                    c1 = Qt::blue;                 
                    c2 = QColor(   0, 128,   0, 192 );
                    c3 = QColor(   0,   0, 128, 192 );
                    break;                         
                                                   
                case 2:                            
                    c0 = Qt::green;                
                    c1 = Qt::red;                  
                    c2 = QColor(   0, 128,   0, 192 );
                    c3 = QColor( 128,   0,   0, 192 );
                    break;                         
                                                   
                case 3:                            
                    c0 = Qt::blue;                 
                    c1 = Qt::green;                
                    c2 = QColor(   0,   0, 128, 192 );
                    c3 = QColor(   0, 128,   0, 192 );
                    break;

                default:
                    c0 = Qt::yellow;
                    c1 = Qt::yellow;
                    c2 = Qt::yellow;
                    c3 = Qt::yellow;
                    break;
            }
        }
        
        QPen pen0( c0 );
        QPen pen1( c1 );
        QPen pen2( c2 );
        QPen pen3( c3 );
        
        m_ui->FpncPlot->graph( CURVE_EVEN_LINES_ID )->setPen( pen0 );
        m_ui->FpncPlot->graph( CURVE_ODD_LINES_ID )->setPen( pen1 );
        m_ui->FpncPlot->graph( CURVE_EVEN_LINES_LOADED_ID )->setPen( pen2 );
        m_ui->FpncPlot->graph( CURVE_ODD_LINES_LOADED_ID )->setPen( pen3 );
    }

    void computeCorrectionDataLines( int column, QVector<int> data0, QVector<int> data1 )
    {
        unsigned i;

        QVector<double> y0(16);
        QVector<double> y1(16);
        QVector<double> y2(16);
        QVector<double> y3(16);

        QVector<int> data2;
        QVector<int> data3;

        bool bAbData = m_ab_data->hasData();
        bool bCdData = m_cd_data->hasData();

        if ( bAbData )
        {
            data2 = m_ab_data->getCorrectionData( column );
        }

        if ( bCdData )
        {
            data3 = m_cd_data->getCorrectionData( column );
        }
        
        for ( i=0; i<16; i++ )
        {
            y0[i] =  (double)(data0[i]);
            y1[i] =  (double)(data1[i]);

            if ( bAbData )
            {
                y2[i] =  (double)(data2[i]);
            }

            if ( bCdData )
            {
                y3[i] =  (double)(data3[i]);
            }
        }

        computeColors( column );

        m_ui->FpncPlot->graph( CURVE_EVEN_LINES_ID )->data()->clear();
        m_ui->FpncPlot->graph( CURVE_ODD_LINES_ID )->data()->clear();
        m_ui->FpncPlot->graph( CURVE_EVEN_LINES_LOADED_ID )->data()->clear();
        m_ui->FpncPlot->graph( CURVE_ODD_LINES_LOADED_ID )->data()->clear();

        m_ui->FpncPlot->graph( CURVE_EVEN_LINES_ID )->setData( x_axis, y0 );
        m_ui->FpncPlot->graph( CURVE_ODD_LINES_ID )->setData( x_axis, y1 );

        if ( bAbData )
        {
            m_ui->FpncPlot->graph( CURVE_EVEN_LINES_LOADED_ID )->setData( x_axis, y2 );
        }
        
        if ( bCdData )
        {
            m_ui->FpncPlot->graph( CURVE_ODD_LINES_LOADED_ID )->setData( x_axis, y3 );
        }

        m_ui->FpncPlot->replot();
    }


    Ui::UI_FpncBox *    m_ui;               /**< ui handle */
    int                 m_bayer_pattern;    /**< running bayer-pattern on device */
    FpncData *          m_ab_data;          /**< FPN correction data even lines */ 
    FpncData *          m_cd_data;          /**< FPN correction data odd lines */ 
    bool                m_ab_synced;
    bool                m_cd_synced;
};

/******************************************************************************
 * FpncBox::FpncBox
 *****************************************************************************/
FpncBox::FpncBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData;

    // initialize UI
    d_data->m_ui->setupUi( this );
    d_data->initFpncPlot( font() );

    // don't display progress bar 
    d_data->m_ui->progressBar->setVisible( false );

    // FPNC stage
    connect( d_data->m_ui->cbxFpncEnable, SIGNAL(stateChanged(int)), this, SIGNAL(FpncEnableChanged(int)) );
    connect( d_data->m_ui->btnCorrectionData0, SIGNAL(clicked()), this, SLOT(onCorrectionData0Click()) );
    connect( d_data->m_ui->btnCorrectionData1, SIGNAL(clicked()), this, SLOT(onCorrectionData1Click()) );
    connect( d_data->m_ui->btnLoad1, SIGNAL(clicked()), this, SLOT(onLoad1Click()) );
    connect( d_data->m_ui->btnLoad2, SIGNAL(clicked()), this, SLOT(onLoad2Click()) );

    connect( d_data->m_ui->kbxColumn, SIGNAL(ValueChanged(int)), this, SIGNAL(FpncColumnChanged(int)) );
    
    connect( d_data->m_ui->InverseGains, SIGNAL(ValuesChanged(int,int,int)), this, SLOT(onInverseGainsChange(int,int,int)) );
    connect( d_data->m_ui->Gains, SIGNAL(ValuesChanged(int,int,int)), this, SLOT(onGainsChange(int,int,int)) );

    connect( d_data->m_ui->FpncPlot->xAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onFpncXRangeChange(QCPRange)));
    connect( d_data->m_ui->FpncPlot->yAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onFpncYRangeChange(QCPRange)));

    ////////////////////
    // operation mode
    ////////////////////
    prepareMode( mode() ); 
}

/******************************************************************************
 * FpncBox::~FpncBox
 *****************************************************************************/
FpncBox::~FpncBox()
{
    delete d_data;
}

/******************************************************************************
 * FpncBox::Filename0
 *****************************************************************************/
QString FpncBox::Filename0() const
{
    return ( d_data->m_ui->letCorrectionData0->text() );
}

/******************************************************************************
 * FpncBox::Filename0
 *****************************************************************************/
void FpncBox::setFilename0( QString & fileName )
{
    d_data->m_ab_data->readDataFile( fileName );
    d_data->m_ui->letCorrectionData0->setText( fileName );
    d_data->m_ui->FpncPlot->graph( CURVE_EVEN_LINES_LOADED_ID )->setVisible( true );
    d_data->m_ui->FpncPlot->graph( CURVE_EVEN_LINES_LOADED_ID )->addToLegend();
    
    // emit an update to display new values
    emit FpncColumnChanged( d_data->m_ui->kbxColumn->value() );
}

/******************************************************************************
 * FpncBox::Filename1
 *****************************************************************************/
QString FpncBox::Filename1() const
{
    return ( d_data->m_ui->letCorrectionData1->text() );
}

/******************************************************************************
 * FpncBox::Filename1
 *****************************************************************************/
void FpncBox::setFilename1( QString & fileName )
{
    d_data->m_cd_data->readDataFile( fileName );
    d_data->m_ui->letCorrectionData1->setText( fileName );
    d_data->m_ui->FpncPlot->graph( CURVE_ODD_LINES_LOADED_ID )->setVisible( true );
    d_data->m_ui->FpncPlot->graph( CURVE_ODD_LINES_LOADED_ID )->addToLegend();

    // emit an update to display new values
    emit FpncColumnChanged( d_data->m_ui->kbxColumn->value() );
}

/******************************************************************************
 * FpncBox::InverseRedGain
 *****************************************************************************/
int FpncBox::InverseRedGain() const
{
    return ( d_data->m_ui->InverseGains->value0() );
}

/******************************************************************************
 * FpncBox::InverseGreenGain
 *****************************************************************************/
int FpncBox::InverseGreenGain() const
{
    return ( d_data->m_ui->InverseGains->value1() );
}

/******************************************************************************
 * FpncBox::InverseBlueGain
 *****************************************************************************/
int FpncBox::InverseBlueGain() const
{
    return ( d_data->m_ui->InverseGains->value2() );
}

/******************************************************************************
 * FpncBox::setInverseGains
 *****************************************************************************/
void FpncBox::setInverseGains( const int red, const int green, const int blue )
{
    int a, b, c, d;

    d_data->m_ui->InverseGains->setValue0( red );
    d_data->m_ui->InverseGains->setValue1( green );
    d_data->m_ui->InverseGains->setValue2( blue );

    switch ( d_data->m_bayer_pattern )
    {
        case 0: // RGGB
            a = red;
            b = c = green;
            d = blue;
            break;

        case 1: // GRBG
            a = d = green;
            b = red;
            c = blue;
            break;

        case 2: // GBRG
            a = d = green;
            b = blue;
            c = red;
            break;

        case 3: // BGGR
            a = blue;
            b = c = green;
            d = red;
            break;

        default:
            return;
    }

    emit FpncInverseGainsChanged( a, b, c, d );
}

/******************************************************************************
 * FpncBox::RedGain
 *****************************************************************************/
int FpncBox::RedGain() const
{
    return ( d_data->m_ui->Gains->value0() );
}

/******************************************************************************
 * FpncBox::GreenGain
 *****************************************************************************/
int FpncBox::GreenGain() const
{
    return ( d_data->m_ui->Gains->value1() );
}

/******************************************************************************
 * FpncBox::BlueGain
 *****************************************************************************/
int FpncBox::BlueGain() const
{
    return ( d_data->m_ui->Gains->value2() );
}

/******************************************************************************
 * FpncBox::setGains
 *****************************************************************************/
void FpncBox::setGains( const int red, const int green, const int blue )
{
    int a, b, c, d;

    d_data->m_ui->Gains->setValue0( red );
    d_data->m_ui->Gains->setValue1( green );
    d_data->m_ui->Gains->setValue2( blue );

    switch ( d_data->m_bayer_pattern )
    {
        case 0: // RGGB
            a = red;
            b = c = green;
            d = blue;
            break;

        case 1: // GRBG
            a = d = green;
            b = red;
            c = blue;
            break;

        case 2: // GBRG
            a = d = green;
            b = blue;
            c = red;
            break;

        case 3: // BGGR
            a = blue;
            b = c = green;
            d = red;
            break;

        default:
            return;
    }

    emit FpncInverseGainsChanged( a, b, c, d );
}

/******************************************************************************
 * FpncBox::prepareMode
 *****************************************************************************/
void FpncBox::prepareMode( const Mode mode )
{
    bool v = (DctWidgetBox::Normal == mode) ? false : true;
    d_data->m_ui->InverseGains->setVisible( v );
    d_data->m_ui->Gains->setVisible( v );
}

/******************************************************************************
 * FpncBox::loadSettings
 *****************************************************************************/
void FpncBox::loadSettings( QSettings & s )
{
    int red, green, blue;
    QString fn;

    s.beginGroup( FPNC_SETTINGS_SECTION_NAME );
    
    fn = s.value( FPNC_SETTINGS_FILENAME_0 ).toString();
    setFilename0( fn );
    
    fn = s.value( FPNC_SETTINGS_FILENAME_1 ).toString();
    setFilename1( fn );

    red   = s.value( FPNC_SETTINGS_RED_INVERSE_GAIN ).toInt();
    green = s.value( FPNC_SETTINGS_GREEN_INVERSE_GAIN ).toInt();
    blue  = s.value( FPNC_SETTINGS_BLUE_INVERSE_GAIN ).toInt();
    setInverseGains( red, green, blue );

    red   = s.value( FPNC_SETTINGS_RED_GAIN ).toInt();
    green = s.value( FPNC_SETTINGS_GREEN_GAIN ).toInt();
    blue  = s.value( FPNC_SETTINGS_BLUE_GAIN ).toInt();
    setGains( red, green, blue );

    s.endGroup();
}

/******************************************************************************
 * FpncBox::saveSettings
 *****************************************************************************/
void FpncBox::saveSettings( QSettings & s )
{
    s.beginGroup( FPNC_SETTINGS_SECTION_NAME );

    s.setValue( FPNC_SETTINGS_FILENAME_0, Filename0() );
    s.setValue( FPNC_SETTINGS_FILENAME_1, Filename1() );

    s.setValue( FPNC_SETTINGS_RED_INVERSE_GAIN  , InverseRedGain() );
    s.setValue( FPNC_SETTINGS_GREEN_INVERSE_GAIN, InverseGreenGain() );
    s.setValue( FPNC_SETTINGS_BLUE_INVERSE_GAIN , InverseBlueGain() );
 
    s.setValue( FPNC_SETTINGS_RED_GAIN  , RedGain() );
    s.setValue( FPNC_SETTINGS_GREEN_GAIN, GreenGain() );
    s.setValue( FPNC_SETTINGS_BLUE_GAIN , BlueGain() );
 
    s.endGroup();
}

/******************************************************************************
 * FpncBox::applySettings
 *****************************************************************************/
void FpncBox::applySettings( void )
{
    // emit an update to display new values
    emit FpncColumnChanged( d_data->m_ui->kbxColumn->value() );

    // set inverse gains and normal gains (using the set methods is easier than reimplementing the switch/case inside them)
    setInverseGains( InverseRedGain(), InverseGreenGain(), InverseBlueGain() );
    setInverseGains( RedGain(), GreenGain(), BlueGain() );
}

/******************************************************************************
 * FpncBox::onBayerPatternChange
 *****************************************************************************/
void FpncBox::onBayerPatternChange( int value )
{
    d_data->m_bayer_pattern = value;
}

/******************************************************************************
 * FpncBox::onFpncEnableChange
 *****************************************************************************/
void FpncBox::onFpncEnableChange( int flag )
{
    // set value of checkbox
    d_data->m_ui->cbxFpncEnable->blockSignals( true );
    d_data->m_ui->cbxFpncEnable->setCheckState( flag ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxFpncEnable->blockSignals( false );
}

/******************************************************************************
 * FpncBox::onFpncCorectionDataChange
 *****************************************************************************/
void FpncBox::onFpncCorectionDataChange( int column, QVector<int> data0, QVector<int> data1 )
{
    d_data->computeCorrectionDataLines( column, data0, data1 );
}

/******************************************************************************
 * FpncBox::onFpncInverseGainsChange
 *****************************************************************************/
void FpncBox::onFpncInverseGainsChange( int a, int b, int c, int d )
{
    int valueA = (a == 0) ? 0 : ((0x8000ul<<11) / a);
    int valueB = (b == 0) ? 0 : ((0x8000ul<<11) / b);
    int valueC = (c == 0) ? 0 : ((0x8000ul<<11) / c);
    int valueD = (d == 0) ? 0 : ((0x8000ul<<11) / d);

    switch ( d_data->m_bayer_pattern )
    {
        case 0: // RGGB
            d_data->m_ui->InverseGains->setValue0( valueA );
            d_data->m_ui->InverseGains->setValue1( valueB );
            d_data->m_ui->InverseGains->setValue2( valueD );
            break;

        case 1: // GRBG
            d_data->m_ui->InverseGains->setValue0( valueB );
            d_data->m_ui->InverseGains->setValue1( valueA );
            d_data->m_ui->InverseGains->setValue2( valueC );
            break;

        case 2: // GBRG
            d_data->m_ui->InverseGains->setValue0( valueC );
            d_data->m_ui->InverseGains->setValue1( valueA );
            d_data->m_ui->InverseGains->setValue2( valueB );
            break;

        case 3: // BGGR
            d_data->m_ui->InverseGains->setValue0( valueD );
            d_data->m_ui->InverseGains->setValue1( valueB );
            d_data->m_ui->InverseGains->setValue2( valueA );
            break;

        default:
            break;
    }
}

/******************************************************************************
 * FpncBox::onFpncGainsChange
 *****************************************************************************/
void FpncBox::onFpncGainsChange( int a, int b, int c, int d )
{
    switch ( d_data->m_bayer_pattern )
    {
        case 0: // RGGB
            d_data->m_ui->Gains->setValue0( a );
            d_data->m_ui->Gains->setValue1( b );
            d_data->m_ui->Gains->setValue2( d );
            break;

        case 1: // GRBG
            d_data->m_ui->Gains->setValue0( b );
            d_data->m_ui->Gains->setValue1( a );
            d_data->m_ui->Gains->setValue2( c );
            break;

        case 2: // GBRG
            d_data->m_ui->Gains->setValue0( c );
            d_data->m_ui->Gains->setValue1( a );
            d_data->m_ui->Gains->setValue2( b );
            break;

        case 3: // BGGR
            d_data->m_ui->Gains->setValue0( d );
            d_data->m_ui->Gains->setValue1( b );
            d_data->m_ui->Gains->setValue2( a );
            break;

        default:
            break;
    }
}

/******************************************************************************
 * FpncBox::onFpncXRangeChange
 *****************************************************************************/
void FpncBox::onFpncXRangeChange( const QCPRange &range )
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

    d_data->m_ui->FpncPlot->xAxis->setRange( boundedRange );
}

/******************************************************************************
 * FpncBox::onFpncYRangeChange
 *****************************************************************************/
void FpncBox::onFpncYRangeChange( const QCPRange &range )
{
    QCPRange boundedRange = range;
    double lowerRangeBound = -300;
    double upperRangeBound =  300;
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

    d_data->m_ui->FpncPlot->yAxis->setRange( boundedRange );
}

/******************************************************************************
 * FpncBox::onCorrectionData0Click
 *****************************************************************************/
void FpncBox::onCorrectionData0Click()
{
    QString fileName  = d_data->m_ui->letCorrectionData0->text();
    QString directory;

    if ( NULL != fileName )
    {
        QFileInfo fi = QFileInfo( fileName );
        directory = fi.absolutePath();
    }
    else
    {
        directory = QDir::currentPath();
    }

    // NOTE: It can fail on gtk-systems when an empty filename is given
    //       in the native dialog-box, because GTK sends a SIGSEGV-signal
    //       to process and this is not handled by Qt.
    fileName = QFileDialog::getOpenFileName( 
        this, tr("Correction Data 0 - File"),
        directory,
        "Select Correction Data Files (*_mem0.bin);;All files (*.*)"
    );

    // check if filename changed
    if ( (NULL != fileName) && (d_data->m_ui->letCorrectionData0->text() != fileName) ) 
    {
        setFilename0( fileName );
    }
}

/******************************************************************************
 * FpncBox::onCorrectionData1Click
 *****************************************************************************/
void FpncBox::onCorrectionData1Click()
{
    QString fileName  = d_data->m_ui->letCorrectionData1->text();
    QString directory;

    if ( NULL != fileName )
    {
        QFileInfo fi = QFileInfo( fileName );
        directory = fi.absolutePath();
    }
    else
    {
        directory = QDir::currentPath();
    }

    // NOTE: It can fail on gtk-systems when an empty filename is given
    //       in the native dialog-box, because GTK sends a SIGSEGV-signal
    //       to process and this is not handled by Qt.
    fileName = QFileDialog::getOpenFileName(
        this, tr("Correction Data 1 - File"), 
        directory,
        "Correction Data Files (*_mem1.bin);;All files (*.*)"
    );

    // check if filename changed
    if ( (NULL != fileName) && (d_data->m_ui->letCorrectionData1->text() != fileName) ) 
    {
        setFilename1( fileName );
    }
}

/******************************************************************************
 * FpncBox::onLoad1Click
 *****************************************************************************/
void FpncBox::onLoad1Click()
{
    if ( d_data->m_ab_data->hasData() )
    {
        QVector<int> data;

        int i;

        d_data->m_ui->progressBar->setMinimum( 0 );
        d_data->m_ui->progressBar->setMaximum( d_data->m_ab_data->getWidth() );
        d_data->m_ui->progressBar->setValue( 0 );
        d_data->m_ui->progressBar->setOrientation( Qt::Horizontal );
        d_data->m_ui->progressBar->setVisible( true );

        for ( i = 0; i<d_data->m_ab_data->getWidth(); i+=2 )
        {
            // get calibration data from db
            data = d_data->m_ab_data->getCalibrationData( i );

            emit FpncColumnCalibrationDataChanged( true, i, data );

            d_data->m_ui->progressBar->setValue( i );
        }

        // emit an update to display new values
        emit FpncColumnChanged( d_data->m_ui->kbxColumn->value() );

        d_data->m_ui->progressBar->setVisible( false );
    }
}
    
/******************************************************************************
 * FpncBox::onLoad2Click
 *****************************************************************************/
void FpncBox::onLoad2Click()
{
    if ( d_data->m_cd_data->hasData() )
    {
        QVector<int> data;

        int i;

        d_data->m_ui->progressBar->setMinimum( 0 );
        d_data->m_ui->progressBar->setMaximum( d_data->m_cd_data->getWidth() );
        d_data->m_ui->progressBar->setValue( 0 );
        d_data->m_ui->progressBar->setOrientation( Qt::Horizontal );
        d_data->m_ui->progressBar->setVisible( true );

        for ( i = 0; i<d_data->m_cd_data->getWidth(); i+=2 )
        {
            // get calibration data from db
            data = d_data->m_cd_data->getCalibrationData( i );

            emit FpncColumnCalibrationDataChanged( false, i, data );

            d_data->m_ui->progressBar->setValue( i );
        }

        // emit an update to display new values
        emit FpncColumnChanged( d_data->m_ui->kbxColumn->value() );

        d_data->m_ui->progressBar->setVisible( false );
    }
}

/******************************************************************************
 * FpncBox::onInverseGainsChange
 *****************************************************************************/
void FpncBox::onInverseGainsChange( int v0, int v1, int v2 )
{
    int a, b, c, d;

    int value0 = (v0 == 0) ? 0 : ((0x800ul<<15) / v0);
    int value1 = (v1 == 0) ? 0 : ((0x800ul<<15) / v1);
    int value2 = (v2 == 0) ? 0 : ((0x800ul<<15) / v2);

    switch ( d_data->m_bayer_pattern )
    {
        case 0: // RGGB
            a = value0;
            b = c = value1;
            d = value2;
            break;

        case 1: // GRBG
            a = d = value1;
            b = value0;
            c = value2;
            break;

        case 2: // GBRG
            a = d = value1;
            b = value2;
            c = value0;
            break;

        case 3: // BGGR
            a = value2;
            b = c = value1;
            d = value0;
            break;

        default:
            return;
    }
    
    emit FpncInverseGainsChanged( a, b, c, d );
}

/******************************************************************************
 * FpncBox::onGainsChange
 *****************************************************************************/
void FpncBox::onGainsChange( int v0, int v1, int v2 )
{
    int a, b, c, d;

    switch ( d_data->m_bayer_pattern )
    {
        case 0: // RGGB
            a = v0;
            b = c = v1;
            d = v2;
            break;

        case 1: // GRBG
            a = d = v1;
            b = v0;
            c = v2;
            break;

        case 2: // GBRG
            a = d = v1;
            b = v2;
            c = v0;
            break;

        case 3: // BGGR
            a = v2;
            b = c = v1;
            d = v0;
            break;

        default:
            return;
    }

    emit FpncGainsChanged( a, b, c, d );
}


