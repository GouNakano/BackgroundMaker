//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <dir.h>
#include <dos.h>
#include "strconv.h"
#include "nsList.h"
#include "nsSearchDir.h"

//-------------------------------------------------------------
int nsSearchDir::MakeDirList(const char *dir)
{
	struct ffblk  f_dir;
	struct ffblk *f_ptr;

	dirlist.Clear();
	if(::findfirst(dir,&f_dir,FA_DIREC | FA_ARCH))
	{
		return -1;
	}
	do
	{
		if(!strcmp(f_dir.ff_name,".") || !strcmp(f_dir.ff_name,".."))
		{
			continue;
		}
		f_ptr = new ffblk;
		*f_ptr = f_dir;
		dirlist.Add(f_ptr);

	}while(!::findnext(&f_dir));
	findclose(&f_dir);

	return 0;
}
int nsSearchDir::MakeDirList(const wchar_t *dir)
{
	std::string adir = wide_to_ansi(dir);

	return MakeDirList(adir.c_str());
}
//-------------------------------------------------------------
ffblk *nsSearchDir::GetDirList(int index)
{
	if(index < 0 || index > dirlist.GetCount() - 1)return NULL;

	return dirlist.GetItem(index);
}
//-------------------------------------------------------------
int nsSearchDir::GetCount()
{
	return dirlist.GetCount();
}

