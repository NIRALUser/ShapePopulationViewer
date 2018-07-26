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

#ifndef GRADIENTWIDGETQT_H
#define GRADIENTWIDGETQT_H

// Qt includes
#include <QWidget>
#include <QRect>
#include <QSize>
#include <QPoint>
#include <QHash>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <sstream>

// solution includes
#include "gradientArrow.h"
#include "colorPointStruct.h"


class gradientWidgetQT : public QWidget
{
   Q_OBJECT

public:
   explicit gradientWidgetQT(QWidget *Qparent = NULL);
   ~gradientWidgetQT();
    bool getAllColors(std::vector<colorPointStruct> * colorPointList);
    bool setAllColors(std::vector<colorPointStruct> * colorPointList);
    int getNumberOfArrows(){return m_arrows.size();}

    void setFocusColor(const QColor &col);
    void setFocusPosition(const qreal newPos);
    const QColor getFocusColor();
    qreal getFocusPosition();

    void saveColorPointList(QString a_filePath);
    void loadColorPointList(QString a_filePath, std::vector<colorPointStruct> * a_colorPointList);
    bool getXmlReaderValue(QXmlStreamReader * a_xmlReader, double * a_colorPointValue);

    void enable(std::vector<colorPointStruct> *a_colorPointList);
    void disable();

public slots:
   void addArrow(const QColor &color, qreal position = 0.5, bool focus = true);
   void deleteFocusArrow();
   void reset();

protected:
   void paintEvent       (QPaintEvent *);
   void mousePressEvent  (QMouseEvent *);
   void mouseDoubleClickEvent(QMouseEvent *);
   void mouseMoveEvent   (QMouseEvent *);
   void mouseReleaseEvent(QMouseEvent *);

   void toggleBackground(bool onOrOff);
   void createBackground(int vertLineSpace = 5, int horLineSpace = 5);
   QPixmap *getBackGround();

   void newFocusColor(const QColor &col, int index);


signals:
   void arrowMovedSignal(const qreal pos);
   void arrowSelectedSignal(const qreal pos);
   void arrowDoubleClickedSignal();
   void noSelectionSignal();

private:
   bool arrowMoving;
   int backgroundVerSpace;
   int backgroundHorSpace;
   QRect viewRect;
   QSize viewSize;
   QPixmap *background;
   QPoint dragStart;
   QHash<int, GradientArrow> m_arrows;
};

#endif
