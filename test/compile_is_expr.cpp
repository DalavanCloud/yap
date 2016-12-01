#include <boost/yap/expression.hpp>

namespace yap = boost::yap;

struct alternate_expr_1
{
    static const yap::expr_kind kind = yap::expr_kind::plus;
    boost::hana::tuple<> elements;
};

struct alternate_expr_2
{
    static const yap::expr_kind kind = yap::expr_kind::plus;
    boost::hana::tuple<int, double> elements;
};


struct non_expr_1
{
};

struct non_expr_2
{
    boost::hana::tuple<int, double> elements;
};

struct non_expr_3
{
    static const int kind = 0;
    boost::hana::tuple<int, double> elements;
};

struct non_expr_4
{
    int kind;
    boost::hana::tuple<int, double> elements;
};

struct non_expr_5
{
    static const yap::expr_kind kind = yap::expr_kind::plus;
};

struct non_expr_6
{
    static const yap::expr_kind kind = yap::expr_kind::plus;
    int elements;
};


void compile_is_expr ()
{
    static_assert(yap::detail::is_hana_tuple<boost::hana::tuple<>>::value);
    static_assert(yap::detail::is_hana_tuple<boost::hana::tuple<int, double>>::value);

    static_assert(! yap::detail::is_hana_tuple<int>::value);

    static_assert(
        yap::detail::is_hana_tuple<
            yap::detail::remove_cv_ref_t<
                decltype(std::declval<yap::terminal<double>>().elements)
            >
        >::value
    );

    static_assert(yap::detail::is_expr<yap::terminal<double>>::value);

    static_assert(yap::detail::is_expr<yap::terminal<double> const>::value);
    static_assert(yap::detail::is_expr<yap::terminal<double> const &>::value);
    static_assert(yap::detail::is_expr<yap::terminal<double> &>::value);
    static_assert(yap::detail::is_expr<yap::terminal<double> &&>::value);

    static_assert(yap::detail::is_expr<yap::placeholder<1>>::value);
    static_assert(yap::detail::is_expr<yap::expression<yap::expr_kind::unary_plus, boost::hana::tuple<yap::terminal<double>>>>::value);
    static_assert(yap::detail::is_expr<yap::expression<yap::expr_kind::plus, boost::hana::tuple<yap::terminal<double>, yap::terminal<double>>>>::value);

    static_assert(yap::detail::is_expr<alternate_expr_1>::value);
    static_assert(yap::detail::is_expr<alternate_expr_2>::value);

    static_assert(! yap::detail::is_expr<int>::value);
    static_assert(! yap::detail::is_expr<non_expr_1>::value);
    static_assert(! yap::detail::is_expr<non_expr_2>::value);
    static_assert(! yap::detail::is_expr<non_expr_3>::value);
    static_assert(! yap::detail::is_expr<non_expr_4>::value);
    static_assert(! yap::detail::is_expr<non_expr_5>::value);
    static_assert(! yap::detail::is_expr<non_expr_6>::value);
}
