//
// Created by AlexanderChiu on 2020/5/21.
//

#ifndef STL_ALLOCATOR_H
#define STL_ALLOCATOR_H

#include "alloc.h"

namespace MYSTL{
    template <class T, class Alloc = alloc>
    class allocator {

        //n表示分配的T的个数
        static T*allocate(size_t n){
            return n==0?0:(T*)Alloc::allocate(n* sizeof(T));
        }

        static T*allocate(void){
            return (T*)Alloc::allocate(sizeof(T));
        }

        static void deallocate(T*p , size_t n){
            if(n!=0){
                Alloc::deallocate(p, n * sizeof(T));
            }
        }

        static void deallocate(T*p){
            Alloc::deallocate(p, sizeof(T));
        }
    };

}


#endif //STL_ALLOCATOR_H
