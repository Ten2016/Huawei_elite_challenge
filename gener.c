#include "gener.h"


// all function

void set_init(){
    for(int i=0; i<MAXSETVEC; ++i)
        set->set_vec[i] = NULL;
}

int set_hash(const char *id){
    long sum=0;
    while(*id!='\0'){
        sum = sum*10+(*id-'0');
        ++id;
    }
    return (int)(sum%MAXPRIM);
}

pHashNode set_add(const char *id){
    int hash = set_hash(id);
    pHashNode p = set->set_vec[hash];
    if(p==NULL){
        p = (pHashNode)malloc(sizeof(HashNode));
        p->circle = NULL;
        p->circlenum = 0;
        p->next = NULL;
        set->set_vec[hash] = p;
    }
    while(p->next){
        if(str_equal(p->next->id, id))
            return p->next;
        if(str_great(p->next->id, id))
            break;
        p = p->next;
    }
    pHashNode q = (pHashNode)malloc(sizeof(HashNode));
    str_cpy(q->id, id);
    q->next = p->next;
    q->circle = NULL;
    q->circlenum = 0;
    p->next = q;
    return q;
}

pHashNode set_find(const char *id){
    int hash = set_hash(id);
    pHashNode p = set->set_vec[hash];
    if(p==NULL) return NULL;
    p = p->next;
    while(p && str_equal(p->id,id)==FALSE)
        p = p->next;
    return p;
}



//stack function
void stack_init(){
    stack[0]=0;
    ps = stack+1;
}
void stack_push(const char *id){
    while(*id!='\0')
        *ps++ = *id++;
    *ps++ = ',';
}
void stack_pop(){
    while(*(--ps)!=',');
    ++ps;
}










void load_data_to_set(){
    char id1[IDSIZE], id2[IDSIZE];
    char *tmp=NULL;
    pd = databuf;
    while(*pd!='\0'){
        tmp=id1;
        while(*pd!=',')
            *tmp++ = *pd++;
        *tmp='\0';
        ++pd;

        tmp=id2;
        while(*pd!=',')
            *tmp++ = *pd++;
        *tmp='\0';

        while(*pd++ != '\n');

        printf("%s %s \n",id1, id2);


        pHashNode p = set_add(id1);
        pHashNode q = set_add(id2);
        pCircleNode s = p->circle;


        if(s==NULL){
            s = (pCircleNode)malloc(sizeof(CircleNode));
            s->next = NULL;
            p->circle = s;
        }
        while(s->next){
            if(str_great(s->next->id, q->id))
                break;
            s = s->next;
        }
        pCircleNode t = (pCircleNode)malloc(sizeof(CircleNode));
        str_cpy(t->id, q->id);
        t->next = s->next;
        s->next = t;
        p->circlenum++;


    }
    return;
}




void str_cpy(char *s, const char *t){
    while(*t!='\0')
        *s++ = *t++;
    *s='\0';
}


bool str_equal(const char *s1, const char *s2){
    while(*s1==*s2 && *s1!='\0'){
        ++s1;
        ++s2;
    }
    return (*s1==*s2 && *s1=='\0');
}

bool str_great(const char *s1, const char *s2){
    long n1=0, n2=0;
    while(*s1!='\0'){
        n1 = n1*10 + (*s1-'0');
        ++s1;
    }
    while(*s2!='\0'){
        n2 = n2*10 + (*s2-'0');
        ++s2;
    }
    return n1>n2;
}

int int_to_str(int n, char *s){
    if(n==0){
        *s++ = '0';
        *s = '\n';
        return 2;
    }
    char tmp[IDSIZE];
    int cnt=0;
    char *t = tmp+IDSIZE-1;
    *t = '\0';
    while(n){
        *(--t) = n%10+'0';
        n /= 10;
    }
    while(*t!='\0'){
        *s++ = *t++;
        ++cnt;
    }
    *s = '\n';
    return cnt+1;
}