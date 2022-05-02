#ifndef NSSEARCHDIRH
#define NSSEARCHDIRH
//---------------------------------------------------------------------------
#include <dir.h>
#include <dos.h>
#include "nsList.h"

class nsSearchDir
{
private:
	nsList<ffblk> dirlist;
public:
	 nsSearchDir(){}
	~nsSearchDir(){}
public:
	int    MakeDirList(const char    *dir);
	int    MakeDirList(const wchar_t *dir);
	ffblk *GetDirList(int index);
	int    GetCount();
};

#endif

