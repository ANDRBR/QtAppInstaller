#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	appList(new AppList),
	editListDialog(new EditCSVList)
{
	ui->setupUi(this);

	if(!appList->ReadAppList()){

		QMessageBox::warning(0,tr("Error reading app list"),tr("File \"List.csv\" Invalid or Not Found"));
		return;
	}

	//create the category widgets
	{
		int categoriesSize = appList->CategoriesSize();
		int appsAmount = appList->AppsAmount();
		int cols = categoriesSize - categoriesSize / 2;

		categoryGroups = new QGroupBox[categoriesSize];
		appLayout = new QVBoxLayout[categoriesSize];
		for(int i=0, row=0; i<categoriesSize; row++){
			for(int col=0; col<cols && i<categoriesSize; col++, i++){
				categoryGroups[i].setTitle(tr(appList->GetCategoryName(i).toStdString().c_str()));
				ui->categoriesLayout->addWidget(&categoryGroups[i], row, col);
				categoryGroups[i].setLayout(&appLayout[i]);
			}
		}

		appSel = new QCheckBox[appsAmount];
		for(int i=0; i<appsAmount; i++){
			appSel[i].setText(appList->GetAppValue(APP_NAMES, i));
			appSel[i].setIcon(QIcon(QString(ICON_DIR_).append(appList->GetAppValue(APP_ICONS, i))));
			appLayout[appList->GetAppCategoryIndex(i)].addWidget(&appSel[i]);
		}
	}

	connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
	connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(OnAbout()));
	connect(ui->actionInvert_Selection, SIGNAL(triggered(bool)), this, SLOT(OnInvertSelection()));
	connect(ui->actionSelect_All, SIGNAL(triggered(bool)), this, SLOT(OnSelectAll()));
	connect(ui->actionSelect_None, SIGNAL(triggered(bool)), this, SLOT(OnSelectNone()));
	connect(ui->actionModify_App_List, SIGNAL(triggered(bool)), this, SLOT(OnModifyAppList()));
	connect(ui->actionOpen_App_Folder, SIGNAL(triggered(bool)), this, SLOT(OnOpenAppFolder()));
	connect(ui->installButton, SIGNAL(clicked(bool)), this, SLOT(OnInstall()));
	connect(editListDialog, SIGNAL(destroyed(QObject*)), this, SLOT(Restart()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::Restart(){
	QProcess::startDetached(QCoreApplication::applicationFilePath());
	close();
}

void MainWindow::OnAbout(){
	QMessageBox::about(0, tr("About AppInstaller"),
					   tr("AppInstaller ") + _VERSION + tr("\n\n"\
					   "\tThis program executes in sequence the apps selected by the user "\
					   "in order to install them in a more comfortable manner."));
}

void MainWindow::OnSelectAll(){
	int nApps = appList->AppsAmount();
	for(int i=0; i<nApps; i++){
		appSel[i].setChecked(true);
	}
}

void MainWindow::OnInvertSelection(){
	int nApps = appList->AppsAmount();
	for(int i=0; i<nApps; i++){
		appSel[i].toggle();
	}
}

void MainWindow::OnSelectNone(){
	int nApps = appList->AppsAmount();
	for(int i=0; i<nApps; i++){
		appSel[i].setChecked(false);
	}
}

void MainWindow::OnModifyAppList(){
	editListDialog->show();
}

void MainWindow::OnOpenAppFolder(){
	QStringList args;
	args << APP_DIR_;
	QProcess::startDetached("explorer", args);
}

void MainWindow::OnInstall(){
	installDialog = new InstallDialog(0, appSel, appList);
	installDialog->show();
	installDialog->install();
}
