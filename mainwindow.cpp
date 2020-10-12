#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QtCore>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidget>

bool Isam = false;
bool Seq = false;

string ISAM_FILENAME = "ISAM_FILE.dat";
string SEQUENTIAL_FILENAME = "SEQUENTIAL_FILE.dat";

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
    QString tg = ui->target1->text();
    std::string target = tg.toStdString();

    switch (STRUCTURE_TYPE){
        case SEQUENTIAL:{
            s_Register newReg;
            newReg = ourSEQUENTIAL.search(target);
            QString accesses = "Secondary Memory Accesses: " +
                    QString::fromStdString(to_string(ourSEQUENTIAL.mem_access_counter_AUX + ourSEQUENTIAL.mem_access_counter_DATA));
            ui->Accesses->setText(accesses);
            update_table_SEQUENTIAL();
            break;
        }

        case ISAM:{
            PageLocation loc;
            loc = ourISAM.search(target);
            QString accesses = "Secondary Memory Accesses: " +
                    QString::fromStdString(to_string(ourISAM.mem_access_counter_INDEX + ourISAM.mem_access_counter_DATA));
            ui->Accesses->setText(accesses);
            update_table_ISAM();
            break;
        }

        default:
            return;
    }

    /*auto result = ourISAM.search("Alexusis Fulton");

      ourISAM.erase("Kurt Nelson");

      ourISAM.insert(Register("Roger Wilson", "roger_wilson","roger_wilson@correo.com","WswASDw123Sd2"));
      ourISAM.insert(Register("Kurt Nelson", "roger_wilson","roger_wilson@correo.com","WswASDw123Sd2"));
      ourISAM.insert(Register("Athena Lloyd", "roger_wilson","roger_wilson@correo.com","WswASDw123Sd2"));
      ourISAM.insert(Register("Aaron Carter", "roger_wilson","roger_wilson@correo.com","WswASDw123Sd2"));
      ourISAM.erase("Rocco Nelson");*/

}

void MainWindow::on_pushButton_4_clicked() //insert
{
    QString tg = ui->target2->text();
    std::string target = tg.toStdString();
    char name[30];
    char user[30];
    char mail[41];
    char pass[12];
    string nam, usr, ml, pss;
    stringstream ss(target);
    getline(ss, nam, ',');
    getline(ss, usr, ',');
    getline(ss, ml, ',');
    getline(ss, pss, ',');
    nam = nam.substr(0,30);
    usr = usr.substr(0,30);
    ml = ml.substr(0,41);
    pss = pss.substr(0,12);
    strcpy(name, nam.c_str());
    strcpy(user, usr.c_str());
    strcpy(mail, ml.c_str());
    strcpy(pass, pss.c_str());

    if (string(name) == "") return;

    bool added;

    switch (STRUCTURE_TYPE){
        case SEQUENTIAL:{
            ourSEQUENTIAL.add(s_Register(name, user, mail, pass));
            QString accesses = "Secondary Memory Accesses: " +
                    QString::fromStdString(to_string(ourSEQUENTIAL.mem_access_counter_AUX + ourSEQUENTIAL.mem_access_counter_DATA));
            ui->Accesses->setText(accesses);
            update_table_SEQUENTIAL();
            break;
        }

        case ISAM:{
            added = ourISAM.insert(Register(name, user, mail, pass));
            QString accesses = "Secondary Memory Accesses: " +
                    QString::fromStdString(to_string(ourISAM.mem_access_counter_INDEX + ourISAM.mem_access_counter_DATA));
            ui->Accesses->setText(accesses);
            update_table_ISAM();
            break;
        }

        default:
            return;
    }
}

void MainWindow::on_pushButton_5_clicked() //delete
{
    // QString target = ui->target3->text();

    QString tg = ui->target3->text();
    std::string target = tg.toStdString();
    bool deleteded = false;

    switch (STRUCTURE_TYPE){
        case SEQUENTIAL:{
            deleteded = ourSEQUENTIAL.delet(target);
            QString accesses = "Secondary Memory Accesses: " +
                    QString::fromStdString(to_string(ourSEQUENTIAL.mem_access_counter_AUX + ourSEQUENTIAL.mem_access_counter_DATA));
            ui->Accesses->setText(accesses);
            update_table_SEQUENTIAL();
            break;
        }

        case ISAM:{
            deleteded = ourISAM.erase(target);
            QString accesses = "Secondary Memory Accesses: " +
                    QString::fromStdString(to_string(ourISAM.mem_access_counter_INDEX + ourISAM.mem_access_counter_DATA));
            ui->Accesses->setText(accesses);
            update_table_ISAM();
            break;
        }

        default:
            return;
    }
}

void MainWindow::update_table_ISAM()
{
    ui->tableWidget->setRowCount(0);
    ifstream if_datafile(ISAM_FILENAME, ios::in | ios::binary);
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

void MainWindow::update_table_SEQUENTIAL()
{
    ui->tableWidget->setRowCount(0);

    auto v = ourSEQUENTIAL.load();

    for (auto i = 0; i < v.size(); ++i){
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());

        ui->tableWidget->setItem
                (ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::fromStdString(string(v[i].name).substr(0,30))));
        ui->tableWidget->setItem
                (ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::fromStdString(string(v[i].user).substr(0,30))));
        ui->tableWidget->setItem
                (ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::fromStdString(string(v[i].mail).substr(0,41))));
        ui->tableWidget->setItem
                (ui->tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::fromStdString(string(v[i].pass).substr(0,12))));
    }
}

void MainWindow::clear_files(){
    fstream clr1(ISAM_FILENAME, ios::out | ios::trunc);
    fstream clr2(ISAM_FILENAME.substr(0, ISAM_FILENAME.length()-4) + "_index" + to_string(1) + ".dat",ios::out | ios::trunc);
    fstream clr3(SEQUENTIAL_FILENAME, ios::out | ios::trunc);
    fstream clr4("auxil.dat", ios::out | ios::trunc);
    clr1.close();
    clr2.close();
    clr3.close();
    clr4.close();
}


void MainWindow::on_pushButton_clicked() // ISAM BUTTON
{
    clear_files();

    STRUCTURE_TYPE = ISAM;
    ourISAM.construct( ISAM_FILENAME , "Usuario.csv");
    QString accesses = "Secondary Memory Accesses: " + QString::fromStdString(to_string(ourISAM.mem_access_counter_INDEX + ourISAM.mem_access_counter_DATA));
    ui->Accesses->setText(accesses);
    update_table_ISAM();
}


void MainWindow::on_pushButton_2_clicked() // SEQUENTIAL BUTTON
{
    clear_files();

    STRUCTURE_TYPE = SEQUENTIAL;
    ourSEQUENTIAL.construct(SEQUENTIAL_FILENAME, "Usuario.csv");
    QString accesses = "Secondary Memory Accesses: " + QString::fromStdString(to_string(ourSEQUENTIAL.mem_access_counter_AUX + ourSEQUENTIAL.mem_access_counter_DATA));
    ui->Accesses->setText(accesses);
    update_table_SEQUENTIAL();
}

void MainWindow::on_pushButton_6_clicked() // REFRESH
{
    switch (STRUCTURE_TYPE){
        case SEQUENTIAL:
            update_table_SEQUENTIAL();
            break;

        case ISAM:
            update_table_ISAM();
            break;

        default:
            return;
    }
}
