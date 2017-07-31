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
 * @file    wbgainbox.cpp
 *
 * @brief   Implementation of white balance gain box
 *
 *****************************************************************************/
#include <QtDebug>

#include "tripplechannelsliderbox.h"
#include "ui_tripplechannelsliderbox.h"

namespace Ui {
    class UI_TrippleChannelSliderBox;
}

/******************************************************************************
 * TrippleChannelSliderBox::PrivateData
 *****************************************************************************/
class TrippleChannelSliderBox::PrivateData
{
public:
    PrivateData()
        : m_ui( new Ui::UI_TrippleChannelSliderBox )
        , m_orientation( Qt::Horizontal )
        , m_min( DEFAULT_VALUE_MIN )
        , m_max( DEFAULT_VALUE_MAX )
        , m_pos( DEFAULT_VALUE_COMMA_POSITION )
    {
        // do nothing
    };

    ~PrivateData()
    {
        delete m_ui;
    };

    Ui::UI_TrippleChannelSliderBox *    m_ui;           /**< ui handle */
    Qt::Orientation                     m_orientation;  /**< slider orientation */
    int                                 m_min;          /**< minimal gain value */
    int                                 m_max;          /**< maximal gain value */
    int                                 m_pos;          /**< position of comma (meaning (2^m_pos) */
};

/******************************************************************************
 * TrippleChannelSliderBox::TrippleChannelSliderBox
 *****************************************************************************/
TrippleChannelSliderBox::TrippleChannelSliderBox( QWidget * parent ) : QGroupBox( parent )
{
    // create private data container
    d_data = new PrivateData;

    // initialize UI
    d_data->m_ui->setupUi( this );
    
    // configure min/max slider value
    d_data->m_ui->sldValue0->setRange( d_data->m_min, d_data->m_max );
    d_data->m_ui->sldValue1->setRange( d_data->m_min, d_data->m_max );
    d_data->m_ui->sldValue2->setRange( d_data->m_min, d_data->m_max );

    // configure min/max spinbox value
    d_data->m_ui->spxValue0->setRange( ((double)(d_data->m_min))/(1<<d_data->m_pos), ((double)(d_data->m_max))/(1<<d_data->m_pos) );
    d_data->m_ui->spxValue1->setRange( ((double)(d_data->m_min))/(1<<d_data->m_pos), ((double)(d_data->m_max))/(1<<d_data->m_pos) );
    d_data->m_ui->spxValue2->setRange( ((double)(d_data->m_min))/(1<<d_data->m_pos), ((double)(d_data->m_max))/(1<<d_data->m_pos) );

    // set comman position in spinbox
    d_data->m_ui->spxValue0->setDecimals( d_data->m_pos );
    d_data->m_ui->spxValue1->setDecimals( d_data->m_pos );
    d_data->m_ui->spxValue2->setDecimals( d_data->m_pos );
    
    d_data->m_ui->spxValue0->setSingleStep( (1.0f/(1<<d_data->m_pos)) );
    d_data->m_ui->spxValue1->setSingleStep( (1.0f/(1<<d_data->m_pos)) );
    d_data->m_ui->spxValue2->setSingleStep( (1.0f/(1<<d_data->m_pos)) );
 
    // connect slider signals with with a slot
    // NOTE: slider works in interger range
    //       doublespinbox works in double range (don't want to implement a doublse slider) 
    connect( d_data->m_ui->sldValue0, SIGNAL(valueChanged(int)), this, SLOT(onIntValue0Change(int)) );
    connect( d_data->m_ui->sldValue1, SIGNAL(valueChanged(int)), this, SLOT(onIntValue1Change(int)) );
    connect( d_data->m_ui->sldValue2, SIGNAL(valueChanged(int)), this, SLOT(onIntValue2Change(int)) );
    
    connect( d_data->m_ui->spxValue0, SIGNAL(valueChanged(double)), this, SLOT(onDoubleValue0Change(double)) );
    connect( d_data->m_ui->spxValue1, SIGNAL(valueChanged(double)), this, SLOT(onDoubleValue1Change(double)) );
    connect( d_data->m_ui->spxValue2, SIGNAL(valueChanged(double)), this, SLOT(onDoubleValue2Change(double)) );
}

/******************************************************************************
 * TrippleChannelSliderBox::~TrippleChannelSliderBox
 *****************************************************************************/
TrippleChannelSliderBox::~TrippleChannelSliderBox()
{
    delete d_data;
}

/******************************************************************************
 * TrippleChannelSliderBox::sliderOrientation
 *****************************************************************************/
Qt::Orientation TrippleChannelSliderBox::sliderOrientation() const 
{
    return ( d_data->m_orientation );
}

/******************************************************************************
 * TrippleChannelSliderBox::setSliderType
 *****************************************************************************/
void TrippleChannelSliderBox::setSliderOrientation( Qt::Orientation orientation )
{
    if ( orientation != d_data->m_orientation )
    {
        // remove labels from layout 
        d_data->m_ui->gridLayout->removeWidget( d_data->m_ui->lblValue0  );
        d_data->m_ui->gridLayout->removeWidget( d_data->m_ui->lblValue1 );
        d_data->m_ui->gridLayout->removeWidget( d_data->m_ui->lblValue2 );
    
        // remove sliders from layout
        d_data->m_ui->gridLayout->removeWidget( d_data->m_ui->sldValue0 );
        d_data->m_ui->gridLayout->removeWidget( d_data->m_ui->sldValue1 );
        d_data->m_ui->gridLayout->removeWidget( d_data->m_ui->sldValue2 );

        // remove spinboxes from layout
        d_data->m_ui->gridLayout->removeWidget( d_data->m_ui->spxValue0 );
        d_data->m_ui->gridLayout->removeWidget( d_data->m_ui->spxValue1 );
        d_data->m_ui->gridLayout->removeWidget( d_data->m_ui->spxValue2 );
     
        // remove spinboxes from layout
        if ( orientation == Qt::Horizontal )
        {
            // set slider orientation horizontal
            d_data->m_ui->sldValue0->setOrientation( Qt::Horizontal );
            d_data->m_ui->sldValue1->setOrientation( Qt::Horizontal );
            d_data->m_ui->sldValue2->setOrientation( Qt::Horizontal );

            // re-add labels to layout
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->lblValue0, 0, 0, Qt::AlignLeft );
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->lblValue1, 1, 0, Qt::AlignLeft );
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->lblValue2, 2, 0, Qt::AlignLeft );

            // re-add sliders to layout
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->sldValue0, 0, 1, Qt::AlignHCenter );
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->sldValue1, 1, 1, Qt::AlignHCenter );
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->sldValue2, 2, 1, Qt::AlignHCenter );

            // re-add spinboxes to layout
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->spxValue0, 0, 2, Qt::AlignRight );
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->spxValue1, 1, 2, Qt::AlignRight  );
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->spxValue2, 2, 2, Qt::AlignRight  );
        }
        else
        {
            // orientation == Qt::Vertical

            // set slider orientation vertical
            d_data->m_ui->sldValue0->setOrientation( Qt::Vertical );
            d_data->m_ui->sldValue1->setOrientation( Qt::Vertical );
            d_data->m_ui->sldValue2->setOrientation( Qt::Vertical );

            // re-add labels to layout
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->lblValue0, 0, 0, Qt::AlignHCenter );
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->lblValue1, 0, 1, Qt::AlignHCenter );
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->lblValue2, 0, 2, Qt::AlignHCenter );
                
            // re-add spinboxes to layout
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->spxValue0, 1, 0, Qt::AlignHCenter );
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->spxValue1, 1, 1, Qt::AlignHCenter );
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->spxValue2, 1, 2, Qt::AlignHCenter );
                
            // re-add sliders to layout
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->sldValue0, 2, 0, Qt::AlignHCenter );
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->sldValue1, 2, 1, Qt::AlignHCenter );
            d_data->m_ui->gridLayout->addWidget( d_data->m_ui->sldValue2, 2, 2, Qt::AlignHCenter );
        }

        d_data->m_orientation = orientation;
    }
}

/******************************************************************************
 * TrippleChannelSliderBox::minValue
 *****************************************************************************/
int TrippleChannelSliderBox::minValue() const
{
    return ( d_data->m_min );
}

/******************************************************************************
 * TrippleChannelSliderBox::setMinValue
 *****************************************************************************/
void TrippleChannelSliderBox::setMinValue( int min )
{
    if ( min >= 0 && d_data->m_max > min )
    {
        d_data->m_ui->sldValue0->setMinimum( min );
        d_data->m_ui->sldValue1->setMinimum( min );
        d_data->m_ui->sldValue2->setMinimum( min );

        d_data->m_ui->spxValue0->setMinimum( ((double)(min))/(1<<d_data->m_pos) );
        d_data->m_ui->spxValue1->setMinimum( ((double)(min))/(1<<d_data->m_pos) );
        d_data->m_ui->spxValue2->setMinimum( ((double)(min))/(1<<d_data->m_pos) );

        d_data->m_min = min;
    }
}

/******************************************************************************
 * TrippleChannelSliderBox::maxValue
 *****************************************************************************/
int TrippleChannelSliderBox::maxValue() const
{
    return ( d_data->m_max );
}

/******************************************************************************
 * TrippleChannelSliderBox::setMaxValue
 *****************************************************************************/
void TrippleChannelSliderBox::setMaxValue( int max )
{
    if ( max >= 0 && max > d_data->m_min )
    {
        d_data->m_ui->sldValue0->setMaximum( max );
        d_data->m_ui->sldValue1->setMaximum( max );
        d_data->m_ui->sldValue2->setMaximum( max );

        d_data->m_ui->spxValue0->setMaximum( ((double)(max))/(1<<d_data->m_pos) );
        d_data->m_ui->spxValue1->setMaximum( ((double)(max))/(1<<d_data->m_pos) );
        d_data->m_ui->spxValue2->setMaximum( ((double)(max))/(1<<d_data->m_pos) );
    
        d_data->m_max = max;
    }
}

/******************************************************************************
 * TrippleChannelSliderBox::commaPos
 *****************************************************************************/
int TrippleChannelSliderBox::commaPos() const
{
    return ( d_data->m_pos );
}

/******************************************************************************
 * TrippleChannelSliderBox::setCommaPos
 *****************************************************************************/
void TrippleChannelSliderBox::setCommaPos( int pos )
{
    if ( (pos >= 0) && (pos < 31) )
    {
        d_data->m_ui->spxValue0->setMinimum( ((double)(d_data->m_min))/(1<<pos) );
        d_data->m_ui->spxValue1->setMinimum( ((double)(d_data->m_min))/(1<<pos) );
        d_data->m_ui->spxValue2->setMinimum( ((double)(d_data->m_min))/(1<<pos) );

        d_data->m_ui->spxValue0->setMaximum( ((double)(d_data->m_max))/(1<<pos) );
        d_data->m_ui->spxValue1->setMaximum( ((double)(d_data->m_max))/(1<<pos) );
        d_data->m_ui->spxValue2->setMaximum( ((double)(d_data->m_max))/(1<<pos) );
    
        d_data->m_ui->spxValue0->setDecimals( d_data->m_pos );
        d_data->m_ui->spxValue1->setDecimals( d_data->m_pos );
        d_data->m_ui->spxValue2->setDecimals( d_data->m_pos );
    
        d_data->m_pos = pos;
    }       
}

/******************************************************************************
 * TrippleChannelSliderBox::value0Name
 *****************************************************************************/
QString TrippleChannelSliderBox::value0Name() const
{
    return ( d_data->m_ui->lblValue0->text() );
}

/******************************************************************************
 * TrippleChannelSliderBox::setValue0Name
 *****************************************************************************/
void TrippleChannelSliderBox::setValue0Name( QString text )
{
    d_data->m_ui->lblValue0->setText( text );
}

/******************************************************************************
 * TrippleChannelSliderBox::value1Name
 *****************************************************************************/
QString TrippleChannelSliderBox::value1Name() const
{
    return ( d_data->m_ui->lblValue1->text() );
}

/******************************************************************************
 * TrippleChannelSliderBox::setValue1Name
 *****************************************************************************/
void TrippleChannelSliderBox::setValue1Name( QString text )
{
    d_data->m_ui->lblValue1->setText( text );
}

/******************************************************************************
 * TrippleChannelSliderBox::value2Name
 *****************************************************************************/
QString TrippleChannelSliderBox::value2Name() const
{
    return ( d_data->m_ui->lblValue2->text() );
}

/******************************************************************************
 * TrippleChannelSliderBox::setValue2Name
 *****************************************************************************/
void TrippleChannelSliderBox::setValue2Name( QString text )
{
    d_data->m_ui->lblValue2->setText( text );
}

/******************************************************************************
 * TrippleChannelSliderBox::onValue0Change
 *****************************************************************************/
void TrippleChannelSliderBox::onValue0Change( int value )
{
    d_data->m_ui->sldValue0->blockSignals( true );
    d_data->m_ui->sldValue0->setValue( value );
    d_data->m_ui->sldValue0->blockSignals( false );

    d_data->m_ui->spxValue0->blockSignals( true );
    d_data->m_ui->spxValue0->setValue( ((double)(value))/(1<<d_data->m_pos) );
    d_data->m_ui->spxValue0->blockSignals( false );
}

/******************************************************************************
 * TrippleChannelSliderBox::onValue1Change
 *****************************************************************************/
void TrippleChannelSliderBox::onValue1Change( int value )
{
    d_data->m_ui->sldValue1->blockSignals( true );
    d_data->m_ui->sldValue1->setValue( value );
    d_data->m_ui->sldValue1->blockSignals( false );

    d_data->m_ui->spxValue1->blockSignals( true );
    d_data->m_ui->spxValue1->setValue( ((double)(value))/(1<<d_data->m_pos) );
    d_data->m_ui->spxValue1->blockSignals( false );
}

/******************************************************************************
 * TrippleChannelSliderBox::onValue2Change
 *****************************************************************************/
void TrippleChannelSliderBox::onValue2Change( int value )
{
    d_data->m_ui->sldValue2->blockSignals( true );
    d_data->m_ui->sldValue2->setValue( value );
    d_data->m_ui->sldValue2->blockSignals( false );

    d_data->m_ui->spxValue2->blockSignals( true );
    d_data->m_ui->spxValue2->setValue( ((double)(value))/(1<<d_data->m_pos) );
    d_data->m_ui->spxValue2->blockSignals( false );
}

/******************************************************************************
 * TrippleChannelSliderBox::onIntValue0Change
 *****************************************************************************/
void TrippleChannelSliderBox::onIntValue0Change( int value )
{
    d_data->m_ui->spxValue0->blockSignals( true );
    d_data->m_ui->spxValue0->setValue( ((double)(value))/(1<<d_data->m_pos) );
    d_data->m_ui->spxValue0->blockSignals( false );

    emit Value0Changed( value );
}

/******************************************************************************
 * TrippleChannelSliderBox::onIntValue1Change
 *****************************************************************************/
void TrippleChannelSliderBox::onIntValue1Change( int value )
{
    d_data->m_ui->spxValue1->blockSignals( true );
    d_data->m_ui->spxValue1->setValue( ((double)(value))/(1<<d_data->m_pos) );
    d_data->m_ui->spxValue1->blockSignals( false );

    emit Value1Changed( value );
}

/******************************************************************************
 * TrippleChannelSliderBox::onIntValue2Change
 *****************************************************************************/
void TrippleChannelSliderBox::onIntValue2Change( int value )
{
    d_data->m_ui->spxValue2->blockSignals( true );
    d_data->m_ui->spxValue2->setValue( ((double)(value))/(1<<d_data->m_pos) );
    d_data->m_ui->spxValue2->blockSignals( false );

    emit Value2Changed( value );
}


/******************************************************************************
 * TrippleChannelSliderBox::onDoubleValue0Change
 *****************************************************************************/
void TrippleChannelSliderBox::onDoubleValue0Change( double value )
{
    int v = (int)((value)*(1<<d_data->m_pos));
    
    d_data->m_ui->sldValue0->blockSignals( true );
    d_data->m_ui->sldValue0->setValue( v );
    d_data->m_ui->sldValue0->blockSignals( false );

    emit Value0Changed( v );
}

/******************************************************************************
 * TrippleChannelSliderBox::onDoubleValue1Change
 *****************************************************************************/
void TrippleChannelSliderBox::onDoubleValue1Change( double value )
{
    int v = (int)((value)*(1<<d_data->m_pos));
    
    d_data->m_ui->sldValue1->blockSignals( true );
    d_data->m_ui->sldValue1->setValue( v );
    d_data->m_ui->sldValue1->blockSignals( false );

    emit Value1Changed( v );
}

/******************************************************************************
 * TrippleChannelSliderBox::onDoubleValue2Change
 *****************************************************************************/
void TrippleChannelSliderBox::onDoubleValue2Change( double value )
{
    int v = (int)((value)*(1<<d_data->m_pos));
    
    d_data->m_ui->sldValue2->blockSignals( true );
    d_data->m_ui->sldValue2->setValue( v );
    d_data->m_ui->sldValue2->blockSignals( false );

    emit Value2Changed( v );
}


