//---------------------------------------------------------------------------

#ifndef GraphicH
#define GraphicH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Chart.hpp>
#include <ExtCtrls.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TFormGraphic : public TForm
{
__published:	// IDE-managed Components
        TToolBar *ToolBar1;
        TToolButton *ToolButton1;
        TToolButton *ToolButton2;
        TChart *ChartNom;
        TLineSeries *Series1;
        TLineSeries *Series2;
        TPointSeries *Series3;
        TPointSeries *Series4;
        void __fastcall ToolButton1Click(TObject *Sender);
        void __fastcall ToolButton2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormGraphic(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormGraphic *FormGraphic;
//---------------------------------------------------------------------------
#endif
