//---------------------------------------------------------------------------

#ifndef MiniatureH
#define MiniatureH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TFormMiniature : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TTimer *Timer1;
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormMiniature(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMiniature *FormMiniature;
//---------------------------------------------------------------------------
#endif
