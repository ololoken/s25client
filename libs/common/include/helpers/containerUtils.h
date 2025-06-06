// Copyright (C) 2005 - 2025 Settlers Freaks (sf-team at siedler25.org)
//
// SPDX-License-Identifier: GPL-2.0-or-later

///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <RTTR_Assert.h>
#include <algorithm>
#include <iterator>
#include <type_traits>

namespace helpers {

namespace detail {

    template<class T, typename = void>
    struct has_pop_front : std::false_type
    {};
    template<class T>
    struct has_pop_front<T, std::void_t<decltype(T::pop_front())>> : std::true_type
    {};
    template<class T>
    constexpr bool has_pop_front_v = has_pop_front<T>::value;

    template<class T, class U, typename = void>
    struct has_find : std::false_type
    {};

    template<class T, class U>
    struct has_find<T, U, std::void_t<decltype(std::declval<T>().find(std::declval<U>()))>> : std::true_type
    {};
    template<class T, class U>
    constexpr bool has_find_v = has_find<T, U>::value;
} // namespace detail

/// Removes an element from a container by its reverse iterator and returns an iterator to the next element
/// IMPORTANT: For containers other than list and set existing iterators may be invalidated!
template<typename T>
auto erase_reverse(T& container, typename T::reverse_iterator it)
{
    return typename T::reverse_iterator(container.erase((++it).base()));
}

template<typename T, typename T_Element>
void erase(T& container, T_Element&& element)
{
    using std::begin;
    using std::end;
    const auto it = std::remove(begin(container), end(container), std::forward<T_Element>(element));
    container.erase(it, end(container));
}

template<typename T, typename T_Predicate>
void erase_if(T& container, T_Predicate&& predicate)
{
    using std::begin;
    using std::end;
    const auto it = std::remove_if(begin(container), end(container), std::forward<T_Predicate>(predicate));
    container.erase(it, end(container));
}

/// Removes the first element in a container
template<typename T>
void pop_front(T& container)
{
    RTTR_Assert(!container.empty());
    if constexpr(detail::has_pop_front_v<T>)
        container.pop_front();
    else
        container.erase(container.begin());
}

/// Effective implementation of find. Uses the containers find function if available
template<typename T, typename U>
auto find(T& container, const U& value)
{
    if constexpr(detail::has_find_v<T, U>)
        return container.find(value);
    else
    {
        using std::begin;
        using std::end;
        return std::find(begin(container), end(container), value);
    }
}

template<typename T, class T_Predicate>
auto find_if(T& container, T_Predicate&& predicate)
{
    using std::begin;
    using std::end;
    return std::find_if(begin(container), end(container), std::forward<T_Predicate>(predicate));
}

/// Returns true if the container contains the given value
/// Uses the find member function if applicable otherwise uses the std::find method
template<typename T, typename U>
bool contains(const T& container, const U& value)
{
    using std::end;
    return find(container, value) != end(container);
}

/// Returns true if the container contains a value matching the predicate
template<typename T, class T_Predicate>
bool contains_if(const T& container, T_Predicate&& predicate)
{
    using std::end;
    return find_if(container, std::forward<T_Predicate>(predicate)) != end(container);
}

/// Count the number of occurrences of the given value. Returns an unsigned value
template<typename T, typename U>
size_t count(const T& container, const U& value)
{
    const auto result = std::count(container.begin(), container.end(), value);
    return static_cast<std::make_unsigned_t<decltype(result)>>(result);
}

/// Count the number of items for which the predicate returns true.
/// Returns an unsigned value
template<typename T, class T_Predicate>
size_t count_if(const T& container, T_Predicate&& predicate)
{
    const auto result = std::count_if(container.begin(), container.end(), std::forward<T_Predicate>(predicate));
    return static_cast<std::make_unsigned_t<decltype(result)>>(result);
}

/// Remove duplicate values from the given sorted container
template<class T>
void makeUniqueSorted(T& container)
{
    const auto newEnd = std::unique(begin(container), end(container));
    container.erase(newEnd, end(container));
}
/// Remove duplicate values from the given container, sorts it first
template<class T>
void makeUnique(T& container)
{
    std::sort(begin(container), end(container));
    makeUniqueSorted(container);
}
template<class T, class T_Predicate>
void makeUnique(T& container, T_Predicate&& predicate)
{
    std::sort(begin(container), end(container), std::forward<T_Predicate>(predicate));
    makeUniqueSorted(container);
}
/// Remove duplicate values from the given container without changing the order
template<class T>
void makeUniqueStable(T& container)
{
    // Containers with less than 2 elements are always unique
    if(container.size() < 2u)
        return;
    auto itInsert = begin(container);
    // We always begin inserting at 2nd pos so skip first
    ++itInsert;
    // And now start inserting all values starting from the 2nd
    for(auto it = itInsert; it != container.end(); ++it)
    {
        // If current element is not found in [begin, insertPos) then add it at insertPos and inc insertPos
        if(std::find(begin(container), itInsert, *it) == itInsert)
            *(itInsert++) = std::move(*it);
    }
    container.erase(itInsert, end(container));
}

/// Returns the index of the given element in the container or -1 when not found
template<class T_Container, class T_Element>
constexpr std::ptrdiff_t indexOf(const T_Container& container, const T_Element& element)
{
    std::ptrdiff_t index = 0;
    for(const auto& curEl : container)
    {
        if(curEl == element)
            return index;
        ++index;
    }
    return -1;
}

/// Returns the index of the first matching element in the container or -1 when not found
template<class T_Container, class T_Predicate>
constexpr std::ptrdiff_t indexOf_if(const T_Container& container, T_Predicate predicate)
{
    std::ptrdiff_t index = 0;
    for(const auto& curEl : container)
    {
        if(predicate(curEl))
            return index;
        ++index;
    }
    return -1;
}

} // namespace helpers
