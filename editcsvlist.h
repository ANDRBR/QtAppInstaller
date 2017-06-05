#ifndef EDITCSVLIST_H
#define EDITCSVLIST_H

#include <QMainWindow>
#include <QTableWidget>
#include <QCloseEvent>
#include "applist.h"

namespace Ui {
	class EditCSVList;
}

class EditCSVList : public QMainWindow
{
		Q_OBJECT

	public:
		explicit EditCSVList(QWidget *parent = 0);
		~EditCSVList();

		void ResetTable();
		void LoadTable();

	public slots:
		void OnItemClicked();
		void OnBrowseDir();
		void OnAddNew();
		void OnDeleteSelected();
		void OnModifySelected();
		void OnAutogenerate();
		void OnItemChanged();
		void OnNewList();
		void OnReload();
		void OnLoad();
		void OnSave();

	private:
		Ui::EditCSVList *_ui;
		AppList *_appList;
		QStringList _headerLabels;

		QTableWidgetItem _previousItem;

	protected:
		void closeEvent(QCloseEvent *event);
};

#endif // EDITCSVLIST_H
