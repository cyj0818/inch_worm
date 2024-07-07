#include "inch_toolbox/inch_misc.h"

using namespace inch;

InchMisc::InchMisc()
: nh_(""), priv_nh_("")
{
  bw_2nd_B = bw_2nd_B.transpose();
  bw_2nd_state = bw_2nd_state.transpose();
  bw_2nd_state_dot = bw_2nd_state_dot.transpose();

  init_butterworth_2nd_filter(40);
}

InchMisc::~InchMisc()
{
  ROS_INFO("Bye InchMisc!");
  ros::shutdown();
}

void InchMisc::test()
{
  ROS_INFO("HI! Here is InchMisc!");
}

double InchMisc::butterworth_2nd_filter(double input_data_, double time_loop_)
{
  // why not doing?
  // Doing Well. good!!!!
  bw_2nd_state_dot = bw_2nd_A * bw_2nd_state + bw_2nd_B * input_data_;
  bw_2nd_state = bw_2nd_state + bw_2nd_state_dot * time_loop_;

  for (int i = 0; i>2; i++)
  {
    bw_2nd_state[i] = debugger_saturation(bw_2nd_state[i]);
  }

  double output_data = bw_2nd_state[1];

  return output_data;
}

void InchMisc::init_butterworth_2nd_filter(double cut_off_freq_)
{
  bw_2nd_A << -sqrt(2) * cut_off_freq_, - cut_off_freq_ * cut_off_freq_,
                          1,                            0;
  bw_2nd_B << cut_off_freq_ * cut_off_freq_, 0;

  bw_2nd_C << 0, 1;

  bw_2nd_state << 0, 0;

  bw_2nd_state_dot << 0, 0;

  ROS_INFO("INIT butterworth_2nd_filter");
}

double InchMisc::PID_controller(double ref_, double meas_, double time_loop_)
{
  double input_error_ = ref_ - meas_;
  double error_dot = NumDiff(meas_, time_loop_);
  
  error_sum = error_sum + input_error_;

  error_sum = debugger_saturation(error_sum);

  return Kp * input_error_ + Ki * error_sum + Kd * error_dot;
}

void InchMisc::init_PID_controller(double Kp_, double Ki_, double Kd_, double cut_off_freq_)
{
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;
  error_i = 0;
  error_sum = 0;
  cut_off_freq = cut_off_freq_;

  if(cut_off_freq_ != 0) init_butterworth_2nd_filter(cut_off_freq_);
}

double InchMisc::Dead_Zone_filter(double input_data_)
{
  return (input_data_ < dead_zone_max && input_data_ > dead_zone_min) ? 0 : input_data_;
}

void InchMisc::init_Dead_Zone_filter(double dead_zone_max_, double dead_zone_min_)
{
  dead_zone_max = dead_zone_max_;
  dead_zone_min = dead_zone_min_;
}

double InchMisc::NumDiff(double input_data_, double time_loop_)
{
  double output_data;
  if (input_data_ != input_data_prev)
  output_data = butterworth_2nd_filter((input_data_ - input_data_prev) / time_loop_, time_loop_);

  input_data_prev = input_data_;

  return output_data;
}

double InchMisc::debugger_saturation(double input_data_)
{
  if (input_data_ > 100) input_data_ = 100;
  else if (input_data_ < -100) input_data_ = -100;
  else return input_data_;
}

double InchMisc::tanh_function(double input_data, double cut_off_force)
{
  double data = input_data / cut_off_force * 4;
<<<<<<< HEAD
  return abs((exp(data) - exp(-data)) / (exp(data) + exp(-data)));
}




/*****************************************************************************
** MPC
*****************************************************************************/
double InchMisc::MPC_controller(double ref_, double meas_, double time_loop_)
{
  double input_error_ = ref_ - meas_;
  double error_dot = NumDiff(meas_, time_loop_);
  
  error_sum = error_sum + input_error_;

  error_sum = debugger_saturation(error_sum);

  return Kp * input_error_ + Ki * error_sum + Kd * error_dot;
}

void InchMisc::init_MPC_controller(double w0_, double zeta_, double cut_off_freq_)
{
  w0 = w0_;
  zeta = zeta_;
  error_i = 0;
  error_sum = 0;
  cut_off_freq = cut_off_freq_;

  if(cut_off_freq_ != 0) init_butterworth_2nd_filter(cut_off_freq_);
}

=======
  return input_data * abs((exp(data) - exp(-data)) / (exp(data) + exp(-data)));
}
>>>>>>> ecf1589b5c9db856482fd14700e97459b22f802e
