#include <stdio.h>
#include <stdlib.h>
#include "timers_lib.h"
/*struct pointer_t{
	struct node_t * ptr;
	unsigned long long count;
    unsigned long long both;
};
*/



struct node_t{
	int value;//TODO: maybe change this
	struct node_t *  next;
};

struct queue_t{
	struct node_t *  Head;
	struct node_t *  Tail;
};



/*int pointer_t_equal(struct pointer_t a , struct pointer_t b){
	return ((a.ptr == b.ptr)&&(a.count == b.count));
}
*/

unsigned long long get_count(unsigned long long a){
    
    unsigned long long b = a >>48;
    return b;
}

unsigned long long get_pointer(unsigned long long a){
    unsigned long long b = a << 16;
    b= b >>16;
    return b;
}

unsigned long long set_count(unsigned long long  a, unsigned long long count){
    unsigned long long count_temp =  count << 48;
    unsigned long long b = get_pointer(a);
    b = b | count_temp;
    return b;
}

unsigned long long set_pointer(unsigned long long a, unsigned long long ptr){
    unsigned long long b = 0;
    unsigned long long c = get_count(a);
    b = set_count(b,c);
    ptr = get_pointer(ptr);
    b= b | ptr;
    return b;
}

unsigned long long set_both(unsigned long long a, unsigned long long ptr, unsigned long long count){
    a=set_pointer(a,ptr);
    a=set_count(a,count);
    return a;
}

void initialize(struct queue_t * Q){//TODO: init count?
	struct node_t * node = (struct node_t *) malloc(sizeof(struct node_t));
	node->next = NULL;
	Q->Head= set_both(Q->Head,node,0); 
	Q->Tail= set_both(Q->Tail,node,0);
}

void enqueue(struct queue_t * Q, int val){
	
	struct node_t * tail;
    struct node_t * new_to_set;
	int temp;	
	struct node_t * node = (struct node_t *) malloc(sizeof(struct node_t));
	node->value = val;
	node->next = NULL;
	while (1){
		tail = Q->Tail;
		struct node_t * next_p = ((struct node_t *)get_pointer(tail))->next;
		if (tail == Q->Tail){
			if (next_p == NULL){
                new_to_set =set_both(new_to_set,node,get_count(next_p) +1);
				if (__sync_bool_compare_and_swap(&((struct node_t * )get_pointer(tail))->next,next_p,new_to_set))
					break;
			}
			else{
                new_to_set=set_both(new_to_set,next_p,get_count(tail)+1);
				temp = __sync_bool_compare_and_swap(&Q->Tail,tail,new_to_set);
			}
		}
	}
    new_to_set=set_both(new_to_set,node,get_count(tail)+1);
	temp = __sync_bool_compare_and_swap(&Q->Tail,tail,new_to_set);			
}


int dequeue(struct queue_t * Q,int * pvalue){

	struct node_t * head;
	struct node_t * tail;
	struct node_t * next;
	int  temp;
	int first_val;
    struct node_t * new_to_set;
	while(1){
		head =  Q->Head;
        first_val=((struct node_t *)get_pointer(head))->value;
		tail =  Q->Tail;
		next =  ((struct node_t *)get_pointer(head))->next;
		if ( head == Q->Head){
			if (head == tail){
				if ( next == NULL) 
					return 0;
                new_to_set =  set_both(new_to_set,next,get_count(tail) +1);
				temp = __sync_bool_compare_and_swap(&Q->Tail,tail,new_to_set);
			}
			else{
                //if ((head==Q->Head) &&(first_val!=((struct node_t *)get_pointer(head))->value)) printf("change detected!\n");
				*pvalue =((struct node_t *)get_pointer(next))->value;
                new_to_set = set_both(new_to_set,next,get_count(head)+1);
				if( __sync_bool_compare_and_swap(&Q->Head,head,new_to_set))
					break;
			}
		}
	}
    //printf(" about to free %p \n",head);
	free(get_pointer(head));
	return 1;
}

void printqueue(struct queue_t * Q){
	
	struct node_t * curr ;
	struct node_t * next ;
	
	curr = Q->Head;
    //printf(" in printqueue curr = %p\n",curr);
	next = ((struct node_t * )get_pointer(Q->Head))->next;
    //printf(" in printqueue next = %p\n",next);
	while ((curr != Q->Tail)&&(curr!=NULL)){
    //printf(" in printqueue curr = %p\n",curr);
    //printf(" in printqueue next = %p\n",next);
		printf("%d ",((struct node_t * )get_pointer(curr))->value);
		curr = next;
		if (next) next = ((struct node_t * )get_pointer(curr))->next;
	}
	//printf("%d ",((struct node_t * )get_pointer(curr))->value);
	printf("\n");
	
}

int main(int argc, char *argv[]){

	int res = 0;
	int val = 0;
	int i ,j,num_threads,count;
    num_threads = atoi(argv[1]);
    count = atoi(argv[2]);
    printf("%d %d \n",num_threads,count);
	struct queue_t * Q = (struct queue_t *) malloc(sizeof(struct queue_t));
	initialize(Q);
    //printf(" #### %p\n",&Q);
    //printf("count -> %llu\n",get_count(&Q));
    //printf("pointer -> %llu\n",get_pointer(&Q));
    //printf("queue head \n");
    //printf(" #### %p\n",&Q->Head);
    //printf("count -> %llu\n",get_count(&Q->Head));
    //printf("pointer -> %llu\n",get_pointer(&Q->Head));

    /*enqueue(Q,5);
    enqueue(Q,7);
    enqueue(Q,1);
    res = dequeue(Q,&val);
    if (res) printf("dequeued %d \n",val);
    res = dequeue(Q,&val);
    if (res) printf("dequeued %d \n",val);
    res = dequeue(Q,&val);
    if (res) printf("dequeued %d \n",val);
    res = dequeue(Q,&val);
    if (res) printf("dequeued %d \n",val);
    printqueue(Q);
    */
    //printf(" %d \n",Q->Head->value);

    //printf(" %d \n",((struct node_t *)get_pointer(Q->Head->next))->value);
    //struct node_t * curr = (struct  node_t *)get_pointer(Q->Head->next); 
    //printf(" %d \n",((struct node_t *)get_pointer(curr->next))->value);
    //printf(" increment count -> %llu\n",get_count(set_count(&Q,get_count(&Q) +1)));
    //printf(" change pointer -> %llu\n",set_pointer(&Q,&res));
    
    
    
    timer_tt * timer=timer_init();
    timer_start(timer);
	#pragma omp parallel for num_threads(num_threads) shared(Q) private(res,val,i,j)
	for(i=0;i<num_threads;i++){
         for (j=0;j<count;j++){
                enqueue(Q,i);
                res = dequeue(Q,&val);
                //if (res) printf("thread %d  dequeued --> %d\n",omp_get_thread_num(),val);
         }
	}
	//printqueue(Q);
    timer_stop(timer);
    double time_res = timer_report_sec(timer);
    printf("num_threasd %d  enq-deqs per thread %d \n",num_threads,count);
    printf("Total time  %lf \n",time_res);
    
	return 1;
}
	
