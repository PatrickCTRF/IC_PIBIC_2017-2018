#include <gpio.h>

using namespace EPOS;

int main()
{
    GPIO g('C',3, GPIO::OUT);
    int k = 0, voltas = 0;

	while(1){
		for(bool b=false;voltas<1;b=1) {
			g.set(b);
		    for(volatile int t=0;t<0xfffff-k;k++);
		    g.set(1-b);
		    for(volatile int t=0;t<k;k++);
		    
		    voltas++;
		}
		
		voltas = 0;
		k++;
		if(k>=0xfffff) k=0;
		
	}
    return 0;
}

