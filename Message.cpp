//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Message.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ElCaption"
#pragma link "ElACtrls"
#pragma resource "*.dfm"
TFormMessage *FormMessage;
//---------------------------------------------------------------------------
__fastcall TFormMessage::TFormMessage(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMessage::ElFormCaption1Buttons0Click(TObject *Sender)
{
   ElFlatMemoMes->Lines->Clear();     
}
//---------------------------------------------------------------------------
