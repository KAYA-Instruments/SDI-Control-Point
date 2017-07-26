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
 * @file    fmtspinbox.h
 *
 * @brief   Class definition of a formated spinbox 
 *
 *****************************************************************************/
#ifndef __FMT_SPIN_BOX_H__
#define __FMT_SPIN_BOX_H__

#include <QDoubleSpinBox>

class FmtSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
    Q_PROPERTY( QString fmt READ getFmt WRITE setFmt DESIGNABLE true )

public:
    explicit FmtSpinBox( QWidget * parent = 0 );
    ~FmtSpinBox();

    QString getFmt() const;
    void setFmt( const QString & );

    double valueFromText( const QString & ) const;
    QString textFromValue( double value ) const;
    QVariant inputMethodQuery ( Qt::InputMethodQuery query ) const;

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __FMT_SPIN_BOX_H__


