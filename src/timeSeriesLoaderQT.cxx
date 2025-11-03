#include "timeSeriesLoaderQT.h"
#include "ui_timeSeriesLoaderQT.h"

timeSeriesLoaderQT::timeSeriesLoaderQT(QWidget *Qparent) :
    QDialog(Qparent),
    ui(new Ui::timeSeriesLoaderQT)
{
    ui->setupUi(this);
    m_layout = new QHBoxLayout(ui->tableWidget);
}

timeSeriesLoaderQT::~timeSeriesLoaderQT()
{
    delete ui;
    delete m_layout;
}


void timeSeriesLoaderQT::displayTable(vtkSmartPointer<vtkTable> table, QDir directory)
{
    m_table = table;
    m_directory = directory;

    //Translate to QT
    vtkNew<vtkQtTableView> tableView;
    tableView->AddRepresentationFromInput(m_table);
    tableView->SetSelectionBehavior(1);                 //to select rows
    tableView->SetSortingEnabled(false);                //to select vertical headers therefore columns
    //tableView->SetSplitMultiComponentColumns(true);   //to tell Bowser to let princess Peach go
    tableView->Update();
    m_tableView = tableView;

    //Display
    m_layout->addWidget(tableView->GetWidget());
    this->exec();
}

void timeSeriesLoaderQT::on_buttonBox_accepted()
{
    //Get rows and columns
    vtkNew<vtkIdTypeArray> rows;
    m_tableView->GetSelectedItems(rows);
    auto columns = m_table->GetNumberOfColumns();

    //Get selected rows
    QList<QFileInfoList> timeSeries;
    for(int i = 0 ; i < rows->GetNumberOfTuples(); i++)
    {
        auto row = rows->GetTuple1(i);
        QFileInfoList fileList;
        for(int j = 0; j < columns; j++)
        {
            vtkVariant test = m_table->GetValue(row,j);
            QString relativePath = test.ToString().c_str();
            fileList.append(QFileInfo(m_directory,relativePath));
        }
        timeSeries.append(fileList);
    }

    // Control the files format
    for (int i = 0; i < timeSeries.size(); i++)
    {
        for (int j = 0; j < timeSeries.at(i).size(); j++)
        {
            QString QFilePath = timeSeries[i][j].absoluteFilePath();
            if (!QFilePath.endsWith(".vtk") && !QFilePath.endsWith(".vtp") && !QFilePath.endsWith(".xml") && !QFilePath.endsWith(".srep.json"))
            {
                timeSeries[i].removeAt(j);
                j--;
                std::ostringstream strs;
                strs << QFilePath.toStdString() << std::endl
                     << "This is not a vtk/vtp/xml/srep.json file."<< std::endl;
                QMessageBox::critical(this,"Wrong file format",QString(strs.str().c_str()), QMessageBox::Ok);
            }
            else if(!timeSeries[i][j].exists())
            {
                timeSeries[i].removeAt(j);
                j--;
                std::ostringstream strs;
                strs << QFilePath.toStdString() << std::endl
                     << "This file does not exist."<< std::endl;
                QMessageBox::critical(this,"File not found",QString(strs.str().c_str()), QMessageBox::Ok);
            }
        }
    }
    emit sig_timeSeriesSelected(timeSeries);
}
