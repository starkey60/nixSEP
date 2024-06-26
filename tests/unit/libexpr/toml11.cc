#include "tests/libexpr.hh"
#include <toml11/toml/region.hpp>
#include <toml11/toml/parser.hpp>

namespace nix {
class TomlParserTest : public LibExprTest {};

MATCHER(IsErr, "") {
    return !arg.is_ok();
}

MATCHER(IsOk, "") {
    return arg.is_ok();
}

MATCHER_P(IsCPPStringEq, s, fmt("The string is equal to \"%1%\"", s)) {
    return arg == s;
}

MATCHER_P(IsCPPIntEq, d, fmt("The int is equal to \"%1%\"", d)) {
    return arg == d;
}

TEST_F(TomlParserTest, not_bool) {
    auto loc = toml::detail::location("test_not_bool", "foo");
    auto v = toml::detail::parse_boolean(loc);
    ASSERT_THAT(v, IsErr());
}

TEST_F(TomlParserTest, escape_sequence) {
    {
        auto loc = toml::detail::location("test_escape_sequence_backslash", "\\\\");
        auto v = toml::detail::parse_escape_sequence(loc);
        ASSERT_THAT(v, IsOk());
        ASSERT_THAT(v.unwrap(), IsCPPStringEq("\\"));
    }
    {
        auto loc = toml::detail::location("test_escape_sequence_b", "\\b");
        auto v = toml::detail::parse_escape_sequence(loc);
        ASSERT_THAT(v, IsOk());
        ASSERT_THAT(v.unwrap(), IsCPPStringEq("\b"));
    }
    {
        auto loc = toml::detail::location("test_escape_sequence_f", "\\f");
        auto v = toml::detail::parse_escape_sequence(loc);
        ASSERT_THAT(v, IsOk());
        ASSERT_THAT(v.unwrap(), IsCPPStringEq("\f"));
    }
    {
        auto loc = toml::detail::location("test_escape_sequence_r", "\\r");
        auto v = toml::detail::parse_escape_sequence(loc);
        ASSERT_THAT(v, IsOk());
        ASSERT_THAT(v.unwrap(), IsCPPStringEq("\r"));
    }

    {
        auto loc = toml::detail::location("test_escape_sequence_invalid_utf8_codepoint", "\\ua");
        auto v = toml::detail::parse_escape_sequence(loc);
        ASSERT_THAT(v, IsErr());
    }

    {
        auto loc = toml::detail::location("test_escape_sequence_A", "\\U00000041");
        auto v = toml::detail::parse_escape_sequence(loc);
        ASSERT_THAT(v, IsOk());
        ASSERT_THAT(v.unwrap(), IsCPPStringEq("A"));
    }

    {
        auto loc = toml::detail::location("test_not_escape_sequence_A", "\\Ua");
        auto v = toml::detail::parse_escape_sequence(loc);
        ASSERT_THAT(v, IsErr());
    }

    {
        auto loc = toml::detail::location("test_not_valid_escape_sequence", "\\C");
        auto v = toml::detail::parse_escape_sequence(loc);
        ASSERT_THAT(v, IsErr());
    }
}

TEST_F(TomlParserTest, binary_integer_hole) {
    auto loc = toml::detail::location("test_binary_integer_hole", "0b00_01");
    auto v = toml::detail::parse_binary_integer(loc);
    ASSERT_THAT(v, IsOk());
    ASSERT_THAT(std::get<0>(v.unwrap()), IsCPPIntEq(1));
}

TEST_F(TomlParserTest, not_binary_integer) {
    auto loc = toml::detail::location("test_not_binary_integer", "foo");
    auto v = toml::detail::parse_binary_integer(loc);
    ASSERT_THAT(v, IsErr());
}

TEST_F(TomlParserTest, not_octal_integer) {
    auto loc = toml::detail::location("test_not_octal_integer", "foo");
    auto v = toml::detail::parse_octal_integer(loc);
    ASSERT_THAT(v, IsErr());
}

TEST_F(TomlParserTest, not_hexadecimal_integer) {
    auto loc = toml::detail::location("test_not_hexadecimal_integer", "foo");
    auto v = toml::detail::parse_hexadecimal_integer(loc);
    ASSERT_THAT(v, IsErr());
}

TEST_F(TomlParserTest, integer_zero_zero) {
    auto loc = toml::detail::location("test_integer_zero_zero", "00");
    auto v = toml::detail::parse_integer(loc);
    ASSERT_THAT(v, IsErr());
}

TEST_F(TomlParserTest, integer_zero_num) {
    auto loc = toml::detail::location("test_integer_zero_num", "01");
    auto v = toml::detail::parse_integer(loc);
    ASSERT_THAT(v, IsErr());
}

TEST_F(TomlParserTest, integer_zero_alpha) {
    auto loc = toml::detail::location("test_integer_zero_alpha", "0c");
    auto v = toml::detail::parse_integer(loc);
    ASSERT_THAT(v, IsErr());
}

TEST_F(TomlParserTest, not_integer) {
    auto loc = toml::detail::location("test_not_integer", "foo");
    auto v = toml::detail::parse_integer(loc);
    ASSERT_THAT(v, IsErr());
}

TEST_F(TomlParserTest, not_floating) {
    auto loc = toml::detail::location("test_not_float", "foo");
    auto v = toml::detail::parse_floating(loc);
    ASSERT_THAT(v, IsErr());
}

TEST_F(TomlParserTest, floating_inf) {
    auto loc = toml::detail::location("test_float_inf", "inf");
    auto v = toml::detail::parse_floating(loc);
    ASSERT_THAT(v, IsOk());
}

TEST_F(TomlParserTest, floating_minf) {
    auto loc = toml::detail::location("test_float_minf", "-inf");
    auto v = toml::detail::parse_floating(loc);
    ASSERT_THAT(v, IsOk());
}

TEST_F(TomlParserTest, floating_nan) {
    auto loc = toml::detail::location("test_float_nan", "nan");
    auto v = toml::detail::parse_floating(loc);
    ASSERT_THAT(v, IsOk());
}

TEST_F(TomlParserTest, floating_mnan) {
    auto loc = toml::detail::location("test_float_mnan", "-nan");
    auto v = toml::detail::parse_floating(loc);
    ASSERT_THAT(v, IsOk());
}
}
