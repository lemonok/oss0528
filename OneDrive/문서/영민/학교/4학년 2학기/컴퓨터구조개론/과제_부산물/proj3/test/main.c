#include <stdio.h>

int main()
{
    char a[10]="12";
    a[0]='3';

    //printf("%s",a);

    //char array[2]={"1234","5678"};
    //printf("%s",array[0]);
    //char arr=array[0];

    //printf("%s",arr);

    //if(!strcmp(arr,"12")){
    //    r3[2]='1';
   //}
    char *s1 = "283";   // "283"�� ���ڿ�
    int num1;

    num1 = atoi(s1);        // ���ڿ��� ������ ��ȯ�Ͽ� num1�� �Ҵ�

    //printf("%d\n", num1);

  FILE *in;
  char ch;


  // test.bin �̶�� ���������� �б� �������� ����
  if ( (in = fopen("C:/Users/lemon/Desktop/test1_t.dat", "rb")) == NULL ) {
    fputs("���� ���� ����!", stderr);
    exit(1);
  }


  // ���� ����, 1����Ʈ�� �о�, ���� ���
  while ( (ch = fgetc(in)) != EOF ) {
    printf("%x ", ch);
  }


  fclose(in);


    return 0;
}
