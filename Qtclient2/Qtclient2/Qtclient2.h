#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Qtclient2.h"
#include <QMainWindow>
//#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>


class Qtclient2 : public QMainWindow
{
	Q_OBJECT

public:
	Qtclient2(QWidget *parent = Q_NULLPTR);

	~Qtclient2();

private:
	Ui::Qtclient2Class ui;
	//QTcpSocket *socket; tcp³s½u¥Î
	QNetworkRequest request;
	//QNetworkAccessManager * manager;
	private slots:

	void on_pushButton_Connect_clicked();

	void on_pushButton_Send_clicked();

	void socket_Read_Data();

	void socket_Disconnected();

	void setLight();

	void setUp();

	void showUi(int showUI);

	void onResult(QNetworkReply* reply);
};
