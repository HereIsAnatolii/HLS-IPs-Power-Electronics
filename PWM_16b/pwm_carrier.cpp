#include <stdint.h>

void pwm_carrier_16b(bool en, short magnitude, short step, short duty, bool *pwm, bool *sync, short *carrier)
{
	/* en - Start the filter,
	 * magnitude - carrier magnitude
	 * step - carrier increment
	 * duty - duty cycle of the PWM (0 -> magnitude)
	 * pwm - pwm output (0 -> 1)
	 * sync - rising/falling states of the pwm (0 - rising, 1 - falling)
	 * carrier - pwm carrier itself
	 */
#pragma HLS INTERFACE ap_ctrl_none port=return

#pragma HLS INTERFACE ap_none port=en
#pragma HLS INTERFACE ap_none port=magnitude
#pragma HLS INTERFACE ap_none port=step
#pragma HLS INTERFACE ap_none port=duty
#pragma HLS INTERFACE ap_none port=carrier
#pragma HLS INTERFACE ap_none port=pwm
#pragma HLS INTERFACE ap_none port=sync

	static short cnt;
	static bool direction;
	static short duty_sampled;

	if(en == 0)
	{	// reset all the values
		cnt = 0;
		direction = 0;
		duty_sampled = 0;
	} else
	if(direction == 0)		// rising edge
	{
		if(cnt <= (magnitude-step))
		{
			cnt += step;
		} else
		{
			cnt = magnitude;
			direction = 1;
		}
	} else					// falling edge
	{
		if(cnt >= step)
		{
			cnt -= step;
		} else
		{
			cnt = 0;
			direction = 0;
			duty_sampled = duty;
		}
	}
	// PWM comparator
	if(duty_sampled > cnt)
	{
		*pwm = en;
	} else
	{
		*pwm = 0;
	}
	*carrier = cnt;
	*sync = direction;
}
