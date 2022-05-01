//---------------------------------------------------------------------------
#ifndef DIBInfH
#define DIBInfH
//---------------------------------------------------------------------------
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "GDIplusUnit.h"

//�w�i����2,3�l�����
enum typBackSub {tbsNone=0 /*�w�i�����Ȃ�*/ ,tbsHalf=128 /*�Е������w�i����������*/,tbsFull=255 /*�����w�i����������*/};

//---------------------------------------------------------------------------
//�s�N�Z���A�N�Z�X�\����
//---------------------------------------------------------------------------
struct PixRGB
{
	unsigned char B;
	unsigned char G;
	unsigned char R;
};

//DIB���
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
	//�R�s�[
	void _copy(const DIBInf& h);
	//�J��
	void _free();
public:
	//�R���X�g���N�^
	DIBInf();
	DIBInf(TBitmap *pb);
	DIBInf(const String& file);
	DIBInf(const DIBInf& h);

	//�f�X�g���N�^
	virtual ~DIBInf();
public:
	DIBInf& operator = (const DIBInf& h);
protected:
	//���W�`�F�b�N
	bool ChechPos(int x,int y);
public:
	//������
	bool Init();
	//�r�b�g�}�b�v�𓾂�
	TBitmap *GetBitmap();
	//DIB��
	bool ConvToDIB(int width,int height,TPixelFormat pf = pf24bit);
	bool ConvToDIB(TPixelFormat pf = pf24bit);
	//DIB->Bitmap��
	bool ConvDibToBitmap();
	bool ConvToBitmap();
	//0�Ŗ��߂�
	bool FillZero();
	//�����F
	bool SetTransparentColor(TColor tc);
public:
	//�L�����o�X
	TCanvas *GetCanvas();
	//���A����
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
	//�t�@�C���ǂݍ���
	bool LoadFromFile(String path,bool IsMakeDIB = false);
	//�ړI�̒l���ŏ���Y�̌���
	int SrchMinY(unsigned char val);
	//�w�肳�ꂽY���W�ŖړI�̒l������X���W��Ԃ�
	int ScanLine(int y,unsigned char val);
	//������DIBInf��OR�ō�������
	bool OR(DIBInf& inf);
	//������DIBInf�Ƒ����Z�ō�������
	bool Plus(DIBInf& inf);
	//�O���C�X�P�[��DIBInf�̒l���������l�����Ȃ�J�b�g����
	bool Cut(unsigned char th);
	//DIBInf�̒l���������l�ȏ�Ȃ甒�ɂ���
	bool White(int th);
	//DIBInf�̒l���������l�ȏ�̔䗦�Ȃ甒�ɂ���(��2�����͔w�i�摜)
	bool White2(DIBInf& BKGInf,int th,int fr=0);
	//�O���C�X�P�[��DIBInf�̍����Z�b�g
	bool Sub(DIBInf& inf);
	//�O���C�X�P�[��DIBInf�̘_����(AND)���Z�b�g
	bool And(DIBInf& inf);
	//�m�C�Y�̍폜
	bool DeleteNoise(int bw,int bh,double border);
	//�w��͈͂̉摜��؂�o����DIBInf�Ɋi�[
	bool CutOutRectImage(DIBInf& src,int left,int top,int right,int bottom);
	//�摜���w��ʒu�ɓ\��t������Bitmap�Ɋi�[
	bool PasteRectImage(DIBInf& src,int left,int top);
	//�c������
	bool Image_dilation();
	bool Image_dilation(int l,int t,int r,int b);
	//���k����
	bool Image_erosion();
	bool Image_erosion(int l,int t,int r,int b);
	//�N���[�W���O����
	bool Closing(int cn);
	bool Closing(int cn,int l,int t,int r,int b);

	//��`�h��Ԃ�
	bool FillRect(const Gdiplus::Rect& R,const PixRGB& col);

public:
	PixRGB* _RGB(int x,int y);
	unsigned char& R(int x,int y);
	unsigned char& G(int x,int y);
	unsigned char& B(int x,int y);
};



#endif
