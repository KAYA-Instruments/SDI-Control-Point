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
 * @file    SingleChannelKnobBox.h
 *
 * @brief   Class definition of a single channel knob box 
 *
 *****************************************************************************/
#ifndef __SINGLE_CHANNEL_KNOB_BOX_H__
#define __SINGLE_CHANNEL_KNOB_BOX_H__

#include <QGroupBox>

/**
 * @brief The Single Channel Knob Box Widget 
 */

class SingleChannelKnobBox : public QGroupBox
{
    Q_OBJECT
   
    // maximal number of allowed zero-crossings
    Q_PROPERTY( int maxRounds READ maxRounds WRITE setMaxRounds DESIGNABLE true)
    
    Q_PROPERTY( int value READ value WRITE setValue DESIGNABLE true )
    Q_PROPERTY( int minimum READ minValue WRITE setMinValue DESIGNABLE true )
    Q_PROPERTY( int maximum READ maxValue WRITE setMaxValue DESIGNABLE true )
    Q_PROPERTY( int base READ base WRITE setBase DESIGNABLE true )

    // note: meaning of comma position is different to decimals property 
    //       It means the bit position of 2^0 = 1. 
    // i.e:  If the comma position wequal to 2 
    //        - A 'one' is reached when setting a value of 1<<2 = 4
    //        - The lowest value (greater than 0) is 1/2^2 = 0.25
    Q_PROPERTY( int comma   READ commaPos WRITE setCommaPos DESIGNABLE true )
   
    Q_PROPERTY( QString fmt READ getFmt WRITE setFmt DESIGNABLE true )
   
    // note:  Some commands need a long time to execute, this is to
    //        ignore a number of value change events, 
    Q_PROPERTY( int maxEvent READ maxEvent WRITE setMaxEvent DESIGNABLE true )
   
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
    Q_PROPERTY( QPixmap backgroundImage READ backgroundImage WRITE setBackgroundImage DESIGNABLE true )
 
public:
    explicit SingleChannelKnobBox( QWidget * parent = 0 );
    ~SingleChannelKnobBox();

    int maxRounds() const;
    void setMaxRounds( int value );

    int base() const;
    void setBase( int value );

    int value() const;
    void setValue( int value );
    
    int minValue() const;
    void setMinValue( int min );
    
    int maxValue() const;
    void setMaxValue( int min );

    void setRange( int min, int max );

    int commaPos() const;
    void setCommaPos( int pos );

    QString getFmt() const;
    void setFmt( const QString & );
    
    int maxEvent() const;
    void setMaxEvent( int noEvt );

    int displayMultiplier() const;
    void setDisplayMultiplier( int factor );

    int maxAngle() const;
    void setMaxAngle(int p_angle);

    QPixmap backgroundImage() const;
    void setBackgroundImage(QPixmap p_img);

    QPixmap needleImage() const;
    void setNeedleImage(QPixmap p_img);

signals:
    void ValueChanged( int value );

public slots:
    void onValueChange( int value );

private slots:
    // knob slots
    void onIntChange( int value );
    void onKnobAction( int value );
    void onKnobReleased();

    // spinbox slots
    void onDoubleChange( double value );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __SINGLE_CHANNEL_KNOB_BOX_H__

