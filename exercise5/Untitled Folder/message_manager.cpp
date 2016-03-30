#include "message_manager.h"

/*message format
 * {
 *  "que":[1,2,3,4],
 *  "dir": 1,
 *  "lastfloor": 1,
 *  "current_state": "RUN"
 *  }
 */
//Extract info from Elevator and make it into a string in json format
std::string make_message(Elevator elev){
    Json::Value message;
    Json::Value order;
    for(int i = 0, i = elev.que.size(){
        order[
        message["que"][i] = elev.que;
    }
} 
Elevator unpack_message(std::string message){
    
}
int main(){
    return 0;
}
