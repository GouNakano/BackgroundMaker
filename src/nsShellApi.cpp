
#include "nsShellApi.h"
//C+Builder�Ǝ�
#ifdef __BORLANDC__
#include <vcl.h>
#endif

namespace nsShellApi
{
	ShellApiErr LastErr; /*�Ō�̔��������װ*/

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
//    01/03/03  
//
//  �����   �F
//  �����   �F
//
//=============================================================

//-------------------------------------------------------------
//  �@�\     �F���ع���݂��N������
//
//  �֐���` �Fbool Shell(const char *App,const char *Cmd)
//
//  �������� �Fpublic
//
//  ����     �Fconst char *App ���ع�����߽
//             const char *Cmd �����ײ݈���
//
//  �߂�l   �Fbool �����H
//
//    
//
//  �����   �F
//-------------------------------------------------------------
bool Shell(const wchar_t *App,const wchar_t *Cmd)
{
	HINSTANCE Inst;
	bool      Res;

	//���ع�����߽����
	if(wcscmp(App,L"") == 0)
	{
		LastErr = saAppNameNothing; /*���ع���ݖ�������*/
		return false;
	}
	//���ع���݋N��
	if(wcscmp(Cmd,L"") == 0)
		Inst = ShellExecuteW(NULL,L"open",App,NULL,NULL,SW_SHOW);
	else
		Inst = ShellExecuteW(NULL,L"open",App,Cmd,NULL,SW_SHOW);
	//�װ����
	if((int)Inst > 32)
	{
		//����
		LastErr = saGood; /*����*/
		Res = true;
	}
	else
	{
		//���s
		//�ݽ�ݽ(���̏ꍇ�װ����)��ShellApiErr�^�ɕϊ�
		LastErr = (ShellApiErr)((int)Inst);
		Res     = false;
	}
	return Res;
}
//-------------------------------------------------------------
//  �@�\     �F�ŏI�װ���擾����
//
//  �֐���` �FShellApiErr GetLastErr()
//
//  �������� �Fpublic
//
//  ����     �F
//
//  �߂�l   �FShellApiErr  �װ
//
//    
//
//  �����   �F
//-------------------------------------------------------------
ShellApiErr GetLastErr()
{
	return LastErr;
}


//-------------------------------------------------------------
//  �@�\     �F���ع���݂��N�����āA�N�����͑҂�
//
//  �֐���` �Fbool ShellAndWait(const char *App,const char *Cmd)
//
//  �������� �Fpublic
//
//  ����     �Fconst char *App ���ع�����߽
//             const char *Cmd �����ײ݈���
//
//  �߂�l   �Fbool �����H
//
//    
//
//  �����   �F
//-------------------------------------------------------------
bool ShellAndWait(const wchar_t *App,const wchar_t *Cmd,int nShow)
{
	//���ع���݋N��
	SHELLEXECUTEINFOW execinfo;
	memset(&execinfo,0,sizeof(execinfo));
	execinfo.cbSize       = sizeof(execinfo);
	execinfo.lpVerb       = L"open";
	execinfo.lpFile       = App;
	execinfo.lpParameters = Cmd;
	execinfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
	execinfo.nShow        = nShow;
//	execinfo.nShow        = SW_SHOWDEFAULT;

	if(!ShellExecuteExW(&execinfo))
	{
		return false;
	}
	//�N���������ع���݂��I������܂ő҂�
	while(1)
	{
		//��۾��҂����s��
		DWORD ProcStatus = WaitForSingleObject(execinfo.hProcess,100);
		//��ѱ�Ď��̏���
#ifdef __BCPLUSPLUS__
		//C++Builder��ү���ޏ���
		Application->ProcessMessages();
#endif
		//��۾���Ԃ�����
		if(ProcStatus != WAIT_TIMEOUT)break;
	}
	return true;
}


}
