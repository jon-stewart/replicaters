#include <stdlib.h>
#include <assert.h>
#include <list.h>

void
list_init(list_t *list)
{
    list->next = list->prev = list;
}

void
list_add(list_t *head, list_t *list)
{
    list_t     *prev;

    assert(head != NULL);
    assert(list != NULL);

    prev = head->prev;

    /* if list is empty */
    if (head->prev == head) {
        head->next = list;
    } else {
        prev->next = list;
    }

    head->prev = list;

    list->prev = prev;
    list->next = head;
}

list_t *
list_rm_front(list_t *head)
{
    list_t     *list = NULL;

    assert(head != NULL);

    if (!list_empty(head)) {
        list = head->next;

        head->next = list->next;
        head->next->prev = head;
    }

    return (list);
}

list_t *
list_rm_back(list_t *head)
{
    list_t     *list = NULL;

    assert(head != NULL);

    if (!list_empty(head)) {
        list = head->prev;

        head->prev = list->prev;
        head->prev->next = head;
    }

    return (list);
}
