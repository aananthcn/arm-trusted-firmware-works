/*
 Author: Aananth C N
 Date: 13 Jun 2020
 License: GPLv2
*/

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>


#include "smc_call.h"

/*
 * W0:
 *  Bit 31      - type (i.e., FAST / YIELDING)
 *  Bit 30      - set to 1 (SMC64/HVC64)
 *  Bit 29:24   - servId (i.e., the service call types)
 *  Bit 23:16   - 0's
 *  Bit 15:0    - funId corresponding to service
 *
 * argc         - number of uint64_t data in argv (not to exceed SMC64_ARG_LEN)
 * argv         - uing64_t data to be passed to x 
 * sessId       - Optional, passed as W6 in X6 register
 * clientId     - Optional, passed as bits[15:0] to W7
 * osId         - Optional, passed as bits[31:16] to W7
 */
int smc(int bit, smc_t type, smc_serv_t servId, uint16_t funId, int argc, uint64_t *argv,
          uint16_t sessId, uint16_t clientId, uint16_t osId, uint64_t retv[])
{
    uint64_t x0_reg, x6_reg, x7_reg;
    uint64_t smc_args[SMC64_ARG_LEN-1];
    int i, bit_mask;

    /* check if users pass arguments without understanding SMCCC rules */
    if (argc > SMC64_ARG_LEN) {
        printk(KERN_ALERT "Error: %d arguments passed, %s() can take upto %d arguments only!",
               argc, __func__, SMC64_ARG_LEN);
        return -1;
    }

    /* check if the return value pointer is valid or not */
    if (retv == NULL) {
        printk(KERN_ALERT "Error: return value pointer (last argument) is NULL!\n");
        return -1;
    }

    /* check other inputs */
    if (type >= SMC_TYPE_MAX) {
        printk(KERN_ALERT "Error: Invalid SMC Call type: %d\n", (int) type);
        return -1;
    }
    if (servId >= SERV_CALL_MAX) {
        printk(KERN_ALERT "Error: Invalid SMC Service ID: %d\n", (int) type);
        return -1;
    }

    /* prepare different fields as per SMCCC rules */
    bit_mask =  (bit == 64) ? (1 << 30) : 0;
    x0_reg = type << 31 | bit_mask | (servId & 0xFFFF) << 24 | (funId & 0xFFFF);
    if (argc >= SMC64_ARG_LEN)
        x6_reg = 0;
    else
        x6_reg = (uint64_t) sessId;
    x7_reg = ((osId << 16) & 0xFFFF0000) | (0xFFFF & clientId);

    /* prepare arguments to the SMC call */
    for (i = 0; i < (SMC64_ARG_LEN-1); ++i) {
        if (i < argc) {
            smc_args[i] = argv[i];
        }
        else {
            smc_args[i] = 0;
        }
    }

    printk(KERN_NOTICE "x0_reg = 0x%016llX\n", x0_reg);

    /* AArch64 assembly instructions - before SMC call */
    asm("STP X4, X5, [SP, #-16]!"); /* push X4, X5 to stack */
    asm("STP X6, X7, [SP, #-16]!"); /* push X6, X7 to stack */
    asm("MOV X7, %[x_7]" :: [x_7] "r" (x7_reg));
    asm("MOV X6, %[x_6]" :: [x_6] "r" (x6_reg));
    asm("MOV X5, %[x_5]" :: [x_5] "r" (smc_args[4]));
    asm("MOV X4, %[x_4]" :: [x_4] "r" (smc_args[3]));
    asm("MOV X3, %[x_3]" :: [x_3] "r" (smc_args[2]));
    asm("MOV X2, %[x_2]" :: [x_2] "r" (smc_args[1]));
    asm("MOV X1, %[x_1]" :: [x_1] "r" (smc_args[0]));
    asm("MOV X0, %[x_0]" :: [x_0] "r" (x0_reg));

    /* The SMC call */
    asm("SMC #0");

    /* AArch64 assembly instructions - after SMC call */
    asm("MOV %[x_0], X0" : [x_0] "=r" (retv[0]) :);
    asm("MOV %[x_1], X1" : [x_1] "=r" (retv[1]) :);
    asm("MOV %[x_2], X2" : [x_2] "=r" (retv[2]) :);
    asm("MOV %[x_3], X3" : [x_3] "=r" (retv[3]) :);
    asm("LDP X6, X7, [SP], #16"); /* pop X6, X7 to stack */
    asm("LDP X4, X5, [SP], #16"); /* pop X4, X5 to stack */

    return 0;
}

int smc64(smc_t type, smc_serv_t servId, uint16_t funId, int argc, uint64_t *argv,
          uint16_t sessId, uint16_t clientId, uint16_t osId, uint64_t retv[])
{
    return smc(64, type, servId, funId, argc, argv, sessId, clientId, osId, retv);
}

int smc32(smc_t type, smc_serv_t servId, uint16_t funId, int argc, uint64_t *argv,
          uint16_t sessId, uint16_t clientId, uint16_t osId, uint64_t retv[])
{
    return smc(32, type, servId, funId, argc, argv, sessId, clientId, osId, retv);
}
