#include <boost/yap/expression.hpp>
#include <boost/yap/expression_free_operators.hpp>
#include <boost/yap/expression_if_else.hpp>

#include <gtest/gtest.h>


template <typename T>
using term = boost::yap::terminal<boost::yap::expression, T>;

template <typename T>
using ref = boost::yap::expression_ref<boost::yap::expression, T>;

namespace yap = boost::yap;
namespace bh = boost::hana;


namespace user {

    struct number
    {
        double value;
    };

    struct eval_xform
    {
        auto operator() (yap::terminal_tag, number const & n)
        { return n; }

        template <typename Expr>
        decltype(auto) operator() (yap::expression<yap::expr_kind::negate, bh::tuple<Expr>> const & expr)
        {
            number const n = transform(yap::value(expr), *this);
            return number{-n.value};
        }

        template <typename Expr1, typename Expr2>
        decltype(auto) operator() (yap::expression<yap::expr_kind::plus, bh::tuple<Expr1, Expr2>> const & expr)
        {
            number const lhs = transform(yap::left(expr), *this);
            number const rhs = transform(yap::right(expr), *this);
            return number{lhs.value + rhs.value};
        }
    };

}

template <typename Expr>
auto make_ref (Expr && expr)
{
    using type = yap::detail::operand_type_t<yap::expression, Expr>;
    //int i = yap::detail::make_operand<type>{};
    return yap::detail::make_operand<type>{}(static_cast<Expr &&>(expr));
}

TEST(just_negate, test_user_expression_transform_4)
{
    {
        term<user::number> a{{1.0}};

        {
            user::number result = transform(a, user::eval_xform{});
            EXPECT_EQ(result.value, 1);
        }

        {
            user::number result = transform(make_ref(a), user::eval_xform{});
            EXPECT_EQ(result.value, 1);
        }

        {
            user::number result = transform(-a, user::eval_xform{});
            EXPECT_EQ(result.value, -1);
        }

        {
            auto expr = make_ref(a);
            user::number result = transform(-expr, user::eval_xform{});
            EXPECT_EQ(result.value, -1);
        }

        {
            auto expr = -a;
            user::number result = transform(expr, user::eval_xform{});
            EXPECT_EQ(result.value, -1);
        }

        {
            auto expr1 = make_ref(a);
            auto expr2 = make_ref(expr1);
            user::number result = transform(expr2, user::eval_xform{});
            EXPECT_EQ(result.value, 1);
        }

        {
            auto expr1 = -a;
            auto expr2 = make_ref(expr1);
            user::number result = transform(expr2, user::eval_xform{});
            EXPECT_EQ(result.value, -1);
        }

        {
            auto expr1 = make_ref(a);
            auto expr2 = -expr1;
            user::number result = transform(expr2, user::eval_xform{});
            EXPECT_EQ(result.value, -1);
        }

        {
            auto expr1 = a;
            auto expr2 = make_ref(expr1);
            auto expr3 = make_ref(expr2);
            user::number result = transform(expr3, user::eval_xform{});
            EXPECT_EQ(result.value, 1);
        }

        {
            auto expr1 = -a;
            auto expr2 = make_ref(expr1);
            auto expr3 = make_ref(expr2);
            user::number result = transform(expr3, user::eval_xform{});
            EXPECT_EQ(result.value, -1);
        }

        {
            auto expr1 = make_ref(a);
            auto expr2 = -expr1;
            auto expr3 = make_ref(expr2);
            user::number result = transform(expr3, user::eval_xform{});
            EXPECT_EQ(result.value, -1);
        }

        {
            auto expr1 = make_ref(a);
            auto expr2 = make_ref(expr1);
            auto expr3 = -expr2;
            user::number result = transform(expr3, user::eval_xform{});
            EXPECT_EQ(result.value, -1);
        }
    }

    {
        user::number result = transform(-term<user::number>{{1.0}}, user::eval_xform{});
        EXPECT_EQ(result.value, -1);
    }
}

TEST(negate_in_plus, test_user_expression_transform_4)
{
    term<user::number> a{{1.0}};
    term<user::number> x{{41.0}};

    {
        user::number result = transform(a + x, user::eval_xform{});
        EXPECT_EQ(result.value, 42);
    }


    {
        user::number result = transform(make_ref(a) + make_ref(x), user::eval_xform{});
        EXPECT_EQ(result.value, 42);
    }

    {
        user::number result = transform(make_ref(a) + x, user::eval_xform{});
        EXPECT_EQ(result.value, 42);
    }

    {
        user::number result = transform(a + make_ref(x), user::eval_xform{});
        EXPECT_EQ(result.value, 42);
    }

    {
        user::number result = transform(a + x, user::eval_xform{});
        EXPECT_EQ(result.value, 42);
    }


    {
        user::number result = transform(-make_ref(a) + make_ref(x), user::eval_xform{});
        EXPECT_EQ(result.value, 40);
    }

    {
        user::number result = transform(-make_ref(a) + x, user::eval_xform{});
        EXPECT_EQ(result.value, 40);
    }

    {
        user::number result = transform(-a + make_ref(x), user::eval_xform{});
        EXPECT_EQ(result.value, 40);
    }

    {
        user::number result = transform(-a + x, user::eval_xform{});
        EXPECT_EQ(result.value, 40);
    }


    {
        user::number result = transform(make_ref(a) + -make_ref(x), user::eval_xform{});
        EXPECT_EQ(result.value, -40);
    }

    {
        user::number result = transform(make_ref(a) + -x, user::eval_xform{});
        EXPECT_EQ(result.value, -40);
    }

    {
        user::number result = transform(a + -make_ref(x), user::eval_xform{});
        EXPECT_EQ(result.value, -40);
    }

    {
        user::number result = transform(a + -x, user::eval_xform{});
        EXPECT_EQ(result.value, -40);
    }


    {
        user::number result = transform(-make_ref(a) + -make_ref(x), user::eval_xform{});
        EXPECT_EQ(result.value, -42);
    }

    {
        user::number result = transform(-make_ref(a) + -x, user::eval_xform{});
        EXPECT_EQ(result.value, -42);
    }

    {
        user::number result = transform(-a + -make_ref(x), user::eval_xform{});
        EXPECT_EQ(result.value, -42);
    }

    {
        user::number result = transform(-a + -x, user::eval_xform{});
        EXPECT_EQ(result.value, -42);
    }
}
