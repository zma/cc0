#include "ILOptimizer.h"
#include <core/IL/ILProgram.h>
#include <iostream>
#include <cstdlib>

ILOptimizer::ILOptimizer()
{

}

ILOptimizer::~ILOptimizer()
{


}

int ILOptimizer::OperandUsedTwice(ILFunction* f, IL::ILOperand* opr)
{
    int n = 0;

    for (std::vector<IL>::iterator iit = f->Body.begin(); iit != f->Body.end(); ++iit)
    {
        for (std::vector<IL::ILOperand>::iterator iopr = iit->Operands.begin(); iopr != iit->Operands.end(); iopr++) {
            if (opr->Equal(&(*iopr))) {
                n++;
            }
        };
    }

    return (n == 2);
}

int ILOptimizer::DeleteNops(ILFunction* f)
{
    int n = 0;
    std::vector<IL> ils;
    ils = f->Body;

    f->Body.clear();

    for (std::vector<IL>::iterator iit = ils.begin(); iit != ils.end(); ++iit)
    {
        if (iit->Opcode == IL::Nop) {
            n++;
        }
        else {
            f->Body.push_back(*iit);
        }
    }

    return n;
}


int ILOptimizer::OptimizeFunction(ILFunction *f)
{
    int optimized = 0;
    std::vector<IL> ils;
    ils = f->Body;

    std::cout << "Optimizing: function START" << std::endl;

    for (std::vector<IL>::iterator iit = ils.begin(); iit != ils.end(); ++iit)
    {
        IL &il = *iit;

        // safety ensurance
        // if ((iit+1) == f->Body.end() || iit == ils.begin()) {
        if ((iit+1) == ils.end() || iit == ils.begin()) {
            continue;
        }

        // branch
        std::cout << "Optimizing: " << il.ToString() << std::endl;
        if(il.Opcode == IL::CompLt || il.Opcode == IL::CompLe || il.Opcode == IL::CompGt || il.Opcode == IL::CompGe || il.Opcode == IL::CompEq || il.Opcode == IL::CompNe) {
            // std::cout << "Ah ha! I got you! clt" << std::endl;
            IL &il2 = *(iit+1);

            if(il2.Opcode == IL::BrZ) {
                std::cout << "Ah ha! I got you! bz after clt -->";
                // and the __temp varaible is not used anymore
                if (il2.Operands.at(0).Equal(&il.Operands.at(0)) &&
                    OperandUsedTwice(f, &il.Operands.at(0))
                ) {

                    std::cout << "Good to optimize." << std::endl;
                    std::cout << "Optimizing: " << il2.ToString() << std::endl;
                    // changed to:
                    if (il.Opcode == IL::CompLt) {
                        il = *(new IL(IL::BrLe, il.Operands.at(2), il.Operands.at(1), il2.Operands.at(1)));
                    } else if (il.Opcode == IL::CompLe) {
                        il = *(new IL(IL::BrLt, il.Operands.at(2), il.Operands.at(1), il2.Operands.at(1)));
                    } else if (il.Opcode == IL::CompGt) {
                        il = *(new IL(IL::BrLe, il.Operands.at(1), il.Operands.at(2), il2.Operands.at(1)));
                    } else if (il.Opcode == IL::CompGe) {
                        il = *(new IL(IL::BrLt, il.Operands.at(1), il.Operands.at(2), il2.Operands.at(1)));
                    } else if (il.Opcode == IL::CompEq) {
                        il = *(new IL(IL::BrNe, il.Operands.at(1), il.Operands.at(2), il2.Operands.at(1)));
                    } else if (il.Opcode == IL::CompNe) {
                        il = *(new IL(IL::BrEq, il.Operands.at(1), il.Operands.at(2), il2.Operands.at(1)));
                    } else {
                        abort();
                    }
                    il2 = *(new IL(IL::Nop));
                    std::cout << "Optimized: " << il.ToString() << std::endl;
                    optimized = 1;
                }
                else {
                    std::cout << "Sadly not okay to optimize." << std::endl;
                }
            }
            if(il2.Opcode == IL::BrNz) {
                std::cout << "Ah ha! I got you! bnz after clt -->";
                if (il2.Operands.at(0).Equal(&il.Operands.at(0)) &&
                    OperandUsedTwice(f, &il.Operands.at(0))
                ) {

                    std::cout << "Good to optimize." << std::endl;
                    std::cout << "Optimizing: " << il2.ToString() << std::endl;
                    // changed to:
                    if (il.Opcode == IL::CompLt) {
                        il = *(new IL(IL::BrLt, il.Operands.at(1), il.Operands.at(2), il2.Operands.at(1)));
                    } else if (il.Opcode == IL::CompLe) {
                        il = *(new IL(IL::BrLe, il.Operands.at(1), il.Operands.at(2), il2.Operands.at(1)));
                    } else if (il.Opcode == IL::CompGt) {
                        il = *(new IL(IL::BrLt, il.Operands.at(2), il.Operands.at(1), il2.Operands.at(1)));
                    } else if (il.Opcode == IL::CompGe) {
                        il = *(new IL(IL::BrLe, il.Operands.at(2), il.Operands.at(1), il2.Operands.at(1)));
                    } else if (il.Opcode == IL::CompEq) {
                        il = *(new IL(IL::BrEq, il.Operands.at(1), il.Operands.at(2), il2.Operands.at(1)));
                    } else if (il.Opcode == IL::CompNe) {
                        il = *(new IL(IL::BrNe, il.Operands.at(1), il.Operands.at(2), il2.Operands.at(1)));
                    } else {
                        abort();
                    }
                    il2 = *(new IL(IL::Nop));
                    std::cout << "Optimized: " << il.ToString() << std::endl;
                    optimized = 1;
                }
                else {
                    std::cout << "Sadly not okay to optimize." << std::endl;
                }

            }
        }

        /*
        // copy proporgation (partial)
        if(il.Opcode == IL::Add) {
            // std::cout << "Ah ha! I got you! clt" << std::endl;
            IL &il2 = *(iit+1);
            if(il2.Opcode == IL::Mov) {
                std::cout << "Ah ha! I got you! mov after add -->";
                if (il2.Operands.at(1).Equal(&il.Operands.at(0)) &&
                    OperandUsedTwice(f, &il.Operands.at(0))
                ) {
                    std::cout << "Good to optimize." << std::endl;
                    std::cout << "Optimizing: " << il2.ToString() << std::endl;
                    // changed to:
                    il = *(new IL(IL::Add, il2.Operands.at(0), il.Operands.at(1), il.Operands.at(2)));
                    il2 = *(new IL(IL::Nop));
                    std::cout << "Optimized: " << il.ToString() << std::endl;
                }
                else {
                    std::cout << "Sadly not okay to optimize." << std::endl;
                }
            }
        }
        */
        // copy proporgation (partial)
        if(il.Opcode == IL::Mov || il.Opcode == IL::Conv) { // Mov and Conv are identical
            // safety check: only if it is not converting types
            if (il.Operands.at(0).OperandType != il.Operands.at(1).OperandType) {
                continue;
            }

            // previous instruction
            IL &il2 = *(iit-1);
            switch (il2.Opcode) {
                // there should be more instructions that are okay.
                // For safety, we only allow a set
                // extend it later
                case IL::Add:
                case IL::And:
                case IL::Sub:
                case IL::Mul:
                case IL::Div:
                // il2.Opcode == IL::Conv ||
                case IL::Ldelem:
                case IL::Ldelema:
                    if (
                        il2.Operands.at(0).Equal(&il.Operands.at(1)) &&
                        // not used in other places
                        OperandUsedTwice(f, &il2.Operands.at(0)) &&
                        // is a variable
                        il2.Operands.at(0).OperandKind == IL::Variable &&
                        // is local
                        il2.Operands.at(0).SymRef->Scope->GetScopeKind() == SymbolScope::Block

                    ) {
                        std::cout << "Ah ha! I got you! mov after some instructions -->";
                        std::cout << "Good to optimize." << std::endl;
                        // changed to:
                        il2 = *(new IL(il2.Opcode, il.Operands.at(0), il2.Operands.at(1), il2.Operands.at(2)));
                        std::cout << "Optimized: " << il2.ToString() << std::endl;
                        il= *(new IL(IL::Nop));
                        std::cout << "Optimized: " << il.ToString() << std::endl;
                        optimized = 1;
                    }
                    else {
                        std::cout << "Sadly not okay to optimize." << std::endl;
                        // std::cout << "Equal: " << il2.Operands.at(0).Equal(&il.Operands.at(1)) << std::endl;
                        // std::cout << "TWice: " << OperandUsedTwice(f, &il2.Operands.at(0)) << std::endl;
                    }
                    break;
                case IL::Conv:
                case IL::Neg:
                case IL::Ldind:
                    if (
                        il2.Operands.at(0).Equal(&il.Operands.at(1)) &&
                        // not used in other places
                        OperandUsedTwice(f, &il2.Operands.at(0)) &&
                        // is a variable
                        il2.Operands.at(0).OperandKind == IL::Variable &&
                        // is local
                        il2.Operands.at(0).SymRef->Scope->GetScopeKind() == SymbolScope::Block
                    ) {
                        std::cout << "Ah ha! I got you! mov after conv instructions -->";
                        std::cout << "Good to optimize." << std::endl;
                        // changed to:
                        il2 = *(new IL(il2.Opcode, il.Operands.at(0), il2.Operands.at(1)));
                        std::cout << "Optimized: " << il2.ToString() << std::endl;
                        il= *(new IL(IL::Nop));
                        std::cout << "Optimized: " << il.ToString() << std::endl;
                        optimized = 1;
                    }
                    else {
                        std::cout << "Sadly not okay to optimize." << std::endl;
                        // std::cout << "Equal: " << il2.Operands.at(0).Equal(&il.Operands.at(1)) << std::endl;
                        // std::cout << "TWice: " << OperandUsedTwice(f, &il2.Operands.at(0)) << std::endl;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    f->Body = ils;

    int n_del_nops = DeleteNops(f);

    std::cout << "Optimized: Deleted " << n_del_nops << " Nops." << std::endl;

    std::cout << "Optimizing: function END" << std::endl;

    return optimized;
}

ILProgram* ILOptimizer::Optimize(ILProgram* il)
{
    // Visit(il);
    _il = new ILProgram();

    // make a copy
    *_il = *il;

    // optimize now
    for (std::vector<ILClass *>::iterator cit = _il->Claases.begin(); cit != _il->Claases.end(); ++cit)
    {
        ILClass *c = *cit;

        // EnterClass(c);

        for (std::vector<ILFunction *>::iterator fit = c->Functions.begin(); fit != c->Functions.end(); ++fit)
        {
            ILFunction *f = *fit;

            // optimize till no place to optimize
            while (OptimizeFunction(f)) {;};

            // EnterFunction(f);

            // for (std::vector<IL>::iterator iit = f->Body.begin(); iit != f->Body.end(); ++iit)
            // {
            //    ;
            // iit->Accept(this);
            // }

            // LeaveFunction(f);
        }

        // LeaveClass(c);
    }


    return _il;
    // return il;
}
