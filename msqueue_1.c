#include <stdio.h>
#include <stdlib.h>
#include "timers_lib.h"




struct node_t{
	int value;//TODO: maybe change this
	struct node_t * next;
};

struct queue_t{
	struct node_t * Head;
	struct node_t * Tail;
    int lock;
};


void initialize(struct queue_t * Q){//TODO: init count?
	struct node_t * node = (struct node_t *) malloc(sizeof(struct node_t));
	node->next = NULL;
	Q->Head = node; //TODO: check this
	Q->Tail = node;
}


void enqueue(struct queue_t * Q, int val){
	
    int store = 0;
    struct node_t * node = (struct node_t *) malloc(sizeof(struct node_t));
    node->value =  val;
    node->next = NULL;
    //while(__sync_lock_test_and_set(&Q->lock,1));
    //#pragma omp critical
    
    struct node_t * tail = Q->Tail;
    tail->next =node;
    Q->Tail = node;
    
    //__sync_lock_test_and_set(&Q->lock,0);

}


int dequeue(struct queue_t * Q, int * val){
    
    
    struct node_t * node = Q->Head;
    struct node_t * next = node->next;
    //struct node_t * tail = Q->Tail;
    if(next == NULL) return 0;
    else{
        *val = next->value;
        Q->Head =next;
    }
    
    free(node);
    
    return 1;
           
}

void printqueue(struct queue_t * Q){
    
    struct node_t * curr ;
    struct node_t * next ;
    
    curr = Q->Head;
    next = Q->Head->next;
    while (curr != Q->Tail){
        printf("%d ",curr->value);
        curr = next;
        next = curr ->next;
    }
    printf("%d ",curr->value);
    printf("\n");
    
}


int main(int argc, char *argv[]){

    int res,val,i,j,num_threads,count; 

    num_threads=atoi(argv[1]);
    count =atoi(argv[2]);

	struct queue_t * Q = (struct queue_t *) malloc(sizeof(struct queue_t));
    //Q->Head =  NULL;
    //Q->Tail =  NULL;
	initialize(Q);
    /*enqueue(Q,5);
    enqueue(Q,7);
    enqueue(Q,4);
    //res =  dequeue(Q,&val);
    //if (res) printf("Dequeued %d \n",val);
    enqueue(Q,1);
    printqueue(Q)*/
    
    timer_tt * timer = timer_init();
    timer_start(timer);
    #pragma omp parallel for num_threads(num_threads) shared(Q) private(res,val,i,j)
    for(i=0;i<num_threads;i++){
        for(j=0; j<count/num_threads;j++){
            #pragma omp critical
            {
                enqueue(Q,i);
            }
            #pragma omp critical
            {    
                res=dequeue(Q,&val);
                //if(res) printf("%d ",val);
            }
        }
    }
    timer_stop(timer);
    double timer_val = timer_report_sec(timer);
    printf("num_threads %d enq-deqs total %d\n",num_threads,count);
    printf("total time %lf\n",timer_val);
    //printqueue(Q);
	return 1;
}
	
