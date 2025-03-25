#ifndef __MYLIB_BITMAP_H
#define __MYLIB_BITMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>
#include <string.h>

// Macros for bitmap
#define BITMAP_MARK "bitmap_mark"
#define BITMAP_TEST "bitmap_test"
#define BITMAP_SIZE "bitmap_size"
#define BITMAP_SET "bitmap_set"
#define BITMAP_SET_ALL "bitmap_set_all"
#define BITMAP_SET_MULTIPLE "bitmap_set_multiple"
#define BITMAP_SCAN "bitmap_scan"
#define BITMAP_SCAN_AND_FLIP "bitmap_scan_and_flip"
#define BITMAP_RESET "bitmap_reset"
#define BITMAP_NONE "bitmap_none"
#define BITMAP_FLIP "bitmap_flip"
#define BITMAP_EXPAND "bitmap_expand"
#define BITMAP_COUNT "bitmap_count"
#define BITMAP_CONTAINS "bitmap_contains"
#define BITMAP_ANY "bitmap_any"
#define BITMAP_ALL "bitmap_all"
#define BITMAP_DUMP "bitmap_dump"
#define OBJECT_BITMAP "bitmap"

/* Bitmap abstract data type. */

/* Element type.

   This must be an unsigned integer type at least as wide as int.

   Each bit represents one bit in the bitmap.
   If bit 0 in an element represents bit K in the bitmap,
   then bit 1 in the element represents bit K+1 in the bitmap,
   and so on. */
typedef unsigned long elem_type;

/* From the outside, a bitmap is an array of bits.  From the
   inside, it's an array of elem_type (defined above) that
   simulates an array of bits. */
struct bitmap
  {
    size_t bit_cnt;     /* Number of bits. */
    elem_type *bits;    /* Elements that represent bits. */
  };


/* Creation and destruction. */
struct bitmap *bitmap_create (size_t bit_cnt);
struct bitmap *bitmap_create_in_buf (size_t bit_cnt, void *, size_t block_size);
size_t bitmap_buf_size (size_t bit_cnt);
struct bitmap *bitmap_expand(struct bitmap *bitmap, int size);
void bitmap_destroy (struct bitmap *);

/* Bitmap size. */
size_t bitmap_size (const struct bitmap *);

/* Setting and testing single bits. */
void bitmap_set (struct bitmap *, size_t idx, bool);
void bitmap_mark (struct bitmap *, size_t idx);
void bitmap_reset (struct bitmap *, size_t idx);
void bitmap_flip (struct bitmap *, size_t idx);
bool bitmap_test (const struct bitmap *, size_t idx);

/* Setting and testing multiple bits. */
void bitmap_set_all (struct bitmap *, bool);
void bitmap_set_multiple (struct bitmap *, size_t start, size_t cnt, bool);
size_t bitmap_count (const struct bitmap *, size_t start, size_t cnt, bool);
bool bitmap_contains (const struct bitmap *, size_t start, size_t cnt, bool);
bool bitmap_any (const struct bitmap *, size_t start, size_t cnt);
bool bitmap_none (const struct bitmap *, size_t start, size_t cnt);
bool bitmap_all (const struct bitmap *, size_t start, size_t cnt);

/* Finding set or unset bits. */
#define BITMAP_ERROR SIZE_MAX
size_t bitmap_scan (const struct bitmap *, size_t start, size_t cnt, bool);
size_t bitmap_scan_and_flip (struct bitmap *, size_t start, size_t cnt, bool);

/* File input and output. */
size_t bitmap_file_size (const struct bitmap *);

/* Debugging. */
void bitmap_dump (const struct bitmap *);

#endif /* bitmap.h */
