#include <string>

#define EMPTY(...)
#define DEFER(...) __VA_ARGS__ EMPTY()
#define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
#define EXPAND(...) __VA_ARGS__

#define LOG_BRANCH_HELPER(c) magic::log_branch(DEFER(__PRETTY_FUNCTION__), c)
#define LOG_BRANCH(c) EXPAND(LOG_BRANCH_HELPER(c))

namespace magic {
void log_branch(std::string function_name, int index);
}
