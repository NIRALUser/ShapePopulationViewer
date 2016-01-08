#include "customizeColorMapByDirectionDialogQT.h"
#include "ui_customizeColorMapByDirectionDialogQT.h"

customizeColorMapByDirectionDialogQT::customizeColorMapByDirectionDialogQT(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::customizeColorMapByDirectionDialogQT)
{
    ui->setupUi(this);

    m_frameAxis.push_back(ui->frame_plusZAxis);
    m_frameAxis.push_back(ui->frame_minusYAxis);
    m_frameAxis.push_back(ui->frame_plusXAxis);
    m_frameAxis.push_back(ui->frame_minusZAxis);
    m_frameAxis.push_back(ui->frame_plusYAxis);
    m_frameAxis.push_back(ui->frame_minusXAxis);
    m_axisColor.push_back(Qt::blue);
    m_axisColor.push_back(Qt::magenta);
    m_axisColor.push_back(Qt::red);
    m_axisColor.push_back(Qt::yellow);
    m_axisColor.push_back(Qt::green);
    m_axisColor.push_back(Qt::cyan);
    m_sameColor = false;
    m_complementaryColor = true;
    m_backgroundColor[0] = 0.1;
    m_backgroundColor[1] = 0.0;
    m_backgroundColor[2] = 0.3;

}

customizeColorMapByDirectionDialogQT::~customizeColorMapByDirectionDialogQT()
{
    delete ui;
}

void customizeColorMapByDirectionDialogQT::AxisColor()
{
    m_labelActors.clear();
    m_arrowActors.clear();

    double arrowTranslate[6][3] = {{0.0,0.2,0.0},
                                   {-0.15,0.15,0.0},
                                   {-0.2,0.0,0.0},
                                   {0.0,-0.2,0.0},
                                   {0.15,-0.15,0.0},
                                   {0.2,0.0,0.0}};

    double labelTranslate[6][2] = {{215.0,302.0},
                                   {110.0,270.0},
                                   {70.0,165.0},
                                   {220.0,35.0},
                                   {315.0,67.0},
                                   {380.0,167.0}};

    const char* label[6] = {"Z Superior","-Y Posterior","X Right","-Z Inferior","Y Anterior","-X Left"};

    for(int i = 0; i < 6; i++)
    {
        /// Create an arrows
        vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
        arrow->Update();

        //translate the sphere
        vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();

        translation->Translate(arrowTranslate[i]);
        if(i < 3) translation->RotateZ(i*45 + 90);
        else translation->RotateZ(i*45 + 135);
        translation->Scale(0.35, 0.35, 0.35);

        vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
#if (VTK_MAJOR_VERSION < 6)
        transformFilter->SetInputConnection(arrow->GetProducerPort());
#else
        transformFilter->SetInputData(arrow->GetOutput());
#endif
        transformFilter->SetTransform(translation);
        transformFilter->Update();

        //mapper of the sphere
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#if (VTK_MAJOR_VERSION < 6)
        mapper->SetInputConnection(transformFilter->GetOutputPort());
#else
        mapper->SetInputData(transformFilter->GetOutput());
#endif

        // color of axis and label
        double colorAxis[3];
        colorAxis[0] = (double)m_axisColor[i].red()/255;
        colorAxis[1] = (double)m_axisColor[i].green()/255;
        colorAxis[2] = (double)m_axisColor[i].blue()/255;


        // Create a actor
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->SetColor( colorAxis );
        m_arrowActors.push_back(actor);

        // label
        vtkSmartPointer<vtkTextActor> labelActor = vtkSmartPointer<vtkTextActor>::New();
        labelActor->GetTextProperty()->SetFontSize ( 14 );
        labelActor->GetTextProperty()->BoldOn();
        labelActor->SetInput ( label[i] );
        labelActor->GetTextProperty()->SetColor ( colorAxis );
        labelActor->SetPosition(labelTranslate[i]);
        m_labelActors.push_back(labelActor);


        // Initialization of the 6 palettes
        QPalette pal;
        pal.setColor(QPalette::Background, m_axisColor[i]);
        m_frameAxis[i]->setPalette( pal );

    }


    if(m_sameColor == true) ui->radioButton_sameColor->click();
    if(m_complementaryColor == true) ui->radioButton_complementaryColor->click();

    /// VISUALIZATION
    vtkRenderer * renderer = vtkRenderer::New();
    vtkRenderWindow *renderWindow = vtkRenderWindow::New();
    renderWindow->AddRenderer(renderer);
    vtkRenderWindowInteractor *interactor = vtkRenderWindowInteractor::New();
    interactor->SetRenderWindow(renderWindow);
    vtkInteractorStyleTrackballCamera * interactorStyle = vtkInteractorStyleTrackballCamera::New();
    interactor->SetInteractorStyle(interactorStyle);

    // Add the actors to the renderer
    for(int i = 0 ; i < 6; i++)
    {
        renderer->AddActor(m_arrowActors[i]);
        renderer->AddActor2D(m_labelActors[i]);
    }

    // Set the background and size
    renderer->SetBackground(m_backgroundColor);
    renderer->ResetCamera();

    widget = new QVTKWidget(ui->widget_AxisColor);
    widget->resize(500,350);
    widget->SetRenderWindow(renderWindow);
//    interactor->EnableRenderOff();

    widget->show();
    interactor->Start();

}

void customizeColorMapByDirectionDialogQT::on_radioButton_sameColor_toggled(bool checked)
{
    if(checked)
    {
        m_sameColor = true;
        m_complementaryColor = false;


        double axisColor[3];
        for(int i = 0; i < 6; i = i + 2)
        {
            int l = ( i + 3) % 6;
            QPalette pal;
            pal.setColor(QPalette::Background, m_axisColor[i]);
            m_frameAxis[l]->setPalette( pal );

            m_axisColor[l] = m_axisColor[i];

            axisColor[0] = (double)m_axisColor[i].red()/255.0;
            axisColor[1] = (double)m_axisColor[i].green()/255.0;
            axisColor[2] = (double)m_axisColor[i].blue()/255.0;

            m_arrowActors[l]->GetProperty()->SetColor( axisColor );
            m_labelActors[l]->GetTextProperty()->SetColor( axisColor );
        }
    }
    widget->GetRenderWindow()->Render();

    emit sig_sameColor_valueChanged(checked);

}

void customizeColorMapByDirectionDialogQT::on_radioButton_complementaryColor_toggled(bool checked)
{
    if(checked)
    {
        m_sameColor = false;
        m_complementaryColor = true;

        QColor color;
        double axisColor[3];
        for(int i = 0; i < 6; i = i + 2)
        {
            int l = ( i + 3) % 6;

            color = m_frameAxis[i]->palette().color(QPalette::Background);

            int r;
            r = 255 - color.red();
            int g;
            g = 255 - color.green();
            int b;
            b = 255 - color.blue();

            m_axisColor[l].setRed(r);
            m_axisColor[l].setGreen(g);
            m_axisColor[l].setBlue(b);

            color.setRed(r);
            color.setBlue(b);
            color.setGreen(g);

            QPalette pal;
            pal.setColor(QPalette::Background, color);
            m_frameAxis[l]->setPalette( pal );

            axisColor[0] = (double)color.red()/255.0;
            axisColor[1] = (double)color.green()/255.0;
            axisColor[2] = (double)color.blue()/255.0;

            m_arrowActors[l]->GetProperty()->SetColor( axisColor );
            m_labelActors[l]->GetTextProperty()->SetColor( axisColor );
        }
    }
    widget->GetRenderWindow()->Render();

    emit sig_complementaryColor_valueChanged(checked);

}

void customizeColorMapByDirectionDialogQT::on_pushButton_pick_plusXAxis_clicked()
{
    QColor color;
    color = QColorDialog::getColor(ui->frame_plusXAxis->palette().color(QPalette::Background));
    if (color.isValid())
    {
        QPalette pal = ui->frame_plusXAxis->palette();
        pal.setColor(QPalette::Background, color);
        ui->frame_plusXAxis->setPalette( pal );

        m_axisColor[2].setRed(color.red());
        m_axisColor[2].setGreen(color.green());
        m_axisColor[2].setBlue(color.blue());

        if(m_complementaryColor)
        {
            m_axisColor[5].setRed(255 - color.red());
            m_axisColor[5].setGreen(255 - color.green());
            m_axisColor[5].setBlue(255 - color.blue());
        }
        else
        {
            m_axisColor[5].setRed(color.red());
            m_axisColor[5].setGreen(color.green());
            m_axisColor[5].setBlue(color.blue());
        }

        pal.setColor(QPalette::Background, m_axisColor[5]);
        ui->frame_minusXAxis->setPalette( pal );

        double plusXcolor[3];
        double minusXcolor[3];

        plusXcolor[0] = (double)color.red()/255.0;
        plusXcolor[1] = (double)color.green()/255.0;
        plusXcolor[2] = (double)color.blue()/255.0;

        for(int j = 0; j < 3; j++)
        {
            if(m_complementaryColor)
            {
                minusXcolor[j] = 1 - plusXcolor[j];

            }
            else
            {
                minusXcolor[j] = plusXcolor[j];
            }
        }

        m_arrowActors[2]->GetProperty()->SetColor( plusXcolor );
        m_arrowActors[5]->GetProperty()->SetColor( minusXcolor );
        m_labelActors[2]->GetTextProperty()->SetColor( plusXcolor );
        m_labelActors[5]->GetTextProperty()->SetColor( minusXcolor );

        widget->GetRenderWindow()->Render();

        emit sig_plusXAxis_valueChanged(color);
    }
}

void customizeColorMapByDirectionDialogQT::on_pushButton_pick_plusYAxis_clicked()
{
    QColor color;
    color = QColorDialog::getColor(ui->frame_plusYAxis->palette().color(QPalette::Background));
    if (color.isValid())
    {
        QPalette pal = ui->frame_plusYAxis->palette();
        pal.setColor(QPalette::Background, color);
        ui->frame_plusYAxis->setPalette( pal );

        m_axisColor[4].setRed(color.red());
        m_axisColor[4].setGreen(color.green());
        m_axisColor[4].setBlue(color.blue());

        if(m_complementaryColor)
        {
            m_axisColor[1].setRed(255 - color.red());
            m_axisColor[1].setGreen(255 - color.green());
            m_axisColor[1].setBlue(255 - color.blue());
        }
        else
        {
            m_axisColor[1].setRed(color.red());
            m_axisColor[1].setGreen(color.green());
            m_axisColor[1].setBlue(color.blue());
        }

        pal.setColor(QPalette::Background, m_axisColor[1]);
        ui->frame_minusYAxis->setPalette( pal );

        double plusXcolor[3];
        double minusXcolor[3];

        plusXcolor[0] = (double)color.red()/255.0;
        plusXcolor[1] = (double)color.green()/255.0;
        plusXcolor[2] = (double)color.blue()/255.0;

        for(int j = 0; j < 3; j++)
        {
            if(m_complementaryColor)
            {
                minusXcolor[j] = 1 - plusXcolor[j];

            }
            else
            {
                minusXcolor[j] = plusXcolor[j];
            }
        }

        m_arrowActors[4]->GetProperty()->SetColor( plusXcolor );
        m_arrowActors[1]->GetProperty()->SetColor( minusXcolor );
        m_labelActors[4]->GetTextProperty()->SetColor( plusXcolor );
        m_labelActors[1]->GetTextProperty()->SetColor( minusXcolor );

        widget->GetRenderWindow()->Render();

        emit sig_plusYAxis_valueChanged(color);
    }
}

void customizeColorMapByDirectionDialogQT::on_pushButton_pick_plusZAxis_clicked()
{
    QColor color;
    color = QColorDialog::getColor(ui->frame_plusZAxis->palette().color(QPalette::Background));
    if (color.isValid())
    {
        QPalette pal = ui->frame_plusZAxis->palette();
        pal.setColor(QPalette::Background, color);
        ui->frame_plusZAxis->setPalette( pal );

        m_axisColor[0].setRed(color.red());
        m_axisColor[0].setGreen(color.green());
        m_axisColor[0].setBlue(color.blue());

        if(m_complementaryColor)
        {
            m_axisColor[3].setRed(255 - color.red());
            m_axisColor[3].setGreen(255 - color.green());
            m_axisColor[3].setBlue(255 - color.blue());
        }
        else
        {
            m_axisColor[3].setRed(color.red());
            m_axisColor[3].setGreen(color.green());
            m_axisColor[3].setBlue(color.blue());
        }

        pal.setColor(QPalette::Background, m_axisColor[3]);
        ui->frame_minusZAxis->setPalette( pal );

        double plusXcolor[3];
        double minusXcolor[3];

        plusXcolor[0] = (double)color.red()/255.0;
        plusXcolor[1] = (double)color.green()/255.0;
        plusXcolor[2] = (double)color.blue()/255.0;

        for(int j = 0; j < 3; j++)
        {
            if(m_complementaryColor)
            {
                minusXcolor[j] = 1 - plusXcolor[j];

            }
            else
            {
                minusXcolor[j] = plusXcolor[j];
            }
        }

        m_arrowActors[0]->GetProperty()->SetColor( plusXcolor );
        m_arrowActors[3]->GetProperty()->SetColor( minusXcolor );
        m_labelActors[0]->GetTextProperty()->SetColor( plusXcolor );
        m_labelActors[3]->GetTextProperty()->SetColor( minusXcolor );

        widget->GetRenderWindow()->Render();

        emit sig_plusZAxis_valueChanged(color);
    }
}

void customizeColorMapByDirectionDialogQT::on_pushButton_default_clicked()
{
    ui->radioButton_complementaryColor->click();

    QPalette pal;

    m_axisColor[0] = Qt::blue;
    m_axisColor[1] = Qt::magenta;
    m_axisColor[2] = Qt::red;
    m_axisColor[3] = Qt::yellow;
    m_axisColor[4] = Qt::green;
    m_axisColor[5] = Qt::cyan;

    double axisColor[3] = {0.0,1.0,1.0};
    for(int i = 0; i < 6; i++)
    {
        pal.setColor(QPalette::Background, m_axisColor[i]);
        m_frameAxis[i]->setPalette( pal );

        int l[6] = {1,0,2,1,0,2};
        if( (i % 2) == 1 )
        {
            axisColor[l[i]] = 1.0;
        }
        else
        {
            axisColor[l[i]] = 0.0;
        }

        m_arrowActors[i]->GetProperty()->SetColor( axisColor );
        m_labelActors[i]->GetTextProperty()->SetColor ( axisColor );
    }

    emit sig_plusXAxis_valueChanged(m_axisColor[2]);
    emit sig_plusYAxis_valueChanged(m_axisColor[4]);
    emit sig_plusZAxis_valueChanged(m_axisColor[0]);

    widget->GetRenderWindow()->Render();
}

void customizeColorMapByDirectionDialogQT::updateAxisColor_value(axisColorStruct* axisColor, bool dialogOpen)
{
    m_axisColor[2].setRed(axisColor->XAxiscolor[0]);
    m_axisColor[2].setGreen(axisColor->XAxiscolor[1]);
    m_axisColor[2].setBlue(axisColor->XAxiscolor[2]);

    m_axisColor[4].setRed(axisColor->YAxiscolor[0]);
    m_axisColor[4].setGreen(axisColor->YAxiscolor[1]);
    m_axisColor[4].setBlue(axisColor->YAxiscolor[2]);

    m_axisColor[0].setRed(axisColor->ZAxiscolor[0]);
    m_axisColor[0].setGreen(axisColor->ZAxiscolor[1]);
    m_axisColor[0].setBlue(axisColor->ZAxiscolor[2]);

    for(int i = 0; i < 6; i = i + 2)
    {
        if(!dialogOpen)
        {
            m_sameColor = axisColor->sameColor;
            m_complementaryColor = axisColor->complementaryColor;
        }

        int l = ( i + 3) % 6;
        if(m_complementaryColor)
        {
            m_axisColor[l].setRed(255 - m_axisColor[i].red() );
            m_axisColor[l].setGreen(255- m_axisColor[i].green());
            m_axisColor[l].setBlue(255 - m_axisColor[i].blue());
        }
        else
        {
            m_axisColor[l].setRed(m_axisColor[i].red() );
            m_axisColor[l].setGreen(m_axisColor[i].green());
            m_axisColor[l].setBlue(m_axisColor[i].blue());
        }

        if(dialogOpen)
        {
            // color of palette
            QPalette pal;
            pal.setColor(QPalette::Background, m_axisColor[i]);
            m_frameAxis[i]->setPalette( pal );

            pal.setColor(QPalette::Background, m_axisColor[l]);
            m_frameAxis[l]->setPalette( pal );

            // color of axis
            double plusXcolor[3];
            double minusXcolor[3];

            plusXcolor[0] = (double)m_axisColor[i].red()/255.0;
            plusXcolor[1] = (double)m_axisColor[i].green()/255.0;
            plusXcolor[2] = (double)m_axisColor[i].blue()/255.0;

            if(axisColor->complementaryColor == m_complementaryColor || axisColor->sameColor == m_sameColor)
            {
                for(int j = 0; j < 3; j++)
                {

                    if(m_complementaryColor)
                    {
                        minusXcolor[j] = 1 - plusXcolor[j];

                    }
                    else
                    {
                        minusXcolor[j] = plusXcolor[j];
                    }
                }
                m_arrowActors[ l ]->GetProperty()->SetColor( minusXcolor );
                m_labelActors[ l ]->GetTextProperty()->SetColor( minusXcolor );
            }
            m_arrowActors[i]->GetProperty()->SetColor( plusXcolor );
            m_labelActors[i]->GetTextProperty()->SetColor( plusXcolor );
        }
    }

    if(dialogOpen)
    {
        if(axisColor->sameColor != m_sameColor) ui->radioButton_sameColor->click();
        if(axisColor->complementaryColor != m_complementaryColor) ui->radioButton_complementaryColor->click();

        widget->GetRenderWindow()->Render();
    }

}

void customizeColorMapByDirectionDialogQT::updateBackgroundColor_valueChanged(double backgroundColor_red, double backgroundColor_green, double backgroundColor_blue, bool dialogOpen)
{

    m_backgroundColor[0] = backgroundColor_red;
    m_backgroundColor[1] = backgroundColor_green;
    m_backgroundColor[2] = backgroundColor_blue;


    if(dialogOpen)
    {
        widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetBackground(m_backgroundColor);
        widget->GetRenderWindow()->Render();
    }

}

void customizeColorMapByDirectionDialogQT::resetColor()
{
    m_axisColor[0] = Qt::blue;
    m_axisColor[1] = Qt::magenta;
    m_axisColor[2] = Qt::red;
    m_axisColor[3] = Qt::yellow;
    m_axisColor[4] = Qt::green;
    m_axisColor[5] = Qt::cyan;
}
