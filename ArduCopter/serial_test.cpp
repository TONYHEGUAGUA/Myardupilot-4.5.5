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

void Copter::uart_print(){
   char c = '!';
   hal.serial(0)->begin(115200);
   hal.serial(1)->begin(115200);
   gcs().send_text(MAV_SEVERITY_INFO,"byte=%c",c);
   hal.serial(1)->write(c);
   hal.serial(0)->write(c);
}

void Copter::console_print(){
   //hal.console->printf("Hello console\n");
   gcs().send_text(MAV_SEVERITY_INFO,"Hello console\n");
}