#ifndef _HONGMENG_H_
#define _HONGMENG_H_

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

#ifndef container_of
#define container_of(p, type, element)                 \
    ({                                                 \
        type *q = (void *)p - offsetof(type, element); \
        q;                                             \
    })
#endif
#endif /* _HONGMENG_H_ */
