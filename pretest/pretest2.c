#include<stdio.h>
#include <string.h>
#define N 10005

int two_to_ten(int x){
    int ans = 0 ;int cnt = 1;
    while(x){
        if(x&1) ans+=cnt;
        cnt*=2;
        x/=10;
    }
    return ans;
}

void ten_to_two(int x){
    int s[155],cnt;
    
    while(x){
        if(x&1) s[++cnt] = 1;
        else s[++cnt] = 0;
        x>>=1;
    }
    for(int i=cnt;i>=1;i--) printf("%d",s[i]);
}

int main(){
    int x;
    scanf("%d",&x);
    printf("%d\n",two_to_ten(x));
    ten_to_two(x);
}
/*
gcc -o a a.c
*/