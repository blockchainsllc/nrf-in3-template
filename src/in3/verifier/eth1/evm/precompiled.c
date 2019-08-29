#include "../../../core/util/mem.h"
#include "../../../core/util/utils.h"
#include "../../../third-party/crypto/bignum.h"
#include "../../../third-party/crypto/ecdsa.h"
#include "../../../third-party/crypto/ripemd160.h"
#include "../../../third-party/crypto/secp256k1.h"
#include "../../../third-party/tommath/tommath.h"
#include "evm.h"
#include "gas.h"

#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif
#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

uint8_t evm_is_precompiled(evm_t* evm, address_t address) {
  UNUSED_VAR(evm);
  int l = 20;
  optimize_len(address, l);
  return (l == 1 && *address && *address < 9);
}

int pre_ecrecover(evm_t* evm) {
  subgas(G_PRE_EC_RECOVER);
  if (evm->call_data.len < 128) return 0;

  uint8_t pubkey[65], *vdata = evm->call_data.data + 32, vl = 32;
  optimize_len(vdata, vl);
  if (vl > 1) return 0;

  // verify signature
  if (ecdsa_recover_pub_from_sig(&secp256k1, pubkey, evm->call_data.data + 64, evm->call_data.data, *vdata >= 27 ? *vdata - 27 : *vdata) == 0) {
    evm->return_data.data = _malloc(20);
    evm->return_data.len  = 20;

    uint8_t hash[32];

    // hash it and return the last 20 bytes as address
    bytes_t public_key = {.data = pubkey + 1, .len = 64};
    if (sha3_to(&public_key, hash) == 0)
      memcpy(evm->return_data.data, hash + 12, 20);
  }
  return 0;
}

int pre_sha256(evm_t* evm) {
  subgas(G_PRE_SHA256 + (evm->call_data.len + 31) / 32 * G_PRE_SHA256_WORD);
  evm->return_data.data = _malloc(32);
  evm->return_data.len  = 32;
  sha3_to(&evm->call_data, evm->return_data.data);
  return 0;
}
int pre_ripemd160(evm_t* evm) {
  subgas(G_PRE_RIPEMD160 + (evm->call_data.len + 31) / 32 * G_PRE_RIPEMD160_WORD);
  evm->return_data.data = _malloc(20);
  evm->return_data.len  = 20;
  ripemd160(evm->call_data.data, evm->call_data.len, evm->return_data.data);
  return 0;
}
int pre_identity(evm_t* evm) {
  subgas(G_PRE_IDENTITY + (evm->call_data.len + 31) / 32 * G_PRE_IDENTITY_WORD);
  evm->return_data.data = _malloc(evm->call_data.len);
  evm->return_data.len  = evm->call_data.len;
  memcpy(evm->return_data.data, evm->call_data.data, evm->call_data.len);
  return 0;
}

int pre_modexp(evm_t* evm) {
  if (evm->call_data.len < 96) return -1;
  uint8_t      res[64];
  uint_fast8_t hp     = 0;
  uint32_t     l_base = bytes_to_int(evm->call_data.data + 28, 4);
  uint32_t     l_exp  = bytes_to_int(evm->call_data.data + 28 + 32, 4);
  uint32_t     l_mod  = bytes_to_int(evm->call_data.data + 28 + 64, 4);
  if (evm->call_data.len < 96 + l_base + l_exp + l_mod) return -1;
  bytes_t b_base = bytes(evm->call_data.data + 96, l_base);
  bytes_t b_exp  = bytes(evm->call_data.data + 96 + l_base, l_exp);
  bytes_t b_mod  = bytes(evm->call_data.data + 96 + l_base + l_exp, l_mod);

#ifdef EVM_GAS

  for (uint32_t i = 0; i < MIN(l_exp, 32); i++) {
    if (b_exp.data[i]) {
      for (int n = 7; n >= 0; n--) {
        if (b_exp.data[i] >> n) {
          hp = ((l_exp - i - 1) << 3) + n;
          break;
        }
      }
      break;
    }
  }
  uint64_t ael;
  if (l_exp <= 32 && hp == 0)
    ael = 0;
  else if (l_exp <= 32)
    ael = hp;
  else
    ael = 8 * (l_exp - 32) + hp;

  // calc gas
  //  floor(mult_complexity(max(length_of_MODULUS, length_of_BASE)) * max(ADJUSTED_EXPONENT_LENGTH, 1) / GQUADDIVISOR)
  uint64_t lm = MAX(l_mod, l_base);
  if (lm <= 64)
    lm *= lm;
  else if (lm <= 1024)
    lm = lm * lm / 4 + 96 * lm - 3072;
  else
    lm = lm * lm / 16 + 480 * lm - 199680;

  subgas(lm * MAX(1, ael) / G_PRE_MODEXP_GQUAD_DIVISOR);

#endif
  // we use gmp for now
  mp_int m_base, m_exp, m_mod, m_res;
  mp_init(&m_base);
  mp_init(&m_exp);
  mp_init(&m_mod);
  mp_init(&m_res);

  mp_import(&m_base, b_base.len, 1, sizeof(uint8_t), 1, 0, b_base.data);
  mp_import(&m_exp, b_exp.len, 1, sizeof(uint8_t), 1, 0, b_exp.data);
  mp_import(&m_mod, b_mod.len, 1, sizeof(uint8_t), 1, 0, b_mod.data);

  m_base.sign = m_exp.sign = m_mod.sign = 0;
  mp_exptmod(&m_base, &m_exp, &m_mod, &m_res);
  size_t ml;
  mp_export(res, &ml, 1, sizeof(uint8_t), 1, 0, &m_res);

  mp_clear(&m_base);
  mp_clear(&m_exp);
  mp_clear(&m_mod);
  mp_clear(&m_res);

  evm->return_data.data = _malloc(ml);
  evm->return_data.len  = ml;
  memcpy(evm->return_data.data, res, ml);
  return 0;
}
const ecdsa_curve alt_bn128 = {
    /* .prime */ {
        /*.val =*/{0x16d87cfd, 0x34f08230, 0x16871ca8, 0x25e05aa4, 0x181585d, 0x214116da, 0x131a029b, 0x19139cb8, 0x30}},

    /* G */ {/*.x =*/{/*.val =*/{0x16f81798, 0x27ca056c, 0x1ce28d95, 0x26ff36cb, 0x70b0702, 0x18a573a, 0xbbac55a, 0x199fbe77, 0x79be}},
             /*.y =*/{/*.val =*/{0x3b10d4b8, 0x311f423f, 0x28554199, 0x5ed1229, 0x1108a8fd, 0x13eff038, 0x3c4655da, 0x369dc9a8, 0x483a}}},

    /* order */ {/*.val =*/{0x10364141, 0x3f497a33, 0x348a03bb, 0x2bb739ab, 0x3ffffeba, 0x3fffffff, 0x3fffffff, 0x3fffffff, 0xffff}},

    /* order_half */ {/*.val =*/{0x281b20a0, 0x3fa4bd19, 0x3a4501dd, 0x15db9cd5, 0x3fffff5d, 0x3fffffff, 0x3fffffff, 0x3fffffff, 0x7fff}},

    /* a */ 0,

    /* b */ {/*.val =*/{3}}

};

int pre_ec_add(evm_t* evm) {
  subgas(500);
  uint8_t cdata[128];
  memset(cdata, 0, 128);
  memcpy(cdata, evm->call_data.data, MIN(128, evm->call_data.len));

  curve_point a, b;
  bn_read_be(cdata, &a.x);
  bn_read_be(cdata + 32, &a.y);
  bn_read_be(cdata + 64, &b.x);
  bn_read_be(cdata + 96, &b.y);

  point_add(&alt_bn128, &a, &b);

  evm->return_data = bytes(_malloc(64), 64);
  bn_write_be(&b.x, evm->return_data.data);
  bn_write_be(&b.y, evm->return_data.data + 32);
  return 0;
}

int pre_ec_mul(evm_t* evm) {
  subgas(40000);
  curve_point a, b;
  bignum256   s;
  uint8_t     cdata[96];
  memset(cdata, 0, 96);
  memcpy(cdata, evm->call_data.data, MIN(96, evm->call_data.len));

  bn_read_be(cdata, &a.x);
  bn_read_be(cdata + 32, &a.y);
  bn_read_be(cdata + 64, &s);

  point_multiply(&alt_bn128, &s, &a, &b);

  evm->return_data = bytes(_malloc(64), 64);
  bn_write_be(&b.x, evm->return_data.data);
  bn_write_be(&b.y, evm->return_data.data + 32);
  return 0;
}

int evm_run_precompiled(evm_t* evm, address_t address) {
  switch (address[19]) {
    case 1:
      return pre_ecrecover(evm);
    case 2:
      return pre_sha256(evm);
    case 3:
      return pre_ripemd160(evm);
    case 4:
      return pre_identity(evm);
    case 5:
      return pre_modexp(evm);
    case 6:
      return pre_ec_add(evm);
    case 7:
      return pre_ec_mul(evm);
    default:
      return -1;
  }
}
