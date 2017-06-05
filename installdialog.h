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
		Ui::InstallDialog *_ui;
		const QString _installingText;
		QCheckBox *_appSel;
		AppList *_appList;

		int _progress;
		int _current;
		QStringList _appsDirs;
		QStringList _appsNames;
		QProcess _process;

		void _OnFinishedInstalling();

};

#endif // INSTALLDIALOG_H
