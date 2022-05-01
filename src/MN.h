//---------------------------------------------------------------------------
#ifndef MNH
#define MNH
//---------------------------------------------------------------------------
#include "DIBInf.h"

//�w�K�f�[�^(NxN)�e���v���[�g�N���X
template<int N> class MN
{
protected:
	double mdata[N][N];
	//R,G,B
	double R_data[N][N];
	double G_data[N][N];
	double B_data[N][N];
	//�z�񉻂��ꂽ��
	bool IsArrayed;
	//�d�ˍ��킹��
	int SuperpositionNum;
protected:
	//�r�b�g�}�b�v��NxN�ɂ��ĕԂ�
	bool CreateNBitmapFromBitmap(DIBInf& NewInf,DIBInf& SrcInf)
	{
		TBitmap *pSrc  = SrcInf.GetBitmap();
		TBitmap *pDest = NewInf.GetBitmap();

		//�傫��
		int w = pSrc->Width;
		int h = pSrc->Height;
		//�V�����傫��
		int nw;
		int nh;
		if(w == h)
		{
			nw = N;
			nh = N;
		}
		else if(w > h)
		{
			nw = N;
			nh = static_cast<double>(h) * static_cast<double>(nw)/static_cast<double>(w);
		}
		else
		{
			nh = N;
			nw = static_cast<double>(w) * static_cast<double>(nh)/static_cast<double>(h);
		}
		//�V�����傫���ɂ���
		pDest->Width  = N;
		pDest->Height = N;
		pDest->PixelFormat = pf24bit;

		int dx = (N-nw)/2;
		int dy = (N-nh)/2;

		//���œh��Ԃ�
		pDest->Canvas->Brush->Color = clBlack;
		pDest->Canvas->Brush->Style = bsSolid;
		pDest->Canvas->FillRect(TRect(0,0,pDest->Width,pDest->Height));


		//������傫���ύX�ŃR�s�[
		BOOL res = StretchBlt(
		  pDest->Canvas->Handle,               //[in] HDC   hdcDest,
		  dx,                                  //[in] int   xDest,
		  dy,                                  //[in] int   yDest,
		  nw,                                  //[in] int   wDest,
		  nh,                                  //[in] int   hDest,
		  pSrc->Canvas->Handle,                //[in] HDC   hdcSrc,
		  0,                                   //[in] int   xSrc,
		  0,                                   //[in] int   ySrc,
		  w,                                   //[in] int   wSrc,
		  h,                                   //[in] int   hSrc,
		  SRCCOPY                              //[in] DWORD rop
		);
		//�`�F�b�N
		if(res == 0)
		{
			return false;
		}

		//DIB�ɂ���
		NewInf.ConvToDIB();

		return (res != 0);
	}
protected:
	//�R�s�[
	virtual void _copy(const MN<N>& h)
	{
		IsArrayed = h.IsArrayed;
		SuperpositionNum = h.SuperpositionNum;
		memcpy(mdata,h.mdata,sizeof(mdata));
		//R,G,B
		memcpy(R_data,h.R_data,sizeof(R_data));
		memcpy(G_data,h.G_data,sizeof(G_data));
		memcpy(B_data,h.B_data,sizeof(B_data));
	}
	//�r�b�g�}�b�v��64x64�z��Ɋi�[
	virtual bool Set64ArrayFromBitmap(double arr[N][N],DIBInf& SrcInf,bool isForceUpdate = false)
	{
		DIBInf NewInf;

		//�r�b�g�}�b�v(DIBInf)��NxN�ɂ��ĕԂ�
		if(CreateNBitmapFromBitmap(NewInf,SrcInf) == false)
		{
			return false;
		}

		//�z��ɒl���Z�b�g
		for(int y=0;y < N;y++)
		{
			for(int x=0;x < N;x++)
			{
				PixRGB *pix = NewInf._RGB(x,y);

				R_data[y][x] = pix->R;
				G_data[y][x] = pix->G;
				B_data[y][x] = pix->B;

				double v = NewInf.GetGray(x,y);

				arr[y][x] = (NewInf.GetGray(x,y) < 0.1f?0:1);
			}
		}
		//�z�񉻂��ꂽ
		IsArrayed = true;
		//�d�ˍ��킹�񐔍X�V
		SuperpositionNum = 1;

		return true;
	}
public:
	MN<N>()
	{
		IsArrayed = false;
		memset(mdata,0,sizeof(mdata));
		memset(R_data,0,sizeof(R_data));
		memset(G_data,0,sizeof(G_data));
		memset(B_data,0,sizeof(B_data));
		SuperpositionNum = 0;
	}
	MN<N>(const MN<N>& him)
	{
		_copy(him);
	}
public:
	int get(int row,int col)
	{
		//���E�`�F�b�N
		if(row < 0 || row > N-1 || col < 0 || col > N-1)
		{
			return -1;
		}
		//�擾����r�b�g�l
		unsigned char bit;
		//row,col�̒l�����o�C�g�𓾂�
		unsigned char byte = mdata[row][col];

		return byte;
	}
	bool set(int row,int col,int val)
	{
		//���E�`�F�b�N
		if(row < 0 || row > N-1 || col < 0 || col > N-1)
		{
			return false;
		}
		//�ݒ肵���o�C�g���Z�b�g
		mdata[row][col] = val;

		return true;
	}
	//�d�ˍ��킹�񐔂𓾂�
	int GetSuperpositionNumber()
	{
		return SuperpositionNum;
	}
	//R�𓾂�
	unsigned char R(int x,int y)
	{
		int r = R_data[y][x] / static_cast<double>(SuperpositionNum);

		return r;
	}
	//G�𓾂�
	unsigned char G(int x,int y)
	{
		int g = G_data[y][x] / static_cast<double>(SuperpositionNum);

		return g;
	}
	//B�𓾂�
	unsigned char B(int x,int y)
	{
		int b = B_data[y][x] / static_cast<double>(SuperpositionNum);

		return b;
	}
public:
	//���
	virtual MN<N>& operator = (const MN<N>& h)
	{
		_copy(h);
		return *this;
	}
	//���Z(�d�ˍ��킹)����
	virtual MN<N>& operator += (const MN<N>& h)
	{
		//�������킹��
		for(int r = 0;r < N;r++)
		{
			for(int c = 0;c < N;c++)
			{
				if(h.mdata[r][c] > 0.1)
				{
					mdata[r][c]  += h.mdata[r][c];
					R_data[r][c] += h.R_data[r][c];
					G_data[r][c] += h.G_data[r][c];
					B_data[r][c] += h.G_data[r][c];
				}
				else
				{
					mdata[r][c] += -1;
				}
			}
		}
		//�d�ˍ��킹�񐔍X�V
		SuperpositionNum++;

		return *this;
	}
	//�����Z����
	virtual MN<N> operator - (const MN<N>& h)
	{
		MN<N> Res;

		//�������킹��
		for(int r = 0;r < N;r++)
		{
			for(int c = 0;c < N;c++)
			{
				Res.mdata[r][c]  = mdata[r][c]  - h.mdata[r][c];
				Res.R_data[r][c] = R_data[r][c] - h.R_data[r][c];
				Res.G_data[r][c] = G_data[r][c] - h.G_data[r][c];
				Res.B_data[r][c] = B_data[r][c] - h.B_data[r][c];
			}
		}
		//�d�ˍ��킹�񐔍X�V
		Res.SuperpositionNum = 1;

		return Res;
	}
	//���l����(RGB)
	virtual bool operator == (const MN<N>& h)
	{
		//���l���`�F�b�N����
		for(int r = 0;r < N;r++)
		{
			for(int c = 0;c < N;c++)
			{
				bool different = false;

				different  = (R_data[r][c] != h.R_data[r][c]);
				different |= (G_data[r][c] != h.G_data[r][c]);
				different |= (B_data[r][c] != h.B_data[r][c]);

				if(different == true)
				{
					return false;
				}
			}
		}
		return true;
	}
	//���l����(RGB:�������l����)
	virtual bool IsSame (const MN<N>& h,int th)
	{
		if(th < 0)
		{
			return false;
		}
		//�Ⴂ�̐�
		int different = 0;
		//���l���`�F�b�N����
		for(int r = 0;r < N;r++)
		{
			for(int c = 0;c < N;c++)
			{
				int r0 = R_data[r][c];
				int g0 = G_data[r][c];
				int b0 = B_data[r][c];

				int r1 = h.R_data[r][c];
				int g1 = h.G_data[r][c];
				int b1 = h.B_data[r][c];

				different += abs(r0-r1);
				different += abs(g0-g1);
				different += abs(b0-b1);

				if(different > th)
				{
					return false;
				}
			}
		}
		return true;
	}
public:
	//���K������
	virtual bool norm()
	{
		double sum   = 0;
		double coeff = 0; //�W��
		//���v�l���o��
		for(int r = 0;r < N;r++)
		{
			for(int c = 0;c < N;c++)
			{
				double v = fabs(mdata[r][c]);

				sum += v*v;
			}
		}
		if(sum < 0.00001)
		{
			return true;
		}
		//�W��
		coeff = 1.0 / sum;
		//�W���������Đ��K��
		for(int r = 0;r < N;r++)
		{
			for(int c = 0;c < N;c++)
			{
				double v = mdata[r][c];

				if(v>= 0.0f)
				{
					mdata[r][c] = (coeff * v * v);
				}
				else
				{
					mdata[r][c] = -(coeff * v * v);
				}

			}
		}
		return true;
	}
	//����MN�Ɣz��̒l����v����|�C���g�𓾂�
	virtual double GetMatchPoint(MN<N>& inf)
	{
		double match_point = 0;
		//�|�C���g�𓾂�
		for(int y=0;y < N;y++)
		{
			for(int x=0;x < N;x++)
			{
				double v  = mdata[y][x];
				double iv = inf.mdata[y][x];

				if((v >= 0.0f && iv > 0.0001f) || (v < 0.0f && iv < 0.0001f))
				{
					match_point += fabs(v);
				}
				else
				{
					match_point -= fabs(v);
				}
			}
		}
		return match_point;
	}
	//0�Ŕz�񉻂���
	virtual bool SetZeroArray()
	{
		memset(mdata,0,sizeof(mdata));
		IsArrayed=true;
		return true;
	}
	//DIBInf����Z�b�g
	virtual bool SetFormDIBInf(DIBInf& inf)
	{
		//�r�b�g�}�b�v��NxN�z��Ɋi�[
		bool res = Set64ArrayFromBitmap(mdata,inf);

		return res;
	}
	//DIBInf����Z�b�g
	virtual bool SetFormDIBInf(DIBInf& Inf,DIBInf& MaskInf)
	{
		//�r�b�g�}�b�v��NxN�z��Ɋi�[
		if(Set64ArrayFromBitmap(mdata,Inf) == false)
		{
			return false;
		}
		//���ς𓾂�
		nsVector ave = CalcAverageWithoutZero();
		//�}�X�N��K�p����
		for(int r = 0;r < N;r++)
		{
			for(int c = 0;c < N;c++)
			{
				if(MaskInf.GetGray(c,r) == tbsFull)
				{
					R_data[r][c] = ave[0];
					G_data[r][c] = ave[1];
					B_data[r][c] = ave[2];
				}
			}
		}
		return true;
	}
	//DIBInf����Z�b�g
	virtual bool SetFormDIBInf(DIBInf& Inf,int x,int y)
	{
		int w = Inf.GetWidth();
		int h = Inf.GetHeight();
		//x,y�͈̔�
		int XMax = (x + N - 1 >= w?w-1:x + N - 1);
		int YMax = (y + N - 1 >= h?h-1:y + N - 1);

		int RMax = YMax - y + 1;
		int CMax = XMax - x + 1;
		//������
		memset(&R_data[0][0],0,sizeof(R_data));
		memset(&G_data[0][0],0,sizeof(G_data));
		memset(&B_data[0][0],0,sizeof(B_data));


		//�l�Z�b�g
		for(int r = 0;r < RMax;r++)
		{
			int ry = y + r;

			for(int c=0;c < CMax;c++)
			{
				int rx = x + c;

				PixRGB *pix = Inf._RGB(rx,ry);

				R_data[r][c] = pix->R;
				G_data[r][c] = pix->G;
				B_data[r][c] = pix->B;
			}
		}
		SuperpositionNum = 1;

		return true;
	}
public:
	//---------------------------------------------------------------------------
	//0�l�ȊO�̕��ϒl�����߂�
	//---------------------------------------------------------------------------
	nsVector CalcAverageWithoutZero()
	{
		nsVector sum;
		int      ElNum = 0;
		double   vR;
		double   vG;
		double   vB;
		nsVector rgb;

		//0�l�ȊO�̕��ϒl�����߂�
		for(int y=0;y < N;y++)
		{
			for(int x=0;x < N;x++)
			{
				rgb.set(R_data[y][x],G_data[y][x],B_data[y][x]);

				if(rgb.leng() < fabs(0.0001f))
				{
					continue;
				}
				//���v�l
				sum += rgb;
				//�v�f��
				ElNum++;
			}
		}
		//0�Ŋ���̂����
		if(ElNum == 0)
		{
			return nsVector(0.0,0.0,0.0);
		}
		//����
		nsVector ave = sum / static_cast<double>(ElNum*SuperpositionNum);

		return ave;
	}
	//---------------------------------------------------------------------------
	//���ϒl�����߂�
	//---------------------------------------------------------------------------
	nsVector CalcAverage()
	{
		nsVector sum;
		double   vR;
		double   vG;
		double   vB;
		nsVector rgb;

		//0�l�ȊO�̕��ϒl�����߂�
		for(int y=0;y < N;y++)
		{
			for(int x=0;x < N;x++)
			{
				rgb.set(R_data[y][x],G_data[y][x],B_data[y][x]);

				//���v�l
				sum += rgb;
			}
		}
		//����
		nsVector ave = sum / static_cast<double>(N*N*SuperpositionNum);

		return ave;
	}
	//---------------------------------------------------------------------------
	//0�l�ȊO��RGB�W���΍������߂�(RGB���ϒl�g�p)
	//---------------------------------------------------------------------------
	nsVector CalcStdDevWithoutZero(nsVector& ave)
	{
		int      ElNum = 0;
		nsVector sum2;
		nsVector rgb;
		nsVector df;

		//0�l�ȊO�̕��ϒl�Ƃ̍��̂Q��̍��v�����߂�
		for(int y=0;y < N;y++)
		{
			for(int x=0;x < N;x++)
			{
				rgb.set(R_data[y][x],G_data[y][x],B_data[y][x]);
				rgb /= static_cast<double>(SuperpositionNum);

				if(rgb.leng() < fabs(0.0001f))
				{
					continue;
				}
				//���v�l
				df    = rgb - ave;
				sum2 += df.comp2();
				//�v�f��
				ElNum++;
			}
		}
		//0�Ŋ���̂����
		if(ElNum == 0)
		{
			return nsVector(0.0,0.0,0.0);
		}
		//���Us^2
		nsVector s2 =  sum2 / static_cast<double>(ElNum);
		//�W���΍�(standard deviation, SD)
		nsVector SD = s2.route();

		return SD;
	}
	//---------------------------------------------------------------------------
	//RGB�W���΍������߂�(RGB���ϒl�g�p)
	//---------------------------------------------------------------------------
	nsVector CalcStdDev(nsVector& ave)
	{
		nsVector sum2;
		nsVector rgb;
		nsVector df;

		//0�l�ȊO�̕��ϒl�Ƃ̍��̂Q��̍��v�����߂�
		for(int y=0;y < N;y++)
		{
			for(int x=0;x < N;x++)
			{
				rgb.set(R_data[y][x],G_data[y][x],B_data[y][x]);
				rgb /= static_cast<double>(SuperpositionNum);

				//���v�l
				df    = rgb - ave;
				sum2 += df.comp2();
			}
		}
		//���Us^2
		nsVector s2 =  sum2 / static_cast<double>(N*N);
		//�W���΍�(standard deviation, SD)
		nsVector SD = s2.route();

		return SD;
	}
	//����DIBInf�Ɣz��̒l����v����|�C���g�𓾂�
	double GetMatchPoint(DIBInf& inf)
	{
		double match_point = 0;
		MN<N>  infN;

		//(�Q�l��)DIBInf����Z�b�g
		infN.SetFormDIBInf(inf);
		//�|�C���g�𓾂�
		double pt = GetMatchPoint(infN);

		return pt;
	}
	//����DIBInf�Ɣz��̒l����v���銄��(%)�𓾂�
	double GetMatchRatio(DIBInf& inf)
	{
		//����DIBInf�Ɣz��̒l����v������𓾂�
		double match_point = GetMatchPoint(inf);

		double point = 100.0 * match_point;

		return point;
	}
	//����MN�Ɣz��̒l����v���銄��(%)�𓾂�
	double GetMatchRatio(MN<N>& inf)
	{
		//�|�C���g�𓾂�
		double match_point = GetMatchPoint(inf);
		double point       = 100.0 * match_point;

		return point;
	}
};

#endif
