/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2002-2022. All rights reserved.
 * Description: Part of KML SCADSS library;
 */

#ifndef KML_SCADSS_H_INCLUDED
#define KML_SCADSS_H_INCLUDED

#include <stdint.h>
#include <mpi.h>
#include "kml_export.h"
#include "kml_service.h"
#include "kml_scasolver_defs.h"

#if defined(__cplusplus)
extern "C" {
#endif

//------------------------------------------------------------------------------

/* Init Scadss handle for sparse matrix */
#define DECLARE_INIT(name, Atype, Itype)                                                      \
    KML_EXPORT int name(            /**/                                                      \
        KmlScasolverTask **pHandle, /* Pointer to Scadss handle */                            \
        Itype n,                    /* Matrix A size */                                       \
        Atype *a,                   /* Elements of matrix A in CSR format */                  \
        Itype *ja,                  /* Columns for each elements of matrix A in CSR format */ \
        Itype *ia,                  /* Index of each row of matrix A in CSR format */         \
        MPI_Comm comm               /* MPI communicator handle */                             \
    )

/* SPD */
DECLARE_INIT(KmlScadssSpdInitSI, float, int);
DECLARE_INIT(KmlScadssSpdInitDI, double, int);
DECLARE_INIT(KmlScadssHpdInitCI, kml_complex_float, int);
DECLARE_INIT(KmlScadssHpdInitZI, kml_complex_double, int);

#undef DECLARE_INIT

//------------------------------------------------------------------------------

/* Analyze matrix */
/* SPD */
KML_EXPORT int KmlScadssSpdAnalyzeSI(KmlScasolverTask **pHandle);
KML_EXPORT int KmlScadssSpdAnalyzeDI(KmlScasolverTask **pHandle);
KML_EXPORT int KmlScadssHpdAnalyzeCI(KmlScasolverTask **pHandle);
KML_EXPORT int KmlScadssHpdAnalyzeZI(KmlScasolverTask **pHandle);

//------------------------------------------------------------------------------

/* Factorize matrix */
/* SPD */
KML_EXPORT int KmlScadssSpdFactorizeSI(KmlScasolverTask **pHandle);
KML_EXPORT int KmlScadssSpdFactorizeDI(KmlScasolverTask **pHandle);
KML_EXPORT int KmlScadssHpdFactorizeCI(KmlScasolverTask **pHandle);
KML_EXPORT int KmlScadssHpdFactorizeZI(KmlScasolverTask **pHandle);

//------------------------------------------------------------------------------
/* Solve factorized sparse matrix */
#define DECLARE_SOLVE(name, Atype, Itype)                            \
    KML_EXPORT int name(            /**/                             \
        KmlScasolverTask **pHandle, /* Pointer to Scadss handle */   \
        Itype nb,                   /* Number of right-hand sides */ \
        Atype *x,                   /* Solutions */                  \
        Itype ldx,                  /* Leading dimension of x */     \
        Atype *b,                   /* Right-hand sides */           \
        Itype ldb                   /* Leading dimension of b */     \
    )

/* SPD */
DECLARE_SOLVE(KmlScadssSpdSolveSI, float, int);
DECLARE_SOLVE(KmlScadssSpdSolveDI, double, int);
DECLARE_SOLVE(KmlScadssHpdSolveCI, kml_complex_float, int);
DECLARE_SOLVE(KmlScadssHpdSolveZI, kml_complex_double, int);

#undef DECLARE_SOLVE

//------------------------------------------------------------------------------

/* Clean handle */
/* SPD */
KML_EXPORT int KmlScadssSpdCleanSI(KmlScasolverTask **pHandle);
KML_EXPORT int KmlScadssSpdCleanDI(KmlScasolverTask **pHandle);
KML_EXPORT int KmlScadssHpdCleanCI(KmlScasolverTask **pHandle);
KML_EXPORT int KmlScadssHpdCleanZI(KmlScasolverTask **pHandle);

//------------------------------------------------------------------------------

/* Get an integer parameter */
#define DECLARE_GET(name, Atype, Itype)                                                                               \
    KML_EXPORT int name(            /**/                                                                              \
        KmlScasolverTask **pHandle, /* Pointer to Scadss handle */                                                    \
        enum KmlSolverParam param,  /* Parameter enum */                                                              \
        Atype *value,               /* Returned value of parameter */                                                 \
        Itype nvalue                /* Capacity of `value` array in elements (ignored if `param` refers to scalar) */ \
    )

DECLARE_GET(KmlScadssSpdGetSIL, int64_t, int);
DECLARE_GET(KmlScadssSpdGetDIL, int64_t, int);
DECLARE_GET(KmlScadssHpdGetCIL, int64_t, int);
DECLARE_GET(KmlScadssHpdGetZIL, int64_t, int);

DECLARE_GET(KmlScadssSpdGetSII, int, int);
DECLARE_GET(KmlScadssSpdGetDII, int, int);
DECLARE_GET(KmlScadssHpdGetCII, int, int);
DECLARE_GET(KmlScadssHpdGetZII, int, int);

DECLARE_GET(KmlScadssSpdGetDID, double, int);
DECLARE_GET(KmlScadssHpdGetZID, double, int);

DECLARE_GET(KmlScadssSpdGetSIS, float, int);
DECLARE_GET(KmlScadssHpdGetCIS, float, int);

#undef DECLARE_GET

//------------------------------------------------------------------------------

/* Set an integer parameter */
#define DECLARE_SET(name, Atype, Itype)                                                                              \
    KML_EXPORT int name(            /**/                                                                             \
        KmlScasolverTask **pHandle, /* Pointer to Scadss handle */                                                   \
        enum KmlSolverParam param,  /* Selected parameter to set */                                                  \
        const Atype *data,          /* Pointer to data */                                                            \
        Itype nvalue                /* Capacity of `data` array in elements (ignored if `param` refers to scalar) */ \
    )

DECLARE_SET(KmlScadssSpdSetSII, int, int);
DECLARE_SET(KmlScadssSpdSetDII, int, int);
DECLARE_SET(KmlScadssHpdSetCII, int, int);
DECLARE_SET(KmlScadssHpdSetZII, int, int);

DECLARE_SET(KmlScadssSpdSetDID, double, int);
DECLARE_SET(KmlScadssHpdSetZID, double, int);

DECLARE_SET(KmlScadssSpdSetSIS, float, int);
DECLARE_SET(KmlScadssHpdSetCIS, float, int);

#undef DECLARE_SET

//------------------------------------------------------------------------------

#if defined(__cplusplus)
}
#endif

#endif // KML_SCADSS_H_INCLUDED
