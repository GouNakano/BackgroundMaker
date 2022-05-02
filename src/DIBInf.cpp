//---------------------------------------------------------------------------

#pragma hdrstop

#include <io.h>
#include "DIBInf.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

static const PixRGB pxBlack = {  0  ,0  ,0};
static const PixRGB pxWihte = {255,255,255};


//===========================================================================
//DIBInf
//===========================================================================

DIBInf::DIBInf()
{
	pBmp        = new TBitmap;
	infinsz     = 0;
	imginsz     = 0;
	pImg        = nullptr;
	hr          = nullptr;
	ByteOfPixel = 3;
}
DIBInf::DIBInf(TBitmap *pb)
{
	infinsz     = 0;
	imginsz     = 0;
	hr          = nullptr;
	pImg        = nullptr;

	pBmp              = new TBitmap;
	pBmp->Width       = pb->Width;
	pBmp->Height      = pb->Height;
	pBmp->PixelFormat = pf24bit;

	pBmp->Canvas->StretchDraw(TRect(0,0,pBmp->Width,pBmp->Height),pb);
	ConvToDIB(pBmp->Width,pBmp->Height);
}
DIBInf::DIBInf(const String& file)
:DIBInf()
{
	try
	{
		pBmp->LoadFromFile(file);
		pBmp->PixelFormat = pf24bit;
		ConvToDIB(pBmp->Width,pBmp->Height);
	}
	catch(Exception& e)
	{
		delete pBmp;
		pBmp = nullptr;
		Init();
		return;
	}
}


DIBInf::DIBInf(const DIBInf& h)
:DIBInf()
{

	_copy(h);
}
DIBInf::~DIBInf()
{

	_free();
}

void DIBInf::_free()
{
	delete pBmp;
	infinsz     = 0;
	imginsz     = 0;
	ByteOfPixel = 3;

	if(pImg != nullptr)
	{
		delete [] pImg;
		pImg = nullptr;
	}
	if(hr != nullptr)
	{
		delete [] hr;
		hr = nullptr;
	}
}

void DIBInf::_copy(const DIBInf& h)
{

	Init();

	pBmp->Width       = h.pBmp->Width;
	pBmp->Height      = h.pBmp->Height;
	pBmp->PixelFormat = h.pBmp->PixelFormat;


	BOOL res = StretchBlt(
	  this->pBmp->Canvas->Handle,          //[in] HDC   hdcDest,
	  0,                                   //[in] int   xDest,
	  0,                                   //[in] int   yDest,
	  pBmp->Width,                         //[in] int   wDest,
	  pBmp->Height,                        //[in] int   hDest,
	  h.pBmp->Canvas->Handle,              //[in] HDC   hdcSrc,
	  0,                                   //[in] int   xSrc,
	  0,                                   //[in] int   ySrc,
	  pBmp->Width,                         //[in] int   wSrc,
	  pBmp->Height,                        //[in] int   hSrc,
	  SRCCOPY                              //[in] DWORD rop
	);

	if(res == 0)
	{
		return;
	}


	if(pImg != nullptr)
	{
		delete [] pImg;
		pImg = nullptr;
	}
	if(hr != nullptr)
	{
		delete [] hr;
		hr = nullptr;
	}
	pImg = new unsigned char[h.imginsz];
	hr   = (BITMAPINFO *)new unsigned char[h.infinsz];

	memcpy(pImg,h.pImg,h.imginsz);
	memcpy(hr,h.hr,h.infinsz);

	imginsz = h.imginsz;
	infinsz = h.infinsz;


	pBmp->Transparent      = h.pBmp->Transparent;
	pBmp->TransparentColor = h.pBmp->TransparentColor;
	pBmp->TransparentMode  = h.pBmp->TransparentMode;


	return;
}

//---------------------------------------------------------------------------
//operator =
//---------------------------------------------------------------------------
DIBInf& DIBInf::operator = (const DIBInf& h)
{
	_copy(h);

	return *this;
}

//---------------------------------------------------------------------------
bool DIBInf::ChechPos(int x,int y)
{
	bool res;

	if((x < 0 || x >= pBmp->Width) || (y < 0 || y >= pBmp->Height))
	{
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
TCanvas *DIBInf::GetCanvas()
{
	return pBmp->Canvas;
}
//---------------------------------------------------------------------------
int  __fastcall DIBInf::GetWidth()
{
	return pBmp->Width;
}
void __fastcall DIBInf::SetWidth(int w)
{
	pBmp->Width = w;
}
int  __fastcall DIBInf::GetHeight()
{
	return pBmp->Height;
}
void __fastcall DIBInf::SetHeight(int h)
{
	pBmp->Height = h;
}

//---------------------------------------------------------------------------
bool DIBInf::Init()
{
	infinsz     = 0;
	imginsz     = 0;
	ByteOfPixel = 3;

	if(pImg != nullptr)
	{
		delete [] pImg;
		pImg = nullptr;
	}
	if(hr != nullptr)
	{
		delete [] hr;
		hr = nullptr;
	}
	return true;
}
//---------------------------------------------------------------------------
TBitmap *DIBInf::GetBitmap()
{
	return pBmp;
}
//---------------------------------------------------------------------------
bool DIBInf::ConvToDIB(int width,int height,TPixelFormat pf)
{
	pBmp->Width       = width;
	pBmp->Height      = height;
	pBmp->PixelFormat = pf;
	pBmp->Dormant();
	pBmp->FreeImage();

	Init();

	GetDIBSizes(pBmp->Handle,infinsz,imginsz);
	pImg = new unsigned char[imginsz];
	hr   = (BITMAPINFO *)new unsigned char[infinsz];
	GetDIB(pBmp->Handle,pBmp->Palette,hr,pImg);


	if(pf == pf8bit)
	{
		ByteOfPixel = 1;
		int sh = hr->bmiHeader.biSize;
		unsigned char *pHr   = (unsigned char *)hr;
		unsigned char *pPlte = &pHr[sh];

		for(int cnt=0;cnt < 256;cnt++)
		{
			pPlte[0] = cnt;
			pPlte[1] = cnt;
			pPlte[2] = cnt;
			pPlte[3] = 0;
			pPlte += 4;
		}
	}
	else
	{
		ByteOfPixel = 3;
	}

	return true;
}
bool DIBInf::ConvToDIB(TPixelFormat pf)
{
	return ConvToDIB(pBmp->Width,pBmp->Height,pf);
}
//---------------------------------------------------------------------------
bool DIBInf::ConvDibToBitmap()
{
	StretchDIBits(
		pBmp->Canvas->Handle,
		0,
		0,
		pBmp->Width,
		pBmp->Height,
		0,
		0,
		pBmp->Width,
		pBmp->Height,
		pImg,
		hr,
		DIB_RGB_COLORS,
		SRCCOPY
	);

	return true;
}
//---------------------------------------------------------------------------
bool DIBInf::ConvToBitmap()
{
	return ConvDibToBitmap();
}
//---------------------------------------------------------------------------
bool DIBInf::FillZero()
{
	memset(pImg,0,imginsz);

	return true;
}
//---------------------------------------------------------------------------
bool DIBInf::SetTransparentColor(TColor tc)
{
	pBmp->Transparent      = true;
	pBmp->TransparentColor = tc;
	pBmp->TransparentMode  = tmFixed;

	return true;
}
//---------------------------------------------------------------------------
bool DIBInf::SetGray(int x,int y,unsigned char val)
{
	if(pBmp->PixelFormat == pf8bit)
	{
		pImg[y*pBmp->Width + x] = val;
	}
	else
	{
		PixRGB *pix = _RGB(x,y);

		if(val == tbsFull)
		{
			*pix = pxWihte;
		}
		else if(val == tbsNone)
		{
			*pix = pxBlack;
		}
		else
		{
			pix->R = val;
			pix->G = val;
			pix->B = val;
		}
	}
	return true;
}
//---------------------------------------------------------------------------
bool DIBInf::AddGray(int x,int y,unsigned char val)
{
	if(pBmp->PixelFormat == pf8bit)
	{
		pImg[y*pBmp->Width + x] += val;
	}
	else
	{
		PixRGB *pix = _RGB(x,y);

		pix->R += val;
		pix->G += val;
		pix->B += val;
	}

	return true;
}

//---------------------------------------------------------------------------
unsigned char DIBInf::GetGray(int x,int y)
{
	unsigned char res;

	if(pBmp->PixelFormat == pf8bit)
	{
		return pImg[y*pBmp->Width + x];
	}
	else
	{
		PixRGB *pix = _RGB(x,y);

		unsigned char r = pix->R;
		unsigned char g = pix->G;
		unsigned char b = pix->B;

		//RGBが同値かチェック
		if(r==g && r==b)
		{
			res = r;
		}
		else
		{
			res = b*0.11 + g*0.59 + r*0.30;
		}
	}
	return res;
}
//---------------------------------------------------------------------------
bool DIBInf::ChangeToGrayScale()
{
	if(pBmp->PixelFormat == pf8bit)
	{
		return true;
	}
	else
	{
		for(int y=0;y < pBmp->Height;y++)
		{
			for(int x=0;x < pBmp->Width;x++)
			{
				unsigned char gray = GetGray(x,y);

				PixRGB *pix = _RGB(x,y);

				pix->R = gray;
				pix->G = gray;
				pix->B = gray;
			}
		}

	}
	return true;
}
//---------------------------------------------------------------------------
bool DIBInf::SaveToFile(String path)
{
	if(pBmp == nullptr)
	{
		return false;
	}
	try
	{
		if(_waccess(path.c_str(),00) == 0)
		{
			if(_waccess(path.c_str(),02) != 0)
			{
				return false;
			}
		}
		pBmp->SaveToFile(path);

	}
	catch(Exception& e)
	{
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
bool DIBInf::LoadFromFile(String path,bool IsMakeDIB)
{
	if(pBmp == nullptr)
	{
		return false;
	}
	try
	{
		if(_waccess(path.c_str(),00) == 0)
		{
			if(_waccess(path.c_str(),04) != 0)
			{
				return false;
			}
		}
		Init();

		pBmp->LoadFromFile(path);

		if(IsMakeDIB == true)
		{
			ConvToDIB();
		}
	}
	catch(Exception& e)
	{
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
int DIBInf::SrchMinY(unsigned char val)
{
	int ymin = -1;

	for(int y=0;y < pBmp->Height;y++)
	{
		if(ScanLine(y,val) >= 0)
		{
			ymin = y;
			break;
		}
	}

	return ymin;
}
//---------------------------------------------------------------------------
int DIBInf::ScanLine(int y,unsigned char val)
{
	if(y < 0 || y > pBmp->Height - 1)
	{
		return -1;
	}

	int ls = ByteOfPixel * pBmp->Width;
	int idx = ls * (pBmp->Height - y -1);

	unsigned char *p = static_cast<unsigned char *>(memchr(&pImg[idx],val,ls));

	if(p == nullptr)
	{
		return -1;
	}

	int x = (p - &pImg[idx])/ByteOfPixel;

	return x;
}
//---------------------------------------------------------------------------
bool DIBInf::OR(DIBInf& inf)
{
	int w = pBmp->Width;
	int h = pBmp->Height;

	for(int y=0;y < h;y++)
	{
		PixRGB *pix     = this->_RGB(0,y);
		PixRGB *pix_inf = inf.  _RGB(0,y);

		for(int x=0;x < w;x++)
		{
			pix->R |= pix_inf->R;
			pix->G |= pix_inf->G;
			pix->B |= pix_inf->B;

			pix++;
			pix_inf++;

		}
	}

	return true;
}
//---------------------------------------------------------------------------
bool DIBInf::Plus(DIBInf& inf)
{
	if(pBmp->PixelFormat != inf.pBmp->PixelFormat)
	{
		return false;
	}

	if(pBmp->PixelFormat == pf24bit)
	{
		for(int y=0;y < pBmp->Height;y++)
		{
			for(int x=0;x < pBmp->Width;x++)
			{
				unsigned char gray = GetGray(x,y);

				PixRGB *pix     = this->_RGB(x,y);
				PixRGB *pix_inf = inf._RGB(x,y);

				pix->R += pix_inf->R;
				pix->G += pix_inf->G;
				pix->B += pix_inf->B;
			}
		}
	}
	else
	{
		for(int y=0;y < pBmp->Height;y++)
		{
			for(int x=0;x < pBmp->Width;x++)
			{
				unsigned char g1 = this->GetGray(x,y);
				unsigned char g2 = inf.GetGray(x,y);

				this->SetGray(x,y,g1+g2);
			}
		}
	}

	return true;
}
//---------------------------------------------------------------------------
bool DIBInf::Cut(unsigned char th)
{
	if(pBmp->PixelFormat == pf24bit)
	{
		for(int y=0;y < pBmp->Height;y++)
		{
			for(int x=0;x < pBmp->Width;x++)
			{
				unsigned char gray = GetGray(x,y);


				if(gray < th)
				{
					PixRGB *pix = this->_RGB(x,y);
					pix->R = 0;
					pix->G = 0;
					pix->B = 0;
				}
			}
		}
	}
	else
	{
		for(int y=0;y < pBmp->Height;y++)
		{
			for(int x=0;x < pBmp->Width;x++)
			{
				unsigned char gray = GetGray(x,y);


				if(gray < th)
				{
					this->SetGray(x,y,0);
				}
			}
		}
	}

	return true;
}
//---------------------------------------------------------------------------
bool DIBInf::White(int th)
{
	if(pBmp->PixelFormat == pf24bit)
	{
		for(int y=0;y < pBmp->Height;y++)
		{
			for(int x=0;x < pBmp->Width;x++)
			{
				unsigned char gray = this->GetGray(x,y);

				if(gray >= th)
				{
					this->SetGray(x,y,tbsFull);
				}
				else
				{
					this->SetGray(x,y,tbsNone);
				}

			}
		}
	}
	else
	{
		for(int y=0;y < pBmp->Height;y++)
		{
			for(int x=0;x < pBmp->Width;x++)
			{
				unsigned char gray = this->GetGray(x,y);

				if(gray >= th)
				{
					this->SetGray(x,y,tbsFull);
				}
			}
		}

	}

	return true;
}
//---------------------------------------------------------------------------
bool DIBInf::White2(DIBInf& BKGInf,int th,int fr)
{
	if(pBmp->PixelFormat == pf24bit)
	{
		for(int y=0;y < pBmp->Height;y++)
		{

			for(int x=0;x < pBmp->Width;x++)
			{
				int gray1 = this ->GetGray(x,y);
				int gray2 = BKGInf.GetGray(x,y);
				if(gray1 < th && gray2 < th)
				{
					this->SetGray(x,y,tbsNone);
					continue;
				}


				double dg = (1.0 * 256 * abs(gray1-gray2))/(gray1+gray2);

				if(dg >= th)
				{
					this->SetGray(x,y,tbsFull);
				}
				else
				{
					this->SetGray(x,y,tbsNone);
				}
			}
		}
	}
	else
	{
		for(int y=0;y < pBmp->Height;y++)
		{
			for(int x=0;x < pBmp->Width;x++)
			{
				unsigned char gray = this->GetGray(x,y);

				if(gray >= th)
				{
					this->SetGray(x,y,tbsFull);
				}
			}
		}

	}

	return true;
}
//---------------------------------------------------------------------------
bool DIBInf::Sub(DIBInf& inf)
{
	if(pBmp->PixelFormat != inf.pBmp->PixelFormat)
	{
		return false;
	}

	if(pBmp->PixelFormat == pf24bit)
	{
		for(int y=0;y < pBmp->Height;y++)
		{
			for(int x=0;x < pBmp->Width;x++)
			{
				PixRGB *pix     = this->_RGB(x,y);
				PixRGB *pix_inf = inf._RGB(x,y);

				pix->R = (pix->R > pix_inf->R?pix->R - pix_inf->R:pix_inf->R - pix->R);
				pix->G = (pix->G > pix_inf->G?pix->G - pix_inf->G:pix_inf->G - pix->G);
				pix->B = (pix->B > pix_inf->B?pix->B - pix_inf->B:pix_inf->B - pix->B);
			}
		}
	}
	else
	{
		for(int y=0;y < pBmp->Height;y++)
		{
			for(int x=0;x < pBmp->Width;x++)
			{
				unsigned char pix     = this->GetGray(x,y);
				unsigned char pix_inf = inf.GetGray(x,y);

				unsigned char gr = (pix > pix_inf?pix - pix_inf:pix_inf - pix);

				this->SetGray(x,y,gr);
			}
		}

	}
	return true;
}
//---------------------------------------------------------------------------
bool DIBInf::And(DIBInf& inf)
{
	if(pBmp->PixelFormat != inf.pBmp->PixelFormat)
	{
		return false;
	}

	if(pBmp->PixelFormat == pf24bit)
	{
		for(int y=0;y < pBmp->Height;y++)
		{
			for(int x=0;x < pBmp->Width;x++)
			{
				PixRGB *pix     = this->_RGB(x,y);
				PixRGB *pix_inf = inf._RGB(x,y);

				pix->R = (pix->R & pix_inf->R);
				pix->G = (pix->G & pix_inf->G);
				pix->B = (pix->B & pix_inf->B);
			}
		}
	}
	else
	{
		for(int y=0;y < pBmp->Height;y++)
		{
			for(int x=0;x < pBmp->Width;x++)
			{
				unsigned char pix     = this->GetGray(x,y);
				unsigned char pix_inf = inf.GetGray(x,y);

				unsigned char gr = (pix & pix_inf);

				this->SetGray(x,y,gr);
			}
		}
	}
	return true;
}

//---------------------------------------------------------------------------
PixRGB* DIBInf::_RGB(int x,int y)
{
	int shift = (imginsz - (pBmp->Width * pBmp->Height * ByteOfPixel))/pBmp->Height;
	int ry    = pBmp->Height - y -1;
	int rx    = pBmp->Width;
	int idx   = ByteOfPixel*pBmp->Width*ry + ry*shift + ByteOfPixel*x;


	PixRGB *pu = reinterpret_cast<PixRGB *>(&pImg[idx]);

	return pu;

}

unsigned char& DIBInf::R(int x,int y)
{
	if(ChechPos(x,y) == false)
	{
		throw std::out_of_range("Illegal coordinate specification");
	}

	PixRGB *pix = _RGB(x,y);

	return pix->R;
}
unsigned char& DIBInf::G(int x,int y)
{
	if(ChechPos(x,y) == false)
	{
		throw std::out_of_range("Illegal coordinate specification");
	}
	PixRGB *pix = _RGB(x,y);

	return pix->G;
}
unsigned char& DIBInf::B(int x,int y)
{
	if(ChechPos(x,y) == false)
	{
		throw std::out_of_range("Illegal coordinate specification");
	}
	PixRGB *pix = _RGB(x,y);

	return pix->B;
}
//-------------------------------------------------------------
bool DIBInf::DeleteNoise(int bw,int bh,double border)
{
	int blkh;
	int blkw;
	int BmpWidth  = GetWidth();
	int BmpHeight = GetHeight();

	PixRGB BLACK = {0};

	for(int y=0; y<BmpHeight;y += bh)
	{
		blkh = BmpHeight - y < bh?BmpHeight - y:bh;

		for(int x=0;x < BmpWidth;x += bw)
		{
			blkw = BmpWidth - x < bw?BmpWidth - x:bw;

			if(blkw == 0 || blkh == 0)
			{
				continue;
			}

			int WhiteNum = 0;

			for(int by = 0;by < blkh;by++)
			{
				for(int bx = 0;bx < blkw;bx++)
				{
					PixRGB *pix = _RGB(x+bx,y+by);

					if(pix->R == tbsFull)
					{
						WhiteNum++;
					}
				}
			}

			double wratio = static_cast<double>(WhiteNum) / static_cast<double>(blkw*blkh);

			if(wratio < border)
			{
				for(int by = 0;by < blkh;by++)
				{
					for(int bx = 0;bx < blkw;bx++)
					{
						PixRGB *pix = _RGB(x+bx,y+by);

						*pix = BLACK;
					}
				}
			}
		}
	}

	return true;
}
//-------------------------------------------------------------
bool DIBInf::CutOutRectImage(DIBInf& SrcInf,int left,int top,int right,int bottom)
{

	ConvToDIB(right  - left,bottom - top,pf24bit);

	for(int by = 0;by < pBmp->Height;by++)
	{
		for(int bx = 0;bx < pBmp->Width;bx++)
		{
			PixRGB *SrcPix = SrcInf._RGB(left + bx,top + by);
			PixRGB *DstPix = this->_RGB(bx,by);

			*DstPix = *SrcPix;
		}
	}

	ConvDibToBitmap();

	return true;

}
//-------------------------------------------------------------
bool DIBInf::PasteRectImage(DIBInf& src,int left,int top)
{
	int w = src.GetWidth();
	int h = src.GetHeight();

	for(int by = 0;by < h;by++)
	{
		for(int bx = 0;bx < w;bx++)
		{
			PixRGB *SrcPix = src._RGB(bx,by);
			PixRGB *DstPix = this->_RGB(left+bx,top+by);

			*DstPix = *SrcPix;
		}
	}

	ConvDibToBitmap();

	return true;
}
//-------------------------------------------------------------
bool DIBInf::Image_dilation()
{
	static const int dxy[5][2] = {{0,0},{0,-1},{0,1},{-1,0},{1,0}};

	DIBInf RefInf(*this);


	int iw = GetWidth();
	int ih = GetHeight();

	for(int iy=1; iy<ih-1; iy++)
	{
		for(int ix=1; ix<iw-1; ix++)
		{
			unsigned char c = tbsNone;

			for(int d=0;d < 5;d++)
			{
				if(RefInf.GetGray(ix+dxy[d][0],iy+dxy[d][1]) != tbsNone)
				{
					c = tbsFull;
					break;
				}
			}
			SetGray(ix,iy,c);
		}
	}
	return true;
}

bool DIBInf::Image_dilation(int l,int t,int r,int b)
{
	static const int dxy[5][2] = {{0,0},{0,-1},{0,1},{-1,0},{1,0}};

	DIBInf RefInf(*this);

	for(int iy=t+1; iy<b-1; iy++)
	{
		for(int ix=l+1; ix<r-1; ix++)
		{
			unsigned char c = tbsNone;

			for(int d=0;d < 5;d++)
			{
				if(RefInf.GetGray(ix+dxy[d][0],iy+dxy[d][1]) != tbsNone)
				{
					c = tbsFull;
					break;
				}
			}
			SetGray(ix,iy,c);
		}
	}
	return true;
}

//-------------------------------------------------------------
bool DIBInf::Image_erosion()
{
	static const int dxy[5][2] = {{0,0},{0,-1},{0,1},{-1,0},{1,0}};

	DIBInf RefInf(*this);

	int iw = GetWidth();
	int ih = GetHeight();


	for(int iy=1; iy<ih-1; iy++)
	{
		for(int ix=1; ix<iw-1; ix++)
		{
			unsigned char c = tbsFull;

			for(int d=0;d < 5;d++)
			{
				if(RefInf.GetGray(ix+dxy[d][0],iy+dxy[d][1]) == tbsNone)
				{
					c = tbsNone;
					break;
				}
			}
			SetGray(ix,iy,c);
		}
	}

	return true;
}

bool DIBInf::Image_erosion(int l,int t,int r,int b)
{
	static const int dxy[5][2] = {{0,0},{0,-1},{0,1},{-1,0},{1,0}};

	DIBInf RefInf(*this);

	for(int iy=t+1; iy<b-1; iy++)
	{
		for(int ix=l+1; ix<r-1; ix++)
		{
			unsigned char c = tbsFull;

			for(int d=0;d < 5;d++)
			{
				if(RefInf.GetGray(ix+dxy[d][0],iy+dxy[d][1]) == tbsNone)
				{
					c = tbsNone;
					break;
				}
			}
			SetGray(ix,iy,c);
		}
	}

	return true;
}

//-------------------------------------------------------------
bool DIBInf::Closing(int cn)
{
	for(int num=0;num < cn;num++)
	{
		Image_dilation();
	}
	for(int num=0;num < cn;num++)
	{
		Image_erosion();
	}

	return true;
}

bool DIBInf::Closing(int cn,int l,int t,int r,int b)
{
	for(int num=0;num < cn;num++)
	{
		Image_dilation(l,t,r,b);
	}
	for(int num=0;num < cn;num++)
	{
		Image_erosion(l,t,r,b);
	}

	return true;
}

//-------------------------------------------------------------
bool DIBInf::FillRect(const Gdiplus::Rect& R,const PixRGB& col)
{
	for(int wy = R.GetTop();wy <= R.GetBottom();wy++)
	{
		for(int wx = R.GetLeft();wx <= R.GetRight();wx++)
		{
			PixRGB *pix = _RGB(wx,wy);

			*pix = col;
		}
	}

	return true;
}
