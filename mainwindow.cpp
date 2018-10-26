#include "mainwindow.h"
#include "about.h"
#include "ui_about.h"
#include "ui_mainwindow.h"
#include "about.h"
#include <QString>
#include <QTextStream>
#include <QCoreApplication>
#include <QFile>
#include <QFont>
#include <QFontDialog>
#include <QInputDialog>
#include <QFileDialog>
#include <string>
#include <stdlib.h>

QFont GREEN;
QString username = qgetenv("USER");
QTextStream out(stdout);
int QNumber = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->username_label->setText("Hello, " + username + ".");
    updateQuestion();
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

void MainWindow::on_TestButton_clicked()
{
    CopyFiles();
    ui->Console_Output->clear();
    ui->Source_Edit->setReadOnly(true);
    QString Code;
    Code = ui->Source_Edit->toPlainText();


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
        ui->Source_Edit->setReadOnly(false);
    }
    // running the program

    if(system("cd $HOME/Documents/Tester/temp/; ./prog.out < input.txt > output.txt")){
        ui->Console_Output->setText("Unable to Run Program!");
        ui->Source_Edit->setReadOnly(false);
        return;
    }
    ui->Console_Output->setText("Checking if your program is showing the Correct Output...");

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
    if(QString::compare(Output, Check) == 0){
        ui->Console_Output->setText("Code Accepted!");
    }
    else{
        ui->Console_Output->setText("Wrong Answer!");
    }
    ui->Source_Edit->setReadOnly(false);
}

void MainWindow::updateQuestion(){
    QString QNo = QString::number(QNumber);
    QString Questionpath = QString("/home/%1/Documents/Tester/Problems/%2/question.txt").arg(username, QNo);
    QFile Question(Questionpath);
    if(Question.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream Q(&Question);
        QString question = Q.readAll();
        ui->Question_label->setText(question);
        ui->TestButton->setEnabled(true);
        ui->Program_Check->setEnabled(true);
        ui->Source_Edit->setReadOnly(false);
    }
    else
    {
        ui->Question_label->setText("Question not available. try another one.");
        ui->TestButton->setEnabled(false);
        ui->Program_Check->setEnabled(false);
        ui->Source_Edit->setReadOnly(true);
    }
}


void MainWindow::on_Program_Check_clicked()
{
    ui->Console_Output->clear();
    ui->Source_Edit->setReadOnly(true);
    QString Code;
    Code = ui->Source_Edit->toPlainText();

    system("touch $HOME/Documents/Tester/temp/source.c;");

    QString path = QString("/home/%1/Documents/Tester/temp/source.c").arg(username);
    QFile sourcecode(path);
    if(sourcecode.open(QIODevice::ReadWrite | QIODevice::Text)){
        QTextStream stream(&sourcecode);
        stream << Code << endl;
        sourcecode.close();
    }

    ui->Source_Edit->setReadOnly(true);
    if(system("cd $HOME/Documents/Tester/temp/; gcc -o prog source.c;")){
        ui->Console_Output->setText("Compilation error!\nCheck Terminal for more details.");
    }
    else{
        ui->Console_Output->setText("Compilation Successful.\nNow test your program against the custom testcases.");
    }
    ui->Source_Edit->setReadOnly(false);
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    QNumber = index + 1;
    updateQuestion();
}


// Menu bar Exit option;

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

void MainWindow::on_actionOpen_Source_Code_file_triggered()
{
    QString Source, SourceCode;
    QString dir = QString("/home/%1/").arg(username);
    Source = QFileDialog::getOpenFileName(this, tr("Open Source Code File"), dir, tr("C Source Code Files (*.c)"));
    QFile sourcefile(Source);
    if(sourcefile.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream stream(&sourcefile);
        SourceCode = stream.readAll();
        sourcefile.close();
    }
    else{
        SourceCode = "For some unknown reason we were unable to open the file!\nTry Copying the code this time, or just write the code here itself.";
    }
    ui->Source_Edit->setText(SourceCode);
}

void MainWindow::on_actionSet_Tab_Width_triggered()
{
    int tablength;
    tablength = QInputDialog::getInt(this, tr("Enter Tab Length"), tr("Enter a value between 1 to 10"), (ui->Source_Edit->tabStopWidth() / 4), 1, 10);
    ui->Source_Edit->setTabStopWidth(tablength * 4);
}

void MainWindow::on_actionChange_Font_triggered()
{
    bool ok;
    QFont current = ui->Source_Edit->font();
    QFont newfont = QFontDialog::getFont(&ok, current, this, "Select a Font");
    if(ok){
        ui->Source_Edit->setFont(newfont);
    }
}

void MainWindow::on_actionAbout_Tester2_triggered()
{
    about aboutwindow;
    aboutwindow.setModal(true);
    aboutwindow.exec();
}

