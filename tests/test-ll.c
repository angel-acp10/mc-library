#include <stdio.h>
#include "linked_list.h"

#define SIZE_A 10
int a[SIZE_A];

void print_list(list_t * list);

int main()
{
    printf("\n0. Creating A array. Size = %d. Values are:\n",SIZE_A);
    for(int i=0; i<SIZE_A; i++){
        a[i] = i;
        printf("\ta[%d] = %d\n", i, a[i]);
    }

    //checks ll_create
    printf("\n1. Create an empty list.\n");
    list_t l = ll_create();
    
    //checks ll_addBack
    printf("\n2. Use ll_addBack to add all elements from array 'a'.\n ");
    for(int i=0; i<SIZE_A; i++)
        ll_addBack(&l, a+i); 
    print_list(&l);

    //checks ll_addFront
    printf("\n3. Use ll_addFront to add all elements from array 'a'.\n");
    printf("Its size should be = %d\n", SIZE_A*2);
    for(int i=0; i<SIZE_A; i++)
        ll_addFront(&l, a+i);
    print_list(&l);

    //checks ll_addAfter with index = 0
    printf("\n4.1. Use ll_addAfter to introduce data=1000 after index = 0\n");
    int aux1 = 1000;
    ll_addAfter(&l, &aux1, 0);
    print_list(&l);

    int idx = ll_size(&l)*1/4;
    printf("4.2. Use ll_addAfter to introduce data=2000 after index = %d\n", idx);
    int aux2 = 2000;
    ll_addAfter(&l, &aux2, idx);
    print_list(&l);

    idx = ll_size(&l)*2/4;
    printf("4.3. Use ll_addAfter to introduce data=3000 after index = %d\n", idx);
    int aux3 = 3000;
    ll_addAfter(&l, &aux3, idx);
    print_list(&l);

    idx = ll_size(&l)*3/4;
    printf("4.4. Use ll_addAfter to introduce data=4000 after index = %d\n", idx);
    int aux4 = 4000;
    ll_addAfter(&l, &aux4, idx);
    print_list(&l);

    idx = ll_size(&l)-1;
    printf("4.5. Use ll_addAfter to introduce data=5000 after index = %d\n", idx);
    int aux5 = 5000;
    ll_addAfter(&l, &aux5, idx);
    print_list(&l);

    //check ll_deleteItem
    printf("\n5.1. Use ll_deleteItem to delete first node\n");
    ll_deleteItem(&l,0);
    print_list(&l);

    idx = ll_size(&l)*1/3;
    printf("5.2. Use ll_deleteItem to delete node at index = %d\n", idx);
    ll_deleteItem(&l,idx);
    print_list(&l);
    
    idx = ll_size(&l)*2/3;
    printf("5.3. Use ll_deleteItem to delete node at index = %d\n", idx);
    ll_deleteItem(&l,idx);
    print_list(&l);

    printf("5.4. Use ll_deleteItem to delete last node\n");
    ll_deleteItem(&l,ll_size(&l)-1);
    print_list(&l);

    //check ll_getFront
    printf("\n6. Use ll_getFront to get data of first node\n");
    printf("\tdata = %d\n", *((int*)ll_getFront(&l)));

    //check ll_getBack
    printf("\n7. Use ll_getBack to get data of last node\n");
    printf("\tdata = %d\n", *((int*)ll_getBack(&l)));

    //check ll_getItem
    printf("\n8.1. Use ll_getItem to get data at index = 0\n");
    printf("\tdata = %d,\n", *((int*)ll_getItem(&l,0)));
    
    idx = ll_size(&l)*1/3;
    printf("8.2. Use ll_getItem to get data at index = %d\n", idx);
    printf("\tdata = %d,\n", *((int*)ll_getItem(&l,idx)));
    
    idx = ll_size(&l)*2/3;
    printf("8.3. Use ll_getItem to get data at index = %d\n", idx);
    printf("\tdata = %d,\n", *((int*)ll_getItem(&l,idx)));
    
    idx = ll_size(&l)-1;
    printf("8.4. Use ll_getItem to get data at index = %d\n", idx);
    printf("\tdata = %d,\n", *((int*)ll_getItem(&l,idx)));

    //check ll_searchData
    printf("\n9.1. Use ll_searchData to get the index where data matches. Search = &aux1\n");
    printf("\ti = %d\n", ll_searchData(&l, &aux1));
    
    printf("9.2. Use ll_searchData to get the index where data matches. Search = a+1\n");
    printf("(which points to data = 1)\n");
    printf("(note: there are to data=1 in list, but it only return the first one)\n");
    printf("\ti = %d\n", ll_searchData(&l, a+1));
    
    printf("9.3. Use ll_searchData to get the index where data matches. Search = &aux4\n");
    printf("\ti = %d\n", ll_searchData(&l, &aux4));
    
    printf("9.4. Use ll_searchData to get the index where data matches. Search = a+9\n");
    printf("(which points to data = 9)\n");
    printf("\ti = %d\n", ll_searchData(&l, a+9));

    //checks if list is empty
    printf("\n10. Check if list is empty with ll_empty.\n");
    ll_empty(&l)? printf("\tTRUE\n") : printf("\tFALSE\n"); 

    //checks ll_deleteBack
    printf("\n11. Delete whole list using ll_deleteBack.\n");
    int s = ll_size(&l);
    for(int i=0; i<s; i++)
        ll_deleteBack(&l);
    print_list(&l);

    //checks if list is empty
    printf("\n12. Check if list is empty with ll_empty.\n");
    ll_empty(&l)? printf("\tTRUE\n") : printf("\tFALSE\n"); 

    //refill list with array A
    printf("\n13. Refill list using ll_addBack to add all elements from array 'a'\n");
    for(int i=0; i<SIZE_A; i++)
        ll_addBack(&l, a+i); 
    print_list(&l);

    //checks ll_deleteFront
    printf("\n14. Delete whole list using ll_deleteFront.\n");
    s = ll_size(&l);
    for(int i=0; i<s; i++)
        ll_deleteFront(&l);
    print_list(&l);

    //refill list with array A
    printf("\n15. Refill list using ll_addBack to add all elements from array 'a'\n");
    for(int i=0; i<SIZE_A; i++)
        ll_addBack(&l, a+i); 
    print_list(&l);

    //checks ll_clearList
    printf("\n16. Delete whole list using ll_clearList.\n");
    ll_clearList(&l);
    print_list(&l);

    return 0;
}

void print_list(list_t * list)
{
    if(ll_size(list)){
        //checks ll_getItem
        for(int i=0; i<ll_size(list); i++)
            printf("\ti = %d,\tdata = %d\n", i, *((int*)ll_getItem(list,i)) ); 
    }else
        printf("\tEMPTY LIST\n");
}
