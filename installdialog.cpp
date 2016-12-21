#include "installdialog.h"
#include "ui_installdialog.h"
#include <QPushButton>

InstallDialog::InstallDialog(QWidget *parent,
							 QCheckBox *selectors,
							 AppList *List
							 ) :
	QDialog(parent),
	ui(new Ui::InstallDialog),
	installingText(tr("Installing: ")),
	appSel(selectors),
	appList(List),
	progress(0),
	current(0)
{
	ui->setupUi(this);
	ui->installationLog->hide();
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	ui->installLabel->setText(installingText);

	connect(ui->groupBox, SIGNAL(toggled(bool)), this, SLOT(OnShowLog()));
	connect(&process, SIGNAL(finished(int)), this, SLOT(OnUpdateProgress()));
}

InstallDialog::~InstallDialog()
{
	delete ui;
}

void InstallDialog::install(){
	int nApps = appList->AppsAmount();
	int nAppsToInstall = 0;

	for(int i=0; i<nApps; i++){
		if(appSel[i].isChecked())
			nAppsToInstall++;
	}

	for(int i=0; i<nApps; i++){
		if(appSel[i].isChecked()){
			appsDirs << appList->GetAppValue(APP_DIRS, i);
			appsNames << appList->GetAppValue(APP_NAMES, i);
		}
	}
	StartNextApp();
}

void InstallDialog::OnShowLog(){
	(ui->groupBox->isChecked()) ?
		ui->installationLog->show():
		ui->installationLog->hide();
}

void InstallDialog::OnUpdateProgress(){
	progress = current * 100 / appsDirs.size();
	ui->progressBar->setValue(progress);
	ui->progressBar->update();
	StartNextApp();
}

void InstallDialog::StartNextApp(){
	if(current == appsDirs.size()){
		OnFinishedInstalling();
		return;
	}

	ui->installLabel->setText(installingText + appsNames.at(current));
	ui->installationLog->appendPlainText(tr("Progress: ") +
										 QString("%1").arg(progress) +
										 "%\n" +
										 installingText +
										 appsNames.at(current) +
										 "\n\n");
	QString cmd = APP_DIR_ + NATIVE_SEP_ + appsDirs.at(current);
	process.start(cmd);
	current++;
}

void InstallDialog::OnFinishedInstalling(){
	this->setCursor(QCursor(Qt::ArrowCursor));
	ui->installLabel->setText(tr("Installation Finished"));
	ui->installationLog->appendPlainText(tr("Progress: ") +
										 QString("%1").arg(progress) +
										 tr("\%\nInstallation Finished"));
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	ui->buttonBox->button(QDialogButtonBox::Cancel)->setEnabled(false);
}
