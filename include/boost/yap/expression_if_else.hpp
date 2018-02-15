// Copyright (C) 2016-2018 T. Zachary Laine
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_YAP_EXPRESSION_IF_ELSE_HPP_INCLUDED
#define BOOST_YAP_EXPRESSION_IF_ELSE_HPP_INCLUDED


namespace boost { namespace yap {

#ifndef BOOST_YAP_DOXYGEN

    BOOST_YAP_USER_EXPR_IF_ELSE(::boost::yap::expression)

#else

    /** \see BOOST_YAP_USER_EXPR_IF_ELSE for full semantics. */
    template <typename Expr1, typename Expr2, typename Expr3>
    auto if_else (Expr1 && expr1, Expr2 && expr2, Expr3 && expr3);

#endif

} }

#endif
