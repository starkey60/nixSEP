#include <string>
#include <iostream>
#include <fstream>
#include <optional>

static std::optional<std::ofstream> output_file;

namespace magic {
void log_branch(std::string function_name, int index) {
    if (!output_file.has_value()) {
        const char* out = std::getenv("out");
        output_file.emplace(std::string(out) + "/mcov.out", std::ios::out | std::ios::app);
    }
    output_file.value() << function_name << " @ " << index << std::endl;
}
}
