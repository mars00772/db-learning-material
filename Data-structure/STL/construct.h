//
// Created by AlexanderChiu on 2020/5/24.
//

#ifndef STL_CONSTRUCT_H
#define STL_CONSTRUCT_H

namespace MYSTL {

    template <class T1, class T2>
    inline void construct(T1 *p, const T2& value){
        new (p) T1(value);
    };

    template <class T>
    inline void destroy(T* p){
        p->~T();
    }

}



#endif //STL_CONSTRUCT_H
