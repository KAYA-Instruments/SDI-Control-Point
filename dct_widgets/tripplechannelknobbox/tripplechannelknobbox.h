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
 * @file    tripplechannelknobbox.h
 *
 * @brief   Class definition of a tripple channel knob box 
 *
 *****************************************************************************/
#ifndef __TRIPPLE_CHANNEL_KNOB_BOX_H__
#define __TRIPPLE_CHANNEL_KNOB_BOX_H__

#include <QGroupBox>

/**
 * @brief The Tripple Channel Knob Box Widget 
 */

class TrippleChannelKnobBox : public QGroupBox
{
    Q_OBJECT

    // maximal number of allowed zero-crossings
    Q_PROPERTY( int maxRounds READ maxRounds WRITE setMaxRounds DESIGNABLE true);
 
    Q_PROPERTY( int base READ base WRITE setBase DESIGNABLE true);

    Q_PROPERTY( int value0 READ value0 WRITE setValue0 DESIGNABLE true )
    Q_PROPERTY( int value1 READ value1 WRITE setValue1 DESIGNABLE true )
    Q_PROPERTY( int value2 READ value2 WRITE setValue2 DESIGNABLE true )

    Q_PROPERTY( int minimum READ minValue WRITE setMinValue DESIGNABLE true )
    Q_PROPERTY( int maximum READ maxValue WRITE setMaxValue DESIGNABLE true )
    
    // note: meaning of comma position is different to decimals property 
    //       It means the bit position of 2^0 = 1. 
    // i.e:  If the comma position wequal to 2 
    //        - A 'one' is reached when setting a value of 1<<2 = 4
    //        - The lowest value (greater than 0) is 1/2^2 = 0.25
    Q_PROPERTY( int comma   READ commaPos WRITE setCommaPos DESIGNABLE true )

    Q_PROPERTY( QString fmt READ getFmt WRITE setFmt DESIGNABLE true )

    // note: The display multiplier is an additional factor to convert from a
    //       value range into into the display range. I.e. The hue value 
    //       is a fix-point number which needs to be converted into to 
    //       display range with a unit of degree [°].
    //       
    //       example Hue: value range from -128 to 127, -90° to 
    //                    comma position at 2^7=128
    //                    value to display [°] = x/2^7 * 90
    Q_PROPERTY( int displayMultiplier READ displayMultiplier WRITE setDisplayMultiplier DESIGNABLE true )

    Q_PROPERTY( int maxAngle READ maxAngle WRITE setMaxAngle DESIGNABLE true )
    Q_PROPERTY( QPixmap needleImage READ needleImage WRITE setNeedleImage DESIGNABLE true )
    Q_PROPERTY( QPixmap backgroundImage0 READ backgroundImage0 WRITE setBackgroundImage0 DESIGNABLE true )
    Q_PROPERTY( QPixmap backgroundImage1 READ backgroundImage1 WRITE setBackgroundImage1 DESIGNABLE true )
    Q_PROPERTY( QPixmap backgroundImage2 READ backgroundImage2 WRITE setBackgroundImage2 DESIGNABLE true )

public:
    explicit TrippleChannelKnobBox( QWidget * parent = 0 );
    ~TrippleChannelKnobBox();

    int maxRounds() const;
    void setMaxRounds( int value );

    int base() const;
    void setBase( int value );

    int value0() const;
    void setValue0( int value );
    
    int value1() const;
    void setValue1( int value );

    int value2() const;
    void setValue2( int value );
 
    int minValue() const;
    void setMinValue( int min );
    
    int maxValue() const;
    void setMaxValue( int min );

    void setRange( int min, int max );
    
    int commaPos() const;
    void setCommaPos( int pos );

    QString getFmt() const;
    void setFmt( const QString & );

    int displayMultiplier() const;
    void setDisplayMultiplier( int factor );

    int maxAngle() const;
    void setMaxAngle(int p_angle);

    QPixmap needleImage() const;
    void setNeedleImage(QPixmap p_img);

    QPixmap backgroundImage0() const;
    void setBackgroundImage0(QPixmap p_img);
    
    QPixmap backgroundImage1() const;
    void setBackgroundImage1(QPixmap p_img);

    QPixmap backgroundImage2() const;
    void setBackgroundImage2(QPixmap p_img);

signals:
    void Value0Changed( int value );
    void Value1Changed( int value );
    void Value2Changed( int value );
    
    void ValuesChanged( int value0, int value1, int value2 );

public slots:
    void onValue0Change( int value );
    void onValue1Change( int value );
    void onValue2Change( int value );
    
    void onValuesChange( int value0, int value1, int value2 );

private slots:
    // knob slots
    void onInt0Change( int value );
    void onInt1Change( int value );
    void onInt2Change( int value );

    void onKnob0Action( int value );
    void onKnob0Released();
    void onKnob1Action( int value );
    void onKnob1Released();
    void onKnob2Action( int value );
    void onKnob2Released();

    // spinbox slots
    void onDouble0Change( double value );
    void onDouble1Change( double value );
    void onDouble2Change( double value );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __TRIPPLE_CHANNEL_KNOB_BOX_H__

