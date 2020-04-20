#include <fcntl.h> 
#include <unistd.h>
#include <stdlib.h>

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
    bool circleflag;
	pCircleNode circle;
    struct hashnode *next;
}HashNode, *pHashNode;

typedef struct {
    pHashNode set_vec[MAXSETVEC];
    pHashNode set_cur[MAXSETVEC];
}Set;

typedef struct anslist{
	char *cir;
	struct anslist *next;
}AnsList;
	



//global var
Set  *set;
char *databuf;
char *pd;
char stack[200];
char *ps;
int resnum;
AnsList *ans;




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
void run(char *num);


// general function
bool str_e(const char *s1, const char *s2);
bool str_ge(const char *s1, const char *s2);
void str_cpy(char *s, const char *t);
int str_len(const char *s);
int int_to_str(int n, char *s);


bool cir_isrepeat(char *id);
void ans_insert(const char *s);
void dfs(char *headid, pHashNode ph, pCircleNode pc);
void run(char *num);
void write_to_file(char *num);






int main(){


   // char datafile[100] = "/data/test_data.txt";


	char datafile[100] = "/home/jialu/Desktop/test2.txt";

    //set init
    set = (Set*)malloc(sizeof(Set));
    set_init();


    //data read
    databuf = (char*)malloc(MAXDATABUF);
    int datafd = open(datafile, O_RDONLY, 0666);
    int cnt = read(datafd, databuf, MAXDATABUF);
    close(datafd);

    //load data to set
    --cnt;
    while(databuf[cnt]>'9'||databuf[cnt]<'0')
        --cnt;
    databuf[cnt+1]='\n';
    databuf[cnt+2]='\0';

    load_data_to_set();
	
	
    //  run
    char num[IDSIZE];
    run(num);
	write_to_file(num);


    free(databuf);
    free(set);
	free(ans);


    return 0;
}




// all function

void set_init(){
    for(int i=0; i<MAXSETVEC; ++i)
        set->set_vec[i] = NULL;
}

int set_hash(const char *id){
    unsigned long sum=0;
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
        p->circleflag = FALSE;
        p->next = NULL;
        set->set_vec[hash] = p;
    }
    while(p->next){
        if(str_e(p->next->id, id))
            return p->next;
        if(str_ge(p->next->id, id))
            break;
        p = p->next;
    }
    pHashNode q = (pHashNode)malloc(sizeof(HashNode));
    str_cpy(q->id, id);
    q->next = p->next;
    q->circle = NULL;
    q->circleflag = FALSE;
    p->next = q;
    return q;
}

pHashNode set_find(const char *id){
    int hash = set_hash(id);
    pHashNode p = set->set_vec[hash];
    if(p==NULL) return NULL;
    p = p->next;
    while(p && str_e(p->id,id)==FALSE)
        p = p->next;
    return p;
}



//stack function
void stack_init(){
    *stack=0;
    ps = stack;
}

void stack_push(const char *id){
    ++ps;
    while(*id!='\0')
        *ps++ = *id++;
    *ps = ',';
    (*stack)++;
}

void stack_pop(){
    while(*(--ps)!=',');
    (*stack)--;
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

        pHashNode p = set_add(id1);
        pHashNode q = set_add(id2);
        pCircleNode s = p->circle;


        if(s==NULL){
			//create head node
            s = (pCircleNode)malloc(sizeof(CircleNode));
            s->next = NULL;
            p->circle = s;
        }
        while(s->next){
            if(str_ge(s->next->id, q->id))
                break;
            s = s->next;
        }
        pCircleNode t = (pCircleNode)malloc(sizeof(CircleNode));
        str_cpy(t->id, q->id);
        t->next = s->next;
        s->next = t;
        p->circleflag=TRUE;

    }
    return;
}




void str_cpy(char *s, const char *t){
    while(*t!='\0')
        *s++ = *t++;
    *s='\0';
}


bool str_e(const char *s1, const char *s2){
    while(*s1==*s2 && *s1!='\0'){
        ++s1;
        ++s2;
    }
    return (*s1==*s2 && *s1=='\0');
}

bool str_ge(const char *s1, const char *s2){
    unsigned long n1=0, n2=0;
    while(*s1!='\0'){
        n1 = n1*10+(*s1-'0');
        ++s1;
    }
    while(*s2!='\0'){
        n2 = n2*10+(*s2-'0');
        ++s2;
    }
    return n1>n2;
}

int str_len(const char *s){
	int ans=0;
	while(*s++ != '\0')
		++ans;
	return ans;
}


int int_to_str(int n, char *s){
    if(n==0){
        *s++ = '0';
        *s = '\n';
        return 2;
    }
    char tmp[IDSIZE];
    char *t = tmp+IDSIZE-1;
    *t = '\0';
    while(n){
        *(--t) = n%10+'0';
        n /= 10;
    }
	int cnt = 0;
    while(*t!='\0'){
        *s++ = *t++;
        ++cnt;
    }
    *s = '\n';
    return cnt+1;
}

bool cir_isrepeat(char *id){
    char *tps = stack+1;
	char *tid = id;
	while(*tps++ != ',');
	for(int i=1; i<*stack; ++i){
		tid = id;
		while( *tps==*tid && *tps!=',' && *tid!='\0'){
			++tps;
			++tid;
		}
		if(*tps==',' && *tid=='\0')
			return TRUE;
		while(*tps++ != ',');
	}
    return FALSE;
}



void ans_insert(const char *s){
	AnsList *pa = ans;
	while(pa->next && *(pa->next->cir)<=*s)
		pa = pa->next;
	AnsList *q = (AnsList*)malloc(sizeof(AnsList));
	q->cir = (char*)malloc(200);
	str_cpy(q->cir, s);
	q->next = pa->next;
	pa->next = q;
}


void dfs(char *headid, pHashNode ph, pCircleNode pc){

    if(*stack>MAXCIR)
        return;
    if(str_ge(headid, pc->id))
		return;
//	write(STDOUT, headid, str_len(headid));
//	write(STDOUT, " ", 1);
//	write(STDOUT, pc->id, str_len(pc->id));
//	write(STDOUT, " ", 1);
	if(cir_isrepeat(pc->id))
		return;

	if(str_e(headid, pc->id)){
		if(*stack>=MINCIR){
			//insert
			*ps = '\0';
			ans_insert(stack);
			*ps = ',';
			++resnum;
		}
		return;
	}
	
	stack_push(pc->id);

//	write(STDOUT, stack+1, ps-stack);
//	write(STDOUT, "\n", 1);
	
    int ni = set_hash(pc->id);
    pHashNode pph = set->set_vec[ni]->next;
    while(str_e(pph->id, pc->id)==FALSE)
        pph = pph->next;
    if(pph->circleflag==FALSE)
        return;
    pCircleNode ppc = pph->circle->next;
    while(ppc){
        dfs(headid, pph, ppc);
        ppc = ppc->next;
    }
	stack_pop();
    return;
} 


void run(char *num){

    pd = databuf;
	resnum = 0;
	ans = (AnsList*)malloc(sizeof(AnsList));
	ans->next= NULL;
	ans->cir = (char*)malloc(10);
	*(ans->cir) = 0;
	

    for(int i=0; i<MAXSETVEC; ++i){
        pHashNode ph = set->set_vec[i];
        if(ph==NULL)
            continue;
        char *phid=NULL;
        ph = ph->next;
        while(ph){
            if(ph->circleflag==FALSE){
                ph = ph->next;
                continue;
            }

            stack_init();
            stack_push(ph->id);

            pCircleNode pc = ph->circle->next;
            while(pc){
                dfs(ph->id, ph, pc);
                pc = pc->next;
            }
            ph = ph->next;
        }
    }
    int_to_str(resnum, num);
	return;
}


void test(const char *id){
	
    int ni = set_hash(id);
    pHashNode pph = set->set_vec[ni];
	if(pph==NULL)
		return;
    while(str_e(pph->id, id)==FALSE)
        pph = pph->next;
    if(pph->circleflag==FALSE)
        return;
    pCircleNode ppc = pph->circle->next;
    while(ppc){
		write(STDOUT, ppc->id, str_len(ppc->id));
		write(STDOUT, "\n", 1);
        ppc = ppc->next;
    }
    return;
	
}


void write_to_file(char *num){
	write(STDOUT, num, str_len(num));

	AnsList *pa = ans->next;
	int ansfd;
	//char ansfile[100]  = "/projects/student/result.txt";

char ansfile[100]  = "/home/jialu/Desktop/res2.txt";



	ansfd = open(ansfile, O_RDWR | O_CREAT | O_TRUNC, 0666);
	write(ansfd, num, str_len(num));
	while(pa){
		
		/*
				char c[10];
		c[0] = pa->cir[0]+'0';
		c[1] = ' ';
		write(STDOUT, c, 2);
		write(STDOUT, pa->cir+1, str_len(pa->cir)-1);
		write(STDOUT, "\n", 1);
		
		*/
		

		write(ansfd, pa->cir+1, str_len(pa->cir)-1);
		write(ansfd, "\n", 1);
		
		
		

		pa = pa->next;
	}
	close(ansfd);
}
	

