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
 * @file    FpncData.cpp
 *
 * @brief   Implementation of FPNC Correction Data
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <QFileInfo>
#include <QFile>
#include <QDataStream>
#include <QtDebug>

#include "FpncData.h"

/******************************************************************************
 * helper funtion
 *****************************************************************************/

/******************************************************************************
 * fpnc_correction_value
 *****************************************************************************/
int fpnc_correction_value
(
    uint32_t const value,
    uint32_t const mask,
    uint32_t const shift
)
{
    uint32_t v = ( value & mask ) >> shift;

    // handle signed bit
    if ( v & 0x800u )
    {
        v |= 0xFFFFF000u;
    }

    return ( (int)v );
}

/******************************************************************************
 * fpnc_validate_value
 *****************************************************************************/
int fpnc_validate_value( uint32_t const value )
{
    if ( value & 0xFF000000u )
    {
        return ( -EINVAL );
    }

    return ( 0 );
}

/******************************************************************************
 * fpnc_uncompress 
 *****************************************************************************/
QVector<int> & fpnc_uncompress( QVector<int> &vec )
{
    // 0..7 *= 1
    vec[ 8] *=  2;
    vec[ 9] *=  2;
    vec[10] *=  4;
    vec[11] *=  4;
    vec[12] *=  8;
    vec[13] *=  8;
    vec[14] *= 16;
    vec[15] *= 16;

    return ( vec );
}

/******************************************************************************
 * FpncData::FpncData
 *****************************************************************************/
FpncData::FpncData( int width, int no_samples )
    : QObject()
    , m_width( width )
    , m_no_samples( no_samples )
    , m_has_data( false )
{
    m_data.resize( (width>>1) * no_samples );
}

/******************************************************************************
 * FpncData::FpncData
 *****************************************************************************/
FpncData::~FpncData()
{
    m_data.clear();
}

/******************************************************************************
 * FpncData::FpncData
 *****************************************************************************/
int FpncData::index( int x, int y )
{
    return ( (y * (m_width>>1)) + x );
}

/******************************************************************************
 * FpncData::readDataFile
 *****************************************************************************/
int FpncData::readDataFile( QString &fname )
{
    QFileInfo fInfo( fname );

    // exists && is-file check
    if ( !fInfo.exists() || !fInfo.isFile() )
    {
        return ( -ENOENT );
    }

    // file size check (no-correction-data-samples = image-width/2 * no_samples)
    qint64 expected = (m_width>>1) * m_no_samples * sizeof(uint32_t);
    if ( expected != fInfo.size() )
    {
        return ( -EINVAL );
    }

    // open file
    QFile file(fname);
    if ( !file.open( QIODevice::ReadOnly ) )
    {
        return ( -EINVAL );
    }

    // check endianess
    QDataStream in( &file );
    in.setByteOrder( QDataStream::LittleEndian );
    in.readRawData( (char *)m_data.data(), expected );

    m_has_data = true;

    return ( 0 );
}

/******************************************************************************
 * FpncData::getCorrectionData
 *****************************************************************************/
QVector<int> FpncData::getCorrectionData( int column )
{
    QVector<int> vec( m_no_samples );
    
    if ( (column < 0) || (column >= m_width) )
    {
        // return a zero-vector
        return ( vec );
    }

    uint32_t mask  = ( column & 0x1 ) ? 0x00FFF000u : 0x00000FFFu; 
    uint32_t shift = ( column & 0x1 ) ?         12u : 0u;

    for ( int i=0; i<m_no_samples; i++ )
    {
        vec[i] = fpnc_correction_value( m_data[index((column>>1), i)], mask, shift );
    }

    vec = fpnc_uncompress( vec );

    return ( vec );
}

/******************************************************************************
 * FpncData::getCalibrationData
 *****************************************************************************/
QVector<int> FpncData::getCalibrationData( int column )
{
    QVector<int> vec( m_no_samples );
    
    if ( (column < 0) || (column >= m_width) )
    {
        // return a zero-vector
        return ( vec );
    }

    for ( int i=0; i<m_no_samples; i++ )
    {
        vec[i] = m_data[index((column>>1), i)];
    }

    return ( vec );
}

