/// \file SWok.cpp
/// \brief File containing the description of the processes of the SWITCH component.
///
/// See also SWok.h

#include "receiver.h"


void receiver::strobe_analysis()
{
while(true)
{
	int j,k;
	sc_bit strobe_s;

	strobe_s=strobe;


	if (rst_n==(sc_bit)0)
	{
		for (j=0;j<2;j++)
		{
			for (k=0;k<(m_N_T*2)+1;k++)
			{

			if (j==0)
				sample_container[j][k]=false;
			else
				sample_container[j][k]=true;

			must_i_sample[k]=false;

			}
		datac_container[j]=empty_NL_PDU;
		}
	actual_sample=1;
	cout<<"AT: "<<sc_simulation_time()<<" Reset"<<endl;
	}
	else // if clk'event (pos o neg)
	{
		//phase1
		sample_container[0][actual_sample]=strobeget(strobe_s,strobelastchange,m_Tsu); //strobeget() to simulate metastability
		synchro_delta_cycle=sc_time_stamp();


		wait(m_Tsw,SC_PS);
		//phase2
		if (must_i_sample[actual_sample]==true)
			datac_container[0]=datac;

		//cout<<"AT: "<<sc_simulation_time()<<" sample_container[0]: "<<sample_container[0][0]<<sample_container[0][1]<<sample_container[0][2]<<endl;
		//cout<<"AT: "<<sc_simulation_time()<<" sample_container[1]: "<<sample_container[1][0]<<sample_container[1][1]<<sample_container[1][2]<<endl;
		//cout<<"AT: "<<sc_simulation_time()<<" must_i_sample: "<<must_i_sample[0]<<must_i_sample[1]<<must_i_sample[2]<<endl;

		if (clk==true)
			update_event_P.notify();
		else
			update_event_N.notify();



		wait(m_Tsw,SC_PS);
		//phase3
		//sample_container[1][actual_sample]=strobe_s; //verr� sostituito con strobeget() per simulare metastab

		strobe_s=strobe; //sample again the input, in case it has changed
		sample_container[1][actual_sample]=strobeget(strobe_s,strobelastchange,m_Tsu);

		if(((sample_container[1][actual_sample]==sample_container[0][actual_sample])&&(sample_container[1][actual_sample-1]!=sample_container[0][actual_sample-1]))
			||((sample_container[1][actual_sample]==sample_container[0][actual_sample])&&(sample_container[1][actual_sample-1]==sample_container[0][actual_sample-1])&&(sample_container[1][actual_sample-1]!=sample_container[1][actual_sample])))
			must_i_sample[actual_sample]=true;
		else
			must_i_sample[actual_sample]=false;

		/*if(m_N_R<=m_N_T)
			must_i_sample[actual_sample]=true;*///<------------------------------------------------

		actual_sample++;
		if (actual_sample>(m_N_T*2))
		{
			sample_container[0][0]=sample_container[0][actual_sample-1];
			sample_container[1][0]=sample_container[1][actual_sample-1];
			actual_sample=1;

		}

	}

wait();
}
}

void receiver::update_P()
{
if (rst_n==(sc_bit)0)
	{
		save_register_P=empty_NL_PDU;
	}
	else if (must_i_sample[actual_sample]==true)//event
		save_register_P=datac_container[0];
	else
		save_register_P=empty_NL_PDU;
	output_update.notify();
return;
}

void receiver::update_N()
{
if (rst_n==(sc_bit)0)
	{
		save_register_N=empty_NL_PDU;
	}
	else if (must_i_sample[actual_sample]==true) //event
		save_register_N=datac_container[0];
	else
		save_register_N=empty_NL_PDU;
	//output_update.notify();
return;
}

void receiver::receiver_register()
{
	if (rst_n==(sc_bit)0)
	{
		dataout=empty_NL_PDU;
		saved_packet=empty_NL_PDU;
		vs_flag=false;
		error_flag=false;
	}
	/*else if ((datac_container[1].payload==save_register.payload)||(save_register.payload==0))
	dataout=datac_container[1];
	else
	dataout=save_register;*/
	else if (vs_flag==false)
	{

		if(save_register_P.empty==true && synch_reg_out.empty==true)
		{
			if(m_N_R>=m_N_T)
				vs_flag=true;
			dataout=synch_reg_out;
			saved_packet=save_register_P;
		}
		else if(save_register_P.empty==false && synch_reg_out.empty==true)
		{
			vs_flag=false;
			dataout=synch_reg_out;
		}
		else if(save_register_P.empty==true && synch_reg_out.empty==false)
		{
			vs_flag=false;
			dataout=save_register_P;
		}
		else // false,false
		{
			vs_flag=false;
			dataout=empty_NL_PDU;
		}
	}
	else
	{

		if(save_register_P.empty==true && synch_reg_out.empty==true) //not possible
		{
			error_flag=true;
		}
		else if(save_register_P.empty==false && synch_reg_out.empty==true)
		{
			vs_flag=true;
			dataout=saved_packet;
			saved_packet=synch_reg_out;
		}
		else if(save_register_P.empty==true && synch_reg_out.empty==false)
		{
			vs_flag=true;
			dataout=saved_packet;
			saved_packet=save_register_P;
		}
		else // false,false
		{
			vs_flag=false;
			dataout=saved_packet;
			saved_packet=empty_NL_PDU;
		}
	}

return;
}

void receiver::time_strobe()
{
	sc_bit strobe_s;

	strobe_s=strobe;

	strobelastchange=sc_time_stamp();

	//manage case in which strobe and clkR change at the same time
	if (synchro_delta_cycle==strobelastchange)
	{
		sample_container[0][actual_sample]=strobeget(strobe_s,strobelastchange,m_Tsu);
		cout<<"actual_sample: "<<actual_sample<<endl;
	}

return;
}

void receiver::synch_reg_module()
{
if (rst_n==(sc_bit)0)
	{
	synch_reg_out=empty_NL_PDU;
	}
	else
		synch_reg_out=save_register_N;
	output_update.notify();
return;
}
