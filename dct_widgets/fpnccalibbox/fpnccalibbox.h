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
