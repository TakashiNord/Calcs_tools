object FormMessage: TFormMessage
  Left = 424
  Top = 177
  Width = 589
  Height = 167
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object ElFlatMemoMes: TElFlatMemo
    Left = 0
    Top = 0
    Width = 581
    Height = 133
    Align = alClient
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 0
    ActiveBorderType = fbtFramed
    Flat = True
    FlatFocusedScrollBars = True
    InactiveBorderType = fbtRaised
  end
  object ElFormCaption1: TElFormCaption
    Active = True
    ActiveLeftColor = clGreen
    BackgroundType = bgtStretchBitmap
    Alignment = taLeftJustify
    PaintBkgnd = pbtAlways
    Buttons = <
      item
        Layout = blGlyphLeft
        Align = taRightJustify
        Caption = 'Очистить'
        FixClick = False
        ActiveColor = clBtnText
        InactiveColor = clBtnText
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        OnClick = ElFormCaption1Buttons0Click
      end>
    SystemFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -8
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Texts = <
      item
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -8
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Caption = 'Сообщения....'
        Layout = blGlyphLeft
        Align = taLeftJustify
      end>
    Left = 16
    Top = 8
  end
end
