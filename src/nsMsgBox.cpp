#include <vcl.h>
#pragma hdrstop
#include <shlwapi.h>
#include "strconv.h"
#include "nsMsgBox.h"


char nsLib::MsgTitle[64] = "";
wchar_t nsLib::wMsgTitle[64] = L"";

// -------------------------------------------------------------
void nsLib::SetMsgBoxTitle(const char *Title)
{
	strcpy(MsgTitle, Title);
	wcscpy(wMsgTitle, String(Title).c_str());
}

void nsLib::SetMsgBoxTitle(const wchar_t *Title)
{
	std::string ti = wide_to_ansi(Title);
	strcpy(MsgTitle, ti.c_str());
	wcscpy(wMsgTitle, Title);
}

// -------------------------------------------------------------
void nsLib::ErrMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	va_list ap;

	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);

	::MessageBoxA(Handle, MsgStr, MsgTitle, MB_OK | MB_ICONERROR);
}

void nsLib::ErrMsgBox(HWND Handle, const wchar_t *Msg, ...) {
	wchar_t MsgStr[512];
	va_list ap;

	va_start(ap, Msg);
	vswprintf(MsgStr, Msg, ap);
	va_end(ap);

	::MessageBoxW(Handle, MsgStr, wMsgTitle, MB_OK | MB_ICONERROR);
}

// -------------------------------------------------------------
void nsLib::ExcMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	va_list ap;

	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);

	::MessageBoxA(Handle, MsgStr, MsgTitle, MB_OK | MB_ICONEXCLAMATION);
}

void nsLib::ExcMsgBox(HWND Handle, const wchar_t *Msg, ...) {
	wchar_t MsgStr[512];
	va_list ap;

	va_start(ap, Msg);
	vswprintf(MsgStr, Msg, ap);
	va_end(ap);

	::MessageBoxW(Handle, MsgStr, wMsgTitle, MB_OK | MB_ICONEXCLAMATION);
}

// -------------------------------------------------------------
void nsLib::InfMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	va_list ap;

	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);

	::MessageBoxA(Handle, MsgStr, MsgTitle, MB_OK | MB_ICONINFORMATION);
}

void nsLib::InfMsgBox(HWND Handle, const wchar_t *Msg, ...) {
	wchar_t MsgStr[512];
	va_list ap;

	va_start(ap, Msg);
	vswprintf(MsgStr, Msg, ap);
	va_end(ap);

	::MessageBoxW(Handle, MsgStr, wMsgTitle, MB_OK | MB_ICONINFORMATION);
}

// -------------------------------------------------------------
nsLib::mbsel nsLib::YesNoMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	mbsel Btn;
	va_list ap;

	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);

	int Res = ::MessageBoxA(Handle, MsgStr, MsgTitle,
		MB_YESNO | MB_ICONINFORMATION);

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

	va_start(ap, Msg);
	vswprintf(MsgStr, Msg, ap);
	va_end(ap);

	int Res = ::MessageBoxW(Handle, MsgStr, wMsgTitle,
		MB_YESNO | MB_ICONINFORMATION);

	if (Res == IDYES)
		Btn = mbselYES;
	else
		Btn = mbselNO;

	return Btn;
}

// -------------------------------------------------------------
nsLib::mbsel nsLib::YesNoExcMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	mbsel Btn;
	va_list ap;

	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);

	int Res = ::MessageBoxA(Handle, MsgStr, MsgTitle,
		MB_YESNO | MB_ICONEXCLAMATION);

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

	va_start(ap, Msg);
	vswprintf(MsgStr, Msg, ap);
	va_end(ap);

	int Res = ::MessageBoxW(Handle, MsgStr, wMsgTitle,
		MB_YESNO | MB_ICONEXCLAMATION);

	if (Res == IDYES)
		Btn = mbselYES;
	else
		Btn = mbselNO;

	return Btn;
}

// -------------------------------------------------------------
nsLib::mbsel nsLib::YesNoCancelMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	mbsel Btn;
	va_list ap;

	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);

	int Res = ::MessageBoxA(Handle, MsgStr, MsgTitle,
		MB_YESNOCANCEL | MB_ICONINFORMATION);

	if (Res == IDYES)
		Btn = mbselYES;
	else if (Res == IDCANCEL)
		Btn = mbselCancel;
	else
		Btn = mbselNO;

	return Btn;
}

// -------------------------------------------------------------
int nsLib::MessageBoxCheck(HWND hwnd, LPCTSTR pszText, LPCTSTR pszTitle,
	UINT uType, int iDefault, LPCTSTR pszRegVal) {
	int retValue = -1;

	retValue = SHMessageBoxCheck(hwnd, pszText, pszTitle, uType, iDefault,
		pszRegVal);

	return retValue;
}
