
#include "ShapePopulationColorMapIO.h"

// Qt includes
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QWidget>

// STD includes
#include <sstream>

namespace ShapePopulationColorMapIO
{

bool getXmlReaderValue(QXmlStreamReader * a_xmlReader, double * a_colorPointValue)
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
        QMessageBox::critical(0,"XML Reader error",QString(str.c_str()),QMessageBox::Ok);
        return false;
    }

    *a_colorPointValue = value;
    return true;
}

void loadColorPointList(QString a_filePath, std::vector<colorPointStruct> * a_colorPointList, QWidget* parent)
{
    QFile file(a_filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        /* If we can't open it, let's show an error message. */
        QMessageBox::critical(parent,"XML Reader error","Couldn't open .spvcm",QMessageBox::Ok);
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
        QMessageBox::critical(parent,"XML Reader error",xmlReader.errorString(), QMessageBox::Ok);
        return;
    }

    /* Removes any device() or data from the reader and resets its internal state to the initial state. */
    xmlReader.clear();

    /* Load the colormap */
    *a_colorPointList = colorPointList;
}

void saveColorPointList(QString a_filePath, const std::vector<colorPointStruct>& colorPointList, QWidget* parent)
{
    //write XML File
    QFile file(a_filePath);
    QFileInfo fileInfo(a_filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        /* show error message if not able to open file */
        QMessageBox::warning(parent, "Read only", "The file is in read only mode");
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

}
