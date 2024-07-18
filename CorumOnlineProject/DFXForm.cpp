#include "stdafx.h"
#include "DFXform.h"
#include <assert.h>

#define MAGIC_NUMBER	77

// definition of Macros and static array.
#define UUE(b) ((b+MAGIC_NUMBER))
#define UUD(b) ((b-MAGIC_NUMBER))

#define XXE(b) ((set[b]))
static char set[] =
        "+-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

inline BYTE XXD(BYTE b)
{
    if(b >= 97) return BYTE(b - 59);    // a ~ z
    if(b >= 65) return BYTE(b - 53);    // A ~ Z
    if(b >= 48) return BYTE(b - 46);    // 0 ~ 9
    if(b == 45) return 1;            // -
    return 0;                        // +
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const unsigned long s_ks0[] = {
	0x0794810e, 0x1aae58d8, 0x04d084be, 0x9f222750, 0xde199f08, 0x6ceb0a22,
	0x904609ad, 0x5441c5e3, 0xe69330b6, 0x3e028eab, 0x011f33eb, 0xa6318e98,
	0xd76f665d, 0x1a726449, 0xb0e9d2dd, 0xbd8fa263, 0xaa5ab282, 0xc8f54e10,
	0x3671f727, 0x0438627b, 0x9842c4ea, 0x518b4d55, 0x6bc4f2af, 0x23487053,
	0x1b538e9b, 0xff6ea2ae, 0x692e5699, 0x021eaca2, 0xebfa40d8, 0x5b1eceef,
	0x893df34b, 0xcc56385b, 0x00e44c28, 0x2d56912e, 0x63beda6a, 0xe8cd74b4,
	0x96ff634f, 0x7e13edbf, 0xd1be5dda, 0x01a00122, 0x23777552, 0x98932337,
	0xec8a0bc1, 0xfe2cc05a, 0x61b9b376, 0x0352b46c, 0x0caeb684, 0x090ed351,
	0x4a739041, 0x880e6172, 0xccf870c7, 0x8b239a3c, 0x1691ba76, 0x32c32b9f,
	0x02758870, 0x2e88b68f, 0x3e40251c, 0x47af5286, 0xca7191a4, 0xd99a0901,
	0x7bed0077, 0x524f59ff, 0x7b7b5ac7, 0xb6f6b385, 0xc746bc0c, 0x1c5fff1c,
	0xaf5ef67f, 0x2e791651, 0x5a360ca0, 0xaddc4635, 0x3f27b5b0, 0xea40d1db,
	0xacecde38, 0x4e046edc, 0x43252880, 0xd3a8c7d6, 0x79d56619, 0x8953fae8,
	0x15e32054, 0x124f1938, 0xb79c12c9, 0x2687a96d, 0x4e30aed1, 0x10102635,
	0xa030960b, 0x2f9d7ec1, 0xc71723db, 0x76099144, 0xaebee1e6, 0xecd1b878,
	0x98e51097, 0x2c973b19, 0x99b1249e, 0xe7a0d967, 0x1b29466c, 0x5d5743a8,
	0x5ab8d2b8, 0xe8c9a1a4, 0xe8aed7fd, 0x71260d28, 0x2bbf99f9, 0xf8461384,
	0xda82122f, 0x1122370c, 0x83f36d66, 0x61566e9a, 0x08f28a28, 0x25a6a40a,
	0x1dc17d45, 0xac7635bd, 0xcc8a0e4d, 0xd8507417, 0xf1d73a19, 0xa1622702,
	0xbe18b143, 0x91fd0868, 0x382156a9, 0x491845bc, 0xb8a9c3ee, 0xfbca0272,
	0x6bcdc2f9, 0xeed4d283, 0xd38ad474, 0xfd1542ea, 0x4446ae4e, 0x18134e29,
	0x6748b739, 0xc179e9c5, 0xbb3b8c2d, 0x919479c5, 0x0faf7ce3, 0xb1d45837,
	0x09980c5b, 0x615155ab, 0xa2cd7457, 0x744431b7, 0xa78a20de, 0xd2892371,
	0x39ef30ba, 0xf42455f8, 0x4e7ff9f9, 0x6cb964eb, 0x2fa28270, 0x5a12e3f0,
	0xf8230733, 0xf89dd92f, 0x1bb2791e, 0x10ec3ed4, 0xdd63fc50, 0x80338291,
	0xd62d67ab, 0xbdcd0719, 0x756cc855, 0x4db7a2a6, 0x7b60de39, 0x4b141d74,
	0x7cac9d87, 0xe6a7b844, 0x61d8ad30, 0xe35ca3da, 0xe8cfeb6b, 0x17bf867f,
	0x2261e733, 0xee83f97f, 0xf4c264c7, 0xe7fd0dac, 0x960adb28, 0x15a7c1da,
	0x17b3f99e, 0x9f9e8d61, 0xd81e1c34, 0x4023db9e, 0x0c0eda37, 0xc91d8972,
	0x3f2a7abb, 0x9a9668c2, 0xcc807916, 0x2d3ac0f5, 0x64fd0663, 0x8bdbca79,
	0x8ef68600, 0xd4f0343e, 0x22a21110, 0xbc11a139, 0xcd9df3fa, 0x087c26e3,
	0x90672ce8, 0x1593ceb7, 0x40e3f049, 0x525c17b7, 0x08d92850, 0xc10073e8,
	0xe274ef72, 0x7a4cc7d2, 0x22c516ec, 0x2833ef01, 0xec3ea13b, 0x894a3986,
	0xb83748a2, 0xf24be676, 0xd56cf6a8, 0xc993ac6a, 0xe37d4d97, 0x0aa436fe,
	0x586d20fe, 0xc2a3a253, 0xfc24f72f, 0x97da028c, 0x6ded8fc1, 0x3e37dc54,
	0x9bf95810, 0x03ccaa57, 0x4ddaf5b9, 0x454d5bc5, 0x9a08c01e, 0xf895d0d2,
	0x715f43e8, 0x21225f38, 0x129fbf80, 0x5d9454b3, 0x92eba994, 0x5a326b84,
	0x5b4b2898, 0x5c6557ef, 0xaa2b9943, 0xba3a3ebd, 0x0fd90b0e, 0x5ee43abb,
	0xf10ac3b8, 0x4939da39, 0x0755b9c5, 0x0d309f8c, 0xe0b719fa, 0x5069808c,
	0x5c0ec3e3, 0x50a56617, 0x309ecb4d, 0x5949b18c, 0x668ffacc, 0x51e0b150,
	0xda6c4b36, 0x2d962d4d, 0xbfa56f26, 0x79bde06e, 0x180f4a7a, 0xd44cf724,
	0x3f5f73d6, 0x715b94e4, 0x62b2b81f, 0x97a94ea7, 0x01089800, 0x2216ae68,
	0x71c4c768, 0x0029b4aa, 0x5c2ead0a, 0xb48b50ef};

static const unsigned long s_ks1[] = {
	0xf4791c69, 0xef1ad500, 0xd5a312b3, 0xfa67e8c0, 0xe155e140, 0xa52a44b4,
	0x97e597e1, 0x297b63de, 0x0bcd376b, 0x93d7ef84, 0x622a3636, 0x459c4ab2,
	0x593e00a0, 0x738e07b5, 0xbeee7088, 0xa6556ea1, 0xf5455bd3, 0x3cfdfccd,
	0x75ef06eb, 0xe674e152, 0x49bfab3b, 0x290f808f, 0x8f29f9b5, 0xde06f2a7,
	0xfcc9914c, 0xb2f48402, 0x56da8979, 0xa75834c8, 0xf8c0edba, 0x91163869,
	0x527f390f, 0x99f67718, 0x6641e07c, 0xbd230d4b, 0x4cd4c88a, 0x4eaecb3d,
	0xaf29cbc5, 0x7109b56c, 0x4ed6383f, 0x9f8d831c, 0x7c95500b, 0x25f421d1,
	0xa0c3c9c3, 0xa4df2fd9, 0xb6e24f03, 0x925281c0, 0xcc18feec, 0xb733a0d9,
	0x86ade9a2, 0xb1ce47bc, 0x9b9196ce, 0x7154e7c3, 0x55d3801d, 0xbc57248c,
	0x142b94bf, 0xab50557a, 0xcc71b4e9, 0x2b190834, 0x82243753, 0x7d747b24,
	0xd4e566ba, 0xb68224f8, 0x6ef9015f, 0x414c2a25, 0x95cab887, 0x593eea60,
	0xa066b2f9, 0x8fa67423, 0x7afe0a83, 0x4a3a0b2e, 0x22684d76, 0x428fa803,
	0x2b9efd25, 0x03a37768, 0x3c3d1169, 0x715459e9, 0x90077320, 0x3ee66053,
	0x776281a9, 0x7581563b, 0xd4d78c6a, 0xf3b13775, 0x9d935c4b, 0xe9e5b29d,
	0x5ee9a938, 0x5bf0ac92, 0xb6cea3a2, 0xa48bbdf5, 0xd95b6cff, 0xf0d0b1af,
	0x0c4f9946, 0xc0c10868, 0x2c8ab573, 0x69bf7611, 0x2893be09, 0x9513635b,
	0x8214a58b, 0xc1696d3d, 0xd159d202, 0xbe50e172, 0x42d49e7b, 0x30bb46f8,
	0x129bd785, 0x1183d292, 0x16e840b7, 0x2fe2f347, 0x34990e28, 0xaaf9c8e2,
	0xe7ae6cf5, 0x774ca470, 0xbfcdf8bf, 0xe04192c5, 0xdfc1442b, 0x02a5ccf7,
	0x7df75660, 0x4d2742e0, 0x65ff288c, 0x08da19a1, 0x7611285f, 0xcab7291a,
	0x2386b98e, 0x02187f73, 0xf35bb151, 0x703fd996, 0x02b1d9e6, 0xa9cd2cb2,
	0x7e4f700c, 0x974b21b9, 0x2b1fa888, 0xf6a594e3, 0xdfad27a4, 0xd9aa142a,
	0x07ab87ab, 0x238d66cb, 0x206fd66e, 0x0e6702cb, 0x3f7318c3, 0xa8b39370,
	0x89d5c000, 0x51d07bc4, 0xbcd53a84, 0x3e824e16, 0xa45a652e, 0xf9725278,
	0xa66c11e4, 0xdea90542, 0x39bb850e, 0x9f19978d, 0x6817fb19, 0xc1156cb8,
	0x52f622f4, 0x1ed49be7, 0xaaf19b96, 0x61f17280, 0x37477d76, 0x8deeefac,
	0x5459d111, 0x76ac48dc, 0x712c1767, 0x45f46441, 0x91ebbf7f, 0xf9f26052,
	0xca62b0df, 0xe1b50c4a, 0xc983c514, 0x23b16ba8, 0x4be54b30, 0x383c36ad,
	0xa4408447, 0x6d1459e1, 0x3cf227f1, 0x64d8748f, 0x0d7ddecc, 0x918a5c44,
	0x2607c6f7, 0xbe139854, 0x2bd9f397, 0x87bfe455, 0xd3e0e955, 0xdebdb1a2,
	0x682f24de, 0x88a1a3d9, 0x4b7b9163, 0x9ee1125e, 0x6e9e1217, 0x0d5b8ad6,
	0xd815feb3, 0x17ce4bac, 0x24829ff5, 0xd05cc98f, 0x022ab31c, 0xa10f2cf2,
	0xb477eb6e, 0xc850d18a, 0x92786fb2, 0xd671c8d4, 0x885c58b7, 0x3025558d,
	0x93fb34cb, 0x8f036f38, 0x474d8644, 0x8007449c, 0x4bf043fb, 0xe510b243,
	0xdba756cc, 0x7163cffc, 0x45d51d18, 0x30266259, 0x6d06e943, 0xfde66832,
	0x49678236, 0x0a129020, 0x6649a1dd, 0x5b7dbf03, 0x61a275aa, 0x4be08e7d,
	0x6f8c1e11, 0x0656c775, 0xd6c4350a, 0x0edfeb94, 0x712c4292, 0xb6ddaecb,
	0x2f49442a, 0xaa9a79cb, 0x95c52d56, 0x65c0e88c, 0x38ef631f, 0xb8df4ac7,
	0x43364092, 0x4aea247c, 0xf7b2923f, 0x13baad6c, 0x269b1cbb, 0xe08a539f,
	0xb7cf141e, 0xd27a35e0, 0x2651a386, 0xdf0ca73d, 0x01c56892, 0x4faab288,
	0x6cf5f5e7, 0x401e91d8, 0x9d4e50af, 0x22183408, 0x60647415, 0x3cabc136,
	0x946acdca, 0x26d00e46, 0xadbac084, 0x4ce2295d, 0x30532079, 0x7221d066,
	0x3a56bae8, 0x2a2ff990, 0xfa89ca90, 0x5f964bce};
	
static const unsigned long s_ks2[] = {
	0xdb9f9bdf, 0x49ca077d, 0x07d680a4, 0x252dbb08, 0x09c7ed87, 0x12a04602,
	0x7c69bb6c, 0x847d0920, 0xcc18f778, 0x00c3db9b, 0xba135f96, 0xa39385af,
	0xdcffe9f5, 0x9cb3471c, 0x1a623c88, 0xad0b51a8, 0x31293585, 0xae2b4a9b,
	0x342363e7, 0x09c2cd41, 0x07848cec, 0x3f28e66c, 0xe2632697, 0x62d59aef,
	0xd43cde2f, 0x99e85c24, 0x3d6f14be, 0x9fa19967, 0x52be5c93, 0x44e72d99,
	0x9dd3ffc1, 0xeac3ec9e, 0x7bb8799e, 0x09e31adf, 0x9d5df944, 0xac18f3c9,
	0x8716e313, 0xf3d8244c, 0x131a512d, 0x8fbd4f5c, 0xef058ef9, 0x48048b73,
	0x1b569aa1, 0x7a0fe20e, 0x6cf2a994, 0x93e4d22c, 0x0ca0a304, 0x97abccd2,
	0xf88ba569, 0x9d34b889, 0x80c37ffc, 0x4f6e6fde, 0xcbbb353d, 0x6ef13fe2,
	0x50cc7e6d, 0x4b756aa8, 0x5db14715, 0x4f59a7c9, 0x940a317d, 0x741da0e3,
	0x6ada0f36, 0xcae87315, 0xa6086991, 0xf3043285, 0xe8e1fb26, 0xa75c62da,
	0x1f95374e, 0x31057fc2, 0x11b5bfa8, 0xf0b2776e, 0xd8bcec98, 0xd73e2a11,
	0x5f834ac3, 0xed26f165, 0xe9ca1994, 0xcd0c1426, 0x8ab6cdbb, 0x28355294,
	0xac4e8fa9, 0x3e0c5cf5, 0x8bfdbb15, 0x699e5a11, 0xb913607a, 0x921b66b5,
	0x9c44c296, 0xb95b0c31, 0xeb27dbec, 0x7257d0d9, 0x34b8d643, 0x62aba787,
	0x59d79325, 0xc61b7d17, 0x0ec62662, 0xed0baeea, 0x5daf578a, 0xb9058d64,
	0x221c2376, 0x2237e06f, 0x8f7d3ac0, 0xb2f261f5, 0xa9a67f46, 0x0a2d3e69,
	0xc94e8cab, 0x5cff9b3f, 0x1c922bd6, 0xef290b70, 0x246fdd71, 0x9e8a1bf7,
	0x354b576b, 0x59a8c756, 0xf86d0076, 0xa22e0212, 0xec80758a, 0x12693232,
	0xe0140560, 0xd2c9b1c4, 0x7a1d35f8, 0x22664e58, 0xb3723d14, 0xd579ca84,
	0x584f89bb, 0xd3de5c5e, 0x8ad439b7, 0x95992e05, 0x3f84a216, 0xad4edd18,
	0xc0c7cbad, 0x3bc7fc3d, 0x2968f090, 0x7972929c, 0xec17029b, 0x31e19761,
	0x4ee825ed, 0x41487b3e, 0xe7d23068, 0x1e02a0e6, 0x26313132, 0x4e0cdb93,
	0xcc45e736, 0xec89f681, 0x6db146a4, 0x273c3171, 0xf2fdf66d, 0xc411bc27,
	0x1914d4c8, 0x98943fec, 0xf3c670b0, 0x0f78520c, 0x65478d64, 0xa8120659,
	0xa7afa4e5, 0x76dc5aa7, 0xc878627b, 0xa0f3c64e, 0x29032531, 0xe198b4ab,
	0xfd138354, 0x0ab2029f, 0xd04fc3f8, 0x7b4b810f, 0xfdc56273, 0xae0f7962,
	0x35628fe0, 0xacd02306, 0xff7baf9d, 0x95052deb, 0x3347dacd, 0x1d463a06,
	0x7e625cd7, 0x09d15fd0, 0xe14c35e5, 0xb608a7c4, 0x31e89966, 0x93f190e4,
	0x99fe718d, 0xa2b58c35, 0x13b9d8cf, 0xfb21803f, 0xf32a9f69, 0x4928488e,
	0x5dc3cad8, 0x4a5f3233, 0xc9dc115f, 0x53807f43, 0x86315f85, 0xcae8e557,
	0x34625694, 0xab190f0b, 0x4972c91b, 0x03381b80, 0x8d49406c, 0x788f1bd9,
	0x9e32781e, 0xbf0c95c1, 0x6c5ee18d, 0x24c103e4, 0xbe5f1e56, 0x06625472,
	0xabad89db, 0x59ca699f, 0x2224acc5, 0x22779825, 0x6384c87d, 0xfe092dc0,
	0x83f052b1, 0x9bc7e4b1, 0xec7071d5, 0x3d1a703c, 0xdb7082a0, 0x3b0ff72a,
	0xe03e948b, 0x7fd99348, 0x628fec55, 0x0a4ed3e6, 0x18fa8282, 0x6e653859,
	0x917c82d7, 0x51beb979, 0xadf3cce0, 0xf11a3f24, 0x21a3746a, 0x9ade2ab9,
	0xf7b44309, 0x3295ca9d, 0x0bb13493, 0xae6be6ba, 0x900af8a3, 0x9ab239fb,
	0x89947317, 0x9561f0d1, 0x50043c93, 0xd2902db1, 0x137721fc, 0x98fe8794,
	0x52eda1fd, 0xc38b8775, 0x62c97085, 0x41be2dd7, 0xed53f647, 0x9540693c,
	0x6f37d039, 0x585ea3af, 0xbb034e16, 0xb38c333b, 0x74acf3dc, 0xe7dde972,
	0x940af84e, 0xc58b87e7, 0xe857c973, 0x367542b4, 0x92b48715, 0xb49c45f5,
	0x86a48443, 0xcea62d4b, 0x0e8fcacf, 0xa85b8e5a};

static const unsigned long s_ks3[] = {
	0x595f3e8e, 0xbd782a84, 0x1421b78c, 0xf4411ed9, 0x669c4eb6, 0x4701311b,
	0xefc5317e, 0x1171fbeb, 0xe2b88257, 0x7eac4732, 0x8731d93b, 0x0fb19798,
	0xb811bd26, 0x19e49f0e, 0x64df2f99, 0x468dc545, 0x0f442018, 0x1358a834,
	0x508ef5ac, 0x51d49596, 0x522f0a61, 0xa4f41369, 0x533b2dc9, 0x07915870,
	0x28ed1e78, 0x45e6d2b3, 0xb7221686, 0x81129ffa, 0x7bdc3d10, 0xb0991555,
	0x04c131b7, 0x1ae33c97, 0x749a861f, 0xdcbc4fd6, 0x03d44171, 0x69d33eed,
	0x7c025cda, 0x043b2efa, 0xbe59460a, 0x47edf7e0, 0x3c3360b6, 0xa044a5c6,
	0x7e8c8017, 0xcf7ff996, 0x9d887168, 0x6942e57f, 0xcbeb716b, 0x58161373,
	0xc89fb2e5, 0x59e45faa, 0x6e32da1d, 0x7c7e581e, 0x26568361, 0xa717c30c,
	0x715fbc82, 0x13c51879, 0x60c88653, 0xce0602ab, 0x1caf582e, 0x3838e3ac,
	0x5f549b6e, 0x86204fcb, 0xf99d2ef6, 0x42648c1a, 0x4c95e3a9, 0xc81118f1,
	0xd0e900f0, 0xca162369, 0x906ac037, 0xcec611e2, 0xab8ecf70, 0xab5af97d,
	0xd3eed28e, 0x0f6d29a3, 0xd2c9dc0b, 0xfc7e9f7c, 0x0080fa63, 0x46719179,
	0xce18a897, 0x170fe6cb, 0x3fbb59ab, 0x6b81bbe9, 0x6937f428, 0x94d9df0e,
	0xf87e519b, 0xb78a58b9, 0xac24c014, 0x4debdc2b, 0xd6e483a8, 0xa4b758ec,
	0xde1c4eef, 0x5b906c47, 0xc04cce86, 0xe977edc9, 0x8b9b208e, 0x175662c6,
	0xe152552c, 0x817687d4, 0x795ef407, 0x1909cd4e, 0xa0d278cd, 0xa4a2d8d2,
	0xb463ceae, 0x3bb800c4, 0xa7ebd9df, 0xcb27d1c9, 0x82e7ee19, 0x96ae8a4c,
	0xe0f95817, 0xb071a2fc, 0x6f66d41c, 0x82793e4c, 0x7298166a, 0x4a30bcf2,
	0x419d42cc, 0x99e62c68, 0xc7a7740d, 0xd44ed26e, 0x038a3a7f, 0xb401a586,
	0x88421475, 0xc6fad277, 0xfe6afac9, 0xec183acb, 0x9ec5d657, 0xd9a0f051,
	0xbab9067f, 0x99b8bc9b, 0x32cfdca7, 0x07ed9b81, 0x00351eb7, 0x55ae3c9c,
	0xad3c869c, 0x88cf84cc, 0xdada42c5, 0xf8070bb3, 0xb82975a9, 0xd4719e38,
	0x90e6b43f, 0x9d0fbb04, 0x3cf28a84, 0xa7431708, 0xacd5f5f9, 0x9b541bf5,
	0x6236e722, 0xf7ef63c3, 0xf664c609, 0x8ca33e2b, 0x93d1ecb9, 0xfe63302d,
	0x789228c3, 0x4808738a, 0x78e03bbb, 0x36cd724a, 0x7a995bbc, 0xe9d24d38,
	0xfac0b4e0, 0x57995862, 0x86f8e2c9, 0xc88c9c9a, 0x410d781c, 0x5a7554f7,
	0x646c7c00, 0x7e047054, 0xb8a6e8a4, 0x784d16cf, 0x1df12bb6, 0xe3471c4c,
	0x06a7a6ec, 0x2b4d90f0, 0xfbc5307f, 0x11a130a6, 0x166d92aa, 0x2ae5f29d,
	0x86650b30, 0x60a07dc8, 0x0f95ced4, 0x70bfae5d, 0x888c7fde, 0x6b111457,
	0x5cfcb99d, 0x35ca74f4, 0x0a3a8bdf, 0x09014637, 0xa4bff67b, 0xf3323667,
	0x53a971c8, 0x54bca58e, 0xd23b6623, 0x616425fb, 0xef59b26d, 0xa4d100c1,
	0x0d0d298a, 0x7a0eb136, 0xa4050fe3, 0x930b6974, 0xbf12a0e4, 0x761b8cdb,
	0x7daa897f, 0xfa77328d, 0x91676ca8, 0xcebaa7f1, 0xc38660d7, 0xf264ea31,
	0x6d696f8a, 0x3b5832bb, 0xfe1516c0, 0xd35d64c4, 0x7ab5c97e, 0xb7a09ec0,
	0xf7156c4d, 0x3730b0ec, 0x2228dbba, 0x79819dc4, 0xb88363d5, 0xf7ea1f8d,
	0x0edf47b3, 0xfc2720cc, 0x8a9b3ced, 0x2bd841cd, 0x2737ed1e, 0xf800571e,
	0xf4da7869, 0x2d86e911, 0x96cdeef0, 0x67b7b43d, 0xc2fed75e, 0x96c227fd,
	0xc47eac5e, 0x803be8f0, 0xfa045b22, 0x3e994d77, 0x5968c7dd, 0xbeb8e33b,
	0xe8284549, 0x4058eaa4, 0x40e62122, 0xd99cd662, 0x12e716a8, 0xf38bd0e8,
	0xa196d921, 0xca9233ed, 0x43ff8f55, 0xf00110e9, 0xe5555211, 0xca89ab9e,
	0x846db1a4, 0x12c5fc8c, 0xb4402d65, 0x53ae2e7b, 0x1d6dbc2b, 0x702423f5,
	0xf9b449d3, 0x1d6ceec9, 0x977d32be, 0x63ad588a};

static const BYTE s_base_key[] =
{
	 72, 110,  19, 111,   8,  99,  13, 140,
	 47,  90, 188, 194,  55,  61, 255, 10,
	 12,   1, 132,  33, 127, 200,  29, 48,
	199, 240,  44,  91,  88, 109,  56, 245
};

void DFOutputDebugString(LPCTSTR pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);

	TCHAR buffer[40960] = {0,};
	wvsprintf(buffer, pFormat, args);

	va_end(args);

	OutputDebugString(buffer);
}

DFXform::DFXform()
{
	m_nKeySize = KEYBYTES;

	InitializeBlowfishWithBaseKey();	
}

DFXform::~DFXform()
{
}

//////////////////////////////////////////////////////////////////////
/////////////
void DFXform::UUEncode(BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize)
{
    register UINT i = 0;
    
    // 패딩이 필요한 사이즈를 구한다.
    UINT nPaddingSize = (nSrcSize % 3) ? 3 - (nSrcSize % 3) : 0;

    // 몇 회의 부분 인코딩이 필요한 지 구한다.
    UINT nCount = (UINT)nSrcSize / 3;

    // 인코딩한다.
    for(i = 0; i <  nCount; i++)
    {
        UUPartialEncode(pSrc + 3*i, pDes + 4*i);
    }

    // 뒷 부분을 패딩하고 인코딩한다(0/1을 이용한다).
    UUPadAndEncode(pSrc + 3*i, pDes + 4*i, nPaddingSize);
    i++;

    // 변환된 전체 사이즈를 구한다.
    *pDesSize = 4*i;
}

void DFXform::UUPadAndEncode(BYTE* pSrc, BYTE* pDes, UINT nPaddingSize)
{
    BYTE Zero   = 0;
    BYTE One    = 1;
    register BYTE c1 = 0, c2 = 0, c3 = 0, c4 = 0;

    if(nPaddingSize == 0)    
    {
        c1 = (BYTE)(One >> 2);
        c2 = (BYTE)((One << 4) & 0x30 | (Zero >> 4) & 0x0F);
        c3 = (BYTE)((Zero << 2) & 0x3C | (Zero >> 6) & 0x03);
        c4 = (BYTE)(Zero & 0x3F);
    }
    else if(nPaddingSize == 1)
    {
        c1 = (BYTE)(pSrc[0] >> 2);
        c2 = (BYTE)((pSrc[0] << 4) & 0x30 | (pSrc[1] >> 4) & 0x0F);
        c3 = (BYTE)((pSrc[1] << 2) & 0x3C | (One >> 6) & 0x03);
        c4 = (BYTE)(One & 0x3F);
    }
    else if(nPaddingSize == 2)
    {
        c1 = (BYTE)(pSrc[0] >> 2);
        c2 = (BYTE)((pSrc[0] << 4) & 0x30 | (One >> 4) & 0x0F);
        c3 = (BYTE)((One << 2) & 0x3C | (Zero >> 6) & 0x03);
        c4 = (BYTE)(Zero & 0x3F);
    }

    pDes[0] = (BYTE)UUE(c1);
    pDes[1] = (BYTE)UUE(c2);
    pDes[2] = (BYTE)UUE(c3);
    pDes[3] = (BYTE)UUE(c4);
}

void DFXform::UUPartialEncode(BYTE* pSrc, BYTE* pDes)
{
    register BYTE c1, c2, c3, c4;

    c1 = (BYTE)(pSrc[0] >> 2);
    c2 = (BYTE)((pSrc[0] << 4) & 0x30 | (pSrc[1] >> 4) & 0x0F);
    c3 = (BYTE)((pSrc[1] << 2) & 0x3C | (pSrc[2] >> 6) & 0x03);
    c4 = (BYTE)(pSrc[2] & 0x3F);

    pDes[0] = (BYTE)UUE(c1);
    pDes[1] = (BYTE)UUE(c2);
    pDes[2] = (BYTE)UUE(c3);
    pDes[3] = (BYTE)UUE(c4);
}

void DFXform::UUDecode(BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize)
{
    register UINT    i = 0;
    register UINT    nCount = 0;

    // 디코딩할 횟수를 구한다.
    nCount = (UINT)nSrcSize / 4;    

    // 디코딩한다.
    for(i = 0; i < nCount; i++)
    {
        UUPartialDecode(pSrc + 4*i, pDes + 3*i);
    }

    // 디코딩된 사이즈를 구한다.
    *pDesSize = 3*i;

    // 패딩을 제거한다.
    if(pDes[*pDesSize - 1] == 0)
        *pDesSize -= 1;
    if(pDes[*pDesSize - 1] == 0)
        *pDesSize -= 1;
    if(pDes[*pDesSize - 1] == 1)
        *pDesSize -= 1;
}

void DFXform::UUPartialDecode(BYTE* pSrc, BYTE* pDes)
{
    register BYTE c1, c2, c3;

    c1 = (BYTE)((UUD(pSrc[0]) << 2) & 0xFC | (UUD(pSrc[1]) >> 4) & 0x03);
    c2 = (BYTE)((UUD(pSrc[1]) << 4) | (UUD(pSrc[2]) >> 2) & 0x0F);
    c3 = (BYTE)((UUD(pSrc[2]) << 6) | UUD(pSrc[3]) & 0x3F);

    pDes[0] = (BYTE)c1;
    pDes[1] = (BYTE)c2;
    pDes[2] = (BYTE)c3;
}

//////////////////////////////////////////////////////////////////////
/////////////
void DFXform::XXEncode(BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize)
{
    register UINT    i = 0;
    
    // 패딩이 필요한 사이즈를 구한다.
    UINT nPaddingSize = (nSrcSize % 3) ? 3 - (nSrcSize % 3) : 0;

    // 몇 회의 부분 인코딩이 필요한 지 구한다.
    UINT nCount = (UINT)nSrcSize / 3;

    // 인코딩한다.
    for(i = 0; i < nCount; i++)
    {
        XXPartialEncode(pSrc + 3*i, pDes + 4*i);
    }

    // 뒷 부분을 패딩하고 인코딩한다(0/1을 이용한다).
    XXPaddAndEncode(pSrc + 3*i, pDes + 4*i, nPaddingSize);
    i++;

    // 변환된 전체 사이즈를 구한다.
    *pDesSize = 4*i;
}

void DFXform::XXPaddAndEncode(BYTE* pSrc, BYTE* pDes, UINT nPaddingSize)
{
    BYTE Zero    = 0;
    BYTE One    = 1;
    register BYTE c1 =0, c2=0, c3=0, c4=0;

    if(nPaddingSize == 0)    
    {
        c1 = (BYTE)(One >> 2);
        c2 = (BYTE)((One <<  4) & 0x30 | (Zero >> 4) & 0x0F);
        c3 = (BYTE)((Zero <<  2) & 0x3C | (Zero >> 6) & 0x03);
        c4 = (BYTE)(Zero & 0x3F);
    }
    else if(nPaddingSize == 1)
    {
        c1 = (BYTE)(pSrc[0] >> 2);
        c2 = (BYTE)((pSrc[0] <<  4) & 0x30 | (pSrc[1] >> 4) & 0x0F);
        c3 = (BYTE)((pSrc[1] <<  2) & 0x3C | (One >> 6) & 0x03);
        c4 = (BYTE)(One & 0x3F);
    }
    else if(nPaddingSize == 2)
    {
        c1 = (BYTE)(pSrc[0] >> 2);
        c2 = (BYTE)((pSrc[0] << 4) & 0x30 | (One >> 4) & 0x0F);
        c3 = (BYTE)((One << 2) & 0x3C | (Zero >> 6) & 0x03);
        c4 = (BYTE)(Zero & 0x3F);
    }

    pDes[0] = (BYTE)XXE(c1);
    pDes[1] = (BYTE)XXE(c2);
    pDes[2] = (BYTE)XXE(c3);
    pDes[3] = (BYTE)XXE(c4);
}

void DFXform::XXPartialEncode(BYTE* pSrc, BYTE* pDes)
{
    register BYTE c1, c2, c3, c4;

    c1 = (BYTE)(pSrc[0] >> 2);
    c2 = (BYTE)((pSrc[0] <<  4) & 0x30 | (pSrc[1] >> 4) & 0x0F);
    c3 = (BYTE)((pSrc[1] <<  2) & 0x3C | (pSrc[2] >> 6) & 0x03);
    c4 = (BYTE)(pSrc[2] & 0x3F);

    pDes[0] = (BYTE)XXE(c1);
    pDes[1] = (BYTE)XXE(c2);
    pDes[2] = (BYTE)XXE(c3);
    pDes[3] = (BYTE)XXE(c4);
}

void DFXform::XXDecode(BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize)
{
    register UINT    i = 0;
    register UINT    nCount = 0;

    // 디코딩할 횟수를 구한다.
    nCount = (UINT)nSrcSize / 4;    

    // 디코딩한다.
    for(i = 0; i < nCount; i++)
    {
        XXPartialDecode(pSrc + 4*i, pDes + 3*i);
    }

    // 디코딩된 사이즈를 구한다.
    *pDesSize = 3*i;

    // 패딩을 제거한다(원 데이터의 마지막 바이트가 0이라면 제거될 수도 있다).
    if(pDes[*pDesSize - 1] == 0)
        *pDesSize -= 1;
    if(pDes[*pDesSize - 1] == 0)
        *pDesSize -= 1;
    if(pDes[*pDesSize - 1] == 1)
        *pDesSize -= 1;
}

void DFXform::XXPartialDecode(BYTE* pSrc, BYTE* pDes)
{
    register BYTE c1, c2, c3;

    c1 = BYTE((XXD(pSrc[0]) <<  2) & 0xFC | (XXD(pSrc[1]) >> 4) & 0x03);
    c2 = BYTE((XXD(pSrc[1]) <<  4) | (XXD(pSrc[2]) >> 2) & 0x0F);
    c3 = BYTE((XXD(pSrc[2]) <<  6) | (XXD(pSrc[3])) & 0x3F);

    pDes[0] = c1;
    pDes[1] = c2;
    pDes[2] = c3;
}


BOOL DFXform::CompareKey(LPBYTE Key)
{
	return (0 == memcmp(Key, m_Key, m_nKeySize));
}

void DFXform::KeyEncrypt(LPBYTE Data)
{
	for(int i = 0; i < m_nKeySize; i++)	Data[i] ^= s_base_key[i%8];
}

void DFXform::KeyDecrypt(LPBYTE Data)
{
	for(int i = 0; i < m_nKeySize; i++)	Data[i] ^= s_base_key[i%8];
}

void DFXform::InitializeBlowfishWithBaseKey()
{
	InitializeBlowfish((LPBYTE)&s_base_key[0], KEYBYTES);
}

//////////////////////////////////////////////////////////////////////////////
// BLOWFISH Encode/Decode Methods

void DFXform::Encode(LPBYTE pSrc, DWORD dwSrcSize, LPBYTE pDes, LPDWORD pDesSize)
{
	assert(pSrc != NULL);
	assert(pDes != NULL);
	assert(dwSrcSize != 0);

	// blowfish 용 패딩 크기를 구한다. 
	BYTE btPaddingSize = (BYTE)((8 - (dwSrcSize % 8)) % 8);
	
	// des에 복사하고 이를 암호화한다.
	memcpy(pDes+1, pSrc, dwSrcSize);

	// 우리는 (BYTE)-1로 메시지의 뒤를 패딩한다!
	if(btPaddingSize) memset(pDes + dwSrcSize + 1, (BYTE)-1, btPaddingSize); 

	// blowfish encoding!!
	*pDesSize = dwSrcSize + btPaddingSize + 1;
	BlowfishEncode(pDes + 1, *pDesSize - 1);
	pDes[0] = btPaddingSize;
}

void DFXform::Encode(LPBYTE pSrc, WORD wdSrcSize, LPBYTE pDes, LPWORD pDesSize)
{
	assert(pSrc != NULL);
	assert(pDes != NULL);
	assert(wdSrcSize != 0);

	// blowfish 용 패딩 크기를 구한다. 
	BYTE btPaddingSize = (BYTE)((8 - (wdSrcSize % 8)) % 8);
	
	// des에 복사하고 이를 암호화한다.
	memcpy(pDes+1, pSrc, wdSrcSize);

	// 우리는 (BYTE)-1로 메시지의 뒤를 패딩한다!
	if(btPaddingSize) memset(pDes + wdSrcSize + 1, (BYTE)-1, btPaddingSize); 

	// blowfish encoding!!
	*pDesSize = WORD(wdSrcSize + btPaddingSize + 1);
	BlowfishEncode(pDes + 1, *pDesSize - 1);
	pDes[0] = btPaddingSize;
}

void DFXform::Decode(LPBYTE pSrc, DWORD dwSrcSize, LPBYTE pDes, LPDWORD pDesSize)
{
	assert(pSrc != NULL);
	assert(pDes != NULL);
	assert(dwSrcSize != 0);
	assert(pSrc[0] < 8);
	assert(((dwSrcSize - 1) % 8) == 0);

#ifdef _DEBUG
	// 잘못된 메시지를 걸러낸다.
	if(pSrc[0] >= 8)		{*pDesSize = 0;return;}	// pSrc[0] == btPaddingSize
	if((dwSrcSize - 1) % 8)	{*pDesSize = 0;return;}
#endif

	BlowfishDecode(pSrc+1, dwSrcSize-1);

	*pDesSize = dwSrcSize - (1 + pSrc[0]);
	memcpy(pDes, pSrc + 1, *pDesSize); 
}

void DFXform::Decode(LPBYTE pSrc, WORD wdSrcSize, LPBYTE pDes, LPWORD pDesSize)
{
	assert(pSrc != NULL);
	assert(pDes != NULL);
	assert(wdSrcSize != 0);
	assert(pSrc[0] < 8);
	assert(((wdSrcSize - 1) % 8) == 0);

#ifdef _DEBUG
	// 잘못된 메시지를 걸러낸다.
	if(pSrc[0] >= 8)		{*pDesSize = 0;return;}	// pSrc[0] == btPaddingSize
	if((wdSrcSize - 1) % 8)	{*pDesSize = 0;return;}
#endif

	BlowfishDecode(pSrc+1, wdSrcSize-1);

	*pDesSize = WORD(wdSrcSize - (1 + pSrc[0]));
	memcpy(pDes, pSrc + 1, *pDesSize); 
}

void DFXform::InitializeBlowfish(LPBYTE k, DWORD len)
{
	assert(len <= MAXKEYBYTES);

	m_nKeySize = len;
	memcpy(m_Key, k, len);

	blf_key();	
}

// nSrcSize 는 8의 배수여야 한다. 이 함수를 호출하기 전에 8바이트로 패딩한다. 
__inline void DFXform::BlowfishEncode(LPVOID pSrc, DWORD dwSrcSize)
{
	assert(dwSrcSize % 8 == 0);
	blf_enc((unsigned long*) pSrc, dwSrcSize / 8);
}

__inline void DFXform::BlowfishDecode(LPVOID pSrc, DWORD dwSrcSize)
{
	assert(dwSrcSize % 8 == 0);
	blf_dec((unsigned long*) pSrc, dwSrcSize / 8);
}

unsigned long DFXform::F(unsigned long x)
{
	blf_ctx& bc = m_blf_ctx;

	unsigned short d = (unsigned short) (x & 0x00ff); x >>= 8;
	unsigned short c = (unsigned short) (x & 0x00ff); x >>= 8;
	unsigned short b = (unsigned short) (x & 0x00ff); x >>= 8;
	unsigned short a = (unsigned short) (x & 0x00ff);

	return ((bc.S[0][a] + bc.S[1][b]) ^ bc.S[2][c]) + bc.S[3][d];
}

void DFXform::blowfish_encipher(unsigned long* xl, unsigned long* xr)
{
	register unsigned long Xl = *xl;
	register unsigned long Xr = *xr;
	register int i;
	blf_ctx& bc = m_blf_ctx;

	for(i = 0; i < N; i++)
	{
		Xl = Xl ^ bc.P[i];
		Xr = F(Xl) ^ Xr;

		SWAP(Xl, Xr);
	}

	SWAP(Xl, Xr);

	Xr = Xr ^ bc.P[N];
	Xl = Xl ^ bc.P[N + 1];

	*xl = Xl;
	*xr = Xr;
}

void DFXform::blowfish_decipher(unsigned long* xl, unsigned long* xr)
{
	register unsigned long Xl = *xl;
	register unsigned long Xr = *xr;
	register int i;
	blf_ctx& bc = m_blf_ctx;

	for(i = N + 1; i > 1; --i)
	{
		Xl = Xl ^ bc.P[i];
		Xr = F(Xl) ^ Xr;

		SWAP(Xl, Xr);
	}

	SWAP(Xl, Xr);

	Xr = Xr ^ bc.P[1];
	Xl = Xl ^ bc.P[0];

	*xl = Xl;
	*xr = Xr;
}

void DFXform::blf_key()
{ 
	short i, j, k;
	unsigned long data, datal, datar;
	blf_ctx& bc = m_blf_ctx;

	memset(&bc, 0, sizeof(blf_ctx));

	for(i = 0; i < 256; i++)
	{
		bc.S[0][i] = s_ks0[i];
		bc.S[1][i] = s_ks1[i];
		bc.S[2][i] = s_ks2[i];
		bc.S[3][i] = s_ks3[i];
	}

	j = 0;
	for(i = 0; i < N + 2; ++i)
	{
		data = 0x00000000;
		for(k = 0; k < 4; ++k)
		{
			data = (data << 8) | m_Key[j];
			if(++j >= m_nKeySize) j = 0;
		}

		bc.P[i] = bc.P[i] ^ data;
	}

	datal = 0x00000000;
	datar = 0x00000000;

	for(i = 0; i < N + 2; i += 2)
	{
		blowfish_encipher(&datal, &datar);
		bc.P[i] = datal;
		bc.P[i + 1] = datar;
	}

	for(i = 0; i < 4; ++i)
	{
		for(j = 0; j < 256; j += 2)
		{
			blowfish_encipher(&datal, &datar);
			bc.S[i][j] = datal;
			bc.S[i][j+1] = datar;
		}
	}	
}

__inline void DFXform::blf_enc(unsigned long* data, unsigned long blocks)
{
	unsigned long* d = data;
	register unsigned long i;

	for(i = 0; i < blocks; i++)
	{
		blowfish_encipher(d, d+1);
		d += 2;
	}
}

__inline void DFXform::blf_dec(unsigned long* data, unsigned long blocks)
{
	unsigned long* d = data;
	register unsigned long i;

	for(i = 0; i < blocks; i++)
	{
		blowfish_decipher(d, d+1);
		d += 2;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DFSimpleXForm::DFSimpleXForm()
{
	SetBaseKey();
}

DFSimpleXForm::DFSimpleXForm(char Key[], int iKeyLen, int iSubKey)
{
	SetKey(Key, iKeyLen, iSubKey);
}

DFSimpleXForm::~DFSimpleXForm()
{

}

void DFSimpleXForm::SetBaseKey()
{
	SetKey(SIMPLE_BASE_KEY, strlen(SIMPLE_BASE_KEY), 3);
}

void DFSimpleXForm::SetKey(char Key[], int iKeyLen, int iSubKey)
{
	memcpy(m_Key, Key, iKeyLen);
	m_iKeyLen = iKeyLen;
	m_iSubKey = iSubKey;
}

void DFSimpleXForm::Convert(LPBYTE pSrc, int iSrcSize)
{
	int nRemainBytes = 0;
	int nProcessed = 0;

	while(nProcessed < iSrcSize)
	{
		LPBYTE szData = LPBYTE(pSrc + nProcessed);
		nRemainBytes = iSrcSize - nProcessed;

		int nLoop = (nRemainBytes < m_iKeyLen) ? nRemainBytes : m_iKeyLen;
		for(int i = 0; i < nLoop; i++)
			szData[i] ^= (m_Key[i] + m_iSubKey);

		nProcessed += nLoop;
	}
}
