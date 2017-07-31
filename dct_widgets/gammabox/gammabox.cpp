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
 * @file    gammabox.cpp
 *
 * @brief   Implementation of gamma setting box
 *
 *****************************************************************************/
#include <QtDebug>

#include "gammabox.h"
#include "ui_gammabox.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_GammaBox;
}

/******************************************************************************
 * local definitions
 *****************************************************************************/
#define THRESHOLD_CURVE_ID          ( 0 )
#define LINEAR_CURVE_ID             ( 1 )
#define NON_LINEAR_CURVE_ID         ( 2 )

#define SCALING_FACTOR              ( 1000.0f )
#define DOUBLE(x)                   ((double)x)

#define TEXT_ENABLE                 ( "Enable" )
#define TEXT_DISABLE                ( "Disable" )

/******************************************************************************
 * STRAIGHT
 *****************************************************************************/
#define STRAIGHT_THRESHOLD          ( 1000 )
#define STRAIGHT_LINEAR_CONTRAST    ( 1000 )
#define STRAIGHT_LINEAR_BRIGHTNESS  ( 0 )
#define STRAIGHT_CONTRAST           ( 0 )
#define STRAIGHT_GAMMA              ( 0 )
#define STRAIGHT_BRIGHTNESS         ( 0 )

/******************************************************************************
 * REC.709
 *****************************************************************************/
#define REC709_THRESHOLD            ( 18 )
#define REC709_LINEAR_CONTRAST      ( 4500 )
#define REC709_LINEAR_BRIGHTNESS    ( 0 )
#define REC709_CONTRAST             ( 1099 )
#define REC709_GAMMA                ( 450 )
#define REC709_BRIGHTNESS           ( -99 )

/******************************************************************************
 * BT.601
 *****************************************************************************/
#define BT601_THRESHOLD             ( 0 )
#define BT601_LINEAR_CONTRAST       ( 0 )
#define BT601_LINEAR_BRIGHTNESS     ( 0 )
#define BT601_CONTRAST              ( 1000 )
#define BT601_GAMMA                 ( 450 )
#define BT601_BRIGHTNESS            ( 0 )

/******************************************************************************
 * Settings
 *****************************************************************************/
#define GAMMA_SETTINGS_SECTION_NAME             ( "GAMMA" )

#define GAMMA_SETTINGS_ENABLE                   ( "enable" )

#define GAMMA_SETTINGS_THRESHOLD_MASTER         ( "threshold" )
#define GAMMA_SETTINGS_GAMMA_MASTER             ( "gamma" )
#define GAMMA_SETTINGS_LINEAR_CONTRAST_MASTER   ( "lcontrast" )
#define GAMMA_SETTINGS_LINEAR_BRIGHTNESS_MASTER ( "lbrightness" )
#define GAMMA_SETTINGS_CONTRAST_MASTER          ( "contrast" )
#define GAMMA_SETTINGS_BRIGHTNESS_MASTER        ( "brightness" )

#define GAMMA_SETTINGS_THRESHOLD_RED            ( "red_threshold" )
#define GAMMA_SETTINGS_GAMMA_RED                ( "red_gamma" )
#define GAMMA_SETTINGS_LINEAR_CONTRAST_RED      ( "red_lcontrast" )
#define GAMMA_SETTINGS_LINEAR_BRIGHTNESS_RED    ( "red_lbrightness" )
#define GAMMA_SETTINGS_CONTRAST_RED             ( "red_contrast" )
#define GAMMA_SETTINGS_BRIGHTNESS_RED           ( "red_brightness" )

#define GAMMA_SETTINGS_THRESHOLD_GREEN          ( "green_threshold" )
#define GAMMA_SETTINGS_GAMMA_GREEN              ( "green_gamma" )
#define GAMMA_SETTINGS_LINEAR_CONTRAST_GREEN    ( "green_lcontrast" )
#define GAMMA_SETTINGS_LINEAR_BRIGHTNESS_GREEN  ( "green_lbrightness" )
#define GAMMA_SETTINGS_CONTRAST_GREEN           ( "green_contrast" )
#define GAMMA_SETTINGS_BRIGHTNESS_GREEN         ( "green_brightness" )

#define GAMMA_SETTINGS_THRESHOLD_BLUE           ( "blue_threshold" )
#define GAMMA_SETTINGS_GAMMA_BLUE               ( "blue_gamma" )
#define GAMMA_SETTINGS_LINEAR_CONTRAST_BLUE     ( "blue_lcontrast" )
#define GAMMA_SETTINGS_LINEAR_BRIGHTNESS_BLUE   ( "blue_lbrightness" )
#define GAMMA_SETTINGS_CONTRAST_BLUE            ( "blue_contrast" )
#define GAMMA_SETTINGS_BRIGHTNESS_BLUE          ( "blue_brightness" )


/******************************************************************************
 * GammaBox::PrivateData
 *****************************************************************************/
class GammaBox::PrivateData
{
public:
    PrivateData( QWidget * parent )
        : m_ui( new Ui::UI_GammaBox )
        , m_enable( true )
    {
        // initialize UI
        m_ui->setupUi( parent );

        /////////////////////
        // threshold 
        /////////////////////

        // configure threshold knob
        m_ui->Threshold->setRange( GAMMA_THRESHOLD_MIN, GAMMA_THRESHOLD_MAX );
        m_ui->Threshold->setCommaPos( GAMMA_THRESHOLD_COMMA_POSITION );
        m_ui->Threshold->setDisplayMultiplier( GAMMA_THRESHOLD_DISPLAY_MULTIPLIER );
        m_ui->Threshold->setBase( GAMMA_THRESHOLD_BASE );
        m_ui->Threshold->setMaxAngle( 180 );
        m_ui->Threshold->setMaxRounds( 4 );
        m_ui->Threshold->setFmt( GAMMA_THRESHOLD_DISPLAY_MASK );

        /////////////////////
        // gamma
        /////////////////////

        // configure gamma knob
        m_ui->Gamma->setRange( GAMMA_GAMMA_MIN, GAMMA_GAMMA_MAX );
        m_ui->Gamma->setCommaPos( GAMMA_GAMMA_COMMA_POSITION );
        m_ui->Gamma->setDisplayMultiplier( GAMMA_GAMMA_DISPLAY_MULTIPLIER );
        m_ui->Gamma->setBase( GAMMA_GAMMA_BASE );
        m_ui->Gamma->setMaxAngle( 180 );
        m_ui->Gamma->setMaxRounds( 4 );
        m_ui->Gamma->setFmt( GAMMA_GAMMA_DISPLAY_MASK );

        /////////////////////
        // linear contrast 
        /////////////////////

        // configure linear contrast knob
        m_ui->LinearContrast->setRange( GAMMA_LINEAR_CONTRAST_MIN, GAMMA_LINEAR_CONTRAST_MAX );
        m_ui->LinearContrast->setCommaPos( GAMMA_LINEAR_CONTRAST_COMMA_POSITION );
        m_ui->LinearContrast->setDisplayMultiplier( GAMMA_LINEAR_CONTRAST_DISPLAY_MULTIPLIER );
        m_ui->LinearContrast->setBase( GAMMA_LINEAR_CONTRAST_BASE );
        m_ui->LinearContrast->setMaxAngle( 180 );
        m_ui->LinearContrast->setMaxRounds( 4 );
        m_ui->LinearContrast->setFmt( GAMMA_LINEAR_CONTRAST_DISPLAY_MASK );

        /////////////////////
        // linear brightness 
        /////////////////////

        // configure linear brightness knob
        m_ui->LinearBrightness->setRange( GAMMA_LINEAR_BRIGHTNESS_MIN, GAMMA_LINEAR_BRIGHTNESS_MAX );
        m_ui->LinearBrightness->setCommaPos( GAMMA_LINEAR_BRIGHTNESS_COMMA_POSITION );
        m_ui->LinearBrightness->setDisplayMultiplier( GAMMA_LINEAR_BRIGHTNESS_DISPLAY_MULTIPLIER );
        m_ui->LinearBrightness->setBase( GAMMA_LINEAR_BRIGHTNESS_BASE );
        m_ui->LinearBrightness->setMaxAngle( 180 );
        m_ui->LinearBrightness->setMaxRounds( 2 );
        m_ui->LinearBrightness->setFmt( GAMMA_LINEAR_BRIGHTNESS_DISPLAY_MASK );

        /////////////////////
        // contrast 
        /////////////////////

        // configure contrast knob
        m_ui->Contrast->setRange( GAMMA_CONTRAST_MIN, GAMMA_CONTRAST_MAX );
        m_ui->Contrast->setCommaPos( GAMMA_CONTRAST_COMMA_POSITION );
        m_ui->Contrast->setDisplayMultiplier( GAMMA_CONTRAST_DISPLAY_MULTIPLIER );
        m_ui->Contrast->setBase( GAMMA_CONTRAST_BASE );
        m_ui->Contrast->setMaxAngle( 180 );
        m_ui->Contrast->setMaxRounds( 4 );
        m_ui->Contrast->setFmt( GAMMA_CONTRAST_DISPLAY_MASK );

        /////////////////////
        // brightness
        /////////////////////

        // configure brightness knob
        m_ui->Brightness->setRange( GAMMA_BRIGHTNESS_MIN, GAMMA_BRIGHTNESS_MAX );
        m_ui->Brightness->setCommaPos( GAMMA_BRIGHTNESS_COMMA_POSITION );
        m_ui->Brightness->setDisplayMultiplier( GAMMA_BRIGHTNESS_DISPLAY_MULTIPLIER );
        m_ui->Brightness->setBase( GAMMA_BRIGHTNESS_BASE );
        m_ui->Brightness->setMaxAngle( 180 );
        m_ui->Brightness->setMaxRounds( 2 );
        m_ui->Brightness->setFmt( GAMMA_BRIGHTNESS_DISPLAY_MASK );

        // initialize values with REC.709
        m_threshold[Master]         = m_threshold[Red]          = m_threshold[Green]        = m_threshold[Blue]         = 18;
        m_linearContrast[Master]    = m_linearContrast[Red]     = m_linearContrast[Green]   = m_linearContrast[Blue]    = 4500;
        m_linearBrightness[Master]  = m_linearBrightness[Red]   = m_linearBrightness[Green] = m_linearBrightness[Blue]  = 0;
        m_contrast[Master]          = m_contrast[Red]           = m_contrast[Green]         = m_contrast[Blue]          = 1099;
        m_gamma[Master]             = m_gamma[Red]              = m_gamma[Green]            = m_gamma[Blue]             = 450;
        m_brightness[Master]        = m_brightness[Red]         = m_brightness[Green]       = m_brightness[Blue]        = -99;
        m_synced[Master]            = m_synced[Red]             = m_synced[Green]           = m_synced[Blue]            = false;

        initGammaPlot( m_ui->GammaPlot );
        initGammaPlot( m_ui->GammaPlotRed );
        initGammaPlot( m_ui->GammaPlotGreen );
        initGammaPlot( m_ui->GammaPlotBlue );

        // initialize curves
        computeThresholdLine( Master );
        computeThresholdLine( Red );
        computeThresholdLine( Green );
        computeThresholdLine( Blue );

        computeLinearGammaLine( Master );
        computeLinearGammaLine( Red );
        computeLinearGammaLine( Green );
        computeLinearGammaLine( Blue );

        computeNonLinearGammaLine( Master );
        computeNonLinearGammaLine( Red );
        computeNonLinearGammaLine( Green );
        computeNonLinearGammaLine( Blue );
        
        QLinearGradient axisRectGradient;
        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(120, 120, 120));
        axisRectGradient.setColorAt(1, QColor(48, 48, 48));
        m_ui->GammaPlot->axisRect()->setBackground(axisRectGradient);

        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(120, 48, 48));
        axisRectGradient.setColorAt(1, QColor(48, 48, 48));
        m_ui->GammaPlotRed->axisRect()->setBackground(axisRectGradient);
 
        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(48, 120, 48));
        axisRectGradient.setColorAt(1, QColor(48, 48, 48));
        m_ui->GammaPlotGreen->axisRect()->setBackground(axisRectGradient);

        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(48, 48, 120));
        axisRectGradient.setColorAt(1, QColor(48, 48, 48));
        m_ui->GammaPlotBlue->axisRect()->setBackground(axisRectGradient);

        // initialize values
        m_ui->Threshold->setValue       ( m_threshold[Master] );
        m_ui->LinearContrast->setValue  ( m_linearContrast[Master] );
        m_ui->LinearBrightness->setValue( m_linearBrightness[Master] );
        m_ui->Contrast->setValue        ( m_contrast[Master] );
        m_ui->Gamma->setValue           ( m_gamma[Master] );
        m_ui->Brightness->setValue      ( m_brightness[Master] );
    };

    ~PrivateData()
    {
        delete m_ui;
    };

    void initGammaPlot( QCustomPlot * plot )
    {
        // threshold curve
        QPen penThreshold( Qt::yellow );
        plot->addGraph();
        plot->graph( THRESHOLD_CURVE_ID )->setPen( penThreshold );

        // linear part of gamma curve
        QPen penLinear( Qt::cyan );
        plot->addGraph();
        plot->graph( LINEAR_CURVE_ID )->setPen( penLinear );

        // non linear part of gamma curve
        QPen penNonLinear( Qt::magenta );
        plot->addGraph();
        plot->graph( NON_LINEAR_CURVE_ID )->setPen( penNonLinear );

        plot->setBackground( QBrush(QColor(48,47,47)) );
        plot->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom );
        plot->axisRect()->setupFullAxesBox(true);
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

        plot->xAxis2->setBasePen( QPen(QColor(177, 177, 177), 1) );
        plot->yAxis2->setBasePen( QPen(QColor(177, 177, 177), 1) );
        plot->xAxis2->setTickPen( QPen(QColor(177, 177, 177), 1) );
        plot->yAxis2->setTickPen( QPen(QColor(177, 177, 177), 1) );
        plot->xAxis2->setSubTickPen( QPen( QColor(177, 177, 177), 1) );
        plot->yAxis2->setSubTickPen( QPen( QColor(177, 177, 177), 1) );
        plot->xAxis2->setTickLabelColor( QColor(177, 177, 177) );
        plot->yAxis2->setTickLabelColor( QColor(177, 177, 177) );
    }

    QCustomPlot * getGammaPlot( GammaChannel ch )
    {
        switch ( ch )
        {
            case Master:
                return ( m_ui->GammaPlot );
            case Red:
                return ( m_ui->GammaPlotRed );
            case Green:
                return ( m_ui->GammaPlotGreen );
            case Blue:
                return ( m_ui->GammaPlotBlue );
            default:
                return ( NULL );
        }
    }

    void computeThresholdLine( GammaChannel ch )
    {
        QVector<double> x(2);
        QVector<double> y(2);

        QCustomPlot * plot = getGammaPlot( ch );

        plot->graph( THRESHOLD_CURVE_ID )->clearData();

        x[0] = x[1] = DOUBLE( m_threshold[ch] ) / SCALING_FACTOR;
        y[0] = 0.0f;
        y[1] = 1.0f;

        plot->graph( THRESHOLD_CURVE_ID )->setData( x, y );
    }

    void computeLinearGammaLine( GammaChannel ch )
    {
        QVector<double> x(2);
        QVector<double> y(2);
        
        QCustomPlot * plot = getGammaPlot( ch );

        plot->graph( LINEAR_CURVE_ID )->clearData();
        
        x[0] = y[0] = DOUBLE( m_linearBrightness[ch] ) / SCALING_FACTOR;
        x[1] = DOUBLE( m_threshold[ch] ) / SCALING_FACTOR;
        y[1] = ( DOUBLE( m_linearContrast[ch] * m_threshold[ch] ) / SCALING_FACTOR + m_linearBrightness[ch] ) / SCALING_FACTOR;

        plot->graph( LINEAR_CURVE_ID )->setData( x, y );
    }

    void computeNonLinearGammaLine( GammaChannel ch )
    {
        bool clip = false;

        QVector<double> x(101);
        QVector<double> y(101);
        
        QCustomPlot * plot = getGammaPlot( ch );

        plot->graph( NON_LINEAR_CURVE_ID )->clearData();

        double d = DOUBLE( SCALING_FACTOR - m_threshold[ch] ) / SCALING_FACTOR / 100;

        x[0] = DOUBLE( m_threshold[ch] ) / SCALING_FACTOR;
        y[0] = DOUBLE( m_contrast[ch] * qPow( x[0], DOUBLE(m_gamma[ch]) / SCALING_FACTOR ) + m_brightness[ch] ) / SCALING_FACTOR;
        for ( unsigned i = 1; i < 101; i++ )
        {
            x[i] = x[i-1] + d;
            if ( !clip )
            {
                y[i] = DOUBLE( m_contrast[ch] * qPow( x[i], DOUBLE(m_gamma[ch]) / SCALING_FACTOR ) + m_brightness[ch] ) / SCALING_FACTOR;
                if ( y[i] > 1.0f )
                {
                    clip = true;
                    y[i] = 1.0f;
                }
            }
            else
            {
                y[i] = 1.0f;
            }
        }

        plot->graph( NON_LINEAR_CURVE_ID )->setData( x, y );
    }

    void setThreshold( GammaBox::GammaChannel ch, int x )
    {
        m_threshold[ch] = x;
        computeThresholdLine( ch );
        computeLinearGammaLine( ch );
        computeNonLinearGammaLine( ch );
        getGammaPlot( ch )->replot();
    }

    void setLinearContrast( GammaBox::GammaChannel ch, int x )
    {
        m_linearContrast[ch] = x;
        computeLinearGammaLine( ch );
        getGammaPlot( ch )->replot();
    }

    void setLinearBrightness( GammaBox::GammaChannel ch, int x )
    {
        m_linearBrightness[ch] = x;
        computeLinearGammaLine( ch );
        getGammaPlot( ch )->replot();
    }

    void setContrast( GammaBox::GammaChannel ch, int x )
    {
        m_contrast[ch] = x;
        computeNonLinearGammaLine( ch );
        getGammaPlot( ch )->replot();
    }

    void setBrightness( GammaBox::GammaChannel ch, int x )
    {
        m_brightness[ch] = x;
        computeNonLinearGammaLine( ch );
        getGammaPlot( ch )->replot();
    }

    void setGamma( GammaBox::GammaChannel ch, int x )
    {
        m_gamma[ch] = x;
        computeNonLinearGammaLine( ch );
        getGammaPlot( ch )->replot();
    }

    void setValues
    ( 
        GammaBox::GammaChannel  ch,
        int                     threshold,
        int                     lcontrast,
        int                     lbrightness,
        int                     contrast,
        int                     gamma,
        int                     brightness
    )
    {
        setThreshold        ( ch, threshold );
        setLinearContrast   ( ch, lcontrast );
        setLinearBrightness ( ch, lbrightness );
        setContrast         ( ch, contrast );
        setGamma            ( ch, gamma );
        setBrightness       ( ch, brightness );

        m_synced[ch] = true;
    }

    void copyValues
    ( 
        GammaBox::GammaChannel  dest,
        GammaBox::GammaChannel  src
    )
    {
        setThreshold        ( dest, m_threshold[src] );
        setLinearContrast   ( dest, m_linearContrast[src] );
        setLinearBrightness ( dest, m_linearBrightness[src] );
        setContrast         ( dest, m_contrast[src] );
        setGamma            ( dest, m_gamma[src] );
        setBrightness       ( dest, m_brightness[src] );
    }

    bool compareValues( GammaBox::GammaChannel ch0, GammaBox::GammaChannel ch1 )
    {
        return ( ((m_threshold[ch0]        == m_threshold[ch1])        &&
                  (m_linearContrast[ch0]   == m_linearContrast[ch1])   &&
                  (m_linearBrightness[ch0] == m_linearBrightness[ch1]) &&
                  (m_contrast[ch0]         == m_contrast[ch1])         &&
                  (m_gamma[ch0]            == m_gamma[ch1])            &&
                  (m_brightness[ch0]       == m_brightness[ch1])       &&
                  (m_brightness[ch0]       == m_brightness[ch1])       ) ? true : false );
        
    }

    void updMasterValues()
    {
        if ( m_synced[Red] && m_synced[Green] && m_synced[Blue] && !m_synced[Master] )
        {
            // search the master curve
    
            // case 1: same curve on green and blue => master=green
            if ( compareValues( Green, Blue ) )
            {
                setThreshold        ( Master, m_threshold[Green] );
                setLinearContrast   ( Master, m_linearContrast[Green] );
                setLinearBrightness ( Master, m_linearBrightness[Green] );
                setContrast         ( Master, m_contrast[Green] );
                setGamma            ( Master, m_gamma[Green] );
                setBrightness       ( Master, m_brightness[Green] );
                m_synced[Master] = true;
            }

            // case 2: same curve on red/green || red/blue => master=red
            // case 3: different curves on all channel     => master=red
            else
            {
                setThreshold        ( Master, m_threshold[Red] );
                setLinearContrast   ( Master, m_linearContrast[Red] );
                setLinearBrightness ( Master, m_linearBrightness[Red] );
                setContrast         ( Master, m_contrast[Red] );
                setGamma            ( Master, m_gamma[Red] );
                setBrightness       ( Master, m_brightness[Red] );
                m_synced[Master] = true;
            }
        }
    }

    Ui::UI_GammaBox *   m_ui;               /**< ui handle */

    bool                m_enable;
    int                 m_threshold[GammaChannelMax];
    int                 m_linearContrast[GammaChannelMax];
    int                 m_linearBrightness[GammaChannelMax];
    int                 m_contrast[GammaChannelMax];
    int                 m_gamma[GammaChannelMax];
    int                 m_brightness[GammaChannelMax];

    bool                m_synced[GammaChannelMax];
};

/******************************************************************************
 * GammaBox::GammaBox
 *****************************************************************************/
GammaBox::GammaBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData( this );
    
    d_data->m_ui->btnEnable->setText( d_data->m_enable ? TEXT_DISABLE : TEXT_ENABLE );

    // signal transition
    connect( d_data->m_ui->Threshold        , SIGNAL(ValueChanged(int)), this, SLOT(onThresholdChange(int)) );
    connect( d_data->m_ui->LinearContrast   , SIGNAL(ValueChanged(int)), this, SLOT(onLinearContrastChange(int)) );
    connect( d_data->m_ui->LinearBrightness , SIGNAL(ValueChanged(int)), this, SLOT(onLinearBrightnessChange(int)) );
    connect( d_data->m_ui->Contrast         , SIGNAL(ValueChanged(int)), this, SLOT(onContrastChange(int)) );
    connect( d_data->m_ui->Gamma            , SIGNAL(ValueChanged(int)), this, SLOT(onGammaChange(int)) );
    connect( d_data->m_ui->Brightness       , SIGNAL(ValueChanged(int)), this, SLOT(onBrightnessChange(int)) );

    connect( d_data->m_ui->GammaSelect, SIGNAL(currentChanged(int)), this, SLOT(onGammaSelectChange(int)) );

    connect( d_data->m_ui->GammaPlot->xAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onMasterXRangeChanged(QCPRange)));
    connect( d_data->m_ui->GammaPlot->yAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onMasterYRangeChanged(QCPRange)));
    
    connect( d_data->m_ui->GammaPlotRed->xAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onRedXRangeChanged(QCPRange)));
    connect( d_data->m_ui->GammaPlotRed->yAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onRedYRangeChanged(QCPRange)));
    
    connect( d_data->m_ui->GammaPlotGreen->xAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onGreenXRangeChanged(QCPRange)));
    connect( d_data->m_ui->GammaPlotGreen->yAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onGreenYRangeChanged(QCPRange)));
    
    connect( d_data->m_ui->GammaPlotBlue->xAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onBlueXRangeChanged(QCPRange)));
    connect( d_data->m_ui->GammaPlotBlue->yAxis , SIGNAL(rangeChanged(QCPRange)), this, SLOT(onBlueYRangeChanged(QCPRange)));

    // connect buttons
    connect( d_data->m_ui->btnEnable  , SIGNAL(clicked()), this, SLOT(onEnableClicked()));
    connect( d_data->m_ui->btnStraight, SIGNAL(clicked()), this, SLOT(onStraightClicked()));
    connect( d_data->m_ui->btnRec709  , SIGNAL(clicked()), this, SLOT(onRec709Clicked()));
    connect( d_data->m_ui->btnBt601   , SIGNAL(clicked()), this, SLOT(onBt601Clicked()));
    connect( d_data->m_ui->btnApply   , SIGNAL(clicked()), this, SLOT(onApplyClicked()));
}

/******************************************************************************
 * GammaBox::~GammaBox
 *****************************************************************************/
GammaBox::~GammaBox()
{
    delete d_data;
}

/******************************************************************************
 * GammaBox::GammaEnable
 *****************************************************************************/
bool GammaBox::GammaEnable() const
{
    return ( d_data->m_enable );
}

/******************************************************************************
 * GammaBox::setGammaEnable
 *****************************************************************************/
void GammaBox::setGammaEnable( const bool value )
{
    d_data->m_ui->btnEnable->setText( value ? TEXT_DISABLE : TEXT_ENABLE );
    d_data->m_enable = value;
    
    emit GammaEnableChanged( value ? 1 : 0 );
}

/******************************************************************************
 * GammaBox::Threshold
 *****************************************************************************/
int GammaBox::Threshold( const GammaBox::GammaChannel ch ) const
{
    return ( (ch < GammaChannelMax) ? d_data->m_threshold[ch] : 0 );
}

/******************************************************************************
 * GammaBox::Gamma
 *****************************************************************************/
int GammaBox::Gamma( const GammaBox::GammaChannel ch ) const
{
    return ( (ch < GammaChannelMax) ? d_data->m_gamma[ch] : 0 );
}

/******************************************************************************
 * GammaBox::LinearContrast
 *****************************************************************************/
int GammaBox::LinearContrast( const GammaBox::GammaChannel ch ) const
{
    return ( (ch < GammaChannelMax) ? d_data->m_linearContrast[ch] : 0 );
}

/******************************************************************************
 * GammaBox::LinearBrightness
 *****************************************************************************/
int GammaBox::LinearBrightness( const GammaBox::GammaChannel ch ) const
{
    return ( (ch < GammaChannelMax) ? d_data->m_linearBrightness[ch] : 0 );
}

/******************************************************************************
 * GammaBox::Contrast
 *****************************************************************************/
int GammaBox::Contrast( const GammaBox::GammaChannel ch ) const
{
    return ( (ch < GammaChannelMax) ? d_data->m_contrast[ch] : 0 );
}

/******************************************************************************
 * GammaBox::Brightness
 *****************************************************************************/
int GammaBox::Brightness( const GammaBox::GammaChannel ch ) const
{
    return ( (ch < GammaChannelMax) ? d_data->m_brightness[ch] : 0 );
}

/******************************************************************************
 * GammaBox::setGammaCurve
 *****************************************************************************/
void GammaBox::setGammaCurve
(
    const GammaBox::GammaChannel ch,
    const int                    threshold,
    const int                    gamma,
    const int                    linearContrast,
    const int                    linearBrightness,
    const int                    contrast,
    const int                    brightness
)
{
    if ( ch < GammaChannelMax )
    {
        d_data->setThreshold( ch, threshold );
        d_data->setGamma( ch, gamma );
        d_data->setLinearContrast( ch, linearContrast );
        d_data->setLinearBrightness( ch, linearBrightness );
        d_data->setContrast( ch, contrast );
        d_data->setBrightness( ch, brightness );

        switch ( ch )
        {
            case GammaBox::Master:
                emit GammaChanged( d_data->m_threshold[ch],
                                   d_data->m_linearContrast[ch],
                                   d_data->m_linearBrightness[ch],
                                   d_data->m_contrast[ch],
                                   d_data->m_gamma[ch],
                                   d_data->m_brightness[ch] );

                d_data->copyValues( GammaBox::Red  , GammaBox::Master );
                d_data->copyValues( GammaBox::Green, GammaBox::Master );
                d_data->copyValues( GammaBox::Blue , GammaBox::Master );
                break;

            case GammaBox::Red:
                emit GammaRedChanged( d_data->m_threshold[ch],
                                      d_data->m_linearContrast[ch],
                                      d_data->m_linearBrightness[ch],
                                      d_data->m_contrast[ch],
                                      d_data->m_gamma[ch],
                                      d_data->m_brightness[ch] );
                break;

            case GammaBox::Green:
                emit GammaGreenChanged( d_data->m_threshold[ch],
                                        d_data->m_linearContrast[ch],
                                        d_data->m_linearBrightness[ch],
                                        d_data->m_contrast[ch],
                                        d_data->m_gamma[ch],
                                        d_data->m_brightness[ch] );
                break;

            case GammaBox::Blue:
                emit GammaBlueChanged( d_data->m_threshold[ch],
                                       d_data->m_linearContrast[ch],
                                       d_data->m_linearBrightness[ch],
                                       d_data->m_contrast[ch],
                                       d_data->m_gamma[ch],
                                       d_data->m_brightness[ch] );
                break;

            default:
                break;
        }
    }
}


/******************************************************************************
 * GammaBox::prepareMode
 *****************************************************************************/
void GammaBox::prepareMode( const Mode )
{
    // @TODO do nothing here
}

/******************************************************************************
 * GammaBox::loadSettings (no looping over channels)
 *****************************************************************************/
void GammaBox::loadSettings( QSettings & s )
{
    int threshold, gamma, linearContrast, linearBrightness, contrast, brightness;

    s.beginGroup( GAMMA_SETTINGS_SECTION_NAME );

    threshold        = s.value( GAMMA_SETTINGS_THRESHOLD_MASTER ).toInt();
    gamma            = s.value( GAMMA_SETTINGS_GAMMA_MASTER ).toInt();
    linearContrast   = s.value( GAMMA_SETTINGS_LINEAR_CONTRAST_MASTER ).toInt();
    linearBrightness = s.value( GAMMA_SETTINGS_LINEAR_BRIGHTNESS_MASTER ).toInt();
    contrast         = s.value( GAMMA_SETTINGS_CONTRAST_MASTER ).toInt();
    brightness       = s.value( GAMMA_SETTINGS_BRIGHTNESS_MASTER ).toInt();
    setGammaCurve( Master, threshold, gamma, linearContrast, linearBrightness, contrast, brightness );

    threshold        = s.value( GAMMA_SETTINGS_THRESHOLD_RED ).toInt();
    gamma            = s.value( GAMMA_SETTINGS_GAMMA_RED ).toInt();
    linearContrast   = s.value( GAMMA_SETTINGS_LINEAR_CONTRAST_RED ).toInt();
    linearBrightness = s.value( GAMMA_SETTINGS_LINEAR_BRIGHTNESS_RED ).toInt();
    contrast         = s.value( GAMMA_SETTINGS_CONTRAST_RED ).toInt();
    brightness       = s.value( GAMMA_SETTINGS_BRIGHTNESS_RED ).toInt();
    setGammaCurve( Red, threshold, gamma, linearContrast, linearBrightness, contrast, brightness );

    threshold        = s.value( GAMMA_SETTINGS_THRESHOLD_GREEN ).toInt();
    gamma            = s.value( GAMMA_SETTINGS_GAMMA_GREEN ).toInt();
    linearContrast   = s.value( GAMMA_SETTINGS_LINEAR_CONTRAST_GREEN ).toInt();
    linearBrightness = s.value( GAMMA_SETTINGS_LINEAR_BRIGHTNESS_GREEN ).toInt();
    contrast         = s.value( GAMMA_SETTINGS_CONTRAST_GREEN ).toInt();
    brightness       = s.value( GAMMA_SETTINGS_BRIGHTNESS_GREEN ).toInt();
    setGammaCurve( Green, threshold, gamma, linearContrast, linearBrightness, contrast, brightness );

    threshold        = s.value( GAMMA_SETTINGS_THRESHOLD_BLUE ).toInt();
    gamma            = s.value( GAMMA_SETTINGS_GAMMA_BLUE ).toInt();
    linearContrast   = s.value( GAMMA_SETTINGS_LINEAR_CONTRAST_BLUE ).toInt();
    linearBrightness = s.value( GAMMA_SETTINGS_LINEAR_BRIGHTNESS_BLUE ).toInt();
    contrast         = s.value( GAMMA_SETTINGS_CONTRAST_BLUE ).toInt();
    brightness       = s.value( GAMMA_SETTINGS_BRIGHTNESS_BLUE ).toInt();
    setGammaCurve( Blue, threshold, gamma, linearContrast, linearBrightness, contrast, brightness );

    setGammaEnable( s.value( GAMMA_SETTINGS_ENABLE ).toBool() );

    s.endGroup();
}

/******************************************************************************
 * GammaBox::saveSettings (no looping over channels)
 *****************************************************************************/
void GammaBox::saveSettings( QSettings & s )
{
    s.beginGroup( GAMMA_SETTINGS_SECTION_NAME );

    s.setValue( GAMMA_SETTINGS_THRESHOLD_MASTER         , Threshold( GammaBox::Master ) );
    s.setValue( GAMMA_SETTINGS_THRESHOLD_RED            , Threshold( GammaBox::Red ) );
    s.setValue( GAMMA_SETTINGS_THRESHOLD_GREEN          , Threshold( GammaBox::Green ) );
    s.setValue( GAMMA_SETTINGS_THRESHOLD_BLUE           , Threshold( GammaBox::Blue ) );

    s.setValue( GAMMA_SETTINGS_GAMMA_MASTER             , Gamma( GammaBox::Master ) );
    s.setValue( GAMMA_SETTINGS_GAMMA_RED                , Gamma( GammaBox::Red ) );
    s.setValue( GAMMA_SETTINGS_GAMMA_GREEN              , Gamma( GammaBox::Green ) );
    s.setValue( GAMMA_SETTINGS_GAMMA_BLUE               , Gamma( GammaBox::Blue ) );

    s.setValue( GAMMA_SETTINGS_LINEAR_CONTRAST_MASTER   , LinearContrast( GammaBox::Master ) );
    s.setValue( GAMMA_SETTINGS_LINEAR_CONTRAST_RED      , LinearContrast( GammaBox::Red ) );
    s.setValue( GAMMA_SETTINGS_LINEAR_CONTRAST_GREEN    , LinearContrast( GammaBox::Green ) );
    s.setValue( GAMMA_SETTINGS_LINEAR_CONTRAST_BLUE     , LinearContrast( GammaBox::Blue ) );
    
    s.setValue( GAMMA_SETTINGS_LINEAR_BRIGHTNESS_MASTER , LinearBrightness( GammaBox::Master ) );
    s.setValue( GAMMA_SETTINGS_LINEAR_BRIGHTNESS_RED    , LinearBrightness( GammaBox::Red ) );
    s.setValue( GAMMA_SETTINGS_LINEAR_BRIGHTNESS_GREEN  , LinearBrightness( GammaBox::Green ) );
    s.setValue( GAMMA_SETTINGS_LINEAR_BRIGHTNESS_BLUE   , LinearBrightness( GammaBox::Blue ) );

    s.setValue( GAMMA_SETTINGS_CONTRAST_MASTER          , Contrast( GammaBox::Master ) );
    s.setValue( GAMMA_SETTINGS_CONTRAST_RED             , Contrast( GammaBox::Red ) );
    s.setValue( GAMMA_SETTINGS_CONTRAST_GREEN           , Contrast( GammaBox::Green ) );
    s.setValue( GAMMA_SETTINGS_CONTRAST_BLUE            , Contrast( GammaBox::Blue ) );
                                                       
    s.setValue( GAMMA_SETTINGS_BRIGHTNESS_MASTER        , Brightness( GammaBox::Master ) );
    s.setValue( GAMMA_SETTINGS_BRIGHTNESS_RED           , Brightness( GammaBox::Red ) );
    s.setValue( GAMMA_SETTINGS_BRIGHTNESS_GREEN         , Brightness( GammaBox::Green ) );
    s.setValue( GAMMA_SETTINGS_BRIGHTNESS_BLUE          , Brightness( GammaBox::Blue ) );
    
    s.setValue( GAMMA_SETTINGS_ENABLE                   , GammaEnable() );

    s.endGroup();
}

/******************************************************************************
 * GammaBox::applySettings
 *****************************************************************************/
void GammaBox::applySettings( void )
{
    // Gamma settings for each channel
    for ( int ch = 0; ch < GammaChannelMax; ch++ )
    {
        emit GammaChanged( d_data->m_threshold[ch],
                           d_data->m_linearContrast[ch],
                           d_data->m_linearBrightness[ch],
                           d_data->m_contrast[ch],
                           d_data->m_gamma[ch],
                           d_data->m_brightness[ch] );
    }

    // Gamma enable
    emit GammaEnableChanged( d_data->m_enable ? 1 : 0 );
}

/******************************************************************************
 * GammaBox::onGammaEnableChange
 *****************************************************************************/
void GammaBox::onGammaEnableChange( int value )
{
    d_data->m_ui->btnEnable->setText( value ? TEXT_DISABLE : TEXT_ENABLE );
    d_data->m_enable = value ? true : false;
}

/******************************************************************************
 * GammaBox::onGammaRedChange
 *****************************************************************************/
void GammaBox::onGammaRedChange
(
    int threshold,
    int lcontrast,
    int lbrightness,
    int contrast,
    int gamma,
    int brightness
)
{
    d_data->setValues( GammaBox::Red,
                threshold, lcontrast, lbrightness, contrast, gamma, brightness );
    d_data->updMasterValues();
}

/******************************************************************************
 * GammaBox::onGammaGreenChange
 *****************************************************************************/
void GammaBox::onGammaGreenChange
(
    int threshold,
    int lcontrast,
    int lbrightness,
    int contrast,
    int gamma,
    int brightness
)
{
    d_data->setValues( GammaBox::Green,
                threshold, lcontrast, lbrightness, contrast, gamma, brightness );
    d_data->updMasterValues();
}

/******************************************************************************
 * GammaBox::onGammaBlueChange
 *****************************************************************************/
void GammaBox::onGammaBlueChange
(
    int threshold,
    int lcontrast,
    int lbrightness,
    int contrast,
    int gamma,
    int brightness
)
{
    d_data->setValues( GammaBox::Blue,
                threshold, lcontrast, lbrightness, contrast, gamma, brightness );
    d_data->updMasterValues();
}

/******************************************************************************
 * GammaBox::onThresholdChange
 *****************************************************************************/
void GammaBox::onThresholdChange( int value )
{
    d_data->setThreshold( (GammaBox::GammaChannel)d_data->m_ui->GammaSelect->currentIndex(), value );
}

/******************************************************************************
 * GammaBox::onLinearContrastChange
 *****************************************************************************/
void GammaBox::onLinearContrastChange( int value )
{
    d_data->setLinearContrast( (GammaBox::GammaChannel)d_data->m_ui->GammaSelect->currentIndex(), value );
}

/******************************************************************************
 * GammaBox::onLinearBrightnessChange
 *****************************************************************************/
void GammaBox::onLinearBrightnessChange( int value )
{
    d_data->setLinearBrightness( (GammaBox::GammaChannel)d_data->m_ui->GammaSelect->currentIndex(), value );
}

/******************************************************************************
 * GammaBox::onContrastChange
 *****************************************************************************/
void GammaBox::onContrastChange( int value )
{
    d_data->setContrast( (GammaBox::GammaChannel)d_data->m_ui->GammaSelect->currentIndex(), value );
}

/******************************************************************************
 * GammaBox::onGammaChange
 *****************************************************************************/
void GammaBox::onGammaChange( int value )
{
    d_data->setGamma( (GammaBox::GammaChannel)d_data->m_ui->GammaSelect->currentIndex(), value );
}

/******************************************************************************
 * GammaBox::onBrightnessChange
 *****************************************************************************/
void GammaBox::onBrightnessChange( int value )
{
    d_data->setBrightness( (GammaBox::GammaChannel)d_data->m_ui->GammaSelect->currentIndex(), value );
}

/******************************************************************************
 * GammaBox::onGammaSelectChange
 *****************************************************************************/
void GammaBox::onGammaSelectChange( int value )
{
    GammaBox::GammaChannel ch = (GammaBox::GammaChannel)value;
    d_data->m_ui->Threshold->setValue       ( d_data->m_threshold[ch] );
    d_data->m_ui->LinearContrast->setValue  ( d_data->m_linearContrast[ch] );
    d_data->m_ui->LinearBrightness->setValue( d_data->m_linearBrightness[ch] );
    d_data->m_ui->Contrast->setValue        ( d_data->m_contrast[ch] );
    d_data->m_ui->Gamma->setValue           ( d_data->m_gamma[ch] );
    d_data->m_ui->Brightness->setValue      ( d_data->m_brightness[ch] );
}

/******************************************************************************
 * GammaBox::SetXRangeChanged
 *****************************************************************************/
void GammaBox::SetXRangeChanged( const QCPRange &range, QCustomPlot * plot )
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
 * GammaBox::SetYRangeChanged
 *****************************************************************************/
void GammaBox::SetYRangeChanged( const QCPRange &range, QCustomPlot * plot )
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
 * GammaBox::onMasterXRangeChanged
 *****************************************************************************/
void GammaBox::onMasterXRangeChanged( const QCPRange &range )
{
    SetXRangeChanged( range, d_data->m_ui->GammaPlot );
}

/******************************************************************************
 * GammaBox::onMasterYRangeChanged
 *****************************************************************************/
void GammaBox::onMasterYRangeChanged( const QCPRange &range )
{
    SetYRangeChanged( range, d_data->m_ui->GammaPlot );
}

/******************************************************************************
 * GammaBox::onRedXRangeChanged
 *****************************************************************************/
void GammaBox::onRedXRangeChanged( const QCPRange &range )
{
    SetXRangeChanged( range, d_data->m_ui->GammaPlotRed );
}

/******************************************************************************
 * GammaBox::onRedYRangeChanged
 *****************************************************************************/
void GammaBox::onRedYRangeChanged( const QCPRange &range )
{
    SetYRangeChanged( range, d_data->m_ui->GammaPlotRed );
}

/******************************************************************************
 * GammaBox::onGreenXRangeChanged
 *****************************************************************************/
void GammaBox::onGreenXRangeChanged( const QCPRange &range )
{
    SetXRangeChanged( range, d_data->m_ui->GammaPlotGreen );
}

/******************************************************************************
 * GammaBox::onGreenYRangeChanged
 *****************************************************************************/
void GammaBox::onGreenYRangeChanged( const QCPRange &range )
{
    SetYRangeChanged( range, d_data->m_ui->GammaPlotGreen );
}

/******************************************************************************
 * GammaBox::onBlueXRangeChanged
 *****************************************************************************/
void GammaBox::onBlueXRangeChanged( const QCPRange &range )
{
    SetXRangeChanged( range, d_data->m_ui->GammaPlotBlue );
}

/******************************************************************************
 * GammaBox::onBlueYRangeChanged
 *****************************************************************************/
void GammaBox::onBlueYRangeChanged( const QCPRange &range )
{
    SetYRangeChanged( range, d_data->m_ui->GammaPlotBlue );
}

/******************************************************************************
 * GammaBox::onEnableClicked
 *****************************************************************************/
void GammaBox::onEnableClicked()
{
    bool value = !d_data->m_enable;
    d_data->m_ui->btnEnable->setText( value ? TEXT_DISABLE : TEXT_ENABLE );
    d_data->m_enable = value;
    emit GammaEnableChanged( value ? 1 : 0 );
}

/******************************************************************************
 * GammaBox::onStraightClicked
 *****************************************************************************/
void GammaBox::onStraightClicked()
{
    GammaBox::GammaChannel ch = (GammaBox::GammaChannel)d_data->m_ui->GammaSelect->currentIndex();
    
    d_data->setValues( ch,
                STRAIGHT_THRESHOLD,
                STRAIGHT_LINEAR_CONTRAST,
                STRAIGHT_LINEAR_BRIGHTNESS,
                STRAIGHT_CONTRAST,
                STRAIGHT_GAMMA,
                STRAIGHT_BRIGHTNESS );

    d_data->m_ui->Threshold->setValue       ( d_data->m_threshold[ch] );
    d_data->m_ui->LinearContrast->setValue  ( d_data->m_linearContrast[ch] );
    d_data->m_ui->LinearBrightness->setValue( d_data->m_linearBrightness[ch] );
    d_data->m_ui->Contrast->setValue        ( d_data->m_contrast[ch] );
    d_data->m_ui->Gamma->setValue           ( d_data->m_gamma[ch] );
    d_data->m_ui->Brightness->setValue      ( d_data->m_brightness[ch] );

    switch ( ch )
    {
        case GammaBox::Master:
            emit GammaChanged( d_data->m_threshold[ch],
                               d_data->m_linearContrast[ch],
                               d_data->m_linearBrightness[ch],
                               d_data->m_contrast[ch],
                               d_data->m_gamma[ch],
                               d_data->m_brightness[ch] );

            d_data->copyValues( GammaBox::Red  , GammaBox::Master );
            d_data->copyValues( GammaBox::Green, GammaBox::Master );
            d_data->copyValues( GammaBox::Blue , GammaBox::Master );
            break;

        case GammaBox::Red:
            emit GammaRedChanged( d_data->m_threshold[ch],
                                  d_data->m_linearContrast[ch],
                                  d_data->m_linearBrightness[ch],
                                  d_data->m_contrast[ch],
                                  d_data->m_gamma[ch],
                                  d_data->m_brightness[ch] );
            break;

        case GammaBox::Green:
            emit GammaGreenChanged( d_data->m_threshold[ch],
                                    d_data->m_linearContrast[ch],
                                    d_data->m_linearBrightness[ch],
                                    d_data->m_contrast[ch],
                                    d_data->m_gamma[ch],
                                    d_data->m_brightness[ch] );
            break;

        case GammaBox::Blue:
            emit GammaBlueChanged( d_data->m_threshold[ch],
                                   d_data->m_linearContrast[ch],
                                   d_data->m_linearBrightness[ch],
                                   d_data->m_contrast[ch],
                                   d_data->m_gamma[ch],
                                   d_data->m_brightness[ch] );
            break;

        default:
            break;
    }
}

/******************************************************************************
 * GammaBox::onRec709Clicked
 *****************************************************************************/
void GammaBox::onRec709Clicked()
{
    GammaBox::GammaChannel ch = (GammaBox::GammaChannel)d_data->m_ui->GammaSelect->currentIndex();
    
    d_data->setValues( ch,
                REC709_THRESHOLD,
                REC709_LINEAR_CONTRAST,
                REC709_LINEAR_BRIGHTNESS,
                REC709_CONTRAST,
                REC709_GAMMA,
                REC709_BRIGHTNESS );

    d_data->m_ui->Threshold->setValue       ( d_data->m_threshold[ch] );
    d_data->m_ui->LinearContrast->setValue  ( d_data->m_linearContrast[ch] );
    d_data->m_ui->LinearBrightness->setValue( d_data->m_linearBrightness[ch] );
    d_data->m_ui->Contrast->setValue        ( d_data->m_contrast[ch] );
    d_data->m_ui->Gamma->setValue           ( d_data->m_gamma[ch] );
    d_data->m_ui->Brightness->setValue      ( d_data->m_brightness[ch] );

    switch ( ch )
    {
        case GammaBox::Master:
            emit GammaChanged( d_data->m_threshold[ch],
                               d_data->m_linearContrast[ch],
                               d_data->m_linearBrightness[ch],
                               d_data->m_contrast[ch],
                               d_data->m_gamma[ch],
                               d_data->m_brightness[ch] );

            d_data->copyValues( GammaBox::Red  , GammaBox::Master );
            d_data->copyValues( GammaBox::Green, GammaBox::Master );
            d_data->copyValues( GammaBox::Blue , GammaBox::Master );
            break;

        case GammaBox::Red:
            emit GammaRedChanged( d_data->m_threshold[ch],
                                  d_data->m_linearContrast[ch],
                                  d_data->m_linearBrightness[ch],
                                  d_data->m_contrast[ch],
                                  d_data->m_gamma[ch],
                                  d_data->m_brightness[ch] );
            break;

        case GammaBox::Green:
            emit GammaGreenChanged( d_data->m_threshold[ch],
                                    d_data->m_linearContrast[ch],
                                    d_data->m_linearBrightness[ch],
                                    d_data->m_contrast[ch],
                                    d_data->m_gamma[ch],
                                    d_data->m_brightness[ch] );
            break;

        case GammaBox::Blue:
            emit GammaBlueChanged( d_data->m_threshold[ch],
                                   d_data->m_linearContrast[ch],
                                   d_data->m_linearBrightness[ch],
                                   d_data->m_contrast[ch],
                                   d_data->m_gamma[ch],
                                   d_data->m_brightness[ch] );
            break;

        default:
            break;
    }
}

/******************************************************************************
 * GammaBox::onBt601Clicked
 *****************************************************************************/
void GammaBox::onBt601Clicked()
{
    GammaBox::GammaChannel ch = (GammaBox::GammaChannel)d_data->m_ui->GammaSelect->currentIndex();
    
    d_data->setValues( ch,
                BT601_THRESHOLD,
                BT601_LINEAR_CONTRAST,
                BT601_LINEAR_BRIGHTNESS,
                BT601_CONTRAST,
                BT601_GAMMA,
                BT601_BRIGHTNESS );

    d_data->m_ui->Threshold->setValue       ( d_data->m_threshold[ch] );
    d_data->m_ui->LinearContrast->setValue  ( d_data->m_linearContrast[ch] );
    d_data->m_ui->LinearBrightness->setValue( d_data->m_linearBrightness[ch] );
    d_data->m_ui->Contrast->setValue        ( d_data->m_contrast[ch] );
    d_data->m_ui->Gamma->setValue           ( d_data->m_gamma[ch] );
    d_data->m_ui->Brightness->setValue      ( d_data->m_brightness[ch] );

    switch ( ch )
    {
        case GammaBox::Master:
            emit GammaChanged( d_data->m_threshold[ch],
                               d_data->m_linearContrast[ch],
                               d_data->m_linearBrightness[ch],
                               d_data->m_contrast[ch],
                               d_data->m_gamma[ch],
                               d_data->m_brightness[ch] );

            d_data->copyValues( GammaBox::Red  , GammaBox::Master );
            d_data->copyValues( GammaBox::Green, GammaBox::Master );
            d_data->copyValues( GammaBox::Blue , GammaBox::Master );
            break;

        case GammaBox::Red:
            emit GammaRedChanged( d_data->m_threshold[ch],
                                  d_data->m_linearContrast[ch],
                                  d_data->m_linearBrightness[ch],
                                  d_data->m_contrast[ch],
                                  d_data->m_gamma[ch],
                                  d_data->m_brightness[ch] );
            break;

        case GammaBox::Green:
            emit GammaGreenChanged( d_data->m_threshold[ch],
                                    d_data->m_linearContrast[ch],
                                    d_data->m_linearBrightness[ch],
                                    d_data->m_contrast[ch],
                                    d_data->m_gamma[ch],
                                    d_data->m_brightness[ch] );
            break;

        case GammaBox::Blue:
            emit GammaBlueChanged( d_data->m_threshold[ch],
                                   d_data->m_linearContrast[ch],
                                   d_data->m_linearBrightness[ch],
                                   d_data->m_contrast[ch],
                                   d_data->m_gamma[ch],
                                   d_data->m_brightness[ch] );
            break;

        default:
            break;
    }
}

/******************************************************************************
 * GammaBox::onApplyClicked
 *****************************************************************************/
void GammaBox::onApplyClicked()
{
    GammaBox::GammaChannel ch = (GammaBox::GammaChannel)d_data->m_ui->GammaSelect->currentIndex();

    switch ( ch )
    {
        case GammaBox::Master:
            emit GammaChanged( d_data->m_threshold[ch],
                               d_data->m_linearContrast[ch],
                               d_data->m_linearBrightness[ch],
                               d_data->m_contrast[ch],
                               d_data->m_gamma[ch],
                               d_data->m_brightness[ch] );

            d_data->copyValues( GammaBox::Red  , GammaBox::Master );
            d_data->copyValues( GammaBox::Green, GammaBox::Master );
            d_data->copyValues( GammaBox::Blue , GammaBox::Master );
            break;

        case GammaBox::Red:
            emit GammaRedChanged( d_data->m_threshold[ch],
                                  d_data->m_linearContrast[ch],
                                  d_data->m_linearBrightness[ch],
                                  d_data->m_contrast[ch],
                                  d_data->m_gamma[ch],
                                  d_data->m_brightness[ch] );
            break;

        case GammaBox::Green:
            emit GammaGreenChanged( d_data->m_threshold[ch],
                                    d_data->m_linearContrast[ch],
                                    d_data->m_linearBrightness[ch],
                                    d_data->m_contrast[ch],
                                    d_data->m_gamma[ch],
                                    d_data->m_brightness[ch] );
            break;

        case GammaBox::Blue:
            emit GammaBlueChanged( d_data->m_threshold[ch],
                                   d_data->m_linearContrast[ch],
                                   d_data->m_linearBrightness[ch],
                                   d_data->m_contrast[ch],
                                   d_data->m_gamma[ch],
                                   d_data->m_brightness[ch] );
            break;

        default:
            break;
    }
}

