#include "sprites.h"

const uint16_t sprite_snake_head [] = {
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0x0000, 0x0000, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0x0000, 0x32a0, 0x32a0, 0x32a0, 0x5480, 0x5480, 0x5480, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f,
        0x32a0, 0x32a0, 0x5480, 0x5480, 0x5480, 0x5480, 0x32a0, 0x32a0, 0x0000, 0x0000, 0x32a0, 0x32a0, 0x0000, 0x0000, 0xf81f, 0xf81f,
        0x32a0, 0x5480, 0x5480, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x0000, 0xffe0, 0xffe0, 0x0000, 0x32a0, 0x32a0, 0x0000, 0xf81f, 0xf800,
        0x5480, 0x32a0, 0x32a0, 0xce00, 0xce00, 0xce00, 0x32a0, 0x32a0, 0x0000, 0x0000, 0x32a0, 0x32a0, 0x21e0, 0x0000, 0xf81f, 0xf800,
        0xce00, 0xce00, 0xce00, 0xce00, 0xce00, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x0000, 0xf800, 0xf81f,
        0xce00, 0xce00, 0xce00, 0xce00, 0xce00, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x0000, 0xf800, 0xf81f,
        0x5480, 0x32a0, 0x32a0, 0xce00, 0xce00, 0xce00, 0x32a0, 0x32a0, 0x0000, 0x0000, 0x32a0, 0x32a0, 0x21e0, 0x0000, 0xf81f, 0xf800,
        0x32a0, 0x5480, 0x5480, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x0000, 0xffe0, 0xffe0, 0x0000, 0x32a0, 0x32a0, 0x0000, 0xf81f, 0xf800,
        0x32a0, 0x32a0, 0x5480, 0x5480, 0x5480, 0x5480, 0x32a0, 0x32a0, 0x0000, 0x0000, 0x32a0, 0x32a0, 0x0000, 0x0000, 0xf81f, 0xf81f,
        0x0000, 0x32a0, 0x32a0, 0x32a0, 0x5480, 0x5480, 0x5480, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f,
        0x0000, 0x0000, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x32a0, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f
};

// 'sprite_logo', 32x32px
const uint16_t sprite_logo [] = {
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xe81d, 0x622c, 0xb5b7, 0xb5b7, 0xb5b7, 0xb5b6, 0x8cd3, 0x8cd3, 0x8cd3, 0x8cd3,
        0x73af, 0x3a4a, 0x3a4a, 0x3a4a, 0x84b2, 0x8d34, 0x8d34, 0x8d34, 0x51eb, 0xe81d, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x702e, 0xdefc, 0xf7df, 0xf7df, 0xf7df, 0xe77d, 0xbe9a, 0xcefb, 0xae18, 0xa5d7,
        0x8c93, 0x9cf4, 0x73d0, 0x5b2e, 0x9d55, 0xd73c, 0xbe9a, 0xc6fc, 0xb659, 0x61ad, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x9012, 0xce59, 0xf7df, 0xf7df, 0xf7df, 0xf79e, 0xb5b6, 0x73ae, 0x0000, 0x0000, 0x0000,
        0x528a, 0x6b4d, 0x9492, 0x7bef, 0x630c, 0x0000, 0x0000, 0x9d34, 0xcf1c, 0x9db7, 0x780f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xb016, 0xa514, 0xf7df, 0xf7df, 0xf7df, 0xe73c, 0x52aa, 0x0000, 0x0000, 0x0000, 0x0000, 0x528a,
        0x528a, 0x0000, 0x0000, 0x0000, 0x0000, 0x9cd3, 0x5acb, 0x7bef, 0x8410, 0xadf7, 0x9d75, 0xa014, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xd01a, 0x7bef, 0xf7df, 0xf7df, 0xf7df, 0xef7d, 0x4228, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8430,
        0x630c, 0xe71c, 0x7bcf, 0x0000, 0x0000, 0x73ae, 0x0000, 0x0000, 0x632c, 0x0000, 0x5aeb, 0x10a2, 0xb817, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xe81d, 0x6acd, 0xf79e, 0xf7df, 0xf7df, 0xf7df, 0xce79, 0x8c51, 0xa534, 0x630c, 0x0000, 0x0000, 0x0000, 0x8410,
        0xad55, 0xffdf, 0x94b2, 0x0000, 0x0000, 0x5aeb, 0x73ae, 0x0000, 0x738e, 0x6b6d, 0x0000, 0x0000, 0x0000, 0xd01a, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0x6a2d, 0xe71c, 0xf7df, 0xf7df, 0xf7df, 0xf7df, 0x632c, 0x7bef, 0x632c, 0x0000, 0x0000, 0x0000, 0x0000, 0x5acb,
        0x4228, 0x6b4d, 0x4228, 0x0000, 0x0000, 0x0000, 0x2104, 0x7bcf, 0x0000, 0x0000, 0x8410, 0x0000, 0x0000, 0x3807, 0xe81d, 0xf81f,
        0xf81f, 0xd81b, 0x636d, 0xd73d, 0xd73d, 0xd75d, 0xdf5d, 0xe79e, 0x0000, 0x39c7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x73ae,
        0x0000, 0x0000, 0x7bef, 0x0000, 0x0000, 0x0000, 0x0000, 0x6b4d, 0x0000, 0x6b6d, 0x0000, 0x0000, 0x0000, 0x2004, 0xe81d, 0xf81f,
        0xf81f, 0xf81f, 0xb016, 0x8d14, 0xc6fc, 0xc6fc, 0xc6fc, 0xcf1c, 0x39c7, 0x73ae, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7bef,
        0x0000, 0x0000, 0x0000, 0x8c51, 0x0000, 0x0000, 0x0000, 0x4a49, 0x7bcf, 0x6b4d, 0x0000, 0x0000, 0x0000, 0xc018, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0x780f, 0xae18, 0xc6fc, 0xc6fc, 0xc6fc, 0x8c92, 0x4228, 0x9cf3, 0x630c, 0x0000, 0x0000, 0x0000, 0x8c51,
        0x0000, 0x0000, 0x0000, 0x0000, 0x8c51, 0x0000, 0x0000, 0x7bcf, 0x8c71, 0x0000, 0x0000, 0x0000, 0x9012, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xe01c, 0xe81d, 0xe81d, 0x51eb, 0xbedb, 0xc6fc, 0xc6fc, 0xbe59, 0x4208, 0x8430, 0x9cd3, 0x0000, 0x0000, 0x0000, 0x7bcf,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7bcf, 0x8c51, 0x632c, 0x9492, 0x0000, 0x0000, 0x4809, 0xf81f, 0xe81d, 0xe01c, 0xf81f,
        0xf81f, 0xe01c, 0xb520, 0xc48e, 0x91f0, 0x63af, 0xc6fc, 0xc6fc, 0xc6fc, 0xa555, 0x7bcf, 0x52aa, 0x0000, 0x0000, 0x0000, 0x4a69,
        0x4228, 0x0000, 0x0000, 0x0000, 0x0000, 0x4228, 0x632c, 0x6b4d, 0x0000, 0x0000, 0x0000, 0xa1f2, 0xc48e, 0xb500, 0xe01c, 0xf81f,
        0xf81f, 0xf81f, 0xc2d5, 0xff00, 0xff00, 0xc560, 0x638d, 0xc6fc, 0xc6fc, 0xc6fb, 0x6b8e, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x738e, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x630c, 0x18c3, 0x0000, 0x3160, 0xc560, 0xff00, 0xff00, 0xc2d5, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xc978, 0xe640, 0xe640, 0x6b00, 0x5a00, 0x84d3, 0xae18, 0xb67a, 0xc6fb, 0x6b8e, 0x0000, 0x0000, 0x0000, 0x0000,
        0x4228, 0x7bcf, 0x630c, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x5a00, 0x7320, 0xe640, 0xe640, 0xc978, 0xf81f, 0xf81f,
        0xd81b, 0xbc8d, 0xeea0, 0xff00, 0xc560, 0xeea0, 0xf6e0, 0xde20, 0xc560, 0xb500, 0x8bc0, 0x83c0, 0x7340, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x5aca, 0x7320, 0x83c0, 0x8bc0, 0xb520, 0xc580, 0xde20, 0xff00, 0xe660, 0xc560, 0xff00, 0xeea0, 0xbc8d, 0xd81b,
        0xc819, 0xcc32, 0xcda0, 0xf6c0, 0xb500, 0xff00, 0xff00, 0xe660, 0xee80, 0xff00, 0xff00, 0xff00, 0xff00, 0xff00, 0xff00, 0xff00,
        0xff00, 0xff00, 0xff00, 0xff00, 0xff00, 0xde40, 0xf6c0, 0xeea0, 0xf6e0, 0xff00, 0xff00, 0x9c60, 0xf6c0, 0xcda0, 0xcc32, 0xc819,
        0xf81f, 0xf81f, 0xf81f, 0xc018, 0xacc0, 0xff00, 0xff00, 0xc580, 0xb500, 0xbd20, 0xde20, 0xd5c0, 0xd5e0, 0xd5e0, 0xde20, 0xc560,
        0xde20, 0xd5e0, 0xcdc0, 0xd5e0, 0xe660, 0xbd20, 0xacc0, 0xbd20, 0xe640, 0xff00, 0xff00, 0x9420, 0xc819, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xd01a, 0xc560, 0xff00, 0xff00, 0xd5e0, 0xd5c0, 0xcda0, 0xcdc0, 0xb500, 0xacc0, 0xb500, 0xde20, 0xbd20,
        0xaca0, 0xb4e0, 0xa480, 0xacc0, 0xde20, 0xde20, 0xd5c0, 0xde20, 0xf6c0, 0xff00, 0xff00, 0xa480, 0xf01e, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xe81d, 0xa014, 0x8acc, 0xa480, 0xc560, 0xde20, 0xeea0, 0xff00, 0xff00, 0xff00, 0xff00, 0xff00, 0xff00,
        0xff00, 0xff00, 0xff00, 0xff00, 0xff00, 0xff00, 0xee80, 0xde20, 0xbd20, 0x9420, 0x8a4e, 0xb016, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf01e, 0xe01c, 0xb817, 0xb016, 0x8010, 0x4a00, 0x8c27, 0x83c0, 0x83c0, 0x83c0,
        0x83c0, 0x83c0, 0x83c0, 0x9427, 0x4a22, 0x780f, 0xb016, 0xc018, 0xe01c, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xa014, 0x9555, 0xc6fc, 0x9d35, 0x5b2e,
        0xcefb, 0xc6db, 0xffdf, 0xce79, 0x8811, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x700e, 0xae59, 0xb639, 0x5b2e,
        0xcefb, 0xdf3c, 0xef5d, 0x622c, 0xf01e, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xe81d, 0x5a8c, 0xc6fb, 0x6bb0,
        0xcefb, 0xf7be, 0x73af, 0xd81b, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xc819, 0x6c10, 0x9d55,
        0xdf3c, 0xb596, 0xb016, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x9813, 0x94f4,
        0xd6ba, 0x780f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x702e,
        0x6a8d, 0xe81d, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xe81d,
        0xd81b, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
        0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f
};