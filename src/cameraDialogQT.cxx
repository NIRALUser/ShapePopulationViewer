#include "cameraDialogQT.h"
#include "ui_cameraDialogQT.h"

cameraDialogQT::cameraDialogQT(QWidget *Qparent) :
    QDialog(Qparent),
    ui(new Ui::cameraDialogQT)
{
    ui->setupUi(this);
    m_cameraconfigDirectory = "~";
}

cameraDialogQT::~cameraDialogQT()
{
    delete ui;
}


void cameraDialogQT::updateCameraConfig(cameraConfigStruct cam)
{
    ui->position_x->setValue(cam.pos_x);
    ui->position_y->setValue(cam.pos_y);
    ui->position_z->setValue(cam.pos_z);
    ui->focalpoint_x->setValue(cam.foc_x);
    ui->focalpoint_y->setValue(cam.foc_y);
    ui->focalpoint_z->setValue(cam.foc_z);
    ui->viewup_vx->setValue(cam.view_vx);
    ui->viewup_vy->setValue(cam.view_vy);
    ui->viewup_vz->setValue(cam.view_vz);
    ui->scale->setValue(cam.scale);
}

void cameraDialogQT::on_pushButton_load_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Paraview Camera Configuration file"),m_cameraconfigDirectory,"PVCC file (*.pvcc)");
    if(filename == "") return;

    QFileInfo file(filename);
    m_cameraconfigDirectory= file.path();
    \
    this->loadCameraConfig(filename);
}


void cameraDialogQT::loadCameraConfig(QString a_filePath)
{
    QFile file(a_filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        /* If we can't open it, let's show an error message. */
        QMessageBox::critical(this,"XML Reader error","Couldn't open .pvcc",QMessageBox::Ok);
        return;
    }

    /* QXmlStreamReader takes any QIODevice. */
    QXmlStreamReader xmlReader(&file);
    cameraConfigStruct cameraConfig;

    /* We'll parse the XML until we reach end of it.*/
    while(!xmlReader.atEnd() && !xmlReader.hasError())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xmlReader.readNext();

        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement && xmlReader.name() == "Property")
        {
            if(xmlReader.attributes().value("name").toString() == "CameraPosition")
            {
                xmlReader.readNext();

                while(!(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "Property"))
                {
                    if(xmlReader.name() == "Element")
                    {
                        /* We've found position x. */
                        if(xmlReader.attributes().value("index").toString() == "0")
                        {
                            cameraConfig.pos_x = xmlReader.attributes().value("value").toString().toDouble();
                        }
                        /* We've found position y. */
                        if(xmlReader.attributes().value("index").toString() == "1")
                        {
                            cameraConfig.pos_y = xmlReader.attributes().value("value").toString().toDouble();
                        }
                        /* We've found position z. */
                        if(xmlReader.attributes().value("index").toString() == "2")
                        {
                            cameraConfig.pos_z = xmlReader.attributes().value("value").toString().toDouble();
                        }
                    }
                    /* ...and next... */
                    xmlReader.readNext();
                }
            }
            if(xmlReader.attributes().value("name").toString() == "CameraFocalPoint")
            {
                xmlReader.readNext();

                while(!(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "Property"))
                {
                    if(xmlReader.name() == "Element")
                    {
                        /* We've found focalPoint x. */
                        if(xmlReader.attributes().value("index").toString() == "0")
                        {
                            cameraConfig.foc_x = xmlReader.attributes().value("value").toString().toDouble();
                        }
                        /* We've found focalPoint y. */
                        if(xmlReader.attributes().value("index").toString() == "1")
                        {
                            cameraConfig.foc_y = xmlReader.attributes().value("value").toString().toDouble();
                        }
                        /* We've found focalPoint z. */
                        if(xmlReader.attributes().value("index").toString() == "2")
                        {
                            cameraConfig.foc_z = xmlReader.attributes().value("value").toString().toDouble();
                        }
                    }
                    /* ...and next... */
                    xmlReader.readNext();
                }
            }
            if(xmlReader.attributes().value("name").toString() == "CameraViewUp")
            {
                xmlReader.readNext();

                while(!(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "Property"))
                {
                    if(xmlReader.name() == "Element")
                    {
                        /* We've found CameraViewUp vx. */
                        if(xmlReader.attributes().value("index").toString() == "0")
                        {
                            cameraConfig.view_vx = xmlReader.attributes().value("value").toString().toDouble();
                        }
                        /* We've found CameraViewUp vy. */
                        if(xmlReader.attributes().value("index").toString() == "1")
                        {
                            cameraConfig.view_vy = xmlReader.attributes().value("value").toString().toDouble();
                        }
                        /* We've found CameraViewUp vz. */
                        if(xmlReader.attributes().value("index").toString() == "2")
                        {
                            cameraConfig.view_vz = xmlReader.attributes().value("value").toString().toDouble();
                        }
                    }
                    /* ...and next... */
                    xmlReader.readNext();
                }
            }
            if(xmlReader.attributes().value("name").toString() == "CameraParallelScale")
            {
                xmlReader.readNext();

                while(!(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "Property"))
                {
                    if(xmlReader.name() == "Element")
                    {
                        /* We've found CameraParallelScale. */
                        if(xmlReader.attributes().value("index").toString() == "0")
                        {
                            cameraConfig.scale = xmlReader.attributes().value("value").toString().toDouble();
                        }
                    }
                    /* ...and next... */
                    xmlReader.readNext();
                }
            }
            if(xmlReader.attributes().value("name").toString() == "CameraParallelProjection")
            {
                xmlReader.readNext();

                while(!(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "Property"))
                {
                    if(xmlReader.name() == "Element")
                    {
                        /* We've found CameraParallelScale. */
                        if(xmlReader.attributes().value("index").toString() == "0")
                        {
                            if (xmlReader.attributes().value("value").toString() == "0")
                            {
                                std::ostringstream strs;
                                strs << "This camera configuration uses a non-parallel projection " << std::endl
                                     << "while ShapePopulationViewer camera uses a parallel projection."<< std::endl
                                     << "(Mapping the loaded values to a parallel projection.)";
                                QMessageBox::critical(this,"CAREFUL",QString(strs.str().c_str()), "I Understand");
                            }
                        }
                    }
                    /* ...and next... */
                    xmlReader.readNext();
                }
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
    emit sig_newCameraConfig(cameraConfig);
}

void cameraDialogQT::on_pushButton_save_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save Paraview Camera Configuration file"),m_cameraconfigDirectory,"PVCC file (*.pvcc)");
    if(filename == "") return;

    QFileInfo file(filename);
    m_cameraconfigDirectory= file.path();
    if(file.suffix() != QString("pvcc")) filename += ".pvcc";

    this->saveCameraConfig(filename);
}



void cameraDialogQT::saveCameraConfig(QString a_filePath)
{
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

        /* PVCameraConfiguration */
        xmlWriter->writeStartElement("PVCameraConfiguration");
        xmlWriter->writeAttribute("description", "ParaView camera configuration");
        xmlWriter->writeAttribute("version", "1.0");

            /* Proxy */
            xmlWriter->writeStartElement("Proxy");
            xmlWriter->writeAttribute("group", "views");
            xmlWriter->writeAttribute("type", "RenderView");
            xmlWriter->writeAttribute("id", "269");                 /// ??? You can find 269 in every id later, what is it? see pvcc writer
            xmlWriter->writeAttribute("servers", "21");             /// ???

                /* Property : Camera Position */
                xmlWriter->writeStartElement("Property");
                xmlWriter->writeAttribute("name", "CameraPosition");
                xmlWriter->writeAttribute("id", "269.CameraPosition");
                xmlWriter->writeAttribute("number_of_elements", "3");

                    /* Element X */
                    xmlWriter->writeEmptyElement("Element");
                    xmlWriter->writeAttribute("index", "0");
                    strs.str(""); strs.clear();
                    strs << ui->position_x->value() ;
                    str = strs.str();
                    xmlWriter->writeAttribute("value", QString(str.c_str()));

                    /* Element Y */
                    xmlWriter->writeEmptyElement("Element");
                    xmlWriter->writeAttribute("index", "1");
                    strs.str(""); strs.clear();
                    strs << ui->position_y->value() ;
                    str = strs.str();
                    xmlWriter->writeAttribute("value", QString(str.c_str()));

                    /* Element Z */
                    xmlWriter->writeEmptyElement("Element");
                    xmlWriter->writeAttribute("index", "2");
                    strs.str(""); strs.clear();
                    strs << ui->position_z->value() ;
                    str = strs.str();
                    xmlWriter->writeAttribute("value", QString(str.c_str()));

                xmlWriter->writeEndElement();

                /* Property : Camera Focal Point */
                xmlWriter->writeStartElement("Property");
                xmlWriter->writeAttribute("name", "CameraFocalPoint");
                xmlWriter->writeAttribute("id", "269.CameraFocalPoint");
                xmlWriter->writeAttribute("number_of_elements", "3");

                    /* Element X */
                    xmlWriter->writeEmptyElement("Element");
                    xmlWriter->writeAttribute("index", "0");
                    strs.str(""); strs.clear();
                    strs << ui->focalpoint_x->value() ;
                    str = strs.str();
                    xmlWriter->writeAttribute("value", QString(str.c_str()));

                    /* Element Y */
                    xmlWriter->writeEmptyElement("Element");
                    xmlWriter->writeAttribute("index", "1");
                    strs.str(""); strs.clear();
                    strs << ui->focalpoint_y->value() ;
                    str = strs.str();
                    xmlWriter->writeAttribute("value", QString(str.c_str()));

                    /* Element Z */
                    xmlWriter->writeEmptyElement("Element");
                    xmlWriter->writeAttribute("index", "2");
                    strs.str(""); strs.clear();
                    strs << ui->focalpoint_z->value() ;
                    str = strs.str();
                    xmlWriter->writeAttribute("value", QString(str.c_str()));

                xmlWriter->writeEndElement();

                /* Property : Camera View Up */
                xmlWriter->writeStartElement("Property");
                xmlWriter->writeAttribute("name", "CameraViewUp");
                xmlWriter->writeAttribute("id", "269.CameraViewUp");
                xmlWriter->writeAttribute("number_of_elements", "3");

                    /* Element VX */
                    xmlWriter->writeEmptyElement("Element");
                    xmlWriter->writeAttribute("index", "0");
                    strs.str(""); strs.clear();
                    strs << ui->viewup_vx->value() ;
                    str = strs.str();
                    xmlWriter->writeAttribute("value", QString(str.c_str()));

                    /* Element VY */
                    xmlWriter->writeEmptyElement("Element");
                    xmlWriter->writeAttribute("index", "1");
                    strs.str(""); strs.clear();
                    strs << ui->viewup_vy->value() ;
                    str = strs.str();
                    xmlWriter->writeAttribute("value", QString(str.c_str()));

                    /* Element VZ */
                    xmlWriter->writeEmptyElement("Element");
                    xmlWriter->writeAttribute("index", "2");
                    strs.str(""); strs.clear();
                    strs << ui->viewup_vz->value() ;
                    str = strs.str();
                    xmlWriter->writeAttribute("value", QString(str.c_str()));

                xmlWriter->writeEndElement();

                /* Property : Center Of Rotation */
                xmlWriter->writeStartElement("Property");
                xmlWriter->writeAttribute("name", "CenterOfRotation");
                xmlWriter->writeAttribute("id", "269.CenterOfRotation");
                xmlWriter->writeAttribute("number_of_elements", "3");

                    /* Element X */
                    xmlWriter->writeEmptyElement("Element");
                    xmlWriter->writeAttribute("index", "0");
                    strs.str(""); strs.clear();
                    strs << ui->focalpoint_x->value() ;
                    str = strs.str();
                    xmlWriter->writeAttribute("value", QString(str.c_str()));

                    /* Element Y */
                    xmlWriter->writeEmptyElement("Element");
                    xmlWriter->writeAttribute("index", "1");
                    strs.str(""); strs.clear();
                    strs << ui->focalpoint_y->value() ;
                    str = strs.str();
                    xmlWriter->writeAttribute("value", QString(str.c_str()));

                    /* Element Z */
                    xmlWriter->writeEmptyElement("Element");
                    xmlWriter->writeAttribute("index", "2");
                    strs.str(""); strs.clear();
                    strs << ui->focalpoint_z->value() ;
                    str = strs.str();
                    xmlWriter->writeAttribute("value", QString(str.c_str()));

                xmlWriter->writeEndElement();

                /* Property : Camera View Angle */
                xmlWriter->writeStartElement("Property");
                xmlWriter->writeAttribute("name", "CameraViewAngle");
                xmlWriter->writeAttribute("id", "269.CameraViewAngle");
                xmlWriter->writeAttribute("number_of_elements", "1");

                    /* Element */
                    xmlWriter->writeEmptyElement("Element");
                    xmlWriter->writeAttribute("index", "0");
                    xmlWriter->writeAttribute("value", "30");               //view angle forced since we use only parallel projection

                xmlWriter->writeEndElement();

                /* Property : Camera Parallel Scale */
                xmlWriter->writeStartElement("Property");
                xmlWriter->writeAttribute("name", "CameraParallelScale");
                xmlWriter->writeAttribute("id", "269.CameraParallelScale");
                xmlWriter->writeAttribute("number_of_elements", "1");

                    /* Element */
                    xmlWriter->writeEmptyElement("Element");
                    xmlWriter->writeAttribute("index", "0");
                    strs.str(""); strs.clear();
                    strs << ui->scale->value() ;
                    str = strs.str();
                    xmlWriter->writeAttribute("value", QString(str.c_str()));

                xmlWriter->writeEndElement();

                /* Property : Camera Parallel Projection */
                xmlWriter->writeStartElement("Property");
                xmlWriter->writeAttribute("name", "CameraParallelProjection");
                xmlWriter->writeAttribute("id", "269.CameraParallelProjection");
                xmlWriter->writeAttribute("number_of_elements", "1");

                    /* Element */
                    xmlWriter->writeEmptyElement("Element");
                    xmlWriter->writeAttribute("index", "0");
                    xmlWriter->writeAttribute("value", "1");                /// ???

                    /* Domain */                                            /// ???
                    xmlWriter->writeEmptyElement("Domain");
                    xmlWriter->writeAttribute("name", "bool");
                    xmlWriter->writeAttribute("id", "269.CameraParallelProjection");

                xmlWriter->writeEndElement();
            xmlWriter->writeEndElement();
        xmlWriter->writeEndElement();
        xmlWriter->writeEndDocument();
        delete xmlWriter;
    }
}

void cameraDialogQT::on_position_x_valueChanged(double arg1)
{
    emit sig_position_x_valueChanged(arg1);
}
void cameraDialogQT::on_position_y_valueChanged(double arg1)
{
    emit sig_position_y_valueChanged(arg1);
}
void cameraDialogQT::on_position_z_valueChanged(double arg1)
{
    emit sig_position_z_valueChanged(arg1);
}
void cameraDialogQT::on_focalpoint_x_valueChanged(double arg1)
{
    emit sig_focalpoint_x_valueChanged(arg1);
}
void cameraDialogQT::on_focalpoint_y_valueChanged(double arg1)
{
    emit sig_focalpoint_y_valueChanged(arg1);
}
void cameraDialogQT::on_focalpoint_z_valueChanged(double arg1)
{
    emit sig_focalpoint_z_valueChanged(arg1);
}
void cameraDialogQT::on_viewup_vx_valueChanged(double arg1)
{
    emit sig_viewup_vx_valueChanged(arg1);
}
void cameraDialogQT::on_viewup_vy_valueChanged(double arg1)
{
    emit sig_viewup_vy_valueChanged(arg1);
}
void cameraDialogQT::on_viewup_vz_valueChanged(double arg1)
{
    emit sig_viewup_vz_valueChanged(arg1);
}
void cameraDialogQT::on_scale_valueChanged(double arg1)
{
    emit sig_scale_valueChanged(arg1);
}
