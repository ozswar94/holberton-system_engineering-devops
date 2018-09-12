#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>
#include <QTimer>
#include <QMainWindow>
#include <QString>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    HANDLE serie;
    DCB dcb;
    QTimer *Anten;
    QTimer *Inven;
    unsigned char* temp;

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void Get_HealthParameters();
    void Inventory();
};

#endif // MAINWINDOW_H
