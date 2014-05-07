#include <stdio.h>
#include <stdlib.h>





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
    struct node_t * tail = Q->Tail;
    struct node_t * node = (struct node_t *) malloc(sizeof(struct node_t));
    node->value =  val;
    node->next = NULL;
    while(__sync_lock_test_and_set(&Q->lock,1));
    tail=node;
    __sync_lock_test_and_set(&Q->lock,0);

}


int dequeue(struct queue_t * Q, int * val){
    
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



	struct queue_t * Q = (struct queue_t *) malloc(sizeof(struct queue_t));
	initialize(Q);
	/*Q->Head.ptr->value = 5;
	printf(" %d \n",Q->Head.ptr->value);
	struct node_t * node = (struct node_t *) malloc(sizeof(struct node_t));
	node->value = 6;
	Q->Head.ptr->next.ptr=node;
	printf(" %d \n",Q->Head.ptr->next.ptr->value);
    */
    enqueue(Q,5);
    enqueue(Q,7);
    enqueue(Q,4);
	return 1;
}
	
