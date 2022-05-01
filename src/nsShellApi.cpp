
#include "nsShellApi.h"
//C+Builder独自
#ifdef __BORLANDC__
#include <vcl.h>
#endif

namespace nsShellApi
{
	ShellApiErr LastErr; /*最後の発生したｴﾗｰ*/

//=============================================================
//
//  ｼｽﾃﾑ名称 ：
//
//  ｻﾌﾞｼｽﾃﾑ名：
//
//  名前空間 ：nsShellApi
//
//  クラス名 ：
//
//  機能     ：Shell機能に関する共通関数群
//
//  継承元   ：
//
//    01/03/03  
//
//  改定者   ：
//  改定者   ：
//
//=============================================================

//-------------------------------------------------------------
//  機能     ：ｱﾌﾟﾘｹｰｼｮﾝを起動する
//
//  関数定義 ：bool Shell(const char *App,const char *Cmd)
//
//  ｱｸｾｽﾚﾍﾞﾙ ：public
//
//  引数     ：const char *App ｱﾌﾟﾘｹｰｼｮﾝﾊﾟｽ
//             const char *Cmd ｺﾏﾝﾄﾞﾗｲﾝ引数
//
//  戻り値   ：bool 成功？
//
//    
//
//  改定者   ：
//-------------------------------------------------------------
bool Shell(const wchar_t *App,const wchar_t *Cmd)
{
	HINSTANCE Inst;
	bool      Res;

	//ｱﾌﾟﾘｹｰｼｮﾝﾊﾟｽﾁｪｯｸ
	if(wcscmp(App,L"") == 0)
	{
		LastErr = saAppNameNothing; /*ｱﾌﾟﾘｹｰｼｮﾝ名が無い*/
		return false;
	}
	//ｱﾌﾟﾘｹｰｼｮﾝ起動
	if(wcscmp(Cmd,L"") == 0)
		Inst = ShellExecuteW(NULL,L"open",App,NULL,NULL,SW_SHOW);
	else
		Inst = ShellExecuteW(NULL,L"open",App,Cmd,NULL,SW_SHOW);
	//ｴﾗｰﾁｪｯｸ
	if((int)Inst > 32)
	{
		//正常
		LastErr = saGood; /*正常*/
		Res = true;
	}
	else
	{
		//失敗
		//ｲﾝｽﾀﾝｽ(この場合ｴﾗｰｺｰﾄﾞ)をShellApiErr型に変換
		LastErr = (ShellApiErr)((int)Inst);
		Res     = false;
	}
	return Res;
}
//-------------------------------------------------------------
//  機能     ：最終ｴﾗｰを取得する
//
//  関数定義 ：ShellApiErr GetLastErr()
//
//  ｱｸｾｽﾚﾍﾞﾙ ：public
//
//  引数     ：
//
//  戻り値   ：ShellApiErr  ｴﾗｰ
//
//    
//
//  改定者   ：
//-------------------------------------------------------------
ShellApiErr GetLastErr()
{
	return LastErr;
}


//-------------------------------------------------------------
//  機能     ：ｱﾌﾟﾘｹｰｼｮﾝを起動して、起動中は待つ
//
//  関数定義 ：bool ShellAndWait(const char *App,const char *Cmd)
//
//  ｱｸｾｽﾚﾍﾞﾙ ：public
//
//  引数     ：const char *App ｱﾌﾟﾘｹｰｼｮﾝﾊﾟｽ
//             const char *Cmd ｺﾏﾝﾄﾞﾗｲﾝ引数
//
//  戻り値   ：bool 成功？
//
//    
//
//  改定者   ：
//-------------------------------------------------------------
bool ShellAndWait(const wchar_t *App,const wchar_t *Cmd,int nShow)
{
	//ｱﾌﾟﾘｹｰｼｮﾝ起動
	SHELLEXECUTEINFOW execinfo;
	memset(&execinfo,0,sizeof(execinfo));
	execinfo.cbSize       = sizeof(execinfo);
	execinfo.lpVerb       = L"open";
	execinfo.lpFile       = App;
	execinfo.lpParameters = Cmd;
	execinfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
	execinfo.nShow        = nShow;
//	execinfo.nShow        = SW_SHOWDEFAULT;

	if(!ShellExecuteExW(&execinfo))
	{
		return false;
	}
	//起動したｱﾌﾟﾘｹｰｼｮﾝが終了するまで待つ
	while(1)
	{
		//ﾌﾟﾛｾｽ待ちを行う
		DWORD ProcStatus = WaitForSingleObject(execinfo.hProcess,100);
		//ﾀｲﾑｱｳﾄ時の処理
#ifdef __BCPLUSPLUS__
		//C++Builderのﾒｯｾｰｼﾞ処理
		Application->ProcessMessages();
#endif
		//ﾌﾟﾛｾｽ状態のﾁｪｯｸ
		if(ProcStatus != WAIT_TIMEOUT)break;
	}
	return true;
}


}
