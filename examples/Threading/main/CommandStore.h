#ifndef COMMAND_STORE_H
#define COMMAND_STORE_H

#include "Constants.h" 
#include "CommandPackage.h" 
#include <utility>
// Stores the commands received by the serial handler
// manages the stack of commands for the arm controller 
class CommandStore
{
  private: 
    void shiftCmds( CommandPackage& new_pkg ); // shifts all the commands down one rung 

    // ----- CONSTANTS ----- // 
    static const int MAX_COMMAND_STORAGE = 10; // only really care about most recent command 

    // ----- DATA ----- // 
    int cmd_index_ = 0; 
    bool locked_ = false; 
    CommandPackage cmd_list_[MAX_COMMAND_STORAGE]; 

  protected: 

  public: 
    CommandStore(); 

    /*
    Gets the most recent command, if the command store isn't locked. 

    Input:
        cmd_pkg: an empty command package to be populated with the command data 

    Output: 
        int: 0 for success, 1 for failure (i.e. the command store is locked)
    */
    int getMostRecentCmd( CommandPackage& cmd_pkg ); 
    void store( CommandPackage& cmd_pkg ); 

    // Public constants 
    static const int LOCKED = 0; 
    static const int UNLOCKED = 1; 
}; 

#endif 