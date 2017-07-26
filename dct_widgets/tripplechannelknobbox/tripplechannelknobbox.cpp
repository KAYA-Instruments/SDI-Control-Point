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
 * @file    tripplechannelknobbox.cpp
 *
 * @brief   Implementation of 3 channel knob box
 *
 *****************************************************************************/
#include <QtDebug>

#include "tripplechannelknobbox.h"
#include "ui_tripplechannelknobbox.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_TrippleChannelKnobBox;
}

/******************************************************************************
 * definitions
 *****************************************************************************/
#define DEFAULT_MIN                ( 0x0000 )   /**< default minimum value */
#define DEFAULT_MAX                ( 0x03ff )   /**< default maximum value */
#define DEFAULT_COMMA_POSITION     ( 8 )        /**< comma position of 2^0 */
#define DEFAULT_FACTOR             ( 1 )

/******************************************************************************
 * helper function
 *****************************************************************************/
static int pow_rec( int x, unsigned int y )
{
    if ( y == 1 )
    {
        return ( x );
    }

    return ( x * pow_rec( x, (y-1) ) );
}

static int powi( int x, int y )
{
    if ( y == 0 )
    {
        return ( 1 );
    }

    if ( y == 1 )
    {
        return ( x );
    }

    if ( (x == 0) || (y < 0) )
    {
        // (y<0) not supported
        return ( 0 );
    }

    if ( x == 2 )
    {
        return ( 1 << y );
    }

    return ( x * pow_rec( x, y - 1) );
}

/******************************************************************************
 * TrippleChannelKnobBox::PrivateData
 *****************************************************************************/
class TrippleChannelKnobBox::PrivateData
{
public:
    PrivateData()
        : m_ui( new Ui::UI_TrippleChannelKnobBox )
        , m_base( 2 )
        , m_min( DEFAULT_MIN )
        , m_max( DEFAULT_MAX )
        , m_pos( DEFAULT_COMMA_POSITION )
        , m_fmt( "%-2.2f" )
        , m_factor( DEFAULT_FACTOR )
        , m_previous0( 0 )
        , m_roundCnt0( 0 )
        , m_previous1( 0 )
        , m_roundCnt1( 0 )
        , m_previous2( 0 )
        , m_roundCnt2( 0 )
        , m_roundMax( 1 )
        , m_tolerance( 10 )
        , m_cntEvents( 0 )
        , m_maxEvents( 10 )
    {
        // do nothing
    };

    ~PrivateData()
    {
        delete m_ui;
    };

    double toDisplayRange( int value )
    {
        return ( ((double)(value * m_factor))/powi( m_base, m_pos ) );
    }
 
    int toValueRange( double value )
    {
        return ( (int)((value * powi( m_base, m_pos ))/m_factor) ) ;
    }

    Ui::UI_TrippleChannelKnobBox *  m_ui;           /**< ui handle */
    int                             m_base;         /**< base for the interger to float conversion */
    int                             m_min;          /**< minimal value */
    int                             m_max;          /**< maximal value */
    int                             m_pos;          /**< position of comma (meaning (2^m_pos)) */
    QString                         m_fmt;          /**< format string to show regarding value */
    int                             m_factor;       /**< display factor, used to convert into presentation domain (like ° for an angle) */

    int                             m_previous0;    /**< previous value */
    int                             m_roundCnt0;    /**< round counter */
    int                             m_previous1;    /**< previous value */
    int                             m_roundCnt1;    /**< round counter */
    int                             m_previous2;    /**< previous value */
    int                             m_roundCnt2;    /**< round counter */
    int                             m_roundMax;     /**< max number of rounds for the knob */
    int                             m_tolerance;    /**< toolerance to detect a zero-crossing */

    int                             m_cntEvents;    /**< slider move event counter */
    int                             m_maxEvents;    /**< slider move event ignore counter */
};

/******************************************************************************
 * TrippleChannelKnobBox::TrippleChannelKnobBox
 *****************************************************************************/
TrippleChannelKnobBox::TrippleChannelKnobBox( QWidget * parent ) : QGroupBox( parent )
{
    // create private data container
    d_data = new PrivateData;

    // initialize UI
    d_data->m_ui->setupUi( this );

    // configure min/max of knob
    d_data->m_ui->knbValue0->setRange( d_data->m_min, d_data->m_max );
    d_data->m_ui->knbValue1->setRange( d_data->m_min, d_data->m_max );
    d_data->m_ui->knbValue2->setRange( d_data->m_min, d_data->m_max );
    
    // configure min/max of spinbox
    d_data->m_ui->sbxValue0->setRange(
        d_data->toDisplayRange( d_data->m_min ), d_data->toDisplayRange( d_data->m_max ) );
    d_data->m_ui->sbxValue1->setRange(
        d_data->toDisplayRange( d_data->m_min ), d_data->toDisplayRange( d_data->m_max ) );
    d_data->m_ui->sbxValue2->setRange(
        d_data->toDisplayRange( d_data->m_min ), d_data->toDisplayRange( d_data->m_max ) );

    // configure comman position of spinbox
    d_data->m_ui->sbxValue0->setDecimals( d_data->m_pos );
    d_data->m_ui->sbxValue1->setDecimals( d_data->m_pos );
    d_data->m_ui->sbxValue2->setDecimals( d_data->m_pos );

    d_data->m_ui->sbxValue0->setSingleStep( d_data->toDisplayRange( 1 ) );
    d_data->m_ui->sbxValue1->setSingleStep( d_data->toDisplayRange( 1 ) );
    d_data->m_ui->sbxValue2->setSingleStep( d_data->toDisplayRange( 1 ) );

    // connect signals for value0
    connect( d_data->m_ui->knbValue0, SIGNAL(valueChanged(int)), this, SLOT(onInt0Change(int)) );
    connect( d_data->m_ui->sbxValue0, SIGNAL(valueChanged(double)), this, SLOT(onDouble0Change(double)) );
    connect( d_data->m_ui->knbValue0, SIGNAL(actionTriggered(int)), this, SLOT(onKnob0Action(int)) );
    connect( d_data->m_ui->knbValue0, SIGNAL(sliderReleased()), this, SLOT(onKnob0Released()) );
    
    // connect signals for value1
    connect( d_data->m_ui->knbValue1, SIGNAL(valueChanged(int)), this, SLOT(onInt1Change(int)) );
    connect( d_data->m_ui->sbxValue1, SIGNAL(valueChanged(double)), this, SLOT(onDouble1Change(double)) );
    connect( d_data->m_ui->knbValue1, SIGNAL(actionTriggered(int)), this, SLOT(onKnob1Action(int)) );
    connect( d_data->m_ui->knbValue1, SIGNAL(sliderReleased()), this, SLOT(onKnob1Released()) );

    // connect signals for value2
    connect( d_data->m_ui->knbValue2, SIGNAL(valueChanged(int)), this, SLOT(onInt2Change(int)) );
    connect( d_data->m_ui->sbxValue2, SIGNAL(valueChanged(double)), this, SLOT(onDouble2Change(double)) );
    connect( d_data->m_ui->knbValue2, SIGNAL(actionTriggered(int)), this, SLOT(onKnob2Action(int)) );
    connect( d_data->m_ui->knbValue2, SIGNAL(sliderReleased()), this, SLOT(onKnob2Released()) );
}

/******************************************************************************
 * TrippleChannelKnobBox::~TrippleChannelKnobBox
 *****************************************************************************/
TrippleChannelKnobBox::~TrippleChannelKnobBox()
{
    delete d_data;
}

/******************************************************************************
 * TrippleChannelKnobBox::maxRounds
 *****************************************************************************/
int TrippleChannelKnobBox::maxRounds() const
{
    return ( d_data->m_roundMax );
}

/******************************************************************************
 * TrippleChannelKnobBox::setMaxRounds
 *****************************************************************************/
void TrippleChannelKnobBox::setMaxRounds( int roundMax )
{
    if ( d_data->m_roundMax != roundMax )
    {
        int v0 = value0();
        int v1 = value1();
        int v2 = value2();

        d_data->m_roundMax = roundMax;

        d_data->m_ui->knbValue0->setRange( 0, (d_data->m_max - d_data->m_min) / roundMax );
        d_data->m_ui->knbValue1->setRange( 0, (d_data->m_max - d_data->m_min) / roundMax );
        d_data->m_ui->knbValue2->setRange( 0, (d_data->m_max - d_data->m_min) / roundMax );
        
        setValue0( v0 );
        setValue1( v1 );
        setValue2( v2 );
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::base
 *****************************************************************************/
int TrippleChannelKnobBox::base() const
{
    return ( d_data->m_base );
}

/******************************************************************************
 * TrippleChannelKnobBox::base
 *****************************************************************************/
void TrippleChannelKnobBox::setBase( int value )
{
    if ( d_data->m_base != value )
    {
        int v0 = value0();
        int v1 = value1();
        int v2 = value2();

        d_data->m_base = value;

        d_data->m_ui->sbxValue0->setSingleStep( d_data->toDisplayRange( 1 ) );
        d_data->m_ui->sbxValue1->setSingleStep( d_data->toDisplayRange( 1 ) );
        d_data->m_ui->sbxValue2->setSingleStep( d_data->toDisplayRange( 1 ) );

        d_data->m_ui->sbxValue0->setMinimum( d_data->toDisplayRange( d_data->m_min ) );
        d_data->m_ui->sbxValue1->setMinimum( d_data->toDisplayRange( d_data->m_min ) );
        d_data->m_ui->sbxValue2->setMinimum( d_data->toDisplayRange( d_data->m_min ) );

        d_data->m_ui->sbxValue0->setMaximum( d_data->toDisplayRange( d_data->m_max ) );
        d_data->m_ui->sbxValue1->setMaximum( d_data->toDisplayRange( d_data->m_max ) );
        d_data->m_ui->sbxValue2->setMaximum( d_data->toDisplayRange( d_data->m_max ) );

        setValue0( v0 );
        setValue1( v1 );
        setValue2( v2 );
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::value0
 *****************************************************************************/
int TrippleChannelKnobBox::value0() const
{
    int v = (d_data->m_roundCnt0 * (d_data->m_ui->knbValue0->maximum() + 1)) 
          + d_data->m_ui->knbValue0->value()
          + d_data->m_min;
    return ( v );
}

/******************************************************************************
 * TrippleChannelKnobBox::setValue0
 *****************************************************************************/
void TrippleChannelKnobBox::setValue0( int value )
{
    onValue0Change( value );
}

/******************************************************************************
 * TrippleChannelKnobBox::value1
 *****************************************************************************/
int TrippleChannelKnobBox::value1() const
{
    int v = (d_data->m_roundCnt1 * (d_data->m_ui->knbValue1->maximum() + 1)) 
          + d_data->m_ui->knbValue1->value()
          + d_data->m_min;
    return ( v );
}

/******************************************************************************
 * TrippleChannelKnobBox::setValue1
 *****************************************************************************/
void TrippleChannelKnobBox::setValue1( int value )
{
    onValue1Change( value );
}

/******************************************************************************
 * TrippleChannelKnobBox::value2
 *****************************************************************************/
int TrippleChannelKnobBox::value2() const
{
    int v = (d_data->m_roundCnt2 * (d_data->m_ui->knbValue2->maximum() + 1)) 
          + d_data->m_ui->knbValue2->value()
          + d_data->m_min;
    return ( v );
}

/******************************************************************************
 * TrippleChannelKnobBox::setValue2
 *****************************************************************************/
void TrippleChannelKnobBox::setValue2( int value )
{
    onValue2Change( value );
}

/******************************************************************************
 * TrippleChannelKnobBox::minValue
 *****************************************************************************/
int TrippleChannelKnobBox::minValue() const
{
    return ( d_data->m_min );
}

/******************************************************************************
 * TrippleChannelKnobBox::setMinValue
 *****************************************************************************/
void TrippleChannelKnobBox::setMinValue( int min )
{
    if ( (d_data->m_max > min) && (d_data->m_min != min) )
    {
        int v0 = value0();
        int v1 = value1();
        int v2 = value2();

        d_data->m_ui->sbxValue0->setMinimum( d_data->toDisplayRange(min) );
        d_data->m_ui->sbxValue1->setMinimum( d_data->toDisplayRange(min) );
        d_data->m_ui->sbxValue2->setMinimum( d_data->toDisplayRange(min) );

        d_data->m_ui->knbValue0->setMaximum( (d_data->m_max - min) / d_data->m_roundMax );
        d_data->m_ui->knbValue1->setMaximum( (d_data->m_max - min) / d_data->m_roundMax );
        d_data->m_ui->knbValue2->setMaximum( (d_data->m_max - min) / d_data->m_roundMax );
        
        d_data->m_min = min;

        setValue0( (v0 < min) ? min : v0 );
        setValue1( (v1 < min) ? min : v1 );
        setValue2( (v2 < min) ? min : v2 );
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::maxValue
 *****************************************************************************/
int TrippleChannelKnobBox::maxValue() const
{
    return ( d_data->m_max );
}

/******************************************************************************
 * TrippleChannelKnobBox::setMaxValue
 *****************************************************************************/
void TrippleChannelKnobBox::setMaxValue( int max )
{
    if ( (max > d_data->m_min) && (d_data->m_max != max) )
    {
        int v0 = value0();
        int v1 = value1();
        int v2 = value2();

        d_data->m_ui->sbxValue0->setMaximum( d_data->toDisplayRange(max) );
        d_data->m_ui->sbxValue1->setMaximum( d_data->toDisplayRange(max) );
        d_data->m_ui->sbxValue2->setMaximum( d_data->toDisplayRange(max) );
        
        d_data->m_ui->knbValue0->setMaximum( (max - d_data->m_min) / d_data->m_roundMax );
        d_data->m_ui->knbValue1->setMaximum( (max - d_data->m_min) / d_data->m_roundMax );
        d_data->m_ui->knbValue2->setMaximum( (max - d_data->m_min) / d_data->m_roundMax );
        
        d_data->m_max = max;

        setValue0( (v0 > max) ? max : v0 );
        setValue1( (v1 > max) ? max : v1 );
        setValue2( (v2 > max) ? max : v2 );
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::setRange
 *****************************************************************************/
void TrippleChannelKnobBox::setRange( int min, int max )
{
    if ( (max > min) &&  ((d_data->m_min != min) || (d_data->m_max != max)) )
    {
        int v0 = value0();
        int v1 = value1();
        int v2 = value2();

        d_data->m_ui->sbxValue0->setRange( d_data->toDisplayRange(min), d_data->toDisplayRange(max) );
        d_data->m_ui->sbxValue1->setRange( d_data->toDisplayRange(min), d_data->toDisplayRange(max) );
        d_data->m_ui->sbxValue2->setRange( d_data->toDisplayRange(min), d_data->toDisplayRange(max) );

        d_data->m_ui->knbValue0->setMaximum( (max - min) / d_data->m_roundMax );
        d_data->m_ui->knbValue1->setMaximum( (max - min) / d_data->m_roundMax );
        d_data->m_ui->knbValue2->setMaximum( (max - min) / d_data->m_roundMax );
        
        d_data->m_min = min;
        d_data->m_max = max;

        setValue0( (v0 < min) ? min : ( (v0 > max) ? max : v0 ) );
        setValue1( (v1 < min) ? min : ( (v1 > max) ? max : v1 ) );
        setValue2( (v2 < min) ? min : ( (v2 > max) ? max : v2 ) );
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::commaPos
 *****************************************************************************/
int TrippleChannelKnobBox::commaPos() const
{
    return ( d_data->m_pos );
}

/******************************************************************************
 * TrippleChannelKnobBox::setCommaPos
 *****************************************************************************/
void TrippleChannelKnobBox::setCommaPos( int pos )
{
    if ( (pos >= 0) && (pos < 31) && (d_data->m_pos != pos) )
    {
        int v0 = value0();
        int v1 = value1();
        int v2 = value2();

        d_data->m_pos = pos;
        
        d_data->m_ui->sbxValue0->setDecimals( pos );
        d_data->m_ui->sbxValue1->setDecimals( pos );
        d_data->m_ui->sbxValue2->setDecimals( pos );
        
        d_data->m_ui->sbxValue0->setSingleStep( d_data->toDisplayRange(1) );
        d_data->m_ui->sbxValue1->setSingleStep( d_data->toDisplayRange(1) );
        d_data->m_ui->sbxValue2->setSingleStep( d_data->toDisplayRange(1) );

        d_data->m_ui->sbxValue0->setMinimum( d_data->toDisplayRange(d_data->m_min) );
        d_data->m_ui->sbxValue1->setMinimum( d_data->toDisplayRange(d_data->m_min) );
        d_data->m_ui->sbxValue2->setMinimum( d_data->toDisplayRange(d_data->m_min) );
        
        d_data->m_ui->sbxValue0->setMaximum( d_data->toDisplayRange(d_data->m_max) );
        d_data->m_ui->sbxValue1->setMaximum( d_data->toDisplayRange(d_data->m_max) );
        d_data->m_ui->sbxValue2->setMaximum( d_data->toDisplayRange(d_data->m_max) );

        setValue0( v0 );
        setValue1( v1 );
        setValue2( v2 );
    }       
}

/******************************************************************************
 * TrippleChannelKnobBox::getFmt
 *****************************************************************************/
QString TrippleChannelKnobBox::getFmt() const
{
    return ( d_data->m_fmt );
}

/******************************************************************************
 * TrippleChannelKnobBox::setFmt
 *****************************************************************************/
void TrippleChannelKnobBox::setFmt( const QString & fmt )
{
    if ( d_data->m_fmt != fmt )
    {
        d_data->m_ui->sbxValue0->setFmt( fmt );
        d_data->m_ui->sbxValue1->setFmt( fmt );
        d_data->m_ui->sbxValue2->setFmt( fmt );
        d_data->m_fmt = fmt;
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::displayMultiplier
 *****************************************************************************/
int TrippleChannelKnobBox::displayMultiplier() const
{
    return ( d_data->m_factor );
}

/******************************************************************************
 * TrippleChannelKnobBox::setCommaPos
 *****************************************************************************/
void TrippleChannelKnobBox::setDisplayMultiplier( int factor )
{
    if ( (factor > 0) && (d_data->m_factor != factor) )
    {
        int v0 = value0();
        int v1 = value1();
        int v2 = value2();

        d_data->m_factor = factor;

        d_data->m_ui->sbxValue0->setSingleStep( d_data->toDisplayRange( 1 ) );
        d_data->m_ui->sbxValue1->setSingleStep( d_data->toDisplayRange( 1 ) );
        d_data->m_ui->sbxValue2->setSingleStep( d_data->toDisplayRange( 1 ) );

        d_data->m_ui->sbxValue0->setMinimum( d_data->toDisplayRange( d_data->m_min ) );
        d_data->m_ui->sbxValue1->setMinimum( d_data->toDisplayRange( d_data->m_min ) );
        d_data->m_ui->sbxValue2->setMinimum( d_data->toDisplayRange( d_data->m_min ) );

        d_data->m_ui->sbxValue0->setMaximum( d_data->toDisplayRange( d_data->m_max ) );
        d_data->m_ui->sbxValue1->setMaximum( d_data->toDisplayRange( d_data->m_max ) );
        d_data->m_ui->sbxValue2->setMaximum( d_data->toDisplayRange( d_data->m_max ) );
        
        setValue0( v0 );
        setValue1( v1 );
        setValue2( v2 );
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::maxAngle
 *****************************************************************************/
int TrippleChannelKnobBox::maxAngle() const
{
    return ( d_data->m_ui->knbValue0->maxAngle() );
}

/******************************************************************************
 * TrippleChannelKnobBox::setMaxAngle
 *****************************************************************************/
void TrippleChannelKnobBox::setMaxAngle( int p_angle )
{
    d_data->m_ui->knbValue0->setMaxAngle( p_angle );
    d_data->m_ui->knbValue1->setMaxAngle( p_angle );
    d_data->m_ui->knbValue2->setMaxAngle( p_angle );
}

/******************************************************************************
 * TrippleChannelKnobBox::needleImage
 *****************************************************************************/
QPixmap TrippleChannelKnobBox::needleImage() const 
{
    return ( d_data->m_ui->knbValue0->needleImage() );
}

/******************************************************************************
 * TrippleChannelKnobBox::setNeedleImage
 *****************************************************************************/
void TrippleChannelKnobBox::setNeedleImage( QPixmap p_img )
{
    d_data->m_ui->knbValue0->setNeedleImage( p_img );
    d_data->m_ui->knbValue1->setNeedleImage( p_img );
    d_data->m_ui->knbValue2->setNeedleImage( p_img );
}

/******************************************************************************
 * TrippleChannelKnobBox::backgroundImage0
 *****************************************************************************/
QPixmap TrippleChannelKnobBox::backgroundImage0() const 
{
    return ( d_data->m_ui->knbValue0->backgroundImage() );
}

/******************************************************************************
 * TrippleChannelKnobBox::setBackgroundImage0
 *****************************************************************************/
void TrippleChannelKnobBox::setBackgroundImage0( QPixmap p_img )
{
    d_data->m_ui->knbValue0->setBackgroundImage( p_img );
}

/******************************************************************************
 * TrippleChannelKnobBox::backgroundImage1
 *****************************************************************************/
QPixmap TrippleChannelKnobBox::backgroundImage1() const 
{
    return ( d_data->m_ui->knbValue1->backgroundImage() );
}

/******************************************************************************
 * TrippleChannelKnobBox::setBackgroundImage1
 *****************************************************************************/
void TrippleChannelKnobBox::setBackgroundImage1( QPixmap p_img )
{
    d_data->m_ui->knbValue1->setBackgroundImage( p_img );
}

/******************************************************************************
 * TrippleChannelKnobBox::backgroundImage2
 *****************************************************************************/
QPixmap TrippleChannelKnobBox::backgroundImage2() const 
{
    return ( d_data->m_ui->knbValue2->backgroundImage() );
}

/******************************************************************************
 * TrippleChannelKnobBox::setBackgroundImage2
 *****************************************************************************/
void TrippleChannelKnobBox::setBackgroundImage2( QPixmap p_img )
{
    d_data->m_ui->knbValue2->setBackgroundImage( p_img );
}

/******************************************************************************
 * TrippleChannelKnobBox::onValue0Change
 *****************************************************************************/
void TrippleChannelKnobBox::onValue0Change( int value )
{
    if ( (value >= d_data->m_min) && (value <= d_data->m_max) )
    {
        // compute knob box value
        int v_knob = value - d_data->m_min;

        d_data->m_roundCnt0 = v_knob / (d_data->m_ui->knbValue0->maximum() + 1);
        if ( d_data->m_roundCnt0 > 0 )
        {
            v_knob -= d_data->m_roundCnt0 * (d_data->m_ui->knbValue0->maximum() + 1);
        }
 
        d_data->m_ui->sbxValue0->blockSignals( true );
        d_data->m_ui->sbxValue0->setValue( d_data->toDisplayRange( value ) );
        d_data->m_ui->sbxValue0->blockSignals( false );
    
        d_data->m_ui->knbValue0->blockSignals( true );
        d_data->m_ui->knbValue0->setValue( v_knob );
        d_data->m_ui->knbValue0->blockSignals( false );

        d_data->m_previous0 = v_knob;
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::onValue1Change
 *****************************************************************************/
void TrippleChannelKnobBox::onValue1Change( int value )
{
    if ( (value >= d_data->m_min) && (value <= d_data->m_max) )
    {
        // compute knob box value
        int v_knob = value - d_data->m_min;

        d_data->m_roundCnt1 = v_knob / (d_data->m_ui->knbValue1->maximum() + 1);
        if ( d_data->m_roundCnt1 > 0 )
        {
            v_knob -= d_data->m_roundCnt1 * (d_data->m_ui->knbValue1->maximum() + 1);
        }
 
        d_data->m_ui->sbxValue1->blockSignals( true );
        d_data->m_ui->sbxValue1->setValue( d_data->toDisplayRange( value ) );
        d_data->m_ui->sbxValue1->blockSignals( false );
    
        d_data->m_ui->knbValue1->blockSignals( true );
        d_data->m_ui->knbValue1->setValue( v_knob );
        d_data->m_ui->knbValue1->blockSignals( false );
        
        d_data->m_previous1 = v_knob;
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::onValue2Change
 *****************************************************************************/
void TrippleChannelKnobBox::onValue2Change( int value )
{
    if ( (value >= d_data->m_min) && (value <= d_data->m_max) )
    {
        // compute knob box value
        int v_knob = value - d_data->m_min;

        d_data->m_roundCnt2 = v_knob / (d_data->m_ui->knbValue2->maximum() + 1);
        if ( d_data->m_roundCnt2 > 0 )
        {
            v_knob -= d_data->m_roundCnt2 * (d_data->m_ui->knbValue2->maximum() + 1);
        }
 
        d_data->m_ui->sbxValue2->blockSignals( true );
        d_data->m_ui->sbxValue2->setValue( d_data->toDisplayRange(value) );
        d_data->m_ui->sbxValue2->blockSignals( false );
    
        d_data->m_ui->knbValue2->blockSignals( true );
        d_data->m_ui->knbValue2->setValue( v_knob );
        d_data->m_ui->knbValue2->blockSignals( false );
        
        d_data->m_previous2 = v_knob;
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::onValuesChange
 *****************************************************************************/
void TrippleChannelKnobBox::onValuesChange( int value0, int value1, int value2 )
{
    onValue0Change( value0 );
    onValue1Change( value1 );
    onValue2Change( value2 );
}

/******************************************************************************
 * TrippleChannelKnobBox::onInt0Change
 *****************************************************************************/
void TrippleChannelKnobBox::onInt0Change( int v_knob )
{
    int max = (d_data->m_ui->knbValue0->maximum() >> 1) - d_data->m_tolerance;
    int v0;

    // detect a max to min crossing
    if ( (v_knob - d_data->m_previous0) < -max )
    {
        if ( d_data->m_roundCnt0 < (d_data->m_roundMax-1) )
        {
            d_data->m_roundCnt0 ++;
        }
    }

    // detect a min to max crossing
    if ( (v_knob - d_data->m_previous0) > max )
    {
        if ( d_data->m_roundCnt0 > 0 )
        {
            d_data->m_roundCnt0 --;
        }
    }

    d_data->m_previous0 = v_knob;

    // compute spinbox value
    v0 = (d_data->m_roundCnt0 * (d_data->m_ui->knbValue0->maximum() + 1)) + v_knob + d_data->m_min;
    
    d_data->m_ui->sbxValue0->blockSignals( true );
    d_data->m_ui->sbxValue0->setValue( d_data->toDisplayRange( v0 ) );
    d_data->m_ui->sbxValue0->blockSignals( false );

    if ( d_data->m_ui->knbValue0->isSliderDown() &&  (d_data->m_cntEvents++ > d_data->m_maxEvents) )
    {
        d_data->m_cntEvents = 0;

        // we expect only one receivers for this signal,
        // which is connected to the tupple or single value signal
        if ( receivers(SIGNAL(ValuesChanged(int,int,int))) > 0 )
        {
            int v1 = (d_data->m_roundCnt1 * (d_data->m_ui->knbValue1->maximum() + 1)) + d_data->m_ui->knbValue1->value() + d_data->m_min;
            int v2 = (d_data->m_roundCnt2 * (d_data->m_ui->knbValue2->maximum() + 1)) + d_data->m_ui->knbValue2->value() + d_data->m_min;
            emit ValuesChanged( v0, v1, v2 );
        }
        else
        {
            emit Value0Changed( v0 );
        }
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::onInt1Change
 *****************************************************************************/
void TrippleChannelKnobBox::onInt1Change( int v_knob )
{
    int max = (d_data->m_ui->knbValue1->maximum() >> 1) - d_data->m_tolerance;
    int v1;

    // detect a max to min crossing
    if ( (v_knob - d_data->m_previous1) < -max )
    {
        if ( d_data->m_roundCnt1 < (d_data->m_roundMax-1) )
        {
            d_data->m_roundCnt1 ++;
        }
    }

    // detect a min to max crossing
    if ( (v_knob - d_data->m_previous1) > max )
    {
        if ( d_data->m_roundCnt1 > 0 )
        {
            d_data->m_roundCnt1 --;
        }
    }
    
    d_data->m_previous1 = v_knob;

    // compute spinbox value
    v1 = (d_data->m_roundCnt1 * (d_data->m_ui->knbValue1->maximum() + 1)) + v_knob + d_data->m_min;
    
    d_data->m_ui->sbxValue1->blockSignals( true );
    d_data->m_ui->sbxValue1->setValue( d_data->toDisplayRange( v1 ) );
    d_data->m_ui->sbxValue1->blockSignals( false );

    if ( d_data->m_ui->knbValue1->isSliderDown() &&  (d_data->m_cntEvents++ > d_data->m_maxEvents) )
    {
        d_data->m_cntEvents = 0;

        // we expect only one receivers for this signal,
        // which is connected to the tupple or single value signal
        if ( receivers(SIGNAL(ValuesChanged(int,int,int))) > 0 )
        {
            int v0 = (d_data->m_roundCnt0 * (d_data->m_ui->knbValue0->maximum() + 1)) + d_data->m_ui->knbValue0->value() + d_data->m_min;
            int v2 = (d_data->m_roundCnt2 * (d_data->m_ui->knbValue2->maximum() + 1)) + d_data->m_ui->knbValue2->value() + d_data->m_min;
            emit ValuesChanged( v0, v1, v2 );
        }
        else
        {
            emit Value1Changed( v1 );
        }
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::onInt2Change
 *****************************************************************************/
void TrippleChannelKnobBox::onInt2Change( int v_knob )
{
    int max = (d_data->m_ui->knbValue2->maximum() >> 1) - d_data->m_tolerance;
    int v2;

    // detect a max to min crossing
    if ( (v_knob - d_data->m_previous2) < -max )
    {
        if ( d_data->m_roundCnt2 < (d_data->m_roundMax-1) )
        {
            d_data->m_roundCnt2 ++;
        }
    }

    // detect a min to max crossing
    if ( (v_knob - d_data->m_previous2) > max )
    {
        if ( d_data->m_roundCnt2 > 0 )
        {
            d_data->m_roundCnt2 --;
        }
    }
    
    d_data->m_previous2 = v_knob;

    // compute spinbox value
    v2 = (d_data->m_roundCnt2 * (d_data->m_ui->knbValue2->maximum() + 1)) + v_knob + d_data->m_min;
    
    d_data->m_ui->sbxValue2->blockSignals( true );
    d_data->m_ui->sbxValue2->setValue( d_data->toDisplayRange( v2 ) );
    d_data->m_ui->sbxValue2->blockSignals( false );

    if ( d_data->m_ui->knbValue2->isSliderDown() &&  (d_data->m_cntEvents++ > d_data->m_maxEvents) )
    {
        d_data->m_cntEvents = 0;

        // we expect only one receivers for this signal,setMaxRounds
        // which is connected to the tupple or single value signal
        if ( receivers(SIGNAL(ValuesChanged(int,int,int))) > 0 )
        {
            int v0 = (d_data->m_roundCnt0 * (d_data->m_ui->knbValue0->maximum() + 1)) + d_data->m_ui->knbValue0->value() + d_data->m_min;
            int v1 = (d_data->m_roundCnt1 * (d_data->m_ui->knbValue1->maximum() + 1)) + d_data->m_ui->knbValue1->value() + d_data->m_min;
            emit ValuesChanged( v0, v1, v2 );
        }
        else
        {
            emit Value2Changed( v2 );
        }
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::onKnob0Action
 *****************************************************************************/
void TrippleChannelKnobBox::onKnob0Action( int v )
{
    if ( v == QAbstractSlider::SliderMove )
    {
        // future value (or value to set)
        int v_knob =  d_data->m_ui->knbValue0->sliderPosition();
        
        // detect a min to max crossing
        if ( d_data->m_roundCnt0 == 0 )
        {
            int max = (d_data->m_ui->knbValue0->maximum() >> 1) - d_data->m_tolerance;
            
            if ( (v_knob - d_data->m_previous0) > max )
            {
                d_data->m_ui->knbValue0->setSliderPosition( d_data->m_ui->knbValue0->minimum() );
            }
        }
  
        // detect a max to min crossing
        if ( d_data->m_roundCnt0 == (d_data->m_roundMax - 1) )
        {
            int max = (d_data->m_ui->knbValue0->maximum() >> 1) - d_data->m_tolerance;
            
            if ( (v_knob - d_data->m_previous0) < -max  )
            {
                d_data->m_ui->knbValue0->setSliderPosition( d_data->m_ui->knbValue0->maximum() );
            }
        }
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::onKnob0Released
 *****************************************************************************/
void TrippleChannelKnobBox::onKnob0Released()
{
    d_data->m_cntEvents = 0;
    emit Value0Changed( (d_data->m_roundCnt0 * (d_data->m_ui->knbValue0->maximum() + 1)) + d_data->m_ui->knbValue0->value() + d_data->m_min );
}

/******************************************************************************
 * TrippleChannelKnobBox::onKnob1Action
 *****************************************************************************/
void TrippleChannelKnobBox::onKnob1Action( int v )
{
    if ( v == QAbstractSlider::SliderMove )
    {
        // future value (or value to set)
        int v_knob =  d_data->m_ui->knbValue1->sliderPosition();
        
        // detect a min to max crossing
        if ( d_data->m_roundCnt1 == 0 )
        {
            int max = (d_data->m_ui->knbValue1->maximum() >> 1) - d_data->m_tolerance;
            
            if ( (v_knob - d_data->m_previous1) > max )
            {
                d_data->m_ui->knbValue1->setSliderPosition( d_data->m_ui->knbValue1->minimum() );
            }
        }
  
        // detect a max to min crossing
        if ( d_data->m_roundCnt1 == (d_data->m_roundMax - 1) )
        {
            int max = (d_data->m_ui->knbValue1->maximum() >> 1) - d_data->m_tolerance;
            
            if ( (v_knob - d_data->m_previous1) < -max  )
            {
                d_data->m_ui->knbValue1->setSliderPosition( d_data->m_ui->knbValue1->maximum() );
            }
        }
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::onKnob1Released
 *****************************************************************************/
void TrippleChannelKnobBox::onKnob1Released()
{
    d_data->m_cntEvents = 0;
    emit Value1Changed( (d_data->m_roundCnt1 * (d_data->m_ui->knbValue1->maximum() + 1)) + d_data->m_ui->knbValue1->value() + d_data->m_min );
}

/******************************************************************************
 * TrippleChannelKnobBox::onKnob2Action
 *****************************************************************************/
void TrippleChannelKnobBox::onKnob2Action( int v )
{
    if ( v == QAbstractSlider::SliderMove )
    {
        // future value (or value to set)
        int v_knob =  d_data->m_ui->knbValue2->sliderPosition();
        
        // detect a min to max crossing
        if ( d_data->m_roundCnt2 == 0 )
        {
            int max = (d_data->m_ui->knbValue2->maximum() >> 1) - d_data->m_tolerance;
            
            if ( (v_knob - d_data->m_previous2) > max )
            {
                d_data->m_ui->knbValue2->setSliderPosition( d_data->m_ui->knbValue2->minimum() );
            }
        }
  
        // detect a max to min crossing
        if ( d_data->m_roundCnt2 == (d_data->m_roundMax - 1) )
        {
            int max = (d_data->m_ui->knbValue2->maximum() >> 1) - d_data->m_tolerance;
            
            if ( (v_knob - d_data->m_previous2) < -max  )
            {
                d_data->m_ui->knbValue2->setSliderPosition( d_data->m_ui->knbValue2->maximum() );
            }
        }
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::onKnob2Released
 *****************************************************************************/
void TrippleChannelKnobBox::onKnob2Released()
{
    d_data->m_cntEvents = 0;
    emit Value2Changed( (d_data->m_roundCnt2 * (d_data->m_ui->knbValue2->maximum() + 1)) + d_data->m_ui->knbValue2->value() + d_data->m_min );
}

/******************************************************************************
 * TrippleChannelKnobBox::onDouble0Change
 *****************************************************************************/
void TrippleChannelKnobBox::onDouble0Change( double value )
{
    int v0 = d_data->toValueRange( value );

    int v_knob = v0 - d_data->m_min;

    d_data->m_roundCnt0 = v_knob / (d_data->m_ui->knbValue0->maximum() + 1);
    if ( d_data->m_roundCnt0 > 0 )
    {
        v_knob -= d_data->m_roundCnt0 * (d_data->m_ui->knbValue0->maximum() + 1);
    }
    
    d_data->m_ui->knbValue0->blockSignals( true );
    d_data->m_ui->knbValue0->setValue( v_knob );
    d_data->m_ui->knbValue0->blockSignals( false );

    d_data->m_previous0 = v_knob;
        
    // we expect only one receivers for this signal,
    // which is connected to the tupple or single value signal
    if ( receivers(SIGNAL(ValuesChanged(int,int,int))) > 0 )
    {
        int v1 = (d_data->m_roundCnt1 * (d_data->m_ui->knbValue1->maximum() + 1)) + d_data->m_ui->knbValue1->value() + d_data->m_min; 
        int v2 = (d_data->m_roundCnt2 * (d_data->m_ui->knbValue2->maximum() + 1)) + d_data->m_ui->knbValue2->value() + d_data->m_min; 
        emit ValuesChanged( v0, v1, v2 );
    }
    else
    {
        emit Value0Changed( v0 );
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::onDouble1Change
 *****************************************************************************/
void TrippleChannelKnobBox::onDouble1Change( double value )
{
    int v1 = d_data->toValueRange( value );

    int v_knob = v1 - d_data->m_min;
    
    d_data->m_roundCnt1 = v_knob / (d_data->m_ui->knbValue1->maximum() + 1);
    if ( d_data->m_roundCnt1 > 0 )
    {
        v_knob -= d_data->m_roundCnt1 * (d_data->m_ui->knbValue1->maximum() + 1);
    }
    
    d_data->m_ui->knbValue1->blockSignals( true );
    d_data->m_ui->knbValue1->setValue( v_knob );
    d_data->m_ui->knbValue1->blockSignals( false );
    
    d_data->m_previous1 = v_knob;

    // we expect only one receivers for this signal,
    // which is connected to the tupple or single value signal
    if ( receivers(SIGNAL(ValuesChanged(int,int,int))) > 0 )
    {
        int v0 = (d_data->m_roundCnt0 * (d_data->m_ui->knbValue0->maximum() + 1)) + d_data->m_ui->knbValue0->value() + d_data->m_min;
        int v2 = (d_data->m_roundCnt2 * (d_data->m_ui->knbValue2->maximum() + 1)) + d_data->m_ui->knbValue2->value() + d_data->m_min; 
        emit ValuesChanged( v0, v1, v2 );
    }
    else
    {
        emit Value1Changed( v1 );
    }
}

/******************************************************************************
 * TrippleChannelKnobBox::onDouble2Change
 *****************************************************************************/
void TrippleChannelKnobBox::onDouble2Change( double value )
{
    int v2 = d_data->toValueRange( value );
    
    int v_knob = v2 - d_data->m_min;
    
    d_data->m_roundCnt2 = v_knob / (d_data->m_ui->knbValue2->maximum() + 1);
    if ( d_data->m_roundCnt2 > 0 )
    {
        v_knob -= d_data->m_roundCnt2 * (d_data->m_ui->knbValue2->maximum() + 1);
    }
    
    d_data->m_ui->knbValue2->blockSignals( true );
    d_data->m_ui->knbValue2->setValue( v_knob );
    d_data->m_ui->knbValue2->blockSignals( false );

    // we expect only one receivers for this signal,
    // which is connected to the tupple or single value signal
    if ( receivers(SIGNAL(ValuesChanged(int,int,int))) > 0 )
    {
        int v0 = (d_data->m_roundCnt0 * (d_data->m_ui->knbValue0->maximum() + 1)) + d_data->m_ui->knbValue0->value() + d_data->m_min;
        int v1 = (d_data->m_roundCnt1 * (d_data->m_ui->knbValue1->maximum() + 1)) + d_data->m_ui->knbValue1->value() + d_data->m_min; 
        emit ValuesChanged( v0, v1, v2 );
    }
    else
    {
        emit Value2Changed( v2 );
    }
}

