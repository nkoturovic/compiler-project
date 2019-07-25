#include "../include/structs.hpp"

namespace cpl {
Error::Error(yy::location loc, std::string msg) : loc(loc), msg(msg) {}
}  // namespace cpl
