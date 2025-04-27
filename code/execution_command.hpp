#ifndef EXECUTION_COMMAND_HPP
#define EXECUTION_COMMAND_HPP

#include <base.h>


enum execution_command_tag
{
    ExecutionCommand_None,

    ExecutionCommand_ExitGame,

    ExecutionCommand_DebugGraph_Off,
    ExecutionCommand_DebugGraph_Fps,
};


struct exec_command
{
    execution_command_tag tag;
};

#endif // EXECUTION_COMMAND_HPP
