#pragma once
#include<bits/stdc++.h>
#include "hash/hash_table.h"

namespace scudb {

template <typename K, typename V>
class ExtendibleHash : public HashTable<K, V> {
public:
  ExtendibleHash(size_t size);
  size_t HashKey(const K &key);
  int GetGlobalDepth() const;
  int GetLocalDepth(int bucket_id) const;
  int GetNumBuckets() const;
  bool Find(const K &key, V &value) override;
  bool Remove(const K &key) override;
  void Insert(const K &key, const V &value) override;

private:

  struct Buckets {
    Buckets(size_t i, int d) : id(i), depth(d) {}
    std::map<K, V> m;
    size_t id = 0;
    int depth = 0;
  };
  int depth;
  int bucket_count_;   
  size_t pair_count_;
  const size_t bucket_num;
  mutable std::mutex mutex_;
  std::vector<std::shared_ptr<Buckets>> bucket_;
  std::shared_ptr<Buckets> split(std::shared_ptr<Buckets> &);
};
}