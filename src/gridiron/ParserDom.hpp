#ifndef GRIDIRON_PARSERDOM_HPP
#define GRIDIRON_PARSERDOM_HPP

#include <htmlcxx2/htmlcxx2_html.hpp>
#include <gridiron/Node.hpp>

namespace GridIron {

    typedef kp::tree<GridIron::Node> Tree;

    class ParserDom : public htmlcxx2::HTML::ParserDom {
    public:
        inline const GridIron::Tree& parseTree(const std::string &html)
        {
            parse(html);
            return root();
        }

        inline const GridIron::Tree& root() { return tree_; }

    protected:
        GridIron::Tree tree_;
        GridIron::Tree::iterator currIt_;
    };
}
#endif //GRIDIRON_PARSERDOM_HPP
