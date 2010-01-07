/**
 * Original file by the_viking, fixed by Rômulo Fernandes
 * Should emulate windows finddata structure
 */
#if defined(__GNUC__) && !defined(__WIN32)
#include "_findfirst.h"
#include "DS_List.h"

static DataStructures::List< _findinfo_t* > fileInfo;

/** 
 * _findfirst - equivalent
 */
long _findfirst(const char *name, _finddata_t *f)
{
	/*
	char* nameCopy = new char[sizeof(name)];
	memset(nameCopy, '\0', sizeof(nameCopy));
	
	strcpy(nameCopy, name);

	char* filter = new char[sizeof(nameCopy)];
	memset(filter, '\0', sizeof(filter));

	char* lastSep = strrchr(nameCopy,'/');
	if(!lastSep)
	{
		strcpy(filter, nameCopy);
		strcpy(nameCopy, ".");
	}
	else
	{
		strcpy(filter, lastSep+1);
		*lastSep = 0;
	}

	DIR* dir = opendir(nameCopy);

	if(!dir)
	{
		return -1;
	}

	_findinfo_t* fi = new _findinfo_t;
	strcpy(fi->filter,filter);
	fi->openedDir = dir;

	while(true)
	{
		dirent* entry = readdir(dir);
		if(entry == 0)
			break;

		if(fnmatch(fi->filter,entry->d_name, 200) == 0)
		{
			strcpy(f->name, entry->d_name);
			break;
		}
	}


	fileInfo.Insert(fi);
	return fileInfo.Size();
	*/
	return 0;
}

/** 
 * _findnext - equivalent
 */
int _findnext(long h, _finddata_t *f)
{
	/*
	_findinfo_t* fi = fileInfo[h];

	while(true)
	{
		dirent* entry = readdir(fi->openedDir);
		if(entry == 0)
			return -1;

		if(fnmatch(fi->filter,entry->d_name, 200) == 0)
		{
			strcpy(f->name, entry->d_name);
			return 0;
		}
	}
	*/
	return -1;
}

/** 
 * _findclose - equivalent
 */
int _findclose(long h)
{
	/*
	_findinfo_t* fi = fileInfo[h];
	delete fi;
*/
	return 0;	
}
#endif
