#ifndef CSVLOADERQT_H
#define CSVLOADERQT_H

#include <QDialog>
#include <QBoxLayout>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <sstream>

#include <vtkQtTableView.h>
#include <vtkIdTypeArray.h>
#include <vtkTable.h>

namespace Ui {
class CSVloaderQT;
}

class CSVloaderQT : public QDialog
{
    Q_OBJECT
    
public:
    explicit CSVloaderQT(QWidget *Qparent = 0);
    ~CSVloaderQT();

    void displayTable(vtkSmartPointer<vtkTable> table, QDir directory);
    
private slots:
    void on_buttonBox_accepted();

signals:
    void sig_itemsSelected(QFileInfoList fileList);

private:
    vtkSmartPointer<vtkQtTableView> m_tableView;
    vtkSmartPointer<vtkTable> m_table;
    QDir m_directory;

    QHBoxLayout * m_layout;
    Ui::CSVloaderQT *ui;
};

#endif // CSVLOADERQT_H
