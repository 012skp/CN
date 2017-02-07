

void xor_pos(char temp_msg[], char gen_pol[], int pos){
	if(temp_msg[pos] == '0') return;
	//printf("xor_pos in: at pos = %d str = %s\n",pos,temp_msg);
	int i=0,j=0;
	for(i=0;i<strlen(gen_pol);i++){
		int n1 = ((int)temp_msg[i+pos])-48;
		int n2 = ((int)gen_pol[j])-48;
		//printf("at i = %d n1 = %d n2 = %d\n",i,n1,n2);
		if(n1 && n2) temp_msg[i+pos] = '0';
		if(n1 && !n2) temp_msg[i+pos] = '1';
		if(!n1 && n2) temp_msg[i+pos] = '1';
		if(!n1 && !n2) temp_msg[i+pos] = '0';
		j++;
	}
	//printf("xor_pos out: at pos = %d str = %s\n",pos,temp_msg);
}


void gen_reduntant_bits(char msg[], char gen_pol[]){
	//printf("generating redundantbits for: %s\n",msg);
	int pos = 0;
	char temp_msg[strlen(msg)+strlen(gen_pol)];
	strcpy(temp_msg,msg);
	for(pos = 0;pos<strlen(gen_pol)-1;pos++) strcat(temp_msg,"0\0");
	//printf("msg was = %s",msg);
	pos = 0;
	for(int i=0;i<strlen(msg);i++){ xor_pos(temp_msg,gen_pol,pos); pos++;}
	int len = strlen(msg);
	strcat(msg,temp_msg+len);
	//printf("now msg = %s\n",msg);
}