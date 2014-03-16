/// \file transmitter.h
/// \brief File containing the entity of the transmitter component.
///
/// See also transmitter.cpp

#ifndef TRANSMITTER_H
#define TRANSMITTER_H
#include <systemc.h>
#include "NOC_package.h"

/*! \class transmitter
    \brief SystemC description of the transmitter module.

    It is the transmitter bolck of the GRLS wrapper.
*/
SC_MODULE(transmitter)
//class transmitter : public sc_module
{
	/// Input port for datas from the switch
	sc_in<Packet> datain;
	/// reset port
	sc_in<bool> rst_n;
	/// clk port
	sc_in<bool> clk;

	/// full port. If it's set it indicates that the fifo of the transmitter is to small to contain all datas
	sc_out<sc_bit> full;
	/// strobe output port.It changes with datac
	sc_out<sc_bit> strobe;
	/// data to the receiver sub-block of the wrapper
	sc_out<Packet> datac;

	/// main process of the transmitter module
	void evaluate();
	//Packet FIFO[FIFO_TX_DIM];
	/// fifo used to manage case of fast transmitter
	Packet* FIFO;
	int i,last_pos,c;

	ofstream tx_file;

	//SC_CTOR(transmitter)
	/// constructor of the transmitter
	SC_HAS_PROCESS(transmitter);
	transmitter(sc_module_name name_ , int N_R , int N_T, int fifo_tx_dim) : sc_module(name_), m_N_R(N_R), m_N_T(N_T),m_fifo_tx_dim(fifo_tx_dim)
	{
		SC_METHOD(evaluate);
		sensitive<<clk.pos()<<rst_n.neg();

		FIFO= new Packet[m_fifo_tx_dim];

		for (i=0;i<m_fifo_tx_dim;i++)
			FIFO[i]=empty_NL_PDU;

		last_pos=0;

		if(m_N_R > m_N_T)
			c=m_N_R;

		tx_file.open ("tx_file.txt",ios::trunc);
		tx_file << "SIMULATION STARTS NOW: "<<endl<<endl;
		tx_file.close();
	}

private:
	int m_N_R,m_N_T,m_fifo_tx_dim;

};
#endif
