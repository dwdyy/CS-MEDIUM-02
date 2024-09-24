#include<stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Bignum.c"
#define uint unsigned int 
Bigint _1e32 ;
Bigint _1 ;
// g++ -o n numPart1.c
typedef struct {
    bool less0;
    Bigint integer;
    Bigint frac;
} PointFixedNum;
PointFixedNum PointFixedNum_Add(PointFixedNum a,PointFixedNum b);
PointFixedNum PointFixedNum_Dec(PointFixedNum a,PointFixedNum b);
void PointFixedNum_init(PointFixedNum *num,char *s){
    init(&num->integer);
    init(&num->frac);
    num->less0 = 0 ;
    int n = strlen(s);
    if(s[0] == '-') {
        num->less0 = 1;
        for(int i=0;i<n;i++) s[i] = s[i+1]; // 删掉符号
    }
    n = strlen(s);
    if(s[n-1] == 'B') { // 二进制
        int pos_point = n-1; // 小数点位置
        for(int i=0;i<n;i++) if(s[i] == '.') pos_point = i;
        for(int i=0,j=pos_point-1;j>=0;j--,i++) // 整数
        if(s[j]=='1')num->integer = Add(num->integer,Get_Bigint(1ll<<i));
        int cnt = 0 ; // 小数有多少位
        for(int j=n-2;j>pos_point;j--,cnt++) // 小数
        if(s[j]=='1')num->frac = Add(num->frac ,Get_Bigint(1ll<<cnt));
        num->frac = Mul(div(_1e32 , Get_Bigint(1ll<<cnt)),num->frac);
    }
    if(s[n-1] == 'D') { // 十进制
        int pos_point = n-1; // 小数点位置
        for(int i=0;i<n;i++) if(s[i] == '.') pos_point = i;
        for(int j=0;j<pos_point;j++) // 整数
        num->integer = Add(Mul(num->integer ,Get_10k(1)) , Get_Bigint(s[j] -'0'));
        int cnt = 0 ; // 记录有多少位小数
        for(int j=pos_point+1;j<n-1;j++)
        num->frac = Add(Mul(num->frac ,Get_10k(1)) , Get_Bigint(s[j]-'0')),cnt++;
        while(cnt<32) num->frac = Mul(num->frac ,Get_10k(1)),cnt++; // 不够9位补到9位
    }
}
uint tmp[35];
void PointFixedNum_print(PointFixedNum *num){
    if(num->frac.size==0 && num->integer.size==0){
        printf("0\n");
        return ;
    }
    if(num->less0 ) printf("-");
    print(&num->integer);
    if(num->frac.size!=0){
        printf(".");
        if(num->frac.size < 32) for(int i=num->frac.size;i<32;i++) printf("0");
        print_no_0(&num->frac);
    }
    printf("D\n");
}

PointFixedNum PointFixedNum_inv(PointFixedNum a){
    PointFixedNum b = a;
    b.less0 ^= 1;
    return b;
}

bool PointFixedNum_Check_le(PointFixedNum a,PointFixedNum b){ // a<b
    if(a.less0 && b.less0) {
        if(Not_equal(a.integer,b.integer)) return Check_le(b.integer,a.integer);
        return Check_le(b.frac,a.frac);
    }
    if(a.less0) return 1;
    if(b.less0) return 0;
    if(Not_equal(a.integer,b.integer)) return Check_le(a.integer,b.integer);
    return Check_le(a.frac,b.frac);
} 

PointFixedNum PointFixedNum_Add(PointFixedNum a,PointFixedNum b){
    if(a.less0 && b.less0) return PointFixedNum_inv(PointFixedNum_Add(PointFixedNum_inv(a),PointFixedNum_inv(b)));
    if(a.less0) return PointFixedNum_Dec(b,PointFixedNum_inv(a));
    if(b.less0) return PointFixedNum_Dec(a,PointFixedNum_inv(b));
    PointFixedNum c = a;
    c.integer = Add(c.integer,b.integer);
    c.frac = Add(c.frac,b.frac);
    if(Check_lef(_1e32,c.frac)) {
        c.frac = Del(c.frac,_1e32);
        c.integer =Add(c.integer,_1);
    }
    if(c.integer.size > 32){
        printf("整数已超过32位\n");
    }
    return c;
}



PointFixedNum PointFixedNum_Dec(PointFixedNum a,PointFixedNum b){
    if(a.less0) return PointFixedNum_inv(PointFixedNum_Add(PointFixedNum_inv(a),b));
    if(b.less0) return PointFixedNum_Add(a,PointFixedNum_inv(b));
    if(PointFixedNum_Check_le(a,b)) return PointFixedNum_inv(PointFixedNum_Dec(b,a));
    PointFixedNum c = a;

    c.integer = Del(c.integer,b.integer);
    if(Check_le(c.frac,b.frac)) {
        c.integer = Del(c.integer,_1);
        c.frac = Add(c.frac,_1e32);
    }
    c.frac=Del(c.frac,b.frac);
    return c;
}

// 11.11D * 11.11D
int tmp_PFN_MUl[1005];

PointFixedNum PointFixedNum_Mul(PointFixedNum a,PointFixedNum b){
    PointFixedNum c = a;
    init(&c.frac);
    init(&c.integer);
    c.less0 = a.less0 ^ b.less0;
    for(int i=0;i<=128;i++) tmp_PFN_MUl[i] = 0;
    if(a.integer.size + b.integer.size -1 > 32)  {printf("溢出");}
    for(int i=0;i<a.integer.size;i++) {
        for(int j=0;j<b.integer.size;j++)
            tmp_PFN_MUl[i+j+64] += (a.integer.num[i] -'0') * (b.integer.num[j] -'0');
        for(int j=0;j<b.frac.size;j++)
            tmp_PFN_MUl[i+j+32] += (a.integer.num[i] -'0') * (b.frac.num[j] -'0');
    }
    for(int i=0;i<a.frac.size;i++) {
        for(int j=0;j<b.integer.size;j++)
            tmp_PFN_MUl[i+j+32] += (a.frac.num[i] -'0') * (b.integer.num[j] -'0');
        for(int j=0;j<b.frac.size;j++)
            tmp_PFN_MUl[i+j] += (a.frac.num[i] -'0') * (b.frac.num[j] -'0');
    }
    for(int i=0;i<=128;i++){
        tmp_PFN_MUl[i+1] += tmp_PFN_MUl[i] / 10;
        tmp_PFN_MUl[i] %=10;
        if(i<32 && tmp_PFN_MUl[i]) printf("error:小数已超过32位\n");
    }
    c.frac.size = 32;c.integer.size = 32;
    for(int i=0;i<c.frac.size;i++) c.frac.num[i] = tmp_PFN_MUl[i+32] + '0';
    for(int i=0;i<c.integer.size;i++) c.integer.num[i] = tmp_PFN_MUl[i+64] + '0';
    solve0(&c.frac);
    solve0(&c.integer);
    return c;
}


int main(){

    _1e32 = Get_10k(32);
    _1 = Get_Bigint(1);
    // FILE *fptr = fopen("CS_M_02.txt","r");
    // char s[1004],t[1004];
    // fgets(t,N,fptr);t[strlen(t)-2] = 0;  
    // PointFixedNum now ;
    // PointFixedNum_init(&now,t); 

    // while(fgets(t,N,fptr)){
        
    //     strcpy(s,t+2);s[strlen(s)-2] = '\0';  
    //     PointFixedNum nxt ;
    //     PointFixedNum_init(&nxt,s);
    //     char opt = t[0];
    //     if(opt=='-') now = PointFixedNum_Dec(now,nxt);
    //     else if(opt=='+') now = PointFixedNum_Add(now,nxt);
    //     else if(opt=='*') now = PointFixedNum_Mul(now,nxt);PointFixedNum_print(&now);
    // }
    

    while(1){
    char s[1004],t[1004];
    char op ;
    scanf("%s",s);
    PointFixedNum dwd;
    PointFixedNum_init(&dwd,s);
    PointFixedNum_print(&dwd);
        
 }
}