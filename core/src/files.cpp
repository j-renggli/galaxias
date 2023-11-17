#include <core/files.h>

#include <array>
#include <filesystem>
#include <fstream>

#define UUID_SYSTEM_GENERATOR 1
#include <uuid>

namespace galaxias
{

namespace
{

template <class T>
void loadFile(const std::filesystem::path& path, Owning1DArray<T>& buffer)
{
    std::ifstream ifs{path, std::ios::binary | std::ios::ate};
    const size_t size = ifs.tellg();
    if (size % sizeof(T))
    {
        throw std::out_of_range("Can't fill buffer with " + std::to_string(size) + " bytes");
    }
    buffer.resize({{size / sizeof(T)}});
    ifs.seekg(0, std::ios::beg);
    ifs.read(reinterpret_cast<char*>(buffer.data()), size);
}

} // namespace

TemporaryFolder::TemporaryFolder()
    : path_{std::filesystem::temp_directory_path() / to_string(uuids::uuid_system_generator{}())}
{
    create_directory(path_);
}

TemporaryFolder::TemporaryFolder(TemporaryFolder&& rhs) noexcept
    : path_{rhs.path_}
{
    rhs.path_ = "";
}

TemporaryFolder::~TemporaryFolder() { remove(); }

TemporaryFolder& TemporaryFolder::operator=(TemporaryFolder&& rhs) noexcept
{
    remove();
    std::swap(path_, rhs.path_);
    return *this;
}

void TemporaryFolder::remove()
{
    if (exists(path_) && path_.string().size() > 0)
    {
        remove_all(path_);
    }
    path_ = "";
}

////////////////////////////////////////////////////////////////

TemporaryFile::TemporaryFile(const std::string& extension)
    : path_{std::filesystem::temp_directory_path() / (to_string(uuids::uuid_system_generator{}()) + extension)}
{
    create_directory(path_);
}

TemporaryFile::TemporaryFile(TemporaryFile&& rhs) noexcept
    : path_{rhs.path_}
{
    rhs.path_ = "";
}

TemporaryFile::~TemporaryFile() { remove(); }

TemporaryFile& TemporaryFile::operator=(TemporaryFile&& rhs) noexcept
{
    remove();
    std::swap(path_, rhs.path_);
    return *this;
}

void TemporaryFile::remove()
{
    if (exists(path_) && path_.string().size() > 0)
    {
        std::filesystem::remove(path_);
    }
    path_ = "";
}

Owning1DArray<uint32_t> FilesManager::loadShader(ShaderType type)
{
    const auto path = std::filesystem::path{"/"} / "home" / "jrenggli" / "temp" /
                      (std::string(type == ShaderType::DirectVertex ? "vert" : "frag") + ".spv");
    Owning1DArray<uint32_t> buffer;
    loadFile(path, buffer);
    return buffer;
}

} // namespace galaxias
