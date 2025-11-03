#include "CSVloaderQT.h"
#include "ui_CSVloaderQT.h"

CSVloaderQT::CSVloaderQT(QWidget *Qparent) :
    QDialog(Qparent),
    ui(new Ui::CSVloaderQT)
{
    ui->setupUi(this);
    m_layout = new QHBoxLayout(ui->tableWidget);
}

CSVloaderQT::~CSVloaderQT()
{
    delete ui;
    delete m_layout;
}


void CSVloaderQT::displayTable(vtkSmartPointer<vtkTable> table, QDir directory)
{
    m_table = table;
    m_directory = directory;

    //Translate to QT
    vtkSmartPointer<vtkQtTableView> tableView = vtkSmartPointer<vtkQtTableView>::New();
    tableView->AddRepresentationFromInput(m_table);
    tableView->SetSelectionBehavior(0);                 //to select single items
    tableView->SetSortingEnabled(false);                //to select vertical headers therefore columns
    //tableView->SetSplitMultiComponentColumns(true);   //to tell Bowser to let princess Peach go
    tableView->Update();
    m_tableView = tableView;

    //Display
    m_layout->addWidget(tableView->GetWidget());
    this->exec();
}

void CSVloaderQT::on_buttonBox_accepted()
{
    //Get rows and columns
    vtkSmartPointer<vtkIdTypeArray> items = vtkSmartPointer<vtkIdTypeArray>::New();
    m_tableView->GetSelectedItems(items);

    //Get selected Items
    QFileInfoList fileList;
    for(int i = 0 ; i < items->GetNumberOfTuples(); i++)
    {
        double* cell = items->GetTuple(i);
        vtkVariant test = m_table->GetValue(cell[0],cell[1]);
        QString relativePath = test.ToString().c_str();
        fileList.append(QFileInfo(m_directory,relativePath));
    }

    // Control the files format
    for (int i = 0; i < fileList.size(); i++)
    {
        QString QFilePath = fileList[i].absoluteFilePath();
        if (!QFilePath.endsWith(".vtk") && !QFilePath.endsWith(".vtp") && !QFilePath.endsWith(".xml") && !QFilePath.endsWith(".srep.json"))
        {
            fileList.removeAt(i);
            i--;
            std::ostringstream strs;
            strs << QFilePath.toStdString() << std::endl
                 << "This is not a vtk/vtp/xml/srep.json file."<< std::endl;
            QMessageBox::critical(this,"Wrong file format",QString(strs.str().c_str()), QMessageBox::Ok);
        }
        else if(!fileList[i].exists())
        {
            fileList.removeAt(i);
            i--;
            std::ostringstream strs;
            strs << QFilePath.toStdString() << std::endl
                 << "This file does not exist."<< std::endl;
            QMessageBox::critical(this,"File not found",QString(strs.str().c_str()), QMessageBox::Ok);
        }
    }

    emit sig_itemsSelected(fileList);

}
