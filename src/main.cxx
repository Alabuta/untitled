#include <iostream>
#include <string>

using namespace std::string_literals;

#include <glm/glm.hpp>

#define VOLK_IMPLEMENTATION
#include <volk/volk.h>

#include <GLFW/glfw3.h>

#include <nlohmann/json.hpp>
#include <fmt/format.h>

#include <boost/cstdfloat.hpp>

int main()
{
    if (auto result = glfwInit(); result != GLFW_TRUE)
        throw std::runtime_error(fmt::format("failed to init GLFW: {0:#x}", result));

    if (auto result = volkInitialize(); result != VK_SUCCESS)
        throw std::runtime_error("failed to initialize 'volk' meta-loader");

    boost::float32_t f = 2;
    glm::vec2 vec{4, 8};
    nlohmann::json json;
    json["pi"s] = 3.141;

    fmt::print("Hello, World! {}:{}\nPI is {}\nf{}", vec.x, vec.y, json.dump(), f);

    return 0;
}
