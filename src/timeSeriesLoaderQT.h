#ifndef TIMESERIESLOADERQT_H
#define TIMESERIESLOADERQT_H

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
class timeSeriesLoaderQT;
}

class timeSeriesLoaderQT : public QDialog
{
    Q_OBJECT

public:
    explicit timeSeriesLoaderQT(QWidget *Qparent = 0);
    ~timeSeriesLoaderQT();

    void displayTable(vtkSmartPointer<vtkTable> table, QDir directory);

private slots:
    void on_buttonBox_accepted();

signals:
    void sig_timeSeriesSelected(QList<QFileInfoList> timeSeries);

private:
    vtkSmartPointer<vtkQtTableView> m_tableView;
    vtkSmartPointer<vtkTable> m_table;
    QDir m_directory;

    QHBoxLayout * m_layout;
    Ui::timeSeriesLoaderQT *ui;
};

#endif // TIMESERIESLOADERQT_H
