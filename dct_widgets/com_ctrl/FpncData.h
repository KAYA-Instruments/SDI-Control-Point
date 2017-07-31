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
 * @file    FpncData.h
 *
 * @brief   Fix Pattern Noise Correction Data
 *
 *****************************************************************************/
#ifndef _FPNC_DATA_H_
#define _FPNC_DATA_H_

#include <inttypes.h>

#include <QVector>

class FpncData : public QObject
{
public:
    // contructor (image width, no of samples per column)
    explicit FpncData( int width = 1920, int no_samples = 16 );
    ~FpncData();

    int readDataFile( QString &fname );

    bool hasData() const
    {
        return ( m_has_data );
    }

    int getWidth() const
    {
        return ( m_width );
    }

    // get correction data (value = 12 bit)
    QVector<int> getCorrectionData( int column );
    
    // get calibration data (value = 24 bit, 2 columns)
    QVector<int> getCalibrationData( int column );

private:
    int index( int x, int y );

private:
    int                 m_width;
    int                 m_no_samples;
    bool                m_has_data;
    QVector<uint32_t>   m_data;
};


// compute correction data from register value
int fpnc_correction_value
(
    uint32_t const value,
    uint32_t const mask,
    uint32_t const shift
);

// validate fpnc value
int fpnc_validate_value
(
    uint32_t const value0
);

// correction data are compressed
QVector<int> & fpnc_uncompress( QVector<int> &vec );

#endif // _FPNC_DATA_H_

