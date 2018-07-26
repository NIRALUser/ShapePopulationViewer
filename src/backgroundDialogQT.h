#ifndef BACKGROUNDDIALOGQT_H
#define BACKGROUNDDIALOGQT_H

#include <QDialog>

namespace Ui {
class backgroundDialogQT;
}

class backgroundDialogQT : public QDialog
{
    Q_OBJECT

public:
    explicit backgroundDialogQT(QWidget *Qparent = 0);
    ~backgroundDialogQT();

private slots:
    void on_pushButton_pick_selected_clicked();
    void on_pushButton_pick_unselected_clicked();
    void on_pushButton_pick_text_clicked();

    void on_pushButton_reset_clicked();

signals:
    void sig_selectedColor_valueChanged(QColor color);
    void sig_unselectedColor_valueChanged(QColor color);
    void sig_textColor_valueChanged(QColor color);

private:
    Ui::backgroundDialogQT *ui;
};

#endif // BACKGROUNDDIALOGQT_H
