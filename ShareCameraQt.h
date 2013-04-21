#ifndef ShareCameraQt_H
#define ShareCameraQt_H

#include <QMainWindow>
#include "ui_ShareCameraQt.h"
#include <QDir>
#include <QVector>
#include "QVTKWidget.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkScalarBarActor.h"
#include <QResizeEvent>
#include <QEvent>
#include <QSize>
#include <QString>
class ShareCameraQt : public QMainWindow, public Ui::ShareCameraQt
{
  Q_OBJECT
public:
  QDir directory;
  QVector<QVTKWidget *> *widgetList;
  QVector<vtkPolyData *> *polyList;
  QVector<vtkPolyDataMapper *> *mapperList;
  QVector<QString> *colorMaps;
  QSize scrollAreaSize;
  QString cmap;
  double* coords;
  double distance;
  int prevCols;
  int prevRows;
  int phi;
  double PI;
  int loaded;
  // Constructor/Destructor
  ShareCameraQt(); 
  ~ShareCameraQt() {};
private:
  bool synced;
  void updateWidgets();
  void updateCMaps();
public slots:

  virtual void slotExit();

protected:
   void ModifiedHandler();
   void resizeEvent(QResizeEvent* event);
private slots:
   //void on_pushButton_clicked();
   void on_checkBox_9_toggled(bool checked);
   void flipMeshes();
   void writeMeshes();
   void openVTKS();
   void on_checkBox_10_toggled(bool checked);
   void on_lineEdit_editingFinished();
//   void on_checkBox_7_toggled(bool checked);
   void on_checkBox_3_toggled(bool checked);
   void on_comboBox_currentIndexChanged(const QString &arg1);
   void on_toolButton_clicked();
   void on_toolButton_2_clicked();
   void on_toolButton_3_clicked();
   void on_toolButton_4_clicked();
   void on_toolButton_5_clicked();
   void on_toolButton_6_clicked();
   void viewChange(int x, int y, int z);
};

#endif
