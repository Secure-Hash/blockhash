#ifndef GENERATEHAH_H
#define GENERATEHAH_H


#include<QDialog>


namespace Ui {
class GenerateHah;
}

class GenerateHah : public QDialog
{
    Q_OBJECT

public:
    explicit GenerateHah(QWidget *parent = 0);
    ~GenerateHah();

private slots:
   void on_FileDialog_clicked();

   void on_Home_clicked();

   void on_cmp_clicked();

   void on_hashgen_clicked();

   void on_comboBox_activated(const QString &arg1);

   void on_hashLength_activated(const QString &arg1);

   void on_hashLength_currentIndexChanged(const QString &arg1);

   void on_hashLength_activated(int index);

   void on_hashLength_currentTextChanged(const QString &arg1);

   void on_label_6_linkActivated(const QString &link);

private:
    Ui::GenerateHah *ui;
};

#endif // GENERATEHAH_H
