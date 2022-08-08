
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "warpdrive/wd.h"
#include "warpdrive/wd_digest.h"
#include "warpdrive/wd_bmm.h"

#define DIGEST_BLOCK_NUM    4
#define DIGEST_BLOCK_SIZE   (512 * 1024)

#define DIGEST_SIZE 32
#define BUFFER_SIZE 256

struct wd_queue_mempool {
    struct wd_queue *q;
    void *base;
    unsigned int *bitmap;
    unsigned int block_size;
    unsigned int mem_size;
    unsigned int block_align_size;
    unsigned int free_num;
    unsigned int fail_times;
    unsigned long long index;
    sem_t mempool_sem;
    int dev;
};

int KAEDriverSM3Digest(unsigned char *in, unsigned int bufSize, unsigned char *digest)
{
    struct wd_queue queue;
    memset(&queue, 0, sizeof(queue));
    queue.capa.alg = "digest";
    wd_request_queue(&queue);

    struct wd_queue_mempool *mempool = NULL;
    struct wd_blkpool_setup pool_setup;
    memset(&pool_setup, 0, sizeof(pool_setup));
    pool_setup.block_size = DIGEST_BLOCK_SIZE;
    pool_setup.block_num = DIGEST_BLOCK_NUM;
    pool_setup.align_size = 64;
    mempool = wd_blkpool_create(&queue, &pool_setup);

    void *ctx = NULL;
    struct wcrypto_digest_ctx_setup setup;
    memset((void *)&setup, 0, sizeof(setup));
    setup.alg = WCRYPTO_SM3;
    setup.mode = WCRYPTO_DIGEST_NORMAL;
    setup.br.alloc = (void *)wd_alloc_blk;
    setup.br.free = (void *)wd_free_blk;
    setup.br.iova_map = (void *)wd_blk_iova_map;
    setup.br.iova_unmap = (void *)wd_blk_iova_unmap;
    setup.br.usr = mempool;
    ctx = wcrypto_create_digest_ctx(&queue, &setup);

    struct wcrypto_digest_op_data op_data;
    memset(&op_data, 0, sizeof(op_data));
    op_data.in = wd_alloc_blk(mempool);
    op_data.priv = NULL;
    op_data.out = wd_alloc_blk(mempool);
    op_data.out_bytes = DIGEST_SIZE;
    memset(op_data.out, 0, op_data.out_bytes);
    op_data.has_next = 0;
    int i = 0;
    int op_bytes = 0;
    // 分块计算哈希值
    do {
        if (bufSize - i > BUFFER_SIZE) {
            op_data.in_bytes = BUFFER_SIZE;
            op_data.has_next = true;
        } else {
            op_data.in_bytes = bufSize - i;
            op_data.has_next = false;
        }
        memcpy(op_data.in, in, op_data.in_bytes);
        wcrypto_do_digest(ctx, &op_data, NULL);
        i += op_data.in_bytes;
        in += op_data.in_bytes;
    } while (i < bufSize);
    memcpy(digest, op_data.out, DIGEST_SIZE);

    // 释放资源
    wd_free_blk(mempool, op_data.in);
    wd_free_blk(mempool, op_data.out);
    wcrypto_del_digest_ctx(ctx);
    wd_release_queue(&queue);
    wd_blkpool_destroy(mempool);
    return 0;
}
