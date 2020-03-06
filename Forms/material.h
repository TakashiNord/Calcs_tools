//---------------------------------------------------------------------------
#ifndef materialH
#define materialH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class TFormMaterial : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *StringGridTab;
        TMemo *Memo1;
        TBitBtn *BitBtn1;
        TGroupBox *GroupBox1;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TRadioButton *RadioButton3;
        TRadioButton *RadioButton4;
        TRadioButton *RadioButton5;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall RadioButton1Click(TObject *Sender);
        void __fastcall RadioButton2Click(TObject *Sender);
        void __fastcall RadioButton3Click(TObject *Sender);
        void __fastcall RadioButton4Click(TObject *Sender);
        void __fastcall RadioButton5Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormMaterial(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMaterial *FormMaterial;
//---------------------------------------------------------------------------
#endif
