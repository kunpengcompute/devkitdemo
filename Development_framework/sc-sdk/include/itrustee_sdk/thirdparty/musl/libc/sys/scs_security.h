/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020 ~ 2020. All rights reserved.
 * Description: Interface declaration for scs
 * Author: Huawei OS Kernel Lab
 * Create: Tur Fri 9 18:36:09 2023
 */

#if defined(CONFIG_ARM64_SCS) && defined(__aarch64__)
bool dl_is_scs_open(void *handle);
int pthread_attr_enable_shadow_call_stack(pthread_attr_t *a);
#endif
