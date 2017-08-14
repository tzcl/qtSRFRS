#ifndef DECKCREATOR_H
#define DECKCREATOR_H

#include <QDialog>

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

private:
    Ui::DeckCreator *ui;
};

#endif // DECKCREATOR_H
