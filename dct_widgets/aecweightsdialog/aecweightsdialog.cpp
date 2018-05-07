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
 * @file    aecweightsdialog.cpp
 *
 * @brief   Implementation of helper dialog to setup the AEC weights
 *
 *****************************************************************************/
#include <QPushButton>

#include "aecweightsdialog.h"
#include "ui_aecweightsdialog.h"

/******************************************************************************
 * AecWeightsDialog::AecWeightsDialog
 *****************************************************************************/
AecWeightsDialog::AecWeightsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AecWeightsDialog)
{
    ui->setupUi(this);

    // Connect Spin Boxes to the weight change event
    connect( ui->sbxWeight1,  SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight2,  SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight3,  SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight4,  SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight5,  SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight6,  SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight7,  SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight8,  SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight9,  SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight10, SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight11, SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight12, SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight13, SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight14, SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight15, SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight16, SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight17, SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight18, SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight19, SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight20, SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight21, SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight22, SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight23, SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight24, SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );
    connect( ui->sbxWeight25, SIGNAL(valueChanged(int)), this, SLOT(onSbxWeightChange(int)) );

    // Connect reset button
    QPushButton* resetButton = ui->buttonBox->button(QDialogButtonBox::RestoreDefaults);
    connect( resetButton, SIGNAL(clicked()), this, SLOT(onBtnResetDefaultsClicked()) );
}

/******************************************************************************
 * AecWeightsDialog::~AecWeightsDialog
 *****************************************************************************/
AecWeightsDialog::~AecWeightsDialog()
{
    delete ui;
}

/******************************************************************************
 * AecWeightsDialog::getAecWeights
 *****************************************************************************/
QVector<int> AecWeightsDialog::getAecWeights( void )
{
    QVector<int> weights;

    // Loop over all spin boxes
    for ( int i = 0; i < 25; i++ )
    {
        // Find the spin box for this index
        QString spinBoxName = QString( "sbxWeight%1" ).arg( i+1 );
        QSpinBox * spinBox = ui->gbxWeights->findChild<QSpinBox *>( spinBoxName );

        // Get value from spin box and store in vector
        weights.append( spinBox->value() );
    }

    return weights;
}

/******************************************************************************
 * AecWeightsDialog::setAecWeights
 *****************************************************************************/
void AecWeightsDialog::setAecWeights( QVector<int> weights )
{
    // Loop over all spin boxes
    for ( int i = 0; i < 25; i++ )
    {
        // Find the spin box for this index
        QString spinBoxName = QString( "sbxWeight%1" ).arg( i+1 );
        QSpinBox * spinBox = ui->gbxWeights->findChild<QSpinBox *>( spinBoxName );

        // Set value in spin box and emit a change event
        spinBox->blockSignals( true );
        spinBox->setValue( weights.at(i) );
        spinBox->blockSignals( false );

        // Signal is manually emitted to emit it even if the spinbox value does not change
        emit AecWeightChanged( i+1, weights.at(i) );
    }
}

/******************************************************************************
 * AecWeightsDialog::onSbxWeightChange
 *****************************************************************************/
void AecWeightsDialog::onSbxWeightChange( int value )
{
    // Get index of the weight spin box
    QString spinBoxName = QObject::sender()->objectName();
    spinBoxName.remove("sbxWeight");
    int spinBoxIndex = spinBoxName.toInt();

    // Emit weight changed event
    emit AecWeightChanged( spinBoxIndex, value );
}

/******************************************************************************
 * AecWeightsDialog::onBtnResetDefaultsClicked
 *****************************************************************************/
void AecWeightsDialog::onBtnResetDefaultsClicked( void )
{
    // Loop over all spin boxes
    for ( int i = 0; i < 25; i++ )
    {
        // Find the spin box for this index
        QString spinBoxName = QString( "sbxWeight%1" ).arg( i+1 );
        QSpinBox * spinBox = ui->gbxWeights->findChild<QSpinBox *>( spinBoxName );

        // Reset spin box value to 1
        spinBox->setValue( 1 );
    }
}

/******************************************************************************
 * AecWeightsDialog::onSbxWeightChange
 *****************************************************************************/
void AecWeightsDialog::onAecWeightsChange( QVector<int> weights )
{
    // Check that the weights vector contains 25 items
    if ( weights.count() != 25 )
    {
        return;
    }

    // Loop over all entries in the weights vector
    for ( int i = 0; i < 25; i++ )
    {
        // Find the spin box for this index
        QString spinBoxName = QString( "sbxWeight%1" ).arg( i+1 );
        QSpinBox * spinBox = ui->gbxWeights->findChild<QSpinBox *>( spinBoxName );

        // Set spin box value to the weight for this index
        spinBox->setValue( weights[i] );
    }
}
