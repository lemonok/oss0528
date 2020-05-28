#include <stdio.h>
#include <stdlib.h>

int main()
{
  FILE *in;
  char ch;


  // test.bin 이라는 이진파일을 읽기 전용으로 열기
  if ( (in = fopen("C:/Users/lemon/Desktop/test6_d.dat", "rb")) == NULL ) {
    fputs("파일 열기 에러!", stderr);
    exit(1);
  }

  //int num;
  //in=fopen("C:/Users/lemon/Desktop/test1_t.dat", "rb");
  //fread(&num,sizeof(num),1,in);
  //printf("%x \n",num);


  //이진 파일, 1바이트씩 읽어, 헥사로 출력
  while ( (ch = fgetc(in)) != EOF ) {
    printf("%x ", ch);
  }

  fclose(in);
  return 0;
}
