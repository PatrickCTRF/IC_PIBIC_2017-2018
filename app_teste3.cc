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

#include <stdio.h>

using namespace EPOS;

OStream cout;
int estado;

void my_reverse(char str[], int len);
char* my_itoa(int num, char* str, int base);


void sender()
{
    char dataBuff[100];
    char *data;
    NIC * nic = new NIC();
    const unsigned int delay_time = 2000000;
    cout << "Hello, I am the sender." << endl;
    cout << "I will send a message every " << delay_time << " microseconds." << endl;
    for(int i = 0; 1; i++){
    	if(estado==1|true){
    		data = itoa(i, dataBuff);
		    cout << "Sending message: " << data << endl;
		    nic->send(i , NIC::ELP, data, sizeof data);
		    cout << "Sent" << endl;
		    cout << (unsigned int)nic->broadcast() << "  " << data << endl; //Vamos ver oq sai nisso daqui.
		    data[0] = ((data[0] - '0' + 1) % 10) + '0';// indiferente na atual situação.
		    estado = 0;
		}
        Alarm::delay(delay_time);//Alarm::delay(10);
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
            
            estado = 1;
        	GPIO g('C',3, GPIO::OUT);
			g.set(0);
			Delay esperando(500000);//0,5 segundos.
			g.set(1);
        }
    }

private:
    Protocol _prot;
    NIC * _nic;
};

void receiver()
{
    
    //led = new GPIO('C',3, GPIO::OUT);
    //led_value = false; //true;
    //led->set(led_value);
    cout << "Hello, I am the receiver 103." << endl;
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

/* 
* function to reverse a string  
*/
void my_reverse(char str[], int len)
{
    int start, end;
    char temp;
    for(start=0, end=len-1; start < end; start++, end--) {
        temp = *(str+start);
        *(str+start) = *(str+end);
        *(str+end) = temp;
    }
}
 
char* my_itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
  
    /* A zero is same "0" string in all base */
    if (num == 0) {
        str[i] = '0';
        str[i + 1] = '\0';
        return str;
    }
  
    /* negative numbers are only handled if base is 10 
       otherwise considered unsigned number */
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }
  
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'A' : rem + '0';
        num = num/base;
    }
  
    /* Append negative sign for negative numbers */
    if (isNegative){
        str[i++] = '-';
    }
  
    str[i] = '\0';
 
    my_reverse(str, i);
  
    return str;
}

int main()
{

	int i;
	estado = 1;
	
	GPIO g('C',3, GPIO::OUT);
	g.set(1);

	//receiver();
    sender();

    while(1);

    return 0;
}
