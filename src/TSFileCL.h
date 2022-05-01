//---------------------------------------------------------------------------
#ifndef TSFileCLH
#define TSFileCLH
//---------------------------------------------------------------------------

#include "nsList.h"
#include <windows.h>
#include <stdio.h>
#include <dos.h>
#include <io.h>
#ifdef  _MSC_VER
#include<direct.h>
#include<sys/types.h>
#include<sys/stat.h>
#else
#include <dir.h>
#endif

//=============================================================
//
//  ���і��� �F
//
//  ��޼��і��F
//
//  ���O��� �F�Ȃ�
//
//  �N���X�� �FTSDirectoryCL
//
//  �@�\     �F��ʂ̃f�B���N�g���֘A������܂Ƃ߂�����
//
//  �p����   �F
//
//  �쐬��   �F
//
//  �����   �F
//  �����   �F
//
//=============================================================
class TSDirectoryCL
{
private:
	//۰�ٸ׽
	class TS_ffblk: public ffblk
	{
	public:
		char BackupFile[MAX_PATH];
	};
private:
	char             BackupDir[MAX_PATH]; //�o�b�N�A�b�v�t�@�C���f�B���N�g��
	nsList<TS_ffblk> DelfList;  //�폜�ς�̧��ؽ�
private:
	bool ProcDeleteDirectory(const char *Path);
public:
	 TSDirectoryCL();    //�R���X�g���N�^
	~TSDirectoryCL();    //�f�X�g���N�^
public:
	bool CopyFile(const char *Src,const char *Dest);
	bool DirectoryCopy(const char *Sce,const char *Des); //�ިڸ�؂̺�߰
	bool DeleteFile(const char *Path);
	bool MakeDirectory(const char *Path);
	bool MakeDirectory(const wchar_t *Path);
	bool DeleteDirectory(const char *Path);
	bool CheckExistFile(const char *Path);
	bool SetCurrentDirectory(const char *Path);
	//̧��(�ިڸ��)�̈ړ�(���͖��̕ύX)
	bool MoveFile(const char *src,const char *dest);
public:
	//Unicode
	bool CopyFile(const wchar_t *Src,const wchar_t *Dest);
	bool DirectoryCopy(const wchar_t *Sce,const wchar_t *Des);
	bool DeleteDirectory(const wchar_t *Path);
};

#endif //TSFileH
