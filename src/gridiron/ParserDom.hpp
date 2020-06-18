#ifndef GRIDIRON_PARSERDOM_HPP
#define GRIDIRON_PARSERDOM_HPP

#include <htmlcxx2/htmlcxx2_html.hpp>
#include <gridiron/Node.hpp>

namespace GridIron {

    typedef kp::tree<GridIron::Node...> Tree;

    class ParserDom : htmlcxx2::HTML::ParserDom {};
}
#endif //GRIDIRON_PARSERDOM_HPP
