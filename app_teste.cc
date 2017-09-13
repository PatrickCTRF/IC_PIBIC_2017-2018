// EPOS Semaphore Component Test Program

#include <utility/ostream.h>
#include <thread.h>
#include <mutex.h>
#include <semaphore.h>
#include <alarm.h>
#include <display.h>
#include <nic.h>
#include <gpio.h>
#include <alarm.h>

using namespace EPOS;

OStream cout;
int estado;

void sender()
{
    char data[] = "0 Hello, World!";
    NIC * nic = new NIC();
    const unsigned int delay_time = 2000000;
    cout << "Hello, I am the sender." << endl;
    cout << "I will send a message every " << delay_time << " microseconds." << endl;
    while(1) {
        cout << "Sending message: " << data << endl;
        nic->send(0x0a000100, NIC::ELP, data, sizeof data);
        cout << "Sent" << endl;
        data[0] = ((data[0] - '0' + 1) % 10) + '0';
        Alarm::delay(delay_time);
    }
}

bool led_value;
GPIO * led;

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
            led_value = !led_value;
            led->set(led_value);
            Frame * f = b->frame();
            auto d = f->data<data_type>();
            cout << endl << "=====================" << endl;
            cout << "Received " << b->size() << " bytes of payload from " << f->src() << " :" << endl;
            for(int i=0; i<b->size()/sizeof(data_type); i++)
                cout << d[i] << " ";
            cout << endl << "=====================" << endl;
            _nic->free(b);
            
        	GPIO g('C',3, GPIO::OUT);
			g.set(1);
			Delay esperando(500000);//0,5 segundos.
			g.set(0);
        }
    }

private:
    Protocol _prot;
    NIC * _nic;
};

void receiver()
{
    
    led = new GPIO('C',3, GPIO::OUT);
    led_value = true;
    led->set(led_value);
    cout << "Hello, I am the receiver." << endl;
    cout << "I will attach myself to the NIC and print every message I get." << endl;
    NIC * nic = new NIC();
    Receiver * r = new Receiver(NIC::ELP, nic);
}


const int iterations = 10;

Mutex table;

Thread * phil[5];
Semaphore * chopstick[5];


int philosopher(int n, int l, int c)
{
    int first = (n < 4)? n : 0;
    int second = (n < 4)? n + 1 : 4;

    for(int i = iterations; i > 0; i--) {

        table.lock();
        Display::position(l, c);
        cout << "thinking";
        table.unlock();

        Delay thinking(2000000);

        chopstick[first]->p();    // get first chopstick
        chopstick[second]->p();   // get second chopstick

        table.lock();
        Display::position(l, c);
        cout << " eating ";
        table.unlock();

        Delay eating(1000000);

        chopstick[first]->v();    // release first chopstick
        chopstick[second]->v();   // release second chopstick
    }

    table.lock();
    Display::position(l, c);
    cout << "  done  ";
    table.unlock();

    return iterations;
}

int main()
{

	int i;
	estado = 0;

	//receiver();
    sender();

    while(1);

    return 0;
}
