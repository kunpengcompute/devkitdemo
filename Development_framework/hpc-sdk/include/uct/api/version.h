/**
* Copyright (C) Mellanox Technologies Ltd. 2001-2014.  ALL RIGHTS RESERVED.
*
* See file LICENSE for terms.
*/


#ifndef UCT_VERSION_H_
#define UCT_VERSION_H_

#define UCT_VERNO_MAJOR            1
#define UCT_VERNO_MINOR            10
#define UCT_VERNO_STRING           "1.10.1"
#define UCT_SCM_VERSION            "b80f802"
                                   
#define UCT_MINOR_BIT              (16UL)
#define UCT_MAJOR_BIT              (24UL)
#define UCT_API                    ((1L<<UCT_MAJOR_BIT)|(10L << UCT_MINOR_BIT))

#define UCT_VERSION(major, minor)  (((major)<<UCT_MAJOR_BIT)|((minor)<<UCT_MINOR_BIT))

#endif
