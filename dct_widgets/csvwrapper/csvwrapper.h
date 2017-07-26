/******************************************************************************
 *
 * Copyright 2017, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
/**
 * @file    csvwrapper.h
 *
 * @brief   Helper methods to load CSV files and convert them to QVectors.
 *
 *****************************************************************************/

#ifndef __CSV_WRAPPER_H__
#define __CSV_WRAPPER_H__

#include <QString>
#include <QVector>
#include <QPair>

int loadTableCsv(const QString & path, const int maxNumRows, const QVector<QPair<int, int>> & boundaries, QVector<QVector<int>> & cols);
int saveTableCsv(const QString & path, const QVector<QVector<int>> & cols);

#endif // __CSV_WRAPPER_H__


