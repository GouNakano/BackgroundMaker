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
	const int ALLOW_MAX  = 150;
	const int ALLOW_SAME_MAX  = 5;
protected:
	DIBInf pDiffBmp;
	TColor         TransColor;
	unsigned char  *pTrans;
	unsigned char  RT;
	unsigned char  GT;
	unsigned char  BT;

	std::map<std::pair<int,int>,std::tuple<int,MN<8>,int>> AllowMap;

	String ImageolderPath;
	const String IMAGEPATH = L"%s\\image%04d.bmp";
protected:
	bool ClearResultDIB();
protected:
	std::set<std::pair<int,int>> DeciedBlocks;
	std::set<std::pair<int,int>> CompBlocks;
public:
	__fastcall TBackGround(TComponent* AOwner);
	virtual __fastcall ~TBackGround();
public:
	bool Init();
	bool GetBackgroundImage(DIBInf& BakInf);
	bool ExecuteOneStep(int fr,int& SetBlockNum,int& AllBlockNum);
	bool RewindFrame();
	DIBInf GetFrame(int fr);
	bool CheckFrameNumber(int fr);
	bool SetImaageDir(String path);
};
//---------------------------------------------------------------------------
#endif
