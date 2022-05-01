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
//コンストラクタ
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
//GDI+開始
//---------------------------------
bool GDIPulsUnit::Startup()
{
	Gdiplus::Status S = GdiplusStartup(&gdiToken,&gdiSI, NULL);

	return (S == Gdiplus::Ok);
}

//---------------------------------
//GDI+終了
//---------------------------------
bool GDIPulsUnit::Shutdown()
{
	GdiplusShutdown(gdiToken);

	return true;
}

//---------------------------------
//描画開始
//---------------------------------
bool GDIPulsUnit::Begin(TCanvas *pc,bool isAlpha,int alphaVal,bool isStrAlpha,int strAlphaVal)
{
	//HDC
	HDC hdc = pc->Handle;
	//Canvas保存
	Canvas = pc;

	//Graphicsオブジェクト作成
	Graphics = new Gdiplus::Graphics(hdc);
	//透明度設定
	FAlphaBlend         = isAlpha;
	FAlphaBlendValue    = alphaVal;
	FStrAlphaBlend      = isStrAlpha;
	FStrAlphaBlendValue = strAlphaVal;

	return true;
}
//---------------------------------
//描画終了
//---------------------------------
bool GDIPulsUnit::End()
{
	//資源解放
	delete Graphics;
	hdc  = 0;

	return true;
}

//---------------------------------
//Graphicsオブジェクト取得
//---------------------------------
Gdiplus::Graphics& GDIPulsUnit::getGraphics()
{
	return *Graphics;
}

//---------------------------------
//Penの作成
//---------------------------------
Gdiplus::Pen *GDIPulsUnit::GetPen()
{
	Gdiplus::Pen *p = nullptr;
	unsigned long RGB = Canvas->Pen->Color;

	int R =  RGB&0xFF;
	int G = (RGB>>8)&0xFF;
	int B = (RGB>>16)&0xFF;
	int w = Canvas->Pen->Width;

	//アルファ値
	if(FAlphaBlend == true)
	{
		//ペン作成
		p = new Gdiplus::Pen(Gdiplus::Color(255 - (FAlphaBlendValue % 256),R,G,B),w);
	}
	else
	{
		//ペン作成
		p = new Gdiplus::Pen(Gdiplus::Color(255,R,G,B),w);
	}
	//線種設定
	switch(Canvas->Pen->Style)
	{
		case psDash: //破線
		{
			p->SetDashStyle(Gdiplus::DashStyleDash);
			break;
		}
		case psDot: //点線
		{
			p->SetDashStyle(Gdiplus::DashStyleDot);
			break;
		}
		case psDashDot: //一点鎖線
		{
			p->SetDashStyle(Gdiplus::DashStyleDashDot);
			break;
		}
		case psDashDotDot: //二点鎖線
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
//ブラシ(単色)の作成
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
	//アルファ値
	if(UsePenColor == false)
	{
		if(FAlphaBlend == true)
		{
			//ブラシ作成
			p = new Gdiplus::SolidBrush(Gdiplus::Color(255 - (FAlphaBlendValue % 256),R,G,B));
		}
		else
		{
			//ブラシ作成
			p = new Gdiplus::SolidBrush(Gdiplus::Color(255,R,G,B));
		}
	}
	else
	{
		if(FStrAlphaBlend == true)
		{
			//ブラシ作成
			p = new Gdiplus::SolidBrush(Gdiplus::Color(255 - (FStrAlphaBlendValue % 256),R,G,B));
		}
		else
		{
			//ブラシ作成
			p = new Gdiplus::SolidBrush(Gdiplus::Color(255,R,G,B));
		}
	}

	return p;
}
//---------------------------------
//フォントの作成
//---------------------------------
Gdiplus::Font *GDIPulsUnit::GetFont()
{
	Gdiplus::FontFamily fontFamily(Canvas->Font->Name.c_str());
	//フォントファミリースタイル
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
//アンチエイリアスを有効にする
//---------------------------------
bool GDIPulsUnit::AntiAlias()
{
	Graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	return true;
}

//---------------------------------
//開始位置の設定
//---------------------------------
bool GDIPulsUnit::MoveTo(int x1,int y1)
{
	SPos.x = x1;
	SPos.y = y1;

	return true;
}

//---------------------------------
//矢印
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
//線を描く
//---------------------------------
bool GDIPulsUnit::LineTo(int x1,int y1,int x2,int y2)
{
	Gdiplus::AdjustableArrowCap *LGPStartArrow = nullptr;
	Gdiplus::AdjustableArrowCap *LGPEndArrow = nullptr;
	//ペン取得
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//矢印

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
		//線を描く
		Graphics->DrawLine(pPen,x1,y1,x2,y2);
	}
	else
	{
		//中間座標
		int cx1 = (x1 + x2)/2;
		int cy1 = (y1 + y2)/2;
		//線を描く1
		if(FStartArrow == true)
		{
			LGPStartArrow = new Gdiplus::AdjustableArrowCap(8, 8, false);
			pPen->SetCustomStartCap(LGPStartArrow);
		}
		LGPEndArrow = new Gdiplus::AdjustableArrowCap(8, 8, false);
		pPen->SetCustomEndCap(LGPEndArrow);
		Graphics->DrawLine(pPen,x1,y1,cx1,cy1);
		//線を描く2
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

	//矢印削除
	if(LGPStartArrow != nullptr)
	{
		delete LGPStartArrow;
	}
	if(LGPEndArrow != nullptr)
	{
		delete LGPEndArrow;
	}
	//ペン削除
	delete pPen;
	//次回の開始位置
	SPos.x   = x2;
	SPos.y   = y2;

	return true;
}

//---------------------------------
//線を描く(終点だけ設定)
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
//円弧を描く(３点設定)
//---------------------------------
bool GDIPulsUnit::Arc(int x1,int y1,int x2,int y2,int x3,int y3,bool& cw,double& sangle,double& eangle)
{
	double cx,cy,r;
	Gdiplus::Rect rect;
	double dr;
	//ペン取得
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//３点から円を作成する
	calcCircleOf2Point(x1,y1,x2,y2,x3,y3,&cx,&cy,&r);
	//時計周り、反時計回り判別
	double OAx = x1 - cx;
	double OAy = y1 - cy;
	double ABx = x2 - x1;
	double ABy = y2 - y1;
	double crs = OAx * ABy - OAy * ABx;
	int    rot = (crs >= 0?1:-1);
	//角度を得る(x 軸から円弧の開始点まで、時計回りに測定した角度 (度単位))
	double ag1 = GetAngle(cx,cy,x1,y1);
	double ag2 = GetAngle(cx,cy,x3,y3);
	//四角形座標作成
	rect.X      = cx - r;
	rect.Y      = cy - r;
	rect.Width  = 2 * r;
	rect.Height = 2 * r;
	//掃引き角度
	if(rot < 0)
	{
		std::swap(ag1,ag2);
	}

	dr = ag2 - ag1;

	if(dr < 0.0)
	{
		dr = 360.0 - (ag1 - ag2);
	}

	//円弧を描く
	Graphics->DrawArc(pPen,rect,ag1,dr);

	//角度情報をセット
	cw     = true;
	sangle = ag1;
	eangle = ag2;

	//ペン削除
	delete pPen;

	return true;
}
//---------------------------------
//円弧を描く2
//---------------------------------
bool GDIPulsUnit::Arc2(int w,int h,bool cw,double sangle,double eangle)
{
	double dr;
	Gdiplus::Rect rect;
	//ペン取得
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//四角形座標作成
	rect.X      = pPen->GetWidth();
	rect.Y      = pPen->GetWidth();
	rect.Width  = w - pPen->GetWidth()*2;
	rect.Height = h - pPen->GetWidth()*2;
	//掃引き角度
	dr = eangle - sangle;

	if(dr < 0.0)
	{
		dr = 360.0 - (sangle - eangle);
	}
	//円弧を描く
	Graphics->DrawArc(pPen,rect,sangle,dr);
	//ペン削除
	delete pPen;

	return true;
}
//---------------------------------
//光子円弧を描く(３点設定)
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
	//描画
	const double Pai  = 3.14159265359;
	const double Pai2 = Pai * 2.0;
	//ペン取得
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//３点から円を作成する
	calcCircleOf2Point(x1,y1,x2,y2,x3,y3,&cx,&cy,&r);

	//角度を得る
	ag1 = GetAngle(cx,cy,x1,y1);
	ag2 = GetAngle(cx,cy,x3,y3);
	//時計周り、反時計回り判別
	double OAx = x1 - cx;
	double OAy = y1 - cy;
	double ABx = x2 - x1;
	double ABy = y2 - y1;
	double crs = OAx * ABy - OAy * ABx;
	int    rot = (crs >= 0?1.0:-1.0);

	//角度を得る(ラジアン)
	rag1 = (ag1 * 2.0 * Pai)/360.0;
	rag2 = (ag2 * 2.0 * Pai)/360.0;

	//掃引き角度
	dx1  = x1 - cx;
	dy1  = y1 - cy;
	dx2  = x2 - cx;
	dy2  = y2 - cy;
	dx3  = x3 - cx;
	dy3  = y3 - cy;

	cosr1 = (dx1*dx2 + dy1*dy2)/(sqrt(dx1*dx1 + dy1*dy1)*sqrt(dx2*dx2 + dy2*dy2));
	cosr2 = (dx2*dx3 + dy2*dy3)/(sqrt(dx2*dx2 + dy2*dy2)*sqrt(dx3*dx3 + dy3*dy3));

	//掃引き角度(rad)
	rdr = acos(cosr1) + acos(cosr2);
	//円弧の長さ(rθ)
	ArcLen = abs(r * rdr);
	//円弧の長さを２０の倍数にする(rθ)
	IArcLen = static_cast<int>(ArcLen);
	Num     = IArcLen / 20;
	Num     = (Num < 1?1:Num);
	//補正した円弧の長さ
	ArcLen = static_cast<double>(Num) * 20.0;
	//補正した掃引き角度
	rdr = ArcLen / r;
	rdr = (rdr < 0.0?-rdr:rdr);
	//描画
	for(int Cnt = 0;Cnt < Num;Cnt++)
	{
		//開始角
		double sarg = rag1 + (rot * rdr * static_cast<double>(Cnt))/static_cast<double>(Num);
		//20分割して描画
		for(int div = 0;div < 21;div++)
		{
			//角度
			double narg = sarg + (rot * rdr * static_cast<double>(div))/(20.0 * static_cast<double>(Num));
			//座標
			double sx = cx + (r * cos(narg));
			double sy = cy + (r * sin(narg));
			//sin波変換
			double SinRad = (static_cast<double>(div) * Pai2) / 20.0;
			sx = sx + 10.0 * cos(narg) * sin(SinRad);
			sy = sy + 10.0 * sin(narg) * sin(SinRad);
			//線を引く
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
	//ペン削除
	delete pPen;

	return true;
}

//---------------------------------
//グルーオン円弧を描く(３点設定)
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
	//描画
	const double Pai  = 3.14159265359;
	const double Pai2 = Pai * 2.0;
	//ペン取得
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//３点から円を作成する
	calcCircleOf2Point(x1,y1,x2,y2,x3,y3,&cx,&cy,&r);

	//角度を得る
	ag1 = GetAngle(cx,cy,x1,y1);
	ag2 = GetAngle(cx,cy,x3,y3);
	//時計周り、反時計回り判別
	int OAx = x1 - cx;
	int OAy = y1 - cy;
	int ABx = x2 - x1;
	int ABy = y2 - y1;
	int crs = OAx * ABy - OAy * ABx;
	double rot = (crs >= 0?1.0:-1.0);
	//角度を得る(ラジアン)
	rag1 = (ag1 * 2.0 * Pai)/360.0;
	rag2 = (ag2 * 2.0 * Pai)/360.0;
	//掃引き角度
	dx1  = x1 - cx;
	dy1  = y1 - cy;
	dx2  = x2 - cx;
	dy2  = y2 - cy;
	dx3  = x3 - cx;
	dy3  = y3 - cy;

	cosr1 = (dx1*dx2 + dy1*dy2)/(sqrt(dx1*dx1 + dy1*dy1)*sqrt(dx2*dx2 + dy2*dy2));
	cosr2 = (dx2*dx3 + dy2*dy3)/(sqrt(dx2*dx2 + dy2*dy2)*sqrt(dx3*dx3 + dy3*dy3));

	//掃引き角度(rad)
	rdr = acos(cosr1) + acos(cosr2);

	//円弧の長さ(rθ)
	ArcLen = abs(r * rdr);
	//円弧の長さを１０の倍数にする(rθ)
	IArcLen = static_cast<int>(ArcLen);
	Num     = IArcLen / 10;
	Num     = (Num < 1?1:Num);
	//補正した円弧の長さ
	ArcLen = static_cast<double>(Num) * 10.0;
	//補正した掃引き角度
	rdr = ArcLen / r;
	rdr = (rdr < 0.0?-rdr:rdr);
	//初期ずらし位置
	double mv = 6.0;
	//初期ずらし角度
	double mrad = mv / r;

	//描画
	for(int Cnt = 0;Cnt < Num;Cnt++)
	{
		//開始角
        double sarg = rag1 + (rot * rdr * static_cast<double>(Cnt))/static_cast<double>(Num);
		//描画数
		int divmax = (Cnt < Num - 1?11:14);
		//20分割して描画
		for(int div = 0;div < divmax;div++)
        {
			//角度
			double narg = sarg + (rot * rdr * static_cast<double>(div))/(10.0 * static_cast<double>(Num));
			//描画原点
			double sx = cx + (r * cos(narg+mrad));
			double sy = cy + (r * sin(narg+mrad));
			//円の中心からの方向単位ベクトル
			double Vx = cos(narg+mrad);
			double Vy = sin(narg+mrad);
			//回転させる
			double sr = (static_cast<double>(div) * Pai2) / 10.0;
			double Rx = 6.0 * ( Vx * sin(sr) + Vy * cos(sr));
			double Ry = 6.0 * (-Vx * cos(sr) + Vy * sin(sr));
			sx = sx + Rx;
			sy = sy + Ry;
			//線を引く
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
	//ペン削除
	delete pPen;

	return true;
}

////---------------------------------
////ベジェ曲線座標導出
////---------------------------------
//T GetBezierCurvePoint(Vector3 &rslt, const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, float t)
//{
//	float mP0 =             (1-t)*(1-t)*(1-t);
//	float mP1 = 3 * t     * (1-t)*(1-t);
//	float mP2 = 3 * t*t   * (1-t);
//	float mP3 =     t*t*t;
//	rslt[0]	= p0[0]*mP0 + p1[0]*mP1 + p2[0]*mP2 + p3[0]*mP3;
//	rslt[1]	= p0[1]*mP0 + p1[1]*mP1 + p2[1]*mP2 + p3[1]*mP3;
//	rslt[2]	= p0[2]*mP0 + p1[2]*mP1 + p2[2]*mP2 + p3[2]*mP3;
//}

//---------------------------------
//グルーオンベジェ曲線描画(４点設定)
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
	//描画
	const double Pai  = 3.14159265359;
	const double Pai2 = Pai * 2.0;
	//ペン取得
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//３点から円を作成する
	calcCircleOf2Point(x1,y1,x2,y2,x3,y3,&cx,&cy,&r);

	//角度を得る
	ag1 = GetAngle(cx,cy,x1,y1);
	ag2 = GetAngle(cx,cy,x3,y3);
	//時計周り、反時計回り判別
	int OAx = x1 - cx;
	int OAy = y1 - cy;
	int ABx = x2 - x1;
	int ABy = y2 - y1;
	int crs = OAx * ABy - OAy * ABx;
	double rot = (crs >= 0?1.0:-1.0);
	//角度を得る(ラジアン)
	rag1 = (ag1 * 2.0 * Pai)/360.0;
	rag2 = (ag2 * 2.0 * Pai)/360.0;
	//掃引き角度
	dx1  = x1 - cx;
	dy1  = y1 - cy;
	dx2  = x2 - cx;
	dy2  = y2 - cy;
	dx3  = x3 - cx;
	dy3  = y3 - cy;

	cosr1 = (dx1*dx2 + dy1*dy2)/(sqrt(dx1*dx1 + dy1*dy1)*sqrt(dx2*dx2 + dy2*dy2));
	cosr2 = (dx2*dx3 + dy2*dy3)/(sqrt(dx2*dx2 + dy2*dy2)*sqrt(dx3*dx3 + dy3*dy3));

	//掃引き角度(rad)
	rdr = acos(cosr1) + acos(cosr2);

	//円弧の長さ(rθ)
	ArcLen = abs(r * rdr);
	//円弧の長さを１０の倍数にする(rθ)
	IArcLen = static_cast<int>(ArcLen);
	Num     = IArcLen / 10;
	Num     = (Num < 1?1:Num);
	//補正した円弧の長さ
	ArcLen = static_cast<double>(Num) * 10.0;
	//補正した掃引き角度
	rdr = ArcLen / r;
	rdr = (rdr < 0.0?-rdr:rdr);
	//初期ずらし位置
	double mv = 6.0;
	//初期ずらし角度
	double mrad = mv / r;

	//描画
	for(int Cnt = 0;Cnt < Num;Cnt++)
	{
		//開始角
		double sarg = rag1 + (rot * rdr * static_cast<double>(Cnt))/static_cast<double>(Num);
		//描画数
		int divmax = (Cnt < Num - 1?11:14);
		//20分割して描画
		for(int div = 0;div < divmax;div++)
		{
			//角度
			double narg = sarg + (rot * rdr * static_cast<double>(div))/(10.0 * static_cast<double>(Num));
			//描画原点
			double sx = cx + (r * cos(narg+mrad));
			double sy = cy + (r * sin(narg+mrad));
			//円の中心からの方向単位ベクトル
			double Vx = cos(narg+mrad);
			double Vy = sin(narg+mrad);
			//回転させる
			double sr = (static_cast<double>(div) * Pai2) / 10.0;
			double Rx = 6.0 * ( Vx * sin(sr) + Vy * cos(sr));
			double Ry = 6.0 * (-Vx * cos(sr) + Vy * sin(sr));
			sx = sx + Rx;
			sy = sy + Ry;
			//線を引く
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
	//ペン削除
	delete pPen;

	return true;
}

//---------------------------------
//ベジェ曲線描画(４点設定)
//---------------------------------
bool GDIPulsUnit::Bezier(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	int cx,cy,r;
	Gdiplus::Rect rect;
	double dr;
	//ペン取得
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//ベジェ曲線描画
	Graphics->DrawBezier(pPen,x1,y1,x2,y2,x3,y3,x4,y4);

	//ペン削除
	delete pPen;

	return true;
}


//---------------------------------
//円を描く
//---------------------------------
bool GDIPulsUnit::Circle(int cx,int cy,int r)
{
	//ペン取得
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//四角形座標作成
	Gdiplus::Rect rect;

	rect.X      = cx - r;
	rect.Y      = cy - r;
	rect.Width  = 2 * r;
	rect.Height = 2 * r;


	//円を描く
	Graphics->DrawEllipse(pPen,rect);

	//ペン削除
	delete pPen;

	return true;
}

//---------------------------------
//円を描く(四角形指定)
//---------------------------------
bool GDIPulsUnit::Ellipse(int x1,int y1,int x2,int y2)
{
	//ペン取得
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//四角形座標作成
	Gdiplus::Rect rect;

	rect.X      = x1;
	rect.Y      = y1;
	rect.Width  = x2 - x1 + 1;
	rect.Height = y2 - y1 + 1;

	//円を描く
	Graphics->DrawEllipse(pPen,rect);

	//ペン削除
	delete pPen;

	return true;
}

//---------------------------------
//四角形を描く
//---------------------------------
bool GDIPulsUnit::Box(int x1,int y1,int x2,int y2)
{
	//ペン取得
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//四角形座標作成
	Gdiplus::Rect rect;

	rect.X      = x1;
	rect.Y      = y1;
	rect.Width  = x2 - x1 - 1;
	rect.Height = y2 - y1 - 1;

	//四角形を描く
	Graphics->DrawRectangle(pPen,rect);

	//ペン削除
	delete pPen;

	return true;
}
//---------------------------------
//枠線付きの塗りつぶした四角を描く(四角形指定)
//---------------------------------
bool GDIPulsUnit::FillBoxAndRectAngle(int x1,int y1,int x2,int y2)
{
	//ペン取得
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//ブラシ取得
	Gdiplus::Brush *pBrush = GetSolidBrush();
	//四角形座標作成
	Gdiplus::Rect rect;

	rect.X      = x1;
	rect.Y      = y1;
	rect.Width  = x2 - x1 - 1;
	rect.Height = y2 - y1 - 1;
	//塗りつぶした四角形描画
	Graphics->FillRectangle(pBrush, rect);
	//四角形を描く
	Graphics->DrawRectangle(pPen,rect);
	//ペン削除
	delete pPen;
	//ブラシ削除
	delete pBrush;

	return true;
}
//---------------------------------
//塗りつぶした四角を描く(四角形指定)
//---------------------------------
bool GDIPulsUnit::FillBox(int x1,int y1,int x2,int y2)
{
	//ブラシ取得
	Gdiplus::Brush *pBrush = GetSolidBrush();
	//四角形座標作成
	Gdiplus::Rect rect;

	rect.X      = x1;
	rect.Y      = y1;
	rect.Width  = x2 - x1 - 1;
	rect.Height = y2 - y1 - 1;
	//塗りつぶした四角形描画
	Graphics->FillRectangle(pBrush, rect);
	//ブラシ削除
	delete pBrush;

	return true;
}
//---------------------------------
//テキストの描画
//---------------------------------
bool GDIPulsUnit::TextOut(String txt,int x,int y)
{
	Gdiplus::FontFamily fontFamily(Canvas->Font->Name.c_str());
	Gdiplus::Font       font(&fontFamily,Canvas->Font->Size,Gdiplus::FontStyleRegular,Gdiplus::UnitPixel);
	Gdiplus::PointF     pointF(x,y);
	//ブラシ取得
	Gdiplus::Brush *pBrush = GetSolidBrush(true);
    //文字列描画
	Graphics->DrawString(txt.c_str(), -1, &font, pointF,pBrush);
	//ブラシ削除
	delete pBrush;

	return true;
}

//-----------------------------------------------------------------------------------
//３点から円を作成する
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
//角度を得る(0-360)
//-----------------------------------------------------------------------------------
double GDIPulsUnit::GetAngle(double  cx, double  cy, double  x, double  y)
{
	double ang;
	//描画
	const double Pai = 3.14159265359;
	//方向ベクトル
	double Vx = x - cx;
	double Vy = y - cy;
	//角度を得る
	if(Vy >= 0)
	{
		//内積(1,0)と
		double ip = Vx;
		//cosθ
        double cs = ip / sqrt(Vx*Vx + Vy*Vy);
		//角度
		ang = (acos(cs) *180) / Pai;
	}
	else
	{
		//内積(1,0)と
		double ip = Vx;
		//cosθ
		double cs = ip / sqrt(Vx*Vx + Vy*Vy);
		//角度
		ang = 360.0 - (acos(cs) *180) / Pai;
	}
	return ang;
}
//-----------------------------------------------------------------------------------
//文字列の幅と高さを得る
//-----------------------------------------------------------------------------------
bool GDIPulsUnit::GetTextSize(String txt,int& width,int& height)
{
	Gdiplus::StringFormat sf;
	Gdiplus::RectF        boundRect;
	Gdiplus::RectF        layoutRect(0.0f, 0.0f, 1024.0f,1024.0f);

	//フォントの作成
	Gdiplus::Font *font = GDIPulsUnit::GetFont();

	//文字列計測
	Graphics->MeasureString(txt.c_str(),txt.Length(),font,layoutRect,&sf,&boundRect);
	//削除
	delete font;
	//幅と高さ
	width  = boundRect.Width;
	height = boundRect.Height;

	return true;
}

