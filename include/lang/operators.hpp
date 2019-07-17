#ifndef __OPERATORS_H__
#define __OPERATORS_H__

#include <string>
#include <map>

namespace cpl::lang {
enum class BinOpId {PLUS,MINUS,MUL,DIV,MOD,INVALID};
class BinOpInfo {
    inline const static std::map <BinOpId, std::string> binop_id_to_string_table = {
        { BinOpId::PLUS, "+"},
        { BinOpId::MINUS, "-"},
        { BinOpId::MUL, "*"},
        { BinOpId::DIV, "/"},
        { BinOpId::MOD, "%"},
        { BinOpId::MOD, "INVALID"}
    };
public:
    BinOpInfo() = delete;
    static std::string binop_id_to_string(BinOpId op_id);
};
} // end ns

#endif
