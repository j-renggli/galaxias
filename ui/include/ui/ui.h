#pragma once

#include <memory>

namespace galaxias
{

class IUI
{
public:
    virtual ~IUI() = default;

    static std::unique_ptr<IUI> create(int argc, char* argv[]);

    virtual int run() = 0;
};

} // namespace galaxias
