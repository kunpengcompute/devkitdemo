/*******************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Part of KML library
 * Author: KML
 * Create: 2020
 ******************************************************************************/

#ifndef KML_SOLVER_H_INCLUDED
#define KML_SOLVER_H_INCLUDED

#include "kml_service.h"
#include "kml_solver_defs.h"

#include "kml_dss.h"
#include "kml_iss.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct KMLVersion KSOLVERVersion;
/** Obtain information about solver library
 *
 * @param ver Pointer to `KSOLVERVersion` structure
 * @return 0 if success, nonzero otherwise
 */
KML_EXPORT int KSOLVERGetVersion(KSOLVERVersion *ver);

#ifdef __cplusplus
}
#endif

#endif
