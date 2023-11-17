#include <cstring>

namespace galaxias
{

template <class T, size_t D, MemType M>
OwningArray<T, D, M>::OwningArray()
    : Array<T, D, M>(nullptr, Dims{0})
    , capacity_{0}
{
}

template <class T, size_t D, MemType M>
OwningArray<T, D, M>::OwningArray(const Dims& dims)
    : OwningArray<T, D, M>()
{
    resize(dims);
}

template <class T, size_t D, MemType M>
OwningArray<T, D, M>::OwningArray(const OwningArray& rhs)
    : OwningArray<T, D, M>(rhs.dims_)
{
    memcpy(Array<T, D, M>::data_, rhs.data(), capacity_ * sizeof(T));
}

template <class T, size_t D, MemType M>
OwningArray<T, D, M>::OwningArray(OwningArray&& rhs) noexcept
    : Array<T, D, M>{rhs.data_, rhs.dims_}
    , capacity_{rhs.capacity_}
{
    rhs.data_ = nullptr;
    rhs.capacity_ = 0;
}

template <class T, size_t D, MemType M>
OwningArray<T, D, M>::~OwningArray()
{
    if (Array<T, D, M>::data_)
    {
        delete[] Array<T, D, M>::data_;
    }
}

template <class T, size_t D, MemType M>
OwningArray<T, D, M>& OwningArray<T, D, M>::operator=(const OwningArray& rhs)
{
    resize(rhs.dims());
    memcpy(Array<T, D, M>::data_, rhs.data(), capacity_ * sizeof(T));
    return *this;
}

template <class T, size_t D, MemType M>
OwningArray<T, D, M>& OwningArray<T, D, M>::operator=(OwningArray&& rhs) noexcept
{
    if (Array<T, D, M>::data_)
    {
        delete[] Array<T, D, M>::data_;
    }
    Array<T, D, M>::data_ = rhs.data_;
    Array<T, D, M>::dims_ = rhs.dims_;
    capacity_ = rhs.capacity_;

    rhs.data_ = nullptr;
    return *this;
}

template <class T, size_t D, MemType M>
void OwningArray<T, D, M>::reserve(size_t capacity)
{
    if (capacity > capacity_)
    {
        T* newData = new T[capacity];
        if (Array<T, D, M>::data_)
        {
            memcpy(newData, Array<T, D, M>::data_, Array<T, D, M>::product(Array<T, D, M>::dims_) * sizeof(T));
            delete Array<T, D, M>::data_;
        }
        Array<T, D, M>::data_ = newData;
        capacity_ = capacity;
    }
}

template <class T, size_t D, MemType M>
void OwningArray<T, D, M>::resize(const Dims& dims)
{
    reserve(Array<T, D, M>::product(dims));
    Array<T, D, M>::dims_ = dims;
}

} // namespace galaxias
