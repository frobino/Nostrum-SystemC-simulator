/// \file NoC_WR.cpp
/// \brief File containing the description of the processes of the NoC (TESTBENCH).
///
/// See also NoC_WR.h
#include "NoC_WR.h"

/// read input files
void NoC_WR::readin()
{
	myfile3.open ("indata5x5");
	//myfile3.open ("indata_muypacket");
	char rubbish[10];
	int d_addr_C,d_addr_R;
	bool eb;
	int payload;
	int i=0;
	int col,row;

	struct Packet Res;

	i=0;
	rst_n=(sc_bit)1;
	rst_n_bool=true;
	wait(10,SC_NS);
	rst_n=(sc_bit)0;
	rst_n_bool=false;
	wait(10,SC_NS);
	wait(10,SC_NS);
	rst_n=(sc_bit)1;
	rst_n_bool=true;
	wait(60,SC_NS);


	while (i<50) //maximum number of lines that can be read from the indata file
	{
		cout<<"ciao"<<endl;
		myfile3>>rubbish; //cycle identifier
		for (row=1;row<=total_dim_row;row++) //col
		{
			myfile3>>rubbish; //row identifier
			for (col=1;col<=total_dim_col;col++) //col
			{
				myfile3>>eb;
				//cout << eb << endl;
				myfile3>>d_addr_R;
				//cout << d_addr_R << endl;
				myfile3>>d_addr_C;
				//cout << d_addr_C << endl;
				myfile3>>payload;
				//cout << payload << endl;

				Res=empty_NL_PDU;
				Res.dest_addr_R=d_addr_R;
				Res.dest_addr_C=d_addr_C;
				Res.empty=eb;
				Res.payload=payload;


				Resource_in_matrix[row-1][col-1]=Res;

				cout<<"Resource_in_matrix ["<<row<<","<<col<<"]="<<Resource_in_matrix[row-1][col-1]<<endl;
			}

		}
		//wait(10,SC_NS);
		wait(1,SC_NS);
		i++;
	}//end while
	for (row=1;row<=total_dim_row;row++) //col
	{
		for (col=1;col<=total_dim_col;col++) //col
		{
		Res=Resource_in_matrix[row-1][col-1];
		Res.empty=false;
		Resource_in_matrix[row-1][col-1]=Res;
		}
	}
	wait();

}
