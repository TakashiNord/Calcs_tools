object FormGraphic: TFormGraphic
  Left = 294
  Top = 287
  Width = 580
  Height = 427
  Caption = 'Номограмма (Мощность,Момент,Обороты шпинделя)'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object ElGraph1: TElGraph
    Left = 0
    Top = 0
    Width = 572
    Height = 393
    ShowLegend = True
    ShowMinMax = True
    ShowGrid = True
    Align = alClient
    LegendBkColor = clBlack
    HGridLines = 15
    VGridLines = 15
    Transparent = False
    DockOrientation = doNoOrient
  end
end
