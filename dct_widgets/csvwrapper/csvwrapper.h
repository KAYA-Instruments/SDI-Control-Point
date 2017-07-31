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


