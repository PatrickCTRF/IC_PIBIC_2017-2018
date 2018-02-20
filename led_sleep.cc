#include <gpio.h>
#include <alarm.h>
#include <machine/cortex/emote3.h>

using namespace EPOS;

OStream cout;

GPIO *led;
bool b=false;

int set_reset_led(void) {
	while(1) {
		b=(b+1)%2;
		led->set(b);
		cout << "yo" << endl;
		Alarm::delay(1000000);
	}
	return 0;
}

int sleeper(void) {
	while(1) {
		eMote3::power(Power_Mode::OFF);
		eMote3::power_user_timer(0,Power_Mode::OFF);		
		eMote3::power_uart(0,Power_Mode::OFF);
		eMote3::power_usb(0,Power_Mode::OFF);
		eMote3::power_ieee802_15_4(Power_Mode::OFF);
		ASM("wfi");
	}
	return 0;
}


int main()
{

    led = new GPIO('C',3, GPIO::OUT);
    led->set(false);
    
    Thread *lthread = new Thread(&set_reset_led);
    Thread *slpthread = new Thread(&sleeper);
    
    int status_a = lthread->join();
    int status_b = slpthread->join();

    return 0;
}
