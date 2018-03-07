puc_Tmp1 = &puc_Buff1[i*255*4];
for(j=0;j<(255*4);j++)
{
	puc_Tmp1[j] = uc2_Buff[(j%4)*255+j/4];//交织

	p2[j]=buff2[(j%255)*4+j/255];//解交织
}