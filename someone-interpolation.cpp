#include<iostream>
#include<cstring>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<cmath>

using namespace std;

int main( )
{ int n;
  char c;
  void sub11(),sub22(),sub33(),sub44(),sub55(),sub66(),sub77();
   do
   {         system("cls");
             cout<<"\n\n\n" ;
             cout<<"\n  1.数的合法输入并相加 " ;
             cout<<"\n  2.线性插值之拉格朗日法" ;
             cout<<"\n  3.二次插值（抛物线插值）之牛顿插值法" ;
             cout<<"\n  4.n次插值之拉格朗日法 " ;
             cout<<"\n  5.分段线性插值 " ;
             cout<<"\n  6.最小二乘" ;
             cout<<"\n  7.高斯消去 " ;
             cout<<"\n  8.    退出" ;
             cout<<"\n    Please Input your choice!(1--8)\n" ;

             scanf("%d",&n);
             switch (n)
               {
                 case 1:  sub11( ); break;
                 case 2:  sub22( ); break;
                 case 3:  sub33( ); break;
                 case 4:  sub44( ); break;
                 case 5:  sub55( ); break;
                 case 6:  sub66( ); break;
                 case 7:  sub77( ); break;
                 case 8:    goto end;
                 default :  cout<<"\n输入有误\n" ;
               }
              cout<<"\n  press anykey to continue\n" ;
              cin>>c;
              c=getchar( );
      } while(n!=8);
      end: ;
}



int is_number(const char *p)
{
    int flag0=0;
    int flag1=0;
    int flag3=0;
    while(*p)
    {
        if (*p<'0' || *p>'9')
        {
            if (*p!='.' && *p!='-') return -1;
            if (*p=='.') flag0++;
            if (*p=='-')
            {
                if (flag3>0) return -1;
                else flag1++;
            }
            if (flag0 > 1 || flag1>1 ) return -1;
        }
        p++;
        flag3++;
    }
    if (flag0==1)
        return 1; //float
    else
        return 0 ;//integer
}


void sub11()
{
    char s1[10],s2[10];
    char *p1,*p2;
    cout << "input first:\n";
    cin >> s1;
    cout << "input second:\n";
    cin >>s2;
    p1=s1;
    p2=s2;
    if (is_number(p1)>=0 && is_number(p2)>=0)
    {       cout<<'\n'<<atof(p1)+atof(p2);  }
    else
    {       cout<<s1<<s2;   }

}


void sub22()
{


    float x0,y0,x1,y1,xm,x,ym;float s;

    cout<<"please input two points:(x0,y0),(x1,y1) and (x0!=x1)"<<endl;
    cin>>x0>>y0>>x1>>y1;
    ym=y0*((x-x1)/(x0-x1))+y1*((x-x0)/(x1-x0));
    s=(y1-y0)/(x1-x0);
    cout<<"please input xm:"<<endl;
    cin>>xm;
    cout<<"ym="<<(y0*((xm-x1)/(x0-x1))+y1*((xm-x0)/(x1-x0)))<<endl;
    cout<<"方程是:y-"<<y1<<"="<<s<<"(x-"<<x1<<")"<<endl;
}
void sub33()
{


    float x0,y0,x1,y1,x2,y2,xm,x,ym;float y01,y12,y012;
    cout<<"please input two points:(x0,y0),(x1,y1),(x2,y2)and (x0!=x1!=x2)"<<endl;
    cin>>x0>>y0>>x1>>y1>>x2>>y2;
    y01=(y0-y1)/(x0-x1);y12=(y1-y2)/(x1-x2);y012=(y01-y12)/(x0-x2);
    ym=y0+(x-x0)*y01+(x-x0)*(x-x1)*y012;
    cout<<"please input xm:"<<endl;
    cin>>xm;
    cout<<"ym="<< y0+(xm-x0)*y01+(xm-x0)*(xm-x1)*y012<<endl;
}
void sub44()
{

    int n,i,j;
    double xm,ym=0;
    double x[200],y[200], l[200];
    cout << "input n=";
    cin >> n;
    cout << endl;
    cout << "input xm=";
    cin >> xm;
    cout << endl;
    cout << "你输入的xm="<<xm<<"那么 ym=?"<< endl;

    for(i=0;i<=n;i++)
    {
        cout << "x[" << i << "]=";
        cin >> x[i];
        cout << endl;
    }
    for(i=0;i<=n;i++)
    {
        cout << "y[" << i << "]=";
        cin >> y[i];
        cout << endl;
    }

    for(i=0;i<=n;i++)
    {
        double b=1,c=1;
        for(j=0;j<=n;j++)
        {
            if(i!=j){b=b*(xm-x[j]);}
        }
        for(j=0;j<=n;j++)
        {
            if(i!=j){c=c*(x[i]-x[j]);}
        }
        l[i]=b/c;
    }

    for(i=0;i<=n;i++)
    {
         ym=ym+y[i]*l[i];
    }

    cout << ym<<endl;
}
void sub55()
{
    int i,n;

    double xm,ym;
    cout << "input xm=";
    cin >> xm;
    cout << endl;
    cout << "你输入的xm="<<xm<<"那么 ym=?"<< endl;

    cout << "你现在拥有几个点的坐标？至少得有2个点";
    cin >> n;
    cout << endl;
    cout << "那么你现在一一输入这些坐标\n";
    double x[200],y[200];
    for(i=0;i<n;i++)
    {
        cout << "x[" << i << "]=";
        cin >> x[i];
        cout << endl;
    }
    for(i=0;i<n;i++)
    {
        cout << "y[" << i << "]=";
        cin >> y[i];
        cout << endl;
    }



    for (i = 0; i<n; i++)
    {
        if (x[i] <= xm && xm<=x[i+1] )
            ym=(y[i]*((xm-x[i+1])/(x[i]-x[i+1]))+y[i+1]*((xm-x[i])/(x[i+1]-x[i])));
            cout<<"ym="<<ym<<endl;
            double s=(y[i+1]-y[i])/(x[i+1]-x[i]);
            cout<<"该段对应的方程是:y-"<<y[i+1]<<"="<<s<<"(x-"<<x[i+1]<<")"<<endl;
            cout<<"其中x的区间为【"<<x[i]<<"，"<<x[i+1]<<"】"<<"显然xm="<<xm<<"在这一区间内"<<endl;
            break;
     }

}
void sub66()
{

    int n,i;
    float a,b,xsum=0,ysum=0,squaresum=0,xy=0,x[25],y[25],D;
    cout<<"请输入点数n:\n";
    cin>>n;
    cout<<"请输入所有点坐标:\n";
    for(i=0;i<n;i++)
    {
        cin>>x[i]>>y[i];
        xsum+=x[i];
        ysum+=y[i];
        squaresum+=x[i]*x[i];
        xy+=x[i]*y[i];
    }
    D=n*squaresum-xsum*xsum;
    if(D==0)
        cout<<"原方程无解或有多组解!\n";
    else
        {
         a=(squaresum*ysum-xsum*xy)/D;
         b=(n*xy-xsum*ysum)/D;
        }
    cout<<"y="<<a<<"+"<<b<<"x\n";



}
void sub77()
{

    cout<<"测试例子："<<endl;
    cout<<"a[1][1] = 4     a[1][2] = 1     a[1][3] = 1     a[1][4] = 1"<<endl;
    cout<<"a[2][1] = 1     a[2][2] = -1    a[2][3] = 2     a[2][4] = -2"<<endl;
    cout<<"a[3][1] = 2     a[3][2] = 2     a[3][3] = -1    a[3][4] = 2"<<endl;
    cout<<"此方程组的解为 ："<<endl;
    cout<<"x[1] = 1        x[2] = -1       x[3] = -2 "<<endl;
    cout<<endl;


    int n;
    cout << "你有几行方程? 不限规模";
    cin >> n;
    cout << endl;

    cout << "请输入" <<n<<"行"<<n+1<<" 列数据:"<<endl;
    cout <<"[注：其中每一行的最后一个值代表b1b2...b"<<n<<"]"<<endl;

    float **a ;
    a=new float*[n+1];
    int i,j;
    for(i=1;i<=n;i++)
    {
        a[i]=new float[n+1];

        for(j=1;j<=n+1;j++)
        {
            cout << "a[" << i << "]["<<j<<"]= ";
            cin >> a[i][j];
        }
        cout << endl;
    }//输入数据

    for(i=1;i<=n;i++)
    {
        if(a[i][i]==0)
        {
            cout<<"无解！"<<endl;
            goto end;
        }
    }

    float t;
    for(i=1;i<=n;i++)
    {
        t=1/a[i][i];
        for(int l=1;l<=n+1;l++)
            a[i][l]=t * a[i][l];
        for(int m=i+1;m<n+1;m++)
        {
            t=a[m][i];
            for(j=1;j<=n+1;j++)
                    a[m][j]=t * a[i][j] - a[m][j];

        }
    }//转化过程




    cout<<endl<<"转化为上三角矩阵："<<endl;
    for( i=1;i<=n;i++)
    {
        for(j=1;j<=n+1;j++)
            cout<<"a["<<i<<"]["<<j<<"] = "<<a[i][j]<<'\t';
        cout<<endl;
    }//输出上三角


    cout<<endl<<"此方程组的解为 ："<<endl;
    float  *x;
    x = new float [n];
    x[n] = a[n][n+1]/a[n][n];

    for( i=n-1;i>0;i--)
    {
        for(j=n;j>i;j--)
            a[i][n+1] = a[i][n+1] - a[i][j] * x[j];
        x[i] = a[i][n+1] / a[i][i];
    }//计算解

    for( i=1;i<=n;i++)
        cout<<"x["<<i<<"] = "<<x[i]<<"\t";
    cout<<endl;//输出解

    end:;

}
