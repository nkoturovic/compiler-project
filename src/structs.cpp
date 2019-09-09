#include "../include/structs.hpp"

namespace compiler::structs {

bool operator==(const FuncProto &fp1, const FuncProto &fp2) {
    if (fp1.name != fp2.name)
        return false;

    if (*fp1.retval_t != *fp2.retval_t)
        return false;

    if (fp1.args.size() != fp2.args.size())
        return false;

    for (unsigned i=0; i < fp1.args.size(); i++) {
        if (*fp1.args[i].type != *fp2.args[i].type)
            return false;
    }

    return true;
}

bool operator!=(const FuncProto &fp1, const FuncProto &fp2) {
    return !(fp1 == fp2);
}

}
