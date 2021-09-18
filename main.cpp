#include <iostream>

#include <glm/glm.hpp>
#include <fmt/format.h>

int main()
{
    glm::vec2 vec{4, 8};
    fmt::print("Hello, World! {}:{}\n", vec.x, vec.y);

    return 0;
}
