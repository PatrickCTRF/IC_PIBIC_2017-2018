#include <gpio.h>
#include "auxiliar_power.h"
#include "mystring.h"

using namespace EPOS;

OStream cout;

int main()
{
    GPIO g('C',3, GPIO::OUT);
    
    char stringMaior[] = "Esta eh string maior";
	char stringMenor[] = "string";

	char* pos = myStrStr(stringMaior, stringMenor);

    for(bool b=false;;b=(b+1)%2) {
        g.set(b);
		delayRealTimeus(500000);
		cout<<pos<<endl;
    }

    return 0;
}
