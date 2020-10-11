#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QtCore>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>

bool Isam = false;
bool Seq = false;
bool Search = false;
bool Insert = false;
bool Delete = false;

std::string filename;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_3_clicked()
{
    QString target = ui->target1->text();

}

void MainWindow::on_pushButton_4_clicked()
{
    QString target = ui->target2->text();
}

void MainWindow::on_pushButton_5_clicked()
{
    QString target = ui->target3->text();
}

