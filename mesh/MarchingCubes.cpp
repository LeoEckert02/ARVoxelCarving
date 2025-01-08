#include "MarchingCubes.h"

const int MarchingCubes::MarchingCubesMeshGenerator::edgeTable[256] = {
    0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
    0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
    0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
    0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
    0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
    0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
    0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
    0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
    0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
    0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
    0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
    0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
    0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
    0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
    0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
    0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
    0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
    0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
    0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
    0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
    0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
    0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
    0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
    0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
    0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
    0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
    0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
    0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
    0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
    0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
    0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
    0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0
};

const int MarchingCubes::MarchingCubesMeshGenerator::triangleTable[256][16] = {
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1 },
    { 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1 },
    { 3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1 },
    { 3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1 },
    { 2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1 },
    { 8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1 },
    { 4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1 },
    { 3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1 },
    { 4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1 },
    { 4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1 },
    { 5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1 },
    { 2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1 },
    { 9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1 },
    { 2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1 },
    { 10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1 },
    { 4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1 },
    { 5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1 },
    { 5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1 },
    { 10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1 },
    { 8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1 },
    { 2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1 },
    { 7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1 },
    { 2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1 },
    { 11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1 },
    { 5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1 },
    { 11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1 },
    { 11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1 },
    { 5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1 },
    { 2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1 },
    { 5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1 },
    { 6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1 },
    { 3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1 },
    { 6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1 },
    { 5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1 },
    { 10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1 },
    { 6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1 },
    { 8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1 },
    { 7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1 },
    { 3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1 },
    { 5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1 },
    { 0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1 },
    { 9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1 },
    { 8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1 },
    { 5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1 },
    { 0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1 },
    { 6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1 },
    { 10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1 },
    { 10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1 },
    { 8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1 },
    { 1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1 },
    { 3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1 },
    { 0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1 },
    { 10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1 },
    { 3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1 },
    { 6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1 },
    { 9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1 },
    { 8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1 },
    { 3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1 },
    { 6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1 },
    { 10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1 },
    { 10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1 },
    { 2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1 },
    { 7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1 },
    { 7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1 },
    { 2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1 },
    { 1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1 },
    { 11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1 },
    { 8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1 },
    { 0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1 },
    { 7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
    { 10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1 },
    { 2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1 },
    { 6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1 },
    { 7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1 },
    { 2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1 },
    { 10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1 },
    { 10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1 },
    { 0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1 },
    { 7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1 },
    { 6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1 },
    { 8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1 },
    { 6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1 },
    { 4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1 },
    { 10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1 },
    { 8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1 },
    { 1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1 },
    { 8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1 },
    { 10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1 },
    { 4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1 },
    { 10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
    { 5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1 },
    { 11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1 },
    { 9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1 },
    { 6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1 },
    { 7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1 },
    { 3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1 },
    { 7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1 },
    { 3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1 },
    { 6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1 },
    { 9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1 },
    { 1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1 },
    { 4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1 },
    { 7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1 },
    { 6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1 },
    { 3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1 },
    { 0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1 },
    { 6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1 },
    { 0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1 },
    { 11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1 },
    { 6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1 },
    { 5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1 },
    { 9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1 },
    { 1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1 },
    { 10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1 },
    { 0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1 },
    { 5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1 },
    { 10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1 },
    { 11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1 },
    { 9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1 },
    { 7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1 },
    { 2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1 },
    { 8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1 },
    { 9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1 },
    { 9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1 },
    { 1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1 },
    { 5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1 },
    { 0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1 },
    { 10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1 },
    { 2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1 },
    { 0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1 },
    { 0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1 },
    { 9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1 },
    { 5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1 },
    { 3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1 },
    { 5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1 },
    { 8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1 },
    { 9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1 },
    { 1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1 },
    { 3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1 },
    { 4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1 },
    { 9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1 },
    { 11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1 },
    { 11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1 },
    { 2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1 },
    { 9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1 },
    { 3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1 },
    { 1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1 },
    { 4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1 },
    { 4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1 },
    { 3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1 },
    { 3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1 },
    { 0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1 },
    { 9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1 },
    { 1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
};

Vector3d MarchingCubes::MarchingCubesMeshGenerator::vertexInterpolate(
    const Vector3d &v1, const Vector3d &v2, 
    double level, double value1, double value2
) {
    double t = (level - value1) / (value2 - value1);
    return (1. - t) * v1 + t * v2;
}

void MarchingCubes::MarchingCubesMeshGenerator::prepareEdgeOccupancy(uint32_t n, uint32_t m, uint32_t p) {
    f_edgeOccupancy.clear();
    f_edgeOccupancy.shrink_to_fit();
    f_edgeOccupancy.resize(n - 1);
    for (unsigned i = 0; i < n - 1; i++) {
        f_edgeOccupancy.at(i).resize(m - 1);
        for (unsigned j = 0; j < m - 1; j++) {
            f_edgeOccupancy.at(i).at(j).resize(m - 1);
            for (unsigned k = 0; k < p - 1; k++) {
                f_edgeOccupancy.at(i).at(j).at(k).resize(12);
                for (unsigned edgeId = 0; edgeId < 12; edgeId++) {
                    f_edgeOccupancy.at(i).at(j).at(k).at(edgeId) = -1;
                }
            }
        }
    }
}

int MarchingCubes::MarchingCubesMeshGenerator::lookupEdgeOccupancy(uint32_t i, uint32_t j, uint32_t k, uint32_t edgeId, uint32_t n, uint32_t m, uint32_t p) const {
    if (f_edgeOccupancy.at(i).at(j).at(k).at(edgeId) != -1) return f_edgeOccupancy.at(i).at(j).at(k).at(edgeId);
    switch (edgeId)
    {
    case 0:
        if (j != 0)
            if (f_edgeOccupancy.at(i).at(j - 1).at(k).at(2) != -1) return f_edgeOccupancy.at(i).at(j - 1).at(k).at(2);
        if (k != 0)
            if (f_edgeOccupancy.at(i).at(j).at(k - 1).at(4) != -1) return f_edgeOccupancy.at(i).at(j).at(k - 1).at(4);
        if (j != 0 && k != 0)
            if (f_edgeOccupancy.at(i).at(j - 1).at(k - 1).at(6) != -1) return f_edgeOccupancy.at(i).at(j - 1).at(k - 1).at(6);
        break;
    case 1:
        if (i != 0)
            if (f_edgeOccupancy.at(i - 1).at(j).at(k).at(3) != -1) return f_edgeOccupancy.at(i - 1).at(j).at(k).at(3);
        if (k != 0)
            if (f_edgeOccupancy.at(i).at(j).at(k - 1).at(5) != -1) return f_edgeOccupancy.at(i).at(j).at(k - 1).at(5);
        if (i != 0 && k != 0)
            if (f_edgeOccupancy.at(i - 1).at(j).at(k - 1).at(7) != -1) return f_edgeOccupancy.at(i - 1).at(j).at(k - 1).at(7);
        break;
    case 2:
        if (j < m - 2)
            if (f_edgeOccupancy.at(i).at(j + 1).at(k).at(0) != -1) return f_edgeOccupancy.at(i).at(j + 1).at(k).at(0);
        if (k != 0)
            if (f_edgeOccupancy.at(i).at(j).at(k - 1).at(6) != -1) return f_edgeOccupancy.at(i).at(j).at(k - 1).at(6);
        if (j < m - 2 && k != 0)
            if (f_edgeOccupancy.at(i).at(j + 1).at(k - 1).at(4) != -1) return f_edgeOccupancy.at(i).at(j + 1).at(k - 1).at(4);
        break;
    case 3:
        if (i < n - 2)
            if (f_edgeOccupancy.at(i + 1).at(j).at(k).at(1) != -1) return f_edgeOccupancy.at(i + 1).at(j).at(k).at(1);
        if (k != 0)
            if (f_edgeOccupancy.at(i).at(j).at(k - 1).at(7) != -1) return f_edgeOccupancy.at(i).at(j).at(k - 1).at(7);
        if (i < n - 2 && k != 0)
            if (f_edgeOccupancy.at(i + 1).at(j).at(k - 1).at(5) != -1) return f_edgeOccupancy.at(i + 1).at(j).at(k - 1).at(5);
        break;
    case 4:
        if (j != 0)
            if (f_edgeOccupancy.at(i).at(j - 1).at(k).at(6) != -1) return f_edgeOccupancy.at(i).at(j - 1).at(k).at(6);
        if (k < p - 2)
            if (f_edgeOccupancy.at(i).at(j).at(k + 1).at(0) != -1) return f_edgeOccupancy.at(i).at(j).at(k + 1).at(0);
        if (j != 0 && k < p - 2)
            if (f_edgeOccupancy.at(i).at(j - 1).at(k + 1).at(2) != -1) return f_edgeOccupancy.at(i).at(j - 1).at(k + 1).at(2);
        break;
    case 5:
        if (i != 0)
            if (f_edgeOccupancy.at(i - 1).at(j).at(k).at(7) != -1) return f_edgeOccupancy.at(i - 1).at(j).at(k).at(7);
        if (k < p - 2)
            if (f_edgeOccupancy.at(i).at(j).at(k + 1).at(1) != -1) return f_edgeOccupancy.at(i).at(j).at(k + 1).at(1);
        if (i != 0 && k < p - 2)
            if (f_edgeOccupancy.at(i - 1).at(j).at(k + 1).at(3) != -1) return f_edgeOccupancy.at(i - 1).at(j).at(k + 1).at(3);
        break;
    case 6:
        if (j < m - 2)
            if (f_edgeOccupancy.at(i).at(j + 1).at(k).at(4) != -1) return f_edgeOccupancy.at(i).at(j + 1).at(k).at(4);
        if (k < p - 2)
            if (f_edgeOccupancy.at(i).at(j).at(k + 1).at(2) != -1) return f_edgeOccupancy.at(i).at(j).at(k + 1).at(2);
        if (j < m - 2 && k < p - 2)
            if (f_edgeOccupancy.at(i).at(j + 1).at(k + 1).at(0) != -1) return f_edgeOccupancy.at(i).at(j + 1).at(k + 1).at(0);
        break;
    case 7:
        if (i < n - 2)
            if (f_edgeOccupancy.at(i + 1).at(j).at(k).at(5) != -1) return f_edgeOccupancy.at(i + 1).at(j).at(k).at(5);
        if (k < p - 2)
            if (f_edgeOccupancy.at(i).at(j).at(k + 1).at(3) != -1) return f_edgeOccupancy.at(i).at(j).at(k + 1).at(3);
        if (i < n - 2 && k < p - 2)
            if (f_edgeOccupancy.at(i + 1).at(j).at(k + 1).at(1) != -1) return f_edgeOccupancy.at(i + 1).at(j).at(k + 1).at(1);
        break;
    case 8:
        if (i < n - 2)
            if (f_edgeOccupancy.at(i + 1).at(j).at(k).at(9) != -1) return f_edgeOccupancy.at(i + 1).at(j).at(k).at(9);
        if (j != 0)
            if (f_edgeOccupancy.at(i).at(j - 1).at(k).at(11) != -1) return f_edgeOccupancy.at(i).at(j - 1).at(k).at(11);
        if (i < n - 2 && j != 0)
            if (f_edgeOccupancy.at(i + 1).at(j - 1).at(k).at(10) != -1) return f_edgeOccupancy.at(i + 1).at(j - 1).at(k).at(10);
        break;
    case 9:
        if (i != 0)
            if (f_edgeOccupancy.at(i - 1).at(j).at(k).at(8) != -1) return f_edgeOccupancy.at(i - 1).at(j).at(k).at(8);
        if (j != 0)
            if (f_edgeOccupancy.at(i).at(j - 1).at(k).at(10) != -1) return f_edgeOccupancy.at(i).at(j - 1).at(k).at(10);
        if (i != 0 && j != 0)
            if (f_edgeOccupancy.at(i - 1).at(j - 1).at(k).at(11) != -1) return f_edgeOccupancy.at(i - 1).at(j - 1).at(k).at(11);
        break;
    case 10:
        if (i != 0)
            if (f_edgeOccupancy.at(i - 1).at(j).at(k).at(11) != -1) return f_edgeOccupancy.at(i - 1).at(j).at(k).at(11);
        if (j < m - 2)
            if (f_edgeOccupancy.at(i).at(j + 1).at(k).at(9) != -1) return f_edgeOccupancy.at(i).at(j + 1).at(k).at(9);
        if (i != 0 && j < m - 2)
            if (f_edgeOccupancy.at(i - 1).at(j + 1).at(k).at(8) != -1) return f_edgeOccupancy.at(i - 1).at(j + 1).at(k).at(8);
        break;
    case 11:
        if (i < n - 2)
            if (f_edgeOccupancy.at(i + 1).at(j).at(k).at(10) != -1) return f_edgeOccupancy.at(i + 1).at(j).at(k).at(10);
        if (j < m - 2)
            if (f_edgeOccupancy.at(i).at(j + 1).at(k).at(8) != -1) return f_edgeOccupancy.at(i).at(j + 1).at(k).at(8);
        if (i < n - 2 && j < m - 2)
            if (f_edgeOccupancy.at(i + 1).at(j + 1).at(k).at(9) != -1) return f_edgeOccupancy.at(i + 1).at(j + 1).at(k).at(9);
        break;
    default:
        break;
    }
    return -1;
}

void MarchingCubes::MarchingCubesMeshGenerator::updateOccupancy(uint32_t i, uint32_t j, uint32_t k, uint32_t edgeId, uint32_t vertexId, uint32_t n, uint32_t m, uint32_t p) {
    f_edgeOccupancy.at(i).at(j).at(k).at(edgeId) = vertexId;
    switch (edgeId)
    {
    case 0:
        if (j != 0)
            f_edgeOccupancy.at(i).at(j - 1).at(k).at(2) = vertexId;
        if (k != 0)
            f_edgeOccupancy.at(i).at(j).at(k - 1).at(4) = vertexId;
        if (j != 0 && k != 0)
            f_edgeOccupancy.at(i).at(j - 1).at(k - 1).at(6) = vertexId;
        break;
    case 1:
        if (i != 0)
            f_edgeOccupancy.at(i - 1).at(j).at(k).at(3) = vertexId;
        if (k != 0)
            f_edgeOccupancy.at(i).at(j).at(k - 1).at(5) = vertexId;
        if (i != 0 && k != 0)
            f_edgeOccupancy.at(i - 1).at(j).at(k - 1).at(7) = vertexId;
        break;
    case 2:
        if (j < m - 2)
            f_edgeOccupancy.at(i).at(j + 1).at(k).at(0) = vertexId;
        if (k != 0)
            f_edgeOccupancy.at(i).at(j).at(k - 1).at(6) = vertexId;
        if (j < m - 2 && k != 0)
            f_edgeOccupancy.at(i).at(j + 1).at(k - 1).at(4) = vertexId;
        break;
    case 3:
        if (i < n - 2)
            f_edgeOccupancy.at(i + 1).at(j).at(k).at(1) = vertexId;
        if (k != 0)
            f_edgeOccupancy.at(i).at(j).at(k - 1).at(7) = vertexId;
        if (i < n - 2 && k != 0)
            f_edgeOccupancy.at(i + 1).at(j).at(k - 1).at(5) = vertexId;
        break;
    case 4:
        if (j != 0)
            f_edgeOccupancy.at(i).at(j - 1).at(k).at(6) = vertexId;
        if (k < p - 2)
            f_edgeOccupancy.at(i).at(j).at(k + 1).at(0) = vertexId;
        if (j != 0 && k < p - 2)
            f_edgeOccupancy.at(i).at(j - 1).at(k + 1).at(2) = vertexId;
        break;
    case 5:
        if (i != 0)
            f_edgeOccupancy.at(i - 1).at(j).at(k).at(7) = vertexId;
        if (k < p - 2)
            f_edgeOccupancy.at(i).at(j).at(k + 1).at(1) = vertexId;
        if (i != 0 && k < p - 2)
            f_edgeOccupancy.at(i - 1).at(j).at(k + 1).at(3) = vertexId;
        break;
    case 6:
        if (j < m - 2)
            f_edgeOccupancy.at(i).at(j + 1).at(k).at(4) = vertexId;
        if (k < p - 2)
            f_edgeOccupancy.at(i).at(j).at(k + 1).at(2) = vertexId;
        if (j < m - 2 && k < p - 2)
            f_edgeOccupancy.at(i).at(j + 1).at(k + 1).at(0) = vertexId;
        break;
    case 7:
        if (i < n - 2)
            f_edgeOccupancy.at(i + 1).at(j).at(k).at(5) = vertexId;
        if (k < p - 2)
            f_edgeOccupancy.at(i).at(j).at(k + 1).at(3) = vertexId;
        if (i < n - 2 && k < p - 2)
            f_edgeOccupancy.at(i + 1).at(j).at(k + 1).at(1) = vertexId;
        break;
    case 8:
        if (i < n - 2)
            f_edgeOccupancy.at(i + 1).at(j).at(k).at(9) = vertexId;
        if (j != 0)
            f_edgeOccupancy.at(i).at(j - 1).at(k).at(11) = vertexId;
        if (i < n - 2 && j != 0)
            f_edgeOccupancy.at(i + 1).at(j - 1).at(k).at(10) = vertexId;
        break;
    case 9:
        if (i != 0)
            f_edgeOccupancy.at(i - 1).at(j).at(k).at(8) = vertexId;
        if (j != 0)
            f_edgeOccupancy.at(i).at(j - 1).at(k).at(10) = vertexId;
        if (i != 0 && j != 0)
            f_edgeOccupancy.at(i - 1).at(j - 1).at(k).at(11) = vertexId;
        break;
    case 10:
        if (i != 0)
            f_edgeOccupancy.at(i - 1).at(j).at(k).at(11) = vertexId;
        if (j < m - 2)
            f_edgeOccupancy.at(i).at(j + 1).at(k).at(9) = vertexId;
        if (i != 0 && j < m - 2)
            f_edgeOccupancy.at(i - 1).at(j + 1).at(k).at(8) = vertexId;
        break;
    case 11:
        if (i < n - 2)
            f_edgeOccupancy.at(i + 1).at(j).at(k).at(10) = vertexId;
        if (j < m - 2)
            f_edgeOccupancy.at(i).at(j + 1).at(k).at(8) = vertexId;
        if (i < n - 2 && j < m - 2)
            f_edgeOccupancy.at(i + 1).at(j + 1).at(k).at(9) = vertexId;
        break;
    default:
        break;
    }
}

void MarchingCubes::MarchingCubesMeshGenerator::handleEdge(
    unsigned i, unsigned j, unsigned k, 
    uint32_t n, uint32_t m, uint32_t p,
    uint32_t edgeId, uint32_t v1Id, uint32_t v2Id, 
    const MarchingCubesCell &cell, double level,
    uint32_t *vertexIds, 
    Mesh::TriangleMesh &mesh
) {
    uint32_t vertexId;
    // Lookup if vertex was already generated
    int edgeOccupancy = lookupEdgeOccupancy(i, j, k, edgeId, n, m, p);
    if (edgeOccupancy == -1) {
        // Vertex for this crossing has not yet been generated
        // Calculate vertex coordinates
        Vector3d coordinates = MarchingCubesMeshGenerator::vertexInterpolate(
            cell.vertices[v1Id],
            cell.vertices[v2Id],
            level,
            cell.values[v1Id],
            cell.values[v2Id]
        );

        Vector4uc color(255, 255, 255, 255);

        if (cell.values[v1Id] <= level) color = cell.colors[0];
        if (cell.values[v2Id] <= level) color = cell.colors[0];

        // Add vertex and save vertex id
        vertexId = mesh.addVertex(
            coordinates,
            color
        );

        // Update occupancy
        updateOccupancy(i, j, k, edgeId, vertexId, n, m, p);
    }
    else {
        // Vertex for this crossing has already been generated
        vertexId = edgeOccupancy;
    }
    // Save vertex id under corresponding edge id
        vertexIds[edgeId] = vertexId;
}

void MarchingCubes::MarchingCubesMeshGenerator::generateMCMesh(Mesh::TriangleMesh &mesh, const SDF::SDFCSample &sdf, double level) {
    // // Clear the output mesh
    // mesh.clear();

    // Prepare edge occupancy temporary field
    std::cout << "Preparing edge occupancy..." << std::endl;
    prepareEdgeOccupancy(sdf.f_n, sdf.f_m, sdf.f_p);

    std::cout << "Handling cells..." << std::endl;
    // Iterate over all cells
    for (unsigned i = 0; i < sdf.f_n - 1; i++) {
        for (unsigned j = 0; j < sdf.f_m - 1; j++) {
            for (unsigned k = 0; k < sdf.f_p - 1; k++) {
                // Make grid cell
                MarchingCubesCell cell;
                cell.values[0] = sdf.getValue(i + 1, j, k);
                cell.values[1] = sdf.getValue(i, j, k);
                cell.values[2] = sdf.getValue(i, j + 1, k);
                cell.values[3] = sdf.getValue(i + 1, j + 1, k);
                cell.values[4] = sdf.getValue(i + 1, j, k + 1);
                cell.values[5] = sdf.getValue(i, j, k + 1);
                cell.values[6] = sdf.getValue(i, j + 1, k + 1);
                cell.values[7] = sdf.getValue(i + 1, j + 1, k + 1);

                cell.vertices[0] = sdf.getPoint(i + 1, j, k);
                cell.vertices[1] = sdf.getPoint(i, j, k);
                cell.vertices[2] = sdf.getPoint(i, j + 1, k);
                cell.vertices[3] = sdf.getPoint(i + 1, j + 1, k);
                cell.vertices[4] = sdf.getPoint(i + 1, j, k + 1);
                cell.vertices[5] = sdf.getPoint(i, j, k + 1);
                cell.vertices[6] = sdf.getPoint(i, j + 1, k + 1);
                cell.vertices[7] = sdf.getPoint(i + 1, j + 1, k + 1);

                cell.colors[0] = sdf.getColor(i + 1, j, k);
                cell.colors[1] = sdf.getColor(i, j, k);
                cell.colors[2] = sdf.getColor(i, j + 1, k);
                cell.colors[3] = sdf.getColor(i + 1, j + 1, k);
                cell.colors[4] = sdf.getColor(i + 1, j, k + 1);
                cell.colors[5] = sdf.getColor(i, j, k + 1);
                cell.colors[6] = sdf.getColor(i, j + 1, k + 1);
                cell.colors[7] = sdf.getColor(i + 1, j + 1, k + 1);

                // Handle cell
                // Determine cell index
                int cellId = 0;
                // Bitwise OR for every vertex state
                if (cell.values[0] < level) cellId |= 1;
                if (cell.values[1] < level) cellId |= 2;
                if (cell.values[2] < level) cellId |= 4;
                if (cell.values[3] < level) cellId |= 8;
                if (cell.values[4] < level) cellId |= 16;
                if (cell.values[5] < level) cellId |= 32;
                if (cell.values[6] < level) cellId |= 64;
                if (cell.values[7] < level) cellId |= 128;

                // If no level crossings skip cell
                if (edgeTable[cellId] == 0) continue;

                // Determine edges along which level crossings happen
                    // Edge id - vertex id relation
                        // 0  - 01
                        // 1  - 12
                        // 2  - 23
                        // 3  - 30
                        // 4  - 45
                        // 5  - 56
                        // 6  - 67
                        // 7  - 74
                        // 8  - 04
                        // 9  - 15
                        // 10 - 26
                        // 11 - 37
                // Bitwise AND, true if edge has a crossing
                uint32_t edgeId;
                uint32_t v1Id;
                uint32_t v2Id;
                uint32_t vertexIds[12];
                //  void handleEdge(unsigned i, unsigned j, unsigned k,
                //     uint32_t edgeId, uint32_t v1Id, uint32_t v2Id,
                //     const MarchingCubesCell& cell,
                //     double level,
                //     uint32_t& vertexId, uint32_t* vertexIds,
                //     Mesh::TriangleMesh& mesh
                //
                if (edgeTable[cellId] & 1) {
                    // Edge edgeId has a crossing
                    edgeId = 0; v1Id = 0; v2Id = 1;
                    handleEdge(i, j, k, sdf.f_n, sdf.f_m, sdf.f_p, edgeId, v1Id, v2Id, cell, level, vertexIds, mesh);
                }
                if (edgeTable[cellId] & 2) {
                    // Edge edgeId has a crossing
                    edgeId = 1; v1Id = 1; v2Id = 2;
                    handleEdge(i, j, k, sdf.f_n, sdf.f_m, sdf.f_p, edgeId, v1Id, v2Id, cell, level, vertexIds, mesh);
                }
                if (edgeTable[cellId] & 4) {
                    // Edge edgeId has a crossing
                    edgeId = 2; v1Id = 2; v2Id = 3;
                    handleEdge(i, j, k, sdf.f_n, sdf.f_m, sdf.f_p, edgeId, v1Id, v2Id, cell, level, vertexIds, mesh);
                }
                if (edgeTable[cellId] & 8) {
                    // Edge edgeId has a crossing
                    edgeId = 3; v1Id = 3; v2Id = 0;
                    handleEdge(i, j, k, sdf.f_n, sdf.f_m, sdf.f_p, edgeId, v1Id, v2Id, cell, level, vertexIds, mesh);
                }
                if (edgeTable[cellId] & 16) {
                    // Edge edgeId has a crossing
                    edgeId = 4; v1Id = 4; v2Id = 5;
                    handleEdge(i, j, k, sdf.f_n, sdf.f_m, sdf.f_p, edgeId, v1Id, v2Id, cell, level, vertexIds, mesh);
                }
                if (edgeTable[cellId] & 32) {
                    // Edge edgeId has a crossing
                    edgeId = 5; v1Id = 5; v2Id = 6;
                    handleEdge(i, j, k, sdf.f_n, sdf.f_m, sdf.f_p, edgeId, v1Id, v2Id, cell, level, vertexIds, mesh);
                }
                if (edgeTable[cellId] & 64) {
                    // Edge edgeId has a crossing
                    edgeId = 6; v1Id = 6; v2Id = 7;
                    handleEdge(i, j, k, sdf.f_n, sdf.f_m, sdf.f_p, edgeId, v1Id, v2Id, cell, level, vertexIds, mesh);
                }
                if (edgeTable[cellId] & 128) {
                    // Edge edgeId has a crossing
                    edgeId = 7; v1Id = 7; v2Id = 4;
                    handleEdge(i, j, k, sdf.f_n, sdf.f_m, sdf.f_p, edgeId, v1Id, v2Id, cell, level, vertexIds, mesh);
                }
                if (edgeTable[cellId] & 256) {
                    // Edge edgeId has a crossing
                    edgeId = 8; v1Id = 0; v2Id = 4;
                    handleEdge(i, j, k, sdf.f_n, sdf.f_m, sdf.f_p, edgeId, v1Id, v2Id, cell, level, vertexIds, mesh);
                }
                if (edgeTable[cellId] & 512) {
                    // Edge edgeId has a crossing
                    edgeId = 9; v1Id = 1; v2Id = 5;
                    handleEdge(i, j, k, sdf.f_n, sdf.f_m, sdf.f_p, edgeId, v1Id, v2Id, cell, level, vertexIds, mesh);
                }
                if (edgeTable[cellId] & 1024) {
                    // Edge edgeId has a crossing
                    edgeId = 10; v1Id = 2; v2Id = 6;
                    handleEdge(i, j, k, sdf.f_n, sdf.f_m, sdf.f_p, edgeId, v1Id, v2Id, cell, level, vertexIds, mesh);
                }
                if (edgeTable[cellId] & 2048) {
                    // Edge edgeId has a crossing
                    edgeId = 11; v1Id = 3; v2Id = 7;
                    handleEdge(i, j, k, sdf.f_n, sdf.f_m, sdf.f_p, edgeId, v1Id, v2Id, cell, level, vertexIds, mesh);
                }

                // Assemble triangles according to the triangleTable and vertexIds
                // For cycle for all triangles for cell
                // Number of triangles is determined by the amount of
                // vertex index tripples in the triangleTable
                for (unsigned corner = 0; triangleTable[cellId][corner] != -1; corner += 3) {
                    // Add triangle to the mesh
                    mesh.addTriangle(
                        vertexIds[triangleTable[cellId][corner]],
                        vertexIds[triangleTable[cellId][corner + 1]],
                        vertexIds[triangleTable[cellId][corner + 2]]
                    );
                }
            }
        }
    }
}
