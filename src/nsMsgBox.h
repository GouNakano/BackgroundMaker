#ifndef NSMSGBOXH
#define NSMSGBOXH
#include <windows.h>
#include <stdio.h>

class nsLib
{
private:
	static char    MsgTitle[64];
	static wchar_t wMsgTitle[64];
public:

	enum mbsel{
		mbselYES,
		mbselNO,
		mbselCancel
	};

	static void  SetMsgBoxTitle(const char    *Title);
	static void  SetMsgBoxTitle(const wchar_t *Title);

	static void  ErrMsgBox(HWND Handle,const char *Msg,...);
	static void  ErrMsgBox(HWND Handle,const wchar_t *Msg,...);

	static void  ExcMsgBox(HWND Handle,const char *Msg,...);
	static void  ExcMsgBox(HWND Handle,const wchar_t *Msg,...);

	static void  InfMsgBox(HWND Handle,const char *Msg,...);
	static void  InfMsgBox(HWND Handle,const wchar_t *Msg,...);

	static mbsel YesNoMsgBox(HWND Handle,const char *Msg,...);
	static mbsel YesNoMsgBox(HWND Handle,const wchar_t *Msg,...);

	static mbsel YesNoExcMsgBox(HWND Handle,const char *Msg,...);
	static mbsel YesNoExcMsgBox(HWND Handle,const wchar_t *Msg,...);

	static mbsel YesNoCancelMsgBox(HWND Handle,const char *Msg,...);

	static int MessageBoxCheck(HWND hwnd, LPCTSTR pszText, LPCTSTR pszTitle, UINT uType, int iDefault, LPCTSTR pszRegVal);

	static int ShowBrowseFolderDlg(HWND OwnerWin,const char *Title,const char *Inidir,char *SelFolder,bool NewDlgStyle=false);
	static int ShowBrowseFolderDlg(HWND OwnerWin,const wchar_t *Title,const wchar_t *Inidir,wchar_t *SelFolder,bool NewDlgStyle=false);
};


#endif //NSMSGBOXH
