#include "infodialog.h"
#include "ui_infodialog.h"

InfoDialog::InfoDialog( QString iconFilePath, QString infoText , QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::InfoDialog )
{
    ui->setupUi( this );

    // Show without frame
    this->setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint | Qt::MSWindowsFixedSizeDialogHint );

    // Set Icon and text
    this->setWindowIcon( QIcon(iconFilePath) );
    this->ui->lblImage->setPixmap( QPixmap(iconFilePath) );
    this->ui->lblInfo->setText(infoText);
}

InfoDialog::~InfoDialog()
{
    delete ui;
}
