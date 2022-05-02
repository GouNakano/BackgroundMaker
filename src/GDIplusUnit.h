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
	GDIPulsUnit();
private:
	void SetStartArrow(bool ar);
	void SetCenterArrow(bool ar);
	void SetEndArrow(bool ar);
public:
	//Draw circle from 3 points
	static double calcCircleOf2Point(double x1, double y1, double x2, double y2, double x3, double y3, double *cx, double *cy, double *r);
	//Get Angle(0-360)
	static double GetAngle(double cx, double cy, double x, double y);
public:
	//GDI+ start
	bool Startup();
	//GDI+ end
	bool Shutdown();
	//disolay start
	bool Begin(TCanvas *pc,bool isAlpha,int alphaVal,bool isStrAlpha = false,int strAlphaVal = 0);
	//disolay end
	bool End();
	//Enable antialiasing
	bool AntiAlias();
	//Get Graphics object
	Gdiplus::Graphics& getGraphics();
	//start position
	bool MoveTo(int x1,int y1);
	//Draw line
	bool LineTo(int x1,int y1,int x2,int y2);
	//Draw line 2
	bool LineTo(int x2,int y2);
	//Draw circle
	bool Arc(int x1,int y1,int x2,int y2,int x3,int y3,bool& cw,double& sangle,double& eangle);
	//Draw circle 2
	bool Arc2(int w,int h,bool cw,double sangle,double eangle);
	//Draw circle 3
	bool PhotonArc(int x1,int y1,int x2,int y2,int x3,int y3);
	//Draw circle 4
	bool GluonArc(int x1,int y1,int x2,int y2,int x3,int y3);
	//Bezier curve drawing (4 points setting)
	bool GluonBezier(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4);
	//Draw circle
	bool Circle(int cx,int cy,int r);
	//Draw circle 2
	bool Ellipse(int x1,int y1,int x2,int y2);
	//Bezier curve drawing 2(4 points setting)
	bool Bezier(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4);
	//Draw rectangle
	bool Box(int x1,int y1,int x2,int y2);
	//Draw rectangle 2
	bool FillBox(int x1,int y1,int x2,int y2);
	//Draw rectangle 3
	bool FillBoxAndRectAngle(int x1,int y1,int x2,int y2);
	//Draw rectangle 4
	bool GetTextSize(String txt,int& width,int& height);
	//Draw text
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
