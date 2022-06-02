#ifndef __INTEGRAL_H
#define __INTEGRAL_H

//---IntegralUsingLPF.h¿¡¼­ ¿Å°Ü¿È----
typedef struct {  float  Alpha;  
				  float  EmfAlphaOld;
				  float  AlphaVol;	
				  float  AlphaCurr;
				  float  Beta;	
				  float  EmfBetaOld;
				  float  BetaVol;	
				  float  BetaCurr;
				  float SampT;
				  float	Wc;
				  float	Rs;
				  float Theta;
				  float Mag;
				  float AlphaDt;
				  float BetaDt;
				  float AlphaOld;
				  float BetaOld;
				  float OmegaHat;
				  float OmegaHatLPF;
		 	 	  void  (*calc)();	
				 } INTEGRAL;	            

typedef INTEGRAL *INTEGRAL_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the INTEGRAL object.
-----------------------------------------------------------------------------*/                     
#define INTEGRAL_DEFAULTS { 0, \
                          0, \
                          0, \
                          0, \
                          0, \
						  0, \
						  0, \
						  0, \
						  0, \
						  0, \
						  0, \
                          0, \
                          0, \
                          0, \
                          0, \
                          0, \
                          0, \
                          0, \
                          0, \
              			  (void (*)(Uint32))integral_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in INTEGRALCLARKE.C
------------------------------------------------------------------------------*/
void integral_calc(INTEGRAL_handle);

//---ipark.h¿¡¼­ ¿Å°Ü¿È----
typedef struct {  float  Alpha;  	// Output: stationary d-axis stator variable 
				  float  Beta;	// Output: stationary q-axis stator variable 
				  float  Angle;	// Input: rotating angle (rad) 
				  float  Ds;		// Input: rotating d-axis stator variable 
				  float  Qs;		// Input: rotating q-axis stator variable 
		 	 	  void  (*calc)();	// Pointer to calculation function 
				 } IPARK;	            

typedef IPARK *IPARK_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the IPARK object.
-----------------------------------------------------------------------------*/                     
#define IPARK_DEFAULTS {  0, \
                          0, \
                          0, \
                          0, \
                          0, \
              			  (void (*)(Uint32))ipark_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in IPARK.C
------------------------------------------------------------------------------*/
void ipark_calc(IPARK_handle);

//---park.h¿¡¼­ ¿Å°Ü¿È----
typedef struct {  float  Alpha;  	// Input: stationary d-axis stator variable 
				  float  Beta;	// Input: stationary q-axis stator variable 
				  float  Angle;	// Input: rotating angle (rad) 
				  float  Ds;		// Output: rotating d-axis stator variable 
				  float  Qs;		// Output: rotating q-axis stator variable 
				  float  DsLPF;
				  float  QsLPF;
		 	 	  void  (*calc)();	// Pointer to calculation function 
				 } PARK;	            

typedef PARK *PARK_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the PARK object.
-----------------------------------------------------------------------------*/                     
#define PARK_DEFAULTS {  0, \
                          0, \
                          0, \
                          0, \
                          0, \
                          0, \
                          0, \
              			  (void (*)(Uint32))park_calc }
              			  
/*------------------------------------------------------------------------------
Prototypes for the functions in PARK.C
------------------------------------------------------------------------------*/
void park_calc(PARK_handle);


//---clark.h¿¡¼­ ¿Å°Ü¿È----
typedef struct {  float32  As;  	// Input: phase-a stator variable  
				  float32  Bs;		// Input: phase-b stator variable  
				  float32  Alpha;	// Output: stationary d-axis stator variable 
				  float32  Beta;	// Output: stationary q-axis stator variable 
		 	 	  void  (*calc)();	// Pointer to calculation function
				 } CLARKE;	            

typedef CLARKE *CLARKE_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the CLARKE object.
-----------------------------------------------------------------------------*/                     
#define CLARKE_DEFAULTS { 0, \
                          0, \
                          0, \
                          0, \
              			  (void (*)(Uint32))clarke_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in CLARKE.C
------------------------------------------------------------------------------*/
void clarke_calc(CLARKE_handle);


//--SVGEN_DQ.H¿¡¼­ ¿Å°Ü¿È
typedef struct 	{ float  Ualpha; 			// Input: reference alpha-axis phase voltage                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
				  float  Ubeta;			// Input: reference beta-axis phase voltage                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
				  float  Vdc;
				  float  DutyU;				// Output: reference phase-a switching function		                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
				  float  DutyV;				// Output: reference phase-b switching function                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
				  float  DutyW;				// Output: reference phase-c switching function                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
				  Uint16 Sector;
				  float  inv_Vdc;
				  
				  
				  void (*calc)();	    // Pointer to calculation function                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
				} SVGENDQ;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																				
typedef SVGENDQ *SVGENDQ_handle;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
//Default initalizer for the SVGENDQ object.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
#define SVGENDQ_DEFAULTS { 0,0,0,0,0,0,0,0,\
						(void (*)(Uint32))svgendq_calc }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
//Prototypes for the functions in SVGEN_DQ.C                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
void svgendq_calc(SVGENDQ_handle);          

//PI_DAMP_Controller.h¿¡¼­ ¿Å°Ü¿È

typedef struct {  float  Ref;   			// Input: Reference input 
				  float  Fdb;   			// Input: Feedback input 
				  float  Err;				// Variable: Error
				  float  Kp;				// Parameter: Proportional gain
				  float  Up;				// Variable: Proportional output 
				  float  Ui;				// Variable: Integral output 
				  float  Ud;				// Variable: Derivative output 	
				  float  OutPreSat; 		// Variable: Pre-saturated output
				  float  OutMax;		    // Parameter: Maximum output 
				  float  OutMin;	    	// Parameter: Minimum output
				  float  Out;   			// Output: PID output 
				  float  SatErr;			// Variable: Saturated difference
				  float  Ki;			    // Parameter: Integral gain
				  float  Kc;		     	// Parameter: Integral correction gain
				  float  Kd; 		        // Parameter: Derivative gain
				  float  SampTime; 		        // Parameter: Derivative gain
				  float  Kdamp; 		        // Parameter: Derivative gain
				  float  FF;
				  float  FF_Out;
				  float  Ka;
				  float  Fb;
				  //float  Err1;		   	    // History: Previous proportional output
		 	 	  void  (*calc)();	  	// Pointer to calculation function
				 } PIDAMP;	            

typedef PIDAMP *PIDAMP_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PIDREG3 object.
-----------------------------------------------------------------------------*/                     
#define PIDAMP_DEFAULTS { 0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0,\
                           0, \
                           0,\
              			  (void (*)(Uint32))PI_Damp_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in PIDREG3.C
------------------------------------------------------------------------------*/

void PI_Damp_calc(PIDAMP_handle);

//--- Controller.c-----
void LPF1(float unit_time,float pole,float input,float *output);
void LPF2(float unit_time, float wc, float input,float *output);
void HPF2(float unit_time, float wc, float input, float *output);
void PI_Damp_Controller(float limit,float discrete_time,float damp_factor,float Kp,float Ki,float reference,float feedback,float *integral,float *output);
void PI_Controller(float limit,float discrete_time,float Kp,float Ki,float error,float *integral,float *output);
void RmsCalculationVoltage(float Freq,float *Sum,float *PeriodTime,float Input, float *Output);
void PI_AntiWindup(float limit,float discrete_time,float Kp,float Ki,float error,float *integral,float *output);
void PI_AntiWindup1(float limit,float discrete_time,float Kp,float Ki,float Kt, float error,float *integral,float *output);
#endif


