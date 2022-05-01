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
//�R���X�g���N�^
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
//GDI+�J�n
//---------------------------------
bool GDIPulsUnit::Startup()
{
	Gdiplus::Status S = GdiplusStartup(&gdiToken,&gdiSI, NULL);

	return (S == Gdiplus::Ok);
}

//---------------------------------
//GDI+�I��
//---------------------------------
bool GDIPulsUnit::Shutdown()
{
	GdiplusShutdown(gdiToken);

	return true;
}

//---------------------------------
//�`��J�n
//---------------------------------
bool GDIPulsUnit::Begin(TCanvas *pc,bool isAlpha,int alphaVal,bool isStrAlpha,int strAlphaVal)
{
	//HDC
	HDC hdc = pc->Handle;
	//Canvas�ۑ�
	Canvas = pc;

	//Graphics�I�u�W�F�N�g�쐬
	Graphics = new Gdiplus::Graphics(hdc);
	//�����x�ݒ�
	FAlphaBlend         = isAlpha;
	FAlphaBlendValue    = alphaVal;
	FStrAlphaBlend      = isStrAlpha;
	FStrAlphaBlendValue = strAlphaVal;

	return true;
}
//---------------------------------
//�`��I��
//---------------------------------
bool GDIPulsUnit::End()
{
	//�������
	delete Graphics;
	hdc  = 0;

	return true;
}

//---------------------------------
//Graphics�I�u�W�F�N�g�擾
//---------------------------------
Gdiplus::Graphics& GDIPulsUnit::getGraphics()
{
	return *Graphics;
}

//---------------------------------
//Pen�̍쐬
//---------------------------------
Gdiplus::Pen *GDIPulsUnit::GetPen()
{
	Gdiplus::Pen *p = nullptr;
	unsigned long RGB = Canvas->Pen->Color;

	int R =  RGB&0xFF;
	int G = (RGB>>8)&0xFF;
	int B = (RGB>>16)&0xFF;
	int w = Canvas->Pen->Width;

	//�A���t�@�l
	if(FAlphaBlend == true)
	{
		//�y���쐬
		p = new Gdiplus::Pen(Gdiplus::Color(255 - (FAlphaBlendValue % 256),R,G,B),w);
	}
	else
	{
		//�y���쐬
		p = new Gdiplus::Pen(Gdiplus::Color(255,R,G,B),w);
	}
	//����ݒ�
	switch(Canvas->Pen->Style)
	{
		case psDash: //�j��
		{
			p->SetDashStyle(Gdiplus::DashStyleDash);
			break;
		}
		case psDot: //�_��
		{
			p->SetDashStyle(Gdiplus::DashStyleDot);
			break;
		}
		case psDashDot: //��_����
		{
			p->SetDashStyle(Gdiplus::DashStyleDashDot);
			break;
		}
		case psDashDotDot: //��_����
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
//�u���V(�P�F)�̍쐬
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
	//�A���t�@�l
	if(UsePenColor == false)
	{
		if(FAlphaBlend == true)
		{
			//�u���V�쐬
			p = new Gdiplus::SolidBrush(Gdiplus::Color(255 - (FAlphaBlendValue % 256),R,G,B));
		}
		else
		{
			//�u���V�쐬
			p = new Gdiplus::SolidBrush(Gdiplus::Color(255,R,G,B));
		}
	}
	else
	{
		if(FStrAlphaBlend == true)
		{
			//�u���V�쐬
			p = new Gdiplus::SolidBrush(Gdiplus::Color(255 - (FStrAlphaBlendValue % 256),R,G,B));
		}
		else
		{
			//�u���V�쐬
			p = new Gdiplus::SolidBrush(Gdiplus::Color(255,R,G,B));
		}
	}

	return p;
}
//---------------------------------
//�t�H���g�̍쐬
//---------------------------------
Gdiplus::Font *GDIPulsUnit::GetFont()
{
	Gdiplus::FontFamily fontFamily(Canvas->Font->Name.c_str());
	//�t�H���g�t�@�~���[�X�^�C��
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
//�A���`�G�C���A�X��L���ɂ���
//---------------------------------
bool GDIPulsUnit::AntiAlias()
{
	Graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	return true;
}

//---------------------------------
//�J�n�ʒu�̐ݒ�
//---------------------------------
bool GDIPulsUnit::MoveTo(int x1,int y1)
{
	SPos.x = x1;
	SPos.y = y1;

	return true;
}

//---------------------------------
//���
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
//����`��
//---------------------------------
bool GDIPulsUnit::LineTo(int x1,int y1,int x2,int y2)
{
	Gdiplus::AdjustableArrowCap *LGPStartArrow = nullptr;
	Gdiplus::AdjustableArrowCap *LGPEndArrow = nullptr;
	//�y���擾
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//���

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
		//����`��
		Graphics->DrawLine(pPen,x1,y1,x2,y2);
	}
	else
	{
		//���ԍ��W
		int cx1 = (x1 + x2)/2;
		int cy1 = (y1 + y2)/2;
		//����`��1
		if(FStartArrow == true)
		{
			LGPStartArrow = new Gdiplus::AdjustableArrowCap(8, 8, false);
			pPen->SetCustomStartCap(LGPStartArrow);
		}
		LGPEndArrow = new Gdiplus::AdjustableArrowCap(8, 8, false);
		pPen->SetCustomEndCap(LGPEndArrow);
		Graphics->DrawLine(pPen,x1,y1,cx1,cy1);
		//����`��2
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

	//���폜
	if(LGPStartArrow != nullptr)
	{
		delete LGPStartArrow;
	}
	if(LGPEndArrow != nullptr)
	{
		delete LGPEndArrow;
	}
	//�y���폜
	delete pPen;
	//����̊J�n�ʒu
	SPos.x   = x2;
	SPos.y   = y2;

	return true;
}

//---------------------------------
//����`��(�I�_�����ݒ�)
//---------------------------------
bool GDIPulsUnit::LineTo(int x2,int y2)
{
	//�n�_�ݒ�
	int x1 = SPos.x;
	int y1 = SPos.y;
	//����`��
	LineTo(x1,y1,x2,y2);

	//����̊J�n�ʒu
	SPos.x   = x2;
	SPos.y   = y2;

	return true;
}

//---------------------------------
//�~�ʂ�`��(�R�_�ݒ�)
//---------------------------------
bool GDIPulsUnit::Arc(int x1,int y1,int x2,int y2,int x3,int y3,bool& cw,double& sangle,double& eangle)
{
	double cx,cy,r;
	Gdiplus::Rect rect;
	double dr;
	//�y���擾
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//�R�_����~���쐬����
	calcCircleOf2Point(x1,y1,x2,y2,x3,y3,&cx,&cy,&r);
	//���v����A�����v��蔻��
	double OAx = x1 - cx;
	double OAy = y1 - cy;
	double ABx = x2 - x1;
	double ABy = y2 - y1;
	double crs = OAx * ABy - OAy * ABx;
	int    rot = (crs >= 0?1:-1);
	//�p�x�𓾂�(x ������~�ʂ̊J�n�_�܂ŁA���v���ɑ��肵���p�x (�x�P��))
	double ag1 = GetAngle(cx,cy,x1,y1);
	double ag2 = GetAngle(cx,cy,x3,y3);
	//�l�p�`���W�쐬
	rect.X      = cx - r;
	rect.Y      = cy - r;
	rect.Width  = 2 * r;
	rect.Height = 2 * r;
	//�|�����p�x
	if(rot < 0)
	{
		std::swap(ag1,ag2);
	}

	dr = ag2 - ag1;

	if(dr < 0.0)
	{
		dr = 360.0 - (ag1 - ag2);
	}

	//�~�ʂ�`��
	Graphics->DrawArc(pPen,rect,ag1,dr);

	//�p�x�����Z�b�g
	cw     = true;
	sangle = ag1;
	eangle = ag2;

	//�y���폜
	delete pPen;

	return true;
}
//---------------------------------
//�~�ʂ�`��2
//---------------------------------
bool GDIPulsUnit::Arc2(int w,int h,bool cw,double sangle,double eangle)
{
	double dr;
	Gdiplus::Rect rect;
	//�y���擾
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//�l�p�`���W�쐬
	rect.X      = pPen->GetWidth();
	rect.Y      = pPen->GetWidth();
	rect.Width  = w - pPen->GetWidth()*2;
	rect.Height = h - pPen->GetWidth()*2;
	//�|�����p�x
	dr = eangle - sangle;

	if(dr < 0.0)
	{
		dr = 360.0 - (sangle - eangle);
	}
	//�~�ʂ�`��
	Graphics->DrawArc(pPen,rect,sangle,dr);
	//�y���폜
	delete pPen;

	return true;
}
//---------------------------------
//���q�~�ʂ�`��(�R�_�ݒ�)
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
	//�`��
	const double Pai  = 3.14159265359;
	const double Pai2 = Pai * 2.0;
	//�y���擾
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//�R�_����~���쐬����
	calcCircleOf2Point(x1,y1,x2,y2,x3,y3,&cx,&cy,&r);

	//�p�x�𓾂�
	ag1 = GetAngle(cx,cy,x1,y1);
	ag2 = GetAngle(cx,cy,x3,y3);
	//���v����A�����v��蔻��
	double OAx = x1 - cx;
	double OAy = y1 - cy;
	double ABx = x2 - x1;
	double ABy = y2 - y1;
	double crs = OAx * ABy - OAy * ABx;
	int    rot = (crs >= 0?1.0:-1.0);

	//�p�x�𓾂�(���W�A��)
	rag1 = (ag1 * 2.0 * Pai)/360.0;
	rag2 = (ag2 * 2.0 * Pai)/360.0;

	//�|�����p�x
	dx1  = x1 - cx;
	dy1  = y1 - cy;
	dx2  = x2 - cx;
	dy2  = y2 - cy;
	dx3  = x3 - cx;
	dy3  = y3 - cy;

	cosr1 = (dx1*dx2 + dy1*dy2)/(sqrt(dx1*dx1 + dy1*dy1)*sqrt(dx2*dx2 + dy2*dy2));
	cosr2 = (dx2*dx3 + dy2*dy3)/(sqrt(dx2*dx2 + dy2*dy2)*sqrt(dx3*dx3 + dy3*dy3));

	//�|�����p�x(rad)
	rdr = acos(cosr1) + acos(cosr2);
	//�~�ʂ̒���(r��)
	ArcLen = abs(r * rdr);
	//�~�ʂ̒������Q�O�̔{���ɂ���(r��)
	IArcLen = static_cast<int>(ArcLen);
	Num     = IArcLen / 20;
	Num     = (Num < 1?1:Num);
	//�␳�����~�ʂ̒���
	ArcLen = static_cast<double>(Num) * 20.0;
	//�␳�����|�����p�x
	rdr = ArcLen / r;
	rdr = (rdr < 0.0?-rdr:rdr);
	//�`��
	for(int Cnt = 0;Cnt < Num;Cnt++)
	{
		//�J�n�p
		double sarg = rag1 + (rot * rdr * static_cast<double>(Cnt))/static_cast<double>(Num);
		//20�������ĕ`��
		for(int div = 0;div < 21;div++)
		{
			//�p�x
			double narg = sarg + (rot * rdr * static_cast<double>(div))/(20.0 * static_cast<double>(Num));
			//���W
			double sx = cx + (r * cos(narg));
			double sy = cy + (r * sin(narg));
			//sin�g�ϊ�
			double SinRad = (static_cast<double>(div) * Pai2) / 20.0;
			sx = sx + 10.0 * cos(narg) * sin(SinRad);
			sy = sy + 10.0 * sin(narg) * sin(SinRad);
			//��������
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
	//�y���폜
	delete pPen;

	return true;
}

//---------------------------------
//�O���[�I���~�ʂ�`��(�R�_�ݒ�)
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
	//�`��
	const double Pai  = 3.14159265359;
	const double Pai2 = Pai * 2.0;
	//�y���擾
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//�R�_����~���쐬����
	calcCircleOf2Point(x1,y1,x2,y2,x3,y3,&cx,&cy,&r);

	//�p�x�𓾂�
	ag1 = GetAngle(cx,cy,x1,y1);
	ag2 = GetAngle(cx,cy,x3,y3);
	//���v����A�����v��蔻��
	int OAx = x1 - cx;
	int OAy = y1 - cy;
	int ABx = x2 - x1;
	int ABy = y2 - y1;
	int crs = OAx * ABy - OAy * ABx;
	double rot = (crs >= 0?1.0:-1.0);
	//�p�x�𓾂�(���W�A��)
	rag1 = (ag1 * 2.0 * Pai)/360.0;
	rag2 = (ag2 * 2.0 * Pai)/360.0;
	//�|�����p�x
	dx1  = x1 - cx;
	dy1  = y1 - cy;
	dx2  = x2 - cx;
	dy2  = y2 - cy;
	dx3  = x3 - cx;
	dy3  = y3 - cy;

	cosr1 = (dx1*dx2 + dy1*dy2)/(sqrt(dx1*dx1 + dy1*dy1)*sqrt(dx2*dx2 + dy2*dy2));
	cosr2 = (dx2*dx3 + dy2*dy3)/(sqrt(dx2*dx2 + dy2*dy2)*sqrt(dx3*dx3 + dy3*dy3));

	//�|�����p�x(rad)
	rdr = acos(cosr1) + acos(cosr2);

	//�~�ʂ̒���(r��)
	ArcLen = abs(r * rdr);
	//�~�ʂ̒������P�O�̔{���ɂ���(r��)
	IArcLen = static_cast<int>(ArcLen);
	Num     = IArcLen / 10;
	Num     = (Num < 1?1:Num);
	//�␳�����~�ʂ̒���
	ArcLen = static_cast<double>(Num) * 10.0;
	//�␳�����|�����p�x
	rdr = ArcLen / r;
	rdr = (rdr < 0.0?-rdr:rdr);
	//�������炵�ʒu
	double mv = 6.0;
	//�������炵�p�x
	double mrad = mv / r;

	//�`��
	for(int Cnt = 0;Cnt < Num;Cnt++)
	{
		//�J�n�p
        double sarg = rag1 + (rot * rdr * static_cast<double>(Cnt))/static_cast<double>(Num);
		//�`�搔
		int divmax = (Cnt < Num - 1?11:14);
		//20�������ĕ`��
		for(int div = 0;div < divmax;div++)
        {
			//�p�x
			double narg = sarg + (rot * rdr * static_cast<double>(div))/(10.0 * static_cast<double>(Num));
			//�`�挴�_
			double sx = cx + (r * cos(narg+mrad));
			double sy = cy + (r * sin(narg+mrad));
			//�~�̒��S����̕����P�ʃx�N�g��
			double Vx = cos(narg+mrad);
			double Vy = sin(narg+mrad);
			//��]������
			double sr = (static_cast<double>(div) * Pai2) / 10.0;
			double Rx = 6.0 * ( Vx * sin(sr) + Vy * cos(sr));
			double Ry = 6.0 * (-Vx * cos(sr) + Vy * sin(sr));
			sx = sx + Rx;
			sy = sy + Ry;
			//��������
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
	//�y���폜
	delete pPen;

	return true;
}

////---------------------------------
////�x�W�F�Ȑ����W���o
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
//�O���[�I���x�W�F�Ȑ��`��(�S�_�ݒ�)
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
	//�`��
	const double Pai  = 3.14159265359;
	const double Pai2 = Pai * 2.0;
	//�y���擾
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//�R�_����~���쐬����
	calcCircleOf2Point(x1,y1,x2,y2,x3,y3,&cx,&cy,&r);

	//�p�x�𓾂�
	ag1 = GetAngle(cx,cy,x1,y1);
	ag2 = GetAngle(cx,cy,x3,y3);
	//���v����A�����v��蔻��
	int OAx = x1 - cx;
	int OAy = y1 - cy;
	int ABx = x2 - x1;
	int ABy = y2 - y1;
	int crs = OAx * ABy - OAy * ABx;
	double rot = (crs >= 0?1.0:-1.0);
	//�p�x�𓾂�(���W�A��)
	rag1 = (ag1 * 2.0 * Pai)/360.0;
	rag2 = (ag2 * 2.0 * Pai)/360.0;
	//�|�����p�x
	dx1  = x1 - cx;
	dy1  = y1 - cy;
	dx2  = x2 - cx;
	dy2  = y2 - cy;
	dx3  = x3 - cx;
	dy3  = y3 - cy;

	cosr1 = (dx1*dx2 + dy1*dy2)/(sqrt(dx1*dx1 + dy1*dy1)*sqrt(dx2*dx2 + dy2*dy2));
	cosr2 = (dx2*dx3 + dy2*dy3)/(sqrt(dx2*dx2 + dy2*dy2)*sqrt(dx3*dx3 + dy3*dy3));

	//�|�����p�x(rad)
	rdr = acos(cosr1) + acos(cosr2);

	//�~�ʂ̒���(r��)
	ArcLen = abs(r * rdr);
	//�~�ʂ̒������P�O�̔{���ɂ���(r��)
	IArcLen = static_cast<int>(ArcLen);
	Num     = IArcLen / 10;
	Num     = (Num < 1?1:Num);
	//�␳�����~�ʂ̒���
	ArcLen = static_cast<double>(Num) * 10.0;
	//�␳�����|�����p�x
	rdr = ArcLen / r;
	rdr = (rdr < 0.0?-rdr:rdr);
	//�������炵�ʒu
	double mv = 6.0;
	//�������炵�p�x
	double mrad = mv / r;

	//�`��
	for(int Cnt = 0;Cnt < Num;Cnt++)
	{
		//�J�n�p
		double sarg = rag1 + (rot * rdr * static_cast<double>(Cnt))/static_cast<double>(Num);
		//�`�搔
		int divmax = (Cnt < Num - 1?11:14);
		//20�������ĕ`��
		for(int div = 0;div < divmax;div++)
		{
			//�p�x
			double narg = sarg + (rot * rdr * static_cast<double>(div))/(10.0 * static_cast<double>(Num));
			//�`�挴�_
			double sx = cx + (r * cos(narg+mrad));
			double sy = cy + (r * sin(narg+mrad));
			//�~�̒��S����̕����P�ʃx�N�g��
			double Vx = cos(narg+mrad);
			double Vy = sin(narg+mrad);
			//��]������
			double sr = (static_cast<double>(div) * Pai2) / 10.0;
			double Rx = 6.0 * ( Vx * sin(sr) + Vy * cos(sr));
			double Ry = 6.0 * (-Vx * cos(sr) + Vy * sin(sr));
			sx = sx + Rx;
			sy = sy + Ry;
			//��������
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
	//�y���폜
	delete pPen;

	return true;
}

//---------------------------------
//�x�W�F�Ȑ��`��(�S�_�ݒ�)
//---------------------------------
bool GDIPulsUnit::Bezier(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	int cx,cy,r;
	Gdiplus::Rect rect;
	double dr;
	//�y���擾
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//�x�W�F�Ȑ��`��
	Graphics->DrawBezier(pPen,x1,y1,x2,y2,x3,y3,x4,y4);

	//�y���폜
	delete pPen;

	return true;
}


//---------------------------------
//�~��`��
//---------------------------------
bool GDIPulsUnit::Circle(int cx,int cy,int r)
{
	//�y���擾
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//�l�p�`���W�쐬
	Gdiplus::Rect rect;

	rect.X      = cx - r;
	rect.Y      = cy - r;
	rect.Width  = 2 * r;
	rect.Height = 2 * r;


	//�~��`��
	Graphics->DrawEllipse(pPen,rect);

	//�y���폜
	delete pPen;

	return true;
}

//---------------------------------
//�~��`��(�l�p�`�w��)
//---------------------------------
bool GDIPulsUnit::Ellipse(int x1,int y1,int x2,int y2)
{
	//�y���擾
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//�l�p�`���W�쐬
	Gdiplus::Rect rect;

	rect.X      = x1;
	rect.Y      = y1;
	rect.Width  = x2 - x1 + 1;
	rect.Height = y2 - y1 + 1;

	//�~��`��
	Graphics->DrawEllipse(pPen,rect);

	//�y���폜
	delete pPen;

	return true;
}

//---------------------------------
//�l�p�`��`��
//---------------------------------
bool GDIPulsUnit::Box(int x1,int y1,int x2,int y2)
{
	//�y���擾
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//�l�p�`���W�쐬
	Gdiplus::Rect rect;

	rect.X      = x1;
	rect.Y      = y1;
	rect.Width  = x2 - x1 - 1;
	rect.Height = y2 - y1 - 1;

	//�l�p�`��`��
	Graphics->DrawRectangle(pPen,rect);

	//�y���폜
	delete pPen;

	return true;
}
//---------------------------------
//�g���t���̓h��Ԃ����l�p��`��(�l�p�`�w��)
//---------------------------------
bool GDIPulsUnit::FillBoxAndRectAngle(int x1,int y1,int x2,int y2)
{
	//�y���擾
	Gdiplus::Pen *pPen = GDIPulsUnit::GetPen();
	//�u���V�擾
	Gdiplus::Brush *pBrush = GetSolidBrush();
	//�l�p�`���W�쐬
	Gdiplus::Rect rect;

	rect.X      = x1;
	rect.Y      = y1;
	rect.Width  = x2 - x1 - 1;
	rect.Height = y2 - y1 - 1;
	//�h��Ԃ����l�p�`�`��
	Graphics->FillRectangle(pBrush, rect);
	//�l�p�`��`��
	Graphics->DrawRectangle(pPen,rect);
	//�y���폜
	delete pPen;
	//�u���V�폜
	delete pBrush;

	return true;
}
//---------------------------------
//�h��Ԃ����l�p��`��(�l�p�`�w��)
//---------------------------------
bool GDIPulsUnit::FillBox(int x1,int y1,int x2,int y2)
{
	//�u���V�擾
	Gdiplus::Brush *pBrush = GetSolidBrush();
	//�l�p�`���W�쐬
	Gdiplus::Rect rect;

	rect.X      = x1;
	rect.Y      = y1;
	rect.Width  = x2 - x1 - 1;
	rect.Height = y2 - y1 - 1;
	//�h��Ԃ����l�p�`�`��
	Graphics->FillRectangle(pBrush, rect);
	//�u���V�폜
	delete pBrush;

	return true;
}
//---------------------------------
//�e�L�X�g�̕`��
//---------------------------------
bool GDIPulsUnit::TextOut(String txt,int x,int y)
{
	Gdiplus::FontFamily fontFamily(Canvas->Font->Name.c_str());
	Gdiplus::Font       font(&fontFamily,Canvas->Font->Size,Gdiplus::FontStyleRegular,Gdiplus::UnitPixel);
	Gdiplus::PointF     pointF(x,y);
	//�u���V�擾
	Gdiplus::Brush *pBrush = GetSolidBrush(true);
    //������`��
	Graphics->DrawString(txt.c_str(), -1, &font, pointF,pBrush);
	//�u���V�폜
	delete pBrush;

	return true;
}

//-----------------------------------------------------------------------------------
//�R�_����~���쐬����
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
//�p�x�𓾂�(0-360)
//-----------------------------------------------------------------------------------
double GDIPulsUnit::GetAngle(double  cx, double  cy, double  x, double  y)
{
	double ang;
	//�`��
	const double Pai = 3.14159265359;
	//�����x�N�g��
	double Vx = x - cx;
	double Vy = y - cy;
	//�p�x�𓾂�
	if(Vy >= 0)
	{
		//����(1,0)��
		double ip = Vx;
		//cos��
        double cs = ip / sqrt(Vx*Vx + Vy*Vy);
		//�p�x
		ang = (acos(cs) *180) / Pai;
	}
	else
	{
		//����(1,0)��
		double ip = Vx;
		//cos��
		double cs = ip / sqrt(Vx*Vx + Vy*Vy);
		//�p�x
		ang = 360.0 - (acos(cs) *180) / Pai;
	}
	return ang;
}
//-----------------------------------------------------------------------------------
//������̕��ƍ����𓾂�
//-----------------------------------------------------------------------------------
bool GDIPulsUnit::GetTextSize(String txt,int& width,int& height)
{
	Gdiplus::StringFormat sf;
	Gdiplus::RectF        boundRect;
	Gdiplus::RectF        layoutRect(0.0f, 0.0f, 1024.0f,1024.0f);

	//�t�H���g�̍쐬
	Gdiplus::Font *font = GDIPulsUnit::GetFont();

	//������v��
	Graphics->MeasureString(txt.c_str(),txt.Length(),font,layoutRect,&sf,&boundRect);
	//�폜
	delete font;
	//���ƍ���
	width  = boundRect.Width;
	height = boundRect.Height;

	return true;
}

