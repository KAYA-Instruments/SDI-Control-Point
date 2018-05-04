#ifndef AECWEIGHTSDIALOG_H
#define AECWEIGHTSDIALOG_H

#include <QDialog>

namespace Ui {
class aecweightsdialog;
}

class aecweightsdialog : public QDialog
{
    Q_OBJECT

public:
    explicit aecweightsdialog(QWidget *parent = 0);
    ~aecweightsdialog();

private:
    Ui::aecweightsdialog *ui;
};

#endif // AECWEIGHTSDIALOG_H
