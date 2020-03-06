//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <Buttons.hpp>
#include "ElPopBtn.hpp"
#include "ElPanel.hpp"
#include "ElToolBar.hpp"
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet2;
        TTabSheet *TabSheet3;
        TImageList *ImageList1;
        TPageControl *PageControl2;
        TTabSheet *TabSheet9;
        TTabSheet *TabSheet10;
        TTabSheet *TabSheet11;
        TTabControl *TabControl1;
        TElGraphicButton *ElGraphicButton1;
        TElToolBar *ElToolBar1;
        TElToolBar *ElToolBar2;
        TElToolButton *ElToolButton1;
        TElToolButton *ElToolButton2;
        TElToolButton *ElToolButton3;
        TElToolButton *ElToolButton4;
        TElToolButton *ElToolButton5;
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
