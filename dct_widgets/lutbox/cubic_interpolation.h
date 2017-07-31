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
 * @file    interpolation.h
 *
 * @brief   Class definition of a cubic spline interpolation
 *
 *****************************************************************************/
#ifndef __CUBIC_INTERPOLATION_H__
#define __CUBIC_INTERPOLATION_H__

class CubicInterpolation
{
public:
    explicit CubicInterpolation();
    ~CubicInterpolation();

    void clear();
    void setSamples( QVector<int> &x, QVector<int> &y );
    int interpolate( int x );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __CUBIC_INTERPOLATION_H__

