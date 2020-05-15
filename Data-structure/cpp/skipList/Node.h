//
// Created by alex on 2020/5/15.
//

#ifndef SKIPLIST_NODE_H
#define SKIPLIST_NODE_H

template <typename K, typename V>
class skipList;

template <typename K, typename V>
class Node{
    friend class skipList<K,V>;
public:
    Node(){}
    Node(K, V);
    ~Node();
    K getKey() const;
    V getValue() const;
private:
    Node<K, V>** forward;
    int level{};
    K key;
    V value;
};

template <typename K, typename V>
Node<K,V>::Node(K k, V v) {
    this->key = k;
    this->value = v;
}

template <typename K, typename V>
Node<K,V>::~Node() {
    delete [] forward;
}

template <typename K, typename V>
K Node<K,V>::getKey() const {
    return key;
}

template <typename K, typename V>
V Node<K,V>::getValue() const {
    return value;
}



#endif //SKIPLIST_NODE_H
