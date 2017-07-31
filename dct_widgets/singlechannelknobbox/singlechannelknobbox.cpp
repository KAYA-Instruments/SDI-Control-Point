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
 * @file    singlechannelknobbox.cpp
 *
 * @brief   Implementation of a single knob with a value edit box
 *
 * NOTE:    If display-range factor used (different to 1) than the knob runs in
 *          value range ( min ... max ), but the spinbox runs in the converted 
 *          display range ( (min*factor)/2^comma ... (max*factor)/2^comma )
 *
 *****************************************************************************/
#include <QtDebug>

#include "singlechannelknobbox.h"
#include "ui_singlechannelknobbox.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_SingleChannelKnobBox;
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
 * SingleChannelKnobBox::PrivateData
 *****************************************************************************/
class SingleChannelKnobBox::PrivateData
{
public:
    PrivateData()
        : m_ui( new Ui::UI_SingleChannelKnobBox )
        , m_base( 2 )
        , m_min( DEFAULT_MIN )
        , m_max( DEFAULT_MAX )
        , m_pos( DEFAULT_COMMA_POSITION )
        , m_factor( DEFAULT_FACTOR )
        , m_previous( 0 )
        , m_roundCnt( 0 )
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

    Ui::UI_SingleChannelKnobBox *   m_ui;       /**< ui handle */
    int                             m_base;     /**< base for the interger to float conversion */
    int                             m_min;      /**< minimal value */
    int                             m_max;      /**< maximal value */
    int                             m_pos;      /**< position of comma (meaning (2^m_pos)) */
    int                             m_factor;   /**< display factor, used to convert into presentation domain (like ° for an angle) */

    int                             m_previous;     /**< previous value */
    int                             m_roundCnt;     /**< round counter */
    int                             m_roundMax;     /**< max number of rounds for the knob */
    int                             m_tolerance;    /**< toolerance to detect a zero-crossing */

    int                             m_cntEvents;
    int                             m_maxEvents;
};

/******************************************************************************
 * SingleChannelKnobBox::SingleChannelKnobBox
 *****************************************************************************/
SingleChannelKnobBox::SingleChannelKnobBox( QWidget * parent ) : QGroupBox( parent )
{
    // create private data container
    d_data = new PrivateData;

    // initialize UI
    d_data->m_ui->setupUi( this );

    // configure min/max of knob
    d_data->m_ui->knbValue->setRange( 0, (d_data->m_max - d_data->m_min) / d_data->m_roundMax );
    
    // configure min/max of spinbox
    d_data->m_ui->sbxValue->setRange(
        d_data->toDisplayRange( d_data->m_min ), d_data->toDisplayRange( d_data->m_max ) );

    // configure comman position of spinbox
    d_data->m_ui->sbxValue->setDecimals( d_data->m_pos );
    d_data->m_ui->sbxValue->setSingleStep( d_data->toDisplayRange( 1 ) );

    connect( d_data->m_ui->knbValue, SIGNAL(valueChanged(int)), this, SLOT(onIntChange(int)) );
    connect( d_data->m_ui->knbValue, SIGNAL(actionTriggered(int)), this, SLOT(onKnobAction(int)) );
    connect( d_data->m_ui->knbValue, SIGNAL(sliderReleased()), this, SLOT(onKnobReleased()) );

    connect( d_data->m_ui->sbxValue, SIGNAL(valueChanged(double)), this, SLOT(onDoubleChange(double)) );
}

/******************************************************************************
 * SingleChannelKnobBox::~SingleChannelKnobBox
 *****************************************************************************/
SingleChannelKnobBox::~SingleChannelKnobBox()
{
    delete d_data;
}

/******************************************************************************
 * SingleChannelKnobBox::maxRounds
 *****************************************************************************/
int SingleChannelKnobBox::maxRounds() const
{
    return ( d_data->m_roundMax );
}

/******************************************************************************
 * SingleChannelKnobBox::setMaxRounds
 *****************************************************************************/
void SingleChannelKnobBox::setMaxRounds( int roundMax )
{
    if ( d_data->m_roundMax != roundMax )
    {
        int v = value();

        d_data->m_roundMax  = roundMax;
        d_data->m_ui->knbValue->setRange( 0, (d_data->m_max - d_data->m_min) / d_data->m_roundMax );
        d_data->m_tolerance = (d_data->m_max - d_data->m_min) / d_data->m_roundMax / 10;

        setValue( v );
    }
}

/******************************************************************************
 * SingleChannelKnobBox::base
 *****************************************************************************/
int SingleChannelKnobBox::base() const
{
    return ( d_data->m_base );
}

/******************************************************************************
 * SingleChannelKnobBox::setBase
 *****************************************************************************/
void SingleChannelKnobBox::setBase( int base )
{
    if ( d_data->m_base != base )
    {
        int v = value();

        d_data->m_base = base;
        d_data->m_ui->sbxValue->setSingleStep( d_data->toDisplayRange( 1 ) );
        d_data->m_ui->sbxValue->setMinimum( d_data->toDisplayRange( d_data->m_min ) );
        d_data->m_ui->sbxValue->setMaximum( d_data->toDisplayRange( d_data->m_max ) );

        setValue( v );
    }
}

/******************************************************************************
 * SingleChannelKnobBox::value
 *****************************************************************************/
int SingleChannelKnobBox::value() const
{
    int v = (d_data->m_roundCnt * (d_data->m_ui->knbValue->maximum() + 1)) 
          + d_data->m_ui->knbValue->value()
          + d_data->m_min;
    return ( v );
}

/******************************************************************************
 * SingleChannelKnobBox::setValue
 *****************************************************************************/
void SingleChannelKnobBox::setValue( int value )
{
    onValueChange( value );
}

/******************************************************************************
 * SingleChannelKnobBox::minValue
 *****************************************************************************/
int SingleChannelKnobBox::minValue() const
{
    return ( d_data->m_min );
}

/******************************************************************************
 * SingleChannelKnobBox::setMinValue
 *****************************************************************************/
void SingleChannelKnobBox::setMinValue( int min )
{
    if ( (d_data->m_max > min) && (d_data->m_min != min) )
    {
        int v = value();
        
        d_data->m_ui->sbxValue->setMinimum( d_data->toDisplayRange( min ) );
        d_data->m_ui->knbValue->setRange( 0, (d_data->m_max - min) / d_data->m_roundMax );
        
        d_data->m_min = min;

        d_data->m_tolerance = (d_data->m_max - d_data->m_min) / d_data->m_roundMax / 10;

        setValue( (v < min) ? min : v );
    }
}

/******************************************************************************
 * SingleChannelKnobBox::maxValue
 *****************************************************************************/
int SingleChannelKnobBox::maxValue() const
{
    return ( d_data->m_max );
}

/******************************************************************************
 * SingleChannelKnobBox::setMaxValue
 *****************************************************************************/
void SingleChannelKnobBox::setMaxValue( int max )
{
    if ( (max > d_data->m_min) && (d_data->m_max != max) )
    {
        int v = value();

        d_data->m_ui->sbxValue->setMaximum( d_data->toDisplayRange( max ) );
        d_data->m_ui->knbValue->setRange( 0, (max - d_data->m_min) / d_data->m_roundMax );

        d_data->m_max = max;
        
        d_data->m_tolerance = (d_data->m_max - d_data->m_min) / d_data->m_roundMax / 10;

        setValue( (v > max) ? max : v );
    }
}

/******************************************************************************
 * SingleChannelKnobBox::setRange
 *****************************************************************************/
void SingleChannelKnobBox::setRange( int min, int max )
{
    if ( (max > min) &&  ((d_data->m_min != min) || (d_data->m_max != max)) )
    {
        int v = value();

        d_data->m_ui->sbxValue->setRange( d_data->toDisplayRange(min), d_data->toDisplayRange(max) );
        d_data->m_ui->knbValue->setMaximum( (max - min) / d_data->m_roundMax );
        
        d_data->m_min = min;
        d_data->m_max = max;
        
        d_data->m_tolerance = (d_data->m_max - d_data->m_min) / d_data->m_roundMax / 10;

        setValue( (v < min) ? min : ( (v > max) ? max : v ) );
    }
}

/******************************************************************************
 * SingleChannelKnobBox::commaPos
 *****************************************************************************/
int SingleChannelKnobBox::commaPos() const
{
    return ( d_data->m_pos );
}

/******************************************************************************
 * SingleChannelKnobBox::setCommaPos
 *****************************************************************************/
void SingleChannelKnobBox::setCommaPos( int pos )
{
    if ( (pos >= 0) && (pos < 31) && (d_data->m_pos != pos) )
    {
        int v = value();

        d_data->m_pos = pos;
        d_data->m_ui->sbxValue->setDecimals( pos );
        d_data->m_ui->sbxValue->setSingleStep( d_data->toDisplayRange( 1 ) );
        d_data->m_ui->sbxValue->setMinimum( d_data->toDisplayRange( d_data->m_min ) );
        d_data->m_ui->sbxValue->setMaximum( d_data->toDisplayRange( d_data->m_max ) );
        
        setValue( v );
    }       
}

/******************************************************************************
 * SingleChannelKnobBox::getFmt
 *****************************************************************************/
QString SingleChannelKnobBox::getFmt() const
{
    return ( d_data->m_ui->sbxValue->getFmt() );
}

/******************************************************************************
 * SingleChannelKnobBox::setFmt
 *****************************************************************************/
void SingleChannelKnobBox::setFmt( const QString & fmt )
{
    d_data->m_ui->sbxValue->setFmt( fmt );
}

/******************************************************************************
 * SingleChannelKnobBox::maxEvent
 *****************************************************************************/
int SingleChannelKnobBox::maxEvent() const
{
    return ( d_data->m_maxEvents );
}

/******************************************************************************
 * SingleChannelKnobBox::setMaxEvent
 *****************************************************************************/
void SingleChannelKnobBox::setMaxEvent( int noEvt )
{
    d_data->m_cntEvents = 0;
    d_data->m_maxEvents = noEvt;
}

/******************************************************************************
 * SingleChannelKnobBox::displayMultiplier
 *****************************************************************************/
int SingleChannelKnobBox::displayMultiplier() const
{
    return ( d_data->m_factor );
}

/******************************************************************************
 * SingleChannelKnobBox::setCommaPos
 *****************************************************************************/
void SingleChannelKnobBox::setDisplayMultiplier( int factor )
{
    if ( (factor > 0) && (d_data->m_factor != factor) )
    {
        int v = value();

        d_data->m_factor = factor;
        d_data->m_ui->sbxValue->setSingleStep( d_data->toDisplayRange( 1 ) );
        d_data->m_ui->sbxValue->setMinimum( d_data->toDisplayRange( d_data->m_min ) );
        d_data->m_ui->sbxValue->setMaximum( d_data->toDisplayRange( d_data->m_max ) );
        
        setValue( v );
    }
}

/******************************************************************************
 * SingleChannelKnobBox::maxAngle
 *****************************************************************************/
int SingleChannelKnobBox::maxAngle() const
{
    return ( d_data->m_ui->knbValue->maxAngle() );
}

/******************************************************************************
 * SingleChannelKnobBox::setMaxAngle
 *****************************************************************************/
void SingleChannelKnobBox::setMaxAngle( int p_angle )
{
    d_data->m_ui->knbValue->setMaxAngle( p_angle );
}

/******************************************************************************
 * SingleChannelKnobBox::needleImage
 *****************************************************************************/
QPixmap SingleChannelKnobBox::needleImage() const 
{
    return ( d_data->m_ui->knbValue->needleImage() );
}

/******************************************************************************
 * SingleChannelKnobBox::setNeedleImage
 *****************************************************************************/
void SingleChannelKnobBox::setNeedleImage( QPixmap p_img )
{
    d_data->m_ui->knbValue->setNeedleImage( p_img );
}

/******************************************************************************
 * SingleChannelKnobBox::backgroundImage
 *****************************************************************************/
QPixmap SingleChannelKnobBox::backgroundImage() const 
{
    return ( d_data->m_ui->knbValue->backgroundImage() );
}

/******************************************************************************
 * SingleChannelKnobBox::setBackgroundImage
 *****************************************************************************/
void SingleChannelKnobBox::setBackgroundImage( QPixmap p_img )
{
    d_data->m_ui->knbValue->setBackgroundImage( p_img );
}

/******************************************************************************
 * SingleChannelKnobBox::onValueChange
 *****************************************************************************/
void SingleChannelKnobBox::onValueChange( int value )
{
    if ( (value >= d_data->m_min) && (value <= d_data->m_max) )
    {
        // compute knob box value
        int v_knob = value - d_data->m_min;

        d_data->m_roundCnt = v_knob / (d_data->m_ui->knbValue->maximum() + 1);
        if ( d_data->m_roundCnt > 0 )
        {
            v_knob -= d_data->m_roundCnt * (d_data->m_ui->knbValue->maximum() + 1);
        }
 
        d_data->m_ui->sbxValue->blockSignals( true );
        d_data->m_ui->sbxValue->setValue( d_data->toDisplayRange( value ) );
        d_data->m_ui->sbxValue->blockSignals( false );
    
        d_data->m_ui->knbValue->blockSignals( true );
        d_data->m_ui->knbValue->setValue( v_knob );
        d_data->m_ui->knbValue->blockSignals( false );

        d_data->m_previous = v_knob;
    }
}

/******************************************************************************
 * SingleChannelKnobBox::onIntChange
 *****************************************************************************/
void SingleChannelKnobBox::onIntChange( int v_knob )
{
    int max = (d_data->m_ui->knbValue->maximum() >> 1) - d_data->m_tolerance;
    int v;

    // detect a max to min crossing
    if ( (v_knob - d_data->m_previous) < -max )
    {
        if ( d_data->m_roundCnt < (d_data->m_roundMax-1) )
        {
            d_data->m_roundCnt ++;
        }
    }

    // detect a min to max crossing
    if ( (v_knob - d_data->m_previous) > max )
    {
        if ( d_data->m_roundCnt > 0 )
        {
            d_data->m_roundCnt --;
        }
    }
    
    d_data->m_previous = v_knob;

    // compute spinbox value
    v = (d_data->m_roundCnt * (d_data->m_ui->knbValue->maximum() + 1)) + v_knob + d_data->m_min;
    
    d_data->m_ui->sbxValue->blockSignals( true );
    d_data->m_ui->sbxValue->setValue( d_data->toDisplayRange( v ) );
    d_data->m_ui->sbxValue->blockSignals( false );

    if ( d_data->m_ui->knbValue->isSliderDown() &&  (d_data->m_cntEvents++ > d_data->m_maxEvents) )
    {
        d_data->m_cntEvents = 0;
        emit ValueChanged( v );
    }
}

/******************************************************************************
 * SingleChannelKnobBox::onKnobAction
 *****************************************************************************/
void SingleChannelKnobBox::onKnobAction( int v )
{
    if ( v == QAbstractSlider::SliderMove )
    {
        // future value (or value to set)
        int v_knob =  d_data->m_ui->knbValue->sliderPosition();
        
        // detect a min to max crossing
        if ( d_data->m_roundCnt == 0 )
        {
            int max = (d_data->m_ui->knbValue->maximum() >> 1) - d_data->m_tolerance;
            
            if ( (v_knob - d_data->m_previous) > max )
            {
                d_data->m_ui->knbValue->setSliderPosition( d_data->m_ui->knbValue->minimum() );
            }
        }
  
        // detect a max to min crossing
        if ( d_data->m_roundCnt == (d_data->m_roundMax - 1) )
        {
            int max = (d_data->m_ui->knbValue->maximum() >> 1) - d_data->m_tolerance;
            
            if ( (v_knob - d_data->m_previous) < -max  )
            {
                d_data->m_ui->knbValue->setSliderPosition( d_data->m_ui->knbValue->maximum() );
            }
        }
    }
}

/******************************************************************************
 * SingleChannelKnobBox::onKnobReleased
 *****************************************************************************/
void SingleChannelKnobBox::onKnobReleased()
{
    d_data->m_cntEvents = 0;
    emit ValueChanged( (d_data->m_roundCnt * (d_data->m_ui->knbValue->maximum() + 1)) + d_data->m_previous + d_data->m_min );
}

/******************************************************************************
 * SingleChannelKnobBox::onDoubleChange
 *****************************************************************************/
void SingleChannelKnobBox::onDoubleChange( double value )
{
    int v = d_data->toValueRange( value );

    int v_knob = v - d_data->m_min;
    
    d_data->m_roundCnt = v_knob / (d_data->m_ui->knbValue->maximum() + 1);
    if ( d_data->m_roundCnt > 0 )
    {
        v_knob -= d_data->m_roundCnt * (d_data->m_ui->knbValue->maximum() + 1);
    }
    
    d_data->m_ui->knbValue->blockSignals( true );
    d_data->m_ui->knbValue->setValue( v_knob );
    d_data->m_ui->knbValue->blockSignals( false );

    d_data->m_previous = v_knob;

    emit ValueChanged( v );
}
