//---------------------------------------------------------------------------

#ifndef GDIplusUnitH
#define GDIplusUnitH
#include <vcl.h>
#include <windows.h>
#include <algorithm>
using std::min;
using std::max;
#include <gdiplus.h>

//---------------------------------------------------------------------------

class GDIPulsUnit
{
private:
	Gdiplus::GdiplusStartupInput  gdiSI;
	ULONG_PTR                     gdiToken;
	Gdiplus::Graphics            *Graphics;
	PAINTSTRUCT                   ps;
	HDC                           hdc;
	TCanvas                      *Canvas;
	TPoint                        SPos;
private:
	bool          FAlphaBlend;
	int           FAlphaBlendValue;
	bool          FStrAlphaBlend;
	int           FStrAlphaBlendValue;
	bool          FStartArrow;
	bool          FCenterArrow;
	bool          FEndArrow;
private:
	Gdiplus::Pen        *GetPen();
	Gdiplus::SolidBrush *GetSolidBrush(bool UsePenColor = false);
	Gdiplus::Font       *GetFont();
public:
	//コンストラクタ
	GDIPulsUnit();
private:
	void SetStartArrow(bool ar);
	void SetCenterArrow(bool ar);
	void SetEndArrow(bool ar);
public:
	//３点から円を作成する
	static double calcCircleOf2Point(double x1, double y1, double x2, double y2, double x3, double y3, double *cx, double *cy, double *r);
	//角度を得る(0-360)
	static double GetAngle(double cx, double cy, double x, double y);
public:
	//GDI+開始
	bool Startup();
	//GDI+終了
	bool Shutdown();
	//描画開始
	bool Begin(TCanvas *pc,bool isAlpha,int alphaVal,bool isStrAlpha = false,int strAlphaVal = 0);
//	bool Begin(TCanvas *pc,bool isAlpha,int alphaVal);
//	bool Begin(TCanvas *pc,bool isAlpha=false,int alphaVal=0);
	//描画終了
	bool End();
	//アンチエイリアスを有効にする
	bool AntiAlias();
	//Graphicsオブジェクト取得
	Gdiplus::Graphics& getGraphics();
	//開始位置の設定
	bool MoveTo(int x1,int y1);
	//線を描く
	bool LineTo(int x1,int y1,int x2,int y2);
	//線を描く(終点だけ設定)
	bool LineTo(int x2,int y2);
	//円弧を描く(３点設定)
	bool Arc(int x1,int y1,int x2,int y2,int x3,int y3,bool& cw,double& sangle,double& eangle);
	//円弧を描く2
	bool Arc2(int w,int h,bool cw,double sangle,double eangle);
	//光子円弧を描く(３点設定)
	bool PhotonArc(int x1,int y1,int x2,int y2,int x3,int y3);
	//グルーオン円弧を描く(３点設定)
	bool GluonArc(int x1,int y1,int x2,int y2,int x3,int y3);
	//グルーオンベジェ曲線描画(４点設定)
	bool GluonBezier(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4);
	//円を描く
	bool Circle(int cx,int cy,int r);
	//円を描く(四角形指定)
	bool Ellipse(int x1,int y1,int x2,int y2);
	//ベジェ曲線描画(４点設定)
	bool Bezier(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4);
	//四角を描く(四角形指定)
	bool Box(int x1,int y1,int x2,int y2);
	//塗りつぶした四角を描く(四角形指定)
	bool FillBox(int x1,int y1,int x2,int y2);
	//枠線付きの塗りつぶした四角を描く(四角形指定)
	bool FillBoxAndRectAngle(int x1,int y1,int x2,int y2);
	//文字列の幅と高さを得る
	bool GetTextSize(String txt,int& width,int& height);
	//テキストの描画
	bool TextOut(String txt,int x,int y);

public:
	__property bool   AlphaBlend         = {read = FAlphaBlend        ,write = FAlphaBlend};
	__property int    AlphaBlendValue    = {read = FAlphaBlendValue   ,write = FAlphaBlendValue};
	__property bool   StrAlphaBlend      = {read = FStrAlphaBlend     ,write = FStrAlphaBlend};
	__property int    StrAlphaBlendValue = {read = FStrAlphaBlendValue,write = FStrAlphaBlendValue};
	__property bool   StartArrow         = {read = FStartArrow        ,write = SetStartArrow};
	__property bool   CenterArrow        = {read = FCenterArrow       ,write = SetCenterArrow};
	__property bool   EndArrow           = {read = FEndArrow          ,write = SetEndArrow};

};


#endif
