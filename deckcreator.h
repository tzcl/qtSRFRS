#ifndef DECKCREATOR_H
#define DECKCREATOR_H

#include <QDialog>

#include "mainwindow.h"

namespace Ui {
class DeckCreator;
}

class DeckCreator : public QDialog
{
    Q_OBJECT

public:
    explicit DeckCreator(QWidget *parent = 0);
    ~DeckCreator();

private slots:
    void on_buttonBox_accepted();

    void on_txt_name_textEdited(const QString &string);

private:
    Ui::DeckCreator *ui;

    MainWindow* getParent();
};

#endif // DECKCREATOR_H
