/***********
 * Includes 
 ***********/ 
#include "linked_list.h"
#include <stdlib.h>

/******************************/
/* static function prototypes 
/******************************/
static node_t* ll_createNode(void * data);
static void ll_deleteNode(node_t * node);

/******************************/
/* public funtion definitions 
/******************************/
/* @brief: creates a list
 * @return: a new list */
list_t ll_create()
{
	list_t list;
	list.head = NULL;
	list.tail = NULL;
	list.size = 0;
	return list;
}

/* @brief: deletes all the nodes of a list
 * @list: list to be cleared */
void ll_clearList(list_t * list)
{
    while(list->size){
        ll_deleteFront(list);
    }
}

/* @brief: returns the size list
 */
uint16_t ll_size(list_t * list)
{
	return list->size;
}

/* @brief: returns "true" if the list is empty
 */
_Bool ll_empty(list_t * list)
{
	return list->head == NULL;
}

/* @brief: inserts a node at head
 * @list: pointer to the list where the node will be inserted
 * @data: pointer to the data that will be inserted
 */
_Bool ll_addFront(list_t * list, void * data)
{
	node_t * new = ll_createNode(data);
	if(new != NULL)
	{
		if(list->head == NULL)
		{
			list->head = new;
			list->tail = new;
		}
		else
		{
			new->prev = NULL;
			new->next = list->head;

			list->head->prev = new;
			list->head = new;
		}
		list->size++;
		return 1;
	}
	return 0;
}

/* @brief: inserts a node at tail
 * @list: pointer to the list where the node will be inserted
 * @data: pointer to the data that will be inserted
 */
_Bool ll_addBack(list_t * list, void * data)
{
	node_t * new = ll_createNode(data);
	if(new != NULL)
	{
		if(list->tail == NULL)
		{
			list->head = new;
			list->tail = new;
		}
		else
		{
			new->prev = list->tail;
			new->next = NULL;

			list->tail->next = new;
			list->tail = new;
		}
		list->size++;
		return 1;
	}
	return 0;
}

/* @brief: inserts a node in a specific position
 * @n: position
 * @list: pointer to the list where the node will be inserted
 * @data: pointer to the data that will be inserted
 */
_Bool ll_addAfter(list_t * list, void * data, uint16_t n)
{
	if( n > list->size-1 ) // the index is bigger than the list
		return 0;

	else if( list->head == NULL )
		return 0;

	else if( n == list->size-1 )
	{
		ll_addBack(list, data);
		return 1;
	}
	else
	{
		node_t* new = ll_createNode(data);
		node_t * pointer;
		uint16_t pos;

		if( n < list->size/2 )
		{
			pointer = list->head;
			pos = 0;
			while( pos < n ){
				pointer = pointer->next;
				pos++;
			}
		}
		else
		{
			pointer = list->tail;
			pos = list->size-1 ;
			while( pos > n )
			{
				pointer = pointer->prev;
				pos--;
			}
		}
		new->next = pointer->next;
		new->prev = pointer;

		pointer->next->prev = new;
		pointer->next = new;
		list->size++;
		return 1;
	}
}

/* @brief: deletes the head node of a list
 * @list: pointer to the list where the node will be deleted
 */
void ll_deleteFront(list_t * list)
{
	if(list->head != NULL) // list is not empty
	{
	    if(list->head == list->tail) // there is just one element
        {
            node_t * removed = list->head;

            list->head = NULL;
            list->tail = NULL;

            ll_deleteNode(removed);
        }
        else // more than one element
        {
            node_t * removed = list->head;

            list->head = removed->next;
            list->head->prev = NULL;

            ll_deleteNode(removed);
        }
        list->size--;
	}

}

/* @brief: deletes the tail node of a list
 * @list: pointer to the list where the node will be deleted
 */
void ll_deleteBack(list_t * list)
{
	if(list->tail != NULL)// list is not empty
	{
	    if(list->head == list->tail) // there is just one element
        {
            node_t * removed = list->tail;

            list->head = NULL;
            list->tail = NULL;

            ll_deleteNode(removed);
        }
	    else // more than one element
        {
            node_t * removed = list->tail;

            list->tail = removed->prev;
            list->tail->next = NULL;

            ll_deleteNode(removed);

        }

		list->size--;
	}
}

/* @brief: deletes a node in a specific position
 * @n: position
 * @list: pointer to the list where the node will be deleted
 */
void ll_deleteItem(list_t * list, uint16_t n)
{
    if(n == 0)
        ll_deleteFront(list);
    else if(n == list->size-1)
        ll_deleteBack(list);
    else if(n < list->size)
    {
        node_t * pointer;
        uint16_t pos;

        if( n < list->size/2 ) // start the search from head
        {
            pointer = list->head;
            pos = 0;
            while( pos < n ){
                pointer = pointer->next;
                pos++;
            }
        }
        else // start the search from tail
        {
            pointer = list->tail;
            pos = list->size-1;
            while( pos > n )
            {
                pointer = pointer->prev;
                pos--;
            }
        }

        pointer->next->prev = pointer->prev;
        pointer->prev->next = pointer->next;

        ll_deleteNode(pointer);

        list->size--;
    }
}

/* @brief: returns a pointer to the data of the first node
 * @list: pointer to the list
 * @return: pointer to the data */
void* ll_getFront(list_t* list)
{
    if(list->head != NULL)
        return list->head->data;

    return NULL;
}

/* @brief: returns a pointer to the data of the last node
 * @list: pointer to the list
 * @return: pointer to the data */
void* ll_getBack(list_t* list)
{
    if(list->tail != NULL)
        return list->tail->data;

    return NULL;
}


/* @brief: return the data pointer of a node in a specific position
 * @n: position
 * @list: pointer to the list where the node will be inserted
 */
void* ll_getItem(list_t* list, uint16_t n)
{
    if(n == 0)
        return ll_getFront(list);
    else if(n == list->size-1)
        return ll_getBack(list);
    else if(n < list->size)
    {
        node_t * pointer;
        uint16_t pos;

        if( n < list->size/2 ) // start the search from head
        {
            pointer = list->head;
            pos = 0;
            while( pos < n ){
                pointer = pointer->next;
                pos++;
            }
        }
        else // start the search from tail
        {
            pointer = list->tail;
            pos = list->size-1;
            while( pos > n )
            {
                pointer = pointer->prev;
                pos--;
            }
        }
        return pointer->data;
    }
    return NULL;
}

/*
 * @brief: searchs if any node of the list contains "key_data"
 * in "data" field.
 * @list: pointer to the list where the search will be performed
 * @key_data: data to search in the list
 * @returns the list index where the node is
 */ 
int16_t ll_searchData(list_t* list, void * key_data)
{
	if(list->head != NULL)//check if list is not empty
	{
		node_t * pointer = list->head;
		int16_t pos = 0;

		do{
			if(pointer->data == key_data)
				return pos;
			pos++;
			pointer = pointer->next;
		}while(pointer != NULL);
	} 
	return -1; // data wasn't found
}

/*******************************/
/* static function definitions 
/*******************************/
/* @brief: creates a new node
 * @data: pointer to the data to be stored
 */
static node_t* ll_createNode(void * data)
{
	node_t* node = (node_t *) malloc(sizeof(node_t));
	if(node != NULL)
	{
		node->next = NULL;
		node->prev = NULL;
		node->data = data;
	}
	return node;
}

/* @brief: deletes an existing node
 * @node: pointer to the node to be deleted
 */
static void ll_deleteNode(node_t * node)
{
	node->prev = NULL;
	node->next = NULL;
	free(node);

}

