//---------------------------------------------------------------------------
#ifndef DIBInfH
#define DIBInfH
//---------------------------------------------------------------------------
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "GDIplusUnit.h"

//背景差分2,3値化情報
enum typBackSub {tbsNone=0 /*背景差分なし*/ ,tbsHalf=128 /*片方だけ背景差分がある*/,tbsFull=255 /*両方背景差分がある*/};

//---------------------------------------------------------------------------
//ピクセルアクセス構造体
//---------------------------------------------------------------------------
struct PixRGB
{
	unsigned char B;
	unsigned char G;
	unsigned char R;
};

//DIB情報
struct DIBInf
{
protected:
	TBitmap       *pBmp;
	unsigned int   infinsz;
	unsigned int   imginsz;
	unsigned char *pImg;
	PBITMAPINFO    hr;
	int            ByteOfPixel;
private:
	//コピー
	void _copy(const DIBInf& h);
	//開放
	void _free();
public:
	//コンストラクタ
	DIBInf();
	DIBInf(TBitmap *pb);
	DIBInf(const String& file);
	DIBInf(const DIBInf& h);

	//デストラクタ
	virtual ~DIBInf();
public:
	DIBInf& operator = (const DIBInf& h);
protected:
	//座標チェック
	bool ChechPos(int x,int y);
public:
	//初期化
	bool Init();
	//ビットマップを得る
	TBitmap *GetBitmap();
	//DIB化
	bool ConvToDIB(int width,int height,TPixelFormat pf = pf24bit);
	bool ConvToDIB(TPixelFormat pf = pf24bit);
	//DIB->Bitmap化
	bool ConvDibToBitmap();
	bool ConvToBitmap();
	//0で埋める
	bool FillZero();
	//透明色
	bool SetTransparentColor(TColor tc);
public:
	//キャンバス
	TCanvas *GetCanvas();
	//幅、高さ
	int  __fastcall GetWidth();
	void __fastcall SetWidth(int w);
	int  __fastcall GetHeight();
	void __fastcall SetHeight(int h);
public:
	bool SetGray(int x,int y,unsigned char val);
	bool AddGray(int x,int y,unsigned char val);
	unsigned char GetGray(int x,int y);
	bool ChangeToGrayScale();
	bool SaveToFile(String path);
	//ファイル読み込み
	bool LoadFromFile(String path,bool IsMakeDIB = false);
	//目的の値が最初のYの検索
	int SrchMinY(unsigned char val);
	//指定されたY座標で目的の値があるX座標を返す
	int ScanLine(int y,unsigned char val);
	//引数のDIBInfとORで合成する
	bool OR(DIBInf& inf);
	//引数のDIBInfと足し算で合成する
	bool Plus(DIBInf& inf);
	//グレイスケールDIBInfの値がしきい値未満ならカットする
	bool Cut(unsigned char th);
	//DIBInfの値がしきい値以上なら白にする
	bool White(int th);
	//DIBInfの値がしきい値以上の比率なら白にする(第2引数は背景画像)
	bool White2(DIBInf& BKGInf,int th,int fr=0);
	//グレイスケールDIBInfの差をセット
	bool Sub(DIBInf& inf);
	//グレイスケールDIBInfの論理積(AND)をセット
	bool And(DIBInf& inf);
	//ノイズの削除
	bool DeleteNoise(int bw,int bh,double border);
	//指定範囲の画像を切り出してDIBInfに格納
	bool CutOutRectImage(DIBInf& src,int left,int top,int right,int bottom);
	//画像を指定位置に貼り付けしてBitmapに格納
	bool PasteRectImage(DIBInf& src,int left,int top);
	//膨張処理
	bool Image_dilation();
	bool Image_dilation(int l,int t,int r,int b);
	//収縮処理
	bool Image_erosion();
	bool Image_erosion(int l,int t,int r,int b);
	//クロージング処理
	bool Closing(int cn);
	bool Closing(int cn,int l,int t,int r,int b);

	//矩形塗りつぶし
	bool FillRect(const Gdiplus::Rect& R,const PixRGB& col);

public:
	PixRGB* _RGB(int x,int y);
	unsigned char& R(int x,int y);
	unsigned char& G(int x,int y);
	unsigned char& B(int x,int y);
};



#endif
