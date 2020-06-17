#ifndef LL_H
#define LL_H

/************
 * Includes 
 ************/ 
#include <stdint.h>

/************
 * Typedefs
 ************/ 
typedef struct node{
    void * data;
    struct node * next;
    struct node * prev;
}node_t;

typedef struct{
	node_t * head;
	node_t * tail;
	uint16_t size;
}list_t;

/**********************
 * Function prototypes
 **********************/
/* @brief: creates a list
 * @return: a new list */
list_t ll_create();

/* @brief: deletes all the nodes of a list
 * @list: list to be cleared */
void ll_clearList(list_t * list);

/* @brief: returns the size list
 */
uint16_t ll_size(list_t * list);

/* @brief: returns "true" if the list is empty
 */
_Bool ll_empty(list_t * list);

/* @brief: inserts a node at head
 * @list: pointer to the list where the node will be inserted
 * @data: pointer to the data that will be inserted
 */
_Bool ll_addFront(list_t * list, void * data);

/* @brief: inserts a node at tail
 * @list: pointer to the list where the node will be inserted
 * @data: pointer to the data that will be inserted
 */
_Bool ll_addBack(list_t * list, void * data);

/* @brief: inserts a node in a specific position
 * @n: position
 * @list: pointer to the list where the node will be inserted
 * @data: pointer to the data that will be inserted
 */
_Bool ll_addAfter(list_t * list, void * data, uint16_t n);

/* @brief: deletes the head node of a list
 * @list: pointer to the list where the node will be deleted
 */
void ll_deleteFront(list_t * list);

/* @brief: deletes the tail node of a list
 * @list: pointer to the list where the node will be deleted
 */
void ll_deleteBack(list_t * list);

/* @brief: deletes a node in a specific position
 * @n: position
 * @list: pointer to the list where the node will be deleted
 */
void ll_deleteItem(list_t * list, uint16_t n);

/* @brief: returns a pointer to the data of the first node
 * @list: pointer to the list
 * @return: pointer to the data */
void* ll_getFront(list_t* list);

/* @brief: returns a pointer to the data of the last node
 * @list: pointer to the list
 * @return: pointer to the data */
void* ll_getBack(list_t* list);

/* @brief: return the data pointer of a node in a specific position
 * @n: position
 * @list: pointer to the list where the node will be inserted
 */
void* ll_getItem(list_t* list, uint16_t n);

/*
 * @brief: searchs if any node of the list contains "key_data"
 * in "data" field.
 * @list: pointer to the list where the search will be performed
 * @key_data: data to search in the list
 * @returns the list index where the node is
 */ 
int16_t ll_searchData(list_t* list, void * key_data);

#endif
