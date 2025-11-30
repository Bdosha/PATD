#ifndef SMARTPTRSTORAGE_H
#define SMARTPTRSTORAGE_H

#include <unordered_map>
#include <mutex>
#include <functional>

// Общее хранилище для всех типов
class SmartPtrStorageBase {
public:
    struct StorageEntry {
        void *ptr;
        size_t refCount;
        std::function<void(void *)> deleter;

        StorageEntry(void *p, std::function<void(void *)> d) : ptr(p), refCount(1), deleter(d) {}
    };

    static SmartPtrStorageBase &getInstance() {
        static SmartPtrStorageBase instance;
        return instance;
    }

    void *store(void *ptr, std::function<void(void *)> deleter) {
        std::lock_guard<std::mutex> lock(mutex);
        if (ptr == nullptr) return nullptr;

        void *key = ptr;
        auto it = storage.find(key);
        if (it != storage.end()) {
            it->second.refCount++;
            deleter(ptr); // Удаляем дубликат
            return it->second.ptr;
        }

        storage.emplace(key, StorageEntry(ptr, deleter));
        return ptr;
    }

    void addRef(void *ptr) {
        if (ptr == nullptr) return;
        std::lock_guard<std::mutex> lock(mutex);
        void *key = ptr;
        auto it = storage.find(key);
        if (it != storage.end()) {
            it->second.refCount++;
        }
    }

    void release(void *ptr) {
        if (ptr == nullptr) return;
        std::lock_guard<std::mutex> lock(mutex);
        void *key = ptr;
        auto it = storage.find(key);
        if (it != storage.end()) {
            it->second.refCount--;
            if (it->second.refCount == 0) {
                it->second.deleter(it->second.ptr);
                storage.erase(it);
            }
        }
    }

    size_t getRefCount(void *ptr) const {
        if (ptr == nullptr) return 0;
        std::lock_guard<std::mutex> lock(mutex);
        void *key = ptr;
        auto it = storage.find(key);
        if (it != storage.end()) {
            return it->second.refCount;
        }
        return 0;
    }

    SmartPtrStorageBase(const SmartPtrStorageBase &) = delete;
    SmartPtrStorageBase &operator=(const SmartPtrStorageBase &) = delete;

private:
    mutable std::unordered_map<void *, StorageEntry> storage;
    mutable std::mutex mutex;

    SmartPtrStorageBase() = default;
    ~SmartPtrStorageBase() {
        std::lock_guard<std::mutex> lock(mutex);
        for (auto &pair : storage) {
            pair.second.deleter(pair.second.ptr);
        }
        storage.clear();
    }
};

// Обертка для обратной совместимости
template<class T>
class SmartPtrStorage {
public:
    static SmartPtrStorageBase &getInstance() {
        return SmartPtrStorageBase::getInstance();
    }

    T *store(T *ptr) {
        auto &base = getInstance();
        return static_cast<T *>(base.store(ptr, [](void *p) { delete static_cast<T *>(p); }));
    }

    void addRef(T *ptr) {
        auto &base = getInstance();
        base.addRef(ptr);
    }

    void release(T *ptr) {
        auto &base = getInstance();
        base.release(ptr);
    }

    size_t getRefCount(T *ptr) const {
        auto &base = getInstance();
        return base.getRefCount(ptr);
    }
};

#endif
