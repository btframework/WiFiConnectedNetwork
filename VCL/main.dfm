object fmMain: TfmMain
  Left = 494
  Top = 184
  BorderStyle = bsSingle
  Caption = 'Demo shows how to identify Connected WiFi Network'
  ClientHeight = 401
  ClientWidth = 646
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 8
    Top = 8
    Width = 129
    Height = 25
    Caption = 'Get information'
    TabOrder = 0
    OnClick = Button1Click
  end
  object lbInfo: TListBox
    Left = 8
    Top = 40
    Width = 625
    Height = 353
    ItemHeight = 13
    TabOrder = 1
  end
  object wclWiFiClient: TwclWiFiClient
    Left = 352
    Top = 152
  end
end
