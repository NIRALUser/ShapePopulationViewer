/****************************************************************************/
// This file is part of the gradLib library originally made by Stian Broen
//
// For more free libraries, please visit <http://broentech.no>
//
// gradLib is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library.  If not, see <http://www.gnu.org/licenses/>
/****************************************************************************/

// local includes
#include "gradientArrow.h"


GradientArrow::GradientArrow()
{
   QPolygon new_area;
   new_area << QPoint(0,6)
        << QPoint(5, 0)
        << QPoint(10,6)
        << QPoint(10, 16)
        << QPoint(0,16)
        << QPoint(0,6);
   //area << QPoint(5,0) << QPoint(5,5) << QPoint(10,12) <<  QPoint(5,19) << QPoint(0,12) << QPoint(5,5) << QPoint(5,0);
   setArea(new_area);
}

GradientArrow::GradientArrow(const GradientArrow &other)
      :m_hasFocus(other.hasFocus()),
      m_index(other.index()),
      m_area(other.area()),
      m_color(other.color()),
      m_pos(other.position())
{
}

