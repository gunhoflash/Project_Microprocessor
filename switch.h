/*

	This header file defines methods to handle the switch inturrupt for options.

	Required PINs             : nothing
	Required external headers : delay.h, fnd.h, buzzer.h

*/

// define modes
#define SWITCH_N_MODE                              4
#define SWITCH_MODE_NONE                           0
#define SWITCH_MODE_OPTIMIZER                      1
#define SWITCH_MODE_LEARNING_RATE                  2
#define SWITCH_MODE_RUN                            3

// define optimizer values
#define SWITCH_N_VALUE_OPTIMIZER                   2
#define SWITCH_VALUE_OPTIMIZER_GD                  0
#define SWITCH_VALUE_OPTIMIZER_MO                  1

// define learning rate values
#define SWITCH_N_VALUE_LEARNING_RATE               3
#define SWITCH_VALUE_LEARNING_RATE_S               0
#define SWITCH_VALUE_LEARNING_RATE_M               1
#define SWITCH_VALUE_LEARNING_RATE_L               2

// define run values
#define SWITCH_N_VALUE_RUN                         2
#define SWITCH_VALUE_RUN_OFF                       0
#define SWITCH_VALUE_RUN_ON                        1

// selected mode and values
volatile int switch_selected_mode                = 0;
volatile int switch_selected_value_optimizer     = 0;
volatile int switch_selected_value_learning_rate = 0;
volatile int switch_selected_value_run           = 0;

// flag and random-number for interrupt
volatile int switch4_flag                        = 0;
volatile int switch5_flag                        = 0;
volatile int switch4_handling_target             = 0;
volatile int switch5_handling_target             = 0;

void select_mode();
void change_value();

// interrupt for selecting mode
SIGNAL(SIG_INTERRUPT4)
{
	// disallow interrupt
	cli();

	int r = rand();
	switch4_handling_target = r;
	delay_ms(200);
	if (r == switch4_handling_target)
	{
		if (switch4_flag == 0)
		{
			switch4_flag = 1;
			EICRB = 0x0f;
			select_mode();
		}
		else
		{
			switch4_flag = 0;
			EICRB = 0x00;
		}
	}

	// allow interrupt
	sei();
}

// interrupt for changing value
SIGNAL(SIG_INTERRUPT5)
{
	// disallow interrupt
	cli();

	int r = rand();
	switch5_handling_target = r;
	delay_ms(200);
	if (r == switch5_handling_target)
	{
		if (switch5_flag == 0)
		{
			switch5_flag = 1;
			EICRB = 0x0f;
			change_value();
		}
		else
		{
			switch5_flag = 0;
			EICRB = 0x00;
		}
	}

	// allow interrupt
	sei();
}

/*
	Description:
		Select mode.
	Parameters:
		nothing
	Return:
		nothing
*/
void select_mode()
{
	buzzer(BUZZER_1);
	switch_selected_mode = (switch_selected_mode + 1) % SWITCH_N_MODE;
	set_fnd1(0, switch_selected_mode);
}

/*
	Description:
		Change value.
	Parameters:
		nothing
	Return:
		nothing
*/
void change_value()
{
	buzzer(BUZZER_2);
	switch (switch_selected_mode)
	{
		case SWITCH_MODE_NONE:
			// do nothning
			break;

		case SWITCH_MODE_OPTIMIZER:
			switch_selected_value_optimizer = (switch_selected_value_optimizer + 1) % SWITCH_N_VALUE_OPTIMIZER;
			set_fnd1(3, switch_selected_value_optimizer);
			delay_ms(200);
			set_fnd1(0, switch_selected_mode);
			break;

		case SWITCH_MODE_LEARNING_RATE:
			switch_selected_value_learning_rate = (switch_selected_value_learning_rate + 1) % SWITCH_N_VALUE_LEARNING_RATE;
			set_fnd1(3, switch_selected_value_learning_rate);
			delay_ms(200);
			set_fnd1(0, switch_selected_mode);
			break;

		case SWITCH_MODE_RUN:
			switch_selected_value_run = (switch_selected_value_run + 1) % SWITCH_N_VALUE_RUN;
			set_fnd1(3, switch_selected_value_run);
			delay_ms(200);
			set_fnd1(0, switch_selected_mode);
			break;
	}
}

/*
	Description:
		Initialize inturrupt setting.
	Parameters:
		nothing
	Return:
		nothing
*/
void switch_init()
{
	// set interrupt options
	EICRA = 0x00;
	EICRB = 0x0f;
	EIMSK = 0x30;

	// allow interrupt
	sei();
}