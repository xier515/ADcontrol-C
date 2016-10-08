#ifndef _SHOW_H
#define _SHOW_H

#define NULL 0
#include <stm32f4xx.h>

typedef struct _StringStruct
{
	unsigned char X;
	unsigned char X_End;
	unsigned char Y;
	char* String;
	unsigned char ChoseEn;
	void* next;
	void* last;
	void (*Switch)(int i);
}StringStruct;      //������Ҫ��ʾ�����ݡ�λ�á���������Ϣ��˫����


extern int shiwen;
extern int shidu;
extern int shiwen_s;
extern int shidu_s;
extern int Nowpage;
extern StringStruct NowStruct;

extern char shiwen1_s[]; //����
extern char shidu1_s[];  //����ʪ��
extern char shiwen2_s[]; //�趨�¶�
extern char shidu2_s[];  //�趨ʪ��
extern char qiangzhiqidong[];  //�յ�ǿ�������¶�
extern char zhouqi[];  //��ϴ����
extern char shiwaiwendu[];
extern char gongzuotianshu_s[];

extern char nian[];
extern char yue[];
extern char ri[];
extern char shi[];
extern char fen[];
extern char miao[];

void NumToString(char *st,s32 num, u8 cnt);
void PageJumpTo(int pagenum);
void PageUpdata(void);
StringStruct ChosedNext(StringStruct st);
StringStruct ChosedLast(StringStruct st);
void Display_Chosed(StringStruct st);
int Err_Init(void);
void Err_Updata(void);
#endif

