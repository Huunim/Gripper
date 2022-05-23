#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMoveEvent>
#include <QResizeEvent>
#include "textoptions.h"
#include "forme.h"
#include "traspar.h"
#include "barra.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
void moveEvent(QMoveEvent *);
void resizeEvent(QResizeEvent *);


private slots:
    void creaCartellaSeNonCe();
    void MSG(QString tx);
    int verificaNomeFile();
    void dimensioni();
    void mostra();
    void cattura();
    void stoppa(QString msg);   
    void creaPlayer();
    void dimensioniSchermo();

    void on_buttonRecord_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_iconbutton_clicked();

    void on_pushButton_5_clicked();

    void on_iconbutton_2_clicked();

    void on_trasparenze_clicked();

    void on_rinumera_button_clicked();

    void on_listWidget_currentRowChanged(int currentRow);

    void on_pushButton_clicked();

    void on_pushButton_6_clicked();


    void on_bars_button_2_clicked();

private:
    Ui::MainWindow *ui;

    QString myHome;
    int x, y, w, h;
    QTimer *timer1;
    QScreen *screen;
    int numFile;
    QString imgFile;
    bool registra;
    int schermoW, schermoH;
    bool topo;



};
#endif // MAINWINDOW_H
