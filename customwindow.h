#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

#include <QWidget>

namespace Ui {
class customwindow;
}

class customwindow : public QWidget
{
    Q_OBJECT

public:
    explicit customwindow(QWidget *parent = nullptr);
    ~customwindow();

private:
    Ui::customwindow *ui;
};

#endif // CUSTOMWINDOW_H
