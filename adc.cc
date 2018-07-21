#include <adc.h>
#include <gpio.h>

#include <utility/ostream.h>
	

#include "auxiliar_power.h"




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

void ultra_delay(){
	
	long i=0, j=0, numb = 316;
	
	while(i++<numb){
		
		while(j++<numb)cout << j << "\n"; j=0;
		
		
	}i=0;
	
}

void led_envio(){
	
	

	led = new GPIO('C',3, GPIO::OUT);

	led->set(1);//Sequencia para indicar dado sendo enviado.
	cout << "a";
    ultra_delay();
    led->set(0);
	cout << "b";
    ultra_delay();
    led->set(1);
	cout << "a";
    ultra_delay();
    led->set(0);
	cout << "b";
    ultra_delay();
    
    free(led);

    return;
}

void handler(const unsigned int & id){

    cout << "Okay!" << endl;
}



int main()
{
	led_envio();
    cout << "Hello main" << endl;
    
    volatile int x,y,z;
    
    z = 0;
	
    //ADC level_adc = ADC{ADC::SINGLE_ENDED_ADC2};
    ADC level_adc = ADC{ADC::TEMPERATURE};
	//auto level_adc = ADC{ADC::TEMPERATURE};
    
    while(1){
    	
    	//Periodic_Thread thread_a(RTConf(period_a * 1000, iterations), &func_a);
    	//int status_a = thread_a.join();
    	
    	for(x = 0; x < 316; x++){
    	for(y = 0; y < 316; y++) {
    		x = x + z;
    		y = y + z;
    	}
    	}
    	
    	//led_envio();
		cout << "teste" << level_adc.read() << endl;
    	//wake_up_after_us(5000000);
    	//setPowerMode(pm_pm1);
    	
		
	}

    return 0;
}
