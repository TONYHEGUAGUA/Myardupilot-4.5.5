#include "Copter.h"
bool configured=false;

void Copter::uart_test(){

    if(!configured)
    {
    hal.serial(3)->begin(57600);
    configured=true;
    }
     int16_t nbytes = hal.serial(3)->available();
     while (nbytes-- > 0) {
        char c = hal.serial(3)->read();
        gcs().send_text(MAV_SEVERITY_INFO,"byte=%c",c);
        hal.serial(3)->write(c);
     }
}

void Copter::uart_print(){
   if(!configured)
    {
      hal.serial(2)->begin(57600);
      configured=true;
    }
   hal.serial(2)->printf("hello uart2\n");
   //hal.serial(3)->println("Hello uart3");
   gcs().send_text(MAV_SEVERITY_INFO,"hello uart printed\n");
}

void Copter::console_print(){
   //hal.console->printf("Hello console\n");
   gcs().send_text(MAV_SEVERITY_INFO,"Hello console\n");
}

//update in 30HZ to communicate with lerobot

void Copter::send_needed_message(){ 
   //only need is velocity and location.
   Location send_loc;
   Vector3f send_vel;
   if (ahrs.get_location(send_loc) && ahrs.get_velocity_NED(send_vel))
   {
      uint8_t send_buf[28];
      float data_F;
      int32_t data_I;
      data_I = send_loc.lat;
      // hal.console->printf("lat:%ld\n",data_I);
      memcpy(&send_buf[3], &data_I, sizeof(float));   //memcpy可以用于内存的快速拷贝，适用于需要灵活高效拷贝数据的场景
      data_I = send_loc.lng;
      // hal.console->printf("lng:%ld\n",data_I);
      memcpy(&send_buf[7], &data_I, sizeof(float));
      data_I = send_loc.alt * 10UL;
      // hal.console->printf("alt:%ld\n",data_I);
      memcpy(&send_buf[11], &data_I, sizeof(float));
      data_F = send_vel.x * 100;
      // hal.console->printf("vx:%f\n",data_F);
      memcpy(&send_buf[15], &data_F, sizeof(float));
      data_F = send_vel.y * 100;
      // hal.console->printf("vy:%f\n",data_F);
      memcpy(&send_buf[19], &data_F, sizeof(float));
      data_F = send_vel.z * 100;
      // hal.console->printf("vz:%f\n",data_F);
      memcpy(&send_buf[23], &data_F, sizeof(float));
      swarm_uart->write(send_buf,sizeof(send_buf)); 
   }

}