#include <QMessageBox>
#include "embeddediconextractor.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	_ui(new Ui::MainWindow),
	_appList(new AppList),
	_editListDialog(new EditCSVList)
{
	_ui->setupUi(this);

	if(!_appList->ReadAppList()){

		QMessageBox::warning(0,tr("Error reading app list"),tr("File \"List.csv\" Invalid or Not Found"));
		return;
	}

	//create the category widgets
	{
		int categoriesSize = _appList->CategoriesSize();
		int appsAmount = _appList->AppsAmount();
		int cols = categoriesSize - categoriesSize / 2;

		_categoryGroups = new QGroupBox[categoriesSize];
		_appLayout = new QVBoxLayout[categoriesSize];
		for(int i=0, row=0; i<categoriesSize; row++){
			for(int col=0; col<cols && i<categoriesSize; col++, i++){
				_categoryGroups[i].setTitle(tr(_appList->GetCategoryName(i).toStdString().c_str()));
				_ui->categoriesLayout->addWidget(&_categoryGroups[i], row, col);
				_categoryGroups[i].setLayout(&_appLayout[i]);
			}
		}

		_appSel = new QCheckBox[appsAmount];
		for(int i=0; i<appsAmount; i++){
			_appSel[i].setText(_appList->GetAppValue(APP_NAMES, i));
			_appSel[i].setIcon(QIcon(EmbeddedIconExtractor(APP_DIR_ + NATIVE_SEP_ + _appList->GetAppValue(APP_DIRS,i)).extract()));
			_appLayout[_appList->GetAppCategoryIndex(i)].addWidget(&_appSel[i]);
		}
	}

	connect(_ui->actionExit, &QAction::triggered, this, &MainWindow::close);
	connect(_ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(OnAbout()));
	connect(_ui->actionInvert_Selection, SIGNAL(triggered(bool)), this, SLOT(OnInvertSelection()));
	connect(_ui->actionSelect_All, SIGNAL(triggered(bool)), this, SLOT(OnSelectAll()));
	connect(_ui->actionSelect_None, SIGNAL(triggered(bool)), this, SLOT(OnSelectNone()));
	connect(_ui->actionModify_App_List, SIGNAL(triggered(bool)), this, SLOT(OnModifyAppList()));
	connect(_ui->actionOpen_App_Folder, SIGNAL(triggered(bool)), this, SLOT(OnOpenAppFolder()));
	connect(_ui->installButton, SIGNAL(clicked(bool)), this, SLOT(OnInstall()));
	connect(_editListDialog, SIGNAL(destroyed(QObject*)), this, SLOT(Restart()));
}

MainWindow::~MainWindow()
{
	delete _ui;
}

void MainWindow::Restart(){
	QProcess::startDetached(QCoreApplication::applicationFilePath());
	close();
}

void MainWindow::OnAbout(){
	QMessageBox::about(0, tr("About AppInstaller"),
					   tr("AppInstaller ") + APP_VERSION + tr("\n\n"\
					   "\tThis program executes in sequence the apps selected by the user "\
					   "in order to install them in a more comfortable manner."));
}

void MainWindow::OnSelectAll(){
	int nApps = _appList->AppsAmount();
	for(int i=0; i<nApps; i++){
		_appSel[i].setChecked(true);
	}
}

void MainWindow::OnInvertSelection(){
	int nApps = _appList->AppsAmount();
	for(int i=0; i<nApps; i++){
		_appSel[i].toggle();
	}
}

void MainWindow::OnSelectNone(){
	int nApps = _appList->AppsAmount();
	for(int i=0; i<nApps; i++){
		_appSel[i].setChecked(false);
	}
}

void MainWindow::OnModifyAppList(){
	_editListDialog->show();
}

void MainWindow::OnOpenAppFolder(){
	QStringList args;
	args << APP_DIR_;
	QProcess::startDetached("explorer", args);
}

void MainWindow::OnInstall(){
	_installDialog = new InstallDialog(0, _appSel, _appList);
	_installDialog->show();
	_installDialog->install();
}
