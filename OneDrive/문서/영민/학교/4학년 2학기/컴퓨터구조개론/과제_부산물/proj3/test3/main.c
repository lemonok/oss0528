#include <stdio.h>
#include <stdlib.h>

int main()
{
  FILE *in;
  char ch;


  // test.bin �̶�� ���������� �б� �������� ����
  if ( (in = fopen("C:/Users/lemon/Desktop/test6_d.dat", "rb")) == NULL ) {
    fputs("���� ���� ����!", stderr);
    exit(1);
  }

  //int num;
  //in=fopen("C:/Users/lemon/Desktop/test1_t.dat", "rb");
  //fread(&num,sizeof(num),1,in);
  //printf("%x \n",num);


  //���� ����, 1����Ʈ�� �о�, ���� ���
  while ( (ch = fgetc(in)) != EOF ) {
    printf("%x ", ch);
  }

  fclose(in);
  return 0;
}
