/******************************************************************************
 *
 * Copyright 2017, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
/**
 * @file    mccslider.h
 *
 * @brief   Class definition of a slider widget that represents one phase of
 *          the MCC equalizer.
 *
 *****************************************************************************/

#ifndef MCCSLIDER_H
#define MCCSLIDER_H

#include <QWidget>
#include <QColor>

/**
 * @brief The Mcc Slider Widget
 */

class MccSlider : public QWidget
{
    Q_OBJECT

    /* Index of the mcc segment this slider represents.
     * Ranges from to a maximum of 32, depending on the amount of phases selected. */
    Q_PROPERTY( int segmentIndex READ segmentIndex WRITE setSegmentIndex DESIGNABLE true )

    /* Color of the blink select button below the sliders
     * it should represent the center color of this segment. */
    Q_PROPERTY( QColor segmentColor READ segmentColor WRITE setSegmentColor DESIGNABLE true )

    /* hue and saturation as set by the sliders */
    Q_PROPERTY( int hue READ hue WRITE setHue DESIGNABLE true )
    Q_PROPERTY( int sat READ sat WRITE setSat DESIGNABLE true )

    /* min and max hue offsets
     * in mcc the hue offset is a 16 bit signed value
     * => value ranges from -32768 to 32767
     * => angle ranges from -180° to 179.994506836 (=32767/32768*180) */
    Q_PROPERTY( int minHue READ minHue WRITE setMinHue DESIGNABLE true )
    Q_PROPERTY( int maxHue READ maxHue WRITE setMaxHue DESIGNABLE true )

    /* min and max saturation offsets
     * in mcc the sat offset is a 16 bit unsigned value
     * => value ranges from 0 to 65535 */
    Q_PROPERTY( int minSat READ minSat WRITE setMinSat DESIGNABLE true )
    Q_PROPERTY( int maxSat READ maxSat WRITE setMaxSat DESIGNABLE true )

    /* hue and saturation ranges in % */
    Q_PROPERTY( int hueRange READ hueRange WRITE setHueRange DESIGNABLE true )
    Q_PROPERTY( int satRange READ satRange WRITE setSatRange DESIGNABLE true )

    /* default hue / saturation values, used to reset the sliders
     * and needed to calculate the range */
    Q_PROPERTY( int defaultHue READ defaultHue WRITE setDefaultHue DESIGNABLE true )
    Q_PROPERTY( int defaultSat READ defaultSat WRITE setDefaultSat DESIGNABLE true )

public:
    explicit MccSlider(int segmentIndex, QColor segmentColor,
                       int minHue, int maxHue,
                       int minSat, int maxSat,
                       int hueRange, int satRange,
                       int defaultHue, int defaultSat,
                       QWidget *parent = 0 );
    ~MccSlider();
    bool eventFilter(QObject *target, QEvent *event);

    int segmentIndex() const;
    void setSegmentIndex( int index );

    QColor segmentColor() const;
    void setSegmentColor( QColor color );

    int hue() const;
    void setHue( int hue );

    int sat() const;
    void setSat( int sat );

    bool blink() const;
    void setBlink( bool blink );

    int minHue() const;
    void setMinHue( int minHue );

    int maxHue() const;
    void setMaxHue( int maxHue );

    int minSat() const;
    void setMinSat( int minSat );

    int maxSat() const;
    void setMaxSat( int maxSat );

    int hueRange() const;
    void setHueRange( int hueRange );

    int satRange() const;
    void setSatRange( int satRange );

    int defaultHue() const;
    void setDefaultHue( int defaultHue );

    int defaultSat() const;
    void setDefaultSat( int defaultSat );

signals:
    void HueChanged( int index, int hue );
    void SatChanged( int index, int sat );
    void BlinkChanged( int index, bool blink );

public slots:
    void onHueChange( int index, int hue );
    void onSatChange( int index, int sat );
    void onBlinkChange( int index, bool blink );

    void onHueRangeChange( int range );
    void onSatRangeChange( int range );

    void onResetSlider( int index );

    void onSliderSelected( int index );

private slots:
    void onHueSliderChange( int value );
    void onSatSliderChange( int value );
    void onBlinkButtonToggled( bool value );

private:
    class PrivateData;
    PrivateData * d_data;

    // helper functions to rescale the sliders on range or min / max change
    void setupHueSliderRange();
    void setupSatSliderRange();
};

#endif // MCCSLIDER_H
