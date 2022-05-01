//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ImportMovieFrameSettingFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TImportMovieFrameSettingForm *ImportMovieFrameSettingForm;
//---------------------------------------------------------------------------
__fastcall TImportMovieFrameSettingForm::TImportMovieFrameSettingForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TImportMovieFrameSettingForm::StartButtonClick(TObject *Sender)
{
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------
