#ifndef SMC_CALL_H
#define SMC_CALL_H
/*
 Author: Aananth C N
 Date: 11 Jun 2020
 License: GPLv2
*/

#define SMC64_ARG_LEN   (6)

typedef enum smc_type {
    SMC_TYPE_YIELDING,
    SMC_TYPE_FAST,
    SMC_TYPE_MAX
} smc_t;

typedef enum smc_serv_type {
    SERV_CALL_ARM_ARCH,
    SERV_CALL_CPU,
    SERV_CALL_SIP,
    SERV_CALL_OEM,
    SERV_CALL_STD_SECURE,
    SERV_CALL_STD_HYPERVISOR,
    SERV_CALL_VENDOR_HYPERVISOR,
    SERV_CALL_RESERVED = 7,
    SERV_CALL_TRUSTED_APP = 48,
    SERV_CALL_TRUSTED_OS = 50,
    SERV_CALL_MAX = 64
} smc_serv_t;

#endif