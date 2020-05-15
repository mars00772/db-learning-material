//
// Created by alex on 2020/5/15.
//

#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

#include <cstddef>
#include <cstdlib>
#include <random>
#include "Node.h"

template<typename K, typename V>
class skipList {
public:
    skipList() {
        createList();
    }

    ~skipList() {
        freeList();
    }

    Node<K, V> *find(K key) const;

    bool insert(K key, V value);

    bool remove(K key, V value);

    int size() {
        return nodeCount;
    }

    int getLevel() {
        return level;
    }

private:
    size_t nodeCount;
    int level;
    static const int MAXLEVEL = 16;
    Node<K, V> *headNode;
    Node<K, V> *footNode;//只做占位用

    void createList();

    void freeList();

    void createNode(int level, Node<K, V> *&node);

    void createNode(int level, Node<K, V> *&node, K key, V value);

    int getRandomLevel();
};

template<typename K, typename V>
void skipList<K, V>::createList() {

    //原始链表的占位节点
    createNode(0, footNode);
    footNode->key = -1;

    createNode(MAXLEVEL, headNode);
    for (int i = 0; i <= MAXLEVEL; i++) {
        headNode->forward[i] = footNode;
    }
    nodeCount = 0;
    this->level = 0;
}

template<typename K, typename V>
void skipList<K, V>::createNode(int level, Node<K, V> *&node) {

    node = new Node<K, V>(NULL, NULL);
    node->forward = new Node<K, V> *[level + 1];
    node->level = level;
}

template<typename K, typename V>
void skipList<K, V>::createNode(int level, Node<K, V> *&node, K key, V value) {
    node = new Node<K, V>(key, value);
    node->forward = new Node<K, V> *[level + 1];
    node->level = level;
}

template<typename K, typename V>
void skipList<K, V>::freeList() {
    Node<K, V> *p = headNode;
    Node<K, V> *q;
    while (p != nullptr) {
        q = p->forward[0];
        delete p;
        p = q;
    }
    delete p;
}

template<typename K, typename V>
Node<K, V> *skipList<K, V>::find(K key) const {
    Node<K, V> *node = headNode;
    for (int l = level; l >= 0; l--) {
        while (node->forward[l] != nullptr && (node->forward[l])->key < key) {
            node = node->forward[l];
        }
    }
    if (node->forward[0] != nullptr && node->forward[0]->key == key) {
        return node->forward[0];
    } else {
        return nullptr;
    }
}

template<typename K, typename V>
bool skipList<K, V>::remove(K key, V value) {
    Node<K, V> *update[MAXLEVEL];
    Node<K, V> *node = headNode;

    for (int l = level; l >= 0; l--) {
        while (node->forward[l] != nullptr
               && node->forward[l]->key < key) {
            node = node->forward[l];
        }
        update[l] = node;
    }

    node = node->forward[0];
    if (node->key != key || node->value !=value ) {
        return false;
    }

    for (int i = 0; i <= level; i++) {
        //从最底层往最上层开始删,如果这一层已经没有那个节点了,往上的层肯定也删除不了
        if (update[i]->forward[i] != node) {
            break;
        }
        update[i]->forward[i] = node->forward[i];
    }

    delete node;

    while(level>0&&headNode->forward[level]==footNode){
        level--;
    }

    nodeCount--;
}


template<typename K, typename V>
bool skipList<K, V>::insert(K key, V value) {
    Node<K, V> *node = headNode;
    int currentLevel = node->forward[0] == footNode ? 0 : getRandomLevel();

    if (currentLevel > level) {
        currentLevel = ++level;
    }

    Node<K, V> *newNode;
    createNode(currentLevel, newNode, key, value);

    for (int l = currentLevel; l >= 0; l--) {
        while (node->forward[l] != nullptr && node->forward[l]->key < key) {
            node = node->forward[l];
        }
        if (currentLevel >= l) {
            //第0层
            if (node->forward[l] == footNode) {
                node->forward[l] = newNode;
            } else if (node->forward[l]!= nullptr && node->forward[l]->key == key) {
                return false;
            } else {
                Node<K, V>*next = node->forward[l];
                node->forward[l] = newNode;
                newNode->forward[l] = next;
            }
        }
    }
    ++nodeCount;
    return true;
}


template <typename K, typename V>
int skipList<K,V>::getRandomLevel() {
    return static_cast<int>(rand()%MAXLEVEL+1);
}


#endif //SKIPLIST_SKIPLIST_H
