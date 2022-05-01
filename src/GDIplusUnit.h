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
	//�R���X�g���N�^
	GDIPulsUnit();
private:
	void SetStartArrow(bool ar);
	void SetCenterArrow(bool ar);
	void SetEndArrow(bool ar);
public:
	//�R�_����~���쐬����
	static double calcCircleOf2Point(double x1, double y1, double x2, double y2, double x3, double y3, double *cx, double *cy, double *r);
	//�p�x�𓾂�(0-360)
	static double GetAngle(double cx, double cy, double x, double y);
public:
	//GDI+�J�n
	bool Startup();
	//GDI+�I��
	bool Shutdown();
	//�`��J�n
	bool Begin(TCanvas *pc,bool isAlpha,int alphaVal,bool isStrAlpha = false,int strAlphaVal = 0);
//	bool Begin(TCanvas *pc,bool isAlpha,int alphaVal);
//	bool Begin(TCanvas *pc,bool isAlpha=false,int alphaVal=0);
	//�`��I��
	bool End();
	//�A���`�G�C���A�X��L���ɂ���
	bool AntiAlias();
	//Graphics�I�u�W�F�N�g�擾
	Gdiplus::Graphics& getGraphics();
	//�J�n�ʒu�̐ݒ�
	bool MoveTo(int x1,int y1);
	//����`��
	bool LineTo(int x1,int y1,int x2,int y2);
	//����`��(�I�_�����ݒ�)
	bool LineTo(int x2,int y2);
	//�~�ʂ�`��(�R�_�ݒ�)
	bool Arc(int x1,int y1,int x2,int y2,int x3,int y3,bool& cw,double& sangle,double& eangle);
	//�~�ʂ�`��2
	bool Arc2(int w,int h,bool cw,double sangle,double eangle);
	//���q�~�ʂ�`��(�R�_�ݒ�)
	bool PhotonArc(int x1,int y1,int x2,int y2,int x3,int y3);
	//�O���[�I���~�ʂ�`��(�R�_�ݒ�)
	bool GluonArc(int x1,int y1,int x2,int y2,int x3,int y3);
	//�O���[�I���x�W�F�Ȑ��`��(�S�_�ݒ�)
	bool GluonBezier(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4);
	//�~��`��
	bool Circle(int cx,int cy,int r);
	//�~��`��(�l�p�`�w��)
	bool Ellipse(int x1,int y1,int x2,int y2);
	//�x�W�F�Ȑ��`��(�S�_�ݒ�)
	bool Bezier(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4);
	//�l�p��`��(�l�p�`�w��)
	bool Box(int x1,int y1,int x2,int y2);
	//�h��Ԃ����l�p��`��(�l�p�`�w��)
	bool FillBox(int x1,int y1,int x2,int y2);
	//�g���t���̓h��Ԃ����l�p��`��(�l�p�`�w��)
	bool FillBoxAndRectAngle(int x1,int y1,int x2,int y2);
	//������̕��ƍ����𓾂�
	bool GetTextSize(String txt,int& width,int& height);
	//�e�L�X�g�̕`��
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
