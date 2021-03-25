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
 * @file    roi_interpolation.h
 *
 * @brief   Class definition of a cubic spline interpolation
 *
 *****************************************************************************/
#ifndef __ROI_INTERPOLATION_H__
#define __ROI_INTERPOLATION_H__

class ROIInterpolation
{
public:
    explicit ROIInterpolation();
    ~ROIInterpolation();

    void clear();
    void setConfig( uint8_t knee, uint16_t slope, uint8_t clip );
    int interpolate( int x );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __ROI_INTERPOLATION_H__

