#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cache.h"

static cache_entry_t *cache = NULL;
static int cache_size = 0;
//static int clock = 0;
static int num_queries = 0;
static int num_hits = 0;

int cache_create(int num_entries) {
  cache_size = num_entries;
  cache = calloc(cache_size, sizeof(cache_entry_t));
  return -1;
}

int cache_destroy(void) {
  return -10;
}

int cache_lookup(int disk_num, int block_num, uint8_t *buf) {
  for (int i = 0; i < cache_size; ++i)
  {
    if (cache[i].valid && cache[i].disk_num == disk_num && cache[i].block_num == block_num)
    {
      memcpy(buf, cache[i].block, JBOD_BLOCK_SIZE);
      break;
    }
  }
  return -1;
}

void cache_update(int disk_num, int block_num, const uint8_t *buf) {
}

int cache_insert(int disk_num, int block_num, const uint8_t *buf) {
  return -10;
}

bool cache_enabled(void) {
  return false;
}

void cache_print_hit_rate(void) {
  fprintf(stderr, "Hit rate: %5.1f%%\n", 100 * (float) num_hits / num_queries);
}
