#include "Copter.h"
bool configured=false;

void Copter::uart_test(){

    if(!configured)
    {
    hal.serial(1)->begin(57600);
    configured=true;
    }
     int16_t nbytes = hal.serial(1)->available();
     while (nbytes-- > 0) {
        char c = hal.serial(1)->read();
        gcs().send_text(MAV_SEVERITY_INFO,"byte=%c",c);
        hal.serial(1)->write(c);
     }
}