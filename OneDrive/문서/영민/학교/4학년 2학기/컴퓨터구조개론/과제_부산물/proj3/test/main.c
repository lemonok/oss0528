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
    char *s1 = "283";   // "283"은 문자열
    int num1;

    num1 = atoi(s1);        // 문자열을 정수로 변환하여 num1에 할당

    //printf("%d\n", num1);

  FILE *in;
  char ch;


  // test.bin 이라는 이진파일을 읽기 전용으로 열기
  if ( (in = fopen("C:/Users/lemon/Desktop/test1_t.dat", "rb")) == NULL ) {
    fputs("파일 열기 에러!", stderr);
    exit(1);
  }


  // 이진 파일, 1바이트씩 읽어, 헥사로 출력
  while ( (ch = fgetc(in)) != EOF ) {
    printf("%x ", ch);
  }


  fclose(in);


    return 0;
}
