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
 * @file    outbox.h
 *
 * @brief   Class definition of input/output box 
 *
 *****************************************************************************/
#ifndef __OUT_BOX_H__
#define __OUT_BOX_H__

#include <dct_widgets_base.h>

/******************************************************************************
 * Colour Difference Coefficient Red Definitions
 *****************************************************************************/
#define OUT_KRED_MIN                         (        0 )
#define OUT_KRED_MAX                         (     5000 )
#define OUT_KRED_COMMA_POSITION              (        4 )
#define OUT_KRED_BASE                        (       10 )
#define OUT_KRED_DISPLAY_MULTIPLIER          (        1 )
#define OUT_KRED_DISPLAY_MASK                (  "%1.4f" )

/******************************************************************************
 * Colour Difference Coefficient Blue Definitions
 *****************************************************************************/
#define OUT_KBLUE_MIN                        (        0 )
#define OUT_KBLUE_MAX                        (     5000 )
#define OUT_KBLUE_COMMA_POSITION             (        4 )
#define OUT_KBLUE_BASE                       (       10 )
#define OUT_KBLUE_DISPLAY_MULTIPLIER         (        1 )
#define OUT_KBLUE_DISPLAY_MASK               (  "%1.4f" )

/******************************************************************************
 * Sdi-Black Definitions
 *****************************************************************************/
#define OUT_SDI_BLACK_MIN                    (      -56 )
#define OUT_SDI_BLACK_MAX                    (       56 )
#define OUT_SDI_BLACK_COMMA_POSITION         (        0 )
#define OUT_SDI_BLACK_BASE                   (       10 )
#define OUT_SDI_BLACK_DISPLAY_MULTIPLIER     (        1 )
#define OUT_SDI_BLACK_DISPLAY_MASK           ( "%-2.0f" )

/******************************************************************************
 * Sdi-White Definitions
 *****************************************************************************/
#define OUT_SDI_WHITE_MIN                    (      -80 )
#define OUT_SDI_WHITE_MAX                    (       79 )
#define OUT_SDI_WHITE_COMMA_POSITION         (        0 )
#define OUT_SDI_WHITE_BASE                   (       10 )
#define OUT_SDI_WHITE_DISPLAY_MULTIPLIER     (        1 )
#define OUT_SDI_WHITE_DISPLAY_MASK           ( "%-2.0f" )

/******************************************************************************
 * Outpu (YUV) Configuration Box Widget
 *****************************************************************************/
class OutBox : public DctWidgetBox
{
    Q_OBJECT

public:
    explicit OutBox( QWidget * parent = 0 );
    ~OutBox();

    void addSdiMode( QString name, int id );
    void addColorSpaceMode( QString name, int id );

    int RedCoefficient() const;
    void setRedCoefficient( const int c );
    int BlueCoefficient() const;
    void setBlueCoefficient( const int c );

    int SdiBlack() const;
    void setSdiBlack( const int value );
    int SdiWhite() const;
    void setSdiWhite( const int value );

    int getSdiMode() const;
    void setSdiMode( const int value );
    int getRawMode() const;
    void setRawMode( const int value );

    void changeSdiStringToHdmi(const bool value);
    void setRawModeVisible(const bool value);

protected:
    void prepareMode( const Mode mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

signals:
    void ColorConversionMatrixChanged( int c0, int c1, int c2,
                                       int c3, int c4, int c5,
                                       int c6, int c7, int c8 );
    void SdiModeChanged( int mode );
    void SdiBlackChanged( int value );
    void SdiWhiteChanged( int value );
    
    void RawModeChanged( int value );

public slots:
    void onSdiOutChange( int );
    void onColorConversionMatrixChange( int c0, int c1, int c2,
                                        int c3, int c4, int c5,
                                        int c6, int c7, int c8 );
    void onSdiModeChange( int mode );
    void onSdiBlackChange( int value );
    void onSdiWhiteChange( int value );
    
    void onRawModeChange( int mode );

private slots:
    void onKredChange( int value );
    void onKblueChange( int value );

    void onBt709Clicked();
    void onBt2020Clicked();
    void onLoadClicked();
    void onSaveClicked();

    void onCbxSdiModeChange( int value );
    void onCbxCsModeChange( int value );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __OUT_BOX_H__

