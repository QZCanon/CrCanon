//
// Created by qinzhou on 23-4-21.
//

#ifndef REFILE_H
#define REFILE_H

#include <iostream>
#include <cxxabi.h>

const char* DemangleAlloc(char const* name)
{
    int status = 0;
    std::size_t size = 0;
    return abi::__cxa_demangle(name, nullptr, &size, &status);
}

template <class T>
std::string TypeName()
{
    char* demang = const_cast<char*>(DemangleAlloc(typeid(T).name()));
    std::string name(demang);
    free(demang);
    if constexpr (std::is_const_v<std::remove_reference_t<T>>) {
        name += " const";
    }
    if constexpr (std::is_rvalue_reference_v<std::remove_reference_t<T>>) {
        name += " &&";
    } else if constexpr (std::is_lvalue_reference_v<T>) {
        name += " &";
    }
    return name;
}

#endif // REFILE_H
