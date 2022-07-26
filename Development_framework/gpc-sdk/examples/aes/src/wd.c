#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "warpdrive/wd.h"
#include "warpdrive/wd_cipher.h"
#include "warpdrive/wd_bmm.h"

#include <aes.h>

#define CIPHER_BLOCK_NUM    4
#define CIPHER_BLOCK_SIZE   (272 * 1024)

struct wd_queue_mempool {
    struct wd_queue *q;
    void *base;
    unsigned int *bitmap;
    unsigned int block_size;
    unsigned int block_num;
    unsigned int mem_size;
    unsigned int block_align_size;
    unsigned int free_num;
    unsigned int fail_times;
    unsigned long long index;
    sem_t mempool_sem;
    int dev;
};

int WDEncrypt(Param *param, char *in, size_t bufSize, char *out, BLOCK_CIPHER_MODE mode)
{
    struct wd_queue queue;
    memset(&queue, 0, sizeof(queue));
    queue.capa.alg = "cipher";
    wd_request_queue(&queue);

    struct wd_queue_mempool *mempool = NULL;
    struct wd_blkpool_setup pool_setup;
    memset(&pool_setup, 0, sizeof(pool_setup));
    pool_setup.block_size = CIPHER_BLOCK_SIZE;
    pool_setup.block_num = CIPHER_BLOCK_NUM;
    pool_setup.align_size = 64;
    mempool = wd_blkpool_create(&queue, &pool_setup);

    struct wcrypto_cipher_ctx_setup setup;
    void *ctx = NULL;

    memset((void *)&setup, 0, sizeof(setup));
    setup.alg = WCRYPTO_CIPHER_AES;
    switch (mode)
    {
        case ECB_MODE:
            setup.mode = WCRYPTO_CIPHER_ECB;
            break;
        case CBC_MODE:
            setup.mode = WCRYPTO_CIPHER_CBC;
            break;
    
        default:
            break;
    }
    setup.br.alloc = (void *)wd_alloc_blk;
    setup.br.free = (void *)wd_free_blk;
    setup.br.iova_map = (void *)wd_blk_iova_map;
    setup.br.iova_unmap = (void *)wd_blk_iova_unmap;
    setup.br.usr = mempool;
    ctx = wcrypto_create_cipher_ctx(&queue, &setup);
    wcrypto_set_cipher_key(ctx, param->key, param->keyLen);

    struct wcrypto_cipher_op_data op_data;
    memset((void *)&op_data, 0, sizeof(op_data));

    op_data.op_type = param->enc ? WCRYPTO_CIPHER_ENCRYPTION : WCRYPTO_CIPHER_DECRYPTION;
    unsigned char *iv = (unsigned char *)"1234567812345678";
    size_t iv_len = 16;
    op_data.iv = wd_alloc_blk(mempool);
    memcpy(op_data.iv, iv, iv_len);
    op_data.iv_bytes = iv_len;

    op_data.in = wd_alloc_blk(mempool);
    op_data.out = wd_alloc_blk(mempool);

    int i = 0;
    int op_bytes = 0;
    do {
        op_bytes = bufSize - i > CIPHER_BLOCK_SIZE ? CIPHER_BLOCK_SIZE : bufSize - i;
        memcpy(op_data.in, in + i, op_bytes);
        op_data.in_bytes = op_bytes;
        op_data.out_bytes = op_bytes;
        wcrypto_do_cipher(ctx, &op_data, NULL);
        memcpy(out + i, op_data.out, op_bytes);
        i += CIPHER_BLOCK_SIZE;
    } while (i < bufSize);

    wd_free_blk(mempool, op_data.in);
    wd_free_blk(mempool, op_data.out);
    wd_free_blk(mempool, op_data.iv);
    wcrypto_del_cipher_ctx(ctx);
    wd_release_queue(&queue);
    wd_blkpool_destroy(mempool);
    return 0;
}
