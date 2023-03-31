#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cache.h"

static cache_entry_t *cache = NULL;
static int cache_size = 0;
static int clock = 0;
static int num_queries = 0;
static int num_hits = 0;

int cache_create(int num_entries)
{
  // printf("%d\n", num_entries);
  if (cache != NULL)
    return -1;
  if (num_entries < 2 || num_entries > 4096)
    return -1;
  cache_size = num_entries;
  cache = calloc(cache_size, sizeof(cache_entry_t));
  return 1;
}

int cache_destroy(void)
{
  if (cache == NULL)
    return -1;
  cache = NULL;
  cache_size = 0;
  free(cache);
  return 1;
}

int cache_lookup(int disk_num, int block_num, uint8_t *buf)
{
  clock++;
  num_queries++;

  if (buf == NULL || cache == NULL)
    return -1;

  for (int i = 0; i < cache_size; ++i)
  {
    // printf("cache's disk_num = %d\n", cache[i].disk_num);
    if (cache[i].valid && cache[i].disk_num == disk_num && cache[i].block_num == block_num)
    {
      num_hits++;
      cache[i].access_time = clock;
      memcpy(buf, cache[i].block, JBOD_BLOCK_SIZE);
      return 1;
      // break;
    }
  }
  return -1;
}

void cache_update(int disk_num, int block_num, const uint8_t *buf)
{
  clock++;

  for (int i = 0; i < cache_size; ++i)
  {
    if (cache[i].valid && cache[i].disk_num == disk_num && cache[i].block_num == block_num)
    {
      // num_hits++;
      cache[i].access_time = clock;
      memcpy(cache[i].block, buf, JBOD_BLOCK_SIZE);
      // return 1;
      // break;
    }
  }
}

int cache_insert(int disk_num, int block_num, const uint8_t *buf)
{
  if (disk_num < 0 || disk_num > 16)
    return -1;
  if (block_num < 0 || block_num > 65536)
    return -1;
  if (buf == NULL || cache == NULL)
    return -1;

  clock++;

  for (int i = 0; i < cache_size; i++)
  {
    if (cache[i].valid && cache[i].disk_num == disk_num && cache[i].block_num == block_num)
    {
      // num_hits++;

      return -1;
      // cache[i].access_time = clock;
      // memcpy(cache[i].block, buf, JBOD_BLOCK_SIZE);
      // return 1;
      // break;
    }
  }
  for (int i = 0; i < cache_size; i++)
  {

    if (cache[i].valid == 0)
    {

      cache[i].valid = 1;
      cache[i].disk_num = disk_num;
      cache[i].block_num = block_num;
      memcpy(cache[i].block, buf, JBOD_BLOCK_SIZE);
      cache[i].access_time = clock;
      return 1;
    }
  }

  int lowest_time = 1e9;
  int lowest_index = 0;

  for (int i = 0; i < cache_size; i++)
  {
    // printf("access time for index %d is %d and lowestTime is %d\n", cache[i].access_time, i, lowest_time);
    if (cache[i].access_time < lowest_time)
    {
      lowest_time = cache[i].access_time;
      lowest_index = i;
    }
  }
  cache[lowest_index].valid = 1;
  cache[lowest_index].disk_num = disk_num;
  cache[lowest_index].block_num = block_num;
  memcpy(cache[lowest_index].block, buf, JBOD_BLOCK_SIZE);
  cache[lowest_index].access_time = clock;
  // return 1;
  return 1;
  // check if any invalid cache is present
  // is present, toh overwrite,
  // else replace the cache with lowest accesstime
}

bool cache_enabled(void)
{
  if (cache == NULL)
    return false;
  return true;
}

void cache_print_hit_rate(void)
{
  // fprintf(stderr, "Num queries: %d\n", num_queries);
  fprintf(stderr, "Hit rate: %5.1f%%\n", 100 * (float)num_hits / num_queries);
}
