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

void
list_rm(list_t *ele)
{
    list_t *prev = ele->prev;
    list_t *next = ele->next;

    prev->next = next;
    next->prev = prev;

    ele->prev = NULL;
    ele->next = NULL;
}

static void
list_insert(list_t *prev, list_t *list, list_t *next)
{
    prev->next = list;
    list->prev = prev;
    list->next = next;
    next->prev = list;
}

void
list_add_sorted(list_t *head, list_t *list, sort_fn func)
{
    list_t *ptr  = NULL;
    list_t *prev = NULL;
    int     loc  = 0;

    assert(head != NULL);
    assert(list != NULL);
    assert(func != NULL);

    if (list_empty(head)) {
        list_add(head, list);
        return;
    }

    prev = head;

    LIST_FOR_EACH(head, ptr) {
        loc = func(ptr, list);
        if (loc == -1) {
            /* Less than */
            list_insert(prev, list, ptr);
            break;
        } else if (loc == 0) {
            /* Equal to */
            list_insert(prev, list, ptr);
            break;
        } else if (loc == +1) {
            /* Greater than */
            prev = ptr;
        }
    }
}
