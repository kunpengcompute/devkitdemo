/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2023. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: linked list data types and basic operations
 */

#ifndef TEE_CLIENT_LIST_H
#define TEE_CLIENT_LIST_H

struct ListNode {
    struct ListNode *next;  /* point to next node  */
    struct ListNode *prev;  /* point to prev node */
};

#define OFFSET_OF(type, member) (unsigned long)(&(((type *)0)->member))
#define CONTAINER_OF(pos, type, member) (type *)(uintptr_t)(((char *)(pos)) - OFFSET_OF(type, member))

#define LIST_DECLARE(name) \
    struct ListNode name = { \
        .next = &(name), \
        .prev = &(name), \
    }

static inline void ListInit(struct ListNode *list)
{
    list->next = list;
    list->prev = list;
}

#define LIST_HEAD(list) ((list)->next)
#define LIST_TAIL(list) ((list)->prev)
#define LIST_EMPTY(list) ((list) == (list)->next)

static inline void ListInsertHead(struct ListNode *list, struct ListNode *entry)
{
    list->next->prev = entry;
    entry->next = list->next;
    entry->prev = list;
    list->next = entry;
}

static inline void ListInsertTail(struct ListNode *list, struct ListNode *entry)
{
    entry->next = list;
    entry->prev = list->prev;
    list->prev->next = entry;
    list->prev = entry;
}

static inline void ListRemoveEntry(struct ListNode *entry)
{
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
}

static inline struct ListNode *ListRemoveHead(struct ListNode *list)
{
    struct ListNode *entry = NULL;
    if (!LIST_EMPTY(list)) {
        entry = list->next;
        ListRemoveEntry(entry);
    }
    return entry;
}

static inline struct ListNode *ListRemoveTail(struct ListNode *list)
{
    struct ListNode *entry = NULL;
    if (!LIST_EMPTY(list)) {
        entry = list->prev;
        ListRemoveEntry(entry);
    }
    return entry;
}

#define LIST_ENTRY(ptr, type, member) \
    ((type *)(((char *)(ptr)) - (unsigned long)(&(((type *)0)->member))))

#define LIST_FOR_EACH(pos, list) \
    for ((pos) = (list)->next; (pos) != (list); (pos) = (pos)->next)

#define LIST_FOR_EACH_SAFE(pos, n, list) \
    for ((pos) = (list)->next, (n) = (pos)->next; (pos) != (list); (pos) = (n), (n) = (pos)->next)

#define LIST_FOR_EACH_ENTRY(pos, list, member)                                                  \
    for ((pos) = LIST_ENTRY((list)->next, typeof(*(pos)), member); &(pos)->member != (list);    \
        (pos) = LIST_ENTRY((pos)->member.next, typeof(*(pos)), member))

#define LIST_FOR_EACH_ENTRY_SAFE(pos, n, list, member)                                                   \
    for ((pos) = LIST_ENTRY((list)->next, typeof(*(pos)), member),                                       \
        (n) = LIST_ENTRY((pos)->member.next, typeof(*(pos)), member);                                    \
        &(pos)->member != (list); (pos) = (n), (n) = LIST_ENTRY((n)->member.next, typeof(*(n)), member))

#endif
