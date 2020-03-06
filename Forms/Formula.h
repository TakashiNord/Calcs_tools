//---------------------------------------------------------------------------

#ifndef FormulaH
#define FormulaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormulaFormAl : public TForm
{
__published:	// IDE-managed Components
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
        TBitBtn *BitBtn1fz;
        TBitBtn *BitBtn2V1;
        TBitBtn *BitBtn3V2;
        TBitBtn *BitBtn4P;
private:	// User declarations
public:		// User declarations
        __fastcall TFormulaFormAl(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormulaFormAl *FormulaFormAl;
//---------------------------------------------------------------------------
#endif
