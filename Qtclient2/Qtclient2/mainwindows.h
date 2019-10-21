#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
	class Qtclient2;
}

class Qtclient2 : public QMainWindow
{
	Q_OBJECT

public:
	explicit Qtclient2(QWidget *parent = 0);
	~Qtclient2();


private:
	Ui::Qtclient2Class ui;
	QTcpSocket *socket;
};

#endif // MAINWINDOW_H