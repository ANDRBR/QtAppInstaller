#include "applist.h"
#include <qfile.h>

bool AppList::ReadAppList(){
	QString line;
	QStringList data;
	QFile file("List.csv");

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
		apps[APP_CATEGORIES].append(data.at(APP_CATEGORIES));
		apps[APP_NAMES].append(data.at(APP_NAMES));
		apps[APP_DIRS].append(data.at(APP_DIRS));
		apps[APP_ICONS].append(data.at(APP_ICONS));

		//add category to the list if necesary
		if(!categories.contains(data.at(APP_CATEGORIES)))
			categories.append(data.at(APP_CATEGORIES));

	}

	return true;
}
