//
// Created by AlexanderChiu on 2020/5/22.
//

#ifndef STL_HASHTABLE_H
#define STL_HASHTABLE_H

#include "alloc.h"
#include "allocator.h"
#include "iterator.h"
#include <vector>
#include "construct.h"
#include <algorithm>

namespace MYSTL{

    //Hash Table Node
    template <typename Value>
    struct hashtable_node{
        hashtable_node* next;
        Value value;
    };

    const int mySTL_num_primers = 28;
    const unsigned int mySTL_primers_list[mySTL_num_primers] = {
            53,         97,         193,       389,       769,
            1543,       3079,       6151,      12289,     24593,
            49157,      98317,      196613,    393241,    786433,
            1572869,    3145739,    6291469,   12582917,  25165843,
            50331653,   100663319,  201326611, 402653189, 805306457,
            1610612741, 3221225473, 4294967291
    };

    inline unsigned int mySTL_next_prime(unsigned int n) {
        const unsigned int* pos = std::lower_bound(mySTL_primers_list, mySTL_primers_list
                                                                  + mySTL_num_primers, n);
        return pos == (mySTL_primers_list + mySTL_num_primers) ?
               *(mySTL_primers_list + mySTL_num_primers - 1) : *pos;
    }

    template <class Value, class Key, class HashFcn, class ExtractKey,
            class EqualKey, class Alloc = alloc>
    struct hashtable_iterator;


    template <class Value, class Key, class HashFcn, class ExtractKey,
            class EqualKey, class Alloc = alloc>
    class hashtable {
    public:
        typedef HashFcn                 hasher;
        typedef EqualKey                key_equal;
        typedef size_t                  size_type;
        typedef Value                   value_type;
        typedef Key                     key_type;
        typedef hashtable_node<Value>   node;
        typedef allocator<node, Alloc>  node_allocator;
        typedef hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
        std::vector<node*, Alloc> buckets;


    private:
        hasher      hash;
        key_equal   equals;
        ExtractKey  get_key;
        size_type            num_elements;


    private:

        node* new_node(const value_type& value){
            node* new_node = node_allocator::allocate();
            new_node->next = nullptr;
            construct(&value, value);
            return new_node;
        }

        void delete_node(node* n){
            destroy(n);
            node_allocator::deallocate(n);
        }

        size_type next_size(size_type n) const {
            return mySTL_next_prime(n);
        }

        void initialize_buckets(size_t n){
            const size_type bucket_nums = next_size(n);
            buckets.reserve(bucket_nums);
            buckets.insert(buckets.end(), bucket_nums, (node*)0);
            num_elements = 0;
        }

        void copy_from(const hashtable& ht){
            buckets.clear();
            buckets.reserve(ht.buckets.size());
            //因为buckets为空，所以实际上.end()就是buckets的起始处
            buckets.insert(buckets.end(), ht.buckets.size(), (hashtable_node*)0);

            for(size_type i = 0; i < ht.buckets.size(); i++){
                if(const node* curNode = ht.buckets[i]){
                    node* copy = new_node(curNode->value);
                    buckets[i] = copy;
                    for(node* next = copy->next; next!= nullptr; curNode=next,next=curNode->next){
                        copy->next = new_node(next->value);
                        copy = copy->next;
                    }
                }
            }
            num_elements = ht.num_elements;
        }


        void clear(){
            for(size_type i = 0; i < buckets.size(); i++){
                node* cur = buckets[i];
                while(cur!= nullptr){
                    node* next = cur->next;
                    delete_node(cur);
                    cur = next;
                }
                buckets[i] = nullptr;
            }
            num_elements = 0;
        }

    public:
        size_type bkt_num(const value_type& obj, size_t n) const {
            return bkt_num_key(get_key(obj), n);
        }
        size_type bkt_num(const value_type& obj) const {
            return bkt_num_key(get_key(obj));
        }

    private:
        size_type bkt_num_key(const key_type&key) const {
            return bkt_num_key(key, buckets.size());
        }
        size_type bkt_num_key(const key_type&key, size_t n) const {
            return hash(key) % n;
        }


    public:
        hashtable(size_type n, const HashFcn& hf, const EqualKey& eql) :
                hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0) {
            initialize_buckets(n);
        }

        //copy constructor
        hashtable(const hashtable& other):hash(other.hash),equals(other.equals),get_key(other.get_key),
        num_elements(0){
            copy_from(other);
        }

        hashtable& operator= (const hashtable& other){
            if(this!=&other){
                hash = other.hash;
                get_key = other.get_key;
                equals = other.equals;
                copy_from(other);
            }
            return *this;
        }

        ~hashtable(){
            clear();
        }


    public:
        size_type size() const {
            return num_elements;
        }

        size_type num_buckets() const {
            return buckets.size();
        }

        iterator end() {
            //return iterator(nullptr, this);
        }
    };


    template <class Value, class Key, class HashFcn, class ExtractKey,
            class EqualKey, class Alloc = alloc>
    struct hashtable_iterator {
        typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
        typedef hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
        typedef hashtable_node<Value>   node;
        typedef forward_iterator_tag    iterator_category;
        typedef Value                   value_type;
        typedef ptrdiff_t               difference_type;
        typedef size_t                  size_type;
        typedef Value&                  reference;

        node* cur;
        hashtable* ht;

        hashtable_iterator(){}

        hashtable_iterator(node* n, hashtable* table):cur(n),ht(table){
        }

        reference operator* () const {
            return cur->value;
        }

        iterator& operator++() {
            const node* old = cur;
            cur = cur->next;
            if (cur == nullptr) {
                size_type bucket = ht->bkt_num(old->value);
                while (cur == nullptr && ++bucket < ht->buckets.size())
                    cur = ht->buckets[bucket];
            }
            return *this;
        }
        //后置++
        iterator operator++(int) {
            iterator t = *this;
            ++*this;
            return t;
        }

        bool operator== (const iterator& other) const {
            return cur == other.cur;
        }

        bool operator!= (const iterator& other) const {
            return cur != other.cur;
        }

    };




}



#endif //STL_HASHTABLE_H
