#ifndef APPLIST_H
#define APPLIST_H

#define APP_CATEGORIES 0
#define APP_NAMES 1
#define APP_DIRS 2
#define APP_ICONS 3
#define NATIVE_SEP_ QDir::toNativeSeparators("/")
#define APP_DIR_ QDir::toNativeSeparators("./Apps")
#define ICON_DIR_ QDir::toNativeSeparators("./Resources/")

#include <QtCore>

class AppList{
	public:
		bool ReadAppList();
		int FindCategoryIndex(QString category);
		int CategoriesSize();
		int AppsAmount();
		int GetAppCategoryIndex(int index);
		QString GetAppValue(int type, int index);
		QString GetCategoryName(int index);

	private:
		QStringList categories;
		QStringList apps[4];
};

#endif // APPLIST_H
