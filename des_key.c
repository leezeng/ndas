#ifndef NDAS_NO_LANSCSI

// IBM PC Implementation of the DES Cryptographic Algorithm
//
// Key Schedule Routine
//
// by Dr B R Gladman (gladman@seven77.demon.co.uk)

#include "inc/sal/types.h"
#include "des.h"

// key shift table

unsigned char   ks_tab[] =
{    0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0
};

xuint32 p2_tab[8][64] =
{
  { 0x00000000L, 0x00000010L, 0x20000000L, 0x20000010L,
    0x00010000L, 0x00010010L, 0x20010000L, 0x20010010L,
    0x00000800L, 0x00000810L, 0x20000800L, 0x20000810L,
    0x00010800L, 0x00010810L, 0x20010800L, 0x20010810L,
    0x00000020L, 0x00000030L, 0x20000020L, 0x20000030L,
    0x00010020L, 0x00010030L, 0x20010020L, 0x20010030L,
    0x00000820L, 0x00000830L, 0x20000820L, 0x20000830L,
    0x00010820L, 0x00010830L, 0x20010820L, 0x20010830L,
    0x00080000L, 0x00080010L, 0x20080000L, 0x20080010L,
    0x00090000L, 0x00090010L, 0x20090000L, 0x20090010L,
    0x00080800L, 0x00080810L, 0x20080800L, 0x20080810L,
    0x00090800L, 0x00090810L, 0x20090800L, 0x20090810L,
    0x00080020L, 0x00080030L, 0x20080020L, 0x20080030L,
    0x00090020L, 0x00090030L, 0x20090020L, 0x20090030L,
    0x00080820L, 0x00080830L, 0x20080820L, 0x20080830L,
    0x00090820L, 0x00090830L, 0x20090820L, 0x20090830L
  },
  { 0x00000000L, 0x00200000L, 0x00000004L, 0x00200004L,
    0x00000400L, 0x00200400L, 0x00000404L, 0x00200404L,
    0x10000000L, 0x10200000L, 0x10000004L, 0x10200004L,
    0x10000400L, 0x10200400L, 0x10000404L, 0x10200404L,
    0x00000001L, 0x00200001L, 0x00000005L, 0x00200005L,
    0x00000401L, 0x00200401L, 0x00000405L, 0x00200405L,
    0x10000001L, 0x10200001L, 0x10000005L, 0x10200005L,
    0x10000401L, 0x10200401L, 0x10000405L, 0x10200405L,
    0x00040000L, 0x00240000L, 0x00040004L, 0x00240004L,
    0x00040400L, 0x00240400L, 0x00040404L, 0x00240404L,
    0x10040000L, 0x10240000L, 0x10040004L, 0x10240004L,
    0x10040400L, 0x10240400L, 0x10040404L, 0x10240404L,
    0x00040001L, 0x00240001L, 0x00040005L, 0x00240005L,
    0x00040401L, 0x00240401L, 0x00040405L, 0x00240405L,
    0x10040001L, 0x10240001L, 0x10040005L, 0x10240005L,
    0x10040401L, 0x10240401L, 0x10040405L, 0x10240405L
  },
  { 0x00000000L, 0x00000002L, 0x00000008L, 0x0000000aL,
    0x00000200L, 0x00000202L, 0x00000208L, 0x0000020aL,
    0x08000000L, 0x08000002L, 0x08000008L, 0x0800000aL,
    0x08000200L, 0x08000202L, 0x08000208L, 0x0800020aL,
    0x00100000L, 0x00100002L, 0x00100008L, 0x0010000aL,
    0x00100200L, 0x00100202L, 0x00100208L, 0x0010020aL,
    0x08100000L, 0x08100002L, 0x08100008L, 0x0810000aL,
    0x08100200L, 0x08100202L, 0x08100208L, 0x0810020aL,
    0x00000100L, 0x00000102L, 0x00000108L, 0x0000010aL,
    0x00000300L, 0x00000302L, 0x00000308L, 0x0000030aL,
    0x08000100L, 0x08000102L, 0x08000108L, 0x0800010aL,
    0x08000300L, 0x08000302L, 0x08000308L, 0x0800030aL,
    0x00100100L, 0x00100102L, 0x00100108L, 0x0010010aL,
    0x00100300L, 0x00100302L, 0x00100308L, 0x0010030aL,
    0x08100100L, 0x08100102L, 0x08100108L, 0x0810010aL,
    0x08100300L, 0x08100302L, 0x08100308L, 0x0810030aL
  },
  { 0x00000000L, 0x01000000L, 0x00001000L, 0x01001000L,
    0x04000000L, 0x05000000L, 0x04001000L, 0x05001000L,
    0x00020000L, 0x01020000L, 0x00021000L, 0x01021000L,
    0x04020000L, 0x05020000L, 0x04021000L, 0x05021000L,
    0x02000000L, 0x03000000L, 0x02001000L, 0x03001000L,
    0x06000000L, 0x07000000L, 0x06001000L, 0x07001000L,
    0x02020000L, 0x03020000L, 0x02021000L, 0x03021000L,
    0x06020000L, 0x07020000L, 0x06021000L, 0x07021000L,
    0x00002000L, 0x01002000L, 0x00003000L, 0x01003000L,
    0x04002000L, 0x05002000L, 0x04003000L, 0x05003000L,
    0x00022000L, 0x01022000L, 0x00023000L, 0x01023000L,
    0x04022000L, 0x05022000L, 0x04023000L, 0x05023000L,
    0x02002000L, 0x03002000L, 0x02003000L, 0x03003000L,
    0x06002000L, 0x07002000L, 0x06003000L, 0x07003000L,
    0x02022000L, 0x03022000L, 0x02023000L, 0x03023000L,
    0x06022000L, 0x07022000L, 0x06023000L, 0x07023000L
  },
  { 0x00000000L, 0x10000000L, 0x00010000L, 0x10010000L,
    0x00000004L, 0x10000004L, 0x00010004L, 0x10010004L,
    0x20000000L, 0x30000000L, 0x20010000L, 0x30010000L,
    0x20000004L, 0x30000004L, 0x20010004L, 0x30010004L,
    0x00100000L, 0x10100000L, 0x00110000L, 0x10110000L,
    0x00100004L, 0x10100004L, 0x00110004L, 0x10110004L,
    0x20100000L, 0x30100000L, 0x20110000L, 0x30110000L,
    0x20100004L, 0x30100004L, 0x20110004L, 0x30110004L,
    0x00001000L, 0x10001000L, 0x00011000L, 0x10011000L,
    0x00001004L, 0x10001004L, 0x00011004L, 0x10011004L,
    0x20001000L, 0x30001000L, 0x20011000L, 0x30011000L,
    0x20001004L, 0x30001004L, 0x20011004L, 0x30011004L,
    0x00101000L, 0x10101000L, 0x00111000L, 0x10111000L,
    0x00101004L, 0x10101004L, 0x00111004L, 0x10111004L,
    0x20101000L, 0x30101000L, 0x20111000L, 0x30111000L,
    0x20101004L, 0x30101004L, 0x20111004L, 0x30111004L
  },
  { 0x00000000L, 0x00000008L, 0x00000100L, 0x00000108L,
    0x00000400L, 0x00000408L, 0x00000500L, 0x00000508L,
    0x00020000L, 0x00020008L, 0x00020100L, 0x00020108L,
    0x00020400L, 0x00020408L, 0x00020500L, 0x00020508L,
    0x00000001L, 0x00000009L, 0x00000101L, 0x00000109L,
    0x00000401L, 0x00000409L, 0x00000501L, 0x00000509L,
    0x00020001L, 0x00020009L, 0x00020101L, 0x00020109L,
    0x00020401L, 0x00020409L, 0x00020501L, 0x00020509L,
    0x02000000L, 0x02000008L, 0x02000100L, 0x02000108L,
    0x02000400L, 0x02000408L, 0x02000500L, 0x02000508L,
    0x02020000L, 0x02020008L, 0x02020100L, 0x02020108L,
    0x02020400L, 0x02020408L, 0x02020500L, 0x02020508L,
    0x02000001L, 0x02000009L, 0x02000101L, 0x02000109L,
    0x02000401L, 0x02000409L, 0x02000501L, 0x02000509L,
    0x02020001L, 0x02020009L, 0x02020101L, 0x02020109L,
    0x02020401L, 0x02020409L, 0x02020501L, 0x02020509L
  },
  { 0x00000000L, 0x00080000L, 0x01000000L, 0x01080000L,
    0x00000010L, 0x00080010L, 0x01000010L, 0x01080010L,
    0x00200000L, 0x00280000L, 0x01200000L, 0x01280000L,
    0x00200010L, 0x00280010L, 0x01200010L, 0x01280010L,
    0x00000200L, 0x00080200L, 0x01000200L, 0x01080200L,
    0x00000210L, 0x00080210L, 0x01000210L, 0x01080210L,
    0x00200200L, 0x00280200L, 0x01200200L, 0x01280200L,
    0x00200210L, 0x00280210L, 0x01200210L, 0x01280210L,
    0x04000000L, 0x04080000L, 0x05000000L, 0x05080000L,
    0x04000010L, 0x04080010L, 0x05000010L, 0x05080010L,
    0x04200000L, 0x04280000L, 0x05200000L, 0x05280000L,
    0x04200010L, 0x04280010L, 0x05200010L, 0x05280010L,
    0x04000200L, 0x04080200L, 0x05000200L, 0x05080200L,
    0x04000210L, 0x04080210L, 0x05000210L, 0x05080210L,
    0x04200200L, 0x04280200L, 0x05200200L, 0x05280200L,
    0x04200210L, 0x04280210L, 0x05200210L, 0x05280210L
  },
  { 0x00000000L, 0x00002000L, 0x08000000L, 0x08002000L,
    0x00000020L, 0x00002020L, 0x08000020L, 0x08002020L,
    0x00000800L, 0x00002800L, 0x08000800L, 0x08002800L,
    0x00000820L, 0x00002820L, 0x08000820L, 0x08002820L,
    0x00040000L, 0x00042000L, 0x08040000L, 0x08042000L,
    0x00040020L, 0x00042020L, 0x08040020L, 0x08042020L,
    0x00040800L, 0x00042800L, 0x08040800L, 0x08042800L,
    0x00040820L, 0x00042820L, 0x08040820L, 0x08042820L,
    0x00000002L, 0x00002002L, 0x08000002L, 0x08002002L,
    0x00000022L, 0x00002022L, 0x08000022L, 0x08002022L,
    0x00000802L, 0x00002802L, 0x08000802L, 0x08002802L,
    0x00000822L, 0x00002822L, 0x08000822L, 0x08002822L,
    0x00040002L, 0x00042002L, 0x08040002L, 0x08042002L,
    0x00040022L, 0x00042022L, 0x08040022L, 0x08042022L,
    0x00040802L, 0x00042802L, 0x08040802L, 0x08042802L,
    0x00040822L, 0x00042822L, 0x08040822L, 0x08042822L
  }
};

/* Conver 32 bit integer to little endian on any platform */
#define int_to_little_endian(v) \
        ((((char*)&(v))[0] & 0xff) |\
        (((xuint32) ((char*)&(v))[1] & 0xff) << 8 ) | \
        (((xuint32) ((char*)&(v))[2] & 0xff) << 16 ) |\
        (((xuint32) ((char*)&(v))[3] & 0xff) << 24))
        

void des_ky(void *kval, void *key)
{   
    xuint32   v0, v1, l0, l1, tt;
    unsigned short  i;

    l0 = int_to_little_endian(*((int*)kval));
    l1 = int_to_little_endian(*((int*)kval+1));

    bit_swap(l1, l0, 4, 0x0f0f0f0f);
    
    bit_swap(l0, l1, 4, 0x01010101);
    
    bit_swap(l0, l0, 9, 0x00550055);
    
    bit_swap(l1, l1, 9, 0x00550055);
    
    bit_swap(l0, l0, 18, 0x00003333);
    
    bit_swap(l1, l1, 18, 0x00003333);
    
    bit_swap(l1, l1, 16, 0x000000ff);
    
    for(i = 0; i < 16; i++) 
    {   
        l0 &= 0x0fffffffL; l1 &= 0x0fffffffL;
        
        if(ks_tab[i])
        {   
            l0 = (l0 >> 2) | (l0 << 26); 
            l1 = (l1 >> 2) | (l1 << 26);
        }
        else
        {   
            l0 = (l0 >> 1) | (l0 << 27); 
            l1 = (l1 >> 1) | (l1 << 27);
        }
        
        v0 = p2_tab[0][byte(l0,0) & 0x3f]
            | p2_tab[1][(byte(l0,1) >> 1) & 0x3f] 
            | p2_tab[2][(byte(l0,2) & 0x1d) | ((byte(l0,2) >> 1) & 0x20) | ((byte(l0,2) >> 6) & 0x02)]
            | p2_tab[3][((byte(l0,0) >> 2) & 0x30) | ((byte(l0,1) >> 7) & 0x01)    | (byte(l0,3) & 0x0e)];
        
        v1 = p2_tab[4][byte(l1,0) & 0x3f]
            | p2_tab[5][(byte(l1,1) & 0x3d) | ((byte(l1,1) >> 6) & 0x02)]
            | p2_tab[6][byte(l1,2) & 0x3f]
            | p2_tab[7][((byte(l1,0) >> 6) & 0x02) | ((byte(l1,2) >> 2) & 0x30)    | (byte(l1,3) & 0x0d)];
        
        ((xuint32*)key)[2 * i]     = (v0 & 0x0000ffff) | (v1 << 16);
        ((xuint32*)key)[2 * i + 1] = (v1 & 0xffff0000) | (v0 >> 16);
    }
}

#endif /* #ifndef NDAS_NO_LANSCSI */

