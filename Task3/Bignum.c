#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#define N 34
int Tmp_mul[N];
void swap(char *a,char *b){char tmp = *a;*a = *b;*b = tmp;}
int max(int a, int b) {  
    return (a > b) ? a : b;  
}  

typedef struct Bigint{ 
    char num[N];
    int size ;
    bool less0; // 正负
}Bigint;
void init(Bigint *a){
    a->size = 0;a->less0 = false;
    memset(a->num,'0',sizeof a->num);
}
Bigint Add(Bigint a,Bigint b);
Bigint Del(Bigint a,Bigint b);
void reverse(Bigint *a){
    for(int i=0;i<a->size/2;i++){
        swap(&a->num[i],&a->num[a->size-i-1]);
    }
}

void print_no_0(Bigint *b){
    if(b->less0 && b->size!=0) printf("-");

    int flag = 0;
    for(;flag<b->size;flag++) if(b->num[flag]!='0') break;
    for(int i=b->size-1;i>=flag;i--) printf("%c",b->num[i]);
    if(b->size==0) printf("0");
}

void print(Bigint *b){
    if(b->less0 && b->size!=0) printf("-");
    for(int i=b->size-1;i>=0;i--) printf("%c",b->num[i]);
    if(b->size==0) printf("0");
}
Bigint inv(Bigint a){ // 取负号
    a.less0 ^=1;
    return a;
}

void solve0(Bigint *c){
    while(c->size&&c->num[c->size-1] == '0') c->size --;
    if(c->size==0) c->less0 = false;
}
bool Check_le(Bigint a,Bigint b){// 判断 a < b
    int flag = 0,ans = 0;
    if(a.less0 && b.less0) {
        return Check_le(inv(b),inv(a));
    }
    else if(a.less0) return 1;
    else if(b.less0) return 0;
    if(a.size !=b.size) return a.size < b.size;
    for(int i=a.size-1;i>=0;i--){
        if(a.num[i] != b.num[i])
        return a.num[i] < b.num[i];
    }
    return false;
}

bool Not_equal(Bigint a,Bigint b){
    if(a.less0 != b.less0 || a.size != b.size ) return true;
    for(int i=0;i<a.size;i++) if(a.num[i] != b.num[i]) return true;
    return false;
}

bool Check_lef(Bigint a,Bigint b){// 判断 a <= b
    int flag = 0,ans = 0;
    if(a.less0 && b.less0) {
        return Check_le(inv(b),inv(a));
    }
    else if(a.less0) return 1;
    else if(b.less0) return 0;
    if(a.size !=b.size) return a.size < b.size;
    for(int i=a.size-1;i>=0;i--){
        if(a.num[i] != b.num[i])
        return a.num[i] < b.num[i];
    }
    return true;
}
Bigint Del(Bigint a,Bigint b){
    if(a.less0)return inv(Add(inv(a),b));
    if(b.less0)return Add(a,inv(b));

    if(Check_le(a,b)) return inv(Del(b,a));
    Bigint c = a;
    c.size = max(a.size,b.size);
    for(int i=0;i<b.size;i++){
        c.num[i] = c.num[i] - b.num[i] + '0';
    }
    for(int i=0;i<c.size;i++){
        if(c.num[i] < '0') {
            c.num[i] += 10;
            c.num[i+1] -= 1;
        }
    }
    solve0(&c);
    return c;
}
Bigint Add(Bigint a,Bigint b){ // 实现加法
    if(a.less0 && b.less0){
        // a<0 && b <0 
        //- (-a) + (-b)
        Bigint c = Add(inv(a),inv(b));
        c.less0^=1;
        return c;
    }else if(a.less0) {
            // a<0 : b - (-a)
            Bigint c = Del(b,inv(a));
            return c;
        }
    else if(b.less0){// b<0
            Bigint c = Del(a,inv(b));
            return c;
    }
    Bigint c;
    init(&c);
    c.size = max(a.size , b.size) + 1;
    // 每一位计算
    for(int i=0;i<c.size;i++)
    c.num[i] = a.num[i] + b.num[i] -'0';
    // 处理进位
    for(int i=0;i<c.size;i++)
    if(c.num[i]>'9'){
        c.num[i+1] += 1;
        c.num[i] -= 10;
    }
    // 处理前导0
    solve0(&c);
    return c;
}
// 输入大整数并返回最后读的字符

char read_char(char x){
    while(x!='+' && x!='-' && x!='*' && x!='/'){
        x = getchar();
    }
    return x;
}

char read(Bigint*a){ 
    char x=' ';
    while(x<'0'||  x>'9') {
        if(x=='-') a->less0=1;
        x = getchar();
    }
    while(x>='0' && x<='9'){
        a->num[a->size++] = x;
        x=getchar();
    }
    reverse(a);
    return x;
}

Bigint Mul(Bigint a,Bigint b){
    Bigint c;init(&c);
    c.less0 = a.less0 ^ b.less0;
    c.size = a.size + b.size + 1;
    for(int i=0;i<c.size;i++) Tmp_mul[i] = 0;
    for(int i=0;i<a.size;i++)
    for(int j=0;j<b.size;j++)
    Tmp_mul[i+j] += (int)(a.num[i]-'0') *(int)(b.num[j]-'0');
    for(int i=0;i<c.size;i++){
        Tmp_mul[i+1] +=  Tmp_mul[i] / 10;
        Tmp_mul[i]  %=10;
        c.num[i] = (char)('0' + Tmp_mul[i]);
    }
    solve0(&c);
    return c;
}
Bigint Get_10k(int x){
    Bigint a;
    init(&a);
    a.size = x+1 ;
    a.num[x] = '1';
    return a; 
}

Bigint Get_Bigint(int x){
    Bigint a ;init(&a);
    while(x){
        a.num[a.size++] = (x%10 + '0');
        x/=10;
    }
    return a;
}

Bigint div(Bigint a,Bigint b){
    Bigint c;init(&c);
    c.less0 = a.less0 ^ b.less0;
    if(a.less0) a = inv(a);
    if(b.less0) b = inv(b);
    c.size = a.size - b.size +1 ; 
    if(c.size < 0) c.size = 0;
    for(int i=c.size-1;i>=0;i--){
        Bigint d = Mul(b,Get_10k(i));
        while(Check_lef(d,a)) {
            c.num[i]=c.num[i] +1;
            a = Del(a,d);
           // print(&a);printf("\n");
           // print(&b);printf("\n");

        }
    }
    return c;
}
// 输出

// gcc -o n num.c
// int main(){
//     Bigint a,b,c;
//     init(&a);
//     init(&b);
//     init(&c);
//     char x;
//     x = read(&a); x=read_char(x);read(&b);
//     if(x=='+') c = Add(a,b);
//     if(x=='-') c = Del(a,b);
//     if(x=='*') c = Mul(a,b);
//     if(x=='/') c = div(a,b);
//     print(&c);printf("\n");
// }
/*
590295810358705651712
*/