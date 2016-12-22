#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTranslator T;
	QSettings settings("Resources/config/config.ini",
					   QSettings::IniFormat);
	QString langFile = "Resources/langs/";

	langFile += settings.value("Language/file").toString();
	/* Uncomment to test config keys
	foreach(QString key, settings.allKeys())
		qInfo(key.toStdString().c_str());
	*/
	T.load(langFile);

	a.installTranslator(&T);

	MainWindow w;
	if(settings.value("MainWindow/maximized").toBool())
		w.showMaximized();
	else
		w.show();


	return a.exec();
}
