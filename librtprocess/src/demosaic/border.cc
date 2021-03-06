/*
 *  This file is part of RawTherapee.
 *
 *  Copyright (c) 2004-2010 Gabor Horvath <hgabor@rawtherapee.com>
 *
 *  RawTherapee is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RawTherapee is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RawTherapee.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <algorithm>

#include "bayerhelper.h"
#include "librtprocess.h"
#include "StopWatch.h"
#include "xtranshelper.h"

using namespace librtprocess;

rpError bayerborder_demosaic(int winw, int winh, int lborders, const float * const *rawData, float **red, float **green, float **blue, const unsigned cfarray[2][2])
{
    BENCHFUN
    if (!validateBayerCfa(3, cfarray)) {
        return RP_WRONG_CFA;
    }

    int bord = lborders;
    int width = winw;
    int height = winh;

    for (int i = 0; i < height; i++) {

        float sum[6];

        for (int j = 0; j < bord; j++) { //first few columns
            for (int c = 0; c < 6; c++) {
                sum[c] = 0;
            }

            for (int i1 = i - 1; i1 < i + 2; i1++)
                for (int j1 = j - 1; j1 < j + 2; j1++) {
                    if ((i1 > -1) && (i1 < height) && (j1 > -1)) {
                        int c = fc(cfarray, i1, j1);
                        sum[c] += rawData[i1][j1];
                        sum[c + 3]++;
                    }
                }

            int c = fc(cfarray, i, j);

            if (c == 1) {
                red[i][j] = sum[0] / sum[3];
                green[i][j] = rawData[i][j];
                blue[i][j] = sum[2] / sum[5];
            } else {
                green[i][j] = sum[1] / sum[4];

                if (c == 0) {
                    red[i][j] = rawData[i][j];
                    blue[i][j] = sum[2] / sum[5];
                } else {
                    red[i][j] = sum[0] / sum[3];
                    blue[i][j] = rawData[i][j];
                }
            }
        }//j

        for (int j = width - bord; j < width; j++) { //last few columns
            for (int c = 0; c < 6; c++) {
                sum[c] = 0;
            }

            for (int i1 = i - 1; i1 < i + 2; i1++)
                for (int j1 = j - 1; j1 < j + 2; j1++) {
                    if ((i1 > -1) && (i1 < height ) && (j1 < width)) {
                        int c = fc(cfarray, i1, j1);
                        sum[c] += rawData[i1][j1];
                        sum[c + 3]++;
                    }
                }

            int c = fc(cfarray, i, j);

            if (c == 1) {
                red[i][j] = sum[0] / sum[3];
                green[i][j] = rawData[i][j];
                blue[i][j] = sum[2] / sum[5];
            } else {
                green[i][j] = sum[1] / sum[4];

                if (c == 0) {
                    red[i][j] = rawData[i][j];
                    blue[i][j] = sum[2] / sum[5];
                } else {
                    red[i][j] = sum[0] / sum[3];
                    blue[i][j] = rawData[i][j];
                }
            }
        }//j
    }//i

    for (int i = 0; i < bord; i++) {

        float sum[6];

        for (int j = bord; j < width - bord; j++) { //first few rows
            for (int c = 0; c < 6; c++) {
                sum[c] = 0;
            }

            for (int i1 = i - 1; i1 < i + 2; i1++)
                for (int j1 = j - 1; j1 < j + 2; j1++) {
                    if ((i1 > -1) && (i1 < height) && (j1 > -1)) {
                        int c = fc(cfarray, i1, j1);
                        sum[c] += rawData[i1][j1];
                        sum[c + 3]++;
                    }
                }

            int c = fc(cfarray, i, j);

            if (c == 1) {
                red[i][j] = sum[0] / sum[3];
                green[i][j] = rawData[i][j];
                blue[i][j] = sum[2] / sum[5];
            } else {
                green[i][j] = sum[1] / sum[4];

                if (c == 0) {
                    red[i][j] = rawData[i][j];
                    blue[i][j] = sum[2] / sum[5];
                } else {
                    red[i][j] = sum[0] / sum[3];
                    blue[i][j] = rawData[i][j];
                }
            }
        }//j
    }

    for (int i = height - bord; i < height; i++) {

        float sum[6];

        for (int j = bord; j < width - bord; j++) { //last few rows
            for (int c = 0; c < 6; c++) {
                sum[c] = 0;
            }

            for (int i1 = i - 1; i1 < i + 2; i1++)
                for (int j1 = j - 1; j1 < j + 2; j1++) {
                    if ((i1 > -1) && (i1 < height) && (j1 < width)) {
                        int c = fc(cfarray, i1, j1);
                        sum[c] += rawData[i1][j1];
                        sum[c + 3]++;
                    }
                }

            int c = fc(cfarray, i, j);

            if (c == 1) {
                red[i][j] = sum[0] / sum[3];
                green[i][j] = rawData[i][j];
                blue[i][j] = sum[2] / sum[5];
            } else {
                green[i][j] = sum[1] / sum[4];

                if (c == 0) {
                    red[i][j] = rawData[i][j];
                    blue[i][j] = sum[2] / sum[5];
                } else {
                    red[i][j] = sum[0] / sum[3];
                    blue[i][j] = rawData[i][j];
                }
            }
        }//j
    }

    return RP_NO_ERROR;
}

void xtransborder_demosaic(int winw, int winh, int border, const float * const *rawData, float **red, float **green, float **blue, const unsigned xtrans[6][6])
{
    BENCHFUN
    const int height = winh, width = winw;

    const float weight[3][3] = {
                                {0.25f, 0.5f, 0.25f},
                                {0.5f,  0.f,  0.5f},
                                {0.25f, 0.5f, 0.25f}
                               };

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (col == border && row >= border && row < height - border) {
                col = width - border;
            }

            float sum[6] = {0.f};

            for (int y = std::max(0, row - 1), v = row == 0 ? 0 : -1; y <= std::min(row + 1, height - 1); y++, v++)
                for (int x = std::max(0, col - 1), h = col == 0 ? 0 : -1; x <= std::min(col + 1, width - 1); x++, h++) {
                    int f = fc(xtrans, y, x);
                    sum[f] += rawData[y][x] * weight[v + 1][h + 1];
                    sum[f + 3] += weight[v + 1][h + 1];
                }

            switch(fc(xtrans, row, col)) {
                case 0:
                    red[row][col] = rawData[row][col];
                    green[row][col] = (sum[1] / sum[4]);
                    blue[row][col] = (sum[2] / sum[5]);
                    break;

                case 1:
                    if(sum[3] == 0.f) { // at the 4 corner pixels it can happen, that we have only green pixels in 2x2 area
                        red[row][col] = green[row][col] = blue[row][col] = rawData[row][col];
                    } else {
                        red[row][col] = (sum[0] / sum[3]);
                        green[row][col] = rawData[row][col];
                        blue[row][col] = (sum[2] / sum[5]);
                    }

                    break;

                case 2:
                    red[row][col] = (sum[0] / sum[3]);
                    green[row][col] = (sum[1] / sum[4]);
                    blue[row][col] = rawData[row][col];
            }
        }
    }
}
