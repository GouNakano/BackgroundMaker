#include <System.hpp>
#include <shellapi.h>
#include <io.h>
#include "strconv.h"
#include "TSFileCL.h"

//-------------------------------------------------------------
//  �@�\     �FTSDirectoryCL�̃R���X�g���N�^
//
//  �֐���` �FTSDirectoryCL()
//
//  �������� �Fpublic
//
//  ����     �F�Ȃ�
//
//  �߂�l   �F�Ȃ�
//
//  �쐬��   �F
//
//  �����   �F
//-------------------------------------------------------------
TSDirectoryCL::TSDirectoryCL()
{
	//�ꎞ�f�B���N�g���𓾂�
	::GetTempPathA(MAX_PATH,BackupDir);
}
//-------------------------------------------------------------
//  �@�\     �FTSDirectoryCL�̃f�X�g���N�^
//
//  �֐���` �F~TSDirectoryCL()
//
//  �������� �Fpublic
//
//  ����     �F�Ȃ�
//
//  �߂�l   �F�Ȃ�
//
//  �쐬��   �F    
//
//  �����   �F
//-------------------------------------------------------------
TSDirectoryCL::~TSDirectoryCL()
{
}
//-------------------------------------------------------------
//  �@�\     �F̧�ق̺�߰
//
//  �֐���` �Fbool CopyFile(const char *Src,const char *Dest)
//
//  �������� �Fpublic
//
//  ����     �Fconst char *Src   ��߰����̧��
//           �Fconst char *Dest  ��߰���̧��
//
//  �߂�l   �Fbool ���� true  ���s false
//
//  �쐬�ҁ@ �F
//
//  �����   �F
//-------------------------------------------------------------
bool TSDirectoryCL::CopyFile(const char *Src,const char *Dest)
{
	char *pFrom = new char[strlen(Src)  + 2];
	char *pTo   = new char[strlen(Dest) + 2];
	bool  fResult,Result;
	SHFILEOPSTRUCTA FileOP;

	strcpy(pFrom,Src);
	pFrom[strlen(Src)  + 1] = '\0';
	strcpy(pTo,Dest);
	pTo[strlen(Dest)  + 1] = '\0';


	FileOP.hwnd                  = NULL;
	FileOP.wFunc                 = FO_COPY;
	FileOP.pFrom                 = pFrom;
	FileOP.pTo                   = pTo;
	FileOP.fFlags                = FOF_SILENT;
	FileOP.fAnyOperationsAborted = FALSE;
	FileOP.hNameMappings         = NULL;
	FileOP.lpszProgressTitle     = "";

	try{
		fResult = (SHFileOperationA(&FileOP) == 0);
	}catch(...){}

	Result  = (fResult == true && FileOP.fAnyOperationsAborted == FALSE);
	delete [] pFrom;
	delete [] pTo;

	return Result;
}
bool TSDirectoryCL::CopyFile(const wchar_t *Src,const wchar_t *Dest)
{
    std::string s = wide_to_ansi(Src);
    std::string d = wide_to_ansi(Dest);

	//���\�b�h���Ăяo��
	bool Res = CopyFile(s.c_str(),d.c_str());

	return Res;
}
//-------------------------------------------------------------
//  �@�\     �F�ިڸ�؂̺�߰
//
//  �֐���` �Fbool DirectoryCopy(char *Sce,char *Des)
//
//  �������� �Fpublic
//
//  ����     �Fconst char *Sce   ��߰�����ިڸ��
//           �Fconst char *Des   ��߰����ިڸ��
//
//  �߂�l   �Fbool ���� true  ���s false
//
//  �쐬�ҁ@ �F
//
//  �����   �F
//-------------------------------------------------------------
bool TSDirectoryCL::DirectoryCopy(const char *Sce,const char *Des)
{
	SHFILEOPSTRUCTA FOperation;
	char SceDir[262];
	char DesDir[262];

	memset(SceDir,0,sizeof(SceDir));
	memset(DesDir,0,sizeof(DesDir));

	strcpy(SceDir,Sce);
	SceDir[strlen(Sce)+1]='\0';
	strcpy(DesDir,Des);
	DesDir[strlen(Des)+1]='\0';

	FOperation.hwnd=NULL;
	FOperation.wFunc=FO_COPY;
	FOperation.pFrom=SceDir;
	FOperation.pTo=DesDir;
	FOperation.fAnyOperationsAborted=false;
	FOperation.hNameMappings=NULL;
	FOperation.lpszProgressTitle=NULL;
	FOperation.fFlags= FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR |
						FOF_SILENT | FOF_MULTIDESTFILES;

	return (SHFileOperationA(&FOperation)==0);
}
bool TSDirectoryCL::DirectoryCopy(const wchar_t *Sce,const wchar_t *Des)
{
    std::string s = wide_to_ansi(Sce);
    std::string d = wide_to_ansi(Des);
	//char�^���\�b�h���Ăяo��
	bool Res = DirectoryCopy(s.c_str(),d.c_str());

	return Res;
}
//-------------------------------------------------------------
//  �@�\     �F̧�ق̍폜
//
//  �֐���` �Fbool DeleteFile(const char *Path)
//
//  �������� �Fpublic
//
//  ����     �Fconst char *Path  �폜����̧��
//
//  �߂�l   �Fbool ���� true  ���s false
//
//  �쐬�ҁ@ �F
//
//  �����   �F
//-------------------------------------------------------------
bool TSDirectoryCL::DeleteFile(const char *Path)
{
	char *pFrom = new char[strlen(Path) + 2];
	bool  fResult,Result;
	strcpy(pFrom,Path);
	pFrom[strlen(Path) + 1] = '\0';

	SHFILEOPSTRUCTA FileOP;

	FileOP.hwnd                  = NULL;
	FileOP.wFunc                 = FO_DELETE;
	FileOP.pFrom                 = pFrom;
	FileOP.fFlags                = FOF_SILENT | FOF_NOCONFIRMATION;
	FileOP.fAnyOperationsAborted = FALSE;
	FileOP.hNameMappings         = NULL;
	FileOP.lpszProgressTitle     = "";

	fResult = (SHFileOperationA(&FileOP) == 0);
	Result  = (fResult == true && FileOP.fAnyOperationsAborted == FALSE);
	delete [] pFrom;

	return Result;
}
//-------------------------------------------------------------
//  �@�\     �F�ިڸ�؂̍쐬
//
//  �֐���` �Fbool MakeDirectory(const char *Path)
//
//  �������� �Fpublic
//
//  ����     �Fconst char *Path   �쐬�����ިڸ��
//
//  �߂�l   �Fbool ���� true  ���s false
//
//  �쐬��   �F
//
//  �����   �F
//-------------------------------------------------------------
bool TSDirectoryCL::MakeDirectory(const char *Path)
{
	SECURITY_ATTRIBUTES S_A;


	S_A.nLength              = sizeof(_SECURITY_ATTRIBUTES);
	S_A.lpSecurityDescriptor = NULL;
	S_A.bInheritHandle       = TRUE;

	if (CreateDirectoryA(Path,NULL) == FALSE)
	{
		return false;
	}
	return true;
}
bool TSDirectoryCL::MakeDirectory(const wchar_t *Path)
{
	std::string ph = wide_to_ansi(Path);

	return MakeDirectory(ph.c_str());
}
//-------------------------------------------------------------
//  �@�\     �F�ިڸ�؂̍폜
//
//  �֐���` �Fbool DeleteDirectory(const char *Path)
//
//  �������� �Fpublic
//
//  ����     �Fconst char *Path
//
//  �߂�l   �Fbool ���� true  ���s false
//
//  �쐬��   �F
//
//  �����   �F
//-------------------------------------------------------------
bool TSDirectoryCL::DeleteDirectory(const char *Path)
{
	//̧�ٍ폜����ؽď���
	DelfList.Clear();
	//�w���ިڸ�؈ȉ������ׂč폜
	bool Ret = ProcDeleteDirectory(Path);
	//�����ިڸ�؍폜
	if(Ret == true)
		Ret = (RemoveDirectoryA(Path) == TRUE);
	//��ԕʏ���
	if(Ret == false)
	{
		//�폜�����s�����Ȃ�̧�ق̕�������������
		for(int Cnt = DelfList.GetCount() - 1;Cnt >= 0;Cnt--)
		{
			//̧�ٍ폜�����ƯĎ擾
			TS_ffblk *DelUnit = (TS_ffblk *)DelfList.GetItem(Cnt);
			//�ިڸ�؂�̧�ق�������
#ifndef _MSC_VER 
			if(DelUnit->ff_attrib & FA_DIREC)
#else
			if(DelUnit->attrib & _A_SUBDIR)
#endif
			{
				//�ިڸ�؂̏ꍇ�́A�쐬����
#ifndef _MSC_VER 
				mkdir(DelUnit->ff_name);
#else
				mkdir(DelUnit->name);
#endif
			}
			else
			{
				//̧�ق̏ꍇ�́A�ޯ����߂���̕���
#ifndef _MSC_VER 
				MoveFileExA(DelUnit->BackupFile,DelUnit->ff_name,MOVEFILE_COPY_ALLOWED);
#else
				MoveFileExA(DelUnit->BackupFile,DelUnit->name,MOVEFILE_COPY_ALLOWED);
#endif
			}
		}
	}
	else
	{
		//�폜�����Ȃ�A�ޯ�����̧�ق��폜
		for(int Cnt = DelfList.GetCount() - 1;Cnt >= 0;Cnt--)
		{
			//̧�ٍ폜�����ƯĎ擾
			TS_ffblk *DelUnit = (TS_ffblk *)DelfList.GetItem(Cnt);
			//�ިڸ�؂�̧�ق�������
#ifndef _MSC_VER 
			if(DelUnit->ff_attrib & FA_DIREC)
#else
			if(DelUnit->attrib & _A_SUBDIR)
#endif
			{
				//�ިڸ�؂̏ꍇ�́A�����Ȃ�
			}
			else
			{
				//�ޯ�����̧�ق͍폜
				unlink(DelUnit->BackupFile);
			}
		}
	}
	//̧�ٍ폜����ؽď���
	DelfList.Clear();

	return Ret;
}
bool TSDirectoryCL::DeleteDirectory(const wchar_t *Path)
{
	std::string aPath = wide_to_ansi(Path);
	//char�^���\�b�h���Ăяo��
	bool Res = DeleteDirectory(aPath.c_str());

	return Res;
}

//-------------------------------------------------------------
//  �@�\     �F�ިڸ�؂̍폜�̍ċA�֐�
//
//  �֐���` �Fbool ProcDeleteDirectory(const char *Path)
//
//  �������� �Fpublic
//
//  ����     �Fconst char *Path
//
//  �߂�l   �Fbool ���� true  ���s false
//
//  �쐬��   �F
//
//  �����   �F
//-------------------------------------------------------------
bool TSDirectoryCL::ProcDeleteDirectory(const char *Path)
{
	nsList<TS_ffblk> fList; //̧��ؽ�
	ffblk            Ffblk;
	int              Cnt;
	int              Ret;
	char             path[256];
#ifdef _MSC_VER 
	long hHandle;
#endif
	//�����߽���쐬
	sprintf(path,"%s\\*.*",Path);
    //̧��ؽč쐬
	for(Cnt = 0;;Cnt++)
	{
		//�����֐����Ă�
		if(Cnt == 0)
		{
#ifndef _MSC_VER 
			Ret = findfirst(path,&Ffblk,FA_DIREC|FA_ARCH);
#else
			hHandle = _findfirst(path,&Ffblk);
			Ret = (hHandle == -1L);
#endif
		}
		else
#ifndef _MSC_VER 
			Ret = findnext(&Ffblk);
#else
			Ret = _findnext(hHandle,&Ffblk);
#endif
		//����
		if(Ret != 0)
		{
#ifndef _MSC_VER 
			Ret = findclose(&Ffblk);
#else
			Ret = _findclose(hHandle);
#endif
			break;
		}
		//.��..�͏Ȃ�
#ifndef _MSC_VER 
		if(strcmp(Ffblk.ff_name,".") == 0 || strcmp(Ffblk.ff_name,"..") == 0)continue;
#else
		if(strcmp(Ffblk.name,".") == 0 || strcmp(Ffblk.name,"..") == 0)continue;
#endif
		//ؽĂɒǉ�
		TS_ffblk *fUnit = new TS_ffblk();
		memcpy((ffblk *)fUnit,&Ffblk,sizeof(Ffblk));
#ifndef _MSC_VER
		sprintf(fUnit->ff_name,"%s\\%s",Path,Ffblk.ff_name);
#else
		sprintf(fUnit->name,"%s\\%s",Path,Ffblk.name);
#endif
		fList.Add(fUnit);
	}
	//̧�ق��폜�\������
	for(Cnt = 0;Cnt < fList.GetCount();Cnt++)
	{
		TS_ffblk *NowFile = (TS_ffblk *)fList.GetItem(Cnt);
		//�Ώۂ��ިڸ�؂̏ꍇ�͏��O
#ifndef _MSC_VER 
		if(!(NowFile->ff_attrib & FA_DIREC))
#else
		if(!(NowFile->attrib & _A_SUBDIR))
#endif
		{
        	char TmpFName[L_tmpnam];
			char BackupFullPath[MAX_PATH];
			//������̧�ٖ��擾
			tmpnam(TmpFName);
			//������̧�كt���p�X�g�ݗ���
			sprintf(BackupFullPath,"%s%s",BackupDir,TmpFName);
			//�폜̧�ٗ����Ư�
			TS_ffblk *DelUnit = new TS_ffblk(*NowFile);
			DelfList.Add(DelUnit);
			//�ޯ����ߐ����������
			strcpy(DelUnit->BackupFile,BackupFullPath);
			//��̧�ق��߰
#ifndef _MSC_VER
//            ::CopyFile(NowFile->ff_name,TmpFName,FALSE);
            ::CopyFileA(NowFile->ff_name,BackupFullPath,FALSE);
			int  Res = unlink(NowFile->ff_name);
#else
//            ::CopyFile(NowFile->name,TmpFName,FALSE);
            ::CopyFile(NowFile->name,BackupFullPath,FALSE);
			int  Res = unlink(NowFile->name);
#endif

			//���s������I���
			if(Res != 0)
            {
				return false;
			}
		}
#ifndef _MSC_VER 
		else if(NowFile->ff_attrib & FA_DIREC)
#else
		else if(NowFile->attrib & _A_SUBDIR)
#endif
		{
			//̧�ق��ިڸ�؂Ȃ�A���̊K�w�ɍs��
#ifndef _MSC_VER 
			if(ProcDeleteDirectory(NowFile->ff_name) == false)
			{
				return false;
            }
#else
			if(ProcDeleteDirectory(NowFile->name) == false)return false;
#endif
			//�폜̧�ٗ����Ư�
			TS_ffblk *DelUnit = new TS_ffblk(*NowFile);
			DelfList.Add(DelUnit);
			//̧�ق̍폜�����������Ȃ�A�ިڸ�؂̍폜���s��
#ifndef _MSC_VER 
//			if(rmdir(NowFile->ff_name)!=0)
			try
			{
				RmDir(NowFile->ff_name);
			}
			catch(...)
			{
				return false;
			}
#else
			if(rmdir(NowFile->name)!=0)return false;
#endif
		}
	}
	return true;
}
//-------------------------------------------------------------
//  �@�\     �F̧�ق̑��݂���������
//
//  �֐���` �Fbool CheckExistFile(const char *Path)
//
//  �������� �Fpublic
//
//  ����     �Fconst char *Path
//
//  �߂�l   �Fbool ���� true  �Ȃ� false
//
//  �쐬��   �F    
//
//  �����   �F
//-------------------------------------------------------------
bool TSDirectoryCL::CheckExistFile(const char *Path)
{
	return (::access(Path,0) == 0);
}
//-------------------------------------------------------------
//  �@�\     �F�����ިڸ�؂�ݒ肷��
//
//  �֐���` �Fbool SetCurrentDirectory(const char *Path)
//
//  �������� �Fpublic
//
//  ����     �Fconst char *Path �ݒ肷������ިڸ��
//
//  �߂�l   �Fbool ���� true  �Ȃ� false
//
//  �쐬��   �F
//
//  �����   �F
//-------------------------------------------------------------
bool TSDirectoryCL::SetCurrentDirectory(const char *Path)
{
	return (::SetCurrentDirectoryA(Path) == TRUE);
}
//-------------------------------------------------------------
//  �@�\     �F̧��(�ިڸ��)�̈ړ�(���͖��̕ύX)
//
//  �֐���` �Fbool MoveFile(const char *src,const char *dest)
//
//  �������� �Fpublic
//
//  ����     �Fconst char *src  �ړ���̧��(�ިڸ��)�����߽
//             const char *dest �ړ���̧��(�ިڸ��)�����߽
//
//  �߂�l   �Fbool  true  OK false NG
//
//  �쐬��   �F
//
//  �����   �F
//-------------------------------------------------------------
bool TSDirectoryCL::MoveFile(const char *src,const char *dest)
{
	return (::MoveFileA(src,dest) == TRUE);
}


