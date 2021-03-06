#include "gen_check_sum.h"
#include <string.h>

void add(char str1[], char str2[], char str3[]){
	int i;
	int carry = 0;
	for(i =3;i>=0;i--){
		int n1 = (int)str1[i] - 48;
		int n2 = (int)str2[i] - 48;
		int sum = n1+n2+carry;
		str3[i] = (char)((sum&1)+48);
	}
	if(carry){
		char one[5] = "0001";
		char ans[5];
		add(str3,one,ans);
		strcpy(str3,ans);
	}
}

void gen_check_sum(char str[]){
	char str1[5],str2[5],str3[5];
	strncpy(str1,str,4);
	strncpy(str2,str+4,4);
	add(str1,str2,str3);
	strncpy(str+8,str3,4);
}