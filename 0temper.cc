

#include <alarm.h>
#include <utility/ostream.h>
#include <i2c.h>

using namespace EPOS;

OStream cout;

int main()
{
    cout << "EPOSMote III I2C Sensor Test" << endl;

    I2C_Temperature_Sensor ts;
    I2C_Humidity_Sensor hs;

    while(true) {
        Alarm::delay(1000000);
        cout << "relative humidity = " << hs.get() << "%" << endl;
        cout << "temperature = " << ts.get() << "C" << endl;
    }

    return 0;
}







/*

using namespace EPOS;

OStream cout;

double sense(unsigned int dev) {
	ADC adc(ADC::TEMPERATURE);
	int val = adc.read();
	float tc;
	if (val > 0) {
		float delta;
		delta = (10000.0*(2047-val))/val;
		float ldelta = logf(delta, E);
		tc = (1/(0.0011253088 + (0.0002347118)*ldelta + 0.00000008 * (ldelta*ldelta*ldelta))) - 273.0;
	} else {
		tc = -273.0;
	}
	// Integer value of Temperature
	return (int)(tc + 0.5);
}

int main()
{

//Temperature_Sensor tmp;


	cout << "hello" << endl;
	
	ADC adc(ADC::TEMPERATURE);

	while(1) {
	
		int val = adc.read();
		float tc;
		if (val > 0) {
			float delta;
			delta = (10000.0*(2047-val))/val;
			float ldelta = logf(delta, E);
			tc = (1/(0.0011253088 + (0.0002347118)*ldelta + 0.00000008 * (ldelta*ldelta*ldelta))) - 273.0;
		} else {
			tc = -273.0;
		}
		
		cout << (int)(tc + 0.5) << endl;
		Alarm::delay(1000000);
		
	}

	/*

    Temperature temperature(6, 1000000, Temperature::ADVERTISED);
    while (1) {
        Alarm::delay(1000000);
        cout << "Val=" << temperature << endl;
    }
    
    

    return 0;
}*/
