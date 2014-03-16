/// \file SWok.h
/// \brief File containing the entity of the Switch unclocked.
///
/// See also SWok.cpp

#ifndef SWok_H
#define SWok_H
#include "systemc.h"
#include "NOC_package.h"

/*! \class SWok
    \brief SystemC description of the unclocked switch module (structural description).

    Structural description of the unclocked switch.
*/
SC_MODULE(SWok)
//class SWok : public sc_module
{
	//input port
	///Inputs for the datas from the adjacent switches
	sc_in<Packet> d_N;
	///Inputs for the datas from the adjacent switches
	sc_in<Packet> d_S;
	///Inputs for the datas from the adjacent switches
	sc_in<Packet> d_W;
	///Inputs for the datas from the adjacent switches
	sc_in<Packet> d_E;
	///Inputs for the datas from the adjacent switches
	sc_in<Packet> d_R;

	///Asynchronous reset
	sc_in<sc_bit> rst_n;
	///Clock signal
	sc_in<bool> clk;
	///Inputs for the average loads from the adjacent switches
	sc_in<int> NorthLoad;
	///Inputs for the average loads from the adjacent switches
	sc_in<int> SouthLoad;
	///Inputs for the average loads from the adjacent switches
	sc_in<int> EastLoad;
	///Inputs for the average loads from the adjacent switches
	sc_in<int> WestLoad;

	public:
	/// indication of the postition of the switch in the NoC
	int row_sw;
	/// indication of the postition of the switch in the NoC
	int col_sw;
	//int t_ACTUAL;


	//output port
	///Outputs for the datas to the adjacent switches
	sc_out<Packet> d_No;
	///Outputs for the datas to the adjacent switches
	sc_out<Packet> d_So;
	///Outputs for the datas to the adjacent switches
	sc_out<Packet> d_Wo;
	///Outputs for the datas to the adjacent switches
	sc_out<Packet> d_Eo;
	///Outputs for the datas to the adjacent switches
	sc_out<Packet> d_Ro;
	///Output to the fifo
	sc_out<sc_bit> d_Resource_ready;
	///Outputs for the averageload to the adjacent switches
	sc_out<int> averageload;


	///Packet variable used to memorize immefiately the value of the input (signals need evaluate-update cycle)
	Packet d_Ns;
	///Packet variable used to memorize immefiately the value of the input (signals need evaluate-update cycle)
	Packet d_Ss;
	///Packet variable used to memorize immefiately the value of the input (signals need evaluate-update cycle)
	Packet d_Ws;
	///Packet variable used to memorize immefiately the value of the input (signals need evaluate-update cycle)
	Packet d_Es;
	///Packet variable used to memorize immefiately the value of the input (signals need evaluate-update cycle)
	Packet d_Rs;
	///Packet variable used to memorize immefiately the value of the output (signals need evaluate-update cycle)
	Packet d_Nso;
	///Packet variable used to memorize immefiately the value of the output (signals need evaluate-update cycle)
	Packet d_Sso;
	///Packet variable used to memorize immefiately the value of the output (signals need evaluate-update cycle)
	Packet d_Wso;
	///Packet variable used to memorize immefiately the value of the output (signals need evaluate-update cycle)
	Packet d_Eso;
	///Packet variable used to memorize immefiately the value of the output (signals need evaluate-update cycle)
	Packet d_Rso;
	///Packets variables used to memorize immefiately the value of the input (signals need evaluate-update cycle)
	Packet sample_in_N;
	///Packets variables used to memorize immefiately the value of the output (signals need evaluate-update cycle)
	Packet old_out_N;
	///Packets variables used to memorize immefiately the value of the input (signals need evaluate-update cycle)
	Packet sample_in_S;
	///Packets variables used to memorize immefiately the value of the output (signals need evaluate-update cycle)
	Packet old_out_S;
	///Packets variables used to memorize immefiately the value of the input (signals need evaluate-update cycle)
	Packet sample_in_W;
	///Packets variables used to memorize immefiately the value of the output (signals need evaluate-update cycle)
	Packet old_out_W;
	///Packets variables used to memorize immefiately the value of the input (signals need evaluate-update cycle)
	Packet sample_in_E;
	///Packets variables used to memorize immefiately the value of the output (signals need evaluate-update cycle)
	Packet old_out_E;
	///Packets variables used to memorize immefiately the value of the input (signals need evaluate-update cycle)
	Packet sample_in_R;
	///Packets variables used to memorize immefiately the value of the output (signals need evaluate-update cycle)
	Packet old_out_R;
	///Vector used to calculate average load
	int average_vector[4];
	///Variable used to memorize immefiately the value of the input (signals need evaluate-update cycle)
	int sample_in_NorthLoad;
	///Variable used to memorize immefiately the value of the input (signals need evaluate-update cycle)
	int sample_in_SouthLoad;
	///Variable used to memorize immefiately the value of the input (signals need evaluate-update cycle)
	int sample_in_WestLoad;
	///Variable used to memorize immefiately the value of the input (signals need evaluate-update cycle)
	int sample_in_EastLoad;
	///output debug file
	ofstream myfile;
	///Used to decide if to sample or not the data from the resource
	sc_bit Resource_ready_old;

	//prototipo del process tipo method
	///Main process: it is activated when the switch has to send packets in the NoC
	void prc_SWok();

	//constructor (contiene definizione process + sensitivity list associata)
	//SC_CTOR(SWok)
	/// Indication that we want to explicit the constructor
	SC_HAS_PROCESS(SWok);
	/// Constructor: Initializes the switch and open the output file to write on it.
	SWok(sc_module_name name_ , int row_noc , int col_noc) : sc_module(name_), m_row_noc(row_noc), m_col_noc(col_noc)
	{
		SC_METHOD(prc_SWok);
		sensitive<<clk<<rst_n; //is the same of having sensitive<<d<<empty
		dont_initialize();
		average_vector[0]=0;average_vector[1]=0;average_vector[2]=0;average_vector[3]=0;
		sample_in_NorthLoad=0;sample_in_SouthLoad=0;sample_in_WestLoad=0;sample_in_EastLoad=0;

		myfile.open ("example.txt",ios::trunc);
		myfile << "SIMULATION STARTS NOW: "<<endl<<endl;
		myfile.close();

		Resource_ready_old=(sc_bit)1;
	}

	private:
		int m_col_noc,m_row_noc;

};
#endif
