#ifndef nsShellApiH
#define nsShellApiH

#include <windows.h>
//=============================================================
//
//  ���і��� �F
//
//  ��޼��і��F
//
//  ���O��� �FnsShellApi
//
//  �N���X�� �F
//
//  �@�\     �FShell�@�\�Ɋւ��鋤�ʊ֐��Q
//
//  �p����   �F
//
//  �쐬�ҁ@ �F
//
//  �����   �F
//  �����   �F
//
//=============================================================
namespace nsShellApi
{
	//�װ���ޒ�`
	enum ShellApiErr {
		 saGood                   = 0                      /*����*/,
		 saAppNameNothing         = -1                     /*���ع���ݖ�������*/,
		 saSE_ERR_ASSOCINCOMPLETE = SE_ERR_ASSOCINCOMPLETE /*�t�@�C�����̊֘A�t�����s���S� �܂��͖����ł��B*/,
		 saSE_ERR_DDEBUSY         = SE_ERR_DDEBUSY         /*DDE Busy */,
		 saSE_ERR_DDEFAIL         = SE_ERR_DDEFAIL         /*DDE�g�����U�N�V���������s���܂����B*/,
		 saSE_ERR_DDETIMEOUT      = SE_ERR_DDETIMEOUT      /*�v���^�C�� �A�E�g*/,
		 saSE_ERR_NOASSOC         = SE_ERR_NOASSOC         /*�w�肳�ꂽ�t�@�C�����g���q�ɂͤ �A�v���P�[�V�������֘A�t�����Ă��܂���B*/,
		 saSE_ERR_SHARE           = SE_ERR_SHARE           /*���L�ᔽ���������܂����B*/
	};
	//���ع���݂��N������
	bool Shell(const wchar_t *App,const wchar_t *Cmd = L"");
	//���ع���݂��N�����āA�N�����͑҂�
	bool ShellAndWait(const wchar_t *App,const wchar_t *Cmd,int nShow);
};

#endif

