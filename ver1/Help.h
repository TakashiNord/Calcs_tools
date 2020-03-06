//---------------------------------------------------------------------------

#ifndef HelpH
#define HelpH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TFormHelp : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
private:	// User declarations
public:		// User declarations
        __fastcall TFormHelp(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormHelp *FormHelp;
//---------------------------------------------------------------------------
#endif
