#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qtexttospeech.h"
#include <QMainWindow>
#include <QTextToSpeech>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_actionClear_triggered();

    void on_actionSolve_triggered();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QTextToSpeech *m_speech;
};
#endif // MAINWINDOW_H
