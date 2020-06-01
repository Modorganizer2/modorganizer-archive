#ifndef ARCHIVE_FORMAT_H
#define ARCHIVE_FORMAT_H

// This header specialize formatter() for some useful types. It should not be
// exposed outside of the library. It also contains some useful methods for
// string manipulation.

#include <fmt/format.h>

#include <cwctype>
#include <filesystem>
#include <stdexcept>
#include <string>

// Specializing fmt::formatter works, but gives warning, for whatever reason... So putting
// everything in the namespace.
namespace fmt {

  // I don't know why fmt does not provide this...
  template<>
  struct formatter<std::string, wchar_t> : formatter<std::wstring, wchar_t>
  {
    template<typename FormatContext>
    auto format(std::string const& s, FormatContext& ctx) {
      return formatter<std::wstring, wchar_t>::format(std::wstring(s.begin(), s.end()), ctx);
    }
  };

  template<>
  struct formatter<std::exception, wchar_t> : formatter<std::string, wchar_t>
  {
    template<typename FormatContext>
    auto format(std::exception const& ex, FormatContext& ctx) {
      return formatter<std::string, wchar_t>::format(ex.what(), ctx);
    }
  };

  template<>
  struct formatter<std::error_code, wchar_t> : formatter<std::string, wchar_t>
  {
    template<typename FormatContext>
    auto format(std::error_code const& ec, FormatContext& ctx) {
      return formatter<std::string, wchar_t>::format(ec.message(), ctx);
    }
  };

  template<>
  struct formatter<std::filesystem::path, wchar_t> : formatter<std::wstring, wchar_t>
  {
    template<typename FormatContext>
    auto format(std::filesystem::path const& path, FormatContext& ctx) {
      return formatter<std::wstring, wchar_t>::format(path.native(), ctx);
    }
  };

}

namespace NArchive {

  /**
   * @brief Join the element of the given container using the given separator.
   *
   * @param c The container. Must be satisfy standard container requirements.
   * @param sep The separator.
   *
   * @return a string containing the element joint, or an empty string if c
   *     is empty.
   */
  template <class C>
  std::wstring join(C const& c, std::wstring const& sep) {
    auto begin = std::begin(c), end = std::end(c);

    if (begin == end) {
      return {};
    }
    std::wstring r = *begin++;
    for (; begin != end; ++begin) {
      r += *begin + sep;
    }

    return r;
  }

  /**
   * @brief Conver the given string to lowercase.
   *
   * @param s The string to convert.
   *
   * @return the converted string.
   */
  inline std::wstring towlower(std::wstring s) {
    std::transform(std::begin(s), std::end(s),
      std::begin(s), [](wchar_t c) { return static_cast<wchar_t>(::towlower(c)); });
    return s;
  }
}


#endif