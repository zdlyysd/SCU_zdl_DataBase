#include<bits/stdc++.h>
#include "hash/extendible_hash.h"
#include "page/page.h"

namespace scudb {

template <typename K, typename V>
ExtendibleHash<K, V>::ExtendibleHash(size_t size)
:depth(0) ,bucket_count_(0), pair_count_(0),  bucket_num(size)
{
    bucket_.emplace_back(new Buckets(0, 0));
    bucket_count_ = 1;
}

template <typename K, typename V>
size_t ExtendibleHash<K, V>::HashKey(const K &key) 
{
    return std::hash<K>()(key);
}

template <typename K, typename V>
int ExtendibleHash<K, V>::GetGlobalDepth() const 
{
    std::lock_guard<std::mutex> lock(mutex_);
    return depth;
}

template <typename K, typename V>
int ExtendibleHash<K, V>::GetLocalDepth(int bucket_id) const 
{
    std::lock_guard<std::mutex> lock(mutex_);
    return bucket_[bucket_id]->depth;
}

template <typename K, typename V>
int ExtendibleHash<K, V>::GetNumBuckets() const 
{
    std::lock_guard<std::mutex> lock(mutex_);
    return bucket_num;
}

template <typename K, typename V>
bool ExtendibleHash<K, V>::Find(const K &key, V &value) 
{
    std::lock_guard<std::mutex> lock(mutex_);
    size_t position = HashKey(key) & ((1 << depth) - 1);

    if(bucket_[position]) 
    {
        if(bucket_[position]->m.find(key) != bucket_[position]->m.end()) 
        {
            value = bucket_[position]->m[key];
            return true;
        }
    }
    return false;
}

template <typename K, typename V>
bool ExtendibleHash<K, V>::Remove(const K &key) 
{
    std::lock_guard<std::mutex> lock(mutex_);
    size_t position = HashKey(key) & ((1 << depth) - 1);
    size_t cnt = 0;

    if(bucket_[position]) 
    {
        auto tmp_bucket = bucket_[position];
        cnt = tmp_bucket->m.erase(key);
        pair_count_ -= cnt;
    }
    if(cnt == 0) return false;
    return true;
}

template <typename K, typename V>
void ExtendibleHash<K, V>::Insert(const K &key, const V &value) 
{
    std::lock_guard<std::mutex> lock(mutex_);
    size_t bucket_id = HashKey(key) & ((1 << depth) - 1);
    if(bucket_[bucket_id] == nullptr) 
    {
        bucket_[bucket_id] = std::make_shared<Buckets>(bucket_id, depth);
        ++ bucket_count_;
    }
    auto bucket = bucket_[bucket_id];

    if(bucket->m.find(key) != bucket->m.end()) 
    {
        bucket->m[key] = value;
        return;
    }

    bucket->m.insert({key, value});
    ++pair_count_;

    if(bucket->m.size() > bucket_num) 
    {
        auto old_index = bucket->id;
        auto old_depth = bucket->depth;
        std::shared_ptr<Buckets> new_bucket = split(bucket);
        if(new_bucket == nullptr) 
        {
            bucket->depth = old_depth;
            return;
        }
        if(bucket->depth > depth) 
        {
            auto size = bucket_.size();
            auto factor = (1 << (bucket->depth - depth));
            depth = bucket->depth;
            bucket_.resize(bucket_.size()*factor);
            bucket_[bucket->id] = bucket;
            bucket_[new_bucket->id] = new_bucket;
            for(size_t i = 0; i < size; ++i) 
            {
                if(bucket_[i]) 
                {
                    if(i < bucket_[i]->id)
                    {
                        bucket_[i].reset();
                    } else 
                    {
                        auto step = 1 << bucket_[i]->depth;
                        for(size_t j = i + step; j < bucket_.size(); j += step) 
                        {
                            bucket_[j] = bucket_[i];
                        }
                    }
                }
            }
        } 
        else 
        {
            for (size_t i = old_index; i < bucket_.size(); i += (1 << old_depth)) 
            {
                bucket_[i].reset();
            }
            bucket_[bucket->id] = bucket;
            bucket_[new_bucket->id] = new_bucket;
            auto step = 1 << bucket->depth;
            for (size_t i = bucket->id + step; i < bucket_.size(); i += step) 
            {
                bucket_[i] = bucket;
            }
            for (size_t i = new_bucket->id + step; i < bucket_.size(); i += step) 
            {
                bucket_[i] = new_bucket;
            }
        }
    }
}

template <typename K, typename V>
std::shared_ptr<typename ExtendibleHash<K, V>::Buckets>
ExtendibleHash<K, V>::split(std::shared_ptr<Buckets> &b) 
{
    auto res = std::make_shared<Buckets>(0, b->depth);
    while(res->m.empty()) 
    {
        b->depth++;
        res->depth++;
        for(auto it = b->m.begin(); it != b->m.end();) 
        {
            if (HashKey(it->first) & (1 << (b->depth - 1))) 
            {
                res->m.insert(*it);
                res->id = HashKey(it->first) & ((1 << b->depth) - 1);
                it = b->m.erase(it);
            } 
            else 
            {
                ++it;
            }
        }
        if(b->m.empty()) 
        {
            b->m.swap(res->m);
            b->id = res->id;
        }
    }
    ++ bucket_count_;
    return res;
}

template class ExtendibleHash<page_id_t, Page *>;
template class ExtendibleHash<Page *, std::list<Page *>::iterator>;

template class ExtendibleHash<int, std::string>;
template class ExtendibleHash<int, std::list<int>::iterator>;
template class ExtendibleHash<int, int>;
}