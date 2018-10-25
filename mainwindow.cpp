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

void CopyFiles(){
    // Deleting any temp files

    system("cd $HOME/Documents/Tester/temp/; rm * ;");

    QString Question = QString::number(QNumber);
    QString inputcopy = QString("cp /home/$USER/Documents/Tester/Problems/%1/input.txt /home/$USER/Documents/Tester/temp/input.txt;").arg(Question);
    std::string INPUTCOPY = inputcopy.toStdString();
    char COMMAND[1000];
    strcpy(COMMAND, INPUTCOPY.c_str());
    system(COMMAND);
    QString checkcopy = QString("cp /home/$USER/Documents/Tester/Problems/%1/check.txt /home/$USER/Documents/Tester/temp/check.txt;").arg(Question);
    std::string CHECKCOPY = checkcopy.toStdString();
    strcpy(COMMAND, CHECKCOPY.c_str());
    system(COMMAND);
}

void MainWindow::on_Program_Check_clicked()
{
    ui->Souce_Edit->setReadOnly(true);
    if(system("cd $HOME/Documents/Tester/temp/; gcc -o prog source.c;")){
        ui->Console_Output->setText("Compilation error!\nCheck Termional for more details.");
    }
    else{
        ui->Console_Output->setText("Compilation Successful.\nNow test your program against the custom testcases.");
    }
    ui->Souce_Edit->setReadOnly(false);
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    QNumber = index + 1;
    out << index;
}

void MainWindow::on_TestButton_clicked()
{
    CopyFiles();

    ui->Souce_Edit->setReadOnly(true);
    QString Code;
    Code = ui->Souce_Edit->toPlainText();


    // Saving the souce code in the file source.c
    system("touch $HOME/Documents/Tester/temp/source.c;");

    /* It is necessory to have /home/'username' instead of $HOME because it won't work inside Qt.
        */

    QString path = QString("/home/%1/Documents/Tester/temp/source.c").arg(username);
    QFile sourcecode(path);
    if(sourcecode.open(QIODevice::ReadWrite | QIODevice::Text)){
        QTextStream stream(&sourcecode);
        stream << Code << endl;
        sourcecode.close();
    }

    // compiling the file
    if(system("cd $HOME/Documents/Tester/temp/; gcc -o prog.out source.c;")){
        ui->Console_Output->setText("Compilation error!\nCheck Termional for more details.");
        ui->Souce_Edit->setReadOnly(false);
    }

    // running the program

    if(system("cd $HOME/Documents/Tester/temp/; ./prog.out < input.txt > output.txt")){
        ui->Console_Output->setText("Unable to Run Program!");
        ui->Souce_Edit->setReadOnly(false);
        return;
    }
    // checking the output
    QString checkpath = QString("/home/%1/Documents/Tester/temp/check.txt").arg(username);
    QString outputpath = QString("/home/%1/Documents/Tester/temp/output.txt").arg(username);
    QString Output, Check;
    QFile output(outputpath);
    if(output.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream a(&output);
        Output = a.readAll();
        output.close();
    }
    else{
        ui->Console_Output->setText("Output file not found, Check if your program provides any output to the console.");
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
