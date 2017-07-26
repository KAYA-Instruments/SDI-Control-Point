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
 * @file    tripplechannelsliderbox.h
 *
 * @brief   Class definition of a tripple channel slider box
 *
 *****************************************************************************/
#ifndef __TRIPPLECHANNELSLIDERBOX_H__
#define __TRIPPLECHANNELSLIDERBOX_H__

#include <QGroupBox>

#define DEFAULT_VALUE_MIN                ( 0x0000 )
#define DEFAULT_VALUE_MAX                ( 0x03ff )
#define DEFAULT_VALUE_COMMA_POSITION     ( 8 )       /**< position of 2^0 */

/**
 * @brief The Tripple channel slider box widget
 */
class TrippleChannelSliderBox : public QGroupBox
{
    Q_OBJECT

    Q_PROPERTY( Qt::Orientation sliderOrientation READ sliderOrientation WRITE setSliderOrientation DESIGNABLE true )
    Q_PROPERTY( int minValue READ minValue WRITE setMinValue DESIGNABLE true )
    Q_PROPERTY( int maxValue READ maxValue WRITE setMaxValue DESIGNABLE true )
    // note: meaning of comma position is different to decimals property 
    //       It means the bit position of 2^0 = 1. 
    // i.e:  If the comma position wequal to 2 
    //        - A 'one' is reached when setting a value of 1<<2 = 4
    //        - The lowest value (greater than 0) is 1/2^2 = 0.25
    Q_PROPERTY( int commaPos READ commaPos WRITE setCommaPos DESIGNABLE true )
    
    Q_PROPERTY( QString value0Name READ value0Name WRITE setValue0Name DESIGNABLE true )
    Q_PROPERTY( QString value1Name READ value1Name WRITE setValue1Name DESIGNABLE true )
    Q_PROPERTY( QString value2Name READ value2Name WRITE setValue2Name DESIGNABLE true )

public:
    explicit TrippleChannelSliderBox( QWidget * parent = 0 );
    ~TrippleChannelSliderBox();

    Qt::Orientation sliderOrientation() const;
    void setSliderOrientation( Qt::Orientation orientation );

    int minValue() const;
    void setMinValue( int min );

    int maxValue( void ) const;
    void setMaxValue( int max );

    int commaPos( void ) const;
    void setCommaPos( int pos );

    QString value0Name( void ) const;
    void setValue0Name( QString text );
    
    QString value1Name( void ) const;
    void setValue1Name( QString text );

    QString value2Name( void ) const;
    void setValue2Name( QString text );

signals:
    void Value0Changed( int value );
    void Value1Changed( int value );
    void Value2Changed( int value );
    
public slots:
    void onValue0Change( int value );
    void onValue1Change( int value );
    void onValue2Change( int value );
    
private slots:
    // slider slots
    void onIntValue0Change( int value );
    void onIntValue1Change( int value );
    void onIntValue2Change( int value );
    
    // double-spin-box slots
    void onDoubleValue0Change( double value );
    void onDoubleValue1Change( double value );
    void onDoubleValue2Change( double value );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __TRIPPLECHANNELSLIDERBOX_H__

