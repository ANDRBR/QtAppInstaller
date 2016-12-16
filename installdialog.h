#ifndef INSTALLDIALOG_H
#define INSTALLDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QCheckBox>
#include "applist.h"

namespace Ui {
	class InstallDialog;
}

class InstallDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit InstallDialog(QWidget *parent = 0,
							   QCheckBox *selectors = NULL,
							   AppList *List = new AppList
							   );
		~InstallDialog();
		void install();
		void StartNextApp();

	public slots:
		void OnShowLog();
		void OnUpdateProgress();

	private:
		Ui::InstallDialog *ui;
		const QString installingText;
		QCheckBox *appSel;
		AppList *appList;

		int progress;
		int current;
		QStringList appsDirs;
		QStringList appsNames;
		QProcess process;

		void OnFinishedInstalling();

};

#endif // INSTALLDIALOG_H
