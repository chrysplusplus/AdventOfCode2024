/* Header for shared components across Advent of Code */

#ifndef _SHARED_H

#include <iostream>
#include <string>

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

#endif // !_SHARED_H
