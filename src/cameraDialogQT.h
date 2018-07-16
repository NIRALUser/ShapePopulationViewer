#ifndef CAMERADIALOGQT_H
#define CAMERADIALOGQT_H

#include <QDialog>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <sstream>

#include "cameraConfigStruct.h"

namespace Ui {
class cameraDialogQT;
}

class cameraDialogQT : public QDialog
{
    Q_OBJECT

public:
    explicit cameraDialogQT(QWidget *Qparent = 0);
    ~cameraDialogQT();

    void loadCameraConfig(QString a_filePath);
    void saveCameraConfig(QString a_filePath);

public slots:
    void updateCameraConfig(cameraConfigStruct cam);

private slots:
    void on_pushButton_load_clicked();
    void on_pushButton_save_clicked();

    void on_position_x_valueChanged(double arg1);
    void on_position_y_valueChanged(double arg1);
    void on_position_z_valueChanged(double arg1);

    void on_focalpoint_x_valueChanged(double arg1);
    void on_focalpoint_y_valueChanged(double arg1);
    void on_focalpoint_z_valueChanged(double arg1);

    void on_viewup_vx_valueChanged(double arg1);
    void on_viewup_vy_valueChanged(double arg1);
    void on_viewup_vz_valueChanged(double arg1);

    void on_scale_valueChanged(double arg1);

signals:
    void sig_position_x_valueChanged(double arg1);
    void sig_position_y_valueChanged(double arg1);
    void sig_position_z_valueChanged(double arg1);

    void sig_focalpoint_x_valueChanged(double arg1);
    void sig_focalpoint_y_valueChanged(double arg1);
    void sig_focalpoint_z_valueChanged(double arg1);

    void sig_viewup_vx_valueChanged(double arg1);
    void sig_viewup_vy_valueChanged(double arg1);
    void sig_viewup_vz_valueChanged(double arg1);

    void sig_scale_valueChanged(double arg1);
    void sig_newCameraConfig(cameraConfigStruct cameraConfig);

private:
    Ui::cameraDialogQT *ui;
    QString m_cameraconfigDirectory;

};

#endif // CAMERADIALOG_H
