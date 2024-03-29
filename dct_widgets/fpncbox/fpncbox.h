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
 * @file    fpncbox.h
 *
 * @brief   Class definition of a fix pattern noise correction configuration box 
 *
 *****************************************************************************/
#ifndef __FPNC_BOX_H__
#define __FPNC_BOX_H__

#include <dct_widgets_base.h>
#include <qcustomplot.h>

/******************************************************************************
 * Fix Pattern Noise Correction Box Widget
 *****************************************************************************/
class FpncBox : public DctWidgetBox
{
    Q_OBJECT

public:
    explicit FpncBox( QWidget * parent = 0 );
    ~FpncBox();

    QString Filename0() const;
    void setFilename0( QString & fileName );

    QString Filename1() const;
    void setFilename1( QString & fileName );

    int InverseRedGain() const;
    int InverseGreenGain() const;
    int InverseBlueGain() const;
    void setInverseGains( const int red, const int green, const int blue );

    int RedGain() const;
    int GreenGain() const;
    int BlueGain() const;
    void setGains( const int red, const int green, const int blue );

protected:
    void prepareMode( const Mode mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

signals:

    void FpncEnableChanged( int flag );
    void FpncInverseGainsChanged( int a, int b, int c, int d );
    void FpncGainsChanged( int a, int b, int c, int d );
    void FpncColumnChanged( int column );
    void FpncColumnCalibrationDataChanged( const bool evenLines, const int column, QVector<int> & data );

public slots:
    void onBayerPatternChange( int value );

    void onFpncEnableChange( int value );
    void onFpncInverseGainsChange( int a, int b, int c, int d );
    void onFpncGainsChange( int a, int b, int c, int d );
    void onFpncCorectionDataChange( int column, QVector<int> data0, QVector<int> data1 );

private slots:
    void onFpncXRangeChange( const QCPRange &range );
    void onFpncYRangeChange( const QCPRange &range );
    
    void onCorrectionData0Click();
    void onCorrectionData1Click();

    void onLoad1Click();
    void onLoad2Click();
    
    void onInverseGainsChange( int v0, int v1, int v2 );
    void onGainsChange( int v0, int v1, int v2 );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __FPNC_BOX_H__

