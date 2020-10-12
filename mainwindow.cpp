#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QtCore>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidget>


//#include "sequential.cpp"

bool Isam = false;
bool Seq = false;

#define ISAM_FILENAME ISAM_FILE.dat;
#define SEQUENTIAL_FILENAME SEQUENTIAL_FILE.dat;

std::string filename;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList header;
    header << "Name" << "User" << "Mail" << "Pass";
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(header);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_3_clicked() // search
{
    QString target = ui->target1->text();
    if (Isam){
        
    } else {
        
    }

    /*auto result = ourISAM.search("Alexusis Fulton");

      ourISAM.erase("Kurt Nelson");

      ourISAM.insert(Register("Roger Wilson", "roger_wilson","roger_wilson@correo.com","WswASDw123Sd2"));
      ourISAM.insert(Register("Kurt Nelson", "roger_wilson","roger_wilson@correo.com","WswASDw123Sd2"));
      ourISAM.insert(Register("Athena Lloyd", "roger_wilson","roger_wilson@correo.com","WswASDw123Sd2"));
      ourISAM.insert(Register("Aaron Carter", "roger_wilson","roger_wilson@correo.com","WswASDw123Sd2"));
      ourISAM.erase("Rocco Nelson");*/

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

    switch (STRUCTURE_TYPE){
        case SEQUENTIAL:

            break;

        case ISAM:

            break;

        default:
            return;
    }
}

void MainWindow::update_table_ISAM()
{
    ui->tableWidget->setRowCount(0);
    ifstream if_datafile("ISAM.dat", ios::in | ios::binary);
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

void MainWindow::update_table_SEQUENTIAL(){

}

void MainWindow::clear_files(){
    fstream clr1 (ISAM_FILENAME, ios::out | ios::trunc);
    fstream clr2 (ourISAM.getindexName(), ios::out | ios::trunc);
    fstream clr3 (SEQUENTIAL_FILENAME, ios::out | ios::trunc);
    fstream clr4 ("auxil.dat", ios::out | ios::trunc);
    clr1.close();
    clr2.close();
    clr3.close();
    clr4.close();
}


void MainWindow::on_pushButton_clicked() // ISAM BUTTON
{
    clear_files();

    STRUCTURE_TYPE = ISAM;
    ourISAM.construct(ISAM_FILENAME, "Usuario.csv");
    update_table_ISAM();
}


void MainWindow::on_pushButton_2_clicked() // SEQUENTIAL BUTTON
{
    clear_files();

    STRUCTURE_TYPE = SEQUENTIAL;
    ourSEQUENTIAL.construct(SEQUENTIAL_FILENAME, "Usuario.csv");
    update_table_SEQUENTIAL();
}

void MainWindow::on_pushButton_6_clicked()
{
    update_table_ISAM();
}
