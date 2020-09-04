#ifndef EXECUTIONSTATUS_H
#define EXECUTIONSTATUS_H

#include <string>

enum class ExecutionStatus {
    UNDEFINED,
    STARTED,
    RUNNING,
    SUCCESS,
    FAILURE
};

std::string StatusToString(const ExecutionStatus & status);

#endif // EXECUTIONSTATUS_H
