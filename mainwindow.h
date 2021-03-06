#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ISAM.cpp"
#include "sequential.cpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum TYPE {ISAM, SEQUENTIAL, NONSET};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    class ISAM ourISAM;
    class SequentialFile ourSEQUENTIAL;
    TYPE STRUCTURE_TYPE = NONSET;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_6_clicked();
    void update_table_ISAM();
    void update_table_SEQUENTIAL();
    void update_result_ISAM(Register reg);
    void update_result_SEQUENTIAL(s_Register reg);
    void clear_files();
    void update_TIMER(int64_t duration);
};
#endif // MAINWINDOW_H
