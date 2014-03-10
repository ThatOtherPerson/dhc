#ifndef DHC_LEXER_LIT_FLOAT_HPP
#define DHC_LEXER_LIT_FLOAT_HPP

#include "literal.hpp"

namespace dhc {
    namespace lexer {

        /**
         * \brief Represents a matched lit_float.
         */
        class lit_float : public literal {
            public:
                /**
                 * \brief Creates a lit_float match.
                 * @param column The column of the source the match
                 *               begins on.
                 * @param type An lit_float representing the token type.
                 * @param length The original matched length.
                 * @param data The matched lit_float.
                 */
                lit_float (unsigned int column, int type, int length, int64_t integral, int64_t fractional, int64_t exponent) : literal(column, type, literal_type::FLOAT), len(length), integral(integral), fractional(fractional), exponent(exponent) {}

                virtual unsigned int length();
                virtual icu::UnicodeString flatten();
                virtual std::vector<std::shared_ptr<match>> children();

                /**
                 * \brief the original matched length
                 */
                const int len;

                /**
                 * \brief (12).34e5
                 */
                const int64_t integral;

                /**
                 * \brief 12.(34)e5
                 */
                const int64_t fractional;

                /**
                 * \brief 12.34e(5)
                 */
                const int64_t exponent;
            protected:
            private:
        };

    }
}

#endif
