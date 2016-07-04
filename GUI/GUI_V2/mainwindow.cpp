#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <sstream>
#include <unistd.h>

/* Setup GUI */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

/* Destructor for GUI */
MainWindow::~MainWindow()
{
    delete ui;
}

/* Slot for hash generation */
void MainWindow::on_btn_generatehash_clicked()
{
    /* Set progress bar */
    dialog.setLabelText(QString("Generating hash..."));
    dialog.setCancelButton(0);
    dialog.setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    dialog.setMinimumWidth(300);

    /* Watcher for asynchronous computation of hash generation*/
    QFutureWatcher<int> futureWatcher;
    QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
    QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
    QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
    QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));

    /* Validate parameters before proceeding to hash generation */
    if(ui->filepath->text()=="")
        QMessageBox::warning(this,("Error Message"),"Kindly browse Image");
    else if(ui->hashsize->currentIndex()==0)
        QMessageBox::warning(this,("Error Message"),"Kindly select hash size");
    else if(ui->savefilename->text()=="")
        QMessageBox::warning(this,("Error Message"),"Select destination to save hash");
    else
    {
        /* Information box */
        //QMessageBox::information(this,("In Progress"),"This may take few minutes.\nPress Ok to continue");
        int hashsize = (int)sqrt(atof(ui->hashsize->currentText().toStdString().c_str()));

        /* Asynchronous computation of hash */
        QFuture<int> future=(QtConcurrent::run(bh,&Blockhash::process_image,ui->filepath->text().toStdString(),hashsize));
                   futureWatcher.setFuture(future);
                   dialog.exec();
        /* Wait for result and take corrective action */
        int res = 1;
        res = future.result();
        if(res!=0){
            QMessageBox::critical(this,("Has generation failed"),QString::fromStdString(bh.get_err()));
            return;
        }

        /* Sign generated hash with own private key */
        bool result = gpg.generate(CHASH_FILE,ui->savefilename->text().toStdString());
        if(result)
        {
            QMessageBox::information(this,("Signature generated"),"Image hash generated successfully");
        }
        else{
            QMessageBox::critical(this,("Signature generation failed"),QString::fromStdString(gpg.get_err()));
        }
        futureWatcher.waitForFinished();
    }
}

/* Reset user input form for hash generation */
void MainWindow::on_btn_reset_clicked()
{
       ui->filepath->setText("");
       ui->labelimage->setText("");
       QPixmap px;
       ui->labelimage->setPixmap(px);
       ui->hashsize->setCurrentIndex(1);
       ui->savefilename->setText("");
}

/* Slot for browsing image */
void MainWindow::on_btn_browse_clicked()
{
    /* Open file dialog box in home directory */
    QString filePath = ui->filepath->text();
    if(filePath==""){
        filePath = QDir::homePath();
    }
    QString filename=QFileDialog::getOpenFileName(this,("Open File"),filePath,("Image Files (*.png *.jpg *.jpeg)"));
    QString pic=filename;
    if(pic != ""){
        /* Set image path in text field */
        ui->filepath->setText(pic);
        QPixmap pix(pic);

        /* Set default save path */
        int lastIndex = pic.lastIndexOf(".")+1;
        pic.remove(lastIndex,pic.length()-lastIndex);
        ui->savefilename->setText(pic.append("asc"));

        /* Display image preview */
        int w = ui->labelimage->width();
        int h = ui->labelimage->height();
        ui->labelimage->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    }
    else if(ui->filepath->text()==""){
        QMessageBox::information(this,(""),"Please select file from your computer");
    }
}

/* Slot to choose first signed hash file for comparison */
void MainWindow::on_btn_browse_2_clicked()
{
    /* Open file dialog box to choose file */
    QString hashPath = ui->hash1->text();
    if(hashPath==""){
        hashPath = QDir::homePath();
    }
    QString filename=QFileDialog::getOpenFileName(this,("Open File"),hashPath,("Hash Files (*.asc)"));

    /* Set hash file path in text field */
    if(filename != "")
        ui->hash1->setText(filename);
    else if(ui->hash1->text()=="")
        QMessageBox::information(this,(""),"Please select file from your computer");
}

/* Slot to choose second signed hash file for comparison */
void MainWindow::on_btn_browse_3_clicked()
{
    /* Open file dialog box to choose file */
    QString hashPath = ui->hash1->text();
    if(hashPath==""){
        hashPath = QDir::homePath();
    }
    QString filename=QFileDialog::getOpenFileName(this,("Open File"),hashPath,("Hash Files (*.asc)"));

    /* Set hash file path in text field */
    if(filename != "")
        ui->hash2->setText(filename);
    else if(ui->hash2->text()=="")
        QMessageBox::information(this,(""),"Please select file from your computer");
}

/* Slot to reset user input form of hash comparison */
void MainWindow::on_btn_reset_2_clicked()
{
    ui->hash1->setText("");
    ui->hash2->setText("");
}

/* Slot to compare two hash files */
void MainWindow::on_btn_comparehash_clicked()
{
    /* User input validation */
    if(ui->hash1->text()=="")
        QMessageBox::warning(this,("Error Message"),"Hash 1 is empty");
    else if(ui->hash2->text()=="")
            QMessageBox::warning(this,("Error Message"),"Hash 2 is empty");
    else
    {
        /* Hash comaprison */
        string fin1=ui->hash1->text().toStdString();
        string fin2=ui->hash2->text().toStdString();
        string hash_file1= ".data/hash_file1";
        string hash_file2= ".data/hash_file2";

        /* Decrypt first hash file and verify its authenticity using signature */
        if(!gpg.verify(fin1,hash_file1)){
            QString error = "Bad signature "+QString::fromStdString(fin1);
            log_E(error.toStdString());
            QMessageBox::critical(this,("Verification "),error);
            return;
            }

         /* Decrypt second hash file and verify its authenticity using signature */
        if(!gpg.verify(fin2,hash_file2)){
            QString error = "Bad signature "+QString::fromStdString(fin2);
            log_E(error.toStdString());
            QMessageBox::critical(this,("Verification "),error);
            return;
            }

        /* Compare both the hashesh */
        float result = bh.compare_hash(hash_file1,hash_file2);
        if(result==-1){
             QMessageBox::critical(this,("Comparison "),QString::fromStdString(bh.get_err()));
             return;
        }

        /* Show results */
        stringstream str_result (stringstream::in | stringstream::out);
        str_result<<result;
        QString msg = QString::fromStdString(str_result.str());
        QMessageBox::information(this,("Comparison "),"Similarity: "+msg+"%\n Threshold set to 70%");

    }
}

/* Slot to specify save location of hash file*/
void MainWindow::on_btn_save_clicked()
{
    /* Save file dialog box */
    QString filename = QFileDialog::getSaveFileName(
            this,
            ("Save Hash"),
            ui->savefilename->text(),
            ("Documents (*.asc)"));
    if(filename==""){
      return;
    }
    /* Append extension to file */
    if(!filename.contains(".asc")){
        filename.append(".asc");
    }
    ui->savefilename->setText(filename);
}
