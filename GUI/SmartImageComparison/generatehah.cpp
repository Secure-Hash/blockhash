#include "generatehah.h"
#include "ui_generatehah.h"
#include"mainwindow.h"
#include"compareimage.h"
#include<QFileDialog>
#include <QMessageBox>
QString pic="";
GenerateHah::GenerateHah(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenerateHah)
{
    ui->setupUi(this);


}

GenerateHah::~GenerateHah()
{
    delete ui;
}

void GenerateHah::on_FileDialog_clicked()
{
        QString filename=QFileDialog::getOpenFileName(this,tr("Open File"),"/home/paresh",tr("Image Files (*.png *.jpg *.bmp)"));
        pic=filename;
        if(pic != ""){
            ui->FilePath->setText(pic);
            QPixmap pix(pic);
            int w = ui->label_image->width();
            int h = ui->label_image->height();
            ui->label_image->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            QMessageBox::information(this,tr("File Uploaded"),"file uploded successfully");
            ui->FileDialog->setEnabled(false);
            ui->label->setVisible(false);
            ui->hashLength->setEnabled(true);
            ui->hashgen->setEnabled(true);
        }
        else{
            QMessageBox::information(this,tr(""),"Please Select File from your computer");
        }
}

void GenerateHah::on_Home_clicked()
{
    this->close();
}


void GenerateHah::on_cmp_clicked()
{
    this->close();
    CompareImage cmpi;
    cmpi.setModal(true);
    cmpi.exec();
}

void GenerateHah::on_hashgen_clicked()
{
    //call hash generation function
    // set text

    QString hashlength=ui->hashLength->currentText();
    if(hashlength==""){
        QMessageBox::information(this,tr("Hash Length Not Selected "),"Please Select hash Length from dropdown");
    }
    ui->hash->setText(hashlength);
}

/*

    QString hashlength=arg1;
    if(hashlength==""){
            QMessageBox::information(this,tr("Hash Length Not Selected "),"Please Select hash Length from dropdown");
    }
     QMessageBox::information(this,tr("Hash Length Not Selected "),"Please Select hash Length from dropdown");
    else{

    }
      ui->hashgen->setEnabled(true);
*/

