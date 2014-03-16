/// \file fifo.cpp
/// \brief File containing the description of the processes of the fifo component.
///
/// See also fifo.h

#include "fifo.h"

/// Defined in fifo.h.
///
/// sample the input every clock period.
/// offers a new output every clock period only if the resource ready signal is true.
/// writes on a file all the changes in the fifo (input output and memorized elements) just for debugging.
void fifo::prc_fifo()
{



    FIFO_in_s=FIFO_in;

	if (rst_n==(sc_bit)0)
	{
		for (i=0;i<10;i++)
		 FIFO_vector[i]=empty_NL_PDU;
		FIFO_reg=empty_NL_PDU;
	}
    else if (clkfifo==(sc_bit)1)
	{

		if (FIFO_in_s.empty == true)
		{
          if (FIFO_LastPosition < 10)
			{
            FIFO_vector[FIFO_LastPosition]=FIFO_in_s;
            FIFO_LastPosition++;
			}
          else
            cout<<"FIFO queue full"<<endl;

        }

        if (Resource_ready==(sc_bit)1)
		{

          if (FIFO_LastPosition > 0)
		  {
            //take packet from FIFO
            FIFO_reg=FIFO_vector[0];

            //decrement FIFO queue length
            FIFO_LastPosition--;
            //move packet one step forward in queue
			for (i=0;i<10;i++)
				FIFO_vector[i]=FIFO_vector[i+1];

		  }
          else    //nothing more in FIFO
		  {
			fifofile.open ("fifo.txt",ios::ate | ios::app);
			fifofile << "AT "<<sc_simulation_time()<< " Coord: ("<< row_fifo <<","<< col_fifo<<")" <<" nothing more in fifo "<< endl;
			fifofile.close();
			FIFO_reg=empty_NL_PDU;
			}
        }
        else
		{
			FIFO_reg=FIFO_reg;
			fifofile.open ("fifo.txt",ios::ate | ios::app);
			fifofile << "AT "<<sc_simulation_time()<< " Coord: ("<< row_fifo <<","<< col_fifo<<")" <<" wait...resready è a 0 "<< endl;
			fifofile.close();

		}
	fifofile.open ("fifo.txt",ios::ate | ios::app);
	fifofile << "AT "<<sc_simulation_time()<< " Coord: ("<< row_fifo <<","<< col_fifo<<")" <<" FIFO_vector[0]: "<<FIFO_vector[0]<< endl;
	fifofile << "AT "<<sc_simulation_time()<< " Coord: ("<< row_fifo <<","<< col_fifo<<")" <<" FIFO_in: "<< FIFO_in<<endl;
	fifofile << "AT "<<sc_simulation_time()<< " Coord: ("<< row_fifo <<","<< col_fifo<<")" <<" FIFO_out: "<< FIFO_reg<<endl;
	fifofile << "AT "<<sc_simulation_time()<< " Coord: ("<< row_fifo <<","<< col_fifo<<")" <<" resourceready: "<< Resource_ready<<endl<<endl;

	fifofile.close();


     }
FIFO_out=FIFO_reg;


}