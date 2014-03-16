/// \file fifo.h
/// \brief File containing the entity of the fifo component.
///
/// See also fifo.cpp

#ifndef FIFO_H
#define FIFO_H
#include "systemc.h"
#include "NOC_package.h"

/*! \class fifo
    \brief SystemC description of the fifo module.

    Classical clocked fifo with asynchronous reset. The fifo takes inputs every clock period (in the inputs to the fifo have the empty bit = true).
	The fifo fournishes a value to the NoC switch (output) every clock period only if the signal Resource_ready equals zero.
*/
SC_MODULE(fifo)
//class fifo : public sc_module
{
	/// It is used to get input values from a file which represents the resource.
	sc_in<Packet> FIFO_in;
	/// reset signal
	sc_in<sc_bit> rst_n;
	/// Input of the resource ready signal. If the resource is ready (true) i can output values from the fifo every clock period.
	sc_in<sc_bit> Resource_ready;
	/// It gives packets to the switch only when the switch is ready to take a new packet (see resourceready signal).
	sc_out<Packet> FIFO_out;

	/// Row and column of the switch in the Network.
	public:
	/// Row of the switch in the Network.
	int row_fifo;
	/// Col of the switch in the Network.
	int col_fifo;

	/*!	Main clocked process of the fifo module.
    */
	void prc_fifo();

	/// Useful to see how many elements are memorized in the fifo.
	int FIFO_LastPosition;

	/// Useful to see how many elements are memorized in the fifo.
	struct Packet FIFO_vector[10];
	/// Variable used to memorize the next output value of the fifo.
	struct Packet FIFO_reg;
	/// Counter for loops.
	int i;
	/// Clock of the fifo
	sc_clock clkfifo;
	/// Open a file in order to write on it.
	ofstream fifofile;
	/// Signal which samples the input.
	struct Packet FIFO_in_s,FIFO_reg_appoggio;

	/// Constructor: Initializes the fifo (all zeros) and open the fifofile to write on it.
	SC_CTOR(fifo):clkfifo("clkfifo",1,SC_NS)
	{
		SC_METHOD(prc_fifo);
		sensitive<<rst_n<<clkfifo; //is the same of having sensitive<<d<<empty
		//dont_initialize();
		FIFO_LastPosition=0;
		for (i=0;i<10;i++)
		 FIFO_vector[i]=empty_NL_PDU;
		FIFO_reg=empty_NL_PDU;



	fifofile.open ("fifo.txt",ios::trunc);
	fifofile << "SIMULATION STARTS NOW: "<<endl<<endl;
	fifofile.close();
	}
};
#endif
