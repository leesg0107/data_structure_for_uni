#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListNode {
  char *item;
  struct ListNode *next;
} ListNode;

typedef struct LinkedListLRU {
  ListNode *head;
  ListNode *tail;
  int size;
  int cache_size;
  char **cache_slots;
  int num_cache_slots;
} LinkedListLRU;

ListNode *createNode(char *item) {
  ListNode *newNode = (ListNode *)malloc(sizeof(ListNode));
  newNode->item = strdup(item);
  newNode->next = NULL;
  return newNode;
}

void appendNode(LinkedListLRU *list, char *item) {
  ListNode *newNode = createNode(item);
  if (list->size == 0) {
    list->head = newNode;
    list->tail = newNode;
  } else {
    list->tail->next = newNode;
    list->tail = newNode;
  }
  list->size++;
  list->cache_slots[list->num_cache_slots++] = strdup(item);
}

char *removeLRU(LinkedListLRU *list) {
  if (list->size == 0) {
    return NULL;
  }
  ListNode *temp = list->head;
  list->head = list->head->next;
  char *lru_page = temp->item;
  free(temp);
  list->size--;
  char *cache_slot_to_remove = list->cache_slots[0];
  for (int i = 0; i < list->num_cache_slots - 1; i++) {
    list->cache_slots[i] = list->cache_slots[i + 1];
  }
  list->num_cache_slots--;
  return lru_page;
}

void updateCache(LinkedListLRU *list, char *page) {
  ListNode *curr = list->head;
  while (curr != NULL) {
    if (strcmp(curr->item, page) == 0) {
      ListNode *prev = NULL;
      ListNode *temp = list->head;
      while (temp != curr) {
        prev = temp;
        temp = temp->next;
      }
      if (prev == NULL) {
        list->head = curr->next;
      } else {
        prev->next = curr->next;
      }
      list->tail->next = curr;
      curr->next = NULL;
      list->tail = curr;
      return;
    }
    curr = curr->next;
  }
  if (list->size == list->cache_size) {
    removeLRU(list);
  }
  appendNode(list, page);
}

LinkedListLRU *initializeCache(int cache_size) {
  LinkedListLRU *cache = (LinkedListLRU *)malloc(sizeof(LinkedListLRU));
  cache->head = NULL;
  cache->tail = NULL;
  cache->size = 0;
  cache->cache_size = cache_size;
  cache->cache_slots = (char **)malloc(sizeof(char *) * cache_size);
  cache->num_cache_slots = 0;
  return cache;
}

void simulateCache(LinkedListLRU *cache, char *page) {
  cache->num_cache_slots++;
  int hit = 0;
  for (int i = 0; i < cache->num_cache_slots; i++) {
    if (strcmp(cache->cache_slots[i], page) == 0) {
      hit = 1;
      break;
    }
  }
  if (hit) {
    cache->num_cache_slots++;
  } else {
    updateCache(cache, page);
  }
}

void printStats(LinkedListLRU *cache) {
  int total_count = cache->num_cache_slots;
  int cache_hit = 0;
  for (int i = 0; i < cache->num_cache_slots; i++) {
    ListNode *curr = cache->head;
    while (curr != NULL) {
      if (strcmp(curr->item, cache->cache_slots[i]) == 0) {
        cache_hit++;
        break;
      }
      curr = curr->next;
    }
  }
  printf("cache_slots=%d, cache_hit=%d, hit ratio=%.9f\n", cache->cache_size,
         cache_hit, (float)cache_hit / total_count);
}

int main() {
  FILE *data_file = fopen("./linkbench.trc", "r");
  if (data_file == NULL) {
    printf("ERROR: Cannot open file.\n");
    return 1;
  }
  char line[256];
  while (fgets(line, sizeof(line), data_file) != NULL) {
    char *page = strtok(line, " ");
    for (int cache_size = 100; cache_size <= 1000; cache_size += 100) {
      LinkedListLRU *cache = initializeCache(cache_size);
      while (fgets(line, sizeof(line), data_file) != NULL) {
        page = strtok(line, " ");
        simulateCache(cache, page);
      }
      printStats(cache);
      rewind(data_file);
    }
  }
  fclose(data_file);
  return 0;
}
