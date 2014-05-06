#include <stdio.h>
#include <stdlib.h>

struct pointer_t{
	struct node_t * ptr;
	unsigned long long count;
};

struct node_t{
	int value;//TODO: maybe change this
	struct pointer_t next;
};

struct queue_t{
	struct pointer_t Head;
	struct pointer_t Tail;
};


void initialize(struct queue_t * Q){//TODO: init count?
	struct node_t * node = (struct node_t *) malloc(sizeof(struct node_t));
	node->next.ptr = NULL;
	Q->Head.ptr = node; //TODO: check this
	Q->Tail.ptr = node;
}

int pointer_t_equal(struct pointer_t a , struct pointer_t b){
	return ((a.ptr == b.ptr)&&(a.count == b.count));
}

void enqueue(struct queue_t * Q, int val){
		
	int temp;	
	struct pointer_t * arg =  (struct pointer_t *) malloc(sizeof(struct pointer_t)); //TODO: is this ok?
	struct node_t * node = (struct node_t *) malloc(sizeof(struct node_t));
	node->value = val;
	node->next.ptr = NULL;
	while (1){
		struct pointer_t tail = Q->Tail;
		struct pointer_t next_p = (tail.ptr)->next;
		if (pointer_t_equal(tail, Q->Tail)){
			if (next_p.ptr == NULL){
				arg->ptr = node;//TODO: is it ok?
				arg->count = next_p.count + 1;
				if (__sync_bool_compare_and_swap(&(tail.ptr->next),next_p,*arg))
					break;
			}
			else{
				arg->ptr = next_p.ptr;
				arg->count = tail.count + 1;
				temp = __sync_bool_compare_and_swap(&Q->Tail,tail,arg);
			}
		}
	}
	arg->ptr = node;
	arg->count = tail.count + 1;
	temp = __sync_bool_compare_and_swap(&Q->Tail,tail,arg);			
}


int main(int argc, char *argv[]){



	struct queue_t * Q = (struct queue_t *) malloc(sizeof(struct queue_t));
	initialize(Q);
	Q->Head.ptr->value = 5;
	printf(" %d \n",Q->Head.ptr->value);
	struct node_t * node = (struct node_t *) malloc(sizeof(struct node_t));
	node->value = 6;
	Q->Head.ptr->next.ptr=node;
	printf(" %d \n",Q->Head.ptr->next.ptr->value);

	return 1;
}
	
