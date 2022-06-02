#include "Global_Variable_Function.h"

void PI_Controller(float limit,float discrete_time,float Kp,float Ki,float error,float *integral,float *output);

void LPF1(float unit_time,float pole,float input,float *output)
{
	*output+=pole*(input-*output)*unit_time;
}
void LPF2(float unit_time, float wc, float input, float *output)
{
	static float a, b;
	static float input_old=0.;
	
	a = (2.0 - wc*unit_time)/(2.0 + wc*unit_time);
	b = (wc*unit_time)/(2.0 + wc*unit_time);
	
	*output = a*(*output) + b*(input+input_old);
	input_old = input;
}
void HPF2(float unit_time, float wc, float input, float *output)
{
	float a, b;
	static float input_old=0.;
		
	a = (2.0 - wc*unit_time)/(2.0 + wc*unit_time);
	b = 2.0/(2.0 + wc*unit_time);
	
	*output = a*(*output) + b*(input-input_old);
	input_old = input;
	
	
}

void PI_Damp_Controller(float limit,float discrete_time,
						float damp_factor,float Kp,float Ki,
						float reference,float feedback,
						float *integral,float *output)
{
	*integral+=Ki*(reference-feedback)*discrete_time;
	if (*integral>fabs(limit))			*integral=fabs(limit);
	else	if (*integral<-fabs(limit))	*integral=-fabs(limit);
	*output=Kp*(damp_factor*reference-feedback)+*integral;
}
void PI_Controller(float limit,float discrete_time,float Kp,float Ki,float error,float *integral,float *output)
{
	*integral+=Ki*(error*discrete_time);
	*output = Kp*error + *integral;
	if (*integral>fabs(limit))			*integral=fabs(limit);
	else	if (*integral<-fabs(limit))	*integral=-fabs(limit);
}


void PI_AntiWindup(float limit,float discrete_time,float Kp,float Ki,float error,float *integral,float *output)
{
	float PI_AntiWindupVar = 1.0;
	
	*integral += Ki*(error-((0)*(PI_AntiWindupVar-*output)))*discrete_time;
	/*----*integral += Ki*(error-((1/Kp)*(*output-PI_AntiWindupVar))*discrete_time);	*/

	PI_AntiWindupVar = Kp*error + *integral;
	
	if(PI_AntiWindupVar > fabs(limit))        *output = fabs(limit);
	else if(PI_AntiWindupVar < -fabs(limit))  *output = -fabs(limit);
	else	*output = PI_AntiWindupVar;
}

void PI_AntiWindup1(float limit,float discrete_time,float Kp,float Ki,float Kt, float error,float *integral,float *output)
{
	float PI_AntiWindupVar = 1.0;
	*integral += Ki*(error-((1/Kt)*(PI_AntiWindupVar-*output))*discrete_time);
	
	PI_AntiWindupVar = Kp*error + *integral;
	
	if(PI_AntiWindupVar > fabs(limit))        *output = fabs(limit);
	else if(PI_AntiWindupVar < -fabs(limit))  *output = -fabs(limit);
	else	*output = PI_AntiWindupVar;
}
void RmsCalculationVoltage(float Freq,float *Sum,float *PeriodTime,float Input, float *Output)
{
	float Tmp = (0);

		
	Tmp = 1/Freq;
	
	//*PeriodTime += SamplingTime;
	if(*PeriodTime > Tmp)
	{
		*Output = sqrt(*Sum * Freq);
		*PeriodTime = 0;
		*Sum = 0;
	}	
	
	*Sum += ((Input*Input)*Ts);
		
	
	
}


