#include <string>
using namespace std::string_literals;

#include <string_view>
using namespace std::string_view_literals;

#include <glm/glm.hpp>
#include <fmt/format.h>
#include <nlohmann/json.hpp>

int main()
{
    glm::vec2 vec{4, 8};
    nlohmann::json json;
    json["pi"s] = 3.141;
    fmt::print("Hello, World! {}:{}\nPI is {}", vec.x, vec.y, json.dump());

    return 0;
}
