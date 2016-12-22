#include "editcsvlist.h"
#include "ui_editcsvlist.h"
#include <QFileDialog>
#include <QMessageBox>

EditCSVList::EditCSVList(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::EditCSVList),
	appList(new AppList),
	headerLabels(tr("Category;Name;Directory;Icon").split(";"))
{
	ui->setupUi(this);

	ResetTable();

	connect(ui->dirBrowse, SIGNAL(clicked(bool)), this, SLOT(OnBrowseDir()));
	connect(ui->iconBrowse, SIGNAL(clicked(bool)), this, SLOT(OnBrowseIcon()));
	connect(ui->deleteSelectedButton, SIGNAL(clicked(bool)), this, SLOT(OnDeleteSelected()));
	connect(ui->actionDelete_Selected, SIGNAL(triggered(bool)), this, SLOT(OnDeleteSelected()));
	connect(ui->addNewButton, SIGNAL(clicked(bool)), this, SLOT(OnAddNew()));
	connect(ui->actionAdd_New, SIGNAL(triggered(bool)), this, SLOT(OnAddNew()));
	connect(ui->modifySelectedButton, SIGNAL(clicked(bool)), this, SLOT(OnModifySelected()));
	connect(ui->actionEdit_Selected, SIGNAL(triggered(bool)), this, SLOT(OnModifySelected()));
	connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(OnNewList()));
	connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(OnSave()));
	connect(ui->actionLoad, SIGNAL(triggered(bool)), this, SLOT(OnLoad()));
	connect(ui->actionReload, SIGNAL(triggered(bool)), this, SLOT(OnReload()));
	connect(ui->actionAutogenerate_Template, SIGNAL(triggered(bool)), this, SLOT(OnAutogenerate()));

}

EditCSVList::~EditCSVList()
{
	delete ui;
}

void EditCSVList::LoadTable(){
	QStringList *categories = appList->GetCategoriesRef();
	int nApps = appList->AppsAmount();

	ui->listTable->clear();
	ui->listTable->setRowCount(nApps);
	ui->listTable->setColumnCount(4);
	ui->listTable->setHorizontalHeaderLabels(headerLabels);

	for(int i=0; i<nApps; i++){
		ui->listTable->setItem(i, 0, new QTableWidgetItem(categories->at(appList->GetAppValue(APP_CATEGORIES,i).toInt())));
		ui->listTable->setItem(i, 1, new QTableWidgetItem(appList->GetAppValue(APP_NAMES,i)));
		ui->listTable->setItem(i, 2, new QTableWidgetItem(appList->GetAppValue(APP_DIRS,i)));
		ui->listTable->setItem(i, 3, new QTableWidgetItem(appList->GetAppValue(APP_ICONS,i)));
	}
	ui->categorySel->addItems(*categories);
}

void EditCSVList::ResetTable(){
	if(!appList->ReadAppList()){
		return;
	}
	LoadTable();
}

void EditCSVList::OnBrowseDir(){
	QString path = QFileDialog::getOpenFileName(this, tr("Select App Directory"), APP_DIR_);
	QDir appsPath(APP_DIR_);

	if(path.isNull())
		return;

	ui->dirEdit->setText(appsPath.relativeFilePath(path));
}

void EditCSVList::OnBrowseIcon(){
	QString path = QFileDialog::getOpenFileName(this, tr("Select Icon Directory"), ICON_DIR_);
	QDir iconsPath(ICON_DIR_);

	if(path.isNull())
		return;

	ui->iconEdit->setText(iconsPath.relativeFilePath(path));
}

void EditCSVList::OnAddNew(){
	int row = ui->listTable->rowCount();
	appList->Append(ui->categorySel->currentText(),
					ui->nameEdit->text(),
					ui->dirEdit->text(),
					ui->iconEdit->text());

	ui->listTable->insertRow(row);

	ui->listTable->setItem(row, 0, new QTableWidgetItem(ui->categorySel->currentText()));
	ui->listTable->setItem(row, 1, new QTableWidgetItem(ui->nameEdit->text()));
	ui->listTable->setItem(row, 2, new QTableWidgetItem(ui->dirEdit->text()));
	ui->listTable->setItem(row, 3, new QTableWidgetItem(ui->iconEdit->text()));

}

void EditCSVList::OnModifySelected(){
	int row;
	QModelIndexList selected = ui->listTable->selectionModel()->selectedRows();
	foreach(QModelIndex index, selected){
		row = index.row();
		ui->listTable->setItem(row, 0, new QTableWidgetItem(ui->categorySel->currentText()));
		ui->listTable->setItem(row, 1, new QTableWidgetItem(ui->nameEdit->text()));
		ui->listTable->setItem(row, 2, new QTableWidgetItem(ui->dirEdit->text()));
		ui->listTable->setItem(row, 3, new QTableWidgetItem(ui->iconEdit->text()));
	}
	OnItemChanged();
}

void EditCSVList::OnDeleteSelected(){
	QModelIndexList selected = ui->listTable->selectionModel()->selectedRows();
	foreach(QModelIndex index, selected){
		ui->listTable->removeRow(index.row());
	}

	OnItemChanged();
}

void EditCSVList::OnNewList(){
	ui->listTable->clear();
	ui->listTable->setRowCount(0);
	ui->listTable->setColumnCount(4);
	ui->listTable->setHorizontalHeaderLabels(headerLabels);
}

void EditCSVList::OnReload(){
	ResetTable();
}

void EditCSVList::OnLoad(){
	QString path = QFileDialog::getOpenFileName(this, tr("Load List"), QDir::homePath(),tr("CSV files (*.csv);;Text files (*.txt);;All files (*)"));
	if(path.isNull())
		return;

	if(!appList->ReadAppList(QDir(path)))
		return;
	LoadTable();
}

void EditCSVList::OnSave(){
	appList->OnSaveList();
}

void EditCSVList::OnItemChanged(){
	appList->Clear();
	for(int i=0; i<ui->listTable->rowCount(); i++){
		appList->Append(ui->listTable->item(i,0)->text(),
						ui->listTable->item(i,1)->text(),
						ui->listTable->item(i,2)->text(),
						ui->listTable->item(i,3)->text());
	}

	ui->categorySel->clear();
	ui->categorySel->addItems(*appList->GetCategoriesRef());
}

void EditCSVList::closeEvent(QCloseEvent *event){
	QMessageBox restartDialog(tr("Relaunch to apply changes"),
							  tr("The program must be relaunched to apply changes.\n\n"
								 "\tDo you wish to relaunch the program?"),
							  QMessageBox::NoIcon, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton);
	if(restartDialog.exec() == QMessageBox::Ok){
		this->setAttribute(Qt::WA_DeleteOnClose);

	}
	event->accept();
}

void EditCSVList::OnAutogenerate(){
	appList->AutogenerateDirs();
	LoadTable();
}
