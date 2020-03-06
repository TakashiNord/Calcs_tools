//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Graphic.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormGraphic *FormGraphic;
//---------------------------------------------------------------------------
__fastcall TFormGraphic::TFormGraphic(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormGraphic::ToolButton1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormGraphic::ToolButton2Click(TObject *Sender)
{
   Series3->Clear();
   Series4->Clear();      
}
//---------------------------------------------------------------------------
