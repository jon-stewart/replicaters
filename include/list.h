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

#define LIST_FOR_EACH(head, ptr) \
    for ((ptr) = (head)->next; (ptr) != (head); (ptr) = (ptr)->next)

#define LIST_FOR_EACH_SAFE(head, nxt, ptr) \
    for (ptr = (head)->next, nxt = ptr->next; ptr != (head); ptr = nxt, nxt = ptr->next)

#define LIST_ENTRY(ptr, type, name) \
    (type *) ((char *)ptr - ((size_t)&((type *)0)->name))

static inline int
list_empty(list_t *list)
{
    return (list->next == list);
}

#endif
