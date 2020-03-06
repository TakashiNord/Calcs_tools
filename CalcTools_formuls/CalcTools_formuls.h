
#include "stdio.h"
#include "stdlib.h"
#include <math.h>

int  EQ_is_zero (double s);
int  EQ_is_equal (double s, double t);
/*s >= t*/
int  EQ_is_ge (double s, double t);
/*s > t*/
int  EQ_is_gt (double s, double t);
/*s <= t*/
int  EQ_is_le (double s, double t);
/*s < t*/
int  EQ_is_lt (double s, double t);
/***********************************************************************/


//----------- ������������ ������� �������� ------
double spl(double x,double y[101],int n1,double a1,double b1, double m2[101]);

//----------- ������������ ������� �������� ------
int polinom_file(char *name_file,int nN);

//----------- ������������ ����������������� �������� ------
double polinom(int n,double arg, double x[101],double y[101]);

//-------- ������� ���������� ��������� ���� ������������----
double fmax (double x, double y);
double fmin (double x, double y);

//--------- ���������� ������ 1----------
double diskan1(int K, double fun[101], double pol[101]) ;

//-------���������� ������ 2-----------
double diskan2(int K, double fun[101], double pol[101]);

/*������ �� ���*/
double Sz ( double D, double t, double B, double Ks);

/*�������� ������� (������ �� ����������� ������) - V1*/
double V1 (int type, double D, double t, double B, double Sz);

/*�������� ������� (������ �� ��������) - V2*/
double V2 (int type, double D, double t, double B, double Sz , double Nct ,double procent ) ;

/*����� �������� n=V*1000/(Pi*D) */
double nN (double D, double V) ;

/*�������� ������� V=Pi*D*n/1000. */
double nV (double D, double n) ;

/*����� ������ vf=sz*z*n) */
double vF (double Sz,int z, double n) ;

/*������ ���� Pz*/
double Pz_forest(int type, double D, double t, double Sz , double n ) ;

/*������ ������� M*/
double Mz_forest(double D, double Pz );

/*������ �������� N*/
double Nz1_forest(double V, double Pz );
