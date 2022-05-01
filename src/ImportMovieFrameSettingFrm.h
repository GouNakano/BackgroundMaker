//---------------------------------------------------------------------------

#ifndef ImportMovieFrameSettingFrmH
#define ImportMovieFrameSettingFrmH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.NumberBox.hpp>
//---------------------------------------------------------------------------
class TImportMovieFrameSettingForm : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TLabel *Label2;
	TEdit *MoviFilePathSText;
	TLabel *Label1;
	TNumberBox *ReadingStartTimeEdit;
	TLabel *Label3;
	TNumberBox *ReadingTimeEdit;
	TLabel *Label4;
	TNumberBox *FrameRateEdit;
	TButton *CancelButton;
	TButton *StartButton;
	void __fastcall StartButtonClick(TObject *Sender);
private:	// ユーザー宣言
public:		// ユーザー宣言
	__fastcall TImportMovieFrameSettingForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TImportMovieFrameSettingForm *ImportMovieFrameSettingForm;
//---------------------------------------------------------------------------
#endif
