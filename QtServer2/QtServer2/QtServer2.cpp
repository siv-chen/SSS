#include "QtServer2.h"
#include "ui_QtServer2.h"
#include "modbus.h"
#include <qpixmap.h>
#include <qtimer.h>
#include <qwidget.h>
#include <string.h>
//#include <iostream>
//-----------------------------
#include "mongoose.h"
static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

struct mg_mgr mgr;
struct mg_connection *nc;

QTimer *timer1 = new QTimer;
QTimer *timer2 = new QTimer;
QTimer *timer3 = new QTimer;
int ret = -1;
int xx = 0, yy = 0;
int a = 0, r = 0, s = 0, c = 0;
int knob = 0, buttonRP = 0, lightONOFF = 0, pressON = 0;
int time_count = 0;
uint8_t  tab_bits[32];
uint16_t tab_reg[32];
uint16_t dest[10];
modbus_t *mb;



QtServer2::QtServer2(QWidget *parent)
	: QMainWindow(parent)
{
	mb = modbus_new_tcp("172.16.17.123", 502);

	if (modbus_connect(mb) != -1) {
		modbus_write_register(mb, 0x1120, 0);
		modbus_write_register(mb, 0x1121, 0xBECF);
		modbus_write_register(mb, 0x1121, 0xAFFE);
		//modbus_write_register(mb, 0x1122,  0);
	}

	modbus_write_bit(mb, 4, 0);

	ui.setupUi(this);
	//ui.lineEdit_Port->setText("8888");
	ui.pushButton_Send->setEnabled(false);

	//--------------------------------------
	

	mg_mgr_init(&mgr, NULL);
	nc = mg_bind(&mgr, s_http_port, ev_handler);
	if (nc == NULL) {
		//printf("Failed to create listener\n");
		//return 1;
	}

	// Set up HTTP server parameters
	mg_set_protocol_http_websocket(nc);
	//s_http_server_opts.enable_directory_listing = "yes";

	//--------------------------------------

	

	//tcp連接槽
	//server = new QTcpServer();
	/*connect(server, &QTcpServer::newConnection, this, &QtServer2::server_New_Connect);
	on_pushButton_Listen_clicked();*/

	QObject::connect(timer1, SIGNAL(timeout()), this, SLOT(showPic()));
	QObject::connect(timer2, SIGNAL(timeout()), this, SLOT(reConnect()));
	QObject::connect(timer3, SIGNAL(timeout()), this, SLOT(httpServer()));
	timer1->start(50);
	timer2->stop();
	timer3->start(0);
	
}
//--------------------------------------------------------------------------------
//----------------------------

QtServer2::~QtServer2()
{
	//server->close();
	//server->deleteLater();
	//delete ui;
}

void QtServer2::on_pushButton_Listen_clicked()
{
	//if (ui.pushButton_Listen->text() == tr("listen"))
	//{
		//從輸入框獲得port
		//int port = ui.lineEdit_Port->text().toInt();
		  int port = 8888;
		//listen指定port
		if (!server->listen(QHostAddress::Any, port))
		{
			//若出錯輸出錯誤訊息
			qDebug() << server->errorString();
			return;
		}
		//
		ui.pushButton_Listen->setText("dislisten");
		qDebug() << "Listen succeessfully!";
	//}
	//else
	//{
	//	
	//	//
	//	if(socket->state() == QAbstractSocket::ConnectedState)
	//	{
	//	//
	//	  socket->disconnectFromHost();
	//	}
	//	
	//	socket->abort();
	//	//取消listen
	//	server->close();
	//	//
	//	ui.pushButton_Listen->setText("listen");
	//	//
	//	ui.pushButton_Send->setEnabled(false);
	//}

}

void QtServer2::on_pushButton_Send_clicked()
{
	if (ui.label_2->text()==("TCPconnect"))
	{
	
	//文字框內容發送
	//socket->write(ui.textEdit_Send->toPlainText().toLatin1());
	/*QString str1,str2,str3,str4;
	str1 = QString::number(knob);
	str2 = QString::number(buttonRP);
	str3 = QString::number(lightONOFF);
	str4 = str1+","+str2+","+str3;
	QByteArray st = str4.toLocal8Bit();
	socket->write(st);
	socket->flush();*/
	}
}

void QtServer2::server_New_Connect()
{
	////接收客戶端連接
	//socket = server->nextPendingConnection();
	////連接qtTCPsocket讀取新數據
	//QObject::connect(socket, &QTcpSocket::readyRead, this, &QtServer2::socket_Read_Data);
	//QObject::connect(socket, &QTcpSocket::disconnected, this, &QtServer2::socket_Disconnected);
	////
	//ui.pushButton_Send->setEnabled(true);
	//ui.label_2->setText("TCPconnect");

	//qDebug() << "A Client connect!";
}

void QtServer2::socket_Read_Data()
{
	//QByteArray buffer;
	////讀取緩衝區數據
	//buffer = socket->readAll();
	//if (!buffer.isEmpty())
	//{
	//	QString str = ui.textEdit_Recv->toPlainText();
	//	//str += tr(buffer);
	//	str = tr(buffer);
	//	//刷新顯示
	//	ui.textEdit_Recv->setText(str);
	//	r = str.toInt();
	//}
}

void QtServer2::socket_Disconnected()
{
	////
	//ui.pushButton_Send->setEnabled(false);
	//ui.label_2->setText("TCPdisconnect");
	//qDebug() << "Disconnected!";
}
//-------------------------------------------------------------------

//-------------------------------------------------------------------
//以下HTTP傳輸協定
void QtServer2::SendRsp(mg_connection *nc, std::string rsp)
{
	// 必須先傳送header
	mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
	// 以json形式返回
	mg_printf_http_chunk(nc, "{\"knob\":\"%s\"}", rsp.c_str());
	// 傳送空白字元快，結束當前響應
	mg_send_http_chunk(nc, "", 0);
}

void QtServer2::ev_handler(mg_connection *nc, int ev, void *p) {
	struct http_message *hm = (struct http_message *) p;
	
	switch (ev) {

		case MG_EV_HTTP_REQUEST:
			if (mg_vcmp(&hm->uri, "/all") == 0) {
				if (mg_vcmp(&hm->method, "GET") == 0) {
					QString str1, str2, str3, str4;
					str1 = QString::number(knob);
					str2 = QString::number(buttonRP);
					str3 = QString::number(lightONOFF);
					str4 = str1 + "\"," +"\"buttonRP\":"+"\""+str2 + "\"," + "\"lightONOFF\":" +"\""+str3;
					std::string strSum = str4.toLocal8Bit().constData();
			        SendRsp(nc, strSum); /* Handle RESTful call */
			    }
				
			}
			if (mg_vcmp(&hm->uri, "/r1") == 0) {

				  if (mg_vcmp(&hm->method, "POST") == 0) {
					  r = 1;
					  SendRsp(nc, "");
				  }
			}
			if (mg_vcmp(&hm->uri, "/r2") == 0) {

				  if (mg_vcmp(&hm->method, "POST") == 0) {
					  r = 2;
					  SendRsp(nc, "");
				  }
			}

			break;
			default:
			break;

		}
	
	
}

void QtServer2::httpServer() {

	mg_mgr_poll(&mgr, 0);

}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//以下MODBUS功能
void QtServer2::reConnect() {
	modbus_close(mb);
	modbus_free(mb);
	mb = modbus_new_tcp("172.16.17.123", 502);
	modbus_connect(mb);
	ret = modbus_read_registers(mb, 0x800, 1, tab_reg);
	if (ret != -1)
	{
		//showUi(8);
		timer1->start(50);

		timer2->stop();

	}

}

void QtServer2::modbusRead() {

	ret = modbus_read_input_bits(mb, 0, 21, tab_bits);
	ret = modbus_read_registers(mb, 0x800, 1, tab_reg);

	if (tab_bits[20] == 1)
	{
		knob = 1;//旋鈕左
	}
	else if (tab_bits[17] == 1)
	{
		knob = 2;//旋鈕右
	}

	if (tab_bits[1] == 1)
	{
		buttonRP = 1; //按鈕壓
	}
	else if (tab_bits[1] == 0)
	{
		buttonRP = 0; //按鈕放
	}

	if (tab_reg[0] != 16)
	{
		lightONOFF = 0;//燈暗
	    
	}
	else
	{
		lightONOFF = 1;//燈亮
		
		
	}


}

void QtServer2::modbusWrite(int w1, int w2, int w3) {
	if (w1 = 1)
	{
		modbus_write_bit(mb, w2, w3);
	}


}


void QtServer2::lightRun() {
	if (knob == 1)////////旋鈕左
	{
		//showUi(1);
		if (yy == 1)
		{
			modbusWrite(1, 4, 0);
			//showUi(3);
			yy = 0;
		}

		if ((buttonRP == 1) && (s == 1))//實體按鈕按下
		{
			//showUi(6);//按鈕按下

			if ((xx == 1))
			{
				if (lightONOFF == 0)//如果燈暗
				{
					modbusWrite(1, 4, 1);
					//showUi(4);//燈亮
					xx = 2;
				}
				else
				{
					modbusWrite(1, 4, 0);
					//showUi(3);//登暗
					xx = 2;
				}
			}
		}
		else if ((r == 1) && (s == 0)) //UI按鈕觸發
		{
			if ((xx == 1))
			{

				if (lightONOFF == 0)//如果燈暗
				{
					modbusWrite(1, 4, 1);
					//showUi(4);//燈亮
					xx = 2;
				}
				else
				{
					modbusWrite(1, 4, 0);
					//showUi(3);//登暗
					xx = 2;
				}

			}

		}
		/*else if ((r == 2) && (s == 0)) //UI按鈕觸發
		{
		if ((xx == 1))
		{

		if (lightONOFF == 0)//如果燈暗
		{
		modbusWrite(1, 4, 1);
		showUi(4);//燈亮
		xx = 0;
		}
		else
		{
		modbusWrite(1, 4, 0);
		showUi(3);//登暗
		xx = 0;
		}

		}

		}*/
		else if ((buttonRP == 0) && (s == 1))
		{
			//showUi(5);
		}
	}
	else if (knob == 2)////////旋鈕右
	{
		//showUi(2);

		if (pressON == 1)
		{
			if ((buttonRP == 1) && (s == 1))//實體按鈕按下
			{
				//showUi(6);
				if (xx == 1)
				{
					if (yy == 0)//未閃爍
					{
						xx = 2;
						yy = 1;

					}
					else
					{
						xx = 2;
						yy = 0;
					}

				}

			}

		}
		else
		{
			if ((buttonRP == 0) && (s == 1))//實體按鈕按下
			{

				if (xx == 1)
				{
					if (yy == 0)//未閃爍
					{
						xx = 0;
						yy = 1;

					}
					else
					{
						xx = 0;
						yy = 0;
					}

				}

			}

		}

		if (pressON == 1)
		{
			if ((r == 1) && (s == 0))//UI按鈕按下觸發
			{
				if (xx == 1)
				{
					if (yy == 0)//未閃爍
					{
						xx = 2;
						yy = 1;
					}
					else
					{
						xx = 2;
						yy = 0;
					}
				}
			}
		}
		else
		{
			if ((r == 2) && (s == 0))//UI按鈕放開觸發
			{
				if (xx == 1)
				{
					if (yy == 0)//未閃爍
					{
						xx = 0;
						yy = 1;
					}
					else
					{
						xx = 0;
						yy = 0;
					}
				}
			}
		}
		if (yy == 1)//閃爍燈循環
		{
			time_count = time_count + 1;
			if (time_count % 15 == 0)//每1000ms
			{
				if (lightONOFF == 0)
				{
					modbusWrite(1, 4, 1);
					//showUi(4);

				}
				else
				{
					modbusWrite(1, 4, 0);
					//showUi(3);
				}
				time_count = 0;
			}

		}
		else
		{
			modbusWrite(1, 4, 0);
			//showUi(3);
		}


	}


}

void QtServer2::showPic() {

	modbusRead();
	//on_pushButton_Send_clicked();
	//連線偵測
	if (ret == -1) {

		{
			//showUi(7);
			timer2->start(50);
			timer1->stop();
		}


	}
	//.........................................................

	if ((buttonRP == 1) && (r == 1))
	{
		c = 1;
	}
	if ((buttonRP == 0) && (r == 2))
	{
		c = 0;
	}

	if ((buttonRP == 0) && (r == 0))
	{
		c = 0;
	}

	pressON = 1;

	//.........................................................
	if (((buttonRP == 0) && (c == 0)) || ((s == 0) && (c == 1)))
	{
		if (pressON == 1)
		{
			if ((r == 2) && (xx == 2))//按下觸發
			{
				//showUi(5);//UI按鈕放開
				xx = 0;
				s = 0;
				c = 3;
			}
			if ((r == 1) && (xx == 0))
			{
				//showUi(6);//UI按鈕壓下
				xx = 1;
				s = 0;
			}
		}
		else
		{
			if ((r == 2) && (xx == 3))//放開觸發
			{
				//showUi(5);//UI按鈕放開
				xx = 1;
				s = 0;
			}
			if ((r == 1) && (xx == 0))
			{
				//showUi(6);//UI按鈕壓下
				xx = 3;
				s = 0;
				c = 3;
			}

		}

	}

	if (((r != 1) && (c == 0)) || ((s == 1) && (c == 1)))
	{
		if (pressON == 1)
		{
			if ((buttonRP == 1) && (xx == 0))//按下觸發
			{
				//showUi(6);
				xx = 1;
				s = 1;
			}
			if ((buttonRP == 0) && (xx == 2))
			{
				//showUi(5);
				xx = 0;
				s = 1;
				c = 3;
			}

		}
		else
		{
			if ((buttonRP == 1) && (xx == 0))//放開觸發
			{
				//showUi(6);
				xx = 3;
				s = 1;
				//c = 3;
			}
			if ((buttonRP == 0) && (xx == 3))
			{
				//showUi(5);
				xx = 1;
				s = 1;
				c = 3;
			}

		}

	}
	//.............................................test

	lightRun();

	//.....................................................




}




