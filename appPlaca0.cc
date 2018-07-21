#include <utility/ostream.h>
#include <nic.h>
#include <gpio.h>
#include <alarm.h>
#include "auxiliar_power.h"
#include "mystring.h"

using namespace EPOS;

OStream cout;
volatile int recebeu;//variável que indica se acabamos de receber um dado e que é zerada logo após enviarmos a mensagem designada.
unsigned int delay_time = 200000;
unsigned int INDICE_DESTA_PLACA = 0000;

bool led_value;
GPIO * led;
char bufferRecepcao[512];
char stringTensaoADC[32];
int valorADC = 3300;
char stringAux[10];
char stringDeAlinhamento[] = "tensao: ";

/**
 * Esta função obtém a parte que contém o valor de tensao na string recebida pelo rádio.
 */
int copiaParte(char* menor, char* maior){
	
	if(menor == 0 || maior == 0){//Se recebemos algum ponteiro nulo, retorne -1.
		
		return -1;
		
	}
	
	//O valor de tensão está logo depois desta palavra, então temos que descontar estes caracteres na hora de lê-lo.
	int offset = myStrlen(stringDeAlinhamento);
	
	int i = 0;
	
	//i não passa de 3, pois a tensao em milivolts só possui 4 casas numerais no total.
	for(i= 0; i<=3 && maior[i+offset] != '\0'; i++){
	
		menor[i] = maior[i+offset];
		
	}
	
	menor[i] = '\0';//Fechamos a string menor
		
	return 0;
}

void led_envio(){

	led->set(1);//Sequencia para indicar dado sendo enviado.
    delayRealTimeus(delay_time/8);
    led->set(0);
	delayRealTimeus(delay_time/8);
    led->set(1);
    delayRealTimeus(delay_time/8);
    led->set(0);
    delayRealTimeus(delay_time/8);
}

void led_recebimento(){

	led->set(1);//Sequencia para indicar dado recebido.
    delayRealTimeus(delay_time);
    led->set(0);
    delayRealTimeus(delay_time);
    
}

void sender(){
	
	/**
	 * O array data[] guarda os caracteres que estão prestes a ser enviado pelo 
	 * rádio. Nós concatenaremos nele o valor de tensão lido pelo ADC sobre a 
	 * bateria para que as placas possam decidir
	 */
    char data[64] = "Valor de tensao: ";
	myItoa(valorADC, stringTensaoADC, 10);
    myStrcat(data, stringTensaoADC);
    
    NIC * nic = new NIC();
    cout << "Hello, I am the sender." << endl;
    cout << "I will send a message every " << delay_time << " microseconds." << endl;

        cout << "Sending message: " << data << endl;
        //nic->send(nic->broadcast(), NIC::ELP, data, sizeof data);
            NIC::Address addr;
            addr[0] = 0x7d;
            addr[1] = 0x3c;
            
		led_envio();
        nic->send(addr, NIC::ELP, data, sizeof data);
        cout << "Sent" << endl;
        data[0] = ((data[0] - '0' + 1) % 10) + '0';
        
        recebeu = 0;
        //recebeu += 1;//Incrementamos para consideramos também que esta placa fez um envio;
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
            int i=0;
            for(i=0; i<b->size()/sizeof(data_type); i++){
            	bufferRecepcao[i] = d[i];//Estamos salvando a mensagem que chega do transmissor.
                cout << d[i] << " ";//d[i] representa o caractere de índice i recebido elo rádio.
            }
            bufferRecepcao[i] = '\0';//Encerramos a string recebida corretamente;
            cout << endl << "=====================" << endl;
            _nic->free(b);
            
            
            cout << endl << "copiaParte = " << copiaParte(stringAux, myStrstr(bufferRecepcao, stringDeAlinhamento));
            cout << endl << "stringAux = " << stringAux << endl;
            cout << endl << "mySTRSTR = " << myStrstr(bufferRecepcao, stringDeAlinhamento) << endl;
            cout << endl << "bufferRecepcao = " << bufferRecepcao << endl;
            led_recebimento();
            recebeu += 1;//Indicamos que recebemos mais 1 dado.
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

	delayRealTimeus(3000000);
	
    cout << "Hello main" << endl;
    
    delay_time = 200000;
	INDICE_DESTA_PLACA = 0000;
    myStrcpy(stringDeAlinhamento, "tensao: ");
    valorADC = 3300;
    bufferRecepcao[511] = '\0';//Colocamos o caractere de fim de string para nao haver risco de acesso incorreto da memória.
	stringAux[9] = '\0';
    
    /*receiver();
    
    while(1){
    	
    	//Sempre que formos reiniciar o ciclo, resetamos a quantidade de mensagens recebidas.
    	recebeu = 0;
    	
    	//Aguardamos que seja a vez deta placa enviar seus valores.
    	while(recebeu != INDICE_DESTA_PLACA);
    	
    	sender();//Enviamos nosso dado de bateria.   
    	
    	//Quando tivermos os dados das 3 placas, seguimos em frente.
    	while(recebeu < 3);//Aguarda os 3 dados.
    	
    	
    	
	}*/
    
    
//==============================================================================
	
    receiver();
    
    sender();	/*Lembrar que somente uma placa deve fazer este "sender()" antes do while interno para INICIAR O CICLO. Senão, todas 						ficariam esperando receber uma mensagem para poder enviar e nenhuma enviaria primeiro.*/
	    
	while(1){
	
	
	if(recebeu != 0){//Só enviaremos novamente a mensagem após recebermos.	
		wake_up_after_us(5000000);
		setPowerMode(pm_pm1);
    	sender();	/*Lembrar que somente uma placa deve fazer este "sender()" antes do while interno para INICIAR O CICLO. Senão, todas 						ficariam esperando receber uma mensagem para poder enviar e nenhuma enviaria primeiro.*/
	}
    	
    }



    return 0;
}
