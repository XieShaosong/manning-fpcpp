#ifndef MTRY_H
#define MTRY_H

#include "expected.h"

template < typename F
         , typename Ret = typename std::result_of<F()>::type
         , typename Exp = expected<Ret, std::exception_ptr>
         >
Exp mtry(F f)
{
    try {
        return Exp::success(f());
    } catch (...) {
        return Exp::error(std::current_exception());
    }
}



#endif /* !MTRY_H */
