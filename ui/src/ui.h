#pragma once

#include <ui/ui.h>

#include <QGuiApplication>

namespace galaxias
{

class UI : public IUI
{
public:
    UI(int argc, char* argv[]);
    ~UI() override = default;

    int run() override;

private:
    int argc_;
    char** argv_;
};

} // namespace galaxias
