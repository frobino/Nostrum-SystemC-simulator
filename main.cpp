/// \file main.cpp
///
/// \brief The main file of the program.
///
/// It contains the main function. The main function instantiates the NOC component and starts the simulation with the command sc_start().
/// In addition the main function can measure the simulation time using some WINDOWS/UNIX commands.

#include "SWok.h"
//#include "threadtime.h"
#include "systemc.h"
#include <time.h>
//#include "windows.h"
#include "NOC_package.h"
#include "fifo.h"
#include "NoC_WR.h"

  
/// \brief Main function of the progam.
///
/// It's the most important function.
/// Generate the NoC system instantiating the NoC1 component.
/// Windows users: check the simulation time using 2 SYSTEMTIME variables and the function GetSystemTime from "windows.h".
/// For testing or to generate waves: use the technique shown in the code between //#INIT_WAVE and //#FINISH_WAVE. If not interested in waves, it is possible to cancel that part of code.
int sc_main (int argc,char *argv[])
{
		

	NoC_WR *NoC1;
	NoC1=new NoC_WR("NoC1");

	

	//SYSTEMTIME now,now1;
  
	//#INIT_WAVE
	sc_trace_file* tracefile;
	tracefile=sc_create_vcd_trace_file("wave");

	sc_trace(tracefile,NoC1->wrapperNS[4-1][1-1]->clkL,"clkL");
	sc_trace(tracefile,NoC1->wrapperNS[4-1][1-1]->clkR,"clkR");
	sc_trace(tracefile,NoC1->wrapperNS[4-1][1-1]->L_datain,"L_datain");
	sc_trace(tracefile,NoC1->wrapperNS[4-1][1-1]->R_datain,"R_datain");
	sc_trace(tracefile,NoC1->wrapperNS[4-1][1-1]->L_dataout,"L_dataout");
	sc_trace(tracefile,NoC1->wrapperNS[4-1][1-1]->R_dataout,"R_dataout");
	sc_trace(tracefile,NoC1->wrapperNS[4-1][1-1]->L_strobe_s,"L_strobe_s");
	sc_trace(tracefile,NoC1->wrapperNS[4-1][1-1]->R_strobe_s,"R_strobe_s");
	sc_trace(tracefile,NoC1->wrapperNS[4-1][1-1]->L_datac_s,"L_datac_s");
	sc_trace(tracefile,NoC1->wrapperNS[4-1][1-1]->R_datac_s,"R_datac_s");
	sc_trace(tracefile,NoC1->wrapperNS[4-1][1-1]->rx_RtoL.save_register_N,"save_register_N");
	sc_trace(tracefile,NoC1->wrapperNS[4-1][1-1]->rx_RtoL.save_register_P,"save_register_P");
	sc_trace(tracefile,NoC1->wrapperNS[4-1][1-1]->rx_RtoL.saved_packet,"saved_packet");
	
	sc_trace_file* tracefile2;
	tracefile2=sc_create_vcd_trace_file("wave2");

	sc_trace(tracefile2,NoC1->wrapperWE[5-1][3-1]->clkL,"clkL");
	sc_trace(tracefile2,NoC1->wrapperWE[5-1][3-1]->clkR,"clkR");
	sc_trace(tracefile2,NoC1->wrapperWE[5-1][3-1]->L_datain,"L_datain");
	sc_trace(tracefile2,NoC1->wrapperWE[5-1][3-1]->R_datain,"R_datain");
	sc_trace(tracefile2,NoC1->wrapperWE[5-1][3-1]->L_dataout,"L_dataout");
	sc_trace(tracefile2,NoC1->wrapperWE[5-1][3-1]->R_dataout,"R_dataout");
	sc_trace(tracefile2,NoC1->wrapperWE[5-1][3-1]->L_strobe_s,"L_strobe_s");
	sc_trace(tracefile2,NoC1->wrapperWE[5-1][3-1]->R_strobe_s,"R_strobe_s");
	sc_trace(tracefile2,NoC1->wrapperWE[5-1][3-1]->L_datac_s,"L_datac_s");
	sc_trace(tracefile2,NoC1->wrapperWE[5-1][3-1]->R_datac_s,"R_datac_s");
	sc_trace(tracefile2,NoC1->wrapperWE[5-1][3-1]->rx_LtoR.datac_container[0],"datac_container[0]");
	sc_trace(tracefile2,NoC1->wrapperWE[5-1][3-1]->rx_LtoR.datac_container[1],"datac_container[1]");
	//#FINISH_WAVE

	//GetSystemTime(&now);
		
	sc_start(1000,SC_NS);
	
	//GetSystemTime(&now1);
	//cout<<(now1.wMilliseconds-now.wMilliseconds)<<endl;

	//#INIT_WAVE
	sc_close_vcd_trace_file(tracefile);
	sc_close_vcd_trace_file(tracefile2);
	//#FINISH_WAVE
	
	return 0;
}
