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
//初期化する
//---------------------------------------------------------------------------
bool TBackGround::Init()
{
	//背景抽出処理のフレームを巻き戻す
	RewindFrame();

	return true;
}

//---------------------------------------------------------------------------
//結果のDIBエリア初期化
//---------------------------------------------------------------------------
bool TBackGround::ClearResultDIB()
{
	int BmpHeight = pDiffBmp.GetHeight();
	int BmpWidth  = pDiffBmp.GetWidth();

	//結果のDIBエリア初期化
	for(int y=0; y<BmpHeight;y++)
	{
		for(int x=0;x < BmpWidth;x++)
		{
			//差分用ビットマップのピクセル色セット
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
//  機能     ：背景抽出処理のフレームを巻き戻す
//
//  関数定義 ：bool RewindFrame()
//
//  ｱｸｾｽﾚﾍﾞﾙ ：
//
//  引数     ：
//
//  戻り値   ：true 成功
//
//
//
//  改定者   ：
//-------------------------------------------------------------
bool TBackGround::RewindFrame()
{
	//結果DIB初期化
	ClearResultDIB();
	//画像が決定したブロッククリア
	DeciedBlocks.clear();
	//画像が補完されたブロッククリア
	CompBlocks.clear();
	//標準偏差、平均の許容マップクリア
	AllowMap.clear();

	return true;
}
//-------------------------------------------------------------
//  機能     ：指定フレームのビットマップを返す
//
//  関数定義 ：TBitmap* GetFrame(int fr)
//
//  ｱｸｾｽﾚﾍﾞﾙ ：
//
//  引数     ：
//
//  戻り値   ：true 成功
//
//
//
//  改定者   ：
//-------------------------------------------------------------
DIBInf TBackGround::GetFrame(int fr)
{
	String path;
	//DIBInf作成
	DIBInf inf;

	try
	{
		//ファイルパス作成
		path.sprintf(IMAGEPATH.c_str(),ImageolderPath.c_str(),fr);
		//ファイルの存在をチェック
		if(FileExists(path) == false)
		{
			throw Exception(L"フレーム番号不正");
		}
		//ファイルを読み込む
		inf.LoadFromFile(path);

		//DIB作成
		inf.ConvToDIB();
		//pDiffBmpが初期化されていない場合
		int w = pDiffBmp.GetWidth();
		int h = pDiffBmp.GetHeight();

		if(w == 0 || h == 0)
		{
			//結果DIB初期化
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
//  機能     ：フレームの存在をチェック
//
//  関数定義 ：bool CheckFrameNumber(int fr)
//
//  ｱｸｾｽﾚﾍﾞﾙ ：
//
//  引数     ：
//
//  戻り値   ：true フレームはある
//
//
//
//  改定者   ：
//-------------------------------------------------------------
bool TBackGround::CheckFrameNumber(int fr)
{
	String path;

	//ファイルパス作成
	path.sprintf(IMAGEPATH.c_str(),ImageolderPath.c_str(),fr);
	//ファイルの存在をチェック
	if(FileExists(path) == false)
	{
		return false;
	}

	return true;
}
//-------------------------------------------------------------
//画像の格納ディレクトリセット
//-------------------------------------------------------------
bool TBackGround::SetImaageDir(String path)
{
	ImageolderPath = path;

	return true;
}

//-------------------------------------------------------------
//  機能     ：背景抽出処理を１ステップ進める
//
//  関数定義 ：bool ExecuteOneStep(int fr,int& SetBlockNum,int& AllBlockNum)
//
//  ｱｸｾｽﾚﾍﾞﾙ ：
//
//  引数     ：
//
//  戻り値   ：true 成功
//
//
//
//  改定者   ：
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
	//透明色の設定
	TTransparentMode TransMode  = tmFixed;
	pDiffBmp.SetTransparentColor(TransColor);

	//データをコピー
	TColor dc;
	unsigned char *pdc = reinterpret_cast<unsigned char *>(&dc);

	unsigned char R1,G1,B1;
	unsigned char RR,GR,BR;

	//ブロックごとの動の有無をセット
	int    blkh;
	int    blkw;
	DIBInf Inf8;
	//ブロック全体
	MN<8>  MN8_block;

	int IdxMaxX = BmpWidth /MVBLOCKH + (BmpWidth %MVBLOCKW==0?0:1);
	int IdxMaxY = BmpHeight/MVBLOCKH + (BmpHeight%MVBLOCKH==0?0:1);

	//許容移動点の設定
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


	//平均と標準偏差
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
			//背景抽出済みか
			bool IsSet = false;

			PixRGB *pixR = pDiffBmp._RGB(x,y);

			RR = pixR->R;
			GR = pixR->G;
			BR = pixR->B;

			if(RR == RT || GR == GT || BR == BT)
			{
				IsSet = false;

				//MN<8>にセット
				Inf8.CutOutRectImage(pb1,x,y,x+blkw,y+blkh);
				//平均
				MN8_block.SetFormDIBInf(Inf8);
				ave_block2 = MN8_block.CalcAverage();
				ave_block  = ave_block2.leng()/sqrt(3.0);
				//標準偏差
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
			//動いたか
			if(IsMove == true || IsSet == true)
			{
				continue;
			}
			//標準偏差チェック
			if(s_block < S_MAX && ave_block < AVE_MAX)
			{
				//標準偏差、平均の許容マップ
				decltype(AllowMap)::iterator iter = AllowMap.find({idxX,idxY});
				if(iter == AllowMap.end())
				{
					AllowMap[{idxX,idxY}] = {1,MN8_block,0};
					continue;
				}
				else
				{

					//チェック
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

					//回数チェック
					if(IsOk == true)
					{
						AllowMap.erase(iter);
					}
					else
					{
						//チェック情報更新
						iter->second = {num + 1,MN8_block,same+1};
						continue;
					}
				}
			}


			//画像が決定したブロック
			if(IsUseMN8 == false)
			{
				DeciedBlocks.insert({idxX,idxY});
				//補完ブロックであれば削除
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
			//画像のピクセルセット
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
	//進捗状況
	int all_pix = IdxMaxX * IdxMaxY;
	int set_pix = 0;

	for(int idxY=0; idxY<IdxMaxY;idxY++)
	{
		int y = idxY * MVBLOCKH;

		blkh = BmpHeight - y < MVBLOCKH?BmpHeight - y:MVBLOCKH;

		for(int idxX=0;idxX < IdxMaxX;idxX++)
		{
			int  x = idxX * MVBLOCKH;
			//ブロック左上の色取得
			PixRGB *pix = pDiffBmp._RGB(x,y);

			RR = pix->R;
			GR = pix->G;
			BR = pix->B;
			//すでに色が設定されているか
			if(RR == RT && GR == GT && BR == BT)
			{
				//まだ設定されていない
			}
			else
			{
				//設定されている
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

