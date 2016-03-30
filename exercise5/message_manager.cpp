#include "message_manager.h"
#include "json/json/json.h"
#include "Elevator.h"

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
    for(int i = 0; i < elev.que.size(); i++){
        message["que"][i] = {elev.que.get_order(i).floor, elev.que.get_order(i).type};
    }
} 
Elevator unpack_message(std::string message){
    return; 
}
int main(){
    Elevator elev;
    Order order(1, EXTERNAL);
    elev.current_state = RUN;
    elev.direction = 1;
    elev.last_floor = 1;
    elev.que.insert(order, elev);

    return 0;
}
