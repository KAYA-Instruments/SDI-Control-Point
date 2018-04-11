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
 * @file    mcceqbox.cpp
 *
 * @brief   Implementation of multi color control box
 *
 *****************************************************************************/
#include <QtDebug>
#include <QFileDialog>

#include <mccslider.h>

#include "mcceqbox.h"
#include "ui_mcceqbox.h"

/******************************************************************************
 * namespaces
 *****************************************************************************/
namespace Ui {
    class UI_MccEqBox;
}

/******************************************************************************
 * local definitions
 *****************************************************************************/
#define MCC_OPERATION_MODE_MASK     ( 0xFFu )
#define MCC_OPERATION_MODE_SHIFT    ( 0u )
#define MCC_OPERATION_MODE(x)       ( (x & MCC_OPERATION_MODE_MASK) >> MCC_OPERATION_MODE_SHIFT )

#define MCC_NO_PHASE_MASK           ( 0xFFFFFF00u )
#define MCC_NO_PHASE_SHIFT          ( 8u )
#define MCC_NO_PHASE(x)             ( (x & MCC_NO_PHASE_MASK) >> MCC_NO_PHASE_SHIFT )

#define MCC_PHASE_MODE(x,y)         ( ((x << MCC_NO_PHASE_SHIFT) & MCC_NO_PHASE_MASK) |           \
                                      ((y << MCC_OPERATION_MODE_SHIFT) & MCC_OPERATION_MODE_MASK) )

#define MCC_SETTINGS_SECTION_NAME       ( "MCC" )
#define MCC_SETTINGS_ENABLE             ( "enable" )
#define MCC_SETTINGS_OP_MODE            ( "op_mode" )
#define MCC_SETTINGS_NO_PHASES          ( "no_phases" )
#define MCC_SETTINGS_HUE_RANGE          ( "hue_range" )
#define MCC_SETTINGS_SATURATION_RANGE   ( "saturation_range" )

#define MCC_SETTINGS_HUE(x)         ( "hue_" + QString::number(x) )
#define MCC_SETTINGS_SATURATION(x)  ( "saturation_" + QString::number(x) )

/******************************************************************************
 * fileExists
 *****************************************************************************/
static bool fileExists( QString & path )
{
    QFileInfo check_file( path );

    // check if file exists and if yes: Is it really a file and no directory?
    return ( check_file.exists() && check_file.isFile() );
}

/******************************************************************************
 * MccEqBox::PrivateData
 *****************************************************************************/
class MccEqBox::PrivateData
{
public:
    PrivateData()
        : m_ui( new Ui::UI_MccEqBox )
        , m_enable( false )
        , m_no_segments( MCC_DEFAULT_COLOR_PHASES )
        , m_active_segment( MCC_DEFAULT_ACTIVE_SEGMENT )
        , m_phase_hue( MCC_MAX_COLOR_PHASES )
        , m_phase_saturation( MCC_MAX_COLOR_PHASES )
        , m_hue_range( 100 )
        , m_sat_range( 100 )
    {
        int i;

        // set default hue values
        for ( i = 0; i < m_phase_hue.count(); ++i )
        {
            m_phase_hue[i] = MCC_HUE_DEFAULT;
        }
        
        // set default saturation values
        for ( i = 0; i < m_phase_saturation.count(); ++i )
        {
            m_phase_saturation[i] = MCC_SATURATION_DEFAULT;
        }
    };

    ~PrivateData()
    {
        delete m_ui;
    };

    Ui::UI_MccEqBox *  m_ui;                /**< ui handle */
    bool             m_enable;              /**< enable state */
    int              m_no_segments;         /**< number of segments / equalizer bands */
    int              m_active_segment;      /**< index of the currently selected segment */
    QVector<int>     m_phase_hue;           /**< hue settings */
    QVector<int>     m_phase_saturation;    /**< saturation settings */
    int              m_hue_range;           /**< range of the hue equalizer sliders in % */
    int              m_sat_range;           /**< range of the saturation equalizer sliders in % */

    QString          m_filename;
};

/******************************************************************************
 * MccEqBox::MccEqBox
 *****************************************************************************/
MccEqBox::MccEqBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData;

    // initialize UI
    d_data->m_ui->setupUi( this );

    // Add operational modes
    addOperationMode( 0, 12 );
    addOperationMode( 1, 16 );
    addOperationMode( MCC_DEFAULT_OPERATION_MODE, MCC_DEFAULT_COLOR_PHASES );
    addOperationMode( 3, 32 );

    /* call hue and saturation changed events to setup the range of the selected phase
     * spin boxes and the text of the min / max labels */
    onHueRangeChange( MCC_HUE_DEFAULT_RANGE );
    onSatRangeChange( MCC_SATURATION_DEFAULT_RANGE );

    // Equalizer bands / segments
    setupEqualizer();

    // connect internal signals
    // global MCC settings
    connect( d_data->m_ui->cbxMccEnable, SIGNAL(stateChanged(int)), this, SLOT(onEnableChange(int)) );
    connect( d_data->m_ui->cbxNoPhases, SIGNAL(activated(int)), this, SLOT(onOpModeChange(int)) );
    connect( d_data->m_ui->btnLoad, SIGNAL(clicked()), this, SLOT(onLoadProfileClick()) );
    connect( d_data->m_ui->btnSave, SIGNAL(clicked()), this, SLOT(onSaveProfileClick()) );

    // equalizer settings
    connect( d_data->m_ui->sbxHueRange, SIGNAL(valueChanged(int)), this, SLOT(onHueRangeChange(int)) );
    connect( d_data->m_ui->sbxSaturationRange, SIGNAL(valueChanged(int)), this, SLOT(onSatRangeChange(int)) );
    connect( d_data->m_ui->btnResetEqualizer, SIGNAL(clicked()), this, SLOT(onResetEqualizerClick()) );

    // selected phase settings
    connect( d_data->m_ui->sbxHue, SIGNAL(valueChanged(double)), this, SLOT(onHueSpinBoxChange(double)) );
    connect( d_data->m_ui->sbxSaturation, SIGNAL(valueChanged(double)), this, SLOT(onSatSpinBoxChange(double)) );
    connect( d_data->m_ui->btnResetPhase, SIGNAL(clicked()), this, SLOT(onResetPhaseClick()) );
}

/******************************************************************************
 * MccEqBox::~MccEqBox
 *****************************************************************************/
MccEqBox::~MccEqBox()
{
    // Stop blinking when widget is closed
    emit MccPhaseIndexChanged( -1 );

    delete d_data;
}

/******************************************************************************
 * MccEqBox::addOperationMode
 *****************************************************************************/
void MccEqBox::addOperationMode( int mode, int no_phases )
{
    uint32_t value = MCC_PHASE_MODE( no_phases, mode );

    d_data->m_ui->cbxNoPhases->blockSignals( true );
    d_data->m_ui->cbxNoPhases->addItem( QString::number(no_phases), QVariant(value) );
    d_data->m_ui->cbxNoPhases->model()->sort( 0 );
    d_data->m_ui->cbxNoPhases->blockSignals( false );
}

/******************************************************************************
 * MccEqBox::MccEnable
 *****************************************************************************/
bool MccEqBox::MccEnable() const
{
    return ( d_data->m_enable );
}

/******************************************************************************
 * MccEqBox::setMccEnable
 *****************************************************************************/
void MccEqBox::setMccEnable( const bool enable )
{
    if ( enable != d_data->m_enable )
    {
        // set value of checkbox
        d_data->m_ui->cbxMccEnable->setCheckState( enable ? Qt::Checked : Qt::Unchecked );

        d_data->m_enable = enable;

        emit MccEnableChanged( enable );
    }
}

/******************************************************************************
 * MccEqBox::MccOpMode
 *****************************************************************************/
int MccEqBox::MccOpMode() const
{
    int v = d_data->m_ui->cbxNoPhases->itemData(
                d_data->m_ui->cbxNoPhases->currentIndex() ).toUInt();
    return ( MCC_OPERATION_MODE( v ) );
}

/******************************************************************************
 * MccEqBox::MccNoPhases
 *****************************************************************************/
int MccEqBox::MccNoPhases() const
{
    int v = d_data->m_ui->cbxNoPhases->itemData(
                d_data->m_ui->cbxNoPhases->currentIndex() ).toUInt();
    return ( MCC_NO_PHASE( v ) );
}

/******************************************************************************
 * MccEqBox::setMccOpMode
 *****************************************************************************/
void MccEqBox::setMccOpMode( const int mode, const int no_phases )
{
    uint32_t value = MCC_PHASE_MODE( no_phases, mode );
    int index = d_data->m_ui->cbxNoPhases->findData( value );
    if ( index >= 0 )
    {
        d_data->m_ui->cbxNoPhases->setCurrentIndex( index );
        d_data->m_no_segments = no_phases;

        emit MccOperationModeChanged( mode, no_phases );

        setupEqualizer();
    }
}

/******************************************************************************
 * MccEqBox::MccHue
 *****************************************************************************/
int MccEqBox::MccHue( const int id ) const
{
    if ( (id>=0) && (id < MCC_MAX_COLOR_PHASES) )
    {
        return ( d_data->m_phase_hue[id] );
    }
    
    return ( 0 );
}

/******************************************************************************
 * MccEqBox::setMccHue
 *****************************************************************************/
void MccEqBox::setMccHue( const int id, const int hue )
{
    if ( (id>=0) && (id < MCC_MAX_COLOR_PHASES) )
    {
        d_data->m_phase_hue[id] = hue;

        // emit internal hue changed signal to adjust the according slider
        emit HueChanged( id, hue );
    }
}

/******************************************************************************
 * MccEqBox::MccSaturation
 *****************************************************************************/
int MccEqBox::MccSaturation( const int id ) const
{
    if ( (id>=0) && (id < MCC_MAX_COLOR_PHASES) )
    {
        return ( d_data->m_phase_saturation[id] );
    }
    
    return ( 0 );
}

/******************************************************************************
 * MccEqBox::setMccSaturation
 *****************************************************************************/
void MccEqBox::setMccSaturation( const int id, const int saturation )
{
    if ( (id>=0) && (id < MCC_MAX_COLOR_PHASES) )
    {
        d_data->m_phase_saturation[id] = saturation;
       
        // emit internal saturation changed signal to adjust the according slider
        emit SatChanged( id, saturation );
    }
}

/******************************************************************************
 * MccEqBox::HueRange
 *****************************************************************************/
int MccEqBox::HueRange() const
{
    return d_data->m_hue_range;
}

/******************************************************************************
 * MccEqBox::setHueRange
 *****************************************************************************/
void MccEqBox::setHueRange( int range )
{
    if ( range >= 1 && range <= 100 )
    {
        d_data->m_hue_range = range;

        // Change value of slider range spinbox
        d_data->m_ui->sbxHueRange->setValue( range );

        // emit internal slider range changed signal to adjust all sliders
        emit HueRangeChanged( range );
    }
}

/******************************************************************************
 * MccEqBox::SatRange
 *****************************************************************************/
int MccEqBox::SatRange() const
{
    return d_data->m_sat_range;
}

/******************************************************************************
 * MccEqBox::setSatRange
 *****************************************************************************/
void MccEqBox::setSatRange( int range )
{
    if ( range >= 1 && range <= 100 )
    {
        d_data->m_sat_range = range;

        // Change value of slider range spinbox
        d_data->m_ui->sbxSaturationRange->setValue( range );

        // emit internal slider range changed signal to adjust all sliders
        emit SatRangeChanged( range );
    }
}


/******************************************************************************
 * setupEqualizer
 *****************************************************************************/
void MccEqBox::setupEqualizer()
{
    // Get layout of equalizer group box widget
    QLayout * layout = d_data->m_ui->gbxEqualizer->layout();

    // Delete all sliders in the layout if the amount of sliders
    QLayoutItem * item;
    while ( (item = layout->takeAt(0)) != 0 )
    {
        delete item->widget();
        delete item;
    }

    // Calculate angle per segment and start angle to color the slider buttons
    float anglePerSegment = 360.0 / (float)d_data->m_no_segments;
    float startAngle = anglePerSegment / 2.0;

    // Add one slider for each segment of the MCC
    MccSlider * slider;
    for ( int segment = 0; segment < d_data->m_no_segments; segment++ )
    {
        // calculate the angle of this segment
        int hueAngle = qRound( startAngle + (float)segment * anglePerSegment );

        // create slider
        slider = new MccSlider( segment, QColor::fromHsv( hueAngle, 255, 255 ),
                                MCC_HUE_MIN, MCC_HUE_MAX,
                                MCC_SATURATION_MIN, MCC_SATURATION_MAX,
                                d_data->m_hue_range, d_data->m_sat_range,
                                MCC_HUE_DEFAULT, MCC_SATURATION_DEFAULT,
                                d_data->m_ui->gbxEqualizer );

        // connect slider slots / signals
        connect( slider, SIGNAL(HueChanged(int,int)), this, SLOT(onHueChange(int,int)) );
        connect( slider, SIGNAL(SatChanged(int,int)), this, SLOT(onSatChange(int,int)) );
        connect( slider, SIGNAL(BlinkChanged(int,bool)), this, SLOT(onBlinkChange(int,bool)) );

        /* If the MccEqBox receives a hue / saturation / blink changed signal it will in turn emit a
         * hue / sat / blink changed signal to all sliders. Each slider will decide whether it has to
         * react to this event or not, according to the index given in the event */
        connect( this, SIGNAL(HueChanged(int,int)), slider, SLOT(onHueChange(int,int)) );
        connect( this, SIGNAL(SatChanged(int,int)), slider, SLOT(onSatChange(int,int)) );
        connect( this, SIGNAL(BlinkChanged(int,bool)), slider, SLOT(onBlinkChange(int,bool)) );

        // If the slider range gets changed, all sliders have to adjust their range
        connect( this, SIGNAL(HueRangeChanged(int)), slider, SLOT(onHueRangeChange(int)) );
        connect( this, SIGNAL(SatRangeChanged(int)), slider, SLOT(onSatRangeChange(int)) );

        // Connect the reset signal
        connect( this, SIGNAL(ResetPhase(int)), slider, SLOT(onResetSlider(int)) );

        // Connect the segement selected signal
        connect ( this, SIGNAL(SegmentSelected(int)), slider, SLOT(onSliderSelected(int)) );

        // add slider to layout
        layout->addWidget( slider );
    }

    deselectActiveSegment();
}

/******************************************************************************
 * deselectActiveSegment
 *****************************************************************************/
void MccEqBox::deselectActiveSegment()
{
    // set value of both spin boxes to 0.0
    d_data->m_ui->sbxHue->blockSignals( true );
    d_data->m_ui->sbxHue->setValue( 0.0 );
    d_data->m_ui->sbxHue->blockSignals( false );

    d_data->m_ui->sbxSaturation->blockSignals( true );
    d_data->m_ui->sbxSaturation->setValue( 0.0 );
    d_data->m_ui->sbxSaturation->blockSignals( false );

    // deactivate the spin boxes (will be reactivated when user adjusts a slider)
    d_data->m_ui->sbxHue->setEnabled( false );
    d_data->m_ui->sbxSaturation->setEnabled( false );
}

/******************************************************************************
 * MccEqBox::prepareMode
 *****************************************************************************/
void MccEqBox::prepareMode( const Mode mode )
{
    (void)mode;

    // do nothing here
}

/******************************************************************************
 * MccEqBox::loadSettings
 *****************************************************************************/
void MccEqBox::loadSettings( QSettings & s )
{
    int i;

    s.beginGroup( MCC_SETTINGS_SECTION_NAME );
   
    // enable
    setMccEnable( s.value( MCC_SETTINGS_ENABLE ).toBool() );

    // operation mode and number of color phases
    setMccOpMode(
        s.value( MCC_SETTINGS_OP_MODE ).toInt(),
        s.value( MCC_SETTINGS_NO_PHASES ).toInt() );

    // Slider Ranges
    setHueRange( s.value( MCC_SETTINGS_HUE_RANGE ).toInt() );
    setSatRange( s.value( MCC_SETTINGS_SATURATION_RANGE ).toInt() );

    // color phases
    for ( i = 0; i < d_data->m_no_segments; i++ )
    {
        int hue = s.value( MCC_SETTINGS_HUE(i) ).toInt();
        int saturation = s.value( MCC_SETTINGS_SATURATION(i) ).toInt();

        // emit event to set phase on device
        emit MccPhaseChanged( i, saturation, hue );

        // emit events to adjust sliders
        emit HueChanged( i, hue );
        emit SatChanged( i, saturation );
    }

    s.endGroup();
}

/******************************************************************************
 * MccEqBox::saveSettings
 *****************************************************************************/
void MccEqBox::saveSettings( QSettings & s )
{
    int i;

    s.beginGroup( MCC_SETTINGS_SECTION_NAME );

    s.setValue( MCC_SETTINGS_ENABLE          , MccEnable() );
    s.setValue( MCC_SETTINGS_OP_MODE         , MccOpMode() );
    s.setValue( MCC_SETTINGS_NO_PHASES       , MccNoPhases() );
    s.setValue( MCC_SETTINGS_HUE_RANGE       , HueRange() );
    s.setValue( MCC_SETTINGS_SATURATION_RANGE, SatRange() );
    
    for ( i = 0; i < MCC_MAX_COLOR_PHASES; i++ )
    {
        s.setValue( MCC_SETTINGS_HUE(i)        , MccHue( i ) );
        s.setValue( MCC_SETTINGS_SATURATION(i) , MccSaturation( i ) );
    }
    
    s.endGroup();
}

/******************************************************************************
 * MccEqBox::applySettings
 *****************************************************************************/
void MccEqBox::applySettings( void )
{
    emit MccEnableChanged( MccEnable() );

    emit MccOperationModeChanged( MccOpMode(), MccNoPhases() );

    for ( int i = 0; i < MccNoPhases(); i++ )
    {
        emit MccPhaseChanged( i, MccSaturation( i ), MccHue( i ) );
    }
}

/******************************************************************************
 * MccEqBox::onMccEnableChange
 *****************************************************************************/
void MccEqBox::onMccEnableChange( const int flag )
{
    bool bEnable = flag ? true : false;
    if ( bEnable != d_data->m_enable )
    {
        // set value of checkbox
        d_data->m_ui->cbxMccEnable->blockSignals( true );
        d_data->m_ui->cbxMccEnable->setCheckState( bEnable ? Qt::Checked : Qt::Unchecked );
        d_data->m_ui->cbxMccEnable->blockSignals( false );
        
        d_data->m_enable = bEnable;
    }
}

/******************************************************************************
 * MccEqBox::onMccOperationModeChange
 *****************************************************************************/
void MccEqBox::onMccOperationModeChange( int mode, int no_phases )
{
    uint32_t value = MCC_PHASE_MODE( no_phases, mode );
    int index = d_data->m_ui->cbxNoPhases->findData( value );
    if ( index >= 0 )
    {
        d_data->m_ui->cbxNoPhases->setCurrentIndex( index );
        d_data->m_no_segments = no_phases;

        setupEqualizer();
    }
}

/******************************************************************************
 * MccEqBox::onMccPhaseChange
 *****************************************************************************/
void MccEqBox::onMccPhaseChange( int id, int saturation, int hue )
{
    if ( (id>=0) && (id < MCC_MAX_COLOR_PHASES) )
    {
        // save new values
        d_data->m_phase_hue[id]        = hue;
        d_data->m_phase_saturation[id] = saturation;

        // emit events to adjust sliders
        emit HueChanged( id, hue );
        emit SatChanged( id, saturation );
    }
}

/******************************************************************************
 * MccEqBox::onEnableChange
 *****************************************************************************/
void MccEqBox::onEnableChange( int enable )
{
    d_data->m_enable = enable ? true : false;
    emit MccEnableChanged( enable );
}

/******************************************************************************
 * MccEqBox::onOpModeChange
 *****************************************************************************/
void MccEqBox::onOpModeChange( int index )
{
    int v = d_data->m_ui->cbxNoPhases->itemData( index ).toUInt();
    int mode        = MCC_OPERATION_MODE( v );
    int no_phases   = MCC_NO_PHASE( v );

    if ( no_phases !=  d_data->m_no_segments )
    {
        // stop blinking
        emit MccPhaseIndexChanged( -1 );

        // emit operation mode changed signal
        emit MccOperationModeChanged( mode, no_phases );

        // save new number of phases in local data
        d_data->m_no_segments = no_phases;

        // setup equalizer with new number of segments
        setupEqualizer();
    }
}

/******************************************************************************
 * MccEqBox::onHueChange
 *****************************************************************************/
void MccEqBox::onHueChange( int id, int hue )
{
    d_data->m_phase_hue[id] = hue;

    // signal that a new slider got selected, if the id changed
    if ( d_data->m_active_segment != id )
    {
        d_data->m_active_segment = id;
        emit SegmentSelected( id );

        // enable the spin boxes (they are disabled when no phase is selected)
        d_data->m_ui->sbxHue->setEnabled( true );
        d_data->m_ui->sbxSaturation->setEnabled( true );

        // setup saturation value in saturation spin box from stored values
        d_data->m_ui->sbxSaturation->setValue( (float)d_data->m_phase_saturation[id] / MCC_SATURATION_DIVISOR );
    }

    // setup hue value in hue spin box with new value
    d_data->m_ui->sbxHue->setValue( (float)hue / MCC_HUE_DIVISOR );

    // emit phase changed signal to transmit new values to the device
    emit MccPhaseChanged( id, d_data->m_phase_saturation[id], hue );
}

/******************************************************************************
 * MccEqBox::onSatChange
 *****************************************************************************/
void MccEqBox::onSatChange( int id, int sat )
{
    d_data->m_phase_saturation[id] = sat;

    // signal that a new slider got selected, if the id changed
    if ( d_data->m_active_segment != id )
    {
        d_data->m_active_segment = id;
        emit SegmentSelected( id );

        // enable the spin boxes (they are disabled when no phase is selected)
        d_data->m_ui->sbxHue->setEnabled( true );
        d_data->m_ui->sbxSaturation->setEnabled( true );

        // setup hue value in hue spin box from stored values
        d_data->m_ui->sbxHue->setValue( (float)d_data->m_phase_hue[id] / MCC_HUE_DIVISOR );
    }

    // setup saturation value in sat spin box with new value
    d_data->m_ui->sbxSaturation->setValue( (float)sat / MCC_SATURATION_DIVISOR );

    // emit phase changed signal transmit new values to the device
    emit MccPhaseChanged( id, sat, d_data->m_phase_hue[id] );
}

/******************************************************************************
 * MccEqBox::onBlinkChange
 *****************************************************************************/
void MccEqBox::onBlinkChange( int id, bool blink)
{
    // signal that a new slider got selected, if the id changed
    if ( d_data->m_active_segment != id )
    {
        d_data->m_active_segment = id;
        emit SegmentSelected( id );

        // enable the spin boxes (they are disabled when no phase is selected)
        d_data->m_ui->sbxHue->setEnabled( true );
        d_data->m_ui->sbxSaturation->setEnabled( true );
    }

    if ( blink )
    {
        // emit signal to uncheck all other blink buttons
        emit BlinkChanged( id , blink );

        // emit signal to enable blinking of this phase on the device
        emit MccPhaseIndexChanged( id );
    }
    else
    {
        // emit signal to stop blinking on the device
        emit MccPhaseIndexChanged( -1 );
    }
}

/******************************************************************************
 * MccEqBox::onHueRangeChange
 *****************************************************************************/
void MccEqBox::onHueRangeChange( int range )
{
    d_data->m_hue_range = range;

    // emit internal slider range changed signal to adjust all sliders
    emit HueRangeChanged( range );

    /* Calculate the maximum and minimum range of hue sliders and adjust the
     * labels and spin box range accordingly. This calculation will also be
     * done in the sliders. */
    int positiveRange, negativeRange, maxRange, maxScaledRange;
    float max, min;

    // get the maximum slider value either in positive (upwards) or negative (downward) direction
    positiveRange = abs( MCC_HUE_MAX - MCC_HUE_DEFAULT );
    negativeRange = abs( MCC_HUE_MIN - MCC_HUE_DEFAULT );
    maxRange = qMax(positiveRange, negativeRange);

    // get the new maximum value according to the defined range
    maxScaledRange = (maxRange * range) / 100;
    // do net exceed the min / max values
    max = (float)qMin( (MCC_HUE_DEFAULT + maxScaledRange), MCC_HUE_MAX ) / (float)MCC_HUE_DIVISOR;
    min = (float)qMax( (MCC_HUE_DEFAULT - maxScaledRange), MCC_HUE_MIN ) / (float)MCC_HUE_DIVISOR;

    // Get the min / max values of the slider and adjust the labels accordingly
    d_data->m_ui->lblMaxHue->setText( QString::number( max, 'f', 2 ) );
    d_data->m_ui->lblMinHue->setText( QString::number( min, 'f', 2 ) );

    // Set range of the hue spin box
    d_data->m_ui->sbxHue->setRange( min, max );
}

/******************************************************************************
 * MccEqBox::onSatRangeChange
 *****************************************************************************/
void MccEqBox::onSatRangeChange( int range )
{
    d_data->m_sat_range = range;

    // emit internal slider range changed signal to adjust all sliders
    emit SatRangeChanged( range );

    /* Calculate the maximum and minimum range of saturation sliders and adjust the
     * labels and spin box range accordingly. This calculation will also be done in
     * the sliders. */
    int positiveRange, negativeRange, maxRange, maxScaledRange;
    float max, min;

    // get the maximum slider value either in positive (upwards) or negative (downward) direction
    positiveRange = abs( MCC_SATURATION_MAX - MCC_SATURATION_DEFAULT );
    negativeRange = abs( MCC_SATURATION_MIN - MCC_SATURATION_DEFAULT );
    maxRange = qMax(positiveRange, negativeRange);

    // get the new maximum value according to the defined range
    maxScaledRange = (maxRange * range) / 100;
    // do net exceed the min / max values
    max = (float)qMin( (MCC_SATURATION_DEFAULT + maxScaledRange), MCC_SATURATION_MAX ) / (float)MCC_SATURATION_DIVISOR;
    min = (float)qMax( (MCC_SATURATION_DEFAULT - maxScaledRange), MCC_SATURATION_MIN ) / (float)MCC_SATURATION_DIVISOR;

    // Get the min / max values of the slider and adjust the labels accordingly
    d_data->m_ui->lblMaxSat->setText( QString::number( max, 'f', 2 ) );
    d_data->m_ui->lblMinSat->setText( QString::number( min, 'f', 2 ) );

    // Set range of the saturation spin box
    d_data->m_ui->sbxSaturation->setRange( min, max );
}

/******************************************************************************
 * MccEqBox::onResetEqualizerClick
 *****************************************************************************/
void MccEqBox::onResetEqualizerClick()
{
    // Stop blinking
    emit MccPhaseIndexChanged( -1 );
    // Reset all sliders
    emit ResetPhase( - 1 );
    // deselect current segment
    d_data->m_active_segment = -1;
    emit SegmentSelected( -1 );
    deselectActiveSegment();
}

/******************************************************************************
 * MccEqBox::onHueSpinBoxChange
 *****************************************************************************/
void MccEqBox::onHueSpinBoxChange( double value )
{
    int activeSegment = d_data->m_active_segment;

    // Check if a segment is currently selected
    if ( activeSegment < 0 )
    {
        return;
    }

    // calculate integer hue value
    int hue = value * MCC_HUE_DIVISOR;

    // emit hue changed signal to adjust the slider
    emit HueChanged( activeSegment, hue );

    // emit phase changed signal to adjust MCC on the device
    emit MccPhaseChanged( activeSegment, d_data->m_phase_saturation[activeSegment], hue );
}

/******************************************************************************
 * MccEqBox::onSatSpinBoxChange
 *****************************************************************************/
void MccEqBox::onSatSpinBoxChange( double value )
{
    int activeSegment = d_data->m_active_segment;

    // Check if a segment is currently selected
    if ( activeSegment < 0 )
    {
        return;
    }

    // calculate integer hue value
    int sat = value * MCC_SATURATION_DIVISOR;

    // emit hue changed signal to adjust the slider
    emit SatChanged( activeSegment, sat );

    // emit phase changed signal to adjust MCC on the device
    emit MccPhaseChanged( activeSegment, sat, d_data->m_phase_hue[activeSegment] );
}

/******************************************************************************
 * MccEqBox::onResetPhaseClick
 *****************************************************************************/
void MccEqBox::onResetPhaseClick()
{
    // Check if a segment is currently selected
    if ( d_data->m_active_segment < 0 )
    {
        return;
    }

    // Reset active phase
    emit ResetPhase( d_data->m_active_segment );
}

/******************************************************************************
 * MccEqBox::onLoadProfileClick
 *****************************************************************************/
void MccEqBox::onLoadProfileClick()
{
    QString directory = QDir::currentPath();

    // NOTE: It can fail on gtk-systems when an empty filename is given
    //       in the native dialog-box, because GTK sends a SIGSEGV-signal
    //       to process and this is not handled by Qt.
    QFileDialog dialog( this );
    dialog.setDefaultSuffix( "mcc" );
    d_data->m_filename = dialog.getOpenFileName(
        this, tr("Load Multi-Color Profile"),
        directory,
        "Select Profile Files (*.mcc);;All files (*.*)"
    );

    if ( NULL != d_data->m_filename )
    {
        QFileInfo file( d_data->m_filename );
        if ( file.suffix().isEmpty() )
        {
            d_data->m_filename += ".mcc";
        }

        if ( fileExists(d_data->m_filename) )
        {
            QSettings settings( d_data->m_filename, QSettings::IniFormat );
            loadSettings( settings );
        }
    }
}

/******************************************************************************
 * MccEqBox::onSaveProfileClick
 *****************************************************************************/
void MccEqBox::onSaveProfileClick()
{
    QString directory = QDir::currentPath();

    // NOTE: It can fail on gtk-systems when an empty filename is given
    //       in the native dialog-box, because GTK sends a SIGSEGV-signal
    //       to process and this is not handled by Qt.
    QFileDialog dialog( this );
    dialog.setDefaultSuffix( "mcc" );
    d_data->m_filename = dialog.getSaveFileName(
        this, tr("Save Multi-Color Profile"),
        directory,
        "Select Profile Files (*.mcc);;All files (*.*)"
    );

    if ( NULL != d_data->m_filename )
    {
        QFileInfo file( d_data->m_filename );
        if ( file.suffix().isEmpty() )
        {
            d_data->m_filename += ".mcc";
        }

        QSettings settings( d_data->m_filename, QSettings::IniFormat );
        saveSettings( settings );
    }
}
