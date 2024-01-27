/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: tee crypto definitions
 */

#ifndef TEE_ARITH_API_H
#define TEE_ARITH_API_H

#include <tee_defines.h>

/*
 * below definitions are defined by Global Platform
 * for compatibility:
 * don't make any change to the content below
 */
typedef uint32_t TEE_BigInt;
typedef uint32_t TEE_BigIntFMM;
typedef uint32_t TEE_BigIntFMMContext;

#define TEE_BigIntSizeInU32(n) ((((n) + 31) / 32) + 2)

/*
 * returns the size of the array of uint32_t values
 *
 * @param modulusSizeInBits [IN]  Size of modulus in bits
 *
 * @return Number of bytes needed to store a TEE_BigIntFMM given a modulus of length modulusSizeInBits
 */
size_t TEE_BigIntFMMSizeInU32(size_t modulusSizeInBits);

/*
 * returns the size of the array of uint32_t values needed to represent a fast modular context
 *
 * @param modulusSizeInBits [IN]  Size of modulus in bits
 *
 * @return Number of bytes needed to store a TEE_BigIntFMMContext given a modulus of length modulusSizeInBits
 */
size_t TEE_BigIntFMMContextSizeInU32(size_t modulusSizeInBits);

/*
 * initializes bigInt
 *
 * @param bigInt[OUT] A pointer to the TEE_BigInt to be initialized
 * @param len[IN] The size in uint32_t of the memory pointed to by bigInt
 *
 * @return void
 */
void TEE_BigIntInit(TEE_BigInt *bigInt, size_t len);

/*
 * calculates the necessary prerequisites for the fast modular multiplication and stores them in a context.
 *
 * @param context[OUT]  A pointer to the TEE_BigIntFMMContext to be initialized
 * @param len[IN] The size in uint32_t of the memory pointed to by context
 * @param modulus[IN] The modulus
 *
 * @return void
 */
void TEE_BigIntInitFMMContext(TEE_BigIntFMMContext *context, size_t len, const TEE_BigInt *modulus);

#if defined(API_LEVEL) && defined(API_LEVEL1_1_1) && (API_LEVEL >= API_LEVEL1_1_1)

/*
 * calculates the necessary prerequisites for the fast modular multiplication and stores them in a context.
 *
 * @param context[OUT]  A pointer to the TEE_BigIntFMMContext to be initialized
 * @param len[IN] The size in uint32_t of the memory pointed to by context
 * @param modulus[IN] The modulus
 *
 * @return #TEE_SUCCESS success
 * @return other failed
 */
TEE_Result TEE_BigIntInitFMMContext1(TEE_BigIntFMMContext *context, size_t len, const TEE_BigInt *modulus);
#endif /* API_LEVEL */

/*
 * initializes bigIntFMM and sets its represented value to zero.
 *
 * @param context[IN]  A pointer to the TEE_BigIntFMM to be initialized
 * @param len[IN] The size in uint32_t of the memory pointed to by bigIntFMM
 *
 * @return void
 */
void TEE_BigIntInitFMM(TEE_BigIntFMM *bigIntFMM, size_t len);

/*
 * converts a bufferLen byte octet string buffer into a TEE_BigInt format.
 *
 * @param dest [OUT] Pointer to a TEE_BigInt to hold the result
 * @param buffer [IN] Pointer to the buffer containing the octet string representation of the integer
 * @param bufferLen [IN] The length of *buffer in bytes
 * @param sign [IN] The sign of dest is set to the sign of sign
 *
 * @return #TEE_SUCCESS support
 * @return #TEE_ERROR_OVERFLOW If memory allocation for the dest is too small
 */
TEE_Result TEE_BigIntConvertFromOctetString(TEE_BigInt *dest, const uint8_t *buffer, size_t bufferLen, int32_t sign);

/*
 * converts the absolute value of an integer in TEE_BigInt format into an octet string
 *
 * @param buffer [OUT]  Output buffer where converted octet string representation of the integer is written
 * @param bufferLen [IN]  The length of *buffer in bytes
 * @param bigInt [IN]  Pointer to the integer that will be converted to an octet string
 *
 * @return #TEE_SUCCESS support
 * @return #TEE_ERROR_SHORT_BUFFER If the output buffer is too small to contain the octet string
 */
TEE_Result TEE_BigIntConvertToOctetString(void *buffer, size_t *bufferLen, const TEE_BigInt *bigInt);

/*
 * sets *dest to the value shortVal
 *
 * @param dest [OUT] Pointer to a TEE_BigInt to store the result
 * @param shortVal [IN] Input value
 *
 * @return void
 */
void TEE_BigIntConvertFromS32(TEE_BigInt *dest, int32_t shortVal);

/*
 * sets *dest to the value of src, including the sign of src.
 *
 * @param dest [OUT] Pointer to an int32_t to store the result
 * @param src [IN] Pointer to the input value
 *
 * @return #TEE_SUCCESS support
 * @return #TEE_ERROR_OVERFLOW If src does not fit within an int32_t
 */
TEE_Result TEE_BigIntConvertToS32(int32_t *dest, const TEE_BigInt *src);

/*
 * checks whether op1>op2, op1==op2, or op1<op2
 *
 * @param op1 [IN] Pointer to the first operand
 * @param op2 [IN] Pointer to the second operand
 *
 * @return 0 if op1==op2
 * @return a positive number if op1>op2
 */
int32_t TEE_BigIntCmp(const TEE_BigInt *op1, const TEE_BigInt *op2);

/*
 * checks whether op>shortVal, op==shortVal, or op<shortVal
 *
 * @param op [IN] Pointer to the first operand
 * @param shortVal [IN] Pointer to the second operand
 *
 * @return 0 if op1==shortVal
 * @return a positive number if op1>shortVal
 */
int32_t TEE_BigIntCmpS32(const TEE_BigInt *op, int32_t shortVal);

/*
 * computes |dest| = |op| >> bits
 *
 * @param dest [OUT] Pointer to TEE_BigInt to hold the shifted result
 * @param op [IN] Pointer to the operand to be shifted
 * @param bits [IN] Number of bits to shift
 *
 * @return void
 */
void TEE_BigIntShiftRight(TEE_BigInt *dest, const TEE_BigInt *op, size_t bits);

/*
 * returns the bitIndexth bit of the natural binary representation of |src|
 *
 * @param src [IN] Pointer to the integer
 * @param bitIndex[IN] The offset of the bit to be read, starting at offset 0 for the least significant bit
 *
 * @return true The Boolean value of the bitIndexth bit in |src| is '1'
 * @return false The Boolean value of the bitIndexth bit in |src| is '0'
 */
bool TEE_BigIntGetBit(const TEE_BigInt *src, uint32_t bitIndex);

/*
 * returns the number of bits in the natural binary representation of |src|; that is, the magnitude of src
 *
 * @param src [IN] Pointer to the integer
 *
 * @return 0 src equals zero
 * @return others The number of bits in the natural binary representation of |src|.
 */
uint32_t TEE_BigIntGetBitCount(const TEE_BigInt *src);

#if defined(API_LEVEL) && defined(API_LEVEL1_2) && (API_LEVEL >= API_LEVEL1_2)
/*
 * sets the bitIndexth bit of the natural binary representation of |op| to 1 or 0
 *
 * @param op [IN/OUT] Pointer to the integer
 * @param bitIndex [IN] The offset of the bit to be set, starting at offset 0 for the least significant bit
 * @param value [IN] The bit value to set where true represents a '1' and false represents a '0'
 *
 * @return #TEE_SUCCESS support
 * @return #TEE_ERROR_OVERFLOW If the bitIndexth bit is larger than allocated bit length of op
 */
TEE_Result TEE_BigIntSetBit(TEE_BigInt *op, uint32_t bitIndex, bool value);

/*
 * assigns the value of src to dest
 *
 * @param dest [OUT] Pointer to TEE_BigInt to be assigned
 * @param src [IN] Pointer to the source operand
 *
 * @return #TEE_SUCCESS support
 * @return #TEE_ERROR_OVERFLOW In case the dest operand cannot hold the value of src
 */
TEE_Result TEE_BigIntAssign(TEE_BigInt *dest, const TEE_BigInt *src);

/*
 * assigns the value of |src| to dest
 *
 * @param dest [OUT] Pointer to TEE_BigInt to be assigned
 * @param src [IN] Pointer to the source operand
 *
 * @return #TEE_SUCCESS support
 * @return #TEE_ERROR_OVERFLOW In case the dest operand cannot hold the value of |src|
 */
TEE_Result TEE_BigIntAbs(TEE_BigInt *dest, const TEE_BigInt *src);
#endif /* API_LEVEL */

/*
 * computes dest = op1 + op2
 *
 * @param dest [OUT] Pointer to TEE_BigInt to store the result op1 + op2
 * @param op1 [IN] Pointer to the first operand
 * @param op2 [IN] Pointer to the second operand
 *
 * @return void
 */
void TEE_BigIntAdd(TEE_BigInt *dest, const TEE_BigInt *op1, const TEE_BigInt *op2);

/*
 * computes dest = op1 - op2
 *
 * @param dest [OUT] Pointer to TEE_BigInt to store the result op1 - op2
 * @param op1 [IN] Pointer to the first operand
 * @param op2 [IN] Pointer to the second operand
 *
 * @return void
 */
void TEE_BigIntSub(TEE_BigInt *dest, const TEE_BigInt *op1, const TEE_BigInt *op2);

/*
 * negates an operand: dest = -src
 *
 * @param dest [OUT] PPointer to TEE_BigInt to store the result -src
 * @param op [IN] Pointer to the operand to be negated
 *
 * @return void
 */
void TEE_BigIntNeg(TEE_BigInt *dest, const TEE_BigInt *src);

/*
 * computes dest = op1 * op2
 *
 * @param dest [OUT] Pointer to TEE_BigInt to store the result op1 * op2
 * @param op1 [IN] Pointer to the first operand
 * @param op2 [IN] Pointer to the second operand
 *
 * @return void
 */
void TEE_BigIntMul(TEE_BigInt *dest, const TEE_BigInt *op1, const TEE_BigInt *op2);

/*
 * computes dest = op * op
 *
 * @param dest [OUT] Pointer to TEE_BigInt to store the result op * op
 * @param op [IN] Pointer to the operand to be squared
 *
 * @return void
 */
void TEE_BigIntSquare(TEE_BigInt *dest, const TEE_BigInt *op);

/*
 * computes dest_r and dest_q such that op1 = dest_q * op2 + dest_r
 *
 * @param dest_q [OUT] Pointer to a TEE_BigInt to store the quotient
 * @param dest_r [OUT] Pointer to a TEE_BigInt to store the remainder
 * @param op1 [IN] Pointer to the first operand, the dividend
 * @param op2 [IN] Pointer to the second operand, the divisor
 *
 * @return #TEE_SUCCESS operation success
 * @return #TEE_ERROR_BAD_PARAMETERS If any of the parameters is NULL
 */
void TEE_BigIntDiv(TEE_BigInt *dest_q, TEE_BigInt *dest_r, const TEE_BigInt *op1, const TEE_BigInt *op2);

/*
 * computes dest = op (mod n) such that 0 <= dest < n.
 *
 * @param dest [OUT] Pointer to TEE_BigInt to hold the result op (mod n)
 * @param op [IN] Pointer to the operand to be reduced mod n
 * @param n [IN] Pointer to the modulus. Modulus SHALL be larger than 1.
 *
 * @return void
 */
void TEE_BigIntMod(TEE_BigInt *dest, const TEE_BigInt *op, const TEE_BigInt *n);

/*
 * computes dest = (op1 + op2) (mod n).
 *
 * @param dest [OUT] Pointer to TEE_BigInt to hold the result (op1 + op2)(mod n)
 * @param op1 [IN] Pointer to the first operand
 * @param op2 [IN] Pointer to the second operand
 * @param n [IN] Pointer to the modulus. Modulus SHALL be larger than 1
 *
 * @return void
 */
void TEE_BigIntAddMod(TEE_BigInt *dest, const TEE_BigInt *op1, const TEE_BigInt *op2, const TEE_BigInt *n);

/*
 * computes dest = (op1 - op2) (mod n).
 *
 * @param dest [OUT] Pointer to TEE_BigInt to hold the result (op1 - op2)(mod n)
 * @param op1 [IN] Pointer to the first operand
 * @param op2 [IN] Pointer to the second operand
 * @param n [IN] Pointer to the modulus. Modulus SHALL be larger than 1
 *
 * @return void
 */
void TEE_BigIntSubMod(TEE_BigInt *dest, const TEE_BigInt *op1, const TEE_BigInt *op2, const TEE_BigInt *n);

/*
 * computes dest = (op1 * op2) (mod n).
 *
 * @param dest [OUT] Pointer to TEE_BigInt to hold the result (op1 * op2)(mod n)
 * @param op1 [IN] Pointer to the first operand
 * @param op2 [IN] Pointer to the second operand
 * @param n [IN] Pointer to the modulus. Modulus SHALL be larger than 1
 *
 * @return void
 */
void TEE_BigIntMulMod(TEE_BigInt *dest, const TEE_BigInt *op1, const TEE_BigInt *op2, const TEE_BigInt *n);

/*
 * computes dest = (op * op) (mod n).
 *
 * @param dest [OUT] Pointer to TEE_BigInt to hold the result (op * op)(mod n)
 * @param op1 [IN] Pointer to the operand
 * @param n [IN] Pointer to the modulus. Modulus SHALL be larger than 1
 *
 * @return void
 */
void TEE_BigIntSquareMod(TEE_BigInt *dest, const TEE_BigInt *op, const TEE_BigInt *n);

/*
 * computes dest such that dest * op = 1 (mod n).
 *
 * @param dest [OUT] Pointer to TEE_BigInt to hold the result (op^-1)(mod n)
 * @param op1 [IN] Pointer to the operand
 * @param n [IN] Pointer to the modulus. Modulus SHALL be larger than 1
 *
 * @return void
 */
void TEE_BigIntInvMod(TEE_BigInt *dest, const TEE_BigInt *op, const TEE_BigInt *n);

/*
 * determines whether gcd(op1, op2) == 1.
 *
 * @param op1 [IN] Pointer to the first operand
 * @param op2 [IN] Pointer to the second operand
 *
 * @return true if gcd(op1, op2) == 1
 * @return fast otherwise
 */
bool TEE_BigIntRelativePrime(const TEE_BigInt *op1, const TEE_BigInt *op2);

/*
 * computes the greatest common divisor of the input parameters op1 and op2.
 *
 * @param gcd [OUT] Pointer to TEE_BigInt to hold the greatest common divisor of op1 and op2
 * @param u [OUT] Pointer to TEE_BigInt to hold the first coefficient
 * @param v [OUT] Pointer to TEE_BigInt to hold the second coefficient
 * @param op1 [IN] Pointer to the first operand
 * @param op2 [IN] Pointer to the second operand
 *
 * @return void
 */
void TEE_BigIntComputeExtendedGcd(TEE_BigInt *gcd, TEE_BigInt *u, TEE_BigInt *v, const TEE_BigInt *op1,
                                  const TEE_BigInt *op2);
/*
 * performs a probabilistic primality test on op
 *
 * @param op [IN] Candidate number that is tested for primality
 * @param confidenceLevel [IN]  The desired confidence level for a non-conclusive test
 *
 * @return 0 If op is a composite number
 * @return 1 If op is guaranteed to be prime
 * @return -1 If the test is non-conclusive but the probability that op is composite is less than 2^(-confidenceLevel)
 */
int32_t TEE_BigIntIsProbablePrime(const TEE_BigInt *op, uint32_t confidenceLevel);

/*
 * converts src into a representation suitable for doing fast modular multiplicatio
 *
 * @param dest [OUT] Pointer to an initialized TEE_BigIntFMM memory area
 * @param src [IN] Pointer to the TEE_BigInt to convert
 * @param n [IN] Pointer to the modulus
 * @param context [IN] Pointer to a context previously initialized using TEE_BigIntInitFMMContext1
 *
 * @return void
 */
void TEE_BigIntConvertToFMM(TEE_BigIntFMM *dest, const TEE_BigInt *src, const TEE_BigInt *n,
                            const TEE_BigIntFMMContext *context);

/*
 * converts src in the fast modular multiplication representation back to a TEE_BigInt representation
 *
 * @param dest [OUT] Pointer to an initialized TEE_BigIntFMM memory area to hold the converted result
 * @param src [IN] Pointer to a TEE_BigIntFMM holding the value in the fast modular multiplication representation
 * @param n [IN] Pointer to the modulus
 * @param context [IN] Pointer to a context previously initialized using TEE_BigIntInitFMMContext1
 *
 * @return void
 */
void TEE_BigIntConvertFromFMM(TEE_BigInt *dest, const TEE_BigIntFMM *src, const TEE_BigInt *n,
                              const TEE_BigIntFMMContext *context);

/*
 * calculates dest = op1 * op2 in the fast modular multiplication representation
 *
 * @param dest [OUT] Pointer to TEE_BigIntFMM to hold the result op1 * op2
 * @param op1 [IN] Pointer to the first operand
 * @param op2 [IN] Pointer to the second operand
 * @param n [IN] Pointer to the modulus
 * @param context [IN] Pointer to a context previously initialized using TEE_BigIntInitFMMContext1
 *
 * @return void
 */
void TEE_BigIntComputeFMM(TEE_BigIntFMM *dest, const TEE_BigIntFMM *op1, const TEE_BigIntFMM *op2, const TEE_BigInt *n,
                          const TEE_BigIntFMMContext *context);

#if defined(API_LEVEL) && defined(API_LEVEL1_1_1) && (API_LEVEL >= API_LEVEL1_1_1)
/*
 * computes dest = (op1 ^ op2) (mod n).
 *
 * @param dest [OUT] Pointer to TEE_BigInt to hold the result (op1 ^ op2)(mod n)
 * @param op1 [IN] Pointer to the first operand
 * @param op2 [IN] Pointer to the second operand
 * @param n [IN] Pointer to the modulus
 * @param context [IN] Pointer to a context previously initialized using TEE_BigIntInitFMMContext1 or NULL
 *
 * @return #TEE_SUCCESS success
 * @return #TEE_ERROR_NOT_SUPPORTED If the value of n is not supported
 */
TEE_Result TEE_BigIntExpMod(TEE_BigInt *des, TEE_BigInt *op1, const TEE_BigInt *op2, const TEE_BigInt *n,
                            TEE_BigIntFMMContext *context);
#endif /* API_LEVEL */

/*
 * check whether n exists to make dest = (op1 ^ op2) (mod n).
 *
 * @param dest [OUT] Pointer to TEE_BigInt to hold the result (op1 ^ op2)(mod n)
 * @param op1 [IN] Pointer to the first operand
 * @param op2 [IN] Pointer to the second operand
 * @param n [IN] Pointer to the modulus
 * @param context [IN] Pointer to a context previously initialized using TEE_BigIntInitFMMContext1 or NULL
 *
 * @return true If the value of n is supported
 * @return false If the value of n is not supported
 */
bool EXT_TEE_BigIntExpMod(TEE_BigInt *out, TEE_BigInt *in, const TEE_BigInt *exp, const TEE_BigInt *n);
#endif
