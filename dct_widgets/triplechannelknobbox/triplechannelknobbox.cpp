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
 * @file    triplechannelknobbox.cpp
 *
 * @brief   Implementation of 3 channel knob box
 *
 *****************************************************************************/
#include <QtDebug>

#include "triplechannelknobbox.h"
#include "ui_triplechannelknobbox.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_TripleChannelKnobBox;
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
 * TripleChannelKnobBox::PrivateData
 *****************************************************************************/
class TripleChannelKnobBox::PrivateData
{
public:
    PrivateData()
        : m_ui( new Ui::UI_TripleChannelKnobBox )
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

    Ui::UI_TripleChannelKnobBox *  m_ui;           /**< ui handle */
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
 * TripleChannelKnobBox::TripleChannelKnobBox
 *****************************************************************************/
TripleChannelKnobBox::TripleChannelKnobBox( QWidget * parent ) : QGroupBox( parent )
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
 * TripleChannelKnobBox::~TripleChannelKnobBox
 *****************************************************************************/
TripleChannelKnobBox::~TripleChannelKnobBox()
{
    delete d_data;
}

/******************************************************************************
 * TripleChannelKnobBox::maxRounds
 *****************************************************************************/
int TripleChannelKnobBox::maxRounds() const
{
    return ( d_data->m_roundMax );
}

/******************************************************************************
 * TripleChannelKnobBox::setMaxRounds
 *****************************************************************************/
void TripleChannelKnobBox::setMaxRounds( int roundMax )
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
 * TripleChannelKnobBox::base
 *****************************************************************************/
int TripleChannelKnobBox::base() const
{
    return ( d_data->m_base );
}

/******************************************************************************
 * TripleChannelKnobBox::base
 *****************************************************************************/
void TripleChannelKnobBox::setBase( int value )
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
 * TripleChannelKnobBox::value0
 *****************************************************************************/
int TripleChannelKnobBox::value0() const
{
    int v = (d_data->m_roundCnt0 * (d_data->m_ui->knbValue0->maximum() + 1)) 
          + d_data->m_ui->knbValue0->value()
          + d_data->m_min;
    return ( v );
}

/******************************************************************************
 * TripleChannelKnobBox::setValue0
 *****************************************************************************/
void TripleChannelKnobBox::setValue0( int value )
{
    onValue0Change( value );
}

/******************************************************************************
 * TripleChannelKnobBox::value1
 *****************************************************************************/
int TripleChannelKnobBox::value1() const
{
    int v = (d_data->m_roundCnt1 * (d_data->m_ui->knbValue1->maximum() + 1)) 
          + d_data->m_ui->knbValue1->value()
          + d_data->m_min;
    return ( v );
}

/******************************************************************************
 * TripleChannelKnobBox::setValue1
 *****************************************************************************/
void TripleChannelKnobBox::setValue1( int value )
{
    onValue1Change( value );
}

/******************************************************************************
 * TripleChannelKnobBox::value2
 *****************************************************************************/
int TripleChannelKnobBox::value2() const
{
    int v = (d_data->m_roundCnt2 * (d_data->m_ui->knbValue2->maximum() + 1)) 
          + d_data->m_ui->knbValue2->value()
          + d_data->m_min;
    return ( v );
}

/******************************************************************************
 * TripleChannelKnobBox::setValue2
 *****************************************************************************/
void TripleChannelKnobBox::setValue2( int value )
{
    onValue2Change( value );
}

/******************************************************************************
 * TripleChannelKnobBox::minValue
 *****************************************************************************/
int TripleChannelKnobBox::minValue() const
{
    return ( d_data->m_min );
}

/******************************************************************************
 * TripleChannelKnobBox::setMinValue
 *****************************************************************************/
void TripleChannelKnobBox::setMinValue( int min )
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
 * TripleChannelKnobBox::maxValue
 *****************************************************************************/
int TripleChannelKnobBox::maxValue() const
{
    return ( d_data->m_max );
}

/******************************************************************************
 * TripleChannelKnobBox::setMaxValue
 *****************************************************************************/
void TripleChannelKnobBox::setMaxValue( int max )
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
 * TripleChannelKnobBox::setRange
 *****************************************************************************/
void TripleChannelKnobBox::setRange( int min, int max )
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
 * TripleChannelKnobBox::commaPos
 *****************************************************************************/
int TripleChannelKnobBox::commaPos() const
{
    return ( d_data->m_pos );
}

/******************************************************************************
 * TripleChannelKnobBox::setCommaPos
 *****************************************************************************/
void TripleChannelKnobBox::setCommaPos( int pos )
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
 * TripleChannelKnobBox::getFmt
 *****************************************************************************/
QString TripleChannelKnobBox::getFmt() const
{
    return ( d_data->m_fmt );
}

/******************************************************************************
 * TripleChannelKnobBox::setFmt
 *****************************************************************************/
void TripleChannelKnobBox::setFmt( const QString & fmt )
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
 * TripleChannelKnobBox::displayMultiplier
 *****************************************************************************/
int TripleChannelKnobBox::displayMultiplier() const
{
    return ( d_data->m_factor );
}

/******************************************************************************
 * TripleChannelKnobBox::setCommaPos
 *****************************************************************************/
void TripleChannelKnobBox::setDisplayMultiplier( int factor )
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
 * TripleChannelKnobBox::maxAngle
 *****************************************************************************/
int TripleChannelKnobBox::maxAngle() const
{
    return ( d_data->m_ui->knbValue0->maxAngle() );
}

/******************************************************************************
 * TripleChannelKnobBox::setMaxAngle
 *****************************************************************************/
void TripleChannelKnobBox::setMaxAngle( int p_angle )
{
    d_data->m_ui->knbValue0->setMaxAngle( p_angle );
    d_data->m_ui->knbValue1->setMaxAngle( p_angle );
    d_data->m_ui->knbValue2->setMaxAngle( p_angle );
}

/******************************************************************************
 * TripleChannelKnobBox::needleImage
 *****************************************************************************/
QPixmap TripleChannelKnobBox::needleImage() const 
{
    return ( d_data->m_ui->knbValue0->needleImage() );
}

/******************************************************************************
 * TripleChannelKnobBox::setNeedleImage
 *****************************************************************************/
void TripleChannelKnobBox::setNeedleImage( QPixmap p_img )
{
    d_data->m_ui->knbValue0->setNeedleImage( p_img );
    d_data->m_ui->knbValue1->setNeedleImage( p_img );
    d_data->m_ui->knbValue2->setNeedleImage( p_img );
}

/******************************************************************************
 * TripleChannelKnobBox::backgroundImage0
 *****************************************************************************/
QPixmap TripleChannelKnobBox::backgroundImage0() const 
{
    return ( d_data->m_ui->knbValue0->backgroundImage() );
}

/******************************************************************************
 * TripleChannelKnobBox::setBackgroundImage0
 *****************************************************************************/
void TripleChannelKnobBox::setBackgroundImage0( QPixmap p_img )
{
    d_data->m_ui->knbValue0->setBackgroundImage( p_img );
}

/******************************************************************************
 * TripleChannelKnobBox::backgroundImage1
 *****************************************************************************/
QPixmap TripleChannelKnobBox::backgroundImage1() const 
{
    return ( d_data->m_ui->knbValue1->backgroundImage() );
}

/******************************************************************************
 * TripleChannelKnobBox::setBackgroundImage1
 *****************************************************************************/
void TripleChannelKnobBox::setBackgroundImage1( QPixmap p_img )
{
    d_data->m_ui->knbValue1->setBackgroundImage( p_img );
}

/******************************************************************************
 * TripleChannelKnobBox::backgroundImage2
 *****************************************************************************/
QPixmap TripleChannelKnobBox::backgroundImage2() const 
{
    return ( d_data->m_ui->knbValue2->backgroundImage() );
}

/******************************************************************************
 * TripleChannelKnobBox::setBackgroundImage2
 *****************************************************************************/
void TripleChannelKnobBox::setBackgroundImage2( QPixmap p_img )
{
    d_data->m_ui->knbValue2->setBackgroundImage( p_img );
}

/******************************************************************************
 * TripleChannelKnobBox::onValue0Change
 *****************************************************************************/
void TripleChannelKnobBox::onValue0Change( int value )
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
 * TripleChannelKnobBox::onValue1Change
 *****************************************************************************/
void TripleChannelKnobBox::onValue1Change( int value )
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
 * TripleChannelKnobBox::onValue2Change
 *****************************************************************************/
void TripleChannelKnobBox::onValue2Change( int value )
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
 * TripleChannelKnobBox::onValuesChange
 *****************************************************************************/
void TripleChannelKnobBox::onValuesChange( int value0, int value1, int value2 )
{
    onValue0Change( value0 );
    onValue1Change( value1 );
    onValue2Change( value2 );
}

/******************************************************************************
 * TripleChannelKnobBox::onInt0Change
 *****************************************************************************/
void TripleChannelKnobBox::onInt0Change( int v_knob )
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
 * TripleChannelKnobBox::onInt1Change
 *****************************************************************************/
void TripleChannelKnobBox::onInt1Change( int v_knob )
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
 * TripleChannelKnobBox::onInt2Change
 *****************************************************************************/
void TripleChannelKnobBox::onInt2Change( int v_knob )
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
 * TripleChannelKnobBox::onKnob0Action
 *****************************************************************************/
void TripleChannelKnobBox::onKnob0Action( int v )
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
 * TripleChannelKnobBox::onKnob0Released
 *****************************************************************************/
void TripleChannelKnobBox::onKnob0Released()
{
    d_data->m_cntEvents = 0;
    emit Value0Changed( (d_data->m_roundCnt0 * (d_data->m_ui->knbValue0->maximum() + 1)) + d_data->m_ui->knbValue0->value() + d_data->m_min );
}

/******************************************************************************
 * TripleChannelKnobBox::onKnob1Action
 *****************************************************************************/
void TripleChannelKnobBox::onKnob1Action( int v )
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
 * TripleChannelKnobBox::onKnob1Released
 *****************************************************************************/
void TripleChannelKnobBox::onKnob1Released()
{
    d_data->m_cntEvents = 0;
    emit Value1Changed( (d_data->m_roundCnt1 * (d_data->m_ui->knbValue1->maximum() + 1)) + d_data->m_ui->knbValue1->value() + d_data->m_min );
}

/******************************************************************************
 * TripleChannelKnobBox::onKnob2Action
 *****************************************************************************/
void TripleChannelKnobBox::onKnob2Action( int v )
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
 * TripleChannelKnobBox::onKnob2Released
 *****************************************************************************/
void TripleChannelKnobBox::onKnob2Released()
{
    d_data->m_cntEvents = 0;
    emit Value2Changed( (d_data->m_roundCnt2 * (d_data->m_ui->knbValue2->maximum() + 1)) + d_data->m_ui->knbValue2->value() + d_data->m_min );
}

/******************************************************************************
 * TripleChannelKnobBox::onDouble0Change
 *****************************************************************************/
void TripleChannelKnobBox::onDouble0Change( double value )
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
 * TripleChannelKnobBox::onDouble1Change
 *****************************************************************************/
void TripleChannelKnobBox::onDouble1Change( double value )
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
 * TripleChannelKnobBox::onDouble2Change
 *****************************************************************************/
void TripleChannelKnobBox::onDouble2Change( double value )
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

