/*******************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2022. All rights reserved.
 * Description: Part of KML library
 * Author: KML
 * Create: 2020
 ******************************************************************************/

#ifndef KML_DSS_H_INCLUDED
#define KML_DSS_H_INCLUDED

#include <stdint.h>
#include "kml_solver_defs.h"
#include "kml_export.h"
#include "kml_service.h"

#if defined(__cplusplus)
extern "C" {
#endif

// -----------------------------------------------------------------------------

/* Init DSS handle for sparse matrix */
#define DECLARE_INIT(name, Atype, Itype)                                                   \
    KML_EXPORT int name(         /**/                                                      \
        KmlSolverTask **pHandle, /* Pointer to DSS solver handle placeholder */            \
        Itype n,                 /* Matrix A size */                                       \
        Atype *a,                /* Elements of matrix A in CSR format */                  \
        Itype *ja,               /* Columns for each elements of matrix A in CSR format */ \
        Itype *ia                /* Index of each row of matrix A in CSR format */         \
    )

DECLARE_INIT(KmlDssSymInitSI, float, int);
DECLARE_INIT(KmlDssSymInitDI, double, int);
DECLARE_INIT(KmlDssSymInitCI, kml_complex_float, int);
DECLARE_INIT(KmlDssSymInitZI, kml_complex_double, int);
DECLARE_INIT(KmlDssGenInitSI, float, int);
DECLARE_INIT(KmlDssGenInitDI, double, int);
DECLARE_INIT(KmlDssGenInitCI, kml_complex_float, int);
DECLARE_INIT(KmlDssGenInitZI, kml_complex_double, int);

#undef DECLARE_INIT

// -----------------------------------------------------------------------------

/* Analyze sparse matrix */
KML_EXPORT int KmlDssSymAnalyzeSI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssSymAnalyzeDI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssSymAnalyzeCI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssSymAnalyzeZI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssGenAnalyzeSI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssGenAnalyzeDI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssGenAnalyzeCI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssGenAnalyzeZI(KmlSolverTask **pHandle);

// -----------------------------------------------------------------------------

/* Factorize sparse matrix */
KML_EXPORT int KmlDssSymFactorizeSI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssSymFactorizeDI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssSymFactorizeCI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssSymFactorizeZI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssGenFactorizeSI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssGenFactorizeDI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssGenFactorizeCI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssGenFactorizeZI(KmlSolverTask **pHandle);
// -----------------------------------------------------------------------------

/* Solve factorized sparse matrix */
#define DECLARE_SOLVE(name, Atype, Itype)                         \
    KML_EXPORT int name(         /**/                             \
        KmlSolverTask **pHandle, /* Pointer to DSS handle */      \
        Itype nb,                /* Number of right-hand sides */ \
        Atype *x,                /* Solutions */                  \
        Itype ldx,               /* Leading dimension of x */     \
        Atype *b,                /* Right-hand sides */           \
        Itype ldb                /* Leading dimension of b */     \
    )

DECLARE_SOLVE(KmlDssSymSolveSI, float, int);
DECLARE_SOLVE(KmlDssSymSolveDI, double, int);
DECLARE_SOLVE(KmlDssSymSolveCI, kml_complex_float, int);
DECLARE_SOLVE(KmlDssSymSolveZI, kml_complex_double, int);
DECLARE_SOLVE(KmlDssGenSolveSI, float, int);
DECLARE_SOLVE(KmlDssGenSolveDI, double, int);
DECLARE_SOLVE(KmlDssGenSolveCI, kml_complex_float, int);
DECLARE_SOLVE(KmlDssGenSolveZI, kml_complex_double, int);

#undef DECLARE_SOLVE

// -----------------------------------------------------------------------------

/* Clean handle */
KML_EXPORT int KmlDssSymCleanSI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssSymCleanDI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssSymCleanCI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssSymCleanZI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssGenCleanSI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssGenCleanDI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssGenCleanCI(KmlSolverTask **pHandle);
KML_EXPORT int KmlDssGenCleanZI(KmlSolverTask **pHandle);

// -----------------------------------------------------------------------------

/* Get an parameter for matrix */
#define DECLARE_GET(name, Atype, Itype)                                                                              \
    KML_EXPORT int name(           /**/                                                                              \
        KmlSolverTask **pHandle,   /* Pointer to DSS handle */                                                       \
        enum KmlSolverParam param, /* Parameter enum */                                                              \
        Atype *value,              /* Returned value of parameter */                                                 \
        Itype nvalue               /* Capacity of `value` array in elements (ignored if `param` refers to scalar) */ \
    )

DECLARE_GET(KmlDssSymGetSIL, int64_t, int);
DECLARE_GET(KmlDssSymGetDIL, int64_t, int);
DECLARE_GET(KmlDssSymGetCIL, int64_t, int);
DECLARE_GET(KmlDssSymGetZIL, int64_t, int);
DECLARE_GET(KmlDssGenGetSIL, int64_t, int);
DECLARE_GET(KmlDssGenGetDIL, int64_t, int);
DECLARE_GET(KmlDssGenGetCIL, int64_t, int);
DECLARE_GET(KmlDssGenGetZIL, int64_t, int);

DECLARE_GET(KmlDssSymGetSII, int, int);
DECLARE_GET(KmlDssSymGetDII, int, int);
DECLARE_GET(KmlDssSymGetCII, int, int);
DECLARE_GET(KmlDssSymGetZII, int, int);
DECLARE_GET(KmlDssGenGetSII, int, int);
DECLARE_GET(KmlDssGenGetDII, int, int);
DECLARE_GET(KmlDssGenGetCII, int, int);
DECLARE_GET(KmlDssGenGetZII, int, int);

DECLARE_GET(KmlDssSymGetDID, double, int);
DECLARE_GET(KmlDssSymGetZID, double, int);
DECLARE_GET(KmlDssGenGetDID, double, int);
DECLARE_GET(KmlDssGenGetZID, double, int);

DECLARE_GET(KmlDssSymGetSIS, float, int);
DECLARE_GET(KmlDssSymGetCIS, float, int);
DECLARE_GET(KmlDssGenGetSIS, float, int);
DECLARE_GET(KmlDssGenGetCIS, float, int);

#undef DECLARE_GET

// -----------------------------------------------------------------------------

/* Set an integer parameter for matrix */
#define DECLARE_SET(name, Atype, Itype)                                                                             \
    KML_EXPORT int name(           /**/                                                                             \
        KmlSolverTask **pHandle,   /* Pointer to DSS handle */                                                      \
        enum KmlSolverParam param, /* Selected parameter to set */                                                  \
        const Atype *data,         /* Pointer to data */                                                            \
        Itype ndata                /* Capacity of `data` array in elements (ignored if `param` refers to scalar) */ \
    )

DECLARE_SET(KmlDssSymSetSII, int, int);
DECLARE_SET(KmlDssSymSetDII, int, int);
DECLARE_SET(KmlDssSymSetCII, int, int);
DECLARE_SET(KmlDssSymSetZII, int, int);
DECLARE_SET(KmlDssGenSetSII, int, int);
DECLARE_SET(KmlDssGenSetDII, int, int);
DECLARE_SET(KmlDssGenSetCII, int, int);
DECLARE_SET(KmlDssGenSetZII, int, int);

DECLARE_SET(KmlDssSymSetDID, double, int);
DECLARE_SET(KmlDssSymSetZID, double, int);
DECLARE_SET(KmlDssGenSetDID, double, int);
DECLARE_SET(KmlDssGenSetZID, double, int);

DECLARE_SET(KmlDssSymSetSIS, float, int);
DECLARE_SET(KmlDssSymSetCIS, float, int);
DECLARE_SET(KmlDssGenSetSIS, float, int);
DECLARE_SET(KmlDssGenSetCIS, float, int);

#undef DECLARE_SET

// -----------------------------------------------------------------------------

#if defined(__cplusplus)
}
#endif

#endif // KML_DSS_H_INCLUDED
