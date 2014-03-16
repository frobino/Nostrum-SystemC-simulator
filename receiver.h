/// \file receiver.h
/// \brief File containing the entity of the receiver component.
///
/// See also receiver.cpp

#ifndef RECEIVER_H
#define RECEIVER_H
#include <systemc.h>
#include "NOC_package.h"
#include "RX_package.h"

/*! \class receiver
    \brief SystemC description of the receiver module.

    It is the receiver block of the GRLS wrapper.
*/
SC_MODULE(receiver)
//class receiver : public sc_module
{
	/// Input port for datac from the transmitter
	sc_in<Packet> datac;
	/// Input port for the strobe from the transmitter
	sc_in<sc_bit> strobe;
	/// reset port
	sc_in<bool> rst_n;
	/// clock port
	sc_in<bool> clk;

	/// output port for the data to the receiver switch
	sc_out<Packet> dataout;

	/// analysis of the strobe and decision if to sample or not the next PC
	void strobe_analysis();
	/// manage the output, deciding from which FF to take the results (pos edge triggered, neg edge triggered, or synch reg)
	void receiver_register();
	/// evaluate when the strobe has changed the last time
	void time_strobe();
	/// manage the positive edge sensitive input FF
	void update_P();
	/// manage the negative edge sensitive input FF
	void update_N();
	/// manage the synchronisation register
	void synch_reg_module();

	/// matrix of 2 lines. In the first line i put the samples sampled on clkR edge, on the second one the ones from clkR2
	bool** sample_container;
	/// used to manage the output
	Packet datac_container[2];
	/// counter to decide which column of sample_container i'm analyzing
	int actual_sample;
	/// general counter
	int i; //general counter
	/// vector used to decide when to sample on clkR1 edge
	bool* must_i_sample;
	/// when strobe has changed the last time
	sc_time strobelastchange;
	/// manage particular case in which strobe and clkR change simultaneously
	sc_time synchro_delta_cycle; //in case strobe and clkR change simultaneously, strobechange is evaluated BEFORE strobeget function

	/// manage kernel
	sc_event update_event_P;
	/// manage kernel
	sc_event update_event_N;
	/// manage kernel
	sc_event output_update;

	/// info in reg_N
	Packet save_register_N;
	/// info in reg_P
	Packet save_register_P;
	/// info to output
	Packet saved_packet;
	/// info from synchreg
	Packet synch_reg_out;

	/// vs flag
	bool vs_flag;
	/// flag set in case of error
	bool error_flag;

	//SC_CTOR(receiver)
	SC_HAS_PROCESS(receiver);
	/// constructor of the transmitter
	receiver(sc_module_name name_ , int N_R , int N_T, int Tsw, int Tsu) : sc_module(name_), m_N_R(N_R), m_N_T(N_T),m_Tsw(Tsw),m_Tsu(Tsu)
	{
		sample_container = new bool*[2];
		sample_container[0] = new bool[(m_N_T*2)+1];
		sample_container[1] = new bool[(m_N_T*2)+1];

		must_i_sample = new bool[(m_N_T*2)+1];

		SC_METHOD(receiver_register);
		//sensitive<<clk.pos()<<rst_n.neg();
		sensitive<<output_update<<rst_n.neg();

		SC_THREAD(strobe_analysis);
		sensitive<<clk<<rst_n.neg();

		SC_METHOD(update_P);
		sensitive<<update_event_P<<rst_n.neg(); //sensitive on clkr1

		SC_METHOD(update_N);
		sensitive<<update_event_N<<rst_n.neg();

		SC_METHOD(synch_reg_module);
		//sensitive<<clk.pos()<<rst_n.neg();
		sensitive<<update_event_P<<rst_n.neg(); //sensitive on clkr1

		SC_METHOD(time_strobe);
		sensitive<<strobe;
		dont_initialize();


	}
private:
	int m_N_R,m_N_T,m_Tsw,m_Tsu;
};
#endif
