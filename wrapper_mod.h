/// \file wrapper_mod.h
/// \brief File containing the entity of the wrapper. It put togheter transmitter and receiver blocks;
///
/// See also wrapper_mod.cpp

#ifndef WRAPPER_H
#define WRAPPER_H

#include "systemc.h"
#include "transmitter.h"
#include "receiver.h"
#include "NOC_package.h"

/*! \class wrapper_mod
    \brief SystemC description of the wrapper module (structural description).

    Structural description of the wrapper. Instantiation and connection of the sub-blocks transmitter and receiver. Everything is modular.
*/
SC_MODULE(wrapper_mod)
//class wrapper_mod : public sc_module
{
	private:
	int m_N_left,m_N_right;


	public:
	///data input port of the left side of the wrapper
 	sc_in<Packet> L_datain;
 	///data output port of the left side of the wrapper
	sc_out<Packet> L_dataout;

	///data input port of the right side of the wrapper
	sc_in<Packet> R_datain;
	///data output port of the right side of the wrapper
	sc_out<Packet> R_dataout;


	///clock input form the switch on the left side of the wrapper
	sc_in<bool> clkL;
	///clock input form the switch on the right side of the wrapper
	sc_in<bool> clkR;
	///reset input
	sc_in<bool> rst_n;

	///instantiation of the transmitters sub-blocks
	transmitter tx_LtoR;
	///instantiation of the transmitters sub-blocks
	transmitter tx_RtoL;
	///instantiation of the receivers sub-blocks
	receiver rx_LtoR;
	///instantiation of the receivers sub-blocks
	receiver rx_RtoL;

	///stobe signal for the left to right couple of tx and rx
	sc_signal<sc_bit> L_strobe_s;
	///full signal for the left to right couple of tx and rx
	sc_signal<sc_bit> L_full_s;
	///stobe signal for the right to left couple of tx and rx
	sc_signal<sc_bit> R_strobe_s;
	///full signal for the right to left couple of tx and rx
	sc_signal<sc_bit> R_full_s;

	///data signal for the left to right couple of tx and rx
	sc_signal<Packet> L_datac_s;
	///data signal for the right to left couple of tx and rx
	sc_signal<Packet> R_datac_s;

	//SC_CTOR(wrapper_mod):tx_LtoR("tx_LtoR",5,3,7),tx_RtoL("tx_RtoL",3,5,7),rx_LtoR("rx_LtoR",5,3,100,25),rx_RtoL("rx_RtoL",3,5,100,25)
	/// indication that we want to explicit the constructor
	SC_HAS_PROCESS(wrapper_mod);
	/// constructor of the wrapper
	wrapper_mod(sc_module_name name_ , int N_left , int N_right) : sc_module(name_), m_N_left(N_left), m_N_right(N_right) ,tx_LtoR("tx_LtoR",m_N_left,m_N_right,7),tx_RtoL("tx_RtoL",m_N_right,m_N_left,7),rx_LtoR("rx_LtoR",m_N_left,m_N_right,100,25),rx_RtoL("rx_RtoL",m_N_right,m_N_left,100,25)
	{
		tx_LtoR.datain(L_datain);
		tx_LtoR.rst_n(rst_n);
		tx_LtoR.clk(clkL);
		tx_LtoR.full(L_full_s);
		tx_LtoR.datac(L_datac_s);
		tx_LtoR.strobe(L_strobe_s);
		rx_LtoR.datac(L_datac_s);
		rx_LtoR.strobe(L_strobe_s);
		rx_LtoR.clk(clkR);
		rx_LtoR.rst_n(rst_n);
		rx_LtoR.dataout(R_dataout);

		tx_RtoL.datain(R_datain);
		tx_RtoL.rst_n(rst_n);
		tx_RtoL.clk(clkR);
		tx_RtoL.full(R_full_s);
		tx_RtoL.datac(R_datac_s);
		tx_RtoL.strobe(R_strobe_s);
		rx_RtoL.datac(R_datac_s);
		rx_RtoL.strobe(R_strobe_s);
		rx_RtoL.clk(clkL);
		rx_RtoL.rst_n(rst_n);
		rx_RtoL.dataout(L_dataout);

	}

};


#endif
