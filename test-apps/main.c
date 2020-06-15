
/*
 Author: Aananth C N
 Date: 12 Jun 2020
 License: GPLv2
*/
#include <stdio.h>
#include <stdint.h>

#include "smc_call.h"

#define SMC_TEST_VERS (0.0)
#define SMC_TEST_DATE ("12 Jun 2020")

void print_retval(uint64_t *retval) {
    for (int i = 0; i < 4; i++) {
        printf("SMC retval[%d] = 0x%16X\n", i, retval[i]);
    }
    printf("\n");
}

void main(void)
{
    printf("SMC test version: %f (%s)\n", SMC_TEST_VERS, SMC_TEST_DATE);
    uint64_t ret_val[4];
    uint64_t fun_arg[6] = {0,0,0,0,0,0};

    /* make a SERV_CALL_ARM_ARCH */
    smc64(SMC_TYPE_FAST, SERV_CALL_ARM_ARCH, 0, 6, fun_arg, 0, 0, 0, ret_val);
    print_retval(ret_val);

    /* make a SERV_CALL_CPU */
    smc64(SMC_TYPE_FAST, SERV_CALL_CPU, 0, 6, fun_arg, 0, 0, 0, ret_val);
    print_retval(ret_val);

    /* make a SERV_CALL_SIP */
    smc64(SMC_TYPE_FAST, SERV_CALL_SIP, 0, 6, fun_arg, 0, 0, 0, ret_val);
    print_retval(ret_val);

    /* make a SERV_CALL_OEM */
    smc64(SMC_TYPE_FAST, SERV_CALL_OEM, 0, 6, fun_arg, 0, 0, 0, ret_val);
    print_retval(ret_val);
}