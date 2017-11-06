#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include <fstream>

using namespace std;
using namespace clang;
using namespace oclint;

class IteratorsAsArgumentRule : public AbstractASTVisitorRule<IteratorsAsArgumentRule>
{
public:
    virtual const string name() const override
    {
        return "iterators as argument";
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
        
    }
    virtual void tearDown() override {
    }

    /* Visit CallExpr */
    bool VisitCallExpr(CallExpr *callExpr)
    {
        FunctionDecl* funcDecl = callExpr->getDirectCallee();
        unsigned num = funcDecl->getNumParams();
        
        for(unsigned i=0;i<num;i++){
            ParmVarDecl* paramVarDecl = funcDecl->getParamDecl(i);
            string type = paramVarDecl->getType().getAsString();
            if(type.size()>=8 && type.substr(type.size()-8,8)=="iterator"){
                string funcName = funcDecl->getNameInfo().getAsString();
                string message = "Consider inspecting iterators which are being passed as arguments to function '"+funcName+"'.";
                addViolation(callExpr,this,message);
                break;
            }
        }
        return true;
    }
     
    
    /* Visit CXXOperatorCallExpr 
    bool VisitCXXOperatorCallExpr(CXXOperatorCallExpr *node)
    {
        return true;
    }
     */

    /* Visit CXXMemberCallExpr 
    bool VisitCXXMemberCallExpr(CXXMemberCallExpr *node)
    {
        return true;
    }*/
     
};

static RuleSet rules(new IteratorsAsArgumentRule());