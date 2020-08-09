//
//  ExpandableHashMap.h
//  GooberEats
//
//  Created by Sarah Liang on 3/5/20.
//  Copyright © 2020 Sarah Liang. All rights reserved.
//

#ifndef ExpandableHashMap_h
#define ExpandableHashMap_h

#include <vector>
#include <utility>
#include <list>
#include <string>
#include <iostream>
#include <cassert>

template<typename KeyType, typename ValueType>

class ExpandableHashMap
{
    public:
        //constructor, sets max load factor to 0.5
        ExpandableHashMap(double maximumLoadFactor = 0.5);
        //destructor (does nothing since no dynamically allocated objects)
        ~ExpandableHashMap(){};
        void reset();
        int size() const;
        void associate(const KeyType& key, const ValueType& value);
        // for a map that can't be modified, return a pointer to const ValueType
        const ValueType* find(const KeyType& key) const;
        // for a modifiable map, return a pointer to modifiable ValueType
        ValueType* find(const KeyType& key) {
            return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
        }
        // C++11 syntax for preventing copying and assignment
        ExpandableHashMap(const ExpandableHashMap&) = delete;
        ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;
    
    private:
        unsigned int getBucketNumber(const KeyType& key, const int& s) const;
        double maxLoadFactor;
        int num_associations = 0;
        int totalBuckets = 8;
        std::vector<std::list<std::pair<KeyType,ValueType>>> hashMap;
};

//constructor
template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType,ValueType>::ExpandableHashMap(double maximumLoadFactor):hashMap(8),maxLoadFactor(maximumLoadFactor)
{}

// returns bucket number of where to place item in hashmap
template<typename KeyType, typename ValueType>
unsigned int ExpandableHashMap<KeyType,ValueType>:: getBucketNumber(const KeyType& key, const int& mapSize) const {
    unsigned int hasher(const KeyType& k);
    unsigned int hashedKey = hasher(key);
    int bucketNumber = hashedKey %= mapSize;
    return bucketNumber;
}

// resets the hashmap back to 8 buckets, deletes all items
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType,ValueType>:: reset() {
    hashMap.clear();
    hashMap.resize(8);
}

// return the number of associations in the hashmap
template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType,ValueType>:: size() const {
    return num_associations;
}

// The associate method associates one item (key) with another (value). If no association currently exists with that key, this method inserts a new association into the hashmap with that key/value pair. If there is  already an association with that key in the hashmap, then the item associated with that key is replaced by the second parameter (value).
// Thus, the hashmap must contain no duplicate keys.
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType,ValueType>:: associate(const KeyType& key, const ValueType& value) {
    // if no association currently exists, insert new association
    if(find(key) == nullptr) {
        // if hash map is over half full, double the size and rehash all elements
        if(num_associations/hashMap.size() >= maxLoadFactor) {
            // double size in temp hash map
            std::vector<std::list<std::pair<KeyType,ValueType>>> tempHashMap(hashMap.size()*2);
            // rehash all elements to evenly redistribute into new buckets
            for(int i=0; i<hashMap.size(); i++) {
                while(!hashMap[i].empty()){
                    const KeyType Ktemp = hashMap[i].front().first;
                    int bucket_num = getBucketNumber(Ktemp,tempHashMap.size());
                    const  ValueType Vtemp = hashMap[i].front().second;
                    tempHashMap[bucket_num].push_back(std::pair<KeyType,ValueType>(Ktemp,Vtemp));
                    hashMap[i].pop_front();
                }
            }
            // set hashmap to temp
            hashMap = tempHashMap;
            // add the new element in
            int bucket_num = getBucketNumber(key, hashMap.size());
            hashMap[bucket_num].push_back(make_pair(key, value));
            num_associations++;
        } else { // did not exceed 50% capacity, so find bucket number for key and insert in hash map
            int bucket_num = getBucketNumber(key, hashMap.size());
            hashMap[bucket_num].push_back(make_pair(key, value));
            num_associations++;
        }
    } else { // key already exists, just update value with new value
        ValueType* vtemp = find(key);
        *vtemp = value;
    }
}

// If no association exists with the given key, return nullptr; otherwise, return a pointer to the value associated with that key.
// This pointer can be used to examine that value, and if the hashmap is allowed to be modified, to modify that value directly within the map (the second overload enables this).
template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType,ValueType>:: find(const KeyType& key) const {
    //get the bucket, iterate to the right element
    int bucket_num  = getBucketNumber(key, hashMap.size());
    auto it = hashMap[bucket_num].begin();
    while(it != hashMap[bucket_num].end()) {
        //if found
        if(it->first == key) {
            return &(it->second);
        } else {
            it++;
        }
    }
    //else not found
    return nullptr;
}

#endif /* ExpandableHashMap_h */
