#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <sstream>

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
        QMessageBox::warning(this,("Error Message"),"Kindly select hash size");
    else
    {
        QMessageBox::information(this,("In Progress"),"This may take few minutes.\nPress Ok to continue");
        int hashsize = (int)sqrt(atof(ui->hashsize->currentText().toStdString().c_str()));

        int res = bh.process_image(ui->filepath->text().toStdString(),hashsize);
        if(res!=0){
            QMessageBox::critical(this,("Has generation failed"),QString::fromStdString(bh.get_err()));
            return;
        }
        bool result = gpg.generate(CHASH_FILE,ui->savefilename->text().toStdString());
        if(result)
        {
            QMessageBox::information(this,("Signature generated"),"Image hash generated successfully");
            on_btn_reset_clicked();
        }
        else{
            QMessageBox::critical(this,("Signature generation failed"),QString::fromStdString(gpg.get_err()));
        }

    }

    //call our function and pass two has file path
}

void MainWindow::on_btn_reset_clicked()
{
       ui->filepath->setText("");
       ui->labelimage->setText("");
       QPixmap px;
       ui->labelimage->setPixmap(px);
       ui->hashsize->setCurrentIndex(0);
       ui->savefilename->setText("");
}

void MainWindow::on_btn_browse_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,("Open File"),"",("Image Files (*.png *.jpg)"));
    ui->filepath->setText("");
    QString pic=filename;
    if(pic != ""){
        ui->filepath->setText(pic);
        QPixmap pix(pic);
        int w = ui->labelimage->width();
        int h = ui->labelimage->height();
        ui->labelimage->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    }
    else{
        QMessageBox::information(this,(""),"Please select file from your computer");
    }
}

void MainWindow::on_btn_browse_2_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,("Open File"),"",("Hash Files (*.asc)"));
    ui->hash1->setText("");
    if(filename != "")
        ui->hash1->setText(filename);
    else
        QMessageBox::information(this,(""),"Please select file from your computer");
}

void MainWindow::on_btn_browse_3_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,("Open File"),"",("Hash Files (*.asc)"));
    ui->hash2->setText("");
    if(filename != "")
        ui->hash2->setText(filename);
    else
        QMessageBox::information(this,("Error Message"),"Please select file from your computer");
}

void MainWindow::on_btn_reset_2_clicked()
{
    ui->hash1->setText("");
    ui->hash2->setText("");
}

void MainWindow::on_btn_comparehash_clicked()
{
    if(ui->hash1->text()=="")
        QMessageBox::warning(this,("Error Message"),"Hash 1 is empty");
    else if(ui->hash2->text()=="")
            QMessageBox::warning(this,("Error Message"),"Hash 2 is empty");
    else
    {
        string fin1=ui->hash1->text().toStdString();
        string fin2=ui->hash2->text().toStdString();
        string hash_file1= ".data/hash_file1";
        string hash_file2= ".data/hash_file2";
        if(!gpg.verify(fin1,hash_file1)){
            log_E("Bad signature");
            QMessageBox::critical(this,("Verification "),"Bad signature");
            return;
            }
        if(!gpg.verify(fin2,hash_file2)){
            log_E("Bad signature");
            QMessageBox::critical(this,("Verification "),"Bad signature");
            return;
            }
        float result = bh.compare_hash(hash_file1,hash_file2);
        if(result==-1){
             QMessageBox::critical(this,("Comparison "),QString::fromStdString(bh.get_err()));
             return;
        }
        stringstream str_result (stringstream::in | stringstream::out);
        str_result<<result;
        QString msg = QString::fromStdString(str_result.str());
        QMessageBox::information(this,("Comparison "),"Similarity: "+msg+"%\n Threshold set to 70%");
        on_btn_reset_2_clicked();
    }
}

void MainWindow::on_btn_save_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
            this,
            ("Save Hash"),
            "",
            ("Documents (*.txt)"));
    ui->savefilename->setText(filename+".asc");

}
