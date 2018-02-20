#include <utility/ostream.h>
#include <nic.h>
#include <gpio.h>
#include <alarm.h>

#include <modbus_ascii.h>

#include <chronometer.h>
#include <secure_nic.h>
#include <utility/string.h>
#include <utility/key_database.h>


#include <aes.h>
#include <uart.h>
#include "home_thing.h"
#include <adc.h>
#include <machine/cortex_m/emote3_watchdog.h>


static const unsigned int DUTY_CYCLE = 1; // %
static const bool half_duty_cycle = true;

using namespace EPOS;

OStream cout;
volatile int recebeu;//variável que indica se acabamos de receber um dado e que é zerada logo após enviarmos a mensagem designada.
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

void sender()
{
    char data[] = "0 Hello, World!";
    NIC * nic = new NIC();
    cout << "Hello, I am the sender." << endl;
    cout << "I will send a message every " << delay_time << " microseconds." << endl;

        cout << "Sending message: " << data << endl;
        //nic->send(nic->broadcast(), NIC::ELP, data, sizeof data);
            NIC::Address addr;
            addr[0] = 0x8b;
            addr[1] = 0x3c;
            
		led_envio();
        nic->send(addr, NIC::ELP, data, sizeof data);
        cout << "Sent" << endl;
        data[0] = ((data[0] - '0' + 1) % 10) + '0';
        
        recebeu = 0;
}

class Receiver : public IEEE802_15_4::Observer
{
    typedef char data_type;

public:
    typedef IEEE802_15_4::Protocol Protocol;
    typedef IEEE802_15_4::Buffer Buffer;
    typedef IEEE802_15_4::Frame Frame;
    typedef IEEE802_15_4::Observed Observed;

    Receiver(const Protocol & p, NIC * nic) : _prot(p), _nic(nic)
    {
        _nic->attach(this, _prot);
    }

    void update(Observed * o, Protocol p, Buffer * b)
    {
        cout << "Received buffer" << reinterpret_cast<void *>(b) << endl;
        if(p == _prot) {
            Frame * f = b->frame();
            auto d = f->data<data_type>();
            cout << endl << "=====================" << endl;
            cout << "Received " << b->size() << " bytes of payload from " << f->src() << " :" << endl;
            for(int i=0; i<b->size()/sizeof(data_type); i++)
                cout << d[i] << " ";
            cout << endl << "=====================" << endl;
            _nic->free(b);
            
            led_recebimento();
            recebeu = 1;
        }
    }

private:
    Protocol _prot;
    NIC * _nic;
};

void receiver()
{
    
    led = new GPIO('C',3, GPIO::OUT);
    cout << "Hello, I am the receiver." << endl;
    cout << "I will attach myself to the NIC and print every message I get." << endl;
    NIC * nic = new NIC();
    Receiver * r = new Receiver(NIC::ELP, nic);
}

int main()
{
    cout << "Hello main" << endl;
    
    led_envio();
    
    // Make sure duty cycle is respected in case of reboot by watchdog
    TSC::wake_up_at(TSC::time_stamp() + ((TSC::us_to_ts(1500000)), &send);
    eMote3::power_mode(eMote3::POWER_MODE_2);
    eMote3::power_mode(eMote3::POWER_MODE::ACTIVE);

    eMote3_Watchdog::enable();

    TSC::Time_Stamp t1;

    send(0);
    while(true) {
        eMote3_Watchdog::kick();

        t1 = TSC::time_stamp();
        TSC::wake_up_at(TSC::time_stamp() + (((t1-t0) * ((100 - DUTY_CYCLE) / DUTY_CYCLE)) << half_duty_cycle), &send);

        eMote3::power_mode(eMote3::POWER_MODE_2);

        eMote3::power_mode(eMote3::POWER_MODE::ACTIVE);
    }



    return 0;
}
