//
// Created by AlexanderChiu on 2020/5/31.
//

#ifndef LOCKFREEQUEUE_LOCKFREEQUEUE_H
#define LOCKFREEQUEUE_LOCKFREEQUEUE_H

#include <atomic>
#include <algorithm>

template <typename T>
class LockFreeQueue {
private:
    struct Node {
        explicit Node(const T& val) : next{nullptr},value{val}{};
        explicit Node(T && val) : next{nullptr},value{val}{};
        std::atomic<Node*> next;
        T value;
    };
    std::atomic<Node*> head;
    std::atomic<Node*> tail;

public:
    LockFreeQueue(){
        head = tail = new Node(T());
    }

    ~LockFreeQueue(){
        Node* cur = head;
        while (cur)
        {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
    }

    //origin version
    void enqueue(const T& value){

        Node* newTailNode = new Node(value);
        Node* oldTailNode = nullptr;
        Node* nullNode = nullptr;
        do{
            oldTailNode = tail.load();
        }while(!oldTailNode->next.compare_exchange_weak(nullNode, newTailNode));
        tail.compare_exchange_weak(oldTailNode, newTailNode);
    }

    void enqueue(T && value){
        Node* newTailNode = new Node((value));
        Node* oldTailNode = nullptr;
        Node* nullNode = nullptr;
        do{
            oldTailNode = tail.load();
        }while(!oldTailNode->next.compare_exchange_weak(nullNode, newTailNode));
        tail.compare_exchange_weak(oldTailNode, newTailNode);
    }
    T dequeue(){
        Node* oldHeadNode = head.load();
        T result;
        do {
            //isDummy
            Node* next = oldHeadNode->next;
            if(next == nullptr){
                return T();
            }else{
                result = next->value;
            }
        }while(!head.compare_exchange_weak(oldHeadNode, oldHeadNode->next));
        delete oldHeadNode;
        return std::move(result);
    }
};


#endif //LOCKFREEQUEUE_LOCKFREEQUEUE_H
