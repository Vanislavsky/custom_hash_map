//
// Created by Сергей Ваниславский on 29.11.2020.
//

#ifndef HASHMAP_HASH_MAP_H
#define HASHMAP_HASH_MAP_H


#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <type_traits>
#include <iostream>

namespace fefu
{

    template <typename K>
    struct FirstKeyHash {
        long operator()(const K& key, size_t table_size) const {
            return key % table_size;
        }
    };

    template <typename K>
    struct SecondKeyHash {
        long operator()(const K& key, size_t table_size) const {
            return ((key-1) % table_size + 1);
        }
    };

    template <class _Ty = void>
    struct equal_to {
        constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const {
            return _Left == _Right;
        }
    };

    template<typename T>
    class allocator {
    public:
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = typename std::add_lvalue_reference<T>::type;
        using const_reference = typename std::add_lvalue_reference<const T>::type;
        using value_type = T;

        pointer allocate(size_type n) {
            return static_cast<pointer>( ::operator new(n*sizeof(T)));
        }

        void deallocate(pointer p, size_type n) noexcept {
            ::operator delete(static_cast<void*>(p), n * sizeof(T));

        }
    };

    template<typename ValueType>
    class hash_map_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using reference = ValueType&;
        using pointer = ValueType*;

        hash_map_iterator() noexcept {}

        hash_map_iterator(pointer point, bool* deleted, size_t cur_index, size_t size): point(point) {
            this->deleted = new bool[size];
            for(int i = 0;i < size; i++)
                this->deleted[i] = deleted[i];
            this->cur_index = cur_index;
            this->size = size;
        }

        hash_map_iterator(const value_type& point, bool* deleted, size_t size) noexcept {
            this->point = point;
            this->deleted = new bool[size];
            for(int i = 0;i < size; i++)
                this->deleted[i] = deleted[i];
            this->cur_index = cur_index;
            this-size = size;
        }

        hash_map_iterator(const hash_map_iterator& other) noexcept {
            point = other.point;
            this->deleted = new bool[other.size];
            for(int i = 0; i < other.size; i++)
                this->deleted[i] = other.deleted[i];
            this->cur_index = other.cur_index;
            this->size = other.size;
        }


        reference operator*() const {
            return *point;
        }
        pointer operator->() const {
            return point;
        }

        // prefix ++
        hash_map_iterator& operator++() {
            point++;
            cur_index++;
            while (cur_index < size) {
                if(*point != nullptr && deleted[cur_index] == false)
                    return *this;
                else {
                    point++;
                    cur_index++;
                }
            }
            return *this;
        }
        // postfix ++
        hash_map_iterator operator++(int) {
            return ++(*this);
        }

        friend bool operator==(const hash_map_iterator<ValueType>& l_point, const hash_map_iterator<ValueType>& r_point) {
            return l_point.point == r_point.point;
        }
        friend bool operator!=(const hash_map_iterator<ValueType>& l_point, const hash_map_iterator<ValueType>& r_point) {
            return !(l_point == r_point);
        }

    private:
        pointer point;
        bool* deleted;
        size_t cur_index;
        size_t size;
    };


    template<typename ValueType>
    class hash_map_const_iterator {
// Shouldn't give non const references on value
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using reference = const ValueType&;
        using pointer = const ValueType*;

        hash_map_const_iterator() noexcept {}
        hash_map_const_iterator(const hash_map_const_iterator& other) noexcept {
            point = other.point;
        }

        hash_map_const_iterator(pointer point, bool* deleted, size_t cur_index, size_t size): point(point) {
                this->deleted = new bool[size];
                for(int i = 0;i < size; i++)
                    this->deleted[i] = deleted[i];
                this->cur_index = cur_index;
                this->size = size;
        }
        //ash_map_const_iterator(const hash_map_iterator<ValueType>& other) noexcept;

        hash_map_const_iterator(const value_type& point, bool* deleted, size_t cur_index, size_t size) noexcept {
            this->point = point;
            this->deleted = new bool[size];
            for(int i = 0;i < size; i++)
                this->deleted[i] = deleted[i];
            this->cur_index = cur_index;
            this-size = size;
        }

        reference operator*() const {
            return *point;
        }
        pointer operator->() const {
            return **point;
        }

        // prefix ++
        hash_map_const_iterator& operator++() {
            point++;
            cur_index++;
            while (cur_index < size) {
                if(*point != nullptr && deleted[cur_index] == false)
                    return *this;
                else {
                    point++;
                    cur_index++;
                }
            }
            return *this;
        }
        // postfix ++
        hash_map_const_iterator operator++(int) {
            return ++(*this);
        }

        friend bool operator==(const hash_map_const_iterator<ValueType>& l_point, const hash_map_const_iterator<ValueType>& r_point) {
            return l_point.point == r_point.point;
        }
        friend bool operator!=(const hash_map_const_iterator<ValueType>& l_point, const hash_map_const_iterator<ValueType>& r_point) {
            return !(l_point == r_point);
        }

    private:
        const ValueType* point;
        bool* deleted;
        size_t cur_index;
        size_t size;
    };


    template<typename K, typename T,
            class FirstHash = FirstKeyHash<K>,
            class SecondHash = SecondKeyHash<K>,
            typename Pred = std::equal_to<K>,
            typename Alloc = allocator<std::pair<const K, T>*>>
    class hash_map
    {
    public:
        using key_type = K;
        using mapped_type = T;
        using allocator_type = Alloc;
        using value_type = std::pair<const key_type, mapped_type>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = hash_map_iterator<value_type*>;
        using const_iterator = hash_map_const_iterator<value_type*>;
        using size_type = std::size_t;

    private:
        size_type SIZE = 37;
        size_type NOT_NULL_SIZE = 0;
        float LOAD_FACTOR = 0.7;
        value_type **table;
        bool *deleted;
        FirstHash firstHash;
        SecondHash secondHash;
        Pred key_equal;
        allocator_type table_allocator;
        allocator<bool> deleted_allocator;

        void filling_standart_value() {
            for(int i = 0; i < SIZE; i++) {
                table[i] = nullptr;
                deleted[i] = false;
            }
        }

        template<typename Key, typename Value>
        std::pair<iterator, bool> insert_value_helper(Key&& key, Value&& value) {
            auto x = firstHash(key, SIZE);
            auto y = secondHash(key, SIZE);
            for (int i = 1; i < SIZE; i++) {
                if (table[x] == nullptr || deleted[x]) {
                    table[x] = new value_type(key, value);
                    deleted[x] = false;
                    NOT_NULL_SIZE++;
                    return {iterator(table + x, deleted, x, SIZE), true};
                } else if (key_equal(table[x]->first, key)) {
                    return {iterator(table + x, deleted, x, SIZE), false};
                }
                x = (x + i * y) % SIZE;
            }
        }

        template<typename Key, typename Value>
        std::pair<iterator, bool> insert_or_assign_value_helper(Key&& key, Value&& value) {
            auto x = firstHash(key, SIZE);
            auto y = secondHash(key, SIZE);
            for (int i = 1; i < SIZE; i++) {
                if (table[x] == nullptr || deleted[x]) {
                    table[x] = new value_type(key, value);
                    deleted[x] = false;
                    NOT_NULL_SIZE++;
                    return {iterator(table + x, deleted, x, SIZE), true};
                } else if (key_equal(table[x]->first, key)) {
                    table[x]->second = value;
                    return { iterator(table + x, deleted, x, SIZE), false};
                }
                x = (x + i * y) % SIZE;
            }
        }

        iterator find_value_helper(const key_type& key) {
            auto x = firstHash(key, SIZE);
            auto y = secondHash(key, SIZE);
            for (int i = 1; i < SIZE; i++) {
                if (table[x] != nullptr) {
                    if (key_equal(table[x]->first, key) && !deleted[x])
                        return iterator(table + x, deleted, x, SIZE);
                } else {
                    return end();
                }
                x = (x + i * y) % SIZE;
            }
            return end();
        }

        template<typename Key>
        mapped_type& operator_helper(Key&& key) {
            if(contains(key))
                return (*find(key))->second;
            else
                return (*insert({key, 0}).first)->second;
        }

    public:
        /// Default constructor.
        hash_map() {
            table = table_allocator.allocate(SIZE);
            deleted = deleted_allocator.allocate(SIZE);
            for(int i = 0; i < SIZE; i++) {
                table[i] = nullptr;
                deleted[i] = false;
            }
        }

        ~hash_map() {
            for (int i = 0; i < SIZE; i++)
                if(table) {
                    if (table[i])
                        delete table[i];
                }
            if(table)
                table_allocator.deallocate(table,SIZE);
            if(deleted)
                deleted_allocator.deallocate(deleted, SIZE);
        }
        /**
         *  @brief  Default constructor creates no elements.
         *  @param n  Minimal initial number of buckets.
         */
        explicit hash_map(size_type n) {
            SIZE = n;
            table = table_allocator.allocate(SIZE);
            deleted = deleted_allocator.allocate(SIZE);
            filling_standart_value();
        }

        /**
         *  @brief  Builds an %hash_map from a range.
         *  @param  first  An input iterator.
         *  @param  last  An input iterator.
         *  @param  n  Minimal initial number of buckets.
         *
         *  Create an %hash_map consisting of copies of the elements from
         *  [first,last).  This is linear in N (where N is
         *  distance(first,last)).
         */
        template<typename InputIterator>
        hash_map(InputIterator first, InputIterator last,
                 size_type n = 0) {
            SIZE = n;
            table = table_allocator.allocate(SIZE);
            deleted = deleted_allocator.allocate(SIZE);
            filling_standart_value();

            for(auto it = first; it != last; it++) {
                insert({(*it)->key, (*it)->value});
            }
        }

        /// Copy constructor.
        hash_map(const hash_map& other) {
            SIZE = other.SIZE;
            NOT_NULL_SIZE = other.NOT_NULL_SIZE;
            table = table_allocator.allocate(SIZE);
            filling_standart_value();

            for(int i = 0; i < other.SIZE; i++) {
                if(table[i])
                    insert({table[i]->first, table[i]->second});
                deleted[i] = other.deleted[i];
            }

            firstHash = other.firstHash;
            secondHash = other.secondHash;
            key_equal = other.key_equal;
        }

        /// Move constructor.
        hash_map(hash_map&& other): table(std::move(other.table)), deleted(std::move(other.deleted)),
        firstHash(std::move(other.firstHash)), secondHash(std::move(other.secondHash)),
        key_equal(std::move(other.key_equal)), SIZE(other.SIZE), NOT_NULL_SIZE(other.NOT_NULL_SIZE),
        table_allocator(std::move(other.table_allocator)), deleted_allocator(std::move(other.deleted_allocator)){
            other.table = nullptr;
            other.deleted = nullptr;
        }

        /**
         *  @brief Creates an %hash_map with no elements.
         *  @param a An allocator object.
         */
        explicit hash_map(const allocator_type& a) {
            table_allocator = a;
            table = table_allocator.allocate(SIZE);
            deleted = deleted_allocator.allocate(SIZE);
            filling_standart_value();
        }

        /*
        *  @brief Copy constructor with allocator argument.
        * @param  uset  Input %hash_map to copy.
        * @param  a  An allocator object.
        */
        hash_map(const hash_map& other,
                 const allocator_type& a) {
            table_allocator = a;
            SIZE = other.SIZE;
            NOT_NULL_SIZE = other.NOT_NULL_SIZE;
            table = table_allocator.allocate(SIZE);
            deleted = deleted_allocator.allocate(SIZE);
            filling_standart_value();

            for(int i = 0; i < other.SIZE; i++) {
                if(table[i])
                    insert({table[i]->first, table[i]->second});
                deleted[i] = other.deleted[i];
            }

            firstHash = other.firstHash;
            secondHash = other.secondHash;
            key_equal = other.key_equal;
        }

        /*
        *  @brief  Move constructor with allocator argument.
        *  @param  uset Input %hash_map to move.
        *  @param  a    An allocator object.
        */
        hash_map(hash_map&& other,
                 const allocator_type& a) : table(std::move(other.table)), deleted(std::move(other.deleted)),
        firstHash(std::move(other.firstHash)), secondHash(std::move(other.secondHash)),
        key_equal(std::move(other.key_equal)), SIZE(other.SIZE), NOT_NULL_SIZE(other.NOT_NULL_SIZE), table_allocator(a){
            table = nullptr;
            deleted = nullptr;
        }

        /**
         *  @brief  Builds an %hash_map from an initializer_list.
         *  @param  l  An initializer_list.
         *  @param n  Minimal initial number of buckets.
         *
         *  Create an %hash_map consisting of copies of the elements in the
         *  list. This is linear in N (where N is @a l.size()).
         */
        hash_map(std::initializer_list<value_type> l,
                 size_type n = 0) {
            SIZE = n;
            table = table_allocator.allocate(SIZE);
            deleted = deleted_allocator.allocate(SIZE);
            filling_standart_value();

            for(int i = 0; i < l.size(); i++)
                insert({l[i].first, l[i].second});
        }

        /// Copy assignment operator.
        hash_map& operator=(const hash_map& other) {
            SIZE = other.SIZE;
            NOT_NULL_SIZE = other.NOT_NULL_SIZE;
            table = table_allocator.allocate(SIZE);
            deleted = deleted_allocator.allocate(SIZE);
            filling_standart_value();

            for(int i = 0; i < other.SIZE; i++) {
                if(table[i])
                    insert({table[i]->first, table[i]->second});
                deleted[i] = other.deleted[i];
            }

            firstHash = other.firstHash;
            secondHash = other.secondHash;
            key_equal = other.key_equal;

            return *this;
        }

        /// Move assignment operator.
        hash_map& operator=(hash_map&& other) {
            table = std::move(other.table);
            deleted = std::move(other.deleted);
            firstHash = std::move(other.firstHash);
            secondHash = std::move(other.secondHash);
            key_equal = std::move(other.key_equal);
            table_allocator = std::move(other.table_allocator);
            deleted_allocator = std::move(other.deleted_allocator);

            SIZE = other.SIZE;
            NOT_NULL_SIZE = other.NOT_NULL_SIZE;

            table = nullptr;
            deleted = nullptr;

            return *this;
        }

        /**
         *  @brief  %hash_map list assignment operator.
         *  @param  l  An initializer_list.
         *
         *  This function fills an %hash_map with copies of the elements in
         *  the initializer list @a l.
         *
         *  Note that the assignment completely changes the %hash_map and
         *  that the resulting %hash_map's size is the same as the number
         *  of elements assigned.
         */
        hash_map& operator=(std::initializer_list<value_type> l) {
            for(int i = 0; i < l.size(); i++)
                insert({l[i].first, l[i].second});
            return *this;
        }

        ///  Returns the allocator object used by the %hash_map.
        allocator_type get_allocator() const noexcept {
            return table_allocator;
        }

        // size and capacity:

        ///  Returns true if the %hash_map is empty.
        bool empty() const noexcept {
            return NOT_NULL_SIZE == 0;
        }

        ///  Returns the size of the %hash_map.
        size_type size() const noexcept {
            return NOT_NULL_SIZE;
        }

        ///  Returns the maximum size of the %hash_map.
        size_type max_size() const noexcept {
            return SIZE;
        }

        // iterators.

        /**
         *  Returns a read/write iterator that points to the first element in the
         *  %hash_map.
         */
        iterator begin() noexcept {
            for(int i = 0; i < SIZE; i++) {
                if(table[i] != nullptr && deleted[i] == false) {
                    return iterator(table + i, deleted, i, SIZE);
                }
            }
            return end();
        }

        //@{
        /**
         *  Returns a read-only (constant) iterator that points to the first
         *  element in the %hash_map.
         */
        const_iterator begin() const noexcept {
            return cbegin();
        }

        const_iterator cbegin() const noexcept {
            for(int i = 0; i < SIZE; i++) {
                if(table[i] != nullptr && deleted[i] == false)
                    return const_iterator(table + i, deleted, i, SIZE);
            }
            return cend();
        }

        /**
         *  Returns a read/write iterator that points one past the last element in
         *  the %hash_map.
         */
        iterator end() noexcept {
            return iterator(table + SIZE, deleted, SIZE, SIZE);
        }

        //@{
        /**
         *  Returns a read-only (constant) iterator that points one past the last
         *  element in the %hash_map.
         */
        const_iterator end() const noexcept {
            return cend();
        }

        const_iterator cend() const noexcept {
            return const_iterator(table + SIZE, deleted, SIZE, SIZE);
        }
        //@}

        // modifiers.

        /**
         *  @brief Attempts to build and insert a std::pair into the
         *  %hash_map.
         *
         *  @param args  Arguments used to generate a new pair instance (see
         *	        std::piecewise_contruct for passing arguments to each
        *	        part of the pair constructor).
        *
        *  @return  A pair, of which the first element is an iterator that points
        *           to the possibly inserted pair, and the second is a bool that
        *           is true if the pair was actually inserted.
        *
        *  This function attempts to build and insert a (key, value) %pair into
        *  the %hash_map.
        *  An %hash_map relies on unique keys and thus a %pair is only
        *  inserted if its first element (the key) is not already present in the
        *  %hash_map.
        *
        *  Insertion requires amortized constant time.
        */
        template<typename... _Args>
        std::pair<iterator, bool> emplace(_Args&&... args) {
            return insert(value_type(std::forward<_Args>(args)...));
        }

        /**
         *  @brief Attempts to build and insert a std::pair into the
         *  %hash_map.
         *
         *  @param k    Key to use for finding a possibly existing pair in
         *                the hash_map.
         *  @param args  Arguments used to generate the .second for a
         *                new pair instance.
         *
         *  @return  A pair, of which the first element is an iterator that points
         *           to the possibly inserted pair, and the second is a bool that
         *           is true if the pair was actually inserted.
         *
         *  This function attempts to build and insert a (key, value) %pair into
         *  the %hash_map.
         *  An %hash_map relies on unique keys and thus a %pair is only
         *  inserted if its first element (the key) is not already present in the
         *  %hash_map.
         *  If a %pair is not inserted, this function has no effect.
         *
         *  Insertion requires amortized constant time.
         */
        template <typename... _Args>
        std::pair<iterator, bool> try_emplace(const key_type& k, _Args&&... args){
            return insert(value_type(std::piecewise_construct, std::forward_as_tuple(k), std::forward_as_tuple(std::forward<_Args>(args)...)));
        }

        // move-capable overload
        template <typename... _Args>
        std::pair<iterator, bool> try_emplace(key_type&& k, _Args&&... args) {
            return insert(value_type(std::piecewise_construct, std::forward_as_tuple(std::move(k)), std::forward_as_tuple(std::forward<_Args>(args)...)));
        }

        //@{
        /**
         *  @brief Attempts to insert a std::pair into the %hash_map.
         *  @param x Pair to be inserted (see std::make_pair for easy
         *	     creation of pairs).
        *
        *  @return  A pair, of which the first element is an iterator that
        *           points to the possibly inserted pair, and the second is
        *           a bool that is true if the pair was actually inserted.
        *
        *  This function attempts to insert a (key, value) %pair into the
        *  %hash_map. An %hash_map relies on unique keys and thus a
        *  %pair is only inserted if its first element (the key) is not already
        *  present in the %hash_map.
        *
        *  Insertion requires amortized constant time.
        */

        std::pair<iterator, bool> insert(const value_type& value)  {
            if(SIZE == 0)
                rehash(32);
            if(NOT_NULL_SIZE / SIZE >= LOAD_FACTOR)
                rehash(2 * SIZE);

            return insert_value_helper(value.first, value.second);
        }

        std::pair<iterator, bool> insert(value_type&& value) {
            if(SIZE == 0)
                rehash(32);
            if((float)NOT_NULL_SIZE / (float)SIZE >= LOAD_FACTOR)
                rehash(2 * SIZE);

            return insert_value_helper(std::move(value.first), std::move(value.second));
        }

        //@}

        /**
         *  @brief A template function that attempts to insert a range of
         *  elements.
         *  @param  first  Iterator pointing to the start of the range to be
         *                   inserted.
         *  @param  last  Iterator pointing to the end of the range.
         *
         *  Complexity similar to that of the range constructor.
         */
        template<typename _InputIterator>
        void insert(_InputIterator first, _InputIterator last) {
            for(auto it = first; it != last; it++) {
                insert({it->first, it->second });
            }
        }

        /**
         *  @brief Attempts to insert a list of elements into the %hash_map.
         *  @param  l  A std::initializer_list<value_type> of elements
         *               to be inserted.
         *
         *  Complexity similar to that of the range constructor.
         */
        void insert(std::initializer_list<value_type> l) {
            for(int i = 0; i < l.size(); i++)
                insert(l[i]);
        }


        /**
         *  @brief Attempts to insert a std::pair into the %hash_map.
         *  @param k    Key to use for finding a possibly existing pair in
         *                the map.
         *  @param obj  Argument used to generate the .second for a pair
         *                instance.
         *
         *  @return  A pair, of which the first element is an iterator that
         *           points to the possibly inserted pair, and the second is
         *           a bool that is true if the pair was actually inserted.
         *
         *  This function attempts to insert a (key, value) %pair into the
         *  %hash_map. An %hash_map relies on unique keys and thus a
         *  %pair is only inserted if its first element (the key) is not already
         *  present in the %hash_map.
         *  If the %pair was already in the %hash_map, the .second of
         *  the %pair is assigned from obj.
         *
         *  Insertion requires amortized constant time.
         */

        template <typename _Obj>
        std::pair<iterator, bool> insert_or_assign(const key_type& key, _Obj&& obj) {
            if(SIZE == 0)
                rehash(32);
            if(NOT_NULL_SIZE / SIZE >= LOAD_FACTOR)
                rehash(2 * SIZE);

            return insert_or_assign_value_helper(key, obj);
        }

        // move-capable overload
        template <typename _Obj>
        std::pair<iterator, bool> insert_or_assign(key_type&& key, _Obj&& obj) {
            if(SIZE == 0)
                rehash(32);
            if(NOT_NULL_SIZE / SIZE >= LOAD_FACTOR)
                rehash(2 * SIZE);

            return insert_or_assign_value_helper(std::move(key), std::move(obj));
        }

        //@{
        /**
         *  @brief Erases an element from an %hash_map.
         *  @param  position  An iterator pointing to the element to be erased.
         *  @return An iterator pointing to the element immediately following
         *          @a position prior to the element being erased. If no such
         *          element exists, end() is returned.
         *
         *  This function erases an element, pointed to by the given iterator,
         *  from an %hash_map.
         *  Note that this function only erases the element, and that if the
         *  element is itself a pointer, the pointed-to memory is not touched in
         *  any way.  Managing the pointer is the user's responsibility.
         */
        iterator erase(const_iterator position) {
            auto key = (*position)->first;
            erase(key);
            return position;
        }

        // LWG 2059.
        iterator erase(iterator position) {
            auto key = (*position)->first;
            erase(key);
            return position;
        }
        //@}

        /**
         *  @brief Erases elements according to the provided key.
         *  @param  x  Key of element to be erased.
         *  @return  The number of elements erased.
         *
         *  This function erases all the elements located by the given key from
         *  an %hash_map. For an %hash_map the result of this function
         *  can only be 0 (not present) or 1 (present).
         *  Note that this function only erases the element, and that if the
         *  element is itself a pointer, the pointed-to memory is not touched in
         *  any way.  Managing the pointer is the user's responsibility.
         */
        size_type erase(const key_type& key) {
            auto x = firstHash(key, SIZE);
            auto y = secondHash(key, SIZE);
            for (int i = 1; i < SIZE; i++) {
                if (table[x] != nullptr) {
                    if (key_equal(table[x]->first, key) && !deleted[x]) {
                        deleted[x] = true;
                        NOT_NULL_SIZE--;
                        return 1;
                    }
                } else {
                    return 0;
                }
                x = (x + i * y) % SIZE;
            }
            return 0;
        }

        /**
         *  @brief Erases a [first,last) range of elements from an
         *  %hash_map.
         *  @param  first  Iterator pointing to the start of the range to be
         *                  erased.
         *  @param last  Iterator pointing to the end of the range to
         *                be erased.
         *  @return The iterator @a last.
         *
         *  This function erases a sequence of elements from an %hash_map.
         *  Note that this function only erases the elements, and that if
         *  the element is itself a pointer, the pointed-to memory is not touched
         *  in any way.  Managing the pointer is the user's responsibility.
         */
        iterator erase(const_iterator first, const_iterator last) {
            for(const_iterator it = first; it != last; it) {
                erase(it);
            }

            return last;
        }

        /**
         *  Erases all elements in an %hash_map.
         *  Note that this function only erases the elements, and that if the
         *  elements themselves are pointers, the pointed-to memory is not touched
         *  in any way.  Managing the pointer is the user's responsibility.
         */
        void clear() noexcept {
            for (int i = 0; i < SIZE; i++)
                if (table[i])
                    delete table[i];
            table_allocator.deallocate(table,SIZE);
            deleted_allocator.deallocate(deleted, SIZE);

            SIZE = 37;
            NOT_NULL_SIZE = 0;
            table = table_allocator.allocate(SIZE);
            deleted = deleted_allocator.allocate(SIZE);
            for(int i = 0; i < SIZE; i++) {
                table[i] = nullptr;
                deleted[i] = false;
            }
        }

        /**
         *  @brief  Swaps data with another %hash_map.
         *  @param  x  An %hash_map of the same element and allocator
         *  types.
         *
         *  This exchanges the elements between two %hash_map in constant
         *  time.
         *  Note that the global std::swap() function is specialized such that
         *  std::swap(m1,m2) will feed to this function.
         */
        void swap(hash_map& x){
            std::swap(table, x.table);
            std::swap(deleted, x.deleted);
            std::swap(SIZE, x.SIZE);
            std::swap(NOT_NULL_SIZE, x.NOT_NULL_SIZE);
            std::swap(LOAD_FACTOR, x.LOAD_FACTOR);
        }

        template<class FirstHash2 = FirstKeyHash<K>,
                class SecondHash2 = SecondKeyHash<K>,
                typename Pred2 = std::equal_to<K>,
                typename Alloc2 = allocator_type>
        void merge(hash_map<K, T, FirstHash2, SecondHash2, Pred2, Alloc>& source) {
            if(source.max_size() > SIZE)
                rehash(source.max_size());

            for(int i = 0; i < source.max_size(); i++) {
                if(source.table[i]) {
                    insert({source.table[i]->first, source.table[i]->second});
                }
            }
        }

        template<class FirstHash2 = FirstKeyHash<K>,
                class SecondHash2 = SecondKeyHash<K>,
                typename Pred2 = std::equal_to<K>,
                typename Alloc2 = allocator_type>
        void merge(hash_map<K, T, FirstHash2, SecondHash2, Pred2, Alloc>&& source) {
            if(source.max_size() > SIZE)
                rehash(source.max_size());

            for(int i = 0; i < source.max_size(); i++) {
                if(source.table[i]) {
                    insert({source.table[i]->first, std::move(source.table[i]->second)});
                }
            }
        }

        // observers.

        ///  Returns the hash functor object with which the %hash_map was
        ///  constructed.
        FirstHash first_hash_function() const {
            return firstHash;
        }

        SecondHash second_hash_function() const {
            return secondHash;
        }

        ///  Returns the key comparison object with which the %hash_map was
        ///  constructed.
        Pred key_eq() const {
            return key_equal;
        }

        // lookup.

        //@{
        /**
         *  @brief Tries to locate an element in an %hash_map.
         *  @param  x  Key to be located.
         *  @return  Iterator pointing to sought-after element, or end() if not
         *           found.
         *
         *  This function takes a key and tries to locate the element with which
         *  the key matches.  If successful the function returns an iterator
         *  pointing to the sought after element.  If unsuccessful it returns the
         *  past-the-end ( @c end() ) iterator.
         */

        iterator find(const key_type& key) {
            return find_value_helper(key);
        }

        const_iterator find(const key_type& key) const {
            return find_value_helper(key);
        }
        //@}

        /**
         *  @brief  Finds the number of elements.
         *  @param  x  Key to count.
         *  @return  Number of elements with specified key.
         *
         *  This function only makes sense for %unordered_multimap; for
         *  %hash_map the result will either be 0 (not present) or 1
         *  (present).
         */
        size_type count(const key_type& x) const {
            if(contains(x))
                return 1;
            return 0;
        }

        /**
         *  @brief  Finds whether an element with the given key exists.
         *  @param  x  Key of elements to be located.
         *  @return  True if there is any element with the specified key.
         */
        bool contains(const key_type& key) const {
            auto x = firstHash(key, SIZE);
            auto y = secondHash(key, SIZE);
            for (int i = 1; i < SIZE; i++) {
                if (table[x] != nullptr) {
                    if (key_equal(table[x]->first, key) && !deleted[x])
                        return true;
                } else {
                    return false;
                }
                x = (x + i * y) % SIZE;
            }
            return false;
        }

        //@{
        /**
         *  @brief  Subscript ( @c [] ) access to %hash_map data.
         *  @param  k  The key for which data should be retrieved.
         *  @return  A reference to the data of the (key,data) %pair.
         *
         *  Allows for easy lookup with the subscript ( @c [] )operator.  Returns
         *  data associated with the key specified in subscript.  If the key does
         *  not exist, a pair with that key is created using default values, which
         *  is then returned.
         *
         *  Lookup requires constant time.
         */

        mapped_type& operator[](const key_type& k) {
            return operator_helper(k);
        }

        mapped_type& operator[](key_type&& k) {
            return operator_helper(std::move(k));
        }
        //@}

        //@{
        /**
         *  @brief  Access to %hash_map data.
         *  @param  k  The key for which data should be retrieved.
         *  @return  A reference to the data whose key is equal to @a k, if
         *           such a data is present in the %hash_map.
         *  @throw  std::out_of_range  If no such data is present.
         */
        mapped_type& at(const key_type& k) {
            if(contains(k))
                return (*find(k))->second;
            else
                throw std::out_of_range("key not found");
        }

        const mapped_type& at(const key_type& k) const {
            return at(k);
        }
        //@}

        // bucket interface.

        /// Returns the number of buckets of the %hash_map.
        size_type bucket_count() const noexcept {
            return NOT_NULL_SIZE;
        }

        /*
        * @brief  Returns the bucket index of a given element.
        * @param  _K  A key instance.
        * @return  The key bucket index.
        */
        size_type bucket(const key_type& _K) const {
            if(contains(_K)) {
                return find(_K).operator->() - begin().operator->();
            } else
                return 0;
        }

        // hash policy.

        /// Returns the average number of elements per bucket.
        float load_factor() const noexcept {
            return NOT_NULL_SIZE / SIZE;
        }

        /// Returns a positive number that the %hash_map tries to keep the
        /// load factor less than or equal to.
        float max_load_factor() const noexcept {
            return LOAD_FACTOR;
        }

        /**
         *  @brief  Change the %hash_map maximum load factor.
         *  @param  z The new maximum load factor.
         */
        void max_load_factor(float z) {
            LOAD_FACTOR = z;
        }

        /**
         *  @brief  May rehash the %hash_map.
         *  @param  n The new number of buckets.
         *
         *  Rehash will occur only if the new number of buckets respect the
         *  %hash_map maximum load factor.
         */
        void rehash(size_type n) {
            size_type PAST_SIZE = SIZE;
            SIZE = n;
            value_type **temp_table = table_allocator.allocate(SIZE);
            for(int i = 0; i < SIZE; i++)
                temp_table[i] = nullptr;
            NOT_NULL_SIZE = 0;
            std::swap(temp_table, table);
            for(int i = 0; i < PAST_SIZE; i++) {
                if(temp_table[i] != nullptr && !deleted[i]) {
                    insert({temp_table[i]->first, temp_table[i]->second});
                }
            }

            deleted = deleted_allocator.allocate(SIZE);
            for(int i = 0; i < SIZE; i++)
                deleted[i] = false;

            for(int i = 0; i < PAST_SIZE; i++) {
                if(temp_table[i])
                    delete temp_table[i];
            }

            table_allocator.deallocate(temp_table,PAST_SIZE);

        }

        /**
         *  @brief  Prepare the %hash_map for a specified number of
         *          elements.
         *  @param  n Number of elements required.
         *
         *  Same as rehash(ceil(n / max_load_factor())).
         */
        void reserve(size_type n) {
            rehash(ceil((float)n / (float)max_load_factor()));
        }
    };

} // namespace fefu



#endif //HASHMAP_HASH_MAP_H
