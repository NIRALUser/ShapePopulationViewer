#ifndef COLORBARSTRUCT_H
#define COLORBARSTRUCT_H

#include <vector>
#include "colorPointStruct.h"

struct colorBarStruct
{
	std::vector<colorPointStruct> colorPointList;
	double range[2];
};

#endif
