#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QtCore>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidget>

#include "ISAM.cpp"
//#include "sequential.cpp"

bool Isam = false;
bool Seq = false;

std::string filename;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Isam isam("Usuario.csv")
    //Seq seq("Usuario.csv")
    QStringList header;
    header << "Name" << "User" << "Mail" << "Pass";
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(header);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_3_clicked()
{
    QString target = ui->target1->text();
    if (Isam){
        
    } else {
        
    }

}

void MainWindow::on_pushButton_4_clicked()
{
    QString target = ui->target2->text();
    if (Isam){
        
    } else {
        
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    QString target = ui->target3->text();
    if (Isam){
        
    } else {
        
    }
}

void MainWindow::update_table_ISAM(){
    ifstream if_datafile("testing.dat", ios::in | ios::binary);
    //ifstream if_indexfile(ourISAM.getindexName(), ios::in | ios::binary);

    Page pag;
    while(if_datafile >> pag){
        for (auto i = 0; i < pag.first_empty; ++i){
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());

            ui->tableWidget->setItem
                    (ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::fromStdString(string(pag.records[i].name))));
            ui->tableWidget->setItem
                    (ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::fromStdString(string(pag.records[i].user))));
            ui->tableWidget->setItem
                    (ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::fromStdString(string(pag.records[i].mail))));
            ui->tableWidget->setItem
                    (ui->tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::fromStdString(string(pag.records[i].pass).substr(0,12))));
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    if (Seq){
        Seq = false;
        Isam = true;
    }
    Isam = true;

    ISAM ourISAM("testing.dat", "Usuario.csv");
    update_table_ISAM();

}



void MainWindow::on_pushButton_2_clicked()
{
      if(Isam){
          Seq = true;
          Isam = false;
      }
      Seq = true;
}

void MainWindow::on_pushButton_6_clicked()
{
    
}
