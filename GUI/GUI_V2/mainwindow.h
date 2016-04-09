#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_btn_generatehash_clicked();

    void on_btn_reset_clicked();

    void on_btn_browse_clicked();

    void on_btn_browse_2_clicked();

    void on_btn_browse_3_clicked();

    void on_btn_reset_2_clicked();

    void on_btn_comparehash_clicked();

    void on_btn_save_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
