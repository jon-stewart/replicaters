#ifndef _DLIST_H_
#define _DLIST_H_

typedef struct list list_t;
struct list {
    list_t     *next;
    list_t     *prev;
};

void list_init(list_t *);

void list_add(list_t *, list_t *);

list_t *list_rm_back(list_t *);

list_t *list_rm_front(list_t *);

#define for_each_list_ele(head, ptr) \
    for ((ptr) = (head)->next; (ptr) != (head); (ptr) = (ptr)->next)

static inline int
list_empty(list_t *list)
{
    return (list->next == list);
}

#endif
