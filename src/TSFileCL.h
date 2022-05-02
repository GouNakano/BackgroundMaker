//---------------------------------------------------------------------------
#ifndef TSFileCLH
#define TSFileCLH
//---------------------------------------------------------------------------

#include "nsList.h"
#include <windows.h>
#include <stdio.h>
#include <dos.h>
#include <io.h>
#ifdef  _MSC_VER
#include<direct.h>
#include<sys/types.h>
#include<sys/stat.h>
#else
#include <dir.h>
#endif

class TSDirectoryCL
{
private:
	class TS_ffblk: public ffblk
	{
	public:
		char BackupFile[MAX_PATH];
	};
private:
	char             BackupDir[MAX_PATH];
	nsList<TS_ffblk> DelfList;
private:
	bool ProcDeleteDirectory(const char *Path);
public:
	 TSDirectoryCL();
	~TSDirectoryCL();
public:
	bool CopyFile(const char *Src,const char *Dest);
	bool DirectoryCopy(const char *Sce,const char *Des);
	bool DeleteFile(const char *Path);
	bool MakeDirectory(const char *Path);
	bool MakeDirectory(const wchar_t *Path);
	bool DeleteDirectory(const char *Path);
	bool CheckExistFile(const char *Path);
	bool SetCurrentDirectory(const char *Path);
	bool MoveFile(const char *src,const char *dest);
public:
	//Unicode
	bool CopyFile(const wchar_t *Src,const wchar_t *Dest);
	bool DirectoryCopy(const wchar_t *Sce,const wchar_t *Des);
	bool DeleteDirectory(const wchar_t *Path);
};

#endif //TSFileH
