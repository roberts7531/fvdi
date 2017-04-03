#include <stdlib.h>
#include <mint/osbind.h>
#include <stdint.h>
#include <driver_vec.h>

/*
 * BaS_gcc driver API backdoor.
 *
 * This works similar (but not identical) to TOS' XBRA protocol. The BaS' side of the API reacts on trap #0 exceptions
 * and checks for a valid signature word ("_BAS") immediately before the trap instruction. If the signature word
 * matches, BaS_gcc returns the API call's result (the address of the internal driver table) in d0.
 * If it's not detected, the call gets forwarded to TOS as normal trap #0 exception.
 */
struct driver_table *get_bas_drivers(void)
{
    struct driver_table *ret = NULL;

    __asm__ __volatile__(
        "   bra.s   1f              \n\t"
        "   .dc.l   0x5f424153      \n\t"       /* "_BAS" - BaS_gcc will use to identify this as valid call, */
        "1: trap    #0              \n\t"       /* otherwise it just gets passed to the TOS trap 0 vector */
        "   move.l  d0,%[ret]       \n\t"
        : [ret] "=m" (ret)  /* output */
        :                   /* no inputs */
        :                   /* clobbered */
    );
    return ret;
}


/*
 * temporarily replace the trap 0 handler with this so we can avoid
 * getting caught by BaS versions that don't understand the driver interface
 * exposure call.
 * If we get here, we have a BaS version that doesn't support the trap 0 interface
 */
static void __attribute__((interrupt)) trap0_catcher(void)
{
    __asm__ __volatile__(
        "       clr.l       d0              \n\t"       // return 0 to indicate not supported
        :
        :
        :
    );
}


long get_driver(void)
{
    struct driver_table *dt;
    void *old_vector;
    char **sysbase = (char **) 0x4f2;
    unsigned long sig;

    /*
     * check if we are on EmuTOS; FireTOS does not provide this interface
     */
    sig = * (long *) ((*sysbase) + 0x2c);


    if (sig == 0x45544f53)  /* "ETOS" */
    {
        /*
         * yes, we found the EmuTOS signature
         */

        old_vector = Setexc(0x20, trap0_catcher);			/* catch trap #0 to avoid crash */
        dt = get_bas_drivers();								/* trap #0 */
        (void) Setexc(0x20, old_vector);					/* set vector to what it was before */

        if (dt)
        {
            struct generic_interface *ifc = &dt->interfaces[0];

            /*
            printf("BaS driver table found at %p, BaS version is %d.%d\r\n", dt,
                    dt->bas_version, dt->bas_revision);
            */

            while (ifc->type != END_OF_DRIVERS)
            {
                /*
                printf("driver\"%s (%s)\" found,\r\n"
                        "interface type is %d (%s),\r\n"
                        "version %d.%d\r\n\r\n",
                        ifc->name, ifc->description, ifc->type, dt_to_str(ifc->type),
                        ifc->version, ifc->revision);
                */

                if (ifc->type == VIDEO_DRIVER)
                {
                    /*
                    printf("\r\nvideo driver found at %p\r\n", ifc);
                    */

                    return (long) ifc->interface.fb;
                }
                ifc++;
            }
        }
        else
        {
            /*
            printf("driver table not found.\r\n");
            */
        }
    }
    else
    {
        /*
        printf("not running on EmuTOS,\r\n(signature 0x%08x instead of 0x%08x\r\n",
                (uint32_t) sig, 0x45544f53);
        */
    }
    return 0L;
}
