#ifndef nsShellApiH
#define nsShellApiH

#include <windows.h>

namespace nsShellApi
{
	//´×°º°ÄÞ’è‹`
	enum ShellApiErr {
		 saGood                   = 0,
		 saAppNameNothing         = -1,
		 saSE_ERR_ASSOCINCOMPLETE = SE_ERR_ASSOCINCOMPLETE,
		 saSE_ERR_DDEBUSY         = SE_ERR_DDEBUSY,
		 saSE_ERR_DDEFAIL         = SE_ERR_DDEFAIL,
		 saSE_ERR_DDETIMEOUT      = SE_ERR_DDETIMEOUT,
		 saSE_ERR_NOASSOC         = SE_ERR_NOASSOC,
		 saSE_ERR_SHARE           = SE_ERR_SHARE
	};
	//Launch the application
	bool Shell(const wchar_t *App,const wchar_t *Cmd = L"");
	//Start the application and wait while it is running
	bool ShellAndWait(const wchar_t *App,const wchar_t *Cmd,int nShow);
};

#endif

