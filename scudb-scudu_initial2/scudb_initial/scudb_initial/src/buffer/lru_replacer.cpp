#include "buffer/lru_replacer.h"
#include "page/page.h"

namespace scudb {

template <typename T> LRUReplacer<T>::LRUReplacer() {}

template <typename T> LRUReplacer<T>::~LRUReplacer() {}

template <typename T> void LRUReplacer<T>::Insert(const T &value) {
    std::lock_guard<std::mutex> guard(mutex);

    auto iter = index.find(value);
    if (iter != index.end()) {
      auto ptr = iter->second;
      if (ptr == head && ptr == tail) {
          head = nullptr;
          tail = nullptr;
      } else if (ptr == head) {
          ptr->next->pre = nullptr;
          head = ptr->next;
      } else if (ptr == tail) {
          ptr->pre->next = nullptr;
          tail = ptr->pre;
      } else {
          ptr->pre->next = ptr->next;
          ptr->next->pre = ptr->pre;
      }
      ptr->pre = nullptr;
      ptr->next = nullptr;

      index.erase(value);
      size--;
    }

    auto node = std::make_shared<Link>(value);
    if (node == nullptr) {
        return;
    }
    node->pre = nullptr;
    node->next = head;

    if (head != nullptr) {
        head->pre = node;
    }
    head = node;
    if (tail == nullptr) {
        tail = node;
    }

    index[node->value] = node;
    size++;
}

template <typename T> bool LRUReplacer<T>::Victim(T &value) {
    std::lock_guard<std::mutex> guard(mutex);

    if (size == 0) {
        return false;
    }
    if (head == tail) {
        value = head->value;
        head = nullptr;
        tail = nullptr;

        index.erase(value);
        size--;
        return true;
    }
    value = tail->value;
    auto discard = tail;
    discard->pre->next = nullptr;
    tail = discard->pre;
    discard->pre = nullptr;

    index.erase(value);
    size--;
    return true;
}

template <typename T> bool LRUReplacer<T>::Erase(const T &value) {
    std::lock_guard<std::mutex> guard(mutex);

    auto iter = index.find(value);
    if (iter == index.end()) {
        return false;
    }

    auto ptr = iter->second;
    if (ptr == head && ptr == tail) {
        head = nullptr;
        tail = nullptr;
    } else if (ptr == head) {
        ptr->next->pre = nullptr;
        head = ptr->next;
    } else if (ptr == tail) {
        ptr->pre->next = nullptr;
        tail = ptr->pre;
    } else {
        ptr->pre->next = ptr->next;
        ptr->next->pre = ptr->pre;
    }
    ptr->pre = nullptr;
    ptr->next = nullptr;

    index.erase(value);
    size--;
    return true;
}

template <typename T> size_t LRUReplacer<T>::Size() { return size; }

template class LRUReplacer<Page *>;

template class LRUReplacer<int>;

}
