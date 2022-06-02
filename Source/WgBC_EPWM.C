#include "Global_Variable_Function.h"


void 	InitEPwm5_AcDcConLD();
void 	InitEPwm6_AcDcConLG();
	
void 	InitEPwm1_BachDcDcConLDHs();
void 	InitEPwm2_BachDcDcConLGHs();	


void InitTzGpio(void);

//==================================================================================//
// 함수명 : InitWGEpwm()			    										//
// 기  능 : WG EPWM  초기화 											//
// DATE   : 2014. 12. 29															//
//==================================================================================//
void InitWGEpwm(void){
	// Stop all the TB clocks
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
	EDIS;
	// Trip Zone
	//InitTzGpio();

	InitEPwm1Gpio();
	InitEPwm2Gpio();
//	InitEPwm3Gpio();// boost---사용안함
	
	InitEPwm1();	// dcdc leading leg
	InitEPwm2();	// dcdc lagging leg
//	InitEPwm3();	// boost---사용안함
	


	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;// Start all the timers synced
	EDIS;

	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	EALLOW;

	PieVectTable.EPWM1_TZINT = &epwm1_tzint_isr;// 트립존 인터럽트는 EPWM1만 사용, 나머지 ePWM은 출력만 HIZ로
	
	PieVectTable.EPWM1_INT = &epwm1_timer_isr;
	//	PieVectTable.EPWM2_INT = &epwm2_timer_isr;
	//	PieVectTable.EPWM3_INT = &epwm3_timer_isr;
	//PieVectTable.EPWM4_INT = &epwm4_timer_isr;
	PieVectTable.EPWM5_INT = &epwm5_timer_isr;	
 
    EDIS;
	
	// Enable CPU INT3 which is connected to EPWM1-3 INT:
   	IER |= M_INT2;

	// Enable EPWM INTn in the PIE: Group 2 interrupt 1-3
   	//PieCtrlRegs.PIEIER2.bit.INTx1 = 1;
   	//PieCtrlRegs.PIEIER2.bit.INTx2 = 1;
	
	// Enable EPWM INTn in the PIE: Group 3 interrupt 1/2/3/4
	//PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx3 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx4 = 1;
	
	// Enable CPU INT3 which is connected to EPWM1/2/3/4 INT
	IER |= M_INT3;	
}


void EPwmInterruptEnable()
{
	// Enable EPWM INTn in the PIE: Group 3 interrupt 1/2/3/4
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx3 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx4 = 1;
	  //PieCtrlRegs.PIEIER3.bit.INTx5 = 1;
}
void EPwmInterruptDisable()
{
	// Enable EPWM INTn in the PIE: Group 3 interrupt 1/2/3/4
	PieCtrlRegs.PIEIER3.bit.INTx1 = 0;
	//PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx3 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx4 = 1;
	  //PieCtrlRegs.PIEIER3.bit.INTx5 = 0;
}


//---------------------------------------------------------------------------
// Example: InitTzGpio: 
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as Trip Zone (TZ) pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.  

void InitTzGpio(void)
{
   EALLOW;
   
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.
//  GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;    // Enable pull-up on GPIO12 (TZ1)
//  GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;    // Enable pull-up on GPIO13 (TZ2)
//   GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;    // Enable pull-up on GPIO14 (TZ3)
//   GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;    // Enable pull-up on GPIO15 (TZ4)

   GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;    // Enable pull-up on GPIO16 (TZ5)
// GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up on GPIO28 (TZ5)

//   GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;    // Enable pull-up on GPIO17 (TZ6) 
// GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;    // Enable pull-up on GPIO29 (TZ6)  
   
/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.  
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 3;  // Asynch input GPIO12 (TZ1)
//   GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3;  // Asynch input GPIO13 (TZ2)
//   GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3;  // Asynch input GPIO14 (TZ3)
//   GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (TZ4)

   GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3;  // Asynch input GPIO16 (TZ5)
// GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (TZ5)

//   GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;  // Asynch input GPIO17 (TZ6) 
// GpioCtrlRegs.GPAQSEL2.bit.GPIO29 = 3;  // Asynch input GPIO29 (TZ6)  

   
/* Configure TZ pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be TZ functional pins.
// Comment out other unwanted lines.   
//   GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;  // Configure GPIO12 as TZ1
//   GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;  // Configure GPIO13 as TZ2
//   GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;  // Configure GPIO14 as TZ3
//   GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;  // Configure GPIO15 as TZ4

   GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 3;  // Configure GPIO16 as TZ5
// GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3;  // Configure GPIO28 as TZ5

//  GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 3;  // Configure GPIO17 as TZ6               
// GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 3;  // Configure GPIO29 as TZ6  

   EDIS;
}


//-----------------------------------------------------------//
void InitEPwm1(void)
{
  
    EPwm1Regs.TBPRD = EPWM_DCDC_TBPRD;                  // Set timer period
   // EPwm1Regs.CMPA = EPWM_DCDC_TBPRD/2;                  // 50%
   
    EPwm1Regs.TBPHS.half.TBPHS = 0x0000;           // Phase is 0
    EPwm1Regs.TBCTR = 0x0000;                      // Clear counter
 
    // Setup TBCLK
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;   // Down count mode
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE; //(0)-// Disable phase loading
 
    EPwm1Regs.TBCTL.bit.HSPCLKDIV 	= 0;       	   // Clock ratio to SYSCLKOUT
    EPwm1Regs.TBCTL.bit.CLKDIV 		= 0;		   // TBCLK = SYSCLOCKOUT/(HSPCLKDIV x CLKDIV) 	
 
    EPwm1Regs.TBCTL.bit.SYNCOSEL = 0x1;		  //Send synchrosygnal to ePWM2 when CTR = zero	

    // Setup compare 
    EPwm1Regs.CMPA.half.CMPA = EPWM_DCDC_TBPRD/2;	  //PWM1 compare unit initialization (half period)
	 
	 EPwm1Regs.TBCTL.bit.PRDLD = 0;				  //Load TBPHS when TBCTR = 0.			

    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;             
	 EPwm1Regs.AQCTLA.bit.PRD = AQ_CLEAR;

    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;// 추가
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;

	 EPwm1Regs.AQCTLB.all = 0x00;     // No EPWMxB signal from Action Qualifier (AQ)   
   
    //Deadband configuration
    EPwm1Regs.DBCTL.bit.IN_MODE   = 0x00;  // EPWMxA is signal to RED and FED
    EPwm1Regs.DBCTL.bit.POLSEL    = 0x02;  // EPWMxA is noninverted bur EPWMxB is inverted
    EPwm1Regs.DBCTL.bit.OUT_MODE  = 0x03;  // EPWMxA and EPWMxB are fully enabled     
   
    //Deadband's rising and falling edges configuration
    EPwm1Regs.DBRED               = DEADTIME_DCDC;    //Set rising  edge delay 
    EPwm1Regs.DBFED               = DEADTIME_DCDC;   //Set falling edge delay 
   
	 EPwm1Regs.ETSEL.bit.SOCAEN    = 1;     // Enable SOCA
    EPwm1Regs.ETSEL.bit.SOCASEL   = 1;     // Enable CNT_zero event for SOCA
    EPwm1Regs.ETPS.bit.SOCAPRD    = 1;     // Generate SOCA on the 1st event
	 EPwm1Regs.ETCLR.bit.SOCA      = 1;     // Clear SOCA flag

	 EPwm1Regs.ETSEL.bit.SOCBEN    = 1;     // Enable SOCA
    EPwm1Regs.ETSEL.bit.SOCBSEL   = 1;     // Enable CNT_zero event for SOCA
    EPwm1Regs.ETPS.bit.SOCBPRD    = 1;     // Generate SOCA on the 1st event
	 EPwm1Regs.ETCLR.bit.SOCB      = 1;     // Clear SOCA flag


	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTRU_CMPA;     // Select INT on Zero event// +일경우 파형좋음, -일경우 나쁨
 	EPwm1Regs.ETSEL.bit.INTEN = 1;                // Enable INT
	EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event
	EPwm1Regs.ETCLR.bit.INT = 1;     // Enable more interrupts	
 	
 	 
	 return; 
}

void InitEPwm2()
{
	EALLOW;
	GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on GPIO2 (EPWM2A)
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;    // Disable pull-up on GPIO3 (EPWM2B)
	/* Configure EPwm-2 pins using GPIO regs*/
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B
	EDIS;
	
	EPwm2Regs.TBPRD = EPWM_DCDC_TBPRD;                         // Set timer period
 //  EPwm2Regs.TBPHS.half.TBPHS = PWM_PERIOD/4;            // Initial phase of PWM_2 (arbitrarily)

	EPwm2Regs.TBPHS.half.TBPHS = 0;            // Initial phase of PWM_2 (arbitrarily)
	EPwm2Regs.TBCTR = 0x0000;                             // Clear counter
   
	// Setup TBCLK
	EPwm2Regs.TBCTL.bit.CTRMODE = 0x00; 	    	// Up count mode
   

	EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE; //	(1)- ENABLE      	 	// Load TBCTR wherever the synchrosignal appears

	EPwm2Regs.TBCTL.bit.PRDLD = 0;				  //Load TBPHS when TBCTR = 0.
   
	EPwm2Regs.TBCTL.bit.HSPCLKDIV =  0;       		// Clock ratio to SYSCLKOUT
	EPwm2Regs.TBCTL.bit.CLKDIV    =  0;             // Slow just to observe on the scope

	EPwm2Regs.TBCTL.bit.SYNCOSEL = 0x1;			   //Send synchrosygnal to ePWM3 when CNT2 = 0 

   // Setup compare 
	EPwm2Regs.CMPA.half.CMPA = EPWM_DCDC_TBPRD/2;		//PWM2 compare unit initialization (half period)
   
 	// PWM2 pin A polarity

	EPwm2Regs.AQCTLA.bit.CAU = AQ_SET	;             
	EPwm2Regs.AQCTLA.bit.PRD = AQ_CLEAR;

	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;// 추가
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
     
	EPwm2Regs.AQCTLB.all = 0x00;   		    // No EPWMxB signal from Action Qualifier (AQ)  
    
   
   //Deadband configuration
	EPwm2Regs.DBCTL.bit.IN_MODE   = 0x00;  // EPWMxA is signal to RED and FED
	EPwm2Regs.DBCTL.bit.POLSEL    = 0x02;  // EPWMxA is noninverted bur EPWMxB is inverted
	EPwm2Regs.DBCTL.bit.OUT_MODE  = 0x03;  // EPWMxA and EPWMxB are fully enabled     
   
   //Deadband's rising and falling edges configuration
    
	EPwm2Regs.DBRED               = DEADTIME_DCDC;     //Set rising  edge delay 
	EPwm2Regs.DBFED               = DEADTIME_DCDC;    //Set falling edge delay 

	//HRPWM configuring
	EALLOW;
	EPwm2Regs.HRCNFG.all 		 = 0; //Clear HRCNFG register
	EPwm2Regs.HRCNFG.bit.CTLMODE = 1; //Phase control mode
	EPwm2Regs.HRCNFG.bit.EDGMODE = 3; //MEP control of both edges
	EDIS;
	//---조심!!! pwm 초기화 안하면 초기 기동시 최대듀티 나갈수 있음
	EPwm2Regs.TBPHS.half.TBPHS = 0;//DutyCount;				//phase shift value
	EPwm2Regs.TBPHS.half.TBPHSHR = 0;
  

}
void InitEPwm3()
{
//EPWM_BOOST_TBPRD_HALF
	// Setup TBCLK
	//EPwm3Regs.TBPRD = EPWM_DCDC_TBPRD_HALF;        		// Set timer period
	EPwm3Regs.TBPRD = EPWM_BOOST_TBPRD_HALF;        		// Set timer period
	EPwm3Regs.TBPHS.half.TBPHS = 0x0000;       		// Phase is 0
	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 		// Count up/down
	EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;    		// Master module
	EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;		// sync down-stream module
	// Setup shadow register load on ZERO
	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	// Set actions
	EPwm3Regs.AQCTLA.bit.ZRO = AQ_SET;			// set actions for EPWM1A
	EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;		
	EPwm3Regs.AQCTLB.bit.PRD = AQ_SET;			// set actions for EPWM1B
	EPwm3Regs.AQCTLB.bit.CBD = AQ_CLEAR;		
	
	EPwm3Regs.TBCTR = 0x0000;                  // Clear counter
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;   // Clock ratio to SYSCLKOUT/2
	EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV2;

	EPwm3Regs.CMPA.half.CMPA = 0;    // Set compare A value init
//	EPwm3Regs.CMPB = 0;              // Set Compare B value init
	EPwm3Regs.CMPB = EPWM_BOOST_TBPRD_HALF;              // Set Compare B value init

	EPwm3Regs.PCCTL.bit.CHPEN = 0;  // Disable PWM-chopping function
/*	 
	// Interrupt where we will change the Compare Values
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
	EPwm1Regs.ETSEL.bit.INTEN = 1;                // Enable INT
	EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event
	EPwm1Regs.ETCLR.bit.INT = 1;     // Enable more interrupts
	*/
}



void InitEPwm1Gpio()
{
   
   EALLOW;
	
	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;    // Disable pull-up on GPIO1 (EPWM1B)

	/* Configure EPWM-1 pins using GPIO regs*/

   GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B

   EDIS;

	return;
}

void InitEPwm2Gpio()
{
   EALLOW;

   GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on GPIO2 (EPWM2A)
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;    // Disable pull-up on GPIO3 (EPWM2B)

	/* Configure EPwm-2 pins using GPIO regs*/

   GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A
   GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B

   EDIS;
}

void InitEPwm3Gpio(void)
{
   EALLOW;

   GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up on GPIO4 (EPWM3A)
   GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;    // Disable pull-up on GPIO5 (EPWM3B)

	/* Configure EPwm-3 pins using GPIO regs*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
   GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B

   EDIS;
}

void InitEPwm4Gpio(void)
{
   EALLOW;

   GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Disable pull-up on GPIO6 (EPWM4A)
   GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;    // Disable pull-up on GPIO7 (EPWM4B)

	/* Configure EPWM-4 pins using GPIO regs*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO6 as EPWM4A
   GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO7 as EPWM4B

   EDIS;
}
           
          
void InitEPwm5Gpio(void)
{
   EALLOW;

   GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;    // Disable pull-up on GPIO6 (EPWM4A)
   GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;    // Disable pull-up on GPIO7 (EPWM4B)

	/* Configure EPWM-5 pins using GPIO regs*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO6 as EPWM4A
   GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO7 as EPWM4B

   EDIS;
}
 
void InitEPwm6Gpio(void)
{
   EALLOW;

   GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;    // Disable pull-up on GPIO6 (EPWM4A)
   GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;    // Disable pull-up on GPIO7 (EPWM4B)

	/* Configure EPWM-5 pins using GPIO regs*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // Configure GPIO6 as EPWM4A
   GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;   // Configure GPIO7 as EPWM4B

   EDIS;
}
