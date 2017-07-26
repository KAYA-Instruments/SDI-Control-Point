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

