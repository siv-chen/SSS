#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtServer2.h"
#include <QTcpServer>
#include <QTcpSocket>

class QtServer2 : public QMainWindow
{
	Q_OBJECT

public:
	QtServer2(QWidget *parent = Q_NULLPTR);
	~QtServer2();

private:
	Ui::QtServer2Class ui;
	QTcpServer* server;
	QTcpSocket* socket;

	private slots:
	void on_pushButton_Listen_clicked();

	void on_pushButton_Send_clicked();

	void server_New_Connect();

	void socket_Read_Data();

	void socket_Disconnected();

	void showPic();

	void lightRun();

	void modbusWrite(int w1, int w2, int w3);

	void modbusRead();

	void reConnect();

	void httpServer();

	static void ev_handler(struct mg_connection *nc, int ev, void *p);

	static void SendRsp(struct mg_connection *nc, std::string rsp);



};
