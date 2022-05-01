//---------------------------------------------------------------------------

#ifndef BackgroundH
#define BackgroundH
//---------------------------------------------------------------------------
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <vector>
#include <map>
#include <set>
#include <stdexcept>
#include <io.h>
#include <string.h>
#include "nsVector.h"
#include "GDIplusUnit.h"
#include "DIBInf.h"
#include "MN.h"
//---------------------------------------------------------------------------
class TBackGround : public TImage
{
public:
	static const int MVBLOCKW;
	static const int MVBLOCKH;
	static const int MVBLOCKHW;
public:
	//�W���΍��A���ς̋��e��
	const int ALLOW_MAX  = 150;
	//�W���΍��A���ς̋��e��
	const int ALLOW_SAME_MAX  = 5;
protected:
	//�擾�����w�i
	DIBInf pDiffBmp;
	//�����F
	TColor         TransColor;
	unsigned char  *pTrans;
	unsigned char  RT;
	unsigned char  GT;
	unsigned char  BT;
	//�W���΍��A���ς̋��e�}�b�v
	std::map<std::pair<int,int>,std::tuple<int,MN<8>,int>> AllowMap;
	//�t�@�C���p�X
	String ImageolderPath;
	const String IMAGEPATH      = L"%s\\image%04d.bmp";
protected:
	//����DIB������
	bool ClearResultDIB();
protected:
	//�摜�����肵���u���b�N
	std::set<std::pair<int,int>> DeciedBlocks;
	//�摜���⊮���ꂽ�u���b�N
	std::set<std::pair<int,int>> CompBlocks;
public:
	__fastcall TBackGround(TComponent* AOwner);
	virtual __fastcall ~TBackGround();
public:
	//����������
	bool Init();
	//�쐬���̔w�i�𓾂�
	bool GetBackgroundImage(DIBInf& BakInf);
	//�w�i���o�������P�X�e�b�v�i�߂�
	bool ExecuteOneStep(int fr,int& SetBlockNum,int& AllBlockNum);
	//�w�i���o�����̃t���[���������߂�
	bool RewindFrame();
	//�w��t���[���̃r�b�g�}�b�v��Ԃ�
	DIBInf GetFrame(int fr);
	//�t���[���̑��݂��`�F�b�N
	bool CheckFrameNumber(int fr);
	//�摜�̊i�[�f�B���N�g���Z�b�g
	bool SetImaageDir(String path);
};
//---------------------------------------------------------------------------
#endif
