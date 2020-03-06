//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Miniature.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMiniature *FormMiniature;
//---------------------------------------------------------------------------
__fastcall TFormMiniature::TFormMiniature(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMiniature::Timer1Timer(TObject *Sender)
{
     Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormMiniature::FormCreate(TObject *Sender)
{
    Show();        
}
//---------------------------------------------------------------------------
