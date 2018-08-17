#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>

namespace Ui {
class InfoDialog;
}

class InfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InfoDialog( QString iconFilePath, QString windowTitle, QString infoText, QWidget *parent = 0 );
    ~InfoDialog();

private:
    Ui::InfoDialog *ui;
};

#endif // INFODIALOG_H
