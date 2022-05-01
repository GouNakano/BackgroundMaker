//---------------------------------------------------------------------------
#ifndef MNH
#define MNH
//---------------------------------------------------------------------------
#include "DIBInf.h"

//学習データ(NxN)テンプレートクラス
template<int N> class MN
{
protected:
	double mdata[N][N];
	//R,G,B
	double R_data[N][N];
	double G_data[N][N];
	double B_data[N][N];
	//配列化されたか
	bool IsArrayed;
	//重ね合わせ回数
	int SuperpositionNum;
protected:
	//ビットマップをNxNにして返す
	bool CreateNBitmapFromBitmap(DIBInf& NewInf,DIBInf& SrcInf)
	{
		TBitmap *pSrc  = SrcInf.GetBitmap();
		TBitmap *pDest = NewInf.GetBitmap();

		//大きさ
		int w = pSrc->Width;
		int h = pSrc->Height;
		//新しい大きさ
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
		//新しい大きさにする
		pDest->Width  = N;
		pDest->Height = N;
		pDest->PixelFormat = pf24bit;

		int dx = (N-nw)/2;
		int dy = (N-nh)/2;

		//黒で塗りつぶす
		pDest->Canvas->Brush->Color = clBlack;
		pDest->Canvas->Brush->Style = bsSolid;
		pDest->Canvas->FillRect(TRect(0,0,pDest->Width,pDest->Height));


		//元から大きさ変更でコピー
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
		//チェック
		if(res == 0)
		{
			return false;
		}

		//DIBにする
		NewInf.ConvToDIB();

		return (res != 0);
	}
protected:
	//コピー
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
	//ビットマップを64x64配列に格納
	virtual bool Set64ArrayFromBitmap(double arr[N][N],DIBInf& SrcInf,bool isForceUpdate = false)
	{
		DIBInf NewInf;

		//ビットマップ(DIBInf)をNxNにして返す
		if(CreateNBitmapFromBitmap(NewInf,SrcInf) == false)
		{
			return false;
		}

		//配列に値をセット
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
		//配列化された
		IsArrayed = true;
		//重ね合わせ回数更新
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
		//境界チェック
		if(row < 0 || row > N-1 || col < 0 || col > N-1)
		{
			return -1;
		}
		//取得するビット値
		unsigned char bit;
		//row,colの値を持つバイトを得る
		unsigned char byte = mdata[row][col];

		return byte;
	}
	bool set(int row,int col,int val)
	{
		//境界チェック
		if(row < 0 || row > N-1 || col < 0 || col > N-1)
		{
			return false;
		}
		//設定したバイトをセット
		mdata[row][col] = val;

		return true;
	}
	//重ね合わせ回数を得る
	int GetSuperpositionNumber()
	{
		return SuperpositionNum;
	}
	//Rを得る
	unsigned char R(int x,int y)
	{
		int r = R_data[y][x] / static_cast<double>(SuperpositionNum);

		return r;
	}
	//Gを得る
	unsigned char G(int x,int y)
	{
		int g = G_data[y][x] / static_cast<double>(SuperpositionNum);

		return g;
	}
	//Bを得る
	unsigned char B(int x,int y)
	{
		int b = B_data[y][x] / static_cast<double>(SuperpositionNum);

		return b;
	}
public:
	//代入
	virtual MN<N>& operator = (const MN<N>& h)
	{
		_copy(h);
		return *this;
	}
	//加算(重ね合わせ)する
	virtual MN<N>& operator += (const MN<N>& h)
	{
		//足し合わせる
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
		//重ね合わせ回数更新
		SuperpositionNum++;

		return *this;
	}
	//引き算する
	virtual MN<N> operator - (const MN<N>& h)
	{
		MN<N> Res;

		//足し合わせる
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
		//重ね合わせ回数更新
		Res.SuperpositionNum = 1;

		return Res;
	}
	//同値判定(RGB)
	virtual bool operator == (const MN<N>& h)
	{
		//同値かチェックする
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
	//同値判定(RGB:しきい値あり)
	virtual bool IsSame (const MN<N>& h,int th)
	{
		if(th < 0)
		{
			return false;
		}
		//違いの数
		int different = 0;
		//同値かチェックする
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
	//正規化する
	virtual bool norm()
	{
		double sum   = 0;
		double coeff = 0; //係数
		//合計値を出す
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
		//係数
		coeff = 1.0 / sum;
		//係数をかけて正規化
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
	//入力MNと配列の値が一致するポイントを得る
	virtual double GetMatchPoint(MN<N>& inf)
	{
		double match_point = 0;
		//ポイントを得る
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
	//0で配列化する
	virtual bool SetZeroArray()
	{
		memset(mdata,0,sizeof(mdata));
		IsArrayed=true;
		return true;
	}
	//DIBInfからセット
	virtual bool SetFormDIBInf(DIBInf& inf)
	{
		//ビットマップをNxN配列に格納
		bool res = Set64ArrayFromBitmap(mdata,inf);

		return res;
	}
	//DIBInfからセット
	virtual bool SetFormDIBInf(DIBInf& Inf,DIBInf& MaskInf)
	{
		//ビットマップをNxN配列に格納
		if(Set64ArrayFromBitmap(mdata,Inf) == false)
		{
			return false;
		}
		//平均を得る
		nsVector ave = CalcAverageWithoutZero();
		//マスクを適用する
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
	//DIBInfからセット
	virtual bool SetFormDIBInf(DIBInf& Inf,int x,int y)
	{
		int w = Inf.GetWidth();
		int h = Inf.GetHeight();
		//x,yの範囲
		int XMax = (x + N - 1 >= w?w-1:x + N - 1);
		int YMax = (y + N - 1 >= h?h-1:y + N - 1);

		int RMax = YMax - y + 1;
		int CMax = XMax - x + 1;
		//初期化
		memset(&R_data[0][0],0,sizeof(R_data));
		memset(&G_data[0][0],0,sizeof(G_data));
		memset(&B_data[0][0],0,sizeof(B_data));


		//値セット
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
	//0値以外の平均値を求める
	//---------------------------------------------------------------------------
	nsVector CalcAverageWithoutZero()
	{
		nsVector sum;
		int      ElNum = 0;
		double   vR;
		double   vG;
		double   vB;
		nsVector rgb;

		//0値以外の平均値を求める
		for(int y=0;y < N;y++)
		{
			for(int x=0;x < N;x++)
			{
				rgb.set(R_data[y][x],G_data[y][x],B_data[y][x]);

				if(rgb.leng() < fabs(0.0001f))
				{
					continue;
				}
				//合計値
				sum += rgb;
				//要素数
				ElNum++;
			}
		}
		//0で割るのを回避
		if(ElNum == 0)
		{
			return nsVector(0.0,0.0,0.0);
		}
		//平均
		nsVector ave = sum / static_cast<double>(ElNum*SuperpositionNum);

		return ave;
	}
	//---------------------------------------------------------------------------
	//平均値を求める
	//---------------------------------------------------------------------------
	nsVector CalcAverage()
	{
		nsVector sum;
		double   vR;
		double   vG;
		double   vB;
		nsVector rgb;

		//0値以外の平均値を求める
		for(int y=0;y < N;y++)
		{
			for(int x=0;x < N;x++)
			{
				rgb.set(R_data[y][x],G_data[y][x],B_data[y][x]);

				//合計値
				sum += rgb;
			}
		}
		//平均
		nsVector ave = sum / static_cast<double>(N*N*SuperpositionNum);

		return ave;
	}
	//---------------------------------------------------------------------------
	//0値以外のRGB標準偏差を求める(RGB平均値使用)
	//---------------------------------------------------------------------------
	nsVector CalcStdDevWithoutZero(nsVector& ave)
	{
		int      ElNum = 0;
		nsVector sum2;
		nsVector rgb;
		nsVector df;

		//0値以外の平均値との差の２乗の合計を求める
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
				//合計値
				df    = rgb - ave;
				sum2 += df.comp2();
				//要素数
				ElNum++;
			}
		}
		//0で割るのを回避
		if(ElNum == 0)
		{
			return nsVector(0.0,0.0,0.0);
		}
		//分散s^2
		nsVector s2 =  sum2 / static_cast<double>(ElNum);
		//標準偏差(standard deviation, SD)
		nsVector SD = s2.route();

		return SD;
	}
	//---------------------------------------------------------------------------
	//RGB標準偏差を求める(RGB平均値使用)
	//---------------------------------------------------------------------------
	nsVector CalcStdDev(nsVector& ave)
	{
		nsVector sum2;
		nsVector rgb;
		nsVector df;

		//0値以外の平均値との差の２乗の合計を求める
		for(int y=0;y < N;y++)
		{
			for(int x=0;x < N;x++)
			{
				rgb.set(R_data[y][x],G_data[y][x],B_data[y][x]);
				rgb /= static_cast<double>(SuperpositionNum);

				//合計値
				df    = rgb - ave;
				sum2 += df.comp2();
			}
		}
		//分散s^2
		nsVector s2 =  sum2 / static_cast<double>(N*N);
		//標準偏差(standard deviation, SD)
		nsVector SD = s2.route();

		return SD;
	}
	//入力DIBInfと配列の値が一致するポイントを得る
	double GetMatchPoint(DIBInf& inf)
	{
		double match_point = 0;
		MN<N>  infN;

		//(２値化)DIBInfからセット
		infN.SetFormDIBInf(inf);
		//ポイントを得る
		double pt = GetMatchPoint(infN);

		return pt;
	}
	//入力DIBInfと配列の値が一致する割合(%)を得る
	double GetMatchRatio(DIBInf& inf)
	{
		//入力DIBInfと配列の値が一致する個数を得る
		double match_point = GetMatchPoint(inf);

		double point = 100.0 * match_point;

		return point;
	}
	//入力MNと配列の値が一致する割合(%)を得る
	double GetMatchRatio(MN<N>& inf)
	{
		//ポイントを得る
		double match_point = GetMatchPoint(inf);
		double point       = 100.0 * match_point;

		return point;
	}
};

#endif
