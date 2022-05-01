#include <vcl.h>
#pragma hdrstop
#include <shlwapi.h>
#include "strconv.h"
#include "nsMsgBox.h"

// ү�����ޯ��������
char nsLib::MsgTitle[64] = "";
wchar_t nsLib::wMsgTitle[64] = L"";

// =============================================================
//
// ���і��� �F
//
// ��޼��і��F
//
// ���O��� �FnsLib
//
// �N���X�� �F
//
// �@�\     �Fү����BOX�֘A�֐�
//
//
// �p����   �F
//
// �쐬��   �F
//
// �����   �F
// �����   �F
//
// =============================================================

// -------------------------------------------------------------
// �@�\     �F�޲�۸ނ����ق��(63�����ȓ�)
//
// �֐���` �Fvoid SetMsgBoxTitle(const char *Title)
//
// �������� �F
//
// ����     �F
//
// �߂�l   �F
//
// �쐬�ҁ@ �F
//
// �����   �F
// -------------------------------------------------------------
void nsLib::SetMsgBoxTitle(const char *Title)
{
	// ��߰
	strcpy(MsgTitle, Title);
	wcscpy(wMsgTitle, String(Title).c_str());
}

void nsLib::SetMsgBoxTitle(const wchar_t *Title)
{
	// ��߰
	std::string ti = wide_to_ansi(Title);
	strcpy(MsgTitle, ti.c_str());
	wcscpy(wMsgTitle, Title);
}

// -------------------------------------------------------------
// �@�\     �F�װ�޲�۸ނ�\��
//
// �֐���` �Fvoid  ErrMsgBox(HWND Handle,const char *Msg,...)
//
// �������� �F
//
// ����     �F
//
// �߂�l   �F
//
// �쐬�ҁ@ �F
//
// �����   �F
// -------------------------------------------------------------
void nsLib::ErrMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	va_list ap;
	// ү���ޑg�ݗ���
	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ү�����ޯ���\��
	::MessageBoxA(Handle, MsgStr, MsgTitle, MB_OK | MB_ICONERROR);
}

void nsLib::ErrMsgBox(HWND Handle, const wchar_t *Msg, ...) {
	wchar_t MsgStr[512];
	va_list ap;
	// ү���ޑg�ݗ���
	va_start(ap, Msg);
	vswprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ү�����ޯ���\��
	::MessageBoxW(Handle, MsgStr, wMsgTitle, MB_OK | MB_ICONERROR);
}

// -------------------------------------------------------------
// �@�\     �F�x���޲�۸ނ�\��
//
// �֐���` �Fvoid  ExcMsgBox(HWND Handle,const char *Msg,...)
//
// �������� �F
//
// ����     �F
//
// �߂�l   �F
//
// �쐬�ҁ@ �F
//
// �����   �F
// -------------------------------------------------------------
void nsLib::ExcMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	va_list ap;
	// ү���ޑg�ݗ���
	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ү�����ޯ���\��
	::MessageBoxA(Handle, MsgStr, MsgTitle, MB_OK | MB_ICONEXCLAMATION);
}

void nsLib::ExcMsgBox(HWND Handle, const wchar_t *Msg, ...) {
	wchar_t MsgStr[512];
	va_list ap;
	// ү���ޑg�ݗ���
	va_start(ap, Msg);
	vswprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ү�����ޯ���\��
	::MessageBoxW(Handle, MsgStr, wMsgTitle, MB_OK | MB_ICONEXCLAMATION);
}

// -------------------------------------------------------------
// �@�\     �F��̫Ұ����޲�۸ނ�\��
//
// �֐���` �Fvoid  InfMsgBox(HWND Handle,const char *Msg,const char *Opt1,const char *Opt2,const char *Opt3)
//
// �������� �F
//
// ����     �F
//
// �߂�l   �F
//
// �쐬�ҁ@ �F
//
// �����   �F
// -------------------------------------------------------------
void nsLib::InfMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	va_list ap;
	// ү���ޑg�ݗ���
	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ү�����ޯ���\��
	::MessageBoxA(Handle, MsgStr, MsgTitle, MB_OK | MB_ICONINFORMATION);
}

void nsLib::InfMsgBox(HWND Handle, const wchar_t *Msg, ...) {
	wchar_t MsgStr[512];
	va_list ap;
	// ү���ޑg�ݗ���
	va_start(ap, Msg);
	vswprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ү�����ޯ���\��
	::MessageBoxW(Handle, MsgStr, wMsgTitle, MB_OK | MB_ICONINFORMATION);
}

// -------------------------------------------------------------
// �@�\     �FYesNo�޲�۸ނ�\��(�ʏ�)
//
// �֐���` �Fmbsel YesNoMsgBox(HWND Handle,const char *Msg,...)
//
// �������� �F
//
// ����     �F
//
// �߂�l   �F
//
// �쐬�ҁ@ �F
//
// �����   �F
// -------------------------------------------------------------
nsLib::mbsel nsLib::YesNoMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	mbsel Btn;
	va_list ap;
	// ү���ޑg�ݗ���
	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ү�����ޯ���\��
	int Res = ::MessageBoxA(Handle, MsgStr, MsgTitle,
		MB_YESNO | MB_ICONINFORMATION);
	// հ�ނ������������݂�����
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
	// ү���ޑg�ݗ���
	va_start(ap, Msg);
	vswprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ү�����ޯ���\��
	int Res = ::MessageBoxW(Handle, MsgStr, wMsgTitle,
		MB_YESNO | MB_ICONINFORMATION);
	// հ�ނ������������݂�����
	if (Res == IDYES)
		Btn = mbselYES;
	else
		Btn = mbselNO;

	return Btn;
}

// -------------------------------------------------------------
// �@�\     �FYesNo�޲�۸ނ�\��(�x��)
//
// �֐���` �Fmbsel YesNoExcMsgBox(HWND Handle,const char *Msg,...)
//
// �������� �F
//
// ����     �F
//
// �߂�l   �F
//
// �쐬�ҁ@ �F
//
// �����   �F
// -------------------------------------------------------------
nsLib::mbsel nsLib::YesNoExcMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	mbsel Btn;
	va_list ap;
	// ү���ޑg�ݗ���
	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ү�����ޯ���\��
	int Res = ::MessageBoxA(Handle, MsgStr, MsgTitle,
		MB_YESNO | MB_ICONEXCLAMATION);
	// հ�ނ������������݂�����
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
	// ү���ޑg�ݗ���
	va_start(ap, Msg);
	vswprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ү�����ޯ���\��
	int Res = ::MessageBoxW(Handle, MsgStr, wMsgTitle,
		MB_YESNO | MB_ICONEXCLAMATION);
	// հ�ނ������������݂�����
	if (Res == IDYES)
		Btn = mbselYES;
	else
		Btn = mbselNO;

	return Btn;
}

// -------------------------------------------------------------
// �@�\     �FYesNo��ݾ��޲�۸ނ�\��
//
// �֐���` �Fmbsel YesNoCancelMsgBox(HWND Handle,const char *Msg,...)
//
// �������� �F
//
// ����     �F
//
// �߂�l   �F
//
// �쐬�ҁ@ �F
//
// �����   �F
// -------------------------------------------------------------
nsLib::mbsel nsLib::YesNoCancelMsgBox(HWND Handle, const char *Msg, ...) {
	char MsgStr[512];
	mbsel Btn;
	va_list ap;
	// ү���ޑg�ݗ���
	va_start(ap, Msg);
	vsprintf(MsgStr, Msg, ap);
	va_end(ap);
	// ү�����ޯ���\��
	int Res = ::MessageBoxA(Handle, MsgStr, MsgTitle,
		MB_YESNOCANCEL | MB_ICONINFORMATION);
	// հ�ނ������������݂�����
	if (Res == IDYES)
		Btn = mbselYES;
	else if (Res == IDCANCEL)
		Btn = mbselCancel;
	else
		Btn = mbselNO;

	return Btn;
}

// -------------------------------------------------------------
// �@�\     �F�`�F�b�N�{�b�N�X�t�޲�۸ނ�\��
//
// �֐���` �Fint MessageBoxCheck(HWND hwnd, LPCTSTR pszText, LPCTSTR pszTitle, UINT uType, int iDefault, LPCTSTR pszRegVal)
//
// �������� �F
//
// ����     �F
//
// �߂�l   �F
//
// �쐬�ҁ@ �F
//
// �����   �F
// -------------------------------------------------------------
int nsLib::MessageBoxCheck(HWND hwnd, LPCTSTR pszText, LPCTSTR pszTitle,
	UINT uType, int iDefault, LPCTSTR pszRegVal) {
	int retValue = -1;

	retValue = SHMessageBoxCheck(hwnd, pszText, pszTitle, uType, iDefault,
		pszRegVal);

	return retValue;
}
