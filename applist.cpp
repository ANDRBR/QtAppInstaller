#define LIST_HEADER "Category,Name,File,Icon"

#include "applist.h"
#include <qfile.h>

bool AppList::ReadAppList(QDir path){
	QString line;
	QStringList data;
	QFile file(QDir::toNativeSeparators(path.path()));

	//CSV file opened
	if (!file.open(QIODevice::ReadOnly))
		return false;

	//test if valid columns
	line = file.readLine().simplified();
	if(line.compare(LIST_HEADER) != 0)
		return false;

	//clear previus value from memory
	categories.clear();
	for(int i=0; i<4; i++)
		apps[i].clear();

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
	file.close();

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

QStringList* AppList::GetCategoriesRef(){
	return &categories;
}

void AppList::Append(QString category,
			QString name,
			QString dir,
			QString icon)
{
	if(!categories.contains(category))
		categories.append(category);

	apps[0].append(QString("%1").arg(categories.indexOf(category)));
	apps[1].append(name);
	apps[2].append(dir);
	apps[3].append(icon);
}

void AppList::RemoveAt(int index){
	apps[0].removeAt(index);
	apps[1].removeAt(index);
	apps[2].removeAt(index);
	apps[3].removeAt(index);
}

void AppList::Replace(int i,
			 QString category,
			 QString name,
			 QString dir,
			 QString icon)
{
	if(!categories.contains(category))
		categories.append(category);

	apps[0].replace(i, QString("%1").arg(categories.indexOf(category)));
	apps[1].replace(i, name);
	apps[2].replace(i, dir);
	apps[3].replace(i, icon);
}

bool AppList::CategoryExists(QString category){
	if(categories.contains(category))
		return true;
	return false;
}

void AppList::Clear(){
	categories.clear();

	for(int i=0; i<4; i++){
		apps[i].clear();
	}
}

void AppList::OnSaveList(){
	int i, j, nApps = AppsAmount();
	QFile file(DEFAULT_LIST_PATH_.path());

	//CSV file opened
	if (!file.open(QIODevice::WriteOnly))
		return;

	file.write(LIST_HEADER);
	file.write("\n");
	for(i=0; i<nApps; i++){
		file.write(categories.at(apps[0].at(i).toInt()).toStdString().c_str());
		for(j=1; j<4; j++){
			file.write(",");
			file.write(apps[j].at(i).toStdString().c_str());
		}
		file.write("\n");
	}

	file.close();
}
