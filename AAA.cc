#include <modbus_ascii.h>
#include <alarm.h>
#include <chronometer.h>
#include <secure_nic.h>
#include <utility/string.h>
#include <utility/key_database.h>
#include <nic.h>
#include <gpio.h>
#include <aes.h>
#include <uart.h>
#include "home_thing.h"
#include <adc.h>
#include <machine/cortex_m/emote3_watchdog.h>

using namespace EPOS;

OStream cout;

Thing<Modbus> * sensor;
NIC * nic;
TSC::Time_Stamp t0;

void handler(GPIO * pin)
{
    cout << "Handler!" << endl;
}

void send(const unsigned int & interrupt_id)
{
    t0 = TSC::time_stamp();
    nic->listen();
    sensor->modbus->report_proactive(Modbus::READ_HOLDING_REGISTER, 0);
    nic->stop_listening();
    return;
}

int main()
{
	while(true) {
	
        TSC::wake_up_at(TSC::time_stamp() + 10000000);

        eMote3::power_mode(eMote3::POWER_MODE_2);

        eMote3::power_mode(eMote3::POWER_MODE::ACTIVE);
	}
    return 0;
}

