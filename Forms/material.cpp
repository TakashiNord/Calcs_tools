//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "material.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMaterial *FormMaterial;
//---------------------------------------------------------------------------
__fastcall TFormMaterial::TFormMaterial(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMaterial::FormCreate(TObject *Sender)
{
 int i,j;
 for (i=0;i<4;i++) for (j=0;j<9;j++) StringGridTab->Cells[i][j]="";

 RadioButton1->Checked=True;
 RadioButton2->Checked=False;
 RadioButton3->Checked=False;
 RadioButton4->Checked=False;
 RadioButton5->Checked=False;

 RadioButton1Click(Sender);

 StringGridTab->Cells[0][0]="Материал";
 StringGridTab->Cells[1][0]="Rm (H/(mm*mm)" ;
 StringGridTab->Cells[2][0]="kc1.1 (H/(mm*mm)" ;
 StringGridTab->Cells[3][0]="Mc";

}
//---------------------------------------------------------------------------

void __fastcall TFormMaterial::BitBtn1Click(TObject *Sender)
{
   int i ; AnsiString s;
   i=StringGridTab->Row  ;
   s=IntToStr(i) + " kc=" + StringGridTab->Cells[2][i] + " mc=" + StringGridTab->Cells[3][i] ; ShowMessage(s);

  try
  {
   s=StringGridTab->Cells[2][i];
   //FormMaterial->ActiveControl=FormMain->EditStKc11;
   FormMain->EditStKc11->Text = s;
//   FormMain->EditStMc->Text= StringGridTab->Cells[3][i] ;
  }
  catch (...)
  {

  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMaterial::RadioButton1Click(TObject *Sender)
{
  if (RadioButton1->Checked==True) {

   int i,j; for (i=0;i<4;i++) for (j=1;j<9;j++) StringGridTab->Cells[i][j]="";

   StringGridTab->Font->Color= clBlue;

   // сталь P
 StringGridTab->Cells[0][1]="Низкоуглеродистая мягкая сталь; ферритная сталь с низкой прочностью на растяжение";
 StringGridTab->Cells[1][1]="<450" ;
 StringGridTab->Cells[2][1]="1350" ;
 StringGridTab->Cells[3][1]="0.21";

 StringGridTab->Cells[0][2]="Низкоуглеродистая автоматная сталь";
 StringGridTab->Cells[1][2]="400<700" ;
 StringGridTab->Cells[2][2]="1500" ;
 StringGridTab->Cells[3][2]="0.22";

 StringGridTab->Cells[0][3]="Нормальная конструкционная сталь, содержание углерода от низкого до среднего (<0.5%C)";
 StringGridTab->Cells[1][3]="450<550" ;
 StringGridTab->Cells[2][3]="1500" ;
 StringGridTab->Cells[3][3]="0.25";

 StringGridTab->Cells[0][4]="Нормальная, низколегированнная сталь и стальные отливки; отпущенная сталь; углеродистая сталь (>0.5%С);ферритная и мартенситная нержавеющая сталь";
 StringGridTab->Cells[1][4]="550<700" ;
 StringGridTab->Cells[2][4]="1700" ;
 StringGridTab->Cells[3][4]="0.24";

 StringGridTab->Cells[0][5]="Нормальная инструментальная сталь; закаленная отпуском сталь;мартенситная, нержавеющая сталь";
 StringGridTab->Cells[1][5]="700<900" ;
 StringGridTab->Cells[2][5]="1900" ;
 StringGridTab->Cells[3][5]="0.24";

 StringGridTab->Cells[0][6]="Инструментальная сталь с тяжелыми свойствами резания; твердая, высоколегированная сталь и стальные отливки; мартенситная, нержавеющая сталь";
 StringGridTab->Cells[1][6]="900<1200" ;
 StringGridTab->Cells[2][6]="2000" ;
 StringGridTab->Cells[3][6]="0.24";

 StringGridTab->Cells[0][7]="Высокопрочная сталь с тяжелыми свойствами резания; закаленная сталь групп 3-6; мартенситная, нержавеющая сталь";
 StringGridTab->Cells[1][7]=">1200" ;
 StringGridTab->Cells[2][7]="2900" ;
 StringGridTab->Cells[3][7]="0.22";

  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMaterial::RadioButton2Click(TObject *Sender)
{
  if (RadioButton2->Checked==True) {

  int i,j; for (i=0;i<4;i++) for (j=1;j<9;j++) StringGridTab->Cells[i][j]="";

  StringGridTab->Font->Color= clPurple;

   // Нержавеющая сталь M
 StringGridTab->Cells[0][1]="Нержавеющая сталь с менее тяжелыми свойствами резания";
 StringGridTab->Cells[1][1]="" ;
 StringGridTab->Cells[2][1]="1750" ;
 StringGridTab->Cells[3][1]="0.22";

 StringGridTab->Cells[0][2]="Молибденовая нержавеющая сталь; аустенитная и дуплексная, с тяжелыми свойствами резания";
 StringGridTab->Cells[1][2]="" ;
 StringGridTab->Cells[2][2]="1900" ;
 StringGridTab->Cells[3][2]="0.20";

 StringGridTab->Cells[0][3]="Аустенитная и дуплексная, с тяжелыми свойствами резания";
 StringGridTab->Cells[1][3]="" ;
 StringGridTab->Cells[2][3]="2050" ;
 StringGridTab->Cells[3][3]="0.20";

 StringGridTab->Cells[0][4]="Аустенитная и дуплексная, с исключительно тяжелыми свойствами резания";
 StringGridTab->Cells[1][4]="" ;
 StringGridTab->Cells[2][4]="2150" ;
 StringGridTab->Cells[3][4]="0.20";

  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMaterial::RadioButton3Click(TObject *Sender)
{
  if (RadioButton3->Checked==True) {

  int i,j; for (i=0;i<4;i++) for (j=1;j<9;j++) StringGridTab->Cells[i][j]="";

   StringGridTab->Font->Color= clRed;

   // Литейный чугун K
 StringGridTab->Cells[0][1]="Литейный чугун средней твердости, серый литейный чугун";
 StringGridTab->Cells[1][1]="" ;
 StringGridTab->Cells[2][1]="1150" ;
 StringGridTab->Cells[3][1]="0.22";

 StringGridTab->Cells[0][2]="Низколегированный литейный чугун, ковкий литейный чугун, литейный чугун с шаровидным графитом";
 StringGridTab->Cells[1][2]="" ;
 StringGridTab->Cells[2][2]="1225" ;
 StringGridTab->Cells[3][2]="0.25";

 StringGridTab->Cells[0][3]="Легированный чугун средней твердости, ковкий литейный чугун,GGG, средние свойства резания";
 StringGridTab->Cells[1][3]="" ;
 StringGridTab->Cells[2][3]="1350" ;
 StringGridTab->Cells[3][3]="0.28";

 StringGridTab->Cells[0][4]="Высоколегированный литейный чугун с тяжелыми свойствами резания; ковкий литейный чугун,GGG, тяжелые свойства резания";
 StringGridTab->Cells[1][4]="" ;
 StringGridTab->Cells[2][4]="1450" ;
 StringGridTab->Cells[3][4]="0.30";

  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMaterial::RadioButton4Click(TObject *Sender)
{
  if (RadioButton4->Checked==True) {

  int i,j; for (i=0;i<4;i++) for (j=1;j<9;j++) StringGridTab->Cells[i][j]="";

  StringGridTab->Font->Color= clGreen;

   // Цветные металлы N
 StringGridTab->Cells[0][1]="Сплавы цветных металлов, легкие для обработки; алюминий с содержанием<16%Si, латуни, цинка и магния";
 StringGridTab->Cells[1][1]="" ;
 StringGridTab->Cells[2][1]="700" ;
 StringGridTab->Cells[3][1]="0.25";

 StringGridTab->Cells[0][2]="Сплавы цветных металлов, трудные для обработки; алюминий с содержанием>16%Si, бронзы, меди, алюминиевые сплавы (никель, медь, магния)";
 StringGridTab->Cells[1][2]="" ;
 StringGridTab->Cells[2][2]="700" ;
 StringGridTab->Cells[3][2]="0.27";

  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMaterial::RadioButton5Click(TObject *Sender)
{
  if (RadioButton5->Checked==True) {

  int i,j; for (i=0;i<4;i++) for (j=1;j<9;j++) StringGridTab->Cells[i][j]="";

  StringGridTab->Font->Color= clOlive;

   // Жаропрочные сплавы и титановые сплавы S
 StringGridTab->Cells[0][1]="Жаропрочные сплавы, содержащие никель, кобальт, железо, твердость<30HRc, инколой 800 и инконеь 601, 617 и 625,монель 400";
 StringGridTab->Cells[1][1]="" ;
 StringGridTab->Cells[2][1]="2600" ;
 StringGridTab->Cells[3][1]="0.24";

 StringGridTab->Cells[0][2]="Жаропрочные сплавы, содержащие никель, кобальт, железо, твердость>30HRc, инколой 718 и инконель 750-Х, и инконол 925, монель К-5008";
 StringGridTab->Cells[1][2]="" ;
 StringGridTab->Cells[2][2]="3300" ;
 StringGridTab->Cells[3][2]="0.24";

 StringGridTab->Cells[0][3]="Титановые сплавы, Ti-6Al-4V";
 StringGridTab->Cells[1][3]="" ;
 StringGridTab->Cells[2][3]="1450" ;
 StringGridTab->Cells[3][3]="0.23";

  }
}
//---------------------------------------------------------------------------

