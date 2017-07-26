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
 * @file    btnarraybox.h
 *
 * @brief   Class definition of a button array group-box
 *
 *****************************************************************************/
#ifndef __BTN_ARRAY_BOX_H__
#define __BTN_ARRAY_BOX_H__

#include <QGroupBox>

class ButtonArrayBox : public QGroupBox
{
    Q_OBJECT
  
    Q_PROPERTY( int maxColumns READ maxColumns WRITE setMaxColumns DESIGNABLE true )

public:
    explicit ButtonArrayBox( QWidget * parent = 0 );
    ~ButtonArrayBox();

    int maxColumns() const;
    void setMaxColumns( int max );
 
    void addButton( QString label, bool newRow = false );
    void addButton( QString label, QPixmap px, bool newRow = false );
    void deleteButtons();

    void setButtonPixemap( int id, QPixmap px );

signals:
    void buttonClicked( int id );

private slots:
    void onButtonClick( int );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __BTN_ARRAY_BOX_H__

