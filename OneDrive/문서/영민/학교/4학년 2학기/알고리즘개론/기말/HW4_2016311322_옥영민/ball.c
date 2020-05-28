#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

int s, k, ku;
int a[MAX+5];

int Skku(int elem){
    int l = 1;
    int r = s;
    int ret = s+1;

    while(l <= r){
        int mid = (l + r)/2;

    if(ku <= a[mid]){
        if(mid < ret)
        ret = mid;
        r = mid - 1;
    }
    else
    l = mid + 1;
    }
    return ret;

}

int main() {
    freopen("ball.inp", "r", stdin);
    freopen("ball.out", "w", stdout);
    scanf("%d", &s);

    for(int i = 1; i <= s; i++)
        scanf("%d", &a[i]);
        //printf("%d", &a[2]);
    for(int i = 2; i <= s; i++)
        a[i] = a[i] + a[i-1];
        scanf("%d", &k);

    for(int i=1; i <= k; i++){
        scanf("%d", &ku);
        printf("%d\n", Skku(ku));
    }

    return 0;
}
