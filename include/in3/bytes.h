/*******************************************************************************
 * This file is part of the Incubed project.
 * Sources: https://github.com/slockit/in3-c
 * 
 * Copyright (C) 2018-2019 slock.it GmbH, Blockchains LLC
 * 
 * 
 * COMMERCIAL LICENSE USAGE
 * 
 * Licensees holding a valid commercial license may use this file in accordance 
 * with the commercial license agreement provided with the Software or, alternatively, 
 * in accordance with the terms contained in a written agreement between you and 
 * slock.it GmbH/Blockchains LLC. For licensing terms and conditions or further 
 * information please contact slock.it at in3@slock.it.
 * 	
 * Alternatively, this file may be used under the AGPL license as follows:
 *    
 * AGPL LICENSE USAGE
 * 
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Affero General Public License as published by the Free Software 
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *  
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
 * PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
 * [Permissions of this strong copyleft license are conditioned on making available 
 * complete source code of licensed works and modifications, which include larger 
 * works using a licensed work, under the same license. Copyright and license notices 
 * must be preserved. Contributors provide an express grant of patent rights.]
 * You should have received a copy of the GNU Affero General Public License along 
 * with this program. If not, see <https://www.gnu.org/licenses/>.
 *******************************************************************************/

// @PUBLIC_HEADER
/** @file
 * util helper on byte arrays.
 * */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef BYTES_H
#define BYTES_H

#define bb_new() bb_newl(32)
#define bb_read(_bb_, _i_, _vptr_) bb_readl((_bb_), (_i_), (_vptr_), sizeof(*_vptr_))
#define bb_read_next(_bb_, _iptr_, _vptr_)      \
  do {                                          \
    size_t _l_ = sizeof(*_vptr_);               \
    bb_readl((_bb_), *(_iptr_), (_vptr_), _l_); \
    *(_iptr_) += _l_;                           \
  } while (0)
#define bb_readl(_bb_, _i_, _vptr_, _l_) memcpy((_vptr_), (_bb_)->b.data + (_i_), _l_)
#define b_read(_b_, _i_, _vptr_) b_readl((_b_), (_i_), _vptr_, sizeof(*_vptr_))
#define b_readl(_b_, _i_, _vptr_, _l_) memcpy(_vptr_, (_b_)->data + (_i_), (_l_))

typedef uint8_t      address_t[20]; /**< pointer to a 20byte address */
typedef uint8_t      bytes32_t[32]; /**< pointer to a 32byte word */
typedef uint_fast8_t wlen_t;        /**< number of bytes within a word (min 1byte but usually a uint) */

/** a byte array */
typedef struct bytes {
  uint32_t len;  /**< the length of the array ion bytes */
  uint8_t* data; /**< the byte-data  */
} bytes_t;

/** a byte-buffer to attach byte-functions. */
typedef struct {
  uint32_t bsize; /**< size of the currently allocated bytes */
  bytes_t  b;     /**< the bytes struct */
} bytes_builder_t;

/** allocates a new byte array with 0 filled */
bytes_t* b_new(char* data, int len);

/** prints a the bytes as hex to stdout */
void b_print(bytes_t* a);

/** prints a the bytes as hex to stdout */
void ba_print(uint8_t* a, size_t l);

/** compares 2 byte arrays and returns 1 for equal and 0 for not equal*/
int b_cmp(bytes_t* a, bytes_t* b);

/** compares 2 byte arrays and returns 1 for equal and 0 for not equal*/
int bytes_cmp(bytes_t a, bytes_t b);

/** frees the data */
void b_free(bytes_t* a);

/** clones a byte array*/
bytes_t* b_dup(bytes_t* a);

/** reads a byte on the current position and updates the pos afterwards. */
uint8_t b_read_byte(bytes_t* b, size_t* pos);
/** reads a integer on the current position and updates the pos afterwards. */
uint32_t b_read_int(bytes_t* b, size_t* pos);
/** reads a long on the current position and updates the pos afterwards. */
uint64_t b_read_long(bytes_t* b, size_t* pos);
/** creates a new string (needs to be freed) on the current position and updates the pos afterwards. */
char* b_new_chars(bytes_t* b, size_t* pos);

/** reads bytes with a fixed length on the current position and updates the pos afterwards. */
bytes_t* b_new_fixed_bytes(bytes_t* b, size_t* pos, int len);

/* creates a new bytes_builder */
bytes_builder_t* bb_newl(size_t l);
/** frees a bytebuilder and its content. */
void bb_free(bytes_builder_t* bb);

/** internal helper to increase the buffer if needed */
int bb_check_size(bytes_builder_t* bb, size_t len);

/** writes a string to the builder. */
void bb_write_chars(bytes_builder_t* bb, char* c, int len);
/** writes bytes to the builder with a prefixed length. */
void bb_write_dyn_bytes(bytes_builder_t* bb, bytes_t* src);
/** writes fixed bytes to the builder. */
void bb_write_fixed_bytes(bytes_builder_t* bb, bytes_t* src);
/** writes a ineteger to the builder. */
void bb_write_int(bytes_builder_t* bb, uint32_t val);
/** writes s long to the builder. */
void bb_write_long(bytes_builder_t* bb, uint64_t val);
/*! writes any integer value with the given length of bytes */
void bb_write_long_be(bytes_builder_t* bb, uint64_t val, int len);
/** writes a single byte to the builder. */
void bb_write_byte(bytes_builder_t* bb, uint8_t val);
/** writes the bytes to the builder. */
void bb_write_raw_bytes(bytes_builder_t* bb, void* ptr, size_t len);
/** resets the content of the builder. */
void bb_clear(bytes_builder_t* bb);
/** replaces or deletes a part of the content. */
void bb_replace(bytes_builder_t* bb, int offset, int delete_len, uint8_t* data, int data_len);
/** frees the builder and moves the content in a newly created bytes struct (which needs to be freed later). */
bytes_t* bb_move_to_bytes(bytes_builder_t* bb);

void bb_push(bytes_builder_t* bb, uint8_t* data, uint8_t len);

uint64_t bb_read_long(bytes_builder_t* bb, size_t* i);
uint32_t bb_read_int(bytes_builder_t* bb, size_t* i);

static inline bytes_t bytes(uint8_t* a, uint32_t len) {
  return (bytes_t){.data = a, .len = len};
}

bytes_t cloned_bytes(bytes_t data);

static inline void b_optimize_len(bytes_t* b) {
  while (b->len > 1 && *b->data == 0) {
    b->data++;
    b->len--;
  }
}

#endif
