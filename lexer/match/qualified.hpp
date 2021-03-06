#ifndef DHC_LEXER_QUALIFIED_HPP
#define DHC_LEXER_QUALIFIED_HPP

#include <graft/match.hpp>

namespace dhc {
    namespace lexer {

        /**
         * \brief Represents a matched qualified id.
         */
        class qualified : public graft::match::match {
            public:
                /**
                 * \brief Creates a qualified match.
                 * @param type An integer representing the token type.
                 * @param modules The modules.
                 */
                qualified (int type, std::vector<icu::UnicodeString> modules) : match(type), modules(modules) {}

                virtual unsigned int length();
                virtual icu::UnicodeString flatten();
                virtual std::vector<std::shared_ptr<match>> children();

                /**
                 * \brief The matched modules.
                 */
                const std::vector<icu::UnicodeString> modules;

            protected:
            private:
        };

    }
}

#endif // DHC_GRAFT_MATCH_CHARACTER_HPP
