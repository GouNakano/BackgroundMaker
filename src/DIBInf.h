//---------------------------------------------------------------------------
#ifndef DIBInfH
#define DIBInfH
//---------------------------------------------------------------------------
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "GDIplusUnit.h"

enum typBackSub {tbsNone=0,tbsHalf=128,tbsFull=255};

struct PixRGB
{
	unsigned char B;
	unsigned char G;
	unsigned char R;
};

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
	void _copy(const DIBInf& h);
	void _free();
public:
	DIBInf();
	DIBInf(TBitmap *pb);
	DIBInf(const String& file);
	DIBInf(const DIBInf& h);

	virtual ~DIBInf();
public:
	DIBInf& operator = (const DIBInf& h);
protected:
	bool ChechPos(int x,int y);
public:
	bool Init();
	TBitmap *GetBitmap();
	bool ConvToDIB(int width,int height,TPixelFormat pf = pf24bit);
	bool ConvToDIB(TPixelFormat pf = pf24bit);

	bool ConvDibToBitmap();
	bool ConvToBitmap();

	bool FillZero();

	bool SetTransparentColor(TColor tc);
public:
	TCanvas *GetCanvas();

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
	bool LoadFromFile(String path,bool IsMakeDIB = false);
	int SrchMinY(unsigned char val);
	int ScanLine(int y,unsigned char val);
	bool OR(DIBInf& inf);
	bool Plus(DIBInf& inf);
	bool Cut(unsigned char th);
	bool White(int th);
	bool White2(DIBInf& BKGInf,int th,int fr=0);
	bool Sub(DIBInf& inf);
	bool And(DIBInf& inf);
	bool DeleteNoise(int bw,int bh,double border);
	bool CutOutRectImage(DIBInf& src,int left,int top,int right,int bottom);
	bool PasteRectImage(DIBInf& src,int left,int top);
	bool Image_dilation();
	bool Image_dilation(int l,int t,int r,int b);
	bool Image_erosion();
	bool Image_erosion(int l,int t,int r,int b);
	bool Closing(int cn);
	bool Closing(int cn,int l,int t,int r,int b);
	bool FillRect(const Gdiplus::Rect& R,const PixRGB& col);

public:
	PixRGB* _RGB(int x,int y);
	unsigned char& R(int x,int y);
	unsigned char& G(int x,int y);
	unsigned char& B(int x,int y);
};

#endif
