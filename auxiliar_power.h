

#define pm_active 0
#define pm_sleep 1
#define pm_pm0 2
#define pm_pm1 3
#define pm_pm2 4
#define pm_pm3 5



#include <emote3.h>

void setPowerMode(int mode){

	
	
	
	switch(mode){
	
		case pm_active:
			
			break;
		
		case pm_sleep:
			scs(SCR) &= ~SLEEPDEEP;
			
			scr(SCGCUART) |= (3);
			scr(SCGCRFC) |= (1);
			scr(SCGCSEC) |= (3);
			scr(SCGCI2C) |= (1);
			scr(SCGCSSI) |= (3);
			scr(SCGCGPT) |= (15);
			
			break;
			
		case pm_pm0:
			scs(SCR) |= SLEEPDEEP;
			scr(PMCTL) = 0;
			
			scr(DCGCUART) &= ~(3);
			scr(DCGCRFC) &= ~(1);
			scr(DCGCSEC) &= ~(3);
			scr(DCGCI2C) &= ~(1);
			scr(DCGCSSI) &= ~(3);
			scr(DCGCGPT) &= ~(15);
			
			
			break;
			
		case pm_pm1:
			scs(SCR) |= SLEEPDEEP;
			scr(PMCTL) = 1;
			break;
			
		case pm_pm2:
			scs(SCR) |= SLEEPDEEP;
			scr(PMCTL) = 2;
			break;
			
		case pm_pm3:
			scs(SCR) |= SLEEPDEEP;
			scr(PMCTL) = 3;
			break;
		
		default:
			break;
	}
	
	
	
	ASM("wfi");
	
}
