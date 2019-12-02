#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define MAX_FRAME_LEN       4 * 1024

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
    unsigned char inpbuf[MAX_FRAME_LEN];
    void deBlank(QString &strs);

protected:
    void keyPressEvent(QKeyEvent *e);

private slots:
    void on_action_Q_triggered();
    void on_action_F1_triggered();
    void on_action_F2_triggered();
    void on_action_C_triggered();
    void on_action_2013_triggered();
    void on_action_S_triggered();
    void on_action_triggered();
    void on_action_H_triggered();
    void on_action_D_triggered();
};

#endif // MAINWINDOW_H
