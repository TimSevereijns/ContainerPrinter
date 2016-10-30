#pragma once

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "Catch.hpp"

#include "../ContainerPrinter/ContainerPrinter.hpp"

#include <algorithm>
#include <list>
#include <set>
#include <vector>

namespace
{
   template<typename Type>
   class VectorWrapper : public std::vector<Type> { };
}

TEST_CASE("Iterable Container Detection")
{
   SECTION("Detect std::vector<...> as being an iterable container type.")
   {
      constexpr auto isAContainer = Traits::is_printable_as_container_v<std::vector<int>>;
      REQUIRE(isAContainer == true);
   }

   SECTION("Detect std::list<...> as being an iterable container type.")
   {
      constexpr auto isAContainer = Traits::is_printable_as_container_v<std::list<int>>;
      REQUIRE(isAContainer == true);
   }

   SECTION("Detect std::set<...> as being an iterable container type.")
   {
      constexpr auto isAContainer = Traits::is_printable_as_container_v<std::set<int>>;
      REQUIRE(isAContainer == true);
   }

   SECTION("Detect array as being an iterable container type.")
   {
      constexpr auto isAContainer = Traits::is_printable_as_container_v<int[10]>;
      REQUIRE(isAContainer == true);
   }

   SECTION("Detect inherited iterable container type.")
   {
      constexpr auto isAContainer = Traits::is_printable_as_container_v<VectorWrapper<int>>;
      REQUIRE(isAContainer == true);
   }

   SECTION("Detect std::string as a type that shouldn't be iterated over.")
   {
      constexpr auto isAContainer = Traits::is_printable_as_container_v<std::string>;
      REQUIRE(isAContainer == false);
   }

   SECTION("Detect std::wstring as a type that shouldn't be iterated over.")
   {
      constexpr auto isAContainer = Traits::is_printable_as_container_v<std::wstring>;
      REQUIRE(isAContainer == false);
   }
}

TEST_CASE("Delimiters")
{
   SECTION("Verify narrow character delimiters for a non-specialized container type.")
   {
      constexpr auto delimiters = Printer::delimiters<char[], char>::values;
      REQUIRE(delimiters.prefix == "[");
      REQUIRE(delimiters.delimiter == ", ");
      REQUIRE(delimiters.postfix == "]");
   }

   SECTION("Verify wide character delimiters for a non-specialized container type.")
   {
      constexpr auto delimiters = Printer::delimiters<wchar_t[], wchar_t>::values;
      REQUIRE(delimiters.prefix == L"[");
      REQUIRE(delimiters.delimiter == L", ");
      REQUIRE(delimiters.postfix == L"]");
   }

   SECTION("Verify narrow character delimiters for a std::set<...>.")
   {
      constexpr auto delimiters = Printer::delimiters<std::set<int>, char>::values;
      REQUIRE(delimiters.prefix == "{");
      REQUIRE(delimiters.delimiter == ", ");
      REQUIRE(delimiters.postfix == "}");
   }

   SECTION("Verify wide character delimiters for a std::set<...>.")
   {
      constexpr auto delimiters = Printer::delimiters<std::set<int>, wchar_t>::values;
      REQUIRE(delimiters.prefix == L"{");
      REQUIRE(delimiters.delimiter == L", ");
      REQUIRE(delimiters.postfix == L"}");
   }

   SECTION("Verify narrow character delimiters for a std::pair<...>.")
   {
      constexpr auto delimiters = Printer::delimiters<std::pair<int, int>, char>::values;
      REQUIRE(delimiters.prefix == "(");
      REQUIRE(delimiters.delimiter == ", ");
      REQUIRE(delimiters.postfix == ")");
   }

   SECTION("Verify wide character delimiters for a std::pair<...>.")
   {
      constexpr auto delimiters = Printer::delimiters<std::pair<int, int>, wchar_t>::values;
      REQUIRE(delimiters.prefix == L"(");
      REQUIRE(delimiters.delimiter == L", ");
      REQUIRE(delimiters.postfix == L")");
   }

   SECTION("Verify narrow character delimiters for a std::tuple<...>.")
   {
      constexpr auto delimiters = Printer::delimiters<std::tuple<int, int>, char>::values;
      REQUIRE(delimiters.prefix == "<");
      REQUIRE(delimiters.delimiter == ", ");
      REQUIRE(delimiters.postfix == ">");
   }

   SECTION("Verify wide character delimiters for a std::tuple<...>.")
   {
      constexpr auto delimiters = Printer::delimiters<std::tuple<int, int>, wchar_t>::values;
      REQUIRE(delimiters.prefix == L"<");
      REQUIRE(delimiters.delimiter == L", ");
      REQUIRE(delimiters.postfix == L">");
   }
}

TEST_CASE("Container Printing")
{
   SECTION("Printing a std::pair<...>.")
   {
      const auto pair = std::make_pair(10, 100);

      std::cout << pair << std::endl;
   }

   SECTION("Printing a std::vector<...>.")
   {
      const std::vector<int> vector { 1, 2, 3, 4 };

      std::cout << vector << std::endl;
   }
}