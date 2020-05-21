//
// Created by AlexanderChiu on 2020/5/21.
// ref: STL源码解读 侯捷
//

#ifndef STL_ALLOC_H
#define STL_ALLOC_H

#include <cstddef>
namespace MYSTL {

    class alloc {
    private:
        /***
         * 空闲链表有16个桶，每个桶分别维护8,16,24....128bytes的字节块
         */
        enum {
            ALIGN = 8
        };
        enum {
            MAX_BYTES = 128
        };
        enum {
            FREE_LISTS = MAX_BYTES / ALIGN
        };
        enum {
            NOBJS = 20
        }; //每次增加的节点个数
    private:
        static size_t ROUND_UP(size_t bytes) {
            return ((bytes + ALIGN - 1) & ~(ALIGN - 1));
        }

    private:
        union obj {
            union obj *next;
            char client_data[1];
        };

        static obj *volatile free_list[FREE_LISTS];
        //根据bytes大小选择使用第n号free_list

        static size_t free_list_index(size_t bytes) {
            return ((bytes + ALIGN - 1) / ALIGN - 1);
        }

        static void *refill(size_t n);

        //分配nobjs个大小为size的空间
        static char *chunk_alloc(size_t size, int &objs);

        static char *start_free; //内存池初始位置
        static char *end_free;
        static size_t heap_size;

    public:
        static void *allocate(size_t n);

        static void deallocate(void *p, size_t n);

        static void *reallocate(void *p, size_t old_size, size_t new_size);
    };
}

#endif //STL_ALLOC_H
