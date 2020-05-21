//
// Created by AlexanderChiu on 2020/5/21.
//

#include <cstdlib>
#include "alloc.h"

namespace MYSTL{

    char* alloc::start_free = 0;
    char* alloc::end_free = 0;
    size_t alloc::heap_size = 0;
    alloc::obj* alloc::free_list[FREE_LISTS] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    static void* alloc::allocate(size_t n) {
        //如果大于128bytes
        if(n>MAX_BYTES){
            return std::malloc(n);
        }

        size_t index = alloc::free_list_index(n);
        alloc::obj* list = alloc::free_list[index];

        if(list){
            free_list[index] = list->next;
            return list;
        }else{
            //如果没找到就重新填充free_list
            return alloc::refill(ROUND_UP(n));
        }
    }

    static void alloc::deallocate(void *p, size_t n) {
        if(n>MAX_BYTES){
            free(p);
            return;
        }
        size_t index = alloc::free_list_index(n);
        obj *q = static_cast<alloc::obj*>(p);
        //q是回收的空闲块指针，回收后插在空闲链表队首，因此其next指向原来的队首
        q->next = free_list[index];
        free_list[index] = q;
    }

    static void* alloc::reallocate(void *p, size_t old_size, size_t new_size) {
        deallocate(p, old_size);
        return allocate(new_size);
    }

    /**
     * 默认分配20块大小为n bytes的block
     *
     * @param n
     * @return
     */
    static void *alloc::refill(size_t n) {

        int objs = 20;
        char *chunk = chunk_alloc(n, objs);

        //如果只分配一块，这个就分配给调用者 不用调整链表结构
        if(objs==1){
            return chunk;
        }

        void* result = chunk;
        obj* current_obj;
        obj* next_obj = (obj*)(chunk + n);

        for(int i = 1; i < objs;i++){
            current_obj = next_obj;
            next_obj = (obj*)((char*)next_obj + n);
            if(i == objs-1){
                current_obj->next = nullptr;
                break;
            }else{
                current_obj->next = next_obj;
            }
        }
        return result;
    }


    /***
     *
     * 大概工作原理：以end_free - start_free来判断内存池的内存是否充足
     *
     * 如果充足就调用20个block返回给free_list
     *
     * 如果不充足，但是又能分配1个以上，那么就把这不足20个的块分配出去
     *
     * 这也是为什么objs传引用，要在递归函数中不断修改为实际能供应的区块数量
     *
     * 如果还是不够就调用malloc，分配量为需求量的两倍
     *
     * @param size 每个块的大小
     * @param objs 默认分配20个块
     * @return
     */
    //实现内存池
    char *alloc::chunk_alloc(size_t size, int &objs) {
        char* result;
        size_t total_bytes = size * objs;
        size_t bytes_left = end_free - start_free;

        //能够满足u分配需求
        if(bytes_left >= total_bytes){
            result = start_free;
            start_free+=total_bytes;
            return result;
        }else if(bytes_left >= size){
            //内存池剩余空间不能满足，但是足够可以分配一个以上的区块
            objs = bytes_left / size;
            total_bytes = size * objs;
            result = start_free;
            start_free+=total_bytes;
            return result;
        }else{

            size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >>4);
            if(bytes_left > 0){
                //让内存池剩余的空间配给相应的free_list
                size_t index = free_list_index(bytes_left);
                static_cast<obj*>(start_free)->next = free_list[index];
                free_list[index] = static_cast<obj*>(start_free);
            }

            start_free = (char*) malloc(bytes_to_get);
            if(start_free==nullptr){
                size_t index;
                obj* volatile p;
                //分配失败，遍历free_list看是否有没有利用的快
                for(size_t s = size; s <= MAX_BYTES; s+=ALIGN){
                    index = free_list_index(s);
                    p = free_list[index];
                    if(p!=nullptr){
                        //释放出未用的块
                        free_list[index] = p->next;
                        start_free = (char*) p;
                        end_free = start_free + s;
                        return chunk_alloc(size, objs);
                    }
                }
                //分配空间失败
                end_free = nullptr;
            }
            heap_size+=bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(size, objs);
        }
    }
}

