#ifndef NYXSYSCALLSINTERFACE_H
#define NYXSYSCALLSINTERFACE_H

#include <interface/SyscallsInterface.hpp>
#include <vector>
#include <iostream>

class NyxSyscallsInterface : public nyx::SyscallsInterface
{
public:
    NyxSyscallsInterface();

    void setOutStream(std::ostream * stream);

private:
    nyx::ast::ExpressionPtr print(std::vector<nyx::ast::ExpressionPtr> * params);
    nyx::ast::ExpressionPtr println(std::vector<nyx::ast::ExpressionPtr> * params);

    void _print(nyx::ast::ExpressionPtr e);

    std::ostream * _ss;
};

#endif // NYXSYSCALLSINTERFACE_H
