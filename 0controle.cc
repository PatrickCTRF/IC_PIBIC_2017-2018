#include <utility/ostream.h>
#include <nic.h>
#include <gpio.h>
#include <alarm.h>

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
            addr[0] = 0xcb;
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
    
    recebeu = 0;

    receiver();
    
    GPIO PTD_0 = GPIO('D',0, GPIO::OUT);
    GPIO PTD_1 = GPIO('D',1, GPIO::OUT);
    GPIO PTD_2 = GPIO('D',2, GPIO::OUT);
    GPIO PTD_3 = GPIO('D',3, GPIO::OUT);
    GPIO PTD_4 = GPIO('D',4, GPIO::OUT);
    GPIO PTD_5 = GPIO('D',5, GPIO::OUT);
    GPIO PTA_7 = GPIO('A',7, GPIO::OUT);
    GPIO PTA_6 = GPIO('A',6, GPIO::OUT);
    
    PTD_5.set();
    PTD_4.set();
    PTD_3.set();
    PTD_2.set();
    PTD_1.set();
    PTD_0.set();
    PTA_7.set();
    PTA_6.set();
    
	while(1){

		sender();
		led_envio();
    	
    }



    return 0;
}
