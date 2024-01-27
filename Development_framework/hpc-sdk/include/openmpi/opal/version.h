/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2011 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2016      Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 * This file should be included by any file that needs full
 * version information for the OPAL project
 */

#ifndef OPAL_VERSIONS_H
#define OPAL_VERSIONS_H

#define OPAL_MAJOR_VERSION 4
#define OPAL_MINOR_VERSION 1
#define OPAL_RELEASE_VERSION 1
#define OPAL_GREEK_VERSION "rc4"
#define OPAL_WANT_REPO_REV @OPAL_WANT_REPO_REV@
#define OPAL_REPO_REV "v2.0.0.RC1-huawei-2-g951640a452"
#ifdef OPAL_VERSION
/* If we included version.h, we want the real version, not the
   stripped (no-r number) verstion */
#undef OPAL_VERSION
#endif
#define OPAL_VERSION "4.1.1rc4"
#define OPAL_CONFIGURE_CLI " \'--prefix=/usr1/workspace/HMPI_hmpi_Compile_OpenEuler22.03SP2_Bisheng3.0.0_MLNX5.8_aarch64/hmpi/build/../bin\' \'--disable-picky\' \'--with-platform=contrib/platform/mellanox/optimized\' \'--enable-mpi1-compatibility\' \'--with-ucx=/usr1/workspace/HMPI_hmpi_Compile_OpenEuler22.03SP2_Bisheng3.0.0_MLNX5.8_aarch64/hmpi/build/../3rdparty/ucx/\' \'--with-ucg=/usr1/workspace/HMPI_hmpi_Compile_OpenEuler22.03SP2_Bisheng3.0.0_MLNX5.8_aarch64/hmpi/build/../3rdparty/ucg/\' \'--with-pic\' \'CFLAGS=-fstack-protector-strong -fPIE -pie -lstdc++ -DHAVE___CLEAR=1 -Wno-error=deprecated-declarations -Wno-unused-command-line-argument -Wno-error\' \'CXXFLAGS=-fstack-protector-strong -fPIE -pie -lstdc++ -DHAVE___CLEAR=1 -Wno-error=deprecated-declarations -Wno-unused-command-line-argument -Wno-error\' \'LDFLAGS=-Wl,-z,relro,-z,now,-z,noexecstack,-s\' \'CC=clang\' \'CXX=clang++\' \'FC=flang\'"

#endif
