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
 * @file    mccbox.cpp
 *
 * @brief   Implementation of multi color control box 
 *
 *****************************************************************************/
#include <QtDebug>
#include <QFileDialog>

#include "mccbox.h"
#include "ui_mccbox.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_MccBox;
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

#define MCC_SETTINGS_SECTION_NAME   ( "MCC" )
#define MCC_SETTINGS_ENABLE         ( "enable" )
#define MCC_SETTINGS_OP_MODE        ( "op_mode" )
#define MCC_SETTINGS_NO_PHASES      ( "no_phases" )

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
 * MccBox::PrivateData
 *****************************************************************************/
class MccBox::PrivateData
{
public:
    PrivateData()
        : m_ui( new Ui::UI_MccBox )
        , m_enable( false )
        , m_phase( 0 )
        , m_phase_hue( MCC_MAX_COLOR_PHASES )
        , m_phase_saturation( MCC_MAX_COLOR_PHASES )
    {
        int i;

        // set default hue values
        for ( i = 0; i < m_phase_hue.count(); ++i )
        {
            m_phase_hue[i] = 0;
        }
        
        // set default saturation values
        for ( i = 0; i < m_phase_saturation.count(); ++i )
        {
            m_phase_saturation[i] = 1 << MCC_SATURATION_COMMA_POSITION;
        }
    };

    ~PrivateData()
    {
        delete m_ui;
    };

    Ui::UI_MccBox *  m_ui;                  /**< ui handle */
    bool             m_enable;              /**< enable state */
    int              m_phase;               /**< currently selected color phase */
    QVector<int>     m_phase_hue;           /**< hue settings */
    QVector<int>     m_phase_saturation;    /**< saturation settings */

    QString          m_filename;
};

/******************************************************************************
 * MccBox::MccBox
 *****************************************************************************/
MccBox::MccBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData;

    // initialize UI
    d_data->m_ui->setupUi( this );

    ////////////////////
    // MCC phase mode
    ////////////////////
    
    // add operational modes
    addOperationMode( 0, 12 );
    addOperationMode( 1, 16 );
    addOperationMode( MCC_DEFAULT_OPERATION_MODE, MCC_DEFAULT_COLOR_PHASES );
    addOperationMode( 3, 32 );

    // set amount of phases and get selected phase
    d_data->m_ui->HueSegment->setNoSegments( MCC_DEFAULT_COLOR_PHASES );
    d_data->m_phase = d_data->m_ui->HueSegment->selectedIndex();

    ////////////////////
    // saturation
    ////////////////////

    // configure saturation
    d_data->m_ui->Saturation->setRange( MCC_SATURATION_MIN, MCC_SATURATION_MAX );
    d_data->m_ui->Saturation->setCommaPos( MCC_SATURATION_COMMA_POSITION );
    d_data->m_ui->Saturation->setDisplayMultiplier( MCC_SATURATION_DISPLAY_MULTIPLIER );
    d_data->m_ui->Saturation->setBase( MCC_SATURATION_BASE );
    d_data->m_ui->Saturation->setMaxAngle( 180 );
    d_data->m_ui->Saturation->setMaxRounds( 4 );
    d_data->m_ui->Saturation->setFmt( MCC_SATURATION_DISPLAY_MASK );

    ////////////////////
    // hue
    ////////////////////

    // configure hue 
    d_data->m_ui->Hue->setRange( MCC_HUE_MIN, MCC_HUE_MAX );
    d_data->m_ui->Hue->setCommaPos( MCC_HUE_COMMA_POSITION );
    d_data->m_ui->Hue->setDisplayMultiplier( MCC_HUE_DISPLAY_MULTIPLIER );
    d_data->m_ui->Hue->setBase( MCC_HUE_BASE );
    d_data->m_ui->Hue->setMaxAngle( 180 );
    d_data->m_ui->Hue->setMaxRounds( 4 );
    d_data->m_ui->Hue->setFmt( MCC_HUE_DISPLAY_MASK );

    // connect internal signals
    connect( d_data->m_ui->cbxMccEnable, SIGNAL(stateChanged(int)), this, SIGNAL(MccEnableChanged(int)) );
    connect( d_data->m_ui->Hue, SIGNAL(ValueChanged(int)), this, SLOT(onHueChange(int)) );
    connect( d_data->m_ui->HueSegment, SIGNAL(clicked(int)), this, SLOT(onPhaseChange(int)) );
    connect( d_data->m_ui->Saturation, SIGNAL(ValueChanged(int)), this, SLOT(onSaturationChange(int)) );
    connect( d_data->m_ui->cbxNoPhases, SIGNAL(activated(int)), this, SLOT(onOpModeChange(int)) );

    connect( d_data->m_ui->btnLoad, SIGNAL(clicked()), this, SLOT(onLoadProfileClick()) );
    connect( d_data->m_ui->btnSave, SIGNAL(clicked()), this, SLOT(onSaveProfileClick()) );
}

/******************************************************************************
 * MccBox::~MccBox
 *****************************************************************************/
MccBox::~MccBox()
{
    delete d_data;
}

/******************************************************************************
 * MccBox::addOperationMode
 *****************************************************************************/
void MccBox::addOperationMode( int mode, int no_phases )
{
    uint32_t value = MCC_PHASE_MODE( no_phases, mode );

    d_data->m_ui->cbxNoPhases->blockSignals( true );
    d_data->m_ui->cbxNoPhases->addItem( QString::number(no_phases), QVariant(value) );
    d_data->m_ui->cbxNoPhases->model()->sort( 0 );
    d_data->m_ui->cbxNoPhases->blockSignals( false );
}

/******************************************************************************
 * MccBox::MccEnable
 *****************************************************************************/
bool MccBox::MccEnable() const
{
    return ( d_data->m_enable );
}

/******************************************************************************
 * MccBox::setMccEnable
 *****************************************************************************/
void MccBox::setMccEnable( const bool enable )
{
    if ( enable != d_data->m_enable )
    {
        // set value of checkbox
        d_data->m_ui->cbxMccEnable->setCheckState( enable ? Qt::Checked : Qt::Unchecked );

        d_data->m_enable = enable;
    }
}

/******************************************************************************
 * MccBox::MccOpMode
 *****************************************************************************/
int MccBox::MccOpMode() const
{
    int v = d_data->m_ui->cbxNoPhases->itemData( 
                d_data->m_ui->cbxNoPhases->currentIndex() ).toUInt();
    return ( MCC_OPERATION_MODE( v ) );
}

/******************************************************************************
 * MccBox::MccNoPhases
 *****************************************************************************/
int MccBox::MccNoPhases() const
{
    int v = d_data->m_ui->cbxNoPhases->itemData( 
                d_data->m_ui->cbxNoPhases->currentIndex() ).toUInt();
    return ( MCC_NO_PHASE( v ) );
}

/******************************************************************************
 * MccBox::setMccOpMode
 *****************************************************************************/
void MccBox::setMccOpMode( const int mode, const int no_phases )
{
    uint32_t value = MCC_PHASE_MODE( no_phases, mode );
    int index = d_data->m_ui->cbxNoPhases->findData( value );
    if ( index >= 0 )
    {
        d_data->m_ui->cbxNoPhases->setCurrentIndex( index );
        if ( no_phases !=  d_data->m_ui->HueSegment->noSegments() )
        {
            // reset selected phase
            d_data->m_phase = 0;
        
            // hue viewer
            d_data->m_ui->HueViewer->setNoSegments( no_phases );
            d_data->m_ui->HueViewer->onSegmentChange( 0 );
            d_data->m_ui->HueViewer->onHueChange( d_data->m_phase_hue[0] );

            // hue segment select
            d_data->m_ui->HueSegment->setNoSegments( no_phases );
            d_data->m_ui->HueSegment->setSelectedIndex( 0 );

            // hue knob-box
            d_data->m_ui->Hue->onValueChange( d_data->m_phase_hue[0] );

            // saturation knob-box
            d_data->m_ui->Saturation->onValueChange( d_data->m_phase_saturation[0] );
        }

        emit MccOperationModeChanged( mode, no_phases );
    }
}

/******************************************************************************
 * MccBox::MccHue
 *****************************************************************************/
int MccBox::MccHue( const int id ) const
{
    if ( (id>=0) && (id < MCC_MAX_COLOR_PHASES) )
    {
        return ( d_data->m_phase_hue[id] );
    }
    
    return ( 0 );
}

/******************************************************************************
 * MccBox::setMccHue
 *****************************************************************************/
void MccBox::setMccHue( const int id, const int hue )
{
    if ( (id>=0) && (id < MCC_MAX_COLOR_PHASES) )
    {
        d_data->m_phase_hue[id] = hue;

        // current phase ?
        if ( id == d_data->m_phase )
        {
            d_data->m_ui->Hue->onValueChange( hue );
            d_data->m_ui->HueViewer->onHueChange( hue );
        }
        
        emit MccPhaseChanged( id, d_data->m_phase_saturation[id], hue );
    }
}

/******************************************************************************
 * MccBox::MccSaturation
 *****************************************************************************/
int MccBox::MccSaturation( const int id ) const
{
    if ( (id>=0) && (id < MCC_MAX_COLOR_PHASES) )
    {
        return ( d_data->m_phase_saturation[id] );
    }
    
    return ( 0 );
}

/******************************************************************************
 * MccBox::setMccSaturation
 *****************************************************************************/
void MccBox::setMccSaturation( const int id, const int saturation )
{
    if ( (id>=0) && (id < MCC_MAX_COLOR_PHASES) )
    {
        d_data->m_phase_saturation[id] = saturation;
       
        // current phase ?
        if ( id == d_data->m_phase )
        {
            d_data->m_ui->Saturation->onValueChange( saturation );
        }

        emit MccPhaseChanged( id, saturation, d_data->m_phase_hue[id] );
    }
}

/******************************************************************************
 * MccBox::setMccPhase
 *****************************************************************************/
void MccBox::setMccPhase( const int id, const int saturation, const int hue )
{
    if ( (id>=0) && (id < MCC_MAX_COLOR_PHASES) )
    {
        d_data->m_phase_hue[id]        = hue;
        d_data->m_phase_saturation[id] = saturation;
       
        // current phase ?
        if ( id == d_data->m_phase )
        {
            d_data->m_ui->Hue->onValueChange( hue );
            d_data->m_ui->HueViewer->onHueChange( hue );
            d_data->m_ui->Saturation->onValueChange( saturation );
        }

        // Only emit the phase changed signal, for phases that are active on the device
        if ( id < MccNoPhases() )
        {
            emit MccPhaseChanged( id, saturation, hue );
        }
    }
}

/******************************************************************************
 * MccBox::prepareMode
 *****************************************************************************/
void MccBox::prepareMode( const Mode mode )
{
    (void)mode;

    // do nothing here
}

/******************************************************************************
 * MccBox::loadSettings
 *****************************************************************************/
void MccBox::loadSettings( QSettings & s )
{
    int i;

    s.beginGroup( MCC_SETTINGS_SECTION_NAME );
   
    // enable
    setMccEnable( s.value( MCC_SETTINGS_ENABLE ).toBool() );

    // operation mode and number of color phases
    setMccOpMode(
        s.value( MCC_SETTINGS_OP_MODE ).toInt(), 
        s.value( MCC_SETTINGS_NO_PHASES ).toInt() );

    // color phases
    for ( i = 0; i < MCC_MAX_COLOR_PHASES; i++ )
    {
        setMccPhase( i,
            s.value( MCC_SETTINGS_SATURATION(i) ).toInt(),
            s.value( MCC_SETTINGS_HUE(i) ).toInt() );
    }
 
    s.endGroup();
}

/******************************************************************************
 * MccBox::saveSettings
 *****************************************************************************/
void MccBox::saveSettings( QSettings & s )
{
    int i;

    s.beginGroup( MCC_SETTINGS_SECTION_NAME );

    s.setValue( MCC_SETTINGS_ENABLE   , MccEnable() );
    s.setValue( MCC_SETTINGS_OP_MODE  , MccOpMode() );
    s.setValue( MCC_SETTINGS_NO_PHASES, MccNoPhases() );
    
    for ( i = 0; i < MCC_MAX_COLOR_PHASES; i++ )
    {
        s.setValue( MCC_SETTINGS_HUE(i)        , MccHue( i ) ); 
        s.setValue( MCC_SETTINGS_SATURATION(i) , MccSaturation( i ) ); 
    }
    
    s.endGroup();
}

/******************************************************************************
 * MccBox::applySettings
 *****************************************************************************/
void MccBox::applySettings( void )
{
    int i;

    emit MccEnableChanged( MccEnable() );
    emit MccOperationModeChanged( MccOpMode(), MccNoPhases() );

    for ( i = 0; i < MCC_MAX_COLOR_PHASES; i++ )
    {
        emit MccPhaseChanged( i, MccSaturation( i ), MccHue( i ));
    }
}


/******************************************************************************
 * MccBox::onMccEnableChange
 *****************************************************************************/
void MccBox::onMccEnableChange( const int flag )
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
 * MccBox::onMccOperationModeChange
 *****************************************************************************/
void MccBox::onMccOperationModeChange( int mode, int no_phases )
{
    uint32_t value = MCC_PHASE_MODE( no_phases, mode );
    int index = d_data->m_ui->cbxNoPhases->findData( value );
    if ( index >= 0 )
    {
        d_data->m_ui->cbxNoPhases->setCurrentIndex( index );
        if ( no_phases !=  d_data->m_ui->HueSegment->noSegments() )
        {
            // reset selected phase
            d_data->m_phase = 0;
        
            // hue viewer
            d_data->m_ui->HueViewer->setNoSegments( no_phases );
            d_data->m_ui->HueViewer->onSegmentChange( 0 );
            d_data->m_ui->HueViewer->onHueChange( d_data->m_phase_hue[0] );

            // hue segment select
            d_data->m_ui->HueSegment->setNoSegments( no_phases );
            d_data->m_ui->HueSegment->setSelectedIndex( 0 );

            // hue knob-box
            d_data->m_ui->Hue->onValueChange( d_data->m_phase_hue[0] );

            // saturation knob-box
            d_data->m_ui->Saturation->onValueChange( d_data->m_phase_saturation[0] );
        }
    }
}

/******************************************************************************
 * MccBox::onMccPhaseChange
 *****************************************************************************/
void MccBox::onMccPhaseChange( int id, int saturation, int hue )
{
    if ( (id>=0) && (id < MCC_MAX_COLOR_PHASES) )
    {
        // save new values
        d_data->m_phase_hue[id]        = hue;
        d_data->m_phase_saturation[id] = saturation;

        // current phase ?
        if ( id == d_data->m_phase )
        {
            d_data->m_ui->Hue->onValueChange( hue );
            d_data->m_ui->HueViewer->onHueChange( hue );
            d_data->m_ui->Saturation->onValueChange( saturation );
        }
    }
}

/******************************************************************************
 * MccBox::onOpModeChange
 *****************************************************************************/
void MccBox::onOpModeChange( int index )
{
    int v = d_data->m_ui->cbxNoPhases->itemData( index ).toUInt();
    int mode        = MCC_OPERATION_MODE( v );
    int no_phases   = MCC_NO_PHASE( v );

    if ( no_phases !=  d_data->m_ui->HueSegment->noSegments() )
    {
        emit MccOperationModeChanged( mode, no_phases );

        // reset selected phase
        d_data->m_phase = 0;
        
        // hue viewer
        d_data->m_ui->HueViewer->setNoSegments( no_phases );
        d_data->m_ui->HueViewer->onSegmentChange( 0 );
        d_data->m_ui->HueViewer->onHueChange( d_data->m_phase_hue[0] );

        // hue segment select
        d_data->m_ui->HueSegment->setNoSegments( no_phases );
        d_data->m_ui->HueSegment->setSelectedIndex( 0 );

        // hue knob-box
        d_data->m_ui->Hue->onValueChange( d_data->m_phase_hue[0] );

        // saturation knob-box
        d_data->m_ui->Saturation->onValueChange( d_data->m_phase_saturation[0] );

        // sendi phase change singnal
        emit MccPhaseIndexChanged( 0 );
    }
}

/******************************************************************************
 * MccBox::onPhaseChange
 *****************************************************************************/
void MccBox::onPhaseChange( int id )
{
    if ( (id>=0) && (id < MCC_MAX_COLOR_PHASES) )
    {
        d_data->m_phase = id;

        // hue viewer
        d_data->m_ui->HueViewer->onSegmentChange( id );
        d_data->m_ui->HueViewer->onHueChange( d_data->m_phase_hue[id] );

        // hue knob-box
        d_data->m_ui->Hue->onValueChange( d_data->m_phase_hue[id] );

        // saturation knob-box
        d_data->m_ui->Saturation->onValueChange( d_data->m_phase_saturation[id] );

        emit MccPhaseIndexChanged( id );
    }
}

/******************************************************************************
 * MccBox::onHueChange
 *****************************************************************************/
void MccBox::onHueChange( int value )
{
    d_data->m_phase_hue[d_data->m_phase] = value;
    d_data->m_ui->HueViewer->onHueChange( value );

    emit MccPhaseChanged( d_data->m_phase, d_data->m_phase_saturation[d_data->m_phase], value );
}

/******************************************************************************
 * MccBox::onSaturationChange
 *****************************************************************************/
void MccBox::onSaturationChange( int value )
{
    d_data->m_phase_saturation[d_data->m_phase] = value;
    
    emit MccPhaseChanged( d_data->m_phase, value, d_data->m_phase_hue[d_data->m_phase] );
}

/******************************************************************************
 * MccBox::onLoadProfileClick
 *****************************************************************************/
void MccBox::onLoadProfileClick()
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
 * MccBox::onSaveProfileClick
 *****************************************************************************/
void MccBox::onSaveProfileClick()
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
 
/******************************************************************************
 * MccBox::enterEvent
 *****************************************************************************/
void MccBox::enterEvent( QEvent * )
{
    emit MccPhaseIndexChanged( d_data->m_phase );
} 

/******************************************************************************
 * MccBox::leaveEvent
 *****************************************************************************/
void MccBox::leaveEvent( QEvent * )
{
    emit MccPhaseIndexChanged( -1 );
} 


