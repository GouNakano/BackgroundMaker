#ifndef nsListH
#define nsListH

#include "string.h"

template <class T> class nsList
{
private:
	const int    AddingNum;
	int          Count;
	int          Capacity;
	T          **Item;
	bool         DelItemClear;
protected:
	bool         Sorted;
private:
	void         AddCapacity();
	bool         CheckIndex(int index)                      const;
	void         QuickSort(int BaseIndex,int  IndexNum);
	int          ListCompare(const  T* Me,const T *Him);
	bool         BinarySearch(const T* Key,int& first,int& last);
	void         HeapSort();
public:
	 nsList();
	~nsList();
public:
	T          *GetItem    (int index)                        const;
	T&          GetItemRef (int index)                        const;
	int         Add        (T *,bool SortFlgUpdate = true);
	bool        Delete     (int index,bool pack=true);
	bool        Delete     (const T *Item);
	int         Insert     (int index,T *item,bool SortFlgUpdate = true);
	void        Clear      ();
	bool        Search     (const T& Key,int &first,int& last);
	T*          Search     (const T& Key);
	int         IndexOf    (const T& Key,int Start = 0);
	int         IndexOf    (const T* Key,int Start = 0);
	void        CopyList   (nsList<T>& List)const;
	bool        SwapItem   (int index1,int index2);
	void        Sort       ();
	void        Pack       ();
	int         SortingAdd (T *item);
	void        SetAllowDeleteItem(bool dflg);
public:
	int         GetCount()                                   const;
public:
	T&          operator[](int index)                        const;
public:
	template <class TX> nsList<T>& operator = (nsList<TX>& List)                                  //éQè∆ÿΩƒÇçÏê¨Ç∑ÇÈ
	{
		Clear();
		for(int Cnt = 0,ItemNum = List.GetCount();Cnt < ItemNum;Cnt++)
		{
			Add((T*)List.GetItem(Cnt));
		}
		SetAllowDeleteItem(false);

		return *this;
	}
};


namespace nsListLib
{
	template <class T1,class T2> void CopyList(nsList<T2>& ToList/*∫Àﬂ∞êÊ*/,nsList<T1>& FromList /*∫Àﬂ∞å≥*/)
	{
		ToList.Clear();

		ToList.SetAllowDeleteItem(false);

		for(int Cnt = 0;Cnt < FromList.GetCount();Cnt++)
		{

			T2 *Val = (T2 *)FromList.GetItem(Cnt);

			ToList.Add(Val);
		}
	}
}
//-------------------------------------------------------------
template <class T> nsList<T>::nsList():AddingNum(1024)
{
	Count         = 0;
	Capacity      = 0;
	Item          = 0;
	Sorted        = false;
	DelItemClear  = true;
}
//-------------------------------------------------------------
template <class T> nsList<T>::~nsList()
{
	//√∞ÃﬁŸÇÃäJï˙
	Clear();
	delete [] Item;
}
//-------------------------------------------------------------
template <class T> void nsList<T>::AddCapacity()
{
	T **NewItem;

	if(AddingNum == 0)
	{
		const_cast<int &>(AddingNum) = 1024;
	}

	NewItem = new T* [Capacity + AddingNum];

	memset(NewItem,0,sizeof(T*) * (Capacity + AddingNum));

	memcpy(NewItem,Item,sizeof(T*) * Capacity);

	delete [] Item;

	Item     = NewItem;
	Capacity = Capacity + AddingNum;
}
//-------------------------------------------------------------
template <class T> bool nsList<T>::CheckIndex(int index) const
{
	return (index >= 0 && index < Count);
}
//-------------------------------------------------------------
template <class T> T* nsList<T>::GetItem(int index) const
{
	if(CheckIndex(index) == false)return 0;

	return Item[index];
}
//-------------------------------------------------------------
template <class T> T& nsList<T>::operator[](int index) const
{
	return *GetItem(index);
}
//-------------------------------------------------------------
template <class T> T& nsList<T>::GetItemRef(int index)const
{
	return *GetItem(index);
}
//-------------------------------------------------------------
template <class T> int nsList<T>::Add(T* item,bool SortFlgUpdate)
{
	if(item == 0)return -1;

	if(Capacity < Count + 1)AddCapacity();

	Item[Count] = item;
	Count++;

	if(SortFlgUpdate == true)Sorted = false;

	return (Count - 1);
}
//-------------------------------------------------------------
template <class T> int nsList<T>::SortingAdd(T *item)
{
	int InsIndex;
	int first,last;

	if(Sorted == false)Sort();
	if(Count == 0)
	{
		return Add(item,false);
	}
	bool Res = Search(*item,first,last);
	if(Res == true)
		InsIndex = Insert(first,item,false);
	else
		InsIndex = Insert(last,item,false);

	return InsIndex;
}
//-------------------------------------------------------------
template <class T> bool nsList<T>::Delete(int index,bool pack)
{
	T* item;

	if(CheckIndex(index) == false)return false;

	item = Item[index];

	if(DelItemClear)
	{
		try{
			delete item;
		}catch(...){}
	}
	Item[index] = NULL;

	if(pack == true)
	{
		memmove(&Item[index],&Item[index+1],sizeof(T*) * (Count - index - 1));

		Item[Count-1] = 0;

		Count--;
	}
	else
	{
		Sorted      = false;
	}

	return true;
}
//-------------------------------------------------------------
template <class T> bool nsList<T>::Delete(const T *Item)
{

	int index = IndexOf(Item);

	if(index < 0)return false;
	bool Res = Delete(index);

	return Res;
}

//-------------------------------------------------------------
template <class T> int nsList<T>::Insert(int index,T* item,bool SortFlgUpdate)
{
	if(index == GetCount())
	{
		bool STmp = Sorted;
		bool Chk  = (Add(item) > -1);

		if(SortFlgUpdate == false)Sorted = STmp;

		return Chk;
	}

	if(CheckIndex(index) == false)return -1;

	if(Capacity < Count + 1)AddCapacity();

	memmove(&Item[index+1],&Item[index],sizeof(T*) * (Count - index));

	Item[index] = item;

	Count++;

	if(SortFlgUpdate == true)Sorted = false;

	return true;

}
//-------------------------------------------------------------
template <class T> void  nsList<T>::Clear()
{
	T        *item;
	int       index;

	for(index = 0;index < Count;index++)
	{
		item = Item[index];

		if(DelItemClear)
		{
			try{
				delete item;
			}catch(...){}
		}
		Item[index] = 0;
	}
	Count  = 0;
}
//-------------------------------------------------------------
template <class T> void nsList<T>::Pack()
{
	int ListNum = GetCount();
	int Cnt;
	int MovePtr;

	for(Cnt=0,MovePtr=0;Cnt < ListNum;Cnt++)
	{
		if(Item[Cnt] != NULL)
		{
			Item[MovePtr] = Item[Cnt];
			MovePtr++;
		}
	}

	Count = MovePtr;

	return;
}
//-------------------------------------------------------------
template <class T> int nsList<T>::GetCount() const
{
	return Count;
}
//-------------------------------------------------------------
template <class T> bool nsList<T>::Search(const T& Key,int &first,int& last)
{

	if(GetCount() == 0)return false;

	if(Sorted == false)Sort();

	bool Ret = BinarySearch(&Key,first,last);

	return Ret;
}
//-------------------------------------------------------------
template <class T> T* nsList<T>::Search(const T& Key)
{
	int first,last;

	bool Res = Search(Key,first,last);

	if(Res == false)return 0;

	return GetItem(first);
}
//-------------------------------------------------------------
template <class T> int nsList<T>::IndexOf(const T& Key,int Start)
{
	for(int Cnt = Start;Cnt < GetCount();Cnt++)
	{

		T& Me =  GetItemRef(Cnt);
		if(Key == Me)return Cnt;
	}
	return -1;
}
//-------------------------------------------------------------
template <class T> int nsList<T>::IndexOf(const T* Key,int Start)
{
	for(int Cnt = Start;Cnt < GetCount();Cnt++)
	{
		if(Key == GetItem(Cnt))return Cnt;
	}
	return -1;
}
//-------------------------------------------------------------
template <class T> void nsList<T>::CopyList(nsList<T>& List)const
{
	T   *Item;
	T   *CpyObj;
	int  Cnt;

	List.Clear();

	for(Cnt=0;Cnt<GetCount();Cnt++)
	{
		if((Item = GetItem(Cnt))!=NULL)
		{
			CpyObj = new T(*Item);
			List.Add(CpyObj);
		}
	}

	return;
}
//-------------------------------------------------------------
template <class T> void  nsList<T>::Sort()
{
	Pack();
	HeapSort();
	Sorted = true;
}
//-------------------------------------------------------------
template <class T> bool nsList<T>::SwapItem(int index1,int index2)
{
	T*  TmpObj;

	if(!CheckIndex(index1) || !CheckIndex(index2))return false;

	TmpObj       = Item[index1];
	Item[index1] = Item[index2];
	Item[index2] = TmpObj;

	return true;
}
//-------------------------------------------------------------
template <class T>  void  nsList<T>::SetAllowDeleteItem(bool dflg)
{
	DelItemClear = dflg;
}
//-------------------------------------------------------------
template <class T> void nsList<T>::HeapSort()
{
	int         i;
	int         j;
	int         k;
	int         n = GetCount();
	T **a = Item;
	T *x;

	for(k=n/2 - 1;k>=0;k--)
	{
		i = k;
		x = a[i];

		while((j = 2 * i + 1) < n)
		{
			if(j < n - 1 && ListCompare(a[j],a[j+1]) < 0)j++;
			if(ListCompare(x,a[j]) >= 0)break;
			a[i] = a[j];
			i=j;
		}
		a[i] = x;
	}

	while(n > 1)
	{
		n--;
		x = a[n];
		a[n] = a[0];
		i = 0;
		while((j=2*i + 1) < n)
		{
			if(j < n - 1 && ListCompare(a[j],a[j+1]) < 0)j++;
			if(ListCompare(x,a[j]) >= 0)break;
			a[i] = a[j];
			i = j;
		}
		a[i] = x;
	}
}

//-------------------------------------------------------------
template <class T> bool nsList<T>::BinarySearch(const T* Key,int& first,int& last)
{
	int firstfind;
	int here;
	int n = GetCount();
	int j = n - 1;
	int i=0,k;
	int cmp;


	if(n<=0)return false;

	do
	{
		k    =(j+i)/2;
		here = k;
		cmp  = ListCompare(Key,Item[here]);
		if(cmp == 0) //î≠å©ÇµÇΩ
		{
			firstfind = here;
			while(here > 0)
			{
				if(!ListCompare(Key,Item[here-1]))here--;
				else                              break;
			}
			first = here;
			here = firstfind;
			while(here < n - 1)
			{
				if(!ListCompare(Key,Item[here+1]))here++;
				else                              break;
			}
			last = here;
			return true;
		}
		if(cmp < 0)j=k-1;
		else       i=k+1;
	}while(j>=i);
	first = j;
	last  = i;

	return false;
}
//-------------------------------------------------------------
template <class T> int nsList<T>::ListCompare(const T* Me,const T* Him)
{
	int         Ret;

	const T& X = *Me;
	const T& Y = *Him;

	if     (X >  Y)Ret = 1;
	else if(X == Y)Ret = 0;
	else           Ret = -1;

	return Ret;
}
#endif //nsListH

