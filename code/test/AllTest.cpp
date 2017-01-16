#include "CompilerTest.hpp"
#include "Checker.hpp"

namespace {
const std::string emptyGeneratedCode = "";

using namespace checker;

class AllTest : public CompilerTest {
public:
};


/**
 * TESTS FROM WEBSITE:
 */

TEST_F(AllTest, secondDeclaration)
{
    auto error = Checker::error + Checker::line + "3" + Checker::duplicateDeclaration + ": 'a'\n";
    language <<
            "{ Błąd w linii 3: druga deklaracja a }\n"
            "VAR\n"
            "  a b a\n"
            "BEGIN\n"
            "  READ a;\n"
            "  b := a;\n"
            "END";

    compiler.run(language, generatedCode);

    EXPECT_STREQ(error.c_str(), this->error.str().c_str());
    EXPECT_STREQ(emptyGeneratedCode.c_str(), this->generatedCode.str().c_str());
}

//TODO dodac linie
TEST_F(AllTest, usingUndeclaredVariable)
{
    auto error = Checker::error + Checker::undeclaredVariable + ": 'a'\n";
    language <<
            "{ Błąd w linii 5: niezadeklarowana zmienna a }\n"
            "VAR\n"
            "  b\n"
            "BEGIN\n"
            "  READ a;\n"
            "  b := a;\n"
            "END";

    compiler.run(language, generatedCode);

    EXPECT_STREQ(error.c_str(), this->error.str().c_str());
    EXPECT_STREQ(emptyGeneratedCode.c_str(), this->generatedCode.str().c_str());
}

TEST_F(AllTest, wrongVariableName)
{
    auto error = Checker::error + Checker::line + "3" + Checker::unrecognizedText;
    language <<
             "{ Błąd w linii 3: nierozpoznany napis a1 }\n"
                     "VAR\n"
                     "  a1 b\n"
                     "BEGIN\n"
                     "  READ a1;\n"
                     "  b := a1;\n"
                     "END";

    compiler.run(language, generatedCode);

    EXPECT_STREQ(error.c_str(), this->error.str().c_str());
    EXPECT_STREQ(emptyGeneratedCode.c_str(), this->generatedCode.str().c_str());
}


} // namespace