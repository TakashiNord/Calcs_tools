object FormMaterial: TFormMaterial
  Left = 632
  Top = 104
  Width = 571
  Height = 565
  ActiveControl = BitBtn1
  AutoSize = True
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSizeToolWin
  BorderWidth = 1
  Caption = '�������������� ��������.....'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object StringGridTab: TStringGrid
    Left = 0
    Top = 152
    Width = 561
    Height = 241
    ColCount = 4
    FixedCols = 0
    RowCount = 9
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goRowSizing, goColSizing]
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    ColWidths = (
      379
      54
      57
      56)
  end
  object Memo1: TMemo
    Left = 0
    Top = 400
    Width = 393
    Height = 129
    Color = clActiveBorder
    Lines.Strings = (
      
        '�������� Kc1.1=�������� ���� ������� � ������� �������� �����=0 ' +
        '�. '
      '��� ������ ����� �������� kc1.1 ������ ���� ���������/��������� '
      
        '��������������� �������, �.�. 1% �� ������ ������ �������� �����' +
        '���� '
      '����. '
      
        '�������� Rm (������ ��������� �� ����������) - ��� �������������' +
        '� '
      '������ ��� ����������� ��������� ������ ��������� ���������, '
      
        '���� �������� �������������� � ������ ����� ������� ��������� ��' +
        ' '
      
        '����������, ��������, ������ ������� ��� ���������� ������������' +
        ' '
      '��������.')
    ReadOnly = True
    TabOrder = 1
  end
  object BitBtn1: TBitBtn
    Left = 432
    Top = 408
    Width = 97
    Height = 65
    Caption = '>> �������..'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    OnClick = BitBtn1Click
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 561
    Height = 145
    Caption = '��� ���������...'
    TabOrder = 3
    object RadioButton1: TRadioButton
      Left = 16
      Top = 24
      Width = 97
      Height = 17
      Caption = '����� (P)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = RadioButton1Click
    end
    object RadioButton2: TRadioButton
      Left = 16
      Top = 48
      Width = 209
      Height = 17
      Caption = '����������� ����� (M)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clFuchsia
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = RadioButton2Click
    end
    object RadioButton3: TRadioButton
      Left = 16
      Top = 72
      Width = 201
      Height = 17
      Caption = '�������� ����� (K)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      OnClick = RadioButton3Click
    end
    object RadioButton4: TRadioButton
      Left = 16
      Top = 96
      Width = 185
      Height = 17
      Caption = '������� ������� (N)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGreen
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      OnClick = RadioButton4Click
    end
    object RadioButton5: TRadioButton
      Left = 16
      Top = 120
      Width = 369
      Height = 17
      Caption = '����������� ������ � ��������� ������ (S)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clOlive
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 4
      OnClick = RadioButton5Click
    end
  end
end
