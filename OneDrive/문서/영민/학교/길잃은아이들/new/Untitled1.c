#include <stdio.h>
typedef struct
{
    char name[10];
    int scores[3];
} STUDENT;

int main()
{
    STUDENT s1 = { "Alice", 80, 70, 60 };
    STUDENT* s2 = &s1;

    printf("%s\n", s2->name);
    for (int i = 0; i < 3; i++)
        printf("%d\n", s2->scores[i]);
    return 0;
}
