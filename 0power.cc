#include <gpio.h>
#include <alarm.h>
#include <machine/cortex/emote3.h>

#include <utility/ostream.h>
#include <nic.h>

#include "auxiliar_power.h"


static const unsigned int DUTY_CYCLE = 1; // %
static const bool half_duty_cycle = true;

using namespace EPOS;

OStream cout;

const unsigned int delay_time = 2000000;

bool led_value;
GPIO * led;

void led_envio(){

	led->set(1);//Sequencia para indicar dado sendo enviado.
    Alarm::delay(delay_time/8);
    led->set(0);
    Alarm::delay(delay_time/8);
    led->set(1);
    Alarm::delay(delay_time/8);
    led->set(0);
    Alarm::delay(delay_time/8);
}

void led_recebimento(){

	led->set(1);//Sequencia para indicar dado recebido.
    Alarm::delay(delay_time);
    led->set(0);
    Alarm::delay(delay_time);
    
}



int main()
{
    cout << "Hello main" << endl;
    
    while(1){
    	led_envio();
    	
    	setPowerMode(pm_pm1);
		
	}

    return 0;
}
