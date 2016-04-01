#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"generatehah.h"
#include"compareimage.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Generate_hash_clicked()
{

    GenerateHah generateHash;
    generateHash.setModal(true);

    generateHash.exec();
}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_Compare_clicked()
{
    CompareImage cmpImg;
    cmpImg.setModal(true);

    cmpImg.exec();
}
