from listNode import ListNode

class LinkedListLRU:
    def __init__(self, cache_size):
        self.head = ListNode('dummy', None)
        self.tail = self.head
        self.node_dict = {}
        self.size = 0
        self.cache_size = cache_size
        self.cache_slots = []  # 캐시 슬롯 리스트 추가

    def append(self, page):
        new_node = ListNode(page, None)
        self.node_dict[page] = new_node
        if self.size == 0:
            self.head.next = new_node
            self.tail = new_node
        else:
            self.tail.next = new_node
            self.tail = new_node
        self.size += 1
        self.cache_slots.append(page)  # 캐시 슬롯에 페이지 추가

    def remove_lru(self):
        if self.size == 0:
            return None
        lru_page = self.head.next.item
        self.head.next = self.head.next.next
        self.node_dict.pop(lru_page)
        self.size -= 1
        self.cache_slots.pop(0)  # 캐시 슬롯에서 가장 오래된 페이지 제거
        return lru_page

    def update(self, page):
        if page in self.node_dict:
            node = self.node_dict[page]
            prev, curr = self.remove(node)
            self.append(page)
        else:
            if self.size == self.cache_size:
                self.remove_lru()
            self.append(page)

    def remove(self, node):
        prev = None
        curr = self.head.next
        while curr != node:
            prev = curr
            curr = curr.next
        if prev:
            prev.next = curr.next
        else:
            self.head.next = curr.next
        if curr == self.tail:
            self.tail = prev
        self.size -= 1
        return prev, curr.next

class CacheSimulator:
    def __init__(self, cache_size):
        self.cache = LinkedListLRU(cache_size)
        self.cache_hit = 0
        self.tot_cnt = 1

    def do_sim(self, page):
        self.tot_cnt += 1
        if page in self.cache.cache_slots:
            self.cache_hit += 1
        else:
            if len(self.cache.cache_slots) < self.cache.cache_size:
                self.cache.update(page)
            else:
                self.cache.remove_lru()
                self.cache.update(page)

    def print_stats(self):
        print(f"cache_slot = {self.cache.cache_size}, cache_hit = {self.cache_hit}, hit ratio = {self.cache_hit / self.tot_cnt}")

if __name__ == "__main__":
    data_file = open("./linkbench.trc")
    lines = data_file.readlines()

    for cache_slots in range(100, 1001, 100):
        cache_sim = CacheSimulator(cache_slots)

        for line in lines:
            page = line.split()[0]
            cache_sim.do_sim(page)

        cache_sim.print_stats()
