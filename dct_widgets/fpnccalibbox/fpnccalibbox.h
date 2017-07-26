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
 * @file    inoutbox.h
 *
 * @brief   Class definition of input/output box
 *
 *****************************************************************************/
#ifndef __FPNC_CALIBBOX_H__
#define __FPNC_CALIBBOX_H__

#include <dct_widgets_base.h>

#include <QItemDelegate>

class FpncDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit FpncDelegate( QObject * parent = 0 );

    QWidget * createEditor( QWidget *parent, const QStyleOptionViewItem &, const QModelIndex & ) const;
    void setEditorData( QWidget *, const QModelIndex & ) const;
    void setModelData( QWidget *, QAbstractItemModel *, const QModelIndex & ) const;
    void updateEditorGeometry( QWidget *, const QStyleOptionViewItem &, const QModelIndex & ) const;

signals:

public slots:

};

/******************************************************************************
 * Fix Pattern Noise Calibration Box Widget
 *****************************************************************************/
class FpncCalibBox : public DctWidgetBox
{
    Q_OBJECT

public:
    explicit FpncCalibBox(QWidget *parent = 0);
    ~FpncCalibBox();

protected:
    void prepareMode( const Mode mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __FPNC_CALIBBOX_H__
