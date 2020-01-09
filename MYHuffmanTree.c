#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 10000       //文件字符最大数
#define codeMax 30      //编码最大位数 

typedef struct {
	char ch;     //指向字符 
	char *code;  //指向编码表 
	int weight;  //权值 
	int parent,LChild,RChild;
}HTNode,*HuffmanTree;

struct {
	char ch;
	int num;
}binary[60];

char text[MAX];      //存储文本
int textlen;         // 文本长度

void menu();  //显示菜单 
HuffmanTree CreateHuffmanTree(int num);  //创建哈弗曼树 
void selectMin(HuffmanTree HT,int len,int*s1,int*s2);     //选取权值最小的两个节点 
void ReadFile(int *key);        //读取文件 
void CreateHuffmanCode(HuffmanTree HT,int n);     //建立哈弗曼编码 
void CodeFile(HuffmanTree HT,int n);      //输出编码完成后的文件 
void TranslationFile(HuffmanTree HT,int n);    //对文件进行译码 
void PrintForm(HuffmanTree HT,int n);            //输出编码表 

int main(){
	system("color f0");
	menu(); 
	return 0;
}

void menu(){
	int n,choice;  //字符总个数,选择参数 
	HuffmanTree HT;  //哈弗曼树 
	while(1){
		system("cls");
		printf("\n\n\n\n\n");
		printf("\t|--------------------欢迎使用哈弗曼编码译码器----------------------|\n");
		printf("\t\t\n");
		printf("\t\t\t\t1.对原始文件进行编码\n\n");
		printf("\t\t\t\t2.导出编码文件\n\n");
		printf("\t\t\t\t3.对文件进行译码并打印\n\n");
		printf("\t\t\t\t4.支持查看编码表\n\n");
		printf("\t\t\t\t5.退出使用\n\n");
		printf("\t|------------------------------------------------------------------|\n");
		printf("\t\t\t请输出您想要进行的操作（1-5）");
		while(scanf("%d",&choice)!=1 || choice < 1 || choice > 5){
			printf("\t\t输入有误，请输入正确选项！！！");
			fflush(stdin); 
		}
		switch(choice){
			case 1:
				ReadFile(&n); 
				HT=CreateHuffmanTree(n); //建立哈弗曼树
				CreateHuffmanCode(HT,n);   //创建哈弗曼编码 
				break;
			case 2:
				CodeFile(HT,n);    //导出编码文件 
				break; 
			case 3:
				TranslationFile(HT,n); //译码 
				break;
			case 4:
				PrintForm(HT,n);      //查看编码表 
				break;
			case 5:
				printf("\t\t\t谢谢使用");
				exit(0);
				break; 
		}
	}
}

void ReadFile(int *key){
	int i ,flag ,j;
	*key = 0;
	char fileName[20]; 
	for(i=0;i<60;i++){
		binary[i].ch='\0';
		binary[i].num=0;
	}
	printf("\n\t\t\t请输入文件名：");
	scanf("%s",fileName);
	FILE *fp = fopen(fileName,"r+");
	if(fp==NULL){
		printf("文件打开失败！\n");
		exit(1);
	}
	
	fgets(text,MAX,fp);
	printf("\n\t\t\t%s\n\n",text);
	for(i=0;i<MAX;i++){
		if(text[i]=='\0'){
			textlen=i;
			break;
		}
	}
	for(i=0;i<textlen;i++,flag=0){
		for(j=0;j< (*key);j++){
			if(binary[j].ch == text[i]){
				binary[j].num++;
				flag=1;
			}
		}
		if(flag == 0){
			binary[*key].ch = text[i];
			binary[*key].num += 1;
			(*key)++;
		}
	}
	printf("\t\t\t文件共有%d种字符，文件总字符数为%d",*key,textlen);
	fclose(fp);
}

void selectMin(HuffmanTree HT,int len,int *s1,int *s2){
	int i ,min=32766;
	for(i=1;i<=len;i++){
		if(HT[i].parent==0){
			if(HT[i].weight < min){
				min = HT[i].weight;
				*s1=i;
			}
		}
	}
	min=32766; 
	for(i=1;i<=len;i++){
		if(i==(*s1)){
			continue;
		}		
		if(HT[i].parent==0){
			if(HT[i].weight < min){
				min = HT[i].weight;
				*s2=i;
			}
		}
	}
}

HuffmanTree CreateHuffmanTree(int num){
	HuffmanTree HT;	
	int n = num;
	int m,i;
	int s1=0,s2=0;
	if(n<=1){
		return NULL;
	}
	m = 2*n-1;
	HT = (HuffmanTree)malloc(sizeof(HTNode)*(m+1));
	
	for(i=1;i<=n;i++){
		HT[i].parent=0;
		HT[i].LChild=0;
		HT[i].RChild=0;
		HT[i].ch=binary[i-1].ch;
		HT[i].weight=binary[i-1].num;
		HT[i].code='\0';
	}
	for(i=n+1;i<=m;i++){
		HT[i].parent=0;
		HT[i].LChild=0;
		HT[i].RChild=0;
		HT[i].ch='\0';
	}
	for(i=n+1;i<=m;i++){
		selectMin(HT,i-1,&s1,&s2);
		HT[i].weight=HT[s1].weight+HT[s2].weight;
		HT[i].LChild=s1;
		HT[i].RChild=s2;
		HT[s1].parent=i;
		HT[s2].parent=i;
	}
	printf("\n\n\t\t\t哈弗曼树的建立已完成\n"); 
	return HT;
}

void CreateHuffmanCode(HuffmanTree HT,int n){
	int i,start,c,p;
	char *cd = (char*)malloc(sizeof(char)*codeMax);
	cd[codeMax-1]='\0';
	for(i=1;i<=n;i++){
		start=codeMax-1;
		c=i;
		p=HT[i].parent;
		while(p!=0){
			--start;
			if(HT[p].LChild==c){
				cd[start]='0';
			}else{
				cd[start]='1';
			}
			c=p;
			p=HT[p].parent;
		}
		HT[i].code=(char *)malloc(sizeof(char)*(codeMax-start));
		strcpy(HT[i].code,&cd[start]);
	}
	free(cd);
	printf("\n\t\t\t哈弗曼编码已完成\n\n\t\t\t"); 
	getch();
}

void CodeFile(HuffmanTree HT,int n){
	int i,j;
	char fileName[20];
	printf("\n\t\t\t请输入编码输出文件名(格式：*.txt)：");
	scanf("%s",fileName);
	FILE *fp = fopen(fileName,"w");
	if(fp==NULL){
		printf("文件建立失败");
		exit(1);
	}
	for(i=0;i<textlen;i++){
		for(j=1;j<=n;j++){
			if(text[i] == HT[j].ch){
				fprintf(fp,HT[j].code);
			}
		}
	}
	fclose(fp);
	printf("\n\t\t\t编码写入文件成功！\n\n\t\t\t"); 
	getch();
}

void PrintForm(HuffmanTree HT,int n){
	if(n==0){
		printf("\n\t\t\t哈夫曼树未建立，请先执行步骤1,2\n\t\t\t");
		getch();
		return; 
	}
	int i;
	for(i=1;i<=n;i++){
		printf("\t\t>>>%2d\tch:%c\tweight:%d\tcode:%s\n",i,HT[i].ch,HT[i].weight,HT[i].code);
	}
	getch();
} 

void TranslationFile(HuffmanTree HT,int n){
	if(n==0){
		printf("\n\t\t\t哈夫曼树未建立，请先执行步骤1,2\n\t\t\t");
		getch();
		return; 
	}
	int i=0,f;
	char fileName[20];	
	char code ,ch;    

	printf("\n\t\t\t请输入需要译码的文件名(格式：*.txt)：");
	scanf("%s",fileName);
	FILE *fp=fopen(fileName,"r");
	FILE *fp1=fopen("decode.txt","w");
	printf("\n\t\t\t");
	if(fp==NULL){
		printf("文件打开失败");
		exit(1);
	}
	while(!feof(fp)){
		code = fgetc(fp);
		if(feof(fp))
			break;
		f=2*n-1;
		while(1){
			if(code=='0'){
				f=HT[f].LChild;
			}else{
				f=HT[f].RChild;
			}
			if(HT[f].LChild==0 && HT[f].RChild==0){
				printf("%c",HT[f].ch);
				fprintf(fp1,"%c",HT[f].ch);
				break;
			}else{
				code=fgetc(fp);
			}
		}
	}
	fflush(stdin);
	printf("\n\n\t\t\t译码完成，如上。");
	printf("\n\n\t\t\t文件已保存至decode.txt");
	fclose(fp);
	fclose(fp1);
	getch();
} 
