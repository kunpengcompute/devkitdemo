/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2002-2022. All rights reserved.
 * Description: Part of KML SCADSS library;
 */

#ifndef KML_SCASOLVER_H_INCLUDED
#define KML_SCASOLVER_H_INCLUDED

#include "kml_service.h"
#include "kml_scasolver_defs.h"

#include "kml_scadss.h"
#include "kml_scaiss.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct KMLVersion KSCASOLVERVersion;
/** Obtain information about solver library
 *
 * @param ver Pointer to `KSCASOLVERVersion` structure
 * @return 0 if success, nonzero otherwise
 */
KML_EXPORT int KSCASOLVERGetVersion(KSCASOLVERVersion *ver);

#if defined(__cplusplus)
}
#endif

#endif // KML_SCASOLVER_H_INCLUDED
