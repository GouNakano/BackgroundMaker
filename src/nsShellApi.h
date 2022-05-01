#ifndef nsShellApiH
#define nsShellApiH

#include <windows.h>
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
//  作成者　 ：
//
//  改定者   ：
//  改定者   ：
//
//=============================================================
namespace nsShellApi
{
	//ｴﾗｰｺｰﾄﾞ定義
	enum ShellApiErr {
		 saGood                   = 0                      /*正常*/,
		 saAppNameNothing         = -1                     /*ｱﾌﾟﾘｹｰｼｮﾝ名が無い*/,
		 saSE_ERR_ASSOCINCOMPLETE = SE_ERR_ASSOCINCOMPLETE /*ファイル名の関連付けが不完全､ または無効です。*/,
		 saSE_ERR_DDEBUSY         = SE_ERR_DDEBUSY         /*DDE Busy */,
		 saSE_ERR_DDEFAIL         = SE_ERR_DDEFAIL         /*DDEトランザクションが失敗しました。*/,
		 saSE_ERR_DDETIMEOUT      = SE_ERR_DDETIMEOUT      /*要求タイム アウト*/,
		 saSE_ERR_NOASSOC         = SE_ERR_NOASSOC         /*指定されたファイル名拡張子には､ アプリケーションが関連付けられていません。*/,
		 saSE_ERR_SHARE           = SE_ERR_SHARE           /*共有違反が発生しました。*/
	};
	//ｱﾌﾟﾘｹｰｼｮﾝを起動する
	bool Shell(const wchar_t *App,const wchar_t *Cmd = L"");
	//ｱﾌﾟﾘｹｰｼｮﾝを起動して、起動中は待つ
	bool ShellAndWait(const wchar_t *App,const wchar_t *Cmd,int nShow);
};

#endif

