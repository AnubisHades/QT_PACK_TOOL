#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QShortcut>
#include <QSettings>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("APP.ico"));//设置应用程序图标
    /* 新建process */
    process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);

    CfgIniPath = QCoreApplication::applicationDirPath()+"/config.ini";

    //读取环境变量
    QString str1,str2;
    read_ini("Env",
             "Env1",
             str1);
    read_ini("Env",
             "Env2",
             str2);

    read_ini("Config",
              "Lastpath",
              OpenLastPath);
    read_ini("Config",
              "AutoPack",
              AutoPackFlg);

    qDebug()<<"AutoPackFlg"<<AutoPackFlg;
    //自动打包时，关闭打包按钮
    if (AutoPackFlg.compare("0")==0) {
        ui->pushButton_Pack->setEnabled(true);
    }else if (AutoPackFlg.compare("1")==0){
        ui->pushButton_Pack->setEnabled(false);
    }else{
        AutoPackFlg = "0";
        ui->pushButton_Pack->setEnabled(true);
    }

    if (OpenLastPath.isEmpty()) {
        OpenLastPath = "D:/";
    }

    //
    if (str1.isEmpty() || str2.isEmpty()) {
        //记录为空时初始化
        write_ini("Config",
                  "Lastpath",
                  OpenLastPath);
        write_ini("Config",
                  "AutoPack",
                  "0");

        str1 = "D:\\Qt\\Qt5.12.7\\5.12.7\\mingw73_32\\bin";
        str2 = "D:/Qt/Qt5.12.7/Tools/mingw730_32\bin";
        write_ini("Env",
                  "Env1",
                  str1);
        write_ini("Env",
                  "Env2",
                  str2);

    }

    {
        str1.replace("/", "\\");
        if (str1.contains("\\",Qt::CaseSensitive) == false) {
            str1 = "D:\\Qt\\Qt5.12.7\\5.12.7\\mingw73_32\\bin";
            write_ini("Env",
                      "Env1",
                      str1);
        }
        ui->lineEdit_EnvPath1->setText(str1);
        ui->lineEdit_EnvPath2->setText(str2);
    }



    /* 按键相关 */
    connect(ui->pushButton_Clear,SIGNAL(clicked()),
            ui->textEdit,SLOT(clear()));                     //清除文本框按钮

    /* 命令行相关 */
    connect(process,SIGNAL(readyRead()),
            this,SLOT(read_data()));                                    //读命令行数据
    connect(process,SIGNAL(readyReadStandardOutput()),
            this,SLOT(read_data()));                      //读命令行标准数据（兼容）
    connect(process,SIGNAL(errorOccurred(QProcess::ProcessError)),
            this,SLOT(error_process()));      //命令行错误处理
    connect(process,SIGNAL(finished(int)),
            this,SLOT(finished_process()));                           //命令行结束处理

    /* 打包相关 */
    connect(this,SIGNAL(SetPackEnv()),
            this,SLOT(PackSetEnv()));

    connect(this,SIGNAL(SetPkWorking()),
            this,SLOT(PackWorking()));

    process->start("cmd.exe");
}

MainWindow::~MainWindow()
{
    /* 关闭cmd */
    process->close();

    /* 释放空间 */
    delete ui;
    delete process;

    /* 拴住 */
    ui = NULL;
    process = NULL;

    /* 信息输出 */
    qDebug() << "end Process" << endl;
}


void MainWindow::read_data()
{
    /* 接收数据 */
    QByteArray bytes = process->readAll();

    /* 显示 */
    QString msg = QString::fromLocal8Bit(bytes);
    ui->textEdit->append(msg);
    ui->textEdit->update();

    /* 信息输出 */
    qDebug() << "Success:read_data:" << msg << endl;
    if (PackFlg == 1) {
        ui->textEdit->append("设置环境变量");
        emit SetPackEnv();
    }else if (PackFlg == 2) {
        ui->textEdit->append("打包中请等待 请勿关闭，执行完成后会输出相关打包结果");
        emit SetPkWorking();
    }

}

void MainWindow::finished_process()
{
    /* 接收数据 */
    int flag = process->exitCode();

    /* 信息输出 */
    qDebug() << "Success:finishedProcess():" << flag << endl;
}

void MainWindow::error_process()
{
    /* 接收数据 */
    int err_code  = process->exitCode();
    QString err = process->errorString();

    /* 显示 */
    ui->textEdit->append(QString("error coed:%1").arg(err_code));
    ui->textEdit->append(err);

    /* 信息输出 */
    qDebug() << "Success:error_process():" << err << endl;
}


void MainWindow::LoaderCmdArg()
{
    CmdArg  ="set PATH="
            +ui->lineEdit_EnvPath1->text()
            +";"
            +ui->lineEdit_EnvPath2->text()
            +";%PATH%";
    qDebug()<<ui->lineEdit_EnvPath1->text();
    qDebug()<<ui->lineEdit_EnvPath2->text();
    qDebug()<<CmdArg;
}
void MainWindow::CmdWrite( QString CmdWrVal)
{
    /* 获得命令，\r\n代替键盘的Enter，没有则不会执行 */
    QString cmd = CmdWrVal + "\r\n";

    /* 转为char*并写入 */
    QByteArray qbarr = cmd.toLatin1();
    char *ch = qbarr.data();
    qint64 len = cmd.length();
    process->write(ch, len);

    /* 信息输出 */
    qDebug() << "Success:write_cmd：" << cmd << endl;
}


void MainWindow::on_pushButton_OpenFile_clicked()
{
    //打开待打包目录
    FilePath = QFileDialog::getOpenFileName(this,
                 tr("打开待打包文件"),
                 OpenLastPath,
                 tr("待打包文件(*.exe)"));
    //显示文件信息
    ui->lineEdit_FilePath->setText(FilePath);

    //打开文件
    QFile File(FilePath);
    File.open(QIODevice::ReadWrite | QIODevice::Text);

    //获取文件信息
    QFileInfo FileIno(File);
    QString name = FileIno.fileName();
    QString CompleteName = FileIno.completeBaseName();

    //更新上次打开的文件路径
    OpenLastPath = FileIno.absolutePath();
    write_ini("Config",
              "Lastpath",
              OpenLastPath);

    QDateTime current_time = QDateTime::currentDateTime();
    QString currentTime = current_time.toString("_yyyyMMdd_hhmm_ss");
    currentTime = "0_PkPrg/Res_"+CompleteName+currentTime;
    NewFilePath = "D:/"+currentTime;

    //创建目录
    QDir dir(NewFilePath); // 注意
    if(dir.exists())
    {
        ui->textEdit->append("路径已存在 清删除重复路径或等待1min，自动路径包含min信息");
        qDebug()<<"路径已存在"<<NewFilePath;
        return;
    }else{
        bool ok = dir.mkpath(NewFilePath);
        qDebug()<<"路径建立结果"<<ok;
    }

    NewFilePath = NewFilePath+"/"+name;
    File.copy(NewFilePath);

    ui->textEdit->append("已复制待打包程序至：");
    ui->textEdit->append(NewFilePath);
    File.close();

    if (AutoPackFlg.compare("0") != 0) {
        PackPrgDeal();
    }
}
void MainWindow::PackPrgDeal()
{
    PackFlg= 0;
    if (ui->lineEdit_FilePath->text().isEmpty()) {
        ui->textEdit->append("文件为路径为空，请打开对应文件");
        qDebug()<<"文件为空";
        return;
    }

    QFileInfo FileIno(NewFilePath);
    QString WorkPath = FileIno.absolutePath();//获取文件路径

    //跳转目录
    QString CmdJumpPath = "cd /d ";
    CmdJumpPath = CmdJumpPath+WorkPath;
    qDebug()<<CmdJumpPath;
    PackFlg= 1;
    CmdWrite(CmdJumpPath);
}
void MainWindow::on_pushButton_Pack_clicked()
{
    PackPrgDeal();
}

void MainWindow::PackSetEnv()
{
    LoaderCmdArg();//设置环境变量
    CmdWrite(CmdArg);
    PackFlg = 2;
}

void MainWindow::PackWorking()
{
    QFileInfo FileIno(NewFilePath);
    QString Name = FileIno.fileName();//获取文件路径
    QString PackCmd;
    PackCmd = "windeployqt.exe "+Name;
    CmdWrite(PackCmd);

    PackFlg = 0;
}
void MainWindow::write_ini(QString section, QString node, QString info)
{
    QSettings iniwrite(CfgIniPath,QSettings::IniFormat);
    QString str;
    str.clear();
    str.append("/");
    str.append(section);
    str.append("/");
    str.append(node);
    iniwrite.setValue(str,info);
}

void MainWindow::read_ini(QString section, QString node, QString &dst_name)
{
    QSettings iniread(CfgIniPath,QSettings::IniFormat);
    QString str;
    str.clear();
    str.append("/");
    str.append(section);
    str.append("/");
    str.append(node);
    dst_name = iniread.value(str).toString();
}


void MainWindow::on_radioButton_clicked()
{
    if (ui->radioButton->isChecked()) {
        AutoPackFlg = "1";
        ui->pushButton_Pack->setEnabled(false);
    }else {
        AutoPackFlg = "0";
        ui->pushButton_Pack->setEnabled(true);
    }

    write_ini("Config",
              "AutoPack",
              AutoPackFlg);
}
