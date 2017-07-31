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
 * @file    csvwrapper.cpp
 *
 * @brief   Helper methods to load CSV files and convert them to QVectors.
 *
 *****************************************************************************/

#include "csvwrapper.h"

#include <QString>
#include <QVector>
#include <QPair>
#include <QMessageBox>

#include <csv/csvparser.h>
#include <csv/csvwriter.h>

/**
 * @brief loadTableCsv This function loads a CSV table into a vector containing one vector for each column
 *                     of the table. It can only work with integers. For each column in the vector cols the
 *                     boundaries (that are the allowed min and max values for this column) can be given as a
 *                     pair in the vector boundaries.
 * @param path Specifies the path where the CSV file is located.
 * @param maxNumRows Maximum number of rows which are loaded. All rows above this value will be ignored.
 * @param boundaries Vector of Pairs. Each pair specifies the min and max allowed values fore one column in cols.
 * @param cols Returned columns which were read from the file. Data will be invalid if this function returns -1.
 * @return Returns -1 if en error occurs, in this case all data in cols is invalid. Returns 0 if no error occurs.
 */
int loadTableCsv( const QString & path, const int maxNumRows, const QVector<QPair<int, int>> & boundaries, QVector<QVector<int>> & cols)
{
    CsvParser *csvparser = CsvParser_new(path.toStdString().c_str(), ",", 0);

    CsvRow *row;
    int rowCount = 0;
    int ret = 0;
    while ( (row = CsvParser_getRow(csvparser)) && (rowCount < maxNumRows) && (ret == 0) )
    {
        const char **rowFields = CsvParser_getFields(row);
        int numColsTable = CsvParser_getNumFields(row);
        int numColsVector = cols.length();

        // Show error message if numCols does not equal the amount of columns in the output vector
        if ( numColsTable != numColsVector)
        {
            QMessageBox msgBox;
            QString msgText = QString("The CSV file %1 contains invalid data:\n\nOne ore more rows have %2 columns, but the output vector has %3 columns.").arg(path).arg(numColsTable).arg(numColsVector);
            msgBox.setText(msgText);
            msgBox.exec();
            ret = -1;
            break;
        }

        // Get value and append to vector if it is in the legal range
        for (int col = 0 ; col < numColsTable ; col++)
        {
            int value = strtol(rowFields[col], NULL, 10);
            int minValue = boundaries.at(col).first;
            int maxValue = boundaries.at(col).second;
            if ( (value >= minValue) && (value <= maxValue) )
            {
                cols[col].append(value);
            }
            else
            {
                QMessageBox msgBox;
                QString msgText = QString("The CSV file %1 contains invalid data:\n\nThe value %2 in column %3 does not lie in the legal range of %4 to %5.").arg(path).arg(value).arg(col+1).arg(minValue).arg(maxValue);
                msgBox.setText(msgText);
                msgBox.exec();
                ret = -1;
                break;
            }
        }

        CsvParser_destroy_row(row);
        rowCount++;
    }

    CsvParser_destroy(csvparser);

    if (rowCount > maxNumRows)
    {
        QMessageBox msgBox;
        QString msgText = QString("The CSV file %1 contains too much data:\n\nThe table contains more than the maximum of %2 allowed rows.").arg(path).arg(maxNumRows);
        msgBox.setText(msgText);
        msgBox.exec();
        ret = -1;
    }

    return ret;
}

/**
 * @brief saveTableCsv This functions saves a table given as a vector of vectors which resemble the columns
 *                     of the table as a CSV (comma seperated values) file. It is mandatory that all columns
 *                     contain an identical amount of rows.
 * @param path Path where the CSV file is stored.
 * @param cols Table as a vector of vectors containing integer numbers.
 * @return Returns -1 if an error occurs, 0 otherwise.
 */
int saveTableCsv(const QString & path, const QVector<QVector<int>> & cols)
{
    CsvWriter *csvWriter = CsvWriter_new(path.toStdString().c_str(), ",", 0);

    // Loop over all rows
    for ( int row = 0; row < cols[0].length(); row++ )  // Assume all columns have the same length
    {
        // Loop over all columns
        for ( int col = 0; col < cols.length(); col++ )
        {
            // Get the value from the current position
            int value = cols[col][row];

            // Convert data to c string
            std::string tempString = std::to_string(value);
            char* data = new char[tempString.length()];
            strcpy(data, tempString.c_str());

            // Write to the table
            if ( CsvWriter_writeField(csvWriter, data) )
            {
                // Show error message if write failed
                QMessageBox msgBox;
                QString msgText = QString("Could not write to CSV file '%1'. Error message: %2").arg(path, CsvWriter_getErrorMessage(csvWriter));
                msgBox.setText(msgText);
                msgBox.exec();
                return -1;
            }
        }
        CsvWriter_nextRow(csvWriter);
    }
    CsvWriter_destroy(csvWriter);

    return 0;
}
