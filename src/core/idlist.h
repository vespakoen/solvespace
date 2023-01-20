#ifndef SOLVESPACE_IDLIST_H
#define SOLVESPACE_IDLIST_H

#include "core.h"

template<class T, class H>
class IdList {
    std::vector<T> elemstore;
    std::vector<int> elemidx;
    std::vector<int> freelist;

public:
    int n = 0; // PAR@@@@@ make this private to see all interesting and suspicious places in
               // SoveSpace ;-)

    friend struct CompareId<T, H>;
    using Compare = CompareId<T, H>;

    struct iterator {
        typedef std::random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef int difference_type;
        typedef T *pointer;
        typedef T &reference;

    public:
        T &operator*() const noexcept {
            return *elem;
        }
        const T *operator->() const noexcept {
            return elem;
        }

        bool operator==(const iterator &p) const {
            return p.position == position;
        }
        bool operator!=(const iterator &p) const {
            return !operator==(p);
        }

        iterator &operator++() {
            ++position;
            if(position >= (int)list->elemidx.size()) {
                elem = nullptr; // PAR@@@@ Remove just debugging
            } else if(0 <= position) {
                elem = &(list->elemstore[list->elemidx[position]]);
            }
            return *this;
        }

        // Needed for std:find_if of gcc used in entity.cpp GenerateEquations
        difference_type operator-(const iterator &rhs) const noexcept {
            return position - rhs.position;
        }

        iterator(IdList<T, H> *l) : position(0), list(l) {
            if(list) {
                if(list->elemstore.size() && list->elemidx.size()) {
                    elem = &(list->elemstore[list->elemidx[position]]);
                }
            }
        };

        iterator(IdList<T, H> *l, int pos) : position(pos), list(l) {
            if(position >= (int)list->elemidx.size()) {
                elem = nullptr;
            } else if(0 <= position) {
                elem = &((list->elemstore)[list->elemidx[position]]);
            }
        };

    private:
        int position;
        T *elem;
        IdList<T, H> *list;
    };


    bool IsEmpty() const {
        return n == 0;
    }

    uint32_t MaximumId() {
        if(IsEmpty()) {
            return 0;
        } else {
            return elemstore[elemidx.back()].h.v;
        }
    }

    H AddAndAssignId(T *t) {
        t->h.v = (MaximumId() + 1);

        // Add at the end of the list.
        elemstore.push_back(*t);
        elemidx.push_back(elemstore.size() - 1);
        ++n;

        return t->h;
    }

    void ReserveMore(int howMuch) {
        elemstore.reserve(elemstore.size() + howMuch);
        elemidx.reserve(elemidx.size() + howMuch);
        //        freelist.reserve(freelist.size() + howMuch);    // PAR@@@@ maybe we should - not
        //        much more RAM
    }

    void Add(T *t) {
        // Look to see if we already have something with the same handle value.
        ssassert(FindByIdNoOops(t->h) == nullptr, ("Handle isn't unique: " + std::to_string(t->h.v)).c_str());

        // Find out where the added element should be.
        auto pos = std::lower_bound(elemidx.begin(), elemidx.end(), *t, Compare(this));

        if(freelist.empty()) { // Add a new element to the store
            elemstore.push_back(*t);
            // Insert a pointer to the element at the correct position
            if(elemidx.empty()) {
                // The list is empty so pos, begin and end are all null.
                // insert does not work in this case.
                elemidx.push_back(elemstore.size() - 1);
            } else {
                elemidx.insert(pos, elemstore.size() - 1);
            }
        } else { // Use the last element from the freelist
            // Insert an index to the element at the correct position
            elemidx.insert(pos, freelist.back());
            // Remove the element from the freelist
            freelist.pop_back();

            // Copy-construct to the element storage.
            elemstore[*pos] = T(*t);
            //            *elemptr[pos] = *t;   // PAR@@@@@@ maybe this?
        }

        ++n;
    }

    T *FindById(H h) {
        T *t = FindByIdNoOops(h);
        ssassert(t != nullptr, ("Cannot find handle: " + std::to_string(h.v)).c_str());
        return t;
    }

    T *FindByIdNoOops(H h) {
        if(IsEmpty()) {
            return nullptr;
        }
        auto it = std::lower_bound(elemidx.begin(), elemidx.end(), h, Compare(this));
        if(it == elemidx.end()) {
            return nullptr;
        } else {
            if(elemstore[*it].h.v != h.v) {
                return nullptr;
            }
            return &elemstore[*it];
        }
    }

    T &Get(size_t i) {
        return elemstore[elemidx[i]];
    }

    T &operator[](size_t i) {
        return Get(i);
    }

    iterator begin() {
        return IsEmpty() ? nullptr : iterator(this);
    }
    iterator end() {
        return IsEmpty() ? nullptr : iterator(this, elemidx.size());
    }

    void ClearTags() {
        for(auto &elt : *this) {
            elt.tag = 0;
        }
    }

    void Tag(H h, int tag) {
        auto it = FindByIdNoOops(h);
        if(it != nullptr) {
            it->tag = tag;
        }
    }

    void RemoveTagged() {
        int src, dest;
        dest = 0;
        for(src = 0; src < n; src++) {
            if(elemstore[elemidx[src]].tag) {
                // this item should be deleted
                elemstore[elemidx[src]].Clear();
                //                elemstore[elemidx[src]].~T(); // Clear below calls the destructors
                freelist.push_back(elemidx[src]);
                elemidx[src] = 0xDEADBEEF; // PAR@@@@@ just for debugging, not needed, remove later
            } else {
                if(src != dest) {
                    elemidx[dest] = elemidx[src];
                }
                dest++;
            }
        }
        n = dest;
        elemidx.resize(n); // Clear left over elements at the end.
    }

    void RemoveById(H h) { // PAR@@@@@ this can be optimized
        ClearTags();
        FindById(h)->tag = 1;
        RemoveTagged();
    }

    void MoveSelfInto(IdList<T, H> *l) {
        l->Clear();
        std::swap(l->elemstore, elemstore);
        std::swap(l->elemidx, elemidx);
        std::swap(l->freelist, freelist);
        std::swap(l->n, n);
    }

    void DeepCopyInto(IdList<T, H> *l) {
        l->Clear();

        for(auto const &it : elemstore) {
            l->elemstore.push_back(it);
        }

        for(auto const &it : elemidx) {
            l->elemidx.push_back(it);
        }

        l->n = n;
    }

    void Clear() {
        for(auto &it : elemidx) {
            elemstore[it].Clear();
            //            elemstore[it].~T(); // clear below calls the destructors
        }
        freelist.clear();
        elemidx.clear();
        elemstore.clear();
        n = 0;
    }
};

#endif
