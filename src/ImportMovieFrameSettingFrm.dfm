object ImportMovieFrameSettingForm: TImportMovieFrameSettingForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Import Movie Frame Setting'
  ClientHeight = 239
  ClientWidth = 541
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 17
    Top = 14
    Width = 180
    Height = 19
    AutoSize = False
    Caption = 'File path of movie file'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label1: TLabel
    Left = 17
    Top = 82
    Width = 224
    Height = 19
    AutoSize = False
    Caption = 'Reading Start Time [sec]'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 17
    Top = 120
    Width = 224
    Height = 19
    AutoSize = False
    Caption = 'Reading Time [sec]'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label4: TLabel
    Left = 17
    Top = 158
    Width = 224
    Height = 19
    AutoSize = False
    Caption = 'Frame rate (FPS)'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object MoviFilePathSText: TEdit
    Left = 17
    Top = 44
    Width = 508
    Height = 21
    AutoSize = False
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    ReadOnly = True
    TabOrder = 0
  end
  object ReadingStartTimeEdit: TNumberBox
    Left = 247
    Top = 81
    Width = 54
    Height = 24
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    MaxLength = 4
    ParentFont = False
    TabOrder = 1
  end
  object ReadingTimeEdit: TNumberBox
    Left = 247
    Top = 117
    Width = 54
    Height = 24
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    MaxLength = 4
    ParentFont = False
    TabOrder = 2
    Value = 60.000000000000000000
  end
  object FrameRateEdit: TNumberBox
    Left = 247
    Top = 157
    Width = 54
    Height = 24
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    MaxLength = 2
    ParentFont = False
    TabOrder = 3
    Value = 10.000000000000000000
  end
  object CancelButton: TButton
    Left = 450
    Top = 200
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
  end
  object StartButton: TButton
    Left = 354
    Top = 200
    Width = 75
    Height = 25
    Caption = 'Start'
    ModalResult = 2
    TabOrder = 5
    OnClick = StartButtonClick
  end
end
