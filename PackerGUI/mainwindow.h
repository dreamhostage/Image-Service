#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Windows.h>
#include <QMessageBox>
#include <QFileDialog>
#include <QValidator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::string path;
    void generate(std::string ip, std::string port);
    QMessageBox mes;

private slots:
    void on_lineIP_textChanged(const QString &arg1);

    void on_linePort_textChanged(const QString &arg1);

    void on_chCur_stateChanged(int arg1);

    bool on_bPack_clicked();

    void on_bDir_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
