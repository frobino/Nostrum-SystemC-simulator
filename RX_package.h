/// \file RX_package.h
/// \brief File containing data structures used to create the wrapper model.
///
/// See also RX_package.cpp

#ifndef RX_PACK_H
#define RX_PACK_H
#include "systemc.h"
#include <iostream>

//--------------------------------------------------------------------------------------

//#define Tsw 100 //sc_PS
/// 0=random function, 1=always sample correct (lucky case of solving metastab), 2=always sample uncorrect (unlucky case of solving metastab)
#define SOLVE_METASTAB 0 // 0=random function, 1=always sample correct (lucky case of solving metastab), 2=always sample uncorrect (unlucky case of solving metastab)

/// function simulating metastability sampling the strobe
sc_bit strobeget(sc_bit strobe,sc_time strobelastchange,int Tsu);

#endif