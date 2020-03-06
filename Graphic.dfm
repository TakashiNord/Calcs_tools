object FormGraphic: TFormGraphic
  Left = 234
  Top = 141
  Width = 870
  Height = 600
  Caption = 'Номограмма '
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 108
  TextHeight = 13
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 862
    Height = 25
    ButtonHeight = 21
    ButtonWidth = 54
    ShowCaptions = True
    TabOrder = 0
    object ToolButton1: TToolButton
      Left = 0
      Top = 2
      Caption = 'Закрыть'
      ImageIndex = 0
      OnClick = ToolButton1Click
    end
    object ToolButton2: TToolButton
      Left = 54
      Top = 2
      Caption = 'Очистить'
      ImageIndex = 1
      OnClick = ToolButton2Click
    end
  end
  object ChartNom: TChart
    Left = 0
    Top = 25
    Width = 862
    Height = 542
    BackWall.Brush.Color = clWhite
    BackWall.Brush.Style = bsClear
    Title.Font.Charset = DEFAULT_CHARSET
    Title.Font.Color = clBlue
    Title.Font.Height = -14
    Title.Font.Name = 'Arial'
    Title.Font.Style = [fsBold]
    Title.Text.Strings = (
      'Номограмма')
    BottomAxis.Title.Caption = 'Обороты шпинделя'
    LeftAxis.Title.Caption = 'Момент'
    Legend.Alignment = laTop
    RightAxis.Title.Caption = 'Мощность'
    View3D = False
    Align = alClient
    TabOrder = 1
    object Series1: TLineSeries
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = clRed
      Title = 'Кривая Мощности'
      VertAxis = aRightAxis
      LinePen.Width = 3
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = True
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object Series2: TLineSeries
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = clGreen
      Title = 'Кривая Момента'
      LinePen.Width = 3
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = True
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object Series3: TPointSeries
      Marks.ArrowLength = 0
      Marks.Visible = False
      SeriesColor = clYellow
      Title = 'Мощность'
      VertAxis = aRightAxis
      Pointer.Brush.Color = clYellow
      Pointer.InflateMargins = True
      Pointer.Pen.Color = clMaroon
      Pointer.Pen.Width = 6
      Pointer.Style = psRectangle
      Pointer.Visible = True
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object Series4: TPointSeries
      Marks.ArrowLength = 0
      Marks.Visible = False
      SeriesColor = clBlue
      Title = 'Момент'
      Pointer.InflateMargins = True
      Pointer.Pen.Color = clLime
      Pointer.Pen.Width = 6
      Pointer.Style = psRectangle
      Pointer.Visible = True
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
  end
end
