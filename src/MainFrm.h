//---------------------------------------------------------------------------

#ifndef MainFrmH
#define MainFrmH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Dialogs.hpp>
#include <vector>
#include "Background.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TPanel *DestPanel;
	TPanel *MainPanel;
	TPanel *SrcPanel;
	TImage *SrcImage;
	TPanel *ToolPanel;
	TBitBtn *ExecuteBtn;
	TButton *ForeBtn;
	TLabel *Label1;
	TLabel *FrameLbl;
	TLabel *Label2;
	TLabel *Label3;
	TButton *SaveBtn;
	TLabel *ProgLbl;
	TProgressBar *ProgressBar;
	TButton *QuitBtn;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall SaveBtnClick(TObject *Sender);
	void __fastcall ExecuteBtnClick(TObject *Sender);
	void __fastcall QuitBtnClick(TObject *Sender);
	void __fastcall ForeBtnClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:
	TBackGround *DestImage;
	bool IsBackgroundStop;
private:
	String TempFolderPath;
	bool   IsLoaded;
	bool   IsMade;
public:
	__fastcall TMainForm(TComponent* Owner);
public:
	//進捗表示
	bool DispProgress(double pg);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
