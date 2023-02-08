
#ifndef FOC_H
#define FOC_H


#define     M_PI    3.14159265358979323846
#define     M_PI_2  1.57079632679489661923
#define     M_PI_4  0.785398163397448309616

#define     MTR_TWOPI       (M_PI*2.0)              // 2*pi
#define     MTR_SQRT_2_3    (0.81649658f)           // sqrt(2/3)
#define     MTR_SQRT_3      (1.73205080f)
#define     MTR_1_SQRT_3    (1.0/1.73205080f)
#define     MTR_1_3         (1.0/3.0)               // 1/3
#define     MTR_2_3         (2.0/3.0) 

typedef struct motor {
    /* Configuration Settings */
    short           module_type;
    short           module_addr;
    short           group_addr;
    short           encoder_type;
    short           halls_type;
    
    short           commutation_mode;
    short           auto_stop_mode;         /* 0 = Disable, 1 = Motor Stop on error, 2 = Enable Motor Off On Error */
    unsigned char   simulated_mode;
    unsigned short  pos_capture_mode;
    short           pos_inv_mode;
    short           dflt_vgp_mode;
    unsigned char   brake_mode;
    
    short           gear_in;
    short           gear_out;
    double          gear_ratio;
    
    /* Motor configuration */
    short           motor_type;             /* 3 for BLDC, 2 for DC Brush or Voice Coil, 0 for Independent Phase Control */
    short           pole_pairs;             /* Number of pole pairs per rotor revolution */
    short           phase_config;           /* Mapping between the outputs and the motor windings */
    short           pos_scale;              /* Encoder resolution scalling to the power of 2 */
    
    /* Current Status Variables */
    short           servo_on;               /* 0 = Open position Loop, 1 = Closed position Loop */
    short           act_state;
    short           act_state_1;
    short           aligning;
    unsigned short  dinputs, dinputs_1, dinputs_2, dinchng;
    unsigned short  dinputs_err_mask;
 
    /* Velocity Profile Generator FIFO Buffer */
//	TMotionProfile  profileFIFO[VPG_BUFF_SIZE];
//	TMotionProfile  *prfl_push;  
//	TMotionProfile  *prfl_push_1;
//	TMotionProfile  *prfl_pull;  
    volatile short  prfl_points;

    /* PVT Streaming Support*/
    volatile short  pvt_points;             /* Number of PVT Points in the buffer */
//    TPosVel         pvt_pointsFIFO[PVT_BUFF_SIZE];
//    TPosVel         *pvt_push;
//    TPosVel         *pvt_end_point;
//    TPosVel         *pvt_start_point;
//    TPosVel         pvt_start;
    double          f_vel0;
    double          f_vel1;
    double          f_time;
    long            frac_vel;
    short           pvt_pos_added;          /* Flag for pairing Position and Velocity added */
    short           pvt_step;               /* Interpolation step (0 to pvt_period - 1) */
    
    unsigned short  pvt_period;             /* Number of Position Loop cycles (crnt_ds) */
    unsigned short  pvt_watermark;          /* FIFO Buffer level - sets flag if level pvt_points goes below */
    
    /* Velocity Profile Generator Parameters */
//    TMotionParams   buffMotion;
//    TMotionParams   trgtMotion;
    
    long            cmd_pos;
    long long       cmd_pos64;
    long            cmd_vel;
    long            cmd_acc;
    long            delta_pos;
    short           direction;

//    t_vpg_state     vpg_state;
    short           vpg_overrun;

    short           stop_request;           /* 0 = No Stop Req, 1 = Abrupt, 2 = Smooth Stop */

    /* Spline based velocity profile generator variables */
    long            start_pos_accel;        /* The position at which the acceleration phase begins */
    long            start_pos_decel;        /* The position at which the deceleration phase begins */
    short           decel_started;          /* Indicates if the deceleration parameters were set */
    double          _fA0, _fA1, _fA2, _fA3; /* Acceleration spline coefficients */
    double          _fD1, _fD2;
    double          _fD3, _fD4, _fD5;       /* Deceleration spline coefficients */
    double          max_vel_calc;           /* Maximum velocity calculated based on the motion parameters */
    double          max_acc_calc;           /* Maximum acceleration calculated based on the motion parameters */
    double          max_dec_calc;           /* Maximum deceleration calculated based on the motion parameters */
    double          pos_calc;
    
    long            time_accel;
    long            time_decel;
    long            time_plateau;
    long            time_current;
    long            time_total;
    long            dist_accel;
    long            dist_platoe;
    long            dist_decel;
    
    /* Encoder Position Feedback */
    volatile long   crnt_pos;
    long            crnt_pos_1;
    long            crnt_vel;
    long            crnt_acc;               /* Motor acceleration *//* RZ/T2 add */
    long long       crnt_vel64;
//    volatile TReg32 real_pos;
//    volatile TReg32 captured_pos;
    volatile long   index_pos;
    long            pos_offset;
    long long       crnt_pos64;
    short           real_rot;               /* rotate */
    long            real_res;               /* resolution */

    /* Home Procedure Control */
    short           fsm_home;       /* Finite state machine for the homing procedure */
    unsigned short  home_inv_mask;
    unsigned short  home_mask;
    long            home_idx_pos;
    short           home_switch;
    long            home_offset;
    long            user_data[8];

    /* Dithering Phasing Control */
    long            dither_last_pos;
    short           dither_last_vel;
    short           dither_cycles;
    short           dither_timer;
    long            dither_delta;
    long            dither_acc;
    long            dither_vel;

    /* I squared t protection control */
    short           I2t_current;    /* Peak current derived from the motor thermal characteristics */
    short           I2t_time;       /* Time at peak current the motor is rated at */
    long            I2t_integral;   /* Calculated runtime as integral of (total_current^2 - climit^2) */
    long            I2t_limit;	    /* Calculated as I2t_current * I2t_time */
    long            I2t_nominal;     /* Calculated runtime as tc_limit^2 */
    unsigned short  total_current;  /* Calculated runtime as abs (Iu) + abs(Iv) */

    unsigned short  tc_limit;
    unsigned short  tc_limit_time;
    unsigned short  tc_limit_timer;

    /* Maximum Position Error Tracking */
    short           pos_error;
    long            pos_error2;
    short           pos_error_limit;
    short           pos_error_time;
    short           pos_error_timer;

    /* Maximum Position Error Fault Conditions */
    long            mecmd_pos;
    long            mecmd_vel;
    short           mecmd_output;
    long            mecrnt_pos;
    unsigned short  mecrnt_output;
    
    /* Position Control Loop Gains */
    short           crnt_kp, crnt_ki, crnt_kd;
    short           crnt_kvff, crnt_kaff, crnt_bp;
    short           crnt_ds;                /* Multiples of 50us */
    short           crnt_bias;

    short           buff_kp, buff_ki, buff_kd;
    short           buff_kvff, buff_kaff, buff_bp;
    short           buff_err_limit;

    short           integral_limit16;
    long            integral_limit;

    short           pos_loop_cmd;
    long            pos_loop_limit;

    short           derivative_err;
    long            integral_err;
    long            derivative_err2;
    long long       integral_err2;
    short           pos_loop_time;      /* Desired position loop cycle time [us] */
    short           pos_loop_timer;

    /* Velocity Control Loop Gains */
    long            in_vel;
    long long       in_vel64;
    long            vel_error;
    long            integral_err_vel;
    long            derivative_err_vel;
    long long       vel_error2;
    long long       integral_err2_vel;
    long long       derivative_err2_vel;
    short           crnt_kp_vel, crnt_ki_vel, crnt_kd_vel;
    short           buff_kp_vel, buff_ki_vel, buff_kd_vel;

    long            integral_limit_vel;
    long            PiOut_limit_vel;

//    long long       array_raw_pos[POS_PAST_DATA];
//    long long       array_vel[POS_PAST_DATA];
    unsigned char   total_num_pos;
    unsigned char   total_num_vel;

    /* Output variables */
    short           output_q;           /* Position Loop Output - Current/Torque Setpoint */
    short           output_d;           
    float           out_u;              /* Phase voltages */
    float           out_v;
    float           out_w;
    
    /* Field Oriented Control Gains */
    short           foc_kp, foc_ki;
    short           foc_kd;
    short           foc_id_err_old;
    short           foc_iq_err_old;
    long            foc_iq_err_int, foc_id_err_int;
    short           foc_iq_err, foc_id_err;

    short           foc_id, foc_iq;       /* Measured D and Q currents */
    short           foc_vd, foc_vq;       /* Calculated D and Q voltages */    
    float           foc_alpha, foc_beta;
    float           calc_alpha, calc_beta;

    /* control mode *//* RZ/T2 add */
    unsigned short  ctrl_mode;      /* ctrl mode (0:position 1:velocity 2:torque) */
    unsigned short  cmd_dir;        /* command direction (0:CW 1:CCW) */
    short           cmd_vel_rpm;    /* command velocity[rpm] */
    long long       cmd_vel64;      /* command velocity[(count/pos_loop_cycle) / 2^32] */
    short           cmd_trq;        /* command torque[mNm] */
    short           est_trq;        /* estimate torque[mNm] */
    float           mtr_ke;         /* back EMF constant[(Vpeak/(rad/s)] */
    long long       in_vel64_old;   /* Previous value for 64-bit target velocity */
    short           output_d_old;   /* Previous value for target d-axis current */
    short           output_q_old;   /* Previous value for target q-axis current */

    /* Raw ADC readings / Current Feedback */
    short           adc1_raw, adc2_raw, adc3_raw;
    short           adc1_offs, adc2_offs, adc3_offs;
    short           adc_iu, adc_iv, adc_iw;
    short           *p_iu, *p_iv;
    
    /* Hall Sensors Feedback */
    unsigned short  hall_state;
    unsigned short  hall_state_1;
    long            hall_change_pos;
    short           hall_inverted;
    short           hall_shift;

    /* Phasing Parameters */
    long            phase_origin;
    short           phasing_mode;
    short           phasing_mode_crnt;
    short           phasing_power;
    short           phasing_time;
    volatile long   phasing_timer;
    short           phase_adv_gain;
    short           phase_vel_comp;
    
    /* Motor Commutation Parameters */
    short           phase_angle;             /* Flux Vector angle */
    short           phase_offset;            /* Flux Vector offset from Index pulse to Max of Phase A */
    short           phase_vector;        
    short           phase_corr;             /* Phase angle correction for factional encoder counts per electrocal cycle */
    short           phase_corr_cntr;    
	short 			PhaseU, PhaseV, PhaseW; /* Phase PWM Output */
    unsigned short  PhaseU_Reg, PhaseV_Reg, PhaseW_Reg; /* Phase PWM reg value set */

    short           pwm_period;
    short           pwm_period_half;
/* RZ/T2 2021.03.26 DEL START  */
/*    short           fault_timer; */
/* RZ/T2 2021.03.26 DEL END  */

    long            ec_per_rev;
    long            ec_per_ecycle;          /* Number of encoder count per electrical phase */
    
    double          counts2rad;             /* Converts angle from counts to radian */
    float           angle_rad;
    float           angle_sin;
    float           angle_cos;
    
    /* bootstrap_charging Parameters *//* RZ/T2 add */
    long            charge_cycle;
    unsigned short  charge_state;

    /* Hardware registers ----------------------------------------------------*/
    unsigned short  *regPosCounter;
    unsigned short  *regPosCapture;
    unsigned char   *regTimerStatus;
    
    
    unsigned short  *regPhaseU;
    unsigned short  *regPhaseV;
    unsigned short  *regPhaseW;
    unsigned short  *regPWMPeriod;
    
    unsigned short  *regADC1;
    unsigned short  *regADC2;
    
    long            flash_pars_offs;
    //t_motor_pars    pars;
    
    /* Absolute encoder parameters and status */
    long            eeprom_addr;
    long            enc_timeout;
    long            enc_baudrate;
    unsigned short  enc_status;
    short           enc_open;
    
    struct motor * slave;
    unsigned long   ErrSts;
    unsigned long   ErrMsk;
              long  Lvolt_Val;
              long  Hvolt_Val;
              long  Ovc_Val;
              long  Ovs_Val;
              long  WOvs_Val;
              long  WOvs_time;
              long  WPosMax_Val;
              long  WPosMin_Val;
              long  crnt_volt;
              long  WEmp_Val;
              long  WInvOverTemp_Val;
              long  crnt_temp;
} t_motor;

extern t_motor motor_1;

void foc_int(void);

//void r_mtu_tgiv3_interrupt(void);
void commutate_foc(t_motor *pm);
void commutate_svm(t_motor *pm, float alpha, float beta); 
void update_pwm(t_motor *pm);
void theta_calculate(t_motor * pm);



#endif



