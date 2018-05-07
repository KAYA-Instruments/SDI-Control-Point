/******************************************************************************
 * Copyright (C) 2018 Dream Chip Technologies GmbH
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
 * @file    aecweightsdialog.h
 *
 * @brief   Declaration of helper dialog to setup the AEC weights
 *
 *****************************************************************************/
#ifndef AECWEIGHTSDIALOG_H
#define AECWEIGHTSDIALOG_H

#include <QDialog>

namespace Ui {
class AecWeightsDialog;
}

class AecWeightsDialog : public QDialog
{
    Q_OBJECT

signals:
    void AecWeightChanged( int index, int value );

public:
    explicit AecWeightsDialog(QWidget *parent = 0);
    ~AecWeightsDialog();

    QVector<int> getAecWeights( void );
    void setAecWeights( QVector<int> weights );

public slots:
    void onAecWeightsChange( QVector<int> weights );

private:
    Ui::AecWeightsDialog *ui;

private slots:
    void onSbxWeightChange( int value );
    void onBtnResetDefaultsClicked( void );
};

#endif // AECWEIGHTSDIALOG_H
