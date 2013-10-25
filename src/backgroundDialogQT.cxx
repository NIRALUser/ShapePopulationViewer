#include "backgroundDialogQT.h"
#include "ui_backgroundDialogQT.h"


#include <QColorDialog>

backgroundDialogQT::backgroundDialogQT(QWidget *Qparent) :
    QDialog(Qparent),
    ui(new Ui::backgroundDialogQT)
{
    ui->setupUi(this);

    on_pushButton_reset_clicked();
}

backgroundDialogQT::~backgroundDialogQT()
{
    delete ui;
}

void backgroundDialogQT::on_pushButton_pick_selected_clicked()
{
    QColor color;
    color = QColorDialog::getColor(ui->frame_selected->palette().color(QPalette::Background));
    if (color.isValid())
    {
        QPalette pal = ui->frame_selected->palette();
        pal.setColor(QPalette::Background, color);
        ui->frame_selected->setPalette( pal );
        emit sig_selectedColor_valueChanged(color);
    }

}

void backgroundDialogQT::on_pushButton_pick_unselected_clicked()
{
    QColor color;
    color = QColorDialog::getColor(ui->frame_unselected->palette().color(QPalette::Background));
    if (color.isValid())
    {
        QPalette pal = ui->frame_unselected->palette();
        pal.setColor(QPalette::Background, color);
        ui->frame_unselected->setPalette( pal );
        emit sig_unselectedColor_valueChanged(color);
    }
}

void backgroundDialogQT::on_pushButton_pick_text_clicked()
{
    QColor color;
    color = QColorDialog::getColor(ui->frame_text->palette().color(QPalette::Background));
    if (color.isValid())
    {
        QPalette pal = ui->frame_text->palette();
        pal.setColor(QPalette::Background, color);
        ui->frame_text->setPalette( pal );
        emit sig_textColor_valueChanged(color);
    }
}

void backgroundDialogQT::on_pushButton_reset_clicked()
{
    QPalette pal;

    QColor color(255*0.1,0,255*0.3);
    pal.setColor(QPalette::Background, color);
    ui->frame_selected->setPalette( pal );
    emit sig_selectedColor_valueChanged(color);

    pal.setColor(QPalette::Background, Qt::black );
    ui->frame_unselected->setPalette( pal );
    emit sig_unselectedColor_valueChanged(Qt::black);

    pal.setColor(QPalette::Background, Qt::white );
    ui->frame_text->setPalette( pal );
    emit sig_textColor_valueChanged(Qt::white);
}
