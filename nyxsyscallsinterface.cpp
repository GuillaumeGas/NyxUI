#include "nyxsyscallsinterface.h"

#include <ast/AstIncludes.hpp>

using namespace nyx;

NyxSyscallsInterface::NyxSyscallsInterface() : _ss(&cout)
{
    addSyscall("print", std::bind(&NyxSyscallsInterface::print, this, std::placeholders::_1));
    addSyscall("println", std::bind(&NyxSyscallsInterface::println, this, std::placeholders::_1));
}

void NyxSyscallsInterface::setOutStream(std::ostream * stream)
{
    _ss = stream;
}

ast::ExpressionPtr NyxSyscallsInterface::print(std::vector<ast::ExpressionPtr> * params)
{
    for (auto it : *params) {
        _print(it->interpretExpression());
    }

    return ast::NullExpression();
}

ast::ExpressionPtr NyxSyscallsInterface::println(std::vector<ast::ExpressionPtr>* params)
{
    if (params)
    {
        for (auto it : *params) {
            _print(it->interpretExpression());
        }
    }

    *_ss << endl;

    return ast::NullExpression();
}

void NyxSyscallsInterface::_print(ast::ExpressionPtr e) {
    vector<ast::ExpressionPtr>* vec = NULL;
    ast::RangePtr range;

    std::ostream & out = *_ss;

    if (e->getType()->value == ast::TYPE::ARRAY || e->getType()->value == ast::TYPE::RANGE)
        out << "[";

    switch (e->getType()->value) {
    case ast::TYPE::INT:
        out << e->getInt();
        break;
    case ast::TYPE::CHAR:
        out << e->getChar();
        break;
    case ast::TYPE::BOOL:
        out << e->getBool();
        break;
    case ast::TYPE::FLOAT:
        out << e->getFloat();
        break;
    case ast::TYPE::STRING:
        vec = e->getArray();
        for (auto it : *vec) {
            _print(it);
        }
        break;
    case ast::TYPE::ARRAY:
        vec = e->getArray();
        for (int i = 0; i < vec->size(); i++) {
            ast::ExpressionPtr obj = (*vec)[i]->interpretExpression();
            _print(obj);
            if (i < vec->size() - 1)
                out << ", ";
        }
        break;
    case ast::TYPE::RANGE:
        out << e->getRangeBegin()->getInt();
        out << " .. ";
        out << e->getRangeEnd()->getInt();
        break;
    default:
        out << "Unknown type " + e->getType()->toString() + "!" << endl;
    }

    if (e->getType()->value == ast::TYPE::ARRAY || e->getType()->value == ast::TYPE::RANGE)
        out << "]";
}
