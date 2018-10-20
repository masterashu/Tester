#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QTextStream>
#include <QCoreApplication>
#include <QFile>
#include <string>
#include <stdlib.h>

QString username = qgetenv("USER");
QTextStream out(stdout);
int QNumber = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    system("cd /home/$USER/Documents/Tester/temp/; rm * ;");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Program_Check_clicked()
{

}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    QNumber = index + 1;
    out << index;
}

void MainWindow::on_TestButton_clicked()
{
    ui->Souce_Edit->setReadOnly(true);
    QString Code;
    Code = ui->Souce_Edit->toPlainText();

    // Deleting any temp files
    system("cd /home/$USER/Documents/Tester/temp/; rm * ;");

    // Saving the souce code in the file source.c
    QString path = QString("/home/%1/Documents/Tester/temp/source.c").arg(username);
    QFile sourcecode(path);
    if(sourcecode.open(QIODevice::ReadWrite | QIODevice::Text)){
        QTextStream stream(&sourcecode);
        stream.operator <<(Code);
        sourcecode.close();
    }

    // compiling the file
    system("cd /home/$USER/Documents/Tester/temp/; gcc -o prog source.c;");

    // running the program
    QString inputpath = QString("cp /home/$USER/Documents/Tester/Problems/%1/input.txt /home/$USER/Documents/Tester/temp/input.txt;").arg(QNumber);
    system("cp /home/$USER/Documents/Tester/Problems/input1.txt /home/$USER/Documents/Tester/temp/input.txt;");
    if(system("cd /home/$USER/Documents/Tester/temp/; ./prog < input.txt > output.txt")){
        ui->Console_Output->setText("Unable to Run Program!");
        return;
    }

    // checking the output
    QString checkpath = QString("/home/%1/Documents/Tester/Problems/check1.txt").arg(username);
    QString outputpath = QString("/home/%1/Documents/Tester/temp/output.txt").arg(username);
    QString Output, Check;
    QFile output(outputpath);
    if(output.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream a(&output);
        Output = a.readAll();
        output.close();
    }
    QFile check(checkpath);
    if(check.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream b(&check);
        Check = b.readAll();
        check.close();
    }
    out << Output << Check;
    if(QString::compare(Output, Check) == 0){
        ui->Console_Output->setText("Code Accepted!");
    }
    else{
        ui->Console_Output->setText("Wrong Answer!");
    }



    ui->Souce_Edit->setReadOnly(false);
}

// Menu bar Exit option;

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit();
}
