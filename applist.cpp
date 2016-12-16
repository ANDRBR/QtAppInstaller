#define LIST_FILE_DIR_ QDir::toNativeSeparators("./List.csv")

#include "applist.h"
#include <qfile.h>

bool AppList::ReadAppList(){
	QString line;
	QStringList data;
	QFile file(LIST_FILE_DIR_);

	//CSV file opened
	if (!file.open(QIODevice::ReadOnly))
		return false;

	//test if valid columns
	line = file.readLine().simplified();
	if(line.compare("Category,Name,File,Icon") != 0)
		return false;

	//parse file
	for(int i=0; !file.atEnd(); i++){
		//read current line
		line = file.readLine().simplified();
		if (line.compare("") == 0)
			continue;

		//split line into tokens
		data = line.split(",");
		if (data.size() != 4){
			file.close();
			return false;
		}

		//add app to the list
		apps[APP_NAMES].append(data.at(APP_NAMES));
		apps[APP_DIRS].append(data.at(APP_DIRS));
		apps[APP_ICONS].append(data.at(APP_ICONS));

		//add category to the list if necesary
		if(!categories.contains(data.at(APP_CATEGORIES)))
			categories.append(data.at(APP_CATEGORIES));

		apps[APP_CATEGORIES].append(QString("%1").arg(FindCategoryIndex(data.at(APP_CATEGORIES))));
	}

	return true;
}

int AppList::FindCategoryIndex(QString category){
	return categories.indexOf(category);
}

int AppList::CategoriesSize(){
	return categories.size();
}

QString AppList::GetCategoryName(int index){
	return categories.at(index);
}

int AppList::AppsAmount(){
	return apps[0].size();
}

int AppList::GetAppCategoryIndex(int index){
	return apps[APP_CATEGORIES].at(index).toInt();
}

QString AppList::GetAppValue(int type, int index){
	return apps[type].at(index);
}
