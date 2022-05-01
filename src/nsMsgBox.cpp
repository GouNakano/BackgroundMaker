#include <vcl.h>
#pragma hdrstop
#include <shlwapi.h>
#include "strconv.h"
#include "nsMsgBox.h"

// ﾒｯｾｰｼﾞﾎﾞｯｸｽのﾀｲﾄﾙ
char nsLib::MsgTitle[64] = "";
wchar_t nsLib::wMsgTitle[64] = L"";

// =============================================================
//
// ｼｽﾃﾑ名称 ：
//
// ｻﾌﾞｼｽﾃﾑ名：
//
// 名前空間 ：nsLib
//
// クラス名 ：
//
// 機能     ：ﾒｯｾｰｼﾞBOX関連関数
//
//
// 継承元   ：
//
// 作成者   ：
//
// 改定者   ：
// 改定者   ：
//
// =============================================================

// -------------------------------------------------------------
// 機能     ：ﾀﾞｲｱﾛｸﾞのﾀｲﾄﾙをｾｯﾄ(63文字以内)
//
// 関数定義 ：void SetMsgBoxTitle(const char *Title)
//
// ｱｸｾｽﾚﾍﾞﾙ ：
//
// 引数     ：
//
// 戻り値   ：
//
// 作成者　 ：
//
// 改定者   ：
// -------------------------------------------------------------
void nsLib::SetMsgBoxTitle(const char *Title)
{
	// ｺﾋﾟｰ
	strcpy(MsgTitle, Title);
	wcscpy(wMsgTitle, String(Title).c_str());
}

void nsLib::SetMsgBoxTitle(const wchar_t *Title)
{
	// ｺﾋﾟｰ
	std::string ti = wide_to_ansi(Title);
	strcpy(MsgTitle, ti.c_str());
	wcscpy(wMsgTitle, Title);
}

// -------------------------------------------------------------
// 機能     ：ｴﾗｰﾀﾞｲｱﾛｸﾞを表示
//
// 関数定義 ：void  ErrMsgBox(HWND Handle,const char *Msg,...)
//
// ｱｸｾｽﾚﾍﾞﾙ ：
//
// 引数     ：
//
// 戻り値   ：
//
// 作成者　 ：
//
// 改定者   ：
// -------------------------------------------------------------
void nsLib::ErrMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	va_list ap;
	// ﾒｯｾｰｼﾞ組み立て
	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ﾒｯｾｰｼﾞﾎﾞｯｸｽ表示
	::MessageBoxA(Handle, MsgStr, MsgTitle, MB_OK | MB_ICONERROR);
}

void nsLib::ErrMsgBox(HWND Handle, const wchar_t *Msg, ...) {
	wchar_t MsgStr[512];
	va_list ap;
	// ﾒｯｾｰｼﾞ組み立て
	va_start(ap, Msg);
	vswprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ﾒｯｾｰｼﾞﾎﾞｯｸｽ表示
	::MessageBoxW(Handle, MsgStr, wMsgTitle, MB_OK | MB_ICONERROR);
}

// -------------------------------------------------------------
// 機能     ：警告ﾀﾞｲｱﾛｸﾞを表示
//
// 関数定義 ：void  ExcMsgBox(HWND Handle,const char *Msg,...)
//
// ｱｸｾｽﾚﾍﾞﾙ ：
//
// 引数     ：
//
// 戻り値   ：
//
// 作成者　 ：
//
// 改定者   ：
// -------------------------------------------------------------
void nsLib::ExcMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	va_list ap;
	// ﾒｯｾｰｼﾞ組み立て
	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ﾒｯｾｰｼﾞﾎﾞｯｸｽ表示
	::MessageBoxA(Handle, MsgStr, MsgTitle, MB_OK | MB_ICONEXCLAMATION);
}

void nsLib::ExcMsgBox(HWND Handle, const wchar_t *Msg, ...) {
	wchar_t MsgStr[512];
	va_list ap;
	// ﾒｯｾｰｼﾞ組み立て
	va_start(ap, Msg);
	vswprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ﾒｯｾｰｼﾞﾎﾞｯｸｽ表示
	::MessageBoxW(Handle, MsgStr, wMsgTitle, MB_OK | MB_ICONEXCLAMATION);
}

// -------------------------------------------------------------
// 機能     ：ｲﾝﾌｫﾒｰｼｮﾝﾀﾞｲｱﾛｸﾞを表示
//
// 関数定義 ：void  InfMsgBox(HWND Handle,const char *Msg,const char *Opt1,const char *Opt2,const char *Opt3)
//
// ｱｸｾｽﾚﾍﾞﾙ ：
//
// 引数     ：
//
// 戻り値   ：
//
// 作成者　 ：
//
// 改定者   ：
// -------------------------------------------------------------
void nsLib::InfMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	va_list ap;
	// ﾒｯｾｰｼﾞ組み立て
	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ﾒｯｾｰｼﾞﾎﾞｯｸｽ表示
	::MessageBoxA(Handle, MsgStr, MsgTitle, MB_OK | MB_ICONINFORMATION);
}

void nsLib::InfMsgBox(HWND Handle, const wchar_t *Msg, ...) {
	wchar_t MsgStr[512];
	va_list ap;
	// ﾒｯｾｰｼﾞ組み立て
	va_start(ap, Msg);
	vswprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ﾒｯｾｰｼﾞﾎﾞｯｸｽ表示
	::MessageBoxW(Handle, MsgStr, wMsgTitle, MB_OK | MB_ICONINFORMATION);
}

// -------------------------------------------------------------
// 機能     ：YesNoﾀﾞｲｱﾛｸﾞを表示(通常)
//
// 関数定義 ：mbsel YesNoMsgBox(HWND Handle,const char *Msg,...)
//
// ｱｸｾｽﾚﾍﾞﾙ ：
//
// 引数     ：
//
// 戻り値   ：
//
// 作成者　 ：
//
// 改定者   ：
// -------------------------------------------------------------
nsLib::mbsel nsLib::YesNoMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	mbsel Btn;
	va_list ap;
	// ﾒｯｾｰｼﾞ組み立て
	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ﾒｯｾｰｼﾞﾎﾞｯｸｽ表示
	int Res = ::MessageBoxA(Handle, MsgStr, MsgTitle,
		MB_YESNO | MB_ICONINFORMATION);
	// ﾕｰｻﾞが押下したﾎﾞﾀﾝをﾁｪｯｸ
	if (Res == IDYES)
		Btn = mbselYES;
	else
		Btn = mbselNO;

	return Btn;
}

nsLib::mbsel nsLib::YesNoMsgBox(HWND Handle, const wchar_t *Msg, ...) {
	wchar_t MsgStr[512];
	mbsel Btn;
	va_list ap;
	// ﾒｯｾｰｼﾞ組み立て
	va_start(ap, Msg);
	vswprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ﾒｯｾｰｼﾞﾎﾞｯｸｽ表示
	int Res = ::MessageBoxW(Handle, MsgStr, wMsgTitle,
		MB_YESNO | MB_ICONINFORMATION);
	// ﾕｰｻﾞが押下したﾎﾞﾀﾝをﾁｪｯｸ
	if (Res == IDYES)
		Btn = mbselYES;
	else
		Btn = mbselNO;

	return Btn;
}

// -------------------------------------------------------------
// 機能     ：YesNoﾀﾞｲｱﾛｸﾞを表示(警告)
//
// 関数定義 ：mbsel YesNoExcMsgBox(HWND Handle,const char *Msg,...)
//
// ｱｸｾｽﾚﾍﾞﾙ ：
//
// 引数     ：
//
// 戻り値   ：
//
// 作成者　 ：
//
// 改定者   ：
// -------------------------------------------------------------
nsLib::mbsel nsLib::YesNoExcMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	mbsel Btn;
	va_list ap;
	// ﾒｯｾｰｼﾞ組み立て
	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ﾒｯｾｰｼﾞﾎﾞｯｸｽ表示
	int Res = ::MessageBoxA(Handle, MsgStr, MsgTitle,
		MB_YESNO | MB_ICONEXCLAMATION);
	// ﾕｰｻﾞが押下したﾎﾞﾀﾝをﾁｪｯｸ
	if (Res == IDYES)
		Btn = mbselYES;
	else
		Btn = mbselNO;

	return Btn;
}

nsLib::mbsel nsLib::YesNoExcMsgBox(HWND Handle, const wchar_t *Msg, ...) {
	wchar_t MsgStr[512];
	mbsel Btn;
	va_list ap;
	// ﾒｯｾｰｼﾞ組み立て
	va_start(ap, Msg);
	vswprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ﾒｯｾｰｼﾞﾎﾞｯｸｽ表示
	int Res = ::MessageBoxW(Handle, MsgStr, wMsgTitle,
		MB_YESNO | MB_ICONEXCLAMATION);
	// ﾕｰｻﾞが押下したﾎﾞﾀﾝをﾁｪｯｸ
	if (Res == IDYES)
		Btn = mbselYES;
	else
		Btn = mbselNO;

	return Btn;
}

// -------------------------------------------------------------
// 機能     ：YesNoｷｬﾝｾﾙﾀﾞｲｱﾛｸﾞを表示
//
// 関数定義 ：mbsel YesNoCancelMsgBox(HWND Handle,const char *Msg,...)
//
// ｱｸｾｽﾚﾍﾞﾙ ：
//
// 引数     ：
//
// 戻り値   ：
//
// 作成者　 ：
//
// 改定者   ：
// -------------------------------------------------------------
nsLib::mbsel nsLib::YesNoCancelMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	mbsel Btn;
	va_list ap;
	// ﾒｯｾｰｼﾞ組み立て
	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ﾒｯｾｰｼﾞﾎﾞｯｸｽ表示
	int Res = ::MessageBoxA(Handle, MsgStr, MsgTitle,
		MB_YESNOCANCEL | MB_ICONINFORMATION);
	// ﾕｰｻﾞが押下したﾎﾞﾀﾝをﾁｪｯｸ
	if (Res == IDYES)
		Btn = mbselYES;
	else if (Res == IDCANCEL)
		Btn = mbselCancel;
	else
		Btn = mbselNO;

	return Btn;
}

// -------------------------------------------------------------
// 機能     ：チェックボックス付ﾀﾞｲｱﾛｸﾞを表示
//
// 関数定義 ：int MessageBoxCheck(HWND hwnd, LPCTSTR pszText, LPCTSTR pszTitle, UINT uType, int iDefault, LPCTSTR pszRegVal)
//
// ｱｸｾｽﾚﾍﾞﾙ ：
//
// 引数     ：
//
// 戻り値   ：
//
// 作成者　 ：
//
// 改定者   ：
// -------------------------------------------------------------
int nsLib::MessageBoxCheck(HWND hwnd, LPCTSTR pszText, LPCTSTR pszTitle,
	UINT uType, int iDefault, LPCTSTR pszRegVal) {
	int retValue = -1;

	retValue = SHMessageBoxCheck(hwnd, pszText, pszTitle, uType, iDefault,
		pszRegVal);

	return retValue;
}
