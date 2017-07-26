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
 * @file    gammabox.cpp
 *
 * @brief   Implementation of gamma setting box
 *
 *****************************************************************************/
#include <QtDebug>

#include "knee_interpolation.h"
#include "kneebox.h"
#include "ui_kneebox.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_KneeBox;
}

/******************************************************************************
 * local definitions
 *****************************************************************************/
#define KNEE_CURVE_ID                   ( 0 )
#define KNEE_POINT_CURVE_ID             ( 1 )
#define WHITE_CLIP_CURVE_ID             ( 2 )

/******************************************************************************
 * Settings
 *****************************************************************************/
#define KNEE_SETTINGS_SECTION_NAME      ( "KNEE" )
#define KNEE_SETTINGS_ENABLE            ( "enable" )
#define KNEE_SETTINGS_POINT             ( "point" )
#define KNEE_SETTINGS_SLOPE             ( "slope" )
#define KNEE_SETTINGS_CLIP              ( "clip" )

/******************************************************************************
 * KneeBox::PrivateData
 *****************************************************************************/
class KneeBox::PrivateData
{
public:
    PrivateData( QWidget * parent )
        : m_ui( new Ui::UI_KneeBox )
        , m_interpolate( new KneeInterpolation )
    {
        // initialize UI
        m_ui->setupUi( parent );

        // kneepoint
        m_ui->KneePoint->setRange( KNEE_POINT_MIN, KNEE_POINT_MAX );
        m_ui->KneePoint->setCommaPos( KNEE_POINT_COMMA_POSITION );
        m_ui->KneePoint->setDisplayMultiplier( KNEE_POINT_DISPLAY_MULTIPLIER );
        m_ui->KneePoint->setBase( KNEE_POINT_BASE );
        m_ui->KneePoint->setMaxAngle( 180 );
        m_ui->KneePoint->setMaxRounds( 1 );
        m_ui->KneePoint->setFmt( KNEE_POINT_DISPLAY_MASK );
        m_ui->KneePoint->setMaxEvent( 20 );

        // kneeslope
        m_ui->KneeSlope->setRange( KNEE_SLOPE_MIN, KNEE_SLOPE_MAX );
        m_ui->KneeSlope->setCommaPos( KNEE_SLOPE_COMMA_POSITION );
        m_ui->KneeSlope->setDisplayMultiplier( KNEE_SLOPE_DISPLAY_MULTIPLIER );
        m_ui->KneeSlope->setBase( KNEE_SLOPE_BASE );
        m_ui->KneeSlope->setMaxAngle( 180 );
        m_ui->KneeSlope->setMaxRounds( 4 );
        m_ui->KneeSlope->setFmt( KNEE_SLOPE_DISPLAY_MASK );
        m_ui->KneeSlope->setMaxEvent( 100 );

        // whiteclip
        m_ui->WhiteClip->setRange( WHITE_CLIP_MIN, WHITE_CLIP_MAX );
        m_ui->WhiteClip->setCommaPos( WHITE_CLIP_COMMA_POSITION );
        m_ui->WhiteClip->setDisplayMultiplier( WHITE_CLIP_DISPLAY_MULTIPLIER );
        m_ui->WhiteClip->setBase( WHITE_CLIP_BASE );
        m_ui->WhiteClip->setMaxAngle( 180 );
        m_ui->WhiteClip->setMaxRounds( 1 );
        m_ui->WhiteClip->setFmt( WHITE_CLIP_DISPLAY_MASK );
        m_ui->WhiteClip->setMaxEvent( 20 );

        // knee-plot
        initKneePlot( m_ui->KneePlot );
        QLinearGradient axisRectGradient;
        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(120, 120, 120));
        axisRectGradient.setColorAt(1, QColor(48, 48, 48));
        m_ui->KneePlot->axisRect()->setBackground(axisRectGradient);
    };

    ~PrivateData()
    {
        delete m_ui;
        delete m_interpolate;
    };

    void initKneePlot( QCustomPlot * plot )
    {
        // knee curve
        QPen penKnee( Qt::white );
        plot->addGraph();
        plot->graph( KNEE_CURVE_ID )->setPen( penKnee );

        // knee point curve
        QPen penKneePoint( Qt::cyan );
        plot->addGraph();
        plot->graph( KNEE_POINT_CURVE_ID )->setPen( penKneePoint );
 
        // white clip curve
        QPen penWhiteClip( Qt::yellow );
        plot->addGraph();
        plot->graph( WHITE_CLIP_CURVE_ID )->setPen( penWhiteClip );

        plot->setBackground( QBrush(QColor(48,47,47)) );
        plot->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom );
        plot->axisRect()->setupFullAxesBox(true);
        plot->xAxis->setLabel( "" );
        plot->xAxis->setLabelColor( QColor(177, 177, 177) );
        plot->yAxis->setLabel( "" );
        plot->yAxis->setLabelColor( QColor(177, 177, 177) );
        plot->xAxis->setRange( 0, 400 );
        plot->yAxis->setRange( 0, 110 );
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

    void setKneeConfig( int point, int slope, int clip )
    {
        m_ui->KneePlot->graph( KNEE_CURVE_ID )->clearData();
        m_ui->KneePlot->graph( KNEE_POINT_CURVE_ID )->clearData();
        m_ui->KneePlot->graph( WHITE_CLIP_CURVE_ID )->clearData();

        QVector<double> x( 1024 );
        QVector<double> y( 1024 );
        m_interpolate->setConfig( point, slope, clip );
        for ( int i=0; i<1024; i+=1 )
        {
            x[i] = (((double)(i<<2)) * 400.0f) / 4096.0f;
            y[i] = (((double)(m_interpolate->interpolate( (i<<2) ) * (i<<2))) / 16384.0f * clip) / 4096.0f;
        }

        QVector<double> x1( 2 );
        QVector<double> y1( 2 );
        x1[0] =   0.0f;
        x1[1] = 400.0f;
        y1[0] = y1[1] = powf( (double)point/clip, 1/0.45f ) * clip;
 
        QVector<double> x2( 2 );
        QVector<double> y2( 2 );
        x2[0] =   0.0f;
        x2[1] = 400.0f;
        y2[0] = y2[1] = (double)clip;
 
        m_ui->KneePlot->graph( KNEE_CURVE_ID )->setData( x, y );
        m_ui->KneePlot->graph( KNEE_POINT_CURVE_ID )->setData( x1, y1 );
        m_ui->KneePlot->graph( WHITE_CLIP_CURVE_ID )->setData( x2, y2 );

        m_ui->KneePlot->replot();
    }

    Ui::UI_KneeBox *        m_ui;
    KneeInterpolation *     m_interpolate;  /**< interpolation class */
};

/******************************************************************************
 * KneeBox::KneeBox
 *****************************************************************************/
KneeBox::KneeBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData( this );

    // signal transition
    connect( d_data->m_ui->cbxEnable, SIGNAL(stateChanged(int)), this, SLOT(onKneeEnableChange(int)) );

    connect( d_data->m_ui->KneePoint, SIGNAL(ValueChanged(int)), this, SLOT(onKneePointChange(int)) );
    connect( d_data->m_ui->KneeSlope, SIGNAL(ValueChanged(int)), this, SLOT(onKneeSlopeChange(int)) );
    connect( d_data->m_ui->WhiteClip, SIGNAL(ValueChanged(int)), this, SLOT(onWhiteClipChange(int)) );
}

/******************************************************************************
 * KneeBox::~KneeBox
 *****************************************************************************/
KneeBox::~KneeBox()
{
    delete d_data;
}

/******************************************************************************
 * KneeBox::KneeEnable
 *****************************************************************************/
bool KneeBox::KneeEnable() const
{
    return ( d_data->m_ui->cbxEnable->isChecked() );
}

/******************************************************************************
 * KneeBox::setKneeEnable
 *****************************************************************************/
void KneeBox::setKneeEnable( const bool enable )
{
    d_data->m_ui->cbxEnable->setCheckState( enable ? Qt::Checked : Qt::Unchecked );
}

/******************************************************************************
 * KneeBox::KneePoint
 *****************************************************************************/
int KneeBox::KneePoint() const
{
    return ( d_data->m_ui->KneePoint->value() );
}

/******************************************************************************
 * KneeBox::setKneePoint
 *****************************************************************************/
void KneeBox::setKneePoint( const int value )
{
    d_data->setKneeConfig( value, KneeSlope(), WhiteClip() );
    if ( KneeEnable() )
    {
        emit KneeConfigChanged( (int)KneeEnable(), value, KneeSlope(), WhiteClip() );
    }
}

/******************************************************************************
 * KneeBox::KneeSlope
 *****************************************************************************/
int KneeBox::KneeSlope() const
{
    return ( d_data->m_ui->KneeSlope->value() );
}

/******************************************************************************
 * KneeBox::setKneeSlope
 *****************************************************************************/
void KneeBox::setKneeSlope( const int value )
{
    d_data->setKneeConfig( KneePoint(), value, WhiteClip() );
    if ( KneeEnable() )
    {
        emit KneeConfigChanged( (int)KneeEnable(), KneePoint(), value, WhiteClip() );
    }
}

/******************************************************************************
 * KneeBox::WhiteClip
 *****************************************************************************/
int KneeBox::WhiteClip() const
{
    return ( d_data->m_ui->WhiteClip->value() );
}

/******************************************************************************
 * KneeBox::setWhiteClip
 *****************************************************************************/
void KneeBox::setWhiteClip( const int value )
{
    d_data->setKneeConfig( KneePoint(), KneeSlope(), value );
    if ( KneeEnable() )
    {
        emit KneeConfigChanged( (int)KneeEnable(), KneePoint(), KneeSlope(), value );
    }
}

/******************************************************************************
 * KneeBox::prepareMode
 *****************************************************************************/
void KneeBox::prepareMode( const Mode )
{
    // @TODO do nothing here
}

/******************************************************************************
 * KneeBox::loadSettings (no looping over channels)
 *****************************************************************************/
void KneeBox::loadSettings( QSettings & s )
{
    s.beginGroup( KNEE_SETTINGS_SECTION_NAME );
    setKneeEnable( s.value( KNEE_SETTINGS_ENABLE ).toBool() );
    setKneePoint( s.value( KNEE_SETTINGS_POINT ).toInt() );
    setKneeSlope( s.value( KNEE_SETTINGS_SLOPE ).toInt() );
    setWhiteClip( s.value( KNEE_SETTINGS_CLIP ).toInt() );
    s.endGroup();
}

/******************************************************************************
 * KneeBox::saveSettings (no looping over channels)
 *****************************************************************************/
void KneeBox::saveSettings( QSettings & s )
{
    s.beginGroup( KNEE_SETTINGS_SECTION_NAME );
    s.setValue( KNEE_SETTINGS_ENABLE, KneeEnable() );
    s.setValue( KNEE_SETTINGS_POINT, KneePoint() );
    s.setValue( KNEE_SETTINGS_SLOPE, KneeSlope() );
    s.setValue( KNEE_SETTINGS_CLIP, WhiteClip() );
    s.endGroup();
}

/******************************************************************************
 * KneeBox::applySettings (no looping over channels)
 *****************************************************************************/
void KneeBox::applySettings( void )
{
    emit KneeConfigChanged( (int)KneeEnable(), KneePoint(), KneeSlope(), WhiteClip() );
}

/******************************************************************************
 * KneeBox::onKneeChange
 *****************************************************************************/
void KneeBox::onKneeConfigChange( int enable, int point, int slope, int clip )
{
    d_data->m_ui->cbxEnable->blockSignals( true );
    d_data->m_ui->cbxEnable->setCheckState( enable ? Qt::Checked : Qt::Unchecked );
    d_data->m_ui->cbxEnable->blockSignals( false );
    
    d_data->m_ui->KneePoint->blockSignals( true );
    d_data->m_ui->KneePoint->setValue( point );
    d_data->m_ui->KneePoint->blockSignals( false );

    d_data->m_ui->KneeSlope->blockSignals( true );
    d_data->m_ui->KneeSlope->setValue( slope );
    d_data->m_ui->KneeSlope->blockSignals( false );

    d_data->m_ui->WhiteClip->blockSignals( true );
    d_data->m_ui->WhiteClip->setValue( clip );
    d_data->m_ui->WhiteClip->blockSignals( false );
    
    d_data->setKneeConfig( point, slope, clip );
}

/******************************************************************************
 * KneeBox::onKneeEnableChange
 *****************************************************************************/
void KneeBox::onKneeEnableChange( int )
{
    setWaitCursor();
    emit KneeConfigChanged( (int)KneeEnable(), KneePoint(), KneeSlope(), WhiteClip() );
    setNormalCursor();
}

/******************************************************************************
 * KneeBox::onKneePointChange
 *****************************************************************************/
void KneeBox::onKneePointChange( int value )
{
    d_data->setKneeConfig( value, KneeSlope(), WhiteClip() );
    if ( KneeEnable() )
    {
        setWaitCursor();
        emit KneeConfigChanged( (int)KneeEnable(), value, KneeSlope(), WhiteClip() );
        setNormalCursor();
    }
}

/******************************************************************************
 * KneeBox::onKneeSlopeChange
 *****************************************************************************/
void KneeBox::onKneeSlopeChange( int value )
{
    d_data->setKneeConfig( KneePoint(), value, WhiteClip() );
    if ( KneeEnable() )
    {
        setWaitCursor();
        emit KneeConfigChanged( (int)KneeEnable(), KneePoint(), value, WhiteClip() );
        setNormalCursor();
    }
}

/******************************************************************************
 * KneeBox::onWhiteClipChange
 *****************************************************************************/
void KneeBox::onWhiteClipChange( int value )
{
    d_data->setKneeConfig( KneePoint(), KneeSlope(), value );
    if ( KneeEnable() )
    {
        setWaitCursor();
        emit KneeConfigChanged( (int)KneeEnable(), KneePoint(), KneeSlope(), value );
        setNormalCursor();
    }
}


