#include "message_manager.h"
#include "json/json.h"
#include "Elevator.h"
#include "que.h"
#include <iostream>
/*message format
 * {
 *  "que":[{*floor*,*order_type*},2,3,4],
 *  "dir": 1,
 *  "lastfloor": 1,
 *  "current_state": "RUN"
 *  }
 */
//Extract info from Elevator and make it into a string in json format
std::string make_message(Elevator elev){
    Json::Value message;
    Json::Value order;
    message["que"] = Json::arrayValue;
    for(int i = 0; i < elev.que.size(); i++){
        order["floor"] = elev.que.get_order(i).floor;
        order["type"] = elev.que.get_order(i).type;
        message["que"][i].append(order);
    }
    Json::FastWriter writer;
    return writer.write(message);
} 
//Elevator unpack_message(std::string message){
 //   return; 
//}
/*
int main(){
    Elevator elev;
    Order order(1, INTERNAL);
    elev.current_state = RUN;
    elev.direction = DIRN_UP;
    elev.last_floor = 1;
    elev.que.insert(order, DIRN_UP, elev.last_floor); 
    std::cout << make_message(elev);
    return 0;
}
*/
