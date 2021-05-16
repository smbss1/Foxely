#ifndef FOX_COMMON_HPP_
#define FOX_COMMON_HPP_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <sstream>
#include <fstream>
#include <memory>
#include <sys/stat.h>
#include <functional>
#include <map>
#include "Benchmark.hpp"

class VM;
class Value;

using NativeFn = std::function<Value(VM*, int, Value*)>;
using NativeMethods = std::map<std::string, NativeFn>;

/*
* @brief Cette fonction permet de hasher la string passé en paramètre
* @param str la chaine de caractère qui sera hasher
* @return un nombre unique qui correspond à la position de la string dans le tableau
* @note Hasher veut dire produire un identifiant unique crypté
*/
uint32_t hashString(const std::string& str);


// Assertions are used to validate program invariants. They indicate things the
// program expects to be true about its internal state during execution. If an
// assertion fails, there is a bug in Wren.
//
// Assertions add significant overhead, so are only enabled in debug builds.
#ifdef DEBUG

    #include <stdio.h>

    #define FOX_ASSERT(condition, message)                                           \
        do                                                                       \
        {                                                                        \
            if (!(condition))                                                      \
            {                                                                      \
                fprintf(stderr, "[%s:%d] Assert failed in %s(): %s\n",               \
                    __FILE__, __LINE__, __func__, message);                          \
                abort();                                                             \
            }                                                                      \
        } while (false)

#else

  #define FOX_ASSERT(condition, message)

#endif

template<typename T>
using scope = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr scope<T> new_scope(Args&& ... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using ref = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr ref<T> new_ref(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

#endif
