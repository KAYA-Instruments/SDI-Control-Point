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
 * @file    outbox.cpp
 *
 * @brief   Implementation of output setting box
 *
 *****************************************************************************/
#include <QtDebug>
#include <QFileDialog>

#include <simple_math/rgb2ycbcr.h>

#include "common.h"
#include "defines.h"
#include "outbox.h"
#include "ui_outbox.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_OutBox;
}

/******************************************************************************
 * local defines
 *****************************************************************************/
#define FIX_PRECISION_S0212             ( 4096.0f )
#define FIX_PRECISION_KRED              ( 10000.0f )
#define FIX_PRECISION_KBLUE             ( 10000.0f )

#define OUT_SETTINGS_SECTION_NAME       ( "OUT" )
#define OUT_SETTINGS_RED_COEFFICIENT    ( "red" )
#define OUT_SETTINGS_BLUE_COEFFICIENT   ( "blue" )
#define OUT_SETTINGS_SDI_BLACK          ( "sdi_black" )
#define OUT_SETTINGS_SDI_WHITE          ( "sdi_white" )
#define OUT_SETTINGS_SDI_MODE           ( "sdi_mode" )
#define OUT_SETTINGS_RAW_MODE           ( "raw_mode" )

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
 * OutBox::PrivateData
 *****************************************************************************/
class OutBox::PrivateData
{
public:
    PrivateData()
        : m_ui( new Ui::UI_OutBox )
        , m_sdiMode( SdiModeFirst )
        , m_csMode( ColorSpaceModeFirst )
    {
        // do nothing
    };

    ~PrivateData()
    {
        delete m_ui;
    };

    void setModes( SdiMode sdiMode, ColorSpaceMode csMode )
    {
        bool enableYuvCoefficients = (csMode == ColorSpaceModeYUV) ? true : false;
        bool enableBlackWhite 
            = ((sdiMode == SdiModeLegal) && (csMode == ColorSpaceModeYUV)) ? true : false;

        m_ui->Kred->setEnabled( enableYuvCoefficients );
        m_ui->Kblue->setEnabled( enableYuvCoefficients );

        m_ui->SdiWhite->setEnabled( enableBlackWhite );
        m_ui->SdiBlack->setEnabled( enableBlackWhite );

        m_sdiMode = sdiMode;
        m_csMode  = csMode;
    }

    void setCoefficients
    (
        float c0, float c1, float c2,
        float c3, float c4, float c5,
        float c6, float c7, float c8
    )
    {
        m_ui->sbxC0->blockSignals( true );
        m_ui->sbxC0->setValue( c0 );
        m_ui->sbxC0->blockSignals( false );

        m_ui->sbxC1->blockSignals( true );
        m_ui->sbxC1->setValue( c1 );
        m_ui->sbxC1->blockSignals( false );

        m_ui->sbxC2->blockSignals( true );
        m_ui->sbxC2->setValue( c2 );
        m_ui->sbxC2->blockSignals( false );

        m_ui->sbxC3->blockSignals( true );
        m_ui->sbxC3->setValue( c3 );
        m_ui->sbxC3->blockSignals( false );

        m_ui->sbxC4->blockSignals( true );
        m_ui->sbxC4->setValue( c4 );
        m_ui->sbxC4->blockSignals( false );

        m_ui->sbxC5->blockSignals( true );
        m_ui->sbxC5->setValue( c5 );
        m_ui->sbxC5->blockSignals( false );

        m_ui->sbxC6->blockSignals( true );
        m_ui->sbxC6->setValue( c6 );
        m_ui->sbxC6->blockSignals( false );

        m_ui->sbxC7->blockSignals( true );
        m_ui->sbxC7->setValue( c7 );
        m_ui->sbxC7->blockSignals( false );

        m_ui->sbxC8->blockSignals( true );
        m_ui->sbxC8->setValue( c8 );
        m_ui->sbxC8->blockSignals( false );
        
        m_ui->sbxY->blockSignals( true );
        m_ui->sbxY->setValue( c0 + c1 + c2 );
        m_ui->sbxY->blockSignals( false );
        
        m_ui->sbxCb->blockSignals( true );
        m_ui->sbxCb->setValue( c3 + c4 + c5 );
        m_ui->sbxCb->blockSignals( false );
        
        m_ui->sbxCr->blockSignals( true );
        m_ui->sbxCr->setValue( c6 + c7 + c8 );
        m_ui->sbxCr->blockSignals( false );
    }

    Ui::UI_OutBox *   m_ui;         /**< ui handle */
    SdiMode           m_sdiMode;    /**< current sdi mode */
    ColorSpaceMode    m_csMode;     /**< current color space mode */
    
    QString           m_filename;   /**< Matrix profile name */
};

/******************************************************************************
 * OutBox::OutBox
 *****************************************************************************/
OutBox::OutBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData;

    // initialize UI
    d_data->m_ui->setupUi( this );

    ///////////////////////////////////////
    // colour difference coefficient red
    ///////////////////////////////////////

    d_data->m_ui->Kred->setRange( OUT_KRED_MIN, OUT_KRED_MAX );
    d_data->m_ui->Kred->setCommaPos( OUT_KRED_COMMA_POSITION );
    d_data->m_ui->Kred->setDisplayMultiplier( OUT_KRED_DISPLAY_MULTIPLIER );
    d_data->m_ui->Kred->setBase( OUT_KRED_BASE );
    d_data->m_ui->Kred->setMaxAngle( 150 );
    d_data->m_ui->Kred->setMaxRounds( 1 );
    d_data->m_ui->Kred->setFmt( OUT_KRED_DISPLAY_MASK );

    ///////////////////////////////////////
    // colour difference coefficient blue
    ///////////////////////////////////////

    d_data->m_ui->Kblue->setRange( OUT_KBLUE_MIN, OUT_KRED_MAX );
    d_data->m_ui->Kblue->setCommaPos( OUT_KBLUE_COMMA_POSITION );
    d_data->m_ui->Kblue->setDisplayMultiplier( OUT_KBLUE_DISPLAY_MULTIPLIER );
    d_data->m_ui->Kblue->setBase( OUT_KBLUE_BASE );
    d_data->m_ui->Kblue->setMaxAngle( 150 );
    d_data->m_ui->Kblue->setMaxRounds( 1 );
    d_data->m_ui->Kblue->setFmt( OUT_KBLUE_DISPLAY_MASK );

    ///////////////////////////////////////
    // SDI black
    ///////////////////////////////////////

    d_data->m_ui->SdiBlack->setRange( OUT_SDI_BLACK_MIN, OUT_SDI_BLACK_MAX );
    d_data->m_ui->SdiBlack->setCommaPos( OUT_SDI_BLACK_COMMA_POSITION );
    d_data->m_ui->SdiBlack->setDisplayMultiplier( OUT_SDI_BLACK_DISPLAY_MULTIPLIER );
    d_data->m_ui->SdiBlack->setBase( OUT_SDI_BLACK_BASE );
    d_data->m_ui->SdiBlack->setMaxAngle( 150 );
    d_data->m_ui->SdiBlack->setMaxRounds( 1 );
    d_data->m_ui->SdiBlack->setFmt( OUT_SDI_BLACK_DISPLAY_MASK );

    ///////////////////////////////////////
    // SDI white
    ///////////////////////////////////////

    d_data->m_ui->SdiWhite->setRange( OUT_SDI_WHITE_MIN, OUT_SDI_WHITE_MAX );
    d_data->m_ui->SdiWhite->setCommaPos( OUT_SDI_WHITE_COMMA_POSITION );
    d_data->m_ui->SdiWhite->setDisplayMultiplier( OUT_SDI_WHITE_DISPLAY_MULTIPLIER );
    d_data->m_ui->SdiWhite->setBase( OUT_SDI_WHITE_BASE );
    d_data->m_ui->SdiWhite->setMaxAngle( 150 );
    d_data->m_ui->SdiWhite->setMaxRounds( 1 );
    d_data->m_ui->SdiWhite->setFmt( OUT_SDI_WHITE_DISPLAY_MASK );

    ////////////////////
    // connect signals
    ////////////////////

    // connect colour difference coefficient signals
    connect( d_data->m_ui->Kred, SIGNAL(ValueChanged(int)), this, SLOT(onKredChange(int)) );
    connect( d_data->m_ui->Kblue, SIGNAL(ValueChanged(int)), this, SLOT(onKblueChange(int)) );

    // connect black/white signals
    connect( d_data->m_ui->SdiWhite, SIGNAL(ValueChanged(int)), this, SIGNAL(SdiWhiteChanged(int)) );
    connect( d_data->m_ui->SdiBlack, SIGNAL(ValueChanged(int)), this, SIGNAL(SdiBlackChanged(int)) );

    // connect preset signals
    connect( d_data->m_ui->btnRec709   , SIGNAL(clicked()), this, SLOT(onBt709Clicked()));
    connect( d_data->m_ui->btnBt601    , SIGNAL(clicked()), this, SLOT(onBt601Clicked()));
    connect( d_data->m_ui->btnSmpte240M, SIGNAL(clicked()), this, SLOT(onSmpte240MClicked()));
    
    connect( d_data->m_ui->btnLoad     , SIGNAL(clicked()), this, SLOT(onLoadClicked()));
    connect( d_data->m_ui->btnSave     , SIGNAL(clicked()), this, SLOT(onSaveClicked()));

    // connect mode signals
    connect( d_data->m_ui->cbxSdiMode, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxSdiModeChange(int)) );
    connect( d_data->m_ui->cbxCsMode , SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxCsModeChange(int)) );
}

/******************************************************************************
 * OutBox::~OutBox
 *****************************************************************************/
OutBox::~OutBox()
{
    delete d_data;
}

/******************************************************************************
 * OutBox::prepareMode
 *****************************************************************************/
void OutBox::prepareMode( const Mode )
{
    // do nothing here
}

/******************************************************************************
 * OutBox::RedCoefficient
 *****************************************************************************/
int OutBox::RedCoefficient() const
{
    return ( d_data->m_ui->Kred->value() );
}

/******************************************************************************
 * OutBox::setRedCoefficient
 *****************************************************************************/
void OutBox::setRedCoefficient( const int c )
{
    d_data->m_ui->Kred->setValue( c );
    onKredChange( c );
}

/******************************************************************************
 * OutBox::BlueCoefficient
 *****************************************************************************/
int OutBox::BlueCoefficient() const
{
    return ( d_data->m_ui->Kblue->value() );
}

/******************************************************************************
 * OutBox::setBlueCoefficient
 *****************************************************************************/
void OutBox::setBlueCoefficient( const int c )
{
    d_data->m_ui->Kblue->setValue( c );
    onKblueChange( c );
}

/******************************************************************************
 * OutBox::SdiBlack
 *****************************************************************************/
int OutBox::SdiBlack() const
{
    return ( d_data->m_ui->SdiBlack->value() );
}

/******************************************************************************
 * OutBox::setSdiBlack
 *****************************************************************************/
void OutBox::setSdiBlack( const int value )
{
    d_data->m_ui->SdiBlack->setValue( value );
    emit SdiBlackChanged( value );
}

/******************************************************************************
 * OutBox::SdiWhite
 *****************************************************************************/
int OutBox::SdiWhite() const
{
    return ( d_data->m_ui->SdiWhite->value() );
}

/******************************************************************************
 * OutBox::setSdiWhite
 *****************************************************************************/
void OutBox::setSdiWhite( const int value )
{
    d_data->m_ui->SdiWhite->setValue( value );
    emit SdiWhiteChanged( value );
}

/******************************************************************************
 * OutBox::getSdiMode
 *****************************************************************************/
int OutBox::getSdiMode() const
{
    return ( d_data->m_ui->cbxSdiMode->currentIndex() );
}

/******************************************************************************
 * OutBox::setSdiMode
 *****************************************************************************/
void OutBox::setSdiMode( const int value )
{
    d_data->m_ui->cbxSdiMode->setCurrentIndex( value );
    emit SdiModeChanged( value );
}

/******************************************************************************
 * OutBox::getRawMode
 *****************************************************************************/
int OutBox::getRawMode() const
{
    return ( d_data->m_ui->cbxCsMode->currentIndex() );
}

/******************************************************************************
 * OutBox::setRawMode
 *****************************************************************************/
void OutBox::setRawMode( const int value )
{
    d_data->m_ui->cbxCsMode->setCurrentIndex( value );
    emit RawModeChanged( value );
}

/******************************************************************************
 * OutBox::setSdiWhite
 *****************************************************************************/
void OutBox::changeSdiStringToHdmi(const bool value)
{
    if ( value )
    {
        d_data->m_ui->SdiBlack->setTitle("HDMI Black");
        d_data->m_ui->SdiWhite->setTitle("HDMI White");
        d_data->m_ui->gbxSdiOutput->setTitle("HDMI Output");
    }
    else
    {
        d_data->m_ui->SdiBlack->setTitle("SDI Black");
        d_data->m_ui->SdiWhite->setTitle("SDI White");
        d_data->m_ui->gbxSdiOutput->setTitle("SDI Output");
    }
}

/******************************************************************************
 * OutBox::setRawModeVisible
 *****************************************************************************/
void OutBox::setRawModeVisible(const bool value)
{
    d_data->m_ui->cbxCsMode->setVisible( value );
}

/******************************************************************************
 * OutBox::loadSettings
 *****************************************************************************/
void OutBox::loadSettings( QSettings & s )
{
    s.beginGroup( OUT_SETTINGS_SECTION_NAME );

    setRedCoefficient( s.value( OUT_SETTINGS_RED_COEFFICIENT).toInt() );
    setBlueCoefficient( s.value( OUT_SETTINGS_BLUE_COEFFICIENT).toInt() );
    setSdiBlack(s.value( OUT_SETTINGS_SDI_BLACK ).toInt() );
    setSdiWhite(s.value( OUT_SETTINGS_SDI_WHITE ).toInt() );
    setSdiMode(s.value( OUT_SETTINGS_SDI_MODE ).toInt() );
    setRawMode(s.value( OUT_SETTINGS_RAW_MODE ).toInt() );

    s.endGroup();
}

/******************************************************************************
 * OutBox::saveSettings
 *****************************************************************************/
void OutBox::saveSettings( QSettings & s )
{
    s.beginGroup( OUT_SETTINGS_SECTION_NAME );

    s.setValue( OUT_SETTINGS_RED_COEFFICIENT , RedCoefficient() );
    s.setValue( OUT_SETTINGS_BLUE_COEFFICIENT, BlueCoefficient() );
    s.setValue( OUT_SETTINGS_SDI_BLACK       , SdiBlack() );
    s.setValue( OUT_SETTINGS_SDI_WHITE       , SdiWhite() );
    s.setValue( OUT_SETTINGS_SDI_MODE        , getSdiMode() );
    s.setValue( OUT_SETTINGS_RAW_MODE        , getRawMode() );

    s.endGroup();
}

/******************************************************************************
 * OutBox::applySettings
 *****************************************************************************/
void OutBox::applySettings( void )
{
    // Calculate matrix coefficients from red and blue coefficients
    float kred  = ((float)(RedCoefficient())) / FIX_PRECISION_KRED;
    float kblue = ((float)(BlueCoefficient())) / FIX_PRECISION_KBLUE;

    float c0, c1, c2;
    float c3, c4, c5;
    float c6, c7, c8;

    int c0_, c1_, c2_;
    int c3_, c4_, c5_;
    int c6_, c7_, c8_;

    int res = cal_YCbCr_coefficients( kred, kblue, &c0, &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8 );
    HANDLE_ERROR( res );

    d_data->setCoefficients( c0, c1, c2, c3, c4, c5, c6, c7, c8 );

    c0_ = (int)(c0 * FIX_PRECISION_S0212 + 0.5f);
    c1_ = (int)(c1 * FIX_PRECISION_S0212 + 0.5f);
    c2_ = (int)(c2 * FIX_PRECISION_S0212 + 0.5f);
    c3_ = (int)(c3 * FIX_PRECISION_S0212 + 0.5f);
    c4_ = (int)(c4 * FIX_PRECISION_S0212 + 0.5f);
    c5_ = (int)(c5 * FIX_PRECISION_S0212 + 0.5f);
    c6_ = (int)(c6 * FIX_PRECISION_S0212 + 0.5f);
    c7_ = (int)(c7 * FIX_PRECISION_S0212 + 0.5f);
    c8_ = (int)(c8 * FIX_PRECISION_S0212 + 0.5f);

    emit ColorConversionMatrixChanged( c0_, c1_, c2_, c3_, c4_, c5_, c6_, c7_, c8_ );

    emit SdiBlackChanged( SdiBlack() );
    emit SdiWhiteChanged( SdiWhite() );
    emit SdiModeChanged( getSdiMode() );
    emit RawModeChanged( getRawMode() );
}

/******************************************************************************
 * OutBox::addCamDepth
 *****************************************************************************/
void OutBox::addSdiMode( QString name, int id )
{
    d_data->m_ui->cbxSdiMode->blockSignals( true );
    d_data->m_ui->cbxSdiMode->addItem( name, id );
    d_data->m_ui->cbxSdiMode->blockSignals( false );
}

/******************************************************************************
 * OutBox::addColorSpaceMode
 *****************************************************************************/
void OutBox::addColorSpaceMode( QString name, int id )
{
    d_data->m_ui->cbxCsMode->blockSignals( true );
    d_data->m_ui->cbxCsMode->addItem( name, id );
    d_data->m_ui->cbxCsMode->blockSignals( false );
}

/******************************************************************************
 * OutBox::onColorConversionMatrixChange
 *****************************************************************************/
void OutBox::onColorConversionMatrixChange
(
    int c0, int c1, int c2,
    int c3, int c4, int c5,
    int c6, int c7, int c8
)
{
    float c0_ = ((float)c0) / FIX_PRECISION_S0212;
    float c1_ = ((float)c1) / FIX_PRECISION_S0212;
    float c2_ = ((float)c2) / FIX_PRECISION_S0212;
    float c3_ = ((float)c3) / FIX_PRECISION_S0212;
    float c4_ = ((float)c4) / FIX_PRECISION_S0212;
    float c5_ = ((float)c5) / FIX_PRECISION_S0212;
    float c6_ = ((float)c6) / FIX_PRECISION_S0212;
    float c7_ = ((float)c7) / FIX_PRECISION_S0212;
    float c8_ = ((float)c8) / FIX_PRECISION_S0212;
    
    d_data->m_ui->Kred->setValue( (int)(c0_ * FIX_PRECISION_KRED) );
    d_data->m_ui->Kblue->setValue( (int)(c2_ * FIX_PRECISION_KBLUE) );

    d_data->setCoefficients( c0_, c1_, c2_, c3_, c4_, c5_, c6_, c7_, c8_ );
}

/******************************************************************************
 * OutBox::onSdiModeChange
 *****************************************************************************/
void OutBox::onSdiModeChange( int value )
{
    int index = d_data->m_ui->cbxSdiMode->findData( value );
    if ( index != -1 )
    {
        d_data->m_ui->cbxSdiMode->blockSignals( true );
        d_data->m_ui->cbxSdiMode->setCurrentIndex( index );
        d_data->m_ui->cbxSdiMode->blockSignals( false );
        
        d_data->setModes( (SdiMode)value, d_data->m_csMode );
    }    
}

/******************************************************************************
 * OutBox::onSdiBlackChange
 *****************************************************************************/
void OutBox::onSdiBlackChange( int value )
{
    d_data->m_ui->SdiBlack->blockSignals( true );
    d_data->m_ui->SdiBlack->setValue( value );
    d_data->m_ui->SdiBlack->blockSignals( false );
}

/******************************************************************************
 * OutBox::onSdiWhiteChange
 *****************************************************************************/
void OutBox::onSdiWhiteChange( int value )
{
    d_data->m_ui->SdiWhite->blockSignals( true );
    d_data->m_ui->SdiWhite->setValue( value );
    d_data->m_ui->SdiWhite->blockSignals( false );
}

/******************************************************************************
 * OutBox::onRawModeChange
 *****************************************************************************/
void OutBox::onRawModeChange( int value )
{
    int index = d_data->m_ui->cbxCsMode->findData( value );
    if ( index != -1 )
    {
        d_data->m_ui->cbxCsMode->blockSignals( true );
        d_data->m_ui->cbxCsMode->setCurrentIndex( index );
        d_data->m_ui->cbxCsMode->blockSignals( false );
    
        d_data->setModes( d_data->m_sdiMode, (ColorSpaceMode)value );
    }    
}

/******************************************************************************
 * OutBox::onKredChange
 *****************************************************************************/
void OutBox::onKredChange( int value )
{
    float kred  = ((float)(value)) / FIX_PRECISION_KRED;
    float kblue = ((float)(d_data->m_ui->Kblue->value())) / FIX_PRECISION_KBLUE;

    float c0, c1, c2;
    float c3, c4, c5;
    float c6, c7, c8;
 
    int c0_, c1_, c2_;
    int c3_, c4_, c5_;
    int c6_, c7_, c8_;

    int res = cal_YCbCr_coefficients( kred, kblue, &c0, &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8 );
    HANDLE_ERROR( res );

    d_data->setCoefficients( c0, c1, c2, c3, c4, c5, c6, c7, c8 );

    c0_ = (int)(c0 * FIX_PRECISION_S0212 + 0.5f);
    c1_ = (int)(c1 * FIX_PRECISION_S0212 + 0.5f);
    c2_ = (int)(c2 * FIX_PRECISION_S0212 + 0.5f);
    c3_ = (int)(c3 * FIX_PRECISION_S0212 + 0.5f);
    c4_ = (int)(c4 * FIX_PRECISION_S0212 + 0.5f);
    c5_ = (int)(c5 * FIX_PRECISION_S0212 + 0.5f);
    c6_ = (int)(c6 * FIX_PRECISION_S0212 + 0.5f);
    c7_ = (int)(c7 * FIX_PRECISION_S0212 + 0.5f);
    c8_ = (int)(c8 * FIX_PRECISION_S0212 + 0.5f);

    emit ColorConversionMatrixChanged( c0_, c1_, c2_, c3_, c4_, c5_, c6_, c7_, c8_ );
}

/******************************************************************************
 * OutBox::onKblueChange
 *****************************************************************************/
void OutBox::onKblueChange( int value )
{
    float kred  = ((float)(d_data->m_ui->Kred->value())) / FIX_PRECISION_KRED;
    float kblue = ((float)(value)) / FIX_PRECISION_KBLUE;
    
    float c0, c1, c2;
    float c3, c4, c5;
    float c6, c7, c8;
    
    int c0_, c1_, c2_;
    int c3_, c4_, c5_;
    int c6_, c7_, c8_;

    int res = cal_YCbCr_coefficients( kred, kblue, &c0, &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8 );
    HANDLE_ERROR( res );
    
    d_data->setCoefficients( c0, c1, c2, c3, c4, c5, c6, c7, c8 );

    c0_ = (int)(c0 * FIX_PRECISION_S0212 + 0.5f);
    c1_ = (int)(c1 * FIX_PRECISION_S0212 + 0.5f);
    c2_ = (int)(c2 * FIX_PRECISION_S0212 + 0.5f);
    c3_ = (int)(c3 * FIX_PRECISION_S0212 + 0.5f);
    c4_ = (int)(c4 * FIX_PRECISION_S0212 + 0.5f);
    c5_ = (int)(c5 * FIX_PRECISION_S0212 + 0.5f);
    c6_ = (int)(c6 * FIX_PRECISION_S0212 + 0.5f);
    c7_ = (int)(c7 * FIX_PRECISION_S0212 + 0.5f);
    c8_ = (int)(c8 * FIX_PRECISION_S0212 + 0.5f);

    emit ColorConversionMatrixChanged( c0_, c1_, c2_, c3_, c4_, c5_, c6_, c7_, c8_ );
}

/******************************************************************************
 * OutBox::onBt709Clicked
 *****************************************************************************/
void OutBox::onBt709Clicked()
{
    float c0, c1, c2;
    float c3, c4, c5;
    float c6, c7, c8;
    
    int c0_, c1_, c2_;
    int c3_, c4_, c5_;
    int c6_, c7_, c8_;

    int res = cal_YCbCr_coefficients_bt709( &c0, &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8 );
    HANDLE_ERROR( res );

    d_data->m_ui->Kred->setValue( (int)(Kr_BT709 * FIX_PRECISION_KRED) );
    d_data->m_ui->Kblue->setValue( (int)(Kb_BT709 * FIX_PRECISION_KBLUE) );
    d_data->setCoefficients( c0, c1, c2, c3, c4, c5, c6, c7, c8 );
 
    c0_ = (int)(c0 * FIX_PRECISION_S0212 + 0.5f);
    c1_ = (int)(c1 * FIX_PRECISION_S0212 + 0.5f);
    c2_ = (int)(c2 * FIX_PRECISION_S0212 + 0.5f);
    c3_ = (int)(c3 * FIX_PRECISION_S0212 + 0.5f);
    c4_ = (int)(c4 * FIX_PRECISION_S0212 + 0.5f);
    c5_ = (int)(c5 * FIX_PRECISION_S0212 + 0.5f);
    c6_ = (int)(c6 * FIX_PRECISION_S0212 + 0.5f);
    c7_ = (int)(c7 * FIX_PRECISION_S0212 + 0.5f);
    c8_ = (int)(c8 * FIX_PRECISION_S0212 + 0.5f);

    emit ColorConversionMatrixChanged( c0_, c1_, c2_, c3_, c4_, c5_, c6_, c7_, c8_ );
}

/******************************************************************************
 * OutBox::onBt601Clicked
 *****************************************************************************/
void OutBox::onBt601Clicked()
{
    float c0, c1, c2;
    float c3, c4, c5;
    float c6, c7, c8;
    
    int c0_, c1_, c2_;
    int c3_, c4_, c5_;
    int c6_, c7_, c8_;

    int res = cal_YCbCr_coefficients_bt601( &c0, &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8 );
    HANDLE_ERROR( res );
    
    d_data->m_ui->Kred->setValue( (int)(Kr_BT601 * FIX_PRECISION_KRED) );
    d_data->m_ui->Kblue->setValue( (int)(Kb_BT601 * FIX_PRECISION_KBLUE) );
    d_data->setCoefficients( c0, c1, c2, c3, c4, c5, c6, c7, c8 );

    c0_ = (int)(c0 * FIX_PRECISION_S0212 + 0.5f);
    c1_ = (int)(c1 * FIX_PRECISION_S0212 + 0.5f);
    c2_ = (int)(c2 * FIX_PRECISION_S0212 + 0.5f);
    c3_ = (int)(c3 * FIX_PRECISION_S0212 + 0.5f);
    c4_ = (int)(c4 * FIX_PRECISION_S0212 + 0.5f);
    c5_ = (int)(c5 * FIX_PRECISION_S0212 + 0.5f);
    c6_ = (int)(c6 * FIX_PRECISION_S0212 + 0.5f);
    c7_ = (int)(c7 * FIX_PRECISION_S0212 + 0.5f);
    c8_ = (int)(c8 * FIX_PRECISION_S0212 + 0.5f);
    
    emit ColorConversionMatrixChanged( c0_, c1_, c2_, c3_, c4_, c5_, c6_, c7_, c8_ );
}

/******************************************************************************
 * OutBox::onSmpte240MClicked
 *****************************************************************************/
void OutBox::onSmpte240MClicked()
{
    float c0, c1, c2;
    float c3, c4, c5;
    float c6, c7, c8;
    
    int c0_, c1_, c2_;
    int c3_, c4_, c5_;
    int c6_, c7_, c8_;

    int res = cal_YCbCr_coefficients_smpte240M( &c0, &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8 );
    HANDLE_ERROR( res );
    
    d_data->m_ui->Kred->setValue( (int)(Kr_SMPTE240M * FIX_PRECISION_KRED) );
    d_data->m_ui->Kblue->setValue( (int)(Kb_SMPTE240M * FIX_PRECISION_KBLUE) );
    d_data->setCoefficients( c0, c1, c2, c3, c4, c5, c6, c7, c8 );

    c0_ = (int)(c0 * FIX_PRECISION_S0212 + 0.5f);
    c1_ = (int)(c1 * FIX_PRECISION_S0212 + 0.5f);
    c2_ = (int)(c2 * FIX_PRECISION_S0212 + 0.5f);
    c3_ = (int)(c3 * FIX_PRECISION_S0212 + 0.5f);
    c4_ = (int)(c4 * FIX_PRECISION_S0212 + 0.5f);
    c5_ = (int)(c5 * FIX_PRECISION_S0212 + 0.5f);
    c6_ = (int)(c6 * FIX_PRECISION_S0212 + 0.5f);
    c7_ = (int)(c7 * FIX_PRECISION_S0212 + 0.5f);
    c8_ = (int)(c8 * FIX_PRECISION_S0212 + 0.5f);
    
    emit ColorConversionMatrixChanged( c0_, c1_, c2_, c3_, c4_, c5_, c6_, c7_, c8_ );
}

/******************************************************************************
 * OutBox::onLoadClicked
 *****************************************************************************/
void OutBox::onLoadClicked()
{
    QString directory = QDir::currentPath();

    // NOTE: It can fail on gtk-systems when an empty filename is given
    //       in the native dialog-box, because GTK sends a SIGSEGV-signal
    //       to process and this is not handled by Qt.
    QFileDialog dialog( this );
    dialog.setDefaultSuffix( "yuv" );
    d_data->m_filename = dialog.getOpenFileName(
        this, tr("Load Matrix Profile"), 
        directory,
        "Select Profile Files (*.yuv);;All files (*.*)"
    );

    if ( NULL != d_data->m_filename )
    {
        QFileInfo file( d_data->m_filename );
        if ( file.suffix().isEmpty() )
        {
            d_data->m_filename += ".yuv";
        }

        if ( fileExists( d_data->m_filename ) )
        {
            QSettings settings( d_data->m_filename, QSettings::IniFormat );
            loadSettings( settings );
        }
    }
}

/******************************************************************************
 * OutBox::onSaveClicked
 *****************************************************************************/
void OutBox::onSaveClicked()
{
    QString directory = QDir::currentPath();

    // NOTE: It can fail on gtk-systems when an empty filename is given
    //       in the native dialog-box, because GTK sends a SIGSEGV-signal
    //       to process and this is not handled by Qt.
    QFileDialog dialog( this );
    dialog.setDefaultSuffix( "yuv" );
    d_data->m_filename = dialog.getSaveFileName(
        this, tr("Save Matrix Profile"), 
        directory,
        "Select Profile Files (*.yuv);;All files (*.*)"
    );

    if ( NULL != d_data->m_filename )
    {
        QFileInfo file( d_data->m_filename );
        if ( file.suffix().isEmpty() )
        {
            d_data->m_filename += ".yuv";
        }

        QSettings settings( d_data->m_filename, QSettings::IniFormat );
        saveSettings( settings );
    }
}

/******************************************************************************
 * OutBox::onCbxSdiModeChange
 *****************************************************************************/
void OutBox::onCbxSdiModeChange( int index )
{
    int value = d_data->m_ui->cbxSdiMode->itemData( index ).toInt();

    d_data->setModes( (SdiMode)value, d_data->m_csMode );

    emit SdiModeChanged( value );
}

/******************************************************************************
 * OutBox::onCbxCsModeChange
 *****************************************************************************/
void OutBox::onCbxCsModeChange( int index )
{
    int value = d_data->m_ui->cbxCsMode->itemData( index ).toInt();

    d_data->setModes( d_data->m_sdiMode, (ColorSpaceMode)value );

    emit RawModeChanged( value );
}

