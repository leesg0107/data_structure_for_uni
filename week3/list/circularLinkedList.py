from linkedListBasic import LinkedListBasic
from listNode import ListNode

class CircularLinkedList(LinkedListBasic):
    def __init__(self):
        super().__init__()
        self.__tail = ListNode('dummy', None)
        self.__tail.next = self.__tail
        self.__numItems = 0

    def insert(self, i: int, newItem):
        if i < 0 or i > self.__numItems:
            print(f"Index {i} out of bounds for insert")
            return

        newNode = ListNode(newItem, None)
        if i == 0:
            newNode.next = self.__tail.next.next
            self.__tail.next.next = newNode
            if self.__numItems == 0:
                self.__tail = newNode
        else:
            prev = self._getNode(i - 1 if i > 0 else self.__numItems - 1)
            newNode.next = prev.next
            prev.next = newNode
            if i == self.__numItems:
                self.__tail = newNode

        self.__numItems += 1

    def append(self, newItem):
        self.insert(self.__numItems, newItem)

    def pop(self, i: int):
        if i < 0 or i >= self.__numItems:
            print(f"Index {i} out of bounds for pop")
            return None

        if i == 0:
            removedNode = self.__tail.next.next
            self.__tail.next.next = removedNode.next
        else:
            prev = self._getNode(i - 1)
            removedNode = prev.next
            prev.next = removedNode.next
            if i == self.__numItems - 1:
                self.__tail = prev

        self.__numItems -= 1
        return removedNode.item

    def remove(self, x):
        curr = self.__tail.next.next
        prev = self.__tail.next
        for i in range(self.__numItems):
            if curr.item == x:
                prev.next = curr.next
                if curr == self.__tail:
                    self.__tail = prev
                self.__numItems -= 1
                return x
            prev = curr
            curr = curr.next
        return None

    def get(self, i: int):
        if i < 0 or i >= self.__numItems:
            print(f"Index {i} out of bounds for get")
            return None

        return self._getNode(i).item

    def size(self):
        return self.__numItems

    def isEmpty(self):
        return self.__numItems == 0

    def printList(self):
        curr = self.__tail.next.next
        for _ in range(self.__numItems):
            print(curr.item, end=' ')
            curr = curr.next
        print()

    def _getNode(self, i: int) -> ListNode:
        if i == -1:
            return self.__tail
        curr = self.__tail.next.next
        for _ in range(i):
            curr = curr.next
        return curr

    def __iter__(self):
        return CircularLinkedListIterator(self)

class CircularLinkedListIterator:
    def __init__(self, alist):
        self.__head = alist._getNode(-1)
        self.iterPosition = self.__head.next

    def __next__(self):
        if self.iterPosition == self.__head:
            raise StopIteration
        else:
            item = self.iterPosition.item
            self.iterPosition = self.iterPosition.next
            return item
