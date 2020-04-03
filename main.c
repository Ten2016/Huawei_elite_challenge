#include <fcntl.h> 
#include <unistd.h>
#include <stdlib.h>

/*

Huawei elite challenge

date:   20200404
version:1.0
by:     xyTen
 */


typedef char bool; 
#define TRUE        1
#define FALSE       0

#define IDSIZE      12


#define STDOUT      1
#define STDERR      2
#define MAXBUF      10000000
#define MAXSETVEC   1000000
#define MAXPRIM     999943


char *datafile = "/home/jialu/Desktop/Huawei/test_data.txt";
char *ansfile  = "00";


//set struct
typedef struct circlenode{
    char id[IDSIZE];
    struct circlenode *next;
}CircleNode, *pCircleNode;

typedef struct hashnode{
    char id[IDSIZE];
    pCircleNode circle;
    struct hashnode *next;
}HashNode, *pHashNode; 

typedef struct hash{
    pHashNode set_vec[MAXBUF];
}Set;


//set function
void set_init(Set *set);
pHashNode set_add(Set *set, const char *id);
bool set_count(const Set *set, const char *id);
pHashNode set_find(const Set *set, const char *id);


//kernel function
bool load_data_to_set(Set *set, char *buf);


// general function
bool str_equ(const char *s1, const char *s2);
void str_cpy(char *s, const char *t);

int main(){

    //set init
    Set *set = (Set*)malloc(sizeof(Set));
    set_init(set);

    //data read
    int fd;
    fd = open(datafile, O_RDONLY, 0666);
    char *buf = (char*)malloc(MAXBUF);
    unsigned cnt = read(fd, buf, MAXBUF);
    buf[cnt]='\0';

    //load data to set 
    if(load_data_to_set(set, buf)==FALSE){
        write(STDERR, "load data to set error...", 20);
        exit(1);
    }





    











    write(STDOUT, buf, 50);


    return 0;
}




// all function

void set_init(Set *set){
    for(int i=0; i<MAXSETVEC; ++i)
        set->set_vec[i] = NULL;
}

pHashNode set_add(Set *set, const char *id){
    int sum=0;
    while(*id!='\0')
        sum += *id++;
    sum %= MAXPRIM;
    pHashNode p = set->set_vec[sum];
    if(p==NULL){
        p = (pHashNode)malloc(sizeof(HashNode));
        p->circle = NULL;
        p->next = NULL;
        set->set_vec[sum] = p;
    }
    while(p->next){
        p = p->next;
        if(str_equ(p->id, id)==TRUE)
            return p;
    }
    pHashNode q = (pHashNode)malloc(sizeof(HashNode));
    str_cpy(q->id, id);
    q->next = NULL;
    q->circle = NULL;
    p->next = q;
    return q;
}

bool set_count(const Set *set, const char *id){
    int sum=0;
    while(*id!='\0')
        sum += *id++;
    pHashNode p = set->set_vec[sum%MAXPRIM]->next;
    while(p && str_equ(p->id,id)==FALSE)
        p = p->next;
    return p==NULL?FALSE:TRUE;  
}

pHashNode set_find(const Set *set, const char *id){
    int sum=0;
    while(*id!='\0')
        sum += *id++;
    pHashNode p = set->set_vec[sum%MAXPRIM]->next;
    while(p && str_equ(p->id,id)==FALSE)
        p = p->next;
    return p;
}

bool load_data_to_set(Set *set, char *buf){
    char id1[IDSIZE], id2[IDSIZE];
    char *iid1=id1, *iid2=id2, *buff=buf;
    while(*buff!='\0'){
        iid1=id1;
        iid2=id2;
        while(*buff!=',')
            *iid1++ = *buff++;
        *iid1 = '\0';
        buff++;
        while(*buff!=',')
            *iid2++ = *buff++;
        *iid2 = '\0';
        while(*buff!='\n')
            buff++;
        buff++;
        pHashNode p = set_add(set, id1);
        pHashNode q = set_add(set, id2);
        pCircleNode s = p->circle;
        if(s==NULL){
            s = (pCircleNode)malloc(sizeof(CircleNode));
            s->next = NULL;
            p->circle = s;
        }
        while(s->next)
            s = s->next;
        pCircleNode t = (pCircleNode)malloc(sizeof(CircleNode));
        str_cpy(t->id, q->id);
        t->next = NULL;
        s->next = t;
    }


    return TRUE;
}








void str_cpy(char *s, const char *t){
    while(*t!='\0') *s++ = *t++;
    *s = '\0';
}


bool str_equ(const char *s1, const char *s2){
    while(*s1!='\0' && *s2!='\0' && *s1++ == *s2++)
        ;
    return (*s1==*s2=='\0');
}