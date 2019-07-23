#ifndef __OPERATORS_H__
#define __OPERATORS_H__

#include <string>
#include <map>

namespace cpl::lang {
enum class BinOpId {PLUS,MINUS,MUL,DIV,MOD,
                   EQ,NEQ,LT,GT,LEQ,GEQ,
                   L_AND,L_OR,
                   B_AND,B_OR,B_XOR,SHL,SHR,
                   INVALID
};

class BinOpInfo {
    inline const static std::map <BinOpId, std::string> binop_id_to_string_table = {
        { BinOpId::PLUS, "+"},
        { BinOpId::MINUS, "-"},
        { BinOpId::MUL, "*"},
        { BinOpId::DIV, "/"},
        { BinOpId::MOD, "%"},
        { BinOpId::EQ, "=="},
        { BinOpId::NEQ, "!="},
        { BinOpId::LT, "<"},
        { BinOpId::GT, ">"},
        { BinOpId::LEQ, "<="},
        { BinOpId::GEQ, "<="},
        { BinOpId::L_AND, "&&"},
        { BinOpId::L_OR, "||"},
        { BinOpId::B_AND, "&"},
        { BinOpId::B_OR, "|"},
        { BinOpId::B_XOR, "^"},
        { BinOpId::SHL, "<<"},
        { BinOpId::SHR, ">>"},
        { BinOpId::MOD, "INVALID"}
    };
public:
    BinOpInfo() = delete;
    static std::string binop_id_to_string(BinOpId op_id);
};
} // end ns

#endif
