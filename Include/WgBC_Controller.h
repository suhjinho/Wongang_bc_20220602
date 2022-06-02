/*------------------------------------------------------------------------------
Prototypes for the functions in PIDREG3.C
------------------------------------------------------------------------------*/
//--- Controller.c-----
void LPF1(float unit_time,float pole,float input,float *output);
void LPF2(float unit_time, float wc, float input,float *output);
void HPF2(float unit_time, float wc, float input, float *output);
void PI_Damp_Controller(float limit,float discrete_time,float damp_factor,float Kp,float Ki,float reference,float feedback,float *integral,float *output);
void PI_Controller(float limit,float discrete_time,float Kp,float Ki,float error,float *integral,float *output);
void RmsCalculationVoltage(float Freq,float *Sum,float *PeriodTime,float Input, float *Output);
void PI_AntiWindup(float limit,float discrete_time,float Kp,float Ki,float error,float *integral,float *output);
void PI_AntiWindup1(float limit,float discrete_time,float Kp,float Ki,float Kt, float error,float *integral,float *output);



