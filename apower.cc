#include <gpio.h>
#include <alarm.h>

#include <utility/ostream.h>
	

#include "auxiliar_power.h"
#include <periodic_thread.h>

static const unsigned int DUTY_CYCLE = 1; // %
//static const bool half_duty_cycle = true;

using namespace EPOS;

OStream cout;
volatile int recebeu;//variável que indica se acabamos de receber um dado e que é zerada logo após enviarmos a mensagem designada.
const unsigned int delay_time = 2000000;

bool led_value;
GPIO * led;

const int iterations = 100;
const long period_a = 100; // ms

void led_envio(){

	led = new GPIO('C',3, GPIO::OUT);

	led->set(1);//Sequencia para indicar dado sendo enviado.
    Alarm::delay(delay_time/8);
    led->set(0);
    Alarm::delay(delay_time/8);
    led->set(1);
    Alarm::delay(delay_time/8);
    led->set(0);
    Alarm::delay(delay_time/8);
    
    free(led);
}

void led_recebimento(){

	led->set(1);//Sequencia para indicar dado recebido.
    Alarm::delay(delay_time);
    led->set(0);
    Alarm::delay(delay_time);
    
}

void handler(const unsigned int & id){

    cout << "Okay!" << endl;
}

int func_a(){

    cout << "A";
    for(int i = 0; i < iterations; i++) {
	    led_envio();
        Periodic_Thread::wait_next();
        cout << "a";
    }
    cout << "A";
    return 'A';
}

int main()
{
    cout << "Hello main" << endl;
    
	//auto int_time = TSC::time_stamp() + (2*TSC::frequency());
    
    while(1){
    	
    	//Periodic_Thread thread_a(RTConf(period_a * 1000, iterations), &func_a);
    	//int status_a = thread_a.join();
    	
    	led_envio();
    	wake_up_after_us(3000000);
    	setPowerMode(pm_pm1);
    	
		
    	//Timer::interrupt(TSC::time_stamp() + 100, handler);
    	
    	//TSC::wake_up_at(int_time, &handler);
    	
    	//int_time = TSC::time_stamp() + (2*TSC::frequency());
		
	}

    return 0;
}
