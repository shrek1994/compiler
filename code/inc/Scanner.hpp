#pragma once

class Scanner : public yyFlexLexer{
public:
    Scanner(std::istream& in, std::ostream& out, std::ostream& debugStream = std::cout)
            : yyFlexLexer(in, debugStream),
              out(out)
    {
        loc = new LexParser::location_type();
    };

    virtual ~Scanner() {
        delete loc;
    };

    using FlexLexer::yylex;

    virtual int yylex(LexParser::semantic_type * const lval,
                      LexParser::location_type * location);
    virtual std::ostream& getOut() { return out; }

private:
    /* yyval ptr */
    LexParser::semantic_type *yylval = nullptr;
    /* location ptr */
    LexParser::location_type *loc    = nullptr;
    std::string txt;
    std::ostream& out = std::cout;
};