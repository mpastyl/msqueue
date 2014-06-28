#include <stdio.h>
#include <stdlib.h>
#include "timers_lib.h"
struct pointer_t{
    __int128 both;
};




struct node_t{
	int value;//TODO: maybe change this
	struct pointer_t  next;
};

struct queue_t{
	struct pointer_t  Head;
	struct pointer_t  Tail;
};



/*int pointer_t_equal(struct pointer_t a , struct pointer_t b){
	return ((a.ptr == b.ptr)&&(a.count == b.count));
}
*/
__int128 get_count(__int128 a){

    __int128 b = a >>64;
    return b;
}

__int128 get_pointer(__int128 a){
    __int128 b = a << 64;
    b= b >>64;
    return b;
}

__int128 set_count(__int128  a, __int128 count){
    __int128 count_temp =  count << 64;
    __int128 b = get_pointer(a);
    b = b | count_temp;
    return b;
}

__int128 set_pointer(__int128 a, __int128 ptr){
    __int128 b = 0;
    __int128 c = get_count(a);
    b = set_count(b,c);
    ptr = get_pointer(ptr);
    b= b | ptr;
    return b;
}

__int128 set_both(__int128 a, __int128 ptr, __int128 count){
    a=set_pointer(a,ptr);
    a=set_count(a,count);
    return a;
}



void initialize(struct queue_t * Q){//TODO: init count?
	struct node_t * node = (struct node_t *) malloc(sizeof(struct node_t));
	node->next.both = 0;
	Q->Head.both= set_both(Q->Head.both,node,0); 
	Q->Tail.both= set_both(Q->Tail.both,node,0);
}

int enqueue(struct queue_t * Q, int val){
	
	struct pointer_t tail;
    __int128 new_to_set;
	int temp;	
    int count = 0;
	struct node_t * node = (struct node_t *) malloc(sizeof(struct node_t));
	node->value = val;
	node->next.both = 0;
	while (1){
		tail = Q->Tail;
		struct pointer_t next_p = ((struct node_t *)get_pointer(tail.both))->next;
		if (tail.both == Q->Tail.both){
			if (get_pointer(next_p.both) == 0){
                new_to_set =set_both(new_to_set,node,get_count(next_p.both) +(__int128)1);
				if (__sync_bool_compare_and_swap(&(((struct node_t * )get_pointer(tail.both))->next.both),next_p.both,new_to_set))
					break;
                else count++;
			}
			else{
                new_to_set=set_both(new_to_set,next_p.both,get_count(tail.both)+(__int128)1);
				temp = __sync_bool_compare_and_swap(&Q->Tail.both,tail.both,new_to_set);
                if(!temp) count++;
			}
		}
	}
    new_to_set=set_both(new_to_set,node,get_count(tail.both)+(__int128)1);
	temp = __sync_bool_compare_and_swap(&Q->Tail.both,tail.both,new_to_set);
    if(!temp) count++;
    return count;
}


int dequeue(struct queue_t * Q,int * pvalue){

	struct pointer_t head;
	struct pointer_t tail;
	struct pointer_t next;
	int  temp;
	int first_val;
    __int128 new_to_set;
    int count = 0;
	while(1){
		head =  Q->Head;
        first_val=((struct node_t *)get_pointer(head.both))->value;
		tail =  Q->Tail;
		next =  ((struct node_t *)get_pointer(head.both))->next;
		if ( head.both == Q->Head.both){
			if (head.both == tail.both){
				if ( get_pointer(next.both) == 0) 
					return count;
                new_to_set =  set_both(new_to_set,next.both,get_count(tail.both) +(__int128)1);

				temp = __sync_bool_compare_and_swap(&Q->Tail.both,tail.both,new_to_set);
                if(!temp) count++;
			}
			else{
                //if ((head==Q->Head) &&(first_val!=((struct node_t *)get_pointer(head))->value)) printf("change detected!\n");
				*pvalue =((struct node_t *)get_pointer(next.both))->value;
                new_to_set = set_both(new_to_set,next.both,get_count(head.both)+(__int128)1);
				if( __sync_bool_compare_and_swap(&Q->Head.both,head.both,new_to_set))
					break;
                else count++;
			}
		}
	}
    //printf(" about to free %p \n",head);
	free(get_pointer(head.both));
	return count;
}

void printqueue(struct queue_t * Q){
	
	struct pointer_t curr ;
	struct pointer_t next ;
	
	curr = Q->Head;
    //printf(" in printqueue curr = %p\n",curr);
	next = ((struct node_t * )get_pointer(Q->Head.both))->next;
    //printf(" in printqueue next = %p\n",next);
	while ((get_pointer(curr.both) != get_pointer(Q->Tail.both))&&(get_pointer(curr.both)!=0)){
    //printf(" in printqueue curr = %p\n",curr);
    //printf(" in printqueue next = %p\n",next);
		printf("%d ",((struct node_t * )get_pointer(curr.both))->value);
		curr = next;
		if (get_pointer(next.both)) next = ((struct node_t * )get_pointer(curr.both))->next;
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
    
    
    srand(time(NULL));   
    int total_count=0;
    int c,k;
    timer_tt * timer=timer_init();
    timer_start(timer);
	#pragma omp parallel for num_threads(num_threads) shared(Q) private(res,val,i,j,c,k) reduction(+: total_count)
	for(i=0;i<num_threads;i++){
         for (j=0;j<count/num_threads;j++){
                total_count += enqueue(Q,i);
                c=rand()%1000;
                for(k=0;k<c;k++);
                total_count += dequeue(Q,&val);
                //if (res) printf("thread %d  dequeued --> %d\n",omp_get_thread_num(),val);
         }
	}
	//printqueue(Q);
    timer_stop(timer);
    double time_res = timer_report_sec(timer);
    printf("num_threasd %d  enq-deqs total %d \n",num_threads,count);
    printf("Total time  %lf \n",time_res);
    printf("total failed CASs %d \n",total_count);
	return 1;
}
	
