//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <algorithm>
#include "nsShellApi.h"
#include "nsMsgBox.h"
#include "TSFileCL.h"
#include "ImportMovieFrameSettingFrm.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//-------------------------------------------------------------
//event of displaying the form
//-------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject *Sender)
{
	//Progress display initialization
	DispProgress(-1.0);
	//Create result image component
	DestImage = new TBackGround(DestPanel);
	DestImage->Parent   = DestPanel;
	DestImage->Name     = L"DestImage";
	DestImage->Align    = alClient;
	DestImage->Stretch  = true;
	DestImage->Transparent = true;


	nsLib::SetMsgBoxTitle(Caption.c_str());

	//make temporary folder
	wchar_t  szPath[1024];
	DWORD    dwBuffSize;

	dwBuffSize = GetTempPathW(sizeof(szPath)/sizeof(szPath[0]),szPath);
	if(_waccess(szPath,06) != 0)
	{
		//temporary folder error
		nsLib::ErrMsgBox(Handle,L"There is not authority for writeing temporary folder.\n[%s]\nTerminate BackgroundMaker",szPath);
		Application->Terminate();
		return;
	}
	TDateTime dt = TDateTime::CurrentDateTime();
	TempFolderPath = String(szPath) + dt.FormatString(L"yyyymmddhhnnsszzz");

	MkDir(TempFolderPath);
	IsLoaded = false;
	IsMade   = false;
	DestImage->SetImaageDir(TempFolderPath);

	ToolPanel->Enabled = true;
}

//---------------------------------------------------------------------------
// form close event
//-------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	TSDirectoryCL dir;

	dir.DeleteDirectory(TempFolderPath.c_str());
}
//-------------------------------------------------------------
//event of before form close
//-------------------------------------------------------------
void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	//停止フラグ
	IsBackgroundStop = true;
	Sleep(50);
	//結果イメージコンポーネント破棄
	delete DestImage;
}

//-------------------------------------------------------------
//execute button
//-------------------------------------------------------------
void __fastcall TMainForm::ExecuteBtnClick(TObject *Sender)
{
	DIBInf BakInf;
	int SetBlockNum = 0;
	int AllBlockNum = 0;

	//Read Movie File check
	if(IsLoaded == false)
	{
		nsLib::ErrMsgBox(Handle, L"Read Movie File has not been executed.");
		return;
	}
	DestImage->RewindFrame();
	IsBackgroundStop = false;
	//complete flag
	bool isComp = false;

	for(int fr=1;DestImage->CheckFrameNumber(fr) == true;fr++)
	{
		if(IsBackgroundStop == true)
		{
			break;
		}
		FrameLbl->Caption = (fr);

		SrcImage->Picture->Graphic = DestImage->GetFrame(fr).GetBitmap();

		DestImage->ExecuteOneStep(fr,SetBlockNum,AllBlockNum);

		double pg = (100.0f * SetBlockNum) / AllBlockNum;

		if(SetBlockNum < AllBlockNum)
		{
			DispProgress(pg);
		}
		else
		{
			DispProgress(-1.0f);
			isComp = true;
			break;
		}
		DestImage->GetBackgroundImage(BakInf);
		DestImage->Picture->Graphic = BakInf.GetBitmap();
		Application->ProcessMessages();
	}
	DestImage->GetBackgroundImage(BakInf);
	DestImage->Picture->Graphic = BakInf.GetBitmap();
	Application->ProcessMessages();

	//message of end
	if(isComp == true)
	{
		nsLib::InfMsgBox(Handle, L"Task of extracting background has been complated normally.");
	}
	else
	{
		nsLib::ExcMsgBox(Handle,L"Task of extracting background has been terminated abnormally.");

	}
}

//-------------------------------------------------------------
//save button
//-------------------------------------------------------------
void __fastcall TMainForm::SaveBtnClick(TObject *Sender)
{
	String SaveFile;

	if(DestImage->Picture == nullptr)
	{
		nsLib::ExcMsgBox(Handle,L"Background image is not made yet.");
		return;
	}
	//save dialog
	if(SaveDialog->Execute() == false)
	{
		return;
	}
	//save
	SaveFile = SaveDialog->Files->Strings[0];

	//check
	if(ExtractFileExt(SaveFile).CompareIC(L".bmp") != 0)
	{
		nsLib::ExcMsgBox(Handle,L"File extension is not.bmp");
		return;
	}
	//save
	DestImage->Picture->SaveToFile(SaveFile);

	//end message
	nsLib::InfMsgBox(Handle, L"Background image is saved normally.");

}
//-------------------------------------------------------------
//Progress display
//-------------------------------------------------------------
bool TMainForm::DispProgress(double pg)
{
	//マイナスの時は消す
	if(pg < 0.0f)
	{
		ProgLbl    ->Visible = false;
		ProgressBar->Visible = false;
		return true;
	}
	ProgLbl    ->Visible  = true;
	ProgressBar->Visible  = true;
	ProgressBar->Position = (int)pg;

	return true;
}

//---------------------------------------------------------------------------
// quit button
//---------------------------------------------------------------------------
void __fastcall TMainForm::QuitBtnClick(TObject *Sender)
{
	Close();
	return;
}
//---------------------------------------------------------------------------
// Read Movie File
//---------------------------------------------------------------------------
void __fastcall TMainForm::ForeBtnClick(TObject *Sender)
{
	String App;
	String Cmd;
	bool   res;
	//Open Dialog
	if(OpenDialog->Execute() == false)
	{
		return;
	}
	//Get movie file name
	String MovieFileName = OpenDialog->Files->Strings[0];
	//checking file exsitance
	if(FileExists(MovieFileName) == false)
	{
		nsLib::ErrMsgBox(Handle,L"There is not specified file.\n[%s]",MovieFileName.c_str());
		return;
	}
	//setting window
	ImportMovieFrameSettingForm->MoviFilePathSText->Text = MovieFileName;

	if(ImportMovieFrameSettingForm->ShowModal() != mrOk)
	{
		return;
	}
	//delete previous file
	Cmd = String("del ") + TempFolderPath +L"\\*.bmp";
	res = _wsystem(Cmd.c_str());

	//call ffmpeg
	int StartTime   = ImportMovieFrameSettingForm->ReadingStartTimeEdit->Value;
	int ReadingTime = ImportMovieFrameSettingForm->ReadingTimeEdit->Value;
	int Fps         = ImportMovieFrameSettingForm->FrameRateEdit->Value;

	App = String(L"ffmpeg");
	Cmd = String(L" -i ") + MovieFileName + L" -ss " + StartTime + L" -t " + ReadingTime + L" -r " + Fps + L" " + TempFolderPath +L"\\image%04d.bmp";


	res = nsShellApi::ShellAndWait(App.c_str(),Cmd.c_str(),SW_SHOW);

	if(res == true)
	{
		IsLoaded = true;
		IsMade   = false;
		//display first frame
		SrcImage ->Picture->Graphic = DestImage->GetFrame(1).GetBitmap();
		DestImage->Picture->Graphic = nullptr;

		nsLib::InfMsgBox(Handle, L"Task of reading movie file has been complated normally.");
	}
	else
	{
		IsLoaded = false;
		nsLib::ExcMsgBox(Handle, L"Task of reading movie file has been terminated abnormally..");
	}
}
//---------------------------------------------------------------------------


