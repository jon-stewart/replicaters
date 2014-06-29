#include <stdlib.h>
#include <assert.h>
#include <dlist.h>

int
dlist_empty(dlist_t *dlist)
{
    int         ret = 0;

    ret = dlist->next == dlist;

    if (ret) {
        assert(dlist->prev == dlist);
    }

    return (ret);
}

void
dlist_init(dlist_t *dlist)
{
    dlist->next = dlist->prev = dlist;
}

void
dlist_add_back(dlist_t *head, dlist_t *dlist)
{
    dlist_t     *prev;

    assert(head != NULL);
    assert(dlist != NULL);

    prev = head->prev;

    /* if dlist is empty */
    if (head->prev == head) {
        head->next = dlist;
    } else {
        prev->next = dlist;
    }

    head->prev = dlist;

    dlist->prev = prev;
    dlist->next = head;
}

dlist_t *
dlist_rm_front(dlist_t *head)
{
    dlist_t     *dlist = NULL;

    assert(head != NULL);

    if (!dlist_empty(head)) {
        dlist = head->next;

        head->next = dlist->next;
        head->next->prev = head;
    }

    return (dlist);
}

dlist_t *
dlist_rm_back(dlist_t *head)
{
    dlist_t     *dlist = NULL;

    assert(head != NULL);

    if (!dlist_empty(head)) {
        dlist = head->prev;

        head->prev = dlist->prev;
        head->prev->next = head;
    }

    return (dlist);
}
