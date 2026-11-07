#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
#include "Tensor.h"

/*
[magic][version][dtype][ndims][shape × ndims][data × numel]
  4B      4B      4B     4B       4B每个         4B每个
字节序: 小端 (x86 / ARM 都是小端, 这里直接写内存)
*/
namespace {

constexpr uint32_t kMagic = 0x534E4554;  // 小端写入后文件里的字节是 "TENS"
constexpr uint32_t kVersion = 1;
constexpr uint32_t kDTypeFloat32 = 0;    // 0 表示 float32
constexpr uint32_t kMaxDims = 8;

// 逐个字段读写, 避免 struct 对齐填充; 固定宽度类型保证跨平台大小一致
template <typename T>
void write_field(std::ofstream& ofs, T value) {
    ofs.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

template <typename T>
T read_field(std::ifstream& ifs, const char* what) {
    T value{};
    ifs.read(reinterpret_cast<char*>(&value), sizeof(T));
    if (!ifs) {
        throw std::runtime_error(std::string("truncated file while reading ") + what);
    }
    return value;
}

}  // namespace

void write_tensor_to_binary(const std::string& filename, const Tensor& tensor) {
    std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
    if (!ofs) throw std::runtime_error("cannot open for write: " + filename);

    const auto& shape = tensor.shape();
    write_field<uint32_t>(ofs, kMagic);
    write_field<uint32_t>(ofs, kVersion);
    write_field<uint32_t>(ofs, kDTypeFloat32);
    write_field<uint32_t>(ofs, static_cast<uint32_t>(shape.size()));
    for (int dim : shape) write_field<uint32_t>(ofs, static_cast<uint32_t>(dim));
    ofs.write(reinterpret_cast<const char*>(tensor.data()),
              static_cast<std::streamsize>(tensor.numel() * sizeof(float)));

    ofs.flush();
    if (!ofs) throw std::runtime_error("write failed: " + filename);
}

Tensor read_tensor_from_binary(const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) throw std::runtime_error("cannot open for read: " + filename);

    if (read_field<uint32_t>(ifs, "magic") != kMagic)
        throw std::runtime_error("invalid magic number");
    if (read_field<uint32_t>(ifs, "version") != kVersion)
        throw std::runtime_error("unsupported version");
    if (read_field<uint32_t>(ifs, "dtype") != kDTypeFloat32)
        throw std::runtime_error("unsupported dtype");

    const uint32_t ndims = read_field<uint32_t>(ifs, "ndims");
    if (ndims == 0 || ndims > kMaxDims) throw std::runtime_error("invalid ndims");

    std::vector<int> shape(ndims);
    size_t numel = 1;
    for (auto& dim : shape) {
        const uint32_t d = read_field<uint32_t>(ifs, "shape");
        if (d == 0 || d > static_cast<uint32_t>(std::numeric_limits<int>::max()))
            throw std::runtime_error("invalid dimension");
        // 防乘法溢出: numel * d * sizeof(float) 不能超过 size_t
        if (numel > std::numeric_limits<size_t>::max() / sizeof(float) / d)
            throw std::runtime_error("shape too large");
        numel *= d;
        dim = static_cast<int>(d);
    }

    // 分配前先核对文件剩余字节数, 防止损坏的 shape 导致巨量分配
    const auto data_begin = ifs.tellg();
    ifs.seekg(0, std::ios::end);
    const auto file_end = ifs.tellg();
    ifs.seekg(data_begin);
    if (static_cast<size_t>(file_end - data_begin) != numel * sizeof(float))
        throw std::runtime_error("data size does not match shape");

    Tensor tensor(shape);
    ifs.read(reinterpret_cast<char*>(tensor.data()),
             static_cast<std::streamsize>(numel * sizeof(float)));
    if (!ifs) throw std::runtime_error("failed reading data");
    return tensor;
}

// ---------------------------- 测试 ----------------------------
static int g_failed = 0;

static void expect(bool cond, const char* msg) {
    std::cout << (cond ? "[PASS] " : "[FAIL] ") << msg << std::endl;
    if (!cond) ++g_failed;
}

template <typename Fn>
static bool throws(Fn fn) {
    try {
        fn();
    } catch (const std::runtime_error& e) {
        std::cout << "       (caught: " << e.what() << ")" << std::endl;
        return true;
    }
    return false;
}

static void test_roundtrip(const std::string& path) {
    Tensor src({1, 2, 3});
    for (size_t i = 0; i < src.numel(); ++i) src.data()[i] = static_cast<float>(i);

    write_tensor_to_binary(path, src);
    Tensor dst = read_tensor_from_binary(path);

    std::cout << "shape:";
    for (int d : dst.shape()) std::cout << " " << d;
    std::cout << "\ndata:";
    for (size_t i = 0; i < dst.numel(); ++i) std::cout << " " << dst.data()[i];
    std::cout << std::endl;

    expect(dst.shape() == src.shape(), "roundtrip: shape equal");
    bool same = dst.numel() == src.numel();
    for (size_t i = 0; same && i < src.numel(); ++i) same = dst.data()[i] == src.data()[i];
    expect(same, "roundtrip: data equal");
}

static void test_bad_magic(const std::string& path) {
    { std::ofstream o(path, std::ios::binary); o << "this is not a tensor file"; }
    expect(throws([&] { read_tensor_from_binary(path); }), "bad magic -> throws");
}

static void test_truncated(const std::string& path) {
    Tensor src({4, 4});
    write_tensor_to_binary(path, src);

    std::ifstream in(path, std::ios::binary);
    std::string bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();
    {
        std::ofstream o(path, std::ios::binary | std::ios::trunc);
        o.write(bytes.data(), static_cast<std::streamsize>(bytes.size() - 4));
    }
    expect(throws([&] { read_tensor_from_binary(path); }), "truncated data -> throws");
}

static void test_missing_file() {
    expect(throws([] { read_tensor_from_binary("no_such_file.bin"); }), "missing file -> throws");
}

int main() {
    const std::string path = "output.bin";
    test_roundtrip(path);
    test_bad_magic(path);
    test_truncated(path);
    test_missing_file();
    std::remove(path.c_str());

    std::cout << (g_failed == 0 ? "ALL PASSED" : "SOME FAILED") << std::endl;
    return g_failed == 0 ? 0 : 1;
}
