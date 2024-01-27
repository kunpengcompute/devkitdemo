/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: DesignWare kunpeng libmath interface definition.
 * Author:
 * Create: 2020-08-28
 */

#ifndef KML_VML_H
#define KML_VML_H
#ifdef __cplusplus
extern "C" {
#endif
#ifndef complex
#define complex _Complex
#endif
#ifndef _Complex_I
#define _Complex_I (__extension__ 1.0iF)
#endif
#undef I
#define I _Complex_I

#define KVML_VERSION_STRUCT_LEN 100
typedef struct {
    char productName[KVML_VERSION_STRUCT_LEN];
    char productVersion[KVML_VERSION_STRUCT_LEN];
    char componentName[KVML_VERSION_STRUCT_LEN];
    char componentVersion[KVML_VERSION_STRUCT_LEN];
    char componentAppendInfo[KVML_VERSION_STRUCT_LEN];
    char softwareName[KVML_VERSION_STRUCT_LEN];
    char softwareVersion[KVML_VERSION_STRUCT_LEN];
}KVMLVersion;
int KVMLGetVersion(KVMLVersion* ver);

/**
 *  KML_VML FUNCTION ACCURACY CONTROL
 *  KML_HA - when KML_HA is set, high accuracy VML functions are called
 *  KML_LA - when KML_LA is set, low accuracy VML functions are called
 *  KML_EP - when KML_EP is set, enhanced performance VML functions are called
 * */
#define KML_LA 0x1
#define KML_HA 0x2
#define KML_EP 0x3

/**
 * @Brief computes abs of vector elements.
 * @param[in]   	len    		Number of elements in the vector
 * @param[in]   	src   	    Pointer to the source vector.
 * @param[out]  	dst   	    Pointer to the destination vector.
 * */
void vsabs(const int len, const float *src, float *dst);
void vdabs(const int len, const double *src, double *dst);
void vcabs(const int len, const float complex *src, float *dst);
void vzabs(const int len, const double complex *src, double *dst);

/**
 * @Brief Adds the elements of two vectors.
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src1, src2 	Pointers to the source vectors.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vsadd(const int len, const float *src1, const float *src2, float *dst);
void vdadd(const int len, const double *src1, const double *src2, double *dst);
void vcadd(const int len, const float complex *src1, const float complex *src2, float complex *dst);
void vzadd(const int len, const double _Complex *src1, const double _Complex *src2, double _Complex *dst);

/**
 * @Brief computes the argument of vector elements.
 * @param[in]   	len    		Number of elements in the vector
 * @param[in]   	src   	    Pointer to the source vector.
 * @param[out]  	dst   	    Pointer to the destination vector.
 * */
void vcarg(const int len, const float complex *src, float *dst);
void vzarg(const int len, const double complex *src, double *dst);

/**
 * @Brief performs element by element division of vector.
 * @param[in]       len         Number of elements in the vector
 * @param[in]       src         Pointer to the source vector.
 * @param[out]      dst         Pointer to the destination vector.
 **/
void vcconj(const int len, const float complex *src, float complex *dst);
void vzconj(const int len, const double complex *src, double complex *dst);

/**
 * @Brief computes modulus of vector elements.
 * @param[in]       len         Number of elements in the vector
 * @param[in]       src         Pointer to the source vector.
 * @param[out]      dst         Pointer to the destination vector.
 * */
void vsfmod(const int len, const float *src1, const float *src2, float *dst);
void vdfmod(const int len, const double *src1, const double *src2, double *dst);

/**
 * @Brief Performs linear fraction transformation of vector elements.
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src1       	Pointer to the source vector a.
 * @param[in]  		src2       	Pointer to the source vector b.
 * @param[in]  		scalea     	Const scalea for linearfrac.
 * @param[in]  		shifta     	Const shifta for linearfrac.
 * @param[in]  		scaleb     	Const scaleb for linearfrac.
 * @param[in]  		shiftb   	Const shiftb for linearfrac.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vslinearfrac(const int len, const float *src1, const float *src2, const float scalea, const float shifta,
    const float scaleb, const float shiftb, float *dst);
void vdlinearfrac(const int len, const double *src1, const double *src2, const double scalea, const double shifta,
    const double scaleb, const double shiftb, double *dst);

/**
 * @Brief computes remainder of vector elements of a and b.
 * @param[in]       len         Number of elements in the vector
 * @param[in]       src1        Pointer to the source vector a.
 * @param[in]       src2        Pointer to the source vector b.
 * @param[out]      dst         Pointer to the destination vector.
 * */
void vsremainder(const int len, const float *src1, const float *src2, float *dst);
void vdremainder(const int len, const double *src1, const double *src2, double *dst);

/**
 * @Brief Subs the elements of two vectors.
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src1, src2 	Pointers to the source vectors.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vssub(const int len, const float *src1, const float *src2, float *dst);
void vdsub(const int len, const double *src1, const double *src2, double *dst);
void vcsub(const int len, const float complex *src1, const float complex *src2, float complex *dst);
void vzsub(const int len, const double complex *src1, const double complex *src2, double complex *dst);

/**
 * @Brief performs element by element squaring of the vector.
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src       	Pointers to the source vectors.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vssqr(const int len, const float *src, float *dst);
void vdsqr(const int len, const double *src, double *dst);

/**
 * @BriefComputes a inverse square root of vector elements.
 * @param[in]       len         Number of elements in the vector
 * @param[in]       src         Pointers to the source vectors.
 * @param[out]      dst         Pointer to the destination vector.
 **/
void vsinvsqrt(const int len, const float *src, float *dst);
void vdinvsqrt(const int len, const double *src, double *dst);

/**
 * @BriefComputes a square root of vector elements.
 * @param[in]       len         Number of elements in the vector
 * @param[in]       src         Pointers to the source vectors.
 * @param[out]      dst         Pointer to the destination vector.
 **/
void vssqrt(const int len, const float *src, float *dst);
void vdsqrt(const int len, const double *src, double *dst);
void vcsqrt(const int len, const float _Complex *src, float _Complex *dst);
void vzsqrt(const int len, const double _Complex *src, double _Complex *dst);

/**
 * @BriefComputes a cube root of vector elements.
 * @param[in]       len         Number of elements in the vector
 * @param[in]       src         Pointers to the source vectors.
 * @param[out]      dst         Pointer to the destination vector.
 **/
void vscbrt(const int len, const float *src, float *dst);
void vdcbrt(const int len, const double *src, double *dst);

/**
 * @BriefComputes a inverse cube root of vector elements.
 * @param[in]       len         Number of elements in the vector
 * @param[in]       src         Pointers to the source vectors.
 * @param[out]      dst         Pointer to the destination vector.
 **/
void vsinvcbrt(const int len, const float *src, float *dst);
void vdinvcbrt(const int len, const double *src, double *dst);

/**
 * @Brief Performs element by element multiplication of vector.
 * @param[in]       len         Number of elements in the vector
 * @param[in]       src1, src2  Pointers to the source vectors.
 * @param[out]      p _dst      Pointer to the destination vector.
 **/
void vsmul(const int len, const float *src1, const float *src2, float *dst);
void vdmul(const int len, const double *src1, const double *src2, double *dst);
void vcmul(const int len, const float _Complex *src1, const float _Complex *src2, float _Complex *dst);
void vzmul(const int len, const double _Complex *src1, const double _Complex *src2, double _Complex *dst);

/**
 * @Brief performs element by element multiplication of src1 vector and conjugated src2 vector.
 * @param[in]       len         Number of elements in the vector
 * @param[in]       src1        Pointer to the first source vector.
 * @param[in]       src2        Pointer to the second source vector.
 * @param[out]      dst         Pointer to the destination vector.
 **/
void vcmulbyconj(const int len, const float complex *src1, const float complex *src2, float complex *dst);
void vzmulbyconj(const int len, const double complex *src1, const double complex *src2, double complex *dst);

/**
 * @Brief performs element by element division of vector.
 * @param[in]       len         Number of elements in the vector
 * @param[in]       src1, src2  Pointers to the source vectors.
 * @param[out]      p _dst      Pointer to the destination vector.
 **/
void vsdiv(const int len, const float *src1, const float *src2, float *dst);
void vddiv(const int len, const double *src1, const double *src2, double *dst);
void vcdiv(const int len, const float _Complex *src1, const float _Complex *src2, float _Complex *dst);
void vzdiv(const int len, const double _Complex *src1, const double _Complex *src2, double _Complex *dst);

/**
 * @Brief computes an exponential of vector elements.
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src       	Pointers to the source vectors.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vsexp(const int len, const float *src, float *dst);
void vdexp(const int len, const double *src, double *dst);
void vcexp(const int len, const float complex *src, float complex *dst);
void vzexp(const int len, const double complex *src, double complex *dst);

/**
 * @Brief computes the base 2 exponential of vector elements
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src       	Pointers to the source vectors.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vsexp2(const int len, const float *src, float *dst);
void vdexp2(const int len, const double *src, double *dst);

/**
 * @Brief computes the base 10 exponential of vector elements
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src       	Pointers to the source vectors.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vsexp10(const int len, const float *src, float *dst);
void vdexp10(const int len, const double *src, double *dst);

/**
 * @Brief computes the base e exponential of vector elements decreased by 1
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src       	Pointers to the source vectors.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vsexpm1(const int len, const float *src, float *dst);
void vdexpm1(const int len, const double *src, double *dst);

/**
 * @Brief computes ln of vector elements.
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src       	Pointers to the source vectors.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vsln(const int len, const float *src, float *dst);
void vdln(const int len, const double *src, double *dst);
void vcln(const int len, const float complex *src, float complex *dst);
void vzln(const int len, const double complex *src, double complex *dst);

/**
 * @Brief computes log2 of vector elements.
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src       	Pointers to the source vectors.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vslog2(const int len, const float *src, float *dst);
void vdlog2(const int len, const double *src, double *dst);

/**
 * @Brief computes log10 of vector elements.
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src       	Pointers to the source vectors.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vslog10(const int len, const float *src, float *dst);
void vdlog10(const int len, const double *src, double *dst);
void vclog10(const int len, const float complex *src, float complex *dst);
void vzlog10(const int len, const double complex *src, double complex *dst);

/**
 * @Brief computes the natural logarithm of vector elements that are increased by 1
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src       	Pointers to the source vectors.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vslog1p(const int len, const float *src, float *dst);
void vdlog1p(const int len, const double *src, double *dst);

/**
 * @Brief computes the exponents of the elements of input vector
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src       	Pointers to the source vectors.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vslogb(const int len, const float *src, float *dst);
void vdlogb(const int len, const double *src, double *dst);

/**
 * @Computes tangent of vector elements.
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src       	Pointers to the source vectors.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vstan(const int len, const float *src, float *dst);
void vdtan(const int len, const double *src, double *dst);

/**
 * @Computes inverse tangent of vector elements.
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src       	Pointers to the source vectors.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vsatan(const int len, const float *src, float *dst);
void vdatan(const int len, const double *src, double *dst);

/**
 * @Computes inverse tangent of vector elements.
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src       	Pointers to the source vectors.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vsatan2(const int len, const float *src1, const float *src2, float *dst);
void vdatan2(const int len, const double *src1, const double *src2, double *dst);

/**
 * @Brief computes pow of vector elements.
 * @param[in]       len         Number of elements in the vector
 * @param[in]       src         Pointers to the source vectors.
 * @param[out]      dst         Pointer to the destination vector.
 * */
void vspow(const int len, const float *src1, const float *src2, float *dst);
void vdpow(const int len, const double *src1, const double *src2, double *dst);
void vcpow(const int len, const float complex *src1, const float complex *src2, float complex *dst);
void vzpow(const int len, const double complex *src1, const double complex *src2, double complex *dst);

/**
 * @Brief computes pow of vector elements a and b, where b is scalar.
 * @param[in]       len         Number of elements in the vector
 * @param[in]       src1        Pointer to the source vectors.
 * @param[in]       src2        Scalar value for power b.
 * @param[out]      dst         Pointer to the destination vector.
 * */
void vspowx(const int len, const float *src1, const float src2, float *dst);
void vdpowx(const int len, const double *src1, const double src2, double *dst);
void vcpowx(const int len, const float complex *src1, const float complex src2, float complex *dst);
void vzpowx(const int len, const double complex *src1, const double complex src2, double complex *dst);
/**
 * @Brief computes pow of vector elements a and b, where a >= 0.
 * @param[in]       len         Number of elements in the vector
 * @param[in]       src1        Pointer to the source vector a.
 * @param[in]       src2        Pointer to the source vector b.
 * @param[out]      dst         Pointer to the destination vector.
 * */
void vspowr(const int len, const float *src1, const float *src2, float *dst);
void vdpowr(const int len, const double *src1, const double *src2, double *dst);

/**
 * @Brief computes (src1^2 + src2^2)^0.5 of vector elements.
 * @param[in]		len			Number of elements in the vector
 * @param[in]  		src1       	Pointer to the first source vector.
 * @param[in]  		src2       	Pointer to the second source vector.
 * @param[out] 		dst        	Pointer to the destination vector.
 * */
void vshypot(const int len, const float *src1, const float *src2, float *dst);
void vdhypot(const int len, const double *src1, const double *src2, double *dst);

/**
 * @Brief computes pow2o3 of vector elements.
 * @param[in]       len         Number of elements in the vector
 * @param[in]       src         Pointers to the source vectors.
 * @param[out]      dst         Pointer to the destination vector.
 * */
void vspow2o3(const int len, const float *src, float *dst);
void vdpow2o3(const int len, const double *src, double *dst);

/**
 * @Brief computes pow3o2 of vector elements.
 * @param[in]       len         Number of elements in the vector
 * @param[in]       src         Pointers to the source vectors.
 * @param[out]      dst         Pointer to the destination vector.
 * */
void vspow3o2(const int len, const float *src, float *dst);
void vdpow3o2(const int len, const double *src, double *dst);

/**
 * @Brief computes sine of vector elements.
 * @param[in]   	len    		Number of elements in the vector
 * @param[in]   	src   	    Pointers to the source vectors.
 * @param[out]  	dst   	    Pointer to the destination vector.
 * */
void vssin(const int len, const float *src, float *dst);
void vdsin(const int len, const double *src, double *dst);

/**
 * @Brief computes cos of vector elements.
 * @param[in]   	len    		Number of elements in the vector
 * @param[in]   	src   	    Pointers to the source vectors.
 * @param[out]  	dst   	    Pointer to the destination vector.
 * */
void vscos(const int len, const float *src, float *dst);
void vdcos(const int len, const double *src, double *dst);

/**
 * @Brief computes inv of vector elements.
 * @param[in]   	len         Number of elements in the vector
 * @param[in]   	src   	    Pointers to the source vectors.
 * @param[out]  	dst   	    Pointer to the destination vector.
 * */
void vsinv(const int len, const float *src, float *dst);
void vdinv(const int len, const double *src, double *dst);

/**
 * @Brief computes cos sin of vector elements.
 * @param[in]   	len    		Number of elements in the vector
 * @param[in]   	src     	Pointer to the source vector.
 * @param[out]  	sindst   	Pointer to the destination vector.
 * @param[out]  	cosdst   	Pointer to the destination vector.
 * */
void vssincos(const int len, const float *src, float *sindst, float *cosdst);
void vdsincos(const int len, const double *src, double *sindst, double *cosdst);

/**
 * @Brief computes sinh of vector elements.
 * @param[in]   	len    		Number of elements in the vector
 * @param[in]   	src   	    Pointers to the source vectors.
 * @param[out]  	dst   	    Pointer to the destination vector.
 * */
void vssinh(const int len, const float *src, float *dst);
void vdsinh(const int len, const double *src, double *dst);

/**
 * @Brief computes cos of vector elements.
 * @param[in]   	len    		Number of elements in the vector
 * @param[in]   	src   	    Pointers to the source vectors.
 * @param[out]  	dst   	    Pointer to the destination vector.
 * */
void vscosh(const int len, const float *src, float *dst);
void vdcosh(const int len, const double *src, double *dst);

/**
 * @Brief computes sinh of vector elements.
 * @param[in]   	len    		Number of elements in the vector
 * @param[in]   	src   	    Pointers to the source vectors.
 * @param[out]  	dst   	    Pointer to the destination vector.
 * */
void vsasinh(const int len, const float *src, float *dst);
void vdasinh(const int len, const double *src, double *dst);

/**
 * @Brief computes cos of vector elements.
 * @param[in]   	len    		Number of elements in the vector
 * @param[in]   	src   	    Pointers to the source vectors.
 * @param[out]  	dst   	    Pointer to the destination vector.
 * */
void vsacosh(const int len, const float *src, float *dst);
void vdacosh(const int len, const double *src, double *dst);

/**
 * @Brief computes tanh of vector elements.
 * @param[in]   	len    		Number of elements in the vector
 * @param[in]   	src   	    Pointers to the source vectors.
 * @param[out]  	dst   	    Pointer to the destination vector.
 * */
void vstanh(const int len, const float *src, float *dst);
void vdtanh(const int len, const double *src, double *dst);

/**
 * @Brief computes atanh of vector elements.
 * @param[in]   	len    		Number of elements in the vector
 * @param[in]   	src   	    Pointers to the source vectors.
 * @param[out]  	dst   	    Pointer to the destination vector.
 * */
void vsatanh(const int len, const float *src, float *dst);
void vdatanh(const int len, const double *src, double *dst);

#ifdef __cplusplus
}
#endif

#endif