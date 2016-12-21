#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define _VERSION "v.0.2"

#include <QMainWindow>
#include <QGroupBox>
#include <QLayout>
#include <QCheckBox>
#include "applist.h"
#include "installdialog.h"
#include "editcsvlist.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	public slots:
		void Restart();
		void OnAbout();
		void OnSelectAll();
		void OnInvertSelection();
		void OnSelectNone();
		void OnModifyAppList();
		void OnOpenAppFolder();
		void OnInstall();
		
	private:
		Ui::MainWindow *ui;
		AppList *appList;
		QCheckBox *appSel;
		QGroupBox *categoryGroups;
		QVBoxLayout *appLayout;
		InstallDialog *installDialog;
		EditCSVList *editListDialog;
};

#endif // MAINWINDOW_H
