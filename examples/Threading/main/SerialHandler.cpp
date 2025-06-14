#include "SerialHandler.h"

// ###################################################### // 
// ################### SERIAL HANDLER ################### // 
// ###################################################### // 

SerialHandler::SerialHandler( CommandStore* cmd_store ) 
  : arg_index_(0),
    cmd_index_(0), 
    new_word_(true), 
    cmd_store_(cmd_store) {} 

SerialHandler::~SerialHandler() 
{
  // deallocate the dynamically allocated memory 
}

void SerialHandler::readCommand()
{
  while ( Serial.available() > 0 ) 
  { 
    if ( new_word_ ) 
    {
      Serial.println("Starting new word"); 
      cmd_index_ = 0; 
      memset(cmd_buffer_, 0, CMD_STRING_BUFFERSIZE); // Clear buffer 
      new_word_ = false;  
    }
    chr_ = Serial.read(); 
    processCharacter(); 
  }
}

void SerialHandler::resetArgumentArray() 
{
  // iterate over the argument array and set to \0 
  for ( uint8_t i=0; i < arg_index_; ++i )
  {
    memset( argument_arr_[i], 0, CMD_STRING_BUFFERSIZE );  
  }
  arg_index_ = 0; 
  new_word_ = true; 
}

void SerialHandler::storeArgument() 
{
  cmd_buffer_[cmd_index_] = '\0'; // null-terminate the string 
  strcpy( argument_arr_[arg_index_++], cmd_buffer_ ); // copy into command array 

  // start new word 
  new_word_ = true; 
} 

void SerialHandler::processCharacter() 
{
  Serial.print("Got character: ");
  Serial.println(chr_); 
  Serial.flush(); 

  if (chr_ == '\r') // End of command
  {
    handleCommandEnd(); 
  }
  else if (chr_ == ' ') // Argument end 
  {
    storeArgument(); 
  }
  else // Regular character
  {
    handleCharacter();
  }
}

void SerialHandler::handleCommandEnd() 
{
  storeArgument(); 
  storeCommand(); 
  resetArgumentArray(); 
}

void SerialHandler::handleCharacter()  
{
  if ( cmd_index_ < CMD_STRING_BUFFERSIZE - 1 ) // prevent buffer overflow 
  {
    cmd_buffer_[cmd_index_++] = chr_; 
  }
  else
  {
    // handle buffer overflow 
    // Serial.println( "Error: Command too long!" ); 
    new_word_ = true; 
  }
}

void SerialHandler::storeCommand()  
{ 
  CommandPackage cmd_pkg; 
  cmd_pkg.populate( argument_arr_, arg_index_ );
  cmd_store_->store( cmd_pkg ); 

  Serial.println(ETX); // ETX (End of Text, 0x03) 
  Serial.flush(); 
}

void SerialHandler::runTarget( void *arg ) 
{
  // read while there are characters in serial 
  while ( true ) { readCommand(); }
}

// Start thread that will continue reading serial data 
void SerialHandler::startReading()
{ 
	// Start thread 
  Serial.println("Starting SerialHandler thread");
	serial_thread_ = new std::thread(&Runnable::runThread, this); 
 	serial_thread_->detach();
}

