/// \file NoC_package.h
/// \brief File containing data structures used to create the NoC model. It also contains prototypes of functions used by the switch to decide where to send the packets;
///
/// See also NoC_package.cpp

#ifndef MYTYPE_H
#define MYTYPE_H
#include "systemc.h"
#include <iostream>
#include <fstream>

//--------------------------------------------------------------------------------------

/// Struct used to store characteristics of the different subnets
struct subnet
{
/// dimension of the subnet (total rows number)
int dim_row; //subnet dimension in the network (4x3, 2x1, ...)
/// dimension of the subnet (total cols number)
int dim_col; //subnet dimension in the network (4x3, 2x1, ...)

///position of the top left switch of the subnet in the entire NoC (row coordinate)
int pos_row; //subnet position in the network
///position of the top left switch of the subnet in the entire NoC (column coordinate)
int pos_col; //subnet position in the network
} ;

//--------------------------------------------------------------------------------------

/// Struct of the packet: field for destination address (row,column), field for source address (row,column), empty bit, hop counter, payload
struct Packet{
	/// row of the destination address
	int dest_addr_R;
	/// col of the destination address
	int dest_addr_C;
	/// row of the source address
	int source_addr_R;
	/// col of the source address
	int source_addr_C;
	/// empty bit: true=valid pck, false=invalid;
	bool empty; //true=valid pck, false=invalid;
	/// hop counter field
	int HC;
	//sc_bv<96> payload;
	/// payload field
	int payload;

	/// describe the = operator
	inline Packet& operator = (const Packet& rhs) {

    dest_addr_R= rhs.dest_addr_R;
	dest_addr_C= rhs.dest_addr_C;
    source_addr_R= rhs.source_addr_R;
	source_addr_C= rhs.source_addr_C;
    empty= rhs.empty;
    HC= rhs.HC;
    payload= rhs.payload;

    return *this;
    }

	/// describe the == operator
    inline bool operator == (const Packet & rhs) const {
    return (rhs.dest_addr_R == dest_addr_R &&
			rhs.dest_addr_C == dest_addr_C &&
    		rhs.source_addr_R == source_addr_R &&
			rhs.source_addr_C == source_addr_C &&
    		rhs.empty == empty &&
    		rhs.HC == HC &&
    		rhs.payload == payload
    		);
    }


	/// describe the sc_trace operator (what to do in case of sc_trace)
    inline friend void sc_trace(sc_trace_file *tf, const Packet & v,
    const std::string & NAME ) {
      sc_trace(tf,v.dest_addr_R, NAME + ".dest_addr_R");
	  sc_trace(tf,v.dest_addr_C, NAME + ".dest_addr_C");
      sc_trace(tf,v.source_addr_R, NAME + ".source_addr_R");
      sc_trace(tf,v.source_addr_C, NAME + ".source_addr_C");
	  sc_trace(tf,v.empty, NAME + ".empty");
      sc_trace(tf,v.HC, NAME + ".HC");
      sc_trace(tf,v.payload, NAME + ".payload");

    }

	/// describe the << operator (what to do in case of cout<< )
    inline friend ostream& operator << ( ostream& os,  Packet const & v ) {
      os << "(" << v.dest_addr_R << "," << std::boolalpha << v.dest_addr_C << "," << std::boolalpha << v.source_addr_R << "," << std::boolalpha << v.source_addr_C << "," << std::boolalpha << v.empty << "," << std::boolalpha << v.HC << "," << std::boolalpha << v.payload << ")";
      return os;
    }

};

//--------------------------------------------------------------------------------------

/// data structure to remember input packets/output priority
struct PriorityOrder{
	/// index of the I/O selected to be the one with maximum priority
	int max_prio;
	/// index of the I/O selected to be the one with high priority
	int hi_prio;
	/// index of the I/O selected to be the one with low priority
	int lo_prio;
	/// index of the I/O selected to be the one with minimum priority
	int min_prio;
};

/// Function which decides the priorities of the input packets. Decision based on the hop counter field of every valid packet received.
struct PriorityOrder sort_4input_priority(int No_HC,int Ea_HC,int So_HC,int We_HC);
/// Function which decides the order in which outputs are proposed to the high priority input packets. Decision based on the load average of the closest switches.
struct PriorityOrder sort_4output_priority(int NorthLoad,int EastLoad,int SouthLoad,int WestLoad);

//--------------------------------------------------------------------------------------

/// Function which analyzes destination field of the input packet and resturns the preferred output in order
sc_bv<4> get_aim(int dest_Row,int dest_Col);

//--------------------------------------------------------------------------------------

/// The most important function of the switch. After analyzing input/output priority, etc... this function decides exactly how to send the input packets, moving them to the right outputs;
int ctrl_box(struct Packet* Sample_in_N,struct Packet* Sample_in_S,struct Packet* Sample_in_W,struct Packet* Sample_in_E,struct Packet* Sample_in_R,struct PriorityOrder input_priority, struct PriorityOrder output_priority,sc_bit* Resource_ready,int row_sw,int col_sw,int m_row_noc,int m_col_noc);
//--------------------------------------------------------------------------------------

/// Calculate the load average of the switch
int loadaverager(int switchload,int average_vector[4],sc_bit rst_n);

//--------------------------------------------------------------------------------------

/// Create the output file example.txt where there are all the movements of the packets in the NoC
void printout(struct Packet* d_Nso,struct Packet* d_Sso,struct Packet* d_Wso,struct Packet* d_Eso,struct Packet* d_Rso,struct Packet* d_Ns,struct Packet* d_Ss,struct Packet* d_Ws,struct Packet* d_Es,struct Packet* d_Rs,int row,int col,ofstream* myfile);


//--------------------------------------------------------------------------------------

/// constant used to refer to the Resource I/O
const int Resource = 4;
/// constant used to refer to the North I/O
const int North = 3;
/// constant used to refer to the East I/O
const int East = 2;
/// constant used to refer to the South I/O
const int South = 1;
/// constant used to refer to the West I/O
const int West = 0;
/// empty packet structure
const struct Packet empty_NL_PDU={0,0,0,0,false,0,0};


//--------------------------------------------------------------------------------------

#endif

