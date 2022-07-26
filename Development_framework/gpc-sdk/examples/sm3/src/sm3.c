
#include "sm3.h"

void sm3_block_data_order(SM3_CTX *ctx, const void *p, size_t num)
{
    const unsigned char *data = p;
    register unsigned int A, B, C, D, E, F, G, H;

    unsigned int W00, W01, W02, W03, W04, W05, W06, W07, W08, W09, W10, W11, W12, W13, W14, W15;

    for (; num--;) {
        A = ctx->A;
        B = ctx->B;
        C = ctx->C;
        D = ctx->D;
        E = ctx->E;
        F = ctx->F;
        G = ctx->G;
        H = ctx->H;

        (void)HOST_c2l(data, W00);
        (void)HOST_c2l(data, W01);
        (void)HOST_c2l(data, W02);
        (void)HOST_c2l(data, W03);
        (void)HOST_c2l(data, W04);
        (void)HOST_c2l(data, W05);
        (void)HOST_c2l(data, W06);
        (void)HOST_c2l(data, W07);
        (void)HOST_c2l(data, W08);
        (void)HOST_c2l(data, W09);
        (void)HOST_c2l(data, W10);
        (void)HOST_c2l(data, W11);
        (void)HOST_c2l(data, W12);
        (void)HOST_c2l(data, W13);
        (void)HOST_c2l(data, W14);
        (void)HOST_c2l(data, W15);

        R1(A, B, C, D, E, F, G, H, 0x79CC4519, W00, W00 ^ W04);
        W00 = EXPAND(W00, W07, W13, W03, W10);
        R1(D, A, B, C, H, E, F, G, 0xF3988A32, W01, W01 ^ W05);
        W01 = EXPAND(W01, W08, W14, W04, W11);
        R1(C, D, A, B, G, H, E, F, 0xE7311465, W02, W02 ^ W06);
        W02 = EXPAND(W02, W09, W15, W05, W12);
        R1(B, C, D, A, F, G, H, E, 0xCE6228CB, W03, W03 ^ W07);
        W03 = EXPAND(W03, W10, W00, W06, W13);
        R1(A, B, C, D, E, F, G, H, 0x9CC45197, W04, W04 ^ W08);
        W04 = EXPAND(W04, W11, W01, W07, W14);
        R1(D, A, B, C, H, E, F, G, 0x3988A32F, W05, W05 ^ W09);
        W05 = EXPAND(W05, W12, W02, W08, W15);
        R1(C, D, A, B, G, H, E, F, 0x7311465E, W06, W06 ^ W10);
        W06 = EXPAND(W06, W13, W03, W09, W00);
        R1(B, C, D, A, F, G, H, E, 0xE6228CBC, W07, W07 ^ W11);
        W07 = EXPAND(W07, W14, W04, W10, W01);
        R1(A, B, C, D, E, F, G, H, 0xCC451979, W08, W08 ^ W12);
        W08 = EXPAND(W08, W15, W05, W11, W02);
        R1(D, A, B, C, H, E, F, G, 0x988A32F3, W09, W09 ^ W13);
        W09 = EXPAND(W09, W00, W06, W12, W03);
        R1(C, D, A, B, G, H, E, F, 0x311465E7, W10, W10 ^ W14);
        W10 = EXPAND(W10, W01, W07, W13, W04);
        R1(B, C, D, A, F, G, H, E, 0x6228CBCE, W11, W11 ^ W15);
        W11 = EXPAND(W11, W02, W08, W14, W05);
        R1(A, B, C, D, E, F, G, H, 0xC451979C, W12, W12 ^ W00);
        W12 = EXPAND(W12, W03, W09, W15, W06);
        R1(D, A, B, C, H, E, F, G, 0x88A32F39, W13, W13 ^ W01);
        W13 = EXPAND(W13, W04, W10, W00, W07);
        R1(C, D, A, B, G, H, E, F, 0x11465E73, W14, W14 ^ W02);
        W14 = EXPAND(W14, W05, W11, W01, W08);
        R1(B, C, D, A, F, G, H, E, 0x228CBCE6, W15, W15 ^ W03);
        W15 = EXPAND(W15, W06, W12, W02, W09);
        R2(A, B, C, D, E, F, G, H, 0x9D8A7A87, W00, W00 ^ W04);
        W00 = EXPAND(W00, W07, W13, W03, W10);
        R2(D, A, B, C, H, E, F, G, 0x3B14F50F, W01, W01 ^ W05);
        W01 = EXPAND(W01, W08, W14, W04, W11);
        R2(C, D, A, B, G, H, E, F, 0x7629EA1E, W02, W02 ^ W06);
        W02 = EXPAND(W02, W09, W15, W05, W12);
        R2(B, C, D, A, F, G, H, E, 0xEC53D43C, W03, W03 ^ W07);
        W03 = EXPAND(W03, W10, W00, W06, W13);
        R2(A, B, C, D, E, F, G, H, 0xD8A7A879, W04, W04 ^ W08);
        W04 = EXPAND(W04, W11, W01, W07, W14);
        R2(D, A, B, C, H, E, F, G, 0xB14F50F3, W05, W05 ^ W09);
        W05 = EXPAND(W05, W12, W02, W08, W15);
        R2(C, D, A, B, G, H, E, F, 0x629EA1E7, W06, W06 ^ W10);
        W06 = EXPAND(W06, W13, W03, W09, W00);
        R2(B, C, D, A, F, G, H, E, 0xC53D43CE, W07, W07 ^ W11);
        W07 = EXPAND(W07, W14, W04, W10, W01);
        R2(A, B, C, D, E, F, G, H, 0x8A7A879D, W08, W08 ^ W12);
        W08 = EXPAND(W08, W15, W05, W11, W02);
        R2(D, A, B, C, H, E, F, G, 0x14F50F3B, W09, W09 ^ W13);
        W09 = EXPAND(W09, W00, W06, W12, W03);
        R2(C, D, A, B, G, H, E, F, 0x29EA1E76, W10, W10 ^ W14);
        W10 = EXPAND(W10, W01, W07, W13, W04);
        R2(B, C, D, A, F, G, H, E, 0x53D43CEC, W11, W11 ^ W15);
        W11 = EXPAND(W11, W02, W08, W14, W05);
        R2(A, B, C, D, E, F, G, H, 0xA7A879D8, W12, W12 ^ W00);
        W12 = EXPAND(W12, W03, W09, W15, W06);
        R2(D, A, B, C, H, E, F, G, 0x4F50F3B1, W13, W13 ^ W01);
        W13 = EXPAND(W13, W04, W10, W00, W07);
        R2(C, D, A, B, G, H, E, F, 0x9EA1E762, W14, W14 ^ W02);
        W14 = EXPAND(W14, W05, W11, W01, W08);
        R2(B, C, D, A, F, G, H, E, 0x3D43CEC5, W15, W15 ^ W03);
        W15 = EXPAND(W15, W06, W12, W02, W09);
        R2(A, B, C, D, E, F, G, H, 0x7A879D8A, W00, W00 ^ W04);
        W00 = EXPAND(W00, W07, W13, W03, W10);
        R2(D, A, B, C, H, E, F, G, 0xF50F3B14, W01, W01 ^ W05);
        W01 = EXPAND(W01, W08, W14, W04, W11);
        R2(C, D, A, B, G, H, E, F, 0xEA1E7629, W02, W02 ^ W06);
        W02 = EXPAND(W02, W09, W15, W05, W12);
        R2(B, C, D, A, F, G, H, E, 0xD43CEC53, W03, W03 ^ W07);
        W03 = EXPAND(W03, W10, W00, W06, W13);
        R2(A, B, C, D, E, F, G, H, 0xA879D8A7, W04, W04 ^ W08);
        W04 = EXPAND(W04, W11, W01, W07, W14);
        R2(D, A, B, C, H, E, F, G, 0x50F3B14F, W05, W05 ^ W09);
        W05 = EXPAND(W05, W12, W02, W08, W15);
        R2(C, D, A, B, G, H, E, F, 0xA1E7629E, W06, W06 ^ W10);
        W06 = EXPAND(W06, W13, W03, W09, W00);
        R2(B, C, D, A, F, G, H, E, 0x43CEC53D, W07, W07 ^ W11);
        W07 = EXPAND(W07, W14, W04, W10, W01);
        R2(A, B, C, D, E, F, G, H, 0x879D8A7A, W08, W08 ^ W12);
        W08 = EXPAND(W08, W15, W05, W11, W02);
        R2(D, A, B, C, H, E, F, G, 0x0F3B14F5, W09, W09 ^ W13);
        W09 = EXPAND(W09, W00, W06, W12, W03);
        R2(C, D, A, B, G, H, E, F, 0x1E7629EA, W10, W10 ^ W14);
        W10 = EXPAND(W10, W01, W07, W13, W04);
        R2(B, C, D, A, F, G, H, E, 0x3CEC53D4, W11, W11 ^ W15);
        W11 = EXPAND(W11, W02, W08, W14, W05);
        R2(A, B, C, D, E, F, G, H, 0x79D8A7A8, W12, W12 ^ W00);
        W12 = EXPAND(W12, W03, W09, W15, W06);
        R2(D, A, B, C, H, E, F, G, 0xF3B14F50, W13, W13 ^ W01);
        W13 = EXPAND(W13, W04, W10, W00, W07);
        R2(C, D, A, B, G, H, E, F, 0xE7629EA1, W14, W14 ^ W02);
        W14 = EXPAND(W14, W05, W11, W01, W08);
        R2(B, C, D, A, F, G, H, E, 0xCEC53D43, W15, W15 ^ W03);
        W15 = EXPAND(W15, W06, W12, W02, W09);
        R2(A, B, C, D, E, F, G, H, 0x9D8A7A87, W00, W00 ^ W04);
        W00 = EXPAND(W00, W07, W13, W03, W10);
        R2(D, A, B, C, H, E, F, G, 0x3B14F50F, W01, W01 ^ W05);
        W01 = EXPAND(W01, W08, W14, W04, W11);
        R2(C, D, A, B, G, H, E, F, 0x7629EA1E, W02, W02 ^ W06);
        W02 = EXPAND(W02, W09, W15, W05, W12);
        R2(B, C, D, A, F, G, H, E, 0xEC53D43C, W03, W03 ^ W07);
        W03 = EXPAND(W03, W10, W00, W06, W13);
        R2(A, B, C, D, E, F, G, H, 0xD8A7A879, W04, W04 ^ W08);
        R2(D, A, B, C, H, E, F, G, 0xB14F50F3, W05, W05 ^ W09);
        R2(C, D, A, B, G, H, E, F, 0x629EA1E7, W06, W06 ^ W10);
        R2(B, C, D, A, F, G, H, E, 0xC53D43CE, W07, W07 ^ W11);
        R2(A, B, C, D, E, F, G, H, 0x8A7A879D, W08, W08 ^ W12);
        R2(D, A, B, C, H, E, F, G, 0x14F50F3B, W09, W09 ^ W13);
        R2(C, D, A, B, G, H, E, F, 0x29EA1E76, W10, W10 ^ W14);
        R2(B, C, D, A, F, G, H, E, 0x53D43CEC, W11, W11 ^ W15);
        R2(A, B, C, D, E, F, G, H, 0xA7A879D8, W12, W12 ^ W00);
        R2(D, A, B, C, H, E, F, G, 0x4F50F3B1, W13, W13 ^ W01);
        R2(C, D, A, B, G, H, E, F, 0x9EA1E762, W14, W14 ^ W02);
        R2(B, C, D, A, F, G, H, E, 0x3D43CEC5, W15, W15 ^ W03);

        ctx->A ^= A;
        ctx->B ^= B;
        ctx->C ^= C;
        ctx->D ^= D;
        ctx->E ^= E;
        ctx->F ^= F;
        ctx->G ^= G;
        ctx->H ^= H;
    }
}

int sm3_init(SM3_CTX *c)
{
    memset(c, 0, sizeof(*c));
    c->A = SM3_A;
    c->B = SM3_B;
    c->C = SM3_C;
    c->D = SM3_D;
    c->E = SM3_E;
    c->F = SM3_F;
    c->G = SM3_G;
    c->H = SM3_H;
    return 1;
}

int sm3_update(SM3_CTX *c, const void *data_, size_t len)
{
    const unsigned char *data = data_;
    unsigned char *p;
    SM3_WORD l;
    size_t n;

    if (len == 0) {
        return 1;
    }

    l = (c->Nl + (((SM3_WORD) len) << 3)) & 0xffffffffUL;
    if (l < c->Nl) {
        c->Nh++;
    }
    c->Nh += (SM3_WORD) (len >> 29); 
    c->Nl = l;

    n = c->num;
    if (n != 0) {
        p = (unsigned char *)c->data;
        if (len >= SM3_CBLOCK || len + n >= SM3_CBLOCK) {
            memcpy(p + n, data, SM3_CBLOCK - n);
            sm3_block_data_order(c, p, 1);
            n = SM3_CBLOCK - n;
            data += n;
            len -= n;
            c->num = 0;
            memset(p, 0, SM3_CBLOCK);
        } else {
            memcpy(p + n, data, len);
            c->num += (unsigned int)len;
            return 1;
        }
    }

    n = len / SM3_CBLOCK;
    if (n > 0) {
        sm3_block_data_order(c, data, n);
        n *= SM3_CBLOCK;
        data += n;
        len -= n;
    }

    if (len != 0) {
        p = (unsigned char *)c->data;
        c->num = (unsigned int)len;
        memcpy(p, data, len);
    }
    return 1;
}

int sm3_final(unsigned char *md, SM3_CTX *c)
{
    unsigned char *p = (unsigned char *)c->data;
    size_t n = c->num;

    p[n] = 0x80;
    n++;

    if (n > (SM3_CBLOCK - 8)) {
        memset(p + n, 0, SM3_CBLOCK - n);
        n = 0;
        sm3_block_data_order(c, p, 1);
    }
    memset(p + n, 0, SM3_CBLOCK - 8 - n);

    p += SM3_CBLOCK - 8;
    (void)HOST_l2c(c->Nh, p);
    (void)HOST_l2c(c->Nl, p);
    p -= SM3_CBLOCK;
    sm3_block_data_order(c, p, 1);
    c->num = 0;
    memset(p, 0, SM3_CBLOCK);
    HASH_MAKE_STRING(c, md);
    return 1;
}