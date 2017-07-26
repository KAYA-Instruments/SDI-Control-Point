/******************************************************************************
 *
 * Copyright 2015, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
/**
 * @file    common.h
 *
 * @brief   
 *
 *****************************************************************************/

#include <QtDebug>
#include <QString>

/******************************************************************************
 * showError
 *****************************************************************************/
void showError( int const res, const char * file, const char * func, int const line )
{
    QString text = QString( "result=%1 (file=%2:func=%3:%4)" ).arg(res).arg(file).arg(func).arg(line);
    qCritical() << text;
}


