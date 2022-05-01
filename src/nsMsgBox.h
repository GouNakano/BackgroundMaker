#ifndef NSMSGBOXH
#define NSMSGBOXH
#include <windows.h>
#include <stdio.h>

//=============================================================
//
//  ｼｽﾃﾑ名称 ：
//
//  ｻﾌﾞｼｽﾃﾑ名：
//
//  名前空間 ：nsLib
//
//  クラス名 ：
//
//  機能     ：ﾒｯｾｰｼﾞBOX関連関数
//
//
//  継承元   ：
//
//  作成者   ：
//
//  改定者   ：
//  改定者   ：
//
//=============================================================
class nsLib
{
private:
	//ﾒｯｾｰｼﾞﾎﾞｯｸｽのﾀｲﾄﾙ
	static char    MsgTitle[64];
	static wchar_t wMsgTitle[64];
public:
	//ﾕｰｻﾞが選択した処置の種類
	enum mbsel{
		mbselYES,     //YESを選択
		mbselNO,      //NOを選択
		mbselCancel   //ｷｬﾝｾﾙを選択
	};

	//ﾀﾞｲｱﾛｸﾞのﾀｲﾄﾙをｾｯﾄ(63文字以内)
	static void  SetMsgBoxTitle(const char    *Title);
	static void  SetMsgBoxTitle(const wchar_t *Title);
	//ｴﾗｰﾀﾞｲｱﾛｸﾞ
	static void  ErrMsgBox(HWND Handle,const char *Msg,...);
	static void  ErrMsgBox(HWND Handle,const wchar_t *Msg,...);
	//警告ﾀﾞｲｱﾛｸﾞを表示
	static void  ExcMsgBox(HWND Handle,const char *Msg,...);
	static void  ExcMsgBox(HWND Handle,const wchar_t *Msg,...);
	//ｲﾝﾌｫﾒｰｼｮﾝﾀﾞｲｱﾛｸﾞ
	static void  InfMsgBox(HWND Handle,const char *Msg,...);
	static void  InfMsgBox(HWND Handle,const wchar_t *Msg,...);
	//YesNoﾀﾞｲｱﾛｸﾞ(通常)
	static mbsel YesNoMsgBox(HWND Handle,const char *Msg,...);
	static mbsel YesNoMsgBox(HWND Handle,const wchar_t *Msg,...);
	//YesNoﾀﾞｲｱﾛｸﾞを表示(警告)
	static mbsel YesNoExcMsgBox(HWND Handle,const char *Msg,...);
	static mbsel YesNoExcMsgBox(HWND Handle,const wchar_t *Msg,...);
	//YesNoｷｬﾝｾﾙﾀﾞｲｱﾛｸﾞを表示
	static mbsel YesNoCancelMsgBox(HWND Handle,const char *Msg,...);
	//チェックボックス付ﾀﾞｲｱﾛｸﾞを表示
	static int MessageBoxCheck(HWND hwnd, LPCTSTR pszText, LPCTSTR pszTitle, UINT uType, int iDefault, LPCTSTR pszRegVal);
	//フォルダー選択ダイアログを表示します
	static int ShowBrowseFolderDlg(HWND OwnerWin,const char *Title,const char *Inidir,char *SelFolder,bool NewDlgStyle=false);
	static int ShowBrowseFolderDlg(HWND OwnerWin,const wchar_t *Title,const wchar_t *Inidir,wchar_t *SelFolder,bool NewDlgStyle=false);
};


#endif //NSMSGBOXH
