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

TEST_F(AllTest, secondDeclaration_1)
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

//TODO dodac linie !

TEST_F(AllTest, usingUndeclaredVariable_2)
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

TEST_F(AllTest, wrongVariableName_4)
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

TEST_F(AllTest, program0)
{
    in << "10\n";
    std::string expectedOut = "? > 0\n> 1\n> 0\n> 1\n";

    language <<
             "VAR\n"
             "    a b\n"
             "BEGIN\n"
             "    READ a;\n"
             "    WHILE a > 0 DO\n"
             "        b := a / 2;\n"
             "        b := 2 * b;\n"
             "        IF a > b THEN WRITE 1;\n"
             "        ELSE WRITE 0;\n"
             "        ENDIF\n"
             "        a := a / 2;\n"
             "    ENDWHILE\n"
             "END";

    compiler.run(language, generatedCode);
    interpreter.run(generatedCode, in, out, info);

    EXPECT_STREQ(expectedOut.c_str(), this->out.str().c_str());
}
TEST_F(AllTest, DISABLED_program1)
{
    std::string expectedOut = "? > 0\n> 1\n> 0\n> 1\n";

    language <<
             "{ sito Eratostenesa }\n"
             "VAR\n"
             "    n j sito[100]\n"
             "BEGIN\n"
             "    n := 100-1;\n"
             "    FOR i FROM n DOWNTO 2 DO\n"
             "        sito[i] := 1;\n"
             "    ENDFOR\n"
             "    FOR i FROM 2 TO n DO\n"
             "        IF sito[i] <> 0 THEN\n"
             "            j := i + i;\n"
             "            WHILE j <= n DO\n"
             "                sito[j] := 0;\n"
             "                j := j + i;\n"
             "            ENDWHILE\n"
             "            WRITE i;\n"
             "        ELSE\n"
             "          SKIP;\n"
             "        ENDIF\n"
             "    ENDFOR\n"
             "END";

    compiler.run(language, generatedCode);
    interpreter.run(generatedCode, in, out, info);

    EXPECT_STREQ("", this->error.str().c_str());
    EXPECT_STREQ("", this->info.str().c_str());
    EXPECT_STREQ(expectedOut.c_str(), this->out.str().c_str());
}


} // namespace