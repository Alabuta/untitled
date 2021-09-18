#include <exception>

#include <string>
using namespace std::string_literals;

#include <string_view>
using namespace std::string_view_literals;

#include <glm/glm.hpp>
#include <fmt/format.h>
#include <GLFW/glfw3.h>
#include <volk/volk.h>
#include <nlohmann/json.hpp>

int main()
{
    if (auto result = glfwInit(); result != GLFW_TRUE)
        throw std::runtime_error(fmt::format("failed to init GLFW: {0:#x}", result));

    if (auto result = volkInitialize(); result != VK_SUCCESS)
        throw std::runtime_error("failed to initialize 'volk' meta-loader"s);

    glm::vec2 vec{4, 8};
    nlohmann::json json;
    json["pi"s] = 3.141;
    fmt::print("Hello, World! {}:{}\nPI is {}", vec.x, vec.y, json.dump());

    return 0;
}
