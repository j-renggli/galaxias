#include <core/array.h>

#include <catch2/catch.hpp>

using namespace galaxias;

TEMPLATE_TEST_CASE("Empty owning array has nullptr", "[array]", uint8_t, int64_t, float, double)
{
    Owning1DArray<TestType> owner;
    CHECK(owner.data() == nullptr);
    CHECK(owner.capacity() == 0);
    CHECK(owner.size() == 0);
    CHECK(owner.dims() == (typename Owning1DArray<TestType>::Dims{{0}}));

    // Which can be reserved
    owner.reserve(5);
    auto data = owner.data();
    CHECK(data != nullptr);
    CHECK(owner.capacity() == 5);
    CHECK(owner.size() == 0);

    // And resized
    owner.resize({{3}});
    CHECK(owner.data() == data);
    CHECK(owner.capacity() == 5);
    CHECK(owner.size() == 3);
    for (size_t i = 0; i < 3; ++i)
    {
        owner[i] = static_cast<TestType>(25 + i);
    }

    // And resize keeps data
    owner.resize({{7}});
    CHECK(owner.data() != data);
    CHECK(owner.capacity() == 7);
    CHECK(owner.size() == 7);
    for (size_t i = 0; i < 3; ++i)
    {
        CHECK(owner[i] == static_cast<TestType>(25 + i));
    }
}

TEMPLATE_TEST_CASE("View on simple owning array has correct observables", "[array]", uint8_t, int64_t, float, double)
{
    constexpr size_t size{8};
    std::array<size_t, 1> dims{{size}};

    {
        // Constructors
        Owning1DArray<TestType> base(dims);
        std::iota(base.data(), base.data() + size, 0);

        Owning1DArray<TestType> copyCtor{base};
        CHECK(copyCtor.capacity() == size);
        CHECK(copyCtor.size() == size);
        CHECK(copyCtor.dims() == dims);
        for (size_t i = 0; i < size; ++i)
        {
            CHECK(copyCtor[i] == static_cast<TestType>(i));
        }

        Owning1DArray<TestType> moveCtor{std::move(base)};
        CHECK(moveCtor.capacity() == size);
        CHECK(moveCtor.size() == size);
        CHECK(moveCtor.dims() == dims);
        for (size_t i = 0; i < size; ++i)
        {
            CHECK(moveCtor[i] == static_cast<TestType>(i));
        }
        CHECK(base.data() == nullptr);
    }

    {
        // Copy operators
        Owning1DArray<TestType> base(dims);
        std::iota(base.data(), base.data() + size, 0);

        Owning1DArray<TestType> copy;
        copy = base;
        CHECK(copy.capacity() == size);
        CHECK(copy.size() == size);
        CHECK(copy.dims() == dims);
        for (size_t i = 0; i < size; ++i)
        {
            CHECK(copy[i] == static_cast<TestType>(i));
        }

        Owning1DArray<TestType> move;
        move = std::move(base);
        CHECK(move.capacity() == size);
        CHECK(move.size() == size);
        CHECK(move.dims() == dims);
        for (size_t i = 0; i < size; ++i)
        {
            CHECK(move[i] == static_cast<TestType>(i));
        }
    }

    //    // Basic array
    //    constexpr TestType newVal{127};
    //    {
    //        ArrayView<TestType, 1> view(data.data(), data.size());
    //        CHECK(view.data() == data.data());
    //        CHECK(view.size() == data.size());
    //        CHECK(view.dims() == (typename ArrayView<TestType, 1>::Dims{{data.size()}}));
    //        view.at(3) = newVal;
    //        CHECK(view[3] == newVal);
    //    }

    //    // Const value with offset
    //    {
    //        ArrayView<const TestType, 1> view(data.data() + 2, data.size() - 4);
    //        CHECK(view.data() == &data[2]);
    //        CHECK(view.size() == data.size() - 4);
    //        CHECK(view.dims() == (typename ArrayView<TestType, 1>::Dims{{data.size() - 4}}));
    //        CHECK(view[1] == newVal);
    //    }
}

//// 2d, change dims

// TEMPLATE_TEST_CASE("View on 2D owning arrays has correct observables", "[array]", uint8_t, int64_t, float, double)
//{
//     std::vector<TestType> data;
//     data.resize(30);
//     std::iota(data.begin(), data.end(), 0);
//     constexpr typename ArrayView<TestType, 2>::Dims baseDims{{6, 5}};

//    {
//        // Constructors
//        ArrayView<TestType, 2> base(data.data(), baseDims);

//        ArrayView<TestType, 2> copyCtor{base};
//        CHECK(copyCtor.data() == data.data());
//        CHECK(copyCtor.size() == data.size());
//        CHECK(copyCtor.dims() == baseDims);

//        ArrayView<TestType, 2> moveCtor{std::move(base)};
//        CHECK(moveCtor.data() == data.data());
//        CHECK(moveCtor.size() == data.size());
//        CHECK(moveCtor.dims() == baseDims);
//    }

//    {
//        // Copy operators
//        ArrayView<TestType, 2> base(data.data(), baseDims);

//        ArrayView<TestType, 2> copy;
//        copy = base;
//        CHECK(copy.data() == data.data());
//        CHECK(copy.size() == data.size());
//        CHECK(copy.dims() == baseDims);

//        ArrayView<TestType, 2> move;
//        move = std::move(base);
//        CHECK(move.data() == data.data());
//        CHECK(move.size() == data.size());
//        CHECK(move.dims() == baseDims);
//    }

//    // Basic array
//    constexpr TestType newVal1{127};
//    //    constexpr TestType newVal2{128};
//    constexpr TestType newVal3{199};
//    {
//        ArrayView<TestType, 2> view(data.data(), baseDims);
//        CHECK(view.data() == data.data());
//        CHECK(view.size() == data.size());
//        CHECK(view.dims() == baseDims);
//        view.at(3) = newVal1;
//        //        view.at(4, 2) = newVal2;
//        view.at({{2, 4}}) = newVal3;
//        CHECK(view[3] == newVal1);
//        //        CHECK(view[4, 2] == newVal2);
//        CHECK(view[{{2, 4}}] == newVal3);
//    }

//    // Const value with offset
//    {
//        constexpr typename ArrayView<TestType, 2>::Dims newDims{{5, 3}};
//        ArrayView<const TestType, 2> view(data.data() + 2, newDims);
//        CHECK(view.data() == &data[2]);
//        CHECK(view.size() == 5 * 3);
//        CHECK(view.dims() == newDims);
//        CHECK(view[1] == newVal1);
//        CHECK(view[{{4, 0}}] == newVal3);
//    }
//}
