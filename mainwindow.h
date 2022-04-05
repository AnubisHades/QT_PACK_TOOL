#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <handleapi.h>
#include <QShortcut>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QProcess *process;
    QShortcut *key;

public slots:
    void write_cmd();
    void read_data();
    void finished_process();
    void error_process();

};

#endif // MAINWINDOW_H
