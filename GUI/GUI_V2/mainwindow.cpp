#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QSlider>
#include<QSpinBox>
#include<QHBoxLayout>
#include<QMessageBox>
#include<QFileDialog>


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


void MainWindow::on_btn_generatehash_clicked()
{
    if(ui->filepath->text()=="")
        QMessageBox::warning(this,("Error Message"),"Kindly browse Image");
    else if(ui->hashsize->currentIndex()==0)
        QMessageBox::warning(this,("Error Message"),"Kindly select Hash Size for Hash Generation");


    //call our function nd pass two has file path
}

void MainWindow::on_btn_reset_clicked()
{
       ui->filepath->setText("");
       ui->labelimage->setText("");
       QPixmap px;
       ui->labelimage->setPixmap(px);
       ui->hashsize->setCurrentIndex(0);
}

void MainWindow::on_btn_browse_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,("Open File"),QDir::homePath(),("Image Files (*.png *.jpg *.bmp)"));
    ui->filepath->setText("");
    //ui->labelimage->setPixmap("");
    QString pic=filename;
    if(pic != ""){
        ui->filepath->setText(pic);
        QPixmap pix(pic);
        int w = ui->labelimage->width();
        int h = ui->labelimage->height();
        ui->labelimage->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    }
    else{
        QMessageBox::information(this,(""),"Please Select File from your computer");
    }
}

void MainWindow::on_btn_browse_2_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,("Open File"),"/home/aditi",("Hash Files (*.asc)"));
    ui->hash1->setText("");
    if(filename != "")
        ui->hash1->setText(filename);
    else
        QMessageBox::information(this,(""),"Please Select File from your computer");
}

void MainWindow::on_btn_browse_3_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,("Open File"),"/home/aditi",("Hash Files (*.asc)"));
    ui->hash2->setText("");
    if(filename != "")
        ui->hash2->setText(filename);
    else
        QMessageBox::information(this,("Error Message"),"Please Select File from your computer");
}

void MainWindow::on_btn_reset_2_clicked()
{
    ui->hash1->setText("");
    ui->hash2->setText("");
    ui->hashsize_2->setCurrentIndex(0);
}

void MainWindow::on_btn_comparehash_clicked()
{
    if(ui->hash1->text()=="")
        QMessageBox::warning(this,("Error Message"),"Hash 1 is Empty");
    else if(ui->hash2->text()=="")
            QMessageBox::warning(this,("Error Message"),"Hash 2 is Empty");
    else if(ui->hashsize->currentIndex()==0)
        QMessageBox::warning(this,("Error Message"),"Kindly select Hash Size for Hash Comparision");
    //else call our function nd pass two has file path
}

void MainWindow::on_btn_save_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
            this,
            ("Save Hash"),
            QDir::homePath(),
            ("Documents (*.txt)"));
    ui->savefilename->setText(filename+".asc");

}
