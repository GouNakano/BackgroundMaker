//---------------------------------------------------------------------------

#pragma hdrstop
#include<math.h>
#ifndef STRICT
#define STRICT
#endif
#include "GDIplusUnit.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

extern "C" VOID WINAPI GdiplusShutdown(ULONG_PTR token);


//---------------------------------
//constructor
//---------------------------------
GDIPulsUnit::GDIPulsUnit()
{
	gdiToken = 0;
	Graphics = 0;
	hdc      = 0;
	Canvas   = 0;
	SPos.x   = 0;
	SPos.y   = 0;
	FStartArrow  = false;
	FCenterArrow = false;
	FEndArrow    = false;
}

//---------------------------------
//GDI+ start
//---------------------------------
bool GDIPulsUnit::Startup()
{
	Gdiplus::Status S = GdiplusStartup(&gdiToken,&gdiSI, NULL);

	return (S == Gdiplus::Ok);
}

//---------------------------------
//GDI+ end
//---------------------------------
bool GDIPulsUnit::Shutdown()
{
	GdiplusShutdown(gdiToken);

	return true;
}

//---------------------------------
//disolay start
//---------------------------------
bool GDIPulsUnit::Begin(TCanvas *pc,bool isAlpha,int alphaVal,bool isStrAlpha,int strAlphaVal)
{
	HDC hdc = pc->Handle;
	Canvas = pc;

	Graphics = new Gdiplus::Graphics(hdc);
	FAlphaBlend         = isAlpha;
	FAlphaBlendValue    = alphaVal;
	FStrAlphaBlend      = isStrAlpha;
	FStrAlphaBlendValue = strAlphaVal;

	return true;
}
//---------------------------------
//disolay end
//---------------------------------
bool GDIPulsUnit::End()
{
	//資源解放
	delete Graphics;
	hdc  = 0;

	return true;
}

//---------------------------------
//get Graphics object
//---------------------------------
Gdiplus::Graphics& GDIPulsUnit::getGraphics()
{
	return *Graphics;
}

//---------------------------------
//create pen
//---------------------------------
Gdiplus::Pen *GDIPulsUnit::GetPen()
{
	Gdiplus::Pen *p = nullptr;
	unsigned long RGB = Canvas->Pen->Color;

	int R =  RGB&0xFF;
	int G = (RGB>>8)&0xFF;
	int B = (RGB>>16)&0xFF;
	int w = Canvas->Pen->Width;

	if(FAlphaBlend == true)
	{
		p = new Gdiplus::Pen(Gdiplus::Color(255 - (FAlphaBlendValue % 256),R,G,B),w);
	}
	else
	{
		p = new Gdiplus::Pen(Gdiplus::Color(255,R,G,B),w);
	}
	//line style
	switch(Canvas->Pen->Style)
	{
		case psDash:
		{
			p->SetDashStyle(Gdiplus::DashStyleDash);
			break;
		}
		case psDot:
		{
			p->SetDashStyle(Gdiplus::DashStyleDot);
			break;
		}
		case psDashDot:
		{
			p->SetDashStyle(Gdiplus::DashStyleDashDot);
			break;
		}
		case psDashDotDot:
		{
			p->SetDashStyle(Gdiplus::DashStyleDashDotDot);
			break;
		}
		default:
		{
			p->SetDashStyle(Gdiplus::DashStyleSolid);
			break;
		}
	}

	return p;
}
//---------------------------------
//create brush
//---------------------------------
Gdiplus::SolidBrush *GDIPulsUnit::GetSolidBrush(bool UsePenColor)
{
	unsigned long RGB;
	Gdiplus::SolidBrush *p = nullptr;

	if(UsePenColor == false)
	{
		RGB = Canvas->Brush->Color;
	}
	else
	{
		RGB = Canvas->Pen->Color;
	}

	int R =  RGB&0xFF;
	int G = (RGB>>8)&0xFF;
	int B = (RGB>>16)&0xFF;

	if(UsePenColor == false)
	{
		if(FAlphaBlend == true)
		{
			p = new Gdiplus::SolidBrush(Gdiplus::Color(255 - (FAlphaBlendValue % 256),R,G,B));
		}
		else
		{
			p = new Gdiplus::SolidBrush(Gdiplus::Color(255,R,G,B));
		}
	}
	else
	{
		if(FStrAlphaBlend == true)
		{
			p = new Gdiplus::SolidBrush(Gdiplus::Color(255 - (FStrAlphaBlendValue % 256),R,G,B));
		}
		else
		{
			p = new Gdiplus::SolidBrush(Gdiplus::Color(255,R,G,B));
		}
	}

	return p;
}
//---------------------------------
//create font
//---------------------------------
Gdiplus::Font *GDIPulsUnit::GetFont()
{
	Gdiplus::FontFamily fontFamily(Canvas->Font->Name.c_str());

	INT style = 0;

	if(Canvas->Font->Style.Contains(fsBold) == true)
	{
		style |= Gdiplus::FontStyleBold;
	}
	if(Canvas->Font->Style.Contains(fsItalic) == true)
	{
		style |= Gdiplus::FontStyleItalic;
	}
	if(Canvas->Font->Style.Contains(fsUnderline) == true)
	{
		style |= Gdiplus::FontStyleUnderline;
	}
	if(Canvas->Font->Style.Contains(fsStrikeOut) == true)
	{
		style |= Gdiplus::FontStyleStrikeout;
	}
	if(style == 0)
	{
		style = Gdiplus::FontStyleRegular;
	}

	Gdiplus::Font *font = new Gdiplus::Font(&fontFamily,Canvas->Font->Size,style,Gdiplus::UnitPixel);

	return font;
}
//---------------------------------
//Enable antialiasing
//---------------------------------
bool GDIPulsUnit::AntiAlias()
{
	Graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	return true;
}

//---------------------------------
//start position
//---------------------------------
bool GDIPulsUnit::MoveTo(int x1,int y1)
{
	SPos.x = x1;
	SPos.y = y1;

	return true;
}

//---------------------------------
//arrow
//---------------------------------
void GDIPulsUnit::SetStartArrow(bool ar)
{
	FStartArrow = ar;
}

void GDIPulsUnit::SetCenterArrow(bool ar)
{
	FCenterArrow = ar;
}

void GDIPulsUnit::SetEndArrow(bool ar)
{
	FEndArrow = ar;
}


//---------------------------------
//draw line
//---------------------------------
bool GDIPulsUnit::LineTo(int x1,int y1,int x2,int y2)
{
	Gdiplus::AdjustableArrowCap *LGPStartArrow = nullptr;
	Gdiplus::AdjustableArrowCap *LGPEndArrow = nullptr;
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();

	if(FCenterArrow == false)
	{
		if(FStartArrow == true)
		{
			LGPStartArrow = new Gdiplus::AdjustableArrowCap(8, 8, false);
			pPen->SetCustomStartCap(LGPStartArrow);
		}
		if(FEndArrow == true)
		{
			LGPEndArrow = new Gdiplus::AdjustableArrowCap(8, 8, false);
			pPen->SetCustomEndCap(LGPEndArrow);
		}
		Graphics->DrawLine(pPen,x1,y1,x2,y2);
	}
	else
	{
		int cx1 = (x1 + x2)/2;
		int cy1 = (y1 + y2)/2;

		if(FStartArrow == true)
		{
			LGPStartArrow = new Gdiplus::AdjustableArrowCap(8, 8, false);
			pPen->SetCustomStartCap(LGPStartArrow);
		}
		LGPEndArrow = new Gdiplus::AdjustableArrowCap(8, 8, false);
		pPen->SetCustomEndCap(LGPEndArrow);
		Graphics->DrawLine(pPen,x1,y1,cx1,cy1);

		if(LGPStartArrow != nullptr)
		{
			delete LGPStartArrow;
			LGPStartArrow = new Gdiplus::AdjustableArrowCap(0, 0, false);
			pPen->SetCustomStartCap(LGPStartArrow);
		}

		if(FEndArrow == false)
		{
			delete LGPEndArrow;
			LGPEndArrow = new Gdiplus::AdjustableArrowCap(0, 0, false);
			pPen->SetCustomEndCap(LGPEndArrow);
		}
		Graphics->DrawLine(pPen,cx1,cy1,x2,y2);
	}

	if(LGPStartArrow != nullptr)
	{
		delete LGPStartArrow;
	}
	if(LGPEndArrow != nullptr)
	{
		delete LGPEndArrow;
	}
	delete pPen;

	SPos.x   = x2;
	SPos.y   = y2;

	return true;
}

//---------------------------------
//Draw a line (set only the end point)
//---------------------------------
bool GDIPulsUnit::LineTo(int x2,int y2)
{
	//始点設定
	int x1 = SPos.x;
	int y1 = SPos.y;
	//線を描く
	LineTo(x1,y1,x2,y2);

	//次回の開始位置
	SPos.x   = x2;
	SPos.y   = y2;

	return true;
}

//---------------------------------
//Draw an arc (3 points setting)
//---------------------------------
bool GDIPulsUnit::Arc(int x1,int y1,int x2,int y2,int x3,int y3,bool& cw,double& sangle,double& eangle)
{
	double cx,cy,r;
	Gdiplus::Rect rect;
	double dr;

	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();

	calcCircleOf2Point(x1,y1,x2,y2,x3,y3,&cx,&cy,&r);

	double OAx = x1 - cx;
	double OAy = y1 - cy;
	double ABx = x2 - x1;
	double ABy = y2 - y1;
	double crs = OAx * ABy - OAy * ABx;
	int    rot = (crs >= 0?1:-1);

	double ag1 = GetAngle(cx,cy,x1,y1);
	double ag2 = GetAngle(cx,cy,x3,y3);

	rect.X      = cx - r;
	rect.Y      = cy - r;
	rect.Width  = 2 * r;
	rect.Height = 2 * r;

	if(rot < 0)
	{
		std::swap(ag1,ag2);
	}

	dr = ag2 - ag1;

	if(dr < 0.0)
	{
		dr = 360.0 - (ag1 - ag2);
	}

	Graphics->DrawArc(pPen,rect,ag1,dr);

	cw     = true;
	sangle = ag1;
	eangle = ag2;

	delete pPen;

	return true;
}
//---------------------------------
//Draw an arc 2
//---------------------------------
bool GDIPulsUnit::Arc2(int w,int h,bool cw,double sangle,double eangle)
{
	double dr;
	Gdiplus::Rect rect;

	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();

	rect.X      = pPen->GetWidth();
	rect.Y      = pPen->GetWidth();
	rect.Width  = w - pPen->GetWidth()*2;
	rect.Height = h - pPen->GetWidth()*2;

	dr = eangle - sangle;

	if(dr < 0.0)
	{
		dr = 360.0 - (sangle - eangle);
	}

	Graphics->DrawArc(pPen,rect,sangle,dr);

	delete pPen;

	return true;
}
//---------------------------------
//Draw an arc 3
//---------------------------------
bool GDIPulsUnit::PhotonArc(int x1,int y1,int x2,int y2,int x3,int y3)
{
	double cx,cy,r;
	double cosr1;
	double cosr2;
	double dx1;
	double dy1;
	double dx2;
	double dy2;
	double dx3;
	double dy3;
	double rdr;
	double ag1;
	double ag2;
	double rag1;
	double rag2;
	double ArcLen;
	double tmp;
	int IArcLen;
	int Num;
	bool IsFirst = true;

	const double Pai  = 3.14159265359;
	const double Pai2 = Pai * 2.0;

	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();

	calcCircleOf2Point(x1,y1,x2,y2,x3,y3,&cx,&cy,&r);

	ag1 = GetAngle(cx,cy,x1,y1);
	ag2 = GetAngle(cx,cy,x3,y3);

	double OAx = x1 - cx;
	double OAy = y1 - cy;
	double ABx = x2 - x1;
	double ABy = y2 - y1;
	double crs = OAx * ABy - OAy * ABx;
	int    rot = (crs >= 0?1.0:-1.0);

	rag1 = (ag1 * 2.0 * Pai)/360.0;
	rag2 = (ag2 * 2.0 * Pai)/360.0;

	dx1  = x1 - cx;
	dy1  = y1 - cy;
	dx2  = x2 - cx;
	dy2  = y2 - cy;
	dx3  = x3 - cx;
	dy3  = y3 - cy;

	cosr1 = (dx1*dx2 + dy1*dy2)/(sqrt(dx1*dx1 + dy1*dy1)*sqrt(dx2*dx2 + dy2*dy2));
	cosr2 = (dx2*dx3 + dy2*dy3)/(sqrt(dx2*dx2 + dy2*dy2)*sqrt(dx3*dx3 + dy3*dy3));

	rdr = acos(cosr1) + acos(cosr2);
	ArcLen = abs(r * rdr);
	IArcLen = static_cast<int>(ArcLen);
	Num     = IArcLen / 20;
	Num     = (Num < 1?1:Num);

	ArcLen = static_cast<double>(Num) * 20.0;

	rdr = ArcLen / r;
	rdr = (rdr < 0.0?-rdr:rdr);

	for(int Cnt = 0;Cnt < Num;Cnt++)
	{
		double sarg = rag1 + (rot * rdr * static_cast<double>(Cnt))/static_cast<double>(Num);

		for(int div = 0;div < 21;div++)
		{
			double narg = sarg + (rot * rdr * static_cast<double>(div))/(20.0 * static_cast<double>(Num));

			double sx = cx + (r * cos(narg));
			double sy = cy + (r * sin(narg));

			double SinRad = (static_cast<double>(div) * Pai2) / 20.0;
			sx = sx + 10.0 * cos(narg) * sin(SinRad);
			sy = sy + 10.0 * sin(narg) * sin(SinRad);

			if(IsFirst == true)
			{
				MoveTo(sx,sy);
				IsFirst = false;
			}
			else
			{
				LineTo(sx,sy);
			}
		}
	}

	delete pPen;

	return true;
}

//---------------------------------
//Draw an arc 3
//---------------------------------
bool GDIPulsUnit::GluonArc(int x1,int y1,int x2,int y2,int x3,int y3)
{
	double cx,cy,r;
	double cosr1;
	double cosr2;
	double rdr;
	double dx1;
	double dy1;
	double dx2;
	double dy2;
	double dx3;
	double dy3;
	double ag1;
	double ag2;
	double rag1;
	double rag2;
	double ArcLen;
	double tmp;
	int IArcLen;
	int Num;
	bool IsFirst = true;

	const double Pai  = 3.14159265359;
	const double Pai2 = Pai * 2.0;

	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();

	calcCircleOf2Point(x1,y1,x2,y2,x3,y3,&cx,&cy,&r);


	ag1 = GetAngle(cx,cy,x1,y1);
	ag2 = GetAngle(cx,cy,x3,y3);

	int OAx = x1 - cx;
	int OAy = y1 - cy;
	int ABx = x2 - x1;
	int ABy = y2 - y1;
	int crs = OAx * ABy - OAy * ABx;
	double rot = (crs >= 0?1.0:-1.0);

	rag1 = (ag1 * 2.0 * Pai)/360.0;
	rag2 = (ag2 * 2.0 * Pai)/360.0;

	dx1  = x1 - cx;
	dy1  = y1 - cy;
	dx2  = x2 - cx;
	dy2  = y2 - cy;
	dx3  = x3 - cx;
	dy3  = y3 - cy;

	cosr1 = (dx1*dx2 + dy1*dy2)/(sqrt(dx1*dx1 + dy1*dy1)*sqrt(dx2*dx2 + dy2*dy2));
	cosr2 = (dx2*dx3 + dy2*dy3)/(sqrt(dx2*dx2 + dy2*dy2)*sqrt(dx3*dx3 + dy3*dy3));

	rdr = acos(cosr1) + acos(cosr2);

	ArcLen = abs(r * rdr);

	IArcLen = static_cast<int>(ArcLen);
	Num     = IArcLen / 10;
	Num     = (Num < 1?1:Num);

	ArcLen = static_cast<double>(Num) * 10.0;

	rdr = ArcLen / r;
	rdr = (rdr < 0.0?-rdr:rdr);

	double mv = 6.0;

	double mrad = mv / r;

	for(int Cnt = 0;Cnt < Num;Cnt++)
	{
		double sarg = rag1 + (rot * rdr * static_cast<double>(Cnt))/static_cast<double>(Num);

		int divmax = (Cnt < Num - 1?11:14);

		for(int div = 0;div < divmax;div++)
		{

			double narg = sarg + (rot * rdr * static_cast<double>(div))/(10.0 * static_cast<double>(Num));

			double sx = cx + (r * cos(narg+mrad));
			double sy = cy + (r * sin(narg+mrad));

			double Vx = cos(narg+mrad);
			double Vy = sin(narg+mrad);

			double sr = (static_cast<double>(div) * Pai2) / 10.0;
			double Rx = 6.0 * ( Vx * sin(sr) + Vy * cos(sr));
			double Ry = 6.0 * (-Vx * cos(sr) + Vy * sin(sr));
			sx = sx + Rx;
			sy = sy + Ry;

			if(IsFirst == true)
			{
				MoveTo(sx,sy);
				IsFirst = false;
			}
			else
			{

				LineTo(sx,sy);
			}
		}
	}

	delete pPen;

	return true;
}

//---------------------------------
//Bezier curve drawing (4 points setting)
//---------------------------------
bool GDIPulsUnit::GluonBezier(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	double cx,cy,r;
	double cosr1;
	double cosr2;
	double rdr;
	double dx1;
	double dy1;
	double dx2;
	double dy2;
	double dx3;
	double dy3;
	double ag1;
	double ag2;
	double rag1;
	double rag2;
	double ArcLen;
	double tmp;
	int IArcLen;
	int Num;
	bool IsFirst = true;

	const double Pai  = 3.14159265359;
	const double Pai2 = Pai * 2.0;

	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();

	calcCircleOf2Point(x1,y1,x2,y2,x3,y3,&cx,&cy,&r);

	ag1 = GetAngle(cx,cy,x1,y1);
	ag2 = GetAngle(cx,cy,x3,y3);

	int OAx = x1 - cx;
	int OAy = y1 - cy;
	int ABx = x2 - x1;
	int ABy = y2 - y1;
	int crs = OAx * ABy - OAy * ABx;
	double rot = (crs >= 0?1.0:-1.0);

	rag1 = (ag1 * 2.0 * Pai)/360.0;
	rag2 = (ag2 * 2.0 * Pai)/360.0;

	dx1  = x1 - cx;
	dy1  = y1 - cy;
	dx2  = x2 - cx;
	dy2  = y2 - cy;
	dx3  = x3 - cx;
	dy3  = y3 - cy;

	cosr1 = (dx1*dx2 + dy1*dy2)/(sqrt(dx1*dx1 + dy1*dy1)*sqrt(dx2*dx2 + dy2*dy2));
	cosr2 = (dx2*dx3 + dy2*dy3)/(sqrt(dx2*dx2 + dy2*dy2)*sqrt(dx3*dx3 + dy3*dy3));

	rdr = acos(cosr1) + acos(cosr2);

	ArcLen = abs(r * rdr);

	IArcLen = static_cast<int>(ArcLen);
	Num     = IArcLen / 10;
	Num     = (Num < 1?1:Num);

	ArcLen = static_cast<double>(Num) * 10.0;

	rdr = ArcLen / r;
	rdr = (rdr < 0.0?-rdr:rdr);

	double mv = 6.0;
	double mrad = mv / r;

	for(int Cnt = 0;Cnt < Num;Cnt++)
	{
		double sarg = rag1 + (rot * rdr * static_cast<double>(Cnt))/static_cast<double>(Num);
		int divmax = (Cnt < Num - 1?11:14);

		for(int div = 0;div < divmax;div++)
		{
			double narg = sarg + (rot * rdr * static_cast<double>(div))/(10.0 * static_cast<double>(Num));

			double sx = cx + (r * cos(narg+mrad));
			double sy = cy + (r * sin(narg+mrad));

			double Vx = cos(narg+mrad);
			double Vy = sin(narg+mrad);

			double sr = (static_cast<double>(div) * Pai2) / 10.0;
			double Rx = 6.0 * ( Vx * sin(sr) + Vy * cos(sr));
			double Ry = 6.0 * (-Vx * cos(sr) + Vy * sin(sr));
			sx = sx + Rx;
			sy = sy + Ry;

			if(IsFirst == true)
			{
				MoveTo(sx,sy);
				IsFirst = false;
			}
			else
			{

				LineTo(sx,sy);
			}
		}
	}
	delete pPen;

	return true;
}

//---------------------------------
//Bezier curve drawing 2(4 points setting)
//---------------------------------
bool GDIPulsUnit::Bezier(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	int cx,cy,r;
	Gdiplus::Rect rect;
	double dr;

	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();

	Graphics->DrawBezier(pPen,x1,y1,x2,y2,x3,y3,x4,y4);

	delete pPen;

	return true;
}


//---------------------------------
//Draw circle
//---------------------------------
bool GDIPulsUnit::Circle(int cx,int cy,int r)
{

	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();

	Gdiplus::Rect rect;

	rect.X      = cx - r;
	rect.Y      = cy - r;
	rect.Width  = 2 * r;
	rect.Height = 2 * r;


	Graphics->DrawEllipse(pPen,rect);

	delete pPen;

	return true;
}

//---------------------------------
//Draw circle 2
//---------------------------------
bool GDIPulsUnit::Ellipse(int x1,int y1,int x2,int y2)
{
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	Gdiplus::Rect rect;

	rect.X      = x1;
	rect.Y      = y1;
	rect.Width  = x2 - x1 + 1;
	rect.Height = y2 - y1 + 1;

	Graphics->DrawEllipse(pPen,rect);

	delete pPen;

	return true;
}

//---------------------------------
//Draw rectangle
//---------------------------------
bool GDIPulsUnit::Box(int x1,int y1,int x2,int y2)
{
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();

	Gdiplus::Rect rect;

	rect.X      = x1;
	rect.Y      = y1;
	rect.Width  = x2 - x1 - 1;
	rect.Height = y2 - y1 - 1;

	Graphics->DrawRectangle(pPen,rect);

	//ペン削除
	delete pPen;

	return true;
}
//---------------------------------
//Draw rectangle 2
//---------------------------------
bool GDIPulsUnit::FillBoxAndRectAngle(int x1,int y1,int x2,int y2)
{
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	Gdiplus::Brush *pBrush = GetSolidBrush();
	Gdiplus::Rect rect;

	rect.X      = x1;
	rect.Y      = y1;
	rect.Width  = x2 - x1 - 1;
	rect.Height = y2 - y1 - 1;

	Graphics->FillRectangle(pBrush, rect);

	Graphics->DrawRectangle(pPen,rect);

	delete pPen;

	delete pBrush;

	return true;
}
//---------------------------------
//Draw rectangle 3
//---------------------------------
bool GDIPulsUnit::FillBox(int x1,int y1,int x2,int y2)
{
	Gdiplus::Brush *pBrush = GetSolidBrush();
	Gdiplus::Rect rect;

	rect.X      = x1;
	rect.Y      = y1;
	rect.Width  = x2 - x1 - 1;
	rect.Height = y2 - y1 - 1;

	Graphics->FillRectangle(pBrush, rect);

	delete pBrush;

	return true;
}
//---------------------------------
//Draw text
//---------------------------------
bool GDIPulsUnit::TextOut(String txt,int x,int y)
{
	Gdiplus::FontFamily fontFamily(Canvas->Font->Name.c_str());
	Gdiplus::Font       font(&fontFamily,Canvas->Font->Size,Gdiplus::FontStyleRegular,Gdiplus::UnitPixel);
	Gdiplus::PointF     pointF(x,y);

	Gdiplus::Brush *pBrush = GetSolidBrush(true);

	Graphics->DrawString(txt.c_str(), -1, &font, pointF,pBrush);

	delete pBrush;

	return true;
}

//-----------------------------------------------------------------------------------
//Draw circle from 3 points
//-----------------------------------------------------------------------------------
double GDIPulsUnit::calcCircleOf2Point(double x1, double y1, double x2, double y2, double x3, double y3, double *cx, double *cy, double *r)
{
	double ox, oy, a, b, c, d ;
	double r1, r2, r3 ;
	double stat ;

	a = x2 - x1 ;
	b = y2 - y1 ;
	c = x3 - x1 ;
	d = y3 - y1 ;

	stat = -1 ;

	if  ((a && d) || (b && c)) {
		ox = x1 + (d * (a * a + b * b) - b * (c * c + d * d)) / (a * d - b * c) / 2 ;
		if  (b) {
			oy = (a * (x1 + x2 - ox - ox) + b * (y1 + y2)) / b / 2 ;
		} else {
			oy = (c * (x1 + x3 - ox - ox) + d * (y1 + y3)) / d / 2 ;
		}
		r1   = sqrt((ox - x1) * (ox - x1) + (oy - y1) * (oy - y1)) ;
		r2   = sqrt((ox - x2) * (ox - x2) + (oy - y2) * (oy - y2)) ;
		r3   = sqrt((ox - x3) * (ox - x3) + (oy - y3) * (oy - y3)) ;
		*cx = ox ;
		*cy = oy ;
		*r  = (r1 + r2 + r3) / 3 ;
		stat = 0 ;
	}

	return stat ;
}

//-----------------------------------------------------------------------------------
//Get angle
//-----------------------------------------------------------------------------------
double GDIPulsUnit::GetAngle(double  cx, double  cy, double  x, double  y)
{
	double ang;

	const double Pai = 3.14159265359;

	double Vx = x - cx;
	double Vy = y - cy;

	if(Vy >= 0)
	{
		double ip = Vx;

		double cs = ip / sqrt(Vx*Vx + Vy*Vy);

		ang = (acos(cs) *180) / Pai;
	}
	else
	{
		double ip = Vx;

		double cs = ip / sqrt(Vx*Vx + Vy*Vy);

		ang = 360.0 - (acos(cs) *180) / Pai;
	}
	return ang;
}
//-----------------------------------------------------------------------------------
//Get the width and height of the string
//-----------------------------------------------------------------------------------
bool GDIPulsUnit::GetTextSize(String txt,int& width,int& height)
{
	Gdiplus::StringFormat sf;
	Gdiplus::RectF        boundRect;
	Gdiplus::RectF        layoutRect(0.0f, 0.0f, 1024.0f,1024.0f);

	Gdiplus::Font *font = GDIPulsUnit::GetFont();

	Graphics->MeasureString(txt.c_str(),txt.Length(),font,layoutRect,&sf,&boundRect);

	delete font;

	width  = boundRect.Width;
	height = boundRect.Height;

	return true;
}

