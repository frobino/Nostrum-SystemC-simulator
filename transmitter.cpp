/// \file transmitter.cpp
/// \brief File containing the description of the processes of the fifo component.
///
/// See also transmitter.h
#include "transmitter.h"

/// Defined in transmitter.h.
///
/// Using the Algorithm 1 it manage situations of fast transmitter or fast receiver.
void transmitter::evaluate()
{
	Packet data_in_s;

	data_in_s=datain;

	if (rst_n==(sc_bit)0)
	{
		datac=empty_NL_PDU;
		strobe=(sc_bit)0;
		full=(sc_bit)0;
		for (i=0;i<m_fifo_tx_dim;i++)
			FIFO[i]=empty_NL_PDU;

		last_pos=0;

		if(m_N_R > m_N_T)
			c=m_N_R;
	}
	else // if clk'event and clk=1
	{

		if(data_in_s.empty==true)
		{
			FIFO[last_pos]=data_in_s;
			last_pos++;
			if(last_pos>=m_fifo_tx_dim)
			{
				full=(sc_bit)1;
				last_pos=0;
			}
		}

		//ALGORITMO1
		if (m_N_R<=m_N_T)
		{
			//always send
			strobe = ~strobe;
			datac=FIFO[0];
			for (i=0;i<m_fifo_tx_dim-1;i++)
				FIFO[i]=FIFO[i+1];
			FIFO[m_fifo_tx_dim-1]=empty_NL_PDU;
			if(last_pos>0)
				{
					last_pos--;

				}
		}
		else if(m_N_R > m_N_T)
		{
			if(c>(m_N_R-m_N_T))
			{
				//send
				datac=FIFO[0];
				for (i=0;i<m_fifo_tx_dim-1;i++)
					FIFO[i]=FIFO[i+1];
				FIFO[m_fifo_tx_dim-1]=empty_NL_PDU;
				if(last_pos>0)
				{
					last_pos--;
					cout<<"ciuz"<<endl;
				}
				strobe = ~strobe;

				c = c- (m_N_R-m_N_T);
			}
			else
			{
				//dont send
				c = c+m_N_T;
			}
		}
		/*else
		{
			r <= 1;
		}*/

	tx_file.open ("tx_file.txt",ios::ate | ios::app);
	tx_file << "AT "<<sc_simulation_time()<<" name: "<< this->name() <<" data_in: " <<data_in_s<<" dataC: " <<datac<<endl;
	tx_file.close();

	}
}