#ifndef APPLIST_H
#define APPLIST_H

#define APP_CATEGORIES 0
#define APP_NAMES 1
#define APP_DIRS 2
#define APP_ICONS 3

#include <QtCore>

class AppList{
	public:
		bool ReadAppList();

	private:
		QStringList categories;
		QStringList apps[4];

};

#endif // APPLIST_H
