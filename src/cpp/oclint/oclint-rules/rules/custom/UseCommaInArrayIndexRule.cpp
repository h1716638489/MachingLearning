#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "clang/Lex/Lexer.h"

using namespace std;
using namespace clang;
using namespace oclint;

class UseCommaInArrayIndexRule : public AbstractASTVisitorRule<UseCommaInArrayIndexRule>
{
public:
    virtual const string name() const override
    {
        return "use comma in array index";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "custom";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.12";
    }

    virtual const std::string description() const override
    {
        return ""; // TODO: fill in the description of the rule.
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example()
    {
        // TODO: modify the example for this rule.
    }
        )rst";
    }

    /* fill in the file name only when it does not match the rule identifier
    virtual const std::string fileName() const override
    {
        return "";
    }
    */

    /* add each threshold's key, description, and its default value to the map.
    virtual const std::map<std::string, std::string> thresholds() const override
    {
        std::map<std::string, std::string> thresholdMapping;
        return thresholdMapping;
    }
    */

    /* add additional document for the rule, like references, notes, etc.
    virtual const std::string additionalDocument() const override
    {
        return "";
    }
    */

    /* uncomment this method when the rule is enabled to be suppressed.
    virtual bool enableSuppress() const override
    {
        return true;
    }
    */
#endif

    virtual void setUp() override {
        sm = &_carrier->getSourceManager();
    }
    virtual void tearDown() override {}

    /* Visit ArraySubscriptExpr */
    bool VisitArraySubscriptExpr(ArraySubscriptExpr *node)
    {
        Expr* idx = node->getIdx();
        if(isa<BinaryOperator>(idx)){
            BinaryOperator* binaryOperator = dyn_cast_or_null<BinaryOperator>(idx);
            Expr* lhs = binaryOperator->getLHS();
            Expr* rhs = binaryOperator->getRHS();
            if(binaryOperator->getOpcode()==BO_Comma && isa<IntegerLiteral>(lhs) && isa<IntegerLiteral>(rhs)){
                string exprStr = expr2str(node);
                string message = "The comma operator ',' in array index expression '"+exprStr+"'.";
                addViolation(node,this,message);
            }
        }
        return true;
    }
private:
    std::string expr2str(Expr *expr) {
        // (T, U) => "T,,"
        string text = clang::Lexer::getSourceText(CharSourceRange::getTokenRange(expr->getSourceRange()), *sm, LangOptions(), 0);
        if (text.at(text.size()-1) == ',')
            return clang::Lexer::getSourceText(CharSourceRange::getCharRange(expr->getSourceRange()), *sm, LangOptions(), 0);
        return text;
    }    
private:
    SourceManager* sm;
    
};

static RuleSet rules(new UseCommaInArrayIndexRule()); 