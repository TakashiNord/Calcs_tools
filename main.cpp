//---------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "about.h"
#include "Graphic.h"
#include "Help.h"
#include "Message.h"

#define PI  3.1415926
#define NUM_MACHINE  5

 struct DEF_MACHINE {
     double MAXSpindel ; // S(об/мин)
     double MAXFeed    ; // F(мм/мин)
     double Power      ; // Nct(kW)
     double Torque     ; // Mz(Hm)
     double CriticSpindel ; // Sc(об/мин)
     char   machine_name[250] ;
     double SPw[2][20]  ; // Точки графика (S,Pw)
     double SMz[2][20]  ; // Точки графика (S,Mz)
 } ;

 struct DEF_MACHINE MACHINE[NUM_MACHINE];

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
 {12000,49,57,62,66,69,72,74,76}
 } ;

 double coeffTi[6][6]={
 {10,16,25,40,50,-1},
 {1.5,2.0,2.5,3.0,3.5,1.0},
 {2.0,2.5,3.0,3.5,4.0,0.9},
 {2.5,3.0,3.5,4.0,4.5,0.8},
 {3.0,3.5,4.0,4.5,5.0,0.7},
 {3.5,4.0,4.5,5.0,6.0,0.6}
 } ;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void TFormMain::OutputMessage(AnsiString s)
{
    FormMessage->ElFlatMemoMes->Lines->Append(s);
    if (NMesWin->Checked==True) FormMessage->Visible=True;
}
//---------------------------------------------------------------------------


int TFormMain::CalcStnV(TObject *Sender)
{
  char s[80]; String str;
  int mach; // станок
  double D; //исходные данные
  double tn,tvc; // рассчитываемые параметры : ..., мощность

  mach=TabMach->ItemIndex;

 /* Проверка на правильность ввода числовых значений*/
  /*диаметр*/
  str=BaseD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=0.;
    s[0]='\0'; sprintf(s,"%.2lf",D); BaseD->Text=s;
    BaseD->SetFocus(); BaseD->SelectAll();
    OutputMessage("Ошибка задания 'диаметра инструмента' - D");
    Application->MessageBox("Ошибка задания 'диаметра инструмента' - D","Предупреждение...",MB_OK);
    return (-1);
  }

  /*обороты */
  str=EditStn->Text;
  tn=atof(str.c_str());
  if (tn<=0.) {
    tn=0;
    s[0]='\0'; sprintf(s,"%.3lf",tn); EditStn->Text=s;
    EditStn->SetFocus(); EditStn->SelectAll();
    OutputMessage("Ошибка задания оборотов шпинделя...<=0");
  }

  // Расчет скорости резания
  tvc=(D*PI*tn)/1000.;
  s[0]='\0'; sprintf(s,"%.3lf",tvc);  EditStVc->Text=s;
  EditStVc->SetFocus(); EditStVc->SelectAll();

  EditStn->Font->Color=clGreen; // clRed; clGreen
  if (tn>=MACHINE[mach].MAXSpindel) {
    EditStn->Font->Color=clRed;
    OutputMessage("Превышение числа оборотов");
  }

  return (0);
}
//---------------------------------------------------------------------------

int TFormMain::CalcStQ(TObject *Sender)
{
  char s[80]; String str;
  int mach; // станок
  double ap,ae; //исходные данные
  double tvf,Q; // рассчитываемые параметры : ..., мощность

  /*Ширина резания*/
  str=BaseB->Text;
  ap=atof(str.c_str());
  if (ap<=0.) {
    ap=0.;
    s[0]='\0'; sprintf(s,"%.3lf",ap); BaseB->Text=s;
    BaseB->SetFocus(); BaseB->SelectAll();
    OutputMessage("Ошибка задания ширины реза...<=0");
  }

  /*Глубина реза*/
  str=BaseT->Text;
  ae=atof(str.c_str());
  if (ae<=0.) {
    ae=0;
    s[0]='\0'; sprintf(s,"%.3lf",ae); BaseT->Text=s;
    BaseT->SetFocus(); BaseT->SelectAll();
    OutputMessage("Ошибка задания глубины реза...<=0");
  }

  /*подачи резания*/
  str=EditStVf->Text;
  tvf=atof(str.c_str());
  if (tvf<=0.) {
    tvf=0;
    s[0]='\0'; sprintf(s,"%.3lf",tvf); EditStVf->Text=s;
    EditStVf->SetFocus(); EditStVf->SelectAll();
    OutputMessage("Ошибка задания подачи реза...<=0");
  }

  // Скорость сьема материала
  Q=(ae*ap*tvf)/1000.;
  s[0]='\0'; sprintf(s,"%.5lf",Q); EditStQ->Text=s;

  return (0);
}
//---------------------------------------------------------------------------


int TFormMain::CalcSthm(TObject *Sender)
{
  double vd; int vi=0 ; char s[80];  String str;
  int mach; // станок
  double D,z,ap,ae,fz; //исходные данные
  double ak,fsi ; // Стружка
  double thm; // рассчитываемые параметры : ..., мощность

  /*диаметр*/
  str=BaseD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=0.;
    s[0]='\0'; sprintf(s,"%.3lf",D); BaseD->Text=s; BaseD->SetFocus(); BaseD->SelectAll();
    Application->MessageBox("Ошибка задания 'диаметра инструмента' - D","Предупреждение...",MB_OK);
    return(-1);
  }

  /*Глубина реза*/
  str=BaseT->Text;
  ae=atof(str.c_str());
  if (ae<=0.) {
    ae=0;
    s[0]='\0'; sprintf(s,"%.3lf",ae); BaseT->Text=s;
    BaseT->SetFocus(); BaseT->SelectAll();
    Application->MessageBox("Ошибка задания 'Глубина реза' - ae","Предупреждение...",MB_OK);
    return (-1);
  }

  /*Подача на зуб*/
  str=EditStfz->Text;
  fz=atof(str.c_str());
  if (fz<=0.) {
    fz=0;
    s[0]='\0'; sprintf(s,"%.5lf",fz); EditStfz->Text=s;
    EditStfz->SetFocus(); EditStfz->SelectAll();
    Application->MessageBox("Ошибка задания 'Подача на зуб' - fz","Предупреждение...",MB_OK);
  }

  // Средняя толщина стружки
  if (D<=0.) {
  	Application->MessageBox("Средняя толщина стружки \n D=0. \n Расчет прерван.","Предупреждение...",MB_OK);
  	return(-1) ;
  }

  vi=ComboBoxStContact->ItemIndex;
  fsi=0.1;
  if (vi==0) { fsi=180./PI*2.*asin(ae/D); } else { fsi=90.+180./PI*asin((ae-D/2.)/(D/2.)); }
  vd=ae/D;

  s[0]='\0'; sprintf(s,"%.2lf",vd); EditaeDc->Text=s;

  //if ((vd*0.3)<=30.) { CheckBoxhm2->Checked=True; CheckBoxhm2Click(Sender); }

  thm=0.1;
  if (CheckBoxhm2->Checked==True) {
     thm=fz*sqrt(ae/D);
  } else {
     /*Угол k*/
     str=EditStk->Text;
     ak=atof(str.c_str());
     if (ak<=-180. || ak>=180.) {
       ak=90.;
     }
     thm=114.7*fz*sin(ak*PI/180.)*(ae/D);
     thm=fabs(thm-floor(thm)); //thm=fabs(thm-ceil(thm));
     if (fsi<=0.) fsi=0.001;
     thm/=fsi;
  }

  s[0]='\0'; sprintf(s,"%.5lf",ak); EditStk->Text=s;
  s[0]='\0'; sprintf(s,"%.8lf",thm); EditSthm2->Text=s;

  EditSthm2->SetFocus(); EditSthm2->SelectAll(); EditSthm2->SelStart=0;
  //Расчет завершен

   return (0);
}
//---------------------------------------------------------------------------

int TFormMain::CalcStFSz(TObject *Sender)
{
  int vi=0 ; char s[80]; String str;
  int mach; // станок
  double z,fz,vf; //исходные данные
  double tn,tvf,tfz; // рассчитываемые параметры : ..., мощность

  mach=TabMach->ItemIndex;

  /*Подача на зуб*/
  str=EditStVf->Text;
  vf=atof(str.c_str());
  if (vf<=0.) {
    vf=0;
    s[0]='\0'; sprintf(s,"%.3lf",vf); EditStVf->Text=s;
    EditStVf->SetFocus(); EditStVf->SelectAll();
  }

  EditStVf->Font->Color=clGreen; // clRed; clGreen
  if (vf>=MACHINE[mach].MAXFeed) {
    EditStVf->Font->Color=clRed; // clRed; clGreen
    OutputMessage("Превышение максимального лимита рабочей подачи");
  }

  /*Подача на зуб*/
  str=Basefz->Text;
  fz=atof(str.c_str());
  if (fz<=0.) {
    fz=0;
    s[0]='\0'; sprintf(s,"%.5lf",fz);  Basefz->Text=s;
    Basefz->SetFocus(); Basefz->SelectAll();
  }

  /*Число зубьев*/
  str=BaseZ->Text;
  vi=atoi(str.c_str());
  if (vi<=0) {
    vi=0;
    s[0]='\0'; sprintf(s,"%d",vi); BaseZ->Text=s;
    BaseZ->SetFocus(); BaseZ->SelectAll();
    Application->MessageBox("Ошибка задания 'Числа зубьев' - z","Предупреждение...",MB_OK);
    return (-1);
  }
  z=(double)vi;

  /*Число оборотов*/
  str=EditStn->Text;
  tn=atof(str.c_str());
  if (tn<=0.) {
    tn=0.;
    s[0]='\0'; sprintf(s,"%.3lf",tn); EditStn->Text=s;
    EditStn->SetFocus(); EditStn->SelectAll();
  }

  // Расчет подачи на зуб
  if (tn<=0.) { Application->MessageBox("Расчет подачи на зуб \n n=0. \n Расчет прерван.","Предупреждение...",MB_OK); return (-1); }
  if (z<=0.) { Application->MessageBox("Расчет подачи на зуб \n z=0. \n Расчет прерван.","Предупреждение...",MB_OK); return (-1); }
  tfz=vf/(z*tn);
  s[0]='\0'; sprintf(s,"%.5lf",tfz);  EditStfz->Text=s;
  EditStfz->SetFocus(); EditStfz->SelectAll();

   return (0);
}
//---------------------------------------------------------------------------

int TFormMain::CalcStNctMz(TObject *Sender)
{
  char s[80]; String str;
  int mach; // станок
  double ap,ae,tn; //исходные данные
  double ak,fsi ; // Стружка
  double kpd,gamma0, kc11, mc ; // материал
  double tvf,thm,tkc,tNmot,tNp,tM; // рассчитываемые параметры : ..., мощность

  mach=TabMach->ItemIndex;

  /*Ширина резания*/
  str=BaseB->Text;
  ap=atof(str.c_str());
  if (ap<=0.) {
    ap=0.;
    s[0]='\0'; sprintf(s,"%.5lf",ap); BaseB->Text=s;
    BaseB->SetFocus(); BaseB->SelectAll();
    Application->MessageBox("Ошибка задания 'Ширины реза' - ap","Предупреждение...",MB_OK);
    return (-1);
  }

  /*Глубина реза*/
  str=BaseT->Text;
  ae=atof(str.c_str());
  if (ae<=0.) {
    ae=0;
    s[0]='\0'; sprintf(s,"%.5lf",ae); BaseT->Text=s;
    BaseT->SetFocus(); BaseT->SelectAll();
    OutputMessage("Ошибка задания 'Глубина реза' - ae<=0.");
  }

  /*Число оборотов*/
  str=EditStn->Text;
  tn=atof(str.c_str());
  if (tn<=0.) {
    tn=0.;
    s[0]='\0'; sprintf(s,"%.3lf",tn); EditStn->Text=s;
    EditStn->SetFocus(); EditStn->SelectAll();
    OutputMessage("Число оборотов шпинделя - n<=0.");
  }

  /*Средняя толщина стружки*/
  str=EditSthm2->Text;
  thm=atof(str.c_str());
  if (thm<=0.) {
    thm=0;
    s[0]='\0'; sprintf(s,"%.8lf",thm); EditSthm2->Text=s;
    EditSthm2->SetFocus(); EditSthm2->SelectAll();
    OutputMessage("Средняя толщина стружки - hm<=0.");
  }

  /*подачи резания*/
  str=EditStVf->Text;
  tvf=atof(str.c_str());
  if (tvf<=0.) {
    tvf=0;
    s[0]='\0'; sprintf(s,"%.5lf",tvf);  EditStVf->Text=s; EditStVf->SetFocus(); EditStVf->SelectAll();
    OutputMessage("Подача резания - vf<=0.");
  }

  // Расчет мощности резания
  str=ComboBoxStkpd->Text; kpd=atof(str.c_str());

  /*Главный передний угол*/
  str=EditStGamma0->Text;
  gamma0=atof(str.c_str());
  if (gamma0<=-100. || gamma0>=100.) {
    gamma0=2.;
    s[0]='\0'; sprintf(s,"%.2lf",gamma0); EditStGamma0->Text=s; EditStGamma0->SetFocus(); EditStGamma0->SelectAll();
    OutputMessage("Главный передний угол (гамма) - превышает лимит <=-100 || >=+100 -> установлен gamma=2");
  }
  /*Коэф материала kc11*/
  str=EditStKc11->Text;
  kc11=atof(str.c_str());
  if (kc11<=0.) {
    kc11=700.;
    s[0]='\0'; sprintf(s,"%.3lf",kc11);  EditStKc11->Text=s; EditStKc11->SetFocus(); EditStKc11->SelectAll();
    OutputMessage("Коэф материала kc11-> (kc11<=0) - установлен kc11=700. ");
  }
 /*if (kc11==700.) {
    OutputMessage("Для более точного вычисления режима резания - выберите нужный материал");
    OutputMessage("      установлен kc11=700 - материал=Сплавы Алюминия");
  }*/
  /*Коэф материала mc*/
  str=EditStMc->Text;
  mc=atof(str.c_str());
  if (mc<=0. || mc>1.) {
    mc=0.25;
    s[0]='\0'; sprintf(s,"%.3lf",mc);  EditStMc->Text=s; EditStMc->SetFocus(); EditStMc->SelectAll();
    OutputMessage("Коэф материала mc-> (mc<=0 или mc>1) - установлен mc=0.25 ");
  }
  /*if (mc==0.25) {
    OutputMessage("Для более точного вычисления режима резания - выберите нужный материал");
    OutputMessage("      установлен mc=0.25 - материал=Сплавы Алюминия");
  }*/
  tkc=0.;
  if (thm<=0.) {
    OutputMessage("Расчет мощности резания->Средняя толщина стружки\n hm<=0. \n Расчет прерван.");
    Application->MessageBox("Расчет мощности резания->Средняя толщина стружки \n hm=0. \n Расчет прерван.","Предупреждение...",MB_OK);
    return (-1);
  }
  tkc=(1.-0.01*gamma0)*kc11/pow(thm,mc );
  s[0]='\0'; sprintf(s,"%.5lf",tkc); EditStKc->Text=s;

  tNmot=0.;
  tNmot=ae*ap*tvf*tkc/(6.*10000000.*kpd);

  s[0]='\0'; sprintf(s,"%.5lf",tNmot); EditStP->Text=s;
  EditStP->Font->Color=clGreen; // clRed; clGreen
  EditStPn->Font->Color=clGreen;
  if (tNmot>=MACHINE[mach].Power) {
    EditStP->Font->Color=clRed; // clRed; clGreen
    EditStPn->Font->Color=clRed;
    OutputMessage("Превышение мощности станка");
  }

  tNp=tNmot;

  if (tn<=MACHINE[mach].CriticSpindel) {
    tM=MACHINE[mach].Torque;
    tNp=tM*(2.*3.1415926)*tn/60.;
    tNp/=1000.; // перевод из вт в квт
   } else {
    tNp=MACHINE[mach].Power;
    tM=tNp*1000.*60./(2*3.1415926*tn); // *1000. - перевод из квт в вт
  }

  s[0]='\0'; sprintf(s,"%.5lf",tNp); EditStPn->Text=s;
  EditStPn->Font->Color=clGreen;
  if (tNp>=MACHINE[mach].Power) {
    EditStPn->Font->Color=clRed;
    OutputMessage("Работа на пределе (или Превышение) - мощности станка");
  }

  s[0]='\0'; sprintf(s,"%.5lf",tM); EditStM->Text=s;
  EditStM->Font->Color=clGreen;
  if (tM>=MACHINE[mach].Torque) {
    EditStM->Font->Color=clRed;
    OutputMessage("Работа на пределе (или Превышение) - крутящего момента");
  }

  //Расчет завершен

   return (0);
}
//---------------------------------------------------------------------------

void TFormMain::CalcStandart(TObject *Sender)
{
  int i;
  /**************************************************************************/
  /* Расчет */

  /*n V*/ /*i=CalcStnV(Sender); if (i<0) return ;*/
  /*************************************************************************/
  int vi=0 ; char s[80]; String str;
  int mach; // станок
  double D,z,vc,fz; //исходные данные
  double tn,tvf; //исходные данные

  mach=TabMach->ItemIndex;

 /* Проверка на правильность ввода числовых значений
    StrToFloat(str) FloatToStr(vd) FloatToText(s,vd,fvExtended, ffFixed,8,8);*/
  /*диаметр*/
  str=BaseD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=0.;
    s[0]='\0'; sprintf(s,"%.3lf",D); BaseD->Text=s; BaseD->SetFocus(); BaseD->SelectAll();
    Application->MessageBox("Ошибка задания 'диаметра инструмента' - D","Предупреждение...",MB_OK);
    return ;
  }

  /*Число зубьев*/
  str=BaseZ->Text;
  vi=atoi(str.c_str());
  if (vi<=0) {
    vi=0;
    s[0]='\0'; sprintf(s,"%d",vi); BaseZ->Text=s; BaseZ->SetFocus(); BaseZ->SelectAll();
    Application->MessageBox("Ошибка задания 'Числа зубьев' - z","Предупреждение...",MB_OK);
    return ;
  }
  z=(double)vi;

  /*Скорость резания*/
  str=BaseVc->Text;
  vc=atof(str.c_str());
  if (vc<=0.) {
    vc=0;
    s[0]='\0'; sprintf(s,"%.3lf",vc); BaseVc->Text=s; BaseVc->SetFocus(); BaseVc->SelectAll();
    Application->MessageBox("Ошибка задания 'Скорости резания' - vc","Предупреждение...",MB_OK);
    return ;
  }

  /*Подача на зуб*/
  str=Basefz->Text;
  fz=atof(str.c_str());
  if (fz<=0.) {
    fz=0;
    s[0]='\0'; sprintf(s,"%.5lf",fz);
    Basefz->Text=s; Basefz->SetFocus(); Basefz->SelectAll();
    Application->MessageBox("Ошибка задания 'Подача на зуб' - fz","Предупреждение...",MB_OK);
    return ;
  }

  // Расчет оборотов
  if (D<=0.) { Application->MessageBox("Расчет оборотов \n D=0. \n Расчет прерван.","Предупреждение...",MB_OK); return ; }
  tn=vc*1000./(D*PI);
  s[0]='\0'; sprintf(s,"%.3lf",tn);  EditStn->Text=s;

  EditStn->Font->Color=clGreen; // clRed; clGreen
  if (tn>=MACHINE[mach].MAXSpindel) {
    EditStn->Font->Color=clRed;
    OutputMessage("Превышение числа оборотов");
  }

  // Расчет скорости резания
  vc=(D*PI*tn)/1000.;
  s[0]='\0'; sprintf(s,"%.3lf",vc);  EditStVc->Text=s;
  /*************************************************************************/

  /*************************************************************************/
  /*Sz F*/ /* i=CalcStFSz(Sender);  if (i<0) return ; */
  /*************************************************************************/
  // Расчет подачи резания
  tvf=fz*z*tn;
  s[0]='\0'; sprintf(s,"%.2lf",tvf);  EditStVf->Text=s;

  EditStVf->Font->Color=clGreen; // clRed; clGreen
  if (tvf>=MACHINE[mach].MAXFeed) {
    EditStVf->Font->Color=clRed; // clRed; clGreen
    OutputMessage("Превышение максимального лимита рабочей подачи");
  }

  // Расчет подачи на зуб
  if (tn<=0.) { Application->MessageBox("Расчет подачи на зуб \n n=0. \n Расчет прерван.","Предупреждение...",MB_OK); return ; }
  if (z<=0.) { Application->MessageBox("Расчет подачи на зуб \n z=0. \n Расчет прерван.","Предупреждение...",MB_OK); return ; }
  fz=tvf/(z*tn);
  s[0]='\0'; sprintf(s,"%.5lf",fz);  EditStfz->Text=s;
  /*************************************************************************/


  /*Q*/ i=CalcStQ(Sender);  if (i<0) return ;
  /*hm*/ i=CalcSthm(Sender);  if (i<0) return ;

  /*Nct Mz*/  i=CalcStNctMz(Sender);  if (i<0) return ;
  /**************************************************************************/
  //Расчет завершен
  return ;
}


int TFormMain::CalcTechAlSz(TObject *Sender)
{
  char s[80];  String str;
  double D,z,B,t,K; //исходные данные
  double Ks; // параметр
  double tSz,tV0; // .....мощность
  double k1,x1,y1,z1;
  double t1,t2,t3,t4,t5;

  /* Проверка на правильность ввода числовых значений
    StrToFloat(str) FloatToStr(vd) FloatToText(s,vd,fvExtended, ffFixed,8,8);*/
  /*диаметр*/
  str=BaseAlD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=30.;
    s[0]='\0'; sprintf(s,"%.3lf",D);
    BaseAlD->Text=s;  BaseAlD->SetFocus();   BaseAlD->SelectAll();
    str="Ошибка задания 'диаметра инструмента' - D -> исправлено....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
  }

  /*Ширина резания*/
  str=BaseAlB->Text;
  B=atof(str.c_str());
  if (B<=0.) {
    B=D/2.;
    s[0]='\0'; sprintf(s,"%.3lf",B);
    BaseAlB->Text=s;  BaseAlB->SetFocus();  BaseAlB->SelectAll();
    str="Ошибка задания 'Ширины реза' - B -> исправлено....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
  }
  // Проверка
  if (D<B) OutputMessage("Диаметр фрезы меньше чем ширина фрезерования 'D<B' !");

  /*Глубина реза*/
  str=BaseAlT->Text;
  t=atof(str.c_str());
  if (t<=0.) {
    t=D/4;
    s[0]='\0'; sprintf(s,"%.3lf",t);
    BaseAlT->Text=s;  BaseAlT->SetFocus();  BaseAlT->SelectAll();
    str="Ошибка задания 'Глубина реза' - t -> исправлено....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
  }

  K=B/D;
  s[0]='\0'; sprintf(s,"%.2lf",K); EditK->Text=s;
  s[0]='\0'; sprintf(s,"%.2lf",t/D); EdittD->Text=s;

  // Чтение параметров....
  //fz
  str=EditTkoef1->Text;
  k1=atof(str.c_str());
  if (k1<=0.) {
    k1=0.04; s[0]='\0'; sprintf(s,"%.3lf",k1);
    EditTkoef1->Text=s;  EditTkoef1->SetFocus();  EditTkoef1->SelectAll();
    OutputMessage("Ошибка задания k1<=0 (Расчет подачи на зуб)-> исправлено....");
  }
  str=EditTX1->Text;
  x1=atof(str.c_str());
  if (x1<=0.) {
    x1=0.75; s[0]='\0'; sprintf(s,"%.3lf",x1);
    EditTX1->Text=s;  EditTX1->SetFocus();  EditTX1->SelectAll();
    OutputMessage("Ошибка задания x1<=0 (Расчет подачи на зуб)-> исправлено....");
  }
  str=EditTY1->Text;
  y1=atof(str.c_str());
  if (y1<=0.) {
    y1=0.1; s[0]='\0'; sprintf(s,"%.3lf",y1);
    EditTY1->Text=s;  EditTY1->SetFocus();  EditTY1->SelectAll();
    OutputMessage("Ошибка задания y1<=0 (Расчет подачи на зуб)-> исправлено....");
  }
  str=EditTZ1->Text;
  z1=atof(str.c_str());
  if (z1<=0.) {
    z1=0.22; s[0]='\0'; sprintf(s,"%.3lf",z1);
    EditTZ1->Text=s;  EditTZ1->SetFocus();  EditTZ1->SelectAll();
    OutputMessage("Ошибка задания z1<=0 (Расчет подачи на зуб)-> исправлено....");
  }
  str=EditTKs->Text;
  Ks=atof(str.c_str());
  if (Ks<=0.) {
    Ks=1.; s[0]='\0'; sprintf(s,"%.3lf",Ks);
    EditTKs->Text=s;  EditTKs->SetFocus();  EditTKs->SelectAll();
    OutputMessage("Ошибка задания Ks<=0 (дополнительного коэффициента)-> исправлено....");
  }

  /**************************************************************************/
  /* Расчет */
  /*Sz*/ t1=pow(D,x1);  t2=pow(t,y1);  t3=pow(B,z1); tSz=k1*t1*Ks/(t2*t3) ;
  if (tSz<=0.0001) {
    str="Ошибка вычисления-> Sz=fz=0.";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
    return (-1);
  }

    //установка значений
  s[0]='\0'; sprintf(s,"%.3lf",tSz); EditSz->Text=s;

  return(0) ;
}

int TFormMain::CalcTechAlV1(TObject *Sender)
{
  int i,j;
  char s[80];  String str;
  double D,B,t,K; //исходные данные
  double tSz,tV1,tV2; // .....мощность
  double k2,x2,y2,z2;
  double t1,t2,t3,t4,t5;

  /* Проверка на правильность ввода числовых значений
    StrToFloat(str) FloatToStr(vd) FloatToText(s,vd,fvExtended, ffFixed,8,8);*/
  /*диаметр*/
  str=BaseAlD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=30.;
    s[0]='\0'; sprintf(s,"%.3lf",D);
    BaseAlD->Text=s;  BaseAlD->SetFocus();   BaseAlD->SelectAll();
    str="Ошибка задания 'диаметра инструмента' - D -> исправлено....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
  }

  /*Ширина резания*/
  str=BaseAlB->Text;
  B=atof(str.c_str());
  if (B<=0.) {
    B=D/2.;
    s[0]='\0'; sprintf(s,"%.3lf",B);
    BaseAlB->Text=s;  BaseAlB->SetFocus();  BaseAlB->SelectAll();
    str="Ошибка задания 'Ширины реза' - B -> исправлено....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
  }
  // Проверка
  if (D<B) OutputMessage("Диаметр фрезы меньше чем ширина фрезерования 'D<B' !");

  /*Глубина реза*/
  str=BaseAlT->Text;
  t=atof(str.c_str());
  if (t<=0.) {
    t=D/4;
    s[0]='\0'; sprintf(s,"%.3lf",t);
    BaseAlT->Text=s;  BaseAlT->SetFocus();  BaseAlT->SelectAll();
    str="Ошибка задания 'Глубина реза' - t -> исправлено....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
  }

  K=B/D;
  s[0]='\0'; sprintf(s,"%.2lf",K); EditK->Text=s;
  s[0]='\0'; sprintf(s,"%.2lf",t/D); EdittD->Text=s;

  //fz
  str=EditSz->Text;
  tSz=atof(str.c_str());
  if (tSz<=0.) {
    tSz=0.; s[0]='\0'; sprintf(s,"%.3lf",tSz);
    EditSz->Text=s;  EditSz->SetFocus();  EditSz->SelectAll();
    OutputMessage("Ошибка задания Sz<=0 (Расчет V1)....");
  }

  //V1
  str=EditTkoef2->Text;
  k2=atof(str.c_str());
  if (k2<=0.) {
    k2=1097.0; s[0]='\0'; sprintf(s,"%.2lf",k2);
    EditTkoef2->Text=s;  EditTkoef2->SetFocus();  EditTkoef2->SelectAll();
    OutputMessage("Ошибка задания k2<=0 (Расчет V1)-> исправлено....");
  }
  str=EditTX2->Text;
  x2=atof(str.c_str());
  if (x2<=0.) {
    x2=0.86; s[0]='\0'; sprintf(s,"%.3lf",x2);
    EditTX2->Text=s;  EditTX2->SetFocus();  EditTX2->SelectAll();
    OutputMessage("Ошибка задания x2<=0 (Расчет V1)-> исправлено....");
  }
  str=EditTY2->Text;
  y2=atof(str.c_str());
  if (y2<=0.) {
    y2=0.81; s[0]='\0'; sprintf(s,"%.3lf",y2);
    EditTY2->Text=s;  EditTY2->SetFocus();  EditTY2->SelectAll();
    OutputMessage("Ошибка задания y2<=0 (Расчет V1)-> исправлено....");
  }
  str=EditTZ2->Text;
  z2=atof(str.c_str());
  if (z2<=0.) {
    z2=0.47; s[0]='\0'; sprintf(s,"%.3lf",z2);
    EditTZ2->Text=s;  EditTZ2->SetFocus();  EditTZ2->SelectAll();
    OutputMessage("Ошибка задания z2<=0 (Расчет V1)-> исправлено....");
  }

  /**************************************************************************/
  K=B/D;
  /* Расчет */
  /*Sz*/
  if (tSz<=0.0001) {
    str="Ошибка вычисления-> Sz=fz=0.";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
    return(-1);
  }
  /*V1*/ t1=pow(tSz,x2);t2=pow(t,y2);  t3=pow(K,z2); tV1=k2/(t1*t2*t3) ;

  //установка значений
  s[0]='\0'; sprintf(s,"%.3lf",tV1); EditV1->Text=s;  EditV->Items->Add(s);

  return(0) ;
}

int TFormMain::CalcTechAlV2(TObject *Sender)
{
  char s[80];  String str;
  double D,B,t,K; //исходные данные
  double kpd ; // кпд станка
  int mach; // станок
  double tSz,tV2; // .....мощность
  double k3,x3,y3,z3,m1;
  double t1,t2,t3,t4,t5;

  mach=TabMach->ItemIndex;

  /* Проверка на правильность ввода числовых значений
    StrToFloat(str) FloatToStr(vd) FloatToText(s,vd,fvExtended, ffFixed,8,8);*/
  /*диаметр*/
  str=BaseAlD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=30.;
    s[0]='\0'; sprintf(s,"%.3lf",D);
    BaseAlD->Text=s;  BaseAlD->SetFocus();   BaseAlD->SelectAll();
    str="Ошибка задания 'диаметра инструмента' - D -> исправлено....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
  }

  /*Ширина резания*/
  str=BaseAlB->Text;
  B=atof(str.c_str());
  if (B<=0.) {
    B=D/2.;
    s[0]='\0'; sprintf(s,"%.3lf",B);
    BaseAlB->Text=s;  BaseAlB->SetFocus();  BaseAlB->SelectAll();
    str="Ошибка задания 'Ширины реза' - B -> исправлено....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
  }
  // Проверка
  if (D<B) OutputMessage("Диаметр фрезы меньше чем ширина фрезерования 'D<B' !");

  /*Глубина реза*/
  str=BaseAlT->Text;
  t=atof(str.c_str());
  if (t<=0.) {
    t=D/4;
    s[0]='\0'; sprintf(s,"%.3lf",t);
    BaseAlT->Text=s;  BaseAlT->SetFocus();  BaseAlT->SelectAll();
    str="Ошибка задания 'Глубина реза' - t -> исправлено....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
  }

  K=B/D;
  s[0]='\0'; sprintf(s,"%.2lf",K); EditK->Text=s;
  s[0]='\0'; sprintf(s,"%.2lf",t/D); EdittD->Text=s;

  // Кпд
  str=ComboBoxTkpd->Text; kpd=atof(str.c_str());

  //fz
  str=EditSz->Text;
  tSz=atof(str.c_str());
  if (tSz<=0.) {
    tSz=0.; s[0]='\0'; sprintf(s,"%.3lf",tSz);
    EditSz->Text=s;  EditSz->SetFocus();  EditSz->SelectAll();
    OutputMessage("Ошибка задания Sz<=0 (Расчет V1)....");
  }

  //V2
  str=EditTkoef3->Text;
  k3=atof(str.c_str());
  if (k3<=0.) {
    k3=49.0; s[0]='\0'; sprintf(s,"%.2lf",k3);
    EditTkoef3->Text=s;  EditTkoef3->SetFocus();  EditTkoef3->SelectAll();
    OutputMessage("Ошибка задания k3<=0 (Расчет V2)-> исправлено....");
  }
  str=EditTX3->Text;
  x3=atof(str.c_str());
  if (x3<=0.) {
    x3=0.684; s[0]='\0'; sprintf(s,"%.3lf",x3);
    EditTX3->Text=s;  EditTX3->SetFocus();  EditTX3->SelectAll();
    OutputMessage("Ошибка задания x3<=0 (Расчет V2)-> исправлено....");
  }
  str=EditTY3->Text;
  y3=atof(str.c_str());
  if (y3<=0.) {
    y3=0.548; s[0]='\0'; sprintf(s,"%.3lf",y3);
    EditTY3->Text=s;  EditTY3->SetFocus();  EditTY3->SelectAll();
    OutputMessage("Ошибка задания y3<=0 (Расчет V2)-> исправлено....");
  }
  str=EditTZ3->Text;
  z3=atof(str.c_str());
  if (z3<=0.) {
    z3=0.286; s[0]='\0'; sprintf(s,"%.3lf",z3);
    EditTZ3->Text=s;  EditTZ3->SetFocus();  EditTZ3->SelectAll();
    OutputMessage("Ошибка задания z3<=0 (Расчет V2)-> исправлено....");
  }
  str=EditTsm1->Text;
  m1=atof(str.c_str());
  if (m1<=0.) {
    m1=1.134; s[0]='\0'; sprintf(s,"%.3lf",m1);
    EditTsm1->Text=s;  EditTsm1->SetFocus();  EditTsm1->SelectAll();
    OutputMessage("Ошибка задания m1<=0 (Расчет V2)-> исправлено....");
  }

  /**************************************************************************/
  K=B/D;
  /* Расчет */
  /*Sz*/
  if (tSz<=0.0001) {
    str="Ошибка вычисления-> Sz=fz=0.";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
    return(-1);
  }

  /*V2*/t1=pow(t,x3);  t2=pow(tSz,y3);t3=pow(D,z3);
        t4=MACHINE[mach].Power*kpd*K*k3/(t1*t2*t3); tV2=pow(t4,m1);
  /**************************************************************************/

  //установка значений
  s[0]='\0'; sprintf(s,"%.3lf",tV2); EditV2->Text=s;  EditV->Items->Add(s);
  return(0) ;
}

int TFormMain::CalcTechAlV0(TObject *Sender)
{
  char s[80];  AnsiString str;
  double tV0,tV; // .....
  int i,count;

  count=EditV->Items->Count;
  i=0; tV0=0.;
  while(i<count)
  {
    str=EditV->Items->Strings[i];
    tV=atof(str.c_str());
    if (i==0) tV0=tV;
    if (tV<=0.) {
       tV=0.;
       OutputMessage("Ошибка преобразования V<=0 (Расчет V)....");
     }
    if (tV0>tV) tV0=tV;
    i++;
  }
  s[0]='\0'; sprintf(s,"%.3lf",tV0);
  EditV->Items->Insert(0,s); EditV->ItemIndex = 0;

  return (0);
}

int TFormMain::CalcTechAlTemp(TObject *Sender)
{
  int i;
  double vd;
  char s[80];  AnsiString str;
  double tF,tV0; // .....мощность
  int iv,ifv;

  // Чтение параметров....
  //V
  EditV->Update();
  str=EditV->Text;
  tV0=atof(str.c_str());
  if (tV0<=0.) {
    tV0=0.;
    //OutputMessage("Ошибка задания V<=0 (Проверка температуры)....");
  }
  //F
  EditF->Update();
  str=EditF->Text;
  tF=atof(str.c_str());
  if (tF<=0.) {
    tF=20.0; s[0]='\0'; sprintf(s,"%.3lf",tF);  EditF->Text=s;
    //OutputMessage("Ошибка задания F<=0 (Проверка температуры)....");
  }

  iv=1; ifv=1;
  for(i=1;i<9;i++) { vd=double(Temperature[0][i]); if (tV0>=vd) iv=i; }
  for(i=1;i<10;i++) { vd=double(Temperature[i][0]); if (tF>=vd) ifv=i; }
  iv++;ifv++;
  if (iv>8) iv=8;
  if (ifv>9) ifv=9;
  //s[0]='\0'; sprintf(s,"%.3lf",Temperature[ifv][iv]); EditTm->Text=s;
  EditTm->Text=IntToStr(Temperature[ifv][iv]) ;
  EditTm->Font->Color=clBlue; // clRed; clGreen; clBlue
  if (Temperature[ifv][iv]>=140) {
    EditTm->Font->Color=clRed;
    OutputMessage("Превышение температурного режима: T>=140°C -> деталь плавиться....... :)");
  }

  //bool CanSelect=True;
  //StringGridT->OnSelectCell(Sender, iv, ifv, CanSelect);

  return(0); ;
}

int TFormMain::CalcTechAln(TObject *Sender)
{
  char s[80];  String str;
  double D,t,K,B; //исходные данные
  int mach; // станок
  double tV0,tn; // .....мощность

  mach=TabMach->ItemIndex;

  /* Проверка на правильность ввода числовых значений
    StrToFloat(str) FloatToStr(vd) FloatToText(s,vd,fvExtended, ffFixed,8,8);*/
  /*диаметр*/
  str=BaseAlD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=30.;
    s[0]='\0'; sprintf(s,"%.3lf",D);
    BaseAlD->Text=s;  BaseAlD->SetFocus();   BaseAlD->SelectAll();
    str="Ошибка задания 'диаметра инструмента' - D -> исправлено....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
  }

  /*Ширина резания*/
  str=BaseAlB->Text;
  B=atof(str.c_str());
  if (B<=0.) {
    B=D/2.;
    s[0]='\0'; sprintf(s,"%.3lf",B);
    BaseAlB->Text=s;  BaseAlB->SetFocus();  BaseAlB->SelectAll();
    str="Ошибка задания 'Ширины реза' - B -> исправлено....";
    OutputMessage(str);
  }
  // Проверка
  if (D<B) OutputMessage("Диаметр фрезы меньше чем ширина фрезерования 'D<B' !");

  /*Глубина реза*/
  str=BaseAlT->Text;
  t=atof(str.c_str());
  if (t<=0.) {
    t=D/4;
    s[0]='\0'; sprintf(s,"%.3lf",t);
    BaseAlT->Text=s;  BaseAlT->SetFocus();  BaseAlT->SelectAll();
    str="Ошибка задания 'Глубина реза' - t -> исправлено....";
    OutputMessage(str);
  }

  K=B/D;
  s[0]='\0'; sprintf(s,"%.2lf",K); EditK->Text=s;
  s[0]='\0'; sprintf(s,"%.2lf",t/D); EdittD->Text=s;

  // Чтение параметров....
   /*V0*/
  str=EditV->Text;
  tV0=atof(str.c_str());
  if (tV0<=0.) {
    tV0=0.0;
    OutputMessage("Ошибка задания скорости реза (м/мин) V=0   .... (Расчет n)");
  }

  /**************************************************************************/
  /* Расчет */
  /*n*/  tn=tV0*1000./(PI*D) ;
  Editn->Font->Color=clGreen; // clRed; clGreen
  if (tn>=MACHINE[mach].MAXSpindel) {
    Editn->Font->Color=clRed;
    OutputMessage("Превышение числа оборотов   (Расчет n)");
  }

  /**************************************************************************/
  //установка значений
  s[0]='\0'; sprintf(s,"%.3lf",tn);  Editn->Text=s;
  return(0);
}

int TFormMain::CalcTechAlF(TObject *Sender)
{
  int vi=0 ; char s[80];  String str;
  double z; //исходные данные
  int mach; // станок
  double tSz,tn,tF; // .....мощность

  mach=TabMach->ItemIndex;

  /*Число зубьев*/
  str=BaseAlZ->Text;
  vi=atoi(str.c_str());
  if (vi<=0) {
    vi=4;
    s[0]='\0'; sprintf(s,"%d",vi);
    BaseAlZ->Text=s;  BaseAlZ->SetFocus();   BaseAlZ->SelectAll();
    str="Ошибка задания 'Числа зубьев' - z -> исправлено....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
  }
  z=(double)vi;

  /*n*/
  str=Editn->Text;
  tn=atof(str.c_str());
  if (tn<=0.) {
    tn=0.; s[0]='\0'; sprintf(s,"%.3lf",tn);
    Editn->Text=s;  Editn->SetFocus();  Editn->SelectAll();
    OutputMessage("Ошибка задания n<=0 (Расчет F)....");
  }
  Editn->Font->Color=clGreen; // clRed; clGreen
  if (tn>=MACHINE[mach].MAXSpindel) {
    Editn->Font->Color=clRed;
    OutputMessage("Превышение числа оборотов  (Расчет F)");
  }

  //fz
  str=EditSz->Text;
  tSz=atof(str.c_str());
  if (tSz<=0.) {
    tSz=0.; s[0]='\0'; sprintf(s,"%.3lf",tSz);
    EditSz->Text=s;  EditSz->SetFocus();  EditSz->SelectAll();
    OutputMessage("Ошибка задания Sz<=0 (Расчет F)....");
  }

  /**************************************************************************/
  /* Расчет */
  /*Sz*/
  if (tSz<=0.0001) {
    str="Ошибка вычисления-> Sz=fz=0.";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
  }

  /*F*/  tF=tSz*tn*z ;
  EditF->Font->Color=clGreen; // clRed; clGreen
  if (tF>=MACHINE[mach].MAXFeed) {
    EditF->Font->Color=clRed;
    OutputMessage("Превышение максимальной подачи резания  (Расчет F)");
  }
  /**************************************************************************/

  //установка значений
  s[0]='\0'; sprintf(s,"%.3lf",tF);  EditF->Text=s;

  return(0);
}

int TFormMain::CalcTechAlPz(TObject *Sender)
{
  int i;
  char s[80];  String str;
  double D,B,t,K; //исходные данные
  double kpd ; // кпд станка
  int mach; // станок
  double tSz,tn,tPz; // .....мощность
  double k4,x4,y4,z4,m2;
  double t1,t2,t3,t4,t5;

  mach=TabMach->ItemIndex;

  /* Проверка на правильность ввода числовых значений
    StrToFloat(str) FloatToStr(vd) FloatToText(s,vd,fvExtended, ffFixed,8,8);*/
  /*диаметр*/
  str=BaseAlD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=30.;
    s[0]='\0'; sprintf(s,"%.3lf",D);
    BaseAlD->Text=s;  BaseAlD->SetFocus();   BaseAlD->SelectAll();
    str="Ошибка задания 'диаметра инструмента' - D -> исправлено....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
  }

  /*Ширина резания*/
  str=BaseAlB->Text;
  B=atof(str.c_str());
  if (B<=0.) {
    B=D/2.;
    s[0]='\0'; sprintf(s,"%.3lf",B);
    BaseAlB->Text=s;  BaseAlB->SetFocus();  BaseAlB->SelectAll();
    str="Ошибка задания 'Ширины реза' - B -> исправлено....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
  }
  // Проверка
  if (D<B) OutputMessage("Диаметр фрезы меньше чем ширина фрезерования 'D<B' !");

  /*Глубина реза*/
  str=BaseAlT->Text;
  t=atof(str.c_str());
  if (t<=0.) {
    t=D/4;
    s[0]='\0'; sprintf(s,"%.3lf",t);
    BaseAlT->Text=s;  BaseAlT->SetFocus();  BaseAlT->SelectAll();
    str="Ошибка задания 'Глубина реза' - t -> исправлено....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
  }

  K=B/D;
  s[0]='\0'; sprintf(s,"%.2lf",K); EditK->Text=s;
  s[0]='\0'; sprintf(s,"%.2lf",t/D); EdittD->Text=s;

  // Кпд
  str=ComboBoxTkpd->Text; kpd=atof(str.c_str());

  //P
  str=EditTkoef4->Text;
  k4=atof(str.c_str());
  if (k4<=0.) {
    k4=246; s[0]='\0'; sprintf(s,"%.2lf",k4);
    EditTkoef4->Text=s;  EditTkoef4->SetFocus();  EditTkoef4->SelectAll();
    OutputMessage("Ошибка задания k4<=0 (Расчет P)-> исправлено....");
  }
  str=EditTX4->Text;
  x4=atof(str.c_str());
  if (x4<=0.) {
    x4=0.684; s[0]='\0'; sprintf(s,"%.3lf",x4);
    EditTX4->Text=s;  EditTX4->SetFocus();  EditTX4->SelectAll();
    OutputMessage("Ошибка задания x4<=0 (Расчет P)-> исправлено....");
  }
  str=EditTY4->Text;
  y4=atof(str.c_str());
  if (y4<=0.) {
    y4=0.546; s[0]='\0'; sprintf(s,"%.3lf",y4);
    EditTY4->Text=s;  EditTY4->SetFocus();  EditTY4->SelectAll();
    OutputMessage("Ошибка задания y4<=0 (Расчет P)-> исправлено....");
  }
  str=EditTZ4->Text;
  z4=atof(str.c_str());
  if (z4<=0.) {
    z4=0.168; s[0]='\0'; sprintf(s,"%.3lf",z4);
    EditTZ4->Text=s;  EditTZ4->SetFocus();  EditTZ4->SelectAll();
    OutputMessage("Ошибка задания z4<=0 (Расчет P)-> исправлено....");
  }
  str=EditTsm2->Text;
  m2=atof(str.c_str());
  if (m2<=0.) {
    m2=0.118; s[0]='\0'; sprintf(s,"%.3lf",m2);
    EditTsm2->Text=s;  EditTsm2->SetFocus();  EditTsm2->SelectAll();
    OutputMessage("Ошибка задания m2<=0 (Расчет P)-> исправлено....");
  }

  //fz
  str=EditSz->Text;
  tSz=atof(str.c_str());
  if (tSz<=0.) {
    tSz=0.; s[0]='\0'; sprintf(s,"%.3lf",tSz);
    EditSz->Text=s;  EditSz->SetFocus();  EditSz->SelectAll();
    OutputMessage("Ошибка задания Sz<=0 (Расчет Pz)....");
  }

  /*n*/
  str=Editn->Text;
  tn=atof(str.c_str());
  if (tn<=0.) {
    tn=0.; s[0]='\0'; sprintf(s,"%.3lf",tn);
    Editn->Text=s;  Editn->SetFocus();  Editn->SelectAll();
    str="Ошибка задания n<=0 (Расчет Pz)....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
    return (-1);
  }
  Editn->Font->Color=clGreen; // clRed; clGreen
  if (tn>=MACHINE[mach].MAXSpindel) {
    Editn->Font->Color=clRed;
    OutputMessage("Превышение числа оборотов (Расчет Pz)");
  }

  /**************************************************************************/
  /* Расчет */
  /*Sz*/
  if (tSz<=0.0001) {
    str="Ошибка вычисления-> Sz=fz=0.";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение..(Расчет Pz).",MB_OK);
    return (-1);
  }

  /*Pz*/ t1=pow(t,x4);  t2=pow(tSz,y4);  t3=pow(D,z4);  t4=pow(tn,m2); tPz=k4*t1*t2*t3/t4;
  //установка значений
  s[0]='\0'; sprintf(s,"%.2lf",tPz); EditPz->Text=s;
  EditPz->Font->Color=clGreen; // clRed; clGreen

  PanelForestP->Visible=False; //False
  // Forest and ВФ-5ВС
  if (mach==0 || mach==1) {

    t5=tPz/1000; // перевод в кН
    for (i=1;i<8;i++) { StringGridP->Cells[0][i]=""; StringGridP->Cells[1][i]=""; }

    i=1;
    if (tn<=6000) {
      s[0]='\0';sprintf(s,"n=%.0lf",tn); StringGridP->Cells[0][i]=s;
      s[0]='\0';sprintf(s,"P=%.3lf • %.2lf=%.3lf",t5,kpd,t5*kpd); StringGridP->Cells[1][i]=s;
      i++;
      if (t5>(2.00*kpd)) {
        EditPz->Font->Color=clRed;
        OutputMessage("Превышение допустимой силовой нагрузки на шпиндель (Расчет Pz)....");
       }
    }
    StringGridP->Cells[0][i]="6000";
    s[0]='\0'; sprintf(s,"2.00 • %.2lf  =%.2lf",kpd,2.00*kpd);  StringGridP->Cells[1][i]=s;

    i++;
    if ((tn>6000) && (tn<=8000)) {
      s[0]='\0';sprintf(s,"n=%.0lf",tn); StringGridP->Cells[0][i]=s;
      s[0]='\0';sprintf(s,"P=%.3lf • %.2lf=%.3lf",t5,kpd,t5*kpd); StringGridP->Cells[1][i]=s;
      i++;
      if (t5>(1.70*kpd)) {
        EditPz->Font->Color=clRed;
        OutputMessage("Превышение допустимой силовой нагрузки на шпиндель (Расчет Pz)....");
       }
    }
    StringGridP->Cells[0][i]="8000";
    s[0]='\0'; sprintf(s,"1.70 • %.2lf  =%.2lf",kpd,1.70*kpd);   StringGridP->Cells[1][i]=s;

    i++;
    if ((tn>8000) && (tn<=10000)) {
      s[0]='\0';sprintf(s,"n=%.0lf",tn); StringGridP->Cells[0][i]=s;
      s[0]='\0';sprintf(s,"P=%.3lf • %.2lf=%.3lf",t5,kpd,t5*kpd); StringGridP->Cells[1][i]=s;
      i++;
      if (t5>(1.50*kpd)) {
        EditPz->Font->Color=clRed;
        OutputMessage("Превышение допустимой силовой нагрузки на шпиндель (Расчет Pz)....");
       }
    }
    StringGridP->Cells[0][i]="10000";
    s[0]='\0'; sprintf(s,"1.50 • %.2lf  =%.2lf",kpd,1.50*kpd);   StringGridP->Cells[1][i]=s;

    i++;
    if ((tn>10000) && (tn<=12000)) {
      s[0]='\0';sprintf(s,"n=%.0lf",tn); StringGridP->Cells[0][i]=s;
      s[0]='\0';sprintf(s,"P=%.3lf • %.2lf=%.3lf",t5,kpd,t5*kpd); StringGridP->Cells[1][i]=s;
      i++;
      if (t5>(1.35*kpd)) {
        EditPz->Font->Color=clRed;
        OutputMessage("Превышение допустимой силовой нагрузки на шпиндель (Расчет Pz)....");
       }
    }
    StringGridP->Cells[0][i]="12000";
    s[0]='\0'; sprintf(s,"1.35 • %.2lf  =%.2lf",kpd,1.35*kpd);  StringGridP->Cells[1][i]=s;

    i++;
    if ((tn>12000) && (tn<=15000)) {
      s[0]='\0';sprintf(s,"n=%.0lf",tn); StringGridP->Cells[0][i]=s;
      s[0]='\0';sprintf(s,"P=%.3lf • %.2lf=%.3lf",t5,kpd,t5*kpd); StringGridP->Cells[1][i]=s;
      i++;
      if (t5>(1.25*kpd)) {
        EditPz->Font->Color=clRed;
        OutputMessage("Превышение допустимой силовой нагрузки на шпиндель (Расчет Pz)....");
       }
    }
    StringGridP->Cells[0][i]="15000";
    s[0]='\0'; sprintf(s,"1.25 • %.2lf  =%.2lf",kpd,1.25*kpd);  StringGridP->Cells[1][i]=s;

    i++;
    if ((tn>15000) && (tn<=18000)) {
      s[0]='\0';sprintf(s,"n=%.0lf",tn); StringGridP->Cells[0][i]=s;
      s[0]='\0';sprintf(s,"P=%.3lf • %.2lf=%.3lf",t5,kpd,t5*kpd); StringGridP->Cells[1][i]=s;
      i++;
      if (t5>(1.20*kpd)) {
        EditPz->Font->Color=clRed;
        OutputMessage("Превышение допустимой силовой нагрузки на шпиндель (Расчет Pz)....");
       }
    }
    StringGridP->Cells[0][i]="18000";
    s[0]='\0'; sprintf(s,"1.20 • %.2lf  =%.2lf",kpd,1.20*kpd);  StringGridP->Cells[1][i]=s;

    i++;
    if ((tn>18000)) {
      s[0]='\0';sprintf(s,"n=%.0lf",tn); StringGridP->Cells[0][i]=s;
      s[0]='\0';sprintf(s,"P=%.3lf • %.2lf=%.3lf",t5,kpd,t5*kpd); StringGridP->Cells[1][i]=s;
      i++;
      if (t5>(1.20*kpd)) {
        EditPz->Font->Color=clRed;
        OutputMessage("Превышение допустимой силовой нагрузки на шпиндель (Расчет Pz)....");
       }
    }

    PanelForestP->Visible=True; //False
  }

  return(0);
}

int TFormMain::CalcTechAlMz(TObject *Sender)
{
  char s[80];  String str;
  double D,t,K,B; //исходные данные
  int mach; // станок
  double tMz,tPz; // .....мощность

  mach=TabMach->ItemIndex;

  /* Проверка на правильность ввода числовых значений
    StrToFloat(str) FloatToStr(vd) FloatToText(s,vd,fvExtended, ffFixed,8,8);*/
  /*диаметр*/
  str=BaseAlD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=30.;
    s[0]='\0'; sprintf(s,"%.3lf",D);
    BaseAlD->Text=s;  BaseAlD->SetFocus();   BaseAlD->SelectAll();
    str="Ошибка задания 'диаметра инструмента' - D -> исправлено....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"Предупреждение...",MB_OK);
  }

  /*Ширина резания*/
  str=BaseAlB->Text;
  B=atof(str.c_str());
  if (B<=0.) {
    B=D/2.;
    s[0]='\0'; sprintf(s,"%.3lf",B);
    BaseAlB->Text=s;  BaseAlB->SetFocus();  BaseAlB->SelectAll();
    str="Ошибка задания 'Ширины реза' - B -> исправлено....";
    OutputMessage(str);
  }
  // Проверка
  if (D<B) OutputMessage("Диаметр фрезы меньше чем ширина фрезерования 'D<B' !");

  /*Глубина реза*/
  str=BaseAlT->Text;
  t=atof(str.c_str());
  if (t<=0.) {
    t=D/4;
    s[0]='\0'; sprintf(s,"%.3lf",t);
    BaseAlT->Text=s;  BaseAlT->SetFocus();  BaseAlT->SelectAll();
    str="Ошибка задания 'Глубина реза' - t -> исправлено....";
    OutputMessage(str);
  }

  K=B/D;
  s[0]='\0'; sprintf(s,"%.2lf",K); EditK->Text=s;
  s[0]='\0'; sprintf(s,"%.2lf",t/D); EdittD->Text=s;

  /*tPz*/
  str=EditPz->Text;
  tPz=atof(str.c_str());
  if (tPz<=0.) {
    tPz=1;
    s[0]='\0'; sprintf(s,"%.3lf",tPz);
    EditPz->Text=s;  EditPz->SetFocus();  EditPz->SelectAll();
    str="Ошибка задания 'Силы реза' - Pz -> исправлено....(Расчет Mz)";
    OutputMessage(str);
  }

  /**************************************************************************/
  /* Расчет */
  /*Mz*/ tMz=tPz*D/(2.*100.);
  EditMz->Font->Color=clGreen; // clRed; clGreen
  if (tMz>=MACHINE[mach].Torque) {
    EditMz->Font->Color=clRed;
    OutputMessage("Превышение максимального момента резания (Расчет Mz)");
  }

  /**************************************************************************/
  //установка значений
  s[0]='\0'; sprintf(s,"%.2lf",tMz); EditMz->Text=s;

  return(0);
}

int TFormMain::CalcTechAlNct(TObject *Sender)
{
  char s[80];  String str;
  double D,t,K,B; //исходные данные
  int mach; // станок
  double tNct,tPz,tV0; // .....мощность

  mach=TabMach->ItemIndex;

  /*V0*/
  str=EditV->Text;
  tV0=atof(str.c_str());
  if (tV0<=0.) {
    str="Ошибка задания 'Скорости реза' - V (Расчет Мощности) ....";
    OutputMessage(str);
  }

  /*tPz*/
  str=EditPz->Text;
  tPz=atof(str.c_str());
  if (tPz<=0.) {
    tPz=1;
    s[0]='\0'; sprintf(s,"%.3lf",tPz);
    EditPz->Text=s;  EditPz->SetFocus();  EditPz->SelectAll();
    str="Ошибка задания 'Силы реза' - Pz (Расчет Мощности) -> исправлено....";
    OutputMessage(str);
  }

  /**************************************************************************/
  /* Расчет */
  /*Nct*/ tNct=tPz*tV0/(1020.*60.);
  EditNp->Font->Color=clGreen; // clRed; clGreen
  if (tNct>=MACHINE[mach].Power) {
    EditNp->Font->Color=clRed;
    OutputMessage("Превышение мощности оборудования");
  }

  /**************************************************************************/
  //установка значений
  s[0]='\0'; sprintf(s,"%.2lf",tNct);EditNp->Text=s;
  return(0);
}

void TFormMain::CalcTechAl(TObject *Sender)
{
  int i;
  /**************************************************************************/
  /* Расчет */
  /*Sz*/ i=CalcTechAlSz(Sender); if (i<0) return ;
  EditV->Clear();
  /*V1*/ i=CalcTechAlV1(Sender);  if (i<0) return ;
  /*V2*/ i=CalcTechAlV2(Sender);  if (i<0) return ;
  /*V0*/ i=CalcTechAlV0(Sender);  if (i<0) return ;
  /*n*/  i=CalcTechAln(Sender);  if (i<0) return ;
  /*F*/  i=CalcTechAlF(Sender);  if (i<0) return ;
  /*Temperature*/  CalcTechAlTemp(Sender);
  /*Pz*/ i=CalcTechAlPz(Sender);  if (i<0) return ;
  /*Mz*/ i=CalcTechAlMz(Sender);  if (i<0) return ;
  /*Nct*/ i=CalcTechAlNct(Sender);  if (i<0) return ;
  /**************************************************************************/
  return ;
}

void TFormMain::CalcTechTi(TObject *Sender)
{
  int i,j;
  int vi=0 ; char s[80]; String str;
  int mach; // станок
  double D,z,t,B,L,hz; //исходные данные
  double timetools, Znorm , lenLD ; // время стойкости
  double vc,fz,tn,tvf,tMz,tPz,tNct; // рассчитываемые параметры : ..., мощность
  double K1,K2,K3,K4,K5,K6,K7,K8; // рассчитываемые параметры : ..., мощность
  double Pxcoeff,Pycoeff, Px,Py;

  K1=K2=K3=K4=K5=K6=K7=K8=1.;

  mach=TabMach->ItemIndex;  // станок

  str=BaseTiD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=25.;
    s[0]='\0'; sprintf(s,"%.3lf",D); BaseTiD->Text=s; BaseTiD->SetFocus(); BaseTiD->SelectAll();
    Application->MessageBox("Ошибка задания 'диаметра инструмента' - D","Предупреждение...",MB_OK);
    return ;
  }

  /*Число зубьев*/
  str=BaseTiZ->Text;
  vi=atoi(str.c_str());
  if (vi<=0) {
    vi=3;
    s[0]='\0'; sprintf(s,"%d",vi); BaseTiZ->Text=s; BaseTiZ->SetFocus(); BaseTiZ->SelectAll();
    Application->MessageBox("Ошибка задания 'Числа зубьев' - z","Предупреждение...",MB_OK);
    return ;
  }
  z=(double)vi;

  /*Ширина резания*/
  str=BaseTiB->Text;
  B=atof(str.c_str());
  if (B<=0.) {
    B=5.;
    s[0]='\0'; sprintf(s,"%.2lf",B); BaseTiB->Text=s; BaseTiB->SetFocus(); BaseTiB->SelectAll();
    Application->MessageBox("Ошибка задания 'Ширины реза' - B","Предупреждение...",MB_OK);
    return ;
  }

  /*Глубина реза*/
  str=BaseTiT->Text;
  t=atof(str.c_str());
  if (t<=0.) {
    t=2.5;
    s[0]='\0'; sprintf(s,"%.3lf",t); BaseTiT->Text=s; BaseTiT->SetFocus(); BaseTiT->SelectAll();
    Application->MessageBox("Ошибка задания 'Глубина реза' - t","Предупреждение...",MB_OK);
    return ;
  }

  /*Вылет инструмента - длина*/
  str=BaseTiL->Text;
  L=atof(str.c_str());
  if (L<=0.) {
    L=10;
    s[0]='\0'; sprintf(s,"%.2lf",L); BaseTiL->Text=s; BaseTiL->SetFocus(); BaseTiL->SelectAll();
    Application->MessageBox("Ошибка задания 'Вылет инструмента - длина' - L","Предупреждение...",MB_OK);
    return ;
  }

  /*Критерий затупления, hz*/
  str=BaseTihz->Text;
  hz=atof(str.c_str());
  if (hz<=0.) {
    hz=0.3;
    s[0]='\0'; sprintf(s,"%.2lf",hz); BaseTihz->Text=s; BaseTihz->SetFocus(); BaseTihz->SelectAll();
    Application->MessageBox("Ошибка задания - Критерий затупления, hz","Предупреждение...",MB_OK);
    return ;
  }

  // проверка
  if (fabs(L-B)>10.) OutputMessage("Разность (L-B)=(длины реж части - ширины фрезерования)не должна превышать 10мм - по рекомендации");

  // коэффициенты
  K1=1;
  if (RadioGroupTiMaterial->ItemIndex==1) K1=0.8;
  K2=1;
  if (RadioGroupTiSurface->ItemIndex==2) K2=0.7;
  K3=1;
  if (D<30.) {
    Znorm=3;
    Znorm=z/Znorm;
    if (Znorm<=0.6) K3=1.5;
    if (Znorm>0.6 && Znorm<=0.85) K3=1.3;
    if (Znorm>0.85) K3=1.;
   } else {
    Znorm=5;
    Znorm=z/Znorm;
    if (Znorm<=1.75) K3=0.7; else K3=0.6;
  }
  K4=1;
  lenLD=L/D;
  for (i=5;i>0;i--) {
   if (D<=((10+16)/2)) {
     j=0;
     if (lenLD<=coeffTi[i][j]) K4=coeffTi[i][5];
   }
   if (D>((10+16)/2)  &&  D<=((16+25)/2) ) {
     j=1;
     if (lenLD<=coeffTi[i][j]) K4=coeffTi[i][5];
   }
   if (D>((16+25)/2)  &&  D<=((25+40)/2) ) {
     j=2;
     if (lenLD<=coeffTi[i][j]) K4=coeffTi[i][5];
   }
   if (D>((25+40)/2)  &&  D<=((40+50)/2) ) {
     j=3;
     if (lenLD<=coeffTi[i][j]) K4=coeffTi[i][5];
   }
   if (D>((40+50)/2)) {
     j=4;
     if (lenLD<=coeffTi[i][j]) K4=coeffTi[i][5];
   }
  }
  K5=1;
  switch (RadioGroupTiTypeDetal->ItemIndex)
  {
   case 0: K5=0.92; break ;
   case 1: K5=0.90; break ;
   case 2: K5=0.70;break ;
   default : break ;
  }

  // Расчет подачи на зуб
  if (D<=0.) { Application->MessageBox("Расчет подачи на зуб \n D=0. \n Расчет прерван.","Предупреждение...",MB_OK); return ; }
  if (B<=0.) { Application->MessageBox("Расчет подачи на зуб \n B=0. \n Расчет прерван.","Предупреждение...",MB_OK); return ; }
  if (z<=0.) { Application->MessageBox("Расчет подачи на зуб \n z=0. \n Расчет прерван.","Предупреждение...",MB_OK); return ; }

  fz=0.014*pow(D,0.75)*(K1*K2*K3*K4*K5)/(pow(t,0.22)*pow(B,0.1)) ;
  s[0]='\0'; sprintf(s,"%.5lf",fz);  EditTifz->Text=s;

  // Стойкость инструмента
  str=ComboBoxTiTime->Text; timetools=atof(str.c_str());

  // коэффициенты
  K1=1;
  if (RadioGroupTiMaterial->ItemIndex==1) K1=1.2;
  K2=1;
  if (timetools==60.) { K2=1.1; }
  if (timetools==240.) { K2=0.9; }
  // отношение факт ширины фрезерования к нормативной
  K3=1;
  if ((B/D)<=0.75) K3=1.2;
  //if (((B/D)>0.75) && ((B/D)<=1.5) ) K3=1.0;
  if ((B/D)>1.5) K3=0.8;
  //
  // K4 см выше
  //
  K5=1;
  switch (RadioGroupTiTypeDetal->ItemIndex)
  {
   case 0: K5=1.00; break ;
   case 1: K5=0.93; break ;
   case 2: K5=0.85;break ;
   default : break ;
  }
  K6=1;
  switch (RadioGroupTiSurface->ItemIndex)
  {
   case 0: K6=1.00; break ;
   case 1: K6=0.90; break ;
   case 2: K6=0.85;break ;
   default : break ;
  }
  K7=1;
  if (RadioGroupTiSkin->ItemIndex==1) K7=0.7;
  K8=1;
  if (RadioGroupTiCoolnt->ItemIndex==1) K8=0.8;

  // Расчет скорости резания
  // материал ComboBoxTiMaterial
  switch (ComboBoxTiMaterial->ItemIndex)
  {
   case 0: /*ВТ20*/
     vc=17.9*pow(D,0.15);
     vc*=1.0;
     vc/=(pow(timetools,0.11)*pow(t,0.19)*pow(fz,0.45)*pow(B,0.12));
   break ;
   case 2: /*ВТ ч6*/
     vc=17.9*pow(D,0.15);
     vc*=1.0;
     vc/=(pow(timetools,0.11)*pow(t,0.19)*pow(fz,0.45)*pow(B,0.12));
     vc*=0.8;
   break ;
   default : /*ВТ22*/
     vc=27.*pow(D,0.18);
     vc*=(K1*K2*K3*K4*K5*K6*K7*K8);
     vc/=(pow(timetools,0.12)*pow(t,0.6)*pow(fz,0.58)*pow(B,0.29)*pow(z,0.1));
   break ;
  }
  s[0]='\0'; sprintf(s,"%.3lf",vc);  EditTiVc->Text=s;

  // Расчет оборотов
  tn=vc*1000./(D*PI);
  s[0]='\0'; sprintf(s,"%.3lf",tn);  EditTin->Text=s;
  EditTin->Font->Color=clGreen; // clRed; clGreen
  if (tn>=MACHINE[mach].MAXSpindel) {
    EditTin->Font->Color=clRed;
    OutputMessage("Превышение числа оборотов");
  }

  // Расчет подачи резания
  tvf=fz*z*tn;
  s[0]='\0'; sprintf(s,"%.2lf",tvf);  EditTiF->Text=s;
  EditTiF->Font->Color=clGreen; // clRed; clGreen
  if (tvf>=MACHINE[mach].MAXFeed) {
    EditTiF->Font->Color=clRed; // clRed; clGreen
    OutputMessage("Превышение максимального лимита рабочей подачи");
  }

   str=EditTiPxcoeff->Text;
   Pxcoeff=atoi(str.c_str());
   Px=Pxcoeff*pow(fz,0.755)*pow(t,0.50)*pow(B,0.65)*pow(hz,0.38);
   s[0]='\0'; sprintf(s,"%.2lf",Px);EditTiPx->Text=s;

   str=EditTiPycoeff->Text;
   Pycoeff=atoi(str.c_str());
   Py=Pycoeff*pow(fz,0.30)*pow(t,0.17)*pow(B,0.32)*pow(hz,0.75);
   s[0]='\0'; sprintf(s,"%.2lf",Py);EditTiPy->Text=s;

   tPz=sqrt(Px*Px+Py*Py);
   s[0]='\0'; sprintf(s,"%.2lf",tPz);EditTiP->Text=s;

  // Расчет Pz
  EditTiPx->Font->Color=clBlack;
  EditTiPy->Font->Color=clBlack;
  EditTiP->Font->Color=clBlack;
  switch (RadioGroupTiPz->ItemIndex)
  {
   case 0: tPz=Px; EditTiPx->Font->Color=clGreen; break ;
   case 1: tPz=Py; EditTiPy->Font->Color=clGreen; break ;
   default : /*tPz=sqrt(Px*Px+Py*Py);*/ EditTiP->Font->Color=clGreen; break ;
  }

  /*Mz*/ tMz=tPz*D/(2.*100.);
  s[0]='\0'; sprintf(s,"%.2lf",tMz); EditTiMz->Text=s;
  EditTiMz->Font->Color=clGreen; // clRed; clGreen
  if (tMz>=MACHINE[mach].Torque) {
    EditTiMz->Font->Color=clRed;
    OutputMessage("Превышение максимального момента резания (Расчет Mz)");
  }

  /*Nct*/ tNct=tPz*vc/(1020.*60.);
  s[0]='\0'; sprintf(s,"%.2lf",tNct);EditTiNct->Text=s;
  EditTiNct->Font->Color=clGreen; // clRed; clGreen
  if (tNct>=MACHINE[mach].Power) {
    EditTiNct->Font->Color=clRed;
    OutputMessage("Превышение мощности оборудования");
  }


  /*  - ЕСЛИ ДЕТАЛЬ МАЛОЙ ЖЕСТКОСТИ */
  // Расчет скорости резания
  switch (RadioGroupTiDirection->ItemIndex)
  {
   case 0: /*попутное*/
     vc=33.55*pow(D,0.26);
     vc*=1.0;
     vc/=(pow(timetools,0.29)*pow(t,0.46)*pow(fz,0.53));
   break ;
   default : /*встречное*/
     vc=13.0*pow(D,0.26);
     vc*=1.0;
     vc/=(pow(timetools,0.45)*pow(t,0.77)*pow(fz,0.92));
   break ;
  }
  s[0]='\0'; sprintf(s,"%.3lf",vc);  EditTiVsmall->Text=s;

  // Расчет оборотов
  tn=vc*1000./(D*PI);
  s[0]='\0'; sprintf(s,"%.3lf",tn);  EditTinsmall->Text=s;
  EditTinsmall->Font->Color=clBlack; // clRed; clGreen
  if (tn>=MACHINE[mach].MAXSpindel) {
    EditTinsmall->Font->Color=clRed;
    OutputMessage("ДЕТАЛЬ МАЛОЙ ЖЕСТКОСТИ-Превышение числа оборотов");
  }

  // Расчет подачи резания
  tvf=fz*z*tn;
  s[0]='\0'; sprintf(s,"%.2lf",tvf);  EditTiFsmall->Text=s;
  EditTiFsmall->Font->Color=clBlack; // clRed; clGreen
  if (tvf>=MACHINE[mach].MAXFeed) {
    EditTiFsmall->Font->Color=clRed; // clRed; clGreen
    OutputMessage("ДЕТАЛЬ МАЛОЙ ЖЕСТКОСТИ-Превышение максимального лимита рабочей подачи");
  }

  /*Nct*/ tNct=tPz*vc/(1020.*60.);
  s[0]='\0'; sprintf(s,"%.2lf",tNct);EditTiNctsmall->Text=s;

  EditTiNctsmall->Font->Color=clBlack; // clRed; clGreen
  if (tNct>=MACHINE[mach].Power) {
    EditTiNctsmall->Font->Color=clRed;
    OutputMessage("ДЕТАЛЬ МАЛОЙ ЖЕСТКОСТИ-Превышение мощности оборудования");
  }

  //Расчет завершен

  return ;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::FormCreate(TObject *Sender)
{
  int i,j;
  int k;
  double t;

  // V-STAR (FOREST-LINE)
  k=0;
  MACHINE[k].MAXSpindel=18000.;
  MACHINE[k].MAXFeed=60000.;
  MACHINE[k].Power=40.;
  MACHINE[k].Torque=57.;
  MACHINE[k].CriticSpindel=6700.;
  MACHINE[k].machine_name[0]='\0';  strcpy(MACHINE[k].machine_name,"V-STAR (FOREST-LINE)\0");
  /* Точки графика (S,Pw)*/
  t=MACHINE[k].Power;
  double SPwVSTAR[2][20]={
   {400,500,630,800,1000,1250,1600,2000,2500,3150,4000,5000,6300,6700,8000,10000,12500,16000,20000,25000},
   {  3,3.3,  4,  5,   6,   7,  10,  12,  15,  19,  23,  30,  38,   t,   t,    t,    t,    t,    t,    t}
  };
  for(i=0;i<2;i++) for(j=0;j<20;j++) MACHINE[k].SPw[i][j]=SPwVSTAR[i][j];
  /*Точки графика (S,Mz)*/
  t=MACHINE[k].Torque;
  double SMzVSTAR[2][20]={
   {400,500,630,800,1000,1250,1600,2000,2500,3150,4000,5000,6300,6700,8000,10000,12500,16000,20000,25000},
   {  t,  t,  t,  t,   t,   t,   t,   t,   t,   t,   t,   t,   t,   t,53.2,   42, 33.6, 26.6,   21, 16.8}
  };
  for(i=0;i<2;i++) for(j=0;j<20;j++) MACHINE[k].SMz[i][j]=SMzVSTAR[i][j];

  // ВС-5ВС
  k=1;
  MACHINE[k].MAXSpindel=18000.;
  MACHINE[k].MAXFeed=12000.;
  MACHINE[k].Power=25.; // 25
  MACHINE[k].Torque=1300.; //  30 1300
  MACHINE[k].CriticSpindel=5500.;
  MACHINE[k].machine_name[0]='\0';  strcpy(MACHINE[k].machine_name,"ВС-5ВС\0");
  /* Точки графика (S,Pw)*/
  t=MACHINE[k].Power;
  double SPwVS5[2][20]={
   {400,500,630,800,1000,1250,1600,2000,2500,3150,4000,5000,6300,6700,8000,10000,12500,16000,17000,18000},
   {  3,3.3,  4,  5,   6,   7,  10,  12,  15,  19,  22,  23,  24,   t,   t,    t,    t,    t,    t,    t}
  };
  for(i=0;i<2;i++) for(j=0;j<20;j++) MACHINE[k].SPw[i][j]=SPwVS5[i][j];
  /*Точки графика (S,Mz)*/
  t=MACHINE[k].Torque;
  double SMzVS5[2][20]={
   {400,500,630,800,1000,1250,1600,2000,2500,3150,4000,5000,6300,6700,8000,10000,12500,16000,17000,18000},
   {  t,  t,  t,  t,   t,   t,   t,   t,   t,   t,   t,   t,   t,   t,1000.2, 900, 800, 700,  500, 300}
  };
  for(i=0;i<2;i++) for(j=0;j<20;j++) MACHINE[k].SMz[i][j]=SMzVS5[i][j];

  // DMF-500L
  k=2;
  MACHINE[k].MAXSpindel=18000.;
  MACHINE[k].MAXFeed=20000.;
  MACHINE[k].Power=25.; //
  MACHINE[k].Torque=87.; //
  MACHINE[k].CriticSpindel=2800.;
  MACHINE[k].machine_name[0]='\0';  strcpy(MACHINE[k].machine_name,"DMF-500L\0");
  /* Точки графика (S,Pw)*/
  t=MACHINE[k].Power;
  double SPwDMF500L[2][20]={
   { 20, 25, 35, 63, 87, 125,245,315,630, 850, 1100 , 1600 ,2000,2800,4000,6300,8000,10000,15000,18000},
   {0.6,0.8,  1,1.6,  2, 2.5,  4,  5,  8,  10,  12.5,  16,  20,   t,   t,   t,   t,    t,    t,    t}
  };
  for(i=0;i<2;i++) for(j=0;j<20;j++) MACHINE[k].SPw[i][j]=SPwDMF500L[i][j];
  /*Точки графика (S,Mz)*/
  t=MACHINE[k].Torque;
  double SMzDMF500L[2][20]={
   { 20, 25, 35, 63, 87, 125,245,315,630, 850, 1100, 1600,2000,2800,4000,6300,8000,10000,15000,18000},
   {  t,  t,  t,  t,  t,   t,  t,  t,  t,   t,    t,    t,   t,   t,  63,  40,31.5,   25, 15.9, 13.3}
  };
  for(i=0;i<2;i++) for(j=0;j<20;j++) MACHINE[k].SMz[i][j]=SMzDMF500L[i][j];

  // DMU-125PA
  k=3;
  MACHINE[k].MAXSpindel=12000.;
  MACHINE[k].MAXFeed=10000.;
  MACHINE[k].Power=19.; //
  MACHINE[k].Torque=82.; //
  MACHINE[k].CriticSpindel=2200.;
  MACHINE[k].machine_name[0]='\0';  strcpy(MACHINE[k].machine_name,"DMU-125PA\0");
  /* Точки графика (S,Pw)*/
  t=MACHINE[k].Power;
  double SPwDMU125PA[2][20]={
   { 20, 25, 50,  63,100, 160,  200, 315, 630, 800, 1000, 1600,2000,2200,4000,5000,6300,8000,10000,12000},
   {0.4,0.6,  1,1.25,1.8, 2.5, 3.15,4.25,   7,   9,   10,   15,  18,   t,   t,   t,   t,   t,    t,    t}
  };
  for(i=0;i<2;i++) for(j=0;j<20;j++) MACHINE[k].SPw[i][j]=SPwDMU125PA[i][j];
  /*Точки графика (S,Mz)*/
  t=MACHINE[k].Torque;
  double SMzDMU125PA[2][20]={
   { 20, 25, 50, 63, 100, 160,  200, 315, 630, 800, 1000, 1600,2000,2200,4000,5000,6300,8000,10000,12000},
   {  t,  t,  t,  t,   t,   t,   t,   t,   t,   t,    t,    t,   t,   t,  50,  38,31.5,  25,   20,   15}
  };
  for(i=0;i<2;i++) for(j=0;j<20;j++) MACHINE[k].SMz[i][j]=SMzDMU125PA[i][j];

  // Variaxis 630-5X
  k=4;
  MACHINE[k].MAXSpindel=12000.;
  MACHINE[k].MAXFeed=50000.;
  MACHINE[k].Power=20.; //
  MACHINE[k].Torque=30.; //
  MACHINE[k].CriticSpindel=2800.;
  MACHINE[k].machine_name[0]='\0';  strcpy(MACHINE[k].machine_name,"Variaxis 630-5X\0");
  /* Точки графика (S,Pw)*/
  t=MACHINE[k].Power;
  double SPwVariaxis630[2][20]={
   { 20, 25, 50,  63,100, 160,  200, 315, 630, 800, 1000, 1600,2000,2800,4000,5000,6300,8000,10000,12000},
   {0.4,0.6,  1,1.25,1.8, 2.5, 3.15,4.25,   7,   9,   10,   15,  18,   t,   t,   t,   t,   t,    t,    t}
  };
  for(i=0;i<2;i++) for(j=0;j<20;j++) MACHINE[k].SPw[i][j]=SPwVariaxis630[i][j];
  /*Точки графика (S,Mz)*/
  t=MACHINE[k].Torque;
  double SMzVariaxis630[2][20]={
   { 20, 25, 50, 63, 100, 160,  200, 315, 630, 800, 1000, 1600,2000,2800,4000,5000,6300,8000,10000,12000},
   {  t,  t,  t,  t,   t,   t,   t,   t,   t,   t,    t,    t,   t,   t,  28,  26, 21.5,  15,   10,    5}
  };
  for(i=0;i<2;i++) for(j=0;j<20;j++) MACHINE[k].SMz[i][j]=SMzVariaxis630[i][j];

  //
  //.....
  //

  try
  {
    TabMach->Items->Clear();
    for (i=0;i<NUM_MACHINE;i++) { TabMach->Items->Append(MACHINE[i].machine_name); }
    //TabMach->Tabs->Assign(ListMachine);
  }
  __finally //(...)
  {
   	 ;
  }
  TabMach->ItemIndex=0;
  TabMachChange(Sender);

  PageControlParam->ActivePageIndex=0;
  PageControlParamChange(Sender);
  //PageControlMethods->ActivePageIndex=0;

  EditStn->Text="0.0";
  EditStVc->Text="0.0";
  EditStP->Text="0.0";
  EditStPn->Text="0.0";
  EditStM->Text="0.0";
  EditSthm2->Text="0.0";
  EditStQ->Text="0.0";
  EditStVf->Text="0.0";
  EditStfz->Text="0.0";

  ComboBoxStContact->ItemIndex=0;
  Graphics::TBitmap *Bitmap0= new Graphics::TBitmap();
  Graphics::TBitmap *Bitmap1= new Graphics::TBitmap();
  try
  {
    ImageListvs->GetBitmap(0 , Bitmap0);
    Imagevs->Picture->Bitmap=Bitmap0 ;
    ImageListhm->GetBitmap(0 , Bitmap1);
    Imagehm->Picture->Bitmap=Bitmap1 ;
  }
  catch (...)
  {

  }
  delete Bitmap0 ;
  delete Bitmap1 ;
  EditStk->Text="90.0";
  ComboBoxStkpd->ItemIndex=0;
  CheckBoxhm2->Checked=False;
  EditaeDc->Text="";
  EditStKc11->Text="700.01";
  EditStMc->Text="0.2501";
  EditStGamma0->Text="2.0";
  EditStKc->Text="0.0";
  CheckBoxhm2Click(Sender);

 for (i=0;i<4;i++) for (j=0;j<9;j++) StringGridTab->Cells[i][j]="";

 RadioButton1->Checked=False;
 RadioButton2->Checked=False;
 RadioButton3->Checked=False;
 RadioButton4->Checked=True; // True
 RadioButton5->Checked=False;

 StringGridTab->Cells[0][0]="Материал";
 StringGridTab->Cells[1][0]="Rm (H/(mm*mm)" ;
 StringGridTab->Cells[2][0]="kc1.1 (H/(mm*mm)" ;
 StringGridTab->Cells[3][0]="Mc";

 RadioButton4Click(Sender);

 StaticTextStMaterial->Caption=RadioButton4->Caption;

  try
  {
    //  0=В95пчТ2  1=1163Т2   2=Д16чТ
    ComboBoxMaterial->Items->Clear();
    ComboBoxMaterial->Items->Append("В95пчТ2");
    ComboBoxMaterial->Items->Append("1163Т2");
    ComboBoxMaterial->Items->Append("Д16чТ");
  }
  catch (...)
  {

  }
  ComboBoxMaterial->ItemIndex=0;
  ComboBoxMaterialChange(Sender);


  try
  {
    //
    ComboBoxRa->Items->Clear();
    ComboBoxRa->Items->Append("...<=1.25");
    ComboBoxRa->Items->Append("1.25<...<=2.5");
    ComboBoxRa->Items->Append("2.5<...<=20.");
  }
  catch (...)
  {

  }
  ComboBoxRa->ItemIndex=0;
  ComboBoxRaChange(Sender);

  ComboBoxTkpd->ItemIndex=0;

  EditSz->Text="0.0";
  EditV->Text="0.0";
  EditV1->Text="0.0";
  EditV2->Text="0.0";
  EditTm->Text="20.0";
  Editn->Text="0.0";
  EditF->Text="0.0";
  EditPz->Text="0.0";
  EditMz->Text="0.0";
  EditNp->Text="0.0";

  PanelFormulaAl->BeginDrag(True,0);
  PanelFormulaAl->EndDrag(True);
  PanelFormulaAl->Visible=False;

  PanelStMaterial->BeginDrag(True,0);
  PanelStMaterial->EndDrag(True);
  PanelStMaterial->Visible=False;

 for(i=0;i<10;i++)  for(j=0;j<9;j++) { StringGridT->Cells[j][i]=IntToStr(Temperature[i][j]) ; }
 StringGridT->Cells[0][0]="";

  PanelTemp->BeginDrag(True,0);
  PanelTemp->EndDrag(True);
  PanelTemp->Visible=False;

  StringGridP->Cells[0][0]=" n (об/мин)";
  StringGridP->Cells[1][0]=" P (кН)";

  StringGridP->Cells[0][1]="...."; // наше
  StringGridP->Cells[0][2]="6000";
  StringGridP->Cells[0][3]="8000";
  StringGridP->Cells[0][4]="10000";
  StringGridP->Cells[0][5]="12000";
  StringGridP->Cells[0][6]="15000";
  StringGridP->Cells[0][7]="18000";

  AnsiString str=ComboBoxTkpd->Text;
  double kpd=atof(str.c_str());
  char s[20];
  StringGridP->Cells[1][1]="...."; // наше
  s[0]='\0'; sprintf(s,"2.00 • %.2lf  =%.2lf",kpd,2.00*kpd);
  StringGridP->Cells[1][2]=s;
  s[0]='\0'; sprintf(s,"1.70 • %.2lf  =%.2lf",kpd,1.70*kpd);
  StringGridP->Cells[1][3]=s;
  s[0]='\0'; sprintf(s,"1.50 • %.2lf  =%.2lf",kpd,1.50*kpd);
  StringGridP->Cells[1][4]=s;
  s[0]='\0'; sprintf(s,"1.35 • %.2lf  =%.2lf",kpd,1.35*kpd);
  StringGridP->Cells[1][5]=s;
  s[0]='\0'; sprintf(s,"1.25 • %.2lf  =%.2lf",kpd,1.25*kpd);
  StringGridP->Cells[1][6]=s;
  s[0]='\0'; sprintf(s,"1.20 • %.2lf  =%.2lf",kpd,1.20*kpd);
  StringGridP->Cells[1][7]=s;

  BaseTiD->ItemIndex=0;
  BaseTiZ->ItemIndex=0;
  BaseTiL->ItemIndex=0;
  BaseTiB->Text="0.00000";
  BaseTiT->Text="0.00000";
  ComboBoxTiTime->ItemIndex=1;
  EditTifz->Text="0.00000";
  EditTin->Text="0.00000";
  EditTiF->Text="0.00000";
  EditTiVc->Text="0.00000";
  EditTiMz->Text="0.00000";
  EditTiNct->Text="0.00000";
  ComboBoxTiMaterial->ItemIndex=0;
  ComboBoxTiMaterialChange(Sender);
  RadioGroupTiSurfaceClick(Sender);
  RadioGroupTiDirectionClick(Sender);
  EditTiVsmall->Text="0.00000";
  EditTinsmall->Text="0.00000";
  EditTiFsmall->Text="0.00000";
  EditTiNctsmall->Text="0.00000";
  BaseTihz->ItemIndex=2;
  EditTiPx->Text="0.00000";
  EditTiPy->Text="0.00000";
  EditTiP->Text="0.00000";
  RadioGroupTiPzClick(Sender);

  PanelCalc->Visible=False; //False
  PanelCalc->BeginDrag(True,0);
  PanelCalc->EndDrag(True);
  PanelCalc->Visible=False; //False

  N2Click(Sender);
/* TabSheetStandard->TabVisible=False;
   TabSheetTechAl->TabVisible=False;
   TabSheetTechTi->TabVisible=False;
*/
  return ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PageControlParamChange(TObject *Sender)
{
  PageControlMethods->ActivePageIndex=PageControlParam->ActivePageIndex;
  PanelCalc->Caption=PageControlMethods->ActivePage->Caption;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnGraphicClick(TObject *Sender)
{
  String str1,str2,str3;
  int i,j; double x,y ,n,p,m;
  int mach; // станок

  mach=TabMach->ItemIndex;

  try
  {

    FormGraphic->Series1->Clear();
    FormGraphic->Series2->Clear();
    FormGraphic->Series3->Clear();
    FormGraphic->Series4->Clear();

    for (i=0;i<20;i++) {
      // Точки графика (S,Pw)
      x=MACHINE[mach].SPw[0][i];
      y=MACHINE[mach].SPw[1][i];
      FormGraphic->Series1->AddXY(x,y,"",clTeeColor);
      // Точки графика (S,Mz)
      x=MACHINE[mach].SMz[0][i];
      y=MACHINE[mach].SMz[1][i];
      FormGraphic->Series2->AddXY(x,y,"",clTeeColor);
    }

    i=PageControlParam->ActivePageIndex;
    //j=PageControlMethods->ActivePageIndex;
    //if (i!=j) { i=j; }
    x=0;y=0;n=0;p=0;m=0;
    switch (i)
    {
     case 0 : // EditStn EditStPn EditStM
      str1=EditStn->Text; str2=EditStPn->Text; str3=EditStM->Text;
     break ;
     case 1 : // Editn EditNp EditMz
      str1=Editn->Text; str2=EditNp->Text; str3=EditMz->Text;
     break ;
     case 2 : // EditTin EditTiNct EditTiMz
      str1=EditTin->Text; str2=EditTiNct->Text; str3=EditTiMz->Text;
     break ;
     default : break ;
    }
    n=atof(str1.c_str()); if (n<=0.) { n=0 ; }
    p=atof(str2.c_str()); if (p<=0.) { p=0 ; }
    m=atof(str3.c_str()); if (m<=0.) { m=0 ; }

    FormGraphic->Series3->AddXY(n,p,"",clTeeColor);
    FormGraphic->Series4->AddXY(n,m,"",clTeeColor);

  }
  catch (...)
  {

  }

  FormGraphic->Show();

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PageControlMethodsChange(TObject *Sender)
{
  PageControlParam->ActivePageIndex=PageControlMethods->ActivePageIndex;
  PanelCalc->Caption=PageControlMethods->ActivePage->Caption;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TFormMain::BitCalcsClick(TObject *Sender)
{
  int i,j;
  i=PageControlParam->ActivePageIndex;
  j=PageControlMethods->ActivePageIndex;
  if (i!=j) {
    i=j;
    PageControlMethodsChange(Sender);
  }
  PanelCalc->Visible=True;//False
  switch (i)
  {
   case 0 : CalcStandart(Sender);   break ;
   case 1 :  CalcTechAl(Sender);    break ;
   case 2 :  CalcTechTi(Sender);    break ;
   default :
     ShowMessage("Nothing");
   break ;
  }

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxStContactChange(TObject *Sender)
{
  Graphics::TBitmap *Bitmap0= new Graphics::TBitmap();
  try
  {
    ImageListvs->GetBitmap(ComboBoxStContact->ItemIndex , Bitmap0);
    Imagevs->Picture->Bitmap=Bitmap0 ;
  }
  catch (...)
  {

  }
  delete Bitmap0 ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CheckBoxhm2Click(TObject *Sender)
{
 int i;
 if (CheckBoxhm2->Checked==True) {
   EditStk->Enabled=False;
   i=1;
  } else {
   EditStk->Enabled=True;
   i=0;
 }
  Graphics::TBitmap *Bitmap0= new Graphics::TBitmap();
  try
  {
    ImageListhm->GetBitmap(i , Bitmap0);
    Imagehm->Picture->Bitmap=Bitmap0 ;
  }
  catch (...)
  {

  }
  delete Bitmap0 ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnStMaterialClick(TObject *Sender)
{
   PanelStMaterial->Visible=True;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N1Click(TObject *Sender)
{
 //Application->MessageBox("Брагина Г.С. - 2004\n Программа создана по руководству 'Технологические рекомендации - выбор режимов резания для алюминиевых и титановых сплавов'","Об авторах..",MB_OK);
 try
  {
    Application->CreateForm(__classid(TFormAbout), &FormAbout);
    FormAbout->ShowModal();
    delete FormAbout;
  }
  catch (Exception &exception)
  {
    ;//Application->ShowException(&exception);
  }
  //OutputMessage("Об авторах..\n\n Автор: Брагина Г.С. - 2004\n Программа создана по руководству 'Технологические рекомендации - выбор режимов резания для алюминиевых и титановых сплавов'");
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N2Click(TObject *Sender)
{
  BaseD->Text="25.0";
  BaseZ->Text="3";
  BaseB->Text="15.0";
  BaseT->Text="5.0";
  BaseVc->Text="1060.0";
  Basefz->Text="0.1";

  BaseAlD->Text="25.0";
  BaseAlZ->Text="3";
  BaseAlB->Text="15.0";
  BaseAlT->Text="5.0";
  ComboBoxMaterial->ItemIndex=0;
  ComboBoxRa->ItemIndex=0;
  ComboBoxTkpd->ItemIndex=0;

  BaseTiD->Text="25.0";
  BaseTiZ->Text="3";
  BaseTiL->Text="70.0";
  BaseTiB->Text="15.0";
  BaseTiT->Text="5.0";
  ComboBoxTiTime->ItemIndex=1;
  BaseTihz->ItemIndex=2;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N4Click(TObject *Sender)
{
   MessageBeep(0);
   Application->Terminate();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::BitBtnHelpClick(TObject *Sender)
{
  FormHelp->Show();
}

//---------------------------------------------------------------------------

void __fastcall TFormMain::N13Click(TObject *Sender)
{
  FormMessage->Visible=True;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N25Click(TObject *Sender)
{
   FormMessage->ElFlatMemoMes->Lines->Clear();
   OutputMessage("Литература..");
   OutputMessage("    ");
   OutputMessage("1. ТР 1.4.2104-2002 ");
   OutputMessage("    Технологические рекомендации.");
   OutputMessage("    Выбор параметров концевых фрез и режимов резания высокоскоростного");
   OutputMessage("    фрезерования алюминиевых и титановых сплавов.");
   OutputMessage("2. Общий каталог - WALTER, Т1,Т2");
   OutputMessage("3. Справочник технолога-машиностроителя, Т1,Т2");
   OutputMessage("     /Под. общ. ред. А.Г. Косиловой и Р.К. Мещерикова - М. Машиностроение,1985 - 496с.");
   OutputMessage("     ");
   OutputMessage(".......     ");
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::NMesWinClick(TObject *Sender)
{
   NMesWin->Checked=!(NMesWin->Checked);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Image9Click(TObject *Sender)
{
  /*Скорость резания*/
  BaseVc->Text=EditStVc->Text; BaseVc->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Image10Click(TObject *Sender)
{
  Basefz->Text=EditStfz->Text; Basefz->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxMaterialChange(TObject *Sender)
{
  int i;
  i=ComboBoxMaterial->ItemIndex; //  0-"В95пчТ2"  1-"1163Т2"  2-"Д16чТ"
  ComboBoxRaChange(Sender);
 try
 {
  switch (i)
  {
    case 0:
     //fz
     EditTkoef1->Text="0.04";
     EditTX1->Text="0.75";
     EditTY1->Text="0.1";
     EditTZ1->Text="0.22";
     //EditTKs->Text="1.0";

     //V1
     EditTkoef2->Text="1097.0";
     EditTX2->Text="0.86";
     EditTY2->Text="0.81";
     EditTZ2->Text="0.47";

     //V2
     EditTkoef3->Text="49.0";
     EditTX3->Text="0.684";
     EditTY3->Text="0.548";
     EditTZ3->Text="0.286";
     EditTsm1->Text="1.134";

     //P
     EditTkoef4->Text="246";
     EditTX4->Text="0.684";
     EditTY4->Text="0.546";
     EditTZ4->Text="0.168";
     EditTsm2->Text="0.118";

    break;
    case 1:case 2:
     //fz
     EditTkoef1->Text="0.04";
     EditTX1->Text="0.75";
     EditTY1->Text="0.1";
     EditTZ1->Text="0.22";
     //EditTKs->Text="1.0";

     //V1
     EditTkoef2->Text="2070.0";
     EditTX2->Text="0.88";
     EditTY2->Text="0.80";
     EditTZ2->Text="0.46";

     //V2
     EditTkoef3->Text="52.0";
     EditTX3->Text="0.56";
     EditTY3->Text="0.344";
     EditTZ3->Text="0.225";
     EditTsm1->Text="1.126";

     //P
     EditTkoef4->Text="224";
     EditTX4->Text="0.506";
     EditTY4->Text="0.344";
     EditTZ4->Text="0.123";
     EditTsm2->Text="0.112";

    break;
    default : break ;
  }
 }
 catch (Exception &exception)
 {
     ;// Application->ShowException(&exception);
  }

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::RadioButton1Click(TObject *Sender)
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

 bool CanSelect=True;
 StringGridTabSelectCell(Sender,1, 1, CanSelect); // bool &CanSelect

  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::RadioButton2Click(TObject *Sender)
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

 bool CanSelect=True;
 StringGridTabSelectCell(Sender,1, 1, CanSelect); // bool &CanSelect

  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::RadioButton3Click(TObject *Sender)
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

 bool CanSelect=True;
 StringGridTabSelectCell(Sender,1, 1, CanSelect); // bool &CanSelect

  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::RadioButton4Click(TObject *Sender)
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

 bool CanSelect=True;
 StringGridTabSelectCell(Sender,1, 1, CanSelect); // bool &CanSelect

  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::RadioButton5Click(TObject *Sender)
{
  if (RadioButton5->Checked==True) {

  int i,j; for (i=0;i<4;i++) for (j=1;j<9;j++) StringGridTab->Cells[i][j]="";

  StringGridTab->Font->Color= clOlive;

   // Жаропрочные сплавы и титановые сплавы S
 StringGridTab->Cells[0][1]="Жаропрочные сплавы, содержащие никель, кобальт, железо, твердость<30HRc, инколой 800 и инконель 601, 617 и 625,монель 400";
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

 bool CanSelect=True;
 StringGridTabSelectCell(Sender,1, 1, CanSelect); // bool &CanSelect

  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnStMaterialOutClick(TObject *Sender)
{
   int i ;  i=StringGridTab->Row  ;
    AnsiString s;
    /* s=IntToStr(i) + " kc=" + StringGridTab->Cells[2][i] + " mc=" + StringGridTab->Cells[3][i] ;
     ShowMessage(s); */

   if (RadioButton1->Checked==True) s=RadioButton1->Caption;
   if (RadioButton2->Checked==True) s=RadioButton2->Caption;
   if (RadioButton3->Checked==True) s=RadioButton3->Caption;
   if (RadioButton4->Checked==True) s=RadioButton4->Caption;
   if (RadioButton5->Checked==True) s=RadioButton5->Caption;

   StaticTextStMaterial->Caption=s;

   EditStKc11->Text = StringGridTab->Cells[2][i];
   EditStMc->Text= StringGridTab->Cells[3][i] ;

   PanelStMaterial->Visible=False;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::StringGridTabSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
 //CanSelect=(StringGridTab->Cells[0][ARow]=="");
 StaticTextMaterial->Caption= StringGridTab->Cells[0][ARow];
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnPanelAlClick(TObject *Sender)
{
   PanelFormulaAl->Visible=True;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxRaChange(TObject *Sender)
{
  int i,k; char s[15];  double Ks;
  i=ComboBoxMaterial->ItemIndex; //  0-"В95пчТ2"  1-"1163Т2"  2-"Д16чТ"
  k=ComboBoxRa->ItemIndex;  // ...<=1.25   1.25<...<=2.5    2.5<...<=20.
  Ks=1;
  switch (i)
  {
   case 0 :
    if (k==0) Ks=0.885*0.66; else Ks=0.885*1.0;
   break;
   case 1 :
    if (k==2) Ks=0.885*1.5; else Ks=1.0*1.0;
   break;
   case 2 :
    if (k==2) Ks=1.0*1.5; else Ks=1.0*0.66;
   break;
   default :
   break;
  }

  s[0]='\0'; sprintf(s,"%.3lf",Ks);  EditTKs->Text=s;

  //BitCalcsClick(Sender);

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::EditVChange(TObject *Sender)
{
  int i;
  /**************************************************************************/
  /* Расчет */
  /*n*/  i=CalcTechAln(Sender);  if (i<0) return ;
  /*F*/  i=CalcTechAlF(Sender);  if (i<0) return ;
  /*Temperature*/ CalcTechAlTemp(Sender);
  /*Pz*/ i=CalcTechAlPz(Sender);  if (i<0) return ;
  /*Mz*/ i=CalcTechAlMz(Sender);  if (i<0) return ;
  /*Nct*/ i=CalcTechAlNct(Sender);  if (i<0) return ;
  /**************************************************************************/
  return ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnAlTm1Click(TObject *Sender)
{
  PanelTemp->Visible=True;
  CalcTechAlTemp (Sender);
  //PanelTemp->Update();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnAln1Click(TObject *Sender)
{
   CalcTechAln(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnAlF1Click(TObject *Sender)
{
   CalcTechAlF(Sender);
   CalcTechAlTemp (Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnAlPz1Click(TObject *Sender)
{
   CalcTechAlPz(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnAlMz1Click(TObject *Sender)
{
   CalcTechAlMz(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnAlNp1Click(TObject *Sender)
{
   CalcTechAlNct(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtn1fzClick(TObject *Sender)
{
  CalcTechAlSz(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtn2V1Click(TObject *Sender)
{
  CalcTechAlV1(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtn3V2Click(TObject *Sender)
{
  CalcTechAlV2(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtn4PClick(TObject *Sender)
{
  CalcTechAlPz(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::RadioGroupTiSurfaceClick(TObject *Sender)
{
  Graphics::TBitmap *Bitmap0= new Graphics::TBitmap();
  try
  {
    ImageListSurface->GetBitmap(RadioGroupTiSurface->ItemIndex , Bitmap0);
    ImageSurface->Picture->Bitmap=Bitmap0 ;
  }
  catch (...)
  {

  }
  delete Bitmap0 ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::RadioGroupTiDirectionClick(TObject *Sender)
{
  Graphics::TBitmap *Bitmap0= new Graphics::TBitmap();
  Graphics::TBitmap *Bitmap1= new Graphics::TBitmap();
  try
  {
    ImageListDirection->GetBitmap(RadioGroupTiDirection->ItemIndex , Bitmap0);
    ImageDirection->Picture->Bitmap=Bitmap0 ;
    ImageListTiVsmall->GetBitmap(RadioGroupTiDirection->ItemIndex , Bitmap1);
    ImageTiVsmall->Picture->Bitmap=Bitmap1 ;
  }
  catch (...)
  {

  }
  delete Bitmap0 ;
  delete Bitmap1 ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxTiMaterialChange(TObject *Sender)
{
  Graphics::TBitmap *Bitmap0= new Graphics::TBitmap();
  try
  {
    ImageListTiMaterial->GetBitmap(ComboBoxTiMaterial->ItemIndex , Bitmap0);
    ImageTiVbase->Picture->Bitmap=Bitmap0 ;
  }
  catch (...)
  {

  }
  delete Bitmap0 ;

  switch (ComboBoxTiMaterial->ItemIndex)
  {
   case 0:case 1:
   EditTiPxcoeff->Text="3850.";
   EditTiPycoeff->Text="1040.";
   break;
   default :
   EditTiPxcoeff->Text="11480.";
   EditTiPycoeff->Text="8570.";
   break;
  }

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::RadioGroupTiPzClick(TObject *Sender)
{
  EditTiPx->Font->Color=clBlack;
  EditTiPy->Font->Color=clBlack;
  EditTiP->Font->Color=clBlack;
  switch (RadioGroupTiPz->ItemIndex)
  {
   case 0: EditTiPx->Font->Color=clGreen; break ;
   case 1: EditTiPy->Font->Color=clGreen; break ;
   default : EditTiP->Font->Color=clGreen; break ;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TabMachChange(TObject *Sender)
{
   int i; char s[250];
   i=TabMach->ItemIndex;

   s[0]='\0';
   sprintf(s,"n=%.0lf (об/мин)\[n(крит)=%.0lf(об/мин)\] F=%.0lf (мм/мин) Nct=%.0lf (кВт) Mz=%.0lf (Hm)", \
     MACHINE[i].MAXSpindel,MACHINE[i].CriticSpindel,MACHINE[i].MAXFeed,MACHINE[i].Power,MACHINE[i].Torque );
   StatusBarMes->SimpleText=s;
   s[0]='\0'; sprintf(s,"Расчет режимов резания: %s ",MACHINE[i].machine_name);
   FormMain->Caption = AnsiString(s);
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::BitBtnSt1Click(TObject *Sender)
{
   CalcStnV(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnSt2Click(TObject *Sender)
{
   CalcStQ(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnSt3Click(TObject *Sender)
{
   CalcSthm(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnSt4Click(TObject *Sender)
{
   CalcStFSz(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnSt5Click(TObject *Sender)
{
   CalcStNctMz(Sender);
}
//---------------------------------------------------------------------------


