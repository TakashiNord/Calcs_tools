//---------------------------------------------------------------------------

#ifndef temperatureH
#define temperatureH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TTemperatureForm : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *StringGridT;
        TLabel *Label1;
        TLabel *Label2;
        TStaticText *StaticText1;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TTemperatureForm(TComponent* Owner);
        void setParam(double tV0, double tF);
        double F0;
        double V0;
};
//---------------------------------------------------------------------------
extern PACKAGE TTemperatureForm *TemperatureForm;
//---------------------------------------------------------------------------
#endif
