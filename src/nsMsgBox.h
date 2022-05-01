#ifndef NSMSGBOXH
#define NSMSGBOXH
#include <windows.h>
#include <stdio.h>

//=============================================================
//
//  ���і��� �F
//
//  ��޼��і��F
//
//  ���O��� �FnsLib
//
//  �N���X�� �F
//
//  �@�\     �Fү����BOX�֘A�֐�
//
//
//  �p����   �F
//
//  �쐬��   �F
//
//  �����   �F
//  �����   �F
//
//=============================================================
class nsLib
{
private:
	//ү�����ޯ��������
	static char    MsgTitle[64];
	static wchar_t wMsgTitle[64];
public:
	//հ�ނ��I���������u�̎��
	enum mbsel{
		mbselYES,     //YES��I��
		mbselNO,      //NO��I��
		mbselCancel   //��ݾق�I��
	};

	//�޲�۸ނ����ق��(63�����ȓ�)
	static void  SetMsgBoxTitle(const char    *Title);
	static void  SetMsgBoxTitle(const wchar_t *Title);
	//�װ�޲�۸�
	static void  ErrMsgBox(HWND Handle,const char *Msg,...);
	static void  ErrMsgBox(HWND Handle,const wchar_t *Msg,...);
	//�x���޲�۸ނ�\��
	static void  ExcMsgBox(HWND Handle,const char *Msg,...);
	static void  ExcMsgBox(HWND Handle,const wchar_t *Msg,...);
	//��̫Ұ����޲�۸�
	static void  InfMsgBox(HWND Handle,const char *Msg,...);
	static void  InfMsgBox(HWND Handle,const wchar_t *Msg,...);
	//YesNo�޲�۸�(�ʏ�)
	static mbsel YesNoMsgBox(HWND Handle,const char *Msg,...);
	static mbsel YesNoMsgBox(HWND Handle,const wchar_t *Msg,...);
	//YesNo�޲�۸ނ�\��(�x��)
	static mbsel YesNoExcMsgBox(HWND Handle,const char *Msg,...);
	static mbsel YesNoExcMsgBox(HWND Handle,const wchar_t *Msg,...);
	//YesNo��ݾ��޲�۸ނ�\��
	static mbsel YesNoCancelMsgBox(HWND Handle,const char *Msg,...);
	//�`�F�b�N�{�b�N�X�t�޲�۸ނ�\��
	static int MessageBoxCheck(HWND hwnd, LPCTSTR pszText, LPCTSTR pszTitle, UINT uType, int iDefault, LPCTSTR pszRegVal);
	//�t�H���_�[�I���_�C�A���O��\�����܂�
	static int ShowBrowseFolderDlg(HWND OwnerWin,const char *Title,const char *Inidir,char *SelFolder,bool NewDlgStyle=false);
	static int ShowBrowseFolderDlg(HWND OwnerWin,const wchar_t *Title,const wchar_t *Inidir,wchar_t *SelFolder,bool NewDlgStyle=false);
};


#endif //NSMSGBOXH
