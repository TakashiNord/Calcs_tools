//---------------------------------------------------------------------------

#ifndef graphicH
#define graphicH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ElCGControl.hpp"
#include "ElGraphs.hpp"
//---------------------------------------------------------------------------
class TFormGraphic : public TForm
{
__published:	// IDE-managed Components
        TElGraph *ElGraph1;
private:	// User declarations
public:		// User declarations
        __fastcall TFormGraphic(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormGraphic *FormGraphic;
//---------------------------------------------------------------------------
#endif
