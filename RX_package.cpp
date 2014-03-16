#include "RX_package.h"

sc_bit strobeget(sc_bit strobe,sc_time strobelastchange,int Tsu)
{
	sc_bit sampled_strobe;
	sc_time diff_time;
	//double actual_time, tsetup;
	
	//diff_time=sc_time_stamp()-sc_time(25,SC_PS);
	diff_time=sc_time_stamp()-sc_time(Tsu,SC_PS);

	//actual_time=sc_simulation_time();
	//tsetup=to_double(sc_time(25,SC_PS));
	
	//if(strobelastchange<sc_simulation_time()-tsetup)
	//if(strobelastchange<(sc_simulation_time()-sc_time(25,SC_PS)))
	if(strobelastchange<diff_time)
		{
		//cout<<"no meta"<<endl;
        //metap0 <= 0;
        sampled_strobe=strobe;
		}
	else
		{
		cout<<"meta at: "<<sc_simulation_time()<<endl;
        //metap0 <= 1;
        //sampled_strobe= $random;
		if (SOLVE_METASTAB==0)
			sampled_strobe= (sc_bit)(rand()%2);
			//sampled_strobe= (sc_bit)0;
		else if (SOLVE_METASTAB==1)
			sampled_strobe=strobe;
		else if (SOLVE_METASTAB==2)
			sampled_strobe=~strobe;

		}

return sampled_strobe;
}