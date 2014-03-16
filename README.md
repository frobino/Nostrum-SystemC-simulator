Nostrum-SystemC-simulator
=========================

SystemC simulator of a highly customizable Nostrum network-on-chip (NoC).

INPUT FILES:
Follow the models included in the sw. There is not control on the correctness of the input files.

* indata5x5:
  start every new cycle with a line indicating the number n of the cycle: n-cycle.
  for every cycle specify row by row the inputs from the resources to the fifos.
  So, for example, if we have specified in the map file that the NoC is 5x5, we expect to find 5 lines for each row, specifying the inputs of the 5 resources each row.

  Example: NoC 3x2 (3 rows, 2 columns)

  1-cycle
  1-row
  //line 1 specifying inputs from resource in position (1,1)
  //line 2 specifying inputs from resource in position (1,2)
  2-row
  //line 1 specifying inputs from resource in position (2,1)
  //line 2 specifying inputs from resource in position (2,2)
  3-row
  //line 1 specifying inputs from resource in position (3,1)
  //line 2 specifying inputs from resource in position (3,2)
  2-cycle
  1-row
  ...
  
  every input packet is specified in this way:
  empty_bit | destination_addr_row | destination_addr_col | payload
      1 		-4 			4 		101
  
  with a single space between the numbers

  1-cycle
  1-row
  1 -4 4 101
  ...
  
* map.txt: 
  in this file we give the informations about how the NoC is configured.

  First line: it specifies the total dimension of the NoC. It needs 3 words (in the example, "dimensione totale rete(row,col):").
  We could also write "total NoC dimension(row,col):". Then 2 numbers divided by spaces must follow: the first is the rows number, the second the columns number.

  Second line: 3 words ("number of subnets:") and 1 number representing it.

  Following lines: for each subnet we need to specify 4 lines.
    1) 3 words ("subnet 0 dimension(row,col):") and the subnet dimension as 2 numbers;
    2) 3 words ("subnet 0 position(row,col):") and then 2 numbers indicating the coordinates (row,col) of the top left switch of the subnet;
    3) 3 words ("subnet 0 clock period(SC_NS):") and the number specifying N
    4) 3 words ("subnet 0 N:") and the number specifying N

    the example attached to the sw create a noc like the following:

    22333
    22333
    55544
    55544
