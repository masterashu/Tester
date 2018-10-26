#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "about.h"
#include "ui_about.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateQuestion();

    void on_Program_Check_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_TestButton_clicked();

    void on_actionExit_triggered();

    void on_actionOpen_Source_Code_file_triggered();

    void on_actionSet_Tab_Width_triggered();

    void on_actionChange_Font_triggered();

    void on_actionAbout_Tester2_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
