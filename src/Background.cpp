//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include <algorithm>
#include <utility>
#include <tuple>
#include <string>

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
	//透明色の設定
	TransColor = static_cast<TColor>(0x00000001);
	//透明色のセット
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
//initialize
//---------------------------------------------------------------------------
bool TBackGround::Init()
{
	RewindFrame();

	return true;
}

//---------------------------------------------------------------------------
//Result DIB area initialization
//---------------------------------------------------------------------------
bool TBackGround::ClearResultDIB()
{
	int BmpHeight = pDiffBmp.GetHeight();
	int BmpWidth  = pDiffBmp.GetWidth();

	for(int y=0; y<BmpHeight;y++)
	{
		for(int x=0;x < BmpWidth;x++)
		{
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
//Rewind the frame of the background extraction process
//-------------------------------------------------------------
bool TBackGround::RewindFrame()
{
	ClearResultDIB();
	DeciedBlocks.clear();
	CompBlocks.clear();
	AllowMap.clear();

	return true;
}
//-------------------------------------------------------------
//Returns a bitmap of the specified frame
//-------------------------------------------------------------
DIBInf TBackGround::GetFrame(int fr)
{
	String path;
	DIBInf inf;

	try
	{
		path.sprintf(IMAGEPATH.c_str(),ImageolderPath.c_str(),fr);
		if(FileExists(path) == false)
		{
			throw Exception(L"Illegal frame number");
		}
		inf.LoadFromFile(path);

		inf.ConvToDIB();
		int w = pDiffBmp.GetWidth();
		int h = pDiffBmp.GetHeight();

		if(w == 0 || h == 0)
		{
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
//Check for the existence of frames
//-------------------------------------------------------------
bool TBackGround::CheckFrameNumber(int fr)
{
	String path;

	path.sprintf(IMAGEPATH.c_str(),ImageolderPath.c_str(),fr);
	if(FileExists(path) == false)
	{
		return false;
	}

	return true;
}
//-------------------------------------------------------------
//Image storage directory set
//-------------------------------------------------------------
bool TBackGround::SetImaageDir(String path)
{
	ImageolderPath = path;

	return true;
}

//-------------------------------------------------------------
//Advance the background extraction process by one step
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

	TTransparentMode TransMode  = tmFixed;
	pDiffBmp.SetTransparentColor(TransColor);

	TColor dc;
	unsigned char *pdc = reinterpret_cast<unsigned char *>(&dc);

	unsigned char R1,G1,B1;
	unsigned char RR,GR,BR;

	int    blkh;
	int    blkw;
	DIBInf Inf8;

	MN<8>  MN8_block;

	int IdxMaxX = BmpWidth /MVBLOCKH + (BmpWidth %MVBLOCKW==0?0:1);
	int IdxMaxY = BmpHeight/MVBLOCKH + (BmpHeight%MVBLOCKH==0?0:1);

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
			bool IsSet = false;

			PixRGB *pixR = pDiffBmp._RGB(x,y);

			RR = pixR->R;
			GR = pixR->G;
			BR = pixR->B;

			if(RR == RT || GR == GT || BR == BT)
			{
				IsSet = false;

				Inf8.CutOutRectImage(pb1,x,y,x+blkw,y+blkh);
				MN8_block.SetFormDIBInf(Inf8);
				ave_block2 = MN8_block.CalcAverage();
				ave_block  = ave_block2.leng()/sqrt(3.0);
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
			if(IsMove == true || IsSet == true)
			{
				continue;
			}
			if(s_block < S_MAX && ave_block < AVE_MAX)
			{
				decltype(AllowMap)::iterator iter = AllowMap.find({idxX,idxY});
				if(iter == AllowMap.end())
				{
					AllowMap[{idxX,idxY}] = {1,MN8_block,0};
					continue;
				}
				else
				{
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

					if(IsOk == true)
					{
						AllowMap.erase(iter);
					}
					else
					{
						iter->second = {num + 1,MN8_block,same+1};
						continue;
					}
				}
			}

			if(IsUseMN8 == false)
			{
				DeciedBlocks.insert({idxX,idxY});

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
			for(int by = 0;by < blkh;by++)
			{
				for(int bx = 0;bx < blkw;bx++)
				{
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

	int all_pix = IdxMaxX * IdxMaxY;
	int set_pix = 0;

	for(int idxY=0; idxY<IdxMaxY;idxY++)
	{
		int y = idxY * MVBLOCKH;

		blkh = BmpHeight - y < MVBLOCKH?BmpHeight - y:MVBLOCKH;

		for(int idxX=0;idxX < IdxMaxX;idxX++)
		{
			int  x = idxX * MVBLOCKH;

			PixRGB *pix = pDiffBmp._RGB(x,y);

			RR = pix->R;
			GR = pix->G;
			BR = pix->B;

			if(RR == RT && GR == GT && BR == BT)
			{
				//Not yet set
			}
			else
			{
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

