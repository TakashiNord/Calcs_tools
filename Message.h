//---------------------------------------------------------------------------

#ifndef MessageH
#define MessageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ElCaption.hpp"
#include "ElACtrls.hpp"
//---------------------------------------------------------------------------
class TFormMessage : public TForm
{
__published:	// IDE-managed Components
        TElFormCaption *ElFormCaption1;
        TElFlatMemo *ElFlatMemoMes;
        void __fastcall ElFormCaption1Buttons0Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormMessage(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMessage *FormMessage;
//---------------------------------------------------------------------------
#endif
