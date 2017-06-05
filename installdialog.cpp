#include "installdialog.h"
#include "ui_installdialog.h"
#include <QPushButton>

InstallDialog::InstallDialog(QWidget *parent,
							 QCheckBox *selectors,
							 AppList *List
							 ) :
	QDialog(parent),
	_ui(new Ui::InstallDialog),
	_installingText(tr("Installing: ")),
	_appSel(selectors),
	_appList(List),
	_progress(0),
	_current(0)
{
	_ui->setupUi(this);
	_ui->installationLog->hide();
	_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	_ui->installLabel->setText(_installingText);

	connect(_ui->groupBox, SIGNAL(toggled(bool)), this, SLOT(OnShowLog()));
	connect(&_process, SIGNAL(finished(int)), this, SLOT(OnUpdateProgress()));
}

InstallDialog::~InstallDialog()
{
	delete _ui;
}

void InstallDialog::install(){
	int nApps = _appList->AppsAmount();
	int nAppsToInstall = 0;

	for(int i=0; i<nApps; i++){
		if(_appSel[i].isChecked())
			nAppsToInstall++;
	}

	for(int i=0; i<nApps; i++){
		if(_appSel[i].isChecked()){
			_appsDirs << _appList->GetAppValue(APP_DIRS, i);
			_appsNames << _appList->GetAppValue(APP_NAMES, i);
		}
	}
	StartNextApp();
}

void InstallDialog::OnShowLog(){
	(_ui->groupBox->isChecked()) ?
		_ui->installationLog->show():
		_ui->installationLog->hide();
}

void InstallDialog::OnUpdateProgress(){
	_progress = _current * 100 / _appsDirs.size();
	_ui->progressBar->setValue(_progress);
	_ui->progressBar->update();
	StartNextApp();
}

void InstallDialog::StartNextApp(){
	if(_current == _appsDirs.size()){
		_OnFinishedInstalling();
		return;
	}

	_ui->installLabel->setText(_installingText + _appsNames.at(_current));
	_ui->installationLog->appendPlainText(tr("Progress: ") +
										 QString("%1").arg(_progress) +
										 "%\n" +
										 _installingText +
										 _appsNames.at(_current) +
										 "\n\n");
	QString cmd = APP_DIR_ + NATIVE_SEP_ + _appsDirs.at(_current);
	_process.start(cmd);
	_current++;
}

void InstallDialog::_OnFinishedInstalling(){
	this->setCursor(QCursor(Qt::ArrowCursor));
	_ui->installLabel->setText(tr("Installation Finished"));
	_ui->installationLog->appendPlainText(tr("Progress: ") +
										 QString("%1").arg(_progress) +
										 tr("\%\nInstallation Finished"));
	_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	_ui->buttonBox->button(QDialogButtonBox::Cancel)->setEnabled(false);
}
