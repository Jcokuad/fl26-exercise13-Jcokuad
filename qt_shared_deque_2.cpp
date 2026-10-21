// qt_shared_deque_2.cpp
#include <iostream>
#include <cassert>

#include "shared_ring_buffer.h"

int main(int argc, char *argv[])
{
  SharedRingBuffer<int> buffer;

  const long int cap = 100;

  // create segment or attach if already made by the consumer first
  if(!buffer.create(cap)){
    std::cerr << "Not able to create or attach to shared memory" << std::endl;
    return 1;
  }

  bool waitForSpace = false;

  while(true){
    // prompts the user via standard input for an integer to enqueue as long as there is space in the buffer
    if(buffer.getCount() < cap){
      waitForSpace = false;
      
      int value;
      std::cout << "$>";

      if(!(std::cin >> value)) { // if value is not inputted
        if(std::cin.eof()) { // check if end of stream has been reached
          // CTRL + D ends input
          std::cout << std::endl;
          break;
        }
        // if not an integer clear and discard the line
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter an integer!!" << std::endl;
        continue;
      }
     
      // enqueue() checks for room again
      if(buffer.enqueue(value)) {
        std::cout << "Enqueued: " << value << std::endl;
      } else {
        std::cout << "Could not enqueue " << value << " due to full buffer." << std::endl;
      } 
    } else if(!waitForSpace) {
      // this means the buffer is full, so prompting stops until the consumer frees a slot in the buffer
      std::cout << "Buffer is full, wait for consumer.............." << std::endl;
      waitForSpace = true;
    }
  }


  return 0;
}
