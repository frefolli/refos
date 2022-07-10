#ifndef __MEMORY_RUNTIME_HPP__
#define __MEMORY_RUNTIME_HPP__

typedef long unsigned int size_t;

inline void *operator new(size_t, void *p)     throw() {
    return p;
}

inline void *operator new[](size_t, void *p)   throw() {
    return p;
}

inline void  operator delete  (void *, void *) throw() {
    
};

inline void  operator delete[](void *, void *) throw() {
    
};

#endif
