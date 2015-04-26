#include <replicaters.h>
#include <string.h>

/*
 * Function:
 *      reg_cb
 *
 * Description:
 *      Registration callback from germs after successful replication.
 *
 *      Add to scum list for future spawning.
 */
static void
reg_cb(void *addr, size_t len, unsigned long gen)
{
    debug("[%s] germ:%p, len:0x%lx, gen:0x%lx\n", __FUNCTION__, addr, (unsigned long) len, gen);

    assert(addr != NULL);
    assert(len > 0);

    vat_scum_add(addr, len, gen);
}

/*
 * Function:
 *      debug_cb
 *
 * Description:
 *      Address of this function is passed as argument to germs.  If/when
 *      called from the germ this func will print out the interesting
 *      registers.
 */
static void
debug_cb(void)
{
    unsigned long rax;
    unsigned long rbx;
    unsigned long rcx;
    unsigned long rdx;
    unsigned long rdi;
    unsigned long rsi;

    asm("mov %0, rax;"
        :"=r"(rax)::"rax");
    asm("mov %0, rbx;"
        :"=r"(rbx)::"rbx");
    asm("mov %0, rcx;"
        :"=r"(rcx)::"rcx");
    asm("mov %0, rdx;"
        :"=r"(rdx)::"rdx");
    asm("mov %0, rdi;"
        :"=r"(rdi)::"rdi");
    asm("mov %0, rsi;"
        :"=r"(rsi)::"rsi");

    debug("[%s] -----------------\n", __FUNCTION__);
    debug("rax: 0x%lx\n", rax);
    debug("rbx: 0x%lx\n", rbx);
    debug("rcx: 0x%lx\n", rcx);
    debug("rdx: 0x%lx\n", rdx);
    debug("rdi: 0x%lx\n", rdi);
    debug("rsi: 0x%lx\n", rsi);
    debug("----------------------------\n\n");
}

/*
 * Function:
 *      spawn
 *
 * Description:
 *      Thread function for passing execution to germ code in vat pool memory.
 */
void *
spawn(void *arg)
{
    germ_t  *germ = (germ_t *) arg;

    /* Dead germs should have been reaped */
    assert(germ->dead == false);
    assert(germ->tid  == 0);

    germ->tid = pthread_self();

    debug("[%s] entry:%p\n", __FUNCTION__, germ->entry);

    if (germ->entry((void *) reg_cb, (void *) debug_cb) == 0) {
        germ->tid = 0;

        debug("[%s] exit:%p\n", __FUNCTION__, germ->entry);
    }

    return (NULL);
}

/*
 * Function:
 *      infect
 *
 * Description:
 *      Read the extracted germ binary .text section and write to start of the
 *      vat memory pool.
 */
void
infect(void)
{
    FILE       *file    = NULL;
    char       *buffer  = NULL;
    void       *addr    = NULL;
    size_t      len     = 0;


    file = fopen(GERM_CODE, "rb");

    fseek(file, 0L, SEEK_END);

    len = ftell(file);

    fseek(file, 0L, SEEK_SET);

    buffer = malloc(sizeof(char) * len);

    fread(buffer, len, len, file);

    fclose(file);

    addr = vat_base_address();

    memcpy(addr, buffer, len);

    free(buffer);

    vat_scum_add(addr, len, 0);
}
