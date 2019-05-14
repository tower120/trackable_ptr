#pragma once

namespace detail{
    extern void require_failed(char const *expr, char const *file, int line){
        std::cerr
            << file << ':' << line
            << " REQUIRE \"" << expr << "\" " << "failed"
            << std::endl;
        std::abort();
    }
}

# define REQUIRE(expr)				\
  ((expr)							\
   ? (void)(0)						\
   : ::detail::require_failed (#expr, __FILE__, __LINE__))
