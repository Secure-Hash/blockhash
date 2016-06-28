#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <blockhash.h>
#include <gpg.h>
#include <QProgressDialog>
#include <QFutureWatcher>
#include <QtConcurrent>

/* Namepspace for all UI class and objects */
namespace Ui {
class MainWindow;
}

/* Class for MainWindow */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /* Explicit constructor */
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /* Slot for hash generation on clicked image */
    void on_btn_generatehash_clicked();

    /* Slot for reset action of hash generation */
    void on_btn_reset_clicked();

    /* Slot for image browse */
    void on_btn_browse_clicked();

    /* Slot for first hash file browse button */
    void on_btn_browse_2_clicked();

    /* Slot for first hash file browse button */
    void on_btn_browse_3_clicked();

    /* Slot for reset button on hash comparison */
    void on_btn_reset_2_clicked();

    /* Slot for hash comparison */
    void on_btn_comparehash_clicked();

    /* Slot for saving hash */
    void on_btn_save_clicked();

private:
    Ui::MainWindow *ui;
    Blockhash bh;
    Gpg gpg;
    QProgressDialog dialog;
};

#endif // MAINWINDOW_H
