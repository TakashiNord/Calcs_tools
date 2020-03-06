//---------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "about.h"
#include "graphic.h"
#include "Help.h"
#include "Message.h"

#define PI  3.1415926
#define NUM_MACHINE  1

 struct DEF_MACHINE {
     double MAXSpindel ; // S(��/���)
     double MAXFeed    ; // F(��/���)
     double Power      ; // Nct(kW)
     double Torque     ; // Mz(Hm)
     char   machine_name[250] ;
     double SPw[15][15]  ; // ����� ������� (S,Pw)
     double SMz[15][15]  ; // ����� ������� (S,Mz)
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
 {12000,49,57,62,66,69,72,74,76},
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


void TFormMain::OutputMessage(AnsiString s)
{
    //FormMessage->ElFlatMemoMes->Lines->Append(s);
    if (NMesWin->Checked==True) FormMessage->Visible=True;
}

void TFormMain::CalcStandart(TObject *Sender)
{
  double vd=0.; int vi=0 ; char s[80];
  String str;
  int mach; // ������
  double D,z,ap,ae,vc,fz; //�������� ������
  double ak,fsi ; // �������
  double kpd,gamma0, kc11, mc ; // ��������
  double tn,tvf,Q,thm,tkc,tNmot; // �������������� ��������� : ..., ��������

  mach=TabMach->TabIndex;

 /* �������� �� ������������ ����� �������� ��������
    StrToFloat(str) FloatToStr(vd) FloatToText(s,vd,fvExtended, ffFixed,8,8);*/
  /*�������*/
  str=BaseD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=0.;
    s[0]='\0'; sprintf(s,"%.3lf",D); BaseD->Text=s; BaseD->SetFocus(); BaseD->SelectAll();
    Application->MessageBox("������ ������� '�������� �����������' - D","��������������...",MB_OK);
    return ;
  }

  /*����� ������*/
  str=BaseZ->Text;
  vi=atoi(str.c_str());
  if (vi<=0) {
    vi=0;
    s[0]='\0'; sprintf(s,"%d",vi); BaseZ->Text=s; BaseZ->SetFocus(); BaseZ->SelectAll();
    Application->MessageBox("������ ������� '����� ������' - z","��������������...",MB_OK);
    return ;
  }
  z=(double)vi;

  /*������ �������*/
  str=BaseB->Text;
  ap=atof(str.c_str());
  if (ap<=0.) {
    ap=0.;
    s[0]='\0'; sprintf(s,"%.4lf",ap); BaseB->Text=s; BaseB->SetFocus(); BaseB->SelectAll();
    Application->MessageBox("������ ������� '������ ����' - ap","��������������...",MB_OK);
    return ;
  }

  /*������� ����*/
  str=BaseT->Text;
  ae=atof(str.c_str());
  if (ae<=0.) {
    ae=0;
    s[0]='\0'; sprintf(s,"%.4lf",ae); BaseT->Text=s; BaseT->SetFocus(); BaseT->SelectAll();
    Application->MessageBox("������ ������� '������� ����' - ae","��������������...",MB_OK);
    return ;
  }

  /*�������� �������*/
  str=BaseVc->Text;
  vc=atof(str.c_str());
  if (vc<=0.) {
    vc=0;
    s[0]='\0'; sprintf(s,"%.3lf",vc); BaseVc->Text=s; BaseVc->SetFocus(); BaseVc->SelectAll();
    Application->MessageBox("������ ������� '�������� �������' - vc","��������������...",MB_OK);
    return ;
  }

  /*������ �� ���*/
  str=Basefz->Text;
  fz=atof(str.c_str());
  if (fz<=0.) {
    fz=0;
    s[0]='\0'; sprintf(s,"%.5lf",fz);
    Basefz->Text=s; Basefz->SetFocus(); Basefz->SelectAll();
    Application->MessageBox("������ ������� '������ �� ���' - fz","��������������...",MB_OK);
    return ;
  }

  // ������ ��������
  if (D<=0.) { Application->MessageBox("������ �������� \n D=0. \n ������ �������.","��������������...",MB_OK); return ; }
  tn=vc*1000./(D*PI);
  s[0]='\0'; sprintf(s,"%.3lf",tn);  EditStn->Text=s;
  EditStn->Font->Color=clGreen; // clRed; clGreen
  if (tn>=MACHINE[mach].MAXSpindel) {
    EditStn->Font->Color=clRed;
    OutputMessage("���������� ����� ��������");
  }

  // ������ �������� �������
  vc=(D*PI*tn)/1000.;
  s[0]='\0'; sprintf(s,"%.3lf",vc);  EditStVc->Text=s;

  // ������ ������ �������
  tvf=fz*z*tn;
  s[0]='\0'; sprintf(s,"%.2lf",tvf);  EditStVf->Text=s;
  EditStVf->Font->Color=clGreen; // clRed; clGreen
  if (tvf>=MACHINE[mach].MAXFeed) {
    EditStVf->Font->Color=clRed; // clRed; clGreen
    OutputMessage("���������� ������������� ������ ������� ������");
  }

  // ������ ������ �� ���
  if (tn<=0.) { Application->MessageBox("������ ������ �� ��� \n n=0. \n ������ �������.","��������������...",MB_OK); return ; }
  if (z<=0.) { Application->MessageBox("������ ������ �� ��� \n z=0. \n ������ �������.","��������������...",MB_OK); return ; }
  fz=tvf/(z*tn);
  s[0]='\0'; sprintf(s,"%.5lf",fz);  EditStfz->Text=s;

  // �������� ����� ���������
  Q=(ae*ap*tvf)/1000.;
  s[0]='\0'; sprintf(s,"%.5lf",Q); EditStQ->Text=s;

  // ������� ������� �������
  if (D<=0.) { Application->MessageBox("������� ������� ������� \n D=0. \n ������ �������.","��������������...",MB_OK); return ; }
  vi=ComboBoxStContact->ItemIndex;
  fsi=0.1;
  if (vi==0) { fsi=180./PI*2.*asin(ae/D); } else { fsi=90.+180./PI*asin((ae-D/2.)/(D/2.)); }
  vd=ae/D;
  s[0]='\0'; sprintf(s,"%.2lf",vd); EditaeDc->Text=s;
  if (vd<=0.3) { CheckBoxhm2->Checked=True; } else { CheckBoxhm2->Checked=False; }
  CheckBoxhm2Click(Sender);
  thm=0.1;
  if (CheckBoxhm2->Checked==True) {
     thm=fz*sqrt(ae/D);
  } else {
     /*���� k*/
     str=EditStk->Text;
     ak=atof(str.c_str());
     if (ak<=0. || ak>=180.) {
       ak=2.;
       s[0]='\0'; sprintf(s,"%.3lf",ak); EditStk->Text=s; EditStk->SetFocus(); EditStk->SelectAll();
       OutputMessage("���� k - ��������� ����� <=0 || >=180 -> ���������� k=2");
     }
     thm=114.7*fz*sin(ak*PI/180.)*(ae/D);
     thm=fabs(thm-floor(thm)); //thm=fabs(thm-ceil(thm));
     if (fsi<=0) fsi=0.001;
     thm/=fsi;
  }
  s[0]='\0'; sprintf(s,"%.8lf",thm); EditSthm2->Text=s;

  // ������ �������� �������
  str=ComboBoxStkpd->Text; kpd=atof(str.c_str());
  /*������� �������� ����*/
  str=EditStGamma0->Text;
  gamma0=atof(str.c_str());
  if (gamma0<=0. || gamma0>=100.) {
    gamma0=2.;
    s[0]='\0'; sprintf(s,"%.3lf",gamma0);
    EditStGamma0->Text=s; EditStGamma0->SetFocus(); EditStGamma0->SelectAll();
    OutputMessage("������� �������� ���� (�����) - ��������� ����� <=0 || >=100 -> ���������� gamma=2");
  }
  /*���� ��������� kc11*/
  str=EditStKc11->Text;
  kc11=atof(str.c_str());
  if (kc11<=0.) {
    kc11=700.;
    s[0]='\0'; sprintf(s,"%.4lf",kc11);
    EditStKc11->Text=s; EditStKc11->SetFocus(); EditStKc11->SelectAll();
    OutputMessage("���� ��������� kc11 - <=0 -> ���������� kc11=700");
  }
  /*if (kc11==700.) {
    OutputMessage("��� ����� ������� ���������� ������ ������� - �������� ������ ��������");
    OutputMessage("      ���������� kc11=700 - ��������=������ ��������");
  }*/

  /*���� ��������� mc*/
  str=EditStMc->Text;
  mc=atof(str.c_str());
  if (mc<=0. || mc>1.) {
    mc=0.25;
    s[0]='\0'; sprintf(s,"%.4lf",mc);
    EditStMc->Text=s; EditStMc->SetFocus(); EditStMc->SelectAll();
    OutputMessage("���� ��������� kc11 - <=0 || >=1. -> ���������� mc=0.25");
  }
  /*if (mc==0.25) {
    OutputMessage("��� ����� ������� ���������� ������ ������� - �������� ������ ��������");
    OutputMessage("      ���������� mc=0.25 - ��������=������ ��������");
  }*/

  tkc=0.;
  if (thm<=0.) {
    OutputMessage("������ �������� �������->������� ������� ������� hm<=0");
    Application->MessageBox("������ �������� �������->������� ������� ������� \n hm=0. \n ������ �������.","��������������...",MB_OK);
    return ;
  }
  tkc=(1.-0.01*gamma0)*kc11/pow(thm,mc );
  s[0]='\0'; sprintf(s,"%.5lf",tkc); EditStKc->Text=s;
  tNmot=0.;
  tNmot=ae*ap*tvf*tkc/(6*10000000.*kpd);
  s[0]='\0'; sprintf(s,"%.5lf",tNmot); EditStP->Text=s;
  EditStP->SetFocus(); EditStP->SelectAll();
  EditStP->SelStart=0;
  EditStP->Font->Color=clGreen; // clRed; clGreen
  if (tNmot>=MACHINE[mach].Power) {
    EditStP->Font->Color=clRed; // clRed; clGreen
    OutputMessage("���������� �������� ������");
  }

  //������ ��������

  return ;
}


int TFormMain::CalcTechAlSz(TObject *Sender)
{
  char s[80];  String str;
  double D,z,B,t,K; //�������� ������
  double Ks; // ��������
  double tSz,tV0; // .....��������
  double k1,x1,y1,z1;
  double t1,t2,t3,t4,t5;

  /* �������� �� ������������ ����� �������� ��������
    StrToFloat(str) FloatToStr(vd) FloatToText(s,vd,fvExtended, ffFixed,8,8);*/
  /*�������*/
  str=BaseD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=30.;
    s[0]='\0'; sprintf(s,"%.3lf",D);
    BaseD->Text=s;  BaseD->SetFocus();   BaseD->SelectAll();
    str="������ ������� '�������� �����������' - D -> ����������....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
  }

  /*������ �������*/
  str=BaseB->Text;
  B=atof(str.c_str());
  if (B<=0.) {
    B=D/2.;
    s[0]='\0'; sprintf(s,"%.3lf",B);
    BaseB->Text=s;  BaseB->SetFocus();  BaseB->SelectAll();
    str="������ ������� '������ ����' - B -> ����������....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
  }
  // ��������
  if (D<B) OutputMessage("������� ����� ������ ��� ������ ������������ 'D<B' !");

  /*������� ����*/
  str=BaseT->Text;
  t=atof(str.c_str());
  if (t<=0.) {
    t=D/4;
    s[0]='\0'; sprintf(s,"%.3lf",t);
    BaseT->Text=s;  BaseT->SetFocus();  BaseT->SelectAll();
    str="������ ������� '������� ����' - t -> ����������....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
  }

  K=B/D;
  s[0]='\0'; sprintf(s,"%.2lf",K); EditK->Text=s;
  s[0]='\0'; sprintf(s,"%.2lf",t/D); EdittD->Text=s;

  // ������ ����������....
  //fz
  str=EditTkoef1->Text;
  k1=atof(str.c_str());
  if (k1<=0.) {
    k1=0.04; s[0]='\0'; sprintf(s,"%.3lf",k1);
    EditTkoef1->Text=s;  EditTkoef1->SetFocus();  EditTkoef1->SelectAll();
    OutputMessage("������ ������� k1<=0 (������ ������ �� ���)-> ����������....");
  }
  str=EditTX1->Text;
  x1=atof(str.c_str());
  if (x1<=0.) {
    x1=0.75; s[0]='\0'; sprintf(s,"%.3lf",x1);
    EditTX1->Text=s;  EditTX1->SetFocus();  EditTX1->SelectAll();
    OutputMessage("������ ������� x1<=0 (������ ������ �� ���)-> ����������....");
  }
  str=EditTY1->Text;
  y1=atof(str.c_str());
  if (y1<=0.) {
    y1=0.1; s[0]='\0'; sprintf(s,"%.3lf",y1);
    EditTY1->Text=s;  EditTY1->SetFocus();  EditTY1->SelectAll();
    OutputMessage("������ ������� y1<=0 (������ ������ �� ���)-> ����������....");
  }
  str=EditTZ1->Text;
  z1=atof(str.c_str());
  if (z1<=0.) {
    z1=0.22; s[0]='\0'; sprintf(s,"%.3lf",z1);
    EditTZ1->Text=s;  EditTZ1->SetFocus();  EditTZ1->SelectAll();
    OutputMessage("������ ������� z1<=0 (������ ������ �� ���)-> ����������....");
  }
  str=EditTKs->Text;
  Ks=atof(str.c_str());
  if (Ks<=0.) {
    Ks=1.; s[0]='\0'; sprintf(s,"%.3lf",Ks);
    EditTKs->Text=s;  EditTKs->SetFocus();  EditTKs->SelectAll();
    OutputMessage("������ ������� Ks<=0 (��������������� ������������)-> ����������....");
  }

  /**************************************************************************/
  /* ������ */
  /*Sz*/ t1=pow(D,x1);  t2=pow(t,y1);  t3=pow(B,z1); tSz=k1*t1*Ks/(t2*t3) ;
  if (tSz<=0.0001) {
    str="������ ����������-> Sz=fz=0.";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
    return (-1);
  }

    //��������� ��������
  s[0]='\0'; sprintf(s,"%.3lf",tSz); EditSz->Text=s;

  return(0) ;
}

int TFormMain::CalcTechAlV1(TObject *Sender)
{
  int i,j;
  char s[80];  String str;
  double D,B,t,K; //�������� ������
  double tSz,tV1,tV2; // .....��������
  double k2,x2,y2,z2;
  double t1,t2,t3,t4,t5;

  /* �������� �� ������������ ����� �������� ��������
    StrToFloat(str) FloatToStr(vd) FloatToText(s,vd,fvExtended, ffFixed,8,8);*/
  /*�������*/
  str=BaseD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=30.;
    s[0]='\0'; sprintf(s,"%.3lf",D);
    BaseD->Text=s;  BaseD->SetFocus();   BaseD->SelectAll();
    str="������ ������� '�������� �����������' - D -> ����������....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
  }

  /*������ �������*/
  str=BaseB->Text;
  B=atof(str.c_str());
  if (B<=0.) {
    B=D/2.;
    s[0]='\0'; sprintf(s,"%.3lf",B);
    BaseB->Text=s;  BaseB->SetFocus();  BaseB->SelectAll();
    str="������ ������� '������ ����' - B -> ����������....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
  }
  // ��������
  if (D<B) OutputMessage("������� ����� ������ ��� ������ ������������ 'D<B' !");

  /*������� ����*/
  str=BaseT->Text;
  t=atof(str.c_str());
  if (t<=0.) {
    t=D/4;
    s[0]='\0'; sprintf(s,"%.3lf",t);
    BaseT->Text=s;  BaseT->SetFocus();  BaseT->SelectAll();
    str="������ ������� '������� ����' - t -> ����������....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
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
    OutputMessage("������ ������� Sz<=0 (������ V1)....");
  }

  //V1
  str=EditTkoef2->Text;
  k2=atof(str.c_str());
  if (k2<=0.) {
    k2=1097.0; s[0]='\0'; sprintf(s,"%.2lf",k2);
    EditTkoef2->Text=s;  EditTkoef2->SetFocus();  EditTkoef2->SelectAll();
    OutputMessage("������ ������� k2<=0 (������ V1)-> ����������....");
  }
  str=EditTX2->Text;
  x2=atof(str.c_str());
  if (x2<=0.) {
    x2=0.86; s[0]='\0'; sprintf(s,"%.3lf",x2);
    EditTX2->Text=s;  EditTX2->SetFocus();  EditTX2->SelectAll();
    OutputMessage("������ ������� x2<=0 (������ V1)-> ����������....");
  }
  str=EditTY2->Text;
  y2=atof(str.c_str());
  if (y2<=0.) {
    y2=0.81; s[0]='\0'; sprintf(s,"%.3lf",y2);
    EditTY2->Text=s;  EditTY2->SetFocus();  EditTY2->SelectAll();
    OutputMessage("������ ������� y2<=0 (������ V1)-> ����������....");
  }
  str=EditTZ2->Text;
  z2=atof(str.c_str());
  if (z2<=0.) {
    z2=0.47; s[0]='\0'; sprintf(s,"%.3lf",z2);
    EditTZ2->Text=s;  EditTZ2->SetFocus();  EditTZ2->SelectAll();
    OutputMessage("������ ������� z2<=0 (������ V1)-> ����������....");
  }

  /**************************************************************************/
  K=B/D;
  /* ������ */
  /*Sz*/
  if (tSz<=0.0001) {
    str="������ ����������-> Sz=fz=0.";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
    return(-1);
  }
  /*V1*/ t1=pow(tSz,x2);t2=pow(t,y2);  t3=pow(K,z2); tV1=k2/(t1*t2*t3) ;

  //��������� ��������
  s[0]='\0'; sprintf(s,"%.3lf",tV1); EditV1->Text=s;  EditV->Items->Add(s);

  return(0) ;
}

int TFormMain::CalcTechAlV2(TObject *Sender)
{
  char s[80];  String str;
  double D,B,t,K; //�������� ������
  double kpd ; // ��� ������
  int mach; // ������
  double tSz,tV2; // .....��������
  double k3,x3,y3,z3,m1;
  double t1,t2,t3,t4,t5;

  mach=TabMach->TabIndex;

  /* �������� �� ������������ ����� �������� ��������
    StrToFloat(str) FloatToStr(vd) FloatToText(s,vd,fvExtended, ffFixed,8,8);*/
  /*�������*/
  str=BaseD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=30.;
    s[0]='\0'; sprintf(s,"%.3lf",D);
    BaseD->Text=s;  BaseD->SetFocus();   BaseD->SelectAll();
    str="������ ������� '�������� �����������' - D -> ����������....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
  }

  /*������ �������*/
  str=BaseB->Text;
  B=atof(str.c_str());
  if (B<=0.) {
    B=D/2.;
    s[0]='\0'; sprintf(s,"%.3lf",B);
    BaseB->Text=s;  BaseB->SetFocus();  BaseB->SelectAll();
    str="������ ������� '������ ����' - B -> ����������....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
  }
  // ��������
  if (D<B) OutputMessage("������� ����� ������ ��� ������ ������������ 'D<B' !");

  /*������� ����*/
  str=BaseT->Text;
  t=atof(str.c_str());
  if (t<=0.) {
    t=D/4;
    s[0]='\0'; sprintf(s,"%.3lf",t);
    BaseT->Text=s;  BaseT->SetFocus();  BaseT->SelectAll();
    str="������ ������� '������� ����' - t -> ����������....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
  }

  K=B/D;
  s[0]='\0'; sprintf(s,"%.2lf",K); EditK->Text=s;
  s[0]='\0'; sprintf(s,"%.2lf",t/D); EdittD->Text=s;

  // ���
  str=ComboBoxTkpd->Text; kpd=atof(str.c_str());

  //fz
  str=EditSz->Text;
  tSz=atof(str.c_str());
  if (tSz<=0.) {
    tSz=0.; s[0]='\0'; sprintf(s,"%.3lf",tSz);
    EditSz->Text=s;  EditSz->SetFocus();  EditSz->SelectAll();
    OutputMessage("������ ������� Sz<=0 (������ V1)....");
  }

  //V2
  str=EditTkoef3->Text;
  k3=atof(str.c_str());
  if (k3<=0.) {
    k3=49.0; s[0]='\0'; sprintf(s,"%.2lf",k3);
    EditTkoef3->Text=s;  EditTkoef3->SetFocus();  EditTkoef3->SelectAll();
    OutputMessage("������ ������� k3<=0 (������ V2)-> ����������....");
  }
  str=EditTX3->Text;
  x3=atof(str.c_str());
  if (x3<=0.) {
    x3=0.684; s[0]='\0'; sprintf(s,"%.3lf",x3);
    EditTX3->Text=s;  EditTX3->SetFocus();  EditTX3->SelectAll();
    OutputMessage("������ ������� x3<=0 (������ V2)-> ����������....");
  }
  str=EditTY3->Text;
  y3=atof(str.c_str());
  if (y3<=0.) {
    y3=0.548; s[0]='\0'; sprintf(s,"%.3lf",y3);
    EditTY3->Text=s;  EditTY3->SetFocus();  EditTY3->SelectAll();
    OutputMessage("������ ������� y3<=0 (������ V2)-> ����������....");
  }
  str=EditTZ3->Text;
  z3=atof(str.c_str());
  if (z3<=0.) {
    z3=0.286; s[0]='\0'; sprintf(s,"%.3lf",z3);
    EditTZ3->Text=s;  EditTZ3->SetFocus();  EditTZ3->SelectAll();
    OutputMessage("������ ������� z3<=0 (������ V2)-> ����������....");
  }
  str=EditTsm1->Text;
  m1=atof(str.c_str());
  if (m1<=0.) {
    m1=1.134; s[0]='\0'; sprintf(s,"%.3lf",m1);
    EditTsm1->Text=s;  EditTsm1->SetFocus();  EditTsm1->SelectAll();
    OutputMessage("������ ������� m1<=0 (������ V2)-> ����������....");
  }

  /**************************************************************************/
  K=B/D;
  /* ������ */
  /*Sz*/
  if (tSz<=0.0001) {
    str="������ ����������-> Sz=fz=0.";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
    return(-1);
  }

  /*V2*/t1=pow(t,x3);  t2=pow(tSz,y3);t3=pow(D,z3);
        t4=MACHINE[mach].Power*kpd*K*k3/(t1*t2*t3); tV2=pow(t4,m1);
  /**************************************************************************/

  //��������� ��������
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
       OutputMessage("������ �������������� V<=0 (������ V)....");
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
  double tF,tV0; // .....��������
  int iv,ifv;

  // ������ ����������....
  //V
  EditV->Update();
  str=EditV->Text;
  tV0=atof(str.c_str());
  if (tV0<=0.) {
    tV0=0.;
    //OutputMessage("������ ������� V<=0 (�������� �����������)....");
  }
  //F
  EditF->Update();
  str=EditF->Text;
  tF=atof(str.c_str());
  if (tF<=0.) {
    tF=20.0; s[0]='\0'; sprintf(s,"%.3lf",tF);  EditF->Text=s;
    //OutputMessage("������ ������� F<=0 (�������� �����������)....");
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
    OutputMessage("���������� �������������� ������: T>=140�C -> ������ ���������....... :)");
  }

  //bool CanSelect=True;
  //StringGridT->OnSelectCell(Sender, iv, ifv, CanSelect);

  return(0); ;
}

int TFormMain::CalcTechAln(TObject *Sender)
{
  char s[80];  String str;
  double D,t,K,B; //�������� ������
  int mach; // ������
  double tV0,tn; // .....��������

  mach=TabMach->TabIndex;

  /* �������� �� ������������ ����� �������� ��������
    StrToFloat(str) FloatToStr(vd) FloatToText(s,vd,fvExtended, ffFixed,8,8);*/
  /*�������*/
  str=BaseD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=30.;
    s[0]='\0'; sprintf(s,"%.3lf",D);
    BaseD->Text=s;  BaseD->SetFocus();   BaseD->SelectAll();
    str="������ ������� '�������� �����������' - D -> ����������....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
  }

  /*������ �������*/
  str=BaseB->Text;
  B=atof(str.c_str());
  if (B<=0.) {
    B=D/2.;
    s[0]='\0'; sprintf(s,"%.3lf",B);
    BaseB->Text=s;  BaseB->SetFocus();  BaseB->SelectAll();
    str="������ ������� '������ ����' - B -> ����������....";
    OutputMessage(str);
  }
  // ��������
  if (D<B) OutputMessage("������� ����� ������ ��� ������ ������������ 'D<B' !");

  /*������� ����*/
  str=BaseT->Text;
  t=atof(str.c_str());
  if (t<=0.) {
    t=D/4;
    s[0]='\0'; sprintf(s,"%.3lf",t);
    BaseT->Text=s;  BaseT->SetFocus();  BaseT->SelectAll();
    str="������ ������� '������� ����' - t -> ����������....";
    OutputMessage(str);
  }

  K=B/D;
  s[0]='\0'; sprintf(s,"%.2lf",K); EditK->Text=s;
  s[0]='\0'; sprintf(s,"%.2lf",t/D); EdittD->Text=s;

  // ������ ����������....
   /*V0*/
  str=EditV->Text;
  tV0=atof(str.c_str());
  if (tV0<=0.) {
    tV0=0.0;
    OutputMessage("������ ������� �������� ���� (�/���) V=0   .... (������ n)");
  }

  /**************************************************************************/
  /* ������ */
  /*n*/  tn=tV0*1000./(PI*D) ;
  Editn->Font->Color=clGreen; // clRed; clGreen
  if (tn>=MACHINE[mach].MAXSpindel) {
    Editn->Font->Color=clRed;
    OutputMessage("���������� ����� ��������   (������ n)");
  }

  /**************************************************************************/
  //��������� ��������
  s[0]='\0'; sprintf(s,"%.3lf",tn);  Editn->Text=s;
  return(0);
}

int TFormMain::CalcTechAlF(TObject *Sender)
{
  int vi=0 ; char s[80];  String str;
  double z; //�������� ������
  int mach; // ������
  double tSz,tn,tF; // .....��������

  mach=TabMach->TabIndex;

  /*����� ������*/
  str=BaseZ->Text;
  vi=atoi(str.c_str());
  if (vi<=0) {
    vi=4;
    s[0]='\0'; sprintf(s,"%d",vi);
    BaseZ->Text=s;  BaseZ->SetFocus();   BaseZ->SelectAll();
    str="������ ������� '����� ������' - z -> ����������....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
  }
  z=(double)vi;

  /*n*/
  str=Editn->Text;
  tn=atof(str.c_str());
  if (tn<=0.) {
    tn=0.; s[0]='\0'; sprintf(s,"%.3lf",tn);
    Editn->Text=s;  Editn->SetFocus();  Editn->SelectAll();
    OutputMessage("������ ������� n<=0 (������ F)....");
  }
  Editn->Font->Color=clGreen; // clRed; clGreen
  if (tn>=MACHINE[mach].MAXSpindel) {
    Editn->Font->Color=clRed;
    OutputMessage("���������� ����� ��������  (������ F)");
  }

  //fz
  str=EditSz->Text;
  tSz=atof(str.c_str());
  if (tSz<=0.) {
    tSz=0.; s[0]='\0'; sprintf(s,"%.3lf",tSz);
    EditSz->Text=s;  EditSz->SetFocus();  EditSz->SelectAll();
    OutputMessage("������ ������� Sz<=0 (������ F)....");
  }

  /**************************************************************************/
  /* ������ */
  /*Sz*/
  if (tSz<=0.0001) {
    str="������ ����������-> Sz=fz=0.";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
  }

  /*F*/  tF=tSz*tn*z ;
  EditF->Font->Color=clGreen; // clRed; clGreen
  if (tF>=MACHINE[mach].MAXFeed) {
    EditF->Font->Color=clRed;
    OutputMessage("���������� ������������ ������ �������  (������ F)");
  }
  /**************************************************************************/

  //��������� ��������
  s[0]='\0'; sprintf(s,"%.3lf",tF);  EditF->Text=s;

  return(0);
}

int TFormMain::CalcTechAlPz(TObject *Sender)
{
  int i;
  char s[80];  String str;
  double D,B,t,K; //�������� ������
  double kpd ; // ��� ������
  int mach; // ������
  double tSz,tn,tPz; // .....��������
  double k4,x4,y4,z4,m2;
  double t1,t2,t3,t4,t5;

  mach=TabMach->TabIndex;

  /* �������� �� ������������ ����� �������� ��������
    StrToFloat(str) FloatToStr(vd) FloatToText(s,vd,fvExtended, ffFixed,8,8);*/
  /*�������*/
  str=BaseD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=30.;
    s[0]='\0'; sprintf(s,"%.3lf",D);
    BaseD->Text=s;  BaseD->SetFocus();   BaseD->SelectAll();
    str="������ ������� '�������� �����������' - D -> ����������....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
  }

  /*������ �������*/
  str=BaseB->Text;
  B=atof(str.c_str());
  if (B<=0.) {
    B=D/2.;
    s[0]='\0'; sprintf(s,"%.3lf",B);
    BaseB->Text=s;  BaseB->SetFocus();  BaseB->SelectAll();
    str="������ ������� '������ ����' - B -> ����������....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
  }
  // ��������
  if (D<B) OutputMessage("������� ����� ������ ��� ������ ������������ 'D<B' !");

  /*������� ����*/
  str=BaseT->Text;
  t=atof(str.c_str());
  if (t<=0.) {
    t=D/4;
    s[0]='\0'; sprintf(s,"%.3lf",t);
    BaseT->Text=s;  BaseT->SetFocus();  BaseT->SelectAll();
    str="������ ������� '������� ����' - t -> ����������....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
  }

  K=B/D;
  s[0]='\0'; sprintf(s,"%.2lf",K); EditK->Text=s;
  s[0]='\0'; sprintf(s,"%.2lf",t/D); EdittD->Text=s;

  // ���
  str=ComboBoxTkpd->Text; kpd=atof(str.c_str());

  //P
  str=EditTkoef4->Text;
  k4=atof(str.c_str());
  if (k4<=0.) {
    k4=246; s[0]='\0'; sprintf(s,"%.2lf",k4);
    EditTkoef4->Text=s;  EditTkoef4->SetFocus();  EditTkoef4->SelectAll();
    OutputMessage("������ ������� k4<=0 (������ P)-> ����������....");
  }
  str=EditTX4->Text;
  x4=atof(str.c_str());
  if (x4<=0.) {
    x4=0.684; s[0]='\0'; sprintf(s,"%.3lf",x4);
    EditTX4->Text=s;  EditTX4->SetFocus();  EditTX4->SelectAll();
    OutputMessage("������ ������� x4<=0 (������ P)-> ����������....");
  }
  str=EditTY4->Text;
  y4=atof(str.c_str());
  if (y4<=0.) {
    y4=0.546; s[0]='\0'; sprintf(s,"%.3lf",y4);
    EditTY4->Text=s;  EditTY4->SetFocus();  EditTY4->SelectAll();
    OutputMessage("������ ������� y4<=0 (������ P)-> ����������....");
  }
  str=EditTZ4->Text;
  z4=atof(str.c_str());
  if (z4<=0.) {
    z4=0.168; s[0]='\0'; sprintf(s,"%.3lf",z4);
    EditTZ4->Text=s;  EditTZ4->SetFocus();  EditTZ4->SelectAll();
    OutputMessage("������ ������� z4<=0 (������ P)-> ����������....");
  }
  str=EditTsm2->Text;
  m2=atof(str.c_str());
  if (m2<=0.) {
    m2=0.118; s[0]='\0'; sprintf(s,"%.3lf",m2);
    EditTsm2->Text=s;  EditTsm2->SetFocus();  EditTsm2->SelectAll();
    OutputMessage("������ ������� m2<=0 (������ P)-> ����������....");
  }

  //fz
  str=EditSz->Text;
  tSz=atof(str.c_str());
  if (tSz<=0.) {
    tSz=0.; s[0]='\0'; sprintf(s,"%.3lf",tSz);
    EditSz->Text=s;  EditSz->SetFocus();  EditSz->SelectAll();
    OutputMessage("������ ������� Sz<=0 (������ Pz)....");
  }

  /*n*/
  str=Editn->Text;
  tn=atof(str.c_str());
  if (tn<=0.) {
    tn=0.; s[0]='\0'; sprintf(s,"%.3lf",tn);
    Editn->Text=s;  Editn->SetFocus();  Editn->SelectAll();
    str="������ ������� n<=0 (������ Pz)....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
    return (-1);
  }
  Editn->Font->Color=clGreen; // clRed; clGreen
  if (tn>=MACHINE[mach].MAXSpindel) {
    Editn->Font->Color=clRed;
    OutputMessage("���������� ����� �������� (������ Pz)");
  }

  /**************************************************************************/
  /* ������ */
  /*Sz*/
  if (tSz<=0.0001) {
    str="������ ����������-> Sz=fz=0.";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������..(������ Pz).",MB_OK);
    return (-1);
  }

  /*Pz*/ t1=pow(t,x4);  t2=pow(tSz,y4);  t3=pow(D,z4);  t4=pow(tn,m2); tPz=k4*t1*t2*t3/t4;

  EditPz->Font->Color=clGreen; // clRed; clGreen

  // Forest
  if (mach==0) {

    t5=tPz/1000; // ������� � ��
    for (i=1;i<8;i++) { StringGridP->Cells[0][i]=""; StringGridP->Cells[1][i]=""; }

    i=1;
    if (tn<=6000) {
      s[0]='\0';sprintf(s,"n=%.0lf",tn); StringGridP->Cells[0][i]=s;
      s[0]='\0';sprintf(s,"P=%.3lf � %.2lf=%.3lf",t5,kpd,t5*kpd); StringGridP->Cells[1][i]=s;
      i++;
      if (t5>(2.00*kpd)) {
        EditPz->Font->Color=clRed;
        OutputMessage("���������� ���������� ������� �������� �� �������� (������ Pz)....");
       }
    }
    StringGridP->Cells[0][i]="6000";
    s[0]='\0'; sprintf(s,"2.00 � %.2lf  =%.2lf",kpd,2.00*kpd);  StringGridP->Cells[1][i]=s;

    i++;
    if ((tn>6000) && (tn<=8000)) {
      s[0]='\0';sprintf(s,"n=%.0lf",tn); StringGridP->Cells[0][i]=s;
      s[0]='\0';sprintf(s,"P=%.3lf � %.2lf=%.3lf",t5,kpd,t5*kpd); StringGridP->Cells[1][i]=s;
      i++;
      if (t5>(1.70*kpd)) {
        EditPz->Font->Color=clRed;
        OutputMessage("���������� ���������� ������� �������� �� �������� (������ Pz)....");
       }
    }
    StringGridP->Cells[0][i]="8000";
    s[0]='\0'; sprintf(s,"1.70 � %.2lf  =%.2lf",kpd,1.70*kpd);   StringGridP->Cells[1][i]=s;

    i++;
    if ((tn>8000) && (tn<=10000)) {
      s[0]='\0';sprintf(s,"n=%.0lf",tn); StringGridP->Cells[0][i]=s;
      s[0]='\0';sprintf(s,"P=%.3lf � %.2lf=%.3lf",t5,kpd,t5*kpd); StringGridP->Cells[1][i]=s;
      i++;
      if (t5>(1.50*kpd)) {
        EditPz->Font->Color=clRed;
        OutputMessage("���������� ���������� ������� �������� �� �������� (������ Pz)....");
       }
    }
    StringGridP->Cells[0][i]="10000";
    s[0]='\0'; sprintf(s,"1.50 � %.2lf  =%.2lf",kpd,1.50*kpd);   StringGridP->Cells[1][i]=s;

    i++;
    if ((tn>10000) && (tn<=12000)) {
      s[0]='\0';sprintf(s,"n=%.0lf",tn); StringGridP->Cells[0][i]=s;
      s[0]='\0';sprintf(s,"P=%.3lf � %.2lf=%.3lf",t5,kpd,t5*kpd); StringGridP->Cells[1][i]=s;
      i++;
      if (t5>(1.35*kpd)) {
        EditPz->Font->Color=clRed;
        OutputMessage("���������� ���������� ������� �������� �� �������� (������ Pz)....");
       }
    }
    StringGridP->Cells[0][i]="12000";
    s[0]='\0'; sprintf(s,"1.35 � %.2lf  =%.2lf",kpd,1.35*kpd);  StringGridP->Cells[1][i]=s;

    i++;
    if ((tn>12000) && (tn<=15000)) {
      s[0]='\0';sprintf(s,"n=%.0lf",tn); StringGridP->Cells[0][i]=s;
      s[0]='\0';sprintf(s,"P=%.3lf � %.2lf=%.3lf",t5,kpd,t5*kpd); StringGridP->Cells[1][i]=s;
      i++;
      if (t5>(1.25*kpd)) {
        EditPz->Font->Color=clRed;
        OutputMessage("���������� ���������� ������� �������� �� �������� (������ Pz)....");
       }
    }
    StringGridP->Cells[0][i]="15000";
    s[0]='\0'; sprintf(s,"1.25 � %.2lf  =%.2lf",kpd,1.25*kpd);  StringGridP->Cells[1][i]=s;

    i++;
    if ((tn>15000) && (tn<=18000)) {
      s[0]='\0';sprintf(s,"n=%.0lf",tn); StringGridP->Cells[0][i]=s;
      s[0]='\0';sprintf(s,"P=%.3lf � %.2lf=%.3lf",t5,kpd,t5*kpd); StringGridP->Cells[1][i]=s;
      i++;
      if (t5>(1.20*kpd)) {
        EditPz->Font->Color=clRed;
        OutputMessage("���������� ���������� ������� �������� �� �������� (������ Pz)....");
       }
    }
    StringGridP->Cells[0][i]="18000";
    s[0]='\0'; sprintf(s,"1.20 � %.2lf  =%.2lf",kpd,1.20*kpd);  StringGridP->Cells[1][i]=s;

    i++;
    if ((tn>18000)) {
      s[0]='\0';sprintf(s,"n=%.0lf",tn); StringGridP->Cells[0][i]=s;
      s[0]='\0';sprintf(s,"P=%.3lf � %.2lf=%.3lf",t5,kpd,t5*kpd); StringGridP->Cells[1][i]=s;
      i++;
      if (t5>(1.20*kpd)) {
        EditPz->Font->Color=clRed;
        OutputMessage("���������� ���������� ������� �������� �� �������� (������ Pz)....");
       }
    }

    PanelForestP->Visible=True; //False
  }

  /**************************************************************************/
  //��������� ��������
  s[0]='\0'; sprintf(s,"%.2lf",tPz); EditPz->Text=s;

  return(0);
}

int TFormMain::CalcTechAlMz(TObject *Sender)
{
  char s[80];  String str;
  double D,t,K,B; //�������� ������
  int mach; // ������
  double tMz,tPz; // .....��������

  mach=TabMach->TabIndex;

  /* �������� �� ������������ ����� �������� ��������
    StrToFloat(str) FloatToStr(vd) FloatToText(s,vd,fvExtended, ffFixed,8,8);*/
  /*�������*/
  str=BaseD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=30.;
    s[0]='\0'; sprintf(s,"%.3lf",D);
    BaseD->Text=s;  BaseD->SetFocus();   BaseD->SelectAll();
    str="������ ������� '�������� �����������' - D -> ����������....";
    OutputMessage(str);
    Application->MessageBox(str.c_str(),"��������������...",MB_OK);
  }

  /*������ �������*/
  str=BaseB->Text;
  B=atof(str.c_str());
  if (B<=0.) {
    B=D/2.;
    s[0]='\0'; sprintf(s,"%.3lf",B);
    BaseB->Text=s;  BaseB->SetFocus();  BaseB->SelectAll();
    str="������ ������� '������ ����' - B -> ����������....";
    OutputMessage(str);
  }
  // ��������
  if (D<B) OutputMessage("������� ����� ������ ��� ������ ������������ 'D<B' !");

  /*������� ����*/
  str=BaseT->Text;
  t=atof(str.c_str());
  if (t<=0.) {
    t=D/4;
    s[0]='\0'; sprintf(s,"%.3lf",t);
    BaseT->Text=s;  BaseT->SetFocus();  BaseT->SelectAll();
    str="������ ������� '������� ����' - t -> ����������....";
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
    str="������ ������� '���� ����' - Pz -> ����������....(������ Mz)";
    OutputMessage(str);
  }

  /**************************************************************************/
  /* ������ */
  /*Mz*/ tMz=tPz*D/(2.*100.);
  EditMz->Font->Color=clGreen; // clRed; clGreen
  if (tMz>=MACHINE[mach].Torque) {
    EditMz->Font->Color=clRed;
    OutputMessage("���������� ������������� ������� ������� (������ Mz)");
  }

  /**************************************************************************/
  //��������� ��������
  s[0]='\0'; sprintf(s,"%.2lf",tMz); EditMz->Text=s;

  return(0);
}

int TFormMain::CalcTechAlNct(TObject *Sender)
{
  char s[80];  String str;
  double D,t,K,B; //�������� ������
  int mach; // ������
  double tNct,tPz,tV0; // .....��������

  mach=TabMach->TabIndex;

  /*V0*/
  str=EditV->Text;
  tV0=atof(str.c_str());
  if (tV0<=0.) {
    str="������ ������� '�������� ����' - V (������ ��������) ....";
    OutputMessage(str);
  }

  /*tPz*/
  str=EditPz->Text;
  tPz=atof(str.c_str());
  if (tPz<=0.) {
    tPz=1;
    s[0]='\0'; sprintf(s,"%.3lf",tPz);
    EditPz->Text=s;  EditPz->SetFocus();  EditPz->SelectAll();
    str="������ ������� '���� ����' - Pz (������ ��������) -> ����������....";
    OutputMessage(str);
  }

  /**************************************************************************/
  /* ������ */
  /*Nct*/ tNct=tPz*tV0/(1020.*60.);
  EditNp->Font->Color=clGreen; // clRed; clGreen
  if (tNct>=MACHINE[mach].Power) {
    EditNp->Font->Color=clRed;
    OutputMessage("���������� ����� ��������");
  }

  /**************************************************************************/
  //��������� ��������
  s[0]='\0'; sprintf(s,"%.2lf",tNct);EditNp->Text=s;
  return(0);
}

void TFormMain::CalcTechAl(TObject *Sender)
{
  int i;
  /**************************************************************************/
  /* ������ */
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
 OutputMessage("Nothing");
 return ;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::FormCreate(TObject *Sender)
{
   int i,j;

  // V-STAR (FOREST-LINE)
  MACHINE[0].MAXSpindel=18000.;
  MACHINE[0].MAXFeed=60000.;
  MACHINE[0].Power=40.;
  MACHINE[0].Torque=54.;
  MACHINE[0].machine_name[0]='\0';  strcpy(MACHINE[0].machine_name,"V-STAR (FOREST-LINE)\0");
  // ����� ������� (S,Pw)
  //MACHINE[0].SPw[15][15]  ;
  // ����� ������� (S,Mz)
  //MACHINE[0].SMz[15][15]  ;

  // ��-5��
  MACHINE[1].MAXSpindel=18000.;
  MACHINE[1].MAXFeed=12000.;
  MACHINE[1].Power=25.; // 25
  MACHINE[1].Torque=30.; //  30
  MACHINE[1].machine_name[0]='\0';  strcpy(MACHINE[1].machine_name,"��-5��\0");

  //
  //.....
  //

  TStringList *ListMachine;
  ListMachine =  new TStringList();
  try
  {
    for (i=0;i<NUM_MACHINE;i++) {
      ListMachine->Add(MACHINE[i].machine_name);
    }
    TabMach->Tabs->Assign(ListMachine);
    delete ListMachine;
  }
  __finally //(...)
  {
   	 ;
  }

  TabMach->TabIndex=0;
  TabMachChange(Sender);
  PageControlMethods->ActivePageIndex=0;
  PageControlMethodsChange(Sender);

  BaseD->Text="30.0";
  BaseZ->Text="2";
  BaseB->Text="10.0";
  BaseT->Text="2.0";
  BaseVc->Text="560.0";
  Basefz->Text="0.450";

  EditStn->Text="0.0";
  EditStVc->Text="0.0";
  EditStP->Text="0.0";
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

 for (i=0;i<4;i++) for (j=0;j<9;j++) StringGridTab->Cells[i][j]="";

 RadioButton1->Checked=False;
 RadioButton2->Checked=False;
 RadioButton3->Checked=False;
 RadioButton4->Checked=True; // True
 RadioButton5->Checked=False;

 StringGridTab->Cells[0][0]="��������";
 StringGridTab->Cells[1][0]="Rm (H/(mm*mm)" ;
 StringGridTab->Cells[2][0]="kc1.1 (H/(mm*mm)" ;
 StringGridTab->Cells[3][0]="Mc";

 RadioButton4Click(Sender);

 StaticTextStMaterial->Caption=RadioButton4->Caption;

  try
  {
    //  0=�95���2  1=1163�2   2=�16��
    ComboBoxMaterial->Items->Clear();
    ComboBoxMaterial->Items->Append("�95���2");
    ComboBoxMaterial->Items->Append("1163�2");
    ComboBoxMaterial->Items->Append("�16��");
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

  StringGridP->Cells[0][0]=" n (��/���)";
  StringGridP->Cells[1][0]=" P (��)";

  StringGridP->Cells[0][1]="...."; // ����
  StringGridP->Cells[0][2]="6000";
  StringGridP->Cells[0][3]="8000";
  StringGridP->Cells[0][4]="10000";
  StringGridP->Cells[0][5]="12000";
  StringGridP->Cells[0][6]="15000";
  StringGridP->Cells[0][7]="18000";

  AnsiString str=ComboBoxTkpd->Text;
  double kpd=atof(str.c_str());
  char s[20];
  StringGridP->Cells[1][1]="...."; // ����
  s[0]='\0'; sprintf(s,"2.00 � %.2lf  =%.2lf",kpd,2.00*kpd);
  StringGridP->Cells[1][2]=s;
  s[0]='\0'; sprintf(s,"1.70 � %.2lf  =%.2lf",kpd,1.70*kpd);
  StringGridP->Cells[1][3]=s;
  s[0]='\0'; sprintf(s,"1.50 � %.2lf  =%.2lf",kpd,1.50*kpd);
  StringGridP->Cells[1][4]=s;
  s[0]='\0'; sprintf(s,"1.35 � %.2lf  =%.2lf",kpd,1.35*kpd);
  StringGridP->Cells[1][5]=s;
  s[0]='\0'; sprintf(s,"1.25 � %.2lf  =%.2lf",kpd,1.25*kpd);
  StringGridP->Cells[1][6]=s;
  s[0]='\0'; sprintf(s,"1.20 � %.2lf  =%.2lf",kpd,1.20*kpd);
  StringGridP->Cells[1][7]=s;

  PanelForestP->BeginDrag(True,0);
  PanelForestP->EndDrag(True);
  PanelForestP->Visible=False; //False

  return ;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::BitCalcsClick(TObject *Sender)
{
  int i; i=PageControlMethods->ActivePage->TabIndex;
  switch (i)
  {
   case 0 :  CalcStandart(Sender);  break ;
   case 1 :  CalcTechAl(Sender);    break ;
   case 2 :  CalcTechTi(Sender);    break ;
   default : 
     ShowMessage("Nothing");
   break ;
  }

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TabMachChange(TObject *Sender)
{
   int i; i=TabMach->TabIndex;
   char s[250]; s[0]='\0';

   s[0]='\0';
   sprintf(s,"n=%.0lf (��/���)  F=%.0lf (��/���)  Nct=%.0lf (���) Mz=%.0lf (Hm)", \
     MACHINE[i].MAXSpindel,MACHINE[i].MAXFeed,MACHINE[i].Power,MACHINE[i].Torque );
   StatusBarMes->SimpleText=s;
   s[0]='\0'; sprintf(s,"������ ������� �������: %s",MACHINE[i].machine_name);
   FormMain->Caption = s;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PageControlMethodsChange(TObject *Sender)
{
  int i; i=PageControlMethods->ActivePage->TabIndex;
  switch (i)
  {
   case 0 :
    LabelB->Caption="������ �������, ap (��) =";
    LabelT->Caption="������� �������, ae (��) =";
    BaseVc->Visible=True;  LabelVc->Visible=True; //Enabled
    Basefz->Visible=True;  Labelfz->Visible=True;
   break ;
   default :
    LabelB->Caption="������ �������, B (��) =";
    LabelT->Caption="������� �������, t (��) =";
    BaseVc->Visible=False; LabelVc->Visible=False; //Enabled
    Basefz->Visible=False; Labelfz->Visible=False;
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
 //Application->MessageBox("������� �.�. - 2004\n ��������� ������� �� ����������� '��������������� ������������ - ����� ������� ������� ��� ����������� � ��������� �������'","�� �������..",MB_OK);
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
  //OutputMessage("�� �������..\n\n �����: ������� �.�. - 2004\n ��������� ������� �� ����������� '��������������� ������������ - ����� ������� ������� ��� ����������� � ��������� �������'");
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

  ComboBoxMaterial->ItemIndex=0;
  ComboBoxRa->ItemIndex=0;
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
   OutputMessage("����������..");
   OutputMessage("    ");
   OutputMessage("1. �� 1.4.2104-2002 ");
   OutputMessage("    ��������������� ������������.");
   OutputMessage("    ����� ���������� �������� ���� � ������� ������� �����������������");
   OutputMessage("    ������������ ����������� � ��������� �������.");
   OutputMessage("2. ����� ������� - WALTER, �1,�2");
   OutputMessage("3. ���������� ���������-���������������, �1,�2");
   OutputMessage("     /���. ���. ���. �.�. ��������� � �.�. ���������� - �. ��������������,1985 - 496�.");
   OutputMessage("     ");
   OutputMessage(".......     ");
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::NMesWinClick(TObject *Sender)
{
   NMesWin->Checked=!(NMesWin->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N11Click(TObject *Sender)
{
   FormGraphic->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnSt1Click(TObject *Sender)
{
  char s[80]; String str;
  int mach; // ������
  double D; //�������� ������
  double tn,tvc; // �������������� ��������� : ..., ��������

  mach=TabMach->TabIndex;

 /* �������� �� ������������ ����� �������� ��������*/
  /*�������*/
  str=BaseD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=0.;
    s[0]='\0'; sprintf(s,"%.3lf",D); BaseD->Text=s; BaseD->SetFocus(); BaseD->SelectAll();
    OutputMessage("������ ������� '�������� �����������' - D");
    Application->MessageBox("������ ������� '�������� �����������' - D","��������������...",MB_OK);
    return ;
  }

  /*������� */
  str=EditStn->Text;
  tn=atof(str.c_str());
  if (tn<=0.) {
    tn=0;
    s[0]='\0'; sprintf(s,"%.3lf",tn); EditStn->Text=s; EditStn->SetFocus(); EditStn->SelectAll();
    OutputMessage("������ ������� �������� ��������...<=0");
  }

  // ������ �������� �������
  tvc=(D*PI*tn)/1000.;
  s[0]='\0'; sprintf(s,"%.3lf",tvc);  EditStVc->Text=s;
  EditStVc->SetFocus(); EditStVc->SelectAll();

  EditStn->Font->Color=clGreen; // clRed; clGreen
  if (tn>=MACHINE[mach].MAXSpindel) {
    EditStn->Font->Color=clRed;
    OutputMessage("���������� ����� ��������");
  }

  return ;

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnSt2Click(TObject *Sender)
{
  char s[80]; String str;
  int mach; // ������
  double ap,ae; //�������� ������
  double tvf,Q; // �������������� ��������� : ..., ��������

  /*������ �������*/
  str=BaseB->Text;
  ap=atof(str.c_str());
  if (ap<=0.) {
    ap=0.;
    s[0]='\0'; sprintf(s,"%.3lf",ap); BaseB->Text=s; BaseB->SetFocus(); BaseB->SelectAll();
    OutputMessage("������ ������� ������ ����...<=0");
  }

  /*������� ����*/
  str=BaseT->Text;
  ae=atof(str.c_str());
  if (ae<=0.) {
    ae=0;
    s[0]='\0'; sprintf(s,"%.3lf",ae); BaseT->Text=s; BaseT->SetFocus(); BaseT->SelectAll();
    OutputMessage("������ ������� ������� ����...<=0");
  }

  /*������ �������*/
  str=EditStVf->Text;
  tvf=atof(str.c_str());
  if (tvf<=0.) {
    tvf=0;
    s[0]='\0'; sprintf(s,"%.3lf",tvf); EditStVf->Text=s; EditStVf->SetFocus(); EditStVf->SelectAll();
    OutputMessage("������ ������� ������ ����...<=0");
  }

  // �������� ����� ���������
  Q=(ae*ap*tvf)/1000.;
  s[0]='\0'; sprintf(s,"%.5lf",Q); EditStQ->Text=s;

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnSt3Click(TObject *Sender)
{
  double vd; int vi=0 ; char s[80];  String str;
  int mach; // ������
  double D,z,ap,ae,fz; //�������� ������
  double ak,fsi ; // �������
  double thm; // �������������� ��������� : ..., ��������

  /*�������*/
  str=BaseD->Text;
  D=atof(str.c_str());
  if (D<=0.) {
    D=0.;
    s[0]='\0'; sprintf(s,"%.3lf",D); BaseD->Text=s; BaseD->SetFocus(); BaseD->SelectAll();
    Application->MessageBox("������ ������� '�������� �����������' - D","��������������...",MB_OK);
    return;
  }

  /*������� ����*/
  str=BaseT->Text;
  ae=atof(str.c_str());
  if (ae<=0.) {
    ae=0;
    s[0]='\0'; sprintf(s,"%.3lf",ae); BaseT->Text=s; BaseT->SetFocus(); BaseT->SelectAll();
    Application->MessageBox("������ ������� '������� ����' - ae","��������������...",MB_OK);
    return;
  }

  /*������ �� ���*/
  str=EditStfz->Text;
  fz=atof(str.c_str());
  if (fz<=0.) {
    fz=0;
    s[0]='\0'; sprintf(s,"%.5lf",fz); EditStfz->Text=s; EditStfz->SetFocus(); EditStfz->SelectAll();
    Application->MessageBox("������ ������� '������ �� ���' - fz","��������������...",MB_OK);
  }

  // ������� ������� �������
  if (D<=0.) { Application->MessageBox("������� ������� ������� \n D=0. \n ������ �������.","��������������...",MB_OK); return ; }
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
     /*���� k*/
     str=EditStk->Text;
     ak=atof(str.c_str());
     if (ak<=0. || ak>=180.) {
       ak=90.;
       s[0]='\0'; sprintf(s,"%.5lf",ak); EditStk->Text=s; EditStk->SetFocus(); EditStk->SelectAll();
     }
     thm=114.7*fz*sin(ak*PI/180.)*(ae/D);
     thm=fabs(thm-floor(thm)); //thm=fabs(thm-ceil(thm));
     if (fsi<=0.) fsi=0.001;
     thm/=fsi;
  }
  s[0]='\0'; sprintf(s,"%.8lf",thm); EditSthm2->Text=s;
  EditSthm2->SetFocus(); EditSthm2->SelectAll();
  EditSthm2->SelStart=0;
  //������ ��������

  return ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnSt4Click(TObject *Sender)
{
  int vi=0 ; char s[80]; String str;
  int mach; // ������
  double z,fz,vf; //�������� ������
  double tn,tvf,tfz; // �������������� ��������� : ..., ��������

  mach=TabMach->TabIndex;

  /*������ �� ���*/
  str=EditStVf->Text;
  vf=atof(str.c_str());
  if (vf<=0.) {
    vf=0;
    s[0]='\0'; sprintf(s,"%.3lf",vf); EditStVf->Text=s; EditStVf->SetFocus(); EditStVf->SelectAll();
  }

  /*������ �� ���*/
  str=Basefz->Text;
  fz=atof(str.c_str());
  if (fz<=0.) {
    fz=0;
    s[0]='\0'; sprintf(s,"%.5lf",fz);  Basefz->Text=s; Basefz->SetFocus(); Basefz->SelectAll();
  }

  /*����� ������*/
  str=BaseZ->Text;
  vi=atoi(str.c_str());
  if (vi<=0) {
    vi=0;
    s[0]='\0'; sprintf(s,"%d",vi); BaseZ->Text=s; BaseZ->SetFocus(); BaseZ->SelectAll();
    Application->MessageBox("������ ������� '����� ������' - z","��������������...",MB_OK);
    return ;
  }
  z=(double)vi;

  /*����� ��������*/
  str=EditStn->Text;
  tn=atof(str.c_str());
  if (tn<=0.) {
    tn=0.;
    s[0]='\0'; sprintf(s,"%.3lf",tn); EditStn->Text=s; EditStn->SetFocus(); EditStn->SelectAll();
  }

  // ������ ������ �� ���
  if (tn<=0.) { Application->MessageBox("������ ������ �� ��� \n n=0. \n ������ �������.","��������������...",MB_OK); return ; }
  if (z<=0.) { Application->MessageBox("������ ������ �� ��� \n z=0. \n ������ �������.","��������������...",MB_OK); return ; }
  tfz=vf/(z*tn);
  s[0]='\0'; sprintf(s,"%.5lf",tfz);  EditStfz->Text=s;
  EditStfz->SetFocus(); EditStfz->SelectAll();

  EditStVf->Font->Color=clGreen; // clRed; clGreen
  if (vf>=MACHINE[mach].MAXFeed) {
    EditStVf->Font->Color=clRed; // clRed; clGreen
    OutputMessage("���������� ������������� ������ ������� ������");
  }

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnSt5Click(TObject *Sender)
{
  char s[80]; String str;
  int mach; // ������
  double ap,ae; //�������� ������
  double ak,fsi ; // �������
  double kpd,gamma0, kc11, mc ; // ��������
  double tvf,thm,tkc,tNmot; // �������������� ��������� : ..., ��������

  mach=TabMach->TabIndex;

  /*������ �������*/
  str=BaseB->Text;
  ap=atof(str.c_str());
  if (ap<=0.) {
    ap=0.;
    s[0]='\0'; sprintf(s,"%.5lf",ap); BaseB->Text=s; BaseB->SetFocus(); BaseB->SelectAll();
    Application->MessageBox("������ ������� '������ ����' - ap","��������������...",MB_OK);
    return ;
  }

  /*������� ����*/
  str=BaseT->Text;
  ae=atof(str.c_str());
  if (ae<=0.) {
    ae=0;
    s[0]='\0'; sprintf(s,"%.5lf",ae); BaseT->Text=s; BaseT->SetFocus(); BaseT->SelectAll();
    OutputMessage("������ ������� '������� ����' - ae<=0.");
    return ;
  }

  /*������� ������� �������*/
  str=EditSthm2->Text;
  thm=atof(str.c_str());
  if (thm<=0.) {
    thm=0;
    s[0]='\0'; sprintf(s,"%.8lf",thm); EditSthm2->Text=s; EditSthm2->SetFocus(); EditSthm2->SelectAll();
    OutputMessage("������� ������� ������� - hm<=0.");
  }

  /*������ �������*/
  str=EditStVf->Text;
  tvf=atof(str.c_str());
  if (tvf<=0.) {
    tvf=0;
    s[0]='\0'; sprintf(s,"%.5lf",tvf);  EditStVf->Text=s; EditStVf->SetFocus(); EditStVf->SelectAll();
    OutputMessage("������ ������� - vf<=0.");
  }

  // ������ �������� �������
  str=ComboBoxStkpd->Text; kpd=atof(str.c_str());
  /*������� �������� ����*/
  str=EditStGamma0->Text;
  gamma0=atof(str.c_str());
  if (gamma0<=0. || gamma0>=100.) {
    gamma0=0.;
    s[0]='\0'; sprintf(s,"%.2lf",gamma0);
    EditStGamma0->Text=s; EditStGamma0->SetFocus(); EditStGamma0->SelectAll();
  }
  /*���� ��������� kc11*/
  str=EditStKc11->Text;
  kc11=atof(str.c_str());
  if (kc11<=0.) {
    kc11=700.;
    s[0]='\0'; sprintf(s,"%.3lf",kc11);
    EditStKc11->Text=s; EditStKc11->SetFocus(); EditStKc11->SelectAll();
    OutputMessage("���� ��������� kc11-> (kc11<=0) - ���������� kc11=700. ");
  }
  /*���� ��������� mc*/
  str=EditStMc->Text;
  mc=atof(str.c_str());
  if (mc<=0. || mc>1.) {
    mc=0.25;
    s[0]='\0'; sprintf(s,"%.3lf",mc);
    EditStMc->Text=s; EditStMc->SetFocus(); EditStMc->SelectAll();
    OutputMessage("���� ��������� mc-> (mc<=0 ��� mc>1) - ���������� mc=0.25 ");
  }
  tkc=0.;
  if (thm<=0.) {
    OutputMessage("������ �������� �������->������� ������� �������\n hm<=0. \n ������ �������.");
    return ;
  }
  tkc=(1.-0.01*gamma0)*kc11/pow(thm,mc );
  s[0]='\0'; sprintf(s,"%.5lf",tkc); EditStKc->Text=s;

  tNmot=0.;
  tNmot=ae*ap*tvf*tkc/(6*10000000.*kpd);

  s[0]='\0'; sprintf(s,"%.5lf",tNmot); EditStP->Text=s;
  EditStP->SetFocus(); EditStP->SelectAll();
  EditStP->SelStart=0;
  EditStP->Font->Color=clGreen; // clRed; clGreen
  if (tNmot>=MACHINE[mach].Power) {
    EditStP->Font->Color=clRed; // clRed; clGreen
    OutputMessage("���������� �������� ������");
  }

  //������ ��������

  return ;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Image9Click(TObject *Sender)
{
  /*�������� �������*/
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
  i=ComboBoxMaterial->ItemIndex; //  0-"�95���2"  1-"1163�2"  2-"�16��"
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

   // ����� P
 StringGridTab->Cells[0][1]="����������������� ������ �����; ��������� ����� � ������ ���������� �� ����������";
 StringGridTab->Cells[1][1]="<450" ;
 StringGridTab->Cells[2][1]="1350" ;
 StringGridTab->Cells[3][1]="0.21";

 StringGridTab->Cells[0][2]="����������������� ���������� �����";
 StringGridTab->Cells[1][2]="400<700" ;
 StringGridTab->Cells[2][2]="1500" ;
 StringGridTab->Cells[3][2]="0.22";

 StringGridTab->Cells[0][3]="���������� ��������������� �����, ���������� �������� �� ������� �� �������� (<0.5%C)";
 StringGridTab->Cells[1][3]="450<550" ;
 StringGridTab->Cells[2][3]="1500" ;
 StringGridTab->Cells[3][3]="0.25";

 StringGridTab->Cells[0][4]="����������, ������������������ ����� � �������� �������; ���������� �����; ������������ ����� (>0.5%�);��������� � ������������ ����������� �����";
 StringGridTab->Cells[1][4]="550<700" ;
 StringGridTab->Cells[2][4]="1700" ;
 StringGridTab->Cells[3][4]="0.24";

 StringGridTab->Cells[0][5]="���������� ���������������� �����; ���������� �������� �����;������������, ����������� �����";
 StringGridTab->Cells[1][5]="700<900" ;
 StringGridTab->Cells[2][5]="1900" ;
 StringGridTab->Cells[3][5]="0.24";

 StringGridTab->Cells[0][6]="���������������� ����� � �������� ���������� �������; �������, ������������������ ����� � �������� �������; ������������, ����������� �����";
 StringGridTab->Cells[1][6]="900<1200" ;
 StringGridTab->Cells[2][6]="2000" ;
 StringGridTab->Cells[3][6]="0.24";

 StringGridTab->Cells[0][7]="������������� ����� � �������� ���������� �������; ���������� ����� ����� 3-6; ������������, ����������� �����";
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

   // ����������� ����� M
 StringGridTab->Cells[0][1]="����������� ����� � ����� �������� ���������� �������";
 StringGridTab->Cells[1][1]="" ;
 StringGridTab->Cells[2][1]="1750" ;
 StringGridTab->Cells[3][1]="0.22";

 StringGridTab->Cells[0][2]="������������ ����������� �����; ����������� � ����������, � �������� ���������� �������";
 StringGridTab->Cells[1][2]="" ;
 StringGridTab->Cells[2][2]="1900" ;
 StringGridTab->Cells[3][2]="0.20";

 StringGridTab->Cells[0][3]="����������� � ����������, � �������� ���������� �������";
 StringGridTab->Cells[1][3]="" ;
 StringGridTab->Cells[2][3]="2050" ;
 StringGridTab->Cells[3][3]="0.20";

 StringGridTab->Cells[0][4]="����������� � ����������, � ������������� �������� ���������� �������";
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

   // �������� ����� K
 StringGridTab->Cells[0][1]="�������� ����� ������� ���������, ����� �������� �����";
 StringGridTab->Cells[1][1]="" ;
 StringGridTab->Cells[2][1]="1150" ;
 StringGridTab->Cells[3][1]="0.22";

 StringGridTab->Cells[0][2]="����������������� �������� �����, ������ �������� �����, �������� ����� � ���������� ��������";
 StringGridTab->Cells[1][2]="" ;
 StringGridTab->Cells[2][2]="1225" ;
 StringGridTab->Cells[3][2]="0.25";

 StringGridTab->Cells[0][3]="������������ ����� ������� ���������, ������ �������� �����,GGG, ������� �������� �������";
 StringGridTab->Cells[1][3]="" ;
 StringGridTab->Cells[2][3]="1350" ;
 StringGridTab->Cells[3][3]="0.28";

 StringGridTab->Cells[0][4]="������������������ �������� ����� � �������� ���������� �������; ������ �������� �����,GGG, ������� �������� �������";
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

   // ������� ������� N
 StringGridTab->Cells[0][1]="������ ������� ��������, ������ ��� ���������; �������� � �����������<16%Si, ������, ����� � ������";
 StringGridTab->Cells[1][1]="" ;
 StringGridTab->Cells[2][1]="700" ;
 StringGridTab->Cells[3][1]="0.25";

 StringGridTab->Cells[0][2]="������ ������� ��������, ������� ��� ���������; �������� � �����������>16%Si, ������, ����, ����������� ������ (������, ����, ������)";
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

   // ����������� ������ � ��������� ������ S
 StringGridTab->Cells[0][1]="����������� ������, ���������� ������, �������, ������, ���������<30HRc, ������� 800 � �������� 601, 617 � 625,������ 400";
 StringGridTab->Cells[1][1]="" ;
 StringGridTab->Cells[2][1]="2600" ;
 StringGridTab->Cells[3][1]="0.24";

 StringGridTab->Cells[0][2]="����������� ������, ���������� ������, �������, ������, ���������>30HRc, ������� 718 � �������� 750-�, � ������� 925, ������ �-5008";
 StringGridTab->Cells[1][2]="" ;
 StringGridTab->Cells[2][2]="3300" ;
 StringGridTab->Cells[3][2]="0.24";

 StringGridTab->Cells[0][3]="��������� ������, Ti-6Al-4V";
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
  i=ComboBoxMaterial->ItemIndex; //  0-"�95���2"  1-"1163�2"  2-"�16��"
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

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnAlTm1Click(TObject *Sender)
{
  PanelTemp->Visible=True;
  CalcTechAlTemp (Sender);
  PanelTemp->Update();
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

void __fastcall TFormMain::EditVChange(TObject *Sender)
{
  int i;
  /**************************************************************************/
  /* ������ */
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


