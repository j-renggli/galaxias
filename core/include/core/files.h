#pragma once

#include "array.h"

#include <cstdint>
#include <filesystem>
#include <memory>
#include <vector>

namespace galaxias
{

class TemporaryFolder
{
public:
    TemporaryFolder();
    //    TemporaryFolder(const std::filesystem::path& parentFolder);
    TemporaryFolder(const TemporaryFolder&) = delete;
    TemporaryFolder(TemporaryFolder&&) noexcept;
    ~TemporaryFolder();

    TemporaryFolder& operator=(const TemporaryFolder&) = delete;
    TemporaryFolder& operator=(TemporaryFolder&&) noexcept;

    const std::filesystem::path path() const { return path_; }

private:
    void remove();

private:
    std::filesystem::path path_;
};

// class File
//{
// public:
//     Owning1DArray<uint8_t>& data() { return data_; }
//     const Owning1DArray<uint8_t>& data() const { return data_; }

// private:
//     Owning1DArray<uint8_t> data_;
// };

class TemporaryFile
{
public:
    TemporaryFile(const std::string& extension = "");
    //    TemporaryFile(const std::filesystem::path& fileOrFolder);
    TemporaryFile(const TemporaryFile&) = delete;
    TemporaryFile(TemporaryFile&&) noexcept;
    ~TemporaryFile();

    TemporaryFile& operator=(const TemporaryFile&) = delete;
    TemporaryFile& operator=(TemporaryFile&&) noexcept;

    const std::filesystem::path path() const { return path_; }

private:
    void remove();

private:
    std::filesystem::path path_;
};

class FilesManager
{
public:
    enum class ShaderType
    {
        DirectVertex,
        DirectFragment,
    };
    static Owning1DArray<uint32_t> loadShader(ShaderType type);
};

} // namespace galaxias
