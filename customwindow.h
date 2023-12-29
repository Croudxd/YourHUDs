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

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::customwindow *ui;
};

#endif // CUSTOMWINDOW_H
