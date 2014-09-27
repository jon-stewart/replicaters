#ifndef _LIST_H_
#define _LIST_H_

#define LIST_INIT(name) { &(name), &(name) }
#define LIST(name) \
    list_t name = LIST_INIT(name)

typedef struct list list_t;
struct list {
    list_t     *next;
    list_t     *prev;
};

void list_init(list_t *);

void list_add(list_t *, list_t *);

list_t *list_rm_back(list_t *);

list_t *list_rm_front(list_t *);

void list_rm(list_t *);

#define for_each_list_ele(head, ptr) \
    for ((ptr) = (head)->next; (ptr) != (head); (ptr) = (ptr)->next)

#define for_each_list_ele_safe(head, n, ptr) \
    for (ptr = (head)->next, n = ptr->next; ptr != (head); ptr = n, n = ptr->next)

static inline int
list_empty(list_t *list)
{
    return (list->next == list);
}

#endif
