/// \file NOC_package.cpp
/// \brief File containing the description of the functions of the NoC package.
///
/// See also NOC_package.h
#include "NOC_package.h"

//-------------------------------------------------------------------------------------------------------------------------

/// Defined in NOC_package.h.
///
/// It uses 2 steps to decide input priority.
/// Step1: set priority between North and East inputs and between South and West inputs.
/// Step2: set priority between the previous "winners" (highest priority of N vs E and highest priority of S vs W) and the "losers" (lowest priority of N vs E and lowest priority of S vs W).
struct PriorityOrder sort_4input_priority(int No_HC,int Ea_HC,int So_HC,int We_HC)
{
	struct PriorityOrder InputPriorityOrder;
	int NE_max_row,NE_min_row,SW_max_row,SW_min_row;
	int NE_max_prio,NE_min_prio,SW_max_prio,SW_min_prio;


	if (No_HC >= Ea_HC){
	  NE_max_row  = North;           //north input
      NE_min_row  = East;            //east input
      NE_max_prio = No_HC;
      NE_min_prio = Ea_HC;
	  }
    else{
      NE_max_row  = East;            //east input
      NE_min_row  = North;           //north input
      NE_max_prio = Ea_HC;
      NE_min_prio = No_HC;
    }

    if (So_HC >= We_HC){
      SW_max_row  = South;           //south input
      SW_min_row  = West;            //west input
      SW_max_prio = So_HC;
      SW_min_prio = We_HC;
	  }
    else{
      SW_max_row  = West;            //west input
      SW_min_row  = South;           //south input
      SW_max_prio = We_HC;
      SW_min_prio = So_HC;
    }

//second step
    if (NE_max_prio >= SW_max_prio){        //largest NE >= largest SW
		InputPriorityOrder.max_prio = NE_max_row;
      if (NE_min_prio >= SW_max_prio)      //smallest NE >= largest SW
		  InputPriorityOrder.hi_prio = NE_min_row;
      else
        InputPriorityOrder.hi_prio = SW_max_row;
      }
    else{
		InputPriorityOrder.max_prio = SW_max_row;
      if (SW_min_prio > NE_max_prio)       //smallest SW > largest NE
        InputPriorityOrder.hi_prio = SW_min_row;
      else
        InputPriorityOrder.hi_prio = NE_max_row;

	}

    if (NE_min_prio < SW_min_prio){         //smallest NE < smallest SW
		InputPriorityOrder.min_prio = NE_min_row;
      if (NE_max_prio < SW_min_prio)       //largest NE < smallest SW
		  InputPriorityOrder.lo_prio = NE_max_row;
      else
        InputPriorityOrder.lo_prio = SW_min_row;
    }
    else  {
      InputPriorityOrder.min_prio = SW_min_row;
      if (SW_max_prio <= NE_min_prio)      //largest SW < smallest NE
        InputPriorityOrder.lo_prio = SW_max_row;
      else
        InputPriorityOrder.lo_prio = NE_min_row;
	}


	return InputPriorityOrder;
}

//-------------------------------------------------------------------------------------------------------------------------

/// Defined in NOC_package.h.
///
/// It uses 2 steps to decide output priority.
/// Step1: set priority between North and East outputs and between South and West outputs.
/// Step2: set priority between the previous "winners" (highest priority of N vs E and highest priority of S vs W) and the "losers" (lowest priority of N vs E and lowest priority of S vs W).
struct PriorityOrder sort_4output_priority(int NorthLoad,int EastLoad,int SouthLoad,int WestLoad)
{
	struct PriorityOrder OutputPriorityOrder;
	int NE_max_row, NE_min_row, SW_max_row, SW_min_row;
	int NE_max_prio, NE_min_prio, SW_max_prio, SW_min_prio;

	//sort load in output priority order
	//first step
    if (NorthLoad >= EastLoad){
      NE_max_row  = North;           //north input
      NE_min_row  = East;            //east input
      NE_max_prio = NorthLoad;
      NE_min_prio = EastLoad;
	}
    else{
      NE_max_row  = East;            //east input
      NE_min_row  = North;           //north input
      NE_max_prio = EastLoad;
      NE_min_prio = NorthLoad;
    }

    if (SouthLoad >= WestLoad){
      SW_max_row  = South;           //south input
      SW_min_row  = West;            //west input
      SW_max_prio = SouthLoad;
      SW_min_prio = WestLoad;
    }
	else{
      SW_max_row  = West;            //west input
      SW_min_row  = South;           //south input
      SW_max_prio = WestLoad;
      SW_min_prio = SouthLoad;
    }

	//second step
    if (NE_max_prio >= SW_max_prio){        //largest NE >= largest SW
		OutputPriorityOrder.min_prio = NE_max_row;   //highest load, so lowest priority! in fact later outputs are presented from 3(min load) to 0(max load)
      if (NE_min_prio >= SW_max_prio)      //smallest NE >= largest SW
		  OutputPriorityOrder.lo_prio = NE_min_row;
      else
		  OutputPriorityOrder.lo_prio = SW_max_row;
    }
    else{
		OutputPriorityOrder.min_prio = SW_max_row;
      if (SW_min_prio > NE_max_prio)       //smallest SW > largest NE
		  OutputPriorityOrder.lo_prio = SW_min_row;
      else
		  OutputPriorityOrder.lo_prio = NE_max_row;
    }

    if (NE_min_prio < SW_min_prio){         //smallest NE < smallest SW
		OutputPriorityOrder.max_prio = NE_min_row;
      if (NE_max_prio < SW_min_prio)       //largest NE < smallest SW
		  OutputPriorityOrder.hi_prio = NE_max_row;
      else
		  OutputPriorityOrder.hi_prio = SW_min_row;
    }
    else{
		OutputPriorityOrder.max_prio = SW_min_row;
      if (SW_max_prio <= NE_min_prio)      //largest SW < smallest NE
		  OutputPriorityOrder.hi_prio = SW_max_row;
      else
		  OutputPriorityOrder.hi_prio = NE_min_row;
    }



	return OutputPriorityOrder;
}

//-------------------------------------------------------------------------------------------------------------------------

/// Defined in NOC_package.h.
///
/// It analyzes the destination field of the packet and sets the favourite direction of the packet.
sc_bv<4> get_aim(int dest_Row,int dest_Col)
{
	sc_bv<4> direction;

	direction="0000";

	if (dest_Row>0) //if the row of the destination is >0 the packet wants to go north
		direction[North]='1';
	if (dest_Col>0) //if the column of the destination is >0 the packet wants to go east
		direction[East]='1';
	if (dest_Row<0) //if the row of the destination is <0 the packet wants to go south
		direction[South]='1';
	if (dest_Col<0) //if the column of the destination is <0 the packet wants to go west
		direction[West]='1';

	return direction;
}

//-------------------------------------------------------------------------------------------------------------------------

/// Defined in NOC_package.h.
///
/// It decides on which output to send all the valid inputs received.
/// Phase1: calculation on number of valid input (switchload).
/// Phase2: get aiming direction.
/// Phase3: try to send packets to resource.
/// Phase4: fix an output for the highest priority input, ... ,fix an output for the lowest priority input.
/// Phase5: considering how many valid inputs i had, i update the outputs with a new value or I leave the old one with the empty bit = false.
int ctrl_box(struct Packet* Sample_in_N,struct Packet* Sample_in_S,struct Packet* Sample_in_W,struct Packet* Sample_in_E,struct Packet* Sample_in_R,struct PriorityOrder input_priority, struct PriorityOrder output_priority,sc_bit* Resource_ready,int row_sw,int col_sw,int m_row_noc,int m_col_noc)
{
	int switchload=0;
	struct Packet Sample_in[5];
	Sample_in[North]=*Sample_in_N;
	Sample_in[South]=*Sample_in_S;
	Sample_in[West]=*Sample_in_W;
	Sample_in[East]=*Sample_in_E;
	Sample_in[Resource]=*Sample_in_R;
	sc_bv<4> aimMatrix[4];
	sc_bv<4> R_aim;
	sc_bv<5> switched;
	int selectvector[5];
	switched="11111"; // 1=not yet assigned, 0=already assigned ->used to indicate which outputs are already been assigned to an input packet
	selectvector[0]=0; // example: on the north output I send the packet arriving from east: selectvector[Nord]=East (keep in mem the final results!)
	selectvector[1]=0;
	selectvector[2]=0;
	selectvector[3]=0;
	selectvector[4]=0;
	sc_bv<4> thread;



	//start calculation of swload
	if (Sample_in[North].empty==true)
		switchload++;
	if (Sample_in[South].empty==true)
		switchload++;
	if (Sample_in[West].empty==true)
		switchload++;
	if (Sample_in[East].empty==true)
		switchload++;
	//end calculation of swload


	//get aiming direction
	aimMatrix[North]=get_aim(Sample_in[North].dest_addr_R,Sample_in[North].dest_addr_C);
	aimMatrix[East]=get_aim(Sample_in[East].dest_addr_R,Sample_in[East].dest_addr_C);
	aimMatrix[South]=get_aim(Sample_in[South].dest_addr_R,Sample_in[South].dest_addr_C);
	aimMatrix[West]=get_aim(Sample_in[West].dest_addr_R,Sample_in[West].dest_addr_C);

	//try to send packet to resource
	if ((aimMatrix[input_priority.max_prio]=="0000") && (Sample_in[input_priority.max_prio].empty==true))
	{
		selectvector[Resource]=input_priority.max_prio;
		Sample_in[input_priority.max_prio].empty=false;
		switched[Resource]='0';
	}
	else if ((aimMatrix[input_priority.hi_prio]=="0000") && (Sample_in[input_priority.hi_prio].empty==true))
	{
		selectvector[Resource]=input_priority.hi_prio;
		Sample_in[input_priority.hi_prio].empty=false;
		switched[Resource]='0';
	}
	else if ((aimMatrix[input_priority.lo_prio]=="0000") && (Sample_in[input_priority.lo_prio].empty==true))
	{
		selectvector[Resource]=input_priority.lo_prio;
		Sample_in[input_priority.lo_prio].empty=false;
		switched[Resource]='0';
	}
	else if ((aimMatrix[input_priority.min_prio]=="0000") && (Sample_in[input_priority.min_prio].empty==true))
	{
		selectvector[Resource]=input_priority.min_prio;
		Sample_in[input_priority.min_prio].empty=false;
		switched[Resource]='0';
	}
	else
	{
		//selectvector[Resource]=0; not necessary
		switched[Resource]='1'; // finally, when I will see switched[Resource]=1, I'll know that on that output there are no more packets to send-> I'll put on that ouput the same value of before with empty bit set at 0!
	}


	//try to route the received packet with max priority
	thread=(aimMatrix[input_priority.max_prio])&(switched(3,0));
	if (Sample_in[input_priority.max_prio].empty==true) //packet exists
	{
		if (thread=="0000") //desired aim not free, switch oA if free
		{
			if (switched[output_priority.max_prio]=='1') //if output with max priority is free...
			{
				selectvector[output_priority.max_prio]=input_priority.max_prio; //...I send on that output the packet with max priority and I indicate this event in the results vector...
				switched[output_priority.max_prio]='0';//...and I set as "busy/occupied" that output
			}
			else if (switched[output_priority.hi_prio]=='1')
			{
				selectvector[output_priority.hi_prio]=input_priority.max_prio;
				switched[output_priority.hi_prio]='0';
			}
			else if (switched[output_priority.lo_prio]=='1')
			{
				selectvector[output_priority.lo_prio]=input_priority.max_prio;
				switched[output_priority.lo_prio]='0';
			}
			else
			{
				selectvector[output_priority.min_prio]=input_priority.max_prio;
				switched[output_priority.min_prio]='0';
			}

		}
		else // the desired aim is free, I have just to understand which is the right direction to route the packet
		{
			cout<<"th: "<<thread<<endl;
			if (thread[output_priority.max_prio]=='1') //packet aiming output with highest priority
			{
				selectvector[output_priority.max_prio]=input_priority.max_prio;
				switched[output_priority.max_prio]='0';
			}
			else if (thread[output_priority.hi_prio]=='1')
			{
				selectvector[output_priority.hi_prio]=input_priority.max_prio;
				switched[output_priority.hi_prio]='0';
			}
			else if (thread[output_priority.lo_prio]=='1')
			{
				selectvector[output_priority.lo_prio]=input_priority.max_prio;
				switched[output_priority.lo_prio]='0';
			}
			else
			{
				selectvector[output_priority.min_prio]=input_priority.max_prio;
				switched[output_priority.min_prio]='0';
			}
		}
	}

//------------zzzzzzzzzzzzzzzzz-------------------zzzzzzzzzzzzzz--------------------------

	//try to route the received packet with priority 2 (hi)
	thread=(aimMatrix[input_priority.hi_prio])&(switched(3,0));
	if (Sample_in[input_priority.hi_prio].empty==true) //packet exists
	{
		if (thread=="0000") //desired aim not free, switch oA if free
		{
			if (switched[output_priority.max_prio]=='1')
			{
				selectvector[output_priority.max_prio]=input_priority.hi_prio;
				switched[output_priority.max_prio]='0';
			}
			else if (switched[output_priority.hi_prio]=='1')
			{
				selectvector[output_priority.hi_prio]=input_priority.hi_prio;
				switched[output_priority.hi_prio]='0';
			}
			else if (switched[output_priority.lo_prio]=='1')
			{
				selectvector[output_priority.lo_prio]=input_priority.hi_prio;
				switched[output_priority.lo_prio]='0';
			}
			else
			{
				selectvector[output_priority.min_prio]=input_priority.hi_prio;
				switched[output_priority.min_prio]='0';
			}

		}
		else // the desired aim is free, I have just to understand which is the right direction to route the packet
		{
			if (thread[output_priority.max_prio]=='1') //packet aiming output with highest priority
			{
				selectvector[output_priority.max_prio]=input_priority.hi_prio;
				switched[output_priority.max_prio]='0';
			}
			else if (thread[output_priority.hi_prio]=='1')
			{
				selectvector[output_priority.hi_prio]=input_priority.hi_prio;
				switched[output_priority.hi_prio]='0';
			}
			else if (thread[output_priority.lo_prio]=='1')
			{
				selectvector[output_priority.lo_prio]=input_priority.hi_prio;
				switched[output_priority.lo_prio]='0';
			}
			else
			{
				selectvector[output_priority.min_prio]=input_priority.hi_prio;
				switched[output_priority.min_prio]='0';
			}
		}
	}

//------------zzzzzzzzzzzzzzzzz-------------------zzzzzzzzzzzzzz--------------------------

	//try to route the received packet with priority 3 (lo)
	thread=(aimMatrix[input_priority.lo_prio])&(switched(3,0));
	if (Sample_in[input_priority.lo_prio].empty==true) //packet exists
	{
		if (thread=="0000") //desired aim not free, switch oA if free
		{
			if (switched[output_priority.max_prio]=='1')
			{
				selectvector[output_priority.max_prio]=input_priority.lo_prio;
				switched[output_priority.max_prio]='0';
			}
			else if (switched[output_priority.hi_prio]=='1')
			{
				selectvector[output_priority.hi_prio]=input_priority.lo_prio;
				switched[output_priority.hi_prio]='0';
			}
			else if (switched[output_priority.lo_prio]=='1')
			{
				selectvector[output_priority.lo_prio]=input_priority.lo_prio;
				switched[output_priority.lo_prio]='0';
			}
			else
			{
				selectvector[output_priority.min_prio]=input_priority.lo_prio;
				switched[output_priority.min_prio]='0';
			}

		}
		else // the desired aim is free, I have just to understand which is the right direction to route the packet
		{
			if (thread[output_priority.max_prio]=='1') //packet aiming output with highest priority
			{
				selectvector[output_priority.max_prio]=input_priority.lo_prio;
				switched[output_priority.max_prio]='0';
			}
			else if (thread[output_priority.hi_prio]=='1')
			{
				selectvector[output_priority.hi_prio]=input_priority.lo_prio;
				switched[output_priority.hi_prio]='0';
			}
			else if (thread[output_priority.lo_prio]=='1')
			{
				selectvector[output_priority.lo_prio]=input_priority.lo_prio;
				switched[output_priority.lo_prio]='0';
			}
			else
			{
				selectvector[output_priority.min_prio]=input_priority.lo_prio;
				switched[output_priority.min_prio]='0';
			}
		}
	}

//------------zzzzzzzzzzzzzzzzz-------------------zzzzzzzzzzzzzz--------------------------

	//try to route the received packet with minimum priority (min)
	thread=(aimMatrix[input_priority.min_prio])&(switched(3,0));
	if (Sample_in[input_priority.min_prio].empty==true) //packet exists
	{
		if (thread=="0000") //desired aim not free, switch oA if free
		{
			if (switched[output_priority.max_prio]=='1')
			{
				selectvector[output_priority.max_prio]=input_priority.min_prio;
				switched[output_priority.max_prio]='0';
			}
			else if (switched[output_priority.hi_prio]=='1')
			{
				selectvector[output_priority.hi_prio]=input_priority.min_prio;
				switched[output_priority.hi_prio]='0';
			}
			else if (switched[output_priority.lo_prio]=='1')
			{
				selectvector[output_priority.lo_prio]=input_priority.min_prio;
				switched[output_priority.lo_prio]='0';
			}
			else
			{
				selectvector[output_priority.min_prio]=input_priority.min_prio;
				switched[output_priority.min_prio]='0';
			}

		}
		else // the desired aim is free, I have just to understand which is the right direction to route the packet
		{
			if (thread[output_priority.max_prio]=='1') //packet aiming output with highest priority
			{
				selectvector[output_priority.max_prio]=input_priority.min_prio;
				switched[output_priority.max_prio]='0';
			}
			else if (thread[output_priority.hi_prio]=='1')
			{
				selectvector[output_priority.hi_prio]=input_priority.min_prio;
				switched[output_priority.hi_prio]='0';
			}
			else if (thread[output_priority.lo_prio]=='1')
			{
				selectvector[output_priority.lo_prio]=input_priority.min_prio;
				switched[output_priority.lo_prio]='0';
			}
			else
			{
				selectvector[output_priority.min_prio]=input_priority.min_prio;
				switched[output_priority.min_prio]='0';
			}
		}
	}

//------------zzzzzzzzzzzzzzzzz-------------------zzzzzzzzzzzzzz--------------------------
cout<<"switched: "<<switched<<endl;
	//I try to take the packet from the resource if not all the output are busy/occupied
	if (switched(3,0)=="0000") //all output busy
		{
		(*Resource_ready)=(sc_bit)0;

		}
	else
	{
		(*Resource_ready)=(sc_bit)1;
		if (Sample_in[Resource].empty==true) //valid packet
		{
			R_aim=get_aim(Sample_in[Resource].dest_addr_R,Sample_in[Resource].dest_addr_C);
			thread=(R_aim & switched(3,0));
			if (thread=="0000") //desired aim not free
			{
				if (switched[output_priority.max_prio]=='1')
				{
					selectvector[output_priority.max_prio]=Resource;
					switched[output_priority.max_prio]='0';
				}
				else if (switched[output_priority.hi_prio]=='1')
				{
					selectvector[output_priority.hi_prio]=Resource;
					switched[output_priority.hi_prio]='0';
				}
				else if (switched[output_priority.lo_prio]=='1')
				{
					selectvector[output_priority.lo_prio]=Resource;
					switched[output_priority.lo_prio]='0';
				}
				else
				{
					selectvector[output_priority.min_prio]=Resource;
					switched[output_priority.min_prio]='0';
				}
			}
			else //desired aim is free, find which one
			{
				if (thread[output_priority.max_prio]=='1')
				{
					selectvector[output_priority.max_prio]=Resource;
					switched[output_priority.max_prio]='0';
				}
				else if (thread[output_priority.hi_prio]=='1')
				{
					selectvector[output_priority.hi_prio]=Resource;
					switched[output_priority.hi_prio]='0';
				}
				else if (thread[output_priority.lo_prio]=='1')
				{
					selectvector[output_priority.lo_prio]=Resource;
					switched[output_priority.lo_prio]='0';
				}
				else
				{
					selectvector[output_priority.min_prio]=Resource;
					switched[output_priority.min_prio]='0';
				}
			}//end if desired aim
		}
	}

//------------zzzzzzzzzzzzzzzzz-------------------zzzzzzzzzzzzzz--------------------------
//now I have finished to decide what to do with the packets. Now, analyzing the vectors called "switched" and "select" I can manage the packets referenced by pointers

//THIS CODE IS THE SOBSTITUTION OF THE "PACKET JUSTIFIER" FROM VHDL DESCRIPTION. ATTENTION: there is NOT the managing of max HC

	cout<<"switched: "<<switched<<endl;

	/*if (switched(3,0)=="0000") //all output busy
		{
		(*Resource_ready)=(sc_bit)0;
		}
	else
		{
		(*Resource_ready)=(sc_bit)1;
		}<--------------------------------------------------------------------------quiquiquiquiquiquiquiqui*/

	cout<<"SV: "<<selectvector[4]<<","<<selectvector[3]<<","<<selectvector[2]<<","<<selectvector[1]<<","<<selectvector[0]<<endl;

	if (switched[North]=='1') //on north no packet to send
	(*Sample_in_N).empty=false;
	else
	{
	Sample_in[selectvector[North]].HC++;
		if(row_sw!=1){
		Sample_in[selectvector[North]].dest_addr_R--;
		Sample_in[selectvector[North]].source_addr_R++;
		}
	(*Sample_in_N)=Sample_in[selectvector[North]];
	}

	if (switched[South]=='1') //on south no packet to send
	(*Sample_in_S).empty=false;
	else
	{
	Sample_in[selectvector[South]].HC++;
		if(row_sw!=m_row_noc){ //or MAXRIGA in a case different from 3x3
		Sample_in[selectvector[South]].dest_addr_R++;
		Sample_in[selectvector[South]].source_addr_R--;
		}
	(*Sample_in_S)=Sample_in[selectvector[South]];
	}

	if (switched[West]=='1') //on west no packet to send
	(*Sample_in_W).empty=false;
	else
	{
	Sample_in[selectvector[West]].HC++;
		if(col_sw!=1){
		Sample_in[selectvector[West]].dest_addr_C++;
		Sample_in[selectvector[West]].source_addr_C--;
		}
	(*Sample_in_W)=Sample_in[selectvector[West]];
	}

	if (switched[East]=='1') //on east no packet to send
	(*Sample_in_E).empty=false;
	else
	{
	Sample_in[selectvector[East]].HC++;
		if(col_sw!=m_col_noc){
		Sample_in[selectvector[East]].dest_addr_C--;
		Sample_in[selectvector[East]].source_addr_C++;
		}
	(*Sample_in_E)=Sample_in[selectvector[East]];
	}

	if (switched[Resource]=='1') //on resource no packet to send
	(*Sample_in_R).empty=false;
	else
	{
	(*Sample_in_R)=Sample_in[selectvector[Resource]];
	//add this line because were not giving out on resourceout----//
	(*Sample_in_R).empty=true;
	//------------------------------------------------------------//
	}

	//returns the results
	return switchload;
}

//-------------------------------------------------------------------------------------------------------------------------

/// Defined in NOC_package.h.
///
/// It calculates the average load of the switch adding togheter the number of valid inputs in the current cycle with the previous 3 cycles results.
int loadaverager(int switchload,int average_vector[4],sc_bit rst_n)
{
int averageload=0;

if(rst_n=='0'){
	average_vector[0]=0;average_vector[1]=0;average_vector[2]=0;average_vector[3]=0;}
else
{
	average_vector[3]=average_vector[2];
	average_vector[2]=average_vector[1];
	average_vector[1]=average_vector[0];
	average_vector[0]=switchload;

	averageload=average_vector[0]+average_vector[1]+average_vector[2]+average_vector[3];
}
return averageload;
}

//-------------------------------------------------------------------------------------------------------------------------

/// Defined in NOC_package.h.
///
/// It print the output file.
void printout(struct Packet* d_Nso,struct Packet* d_Sso,struct Packet* d_Wso,struct Packet* d_Eso,struct Packet* d_Rso,struct Packet* d_Ns,struct Packet* d_Ss,struct Packet* d_Ws,struct Packet* d_Es,struct Packet* d_Rs,int row,int col,ofstream* myfile)
{
(*myfile).open ("example.txt",ios::ate | ios::app);
(*myfile) << "AT " << sc_simulation_time() << " Coord: ("<< row <<","<< col<<")" <<endl<<endl;
(*myfile) << "IN:	N: " << (*d_Ns) << " S: " << (*d_Ss)<< " W: " << (*d_Ws)<< " E: " << (*d_Es)<< " R: " << (*d_Rs) <<endl;
(*myfile) << "OUT:	N: " << (*d_Nso) << " S: " << (*d_Sso)<< " W: " << (*d_Wso)<< " E: " << (*d_Eso)<< " R: " << (*d_Rso) <<endl<<endl;


(*myfile).close();
return;
}


