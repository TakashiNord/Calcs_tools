//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "temperature.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTemperatureForm *TemperatureForm;
//---------------------------------------------------------------------------
__fastcall TTemperatureForm::TTemperatureForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TTemperatureForm::FormCreate(TObject *Sender)
{

 int i,j;

 long int Temperature[10][9]={
 {0,500,1000,1500,2000,2500,3000,3500,4000},
 {1200,150,175,191,204,214,223,230,237},
 {2400,108,125,137,146,153,160,165,170},
 {3600,89,104,112,119,125,130,134,139},
 {4800,77,90,98,105,110,114,118,122},
 {6000,69,81,88,94,99,103,107,111},
 {7200,64,74,81,87,91,95,99,102},
 {9600,55,64,70,75,79,82,85,87},
 {10800,52,60,65,70,73,77,79,81},
 {12000,49,57,62,66,69,72,74,76},
} ;

 for(i=0;i<10;i++)  for(j=0;j<9;j++) { StringGridT->Cells[j][i]=IntToStr(Temperature[i][j]) ; }
 StringGridT->Cells[0][0]="";

 //setParam(0., 0.);

/* i=1;
 int value;
 value=500;
 for(i=1;i<9;i++) { StringGridT->Cells[i][0]=IntToStr(value) ;  value+=500; }

 value=1200;
 for(i=1;i<7;i++) { StringGridT->Cells[0][i]=IntToStr(value) ;  value+=1200; }
 value=9600; StringGridT->Cells[0][7]=IntToStr(value) ;
 value=10800; StringGridT->Cells[0][8]=IntToStr(value) ;
 value=12000; StringGridT->Cells[0][9]=IntToStr(value) ;

 i=1;
 value=150; StringGridT->Cells[1][i]=IntToStr(value) ;
 value=175; StringGridT->Cells[2][i]=IntToStr(value) ;
 value=191; StringGridT->Cells[3][i]=IntToStr(value) ;
 value=204; StringGridT->Cells[4][i]=IntToStr(value) ;
 value=214; StringGridT->Cells[5][i]=IntToStr(value) ;
 value=223; StringGridT->Cells[6][i]=IntToStr(value) ;
 value=230; StringGridT->Cells[7][i]=IntToStr(value) ;
 value=237; StringGridT->Cells[8][i]=IntToStr(value) ;

 i=2;
 value=108; StringGridT->Cells[1][i]=IntToStr(value) ;
 value=125; StringGridT->Cells[2][i]=IntToStr(value) ;
 value=137; StringGridT->Cells[3][i]=IntToStr(value) ;
 value=146; StringGridT->Cells[4][i]=IntToStr(value) ;
 value=153; StringGridT->Cells[5][i]=IntToStr(value) ;
 value=160; StringGridT->Cells[6][i]=IntToStr(value) ;
 value=165; StringGridT->Cells[7][i]=IntToStr(value) ;
 value=170; StringGridT->Cells[8][i]=IntToStr(value) ;

 i=3;
 value=89; StringGridT->Cells[1][i]=IntToStr(value) ;
 value=104; StringGridT->Cells[2][i]=IntToStr(value) ;
 value=112; StringGridT->Cells[3][i]=IntToStr(value) ;
 value=119; StringGridT->Cells[4][i]=IntToStr(value) ;
 value=125; StringGridT->Cells[5][i]=IntToStr(value) ;
 value=130; StringGridT->Cells[6][i]=IntToStr(value) ;
 value=134; StringGridT->Cells[7][i]=IntToStr(value) ;
 value=139; StringGridT->Cells[8][i]=IntToStr(value) ;

 i=4;
 value=77; StringGridT->Cells[1][i]=IntToStr(value) ;
 value=90; StringGridT->Cells[2][i]=IntToStr(value) ;
 value=98; StringGridT->Cells[3][i]=IntToStr(value) ;
 value=105; StringGridT->Cells[4][i]=IntToStr(value) ;
 value=110; StringGridT->Cells[5][i]=IntToStr(value) ;
 value=114; StringGridT->Cells[6][i]=IntToStr(value) ;
 value=118; StringGridT->Cells[7][i]=IntToStr(value) ;
 value=122; StringGridT->Cells[8][i]=IntToStr(value) ;

 i=5;
 value=69; StringGridT->Cells[1][i]=IntToStr(value) ;
 value=81; StringGridT->Cells[2][i]=IntToStr(value) ;
 value=88; StringGridT->Cells[3][i]=IntToStr(value) ;
 value=94; StringGridT->Cells[4][i]=IntToStr(value) ;
 value=99; StringGridT->Cells[5][i]=IntToStr(value) ;
 value=103; StringGridT->Cells[6][i]=IntToStr(value) ;
 value=107; StringGridT->Cells[7][i]=IntToStr(value) ;
 value=111; StringGridT->Cells[8][i]=IntToStr(value) ;

 i=6;
 value=64; StringGridT->Cells[1][i]=IntToStr(value) ;
 value=74; StringGridT->Cells[2][i]=IntToStr(value) ;
 value=81; StringGridT->Cells[3][i]=IntToStr(value) ;
 value=87; StringGridT->Cells[4][i]=IntToStr(value) ;
 value=91; StringGridT->Cells[5][i]=IntToStr(value) ;
 value=95; StringGridT->Cells[6][i]=IntToStr(value) ;
 value=99; StringGridT->Cells[7][i]=IntToStr(value) ;
 value=102; StringGridT->Cells[8][i]=IntToStr(value) ;

 i=7;
 value=55; StringGridT->Cells[1][i]=IntToStr(value) ;
 value=64; StringGridT->Cells[2][i]=IntToStr(value) ;
 value=70; StringGridT->Cells[3][i]=IntToStr(value) ;
 value=75; StringGridT->Cells[4][i]=IntToStr(value) ;
 value=79; StringGridT->Cells[5][i]=IntToStr(value) ;
 value=82; StringGridT->Cells[6][i]=IntToStr(value) ;
 value=85; StringGridT->Cells[7][i]=IntToStr(value) ;
 value=87; StringGridT->Cells[8][i]=IntToStr(value) ;

 i=8;
 value=52; StringGridT->Cells[1][i]=IntToStr(value) ;
 value=60; StringGridT->Cells[2][i]=IntToStr(value) ;
 value=65; StringGridT->Cells[3][i]=IntToStr(value) ;
 value=70; StringGridT->Cells[4][i]=IntToStr(value) ;
 value=73; StringGridT->Cells[5][i]=IntToStr(value) ;
 value=77; StringGridT->Cells[6][i]=IntToStr(value) ;
 value=79; StringGridT->Cells[7][i]=IntToStr(value) ;
 value=81; StringGridT->Cells[8][i]=IntToStr(value) ;

 i=9;
 value=49; StringGridT->Cells[1][i]=IntToStr(value) ;
 value=57; StringGridT->Cells[2][i]=IntToStr(value) ;
 value=62; StringGridT->Cells[3][i]=IntToStr(value) ;
 value=66; StringGridT->Cells[4][i]=IntToStr(value) ;
 value=69; StringGridT->Cells[5][i]=IntToStr(value) ;
 value=72; StringGridT->Cells[6][i]=IntToStr(value) ;
 value=74; StringGridT->Cells[7][i]=IntToStr(value) ;
 value=76; StringGridT->Cells[8][i]=IntToStr(value) ;
*/

}

//---------------------------------------------------------------------------
void TTemperatureForm::setParam(double tV0, double tF)
{
   V0=tV0;
   F0=tF;
}

//---------------------------------------------------------------------------


void __fastcall TTemperatureForm::FormShow(TObject *Sender)
{
/*  int i,j;
  int iv,ifv;

 long int Temperature[10][9]={
 {0,500,1000,1500,2000,2500,3000,3500,4000},
 {1200,150,175,191,204,214,223,230,237},
 {2400,108,125,137,146,153,160,165,170},
 {3600,89,104,112,119,125,130,134,139},
 {4800,77,90,98,105,110,114,118,122},
 {6000,69,81,88,94,99,103,107,111},
 {7200,64,74,81,87,91,95,99,102},
 {9600,55,64,70,75,79,82,85,87},
 {10800,52,60,65,70,73,77,79,81},
 {12000,49,57,62,66,69,72,74,76},
} ;

  for(i=1;i<9;i++) { if (V0>=Temperature[0][i]) { iv=i+1; } }
  for(i=1;i<10;i++) { if (F0>=Temperature[i][0]) ifv=i+1; }
  if (iv>8) iv=8;
  if (ifv>9) ifv=9;

  bool CanSelect=True;
  StringGridT->OnSelectCell(Sender, ifv, iv,CanSelect); */
}
//---------------------------------------------------------------------------

