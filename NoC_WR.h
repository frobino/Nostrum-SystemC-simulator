/// \file NoC_WR.h
/// \brief File containing the entity of the NoC. It put togheter switches, wrappers and fifos;
///
/// See also NoC_WR.cpp

#ifndef NoC_WR_H
#define NoC_WR_H
#include "systemc.h"
#include "NOC_package.h"
#include "SWok.h"
#include "fifo.h"
#include "wrapper_mod.h"

/*! \class NoC_WR
    \brief SystemC description of the NoC module (structural description).

    Structural description of the NoC. Instantiation and connection of switches, wrappers and fifos. Everything is modular.
*/
//SC_MODULE(NoC_WR)
class NoC_WR : public sc_module
{

public:
/// The matrix of signals which connects the hypotetical resource (represented by the indata file) and the input of fifo
sc_signal<Packet>** Resource_in_matrix;
/// The matrix of signals which receives the packet from the switch to the resource
sc_signal<Packet>** Resource_out_matrix;
/// The matrix of signals which connects the output of the fifo and the resource input of every switch
sc_signal<Packet>** fout;

/// The matrix of signals which connects the switches in the vertical direction (north I with wrapper)
sc_signal<Packet>** North_in;
/// The matrix of signals which connects the switches in the vertical direction (north O with wrapper)
sc_signal<Packet>** North_out;
/// The matrix of signals which connects the switches in the horizontal direction (west I with wrapper)
sc_signal<Packet>** West_in;
/// The matrix of signals which connects the switches in the horizontal direction (west O with wrapper)
sc_signal<Packet>** West_out;
/// The matrix of signals which connects the switches in the horizontal direction (east I with wrapper)
sc_signal<Packet>** East_in;
/// The matrix of signals which connects the switches in the horizontal direction (east O with wrapper)
sc_signal<Packet>** East_out;
/// The matrix of signals which connects the switches in the vertical direction (south I with wrapper)
sc_signal<Packet>** South_in;
/// The matrix of signals which connects the switches in the vertical direction (south O with wrapper)
sc_signal<Packet>** South_out;

/// The matrix of signals used to connect the switches of the last column to itself (east out returns on east in)
sc_signal<Packet>** East_inout;
/// The matrix of signals used to connect the switches of the last row to itself (south out returns on south in)
sc_signal<Packet>** South_inout;
/// The matrix of signals used to connect the switches of the first row to itself (north out returns on north in)
sc_signal<Packet>** North_inout;
/// The matrix of signals used to connect the switches of the first column to itself (west out returns on west in)
sc_signal<Packet>** West_inout;

/// The matrix of signals used to connect the load average informations
sc_signal<int>** averageload;//it works as averageload and as avout
/// The signal used to set to zero the averageload input of the switch at the limit of the region
sc_signal<int> averageload_at0;

/// The matrix of signals used to connect the switch signals which indicates the request of new resource data between switch and fifo
sc_signal<sc_bit>** Resource_Ready_matrix;

/// The signal for the asynchronous reset
sc_signal<sc_bit> rst_n; //>NO MATRIX<
/// reset in bool format
sc_signal<bool> rst_n_bool;

/// Matrix of pointers to switch structures
SWok*** s;

/// Matrix of pointers to wrapper structures
wrapper_mod*** wrapperNS;
/// Matrix of pointers to wrapper structures
wrapper_mod*** wrapperWE;

/// Matrix of pointers to fifo structures
fifo*** f;

/// File which rapresent the inputs from the resource to the NoC
ifstream myfile3;
/// Function reading the input files and creating the necessary data structures
void readin();

//----elements for map-------

/// Matrix created from map.txt input file and used as a map
int** the_map;
/// Vector in which information about the subnets are stored
subnet* subnets;
/// Vector in which information about the clock of the subnets are stored
int* clock_periods;
/// Matrix of clocks for the subnets
sc_clock** clocks;
/// entire NoC rows number
int total_dim_row;
/// entire NoC columns number
int total_dim_col;
/// number of subnets in the NoC
int number_subnets;
/// counter
int general_count;
/// row counter
int row_count;
/// col counter
int col_count;
/// row counter
int row;
/// col counter
int col;
/// String necessary to remove unuseful string from the input files
char rubbish[20];
/// String necessary to instantiate components in SystemC (they need a name)
char tmp_str[20];
/// File which represents the inputs from the resource to the NoC
ifstream mapfile;

//------config wrappers--------
/// Variebles for configuring the wrappers
int* Ns;
/// Varieble for configuring the wrappers
int N_left;
/// Varieble for configuring the wrappers
int N_right;

/// constructor of the NoC
SC_CTOR(NoC_WR)
{
	//initialization
	averageload_at0=0;

	//read from file and create map
	mapfile.open ("map.txt");

	mapfile>>rubbish;mapfile>>rubbish;mapfile>>rubbish; //total row dimensions(row,col):
	mapfile>>total_dim_row;
	mapfile>>total_dim_col;
	//create dinamically map-matrix
	the_map = new int*[total_dim_row];
	for (general_count=0;general_count<total_dim_row;general_count++)
	{
		the_map[general_count] = new int[total_dim_col];
	}


	mapfile>>rubbish;mapfile>>rubbish;mapfile>>rubbish; //number of subnets:
	mapfile>>number_subnets;
	//create dinamically info subnets structure
	subnets=new subnet[number_subnets];
	clock_periods=new int[number_subnets];
	Ns=new int[number_subnets];

	clocks=new sc_clock*[number_subnets];

	for (general_count=0;general_count<number_subnets;general_count++)
	{
	mapfile>>rubbish;mapfile>>rubbish;mapfile>>rubbish; //subnet n dimension(row,col):
	mapfile>>subnets[general_count].dim_row;
	mapfile>>subnets[general_count].dim_col;

	mapfile>>rubbish;mapfile>>rubbish;mapfile>>rubbish; //subnet n position(row,col):
	mapfile>>subnets[general_count].pos_row;
	mapfile>>subnets[general_count].pos_col;

	mapfile>>rubbish;mapfile>>rubbish;mapfile>>rubbish;mapfile>>rubbish; //subnet n clock period(SC_NS):
	mapfile>>clock_periods[general_count];

	mapfile>>rubbish;mapfile>>rubbish;mapfile>>rubbish; //subnet n N:
	mapfile>>Ns[general_count];

	sprintf(tmp_str,"clk%d",general_count);

	clocks[general_count]=new sc_clock(tmp_str,clock_periods[general_count],SC_NS);

	}

	mapfile.close();

	for (general_count=0;general_count<number_subnets;general_count++)
	{
		for (row_count=subnets[general_count].pos_row;row_count<((subnets[general_count].pos_row)+(subnets[general_count].dim_row));row_count++)
		{
			for (col_count=subnets[general_count].pos_col;col_count<((subnets[general_count].pos_col)+(subnets[general_count].dim_col));col_count++)
			{
				the_map[row_count][col_count]=general_count;
			}
		}
	}

	//instantiation data structures and mapping (now I have the map)

	Resource_in_matrix = new sc_signal<Packet>*[total_dim_row];
	Resource_out_matrix = new sc_signal<Packet>*[total_dim_row];
	fout = new sc_signal<Packet>*[total_dim_row];
	North_in = new sc_signal<Packet>*[total_dim_row];
	North_out = new sc_signal<Packet>*[total_dim_row];
	West_in = new sc_signal<Packet>*[total_dim_row];
	West_out = new sc_signal<Packet>*[total_dim_row];
	East_in = new sc_signal<Packet>*[total_dim_row];
	East_out = new sc_signal<Packet>*[total_dim_row];
	South_in = new sc_signal<Packet>*[total_dim_row];
	South_out = new sc_signal<Packet>*[total_dim_row];
	East_inout = new sc_signal<Packet>*[total_dim_row];
	South_inout = new sc_signal<Packet>*[total_dim_row];
	North_inout = new sc_signal<Packet>*[total_dim_row];
	West_inout = new sc_signal<Packet>*[total_dim_row];

	averageload = new sc_signal<int>*[total_dim_row];

	Resource_Ready_matrix= new sc_signal<sc_bit>*[total_dim_row];

	s=new SWok**[total_dim_row];
	f=new fifo**[total_dim_row];
	wrapperNS=new wrapper_mod**[total_dim_row+1];
	wrapperWE=new wrapper_mod**[total_dim_row];

	for (row_count=0;row_count<total_dim_row;row_count++)
		{
		Resource_in_matrix[row_count] = new sc_signal<Packet>[total_dim_col];
		Resource_out_matrix[row_count] = new sc_signal<Packet>[total_dim_col];
		fout[row_count] = new sc_signal<Packet>[total_dim_col];
		North_in[row_count] = new sc_signal<Packet>[total_dim_col];
		North_out[row_count] = new sc_signal<Packet>[total_dim_col];
		West_in[row_count] = new sc_signal<Packet>[total_dim_col];
		West_out[row_count] = new sc_signal<Packet>[total_dim_col];
		East_in[row_count] = new sc_signal<Packet>[total_dim_col];
		East_out[row_count] = new sc_signal<Packet>[total_dim_col];
		South_in[row_count] = new sc_signal<Packet>[total_dim_col];
		South_out[row_count] = new sc_signal<Packet>[total_dim_col];

		East_inout[row_count] = new sc_signal<Packet>[total_dim_col];
		South_inout[row_count] = new sc_signal<Packet>[total_dim_col];
		North_inout[row_count] = new sc_signal<Packet>[total_dim_col];
		West_inout[row_count] = new sc_signal<Packet>[total_dim_col];

		averageload[row_count] = new sc_signal<int>[total_dim_col];

		Resource_Ready_matrix[row_count]= new sc_signal<sc_bit>[total_dim_col];

		s[row_count]=new SWok*[total_dim_col];
		f[row_count]=new fifo*[total_dim_col];
		//wrapperNS[row_count]=new wrapper_mod*[total_dim_col];
		wrapperWE[row_count]=new wrapper_mod*[total_dim_col+1];

		}

	for (row_count=0;row_count<(total_dim_row+1);row_count++)
		{
		wrapperNS[row_count]=new wrapper_mod*[total_dim_col];
		}


	//--------> switchmapping

	for (row=1;row<=total_dim_row;row++)// to NumberOfRows generate //row
		{
			for (col=1;col<=total_dim_col;col++) //col
			{
				sprintf(tmp_str,"s%d%d",row,col);
				s[row-1][col-1]=new SWok(tmp_str,total_dim_row,total_dim_col);


				s[row-1][col-1]->row_sw=row;
				s[row-1][col-1]->col_sw=col;
				//map input e output packets
				s[row-1][col-1]->d_N(North_in[row-1][col-1]);
				s[row-1][col-1]->d_No(North_out[row-1][col-1]);

				s[row-1][col-1]->d_W(West_in[row-1][col-1]);
				s[row-1][col-1]->d_Wo(West_out[row-1][col-1]);

				//s[row-1][col-1]->d_S(North_out[row+1-1][col-1]);
				//s[row-1][col-1]->d_So(North_in[row+1-1][col-1]);
				s[row-1][col-1]->d_S(South_in[row-1][col-1]);
				s[row-1][col-1]->d_So(South_out[row-1][col-1]);


				//s[row-1][col-1]->d_E(West_out[row-1][col+1-1]);
				//s[row-1][col-1]->d_Eo(West_in[row-1][col+1-1]);
				s[row-1][col-1]->d_E(East_in[row-1][col-1]);
				s[row-1][col-1]->d_Eo(East_out[row-1][col-1]);

				s[row-1][col-1]->d_R(fout[row-1][col-1]);
				s[row-1][col-1]->d_Ro(Resource_out_matrix[row-1][col-1]);

				s[row-1][col-1]->NorthLoad(averageload_at0);
				s[row-1][col-1]->SouthLoad(averageload_at0);
				s[row-1][col-1]->EastLoad(averageload_at0);
				s[row-1][col-1]->WestLoad(averageload_at0);

				//map output load
				s[row-1][col-1]->averageload(averageload[row-1][col-1]);

				//map output resource ready
				s[row-1][col-1]->d_Resource_ready(Resource_Ready_matrix[row-1][col-1]);
				s[row-1][col-1]->rst_n(rst_n);

				//map clk
				s[row-1][col-1]->clk(*clocks[(the_map[row-1][col-1])]);

			}
	}

	//--------> wrappperNS mapping

	for (row=1;row<=(total_dim_row+1);row++)// to NumberOfRows generate //row
			{
				for (col=1;col<=total_dim_col;col++) //col
				{
					if(row==1){
					N_left=Ns[the_map[row-1][col-1]];
					N_right=Ns[the_map[row-1][col-1]];
					}
					else if(row==(total_dim_row+1)){
					N_left=Ns[the_map[row-2][col-1]];
					N_right=Ns[the_map[row-2][col-1]];
					}
					else{
					//--------I added this because it was exchanging Nright and Nleft

					//N_left=Ns[the_map[row-1][col-1]];
					//N_right=Ns[the_map[row-2][col-1]];
					N_right=Ns[the_map[row-1][col-1]];
					N_left=Ns[the_map[row-2][col-1]];
					}

					sprintf(tmp_str,"wNS%d%d",row,col);
					//wrapperNS[row-1][col-1]=new wrapper_mod(tmp_str);
					wrapperNS[row-1][col-1]=new wrapper_mod(tmp_str,N_left,N_right);

					wrapperNS[row-1][col-1]->rst_n(rst_n_bool);

					if(row==1){
						wrapperNS[row-1][col-1]->R_dataout(North_inout[row-1][col-1]);
						wrapperNS[row-1][col-1]->R_datain(North_inout[row-1][col-1]);}
					else{
						wrapperNS[row-1][col-1]->R_dataout(South_in[row-2][col-1]);
						wrapperNS[row-1][col-1]->R_datain(South_out[row-2][col-1]);}

					if(row==(total_dim_row+1)){
						wrapperNS[row-1][col-1]->L_dataout(South_inout[row-2][col-1]);
						wrapperNS[row-1][col-1]->L_datain(South_inout[row-2][col-1]);}
					else{
						wrapperNS[row-1][col-1]->L_dataout(North_in[row-1][col-1]);
						wrapperNS[row-1][col-1]->L_datain(North_out[row-1][col-1]);}
					//clks
					if(row==1){
						wrapperNS[row-1][col-1]->clkL(*clocks[the_map[row-1][col-1]]);
						wrapperNS[row-1][col-1]->clkR(*clocks[the_map[row-1][col-1]]);}
					else if(row==(total_dim_row+1)){
						wrapperNS[row-1][col-1]->clkL(*clocks[the_map[row-2][col-1]]);
						wrapperNS[row-1][col-1]->clkR(*clocks[the_map[row-2][col-1]]);}
					else{
						wrapperNS[row-1][col-1]->clkL(*clocks[the_map[row-1][col-1]]);
						wrapperNS[row-1][col-1]->clkR(*clocks[the_map[row-2][col-1]]);}

				}
			}


	//--------> wrappperWE mapping

	for (row=1;row<=total_dim_row;row++)// to NumberOfRows generate //row
			{
				for (col=1;col<=(total_dim_col+1);col++) //col
				{
					if(col==1){
					N_left=Ns[the_map[row-1][col-1]];
					N_right=Ns[the_map[row-1][col-1]];
					}
					else if(col==(total_dim_col+1)){
					N_left=Ns[the_map[row-1][col-2]];
					N_right=Ns[the_map[row-1][col-2]];
					}
					else{
					//--------I added this because it was exchanging Nright and Nleft

					//N_left=Ns[the_map[row-1][col-2]];
					//N_right=Ns[the_map[row-1][col-1]];
					N_right=Ns[the_map[row-1][col-2]];
					N_left=Ns[the_map[row-1][col-1]];
					}

					sprintf(tmp_str,"wWE%d%d",row,col);
					//wrapperWE[row-1][col-1]=new wrapper_mod(tmp_str);
					wrapperWE[row-1][col-1]=new wrapper_mod(tmp_str,N_left,N_right);

					wrapperWE[row-1][col-1]->rst_n(rst_n_bool);

					if(col==1){
						wrapperWE[row-1][col-1]->L_dataout(West_inout[row-1][col-1]);
						wrapperWE[row-1][col-1]->L_datain(West_inout[row-1][col-1]);}
					else{
						wrapperWE[row-1][col-1]->L_dataout(East_in[row-1][col-2]);
						wrapperWE[row-1][col-1]->L_datain(East_out[row-1][col-2]);}

					if(col==(total_dim_col+1)){
						wrapperWE[row-1][col-1]->R_dataout(East_inout[row-1][col-2]);
						wrapperWE[row-1][col-1]->R_datain(East_inout[row-1][col-2]);}
					else{
						wrapperWE[row-1][col-1]->R_dataout(West_in[row-1][col-1]);
						wrapperWE[row-1][col-1]->R_datain(West_out[row-1][col-1]);}
					//clks
					if(col==1){
						wrapperWE[row-1][col-1]->clkL(*clocks[the_map[row-1][col-1]]);
						wrapperWE[row-1][col-1]->clkR(*clocks[the_map[row-1][col-1]]);}
					else if(col==(total_dim_col+1)){
						wrapperWE[row-1][col-1]->clkL(*clocks[the_map[row-1][col-2]]);
						wrapperWE[row-1][col-1]->clkR(*clocks[the_map[row-1][col-2]]);}
					else{
						wrapperWE[row-1][col-1]->clkL(*clocks[the_map[row-1][col-2]]);
						wrapperWE[row-1][col-1]->clkR(*clocks[the_map[row-1][col-1]]);}

				}
			}

	//--------> fifo mapping
	for (row=1;row<=total_dim_row;row++)// to NumberOfRows generate //row
	{
		for (col=1;col<=total_dim_col;col++) //col
		{
		sprintf(tmp_str,"f%d%d",row,col);
		f[row-1][col-1]=new fifo(tmp_str);
		f[row-1][col-1]->FIFO_in(Resource_in_matrix[row-1][col-1]);
		f[row-1][col-1]->rst_n(rst_n);
		f[row-1][col-1]->Resource_ready(Resource_Ready_matrix[row-1][col-1]);
		f[row-1][col-1]->FIFO_out(fout[row-1][col-1]);
		f[row-1][col-1]->row_fifo=row;
		f[row-1][col-1]->col_fifo=col;
		}
	}


	//addson
	SC_THREAD(readin);
}


};

#endif
