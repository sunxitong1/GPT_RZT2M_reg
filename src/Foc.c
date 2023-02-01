#include "Foc.h"
#include "bsp_api.h"

t_motor motor_1;

short vq_set = 0;
short vd_set = 0;

void foc_int(void)
{
//    motor_1.mapPhaseU = (unsigned short *)&(R_MTU3->TGRD);
//    motor_1.mapPhaseV = (unsigned short *)&(R_MTU4->TGRC);
//    motor_1.mapPhaseW = (unsigned short *)&(R_MTU4->TGRD);
//    motor_1.regPWMPeriod = (unsigned short *)&(R_MTU3->TGRA);
//	
//	motor_1.angle_cos = 0;
//	motor_1.angle_sin = 0;
//	motor_1.pwm_period = R_MTU3->TGRA;
//	motor_1.pwm_period_half = motor_1.pwm_period >> 1;

}



void commutate_foc(t_motor *pm) 
{
    
    pm->foc_vq = vq_set;
    pm->foc_vd = vd_set;
    
    /* Inverse Park Transformation */
//    pm->foc_alpha = pm->foc_vd * pm->angle_cos - pm->foc_vq * pm->angle_sin;
//    pm->foc_beta  = pm->foc_vq * pm->angle_cos + pm->foc_vd * pm->angle_sin;

	pm->foc_alpha = 6553;
	pm->foc_beta = 0;
	commutate_svm(pm, (float)pm->foc_alpha * (float)(1/32767.0), (float)pm->foc_beta * (float)(1/32767.0));
    update_pwm(pm);
}


void commutate_svm(t_motor *pm, float alpha, float beta) 
{
    float _x, _y, _z;
    float _u, _v, _w;
    
    _x = beta;
    _y = (beta + alpha * (float)MTR_SQRT_3) * (float)0.5;
    _z = (beta - alpha * (float)MTR_SQRT_3) * (float)0.5;

    /* Determine the sector */
    if (_y < 0.0)
    {
        if (_z < 0.0)
        {
            // Sector V
            _u = (1 + _y - _z) * (float)0.5;
            _v = _u + _z;
            _w = _u - _y;
        }
        else if (_x <= 0.0)
        {
            // Sector IV
            _u = (1 + _x - _z) * (float)0.5;
            _v = _u + _z;
            _w = _v - _x;
        }
        else
        {
            // Sector III
            _u = (1 - _x + _y) * (float)0.5;
            _w = _u - _y;
            _v = _w + _x;
        }
    }
    else
    {
        if (_z >= 0.0)
        {
            // Sector II
            _u = (1 + _y - _z) * (float)0.5;
            _v = _u + _z;
            _w = _u - _y;
        }
        else if (_x <= 0.0)
        {
            // Sector VI
            _u = (1 - _x + _y) * (float)0.5;
            _w = _u - _y;
            _v = _w + _x;
        }
        else
        {
            // Sector I
            _u = (1 + _x - _z) * (float)0.5;
            _v = _u + _z;
            _w = _v - _x;
        }
    }
    
    /* Shift and Scale the result range from 0 .. 1 to -0.5 .. 0.5*/
    pm->out_u = (_u - (float)0.5);
    pm->out_v = (_v - (float)0.5);
    pm->out_w = (_w - (float)0.5);
}

void update_pwm(t_motor *pm)
{
    float _u, _v, _w;
    
    if (pm->simulated_mode != 0)
      return;
    pm->out_u = (float)(0.2);
	pm->out_v = (float)(0.2);
	pm->out_w = (float)(0.2);
//    if (pm->charge_state == BS_CHARGE_END)
    {
        /* Scale PWM values from +/-0.5 to +/-32767 (for reporting only) */
        pm->PhaseU = (short)(pm->out_u * 65535);
        pm->PhaseV = (short)(pm->out_v * 65535);
        pm->PhaseW = (short)(pm->out_w * 65535);
        
        /* Scale PWM values from +/-0.5 to +/-pwm_period */
        _u = pm->out_u * (float)pm->pwm_period;
        _v = pm->out_v * (float)pm->pwm_period;
        _w = pm->out_w * (float)pm->pwm_period;
        
        /* Update PWM timers */
//        *pm->mapPhaseU = (unsigned short)(pm->pwm_period_half - (short)_u);
//        *pm->mapPhaseV = (unsigned short)(pm->pwm_period_half - (short)_v);
//        *pm->mapPhaseW = (unsigned short)(pm->pwm_period_half - (short)_w);
    }
//    else
//    {
//        /* Lower arm ON *//* RZ/T2 add */
//        *pm->mapPhaseU = *pm->regPWMPeriod;
//        *pm->mapPhaseV = *pm->regPWMPeriod;
//        *pm->mapPhaseW = *pm->regPWMPeriod;
//    }
}

