#include <algorithm>
#include "CommandStore.h" 

CommandStore::CommandStore()
    : cmd_index_(0), 
      locked_(false) {} 

void CommandStore::store( CommandPackage& cmd_pkg ) 
{
    // Lock the commands while data is being changed 
    locked_ = true; 

    Serial.println("Storing Command"); 
    Serial.print("cmd type: "); 

    char cmd; 
    cmd_pkg.getCmdType(cmd); 
    Serial.println(cmd); 

    // shift all the commands down 
    shiftCmds( cmd_pkg ); 

    if (cmd_index_ < MAX_COMMAND_STORAGE - 1)
        ++cmd_index_;

    locked_ = false; 
    Serial.println("Command stored"); 
}

void CommandStore::shiftCmds( CommandPackage& new_pkg ) 
{
    // shift all the commands down 
    for (int i = cmd_index_; i > 0; --i)
    {
        cmd_list_[i] = std::move(cmd_list_[i - 1]);
    }
    cmd_list_[0] = std::move(new_pkg); 
}

int CommandStore::getMostRecentCmd(CommandPackage& cmd_pkg) 
{
    if (!locked_ && cmd_index_ > 0) 
    {
        cmd_pkg = std::move(cmd_list_[0]);
        return UNLOCKED; 
    }
    return LOCKED; 
}

