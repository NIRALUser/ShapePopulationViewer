#ifndef SHAPEPOPULATIONSTRUCT_H
#define SHAPEPOPULATIONSTRUCT_H

// Qt includes
#include <QColor>

// STD includes
#include <vector>

struct axisColorStruct
{
    double XAxiscolor[3];
    double YAxiscolor[3];
    double ZAxiscolor[3];

    bool complementaryColor;
    bool sameColor;
};

struct cameraConfigStruct
{
    double pos_x;
    double pos_y;
    double pos_z;
    double foc_x;
    double foc_y;
    double foc_z;
    double view_vx;
    double view_vy;
    double view_vz;
    double scale;
};

struct colorPointStruct
{
    double pos;
    double r;
    double g;
    double b;
};

struct colorBarStruct
{
    std::vector<colorPointStruct> colorPointList;
    double range[2];
};

struct magnitudStruct
{
    double min;
    double max;
};

#endif
