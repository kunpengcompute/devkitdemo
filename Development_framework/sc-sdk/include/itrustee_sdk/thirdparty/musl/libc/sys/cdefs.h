#ifndef __HM_TEEOS_LIBC_COMPATABLE_CDEFS_H_
#define __HM_TEEOS_LIBC_COMPATABLE_CDEFS_H_

#define __flexarr []
#define __always_inline __attribute__((always_inline))

#ifdef __cplusplus
// c++ end_decls
#define __END_DECLS }
// c++ throw
#define __THROW throw ()
// c++ begin
#define __BEGIN_DECLS extern "C" {
#else
// for c language macros
#define __END_DECLS
#define __THROW
// in c language __BEGIN_DELS is empty.
#define __BEGIN_DECLS
#endif

#endif /* __HM_TEEOS_LIBC_COMPATABLE_CDEFS_H_ */