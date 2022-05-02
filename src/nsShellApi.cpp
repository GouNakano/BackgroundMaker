
#include "nsShellApi.h"
//for C+Builder
#ifdef __BORLANDC__
#include <vcl.h>
#endif

namespace nsShellApi
{
	ShellApiErr LastErr;

//-------------------------------------------------------------
//Launch the application
//-------------------------------------------------------------
bool Shell(const wchar_t *App,const wchar_t *Cmd)
{
	HINSTANCE Inst;
	bool      Res;

	if(wcscmp(App,L"") == 0)
	{
		LastErr = saAppNameNothing;
		return false;
	}
	if(wcscmp(Cmd,L"") == 0)
		Inst = ShellExecuteW(NULL,L"open",App,NULL,NULL,SW_SHOW);
	else
		Inst = ShellExecuteW(NULL,L"open",App,Cmd,NULL,SW_SHOW);

	if((int)Inst > 32)
	{
		LastErr = saGood;
		Res = true;
	}
	else
	{
		//fail
		LastErr = (ShellApiErr)((int)Inst);
		Res     = false;
	}
	return Res;
}
//-------------------------------------------------------------
//get last error
//-------------------------------------------------------------
ShellApiErr GetLastErr()
{
	return LastErr;
}

//-------------------------------------------------------------
//  Start the application and wait while it is running
//-------------------------------------------------------------
bool ShellAndWait(const wchar_t *App,const wchar_t *Cmd,int nShow)
{
	SHELLEXECUTEINFOW execinfo;
	memset(&execinfo,0,sizeof(execinfo));
	execinfo.cbSize       = sizeof(execinfo);
	execinfo.lpVerb       = L"open";
	execinfo.lpFile       = App;
	execinfo.lpParameters = Cmd;
	execinfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
	execinfo.nShow        = nShow;

	if(!ShellExecuteExW(&execinfo))
	{
		return false;
	}

	while(1)
	{
		//ÌßÛ¾½‘Ò‚¿‚ğs‚¤
		DWORD ProcStatus = WaitForSingleObject(execinfo.hProcess,100);
		//À²Ñ±³Ä‚Ìˆ—
#ifdef __BCPLUSPLUS__
		//C++Builder‚ÌÒ¯¾°¼Şˆ—
		Application->ProcessMessages();
#endif
		//ÌßÛ¾½ó‘Ô‚ÌÁª¯¸
		if(ProcStatus != WAIT_TIMEOUT)break;
	}
	return true;
}


}
