#pragma once

#include <memory>
#include <numeric>

namespace galaxias
{

enum class MemType
{
    Host,
    Device,
};

template <class T, size_t D, MemType M>
class Array
{
public:
    using value_type = T;
    using Dims = std::array<size_t, D>;

    Array()
        : data_{nullptr}
        , dims_{{0}}
    {
    }
    Array(T* data, size_t size)
        : data_{data}
        , dims_{{size}} // {{size, 1, 1, 1, ...}}
    {
        for (size_t i = 1; i < D; ++i)
        {
            dims_[i] = 1;
        }
    }
    Array(T* data, const Dims& dims)
        : data_{data}
        , dims_{dims}
    {
    }
    Array(const Array&) = default;
    Array(Array&&) noexcept = default;

    Array& operator=(const Array&) = default;
    Array& operator=(Array&&) noexcept = default;

    T* data() { return data_; }
    const T* data() const { return data_; }
    const Dims& dims() const { return dims_; }
    size_t bytes() const { return size() * sizeof(T); }
    size_t size() const { return product(dims_); }

    // 1D Access & view
    T& at(size_t index) { return data_[index]; }
    T& at(const Dims& index) { return at(indexOf(index)); }
    const T& at(size_t index) const { return data_[index]; }
    const T& at(const Dims& index) const { return at(indexOf(index)); }
    T& operator[](size_t index) { return data_[index]; } // {{size, 1, 1, 1, ...}}
    T& operator[](const Dims& index) { return operator[](indexOf(index)); }
    const T& operator[](size_t index) const { return data_[index]; }
    const T& operator[](const Dims& index) const { return operator[](indexOf(index)); }

protected:
    size_t indexOf(const Dims& index) const
    {
        size_t idx = index[0];
        for (size_t i = 1; i < D; ++i)
        {
            idx *= dims_[i];
            idx += index[i];
        }
        return idx;
    }

    static size_t product(const Dims& dims)
    {
        return std::accumulate(dims.begin(), dims.end(), 1, std::multiplies<size_t>());
    }

protected:
    T* data_;
    Dims dims_;
};

template <class T, size_t D>
using ArrayView = Array<T, D, MemType::Host>;

template <class T>
using View1DArray = ArrayView<T, 1>;

template <class T, size_t D, MemType M = MemType::Host>
class OwningArray : public Array<T, D, M>
{
public:
    using Dims = typename Array<T, D, M>::Dims;

    OwningArray();
    OwningArray(const Dims& dims);
    OwningArray(const OwningArray&);
    OwningArray(OwningArray&&) noexcept;
    ~OwningArray();

    OwningArray& operator=(const OwningArray&);
    OwningArray& operator=(OwningArray&&) noexcept;

    void reserve(size_t capacity);
    void resize(const Dims& dims);

    size_t capacity() const { return capacity_; }

private:
    size_t capacity_;
};

template <class T>
using Owning1DArray = OwningArray<T, 1>;

} // namespace galaxias

#include "array.inl"
