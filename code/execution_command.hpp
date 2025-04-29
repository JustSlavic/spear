#ifndef EXECUTION_COMMAND_HPP
#define EXECUTION_COMMAND_HPP

#include <base.h>


enum execution_command_tag
{
    ExecutionCommand_None,

    ExecutionCommand_ExitGame,

    ExecutionCommand_DebugDraw_Off,
    ExecutionCommand_DebugDraw_Fps,
};


struct exec_command
{
    execution_command_tag tag;
};

#endif // EXECUTION_COMMAND_HPP
