/* Header for shared components across Advent of Code */

#ifndef _SHARED_H

#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <cstdint>

using Stm = std::basic_istream<char>;

Stm& stmNext(
    Stm& stream,
    std::string *str = nullptr,
    char delim = '\n')
{
  if (str) str->erase();
  for (char c{}; stream.get(c); )
  {
    if (c == delim) break;
    if (str) *(str) += c;
  }
  return stream;
}

constexpr
std::pair<std::string_view, std::string_view>
svSplitFirst(std::string_view sv, char separator) noexcept
{
  auto it = std::begin(sv);
  for ( ; it != std::end(sv); ++it)
    if (*it == separator)
      break;

  if (it == std::end(sv)) {
    return { sv, {} };
  }

  if (it + 1 == std::end(sv)) {
    return { {std::begin(sv), it}, {} };
  }

  return { {std::begin(sv), it}, {it + 1, std::end(sv)} };
}

template <class IntType = uint64_t>
constexpr
std::tuple<bool, IntType, std::string_view>
svFormatToInt(std::string_view sv) noexcept
{
  IntType value{};
  auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
  if (ec != std::errc())
    return {false, {}, sv};
  return {true, value, {ptr, std::end(sv)}};
}

#endif // !_SHARED_H
