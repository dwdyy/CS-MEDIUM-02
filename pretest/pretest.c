#include<stdio.h>
#include <string.h>
#define N 10005

FILE *fptr;

double ten_to_ten(char *s){
    double ans = 0 ;
    int n = strlen(s);int j;
    for(int i=0;i<n-1;i++){
        if(s[i] =='.') {
            j = i ;
            break;
        }
    }
    double now = 1;
    for(int i=j-1;i>=0;i--){
        ans +=now * (s[i]-'0');
        now *=10;
    }
    now =  0.1;
    for(int i=j+1;i<n-1;i++){
        ans += now * (s[i]-'0');
        now /=10;
    }
    return ans;
}

double two_to_ten(char *s){
    double ans = 0 ;
    int n = strlen(s);int j;
    for(int i=0;i<n-1;i++){
        if(s[i] =='.') {
            j = i ;
            break;
        }
    }
    double now = 1;
    for(int i=j-1;i>=0;i--){
        ans +=now * (s[i]-'0');
        now *=2;
    }
    now =  0.5;
    for(int i=j+1;i<n-1;i++){
        ans += now * (s[i]-'0');
        now /=2;
    }
    return ans;
}

char t[N],s[N];

int main(){
    fptr = fopen("CS_M_02.txt","r");
    while(fgets(t,N,fptr)){
        
        
        strcpy(s,t+2);int n = strlen(s);
        s[n-2] = '\0';n = strlen(s);
        if(s[n-1] == 'B') printf("%lf",two_to_ten(s));
        else printf("%lf",ten_to_ten(s));
        printf("\n");
    }
}
/*
gcc -o a a.c
*/