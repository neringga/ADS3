#ifndef PQ_H_INCLUDED
#define PQ_H_INCLUDED
#include "PQ_dataType.h"
/*
*******Error codes**********
  Queue was already created or Queue was not created yet // return 3
  Failed to allocate memory for the node. Might be full // return 2
  if queue is empty // return 1
  if queue is not empty || No errors ||  // return 0
*/
typedef struct priority_queue
{
	PQ_dataType element;
	int priority;
	struct priority_queue *next;
}*PNode;
typedef struct PQ_head
{
	PNode start;
}*Priority_Queue;


Priority_Queue Create_queue(int *error);// allocates memory and returns a pointer to the 'PQ_head' structure
int Delete_queue(Priority_Queue * queue);// allocates memory and returns a pointer to the 'PQ_head' structure
int Is_empty(Priority_Queue queue);
int Is_full(Priority_Queue queue);
int Insert_element(Priority_Queue * queue, PQ_dataType input_value, int input_priority);
int Delete_element(Priority_Queue * queue, PQ_dataType *value);
int Get_first_element(Priority_Queue queue, PQ_dataType *number, int *priority);

#endif // PQ_H_INCLUDED

