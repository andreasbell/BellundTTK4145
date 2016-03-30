#pragma once
#include <string>
#include "Elevator.h" 
#include <map>
/*message format
 * {
 *  "que":[1,2,3,4],
 *  "dir": 1,
 *  "lastfloor": 1,
 *  "current_state": "RUN"
 *  }
 */

std::string make_message(Elevator elev); 
Elevator unpack_message(std::string message);

