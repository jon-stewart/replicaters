#ifndef _DLIST_H_
#define _DLIST_H_

typedef struct dlist dlist_t;
struct dlist {
    dlist_t     *next;
    dlist_t     *prev;
};

int dlist_empty(dlist_t *);

void dlist_init(dlist_t *);

void dlist_add_back(dlist_t *, dlist_t *);

dlist_t *dlist_rm_front(dlist_t *);

dlist_t *dlist_rm_back(dlist_t *);

#define for_each_dlist_ele(head, ptr) \
    for ((ptr) = (head)->next; (ptr) != (head); (ptr) = (ptr)->next)

#endif
