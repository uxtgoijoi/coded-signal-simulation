// 卷积编码(r=1/2,k=7)
// g1= 1 1 1 1 0 0 1
// g2= 1 0 1 1 0 1 1
//　输入l bit，输出(2*l+(k-1)*2)bit，在信息序列较长的时候接近于1/2
// 当前程序给每bit分配了1个int的空间，可改进

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cv_code(int *buff_in,int k,int *buff_out);
int move(int *buff);//右移

int main()
{
	int m[7]={1,1,1,1,1,1,1,};//读入数据
	//
	int out[26];//
	cv_code(m,7,out);//k=7

	// for(int i=0;i<10;i++)
	// {
	// 	printf("%d ",out[i]);
	// }
	// printf("\n");

	return 0;
}

int cv_code(int *buff_in,int k,int *buff_out)
{
	int len=sizeof(buff_in)/sizeof(int)+1;
	int count=0;
	int *temp;
	temp=(int*)calloc(k,sizeof(int));
	memset(temp,0,k);
    for(int i=0;i<len;i++)
    {
    	move(temp);
    	temp[0]=buff_in[i];
    	buff_out[count++]=(temp[0]+temp[1]+temp[2]+temp[3]+temp[6])%2;//
    	buff_out[count++]=(temp[0]+temp[2]+temp[3]+temp[5]+temp[6])%2;//
    }
    for(int i=0;i<len-1;i++)
    {
    	move(temp);
    	temp[0]=0;
    	buff_out[count++]=(temp[0]+temp[1]+temp[2]+temp[3]+temp[6])%2;//
    	buff_out[count++]=(temp[0]+temp[2]+temp[3]+temp[5]+temp[6])%2;//
    }

	free(temp);
	return 0;
}

int move(int *buff)
{
	int len=sizeof(buff)/sizeof(int)+1;
	for(int i=0;i<len-1;i++)
	{
		buff[len-i-1]=buff[len-i-2];
	}
}