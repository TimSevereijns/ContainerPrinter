#pragma once

#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace Traits
{
   /**
   * @brief P.F.M
   *
   * @see N3911
   */
   template<class...>
   using void_t = void;

   template<typename Type>
   using unqualified_t = std::remove_cv_t<std::remove_reference_t<Type>>;

   /**
   * Base case for the testing of STD compatible container types.
   */
   template<
      typename /*Type*/,
      typename = void
   >
   struct is_printable_as_container : std::false_type
   {
   };

   /**
   * @brief Specialization to ensure that Standard Library compatible containers that have
   * `begin()` and an `end()` member functions, as well as the `value_type` and `iterator` typedefs
   * are treated as printable containers.
   */
   template<typename Type>
   struct is_printable_as_container<
      Type,
      void_t<
         decltype(std::declval<Type&>().begin()),
         decltype(std::declval<Type&>().end()),
         typename Type::value_type,
         typename Type::iterator
      >
   > : public std::true_type
   {
   };

   /**
   * @brief Specialization to treat std::pair<...> as a printable container type.
   */
   template<
      typename FirstType,
      typename SecondType
   >
   struct is_printable_as_container<std::pair<FirstType, SecondType>> : public std::true_type
   {
   };

   /**
   * @brief Specialization to treat arrays as printable container types.
   */
   template<
      typename ArrayType,
      std::size_t ArraySize
   >
   struct is_printable_as_container<ArrayType[ArraySize]> : public std::true_type
   {
   };

   /**
   * @brief Narrow character array specialization in order to ensure that we print character arrays
   * as string and not as delimiter containers.
   */
   template<std::size_t ArraySize>
   struct is_printable_as_container<char[ArraySize]> : public std::false_type
   {
   };

   /**
   * @brief Wide character array specialization in order to ensure that we print character arrays
   * as string and not as delimiter containers.
   */
   template<std::size_t ArraySize>
   struct is_printable_as_container<wchar_t[ArraySize]> : public std::false_type
   {
   };

   /**
   * @brief String specialization in order to ensure that we treat strings as nothing more than strings.
   */
   template<
      typename CharacterType,
      typename TraitsType,
      typename AllocatorType
   >
   struct is_printable_as_container<
      std::basic_string<
      CharacterType,
         TraitsType,
         AllocatorType
      >
   > : public std::false_type
   {
   };

   /**
   * @brief Helper variable template.
   */
   template<typename Type>
   constexpr bool is_printable_as_container_v = is_printable_as_container<Type>::value;
}

namespace Printer
{
   /**
   * Convenience struct to neatly wrap up all the additional characters we'll need in order to
   * print out the containers.
   */
   template<typename DelimiterType>
   struct delimiter_values
   {
      using type = DelimiterType;

      const type* prefix;
      const type* delimiter;
      const type* postfix;
   };

   /**
   *
   */
   template<
      typename /*ContainerType*/,
      typename DelimiterType
   >
   struct delimiters
   {
      using type = delimiter_values<DelimiterType>;

      static const type values;
   };

   /**
   * Default narrow delimiters for any container type that isn't specialized.
   */
   template<typename ContainerType>
   struct delimiters<ContainerType, char>
   {
      using type = delimiter_values<char>;

      static constexpr type values = { "[", ", ", "]" };
   };

   /**
   * Default wide delimiters for any container type that isn't specialized.
   */
   template<typename ContainerType>
   struct delimiters<ContainerType, wchar_t>
   {
      using type = delimiter_values<wchar_t>;

      static constexpr type values = { L"[", L", ", L"]" };
   };

   /**
   * Narrow character specialization for std::set<...>.
   */
   template<
      typename Type,
      typename ComparatorType,
      typename AllocatorType
   >
   struct delimiters<std::set<Type, ComparatorType, AllocatorType>, char>
   {
      static constexpr delimiter_values<char> values = { "{", ", ", "}" };
   };

   /**
   * Wide character specialization for std::set<...>.
   */
   template<
      typename Type,
      typename ComparatorType,
      typename AllocatorType
   >
   struct delimiters<std::set<Type, ComparatorType, AllocatorType>, wchar_t>
   {
      static constexpr delimiter_values<wchar_t> values = { L"{", L", ", L"}" };
   };

   /**
   * Narrow character specialization for std::pair<...>.
   */
   template<
      typename FirstType,
      typename SecondType
   >
   struct delimiters<std::pair<FirstType, SecondType>, char>
   {
      static constexpr delimiter_values<char> values = { "(", ", ", ")" };
   };

   /**
   * Wide character specialization for std::pair<...>.
   */
   template<
      typename FirstType,
      typename SecondType
   >
   struct delimiters<std::pair<FirstType, SecondType>, wchar_t>
   {
      static constexpr delimiter_values<wchar_t> values = { L"(", L", ", L")" };
   };

   /**
   * Narrow character specialization for std::tuple<...>.
   */
   template<typename... Types>
   struct delimiters<std::tuple<Types...>, char>
   {
      static constexpr delimiter_values<char> values = { "<", ", ", ">" };
   };

   /**
   * Wide character specialization for std::tuple<...>.
   */
   template<typename... Types>
   struct delimiters<std::tuple<Types...>, wchar_t>
   {
      static constexpr delimiter_values<wchar_t> values = { L"<", L", ", L">" };
   };

   /**
   *
   */
   template<
      typename ContainerType,
      typename CharacterType,
      typename TraitsType
   >
   void PrintingHelper(
      std::basic_ostream<CharacterType, TraitsType>& stream,
      const ContainerType& container)
   {
      static constexpr auto delimiters = Printer::delimiters<ContainerType, CharacterType>::values;

      if (std::distance(std::begin(container), std::end(container)) == 0)
      {
         return;
      }

      stream << delimiters.prefix;

      std::for_each(std::begin(container), std::end(container) - 1,
         [&stream](const auto& value)
      {
         stream << value << delimiters.delimiter;
      });

      stream << container.back() << delimiters.postfix;
   }

   /**
   *
   */
   template<
      typename FirstType,
      typename SecondType,
      typename CharacterType,
      typename TraitsType
   >
   void PrintingHelper(
      std::basic_ostream<CharacterType, TraitsType>& stream,
      const std::pair<FirstType, SecondType>& container)
   {
      static constexpr auto delimiters =
         Printer::delimiters<
            Traits::unqualified_t<decltype(container)>,
            CharacterType
         >::values;

      stream
         << delimiters.prefix
         << container.first
         << delimiters.delimiter
         << container.second
         << delimiters.postfix;
   }

   /**
   *
   */
   template<
      typename ContainerType,
      typename CharacterType,
      typename TraitsType
   >
   class ContainerPrinter
   {
   public:
      ContainerPrinter(const ContainerType& container)
         : m_container{ container }
      {
      }

      inline void PrintTo(std::basic_ostream<CharacterType, TraitsType>& stream) const
      {
         PrintingHelper(stream, m_container);
      }

   private:
      const ContainerType& m_container;
   };
}

/**
*
*/
template<
   typename ContainerType,
   typename CharacterType,
   typename TraitsType,
   typename = std::enable_if_t<Traits::is_printable_as_container_v<ContainerType>>
>
auto& operator<<(
   std::basic_ostream<CharacterType, TraitsType>& stream,
   const ContainerType& container)
{
   Printer::ContainerPrinter<ContainerType, CharacterType, TraitsType>(container).PrintTo(stream);

   return stream;
}