// This software is in the public domain. Where that dedication is not
// recognized, you are granted a perpetual, irrevocable license to copy,
// distribute, and modify this file as you see fit.
// https://github.com/ddiakopoulos/tinyply

// This file is only required for the example and test programs.


#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>
#include <iterator>

#define TINYPLY_IMPLEMENTATION
#include "tinyply.h"
#include "fstr.h"

inline std::vector<uint8_t> read_file_binary(const std::string & pathToFile)
{
    std::ifstream file(pathToFile, std::ios::binary);
    std::vector<uint8_t> fileBufferBytes;

    if (file.is_open())
    {
        file.seekg(0, std::ios::end);
        size_t sizeBytes = file.tellg();
        file.seekg(0, std::ios::beg);
        fileBufferBytes.resize(sizeBytes);
        if (file.read((char*)fileBufferBytes.data(), sizeBytes)) return fileBufferBytes;
    }
    else throw std::runtime_error("could not open binary ifstream to path " + pathToFile);
    return fileBufferBytes;
}

struct memory_buffer : public std::streambuf
{
    char * p_start {nullptr};
    char * p_end {nullptr};
    size_t size;

    memory_buffer(char const * first_elem, size_t size)
        : p_start(const_cast<char*>(first_elem)), p_end(p_start + size), size(size)
    {
        setg(p_start, p_start, p_end);
    }

    pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which) override
    {
        if (dir == std::ios_base::cur) gbump(static_cast<int>(off));
        else setg(p_start, (dir == std::ios_base::beg ? p_start : p_end) + off, p_end);
        return gptr() - p_start;
    }

    pos_type seekpos(pos_type pos, std::ios_base::openmode which) override
    {
        return seekoff(pos, std::ios_base::beg, which);
    }
// Generated to_string for PUBLIC STRUCT_DECL memory_buffer
  public:
  auto to_string() const {
    return fstr::format(R"( memory_buffer: char * p_start={}, p_end={}, int size={}
)", fmt::ptr(p_start), fmt::ptr(p_end), size);
  }
};

struct memory_stream : virtual memory_buffer, public std::istream
{
    memory_stream(char const * first_elem, size_t size)
        : memory_buffer(first_elem, size), std::istream(static_cast<std::streambuf*>(this)) {}
// Generated to_string for PUBLIC STRUCT_DECL memory_stream
  public:
  auto to_string() const {
    return fstr::format(R"( memory_stream: char * p_start={}, p_end={}, int size={}
)", fmt::ptr(this->p_start), fmt::ptr(this->p_end), this->size);
  }
};

class manual_timer
{
    std::chrono::high_resolution_clock::time_point t0;
    double timestamp{ 0.0 };
public:
    void start() { t0 = std::chrono::high_resolution_clock::now(); }
    void stop() { timestamp = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count() * 1000.0; }
    const double & get() { return timestamp; }
// Generated to_string for PUBLIC CLASS_DECL manual_timer
  public:
  auto to_string() const {
    return fstr::format(R"( manual_timer: int t0={}, double timestamp={}
)", t0, timestamp);
  }
};

struct float2 { float x, y; // Generated to_string for PUBLIC STRUCT_DECL float2
  public:
  auto to_string() const {
    return fstr::format(R"( float2: float x={}, y={}
)", x, y);
  }
};
struct float3 { float x, y, z; // Generated to_string for PUBLIC STRUCT_DECL float3
  public:
  auto to_string() const {
    return fstr::format(R"( float3: float x={}, y={}, z={}
)", x, y, z);
  }
};
struct double3 { double x, y, z; // Generated to_string for PUBLIC STRUCT_DECL double3
  public:
  auto to_string() const {
    return fstr::format(R"( double3: double x={}, y={}, z={}
)", x, y, z);
  }
};
struct uint3 { uint32_t x, y, z; // Generated to_string for PUBLIC STRUCT_DECL uint3
  public:
  auto to_string() const {
    return fstr::format(R"( uint3: int x={}, y={}, z={}
)", x, y, z);
  }
};
struct uint4 { uint32_t x, y, z, w; // Generated to_string for PUBLIC STRUCT_DECL uint4
  public:
  auto to_string() const {
    return fstr::format(R"( uint4: int x={}, y={}, z={}, w={}
)", x, y, z, w);
  }
};

struct geometry
{
    std::vector<float3> vertices;
    std::vector<float3> normals;
    std::vector<float2> texcoords;
    std::vector<uint3> triangles;
// Generated to_string for PUBLIC STRUCT_DECL geometry
  public:
  auto to_string() const {
    return fstr::format(R"( geometry: int vertices={}, normals={}, texcoords={}, triangles={}
)", vertices, normals, texcoords, triangles);
  }
};

inline geometry make_cube_geometry()
{
    geometry cube;

    const struct CubeVertex { float3 position, normal; float2 texCoord; } verts[] = {
    { { -1, -1, -1 },{ -1, 0, 0 },{ 0, 0 } },{ { -1, -1, +1 },{ -1, 0, 0 },{ 1, 0 } },{ { -1, +1, +1 },{ -1, 0, 0 },{ 1, 1 } },{ { -1, +1, -1 },{ -1, 0, 0 },{ 0, 1 } },
    { { +1, -1, +1 },{ +1, 0, 0 },{ 0, 0 } },{ { +1, -1, -1 },{ +1, 0, 0 },{ 1, 0 } },{ { +1, +1, -1 },{ +1, 0, 0 },{ 1, 1 } },{ { +1, +1, +1 },{ +1, 0, 0 },{ 0, 1 } },
    { { -1, -1, -1 },{ 0, -1, 0 },{ 0, 0 } },{ { +1, -1, -1 },{ 0, -1, 0 },{ 1, 0 } },{ { +1, -1, +1 },{ 0, -1, 0 },{ 1, 1 } },{ { -1, -1, +1 },{ 0, -1, 0 },{ 0, 1 } },
    { { +1, +1, -1 },{ 0, +1, 0 },{ 0, 0 } },{ { -1, +1, -1 },{ 0, +1, 0 },{ 1, 0 } },{ { -1, +1, +1 },{ 0, +1, 0 },{ 1, 1 } },{ { +1, +1, +1 },{ 0, +1, 0 },{ 0, 1 } },
    { { -1, -1, -1 },{ 0, 0, -1 },{ 0, 0 } },{ { -1, +1, -1 },{ 0, 0, -1 },{ 1, 0 } },{ { +1, +1, -1 },{ 0, 0, -1 },{ 1, 1 } },{ { +1, -1, -1 },{ 0, 0, -1 },{ 0, 1 } },
    { { -1, +1, +1 },{ 0, 0, +1 },{ 0, 0 } },{ { -1, -1, +1 },{ 0, 0, +1 },{ 1, 0 } },{ { +1, -1, +1 },{ 0, 0, +1 },{ 1, 1 } },{ { +1, +1, +1 },{ 0, 0, +1 },{ 0, 1 } }};

    std::vector<uint4> quads = { { 0, 1, 2, 3 },{ 4, 5, 6, 7 },{ 8, 9, 10, 11 },{ 12, 13, 14, 15 },{ 16, 17, 18, 19 },{ 20, 21, 22, 23 } };

    for (auto & q : quads)
    {
        cube.triangles.push_back({ q.x,q.y,q.z });
        cube.triangles.push_back({ q.x,q.z,q.w });
    }

    for (int i = 0; i < 24; ++i)
    {
        cube.vertices.push_back(verts[i].position);
        cube.normals.push_back(verts[i].normal);
        cube.texcoords.push_back(verts[i].texCoord);
    }

    return cube;
}

int main(int argc, char *argv[]) {
  using std::cout;
  cout << fmt::format("file: {}\ntime: {}\n", __FILE_NAME__, __TIMESTAMP__);

  geometry cube = make_cube_geometry();
  fmt::print(fmt::format(" cube={} ", cube));
  using namespace tinyply;

   PlyFile cube_file;

    cube_file.add_properties_to_element("vertex", { "x", "y", "z" },
        Type::FLOAT32, cube.vertices.size(), reinterpret_cast<uint8_t*>(cube.vertices.data()), Type::INVALID, 0);

    cube_file.add_properties_to_element("vertex", { "nx", "ny", "nz" },
        Type::FLOAT32, cube.normals.size(), reinterpret_cast<uint8_t*>(cube.normals.data()), Type::INVALID, 0);

    cube_file.add_properties_to_element("vertex", { "u", "v" },
        Type::FLOAT32, cube.texcoords.size() , reinterpret_cast<uint8_t*>(cube.texcoords.data()), Type::INVALID, 0);

    cube_file.add_properties_to_element("face", { "vertex_indices" },
        Type::UINT32, cube.triangles.size(), reinterpret_cast<uint8_t*>(cube.triangles.data()), Type::UINT8, 3);

    cube_file.get_comments().push_back("generated by tinyply 2.3");

  fmt::print(fmt::format(" cube_file.get_elements()={} ", cube_file.get_elements()));
  return EXIT_SUCCESS;
}