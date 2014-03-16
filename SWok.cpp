/// \file SWok.cpp
/// \brief File containing the description of the processes of the SWITCH component.
///
/// See also SWok.h

#include "SWok.h"


//descrizione del process
///This process is activated when something changes on the switch inputs and the time is right to calculate where to send the data inputs.
///It is divided in different phases:
/// * evaluate input and output priority
/// * calculation of where to send the valid input packets using the ctrl_box function (see NoC_package)
/// * update outputs: if there is a valid packet to send, put it on the correct output, if not, leave the old output with the empty bit=0
void SWok::prc_SWok()
{

if (clk==true)
{ // inizio if clk'event and clk==1
if(rst_n==(sc_bit)1){

	d_Ns=d_N;
	d_Ss=d_S;
	d_Ws=d_W;
	d_Es=d_E;
	if (Resource_ready_old==(sc_bit)1)
		d_Rs=d_R;

	sample_in_N=d_N;
	sample_in_S=d_S;
	sample_in_W=d_W;
	sample_in_E=d_E;
	if (Resource_ready_old==(sc_bit)1)
		sample_in_R=d_R;

	sample_in_NorthLoad=NorthLoad;
	sample_in_SouthLoad=SouthLoad;
	sample_in_EastLoad=EastLoad;
	sample_in_WestLoad=WestLoad;



//------------------------------------DECIDE IF TO NOTIFY OR NOT--------------------------------------------------



	//if all input packets are empty
	if ((sample_in_N.empty==false)&&(sample_in_S.empty==false)&&(sample_in_W.empty==false)&&(sample_in_E.empty==false)&&(sample_in_R.empty==false))
		{
		old_out_N.empty=false;
		d_No=old_out_N;
		old_out_S.empty=false;
		d_So=old_out_S;
		old_out_W.empty=false;
		d_Wo=old_out_W;
		old_out_E.empty=false;
		d_Eo=old_out_E;
		old_out_R.empty=false;
		d_Ro=old_out_R;

		cout<<"ALL IN packets emtpy"<<endl;
		printout(&old_out_N,&old_out_S,&old_out_W,&old_out_E,&old_out_R,&d_Ns,&d_Ss,&d_Ws,&d_Es,&d_Rs,row_sw,col_sw,&myfile);
		d_Resource_ready=(sc_bit)1;
		averageload=loadaverager(0,average_vector,rst_n);
		myfile.open ("example.txt",ios::ate | ios::app);
		myfile << "AT "<<sc_simulation_time()<<" Northload: "<< sample_in_NorthLoad <<" Southload: " <<sample_in_SouthLoad<<" Eastload: " <<sample_in_EastLoad<<" Westload: " <<sample_in_WestLoad<<endl;
		myfile << "Averageload: "<< averageload <<endl<<endl;
		//myfile << "RReady: "<< Resource_ready<<endl<<endl;
		myfile.close();
		}

	//else i have a valid new packet to transmit AND i have also to update the d_addr and s_addr fields and to check for virtual circuits so:
	// code for managing d_addr and s_addr fields
	else
		{

		//manage routing (RETURN PACKET in the correct order--use pointers)

		struct PriorityOrder input_priority,output_priority;

		input_priority=sort_4input_priority(sample_in_N.HC,sample_in_E.HC,sample_in_S.HC,sample_in_W.HC);
		output_priority=sort_4output_priority(sample_in_NorthLoad,sample_in_EastLoad,sample_in_SouthLoad,sample_in_WestLoad);

		int switchload;
		sc_bit Resource_ready;

		switchload=ctrl_box(&sample_in_N,&sample_in_S,&sample_in_W,&sample_in_E,&sample_in_R,input_priority,output_priority,&Resource_ready,row_sw,col_sw,m_row_noc,m_col_noc);
		d_Resource_ready=Resource_ready;
		Resource_ready_old=Resource_ready;

		averageload=loadaverager(switchload,average_vector,rst_n);

		//down here I decide if and how to send them:
		//if the input signal "empty" (no valid packet) is set i leave the same output as before
		if (sample_in_N.empty==false)
		{
		old_out_N.empty=false;
		d_No=old_out_N;
		d_Nso=old_out_N;

		cout<<"SI pacchetto emtpy"<<endl;
		}

		//else i have a valid new packet to transmit AND i have also to update the d_addr and s_addr fields and to check for virtual circuits so:
		// code for managing d_addr and s_addr fields
		else
		{
		d_No=sample_in_N;
		d_Nso=sample_in_N;
		//evaluate_in.notify
		cout<<"NON ho letto emtpy"<<endl;
		}
		//end_if


		if (sample_in_S.empty==false)
		{
		old_out_S.empty=false;
		d_So=old_out_S;
		d_Sso=old_out_S;
		cout<<"SI pacchetto emtpy"<<endl;
		}

		else
		{
		d_So=sample_in_S;
		d_Sso=sample_in_S;
		//evaluate_in.notify
		cout<<"NON ho letto emtpy"<<endl;
		}

		if (sample_in_W.empty==false)
		{
		old_out_W.empty=false;
		d_Wo=old_out_W;
		d_Wso=old_out_W;
		cout<<"SI pacchetto emtpy"<<endl;
		}

		else
		{
		d_Wo=sample_in_W;
		d_Wso=sample_in_W;
		//evaluate_in.notify
		cout<<"NON ho letto emtpy"<<endl;
		}

		if (sample_in_E.empty==false)
		{
		old_out_E.empty=false;
		d_Eo=old_out_E;
		d_Eso=old_out_E;
		cout<<"SI pacchetto emtpy"<<endl;
		}

		else
		{
		d_Eo=sample_in_E;
		d_Eso=sample_in_E;
		//evaluate_in.notify
		cout<<"NON ho letto emtpy"<<endl;
		}

		if (sample_in_R.empty==false)
		{
		old_out_R.empty=false;
		d_Ro=old_out_R;
		d_Rso=old_out_R;
		//d_Resource_ready=(sc_bit)0;
		cout<<"SI pacchetto emtpy"<<endl;
		}

		else
		{
		d_Ro=sample_in_R;
		d_Rso=sample_in_R;
		//d_Resource_ready=(sc_bit)1;
		//evaluate_in.notify
		cout<<"NON ho letto emtpy"<<endl;
		}
		printout(&d_Nso,&d_Sso,&d_Wso,&d_Eso,&d_Rso,&d_Ns,&d_Ss,&d_Ws,&d_Es,&d_Rs,row_sw,col_sw,&myfile);
		myfile.open ("example.txt",ios::ate | ios::app);
		myfile << "AT "<<sc_simulation_time()<<" Northload: "<< sample_in_NorthLoad <<" Southload: " <<sample_in_SouthLoad<<" Eastload: " <<sample_in_EastLoad<<" Westload: " <<sample_in_WestLoad<<endl;
		myfile << "Switchload: "<< switchload<< " Averagevector: "<<average_vector[0] <<", "<<average_vector[1] <<", "<<average_vector[2] <<", "<<average_vector[3] <<", "<<endl;
		myfile << "Averageload: "<< averageload <<endl<<endl;
		myfile.close();
		/*myfile.open ("example.txt",ios::ate | ios::app);
		myfile << "AT "<<sc_simulation_time()<< "r: "<<row_sw <<"c: "<<col_sw<<" Northload: "<< NorthLoad <<" Southload: " <<SouthLoad<<" Eastload: " <<EastLoad<<" Westload: " <<WestLoad<<endl<<endl;
		myfile.close();
		*/
		}
	//end_if


//-----------------------------------------END NOTIFICATION DECISION---------------------------------------------------

	//remember2=remember;



	if (sample_in_N.empty==true)
	{
		old_out_N=sample_in_N;
	}
	sample_in_N=d_Ns;
	sample_in_NorthLoad=NorthLoad;


	if (sample_in_S.empty==true)
	{
		old_out_S=sample_in_S;
	}
	sample_in_S=d_Ss;
	sample_in_SouthLoad=SouthLoad;


	if (sample_in_W.empty==true)
	{
		old_out_W=sample_in_W;
	}
	sample_in_W=d_Ws;
	sample_in_WestLoad=WestLoad;


	if (sample_in_E.empty==true)
	{
		old_out_E=sample_in_E;
	}
	sample_in_E=d_Es;
	sample_in_EastLoad=EastLoad;


	if (sample_in_R.empty==true)
	{
		old_out_R=sample_in_R;
	}
	sample_in_R=d_Rs;





//---------------------------------------------------------------------------------------------------------------------------------------------------------

	/*
	if(((old_out_N.payload==sample_in_N.payload)&&(old_out_S.payload==sample_in_S.payload)&&(old_out_W.payload==sample_in_W.payload)&&(old_out_E.payload==sample_in_E.payload)&&(old_out_R.payload==sample_in_R.payload)) ||((d_Ns.empty==false)&&(d_Ss.empty==false)&&(d_Ws.empty==false)&&(d_Es.empty==false)&&(d_Rs.empty==false))) //<------------- (remember2==remember): 2 input uguali di seguito sono arrivati -> disattivo il clk indipendedtemente da validit�;
	{
		d_Resource_ready=(sc_bit)1;																//empty==(sc_bit)(true): empty mi � arrivato=> disattivo il clk
		infotoclk=0;
	}
	else
		infotoclk=1;*/
}//end if rst_n
else{
	sample_in_N.empty=false;
	sample_in_S.empty=false;
	sample_in_W.empty=false;
	sample_in_E.empty=false;
	sample_in_R.empty=false;

	sample_in_N.dest_addr_R=0;
	sample_in_S.dest_addr_R=0;
	sample_in_W.dest_addr_R=0;
	sample_in_E.dest_addr_R=0;
	sample_in_R.dest_addr_R=0;

	sample_in_N.dest_addr_C=0;
	sample_in_S.dest_addr_C=0;
	sample_in_W.dest_addr_C=0;
	sample_in_E.dest_addr_C=0;
	sample_in_R.dest_addr_C=0;

	sample_in_N.HC=0;
	sample_in_S.HC=0;
	sample_in_W.HC=0;
	sample_in_E.HC=0;
	sample_in_R.HC=0;

	sample_in_N.payload=0;
	sample_in_S.payload=0;
	sample_in_W.payload=0;
	sample_in_E.payload=0;
	sample_in_R.payload=0;

	sample_in_N.source_addr_R=0;
	sample_in_S.source_addr_R=0;
	sample_in_W.source_addr_R=0;
	sample_in_E.source_addr_R=0;
	sample_in_R.source_addr_R=0;

	sample_in_N.source_addr_C=0;
	sample_in_S.source_addr_C=0;
	sample_in_W.source_addr_C=0;
	sample_in_E.source_addr_C=0;
	sample_in_R.source_addr_C=0;

	old_out_N.empty=false;
	old_out_S.empty=false;
	old_out_W.empty=false;
	old_out_E.empty=false;
	old_out_R.empty=false;

	old_out_N.dest_addr_R=0;
	old_out_S.dest_addr_R=0;
	old_out_W.dest_addr_R=0;
	old_out_E.dest_addr_R=0;
	old_out_R.dest_addr_R=0;

	old_out_N.dest_addr_C=0;
	old_out_S.dest_addr_C=0;
	old_out_W.dest_addr_C=0;
	old_out_E.dest_addr_C=0;
	old_out_R.dest_addr_C=0;

	old_out_N.HC=0;
	old_out_S.HC=0;
	old_out_W.HC=0;
	old_out_E.HC=0;
	old_out_R.HC=0;

	old_out_N.payload=0;
	old_out_S.payload=0;
	old_out_W.payload=0;
	old_out_E.payload=0;
	old_out_R.payload=0;

	old_out_N.source_addr_R=0;
	old_out_S.source_addr_R=0;
	old_out_W.source_addr_R=0;
	old_out_E.source_addr_R=0;
	old_out_R.source_addr_R=0;

	old_out_N.source_addr_C=0;
	old_out_S.source_addr_C=0;
	old_out_W.source_addr_C=0;
	old_out_E.source_addr_C=0;
	old_out_R.source_addr_C=0;

	d_No=sample_in_N;
	d_So=sample_in_S;
	d_Wo=sample_in_W;
	d_Eo=sample_in_E;
	d_Ro=sample_in_R;
	d_Resource_ready=(sc_bit)1;

	average_vector[0]=0;average_vector[1]=0;average_vector[2]=0;average_vector[3]=0;
	sample_in_NorthLoad=0;sample_in_SouthLoad=0;sample_in_WestLoad=0;sample_in_EastLoad=0;

	printout(&sample_in_N,&sample_in_S,&sample_in_W,&sample_in_E,&sample_in_R,&d_Ns,&d_Ss,&d_Ws,&d_Es,&d_Rs,row_sw,col_sw,&myfile); //<----- print new

}//end else rst_n


}//fine if clk'event and clk==1

}

