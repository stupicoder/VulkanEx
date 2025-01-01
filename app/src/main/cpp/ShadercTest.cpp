//
// Created by huhcm on 2025-01-01.
//

#include <sstream>
#include <string>
#include <string_view>
#include <gtest/gtest.h>
#include <shaderc/shaderc.hpp>

TEST(shaderc, compile) {
    constexpr std::string_view code{
        "#version 310 es\n"
        "void main() {"
        "}"
    };

    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    auto result = compiler.CompileGlslToSpvAssembly(
            code.data(), code.size(), shaderc_vertex_shader, "test", options);
    EXPECT_EQ(result.GetCompilationStatus(), shaderc_compilation_status_success);

    std::string spv(result.cbegin(), result.cend());
    std::istringstream iss(spv);
    std::string header;
    std::getline(iss, header);
    EXPECT_EQ(header, "; SPIR-V");
}

