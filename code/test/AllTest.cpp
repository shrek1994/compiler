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
    in << "1345601\n";
    std::string expectedOut = "? > 1\n> 0\n> 0\n> 0\n> 0\n> 0\n> 1\n> 0\n> 0\n> 0\n> 0\n> 1\n> 0\n> 0\n> 0\n> 1\n> 0\n> 0\n> 1\n> 0\n> 1\n";

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

TEST_F(AllTest, program1)
{
    std::string expectedOut = "> 2\n> 3\n> 5\n> 7\n> 11\n> 13\n> 17\n> 19\n> 23\n> 29\n> 31\n> 37\n> 41\n> 43\n> 47\n> 53\n> 59\n> 61\n> 67\n> 71\n> 73\n> 79\n> 83\n> 89\n> 97\n";

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
    EXPECT_STREQ(expectedOut.c_str(), this->out.str().c_str());
}

TEST_F(AllTest, program2)
{
//    in << "12345678903\n";
//    std::string expectedOut = "? > 3\n> 1\n> 4115226301\n> 1\n";
    in << "1234567890\n";
    std::string expectedOut = "? > 2\n> 1\n> 3\n> 2\n> 5\n> 1\n> 3607\n> 1\n> 3803\n> 1\n";

    language <<
             "{ Rozklad liczby na czynniki pierwsze }\n"
                     "VAR\n"
                     "    n m reszta potega dzielnik\n"
                     "BEGIN\n"
                     "    READ n;\n"
                     "    dzielnik := 2;\n"
                     "    m := dzielnik * dzielnik;\n"
                     "    WHILE n >= m DO\n"
                     "        potega := 0;\n"
                     "        reszta := n % dzielnik;\n"
                     "        WHILE reszta = 0 DO\n"
                     "            n := n / dzielnik;\n"
                     "            potega := potega + 1;\n"
                     "            reszta := n % dzielnik;\n"
                     "        ENDWHILE\n"
                     "        IF potega > 0 THEN { czy znaleziono dzielnik }\n"
                     "            WRITE dzielnik;\n"
                     "            WRITE potega;\n"
                     "        ELSE\n"
                     "            dzielnik := dzielnik + 1;\n"
                     "            m := dzielnik * dzielnik;\n"
                     "        ENDIF\n"
                     "    ENDWHILE\n"
                     "    IF n <> 1 THEN { ostatni dzielnik }\n"
                     "        WRITE n;\n"
                     "        WRITE 1;\n"
                     "    ELSE\n"
                     "        SKIP;\n"
                     "    ENDIF\n"
                     "END";

    compiler.run(language, generatedCode);
    interpreter.run(generatedCode, in, out, info);

    EXPECT_STREQ("", this->error.str().c_str());
    EXPECT_STREQ(expectedOut.c_str(), this->out.str().c_str());
}

TEST_F(AllTest, program3)
{
    in << "1234567890\n"
          "1234567890987654321\n"
          "987654321\n";
    std::string expectedOut = "? ? ? > 674106858\n";

    language <<
             "{ a ^ b mod c }\n"
                     "VAR\n"
                     "    a b c wynik pot wybor\n"
                     "BEGIN\n"
                     "    READ a;\n"
                     "    READ b;\n"
                     "    READ c;\n"
                     "    wynik:=1;\n"
                     "    pot:=a%c;\n"
                     "    WHILE b>0 DO\n"
                     "\t\twybor:=b%2;\n"
                     "\t\tIF wybor=1 THEN\n"
                     "\t\t\twynik:=wynik*pot;\n"
                     "\t\t\twynik:=wynik%c;\n"
                     "\t\tELSE\n"
                     "\t\t\tSKIP;\n"
                     "\t\tENDIF\n"
                     "\t\tb:=b/2;\n"
                     "\t\tpot:=pot*pot;\n"
                     "\t\tpot:=pot%c;\n"
                     "    ENDWHILE\n"
                     "    WRITE wynik;\n"
             "END";

    compiler.run(language, generatedCode);
    interpreter.run(generatedCode, in, out, info);

    EXPECT_STREQ("", this->error.str().c_str());
    EXPECT_STREQ(expectedOut.c_str(), this->out.str().c_str());
}


TEST_F(AllTest, program4)
{
    in << "20\n";
    std::string expectedOut = "? > 2432902008176640000\n";

    language <<
             "{ Silnia }\n"
                     "VAR\n"
                     "  s[101] n m a j\n"
                     "BEGIN\n"
                     "    READ n;\n"
                     "    s[0]:=1;\n"
                     "    m:=n;\n"
                     "    FOR i FROM 1 TO m DO\n"
                     "\t\ta:=i%2;\n"
                     "\t\tj:=i-1;\n"
                     "\t\tIF a=1 THEN\n"
                     "\t\t\ts[i]:=s[j]*m;\n"
                     "\t\tELSE\n"
                     "\t\t\ts[i]:=m*s[j];\n"
                     "\t\tENDIF\n"
                     "\t\tm:=m-1;\n"
                     "    ENDFOR\n"
                     "    WRITE s[n];\n"
                     "END";

    compiler.run(language, generatedCode);
    interpreter.run(generatedCode, in, out, info);

    EXPECT_STREQ("", this->error.str().c_str());
    EXPECT_STREQ(expectedOut.c_str(), this->out.str().c_str());
}

} // namespace