#include "ExecutionStatus.h"


std::string StatusToString(const ExecutionStatus & status)
{
    switch (status)
    {
    case ExecutionStatus::UNDEFINED:
        return "Undefined";
    case ExecutionStatus::STARTED:
        return "Started";
    case ExecutionStatus::RUNNING:
        return "Running";
    case ExecutionStatus::SUCCESS:
        return "Success";
    case ExecutionStatus::FAILURE:
        return "Failure";
    }
    return "Unknown status";
}
