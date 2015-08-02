#include <iostream>
#include <algorithm>
#include <list>
#include <unordered_map>
#include <memory>

using namespace std;

#define BUFFER_SIZE 512
const int MAX_BUFFERS = 10;

/* memory buffer */
class Buffer {
private:
    char buf[BUFFER_SIZE];
public:
    Buffer(const char* _buf)
    {
        copy_n(_buf, sizeof(_buf), buf);
    };
    ~Buffer() {};
    const char* get_Buffer()
    {
        return buf;
    }
};

/* LRU Cache template */
template <class T>
class LruList {
private:
    list<T> lru;
public:
    void add(const T& entry)
    {
        /* add entry to end of list */
        lru.push_back(entry);
    }
    void remove(const T& entry)
    {
        if (!lru.empty())
            lru.remove(entry);
    }
    bool access(const T& entry)
    {
        bool cacheHit = false;
        if (!lru.empty()) {
            for (auto& x : lru) {
                /* remove entry from current position and
                 * move to front of list */
                if (x == entry) {
                    lru.remove(x);
                    lru.push_front(entry);
                    cacheHit = true;
                    break;
                }
            }
        }
        return cacheHit;
    }
    void evict(int size)
    {
        if (!lru.empty() && (lru.size() > size))
            lru.resize((lru.size() - size));
    }
    void print_lru()
    {
        for (auto& x : lru) {
            cout << "Buffer :\t" << x->get_Buffer() << endl;
        }
    }
};

void print_cacheMiss(int index)
{
    cout << "Cache Miss with index :\t" << index << endl;
}

/* cache hash template using LRU */
template <class T>
class cacheHash {
private:
    unordered_map<int,T> hash;
    LruList<T> lruList;
public:
    void add(int index, const T& entry)
    {
        lruList.add(entry);
        hash.insert(pair<int,T>(index, entry));
    }
    void remove(int index)
    {
        if (hash.count(index)) {
            lruList.remove(hash.at(index));
            hash.erase(index);
        }
    }
    void access(int index)
    {
        if (hash.count(index)) {
            if (!lruList.access(hash.at(index))) {
                print_cacheMiss(index);
            }
        } else {
            print_cacheMiss(index);
        }
    }
    void evict(int size)
    {
        /* first evict from the list */
        lruList.evict(size);
        /* then delete the evicted buffers from the hash */
        for (auto& x : hash) {
            /* use count of shared_ptr will be 1 for the
             * buffers which are only accessed by the
             * hash map and deleted from the list
             */
            if (x.second.use_count() <= 1)
                hash.erase(x.first);
        }
    }
    void print_cache_hash()
    {
        lruList.print_lru();
        cout << endl << endl;
    }
};

int main() {
    cacheHash<shared_ptr<Buffer>> cHash;
    string test_str("buffer");
    int i;
    for (i = 1; i <= MAX_BUFFERS; i++) {
        string s = test_str + to_string(i);
        shared_ptr<Buffer> buf = make_shared<Buffer>(s.c_str());
        cHash.add(i, buf);
    }
    cHash.print_cache_hash();
    cHash.access(5);
    cHash.print_cache_hash();
    cHash.remove(4);
    cHash.access(4);
    cHash.evict(2);
    cHash.print_cache_hash();
    return 0;
}