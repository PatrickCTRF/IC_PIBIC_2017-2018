#include <gpio.h>
#include <alarm.h>

using namespace EPOS;

void led_recebimento(){

	led->set(1);//Sequencia para indicar dado recebido.
    Alarm::delay(delay_time);
    led->set(0);
    Alarm::delay(delay_time);
    
}

int main()
{
    while(1){
    	
    	led_recebimento();	
    	
    }
    
    return 0;
}

