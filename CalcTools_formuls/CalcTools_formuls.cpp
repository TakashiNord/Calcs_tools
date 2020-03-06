// *.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "stdlib.h"
#include <math.h>

static double Temperature[2][9]={
 {150,108,89,77,69,64,55,52,49},	
 {175,125,104,90,81,74,64,60,57}
} ;

/***********************************************************************/
int  EQ_is_zero (double s)
{
  double mom_system_tolerance=0.00000001 ;
  if (fabs(s)<= mom_system_tolerance) { return(1); } else { return(0) ; }
}
/***********************************************************************/
int  EQ_is_equal (double s, double t)
{
	double mom_system_tolerance=0.00000001 ;
	if (fabs(s-t)<= mom_system_tolerance) { return(1); } else { return(0) ; }
}
/***********************************************************************/
/*s >= t*/
int  EQ_is_ge (double s, double t)
{
         double mom_system_tolerance=0.00000001 ;
         if (s > (t - mom_system_tolerance)) { return(1); } else { return(0) ; }
}
/***********************************************************************/
/*s > t*/
int  EQ_is_gt (double s, double t)
{
         double mom_system_tolerance=0.00000001 ;
         if (s > (t + mom_system_tolerance)) { return(1); } else { return(0) ; }
}
/***********************************************************************/
/*s <= t*/
int  EQ_is_le (double s, double t)
{
         double mom_system_tolerance=0.00000001 ;
         if (s < (t + mom_system_tolerance)) { return(1); } else { return(0) ; }
}
/***********************************************************************/
/*s < t*/
int  EQ_is_lt (double s, double t)
{
         double mom_system_tolerance=0.00000001 ;
         if (s > (t - mom_system_tolerance)) { return(1); } else { return(0) ; }
}
/***********************************************************************/


//----------- Формирование сплайна полинома ------
double spl(double x,double y[101],int n1,double a1,double b1, double m2[101])
{
/* Применение:
 h=(b1-a1)/(n-1);
 for(i=0;i<29;i++) { m[i]=a1+h*i; y[i]=fun(m[i]); }
 h1=(b1-a1)/100.0;
 for(i=0;i<101;i++)
 { m1[i]=a1+i*h1;y1[i]=fun(m1[i]);o[i]=spl(m1[i],y,n,m);c1[i]=fabs(o[i]-y1[i]); }
 for(i=0,P1=c1[0],sum=0;i<101;i++) { if (P1<c1[i]) { P1=c1[i]; } sum=sum+c1[i]*c1[i]; }
 P2=sqrt((1.0/101.0)*sum);
 */
 double h2,c[29],al[29],bt[29],a[29],b[29],S,d[29],t[29];
  int i,j,n2;
  n2=n1-1;
  /*double a1=-1,b1=1;*/
  h2=(b1-a1)/(n1-1);al[1]=0.0;bt[1]=0.0;
  for(i=0;i<=n2;i++) a[i]=y[i];
  for(i=1;i<n2;i++)
  {
   t[i]=(3*(y[i+1]-2*y[i]+y[i-1]))/h2;
   al[i+1]=-h2/(h2*al[i]+4*h2);
   bt[i+1]=(t[i]-h2*bt[i])/(h2*al[i]+4*h2);
  }

  c[n1-1]=0;
  for(i=n1-2;i>=0;i--) c[i]=al[i+1]*c[i+1]+bt[i+1];
  for(i=0;i<n2;i++)
  {
	  b[i]=(y[i+1]-y[i])/h2-(h2*(c[i+1]+2*c[i]))/3;
	  d[i]=(c[i+1]-c[i])/(3*h2);
  }
  j=int((x-a1)/h2);
  S=a[j]+b[j]*(x-m2[j])+c[j]*pow((x-m2[j]),2)+d[j]*pow((x-m2[j]),3);
  return S;
}

//----------- Формирование сплайна полинома ------
int polinom_file(char *name_file,int nN)
{
 double x[101],y[101];
 int i;
 FILE *rf;
 rf=fopen(name_file,"r"); //"test_tb.txt"
 if (rf==NULL) { puts("Can't open file");   return(-1); }
 for(i=0;i<nN;i++) { fscanf(rf,"%f %f",&x[i],&y[i]); }
 fclose(rf);
 return(0);
}

//----------- Формирование интерполяционного полинома ------
double polinom(int n,double arg, double x[101],double y[101])
{ /* Вычисляет значение интерполяционного полинома степени n-1
     в форме Лагранжа в точке arg, используя интерполяционную таблицу
	 x[i],  y[i] */
  double pr,s;
  int i,j;
  for(i=0,s=0.;i<n;i++)
  { for(j=0,pr=1.;j<n;j++) if (i!=j) pr*=((arg-x[j])/(x[i]-x[j]));
    s+=y[i]*pr;
  }
  return s;
}

//-------- Функция вычисления максимума двух вещественных----
double fmax (double x, double y) { if (x>y) return x; else return y;}
double fmin (double x, double y) { if (x<y) return x; else return y;}

//--------- Дискретный аналог 1----------
double diskan1(int K, double fun[101], double pol[101])
{ double P1, nr;int i;
  P1=fabs(fun[0]-pol[0]);
  for (i=0;i<K;i++) { nr=fabs(fun[i]-pol[i]);P1=fmax(P1,nr); }
  return P1;
}

//-------Дискретный аналог 2-----------
double diskan2(int K, double fun[101], double pol[101])
{ double s,nrq;int i;
  s=pow((fun[0]-pol[0]),2);
  for(i=0;i<K;i++){ nrq=pow(fabs(fun[i]-pol[i]),2);s=s+nrq;}
  return pow(s/K,0.5);
}

/*Подача на зуб*/
double Sz ( double D, double t, double B, double Ks)
{
 double t1,t2,t3;
 double tSz=-1.;
 if (EQ_is_le(D,0.)) return(tSz);
 if (EQ_is_le(t,0.)) return(tSz);
 if (EQ_is_le(B,0.)) return(tSz);
 if (EQ_is_le(Ks,0.)) return(tSz);

 t1=pow(D,0.75);
 t2=pow(t,0.1);
 t3=pow(B,0.22);

 tSz=0.04*t1*Ks/(t2*t3) ;

 return (tSz);
}

/*Скорость резания (исходя из температуры сплава) - V1*/
double V1 (int type, double D, double t, double B, double Sz)
{
 double t1,t2,t3;
 double K=0;
 double tV=-1.;
 if (EQ_is_le(D,0.)) return(tV);
 if (EQ_is_le(t,0.)) return(tV);
 if (EQ_is_le(B,0.)) return(tV);
 if (EQ_is_le(Sz,0.)) return(tV);

 K=B/D;

 if (type==1) {
     //1=1163Т2 2=Д16чТ
     t1=pow(Sz,0.88);
     t2=pow(t,0.8);
     t3=pow(K,0.46);
     tV=2070./(t1*t2*t3) ;
 }

 if (type==2) {
     //0=В95пчТ2
     t1=pow(Sz,0.86);
     t2=pow(t,0.81);
     t3=pow(K,0.47);
     tV=1097./(t1*t2*t3) ;
 }

 return (tV);
}

/*Скорость резания (исходя из мощности) - V2*/
double V2 (int type, double D, double t, double B, double Sz , double Nct ,double procent )
{
 double t1,t2,t3,t4;
 double K=0;
 double tV=-1.;
 if (EQ_is_le(D,0.)) return(tV);
 if (EQ_is_le(t,0.)) return(tV);
 if (EQ_is_le(B,0.)) return(tV);
 if (EQ_is_le(Sz,0.)) return(tV);

 K=B/D;

 if (type==1) {
     //1=1163Т2 2=Д16чТ
     t1=pow(t,0.56);
     t2=pow(Sz,0.344);
     t3=pow(D,0.225);
     t4=Nct*procent*K*52./(t1*t2*t3) ;
     tV=pow(t4,1.126);
 }

 if (type==2) {
     //0=В95пчТ2
     t1=pow(t,0.684);
     t2=pow(Sz,0.548);
     t3=pow(D,0.286);
     t4=Nct*procent*K*49./(t1*t2*t3) ;
     tV=pow(t4,1.134);
 }

 return (tV);
}

/*Число оборотов n=V*1000/(Pi*D) */
double nN (double D, double V)
{
 double tN=-1.;
 if (EQ_is_le(D,0.)) return(tN);
 if (EQ_is_le(V,0.)) return(tN);
 tN=V*1000./(3.1415926*D) ;
 return (tN);
}

/*Скорость резания V=Pi*D*n/1000. */
double nV (double D, double n)
{
 double tV=-1.;
 if (EQ_is_le(D,0.)) return(tV);
 if (EQ_is_le(n,0.)) return(tV);
 tV=3.1415926*D*n/1000. ;
 return (tV);
}

/*Число подачи vf=sz*z*n) */
double vF (double Sz,int z, double n)
{
 double tF=-1.;
 if (z<=0) return(tF);
 if (EQ_is_le(n,0.)) return(tF);
 if (EQ_is_le(Sz,0.)) return(tF);

 tF=Sz*n*z ;
 return (tF);
}


/*Расчет силы Pz*/
double Pz_forest(int type, double D, double t, double Sz , double n )
{
 double t1,t2,t3,t4;
 double tP=-1.;
 if (EQ_is_le(D,0.)) return(tP);
 if (EQ_is_le(t,0.)) return(tP);
 if (EQ_is_le(n,0.)) return(tP);
 if (EQ_is_le(Sz,0.)) return(tP);

 if (type==1) {
     t1=pow(t,0.506);
     t2=pow(Sz,0.344);
     t3=pow(D,0.123);
     t4=pow(n,0.112);
     tP=224.*t1*t2*t3/t4;
 }

 if (type==2) {
     t1=pow(t,0.684);
     t2=pow(Sz,0.546);
     t3=pow(D,0.168);
     t4=pow(n,0.118);
     tP=246*t1*t2*t3/t4;
 }

 return (tP);
}


/*Расчет момента M*/
double Mz_forest(double D, double Pz )
{
 double tM=-1.;
 if (EQ_is_le(D,0.)) return(tM);
 if (EQ_is_le(Pz,0.)) return(tM);

 tM=Pz*D/(2.*100.);

 return (tM);
}

/*Расчет мощности N*/
double Nz1_forest(double V, double Pz )
{
 double tN=-1.;
 if (EQ_is_le(V,0.)) return(tN);
 if (EQ_is_le(Pz,0.)) return(tN);

 tN=Pz*V/(1020.*60.);

 return (tN);
}


/*
int main(int argc, char* argv[])
{
	double B, t, D ;
	int z = 2 ;
	D=30;
	B=10;
	t=3;

	double Sz1;
	Sz1=Sz( D, t, B, 0.88);
	printf("Sz=%.3lf \n", Sz1);

	double v11,v12;
	v11=V1 (1, D, t, B, Sz1);
	printf("1 = v11=%.3lf \n", v11);

	v12=V1 (2, D, t, B, Sz1);
	printf("2 = v12=%.3lf \n", v12);


	double v21,v22;
	v21=V2 (1, D, t, B, Sz1, 26, 1);
	printf("1 = v21=%.3lf \n", v21);

	v22=V2 (2, D, t, B, Sz1, 26, 1);
	printf("2 = v22=%.3lf \n", v22);

	double n;
	n=nN (D, v11 );
	printf(" n=%.3lf (v11=%.3lf) \n", n, v11);
	n=nN (D, v12 );
	printf(" n=%.3lf (v12=%.3lf) \n", n, v12);

	
	double vf;
	vf=vF (Sz1,z,n) ;
	printf(" vf=%.3lf (n=%.3lf) \n",vf, n);

	double p1;
	p1=Pz_forest(1, D, t, Sz1 , n ) ;
	printf("1 p1=%.3lf \n",p1);

	n=nN (D, v21 );
	printf(" n=%.3lf (v21=%.3lf) \n", n, v21);
	n=nN (D, v22 );
	printf(" n=%.3lf (v22=%.3lf) \n", n, v22);

	p1=Pz_forest(2, D, t, Sz1 , n ) ;
	printf("2 p2=%.3lf \n",p1);

	printf("__END__\n");
	return 0;
}

*/
