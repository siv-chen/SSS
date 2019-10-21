#include "Qtclient2.h"
#include "ui_Qtclient2.h"
//#include <QNetworkAccessManager>
//#include <QNetworkRequest>
//#include <QNetworkReply>
//#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


int r = 0;
//QTimer *timer1 = new QTimer;
//QNetworkAccessManager* manager = new QNetworkAccessManager;
QNetworkAccessManager *manager = new QNetworkAccessManager;
QNetworkAccessManager networkManager;



Qtclient2::Qtclient2(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);


	QPixmap *pixmap = new QPixmap("D:/ALEX/QT/lightbulb_off.png");
	QPixmap fitpixmap = pixmap->scaled(271, 201, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui.lablight->setPixmap(fitpixmap);

	QIcon ButtonIcon("D:/ALEX/QT/button_release.png");
	ui.PB1->setIcon(ButtonIcon);

	QIcon ButtonIcon1("D:/ALEX/QT/switches-off.png");
	ui.PB2->setIcon(ButtonIcon1);


	//以下TCP
	//socket = new QTcpSocket();
	//連接槽
	//QObject::connect(socket, &QTcpSocket::readyRead, this, &Qtclient2::socket_Read_Data);
	//QObject::connect(socket, &QTcpSocket::disconnected, this, &Qtclient2::socket_Disconnected);
	//ui.pushButton_Send->setEnabled(true);
	//ui.lineEdit_IP->setText("127.0.0.1");
	//ui.lineEdit_Port->setText("8888");
	//on_pushButton_Connect_clicked();

	//以下HTTP
	/*QObject::connect(timer1, SIGNAL(timeout()), this, SLOT(httpClient()));
	timer1->start(10);*/
	request.setUrl(QUrl("http://127.0.0.1:8000/all"));
	QNetworkReply* currentReply = networkManager.get(request);
	connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResult(QNetworkReply*)));

}

Qtclient2::~Qtclient2()
{
	delete manager;
	//delete this->socket;
	//delete ui;
}

void Qtclient2::on_pushButton_Connect_clicked()
{
	//if (ui.pushButton_Connect->text() == tr("Connect"))
	//{
	//	QString IP;
	//	int port;

	//	//獲得ip地址
	//	IP = ui.lineEdit_IP->text();
	//	//IP = ("172.0.0.1");
	//	//獲取port
	//	//port = ui.lineEdit_Port->text().toInt();
	//	port = 8888;

	//	//取消連結
	//	socket->abort();
	//	//連接server
	//	socket->connectToHost(IP, port);

	//	//等待連接成功
	//	if (!socket->waitForConnected(30000))
	//	{
	//		qDebug() << "Connection failed!";
	//		return;
	//	}
	//	qDebug() << "Connect successfully!";

	//	//
	//	ui.pushButton_Send->setEnabled(true);
	//	
	//	ui.pushButton_Connect->setText("DisC");
	//}
	//else
	//{
	//	//段開連接
	//	socket->disconnectFromHost();
	//	//
	//	ui.pushButton_Connect->setText("Connect");
	//	ui.pushButton_Send->setEnabled(false);
	//}
}

void Qtclient2::on_pushButton_Send_clicked()
{
	/*QString strSend;
	strSend = QString::number(r);
	QByteArray arrSend = strSend.toLocal8Bit();
	socket->write(arrSend);
	socket->flush();*/
	
	request.setUrl(QUrl("http://127.0.0.1:8000/all"));
	QNetworkReply* currentReply = networkManager.get(request);
	
}

void Qtclient2::socket_Read_Data()
{


	//以下TCP
	//QByteArray buffer;
	////
	//buffer = socket->readAll();
	//if (!buffer.isEmpty())
	//{
	//	QString str = ui.textEdit_Recv->toPlainText();
	//	str = tr(buffer);
	//	//
	//	ui.textEdit_Recv->setText(str);

	//	if (str.section(',',0,0) == "1")//旋鈕左
	//	{
	//		showUi(1);
	//	}
	//	else if (str.section(',', 0, 0) == "2")//旋鈕右
	//	{
	//		showUi(2);
	//	}

	//	if ((str.section(',', 1, 1) == "1")&&(r!=1))//按鈕壓下
	//	{
	//		showUi(6);
	//		r = 3;
	//	}
	//	else if ((str.section(',', 1, 1) == "0")&&(r!=1))//按鈕放開
	//	{
	//		showUi(5);
	//		r = 2;
	//	}

	//	if (str.section(',', 2, 2) == "0")//燈暗
	//	{
	//		showUi(3);
	//	}
	//	else if (str.section(',', 2, 2) == "1")//燈亮
	//	{
	//		showUi(4);
	//	}

	//	str.clear();
	//	
	//}
	//--------------------------------------------------------
	//以下HTTP
	QString str = ui.textEdit_Recv->toPlainText();
	QJsonParseError jsonError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(str.toLatin1(), &jsonError);

	/*if (jsonError.error != QJsonParseError::NoError)
	{
		ui.textEdit_Send->setText("JSON格式錯誤");
	}
	else
	{
		ui.textEdit_Send->setText("JSON格式正常");
	}*/
	QJsonObject obj = jsonDoc.object();
	QString knob = obj.value("knob").toString();
	QString buttonRP = obj.value("buttonRP").toString();
	QString lightONOFF = obj.value("lightONOFF").toString();
	//QString str2;
		if (knob == "1")//旋鈕左
		{
			showUi(1);
		}
		else if (knob == "2")//旋鈕右
		{
			showUi(2);
		}

		if ((buttonRP == "1")&&(r!=1))//按鈕壓下
		{
			showUi(6);
			r = 3;
		}
		else if ((buttonRP == "0")&&(r!=1))//按鈕放開
		{
			showUi(5);
			r = 2;
		}

		if (lightONOFF == "0")//燈暗
		{
			showUi(3);
		}
		else if (lightONOFF == "1")//燈亮
		{
			showUi(4);
		}

		str.clear();
}

void Qtclient2::socket_Disconnected()
{
	////
	//ui.pushButton_Send->setEnabled(false);
	////
	//ui.pushButton_Connect->setText("Connect");
	//qDebug() << "Disconnected!";
}

void Qtclient2::onResult(QNetworkReply* reply)
{
	on_pushButton_Send_clicked();
	
	if (reply->error()) {
		qDebug() << reply->errorString();
		QString errr = reply->errorString();
		ui.textEdit_Recv->setText(errr);
		ui.label_5->setText(QStringLiteral("Server端連線狀態:Unconnect!!"));
		return;
	}
	ui.label_5->setText(QStringLiteral("Server端連線狀態:connect!!"));
	QString answer = reply->readAll();
	ui.textEdit_Recv->setText(answer);

	//delete networkManager.get(request);
	delete reply;

	socket_Read_Data();
	

}


//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Qtclient2::setLight() {
	if (r != 3)
	{
		r = 1;
		showUi(6);
		//on_pushButton_Send_clicked();
		QString js = "";
		// 轉為 UTF8 
		QByteArray postData = js.toUtf8();
		// 設定post位置
		QUrl serviceUr =  QUrl("http://127.0.0.1:8000/r1");
		// 創建請求對象
		QNetworkRequest request(serviceUr);
		// 設至內容類型 application/json
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
		// 
		manager->post(request, postData);

		//delete manager;
		

	}
}

void Qtclient2::setUp() {
	if (r != 3)
	{
		r = 2;
		showUi(5);
		//on_pushButton_Send_clicked();
		QString js = "";
		// 轉為 UTF8 
		QByteArray postData = js.toUtf8();
		// 設定post位置
		QUrl serviceUr = QUrl("http://127.0.0.1:8000/r2");
		// 創建請求對象
		QNetworkRequest request(serviceUr);
		// 設至內容類型 application/json
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
		// 
		//QNetworkReply* currentReply = networkManager.post(request, postData);
		manager->post(request, postData);
	}
}

void Qtclient2::showUi(int showUI) {

	if (showUI == 1)      //switch左
	{
		QIcon ButtonIcon("D:/ALEX/QT/switches-off.png");
		ui.PB2->setIcon(ButtonIcon);
	}
	else if (showUI == 2)   //switch右
	{

		QIcon ButtonIcon("D:/ALEX/QT/switches_on.png");
		ui.PB2->setIcon(ButtonIcon);

	}
	else if (showUI == 3) //燈暗
	{
		QPixmap *pixmap = new QPixmap("D:/ALEX/QT/lightbulb_off.png");
		QPixmap fitpixmap = pixmap->scaled(271, 201, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		ui.lablight->setPixmap(fitpixmap);
	}
	else if (showUI == 4) //燈亮
	{
		QPixmap *pixmap = new QPixmap("D:/ALEX/QT/lightbulb_on.png");
		QPixmap fitpixmap = pixmap->scaled(271, 201, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		ui.lablight->setPixmap(fitpixmap);
	}

	else if (showUI == 5) //按鈕放開
	{
		QIcon ButtonIcon2("D:/ALEX/QT/button_release.png");
		ui.PB1->setIcon(ButtonIcon2);
	}

	else if (showUI == 6)//按鈕壓下
	{
		QIcon ButtonIcon("D:/ALEX/QT/button_press.png");
		ui.PB1->setIcon(ButtonIcon);
	}
	else if (showUI == 7)//show net disconnect
	{
		//ui.labtxt->setText(QStringLiteral("連線狀態:Disconnect!!"));
	}
	else if (showUI == 8)//show net connect
	{
		//ui.labtxt->setText(QStringLiteral("連線狀態:Connect!!"));
	}

}

