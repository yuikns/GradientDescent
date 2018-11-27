#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define DS_SIZE 10
#define DEVIATION 0.02

typedef struct
{
    double x[1000];
    double y ;
} DS;

double l(double x[], double w[], int wl, double y);
double f(double x[], double w[], int wl, double d);

void print_ws(double w[],int wl,int endl);
double lose_check(DS ds[],int dsl,double wc[], int wl);

double sgd(DS ds[],int dsl,double e,double wc[],int wl);
double bgd(DS ds[],int dsl,double e,double wc[],int wl);

void print_ws(double w[],int wl,int endl)
{
    for(int i = 0; i < wl ; i ++ )
    {
        printf("%.4f ",w[i]);
    }
    if(endl) printf("\n");
}

inline double nsq(double x,double v)
{
    double r = 1;
    for(int i = 0 ; i < v ; i ++)
    {
        r *= x;
    }
    return r;
}

double lose_check(DS ds[],int dsl,double wc[], int wl)
{
    double all_lose = 0;
    for(int i = 0 ; i < dsl ; i ++ )
    {
        all_lose += l(ds[i].x,wc,wl,ds[i].y);
    }
    return all_lose/dsl;
}


double bgd(DS ds[],int dsl,double e,double wc[],int wl)
{
    double m_ls;
    double delta = 0.0000001; // learn rate
    int loop = 0;
    double p_ls = -1;
    while((m_ls = lose_check(ds,dsl,wc,wl)) > e )
    {
        if(p_ls != -1 && p_ls <= m_ls)
        {
            delta /= 10;
            printf("delta %f => %f \n",delta * 10 , delta);
        }
        if(delta < 0.00000000001)
        {
            printf("jump out for too small scale");
            break ;
        }

        if(loop >= 3000000000)
        {
            printf("jump out for loop time limit (10000)\n");
            break;
        }
        if(loop % 1000000 == 0 )
        {
            print_ws(wc,wl,0);
            printf(" e = %.6f  lose = %f \n",e,m_ls);
        }
        double lose_sum[wl];
        memset(lose_sum,0,sizeof(double) * wl);
        for(int i = 0 ; i < dsl ; i ++ )
        {
            double fx = f(ds[i].x,wc,wl,0);
            for(int j = 0 ; j < wl ; j ++ )
            {
                lose_sum[j] += (ds[i].y - fx) * (j > 0 ? ds[i].x[j-1] :1);
            }
        }
        for(int j = 0 ; j < wl ; j ++ )
        {
            wc[j] += delta * lose_sum[j] / dsl;
        }
        loop ++ ;
    }
    return m_ls;
}

double sgd(DS ds[],int dsl,double e,double wc[],int wl)
{
    double m_ls;
    double delta = 0.0000003; // learn rate
    int loop = 0 ;
    double p_ls = -1;
    while((m_ls = lose_check(ds,dsl,wc,wl)) > e )
    {
        if(p_ls != -1 && p_ls <= m_ls)
        {
            delta /= 2;
            printf("delta %.3f => %.3f \n",delta * 10 , delta);
        }
        if(delta < 0.00000000001)
        {
            printf("jump out for too small scale");
            break ;
        }

        if(loop >= 30000000)
        {
            printf("jump out for loop time limit (30000000)\n");
            break;
        }

        if(loop % 1000000 == 0 )
        {
            print_ws(wc,wl,0);
            printf(" e = %.6f  lose = %f \n",e,m_ls);
        }
        for(int i = 0 ; i < dsl ; i ++ )
        {
            double fx = f(ds[i].x,wc,wl,0);
            for(int j = 0 ; j < wl ; j ++ )
            {
                wc[j] +=  delta * ( ds[i].y - fx )  * ( j > 0 ? ds[i].x[j-1] : 1 );
            }
        }
        loop ++ ;
    }
    return m_ls;
}

double l(double x[], double w[], int wl, double y)
{
    double fx = f(x,w,wl,0);
    return (fx - y) * (fx - y) / 2;
}

// function
double f(double x[], double w[], int wl, double d)
{
    double fx = 0 ;
    for(int i = 0 ; i < wl ; i ++ )
        fx += i > 0 ? x[i-1] * w[i] : w[i];
    return fx + (rand() % 6 - 3) * d;
}

int main(int argc,char * argv[])
{
    srand((unsigned long)time(NULL));
    // set para
    int wl = 3;
    double w[wl] ;
    printf("WL : %d \n",wl);
    for (int i = 0 ; i < wl ; i ++ )
    {
        w[i] = rand() % 10 - 5;
        printf("w[%d] = %.3f \n",i,w[i]);
    }
    DS ds[DS_SIZE];
    printf("data set size : %d \n",DS_SIZE);
    printf("deviation : %.3f \n",DEVIATION);
    for(int i = 0 ; i < DS_SIZE ; i  ++ )
    {
        printf("ds[%d] : x = [",i);
        for(int j = 0 ; j < wl - 1; j ++ )
        {
            ds[i].x[j] = (j == 0) ? (double) (rand() % ( DS_SIZE * 5)) : nsq(ds[i].x[0],j+1);
            printf("%.3f",ds[i].x[j]);
            if(j <  wl - 2 ) putchar(',');
        }
        ds[i].y = f(ds[i].x,w,wl,DEVIATION);
        printf("] y = %.3f \n", ds[i].y);
    }

    double epsilon = 0.0001;
    printf("e is : %.3f \n",epsilon);
    printf("real w are : \n");
    print_ws(w,wl,1);
    double wc[wl];
    memset(wc,0,sizeof(double) * wl);
    printf("start gd ...\n");
    getchar();
    double lose = sgd(ds,DS_SIZE,epsilon,wc,wl);
    //double lose = bgd(ds,DS_SIZE,epsilon,wc,wl);
    printf("real w are : \n");
    print_ws(w,wl,1);
    printf("result with lose %.6f is : \n",lose);
    print_ws(wc,wl,1);
    return 0;
}
