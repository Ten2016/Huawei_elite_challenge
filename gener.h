#include <fcntl.h> 
#include <unistd.h>
#include <stdlib.h>

#include <stdio.h>



typedef char bool;

#define TRUE        1
#define FALSE       0

#define STDOUT      1
#define STDERR      2

#define IDSIZE      14
#define MAXDATABUF  10000000
#define MAXSETVEC   100000
#define MAXPRIM     99991

#define MINCIR      3
#define MAXCIR      7




//set struct
typedef struct circlenode{
    char id[IDSIZE];
    struct circlenode *next;

}CircleNode, *pCircleNode;

typedef struct hashnode{
    char id[IDSIZE];
    unsigned char circlenum;
    struct hashnode *next;
    struct hashnode *curr;
    pCircleNode circle;
}HashNode, *pHashNode;

typedef struct hash{
    pHashNode set_vec[MAXSETVEC];
    pHashNode set_cur[MAXSETVEC];
}Set;


//global var
Set  *set;
char *databuf;
char *pd;
char stack[200];
char *ps;
int  resnum;

//set function
void set_init();
int  set_hash(const char *id);
pHashNode set_add(const char *id);
pHashNode set_find(const char *id);

//stack function
void stack_init();
void stack_push(const char *id);
void stack_pop();



//kernel function
void load_data_to_set();
int run(char *num);


// general function
bool str_equal(const char *s1, const char *s2);
bool str_great(const char *s1, const char *s2);
void str_cpy(char *s, const char *t);
int int_to_str(int n, char *s);