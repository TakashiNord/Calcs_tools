//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Grids.hpp>

//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TLabel *Label2;
        TComboBox *BaseD;
        TComboBox *BaseZ;
        TTabControl *TabMach;
        TPopupMenu *PopupMenuFormMain;
        TMenuItem *N1;
        TMenuItem *N2;
        TStatusBar *StatusBarMes;
        TMenuItem *N3;
        TMenuItem *N4;
        TLabel *LabelB;
        TLabel *LabelT;
        TEdit *BaseB;
        TEdit *BaseT;
        TPageControl *PageControlMethods;
        TTabSheet *TabSheetStandard;
        TTabSheet *TabSheetTechAl;
        TLabel *Label6;
        TLabel *LabelVc;
        TEdit *BaseVc;
        TLabel *Label3;
        TComboBox *ComboBoxMaterial;
        TGroupBox *GroupBox3;
        TImage *Image6;
        TEdit *EditStQ;
        TLabel *Label21;
        TGroupBox *GroupBox4;
        TImage *Image7;
        TEdit *EditStVf;
        TLabel *Label18;
        TImage *Image3;
        TEdit *EditStfz;
        TLabel *Label19;
        TGroupBox *GroupBox5;
        TImage *Image5;
        TEdit *EditStP;
        TLabel *Label22;
        TLabel *Label23;
        TComboBox *ComboBoxStkpd;
        TGroupBox *GroupBox6;
        TEdit *EditSthm2;
        TLabel *Label20;
        TComboBox *ComboBoxStContact;
        TLabel *Label25;
        TLabel *Label26;
        TEdit *EditStk;
        TCheckBox *CheckBoxhm2;
        TImageList *ImageListvs;
        TImage *Image8;
        TImage *Imagevs;
        TImage *Imagehm;
        TImageList *ImageListhm;
        TLabel *Label27;
        TLabel *Label28;
        TEdit *EditaeDc;
        TEdit *EditStKc;
        TEdit *EditStGamma0;
        TLabel *Label32;
        TLabel *Label33;
        TImage *Image4;
        TLabel *Label31;
        TGroupBox *GroupBox7;
        TImage *Image1;
        TEdit *EditStn;
        TLabel *Label4;
        TImage *Image2;
        TEdit *EditStVc;
        TLabel *Label5;
        TLabel *Labelfz;
        TEdit *Basefz;
        TBitBtn *BitBtnSt1;
        TBitBtn *BitBtnSt2;
        TBitBtn *BitBtnSt3;
        TBitBtn *BitBtnSt4;
        TBitBtn *BitBtnSt5;
        TMainMenu *MainMenu1;
        TMenuItem *NClose;
        TMenuItem *N6;
        TMenuItem *N7;
        TMenuItem *N8;
        TMenuItem *N9;
        TMenuItem *N10;
        TLabel *Label17;
        TMenuItem *N11;
        TBitBtn *BitBtnHelp;
        TBitBtn *BitCalcs;
        TImage *Image9;
        TImage *Image10;
        TMenuItem *N12;
        TMenuItem *N13;
        TTabSheet *TabSheetTechTi;
        TLabel *Label36;
        TComboBox *ComboBoxRa;
        TGroupBox *GroupBox2;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label15;
        TLabel *Label16;
        TEdit *EditSz;
        TEdit *EditV1;
        TEdit *EditV2;
        TEdit *EditTm;
        TEdit *Editn;
        TEdit *EditF;
        TEdit *EditPz;
        TEdit *EditMz;
        TEdit *EditNp;
        TComboBox *EditV;
        TImage *Image13;
        TImage *Image14;
        TImage *Image15;
        TComboBox *ComboBoxTkpd;
        TLabel *Label48;
        TImage *Image18;
        TImage *Image19;
        TBitBtn *BitBtnPanelAl;
        TMenuItem *N14;
        TMenuItem *N15;
        TMenuItem *N16;
        TMenuItem *N17;
        TMenuItem *N18;
        TMenuItem *N19;
        TPanel *PanelFormulaAl;
        TGroupBox *GroupBox11;
        TImage *Image11;
        TLabel *Label42;
        TLabel *Label39;
        TLabel *Label40;
        TLabel *Label41;
        TLabel *Label43;
        TEdit *EditTkoef1;
        TEdit *EditTX1;
        TEdit *EditTY1;
        TEdit *EditTZ1;
        TEdit *EditTKs;
        TBitBtn *BitBtn1fz;
        TGroupBox *GroupBox10;
        TImage *Image12;
        TLabel *Label44;
        TLabel *Label46;
        TLabel *Label47;
        TLabel *Label45;
        TEdit *EditTkoef2;
        TEdit *EditTX2;
        TEdit *EditTY2;
        TEdit *EditTZ2;
        TBitBtn *BitBtn2V1;
        TGroupBox *GroupBox9;
        TImage *Image16;
        TLabel *Label35;
        TLabel *Label49;
        TLabel *Label50;
        TLabel *Label51;
        TLabel *Label52;
        TEdit *EditTkoef3;
        TEdit *EditTX3;
        TEdit *EditTY3;
        TEdit *EditTZ3;
        TEdit *EditTsm1;
        TBitBtn *BitBtn3V2;
        TGroupBox *GroupBox8;
        TLabel *Label53;
        TLabel *Label54;
        TLabel *Label55;
        TLabel *Label56;
        TLabel *Label57;
        TImage *Image17;
        TEdit *EditTkoef4;
        TEdit *EditTX4;
        TEdit *EditTY4;
        TEdit *EditTZ4;
        TEdit *EditTsm2;
        TBitBtn *BitBtn4P;
        TMenuItem *N20;
        TMenuItem *N21;
        TPanel *PanelStMaterial;
        TGroupBox *GroupBoxStMaterial;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TRadioButton *RadioButton3;
        TRadioButton *RadioButton4;
        TRadioButton *RadioButton5;
        TStringGrid *StringGridTab;
        TMemo *Memo1;
        TMenuItem *N22;
        TBitBtn *BitBtnStMaterialOut;
        TStaticText *StaticTextMaterial;
        TGroupBox *GroupBox12;
        TLabel *Label37;
        TEdit *EditK;
        TLabel *Label38;
        TEdit *EdittD;
        TBitBtn *BitBtnAlTm1;
        TBitBtn *BitBtnAln1;
        TBitBtn *BitBtnAlF1;
        TBitBtn *BitBtnAlPz1;
        TBitBtn *BitBtnAlMz1;
        TBitBtn *BitBtnAlNp1;
        TPanel *PanelTemp;
        TStaticText *StaticText1;
        TLabel *Label58;
        TStringGrid *StringGridT;
        TLabel *Label59;
        TMenuItem *N23;
        TMenuItem *N24;
        TPanel *PanelForestP;
        TStringGrid *StringGridP;
        TStaticText *StaticText2;
        TGroupBox *GroupBox13;
        TLabel *Label34;
        TEdit *EditStKc11;
        TEdit *EditStMc;
        TLabel *Label29;
        TLabel *Label30;
        TBitBtn *BitBtnStMaterial;
        TStaticText *StaticTextStMaterial;
        TMenuItem *N5;
        TMenuItem *N25;
        TRadioGroup *RadioGroupTiMaterial;
        TRadioGroup *RadioGroupTiCoolnt;
        TRadioGroup *RadioGroupTiSkin;
        TRadioGroup *RadioGroupTiSurface;
        TRadioGroup *RadioGroupTiTypeDetal;
        TComboBox *ComboBoxTiTime;
        TLabel *Label24;
        TImageList *ImageListTiDir;
        TMenuItem *N26;
        TMenuItem *NMesWin;
        void __fastcall BitCalcsClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall N1Click(TObject *Sender);
        void __fastcall N2Click(TObject *Sender);
        void __fastcall N4Click(TObject *Sender);
        void __fastcall N11Click(TObject *Sender);
        void __fastcall N13Click(TObject *Sender);
        void __fastcall TabMachChange(TObject *Sender);
        void __fastcall PageControlMethodsChange(TObject *Sender);
        void __fastcall ComboBoxStContactChange(TObject *Sender);
        void __fastcall CheckBoxhm2Click(TObject *Sender);
        void __fastcall BitBtnStMaterialClick(TObject *Sender);
        void __fastcall BitBtnHelpClick(TObject *Sender);
        void __fastcall BitBtnSt1Click(TObject *Sender);
        void __fastcall BitBtnSt2Click(TObject *Sender);
        void __fastcall BitBtnSt4Click(TObject *Sender);
        void __fastcall BitBtnSt3Click(TObject *Sender);
        void __fastcall BitBtnSt5Click(TObject *Sender);
        void __fastcall Image9Click(TObject *Sender);
        void __fastcall Image10Click(TObject *Sender);
        void __fastcall ComboBoxMaterialChange(TObject *Sender);
        void __fastcall RadioButton1Click(TObject *Sender);
        void __fastcall RadioButton2Click(TObject *Sender);
        void __fastcall RadioButton3Click(TObject *Sender);
        void __fastcall RadioButton4Click(TObject *Sender);
        void __fastcall RadioButton5Click(TObject *Sender);
        void __fastcall BitBtnStMaterialOutClick(TObject *Sender);
        void __fastcall StringGridTabSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall BitBtnPanelAlClick(TObject *Sender);
        void __fastcall ComboBoxRaChange(TObject *Sender);
        void __fastcall BitBtnAlTm1Click(TObject *Sender);
        void __fastcall BitBtnAln1Click(TObject *Sender);
        void __fastcall BitBtnAlF1Click(TObject *Sender);
        void __fastcall BitBtnAlPz1Click(TObject *Sender);
        void __fastcall BitBtnAlMz1Click(TObject *Sender);
        void __fastcall BitBtnAlNp1Click(TObject *Sender);
        void __fastcall BitBtn1fzClick(TObject *Sender);
        void __fastcall BitBtn2V1Click(TObject *Sender);
        void __fastcall BitBtn3V2Click(TObject *Sender);
        void __fastcall BitBtn4PClick(TObject *Sender);
        void __fastcall EditVChange(TObject *Sender);
        void __fastcall N25Click(TObject *Sender);
        void __fastcall NMesWinClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        void CalcStandart(TObject *Sender);
        void CalcTechAl(TObject *Sender);
        int CalcTechAlSz(TObject *Sender);
        int CalcTechAlV1(TObject *Sender);
        int CalcTechAlV2(TObject *Sender);
        int CalcTechAlV0(TObject *Sender);
        int CalcTechAlTemp(TObject *Sender);
        int CalcTechAln(TObject *Sender);
        int CalcTechAlF(TObject *Sender);
        int CalcTechAlPz(TObject *Sender);
        int CalcTechAlMz(TObject *Sender);
        int CalcTechAlNct(TObject *Sender);
        void CalcTechTi(TObject *Sender);
        void OutputMessage(AnsiString s);
        __fastcall TFormMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
