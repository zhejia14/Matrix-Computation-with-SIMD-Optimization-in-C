#include <stdio.h>
#include <time.h>
#include <xmmintrin.h>
float MulSum(float A[][200], float B[][200],int i)					/*calculate function ; Input: data A ,data B ,and i (row)*/
{   float tmp[4]__attribute__((aligned(16)));						/*save the complete data;Align data for good performance*/
    __m128 *a, *b, *temp;
    a=(__m128*)A[i];												/*Let A data's row be vectors each of them has four data,so 200 data -->> 50 data*/
    temp=(__m128*)tmp;
    float val=0;													/*The sum of the answer*/
    for(int k=0;k<200;k++)											/*for loop count B data's row*/
    {   b=(__m128*)B[k];											/*Let B data's row be vectors each of them has four data,so 200 data -->> 50 data*/
        for(int j=0;j<50;j++)										/*for loop count the column*/
        {
            *temp=_mm_mul_ps(a[j],b[j]);							/*SIMD instruction a vector * b vector= temp vector*/
            val+=tmp[0]+tmp[1]+tmp[2]+tmp[3];						/*val= the sum of temp vector's 4 elements */
        }
    }
    return val;														/*return the sum*/

}
struct timespec diff(struct timespec start, struct timespec end)
{   struct timespec temp;
    if((end.tv_nsec-start.tv_nsec<0))
    {   temp.tv_sec=end.tv_sec-start.tv_sec-1;
        temp.tv_nsec=1000000000+end.tv_nsec-start.tv_nsec;
    }
    else
    {   temp.tv_sec=end.tv_sec-start.tv_sec;
        temp.tv_nsec=end.tv_nsec-start.tv_nsec;
    }
    return  temp;
}
int main()																/*begin function*/
{   float A[200][200]__attribute__((aligned(16)));						/*Align data for good performance*/
    float B[200][200]__attribute__((aligned(16)));						/*Align data for good performance*/
    float sum[200]={0};
    struct timespec time1, time2, time3, time4, time5, time6;
    for(int i=0;i<200;i++)												/*Let A and B 198,199 column 0; for calculating more conveninent */
	{	for(int j=198;j<200;j++)										
		{
			A[i][j] = 0;
			B[i][j] = 0;
		}
	}
    clock_gettime(CLOCK_MONOTONIC, &time1);								/*Record time1*/
    FILE *nfptr=fopen("output.txt", "w");								/*Create output.txt for writing*/
    FILE *fptr=fopen("data.txt", "r");									/*Open data.txt for reading*/
    if(fptr==NULL)puts("OPEN FAIL\n");									/*If fail, show OPEN FAIL */
    else																/*Read data*/
    {   for(int i=0;i<200;i++)											/*for loop for A matrix row*/
        {       for(int j=0;j<198;j++)fscanf(fptr, "%f", &A[i][j]);		/*for loop for A matrix column */
        }
        for(int k=0;k<200;k++)											/*for loop for B matrix row*/
        {       for(int m=0;m<198;m++)fscanf(fptr, "%f", &B[k][m]);		/*for loop for B matrix column */
        }
    }
    fclose(fptr);														/*close data.txt*/

    clock_gettime(CLOCK_MONOTONIC, &time2);								/*Record time2*/

    clock_gettime(CLOCK_MONOTONIC, &time3);								/*Record time3*/

    for(int i=0;i<200;i++)
        sum[i]=MulSum(A, B, i);											/*calculate ; Input: matrix A ,matrix B ,and i (row)*/

    clock_gettime(CLOCK_MONOTONIC, &time4);								/*Record time4*/
    clock_gettime(CLOCK_MONOTONIC, &time5);								/*Record time5*/
    for(int i=0;i<200;i++)
    {
        fprintf(nfptr, "%f\n", sum[i]);									/*write answer to output.txt*/
    }
    clock_gettime(CLOCK_MONOTONIC, &time6);								/*Record time6*/
    fclose(nfptr);														/*close output.txt*/

    printf("READ %ld : %ld\n", diff(time1, time2).tv_sec, diff(time1, time2).tv_nsec);		/*show READE time(time2-time1)*/
    printf("CALCULATE %ld : %ld\n", diff(time3, time4).tv_sec, diff(time3, time4).tv_nsec);	/*show CALCUATE time(time4-time3)*/
    printf("WRITE %ld : %ld\n", diff(time5, time6).tv_sec, diff(time5, time6).tv_nsec);		/*show WRITE time(time6-time5)*/
    return 0;															/*end funtion*/
}

