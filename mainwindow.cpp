#include "mainwindow.h"
#include "ui_mainwindow.h"

#define START	0x02
#define LONG	0x00
#define LONG2	0x08
#define CTRL	0x00
#define TIMER	4000
#define MASK1	0xFFFF
#define MASK2	0x00FF

#define BAUDRATE	115200
#define BYTESIZE	8


using namespace std;

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow){
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

//------------------------------- Calcul du CRC 16 CCITT --------------

static unsigned char* calc_crc(unsigned char donnee[], unsigned int size){
	unsigned int crc_init = MASK1;
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
	crc[1]= crc_init & MASK2;
	return crc;
}

//TestLecteur Commande Etat du lecteur
void MainWindow::on_pushButton_clicked(){
	const int size = 12;
	unsigned char data[50];
	unsigned long data_size;
	unsigned char* buffer;
	QString envoi;

	Data[0] = START;
	Data[1] = LONG;
	Data[2] = LONG2;
	Data[3] = CTRL;
	Data[4] = CTRL;
	//Commande
	Data[5] = 0x00;
	Data[6] = 0x00;
	Data[7] = 0x00;
	Data[8] = 0x24;
	Data[9] = 0xAA;
	Data[10] = 0x55;
	Data[11] = 0x00;
	Data[12] = 0x00;
	//Add CRC in the trame
	buffer = calc_crc(data, size);
	Data[13] = buffer[0];
	Data[14] = buffer[1];
	
	data_size = sizeof(data);
	for(int i = 0; i < data_size ; i++){
		envoi += QString::number(data[i], 16).toUpper();
		envoi += " ";
	}
	
	ui->label_3->setText(envoi);
	WriteFile(serie,Data,taille, &taille, 0);
	Anten->start(TIMER);
}

void MainWindow::Get_HealthParameters(){
	COMSTAT etat;
	unsigned long nb_lus = 0;
	QString message;

	ClearCommError(serie,0,&etat);
	if(etat.cbInQue)
		ReadFile(serie,temp,etat.cbInQue,&nb_lus,0);

	for(int i = 0; i < etat.cbInQue; i++){
		message += QString::number(temp[i], 16).toUpper();
		message += " ";
	}

	ui->lineEdit->setText(message);
	Anten->stop();
}

void MainWindow::on_pushButton_2_clicked(){
	const int size = 12;
	unsigned char data[100];
	unsigned char* buffer;
	unsigned long data_size;
	QString envoi;

	data[0] = START;
	data[1] = LONG;
	data[2] = LONG2;
	data[3] = CTRL;
	data[4] = CTRL;
	//Commande
	data[5] = 0x00;
	data[6] = 0x08;
	data[7] = 0x00;
	data[8] = 0x01;
	data[9] = 0xAA;
	data[10] = 0x55;
	data[11] = 0x00;
	data[12] = 0x00;
	//Add CRC in the trame
	buffer = calc_crc(Data, size);
	data[13] = buffer[0];
	data[14] = buffer[1];

	data_size = sizeof(data);
	for(int i = 0; i < data_size; i++){
		envoi += QString::number(data[i], 16).toUpper();
		envoi += " ";
	}

	WriteFile(serie, data, data_size, &data_size, 0);
	Inven->start(TIMER);
}

QString* MainWindow::Inventory(){
	COMSTAT etat;
	ClearCommError(serie,0,&etat);
	unsigned long nb_read;
	QString message;
	bool ok;
	int x = 10;
	int y = 0;
	QString nb_article;
	int nb;
	size_EPC = 11;

	if(etat.cbInQue)
		ReadFile(serie,temp,etat.cbInQue,&nb_read,0);
		for(int i = 0; i<etat.cbInQue;i++){
			message += QString::number(temp[i],16).toUpper();
			message += " ";
		}

	ui->textEdit->append("Message reçue : "+message);
	nb_article = QString::number(temp[9],16).toUpper();
	nb = nb_article.toInt(&ok,10);
	ui->textEdit->append("Nombre article : "+nb_article+ "\n");
	QString EPC[nb];
	
	//cut de la trame pour correspondre au ticket
	while(y < nb){
		unsigned char data[16];

		for(int i =0;i<16;i++){
			data[i] = temp[x];
			x++;
		}
		for(int i =11;i<13;i++){
			EPC[y] += QString::number(data[i],16).toUpper();
			EPC[y] += " ";
		}

	ui->textEdit->append(EPC[y]+"\n");
	y++;
	//x+=sizeof(data);
	}

	return EPC;
}

void MainWindow::on_pushButton_3_clicked(){
	Inven->stop();
}

MainWindow::~MainWindow(){
	delete ui;
}

