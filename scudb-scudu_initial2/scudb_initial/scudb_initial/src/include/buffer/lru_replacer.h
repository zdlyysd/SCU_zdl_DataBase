
#pragma once
#include<bits/stdc++.h>
#include "buffer/replacer.h"

namespace scudb {

template <typename T> class LRUReplacer : public Replacer<T> {
public:

  LRUReplacer();

  ~LRUReplacer();

  void Insert(const T &value);

  bool Victim(T &value);

  bool Erase(const T &value);

  size_t Size();

private:

  struct Link {
    Link(T v) :value(v) {
    }

    T value;
    std::shared_ptr<Link> pre;
    std::shared_ptr<Link> next;
  };

  std::shared_ptr<Link> head;
  std::shared_ptr<Link> tail;
  int size = 0;
  std::mutex mutex;
  std::map<T, std::shared_ptr<Link>> index;
};
}
