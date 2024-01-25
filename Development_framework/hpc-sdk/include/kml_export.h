/*******************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
 * Description: Part of KML library
 ******************************************************************************/

#ifndef KML_EXPORT_H_INCLUDED
#define KML_EXPORT_H_INCLUDED

#ifdef KML_STATIC_DEFINE
#define KML_EXPORT
#define KML_NO_EXPORT
#else
#ifndef KML_EXPORT
#define KML_EXPORT __attribute__((visibility("default")))
#endif
#endif

#ifndef KML_NO_EXPORT
#define KML_NO_EXPORT __attribute__((visibility("hidden")))
#endif

#ifndef KML_DEPRECATED
#define KML_DEPRECATED __attribute__((__deprecated__))
#endif

#ifndef KML_DEPRECATED_EXPORT
#define KML_DEPRECATED_EXPORT KML_EXPORT KML_DEPRECATED
#endif

#ifndef KML_DEPRECATED_NO_EXPORT
#define KML_DEPRECATED_NO_EXPORT KML_NO_EXPORT KML_DEPRECATED
#endif

#endif
