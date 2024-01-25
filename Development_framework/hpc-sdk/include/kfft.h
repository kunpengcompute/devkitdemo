/*
 * @Copyright: Copyright (c) Huawei Technologies Co., Ltd. 2020-2022. All rights reserved.
 * @Description: global api declaration of kml_fft, include single and double precision
 * @Author: kpl
 * @Date: 2020-04-09 18:53:21
 */
#ifndef __KML_FFT_H__
#define __KML_FFT_H__

#include <stdio.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define CPX_ELEM 2

/* common global definition */
#define KML_FFT_FORWARD (-1)
#define KML_FFT_BACKWARD 1

#define KML_FFT_NO_TIMELIMIT (-1.0)

#define KML_FFT_MEASURE 0U
#define KML_FFT_DESTROY_INPUT (1U << 0)
#define KML_FFT_PATIENT (1U << 5)
#define KML_FFT_ESTIMATE (1U << 6)
#define KML_FFT_PRESERVE_INPUT (1U << 4)

enum r2r_kind_general {
    KML_FFT_R2HC = 0, KML_FFT_HC2R = 1, KML_FFT_DHT = 2,
    KML_FFT_REDFT00 = 3, KML_FFT_REDFT01 = 4, KML_FFT_REDFT10 = 5, KML_FFT_REDFT11 = 6,
    KML_FFT_RODFT00 = 7, KML_FFT_RODFT01 = 8, KML_FFT_RODFT10 = 9, KML_FFT_RODFT11 = 10
};

struct iodim_general {
    int n;
    int is;
    int os;
};

struct iodim64_general {
    ptrdiff_t n;
    ptrdiff_t is;
    ptrdiff_t os;
};

typedef struct {
    double r;
    double i;
} kml_fft_complex;

struct kml_fft_plan_s;
typedef struct kml_fft_plan_s *kml_fft_plan;

typedef struct iodim_general kml_fft_iodim;
typedef struct iodim64_general kml_fft_iodim64;

typedef enum r2r_kind_general kml_fft_r2r_kind;

#define VERSION_STRUCT_LEN 100
typedef struct {
    char productName[VERSION_STRUCT_LEN];
    char productVersion[VERSION_STRUCT_LEN];
    char componentName[VERSION_STRUCT_LEN];
    char componentVersion[VERSION_STRUCT_LEN];
    char componentAppendInfo[VERSION_STRUCT_LEN];
    char softwareName[VERSION_STRUCT_LEN];
    char softwareVersion[VERSION_STRUCT_LEN];
}KFFTVersion;
int KFFTGetVersion(KFFTVersion *ver);

/*
 * kml_fft_plan_dft_1d() - 1-d complex-to-complex double precision FFT Interface.
 * @n:          the length of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @sign:       an integer denoting whether a forward or backward FFT transform should be performed.
 *              sign = -1 (KML_FFT_FORWARD) for a forward transform
 *              sign = 1 (KML_FFT_BACKWARD) for a backwards transform
 * @flags:      planning options, unused.
 *
 * Plan a 1-d complex-to-complex FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft().
 */
kml_fft_plan kml_fft_plan_dft_1d(int n,
    kml_fft_complex *in, kml_fft_complex *out,
    int sign, unsigned flags);

/*
 * kml_fft_plan_dft_2d() - 2-d complex-to-complex double precision FFT Interface.
 * @n0:         the length of the first dimension of the FFT sequence.
 * @n1:         the length of the second dimension of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @sign:       an integer denoting whether a forward or backward FFT transform should be performed.
 *              sign = -1 (KML_FFT_FORWARD) for a forward transform
 *              sign = 1 (KML_FFT_BACKWARD) for a backwards transform
 * @flags:      planning options, unused.
 *
 * Plan a 2-d complex-to-complex FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 * * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft().
 */
kml_fft_plan kml_fft_plan_dft_2d(int n0, int n1,
    kml_fft_complex *in, kml_fft_complex *out,
    int sign, unsigned flags);

/*
 * kml_fft_plan_dft_3d() - 3-d complex-to-complex double precision FFT Interface.
 * @n0:         the length of the first dimension of the FFT sequence.
 * @n1:         the length of the second dimension of the FFT sequence.
 * @n2:         the length of the third dimension of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @sign:       an integer denoting whether a forward or backward FFT transform should be performed.
 *              sign = -1 (KML_FFT_FORWARD) for a forward transform
 *              sign = 1 (KML_FFT_BACKWARD) for a backwards transform
 * @flags:      planning options, unused.
 *
 * Plan a 3-d complex-to-complex FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft().
 */
kml_fft_plan kml_fft_plan_dft_3d(int n0, int n1, int n2,
    kml_fft_complex *in, kml_fft_complex *out,
    int sign, unsigned flags);

/*
 * kml_fft_plan_dft() - n-dimensional complex-to-complex double precision FFT Interface.
 * @rank:       the number of dimensions of the FFT problem to be solved. 1<=rank<=3
 * @n:          an array of dimension [rank], contains the size of each dimension of the FFT to be solved.
 *              n[i] >= 1 for i in 0, rank-1
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @sign:       an integer denoting whether a forward or backward FFT transform should be performed.
 *              sign = -1 (KML_FFT_FORWARD) for a forward transform
 *              sign = 1 (KML_FFT_BACKWARD) for a backwards transform
 * @flags:      planning options, unused.
 *
 * Plan a basic FFTW interface call for planning an n-dimensional complex-to-complex FFT operation
 * on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft().
 */
kml_fft_plan kml_fft_plan_dft(int rank, const int *n,
    kml_fft_complex *in, kml_fft_complex *out,
    int sign, unsigned flags);

/*
 * kml_fft_plan_many_dft() - advanced complex-to-complex double precision FFT Interface.
 * @rank:       the number of dimensions of the FFT problem to be solved. 1<=rank<=3
 * @n:          an array of dimension [rank], contains the size of each dimension of the FFT to be solved.
 * @howmany:    the number of multi-dimensional FFT transforms to be computed.
 * @in:         the input sequence on which to perform the FFT.
 * @inembed:    an array of dimension [rank] or NULL.
 *              This array denotes the dimensionality of a potentially larger space of which the input array, in,
 *              dimensions given by n, is a sub array.
 * @istride:    the distance between successive elements of the input data.
 * @idist:      the distance between the start of each of the howmany transforms in the input data.
 * @out:        the output sequence with the resulting transform coefficients.
 * @onembed:    an array of dimension [rank] or NULL.
 *              This array denotes the dimensionality of a potentially larger space of which the output array, in,
 *              dimensions given by n, is a sub array.
 * @ostride:    the distance between successive elements of the output data.
 * @odist:      the distance between the start of each of the howmany transforms in the output data.
 * @sign:       an integer denoting whether a forward or backward FFT transform should be performed.
 *              sign = -1 (KML_FFT_FORWARD) for a forward transform
 *              sign = 1 (KML_FFT_BACKWARD) for a backwards transform
 * @flags:      planning options, unused.
 *
 * An advanced FFTW interface call for planning an n-dimensional complex-to-complex FFT operation
 * on howmany sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft().
 */
kml_fft_plan kml_fft_plan_many_dft(int rank, const int *n, int howmany,
    kml_fft_complex *in, const int *inembed, int istride, int idist,
    kml_fft_complex *out, const int *onembed, int ostride, int odist,
    int sign, unsigned flags);

/*
 * kml_fft_plan_dft_r2c_1d() - 1-d real-to-complex double precision FFT Interface.
 * @n:          the length of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * Plan a 1-d real-to-complex FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft_r2c().
 */
kml_fft_plan kml_fft_plan_dft_r2c_1d(int n, double *in, kml_fft_complex *out, unsigned flags);

/*
 * kml_fft_plan_dft_r2c_2d() - 2-d real-to-complex double precision FFT Interface.
 * @n0:         the length of the first dimension of the FFT sequence.
 * @n1:         the length of the second dimension of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * Plan a 2-d real-to-complex FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft_r2c().
 */
kml_fft_plan kml_fft_plan_dft_r2c_2d(int n0, int n1, double *in, kml_fft_complex *out, unsigned flags);

/*
 * kml_fft_plan_dft_r2c_3d() - 3-d real-to-complex double precision FFT Interface.
 * @n0:         the length of the first dimension of the FFT sequence.
 * @n1:         the length of the second dimension of the FFT sequence.
 * @n2:         the length of the third dimension of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * Plan a 3-d real-to-complex FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft_r2c().
 */
kml_fft_plan kml_fft_plan_dft_r2c_3d(int n0, int n1, int n2, double *in, kml_fft_complex *out, unsigned flags);

/*
 * kml_fft_plan_dft_r2c() - n-dimensional real-to-complex double precision FFT Interface.
 * @rank:       the number of dimensions of the FFT problem to be solved. 1<=rank<=3
 * @n:          an array of dimension [rank], contains the size of each dimension of the FFT to be solved.
 *              n[i] >= 1 for i in 0, rank-1
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * Plan a basic FFTW interface call for planning an n-dimensional real-to-complex FFT operation
 * on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft_r2c().
 */
kml_fft_plan kml_fft_plan_dft_r2c(int rank, const int *n, double *in, kml_fft_complex *out, unsigned flags);

/*
 * kml_fft_plan_many_dft_r2c() - advanced real-to-complex double precision FFT Interface.
 * @rank:       the number of dimensions of the FFT problem to be solved. 1<=rank<=3
 * @n:          an array of dimension [rank], contains the size of each dimension of the FFT to be solved.
 * @howmany:    the number of multi-dimensional FFT transforms to be computed.
 * @in:         the input sequence on which to perform the FFT.
 * @inembed:    an array of dimension [rank] or NULL.
 *              This array denotes the dimensionality of a potentially larger space of which the input array, in,
 *              dimensions given by n, is a sub array.
 * @istride:    the distance between successive elements of the input data.
 * @idist:      the distance between the start of each of the howmany transforms in the input data.
 * @out:        the output sequence with the resulting transform coefficients.
 * @onembed:    an array of dimension [rank] or NULL.
 *              This array denotes the dimensionality of a potentially larger space of which the output array, in,
 *              dimensions given by n, is a sub array.
 * @ostride:    the distance between successive elements of the output data.
 * @odist:      the distance between the start of each of the howmany transforms in the output data.
 * @flags:      planning options, unused.
 *
 * An advanced FFTW interface call for planning an n-dimensional real-to-complex FFT operation
 * on howmany sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft_r2c().
 */
kml_fft_plan kml_fft_plan_many_dft_r2c(int rank, const int *n,
    int howmany,
    double *in, const int *inembed,
    int istride, int idist,
    kml_fft_complex *out, const int *onembed,
    int ostride, int odist,
    unsigned flags);

/*
 * kml_fft_plan_dft_c2r_1d() - 1-d complex-to-real double precision FFT Interface.
 * @n:          the length of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * Plan a 1-d complex-to-real FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft_c2r().
 */
kml_fft_plan kml_fft_plan_dft_c2r_1d(int n, kml_fft_complex *in, double *out, unsigned flags);

/*
 * kml_fft_plan_dft_c2r_2d() - 2-d complex-to-real double precision FFT Interface.
 * @n0:         the length of the first dimension of the FFT sequence.
 * @n1:         the length of the second dimension of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * Plan a 2-d complex-to-real FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft_c2r().
 */
kml_fft_plan kml_fft_plan_dft_c2r_2d(int n0, int n1, kml_fft_complex *in, double *out, unsigned flags);

/*
 * kml_fft_plan_dft_c2r_3d() - 3-d complex-to-real double precision FFT Interface.
 * @n0:         the length of the first dimension of the FFT sequence.
 * @n1:         the length of the second dimension of the FFT sequence.
 * @n2:         the length of the third dimension of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * Plan a 3-d complex-to-real FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft_c2r().
 */
kml_fft_plan kml_fft_plan_dft_c2r_3d(int n0, int n1, int n2, kml_fft_complex *in, double *out, unsigned flags);

/*
 * kml_fft_plan_dft_c2r() - n-dimensional complex-to-real double precision FFT Interface.
 * @rank:       the number of dimensions of the FFT problem to be solved. 1<=rank<=3
 * @n:          an array of dimension [rank], contains the size of each dimension of the FFT to be solved.
 *              n[i] >= 1 for i in 0, rank-1
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * Plan a basic FFTW interface call for planning an n-dimensional complex-to-real FFT operation
 * on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft_c2r().
 */
kml_fft_plan kml_fft_plan_dft_c2r(int rank, const int *n, kml_fft_complex *in, double *out, unsigned flags);

/*
 * kml_fft_plan_many_dft_c2r() - advanced complex-to-real double precision FFT Interface.
 * @rank:       the number of dimensions of the FFT problem to be solved. 1<=rank<=3
 * @n:          an array of dimension [rank], contains the size of each dimension of the FFT to be solved.
 * @howmany:    the number of multi-dimensional FFT transforms to be computed.
 * @in:         the input sequence on which to perform the FFT.
 * @inembed:    an array of dimension [rank] or NULL.
 *              This array denotes the dimensionality of a potentially larger space of which the input array, in,
 *              dimensions given by n, is a sub array.
 * @istride:    the distance between successive elements of the input data.
 * @idist:      the distance between the start of each of the howmany transforms in the input data.
 * @out:        the output sequence with the resulting transform coefficients.
 * @onembed:    an array of dimension [rank] or NULL.
 *              This array denotes the dimensionality of a potentially larger space of which the output array, in,
 *              dimensions given by n, is a sub array.
 * @ostride:    the distance between successive elements of the output data.
 * @odist:      the distance between the start of each of the howmany transforms in the output data.
 * @flags:      planning options, unused.
 *
 * An advanced FFTW interface call for planning an n-dimensional complex-to-real FFT operation
 * on howmany sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft_c2r().
 */
kml_fft_plan kml_fft_plan_many_dft_c2r(int rank, const int *n, int howmany,
    kml_fft_complex *in, const int *inembed, int istride, int idist,
    double *out, const int *onembed, int ostride, int odist,
    unsigned flags);

/*
 * kml_fft_plan_r2r_1d() - 1-d real-to-real double precision FFT Interface.
 * @n:          the length of the the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @kind:       the r2r transform kind of the FFT sequence.
 * @flags:      planning options, unused.
 *
 * Plan a 1-d real-to-real FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_r2r().
 */
kml_fft_plan kml_fft_plan_r2r_1d(int n, double *in, double *out, kml_fft_r2r_kind kind, unsigned flags);

/*
 * kml_fft_plan_r2r_2d() - 2-d real-to-real double precision FFT Interface.
 * @n0:         the length of the first dimension of the FFT sequence.
 * @n1:         the length of the second dimension of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @kind0:      the r2r transform kind of the first dimension of the FFT sequence.
 * @kind1:      the r2r transform kind of the second dimension of the FFT sequence.
 * @flags:      planning options, unused.
 *
 * Plan a 2-d real-to-real FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_r2r().
 */
kml_fft_plan kml_fft_plan_r2r_2d(int n0, int n1, double *in, double *out,
    kml_fft_r2r_kind kind0, kml_fft_r2r_kind kind1, unsigned flags);

/*
 * kml_fft_plan_r2r_3d() - 3-d real-to-real double precision FFT Interface.
 * @n0:         the length of the first dimension of the FFT sequence.
 * @n1:         the length of the second dimension of the FFT sequence.
 * @n2:         the length of the third dimension of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @kind0:      the r2r transform kind of the first dimension of the FFT sequence.
 * @kind1:      the r2r transform kind of the second dimension of the FFT sequence.
 * @kind2:      the r2r transform kind of the third dimension of the FFT sequence.
 * @flags:      planning options, unused.
 *
 * Plan a 3-d real-to-real FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_r2r().
 */
kml_fft_plan kml_fft_plan_r2r_3d(int n0, int n1, int n2,
    double *in, double *out, kml_fft_r2r_kind kind0,
    kml_fft_r2r_kind kind1, kml_fft_r2r_kind kind2,
    unsigned flags);

/*
 * kml_fft_plan_r2r() - n-dimensional real-to-real double precision FFT Interface.
 * @rank:       the number of dimensions of the FFT problem to be solved. 1<=rank<=3
 * @n:          an array of dimension [rank], contains the size of each dimension of the FFT to be solved.
 *              n[i] >= 1 for i in 0, rank-1
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @kind:       an array of dimension [rank], contains the transform kind of each dimension of the FFT to be solved.
 * @flags:      planning options, unused.
 *
 * Plan a basic FFTW interface call for planning an n-dimensional real-to-real FFT operation
 * on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_r2r().
 */
kml_fft_plan kml_fft_plan_r2r(int rank, const int *n,
    double *in, double *out,
    const kml_fft_r2r_kind *kind, unsigned flags);

/*
 * kml_fft_plan_many_r2r() - advanced real-to-real double precision FFT Interface.
 * @rank:       the number of dimensions of the FFT problem to be solved. 1<=rank<=3
 * @n:          an array of dimension [rank], contains the size of each dimension of the FFT to be solved.
 * @howmany:    the number of multi-dimensional FFT transforms to be computed.
 * @in:         the input sequence on which to perform the FFT.
 * @inembed:    an array of dimension [rank] or NULL.
 *              This array denotes the dimensionality of a potentially larger space of which the input array, in,
 *              dimensions given by n, is a sub array.
 * @istride:    the distance between successive elements of the input data.
 * @idist:      the distance between the start of each of the howmany transforms in the input data.
 * @out:        the output sequence with the resulting transform coefficients.
 * @onembed:    an array of dimension [rank] or NULL.
 *              This array denotes the dimensionality of a potentially larger space of which the output array, in,
 *              dimensions given by n, is a sub array.
 * @ostride:    the distance between successive elements of the output data.
 * @odist:      the distance between the start of each of the howmany transforms in the output data.
 * @kind:       an array of dimension [rank], contains the transform kind of each dimension of the FFT to be solved.
 * @flags:      planning options, unused.
 *
 * An advanced FFTW interface call for planning an n-dimensional real-to-real FFT operation
 * on howmany sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_r2r().
 */
kml_fft_plan kml_fft_plan_many_r2r(int rank, const int *n, int howmany,
    double *in, const int *inembed, int istride, int idist,
    double *out, const int *onembed, int ostride, int odist,
    const kml_fft_r2r_kind *kind, unsigned flags);

/*
 * kml_fft_plan_guru_dft() - guru complex-to-complex double precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, of type fftw_complex, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @sign:       an integer denoting whether a forward or backward FFT transform should be performed.
 *              sign = -1 (KML_FFT_FORWARD) for a forward transform
 *              sign = 1 (KML_FFT_BACKWARD) for a backwards transform
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional complex-to-complex FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft().
 */
kml_fft_plan kml_fft_plan_guru_dft(int rank, const kml_fft_iodim *dims,
    int howmany_rank, const kml_fft_iodim *howmany_dims,
    kml_fft_complex *in, kml_fft_complex *out,
    int sign, unsigned flags);

/*
 * kml_fft_plan_guru_split_dft() - guru complex-to-complex double precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, of type fftw_complex, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @ri:         the real parts of the input sequence on which to perform the FFT.
 * @ii:         the imaginary parts of the input sequence on which to perform the FFT.
 * @ro:         the real parts of the output sequence with the resulting transform coefficients.
 * @io:         the imaginary parts of the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional complex-to-complex FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 * Input and output data is stored in separate arrays for real and imaginary parts.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_split_dft().
 */
kml_fft_plan kml_fft_plan_guru_split_dft(int rank, const kml_fft_iodim *dims,
    int howmany_rank, const kml_fft_iodim *howmany_dims,
    double *ri, double *ii, double *ro, double *io,
    unsigned flags);

/*
 * kml_fft_plan_guru64_dft() - guru complex-to-complex double precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the length of the i th dimension.
 *              ptrdiff_t is – the stride between successive elements in the input vector in the i th dimension.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t is – the number of memory elements, of type fftw_complex, between the start of transforms in
 *                  the i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @sign:       an integer denoting whether a forward or backward FFT transform should be performed.
 *              sign = -1 (KML_FFT_FORWARD) for a forward transform
 *              sign = 1 (KML_FFT_BACKWARD) for a backwards transform
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional complex-to-complex FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner. Allows selected parameters to be specified as 64-bit integers.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft().
 */
kml_fft_plan kml_fft_plan_guru64_dft(int rank, const kml_fft_iodim64 *dims,
    int howmany_rank, const kml_fft_iodim64 *howmany_dims,
    kml_fft_complex *in, kml_fft_complex *out,
    int sign, unsigned flags);

/*
 * kml_fft_plan_guru64_split_dft() - guru complex-to-complex double precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the length of the i th dimension.
 *              ptrdiff_t is – the stride between successive elements in the input vector in the i th dimension.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t is – the number of memory elements, of type fftw_complex, between the start of transforms in
 *                  the i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @ri:         the real parts of the input sequence on which to perform the FFT.
 * @ii:         the imaginary parts of the input sequence on which to perform the FFT.
 * @ro:         the real parts of the output sequence with the resulting transform coefficients.
 * @io:         the imaginary parts of the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional complex-to-complex FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner. Allows selected parameters to be specified as 64-bit integers.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_split_dft().
 */
kml_fft_plan kml_fft_plan_guru64_split_dft(int rank, const kml_fft_iodim64 *dims,
    int howmany_rank, const kml_fft_iodim64 *howmany_dims,
    double *ri, double *ii, double *ro, double *io,
    unsigned flags);

/*
 * kml_fft_plan_guru_dft() - guru real-to-complex double precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional real-to-complex FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft_r2c().
 */
kml_fft_plan kml_fft_plan_guru_dft_r2c(int rank, const kml_fft_iodim *dims,
    int howmany_rank,
    const kml_fft_iodim *howmany_dims,
    double *in, kml_fft_complex *out,
    unsigned flags);

/*
 * kml_fft_plan_guru_split_dft_r2c() - guru real-to-complex double precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @ro:         the real parts of the output sequence with the resulting transform coefficients.
 * @io:         the imaginary parts of the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional real-to-complex FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 * Input and output data is stored in separate arrays for real and imaginary parts.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_split_dft_r2c().
 */
kml_fft_plan kml_fft_plan_guru_split_dft_r2c(int rank, const kml_fft_iodim *dims,
    int howmany_rank,
    const kml_fft_iodim *howmany_dims,
    double *in, double *ro, double *io,
    unsigned flags);

/*
 * kml_fft_plan_guru64_dft_r2c() - guru real-to-complex double precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional real-to-complex FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner. Allows selected parameters to be specified as 64-bit integers.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft_r2c().
 */
kml_fft_plan kml_fft_plan_guru64_dft_r2c(int rank, const kml_fft_iodim64 *dims,
    int howmany_rank, const kml_fft_iodim64 *howmany_dims,
    double *in, kml_fft_complex *out,
    unsigned flags);

/*
 * kml_fft_plan_guru64_split_dft_r2c() - guru real-to-complex double precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the length of the i th dimension.
 *              ptrdiff_t is – the stride between successive elements in the input vector in the i th dimension.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @ro:         the real parts of the output sequence with the resulting transform coefficients.
 * @io:         the imaginary parts of the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional real-to-complex FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner. Allows selected parameters to be specified as 64-bit integers.
 * Input and output data is stored in separate arrays for real and imaginary parts.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_split_dft_r2c().
 */
kml_fft_plan kml_fft_plan_guru64_split_dft_r2c(int rank, const kml_fft_iodim64 *dims,
    int howmany_rank, const kml_fft_iodim64 *howmany_dims,
    double *in, double *ro, double *io,
    unsigned flags);

/*
 * kml_fft_plan_guru_dft_c2r() - guru complex-to-real double precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional complex-to-real FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft_c2r().
 */
kml_fft_plan kml_fft_plan_guru_dft_c2r(int rank, const kml_fft_iodim *dims,
    int howmany_rank,
    const kml_fft_iodim *howmany_dims,
    kml_fft_complex *in, double *out,
    unsigned flags);

/*
 * kml_fft_plan_guru_split_dft_c2r() - guru complex-to-real double precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @ri:         the real parts of the input sequence on which to perform the FFT.
 * @ii:         the imaginary parts of the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional complex-to-real FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 * Input and output data is stored in separate arrays for real and imaginary parts.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_split_dft_c2r().
 */
kml_fft_plan kml_fft_plan_guru_split_dft_c2r(int rank, const kml_fft_iodim *dims,
    int howmany_rank,
    const kml_fft_iodim *howmany_dims,
    double *ri, double *ii, double *out,
    unsigned flags);

/*
 * kml_fft_plan_guru64_dft_c2r() - guru complex-to-real double precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the length of the i th dimension.
 *              ptrdiff_t is – the stride between successive elements in the input vector in the i th dimension.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional complex-to-real FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner. Allows selected parameters to be specified as 64-bit integers.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_dft_c2r().
 */
kml_fft_plan kml_fft_plan_guru64_dft_c2r(int rank, const kml_fft_iodim64 *dims,
    int howmany_rank, const kml_fft_iodim64 *howmany_dims,
    kml_fft_complex *in, double *out,
    unsigned flags);

/*
 * kml_fft_plan_guru64_split_dft_c2r() - guru complex-to-real double precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @ri:         the real parts of the input sequence on which to perform the FFT.
 * @ii:         the imaginary parts of the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional complex-to-real FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner. Allows selected parameters to be specified as 64-bit integers.
 * Input and output data is stored in separate arrays for real and imaginary parts.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_split_dft_c2r().
 */
kml_fft_plan kml_fft_plan_guru64_split_dft_c2r(int rank, const kml_fft_iodim64 *dims,
    int howmany_rank, const kml_fft_iodim64 *howmany_dims,
    double *ri, double *ii, double *out,
    unsigned flags);

/*
 * kml_fft_plan_guru_r2r() - guru real-to-real double precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @kind:       an array of dimension [rank], contains the transform kind of each dimension of the FFT to be solved.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional real-to-real FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_r2r().
 */
kml_fft_plan kml_fft_plan_guru_r2r(int rank, const kml_fft_iodim *dims,
    int howmany_rank, const kml_fft_iodim *howmany_dims,
    double *in, double *out,
    const kml_fft_r2r_kind *kind, unsigned flags);

/*
 * kml_fft_plan_guru64_r2r() - guru real-to-real double precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the length of the i th dimension.
 *              ptrdiff_t is – the stride between successive elements in the input vector in the i th dimension.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @kind:       an array of dimension [rank], contains the transform kind of each dimension of the FFT to be solved.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional real-to-real FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner. Allows selected parameters to be specified as 64-bit integers.
 *
 * @return:     plan returned that can be used in subsequent kml_fft_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fft_execute_r2r().
 */
kml_fft_plan kml_fft_plan_guru64_r2r(int rank, const kml_fft_iodim64 *dims,
    int howmany_rank, const kml_fft_iodim64 *howmany_dims,
    double *in, double *out,
    const kml_fft_r2r_kind *kind, unsigned flags);

/*
 * kml_fft_execute() - executes a previously created FFT plan of double precision.
 * @p:          an FFT plan created from a preceding call.
 */
void kml_fft_execute(const kml_fft_plan p);

/*
 * kml_fft_execute_dft() - executes a previously created FFT plan of double precision.
 * @p:          an FFT plan created from a preceding call.
 * @in:         a pointer to a previously allocated array of data, on which to calculate a complex-to-complex FFT.
 * @out:        a pointer to a previously allocated array of data,
 *              into which to return the results of the complex-to-complex FFT.
 *
 * executes a previously created FFT plan, however input and output data area maybe
 *     different than those given at planning time
 */
void kml_fft_execute_dft(const kml_fft_plan p, kml_fft_complex *in, kml_fft_complex *out);

/*
 * kml_fft_execute_dft_r2c() - executes a previously created real-to-complex FFT plan of double precision.
 * @p:          an FFT plan created from a preceding call.
 * @in:         a pointer to a previously allocated array of data, on which to calculate a real-to-complex FFT.
 * @out:        a pointer to a previously allocated array of data,
 *              into which to return the results of the real-to-complex FFT.
 *
 * executes a previously created real-to-complex FFT plan, however input and output data area maybe
 *     different than those given at planning time
 */
void kml_fft_execute_dft_r2c(const kml_fft_plan p, double *in, kml_fft_complex *out);

/*
 * kml_fft_execute_dft_c2r() - executes a previously created complex-to-real FFT plan of double precision.
 * @p:          an FFT plan created from a preceding call.
 * @in:         a pointer to a previously allocated array of data, on which to calculate a complex-to-real FFT.
 * @out:        a pointer to a previously allocated array of data,
 *              into which to return the results of the complex-to-real FFT.
 *
 * executes a previously created complex-to-real FFT plan, however input and output data area maybe
 *     different than those given at planning time
 */
void kml_fft_execute_dft_c2r(const kml_fft_plan p, kml_fft_complex *in, double *out);

/*
 * kml_fft_execute_split_dft() - executes a previously created FFT plan of double precision.
 * @p:          an FFT plan created from a preceding call.
 * @ri:         a previously allocated array of the real parts of data, on which to calculate the FFT.
 * @ii:         a previously allocated array of the imaginary parts of data, on which to calculate the FFT.
 * @ro:         a previously allocated array, into which to return the the real parts of results of the FFT.
 * @io:         a previously allocated array, into which to return the the imaginary parts of results of the FFT.
 *
 * executes a previously created FFT plan, however input and output data area maybe
 *     different than those given at planning time.
 */
void kml_fft_execute_split_dft(const kml_fft_plan p, double *ri, double *ii, double *ro, double *io);

/*
 * kml_fft_execute_split_dft_r2c() - executes a previously created FFT plan of double precision.
 * @p:          an FFT plan created from a preceding call.
 * @in:         a pointer to a previously allocated array of data, on which to calculate a real-to-complex FFT.
 * @ro:         a previously allocated array, into which to return the the real parts of results of the FFT.
 * @io:         a previously allocated array, into which to return the the imaginary parts of results of the FFT.
 *
 * executes a previously created FFT plan, however input and output data area maybe
 *     different than those given at planning time.
 */
void kml_fft_execute_split_dft_r2c(const kml_fft_plan p, double *in, double *ro, double *io);

/*
 * kml_fft_execute_split_dft_c2r() - executes a previously created FFT plan of double precision.
 * @p:          an FFT plan created from a preceding call.
 * @ri:         a previously allocated array of the real parts of data, on which to calculate the FFT.
 * @ii:         a previously allocated array of the imaginary parts of data, on which to calculate the FFT.
 * @out:        a pointer to a previously allocated array of data,
 *              into which to return the results of the complex-to-real FFT.
 *
 * executes a previously created FFT plan, however input and output data area maybe
 *     different than those given at planning time.
 */
void kml_fft_execute_split_dft_c2r(const kml_fft_plan p, double *ri, double *ii, double *out);

/*
 * kml_fft_execute_r2r() - executes a previously created real-to-real FFT plan of double precision.
 * @p:          an FFT plan created from a preceding call.
 * @in:         a pointer to a previously allocated array of data, on which to calculate a real-to-real FFT.
 * @out:        a pointer to a previously allocated array of data,
 *              into which to return the results of the real-to-real FFT.
 *
 * executes a previously created real-to-real FFT plan, however input and output data area maybe
 *     different than those given at planning time
 */
void kml_fft_execute_r2r(const kml_fft_plan p, double *in, double *out);

/*
 * kml_fft_destroy_plan() - frees all memory associated with the creation of an FFT plan.
 * @p:          a pointer to a previously created plan.
 *
 * The plan should not be reused after this call.
 */
void kml_fft_destroy_plan(kml_fft_plan p);

/*
 * kml_fft_plan_with_nthreads() - specify the number of threads used for the execution of FFTs.
 * @nthreads:   the number of threads requested to be used when executing subsequent FFT plans. nthreads>=1.
 */
void kml_fft_plan_with_nthreads(int nthreads);

/*
 * kml_fft_init_threads() - initialise the use of threading in the execution of FFTs.
 *
 * @return:     return 1 means succeess, return 0 means failed.
 */
int kml_fft_init_threads(void);

/*
 * kml_fft_cleanup_threads() - discard memory for use of threading previously.
 */
void kml_fft_cleanup_threads(void);

/*
 * kml_fft_cleanup() - discard memory use previously.
 */
void kml_fft_cleanup(void);

/*
 * kml_fft_malloc() - allocates a block of data of the requested size.
 * @nthreads:   the size, in bytes, of array to be created.
 *
 * return:      a pointer, of type void*, to an allocated block of memory of the requested size
 */
void *kml_fft_malloc(size_t n);

/*
 * kml_fft_free() - frees the memory created in a previous or kml_fft_malloc() call.
 * @p:          a pointer to a previously created array.
 */
void kml_fft_free(void *p);

typedef struct {
    float r;
    float i;
} kml_fftf_complex;

struct kml_fftf_plan_s;
typedef struct kml_fftf_plan_s *kml_fftf_plan;

typedef struct iodim_general kml_fftf_iodim;
typedef struct iodim64_general kml_fftf_iodim64;

typedef enum r2r_kind_general kml_fftf_r2r_kind;

/*
 * kml_fftf_plan_dft_1d() - 1-d complex-to-complex float precision FFT Interface.
 * @n:          the length of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @sign:       an integer denoting whether a forward or backward FFT transform should be performed.
 *              sign = -1 (KML_FFT_FORWARD) for a forward transform
 *              sign = 1 (KML_FFT_BACKWARD) for a backwards transform
 * @flags:      planning options, unused.
 *
 * Plan a 1-d complex-to-complex FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft().
 */
kml_fftf_plan kml_fftf_plan_dft_1d(int n,
    kml_fftf_complex *in, kml_fftf_complex *out,
    int sign, unsigned flags);

/*
 * kml_fftf_plan_dft_2d() - 2-d complex-to-complex float precision FFT Interface.
 * @n0:         the length of the first dimension of the FFT sequence.
 * @n1:         the length of the second dimension of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @sign:       an integer denoting whether a forward or backward FFT transform should be performed.
 *              sign = -1 (KML_FFT_FORWARD) for a forward transform
 *              sign = 1 (KML_FFT_BACKWARD) for a backwards transform
 * @flags:      planning options, unused.
 *
 * Plan a 2-d complex-to-complex FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft().
 */
kml_fftf_plan kml_fftf_plan_dft_2d(int n0, int n1,
    kml_fftf_complex *in, kml_fftf_complex *out,
    int sign, unsigned flags);

/*
 * kml_fftf_plan_dft_3d() - 3-d complex-to-complex float precision FFT Interface.
 * @n0:         the length of the first dimension of the FFT sequence.
 * @n1:         the length of the second dimension of the FFT sequence.
 * @n2:         the length of the third dimension of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @sign:       an integer denoting whether a forward or backward FFT transform should be performed.
 *              sign = -1 (KML_FFT_FORWARD) for a forward transform
 *              sign = 1 (KML_FFT_BACKWARD) for a backwards transform
 * @flags:      planning options, unused.
 *
 * Plan a 3-d complex-to-complex FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft().
 */
kml_fftf_plan kml_fftf_plan_dft_3d(int n0, int n1, int n2,
    kml_fftf_complex *in, kml_fftf_complex *out,
    int sign, unsigned flags);

/*
 * kml_fftf_plan_dft() - n-dimensional complex-to-complex float precision FFT Interface.
 * @rank:       the number of dimensions of the FFT problem to be solved. 1<=rank<=3
 * @n:          an array of dimension [rank], contains the size of each dimension of the FFT to be solved.
 *              n[i] >= 1 for i in 0, rank-1
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @sign:       an integer denoting whether a forward or backward FFT transform should be performed.
 *              sign = -1 (KML_FFT_FORWARD) for a forward transform
 *              sign = 1 (KML_FFT_BACKWARD) for a backwards transform
 * @flags:      planning options, unused.
 *
 * Plan a basic FFTW interface call for planning an n-dimensional complex-to-complex FFT operation
 * on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft().
 */
kml_fftf_plan kml_fftf_plan_dft(int rank, const int *n,
    kml_fftf_complex *in, kml_fftf_complex *out,
    int sign, unsigned flags);

/*
 * kml_fftf_plan_many_dft() - advanced complex-to-complex float precision FFT Interface.
 * @rank:       the number of dimensions of the FFT problem to be solved. 1<=rank<=3
 * @n:          an array of dimension [rank], contains the size of each dimension of the FFT to be solved.
 * @howmany:    the number of multi-dimensional FFT transforms to be computed.
 * @in:         the input sequence on which to perform the FFT.
 * @inembed:    an array of dimension [rank] or NULL.
 *              This array denotes the dimensionality of a potentially larger space of which the input array, in,
 *              dimensions given by n, is a sub array.
 * @istride:    the distance between successive elements of the input data.
 * @idist:      the distance between the start of each of the howmany transforms in the input data.
 * @out:        the output sequence with the resulting transform coefficients.
 * @onembed:    an array of dimension [rank] or NULL.
 *              This array denotes the dimensionality of a potentially larger space of which the output array, in,
 *              dimensions given by n, is a sub array.
 * @ostride:    the distance between successive elements of the output data.
 * @odist:      the distance between the start of each of the howmany transforms in the output data.
 * @sign:       an integer denoting whether a forward or backward FFT transform should be performed.
 *              sign = -1 (KML_FFT_FORWARD) for a forward transform
 *              sign = 1 (KML_FFT_BACKWARD) for a backwards transform
 * @flags:      planning options, unused.
 *
 * An advanced FFTW interface call for planning an n-dimensional complex-to-complex FFT operation
 * on howmany sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft().
 */
kml_fftf_plan kml_fftf_plan_many_dft(int rank, const int *n, int howmany,
    kml_fftf_complex *in, const int *inembed, int istride, int idist,
    kml_fftf_complex *out, const int *onembed, int ostride, int odist,
    int sign, unsigned flags);

/*
 * kml_fftf_plan_dft_r2c_1d() - 1-d real-to-complex float precision FFT Interface.
 * @n:          the length of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * Plan a 1-d real-to-complex FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft_r2c().
 */
kml_fftf_plan kml_fftf_plan_dft_r2c_1d(int n, float *in, kml_fftf_complex *out, unsigned flags);

/*
 * kml_fftf_plan_dft_r2c_2d() - 2-d real-to-complex float precision FFT Interface.
 * @n0:         the length of the first dimension of the FFT sequence.
 * @n1:         the length of the second dimension of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * Plan a 2-d real-to-complex FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft_r2c().
 */
kml_fftf_plan kml_fftf_plan_dft_r2c_2d(int n0, int n1, float *in, kml_fftf_complex *out, unsigned flags);

/*
 * kml_fftf_plan_dft_r2c_3d() - 3-d real-to-complex float precision FFT Interface.
 * @n0:         the length of the first dimension of the FFT sequence.
 * @n1:         the length of the second dimension of the FFT sequence.
 * @n2:         the length of the third dimension of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * Plan a 3-d real-to-complex FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft_r2c().
 */
kml_fftf_plan kml_fftf_plan_dft_r2c_3d(int n0, int n1, int n2, float *in, kml_fftf_complex *out, unsigned flags);

/*
 * kml_fftf_plan_dft_r2c() - n-dimensional real-to-complex float precision FFT Interface.
 * @rank:       the number of dimensions of the FFT problem to be solved. 1<=rank<=3
 * @n:          an array of dimension [rank], contains the size of each dimension of the FFT to be solved.
 *              n[i] >= 1 for i in 0, rank-1
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * Plan a basic FFTW interface call for planning an n-dimensional real-to-complex FFT operation
 * on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft_r2c().
 */
kml_fftf_plan kml_fftf_plan_dft_r2c(int rank, const int *n, float *in, kml_fftf_complex *out, unsigned flags);

/*
 * kml_fftf_plan_many_dft_r2c() - advanced real-to-complex float precision FFT Interface.
 * @rank:       the number of dimensions of the FFT problem to be solved. 1<=rank<=3
 * @n:          an array of dimension [rank], contains the size of each dimension of the FFT to be solved.
 * @howmany:    the number of multi-dimensional FFT transforms to be computed.
 * @in:         the input sequence on which to perform the FFT.
 * @inembed:    an array of dimension [rank] or NULL.
 *              This array denotes the dimensionality of a potentially larger space of which the input array, in,
 *              dimensions given by n, is a sub array.
 * @istride:    the distance between successive elements of the input data.
 * @idist:      the distance between the start of each of the howmany transforms in the input data.
 * @out:        the output sequence with the resulting transform coefficients.
 * @onembed:    an array of dimension [rank] or NULL.
 *              This array denotes the dimensionality of a potentially larger space of which the output array, in,
 *              dimensions given by n, is a sub array.
 * @ostride:    the distance between successive elements of the output data.
 * @odist:      the distance between the start of each of the howmany transforms in the output data.
 * @flags:      planning options, unused.
 *
 * An advanced FFTW interface call for planning an n-dimensional real-to-complex FFT operation
 * on howmany sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft_r2c().
 */
kml_fftf_plan kml_fftf_plan_many_dft_r2c(int rank, const int *n,
    int howmany,
    float *in, const int *inembed,
    int istride, int idist,
    kml_fftf_complex *out, const int *onembed,
    int ostride, int odist,
    unsigned flags);

/*
 * kml_fftf_plan_dft_c2r_1d() - 1-d complex-to-real float precision FFT Interface.
 * @n:          the length of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * Plan a 1-d complex-to-real FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft_c2r().
 */
kml_fftf_plan kml_fftf_plan_dft_c2r_1d(int n, kml_fftf_complex *in, float *out, unsigned flags);

/*
 * kml_fftf_plan_dft_c2r_2d() - 2-d complex-to-real float precision FFT Interface.
 * @n0:         the length of the first dimension of the FFT sequence.
 * @n1:         the length of the second dimension of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * Plan a 2-d complex-to-real FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft_c2r().
 */
kml_fftf_plan kml_fftf_plan_dft_c2r_2d(int n0, int n1, kml_fftf_complex *in, float *out, unsigned flags);

/*
 * kml_fftf_plan_dft_c2r_3d() - 3-d complex-to-real float precision FFT Interface.
 * @n0:         the length of the first dimension of the FFT sequence.
 * @n1:         the length of the second dimension of the FFT sequence.
 * @n2:         the length of the third dimension of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * Plan a 3-d complex-to-real FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft_c2r().
 */
kml_fftf_plan kml_fftf_plan_dft_c2r_3d(int n0, int n1, int n2, kml_fftf_complex *in, float *out, unsigned flags);

/*
 * kml_fftf_plan_dft_c2r() - n-dimensional complex-to-real float precision FFT Interface.
 * @rank:       the number of dimensions of the FFT problem to be solved. 1<=rank<=3
 * @n:          an array of dimension [rank], contains the size of each dimension of the FFT to be solved.
 *              n[i] >= 1 for i in 0, rank-1
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * Plan a basic FFTW interface call for planning an n-dimensional complex-to-real FFT operation
 * on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft_c2r().
 */
kml_fftf_plan kml_fftf_plan_dft_c2r(int rank, const int *n, kml_fftf_complex *in, float *out, unsigned flags);

/*
 * kml_fftf_plan_many_dft_c2r() - advanced complex-to-real float precision FFT Interface.
 * @rank:       the number of dimensions of the FFT problem to be solved. 1<=rank<=3
 * @n:          an array of dimension [rank], contains the size of each dimension of the FFT to be solved.
 * @howmany:    the number of multi-dimensional FFT transforms to be computed.
 * @in:         the input sequence on which to perform the FFT.
 * @inembed:    an array of dimension [rank] or NULL.
 *              This array denotes the dimensionality of a potentially larger space of which the input array, in,
 *              dimensions given by n, is a sub array.
 * @istride:    the distance between successive elements of the input data.
 * @idist:      the distance between the start of each of the howmany transforms in the input data.
 * @out:        the output sequence with the resulting transform coefficients.
 * @onembed:    an array of dimension [rank] or NULL.
 *              This array denotes the dimensionality of a potentially larger space of which the output array, in,
 *              dimensions given by n, is a sub array.
 * @ostride:    the distance between successive elements of the output data.
 * @odist:      the distance between the start of each of the howmany transforms in the output data.
 * @flags:      planning options, unused.
 *
 * An advanced FFTW interface call for planning an n-dimensional complex-to-real FFT operation
 * on howmany sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft_c2r().
 */
kml_fftf_plan kml_fftf_plan_many_dft_c2r(int rank, const int *n, int howmany,
    kml_fftf_complex *in, const int *inembed, int istride, int idist,
    float *out, const int *onembed, int ostride, int odist,
    unsigned flags);

/*
 * kml_fftf_plan_r2r_1d() - 1-d real-to-real float precision FFT Interface.
 * @n:          the length of the the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @kind:       the r2r transform kind of the FFT sequence.
 * @flags:      planning options, unused.
 *
 * Plan a 1-d real-to-real FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_r2r().
 */
kml_fftf_plan kml_fftf_plan_r2r_1d(int n, float *in, float *out, kml_fftf_r2r_kind kind, unsigned flags);

/*
 * kml_fftf_plan_r2r_2d() - 2-d real-to-real float precision FFT Interface.
 * @n0:         the length of the first dimension of the FFT sequence.
 * @n1:         the length of the second dimension of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @kind0:      the r2r transform kind of the first dimension of the FFT sequence.
 * @kind1:      the r2r transform kind of the second dimension of the FFT sequence.
 * @flags:      planning options, unused.
 *
 * Plan a 2-d real-to-real FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_r2r().
 */
kml_fftf_plan kml_fftf_plan_r2r_2d(int n0, int n1, float *in, float *out,
    kml_fftf_r2r_kind kind0, kml_fftf_r2r_kind kind1, unsigned flags);

/*
 * kml_fftf_plan_r2r_3d() - 3-d real-to-real float precision FFT Interface.
 * @n0:         the length of the first dimension of the FFT sequence.
 * @n1:         the length of the second dimension of the FFT sequence.
 * @n2:         the length of the third dimension of the FFT sequence.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @kind0:      the r2r transform kind of the first dimension of the FFT sequence.
 * @kind1:      the r2r transform kind of the second dimension of the FFT sequence.
 * @kind2:      the r2r transform kind of the third dimension of the FFT sequence.
 * @flags:      planning options, unused.
 *
 * Plan a 3-d real-to-real FFT operation on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_r2r().
 */
kml_fftf_plan kml_fftf_plan_r2r_3d(int n0, int n1, int n2,
    float *in, float *out, kml_fftf_r2r_kind kind0,
    kml_fftf_r2r_kind kind1, kml_fftf_r2r_kind kind2,
    unsigned flags);

/*
 * kml_fftf_plan_r2r() - n-dimensional real-to-real float precision FFT Interface.
 * @rank:       the number of dimensions of the FFT problem to be solved. 1<=rank<=3
 * @n:          an array of dimension [rank], contains the size of each dimension of the FFT to be solved.
 *              n[i] >= 1 for i in 0, rank-1
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @kind:       an array of dimension [rank], contains the transform kind of each dimension of the FFT to be solved.
 * @flags:      planning options, unused.
 *
 * Plan a basic FFTW interface call for planning an n-dimensional real-to-real FFT operation
 * on a single, contiguous data sequence.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_r2r().
 */
kml_fftf_plan kml_fftf_plan_r2r(int rank, const int *n,
    float *in, float *out,
    const kml_fftf_r2r_kind *kind, unsigned flags);

/*
 * kml_fftf_plan_many_r2r() - advanced real-to-real float precision FFT Interface.
 * @rank:       the number of dimensions of the FFT problem to be solved. 1<=rank<=3
 * @n:          an array of dimension [rank], contains the size of each dimension of the FFT to be solved.
 * @howmany:    the number of multi-dimensional FFT transforms to be computed.
 * @in:         the input sequence on which to perform the FFT.
 * @inembed:    an array of dimension [rank] or NULL.
 *              This array denotes the dimensionality of a potentially larger space of which the input array, in,
 *              dimensions given by n, is a sub array.
 * @istride:    the distance between successive elements of the input data.
 * @idist:      the distance between the start of each of the howmany transforms in the input data.
 * @out:        the output sequence with the resulting transform coefficients.
 * @onembed:    an array of dimension [rank] or NULL.
 *              This array denotes the dimensionality of a potentially larger space of which the output array, in,
 *              dimensions given by n, is a sub array.
 * @ostride:    the distance between successive elements of the output data.
 * @odist:      the distance between the start of each of the howmany transforms in the output data.
 * @kind:       an array of dimension [rank], contains the transform kind of each dimension of the FFT to be solved.
 * @flags:      planning options, unused.
 *
 * An advanced FFTW interface call for planning an n-dimensional real-to-real FFT operation
 * on howmany sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 *
 * Note that in and out can be the same pointer for an “in-place” transform.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_r2r().
 */
kml_fftf_plan kml_fftf_plan_many_r2r(int rank, const int *n, int howmany,
    float *in, const int *inembed, int istride, int idist,
    float *out, const int *onembed, int ostride, int odist,
    const kml_fftf_r2r_kind *kind, unsigned flags);

/*
 * kml_fftf_plan_guru_dft() - guru complex-to-complex float precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, of type fftw_complex, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @sign:       an integer denoting whether a forward or backward FFT transform should be performed.
 *              sign = -1 (KML_FFT_FORWARD) for a forward transform
 *              sign = 1 (KML_FFT_BACKWARD) for a backwards transform
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional complex-to-complex FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft().
 */
kml_fftf_plan kml_fftf_plan_guru_dft(int rank, const kml_fftf_iodim *dims,
    int howmany_rank, const kml_fftf_iodim *howmany_dims,
    kml_fftf_complex *in, kml_fftf_complex *out,
    int sign, unsigned flags);

/*
 * kml_fftf_plan_guru_split_dft() - guru complex-to-complex float precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, of type fftw_complex, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @ri:         the real parts of the input sequence on which to perform the FFT.
 * @ii:         the imaginary parts of the input sequence on which to perform the FFT.
 * @ro:         the real parts of the output sequence with the resulting transform coefficients.
 * @io:         the imaginary parts of the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional complex-to-complex FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 * Input and output data is stored in separate arrays for real and imaginary parts.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_split_dft().
 */
kml_fftf_plan kml_fftf_plan_guru_split_dft(int rank, const kml_fftf_iodim *dims,
    int howmany_rank, const kml_fftf_iodim *howmany_dims,
    float *ri, float *ii, float *ro, float *io,
    unsigned flags);

/*
 * kml_fftf_plan_guru64_dft() - guru complex-to-complex float precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the length of the i th dimension.
 *              ptrdiff_t is – the stride between successive elements in the input vector in the i th dimension.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t is – the number of memory elements, of type fftw_complex, between the start of transforms in
 *                  the i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @sign:       an integer denoting whether a forward or backward FFT transform should be performed.
 *              sign = -1 (KML_FFT_FORWARD) for a forward transform
 *              sign = 1 (KML_FFT_BACKWARD) for a backwards transform
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional complex-to-complex FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner. Allows selected parameters to be specified as 64-bit integers.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft().
 */
kml_fftf_plan kml_fftf_plan_guru64_dft(int rank, const kml_fftf_iodim64 *dims,
    int howmany_rank, const kml_fftf_iodim64 *howmany_dims,
    kml_fftf_complex *in, kml_fftf_complex *out,
    int sign, unsigned flags);

/*
 * kml_fftf_plan_guru64_split_dft() - guru complex-to-complex float precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the length of the i th dimension.
 *              ptrdiff_t is – the stride between successive elements in the input vector in the i th dimension.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t is – the number of memory elements, of type fftw_complex, between the start of transforms in
 *                  the i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @ri:         the real parts of the input sequence on which to perform the FFT.
 * @ii:         the imaginary parts of the input sequence on which to perform the FFT.
 * @ro:         the real parts of the output sequence with the resulting transform coefficients.
 * @io:         the imaginary parts of the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional complex-to-complex FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner. Allows selected parameters to be specified as 64-bit integers.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_split_dft().
 */
kml_fftf_plan kml_fftf_plan_guru64_split_dft(int rank, const kml_fftf_iodim64 *dims,
    int howmany_rank, const kml_fftf_iodim64 *howmany_dims,
    float *ri, float *ii, float *ro, float *io,
    unsigned flags);

/*
 * kml_fftf_plan_guru_dft() - guru real-to-complex float precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional real-to-complex FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft_r2c().
 */
kml_fftf_plan kml_fftf_plan_guru_dft_r2c(int rank, const kml_fftf_iodim *dims,
    int howmany_rank,
    const kml_fftf_iodim *howmany_dims,
    float *in, kml_fftf_complex *out,
    unsigned flags);

/*
 * kml_fftf_plan_guru_split_dft_r2c() - guru real-to-complex float precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @ro:         the real parts of the output sequence with the resulting transform coefficients.
 * @io:         the imaginary parts of the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional real-to-complex FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 * Input and output data is stored in separate arrays for real and imaginary parts.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_split_dft_r2c().
 */
kml_fftf_plan kml_fftf_plan_guru_split_dft_r2c(int rank, const kml_fftf_iodim *dims,
    int howmany_rank,
    const kml_fftf_iodim *howmany_dims,
    float *in, float *ro, float *io,
    unsigned flags);

/*
 * kml_fftf_plan_guru64_dft_r2c() - guru real-to-complex float precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional real-to-complex FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner. Allows selected parameters to be specified as 64-bit integers.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft_r2c().
 */
kml_fftf_plan kml_fftf_plan_guru64_dft_r2c(int rank, const kml_fftf_iodim64 *dims,
    int howmany_rank, const kml_fftf_iodim64 *howmany_dims,
    float *in, kml_fftf_complex *out,
    unsigned flags);

/*
 * kml_fftf_plan_guru64_split_dft_r2c() - guru real-to-complex float precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the length of the i th dimension.
 *              ptrdiff_t is – the stride between successive elements in the input vector in the i th dimension.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @ro:         the real parts of the output sequence with the resulting transform coefficients.
 * @io:         the imaginary parts of the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional real-to-complex FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner. Allows selected parameters to be specified as 64-bit integers.
 * Input and output data is stored in separate arrays for real and imaginary parts.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_split_dft_r2c().
 */
kml_fftf_plan kml_fftf_plan_guru64_split_dft_r2c(int rank, const kml_fftf_iodim64 *dims,
    int howmany_rank, const kml_fftf_iodim64 *howmany_dims,
    float *in, float *ro, float *io,
    unsigned flags);

/*
 * kml_fftf_plan_guru_dft_c2r() - guru complex-to-real float precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional complex-to-real FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft_c2r().
 */
kml_fftf_plan kml_fftf_plan_guru_dft_c2r(int rank, const kml_fftf_iodim *dims,
    int howmany_rank,
    const kml_fftf_iodim *howmany_dims,
    kml_fftf_complex *in, float *out,
    unsigned flags);

/*
 * kml_fftf_plan_guru_split_dft_c2r() - guru complex-to-real float precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @ri:         the real parts of the input sequence on which to perform the FFT.
 * @ii:         the imaginary parts of the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional complex-to-real FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 * Input and output data is stored in separate arrays for real and imaginary parts.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_split_dft_c2r().
 */
kml_fftf_plan kml_fftf_plan_guru_split_dft_c2r(int rank, const kml_fftf_iodim *dims,
    int howmany_rank,
    const kml_fftf_iodim *howmany_dims,
    float *ri, float *ii, float *out,
    unsigned flags);

/*
 * kml_fftf_plan_guru64_dft_c2r() - guru complex-to-real float precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the length of the i th dimension.
 *              ptrdiff_t is – the stride between successive elements in the input vector in the i th dimension.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional complex-to-real FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner. Allows selected parameters to be specified as 64-bit integers.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_dft_c2r().
 */
kml_fftf_plan kml_fftf_plan_guru64_dft_c2r(int rank, const kml_fftf_iodim64 *dims,
    int howmany_rank, const kml_fftf_iodim64 *howmany_dims,
    kml_fftf_complex *in, float *out,
    unsigned flags);

/*
 * kml_fftf_plan_guru64_split_dft_c2r() - guru complex-to-real float precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @ri:         the real parts of the input sequence on which to perform the FFT.
 * @ii:         the imaginary parts of the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional complex-to-real FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner. Allows selected parameters to be specified as 64-bit integers.
 * Input and output data is stored in separate arrays for real and imaginary parts.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_split_dft_c2r().
 */
kml_fftf_plan kml_fftf_plan_guru64_split_dft_c2r(int rank, const kml_fftf_iodim64 *dims,
    int howmany_rank, const kml_fftf_iodim64 *howmany_dims,
    float *ri, float *ii, float *out,
    unsigned flags);

/*
 * kml_fftf_plan_guru_r2r() - guru real-to-real float precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the length of the i th dimension.
 *              int is – the stride between successive elements in the input vector in the i th dimension.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              int n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              int is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              int os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @kind:       an array of dimension [rank], contains the transform kind of each dimension of the FFT to be solved.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional real-to-real FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_r2r().
 */
kml_fftf_plan kml_fftf_plan_guru_r2r(int rank, const kml_fftf_iodim *dims,
    int howmany_rank, const kml_fftf_iodim *howmany_dims,
    float *in, float *out,
    const kml_fftf_r2r_kind *kind, unsigned flags);

/*
 * kml_fftf_plan_guru64_r2r() - guru real-to-real float precision FFT Interface.
 * @rank:       the number of dimensions of each individual FFT problem to be solved. 1<=rank<=3
 * @dims:       an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the length of the i th dimension.
 *              ptrdiff_t is – the stride between successive elements in the input vector in the i th dimension.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @howmany_rank: the number of dimensions needed to describe the memory access pattern for the start addresses
 *              of each of the multi-dimensional FFT transforms to be computed.
 * @howmany_dims:an array of structures, dimension [rank], where the members of dims[i],
 *              for i in 0, rank-1, are defined to be:
 *              ptrdiff_t n – the number of transforms to be computed of the i th dimension of
 *                  the howmany_rank- dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t is – the number of memory elements, between the start of transforms in the
 *                  i th dimension. of the howmany_rank-dimensional space mapped by the transforms to be computed.
 *              ptrdiff_t os – the stride between successive elements in the output vector in the i th dimension.
 * @in:         the input sequence on which to perform the FFT.
 * @out:        the output sequence with the resulting transform coefficients.
 * @kind:       an array of dimension [rank], contains the transform kind of each dimension of the FFT to be solved.
 * @flags:      planning options, unused.
 *
 * A guru FFTW interface call for planning an n-dimensional real-to-real FFT operation
 * on multiple sets of data. The data for an individual FFT does not need to be contiguous, and
 * can be provided in a strided manner. Allows selected parameters to be specified as 64-bit integers.
 *
 * @return:     plan returned that can be used in subsequent kml_fftf_execute() calls on the previously supplied
 *              in and out arrays, or using different arrays via kml_fftf_execute_r2r().
 */
kml_fftf_plan kml_fftf_plan_guru64_r2r(int rank, const kml_fftf_iodim64 *dims,
    int howmany_rank, const kml_fftf_iodim64 *howmany_dims,
    float *in, float *out,
    const kml_fftf_r2r_kind *kind, unsigned flags);

/*
 * kml_fftf_execute() - executes a previously created FFT plan of float precision.
 * @p:          an FFT plan created from a preceding call.
 */
void kml_fftf_execute(const kml_fftf_plan p);

/*
 * kml_fftf_execute_dft() - executes a previously created FFT plan of float precision.
 * @p:          an FFT plan created from a preceding call.
 * @in:         a pointer to a previously allocated array of data, on which to calculate a complex-to-complex FFT.
 * @out:        a pointer to a previously allocated array of data,
 *              into which to return the results of the complex-to-complex FFT.
 *
 * executes a previously created FFT plan, however input and output data area maybe
 *     different than those given at planning time
 */
void kml_fftf_execute_dft(const kml_fftf_plan p, kml_fftf_complex *in, kml_fftf_complex *out);

/*
 * kml_fftf_execute_dft_r2c() - executes a previously created real-to-complex FFT plan of float precision.
 * @p:          an FFT plan created from a preceding call.
 * @in:         a pointer to a previously allocated array of data, on which to calculate a real-to-complex FFT.
 * @out:        a pointer to a previously allocated array of data,
 *              into which to return the results of the real-to-complex FFT.
 *
 * executes a previously created real-to-complex FFT plan, however input and output data area maybe
 *     different than those given at planning time
 */
void kml_fftf_execute_dft_r2c(const kml_fftf_plan p, float *in, kml_fftf_complex *out);

/*
 * kml_fftf_execute_dft_c2r() - executes a previously created complex-to-real FFT plan of float precision.
 * @p:          an FFT plan created from a preceding call.
 * @in:         a pointer to a previously allocated array of data, on which to calculate a complex-to-real FFT.
 * @out:        a pointer to a previously allocated array of data,
 *              into which to return the results of the complex-to-real FFT.
 *
 * executes a previously created complex-to-real FFT plan, however input and output data area maybe
 *     different than those given at planning time
 */
void kml_fftf_execute_dft_c2r(const kml_fftf_plan p, kml_fftf_complex *in, float *out);

/*
 * kml_fftf_execute_split_dft() - executes a previously created FFT plan of float precision.
 * @p:          an FFT plan created from a preceding call.
 * @ri:         a previously allocated array of the real parts of data, on which to calculate the FFT.
 * @ii:         a previously allocated array of the imaginary parts of data, on which to calculate the FFT.
 * @ro:         a previously allocated array, into which to return the the real parts of results of the FFT.
 * @io:         a previously allocated array, into which to return the the imaginary parts of results of the FFT.
 *
 * executes a previously created FFT plan, however input and output data area maybe
 *     different than those given at planning time.
 */
void kml_fftf_execute_split_dft(const kml_fftf_plan p, float *ri, float *ii, float *ro, float *io);

/*
 * kml_fftf_execute_split_dft_r2c() - executes a previously created FFT plan of float precision.
 * @p:          an FFT plan created from a preceding call.
 * @in:         a pointer to a previously allocated array of data, on which to calculate a real-to-complex FFT.
 * @ro:         a previously allocated array, into which to return the the real parts of results of the FFT.
 * @io:         a previously allocated array, into which to return the the imaginary parts of results of the FFT.
 *
 * executes a previously created FFT plan, however input and output data area maybe
 *     different than those given at planning time.
 */
void kml_fftf_execute_split_dft_r2c(const kml_fftf_plan p, float *in, float *ro, float *io);

/*
 * kml_fftf_execute_split_dft_c2r() - executes a previously created FFT plan of float precision.
 * @p:          an FFT plan created from a preceding call.
 * @ri:         a previously allocated array of the real parts of data, on which to calculate the FFT.
 * @ii:         a previously allocated array of the imaginary parts of data, on which to calculate the FFT.
 * @out:        a pointer to a previously allocated array of data,
 *              into which to return the results of the complex-to-real FFT.
 *
 * executes a previously created FFT plan, however input and output data area maybe
 *     different than those given at planning time.
 */
void kml_fftf_execute_split_dft_c2r(const kml_fftf_plan p, float *ri, float *ii, float *out);

/*
 * kml_fftf_execute_r2r() - executes a previously created real-to-real FFT plan of float precision.
 * @p:          an FFT plan created from a preceding call.
 * @in:         a pointer to a previously allocated array of data, on which to calculate a real-to-real FFT.
 * @out:        a pointer to a previously allocated array of data,
 *              into which to return the results of the real-to-real FFT.
 *
 * executes a previously created real-to-real FFT plan, however input and output data area maybe
 *     different than those given at planning time
 */
void kml_fftf_execute_r2r(const kml_fftf_plan p, float *in, float *out);

/*
 * kml_fftf_destroy_plan() - frees all memory associated with the creation of an FFT plan.
 * @p:          a pointer to a previously created plan.
 *
 * The plan should not be reused after this call.
 */
void kml_fftf_destroy_plan(kml_fftf_plan p);

/*
 * kml_fftf_plan_with_nthreads() - specify the number of threads used for the execution of FFTs.
 * @nthreads:   the number of threads requested to be used when executing subsequent FFT plans. nthreads>=1.
 */
void kml_fftf_plan_with_nthreads(int nthreads);

/*
 * kml_fftf_init_threads() - initialise the use of threading in the execution of FFTs.
 *
 * @return:     return 1 means succeess, return 0 means failed.
 */
int kml_fftf_init_threads(void);

/*
 * kml_fftf_cleanup_threads() - discard memory for use of threading previously.
 */
void kml_fftf_cleanup_threads(void);

/*
 * kml_fftf_cleanup() - discard memory use previously.
 */
void kml_fftf_cleanup(void);

/*
 * kml_fftf_malloc() - allocates a block of data of the requested size.
 * @nthreads:   the size, in bytes, of array to be created.
 *
 * return:      a pointer, of type void*, to an allocated block of memory of the requested size
 */
void *kml_fftf_malloc(size_t n);

/*
 * kml_fftf_free() - frees the memory created in a previous or kml_fftf_malloc() call.
 * @p:          a pointer to a previously created array.
 */
void kml_fftf_free(void *p);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __KML_FFT_H__ */
