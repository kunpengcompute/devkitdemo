/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 */

#ifndef UCG_VERSION_H_
#define UCG_VERSION_H_

/* Macros related to version information. */
#define UCG_VERSION(_major, _minor, _patch)  \
    (((_major) << UCG_VERSION_MAJOR_SHIFT) | \
     ((_minor) << UCG_VERSION_MINOR_SHIFT) | \
     ((_patch) << UCG_VERSION_PATCH_SHIFT))

#define UCG_VERSION_MAJOR_SHIFT    10
#define UCG_VERSION_MINOR_SHIFT    10
#define UCG_VERSION_PATCH_SHIFT    12

#define UCG_API_MAJOR    1
#define UCG_API_MINOR    3
#define UCG_API_PATCH    1
#define UCG_API_VERSION  UCG_VERSION(1, 3, 1)
#define UCG_API_VERSION_STR "1.3.1"

#endif
