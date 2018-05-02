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
 * @file    mccslider.cpp
 *
 * @brief   Class implementation of a slider widget that represents one phase
 *          of the MCC equalizer.
 *
 *****************************************************************************/

#include "mccslider.h"
#include "ui_mccslider.h"

/******************************************************************************
 * namespaces
 *****************************************************************************/
namespace Ui {
    class MccSlider;
}

/******************************************************************************
 * MccSlider::PrivateData
 *****************************************************************************/
class MccSlider::PrivateData
{
public:
    PrivateData( int segmentIndex, QColor segmentColor,
                 int minHue, int maxHue,
                 int minSat, int maxSat,
                 int hueRange, int satRange,
                 int defaultHue, int defaultSat )
        : m_ui( new Ui::MccSlider )
        , m_segmentIndex( segmentIndex )
        , m_segmentColor( segmentColor )
        , m_minHue( minHue )
        , m_maxHue( maxHue )
        , m_minSat( minSat )
        , m_maxSat( maxSat )
        , m_hueRange( hueRange )
        , m_satRange( satRange )
        , m_defaultHue( defaultHue )
        , m_defaultSat( defaultSat )
    {
    };

    ~PrivateData()
    {
        delete m_ui;
    };

    Ui::MccSlider *         m_ui;               /**< ui handle */
    int                     m_segmentIndex;     /**< index of the mcc segment */
    QColor                  m_segmentColor;     /**< center color of the mcc segment */
    int                     m_minHue;           /**< minimum Hue, if range is 100% */
    int                     m_maxHue;           /**< maximum Hue, if range is 100% */
    int                     m_minSat;           /**< minimum Saturation, if range is 100% */
    int                     m_maxSat;           /**< maximum Saturation, if range is 100% */
    int                     m_hueRange;         /**< range of the hue slider in % */
    int                     m_satRange;         /**< range of the saturation slider in % */
    int                     m_defaultHue;       /**< default hue value (0 position) */
    int                     m_defaultSat;       /**< default saturation value (0 position) */
};

/******************************************************************************
 * MccSlider::MccSlider(
 *****************************************************************************/
MccSlider::MccSlider( int segmentIndex, QColor segmentColor,
                      int minHue, int maxHue,
                      int minSat, int maxSat,
                      int hueRange, int satRange,
                      int defaultHue, int defaultSat,
                      QWidget *parent )
    : QWidget(parent)
{
    // create private data container
    d_data = new PrivateData( segmentIndex, segmentColor,
                              minHue, maxHue,
                              minSat, maxSat,
                              hueRange, satRange,
                              defaultHue, defaultSat );

    // initialize UI
    d_data->m_ui->setupUi(this);

    // Setup slider (ranged min / max and value)
    setupHueSliderRange();
    setHue( defaultHue );
    setupSatSliderRange();
    setSat( defaultSat );

    // Install event filter on sliders to catch double click events
    d_data->m_ui->hueSlider->installEventFilter(this);
    d_data->m_ui->satSlider->installEventFilter(this);

    // Setup button unchecked / checked color
    QString colorString = QString( "QPushButton { background-color: %1; }\n"
                                   "QPushButton:checked { background-color: white; }\n"  ).arg(segmentColor.name() );
    d_data->m_ui->blinkButton->setStyleSheet(colorString);

    // Connect internal signals
    connect( d_data->m_ui->hueSlider, SIGNAL(valueChanged(int)), this, SLOT(onHueSliderChange(int)));
    connect( d_data->m_ui->satSlider, SIGNAL(valueChanged(int)), this, SLOT(onSatSliderChange(int)));
    connect( d_data->m_ui->blinkButton, SIGNAL(toggled(bool)), this, SLOT(onBlinkButtonToggled(bool)));
}

/******************************************************************************
 * MccSlider::~MccSlider(
 *****************************************************************************/
MccSlider::~MccSlider()
{
    delete d_data;
}

/******************************************************************************
 * MccSlider::eventFilter(
 *****************************************************************************/
bool MccSlider::eventFilter(QObject *target, QEvent *event)
{
    // On double click on the hue slider, reset it to default position
    if( target == d_data->m_ui->hueSlider )
    {
        if ( event->type() == QEvent::MouseButtonDblClick )
        {
            d_data->m_ui->hueSlider->setValue( defaultHue() );
            return true;
        }
    }

    // On double clock on the saturation slider, reset it to default position
    else if( target == d_data->m_ui->satSlider )
    {
        if ( event->type() == QEvent::MouseButtonDblClick )
        {
            d_data->m_ui->satSlider->setValue( defaultSat() );
            return true;
        }
    }

    // All other events are processed by the sliders themself
    return false;
}

/******************************************************************************
 * MccSlider::segmentIndex
 *****************************************************************************/
int MccSlider::segmentIndex() const
{
    return ( d_data->m_segmentIndex );
}

/******************************************************************************
 * MccSlider::setSegmentIndex
 *****************************************************************************/
void MccSlider::setSegmentIndex( int index )
{
    d_data->m_segmentIndex = index;
}

/******************************************************************************
 * MccSlider::segmentColor
 *****************************************************************************/
QColor MccSlider::segmentColor() const
{
    return ( d_data->m_segmentColor );
}

/******************************************************************************
 * MccSlider::setSegmentColor
 *****************************************************************************/
void MccSlider::setSegmentColor( QColor color )
{
    d_data->m_segmentColor = color;
}

/******************************************************************************
 * MccSlider::hue
 *****************************************************************************/
int MccSlider::hue() const
{
    return ( d_data->m_ui->hueSlider->value() );
}

/******************************************************************************
 * MccSlider::setHue
 *****************************************************************************/
void MccSlider::setHue( int hue )
{
    d_data->m_ui->hueSlider->blockSignals( true );
    d_data->m_ui->hueSlider->setValue( hue );
    d_data->m_ui->hueSlider->blockSignals( false );
}

/******************************************************************************
 * MccSlider::sat
 *****************************************************************************/
int MccSlider::sat() const
{
    return ( d_data->m_ui->satSlider->value() );
}

/******************************************************************************
 * MccSlider::setSat
 *****************************************************************************/
void MccSlider::setSat( int sat )
{
    d_data->m_ui->satSlider->blockSignals( true );
    d_data->m_ui->satSlider->setValue( sat );
    d_data->m_ui->satSlider->blockSignals( false );
}

/******************************************************************************
 * MccSlider::blink
 *****************************************************************************/
bool MccSlider::blink() const
{
    return ( d_data->m_ui->blinkButton->isChecked() );
}

/******************************************************************************
 * MccSlider::setBlink
 *****************************************************************************/
void MccSlider::setBlink( bool blink )
{
    d_data->m_ui->blinkButton->blockSignals( true );
    d_data->m_ui->blinkButton->setChecked( blink );
    d_data->m_ui->blinkButton->blockSignals( false );
}

/******************************************************************************
 * MccSlider::minHue
 *****************************************************************************/
int MccSlider::minHue() const
{
    return ( d_data->m_minHue );
}

/******************************************************************************
 * MccSlider::setMinHue
 *****************************************************************************/
void MccSlider::setMinHue( int minHue )
{
    d_data->m_minHue = minHue;

    setupHueSliderRange();
}

/******************************************************************************
 * MccSlider::maxHue
 *****************************************************************************/
int MccSlider::maxHue() const
{
    return ( d_data->m_maxHue);
}

/******************************************************************************
 * MccSlider::setMaxHue
 *****************************************************************************/
void MccSlider::setMaxHue( int maxHue )
{
    d_data->m_maxHue = maxHue;

    setupHueSliderRange();
}

/******************************************************************************
 * MccSlider::minSat
 *****************************************************************************/
int MccSlider::minSat() const
{
    return ( d_data->m_minSat );
}

/******************************************************************************
 * MccSlider::setMinSat
 *****************************************************************************/
void MccSlider::setMinSat( int minSat )
{
    d_data->m_minSat = minSat;

    setupSatSliderRange();
}

/******************************************************************************
 * MccSlider::maxSat
 *****************************************************************************/
int MccSlider::maxSat() const
{
    return ( d_data->m_maxSat );
}

/******************************************************************************
 * MccSlider::setMaxSat
 *****************************************************************************/
void MccSlider::setMaxSat( int maxSat )
{
    d_data->m_minSat = maxSat;

    setupSatSliderRange();
}

/******************************************************************************
 * MccSlider::hueRange
 *****************************************************************************/
int MccSlider::hueRange() const
{
    return ( d_data->m_hueRange );
}

/******************************************************************************
 * MccSlider::setHueRange
 *****************************************************************************/
void MccSlider::setHueRange( int hueRange )
{
    d_data->m_hueRange = hueRange;

    setupHueSliderRange();
}

/******************************************************************************
 * MccSlider::satRange
 *****************************************************************************/
int MccSlider::satRange() const
{
    return ( d_data->m_satRange );
}

/******************************************************************************
 * MccSlider::setSatRange
 *****************************************************************************/
void MccSlider::setSatRange( int satRange )
{
    d_data->m_satRange = satRange;

    setupSatSliderRange();
}

/******************************************************************************
 * MccSlider::defaultHue
 *****************************************************************************/
int MccSlider::defaultHue() const
{
    return ( d_data->m_defaultHue );
}

/******************************************************************************
 * MccSlider::setDefaultHue
 *****************************************************************************/
void MccSlider::setDefaultHue( int defaultHue )
{
    d_data->m_defaultHue = defaultHue;

    setupHueSliderRange();
}

/******************************************************************************
 * MccSlider::defaultSat
 *****************************************************************************/
int MccSlider::defaultSat() const
{
    return ( d_data->m_defaultSat );
}

/******************************************************************************
 * MccSlider::setDefaultSat
 *****************************************************************************/
void MccSlider::setDefaultSat( int defaultSat )
{
    d_data->m_defaultSat = defaultSat;

    setupSatSliderRange();
}

/******************************************************************************
 * MccSlider::setupHueSliderRange
 *****************************************************************************/
void MccSlider::setupHueSliderRange()
{
    // get the maximum slider value either in positive (upwards) or negative (downward) direction
    int positiveRange = abs( d_data->m_maxHue - d_data->m_defaultHue );
    int negativeRange = abs( d_data->m_minHue - d_data->m_defaultHue );
    int maxRange = qMax(positiveRange, negativeRange);

    // get the new maximum value according to the defined range
    int maxScaledRange = (maxRange * d_data->m_hueRange) / 100;
    // do net exceed the min / max values
    int max = qMin( (d_data->m_defaultHue + maxScaledRange), d_data->m_maxHue );
    int min = qMax( (d_data->m_defaultHue - maxScaledRange), d_data->m_minHue );

    /* Note: Do not block signals to emit change events if
     * the slider value gets changed because it was bigger than
     * min / max */
    d_data->m_ui->hueSlider->setRange( min, max );

    /* Also adjust the step size, so that the user can use the arrow keys to adjust values
     * without having to step through more than 30000 values */
    d_data->m_ui->hueSlider->setSingleStep( (max - min) / 100 );
}

/******************************************************************************
 * MccSlider::setupSatSliderRange
 *****************************************************************************/
void MccSlider::setupSatSliderRange()
{
    // get the maximum slider value either in positive (upwards) or negative (downward) direction
    int positiveRange = abs( d_data->m_maxSat - d_data->m_defaultSat );
    int negativeRange = abs( d_data->m_minSat - d_data->m_defaultSat );
    int maxRange = qMax(positiveRange, negativeRange);

    // get the new maximum value according to the defined range
    int maxScaledRange = (maxRange * d_data->m_satRange) / 100;
    // do net exceed the min / max values
    int max = qMin( (d_data->m_defaultSat + maxScaledRange), d_data->m_maxSat );
    int min = qMax( (d_data->m_defaultSat - maxScaledRange), d_data->m_minSat );

    /* Note: Do not block signals to emit change events if
     * the slider value gets changed because it was bigger than
     * min / max */
    d_data->m_ui->satSlider->setRange( min, max );

    /* Also adjust the step size, so that the user can use the arrow keys to adjust values
     * without having to step through more than 30000 values */
    d_data->m_ui->satSlider->setSingleStep( (max - min) / 100 );
}

/******************************************************************************
 * MccSlider::onHueChange
 *****************************************************************************/
void MccSlider::onHueChange( int index, int hue )
{
    // If the hue of this phase changed, apply it
    if ( index == d_data->m_segmentIndex )
    {
        setHue( hue );
    }
}

/******************************************************************************
 * MccSlider::onSatChange
 *****************************************************************************/
void MccSlider::onSatChange( int index, int sat )
{
    // If the saturation of this phase got changed, apply it
    if ( index == d_data->m_segmentIndex )
    {
        setSat( sat );
    }
}

/******************************************************************************
 * MccSlider::onBlinkChange
 *****************************************************************************/
void MccSlider::onBlinkChange( int index, bool blink )
{
    // If the blink mode of this phase got changed, apply it
    if ( index == d_data->m_segmentIndex )
    {
        setBlink( blink );
    }
    // Else another phase has been selected, so set blink to false
    else
    {
        setBlink( false );
    }
}

/******************************************************************************
 * MccSlider::onHueRangeChange
 *****************************************************************************/
void MccSlider::onHueRangeChange( int range )
{
    d_data->m_hueRange = range;

    setupHueSliderRange();
}

/******************************************************************************
 * MccSlider::onSatRangeChange
 *****************************************************************************/
void MccSlider::onSatRangeChange( int range )
{
    d_data->m_satRange = range;

    setupSatSliderRange();
}

/******************************************************************************
 * MccSlider::onResetSlider
 *****************************************************************************/
void MccSlider::onResetSlider( int index )
{
    // Reset slider if index matches this sliders index, or all sliders have to be reset (index == -1)
    if ( (index == d_data->m_segmentIndex) || (index == -1) )
    {
        // Set sliders to default values (do not use the set function, because they block the change signal)
        d_data->m_ui->hueSlider->setValue( d_data->m_defaultHue );
        d_data->m_ui->satSlider->setValue( d_data->m_defaultSat );

        // deactivate blinking
        d_data->m_ui->blinkButton->setChecked( false );
    }
}

/******************************************************************************
 * MccSlider::onSliderSelected
 *****************************************************************************/
void MccSlider::onSliderSelected( int index )
{
    // If this slider is selected, color the slider grooves grey
    if ( index == d_data->m_segmentIndex )
    {
        d_data->m_ui->hueSlider->setStyleSheet( "QSlider::groove:vertical { background-color: darkGrey; }");
        d_data->m_ui->satSlider->setStyleSheet( "QSlider::groove:vertical { background-color: darkGrey; }");
    }
    // Else set default style
     else
    {
        d_data->m_ui->hueSlider->setStyleSheet( "" );
        d_data->m_ui->satSlider->setStyleSheet( "" );
    }
}

/******************************************************************************
 * MccSlider::onHueSliderChange
 *****************************************************************************/
void MccSlider::onHueSliderChange( int value )
{
    // Emit hue changed signal
    emit HueChanged( d_data->m_segmentIndex, value );
}

/******************************************************************************
 * MccSlider::onSatSliderChange
 *****************************************************************************/
void MccSlider::onSatSliderChange( int value )
{
    // Emit saturation changed signal
    emit SatChanged( d_data->m_segmentIndex, value );
}

/******************************************************************************
 * MccSlider::onBlinkButtonToggled
 *****************************************************************************/
void MccSlider::onBlinkButtonToggled( bool value )
{
    // Emit blink changed signal
    emit BlinkChanged( d_data->m_segmentIndex, value );
}
