object MainForm: TMainForm
  Left = 100
  Top = 100
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'BackgroundMaker'
  ClientHeight = 516
  ClientWidth = 1048
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object MainPanel: TPanel
    Left = 0
    Top = 41
    Width = 1048
    Height = 475
    Align = alClient
    BevelOuter = bvLowered
    ParentBackground = False
    TabOrder = 1
    object Label2: TLabel
      Left = 13
      Top = 30
      Width = 499
      Height = 19
      Alignment = taCenter
      AutoSize = False
      Caption = 'Original video frame'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 518
      Top = 30
      Width = 517
      Height = 19
      Alignment = taCenter
      AutoSize = False
      Caption = 'Background extraction image'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object SrcPanel: TPanel
      Left = 12
      Top = 63
      Width = 500
      Height = 375
      DoubleBuffered = False
      ParentDoubleBuffered = False
      TabOrder = 0
      object SrcImage: TImage
        Left = 1
        Top = 1
        Width = 498
        Height = 373
        Align = alClient
        Center = True
        Stretch = True
        ExplicitLeft = 76
        ExplicitTop = 136
        ExplicitWidth = 318
        ExplicitHeight = 238
      end
    end
  end
  object DestPanel: TPanel
    Left = 535
    Top = 104
    Width = 500
    Height = 375
    ParentBackground = False
    TabOrder = 0
  end
  object ToolPanel: TPanel
    Left = 0
    Top = 0
    Width = 1048
    Height = 41
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 2
    object Label1: TLabel
      Left = 699
      Top = 14
      Width = 51
      Height = 19
      Caption = 'Frame'
      Color = 16757092
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object FrameLbl: TLabel
      Left = 757
      Top = 14
      Width = 42
      Height = 19
      Alignment = taRightJustify
      AutoSize = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object ProgLbl: TLabel
      Left = 808
      Top = 14
      Width = 71
      Height = 19
      Caption = 'Progress'
      Color = 58339
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object ExecuteBtn: TBitBtn
      Left = 153
      Top = 10
      Width = 130
      Height = 25
      Caption = 'Extract Background '
      TabOrder = 0
      OnClick = ExecuteBtnClick
    end
    object ForeBtn: TButton
      Left = 5
      Top = 10
      Width = 130
      Height = 25
      Caption = 'Read Movie File'
      TabOrder = 1
      OnClick = ForeBtnClick
    end
    object SaveBtn: TButton
      Left = 302
      Top = 10
      Width = 130
      Height = 25
      Caption = 'Save Background  Image'
      TabOrder = 2
      OnClick = SaveBtnClick
    end
    object ProgressBar: TProgressBar
      Left = 889
      Top = 16
      Width = 150
      Height = 17
      BackgroundColor = clWhite
      Step = 1
      TabOrder = 3
    end
    object QuitBtn: TButton
      Left = 498
      Top = 10
      Width = 75
      Height = 25
      Caption = 'Quit'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      OnClick = QuitBtnClick
    end
  end
  object OpenDialog: TOpenDialog
    FileName = 'F:\Users\nakano\Documents\'#29289#29702#21205#30011'\'#22320#29508#23569#22899#20108#31840'\'#31532'15'#35441#12288#12371#12398#22269#12398#12383#12417#12395'.wmv'
    Filter = 'Movie files|*.avi;*.mpeg;*.mp2;*.mp3;*.mp4;*.wmv'
    Left = 364
    Top = 61
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'bmp'
    Filter = 'Background Image File|*.bmp'
    Left = 424
    Top = 61
  end
end
