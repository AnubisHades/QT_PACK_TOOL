#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <handleapi.h>
#include <QShortcut>
#include <qstring.h>

#include<QProcess>

#include<QDebug>

#include<QFile>
#include<QFileDialog>
#include<QFileInfo>
#include<QDir>

#include<QTime>
#include<QDate>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString CmdArg;
    void LoaderCmdArg();
    void CmdWrite(QString CmdWrVal);

    QString       FilePath;
    QString       NewFilePath;
    QProcess    CMD;

    int PackFlg = 0;
    QString AutoPackFlg = 0;
    QString OpenLastPath;
    QString OpenLastPathQtBat;

    //path为ini配置文件路径，info为写入内容，dst_name为取出结果
    QString CfgIniPath;
    void write_ini(QString section , QString node , QString info );//写ini
    void read_ini(QString section , QString node , QString &dst_name );//读ini


    void PackPrgDeal();
private:
    Ui::MainWindow *ui;
    QProcess *process;
    QShortcut *key;

signals:
    void SetPkWorking();
    void SetPackEnv();
public slots:
    void read_data();
    void finished_process();
    void error_process();

    void PackWorking();
    void PackSetEnv();
private slots:


    void on_pushButton_OpenFile_clicked();
    void on_pushButton_Pack_clicked();
    void on_radioButton_clicked();
    void on_pushButton_OpenQTcmd_clicked();

};

#endif // MAINWINDOW_H
