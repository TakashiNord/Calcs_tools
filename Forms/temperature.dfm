object TemperatureForm: TTemperatureForm
  Left = 319
  Top = 211
  AutoSize = True
  BorderStyle = bsDialog
  Caption = 'Температура поверхности детали'
  ClientHeight = 241
  ClientWidth = 525
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 256
    Top = 0
    Width = 187
    Height = 16
    Caption = 'Скорость резания, м/мин'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 8
    Top = 120
    Width = 124
    Height = 16
    Caption = 'Подача (мм/мин)'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    Layout = tlCenter
  end
  object StringGridT: TStringGrid
    Left = 140
    Top = 16
    Width = 385
    Height = 225
    ColCount = 9
    DefaultColWidth = 40
    DefaultRowHeight = 20
    RowCount = 10
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goTabs]
    TabOrder = 0
    RowHeights = (
      20
      20
      20
      20
      20
      20
      20
      20
      20
      20)
  end
  object StaticText1: TStaticText
    Left = 0
    Top = 0
    Width = 137
    Height = 105
    AutoSize = False
    BorderStyle = sbsSunken
    Caption = 
      'Зависимость температуры поверхности детали'#13#10'(в градусах Цельсия)' +
      ' от скорости резания и подачи.'
    TabOrder = 1
  end
end
