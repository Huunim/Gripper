#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QMessageBox>
#include <QScreen>
#include <QWindow>
#include <QTimer>
#include <QTextStream>
#include <QDesktopServices>
#include <QCursor>
#include "globali.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // CONFIG

    ui->listWidget->setVisible(false);
    ui->buttonRecord->setStyleSheet("Background-color:#87ff81");
    ui->pushButton_6->setStyleSheet("Background-color:#ff8f00");
    numFile = 0;
    registra = false;    
    creaCartellaSeNonCe();
    dimensioniSchermo();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::creaCartellaSeNonCe()
{
    myHome = QDir::homePath();

        QDir cartella(myHome + "/GripperImages");
        if(!cartella.exists()){
            cartella.mkpath(myHome + "/GripperImages");
            // crea guida
            // leggi risorsa
            QString guida;
            QFile fl(":/fast_guide.txt");
            if(!fl.open(QFile::ReadOnly | QFile::Text)){}
            QTextStream in(&fl);
            guida = in.readAll();
            fl.close();
            // crea guida
            QFile xfile(myHome + "/GripperImages/fast_guide.txt");
                if(!xfile.open(QFile::WriteOnly | QFile::Text)){}
                  QTextStream out(&xfile);
                  out << guida;
                  xfile.flush();
                  xfile.close();
            //------
             MSG("GripperImages folder and guide created! ");
        }
}

void MainWindow::MSG(QString tx)
{
    QMessageBox::information(this, "Gripper", tx);
}

int MainWindow::verificaNomeFile()
{
    QDir cartella(myHome + "/GripperImages/" + ui->lineEdit_7->text());
    if(!cartella.exists()){
        cartella.mkpath(myHome + "/GripperImages/" + ui->lineEdit_7->text());
    }else{
        MSG("The folder " + ui->lineEdit_7->text() + " already exists.");
        return 0;
    }
   return 1;
}

void MainWindow::moveEvent(QMoveEvent *ev){
    if(!registra){  dimensioni();}

}

void MainWindow::dimensioni(){
    if(!registra && !ui->checkBox->isChecked() && !ui->checkBox_2->isChecked()){
    x = this->pos().x();
    y = this->pos().y();
    h = this->frameSize().height();

    w = this->frameSize().width();
    // porta a pari se selezionato
    if(ui->semprePariCheck->isChecked()){
        w = w -(w % 2 );
        h = h -(h % 2 );
    }

    mostra();
    }

}

void MainWindow::mostra()
{
    ui->lineEdit_3->setText(QString::number(x));
    ui->lineEdit_4->setText(QString::number(y));
    ui->lineEdit_5->setText(QString::number(w));
    ui->lineEdit_6->setText(QString::number(h));
    double mega = w*h;
    mega = mega /1000000;
    ui->lineMegaPixel->setText(QString::number(mega, 'g', 2));
}


void MainWindow::resizeEvent(QResizeEvent *ev){
    if(!registra){ dimensioni();}

}


void MainWindow::on_buttonRecord_clicked() // REC
{
    // verifiche ----------------------------------------
    if(ui->buttonRecord->text() == "STOP"){
        stoppa("Html player created");
        return;
    }
    if(ui->buttonRecord->text()=="REC"){
    //---------------------------------------------------
   screen = QGuiApplication::primaryScreen();
       if(const QWindow *window = windowHandle()){ screen = window->screen();}
       if(!screen){return;}
//   //-------------------------------------------------
   QDir d;
   if(d.exists(myHome + "/GripperImages")==false){
       MSG("Folder not found!");
       return;
   }

   imgFile = myHome + "/GripperImages/" + ui->lineEdit_7->text() + "/" + ui->lineEdit_7->text() + "_";
   //-----------------------------------------------
   if(verificaNomeFile() == 0 ){ return; }
   //FINE VERIFICHE ---------------------------------

   // singolo screenshot-----------------------------
   if(ui->checkshot->isChecked()){
       cattura();
       numFile=0;
       MSG("Saved in " + imgFile + ".jpg");
       return;
   }

   //IMPOSTAZIONI x sequenza --------------------------
   if(ui->checkMinimizza->isChecked()){
       this->setWindowState(Qt::WindowMinimized);
   }
   //---------------------------------------------------

   ui->pushButton_3->setEnabled(false);
   ui->buttonRecord->setText("STOP");
   ui->buttonRecord->setStyleSheet("Background-color:#ff0000");
   ui->spinBox->setEnabled(false);
   ui->label_8->setText("0");
   //------------------------------------------------
    if(ui->checkMouse->isChecked()){ topo = true;}
    else{topo = false;}
   //------------------------------------------------
    }
     // da qui anche con resume
    if(ui->buttonRecord->text() == "RESUME"){
        ui->buttonRecord->setText("STOP");
        ui->buttonRecord->setStyleSheet("Background-color:#ff0000");
    }

   int intervallo = ui->spinBox->value();
   if(intervallo < 60){
       intervallo = 60;
       ui->spinBox->setValue(60);
   }

   //--------------------------------------------------   
   registra = true;
   //---------------------------------------------------

   timer1 = new QTimer(this);
   connect(timer1, SIGNAL(timeout()), this, SLOT( cattura()));
   timer1->start(intervallo);
}

void MainWindow::cattura(){
    QPixmap pix;
    pix= screen->grabWindow(0, x, y, w, h);

    QImage r = pix.toImage();
    QPainter pa(&r);
    //puntatore non catturato >:   mi tocca disegnarlo....
    if(topo){      
      QImage mous(":/mous.png");
      int mousex = QCursor::pos().x()-x;
      int mousey = QCursor::pos().y()-y;

      pa.drawImage(mousex, mousey, mous);

    }
    //-------------------
    //salva file
    numFile++;
    if(numFile > 9999){stoppa("Exceeded the maximum number of images "); return;}
    // ...
    QString suffisso = "000" + QString::number(numFile);
    suffisso =suffisso.right(4);
    suffisso = imgFile + suffisso + ".jpg";
    r.save(suffisso, "JPG");


}

void MainWindow::on_pushButton_6_clicked()
{
    // pausa
    if(ui->buttonRecord->text() == "REC" || ui->buttonRecord->text() == "RESUME"){return;}
    timer1->stop();
    ui->buttonRecord->setText("RESUME");
    ui->buttonRecord->setStyleSheet("Background-color:#FFff81");
    //-----------------------------------------------
    ui->label_8->setText(QString::number(numFile));
    //------------------------------------------------
     if(this->width() < 100){
     this->setGeometry(10,10, 488, 388);
     }
     ui->buttonRecord->setGeometry(360, 60, 86,86);
    //------------------------------------------------
    registra = false;


}

void MainWindow::stoppa(QString msg)
{
    timer1->stop();
    ui->buttonRecord->setText("REC");
    ui->buttonRecord->setStyleSheet("Background-color:#87ff81");
    //-----------------------------------------------

    ui->pushButton_3->setEnabled(true);
    ui->spinBox->setEnabled(true);
    //------------------------------------------------
    ui->label_8->setText(QString::number(numFile));
    numFile = 0;
    //------------------------------------------------     
     if(this->width() < 100){
     this->setGeometry(10,10, 488, 388);
     }
     ui->buttonRecord->setGeometry(360, 60, 86,86);
    //------------------------------------------------
    registra = false;
    //------------------------------------------------
    creaPlayer();
    MSG(msg);
}



void MainWindow::creaPlayer()
{

    QString player; // = ui->res1->toPlainText();
    QFile fl(":/html.txt");
    if(!fl.open(QFile::ReadOnly | QFile::Text)){}
    QTextStream in(&fl);
    player = in.readAll();
    fl.close();

    int tempo =ui->spinBox->value();
    player.replace("ZZZ", QString::number(tempo)); // intervallo
    player.replace("UUU", ui->label_8->text());
    player.replace("fff", imgFile );
    player.replace("W?", QString::number(w));
    player.replace("H?", QString::number(h));
    double ffmpegInterval = tempo * 0.001;
    if(w % 2 == 1 || h % 2 == 1){
        player.replace("T?", "Odd height or width, not convertible to mp4");

    }else{
      player.replace("T?", "ffmpeg -f image2 -r 1/" + QString::number(ffmpegInterval, 'g', 12) + " -i " + imgFile + "%04d.jpg " + imgFile + ".mp4");
    }
    // crea file
    QFile xfile(imgFile + "PLAYER.html");
        if(!xfile.open(QFile::WriteOnly | QFile::Text)){}
          QTextStream out(&xfile);
          out << player;
          xfile.flush();
          xfile.close();

}

void MainWindow::dimensioniSchermo()
{
    QScreen *scrin = QGuiApplication::primaryScreen();
    QRect schermo = scrin->geometry();
    schermoH = schermo.height();
    schermoW = schermo.width();
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked){
        h = schermoH;
        w = schermoW;
        x=0;  y=0;
        mostra();
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    // open folder
    if(QDir(myHome + "/GripperImages").exists()){
       QDesktopServices::openUrl(QUrl(myHome + "/GripperImages"));
    }else{
        MSG("Folder not found!");

    }
}

void MainWindow::on_pushButton_2_clicked()
// riduci aumenta
{
    if(ui->buttonRecord->pos().x() == 1){
        this->setGeometry(this->pos().x(), this->pos().y(), 488, 388);
        ui->buttonRecord->setGeometry(360, 60, 86,86);

    }else{
    this->setGeometry(this->pos().x(), this->pos().y(), 88, 88);
    ui->buttonRecord->setGeometry(1, 1, 80,80);
    }

}

void MainWindow::on_pushButton_4_clicked()
{
   if(QFile::exists(myHome + "/GripperImages/" + ui->lineEdit_7->text() + "/" + ui->lineEdit_7->text() + "_PLAYER.html")){
    QDesktopServices::openUrl(QUrl(myHome + "/GripperImages/" + ui->lineEdit_7->text() + "/" + ui->lineEdit_7->text() + "_PLAYER.html" ));
   }else{
          MSG("The player " + ui->lineEdit_7->text() + "_PLAYER.html, does not exist.");
   }
}

void MainWindow::on_iconbutton_clicked()
{
    QPixmap ikon(":/gri.ico");
    ikon.save(myHome + "/gripperIcon.png", "PNG");
    MSG("Icon saved in home");
}

void MainWindow::on_pushButton_5_clicked()
{
    nomi::nFile = myHome + "/GripperImages/" + ui->lineEdit_7->text() + "/" + ui->lineEdit_7->text() + "_";
    textOptions opzioniTesto(this);
    opzioniTesto.setWindowTitle("Options text on " + ui->lineEdit_7->text());
    opzioniTesto.exec();

}


void MainWindow::on_iconbutton_2_clicked()
{
    nomi::nFile = myHome + "/GripperImages/" + ui->lineEdit_7->text() + "/" + ui->lineEdit_7->text() + "_";
    forme faForme(this);
    faForme.setWindowTitle("Shapes on " + ui->lineEdit_7->text());
    faForme.exec();

}


void MainWindow::on_trasparenze_clicked()
{
  nomi::nFile = myHome + "/GripperImages/" + ui->lineEdit_7->text() + "/" + ui->lineEdit_7->text() + "_";
  Traspar trx(this);
  trx.setWindowTitle("Colors on " + ui->lineEdit_7->text());
  trx.exec();

}

void MainWindow::on_rinumera_button_clicked()
{
    // rinumera
    int q = QMessageBox::question(this, "Gripper", "Renumber the files in the folder?", QMessageBox::Yes| QMessageBox::No);
    if(q== QMessageBox::No){ return;}

    QDir cartellaDaNumerare(myHome + "/GripperImages/" + ui->lineEdit_7->text());
    if(cartellaDaNumerare.exists()){
         QStringList myimg= cartellaDaNumerare.entryList();
         int numFile = 1;
         QString sNum="";
         foreach (QString nomeFile, myimg) {
            if(nomeFile.startsWith( ui->lineEdit_7->text()) && nomeFile.endsWith(".jpg", Qt::CaseInsensitive)){
               sNum = QString::number(numFile);
               sNum = "0000" + sNum;
               sNum = sNum.right(4);
               QFile::rename(myHome + "/GripperImages/" + ui->lineEdit_7->text() + "/" + nomeFile, myHome + "/GripperImages/" + ui->lineEdit_7->text() + "/" + ui->lineEdit_7->text() + "_" + sNum + ".jpg"  );
               numFile++;
            }
         }
         MSG("Done. The player has not been modified");

    }
    else{
        MSG( "Folder not found");
        return;
    }
}


void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    if(currentRow > -1){
        ui->lineEdit_7->setText(ui->listWidget->item(currentRow)->text());
        ui->listWidget->setVisible(false);
    }

}

void MainWindow::on_pushButton_clicked()
{
    if(ui->listWidget->isVisible()){
        ui->listWidget->setVisible(false);
    }else{
    ui->listWidget->clear();
    QDir dir(myHome + "/GripperImages");
    QStringList tutte= dir.entryList();

    foreach (QString nomeFile, tutte){
        QFileInfo fi(myHome + "/GripperImages/" + nomeFile) ;
        if( fi.isDir() && !nomeFile.startsWith(".")){ui->listWidget->addItem(nomeFile);}

    }
    ui->listWidget->setVisible(true);
    }
}

void MainWindow::on_bars_button_2_clicked()
{
    int larghBarraTitolo;

    larghBarraTitolo = this->frameSize().height() - this->height();

    Barra *bar1= new Barra(this);
    bar1->setGeometry(x,y-larghBarraTitolo-1, w,1);
    bar1->setWindowFlag(Qt::WindowStaysOnTopHint);
    bar1->show();
    //  seconda barra

    Barra *bar2= new Barra(this);
    bar2->setGeometry(x,y+h, w,1);
    bar2->setWindowFlag(Qt::WindowStaysOnTopHint);
    bar2->show();
}
