/*
 * Seven Kingdoms: Ambition
 *
 * Copyright 2026 Tim Sviridov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 *
 * Header file for Ambition::ErrorHandling.
 */

#pragma once

#include <boost/exception/diagnostic_information.hpp>
#include <boost/throw_exception.hpp>
#include <cstdlib>
#include <exception>
#include <format>
#include <iostream>
#include <source_location>
#include <stdexcept>
#include <string_view>


namespace Ambition::ErrorHandling {

/**
 * This is an assertion (assumption) error.  Do not throw, or try to recover
 * from, this.
 */
class AssertError : public std::exception {
public:
  AssertError(
    std::string_view expression,
    std::string_view message = {},
    const std::source_location location = std::source_location::current()
  )
    : expression(expression)
    , message(message)
    , location(location)
  { }

  const char* what() const noexcept override {
    if (generated.empty()) {
      generated = std::format(
        "Assertion `{}` failed at {}:{}:{} in function {}",
        expression,
        location.file_name(),
        location.line(),
        location.column(),
        location.function_name()
      );

      if (!message.empty()) {
        generated += ": ";
        generated += message;
      }
    }

    return generated.c_str();
  }

private:
  const std::string expression;
  const std::string message;
  const std::source_location location;
  mutable std::string generated;
};


namespace Exceptions {

class newer_type_version : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;

  newer_type_version(
    std::string_view typeName,
    const int archiveVersion,
    const int currentVersion
  )
    : std::runtime_error::runtime_error(
      std::format(
        "{} archive version {} is newer than the current version {}.",
        typeName,
        archiveVersion,
        currentVersion
      )
    )
  { }
};

} // namespace Ambition::ErrorHandling::Exceptions


inline void console_reporter(
  std::string_view message
) noexcept {
  std::cerr << message << '\n' << std::flush;
}

using Reporter = void (*)(std::string_view) noexcept;
constexpr Reporter default_reporter = console_reporter;
inline Reporter error_reporter = default_reporter;

inline void terminationHandler() noexcept {
  auto exception = std::current_exception();
  if (error_reporter && exception) {
    try {
      std::rethrow_exception(exception);
    } catch (const AssertError& e) {
      error_reporter(e.what());
    } catch (const boost::exception& e) {
      error_reporter(
        std::string("Unhandled exception\n") + boost::diagnostic_information(e)
      );
    } catch (const std::exception& e) {
      error_reporter(std::string("Unhandled exception: ") + e.what());
    } catch (...) {
      error_reporter("Unhandled unknown thrown object");
    }
  }

  std::abort();
}

inline void set_up_error_reporting(
  Reporter reporter = default_reporter
) noexcept {
  error_reporter = reporter;
  std::set_terminate(&terminationHandler);
}

#ifdef NDEBUG

# define assume(expr, ...) (static_cast<void>(sizeof(expr)))

#else

[[noreturn]]
inline void assumption_broken(
  std::string_view expression,
  std::string_view message = {},
  const std::source_location location = std::source_location::current()
) noexcept {
  /* Create an AssertError as the current exception before terminating, so that
   * terminationHandler() has something to look at.
   *
   * We throw and catch the exception immediately to avoid unwinding the stack
   * for real, so that the application state is preserved for debugging. */
  try { throw AssertError(expression, message, location); }
  catch (...) { std::terminate(); }
}

/**
 * Assume (assert) an expression to be true.  Otherwise, terminate the program.
 *
 * This serves the role of `assert`, but does not use the same name in order to
 * not conflict with other implementations.
 */
#define assume(expression, ...) \
  (static_cast<bool>(expression) \
    ? static_cast<void>(0) \
    : Ambition::ErrorHandling::assumption_broken( \
      #expression __VA_OPT__(, __VA_ARGS__) \
    ) \
  )

#endif


using errinfo_expression = boost::error_info<struct tag_expression, const char*>;

#define enforce(expression, exception) \
  (static_cast<bool>(expression) \
    ? static_cast<void>(0) \
    : BOOST_THROW_EXCEPTION( \
      boost::enable_error_info(exception) \
        << Ambition::ErrorHandling::errinfo_expression(#expression) \
    ) \
  )

} // namespace Ambition::ErrorHandling
