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
	//標準偏差、平均の許容回数
	const int ALLOW_MAX  = 150;
	//標準偏差、平均の許容回数
	const int ALLOW_SAME_MAX  = 5;
protected:
	//取得した背景
	DIBInf pDiffBmp;
	//透明色
	TColor         TransColor;
	unsigned char  *pTrans;
	unsigned char  RT;
	unsigned char  GT;
	unsigned char  BT;
	//標準偏差、平均の許容マップ
	std::map<std::pair<int,int>,std::tuple<int,MN<8>,int>> AllowMap;
	//ファイルパス
	String ImageolderPath;
	const String IMAGEPATH      = L"%s\\image%04d.bmp";
protected:
	//結果DIB初期化
	bool ClearResultDIB();
protected:
	//画像が決定したブロック
	std::set<std::pair<int,int>> DeciedBlocks;
	//画像が補完されたブロック
	std::set<std::pair<int,int>> CompBlocks;
public:
	__fastcall TBackGround(TComponent* AOwner);
	virtual __fastcall ~TBackGround();
public:
	//初期化する
	bool Init();
	//作成中の背景を得る
	bool GetBackgroundImage(DIBInf& BakInf);
	//背景抽出処理を１ステップ進める
	bool ExecuteOneStep(int fr,int& SetBlockNum,int& AllBlockNum);
	//背景抽出処理のフレームを巻き戻す
	bool RewindFrame();
	//指定フレームのビットマップを返す
	DIBInf GetFrame(int fr);
	//フレームの存在をチェック
	bool CheckFrameNumber(int fr);
	//画像の格納ディレクトリセット
	bool SetImaageDir(String path);
};
//---------------------------------------------------------------------------
#endif
