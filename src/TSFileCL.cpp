#include <System.hpp>
#include <shellapi.h>
#include <io.h>
#include "strconv.h"
#include "TSFileCL.h"

//-------------------------------------------------------------
//  機能     ：TSDirectoryCLのコンストラクタ
//
//  関数定義 ：TSDirectoryCL()
//
//  ｱｸｾｽﾚﾍﾞﾙ ：public
//
//  引数     ：なし
//
//  戻り値   ：なし
//
//  作成者   ：
//
//  改定者   ：
//-------------------------------------------------------------
TSDirectoryCL::TSDirectoryCL()
{
	//一時ディレクトリを得る
	::GetTempPathA(MAX_PATH,BackupDir);
}
//-------------------------------------------------------------
//  機能     ：TSDirectoryCLのデストラクタ
//
//  関数定義 ：~TSDirectoryCL()
//
//  ｱｸｾｽﾚﾍﾞﾙ ：public
//
//  引数     ：なし
//
//  戻り値   ：なし
//
//  作成者   ：    
//
//  改定者   ：
//-------------------------------------------------------------
TSDirectoryCL::~TSDirectoryCL()
{
}
//-------------------------------------------------------------
//  機能     ：ﾌｧｲﾙのｺﾋﾟｰ
//
//  関数定義 ：bool CopyFile(const char *Src,const char *Dest)
//
//  ｱｸｾｽﾚﾍﾞﾙ ：public
//
//  引数     ：const char *Src   ｺﾋﾟｰ元のﾌｧｲﾙ
//           ：const char *Dest  ｺﾋﾟｰ先のﾌｧｲﾙ
//
//  戻り値   ：bool 成功 true  失敗 false
//
//  作成者　 ：
//
//  改定者   ：
//-------------------------------------------------------------
bool TSDirectoryCL::CopyFile(const char *Src,const char *Dest)
{
	char *pFrom = new char[strlen(Src)  + 2];
	char *pTo   = new char[strlen(Dest) + 2];
	bool  fResult,Result;
	SHFILEOPSTRUCTA FileOP;

	strcpy(pFrom,Src);
	pFrom[strlen(Src)  + 1] = '\0';
	strcpy(pTo,Dest);
	pTo[strlen(Dest)  + 1] = '\0';


	FileOP.hwnd                  = NULL;
	FileOP.wFunc                 = FO_COPY;
	FileOP.pFrom                 = pFrom;
	FileOP.pTo                   = pTo;
	FileOP.fFlags                = FOF_SILENT;
	FileOP.fAnyOperationsAborted = FALSE;
	FileOP.hNameMappings         = NULL;
	FileOP.lpszProgressTitle     = "";

	try{
		fResult = (SHFileOperationA(&FileOP) == 0);
	}catch(...){}

	Result  = (fResult == true && FileOP.fAnyOperationsAborted == FALSE);
	delete [] pFrom;
	delete [] pTo;

	return Result;
}
bool TSDirectoryCL::CopyFile(const wchar_t *Src,const wchar_t *Dest)
{
    std::string s = wide_to_ansi(Src);
    std::string d = wide_to_ansi(Dest);

	//メソッドを呼び出し
	bool Res = CopyFile(s.c_str(),d.c_str());

	return Res;
}
//-------------------------------------------------------------
//  機能     ：ﾃﾞｨﾚｸﾄﾘのｺﾋﾟｰ
//
//  関数定義 ：bool DirectoryCopy(char *Sce,char *Des)
//
//  ｱｸｾｽﾚﾍﾞﾙ ：public
//
//  引数     ：const char *Sce   ｺﾋﾟｰ元のﾃﾞｨﾚｸﾄﾘ
//           ：const char *Des   ｺﾋﾟｰ先のﾃﾞｨﾚｸﾄﾘ
//
//  戻り値   ：bool 成功 true  失敗 false
//
//  作成者　 ：
//
//  改定者   ：
//-------------------------------------------------------------
bool TSDirectoryCL::DirectoryCopy(const char *Sce,const char *Des)
{
	SHFILEOPSTRUCTA FOperation;
	char SceDir[262];
	char DesDir[262];

	memset(SceDir,0,sizeof(SceDir));
	memset(DesDir,0,sizeof(DesDir));

	strcpy(SceDir,Sce);
	SceDir[strlen(Sce)+1]='\0';
	strcpy(DesDir,Des);
	DesDir[strlen(Des)+1]='\0';

	FOperation.hwnd=NULL;
	FOperation.wFunc=FO_COPY;
	FOperation.pFrom=SceDir;
	FOperation.pTo=DesDir;
	FOperation.fAnyOperationsAborted=false;
	FOperation.hNameMappings=NULL;
	FOperation.lpszProgressTitle=NULL;
	FOperation.fFlags= FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR |
						FOF_SILENT | FOF_MULTIDESTFILES;

	return (SHFileOperationA(&FOperation)==0);
}
bool TSDirectoryCL::DirectoryCopy(const wchar_t *Sce,const wchar_t *Des)
{
    std::string s = wide_to_ansi(Sce);
    std::string d = wide_to_ansi(Des);
	//char型メソッドを呼び出し
	bool Res = DirectoryCopy(s.c_str(),d.c_str());

	return Res;
}
//-------------------------------------------------------------
//  機能     ：ﾌｧｲﾙの削除
//
//  関数定義 ：bool DeleteFile(const char *Path)
//
//  ｱｸｾｽﾚﾍﾞﾙ ：public
//
//  引数     ：const char *Path  削除するﾌｧｲﾙ
//
//  戻り値   ：bool 成功 true  失敗 false
//
//  作成者　 ：
//
//  改定者   ：
//-------------------------------------------------------------
bool TSDirectoryCL::DeleteFile(const char *Path)
{
	char *pFrom = new char[strlen(Path) + 2];
	bool  fResult,Result;
	strcpy(pFrom,Path);
	pFrom[strlen(Path) + 1] = '\0';

	SHFILEOPSTRUCTA FileOP;

	FileOP.hwnd                  = NULL;
	FileOP.wFunc                 = FO_DELETE;
	FileOP.pFrom                 = pFrom;
	FileOP.fFlags                = FOF_SILENT | FOF_NOCONFIRMATION;
	FileOP.fAnyOperationsAborted = FALSE;
	FileOP.hNameMappings         = NULL;
	FileOP.lpszProgressTitle     = "";

	fResult = (SHFileOperationA(&FileOP) == 0);
	Result  = (fResult == true && FileOP.fAnyOperationsAborted == FALSE);
	delete [] pFrom;

	return Result;
}
//-------------------------------------------------------------
//  機能     ：ﾃﾞｨﾚｸﾄﾘの作成
//
//  関数定義 ：bool MakeDirectory(const char *Path)
//
//  ｱｸｾｽﾚﾍﾞﾙ ：public
//
//  引数     ：const char *Path   作成するﾃﾞｨﾚｸﾄﾘ
//
//  戻り値   ：bool 成功 true  失敗 false
//
//  作成者   ：
//
//  改定者   ：
//-------------------------------------------------------------
bool TSDirectoryCL::MakeDirectory(const char *Path)
{
	SECURITY_ATTRIBUTES S_A;


	S_A.nLength              = sizeof(_SECURITY_ATTRIBUTES);
	S_A.lpSecurityDescriptor = NULL;
	S_A.bInheritHandle       = TRUE;

	if (CreateDirectoryA(Path,NULL) == FALSE)
	{
		return false;
	}
	return true;
}
bool TSDirectoryCL::MakeDirectory(const wchar_t *Path)
{
	std::string ph = wide_to_ansi(Path);

	return MakeDirectory(ph.c_str());
}
//-------------------------------------------------------------
//  機能     ：ﾃﾞｨﾚｸﾄﾘの削除
//
//  関数定義 ：bool DeleteDirectory(const char *Path)
//
//  ｱｸｾｽﾚﾍﾞﾙ ：public
//
//  引数     ：const char *Path
//
//  戻り値   ：bool 成功 true  失敗 false
//
//  作成者   ：
//
//  改定者   ：
//-------------------------------------------------------------
bool TSDirectoryCL::DeleteDirectory(const char *Path)
{
	//ﾌｧｲﾙ削除履歴ﾘｽﾄ消去
	DelfList.Clear();
	//指定ﾃﾞｨﾚｸﾄﾘ以下をすべて削除
	bool Ret = ProcDeleteDirectory(Path);
	//自己ﾃﾞｨﾚｸﾄﾘ削除
	if(Ret == true)
		Ret = (RemoveDirectoryA(Path) == TRUE);
	//状態別処理
	if(Ret == false)
	{
		//削除が失敗したならﾌｧｲﾙの復活処理をする
		for(int Cnt = DelfList.GetCount() - 1;Cnt >= 0;Cnt--)
		{
			//ﾌｧｲﾙ削除履歴ﾕﾆｯﾄ取得
			TS_ffblk *DelUnit = (TS_ffblk *)DelfList.GetItem(Cnt);
			//ﾃﾞｨﾚｸﾄﾘかﾌｧｲﾙかをﾁｪｯｸ
#ifndef _MSC_VER 
			if(DelUnit->ff_attrib & FA_DIREC)
#else
			if(DelUnit->attrib & _A_SUBDIR)
#endif
			{
				//ﾃﾞｨﾚｸﾄﾘの場合は、作成処理
#ifndef _MSC_VER 
				mkdir(DelUnit->ff_name);
#else
				mkdir(DelUnit->name);
#endif
			}
			else
			{
				//ﾌｧｲﾙの場合は、ﾊﾞｯｸｱｯﾌﾟからの復活
#ifndef _MSC_VER 
				MoveFileExA(DelUnit->BackupFile,DelUnit->ff_name,MOVEFILE_COPY_ALLOWED);
#else
				MoveFileExA(DelUnit->BackupFile,DelUnit->name,MOVEFILE_COPY_ALLOWED);
#endif
			}
		}
	}
	else
	{
		//削除成功なら、ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙを削除
		for(int Cnt = DelfList.GetCount() - 1;Cnt >= 0;Cnt--)
		{
			//ﾌｧｲﾙ削除履歴ﾕﾆｯﾄ取得
			TS_ffblk *DelUnit = (TS_ffblk *)DelfList.GetItem(Cnt);
			//ﾃﾞｨﾚｸﾄﾘかﾌｧｲﾙかをﾁｪｯｸ
#ifndef _MSC_VER 
			if(DelUnit->ff_attrib & FA_DIREC)
#else
			if(DelUnit->attrib & _A_SUBDIR)
#endif
			{
				//ﾃﾞｨﾚｸﾄﾘの場合は、処理なし
			}
			else
			{
				//ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙは削除
				unlink(DelUnit->BackupFile);
			}
		}
	}
	//ﾌｧｲﾙ削除履歴ﾘｽﾄ消去
	DelfList.Clear();

	return Ret;
}
bool TSDirectoryCL::DeleteDirectory(const wchar_t *Path)
{
	std::string aPath = wide_to_ansi(Path);
	//char型メソッドを呼び出し
	bool Res = DeleteDirectory(aPath.c_str());

	return Res;
}

//-------------------------------------------------------------
//  機能     ：ﾃﾞｨﾚｸﾄﾘの削除の再帰関数
//
//  関数定義 ：bool ProcDeleteDirectory(const char *Path)
//
//  ｱｸｾｽﾚﾍﾞﾙ ：public
//
//  引数     ：const char *Path
//
//  戻り値   ：bool 成功 true  失敗 false
//
//  作成者   ：
//
//  改定者   ：
//-------------------------------------------------------------
bool TSDirectoryCL::ProcDeleteDirectory(const char *Path)
{
	nsList<TS_ffblk> fList; //ﾌｧｲﾙﾘｽﾄ
	ffblk            Ffblk;
	int              Cnt;
	int              Ret;
	char             path[256];
#ifdef _MSC_VER 
	long hHandle;
#endif
	//検索ﾊﾟｽを作成
	sprintf(path,"%s\\*.*",Path);
    //ﾌｧｲﾙﾘｽﾄ作成
	for(Cnt = 0;;Cnt++)
	{
		//検索関数を呼ぶ
		if(Cnt == 0)
		{
#ifndef _MSC_VER 
			Ret = findfirst(path,&Ffblk,FA_DIREC|FA_ARCH);
#else
			hHandle = _findfirst(path,&Ffblk);
			Ret = (hHandle == -1L);
#endif
		}
		else
#ifndef _MSC_VER 
			Ret = findnext(&Ffblk);
#else
			Ret = _findnext(hHandle,&Ffblk);
#endif
		//ﾁｪｯｸ
		if(Ret != 0)
		{
#ifndef _MSC_VER 
			Ret = findclose(&Ffblk);
#else
			Ret = _findclose(hHandle);
#endif
			break;
		}
		//.と..は省く
#ifndef _MSC_VER 
		if(strcmp(Ffblk.ff_name,".") == 0 || strcmp(Ffblk.ff_name,"..") == 0)continue;
#else
		if(strcmp(Ffblk.name,".") == 0 || strcmp(Ffblk.name,"..") == 0)continue;
#endif
		//ﾘｽﾄに追加
		TS_ffblk *fUnit = new TS_ffblk();
		memcpy((ffblk *)fUnit,&Ffblk,sizeof(Ffblk));
#ifndef _MSC_VER
		sprintf(fUnit->ff_name,"%s\\%s",Path,Ffblk.ff_name);
#else
		sprintf(fUnit->name,"%s\\%s",Path,Ffblk.name);
#endif
		fList.Add(fUnit);
	}
	//ﾌｧｲﾙが削除可能かﾁｪｯｸ
	for(Cnt = 0;Cnt < fList.GetCount();Cnt++)
	{
		TS_ffblk *NowFile = (TS_ffblk *)fList.GetItem(Cnt);
		//対象がﾃﾞｨﾚｸﾄﾘの場合は除外
#ifndef _MSC_VER 
		if(!(NowFile->ff_attrib & FA_DIREC))
#else
		if(!(NowFile->attrib & _A_SUBDIR))
#endif
		{
        	char TmpFName[L_tmpnam];
			char BackupFullPath[MAX_PATH];
			//ﾃﾝﾎﾟﾗﾘﾌｧｲﾙ名取得
			tmpnam(TmpFName);
			//ﾃﾝﾎﾟﾗﾘﾌｧｲﾙフルパス組み立て
			sprintf(BackupFullPath,"%s%s",BackupDir,TmpFName);
			//削除ﾌｧｲﾙ履歴ﾕﾆｯﾄ
			TS_ffblk *DelUnit = new TS_ffblk(*NowFile);
			DelfList.Add(DelUnit);
			//ﾊﾞｯｸｱｯﾌﾟ先を押さえる
			strcpy(DelUnit->BackupFile,BackupFullPath);
			//現ﾌｧｲﾙをｺﾋﾟｰ
#ifndef _MSC_VER
//            ::CopyFile(NowFile->ff_name,TmpFName,FALSE);
            ::CopyFileA(NowFile->ff_name,BackupFullPath,FALSE);
			int  Res = unlink(NowFile->ff_name);
#else
//            ::CopyFile(NowFile->name,TmpFName,FALSE);
            ::CopyFile(NowFile->name,BackupFullPath,FALSE);
			int  Res = unlink(NowFile->name);
#endif

			//失敗したら終わり
			if(Res != 0)
            {
				return false;
			}
		}
#ifndef _MSC_VER 
		else if(NowFile->ff_attrib & FA_DIREC)
#else
		else if(NowFile->attrib & _A_SUBDIR)
#endif
		{
			//ﾌｧｲﾙがﾃﾞｨﾚｸﾄﾘなら、次の階層に行く
#ifndef _MSC_VER 
			if(ProcDeleteDirectory(NowFile->ff_name) == false)
			{
				return false;
            }
#else
			if(ProcDeleteDirectory(NowFile->name) == false)return false;
#endif
			//削除ﾌｧｲﾙ履歴ﾕﾆｯﾄ
			TS_ffblk *DelUnit = new TS_ffblk(*NowFile);
			DelfList.Add(DelUnit);
			//ﾌｧｲﾙの削除が成功したなら、ﾃﾞｨﾚｸﾄﾘの削除を行う
#ifndef _MSC_VER 
//			if(rmdir(NowFile->ff_name)!=0)
			try
			{
				RmDir(NowFile->ff_name);
			}
			catch(...)
			{
				return false;
			}
#else
			if(rmdir(NowFile->name)!=0)return false;
#endif
		}
	}
	return true;
}
//-------------------------------------------------------------
//  機能     ：ﾌｧｲﾙの存在をﾁｪｯｸする
//
//  関数定義 ：bool CheckExistFile(const char *Path)
//
//  ｱｸｾｽﾚﾍﾞﾙ ：public
//
//  引数     ：const char *Path
//
//  戻り値   ：bool ある true  ない false
//
//  作成者   ：    
//
//  改定者   ：
//-------------------------------------------------------------
bool TSDirectoryCL::CheckExistFile(const char *Path)
{
	return (::access(Path,0) == 0);
}
//-------------------------------------------------------------
//  機能     ：ｶﾚﾝﾄﾃﾞｨﾚｸﾄﾘを設定する
//
//  関数定義 ：bool SetCurrentDirectory(const char *Path)
//
//  ｱｸｾｽﾚﾍﾞﾙ ：public
//
//  引数     ：const char *Path 設定するｶﾚﾝﾄﾃﾞｨﾚｸﾄﾘ
//
//  戻り値   ：bool ある true  ない false
//
//  作成者   ：
//
//  改定者   ：
//-------------------------------------------------------------
bool TSDirectoryCL::SetCurrentDirectory(const char *Path)
{
	return (::SetCurrentDirectoryA(Path) == TRUE);
}
//-------------------------------------------------------------
//  機能     ：ﾌｧｲﾙ(ﾃﾞｨﾚｸﾄﾘ)の移動(又は名称変更)
//
//  関数定義 ：bool MoveFile(const char *src,const char *dest)
//
//  ｱｸｾｽﾚﾍﾞﾙ ：public
//
//  引数     ：const char *src  移動元ﾌｧｲﾙ(ﾃﾞｨﾚｸﾄﾘ)のﾌﾙﾊﾟｽ
//             const char *dest 移動先ﾌｧｲﾙ(ﾃﾞｨﾚｸﾄﾘ)のﾌﾙﾊﾟｽ
//
//  戻り値   ：bool  true  OK false NG
//
//  作成者   ：
//
//  改定者   ：
//-------------------------------------------------------------
bool TSDirectoryCL::MoveFile(const char *src,const char *dest)
{
	return (::MoveFileA(src,dest) == TRUE);
}


