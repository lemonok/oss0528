#include <stdio.h>
#include <stdlib.h>

int delta; char calm;


int main() {
    char come, go; int hollow;
    hollow = come = go = 0;
    while( hollow < delta )  {
        calm = come + go;
        hollow = calm - delta + come + 5;
    }
    printf(hollow);
}
