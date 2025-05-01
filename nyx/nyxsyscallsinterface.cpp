#include "nyxsyscallsinterface.h"

#include <ast/AstIncludes.hpp>

using namespace nyx;

NyxSyscallsInterface::NyxSyscallsInterface(std::ostream & outstream, GameInfo * gameInfo) : _out(outstream), _gameInfo(gameInfo)
{
    addSyscall("print", std::bind(&NyxSyscallsInterface::print, this, std::placeholders::_1));
    addSyscall("println", std::bind(&NyxSyscallsInterface::println, this, std::placeholders::_1));
    addSyscall("getGridInfo", std::bind(&NyxSyscallsInterface::getGridInfo, this, std::placeholders::_1));
    addSyscall("moveRight", std::bind(&NyxSyscallsInterface::moveRight, this, std::placeholders::_1));
    addSyscall("moveLeft", std::bind(&NyxSyscallsInterface::moveLeft, this, std::placeholders::_1));
    addSyscall("moveUp", std::bind(&NyxSyscallsInterface::moveUp, this, std::placeholders::_1));
    addSyscall("moveDown", std::bind(&NyxSyscallsInterface::moveDown, this, std::placeholders::_1));
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

    _out << endl;

    return ast::NullExpression();
}

ast::ExpressionPtr NyxSyscallsInterface::getGridInfo(std::vector<ast::ExpressionPtr>* params)
{
    const vector<vector<int>> & grid = _gameInfo->getGrid()->getGrid();

    auto vec = new vector<ast::ExpressionPtr>();
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid.size(); j++)
        {
            vec->push_back(ast::Expression::New<ast::Int>(grid[i][j], nullptr));
        }
    }

    map<string, ast::ExpressionPtr> members;
    members["nbColumns"] = ast::Expression::New<ast::Int>(grid.size(), nullptr);
    members["nbLines"] = ast::Expression::New<ast::Int>(grid.size(), nullptr);
    members["x"] = ast::Expression::New<ast::Int>(_gameInfo->getBot().x, nullptr);
    members["y"] = ast::Expression::New<ast::Int>(_gameInfo->getBot().y, nullptr);
    members["grid"] = ast::Expression::New<ast::Array>(vec, nullptr);

    return ast::Expression::New<ast::StructExpr>("GridInfo", members, true, nullptr);
}

ast::ExpressionPtr NyxSyscallsInterface::moveRight(std::vector<ast::ExpressionPtr>* params)
{
    _gameInfo->moveRight();

    return ast::NullExpression();
}

ast::ExpressionPtr NyxSyscallsInterface::moveLeft(std::vector<ast::ExpressionPtr>* params)
{
    _gameInfo->moveLeft();

    return ast::NullExpression();
}

ast::ExpressionPtr NyxSyscallsInterface::moveUp(std::vector<ast::ExpressionPtr>* params)
{
    _gameInfo->moveUp();

    return ast::NullExpression();
}

ast::ExpressionPtr NyxSyscallsInterface::moveDown(std::vector<ast::ExpressionPtr>* params)
{
    _gameInfo->moveDown();

    return ast::NullExpression();
}

void NyxSyscallsInterface::_print(ast::ExpressionPtr e) {
    vector<ast::ExpressionPtr>* vec = NULL;
    ast::RangePtr range;

    if (e->getType()->value == ast::TYPE::ARRAY || e->getType()->value == ast::TYPE::RANGE)
        _out << "[";

    switch (e->getType()->value) {
    case ast::TYPE::INT:
        _out << e->getInt();
        break;
    case ast::TYPE::CHAR:
        _out << e->getChar();
        break;
    case ast::TYPE::BOOL:
        _out << e->getBool();
        break;
    case ast::TYPE::FLOAT:
        _out << e->getFloat();
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
                _out << ", ";
        }
        break;
    case ast::TYPE::RANGE:
        _out << e->getRangeBegin()->getInt();
        _out << " .. ";
        _out << e->getRangeEnd()->getInt();
        break;
    default:
        _out << "Unknown type " + e->getType()->toString() + "!" << endl;
    }

    if (e->getType()->value == ast::TYPE::ARRAY || e->getType()->value == ast::TYPE::RANGE)
        _out << "]";
}
