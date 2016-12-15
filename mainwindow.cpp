#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "applist.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	AppList appList;
	if(appList.ReadAppList())
		qDebug("Cool");

}

MainWindow::~MainWindow()
{
	delete ui;
}
