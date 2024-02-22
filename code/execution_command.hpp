#ifndef EXECUTION_COMMAND_HPP
#define EXECUTION_COMMAND_HPP

#include <base.h>


struct exec_command
{
    enum exec_command_kind
    {
        exit
    };
    exec_command_kind kind;
};

#endif // EXECUTION_COMMAND_HPP
