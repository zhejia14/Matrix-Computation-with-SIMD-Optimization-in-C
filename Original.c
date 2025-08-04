#include <stdio.h>
#include <time.h>
float MulSum(float A[][198], float B[][198],int i)						/*calculate function ; Input: data A ,data B ,and i (row)*/
{   float val=0;														/*The sum of the answer*/
    for(int j=0;j<198;j++)												/*for loop count the column*/
    {   for(int k=0;k<200;k++)											/*for loop count B data's row*/
        {
                val+=A[i][j]*B[k][j];									/*A data's row * each of B data */
        }

    }
    return val;															/*return the sum*/

}
struct timespec diff(struct timespec start, struct timespec end)		/*Counting time function */
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
{   float A[200][198]={0};
    float B[200][198]={0};
    float sum[200]={0};
    struct timespec time1, time2, time3, time4, time5, time6;

    clock_gettime(CLOCK_MONOTONIC, &time1);								/*Record time1*/
    FILE *nfptr=fopen("output.txt", "w");								/*Create output.txt for writing*/
    FILE *fptr=fopen("data.txt", "r");									/*Open data.txt for reading*/
    if(fptr==NULL)puts("OPEN FAIL\n");									/*If fail, show OPEN FAIL */
    else                                                                /*Read data*/
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
    for(int i=0;i<200;i++)												/*for loop for A data's row and sum array*/
    {
        sum[i]=MulSum(A, B, i);											/*calculate ; Input: matrix A ,matrix B ,and i (row)*/
    }
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
