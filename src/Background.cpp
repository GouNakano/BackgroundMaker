//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include <algorithm>
#include <utility>
#include <tuple>
#include <string>

#include "nsDebug.h"
#include "nsSearchDir.h"
#include "Background.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
const int    TBackGround::MVBLOCKW   =    8;
const int    TBackGround::MVBLOCKH   =    TBackGround::MVBLOCKW;
const int    TBackGround::MVBLOCKHW  =   64;

//---------------------------------------------------------------------------
// constructor
//---------------------------------------------------------------------------
__fastcall TBackGround::TBackGround(TComponent* AOwner)
:TImage(AOwner)
{
	//�����F�̐ݒ�
	TransColor = static_cast<TColor>(0x00000001);
	//�����F�̃Z�b�g
	pTrans  = reinterpret_cast<unsigned char *>(&TransColor);

	RT = pTrans[0];
	GT = pTrans[1];
	BT = pTrans[2];
}
//---------------------------------------------------------------------------
//destructor
//---------------------------------------------------------------------------
TBackGround::~TBackGround()
{
}
//---------------------------------------------------------------------------
//Get the background being created
//---------------------------------------------------------------------------
bool TBackGround::GetBackgroundImage(DIBInf& BakInf)
{
	BakInf = pDiffBmp;

	return true;
}
//---------------------------------------------------------------------------
//����������
//---------------------------------------------------------------------------
bool TBackGround::Init()
{
	//�w�i���o�����̃t���[���������߂�
	RewindFrame();

	return true;
}

//---------------------------------------------------------------------------
//���ʂ�DIB�G���A������
//---------------------------------------------------------------------------
bool TBackGround::ClearResultDIB()
{
	int BmpHeight = pDiffBmp.GetHeight();
	int BmpWidth  = pDiffBmp.GetWidth();

	//���ʂ�DIB�G���A������
	for(int y=0; y<BmpHeight;y++)
	{
		for(int x=0;x < BmpWidth;x++)
		{
			//�����p�r�b�g�}�b�v�̃s�N�Z���F�Z�b�g
			PixRGB *px = pDiffBmp._RGB(x,y);

			px->R = RT;
			px->G = GT;
			px->B = BT;
		}
	}
	pDiffBmp.ConvDibToBitmap();

	return true;
}

//-------------------------------------------------------------
//  �@�\     �F�w�i���o�����̃t���[���������߂�
//
//  �֐���` �Fbool RewindFrame()
//
//  �������� �F
//
//  ����     �F
//
//  �߂�l   �Ftrue ����
//
//
//
//  �����   �F
//-------------------------------------------------------------
bool TBackGround::RewindFrame()
{
	//����DIB������
	ClearResultDIB();
	//�摜�����肵���u���b�N�N���A
	DeciedBlocks.clear();
	//�摜���⊮���ꂽ�u���b�N�N���A
	CompBlocks.clear();
	//�W���΍��A���ς̋��e�}�b�v�N���A
	AllowMap.clear();

	return true;
}
//-------------------------------------------------------------
//  �@�\     �F�w��t���[���̃r�b�g�}�b�v��Ԃ�
//
//  �֐���` �FTBitmap* GetFrame(int fr)
//
//  �������� �F
//
//  ����     �F
//
//  �߂�l   �Ftrue ����
//
//
//
//  �����   �F
//-------------------------------------------------------------
DIBInf TBackGround::GetFrame(int fr)
{
	String path;
	//DIBInf�쐬
	DIBInf inf;

	try
	{
		//�t�@�C���p�X�쐬
		path.sprintf(IMAGEPATH.c_str(),ImageolderPath.c_str(),fr);
		//�t�@�C���̑��݂��`�F�b�N
		if(FileExists(path) == false)
		{
			throw Exception(L"�t���[���ԍ��s��");
		}
		//�t�@�C����ǂݍ���
		inf.LoadFromFile(path);

		//DIB�쐬
		inf.ConvToDIB();
		//pDiffBmp������������Ă��Ȃ��ꍇ
		int w = pDiffBmp.GetWidth();
		int h = pDiffBmp.GetHeight();

		if(w == 0 || h == 0)
		{
			//����DIB������
			pDiffBmp = inf;
			ClearResultDIB();
		}
	}
	catch(Exception& e)
	{
		throw e;
	}

	return inf;
}

//-------------------------------------------------------------
//  �@�\     �F�t���[���̑��݂��`�F�b�N
//
//  �֐���` �Fbool CheckFrameNumber(int fr)
//
//  �������� �F
//
//  ����     �F
//
//  �߂�l   �Ftrue �t���[���͂���
//
//
//
//  �����   �F
//-------------------------------------------------------------
bool TBackGround::CheckFrameNumber(int fr)
{
	String path;

	//�t�@�C���p�X�쐬
	path.sprintf(IMAGEPATH.c_str(),ImageolderPath.c_str(),fr);
	//�t�@�C���̑��݂��`�F�b�N
	if(FileExists(path) == false)
	{
		return false;
	}

	return true;
}
//-------------------------------------------------------------
//�摜�̊i�[�f�B���N�g���Z�b�g
//-------------------------------------------------------------
bool TBackGround::SetImaageDir(String path)
{
	ImageolderPath = path;

	return true;
}

//-------------------------------------------------------------
//  �@�\     �F�w�i���o�������P�X�e�b�v�i�߂�
//
//  �֐���` �Fbool ExecuteOneStep(int fr,int& SetBlockNum,int& AllBlockNum)
//
//  �������� �F
//
//  ����     �F
//
//  �߂�l   �Ftrue ����
//
//
//
//  �����   �F
//-------------------------------------------------------------
bool TBackGround::ExecuteOneStep(int fr,int& SetBlockNum,int& AllBlockNum)
{
	const double AVE_MAX = 96.0;
	const double S_MAX   =  8.0;

	DIBInf BinInf  = GetFrame(fr+0);
	DIBInf BinInf5 = GetFrame(fr+5);

	BinInf.Sub(BinInf5);
	BinInf.White(8);
	BinInf.ConvDibToBitmap();

	DIBInf pb1 = GetFrame(fr+0);

	int BmpHeight = BinInf.GetHeight();
	int BmpWidth  = BinInf.GetWidth();
	//�����F�̐ݒ�
	TTransparentMode TransMode  = tmFixed;
	pDiffBmp.SetTransparentColor(TransColor);

	//�f�[�^���R�s�[
	TColor dc;
	unsigned char *pdc = reinterpret_cast<unsigned char *>(&dc);

	unsigned char R1,G1,B1;
	unsigned char RR,GR,BR;

	//�u���b�N���Ƃ̓��̗L�����Z�b�g
	int    blkh;
	int    blkw;
	DIBInf Inf8;
	//�u���b�N�S��
	MN<8>  MN8_block;

	int IdxMaxX = BmpWidth /MVBLOCKH + (BmpWidth %MVBLOCKW==0?0:1);
	int IdxMaxY = BmpHeight/MVBLOCKH + (BmpHeight%MVBLOCKH==0?0:1);

	//���e�ړ��_�̐ݒ�
	int    AllowMovePts;
	int    AllowMaskPts;
	double Progress;


	if(AllBlockNum < 1)
	{
		Progress = 0;
	}
	else
	{
		Progress = (100.0f * SetBlockNum) / AllBlockNum;
	}

	if(Progress < 80.0)
	{
		AllowMovePts = 0;
		AllowMaskPts = 0;
	}
	else if(Progress < 90.0)
	{
		AllowMovePts = 1;
		AllowMaskPts = 1;
	}
	else if(Progress < 95.0)
	{
		AllowMovePts = 3;
		AllowMaskPts = 2;
	}
	else if(Progress < 99.7)
	{
		AllowMovePts = 5;
		AllowMaskPts = 3;
	}
	else
	{
		AllowMovePts = 64;
		AllowMaskPts = 6;
	}


	//���ςƕW���΍�
	double   ave_block;
	nsVector ave_block2;
	double   s_block;
	nsVector s_block2;

	for(int idxY=0; idxY<IdxMaxY;idxY++)
	{
		int y = idxY * MVBLOCKH;

		blkh = BmpHeight - y < MVBLOCKH?BmpHeight - y:MVBLOCKH;

		for(int idxX=0;idxX < IdxMaxX;idxX++)
		{
			bool IsMove = false;
			int  x      = idxX * MVBLOCKH;

			blkw = BmpWidth - x < MVBLOCKW?BmpWidth - x:MVBLOCKW;

			if(DeciedBlocks.find({idxX,idxY}) != DeciedBlocks.end())
			{
				if(CompBlocks.find({idxX,idxY}) == CompBlocks.end())
				{
					continue;
				}
			}
			//�w�i���o�ς݂�
			bool IsSet = false;

			PixRGB *pixR = pDiffBmp._RGB(x,y);

			RR = pixR->R;
			GR = pixR->G;
			BR = pixR->B;

			if(RR == RT || GR == GT || BR == BT)
			{
				IsSet = false;

				//MN<8>�ɃZ�b�g
				Inf8.CutOutRectImage(pb1,x,y,x+blkw,y+blkh);
				//����
				MN8_block.SetFormDIBInf(Inf8);
				ave_block2 = MN8_block.CalcAverage();
				ave_block  = ave_block2.leng()/sqrt(3.0);
				//�W���΍�
				s_block2  = MN8_block.CalcStdDev(ave_block2);
				s_block   = s_block2.leng()/sqrt(3);
			}
			else
			{
				IsSet = true;
			}

			bool IsUseMN8 = false;

			if(IsSet == false)
			{
				bool IsComp = (CompBlocks.find({idxX,idxY}) != CompBlocks.end());


				if(IsComp == false && s_block < 3.0f && s_block > 0.01f)
				{
					IsMove = false;
					IsUseMN8 = true;
				}
				else
				{
					int MoveCount = 0;

					for(int by = 0;by < blkh;by++)
					{
						for(int bx = 0;bx < blkw;bx++)
						{
							if(BinInf.GetGray(x+bx,y+by) == tbsFull)
							{
								MoveCount++;
								if(MoveCount > AllowMovePts)
								{
									IsMove = true;
									break;
								}
							}
						}
						if(IsMove == true)
						{
							break;
						}
					}
				}
			}
			else
			{
				IsMove = false;
			}
			//��������
			if(IsMove == true || IsSet == true)
			{
				continue;
			}
			//�W���΍��`�F�b�N
			if(s_block < S_MAX && ave_block < AVE_MAX)
			{
				//�W���΍��A���ς̋��e�}�b�v
				decltype(AllowMap)::iterator iter = AllowMap.find({idxX,idxY});
				if(iter == AllowMap.end())
				{
					AllowMap[{idxX,idxY}] = {1,MN8_block,0};
					continue;
				}
				else
				{

					//�`�F�b�N
					std::tuple<int,MN<8>,int>& chk = iter->second;
					int    num  = std::get<0>(chk);
					MN<8>& mn   = std::get<1>(chk);
					int    same = std::get<2>(chk);

					bool IsOk = false;

					if(mn == MN8_block)
					{
						if(same > ALLOW_SAME_MAX)
						{
							IsOk = true;
						}
					}
					else
					{
						if(num > ALLOW_MAX)
						{
							IsOk = true;
						}
						else
						{
							same =0;
						}
					}

					//�񐔃`�F�b�N
					if(IsOk == true)
					{
						AllowMap.erase(iter);
					}
					else
					{
						//�`�F�b�N���X�V
						iter->second = {num + 1,MN8_block,same+1};
						continue;
					}
				}
			}


			//�摜�����肵���u���b�N
			if(IsUseMN8 == false)
			{
				DeciedBlocks.insert({idxX,idxY});
				//�⊮�u���b�N�ł���΍폜
				decltype(CompBlocks)::iterator iter = CompBlocks.find({idxX,idxY});

				if(iter != CompBlocks.end())
				{
					CompBlocks.erase(iter);
				}
			}
			else
			{
				CompBlocks.insert({idxX,idxY});
			}
			//�摜�̃s�N�Z���Z�b�g
			for(int by = 0;by < blkh;by++)
			{
				for(int bx = 0;bx < blkw;bx++)
				{
					//set color
					PixRGB *pixf = pb1._RGB(x+bx,y+by);
					PixRGB *pix  = pDiffBmp._RGB(x+bx,y+by);

					pix->R = pixf->R;
					pix->G = pixf->G;
					pix->B = pixf->B;
				}
			}
		}
	}
	//convert to bitmap
	pDiffBmp.ConvDibToBitmap();
	//�i����
	int all_pix = IdxMaxX * IdxMaxY;
	int set_pix = 0;

	for(int idxY=0; idxY<IdxMaxY;idxY++)
	{
		int y = idxY * MVBLOCKH;

		blkh = BmpHeight - y < MVBLOCKH?BmpHeight - y:MVBLOCKH;

		for(int idxX=0;idxX < IdxMaxX;idxX++)
		{
			int  x = idxX * MVBLOCKH;
			//�u���b�N����̐F�擾
			PixRGB *pix = pDiffBmp._RGB(x,y);

			RR = pix->R;
			GR = pix->G;
			BR = pix->B;
			//���łɐF���ݒ肳��Ă��邩
			if(RR == RT && GR == GT && BR == BT)
			{
				//�܂��ݒ肳��Ă��Ȃ�
			}
			else
			{
				//�ݒ肳��Ă���
				set_pix++;
			}
		}
	}
	//Completion rate
	SetBlockNum = set_pix;
	AllBlockNum = all_pix;

	return true;
}


//---------------------------------------------------------------------------

