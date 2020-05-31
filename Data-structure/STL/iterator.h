//
// Created by AlexanderChiu on 2020/5/21.
//

#ifndef STL_ITERATOR_H
#define STL_ITERATOR_H

#include <cstddef>

namespace MYSTL {

    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_itetator_tag : public forward_iterator_tag{};
    struct random_access_iterator_tag : public bidirectional_itetator_tag{};

    template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
    struct iterator {
        typedef Category iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef Pointer pointer;
        typedef Reference reference;
    };




}



#endif //STL_ITERATOR_H
