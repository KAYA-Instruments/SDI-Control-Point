#include "aecweightsdialog.h"
#include "ui_aecweightsdialog.h"

aecweightsdialog::aecweightsdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aecweightsdialog)
{
    ui->setupUi(this);
}

aecweightsdialog::~aecweightsdialog()
{
    delete ui;
}
