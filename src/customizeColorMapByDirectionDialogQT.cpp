#include "customizeColorMapByDirectionDialogQT.h"
#include "ui_customizeColorMapByDirectionDialogQT.h"

customizeColorMapByDirectionDialogQT::customizeColorMapByDirectionDialogQT(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::customizeColorMapByDirectionDialogQT)
{
    ui->setupUi(this);
}

customizeColorMapByDirectionDialogQT::~customizeColorMapByDirectionDialogQT()
{
    delete ui;
}
