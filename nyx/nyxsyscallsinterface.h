#ifndef NYXSYSCALLSINTERFACE_H
#define NYXSYSCALLSINTERFACE_H

#include <interface/SyscallsInterface.hpp>
#include <vector>
#include <iostream>

#include "../game/gameinfo.h"

class NyxSyscallsInterface : public nyx::SyscallsInterface
{
public:
    NyxSyscallsInterface(std::ostream & outstream, GameInfo * gameInfo);

    void setOutStream(std::ostream * stream);

private:
    nyx::ast::ExpressionPtr print(std::vector<nyx::ast::ExpressionPtr> * params);
    nyx::ast::ExpressionPtr println(std::vector<nyx::ast::ExpressionPtr> * params);
    nyx::ast::ExpressionPtr getGridInfo(std::vector<nyx::ast::ExpressionPtr> * params);
    nyx::ast::ExpressionPtr moveRight(std::vector<nyx::ast::ExpressionPtr> * params);
    nyx::ast::ExpressionPtr moveLeft(std::vector<nyx::ast::ExpressionPtr> * params);
    nyx::ast::ExpressionPtr moveUp(std::vector<nyx::ast::ExpressionPtr> * params);
    nyx::ast::ExpressionPtr moveDown(std::vector<nyx::ast::ExpressionPtr> * params);

    void _print(nyx::ast::ExpressionPtr e);

    std::ostream & _out;
    GameInfo *_gameInfo;
};

#endif // NYXSYSCALLSINTERFACE_H
