#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListNode {
  char *item;
  struct ListNode *next;
  struct ListNode *prev;
} ListNode;

typedef struct LinkedListLRU {
  ListNode *head;
  ListNode *tail;
  int size;
  int cache_size;
} LinkedListLRU;

ListNode *createNode(char *item) {
  ListNode *newNode = (ListNode *)malloc(sizeof(ListNode));
  newNode->item = strdup(item);
  newNode->next = NULL;
  newNode->prev = NULL;
  return newNode;
}

void appendNode(LinkedListLRU *list, char *item) {
  ListNode *newNode = createNode(item);
  if (list->size == 0) {
    list->head = newNode;
    list->tail = newNode;
  } else {
    list->tail->next = newNode;
    newNode->prev = list->tail;
    list->tail = newNode;
  }
  list->size++;
}

void moveToTail(LinkedListLRU *list, ListNode *node) {
  if (node == list->tail) {
    return;
  }
  if (node == list->head) {
    list->head = node->next;
    list->head->prev = NULL;
  } else {
    node->prev->next = node->next;
    node->next->prev = node->prev;
  }
  node->next = NULL;
  node->prev = list->tail;
  list->tail->next = node;
  list->tail = node;
}

void removeLRU(LinkedListLRU *list) {
  if (list->size == 0) {
    return;
  }
  ListNode *lruNode = list->head;
  list->head = lruNode->next;
  if (list->head != NULL) {
    list->head->prev = NULL;
  }
  free(lruNode->item);
  free(lruNode);
  list->size--;
}

int cacheContains(LinkedListLRU *list, char *page) {
  ListNode *curr = list->head;
  while (curr != NULL) {
    if (strcmp(curr->item, page) == 0) {
      moveToTail(list, curr);
      return 1;
    }
    curr = curr->next;
  }
  return 0;
}

void updateCache(LinkedListLRU *list, char *page) {
  if (cacheContains(list, page)) {
    return;
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
  return cache;
}

void freeCache(LinkedListLRU *cache) {
  ListNode *curr = cache->head;
  while (curr != NULL) {
    ListNode *temp = curr;
    curr = curr->next;
    free(temp->item);
    free(temp);
  }
  free(cache);
}

void simulateCache(LinkedListLRU *cache, char *page) {
  updateCache(cache, page);
}

void printStats(LinkedListLRU *cache, int total_count, int cache_hit) {
  printf("cache_slot = %d, cache_hit = %d, hit ratio = %.17f\n",
         cache->cache_size, cache_hit, (double)cache_hit / total_count);
}

int main() {
  FILE *data_file = fopen("./linkbench.trc", "r");
  if (data_file == NULL) {
    printf("Failed to open the file.\n");
    return 1;
  }

  char line[256];
  int cache_slots;
  for (cache_slots = 100; cache_slots <= 1000; cache_slots += 100) {
    LinkedListLRU *cache_sim = initializeCache(cache_slots);
    fseek(data_file, 0, SEEK_SET);

    int total_count = 0;
    int cache_hit = 0;
    while (fgets(line, sizeof(line), data_file)) {
      char *page = strtok(line, " ");
      if (cacheContains(cache_sim, page)) {
        cache_hit++;
      }
      simulateCache(cache_sim, page);
      total_count++;
    }

    printStats(cache_sim, total_count, cache_hit);
    freeCache(cache_sim);
  }

  fclose(data_file);
  return 0;
}
