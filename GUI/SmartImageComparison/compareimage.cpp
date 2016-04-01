#include "compareimage.h"
#include "ui_compareimage.h"
#include <QMessageBox>

CompareImage::CompareImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompareImage)
{

    ui->setupUi(this);
}

CompareImage::~CompareImage()
{
    delete ui;
}

void CompareImage::on_cmp_clicked()
{
    QString str1=ui->textimage1->toPlainText();
    QString str2=ui->textimage2->toPlainText();
    if(str1=="" && str2=="")
        QMessageBox::information(this,tr(""),"Image 1 and Image 2 Hash shold not be empty.");

   else if(str1=="")
        QMessageBox::information(this,tr(""),"Image 1 Hash shold not be empty.");

    else if(str2=="")
        QMessageBox::information(this,tr(""),"Image 2 Hash shold not be empty.");
        //hash cmp function call

}
