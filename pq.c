#include <stdlib.h>
#include <stdio.h>
#include "pq.h"
#include "PQ_dataType.h"
/*
*******Error codes**********
  Queue was already created or Queue was not created yet // return 3
  Failed to allocate memory for the node. Might be full // return 2
  if queue is empty // return 1
  if queue is not empty || queue is not full || No errors  // return 0
*/
Priority_Queue Create_queue(int *error)
{
    Priority_Queue queue = malloc(sizeof(struct PQ_head));
    if (queue != NULL)  queue->start = NULL;
    else *error = 2;
    return queue;
}

int Is_empty(Priority_Queue queue)
{
	if(queue == NULL)   return 3;
	return queue -> start == NULL;
}

int Delete_queue(Priority_Queue * queue)
{
	if((*queue) == NULL) return 3;
	else
    {
        while((*queue) -> start != NULL)
        {
            PNode temp = (*queue) -> start;
            (*queue) -> start = (*queue) -> start -> next;
            free(temp);
        }
        free(queue);
        return 0;
    }
}

int Insert_element(Priority_Queue * queue, PQ_dataType input_value, int input_priority)
{
	if((*queue) == NULL) return 3;
	PNode node = malloc(sizeof(struct priority_queue));
	if(node == NULL) return 2;
	else
	{
		node -> element = input_value;
		node -> priority = input_priority;
		node -> next = NULL;
		if((*queue) -> start == NULL)	(*queue) -> start = node;
		else if((*queue) -> start -> priority < input_priority)
		{
            node -> next = (*queue) -> start;
            (*queue) -> start = node;
		}
		else
		{
			PNode previous = (*queue) -> start;
			PNode temp = (*queue) -> start -> next;
			while(temp != NULL && input_priority <= temp -> priority)
			{
				previous = temp;
				temp = temp -> next;
			}
            if(temp == NULL)	previous -> next = node;
            else
            {
                node -> next = temp;
                previous -> next = node;
            }
		}
	 }return 0;

}

int Delete_element(Priority_Queue *queue, PQ_dataType *value)
{
	if((*queue) == NULL)	return 3;
	else if ((*queue) -> start == NULL)	return 1;
	else
	{
		PNode temp = (*queue) -> start;
		*value = temp -> element;
		(*queue) -> start=(*queue) -> start -> next;
		free(temp);
		return 0;
	}
}

int Is_full(Priority_Queue queue)
{
	if(queue == NULL) return 3;
    PNode node = malloc(sizeof(struct priority_queue));
	if(node == NULL)
    {
        free(node);
        return 2;
    }
	else
    {
        free(node);
        return 0;
    }
}

int Get_first_element(Priority_Queue queue, PQ_dataType *element, int *priority)
{
	if(queue == NULL) return 3;
	if(queue -> start != NULL)
	{
		*element = queue -> start -> element;
		*priority = queue -> start -> priority;
		return 0;
	}else return 1;
}