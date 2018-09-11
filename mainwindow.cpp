#include "mainwindow.h"
#include "ui_mainwindow.h"

#define START	0X02
#define LONG	0X00
#define LONG2	0X08
#define	CTRL	0X00
#define	BAUDRATE	115200
#define	BYTESIZE	8
#define	TIMER	4000

using namespace std;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),ui(new Ui::MainWindow){
     ui->setupUi(this);
    char  port[6] = "COM1:";

    WCHAR  *wport = new  WCHAR[6];//Conversion du char en w_char
    mbstowcs (wport,port,6);
    serie = CreateFile(wport,GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if(serie == INVALID_HANDLE_VALUE)
        ui->label->setText("erreur COM1");
    else{
	ui->label->setText("COM1 OK");
    GetCommState(serie, &dcb);
    dcb.BaudRate = BAUDRATE;
    dcb.Parity = NOPARITY;
    dcb.ByteSize = BYTESIZE;
    dcb.StopBits = ONESTOPBIT;
    SetCommState(serie, &dcb);
	}
    Anten = new QTimer(this);
    connect(Anten, SIGNAL(timeout()), this, SLOT(Get_HealthParameters()));
    Inven = new QTimer(this);
    connect(Inven, SIGNAL(timeout()), this, SLOT(Inventory()));
}

//fonction de du crc
/*unsigned char* MainWindow::calc_crc(unsigned char *Data,unsigned int N){
    crc_init = 0xFFFF;

    for(int i = 5;i < N-1; i++){
        crc_init = crc_init ^(Data[i]<< 8);
        for(int i = 0; i < 7; i++){
            if (crc_init & 0x8000)
                crc_init = (crc_init << 1)^ 0x1021;
            else crc_init = crc_init << 1;
        }
    }

    unsigned char crc[2];
    crc[1] = crc_init & 0x00FF;
    crc[0] = crc_init >> 8;
    return crc;
}*/

//------------------------------- Calcul du CRC 16 CCITT --------------

unsigned char* MainWindow::calc_crc(unsigned char donnee[], unsigned int size){
	unsigned int crc_init = 0xFFFF;
	char index;
	unsigned int i;
	unsigned char crc[2];
	
	for(i = 1; i < size+1; i++){
		crc_init = (unsigned short)(crc_init ^ (donnee[i] << 8));
		for(index = 0; index <= 7; index++){
			if(crc_init & 0x8000)
				crc_init = (unsigned short)((crc_init << 1) ^ 0x1021);
			else
				crc_init = (unsigned short)(crc_init << 1);
		}
	}
	crc[0]= crc_init >> 8;
	crc[1]= crc_init & 0x00FF;
	return crc;
}

//TestLecteur Commande Etat du lecteur
void MainWindow::on_pushButton_clicked(){
	int N = 12;
	unsigned char Data[50];
	unsigned long taille = 15;
    unsigned char* buffer = calc_crc(Data, N);
    QString envoi;
    
	Data[0] = START;
    Data[1] = LONG;
    Data[2] = LONG2;
    Data[3] = CTRL;
    Data[4] = CTRL;
    Data[5] = 0x00;//Commande
    Data[6] = 0x00;
    Data[7] = 0x00;
    Data[8] = 0x24;
    Data[9] = 0xAA;
    Data[10] = 0x55;
    Data[11] = 0x00;
    Data[12] = 0x00;
    Data[13] = buffer[0];
    Data[14] = buffer[1];
    for(int i = 0; i < 15 ; i++){
       envoi += QString::number(Data[i], 16).toUpper();
       envoi += " ";
    }
         ui->label_3->setText(envoi);
   WriteFile(serie,Data,taille, &taille, 0);
   Anten->start(TIMER);
}

void MainWindow::Get_HealthParameters(){
    COMSTAT etat;
    unsigned long nb_lus;
    QString message;
    
	ClearCommError(serie,0,&etat);
    if(etat.cbInQue)
        ReadFile(serie,temp,etat.cbInQue,&nb_lus,0);
   for(int i = 0; i < etat.cbInQue;i++){
       message += QString::number(temp[i], 16).toUpper();
       message += " ";
   }
     ui->lineEdit->setText(message);
     Anten->stop();
}

void MainWindow::on_pushButton_2_clicked(){
    int N = 12;
    unsigned char Data[100];
    unsigned char* buffer = calc_crc(Data,N);
    unsigned long taille = sizeof(Data);
    QString envoi;

    Data[0] = START;
    Data[1] = LONG;
    Data[2] = LONG2;
    Data[3] = CTRL;
    Data[4] = CTRL;
    Data[5] = 0x00;//Commande
    Data[6] = 0x08;
    Data[7] = 0x00;
    Data[8] = 0x01;
    Data[9] = 0xAA;
    Data[10] = 0x55;
    Data[11] = 0x00;
    Data[12] = 0x00;
    Data[13] = buffer[0];
    Data[14] = buffer[1];
    for(int i = 0; i < taille; i++){
        envoi += QString::number(Data[i], 16).toUpper();
        envoi += " ";
    }
    WriteFile(serie, Data, taille, &taille, 0);
    Inven->start(TIMER);
}

QString* MainWindow::Inventory(){
    COMSTAT etat;
    ClearCommError(serie,0,&etat);
    unsigned long nb_lus;
    QString message;
    bool ok;
    int x = 10;
    int y = 0;
    taille_EPC = 11;

    if(etat.cbInQue)
        ReadFile(serie,temp,etat.cbInQue,&nb_lus,0);
    for(int i = 0; i<etat.cbInQue;i++){
        message += QString::number(temp[i],16).toUpper();
       message += " ";
    }
    ui->textEdit->append("Message reçue : "+message);
    QString nb_article = QString::number(temp[9],16).toUpper();
    int nb = nb_article.toInt(&ok,10);
    ui->textEdit->append("Nombre article : "+nb_article+ "\n");
    QString EPC[nb];

   //cut de la trame pour correspondre au ticket
     while(y < nb){
     	unsigned char Data[16];
     	for(int i =0;i<16;i++){
        	 Data[i] = temp[x];
         	x++;
     	}
     	for(int i =11;i<13;i++){
        	EPC[y] += QString::number(Data[i],16).toUpper();
     		EPC[y] += " ";
		}
     ui->textEdit->append(EPC[y]+"\n");
	 y++;
    //x+=sizeof(Data);
	}
    return EPC;
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_pushButton_3_clicked(){
	Inven->stop();
}
