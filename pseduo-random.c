//伪随机化信号
//伪随机序列生成多项式h(x)=x^8+x^7+x^5+x^3+1
#include <stdio.h>
#include <string.h>

void getPseduoRandomCode(unsigned char *puc_RandomCode);

int main()
{
	int i;
	unsigned char uc_PseduoRandomCode[32];
	memset(uc_PseduoRandomCode,0,32);

	getPseduoRandomCode(uc_PseduoRandomCode);

	for(i=0;i<32;i++)
	{
		printf("%d ",uc_PseduoRandomCode[i]);
	}
	printf("\n");
	//与输入信号做异或操作
	
	return 0;
}

//获得伪随机码
void getPseduoRandomCode(unsigned char *puc_RandomCode)
{	
	int i,j,n_Tmp;
	unsigned char uc_Tmp[256];
	
	memset(uc_Tmp,1,256);

	for(i=8;i<256;i++)
	{
		uc_Tmp[i] = (uc_Tmp[i-1]+uc_Tmp[i-3]+uc_Tmp[i-5]+uc_Tmp[i-8])%2;
	}
	
	for(i=0;i<32;i++)
	{
		for(j=0;j<8;j++)
		{
			//先读入的放在字节低位
			puc_RandomCode[i] = 2*puc_RandomCode[i]+uc_Tmp[i*8+(7-j)];
		}
	}	
}
