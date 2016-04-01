#ifndef COMPAREIMAGE_H
#define COMPAREIMAGE_H
#include <QDialog>

namespace Ui {
class CompareImage;
}

class CompareImage : public QDialog
{
    Q_OBJECT

public:
    explicit CompareImage(QWidget *parent = 0);
    ~CompareImage();

private slots:
    void on_cmp_clicked();

private:
    Ui::CompareImage *ui;
};

#endif // COMPAREIMAGE_H
