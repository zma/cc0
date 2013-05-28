#include "ILVisitor.h"
#include <core/Core.h>

ILVisitor::ILVisitor()
{

}

ILVisitor::~ILVisitor()
{

}

void ILVisitor::Visit(ILProgram* il)
{
    EnterProgram(il);
    
    for (std::vector<ILClass *>::iterator cit = il->Claases.begin(); cit != il->Claases.end(); ++cit)
    {
        ILClass *c = *cit;
        
        EnterClass(c);
        
        for (std::vector<ILFunction *>::iterator fit = c->Functions.begin(); fit != c->Functions.end(); ++fit)
        {
            ILFunction *f = *fit;
            
            EnterFunction(f);
            
            for (std::vector<IL>::iterator iit = f->Body.begin(); iit != f->Body.end(); ++iit)
            {
                iit->Accept(this);
            }
            
            LeaveFunction(f);
        }
        
        LeaveClass(c);
    }
    
    LeaveProgram(il);
}

