#include "editcsvlist.h"
#include "ui_editcsvlist.h"
#include <QFileDialog>
#include <QMessageBox>

EditCSVList::EditCSVList(QWidget *parent) :
	QMainWindow(parent),
	_ui(new Ui::EditCSVList),
	_appList(new AppList),
	_headerLabels(tr("Category;Name;Directory").split(";"))
{
	_ui->setupUi(this);

	ResetTable();

	connect(_ui->listTable , SIGNAL(cellClicked(int,int)), this, SLOT(OnItemClicked()));
	connect(_ui->dirBrowse , SIGNAL(clicked(bool)), this, SLOT(OnBrowseDir()));
	connect(_ui->deleteSelectedButton, SIGNAL(clicked(bool)), this, SLOT(OnDeleteSelected()));
	connect(_ui->actionDelete_Selected, SIGNAL(triggered(bool)), this, SLOT(OnDeleteSelected()));
	connect(_ui->addNewButton, SIGNAL(clicked(bool)), this, SLOT(OnAddNew()));
	connect(_ui->actionAdd_New, SIGNAL(triggered(bool)), this, SLOT(OnAddNew()));
	connect(_ui->modifySelectedButton, SIGNAL(clicked(bool)), this, SLOT(OnModifySelected()));
	connect(_ui->actionEdit_Selected, SIGNAL(triggered(bool)), this, SLOT(OnModifySelected()));
	connect(_ui->actionNew , SIGNAL(triggered(bool)), this, SLOT(OnNewList()));
	connect(_ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(OnSave()));
	connect(_ui->actionLoad, SIGNAL(triggered(bool)), this, SLOT(OnLoad()));
	connect(_ui->actionReload, SIGNAL(triggered(bool)), this, SLOT(OnReload()));
	connect(_ui->actionAutogenerate_Template, SIGNAL(triggered(bool)), this, SLOT(OnAutogenerate()));

}

EditCSVList::~EditCSVList()
{
	delete _ui;
}

void EditCSVList::LoadTable(){
	QStringList *categories = _appList->GetCategoriesRef();
	int nApps = _appList->AppsAmount();

	_ui->listTable->clear();
	_ui->listTable->setRowCount(nApps);
	_ui->listTable->setColumnCount(N_COLS);
	_ui->listTable->setHorizontalHeaderLabels(_headerLabels);

	for(int i=0; i<nApps; i++){
		_ui->listTable->setItem(i, 0, new QTableWidgetItem(categories->at(_appList->GetAppValue(APP_CATEGORIES,i).toInt())));
		_ui->listTable->setItem(i, 1, new QTableWidgetItem(_appList->GetAppValue(APP_NAMES,i)));
		_ui->listTable->setItem(i, 2, new QTableWidgetItem(_appList->GetAppValue(APP_DIRS,i)));
	}
	_ui->categorySel->addItems(*categories);
}

void EditCSVList::ResetTable(){
	if(!_appList->ReadAppList()){
		return;
	}
	LoadTable();
}

void EditCSVList::OnItemClicked(){
	int row = _ui->listTable->selectionModel()->selectedRows()[0].row();
	QString txt;

	txt = _ui->listTable->model()->data(_ui->listTable->model()->index(row,0)).toString();
	_ui->categorySel->setCurrentIndex(_ui->categorySel->findText(txt));
	txt = _ui->listTable->model()->data(_ui->listTable->model()->index(row,1)).toString();
	_ui->nameEdit->setText(txt);
	txt = _ui->listTable->model()->data(_ui->listTable->model()->index(row,2)).toString();
	_ui->dirEdit->setText(txt);
}

void EditCSVList::OnBrowseDir(){
	QString path = QFileDialog::getOpenFileName(this, tr("Select App Directory"), APP_DIR_);
	QDir appsPath(APP_DIR_);

	if(path.isNull())
		return;

	_ui->dirEdit->setText(appsPath.relativeFilePath(path));
}

void EditCSVList::OnAddNew(){
	int row = _ui->listTable->rowCount();
	_appList->Append(_ui->categorySel->currentText(),
					_ui->nameEdit->text(),
					_ui->dirEdit->text());

	_ui->listTable->insertRow(row);

	_ui->listTable->setItem(row, 0, new QTableWidgetItem(_ui->categorySel->currentText()));
	_ui->listTable->setItem(row, 1, new QTableWidgetItem(_ui->nameEdit->text()));
	_ui->listTable->setItem(row, 2, new QTableWidgetItem(_ui->dirEdit->text()));

}

void EditCSVList::OnModifySelected(){
	int row;
	QModelIndexList selected = _ui->listTable->selectionModel()->selectedRows();
	foreach(QModelIndex index, selected){
		row = index.row();
		_ui->listTable->setItem(row, 0, new QTableWidgetItem(_ui->categorySel->currentText()));
		_ui->listTable->setItem(row, 1, new QTableWidgetItem(_ui->nameEdit->text()));
		_ui->listTable->setItem(row, 2, new QTableWidgetItem(_ui->dirEdit->text()));
	}
	OnItemChanged();
}

void EditCSVList::OnDeleteSelected(){
	QModelIndexList selected = _ui->listTable->selectionModel()->selectedRows();
	foreach(QModelIndex index, selected){
		_ui->listTable->removeRow(index.row());
	}

	OnItemChanged();
}

void EditCSVList::OnNewList(){
	_ui->listTable->clear();
	_ui->listTable->setRowCount(0);
	_ui->listTable->setColumnCount(4);
	_ui->listTable->setHorizontalHeaderLabels(_headerLabels);
}

void EditCSVList::OnReload(){
	ResetTable();
}

void EditCSVList::OnLoad(){
	QString path = QFileDialog::getOpenFileName(this, tr("Load List"), QDir::homePath(),tr("CSV files (*.csv);;Text files (*.txt);;All files (*)"));
	if(path.isNull())
		return;

	if(!_appList->ReadAppList(QDir(path)))
		return;
	LoadTable();
}

void EditCSVList::OnSave(){
	_appList->OnSaveList();
}

void EditCSVList::OnItemChanged(){
	_appList->Clear();
	for(int i=0; i<_ui->listTable->rowCount(); i++){
		_appList->Append(_ui->listTable->item(i,0)->text(),
						_ui->listTable->item(i,1)->text(),
						_ui->listTable->item(i,2)->text());
	}

	_ui->categorySel->clear();
	_ui->categorySel->addItems(*_appList->GetCategoriesRef());
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
	_appList->AutogenerateDirs();
	LoadTable();
}
