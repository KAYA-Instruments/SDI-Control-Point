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
 * @file    fpnccalibbox.cpp
 *
 * @brief   Implementation of FPN calibration widget
 *
 *****************************************************************************/
#include <QtDebug>
#include <QSpinBox>
#include <QModelIndex>
#include <QStandardItemModel>

#include "fpnccalibbox.h"
#include "ui_fpnccalibbox.h"

/******************************************************************************
 * namespaces
 *****************************************************************************/
namespace Ui {
    class UI_FpncCalibBox;
}

/******************************************************************************
 * local defines
 *****************************************************************************/
#define NO_SAMPLE_POINTS        ( 21 )
#define NO_SAMPLE_POINT_VALUES  ( 2 )

/******************************************************************************
 * FpncDelegate::Delegate
 *****************************************************************************/
FpncDelegate::FpncDelegate( QObject * parent )
    : QItemDelegate( parent )
{

}

/******************************************************************************
 * FpncDelegate::createEditor
 *****************************************************************************/
QWidget * FpncDelegate::createEditor
(
    QWidget *                       parent,
    const QStyleOptionViewItem &    option,
    const QModelIndex &             index
) const
{
    (void) option;
    (void) index;

    QSpinBox * editor = new QSpinBox( parent ); 
    editor->setRange( 0, 100 );
    return ( editor );
}

/******************************************************************************
 * FpncDelegate::setEditorData
 *****************************************************************************/
void FpncDelegate::setEditorData
(
    QWidget *           editor,
    const QModelIndex & index
) const
{
    int value = index.model()->data( index, Qt::EditRole ).toInt();

    QSpinBox * spinbox = static_cast<QSpinBox *>(editor);
    spinbox->setValue ( value );
}

/******************************************************************************
 * FpncDelegate::setModelData
 *****************************************************************************/
void FpncDelegate::setModelData
(
    QWidget *               editor,
    QAbstractItemModel *    model,
    const QModelIndex &index
) const
{
    QSpinBox * spinbox = static_cast<QSpinBox *>(editor);

    spinbox->interpretText();
    int value = spinbox->value();
    model->setData( index, value, Qt::EditRole );
}

/******************************************************************************
 * FpncDelegate::updateEditorGeometry
 *****************************************************************************/
void FpncDelegate::updateEditorGeometry
(
    QWidget *                       editor,
    const QStyleOptionViewItem &    option,
    const QModelIndex &             index
) const
{
    (void) index;

    editor->setGeometry( option.rect );
}

/******************************************************************************
 * FpncCalibBox::PrivateData
 *****************************************************************************/
class FpncCalibBox::PrivateData
{
public:
    PrivateData()
        : m_ui( new Ui::UI_FpncCalibBox )
    {
        // do nothing
    };

    ~PrivateData()
    {
        delete m_ui;
    };

    Ui::UI_FpncCalibBox *  m_ui;        /**< ui handle */
    QStandardItemModel *   m_model;     /**< data model */
    FpncDelegate *         m_delegate;  /**< delegation class */
};


/******************************************************************************
 * FpncCalibBox::FpncCalibBox
 *****************************************************************************/
FpncCalibBox::FpncCalibBox( QWidget *parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData();

    d_data->m_ui->setupUi( this );

    d_data->m_delegate = new FpncDelegate( this );
    d_data->m_model = new QStandardItemModel( NO_SAMPLE_POINTS, NO_SAMPLE_POINT_VALUES, this );

    for (int row=0; row < NO_SAMPLE_POINTS; ++row )
    {
        for (int col=0; col < NO_SAMPLE_POINT_VALUES; ++col )
        {
            QModelIndex index = d_data->m_model->index( row, col, QModelIndex() );
            d_data->m_model->setData( index, 0 );
        }
    }

    d_data->m_ui->tblSamplePoints->setModel( d_data->m_model );
    d_data->m_ui->tblSamplePoints->setItemDelegate( d_data->m_delegate );
}

/******************************************************************************
 * FpncCalibBox::~FpncCalibBox
 *****************************************************************************/
FpncCalibBox::~FpncCalibBox()
{
    delete d_data;
}

/******************************************************************************
 * FpncCalibBox::prepareMode
 *****************************************************************************/
void FpncCalibBox::prepareMode( const Mode )
{
    // do nothing here
}

/******************************************************************************
 * FpncCalibBox::loadSettings
 *****************************************************************************/
void FpncCalibBox::loadSettings( QSettings & )
{
    // do nothing here
}

/******************************************************************************
 * FpncCalibBox::saveSettings
 *****************************************************************************/
void FpncCalibBox::saveSettings( QSettings & )
{
    // do nothing here
}


