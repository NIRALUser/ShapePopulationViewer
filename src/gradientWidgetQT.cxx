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

// Qt includes
#include <QPainter>
#include <QMouseEvent>

// local includes
#include "gradientWidgetQT.h"

gradientWidgetQT::gradientWidgetQT(QWidget *Qparent)
   :QWidget(Qparent),
   arrowMoving(false),
   backgroundVerSpace(5),
   backgroundHorSpace(5),
   background(NULL)
{
   qRegisterMetaType<GradientArrow>("GradientArrow");
   setFocusPolicy(Qt::StrongFocus);
   viewRect = QRect(QPoint(0,0), QPoint(0,0));
   setMouseTracking(true);
   update();
}

gradientWidgetQT::~gradientWidgetQT()
{
   delete background;
}

//f+//////////////////////////////////////////////////////////////////////////
//f-//////////////////////////////////////////////////////////////////////////

bool gradientWidgetQT::getAllColors(std::vector<colorPointStruct> * colorPointList)
{
   colorPointList->clear();

   if(m_arrows.isEmpty())
   {
      return false;
   }

   QHashIterator<int, GradientArrow> it(m_arrows);
   while(it.hasNext())
   {
      it.next();
      colorPointStruct colorPoint;
      colorPoint.pos = it.value().position();
      colorPoint.r = (double)(it.value().color().red()) / 255.0 ;
      colorPoint.g = (double)(it.value().color().green()) / 255.0 ;
      colorPoint.b = (double)(it.value().color().blue()) / 255.0 ;
      colorPointList->push_back(colorPoint);
   }

   return true;
}

bool gradientWidgetQT::setAllColors(std::vector<colorPointStruct> * colorPointList)
{

    this->blockSignals(true);

    m_arrows.clear();

    for(unsigned int i = 0 ;  i < colorPointList->size() ; i++)
    {
        QColor color(colorPointList->at(i).r*255,colorPointList->at(i).g*255,colorPointList->at(i).b*255);
        this->addArrow(color,colorPointList->at(i).pos);
    }

    this->blockSignals(false);

    return true;
}


//f+//////////////////////////////////////////////////////////////////////////
//f-//////////////////////////////////////////////////////////////////////////

bool gradientWidgetQT::getXmlReaderValue(QXmlStreamReader * a_xmlReader, double * a_colorPointValue)
{
    a_xmlReader->readNext();
    if(a_xmlReader->tokenType() != QXmlStreamReader::Characters) return false;

    double value =  a_xmlReader->text().toString().toDouble();
    if(value < 0.0 || value > 1.0)
    {
        std::ostringstream strs;
        std::string str;
        strs << "Error with value column " << a_xmlReader->columnNumber() << std::endl
             << "Value = " << value << " while should be between 0 and 1";
        str = strs.str();
        QMessageBox::critical(this,"XML Reader error",QString(str.c_str()),QMessageBox::Ok);
        return false;
    }

    *a_colorPointValue = value;
    return true;
}

void gradientWidgetQT::loadColorPointList(QString a_filePath, std::vector<colorPointStruct> * a_colorPointList)
{
    QFile file(a_filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        /* If we can't open it, let's show an error message. */
        QMessageBox::critical(this,"XML Reader error","Couldn't open .spvcm",QMessageBox::Ok);
        return;
    }

    /* QXmlStreamReader takes any QIODevice. */
    QXmlStreamReader xmlReader(&file);
    std::vector<colorPointStruct> colorPointList;

    /* We'll parse the XML until we reach end of it.*/
    while(!xmlReader.atEnd() && !xmlReader.hasError())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xmlReader.readNext();

        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument) continue;

        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement)
        {
            /* If it's named colormap, we'll go to the next.*/
            if(xmlReader.name() == "colormap") continue;

            /* If it's named colorpoint, we'll dig the information from there.*/
            if(xmlReader.name() == "colorpoint")
            {
                colorPointStruct ColorPoint;
                xmlReader.readNext();

                while(!(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "colorpoint"))
                {
                    if(xmlReader.tokenType() == QXmlStreamReader::StartElement)
                    {
                        /* We've found position. */
                        if(xmlReader.name() == "position")
                        {
                            getXmlReaderValue(&xmlReader,&ColorPoint.pos);
                        }
                        /* We've found Red value. */
                        if(xmlReader.name() == "R")
                        {
                            getXmlReaderValue(&xmlReader,&ColorPoint.r);
                        }
                        /* We've found Green value. */
                        if(xmlReader.name() == "G")
                        {
                            getXmlReaderValue(&xmlReader,&ColorPoint.g);
                        }
                        /* We've found Blue value. */
                        if(xmlReader.name() == "B")
                        {
                            getXmlReaderValue(&xmlReader,&ColorPoint.b);
                        }
                    }
                    /* ...and next... */
                    xmlReader.readNext();
                }
                /* Add the point to the colorMap */
                colorPointList.push_back(ColorPoint);
            }
        }
    }

    /* Error handling. */
    if(xmlReader.hasError())
    {
        QMessageBox::critical(this,"XML Reader error",xmlReader.errorString(), QMessageBox::Ok);
        return;
    }

    /* Removes any device() or data from the reader and resets its internal state to the initial state. */
    xmlReader.clear();

    /* Load the colormap */
    *a_colorPointList = colorPointList;
    this->setAllColors(a_colorPointList);
}

void gradientWidgetQT::saveColorPointList(QString a_filePath)
{
    //Get ColorPointList
    std::vector<colorPointStruct> colorPointList;
    getAllColors(&colorPointList);

    //write XML File
    QFile file(a_filePath);
    QFileInfo fileInfo(a_filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        /* show error message if not able to open file */
        QMessageBox::warning(0, "Read only", "The file is in read only mode");
    }
    else
    {
        std::ostringstream strs;
        std::string str;

        QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();
        xmlWriter->setAutoFormatting(true);
        xmlWriter->setDevice(&file);

        xmlWriter->writeStartDocument();

        /* SPVColorMap */
        xmlWriter->writeStartElement("SPVColorMap");
        xmlWriter->writeAttribute("description", "ShapePopulationViewer Color Map configuration");
        xmlWriter->writeAttribute("version", "1.0");

            /* ColorMap */
            xmlWriter->writeStartElement("colormap");
            xmlWriter->writeAttribute("name", fileInfo.baseName());
            strs.str(""); strs.clear();
            strs << colorPointList.size() ;
            str = strs.str();
            xmlWriter->writeAttribute("points", QString(str.c_str()));

            for(unsigned int i = 0 ; i < colorPointList.size() ; i++ )
            {
                /* ColorPoint */
                xmlWriter->writeStartElement("colorpoint");
                strs.str(""); strs.clear();
                strs << i ;
                str = strs.str();
                xmlWriter->writeAttribute("index", QString(str.c_str()));

                    /* Position */
                    xmlWriter->writeStartElement("position");
                    strs.str(""); strs.clear();
                    strs << colorPointList[i].pos ;
                    str = strs.str();
                    xmlWriter->writeCharacters(QString(str.c_str()));
                    xmlWriter->writeEndElement();

                    /* Red */
                    xmlWriter->writeStartElement("R");
                    strs.str(""); strs.clear();
                    strs << colorPointList[i].r ;
                    str = strs.str();
                    xmlWriter->writeCharacters(QString(str.c_str()));
                    xmlWriter->writeEndElement();

                    /* Green */
                    xmlWriter->writeStartElement("G");
                    strs.str(""); strs.clear();
                    strs << colorPointList[i].g ;
                    str = strs.str();
                    xmlWriter->writeCharacters(QString(str.c_str()));
                    xmlWriter->writeEndElement();

                    /* Blue */
                    xmlWriter->writeStartElement("B");
                    strs.str(""); strs.clear();
                    strs << colorPointList[i].b ;
                    str = strs.str();
                    xmlWriter->writeCharacters(QString(str.c_str()));
                    xmlWriter->writeEndElement();

                xmlWriter->writeEndElement();
            }
            xmlWriter->writeEndElement();
        xmlWriter->writeEndElement();

        xmlWriter->writeEndDocument();
        delete xmlWriter;
    }
}


//f+//////////////////////////////////////////////////////////////////////////
//f-//////////////////////////////////////////////////////////////////////////

void gradientWidgetQT::enable(std::vector<colorPointStruct> * a_colorPointList)
{
    this->setAllColors(a_colorPointList);
}

void gradientWidgetQT::disable()
{
    std::vector<colorPointStruct> colorpointList;
    colorpointList.clear();
    this->setAllColors(&colorpointList);
}

//f+//////////////////////////////////////////////////////////////////////////
//f-//////////////////////////////////////////////////////////////////////////
void gradientWidgetQT::createBackground(int vertLineSpace, int horLineSpace)
{
   backgroundVerSpace = vertLineSpace;
   backgroundHorSpace = horLineSpace;
   if(NULL != background)
   {
      delete background;
      background = NULL;
   }
   background = new QPixmap(QSize(800,800));
   background->fill(Qt::white);
   QPainter painter(background);

   // draw vertical lines
   int vert = 0;
   while(vert < background->width())
   {
      const QPoint lineStart(vert,0);
      const QPoint lineStop(vert, background->height());
      painter.drawLine(lineStart, lineStop);
      vert += vertLineSpace;
   }

   // draw horizontal lines
   int horiz = 0;
   while(horiz < background->height())
   {
      const QPoint lineStart(0, horiz);
      const QPoint lineStop (background->width(), horiz);
      painter.drawLine(lineStart, lineStop);
      horiz += horLineSpace;
   }
   painter.end();
   update();
}

QPixmap* gradientWidgetQT::getBackGround()
{
   if(NULL == background)
   {
      createBackground(backgroundVerSpace, backgroundHorSpace);
   }
   return background;
}

void gradientWidgetQT::toggleBackground(bool onOrOff)
{
   if(onOrOff == false)
   {
      if(NULL == background)
      {
         background = new QPixmap(viewRect.size());
      }
      background->fill(Qt::transparent);
      update();
   }
   else
   {
      createBackground(backgroundVerSpace, backgroundHorSpace);
   }
}

//f+//////////////////////////////////////////////////////////////////////////
//f-//////////////////////////////////////////////////////////////////////////
void gradientWidgetQT::addArrow(const QColor &color, qreal position, bool focus)
{
   GradientArrow arrow;
   arrow.setHasFocus(focus);
   arrow.setColor(color);
   arrow.setPosition(position);

   if(arrow.hasFocus())
   {
      QMutableHashIterator<int, GradientArrow> it(m_arrows);
      while(it.hasNext())
      {
         it.next();
         GradientArrow anArrow = it.value();
         anArrow.setHasFocus(false);
         it.setValue(anArrow);
      }
   }

   int index = 0;
   while(m_arrows.contains(index))
   {
      index++;
   }
   arrow.setIndex(index);
   m_arrows.insert(arrow.index(), arrow);
   update();

   emit arrowSelectedSignal(arrow.position());
}


void gradientWidgetQT::deleteFocusArrow()
{
   if(m_arrows.size() <= 2 )
   {
      return;
   }
   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   while(it.hasNext())
   {
      it.next();
      GradientArrow arrow = it.value();
      if(arrow.hasFocus())
      {
         it.remove();
         break;
      }
   }
   it.toBack();
   if(it.hasPrevious())
   {
      it.previous();
      GradientArrow newFocus = it.value();
      newFocus.setHasFocus(true);
      it.setValue(newFocus);
      emit arrowSelectedSignal(newFocus.position());
   }
   update();
}

void gradientWidgetQT::reset()
{
       m_arrows.clear();
       this->blockSignals(true);
       this->addArrow(Qt::green, 0.0);
       this->addArrow(Qt::yellow, 0.5);
       this->blockSignals(false);
       this->addArrow(Qt::red, 1.0);
}

//f+//////////////////////////////////////////////////////////////////////////
//f-//////////////////////////////////////////////////////////////////////////
void gradientWidgetQT::setFocusColor(const QColor &col)
{
   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   while(it.hasNext())
   {
      it.next();
      GradientArrow anArrow = it.value();
      if(anArrow.hasFocus())
      {
         anArrow.setColor(col);
         it.setValue(anArrow);
      }
   }
}

const QColor gradientWidgetQT::getFocusColor()
{
   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   while(it.hasNext())
   {
      it.next();
      GradientArrow anArrow = it.value();
      if(anArrow.hasFocus())
      {
         return anArrow.color();
      }
   }

   return Qt::white;
}

void gradientWidgetQT::setFocusPosition(const qreal newPos)
{
   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   while(it.hasNext())
   {
      it.next();
      GradientArrow anArrow = it.value();
      if(anArrow.hasFocus())
      {
         anArrow.setPosition(newPos);
         it.setValue(anArrow);
         update();
      }
   }
}

qreal gradientWidgetQT::getFocusPosition()
{
   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   while(it.hasNext())
   {
      it.next();
      GradientArrow anArrow = it.value();
      if(anArrow.hasFocus())
      {
         return anArrow.position();
      }
   }

   return qreal(0);
}
void gradientWidgetQT::newFocusColor(const QColor &col, int index)
{

   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   while(it.hasNext())
   {
      it.next();
      GradientArrow arrow = it.value();
      int key = it.key();
      if(key == index)
      {
         arrow.setColor(col);
         arrow.setHasFocus(true);
         update();
      }
      else
      {
         arrow.setHasFocus(false);
      }
      it.setValue(arrow);
   }

}


//f+//////////////////////////////////////////////////////////////////////////
//f-//////////////////////////////////////////////////////////////////////////
void gradientWidgetQT::paintEvent(QPaintEvent *)
{
   if(viewRect.size().isNull()  || 
      viewRect.size().isEmpty() ||
      viewRect.topLeft() == viewRect.bottomRight())
   {
      viewRect = QRect(QPoint(20,0), QPoint(this->width() - 30, this->height()/3*2 - 15));
      viewRect.translate(5,5);
      createBackground();
   }
   QPainter painter(this);
   if(NULL != background)
   {
      painter.drawPixmap(viewRect, *background, viewRect);
   }
   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
   painter.setRenderHint(QPainter::Antialiasing);

   QPoint gradStart = QPoint(viewRect.topLeft ().x() ,viewRect.bottomLeft ().y()/2);
   QPoint gradStop  = QPoint(viewRect.topRight().x(), viewRect.bottomRight().y()/2);
   QLinearGradient grad(gradStart, gradStop);

   QPen lastPen = painter.pen();
   QHashIterator<int, GradientArrow> it(m_arrows);
   while(it.hasNext())
   {
      it.next();
      GradientArrow m_arrow = it.value();
      grad.setColorAt(m_arrow.position(), m_arrow.color());
      QPolygon arrowPolygon = m_arrow.area();
      arrowPolygon.translate(m_arrow.position() * (this->width() - 50 ) +20, this->height()/3*2 - 8);
      QPainterPath paintPath;
      paintPath.addPolygon(arrowPolygon);
      painter.setBrush(QBrush(m_arrow.color()));
      painter.drawPath(paintPath);
      painter.setBrush(QBrush(Qt::NoBrush));

      if(m_arrow.hasFocus())
      {
         QPen boldPen;
         boldPen.setWidth(3);
         painter.setPen(boldPen);
         painter.drawPolygon(arrowPolygon);
         painter.setPen(lastPen);
      }
      else
      {
         QPen boldPen;
         boldPen.setWidth(1);
         painter.setPen(boldPen);
         painter.drawPolygon(arrowPolygon);
         painter.setPen(lastPen);
      }
   }

   QBrush brush(grad);
   painter.fillRect(viewRect, brush);
   painter.drawRect(viewRect);
   painter.end();
}

void gradientWidgetQT::mousePressEvent(QMouseEvent *Qevent)
{
   bool arrowSelected = false;

   dragStart = Qevent->pos();
   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   it.toBack();
    while(it.hasPrevious())
    {
        it.previous();
        GradientArrow arrow = it.value();
        QPolygon poly = arrow.area();
        poly.translate(arrow.position() * (this->width() - 50) +20, this->height()/3*2 -8);

        if(poly.containsPoint(dragStart, Qt::OddEvenFill))
        {
            arrowMoving = true;
            arrow.setHasFocus(true);
            this->newFocusColor(arrow.color(), arrow.index());
            arrowSelected = true;
            emit arrowSelectedSignal(arrow.position());
        }
        else
        {
            arrow.setHasFocus(false);
        }
        it.setValue(arrow);

    }
    if (!arrowSelected) emit noSelectionSignal();

    update();
}

void gradientWidgetQT::mouseDoubleClickEvent(QMouseEvent *Qevent)
{
    bool arrowSelected = false;

    dragStart = Qevent->pos();
    QMutableHashIterator<int, GradientArrow> it(m_arrows);
    it.toBack();

     while(it.hasPrevious())
     {
         it.previous();
         GradientArrow arrow = it.value();
         QPolygon poly = arrow.area();
         poly.translate(arrow.position() * (this->width() - 50) +20, this->height()/3*2 -8);

         if(poly.containsPoint(dragStart, Qt::OddEvenFill))
         {
             arrowSelected = true;
         }
         it.setValue(arrow);
     }

     if (arrowSelected) emit arrowDoubleClickedSignal();

}

void gradientWidgetQT::mouseMoveEvent(QMouseEvent *Qevent)
{
   if(!arrowMoving) return;
   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   int count = 0;
   while(it.hasNext())
   {
      it.next();
      GradientArrow arrow = it.value();
      if(arrow.hasFocus())
      {
         count++;
         qreal dPos = (qreal)(Qevent->pos().x() - dragStart.x())/((qreal)this->width() - 40);
         if(arrow.position() + dPos > 1 || arrow.position() + dPos < 0)
         {
            return;
         }
         qreal lastPos = arrow.position();
         arrow.setPosition(lastPos + dPos);
         it.setValue(arrow);
         emit arrowMovedSignal(arrow.position());
      }
   }
   dragStart = Qevent->pos();
   update();
}

void gradientWidgetQT::mouseReleaseEvent(QMouseEvent *)
{
   arrowMoving = false;
}
