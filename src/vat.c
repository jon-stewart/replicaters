#include <replicaters.h>
#include <string.h>

void *vat = NULL;

void
vat_init(void)
{
    vat = malloc(VAT_SIZE);
    assert(vat != NULL);
}

void
vat_destroy(void)
{
    assert(vat != NULL);
    free(vat);
}
