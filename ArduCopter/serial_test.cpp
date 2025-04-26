#include "Copter.h"
bool configured=false;

void serial_setup(void);


/*
  setup one UART at 57600
 */
static void setup_uart(AP_HAL::UARTDriver *uart, const char *name)
{
    if (uart == nullptr) {
        // that UART doesn't exist on this platform
        return;
    }
    uart->begin(57600);
}

void serial_setup(void)
{
    /*
      start all UARTs at 57600 with default buffer sizes
    */

    //hal.scheduler->delay(1000); //Ensure that hal.serial(n) can be initialized

    setup_uart(hal.serial(0), "SERIAL0");  // console
    setup_uart(hal.serial(1), "SERIAL1");  // telemetry 1
    setup_uart(hal.serial(2), "SERIAL2");  // telemetry 2
    setup_uart(hal.serial(3), "SERIAL3");  // 1st GPS
    setup_uart(hal.serial(4), "SERIAL4");  // 2nd GPS
}

static void test_uart(AP_HAL::UARTDriver *uart, const char *name)
{
    if (uart == nullptr) {
        // that UART doesn't exist on this platform
        return;
    }
    uart->printf("Hello on UART %s at %.3f seconds\n",
                 name, (double)(AP_HAL::millis() * 0.001f));
}

void Copter::all_uart_test(){
   if(!configured){
      serial_setup();
   }
   test_uart(hal.serial(0), "SERIAL0");
   test_uart(hal.serial(1), "SERIAL1");
   test_uart(hal.serial(2), "SERIAL2");
   test_uart(hal.serial(3), "SERIAL3");
   test_uart(hal.serial(4), "SERIAL4");
}

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
   AP_HAL::UARTDriver *send_uart = hal.serial(1);
   Location send_loc;
   Vector3f send_vel;
   if (ahrs.get_location(send_loc) && ahrs.get_velocity_NED(send_vel))
   {
      //模仿swarm模式的数据传输，传输位置信息与速度信息
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
      send_uart->write(send_buf,sizeof(send_buf)); 
   }

}