#ifndef DHC_GRAFT_PATTERN_COMPOUND_HPP
#define DHC_GRAFT_PATTERN_COMPOUND_HPP

#include "../pattern.hpp"

#include "../match/sequence.hpp"

#include <memory>
#include <vector>

namespace dhc {
    namespace graft {
        namespace pattern {

            /**
             * \brief A pattern that matches a sequence of multiple patterns.
             */
            class compound : public pattern
            {
                public:
                    /**
                     * \brief Create a compound pattern.
                     * @param p The list of patterns to match.
                     * @param type The type of the matched token.
                     * @param callback a callback to perform additional
                     *                 processing on the match returned
                     */
                    compound(std::vector<std::shared_ptr<pattern>>&& p, int type = -1, match_func callback = nullptr) : pattern(type, callback), pat(p) {}

                    /**
                     * \brief Add a pattern to the list of patterns.
                     *
                     * This just adds the pattern to the end of pat.
                     * It's needed because a compound pattern containing
                     * a pointer to itself must first be created and
                     * then added to itself.
                     *
                     * @param pat the pattern to add.
                     */
                    void add_pattern(std::shared_ptr<pattern> pat);
                protected:
                    virtual std::shared_ptr<match::match> findmatch(scanner::scanner& s);

                    /**
                     * \brief The sequence of patterns to match.
                     */
                    std::vector<std::shared_ptr<pattern>> pat;
                private:
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_COMPOUND_HPP
