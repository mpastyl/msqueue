#include <stdio.h>
#include <stdlib.h>

/*struct pointer_t{
	struct node_t * ptr;
	unsigned long long count;
};
*/

struct node_t{
	int value;//TODO: maybe change this
	struct node_t * next;
};

struct queue_t{
	struct node_t * Head;
	struct node_t * Tail;
};


void initialize(struct queue_t * Q){//TODO: init count?
	struct node_t * node = (struct node_t *) malloc(sizeof(struct node_t));
	node->next = NULL;
	Q->Head = node; //TODO: check this
	Q->Tail = node;
}

/*int pointer_t_equal(struct pointer_t a , struct pointer_t b){
	return ((a.ptr == b.ptr)&&(a.count == b.count));
}
*/
void enqueue(struct queue_t * Q, int val){
	
	struct node_t * tail;
	int temp;	
	struct node_t * node = (struct node_t *) malloc(sizeof(struct node_t));
	node->value = val;
	node->next = NULL;
	while (1){
		tail = Q->Tail;
		struct node_t * next_p = tail->next;
		if (tail == Q->Tail){
			if (next_p == NULL){
				if (__sync_bool_compare_and_swap(&tail->next,next_p,node))
					break;
			}
			else{
				temp = __sync_bool_compare_and_swap(&Q->Tail,tail,next_p);
			}
		}
	}
	temp = __sync_bool_compare_and_swap(&Q->Tail,tail,node);			
}


int dequeue(struct queue_t * Q,int * pvalue){

	struct node_t * head;
	struct node_t * tail;
	struct node_t * next;
	int temp;
	
	while(1){
		head =  Q->Head;
		tail =  Q->Tail;
		next =  head->next;
		if ( head == Q->Head){
			if (head == tail){
				if ( next == NULL) 
					return 0;
				temp = __sync_bool_compare_and_swap(&Q->Tail,tail,next);
			}
			else{
				*pvalue = next->value;
				if( __sync_bool_compare_and_swap(&Q->Head,head,next))
					break;
			}
		}
	}
	free(head);
	return 1;
}

void printqueue(struct queue_t * Q){
	
	struct node_t * curr ;
	struct node_t * next ;
	
	curr = Q->Head;
	next = Q->Head->next;
	while (curr != Q->Tail){
		printf(" %d ",curr->value);
		curr = next;
		next = curr ->next;
	}
	printf("%d ",curr->value);
	printf("\n");
	
}

int main(int argc, char *argv[]){

	int res = 0;
	int val = 0;
	int i ;
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
	enqueue(Q,2);
	enqueue(Q,4);
	enqueue(Q,5);
	enqueue(Q,3);
	printqueue(Q);

	for(i =0 ;i <8 ; i++){
		res = dequeue(Q,&val);
		if (res == 1)printf(" dequeued ---> %d \n",val);
		else printf(" queue is empty \n");
	}
	
	enqueue(Q,7);
	printqueue(Q);
	return 1;
}
	
