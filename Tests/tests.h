//
// Created by Сергей Ваниславский on 04.12.2020.
//

#ifndef CUSTOM_HASH_MAP_TESTS_H
#define CUSTOM_HASH_MAP_TESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../HashMap/hash_map.h"

TEST (HashMapTesting, InsertElements) {
    fefu::hash_map<int, char> a;
    a.insert({10, 's'});
    a.insert({11, 'd'});
    a.insert({12, 'w'});
    a.insert({21, 'a'});
    a.insert({55, 'g'});
    a.insert({4, 'l'});

    //hash_map contains this elements
    ASSERT_TRUE(a.contains(10));
    ASSERT_TRUE(a.contains(11));
    ASSERT_TRUE(a.contains(12));
    ASSERT_TRUE(a.contains(21));
    ASSERT_TRUE(a.contains(55));
    ASSERT_TRUE(a.contains(4));

    // hash_map doesn't contain this elements
    ASSERT_TRUE(!a.contains(1));
    ASSERT_TRUE(!a.contains(100));
    ASSERT_TRUE(!a.contains(30));

    //hash_map already contains these keys
    ASSERT_TRUE(a.insert({10, 's'}).second == false);
    ASSERT_TRUE(a.insert({10, 's'}).second == false);
    ASSERT_TRUE(a.insert({10, 's'}).second == false);

}



//TEST (HashMapTesting, test) {
//    fefu::hash_map<int, int> a;
//    for(int i = 0; i < 500; i++) {
//        a.insert({i * 3, i * 4});
//    }
//
////    for(int i = 0; i < 100; i++) {
////        a.contains(i * 3);
////    }
//}



TEST (HashMapTesting, InsertOrAssignElements) {
    fefu::hash_map<int, char> a;
    a.insert_or_assign(10, 's');
    a.insert_or_assign(11, 'a');
    a.insert_or_assign(12, 'f');
    a.insert_or_assign(12, 'y');

    //hash_map contains this elements
    ASSERT_TRUE(a.contains(10));
    ASSERT_TRUE(a.contains(11));
    ASSERT_TRUE(a.contains(12));

    //hash_map contains element with value 'y'
    ASSERT_TRUE((*a.find(12))->second == 'y');

}

TEST (HashMapTesting, EmplaceElements) {
    fefu::hash_map<int, char> a;
   a.emplace(10,'s');
    a.emplace(20,'s');
   a.emplace(std::piecewise_construct,
             std::forward_as_tuple(30),
             std::forward_as_tuple('w'));


    ASSERT_TRUE(a.contains(10));
    ASSERT_TRUE(a.contains(20));
    ASSERT_TRUE(a.contains(30));


}

TEST (HashMapTesting, EraseElements) {
    fefu::hash_map<int, char> a;
    a.insert({10, 's'});
    a.insert({11, 'd'});
    a.insert({12, 'w'});
    a.insert({21, 'a'});
    a.insert({55, 'g'});
    a.insert({4, 'l'});

    //hash_map contains this elements
    ASSERT_TRUE(a.contains(10));
    ASSERT_TRUE(a.contains(11));
    ASSERT_TRUE(a.contains(12));
    ASSERT_TRUE(a.contains(21));
    ASSERT_TRUE(a.contains(55));
    ASSERT_TRUE(a.contains(4));

    a.erase(10);
    a.erase(11);
    a.erase(12);
    a.erase(4);

    //hash_map no longer contains these elements
    ASSERT_TRUE(!a.contains(10));
    ASSERT_TRUE(!a.contains(11));
    ASSERT_TRUE(!a.contains(12));
    ASSERT_TRUE(!a.contains(4));

    //hash_map still contains these elements
    ASSERT_TRUE(a.contains(21));
    ASSERT_TRUE(a.contains(55));

}

TEST (HashMapTesting, EraseElementsByPosition) {
    fefu::hash_map<int, char> a;
    a.insert({0, 's'});
    a.insert({1, 'w'});
    a.insert({2, 'w'});

    ASSERT_TRUE(a.contains(0));
    ASSERT_TRUE(a.contains(1));
    ASSERT_TRUE(a.contains(2));

    fefu::hash_map<int, char>::iterator it = a.begin();
    a.erase(it);
    it++;
    a.erase(it);
    it++;
    a.erase(it);

    ASSERT_TRUE(!a.contains(0));
    ASSERT_TRUE(!a.contains(1));
    ASSERT_TRUE(!a.contains(2));

}


TEST (HashMapTesting, BracketsOperator) {
    fefu::hash_map<int, char> a;
    a[5] = 's';
    a[2] = 'w';
    ASSERT_TRUE(a[5] == 's');
    ASSERT_TRUE(a[2] == 'w');

}

TEST (HashMapTesting, AtElements) {
    fefu::hash_map<int, char> a;
    a.insert({5, 's'});
    a.insert({2, 'l'});
    a.insert({15, 'd'});
    a.insert({222, 'q'});

    ASSERT_TRUE(a.at(222) == 'q');
    ASSERT_TRUE(a.at(5) == 's');
    ASSERT_TRUE(a.at(2) == 'l');
    ASSERT_TRUE(a.at(15) == 'd');

}

TEST (HashMapTesting, SizeTest) {
    fefu::hash_map<int, char> a;
    a.insert({10, 's'});
    a.insert({11, 'd'});
    a.insert({12, 'w'});
    a.insert({21, 'a'});
    a.insert({55, 'g'});
    a.insert({4, 'l'});

    a[100] = 'k';
    a[200] = 'p';


    a.erase(10);
    a.erase(11);
    a.erase(21);

    a.erase(99);
    a.erase(98);
    a.erase(90);

    ASSERT_TRUE(a.size() == 5);
}

TEST (HashMapTesting, IteratorTest) {
    fefu::hash_map<int, char> a;
    a.insert({5, 's'});
    a.insert({2, 'l'});
    a.insert({15, 'd'});
    a.insert({222, 'q'});

    bool check_s = false;
    bool check_l = false;
    bool check_d = false;
    bool check_q = false;
    int max_count = 0;
    int count = 0;

    fefu::hash_map<int, char>::iterator it;
    for(it = a.begin(); it != a.end(); ++it) {
        max_count++;
        if((*it)->second == 's') {
            check_s = true;
            (*it)->second = 'i';
            ASSERT_TRUE((*it)->second == 'i');
        }
        if((*it)->second == 'l')
            check_l = true;
        if((*it)->second == 'd')
            check_d = true;
        if((*it)->second == 'q')
            check_q = true;

    }

    ASSERT_TRUE(check_s);
    ASSERT_TRUE(check_l);
    ASSERT_TRUE(check_d);
    ASSERT_TRUE(check_q);

    ASSERT_TRUE(max_count == a.size());

    a.erase(222);
    a.erase(5);
    max_count = 0;

    for(it = a.begin(); it != a.end(); ++it) {
        max_count++;
    }

    ASSERT_TRUE(max_count == a.size());

}

TEST (HashMapTesting, ConstIteratorTest) {
    fefu::hash_map<int, char> a;
    a.insert({5, 's'});
    a.insert({2, 'l'});
    a.insert({15, 'd'});
    a.insert({222, 'q'});

    bool check_s = false;
    bool check_l = false;
    bool check_d = false;
    bool check_q = false;
    int max_count = 0;
    int count = 0;

    fefu::hash_map<int, char>::const_iterator it;
    for(it = a.cbegin(); it != a.cend(); ++it) {
        max_count++;
        if((*it)->second == 's')
            check_s = true;
        if((*it)->second == 'l')
            check_l = true;
        if((*it)->second == 'd')
            check_d = true;
        if((*it)->second == 'q')
            check_q = true;

    }

    ASSERT_TRUE(check_s);
    ASSERT_TRUE(check_l);
    ASSERT_TRUE(check_d);
    ASSERT_TRUE(check_q);

    ASSERT_TRUE(max_count == a.size());

    a.erase(222);
    a.erase(5);
    max_count = 0;

    for(it = a.cbegin(); it != a.cend(); ++it) {
        max_count++;
    }

    ASSERT_TRUE(max_count == a.size());

}

TEST (HashMapTesting, MergeTest) {
    fefu::hash_map<int, char> a;
    a.insert({5, 's'});
    a.insert({2, 'l'});
    a.insert({15, 'd'});
    a.insert({222, 'q'});

    fefu::hash_map<int, char> b;
    b.insert({205, 'p'});
    b.insert({908, 'm'});
    b.insert({144, 'n'});
    b.insert({445, 'o'});

    a.merge(b);

    ASSERT_TRUE(a.contains(5));
    ASSERT_TRUE(a.contains(2));
    ASSERT_TRUE(a.contains(15));
    ASSERT_TRUE(a.contains(222));
    ASSERT_TRUE(a.contains(205));
    ASSERT_TRUE(a.contains(908));
    ASSERT_TRUE(a.contains(144));
    ASSERT_TRUE(a.contains(445));

}

TEST (HashMapTesting, SwapTest) {
    fefu::hash_map<int, char> a;
    a.insert({5, 's'});
    a.insert({2, 'l'});
    a.insert({15, 'd'});
    a.insert({222, 'q'});

    fefu::hash_map<int, char> b;
    b.insert({205, 'p'});
    b.insert({908, 'm'});
    b.insert({144, 'n'});
    b.insert({445, 'o'});

    a.swap(b);

    ASSERT_TRUE(a.contains(205));
    ASSERT_TRUE(a.contains(908));
    ASSERT_TRUE(a.contains(144));
    ASSERT_TRUE(a.contains(445));

    ASSERT_TRUE(b.contains(5));
    ASSERT_TRUE(b.contains(2));
    ASSERT_TRUE(b.contains(15));
    ASSERT_TRUE(b.contains(222));

}

TEST (HashMapTesting, BucketTest) {
    fefu::hash_map<int, char> a;
    a.insert({5, 's'});
    a.insert({3, 'd'});
    a.insert({9, 'l'});

//    ASSERT_TRUE(a.bucket(5) == 5);
//    ASSERT_TRUE(a.bucket(3) == 3);
//    ASSERT_TRUE(a.bucket(9) == 9);


}

TEST (HashMapTesting, ClearAllTest) {
    fefu::hash_map<int, char> a;
    a.insert({5, 's'});
    a.insert({3, 'd'});
    a.insert({9, 'l'});

    a.clear();

    ASSERT_TRUE(a.empty());
}

TEST (HashMapTesting, RehashTest) {
    fefu::hash_map<int, char> a;
    int max_size = a.max_size();

    a.insert({10, 's'});
    a.insert({11, 'd'});
    a.insert({12, 'w'});
    a.insert({21, 'a'});
    a.insert({55, 'g'});
    a.insert({4, 'l'});
    a.insert({102, 's'});
    a.insert({121, 'd'});
    a.insert({142, 'w'});
    a.insert({211, 'a'});
    a.insert({515, 'g'});
    a.insert({41, 'l'});
    a.insert({100, 's'});
    a.insert({110, 'd'});
    a.insert({120, 'w'});
    a.insert({210, 'a'});
    a.insert({550, 'g'});
    a.insert({40, 'l'});
    a.insert({108, 's'});
    a.insert({118, 'd'});
    a.insert({128, 'w'});
    a.insert({218, 'a'});
    a.insert({558, 'g'});
    a.insert({48, 'l'});

    ASSERT_TRUE(2 * max_size == a.max_size());
}

//custom_class for tests
class my_class {
public:
    my_class(int a, int b): important_value(a), low_value(b) {}

    int get_important_value() {
        return important_value;
    }

    int get_low_value() {
        return important_value;
    }

private:
    int important_value;
    int low_value;

};


//custom_equal for tests
struct equal_to {
    bool operator()(my_class _Left, my_class _Right) const {
        return _Left.get_important_value() == _Right.get_important_value();
    }
};


//custom_first_key_hash for tests
struct FirstKeyHash {
    long operator()(my_class key, size_t table_size) const {
        return key.get_low_value() % table_size;
    }
};


//custom_second_key_hash for tests
struct SecondKeyHash {
    long operator()(my_class key, size_t table_size) const {
        return ((key.get_low_value() - 1) % table_size + 1);
    }
};

TEST (HashMapTesting, CustomClassTest) {
    fefu::hash_map<my_class, char, FirstKeyHash, SecondKeyHash, equal_to> a;
    a.insert({ {10, 2}, 'w' });
    a.insert({ {44, 33}, 'q' });
    a.insert({ {6, 21}, 'b' });

    a[{31, 11}] = 'u';
    a[{312, 211}] = 'u';

    ASSERT_TRUE(a.contains({10, 2}));
    ASSERT_TRUE(a.contains({44, 33}));
    ASSERT_TRUE(a.contains({6, 21}));
    ASSERT_TRUE(a.contains({31, 11}));
    ASSERT_TRUE(a.contains({312, 211}));

    ASSERT_TRUE((*a.find({10, 2}))->second == 'w');
    ASSERT_TRUE((*a.find({44, 33}))->second == 'q');
    ASSERT_TRUE((*a.find({6, 21}))->second == 'b');
    ASSERT_TRUE((*a.find({31, 11}))->second == 'u');
    ASSERT_TRUE((*a.find({312, 211}))->second == 'u');

    a.erase({10, 2});
    a.erase({31, 11});

        ASSERT_TRUE(!a.contains({10, 2}));
        ASSERT_TRUE(!a.contains({31, 11}));
}


#endif //CUSTOM_HASH_MAP_TESTS_H
