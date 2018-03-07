/*只编码了223个字符，未考虑多于223的情况*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_STRING 256
#define n 255  //code length
#define MESSAGE_LENGTH 223  //k
#define m 8 //n+1=2^m

int t=(n-MESSAGE_LENGTH)/2;

int a[256][8]={0};//伽罗华域元素
int alpha[256]={0};//decimalize a
int genpolycoef[33];//生成多项式系数，n-k+1个
int rs_out[n]={0};//rs编码的输出

int gen_gf();//生成伽罗华域的元素，包括二进制和十进制
int index_of(int val);//在伽罗华域上求已知值的指数
int alpha_of(int index);
int gf_add(int index1,int index2);//伽罗华域加法，其减法与加法相同，均为异或计算
int gf_mul(int index1,int index2);//伽罗华域乘法，求余
int gf_rev(int index);//求逆
int rs_code(int message[MESSAGE_LENGTH]);
int rs_decode(int *r,int *out);//r为接收到的编码，out存放译码结果
int tri(int val);//三角基
int buffer_right_move(int *buffer , int buf_len,int count);//数组右移

int main()
{
	int i;
	
	//读入k字节信息码
	// FILE *fp;
	// fp=fopen("message","r");
	// int message[MESSAGE_LENGTH],rs_out[n];
	// for(i=0;i<MESSAGE_LENGTH;i++)
	// {
	// 	message[i]=fgetc(fp);
	// 	printf("%d ",rs_out[i]);//
	// 	rs_out[i]=message[i];
	// }
	// fclose(fp);

	// for(i=0;i<255;i++)
	// {
	// 	printf("%d ",rs_out[i]);
	// }
	// printf("\n");

	gen_gf();

	// //计算生成多项式系数，并与matlab生成结果对比，验证生成元素及加法乘法运算无误
	// int temp,index; 
	// temp=112;
	// for(index=113;index<=143;index++)
	// {
	// 	temp=gf_add(temp,index);//累加计算x^31的系数
	// 	temp=gf_mul(temp,index);//累乘计算x^0的系数
	// }
	// printf("%d\n",alpha[temp+1]);

	//源码
	int message[MESSAGE_LENGTH];
	for(i=0;i<MESSAGE_LENGTH;i++)
	{
		message[i]=index_of(i+1);
		rs_out[i]=i+1;
	}

	//RS编码
	rs_code(message);//输入为index

	// for(i=0;i<n;i++)
	// {
	// 	//rs_out[i]=tri(rs_out[i]);// 转化为三角基
	// 	printf("%d ",rs_out[i]);
	// }
	// printf("\n");

	//加入误差
	rs_out[8]=2;
	rs_out[16]=2;

	//RS译码
	int *rs_decode_out;
	rs_decode_out=(int *)calloc(223,sizeof(int));//接收译码结果
	rs_decode(rs_out,rs_decode_out);
	
	//打印译码结果
	for(i=0;i<223;i++)
	{
		printf("%d ",rs_decode_out[i]);
	}
	printf("\n");
	
	free(rs_decode_out);

	return 0;
}

int gen_gf()
{
	int i,j,k;
	int p[m]={1,1,1,0,0,0,0,1};//本原多项式x^8=x^7+x^2+x+1
	
	for(i=0;i<m;i++)
	{
		a[i+1][m-i-1]=1;
	}
	for(i=m;i<n;i++)
	{
		for(k=0;k<m;k++)
		{
			if(p[k]==1)
			{
				for(j=0;j<m;j++)
				{
					a[i+1][j]^=a[i+1-(m-k)][j];	
				}
			}
		}	
	}
	
	// //print binary
	// for(i=0;i<n+1;i++)
	// {
	// 	printf("%d ",i-1);
	// 	for(j=0;j<m;j+ +)
	// 	{
	// 		printf("%d",a[i][j]);
	// 	}
	// 	printf("\n");
	// }

	//decimal
	for(i=0;i<=n;i++)
	{
		for(j=0;j<m;j++)
		{
			alpha[i]+=a[i][j]*pow(2,m-1-j);
		}
		//printf("%d %d\n ",i-1,alpha[i]);
	}

	//从matlab中得到生成多项式系数rsgenpoly(255,223,391,112)
	genpolycoef[0] = 1;
	genpolycoef[1] = 69;
	genpolycoef[2] = 251;
	genpolycoef[3] = 239;
	genpolycoef[4] = 38;
	genpolycoef[5] = 105;
	genpolycoef[6] = 20;
	genpolycoef[7] = 238;
	genpolycoef[8] = 218;
	genpolycoef[9] = 114;
	genpolycoef[10] = 188;
	genpolycoef[11] = 229;
	genpolycoef[12] = 112;
	genpolycoef[13] = 139;
	genpolycoef[14] = 85;
	genpolycoef[15] = 64;
	genpolycoef[16] = 213;
	genpolycoef[17] = 64;
	genpolycoef[18] = 85;
	genpolycoef[19] = 139;
	genpolycoef[20] = 112;
	genpolycoef[21] = 229;
	genpolycoef[22] = 188;
	genpolycoef[23] = 114;
	genpolycoef[24] = 218;
	genpolycoef[25] = 238;
	genpolycoef[26] = 20;
	genpolycoef[27] = 105;
	genpolycoef[28] = 38;
	genpolycoef[29] = 239;
	genpolycoef[30] = 251;
	genpolycoef[31] = 69;
	genpolycoef[32] = 1;

	return 0;
}

int index_of(int val)
{

	int index;
	if(val>=0&&val<=n)//2^m-1
	{
		for(int i=0;i<=n;i++)
		{
			if(alpha[i]==val)
			{
				index=i-1;
				break;
			}
		}
	}
	else 
	{
		printf("%d isn't in gf(2^%d)\n", val,m);
	}

	return index;
}

int alpha_of(int index)
{
	int val;
	if(index>=-1&&index<n)
	{
		val=alpha[index+1];
	}
	else
	{
		printf("alpha_of error\n");
		exit(-1);
	}

	return val;
}

int gf_add(int index1,int index2)
{
	int temp[m];
	int val=0;
	int i;

	//判断输入是否合法
	//待修改，未考虑负指，此处-1次指0元素
	if(index1<-1)
	{
		printf("index1 < 0\n");
		exit(-1);
	}
	else if(index2<-1)
	{
		printf("index2 < 0\n");
		exit(-1);
	}

	if(index1==-1)
	{
		return index2;
	}
	else if(index2==-1)
	{
		return index1;
	}

	index1=index1%n;
	index2=index2%n;
	
	//伽罗华域的加法
	for(i=0;i<m;i++)
	{
		temp[i]=a[index1+1][i]^a[index2+1][i];
	}	

	for(i=0;i<m;i++)
	{
		val+=temp[i]*pow(2,m-1-i);
	}	
	//printf("%d\n", val);
	return index_of(val);
}

int gf_mul(int index1,int index2)
{
	int index;
	
	//判断输入是否合法
	//待修改，未考虑负指，此处-1次指0元素
	if(index1<-1)
	{
		printf("index1 < -1");
		exit(-1);
	}
	else if(index2<-1)
	{
		printf("index2 < -1");
		exit(-1);
	}
	
	//伽罗华域的乘法
	if(index1==-1||index2==-1)
		index=-1;
	else
		index=(index1+index2)%n;//2^m-1更确切
	//求余是将结果降到(0,2^m-1)内

	return index;
}

//伽罗华域下的求倒，转化除法用
//有待提高
int gf_rev(int index)
{
	return (n-index)%n;
}

int rs_code(int message[])
{
	int i,expn,temp,step;

	for(i=0;i<=32;i++)
	{
		genpolycoef[i]=index_of(genpolycoef[i]);
	}

	for(expn=254,step=1;expn>63;expn--,step++)
	{
		temp=message[0];
		//printf("%d\n",step);
		for(i=0;i<32;i++)
		{
			message[i]=gf_add(gf_mul(genpolycoef[i+1],temp),message[i+1]);
		}
		while(i<MESSAGE_LENGTH-step)
		{
			message[i]=message[i+1];
			i++;
		}
	}
	for(;expn>31;expn--)
	{
		temp=message[0];
		for(i=0;i<31;i++)
		{
			message[i]=gf_add(gf_mul(genpolycoef[i+1],temp),message[i+1]);
		}
		message[31]=gf_mul(genpolycoef[32],temp);
	}

	//校验码
	for(i=0;i<32;i++)
	{
		rs_out[MESSAGE_LENGTH+i]=alpha_of(message[i]);
	}

	return 0;
}

int tri(int val)
{
	int index;
	int beta[m]={0};
	index=index_of(val);
	beta[0]=a[index+1][0];
	beta[1]=a[index+1][0]^a[index+1][1];
	beta[2]=a[index+1][0]^a[index+1][1]^a[index+1][2];
	beta[3]=a[index+1][0]^a[index+1][1]^a[index+1][2]^a[index+1][3];
	beta[4]=a[index+1][0]^a[index+1][1]^a[index+1][2]^a[index+1][3]^a[index+1][4];
	beta[5]=a[index+1][0]^a[index+1][1]^a[index+1][2]^a[index+1][3]^a[index+1][4]^a[index+1][5];
	beta[6]=a[index+1][1]^a[index+1][2]^a[index+1][3]^a[index+1][4]^a[index+1][5]^a[index+1][6];
	beta[7]=a[index+1][2]^a[index+1][3]^a[index+1][4]^a[index+1][5]^a[index+1][6]^a[index+1][7];
	val=0;
	for(int i=0;i<8;i++)
	{
		val+=beta[i]*pow(2,7-i);
	}

	return val;
}

int rs_decode(int *r,int *out)
{
	//缺：判断r、out的存储空间是否正确
	//r:255; out:223

	int s[32];//伴随多项式
	//设初值为0,index为-1
	for(int i=0;i<32;i++)
	{
		s[i]=-1;
	}

	for(int i=0;i<2*t;i++)
	{
		for (int j=0;j<n;j++)
		{
			s[i]=gf_add(gf_mul(index_of(r[j]),(i+112)*j),s[i]);
		}
	}

	//不包含错误的情况下，s应等于0,本程序s为index,所以应输出-1;
	//在包含错误的情况下，有s_j=e(alpha^j)，可辅助检验伴随式正误
	//《数字通信》P349-350
	//通过s可以判断是否包含错误，若不包含错误则输出了
	for(int i=0;i<32;i++)
	{
		if(s[i]!=-1)
		{
			break;
		}
		if(i==31)
		{
			for(int j=0;j<223;j++)
			{
				out[j]=r[j];
			}
			return 0;
		}
	}

	//由Berlekamp-Massey Algorithm求错误位置多项式
	//《RS纠错编码原理及其实现方法》
	//初始化
	int Lambda[34][33]={0};//alpha
	Lambda[0][0]=1;
	Lambda[1][0]=1;
	int D[34]={0,0};//D为Lambda的阶次
	int d[34];
	for(int i=0;i<34;i++)
	{
		d[i]=-1;
	}
	d[0]=index_of(1);
	d[1]=s[0];//s1,索引为0,s0不参与运算

	int i_max=-1;//j行之前第i行，使i-D(i)最大且d_i!=0
	int i_D=-1;//i_max的i-D(i)
	for(int j=0;j<32;j++)
	{
		//计算Lambda、D
		if(d[j+1]==-1)//此处为index,是-1
		{
			for(int i=0;i<33;i++)
			{
				Lambda[j+2][i]=Lambda[j+1][i];
			}
			D[j+2]=D[j+1];
		}
		else
		{
			int temp[33];
			
			for(int i=0;i<33;i++)
			{
				temp[i]=gf_mul(gf_mul(d[j+1],gf_rev(d[i_max+1])),index_of(Lambda[i_max+1][i]));
			}
			
			buffer_right_move(temp,33,j-i_max);
			
			for(int i=0;i<33;i++)
			{
				Lambda[j+2][i]=alpha_of(gf_add(index_of(Lambda[j+1][i]),temp[i]));//
			}

			D[j+2]=D[i_max+1]+j-i_max;//
			
			if(j-D[j+1]>i_D)
			{
				i_max=j;
				i_D=j-D[j+1];
			}
		}

		//计算d
		if(j==31)
			break;
		d[j+2]=s[j+1];
		for(int i=1;i<D[j+2]+1;i++)
		{
			d[j+2]=gf_add(d[j+2],gf_mul(index_of(Lambda[j+2][i]),s[j+1-i]));
		}
	}

	//chien搜索法，求错误多项式的根即错误位置
	int count=0;
	int error[16];
	for(int i=n;i>0;i--)
	{
		int result=-1;
		for(int j=0;j<33;j++)
		{
			result=gf_add(result,gf_mul(i*j,index_of(Lambda[33][j])));
		}
		if(result==-1)
		{
			if(count==16)
			{
				printf("错误太多超出纠错能力\n");
				exit(-1);
			}
			error[count]=n-i;//错误位置
			printf("%d\n",error[count]);
			count++;
		}
	}

	//Forney算法计算错误图样
	int Omega[32];//index
	for(int i=0;i<32;i++)
	{
		Omega[i]=-1;
	}
	//Lambda(x)*s(x)(mod x^32)
	for(int i=0;i<32;i++)
	{
		for(int j=0;j<i+1;j++)
		{
			Omega[i]=gf_add(Omega[i],gf_mul(index_of(Lambda[33][j]),s[i-j]));
		}//Omega为升序
	}

	int *e;
	e=(int *)calloc(count,sizeof(int));
	
	for(int i=0;i<count;i++)
	{
		int e_u=-1,e_d=-1;//错误值计算公式的分子分母，初始化为index=-1
		for(int j=0;j<32;j++)
		{
			e_u=gf_add(e_u,gf_mul(Omega[j],(n-error[i])*j));
		}
		for(int j=1;j<=count;j+=2)
		{
			e_d=gf_add(e_d,gf_mul(index_of(Lambda[33][j]),(n-error[i])*(j-1)));
		}
		e[i]=gf_mul(e_u,gf_rev(e_d));
	}

	//r+e
	for(int i=0;i<223;i++)
	{
		out[i]=r[i];
	}
	for(int i=0;i<count;i++)
	{
		out[error[i]]=alpha_of(gf_add(index_of(out[error[i]]),e[i]));
	}

	free(e);

	return 0;
}

int buffer_right_move(int *buffer , int buf_len,int count)  
{   
    for(int j=0;j<count;j++)
    {
        char tmp = buffer[buf_len - 1];  
        for(int i = buf_len ; i > 0 ; i--)  
        {  
            buffer[i] = buffer[i-1] ;   
        }  
        buffer[0] = tmp ;
    } 

    return 0; 
}





