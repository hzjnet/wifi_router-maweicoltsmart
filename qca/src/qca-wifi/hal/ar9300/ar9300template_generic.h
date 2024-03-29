/*
 * Copyright (c) 2011, 2017 Qualcomm Innovation Center, Inc.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Innovation Center, Inc.
 *
 * Copyright (c) 2008-2010, Atheros Communications Inc. 
 * All Rights Reserved.
 *
 * 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * 
 */

/*
 * READ THIS NOTICE!
 *
 * Values defined in this file may only be changed under exceptional circumstances.
 *
 * Please ask Fiona Cain before making any changes.
 */

#ifndef __ar9300templateGeneric_h__
#define __ar9300templateGeneric_h__
#ifndef ART_BUILD
static ar9300_eeprom_t ar9300_template_jet =
#else
static ar5500_eeprom_t ar5500_template =
#endif
{

	2, //  eeprom_version;

    ar9300_eeprom_template_jet, //  New template_version for Jet.

    {0,2,3,4,5,6}, //mac_addr[6];

    //static  A_UINT8   custData[OSPREY_CUSTOMER_DATA_SIZE]=

	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

    //static OSPREY_BASE_EEP_HEADER base_eep_header=

	{
		    {0,0x1f},	//   reg_dmn[2]; //Does this need to be outside of this structure, if it gets written after calibration
            0xff,	//   txrx_mask;  //4 bits tx and 4 bits rx  //RayH: txrx_mask=0xff for jet
		    {AR9300_OPFLAGS_11G, 0},	//   op_cap_flags;
		    0,		//   rf_silent;
		    0,		//   blue_tooth_options;
		    0,		//   device_cap;
		    5,		//   device_type; // takes lower byte in eeprom location
		    OSPREY_PWR_TABLE_OFFSET,	//    pwrTableOffset; // offset in dB to be added to beginning of pdadc table in calibration
			{0x40,0x40},	//   params_for_tuning_caps[2];  //placeholder, get more details from Don
            0x0c,     //feature_enable; //bit0 - enable tx temp comp
                             //bit1 - enable tx volt comp
                             //bit2 - enable fastClock - default to 1
                             //bit3 - enable doubling - default to 1
                             //bit4 - enable internal regulator - default to 0
            0,       //misc_configuration: bit0 - turn down drivestrength
			3,		// eeprom_write_enable_gpio
			0,		// wlan_disable_gpio
            8,		// wlan_led_gpio
            0xff,		// rx_band_select_gpio
            0x00,			// txrxgain
            0x633c8186,		//   swreg
    },


	//static JET_MODAL_EEP_HEADER modal_header_2g=
	{

		    0x200220,	            //  Tx ant_ctrl_common;                  // 4   idle, t1, t2, b (4 bits per setting)
		    0x11111,		        //  Rx ant_ctrl_common2;                 // 4    ra1l1, ra2l1, ra1l2, ra2l2, ra12
		    {0x10,0x10,0x10,0x10},	//  ant_ctrl_chain[JET_MAX_CHAINS];      // 6   idle, t, r, rx1, rx12, b (2 bits each)
		    {0x20,0x20,0x20,0x20},	//   xatten1_db[JET_MAX_CHAINS];         // 3  //xatten1_db for merlin (0xa20c/b20c 5:0)
		    {0x8,0x8,0x8,0x8},		//   xatten1_margin[JET_MAX_CHAINS];     // 3  //xatten1_margin for merlin (0xa20c/b20c 16:12
			0x13,		   			//   temp_slope;
			0,				//    voltSlope;
		    {0,0,0,0,0}, // spur_chans[OSPREY_EEPROM_MODAL_SPURS];  // spur channels in usual fbin coding format
		    {-1,0,0,0},			//    noise_floor_thresh_ch[JET_MAX_CHAINS]; //     //Check if the register is per chain
            {1,1,1,1},       // obdboffst_aux1                       // 3    //obdboffst2G_aux1 for jet (0x16044/0x16444/0x16844, 5:3)
            {1,1,1,1},       // obdboffst_aux2                       // 3    //obdboffst2G_aux2 for jet (0x16044/0x16444/0x16844, 2:0)
            {0x61,0x61,0x61,0x61},      // tempslopeExt
            {0},            // reserved
			0,											// quick drop
		    0,				//   xpa_bias_lvl;                            // 1
		    0x0e,			//   tx_frame_to_data_start;                    // 1
		    0x0e,			//   tx_frame_to_pa_on;                         // 1
		    3,				//   txClip;                                     // 4 bits tx_clip, 4 bits dac_scale_cck
		    0,				//    antenna_gain;                           // 1
		    0x2c,			//   switchSettling;                        // 1
		    -30,			//    adcDesiredSize;                        // 1
		    0,				//   txEndToXpaOff;                         // 1
		    0x2,			//   txEndToRxOn;                           // 1
		    0xe,			//   tx_frame_to_xpa_on;                        // 1
		    28,				//   thresh62;                              // 1
			0x0c80C080,		//	 paprd_rate_mask_ht20					// 4
			0xF0,			//   paprd_rate_mask_4ss_ht20				// 4
            0x0080C080,		//	 paprd_rate_mask_ht40					// 4
			0xF0,			//	 paprd_rate_mask_4ss_ht40				// 4
		    0,				//   switchcomspdt;                         // 2
			0,				// bit: 0,1:chain0, 2,3:chain1, 4,5:chain2
			0,				//  rf_gain_cap
			0,				//  tx_gain_cap
			{0},            //futureModal[1];
            0x0,            // xtal_ppm_target, PPM target
            3,              // xtal_ppm_tolerance, PPM tolerance, default +/- 3 PPM
            0x13,           // temp_slope_high_3
            0x0             // eep_iqmask
	},

	{
			0,					    // ant_div_control
			{0},					// base_ext1
            34,                     // paprd_training_power
			0,						// misc_enable
			{0,0,0,0,0,0,0,0},		// temp slop extension
		0,									// quick drop low
		0,									// quick drop high
	},

	//static A_UINT8 cal_freq_pier_2g[OSPREY_NUM_2G_CAL_PIERS]=
	{
		FREQ2FBIN(2412, 1),
		FREQ2FBIN(2437, 1),
		FREQ2FBIN(2472, 1)
	},

	//static OSP_CAL_DATA_PER_FREQ_OP_LOOP cal_pier_data_2g[JET_MAX_CHAINS][OSPREY_NUM_2G_CAL_PIERS]=

	{	{{0,0,0,0,0,0},  {0,0,0,0,0,0},  {0,0,0,0,0,0}},
		{{0,0,0,0,0,0},  {0,0,0,0,0,0},  {0,0,0,0,0,0}},
		{{0,0,0,0,0,0},  {0,0,0,0,0,0},  {0,0,0,0,0,0}},
		{{0,0,0,0,0,0},  {0,0,0,0,0,0},  {0,0,0,0,0,0}},
	},

	//A_UINT8 cal_target_freqbin_cck[OSPREY_NUM_2G_CCK_TARGET_POWERS];

	{
		FREQ2FBIN(2412, 1),
		FREQ2FBIN(2484, 1)
	},

	//A_UINT8 cal_target_freqbin_2g[OSPREY_NUM_2G_20_TARGET_POWERS]
	{
		FREQ2FBIN(2412, 1),
		FREQ2FBIN(2437, 1),
		FREQ2FBIN(2472, 1)
	},

	//A_UINT8 cal_target_freqbin_2g_ht20[OSPREY_NUM_2G_20_TARGET_POWERS]
	{
		FREQ2FBIN(2412, 1),
		FREQ2FBIN(2437, 1),
		FREQ2FBIN(2472, 1)
	},

	//A_UINT8 cal_target_freqbin_2g_ht40[OSPREY_NUM_2G_40_TARGET_POWERS]
	{
		FREQ2FBIN(2412, 1),
		FREQ2FBIN(2437, 1),
		FREQ2FBIN(2472, 1)
	},

	//static CAL_TARGET_POWER_LEG cal_target_power_cck[OSPREY_NUM_2G_CCK_TARGET_POWERS]=
	{
		//1L-5L,5S,11L,11S
        {{36,36,36,36}},
        {{36,36,36,36}}
	 },

	//static CAL_TARGET_POWER_LEG cal_target_power_2g[OSPREY_NUM_2G_20_TARGET_POWERS]=
	{
        //6-24,36,48,54
		{{32,32,28,24}},
		{{32,32,28,24}},
		{{32,32,28,24}},
	},

	//static   OSP_CAL_TARGET_POWER_HT  cal_target_power_2g_ht20[OSPREY_NUM_2G_20_TARGET_POWERS]=
	{
        //0_8_16_24, 1-3_9-11_17-19_25-27,
        //   MCS 4, 5, 6, 7, 12,13,14,15, 20,21,22,23, 28,29,30,31
		{{32,32,32,32,28,20, 32,32,28,20, 32,32,28,20, 32,32,28,20}},
		{{32,32,32,32,28,20, 32,32,28,20, 32,32,28,20, 32,32,28,20}},
		{{32,32,32,32,28,20, 32,32,28,20, 32,32,28,20, 32,32,28,20}},
	},

	//static    OSP_CAL_TARGET_POWER_HT  cal_target_power_2g_ht40[OSPREY_NUM_2G_40_TARGET_POWERS]=
	{
        //0_8_16_24, 1-3_9-11_17-19_25-27,
        //   MCS 4, 5, 6, 7, 12,13,14,15, 20,21,22,23, 28,29,30,31
		{{32,32,32,32,28,20, 32,32,28,20, 32,32,28,20, 32,32,28,20}},
		{{32,32,32,32,28,20, 32,32,28,20, 32,32,28,20, 32,32,28,20}},
		{{32,32,32,32,28,20, 32,32,28,20, 32,32,28,20, 32,32,28,20}},
	},

    //static    A_UINT8            ctl_index_2g[OSPREY_NUM_CTLS_2G]=

    {

        0x11,
        0x12,
        0x15,
        0x17,
        0x41,
        0x42,
        0x45,
        0x47,
        0x31,
        0x32,
        0x35,
        0x37
    },

//A_UINT8   ctl_freqbin_2G[OSPREY_NUM_CTLS_2G][OSPREY_NUM_BAND_EDGES_2G];

	{
		{FREQ2FBIN(2412, 1),
		 FREQ2FBIN(2417, 1),
		 FREQ2FBIN(2457, 1),
		 FREQ2FBIN(2462, 1)},

		{FREQ2FBIN(2412, 1),
		 FREQ2FBIN(2417, 1),
		 FREQ2FBIN(2462, 1),
		 0xFF},

		{FREQ2FBIN(2412, 1),
		 FREQ2FBIN(2417, 1),
		 FREQ2FBIN(2462, 1),
		 0xFF},

		{FREQ2FBIN(2422, 1),
		 FREQ2FBIN(2427, 1),
		 FREQ2FBIN(2447, 1),
		 FREQ2FBIN(2452, 1)},

		{/*Data[4].ctl_edges[0].bChannel*/FREQ2FBIN(2412, 1),
		/*Data[4].ctl_edges[1].bChannel*/FREQ2FBIN(2417, 1),
		/*Data[4].ctl_edges[2].bChannel*/FREQ2FBIN(2472, 1),
		/*Data[4].ctl_edges[3].bChannel*/FREQ2FBIN(2484, 1)},

		{/*Data[5].ctl_edges[0].bChannel*/FREQ2FBIN(2412, 1),
		 /*Data[5].ctl_edges[1].bChannel*/FREQ2FBIN(2417, 1),
		 /*Data[5].ctl_edges[2].bChannel*/FREQ2FBIN(2472, 1),
		 0},

		{/*Data[6].ctl_edges[0].bChannel*/FREQ2FBIN(2412, 1),
		 /*Data[6].ctl_edges[1].bChannel*/FREQ2FBIN(2417, 1),
		 FREQ2FBIN(2472, 1),
		 0},

		{/*Data[7].ctl_edges[0].bChannel*/FREQ2FBIN(2422, 1),
		 /*Data[7].ctl_edges[1].bChannel*/FREQ2FBIN(2427, 1),
		 /*Data[7].ctl_edges[2].bChannel*/FREQ2FBIN(2447, 1),
		 /*Data[7].ctl_edges[3].bChannel*/FREQ2FBIN(2462, 1)},

		{/*Data[8].ctl_edges[0].bChannel*/FREQ2FBIN(2412, 1),
		 /*Data[8].ctl_edges[1].bChannel*/FREQ2FBIN(2417, 1),
		 /*Data[8].ctl_edges[2].bChannel*/FREQ2FBIN(2472, 1),
		 0},

		{/*Data[9].ctl_edges[0].bChannel*/FREQ2FBIN(2412, 1),
		 /*Data[9].ctl_edges[1].bChannel*/FREQ2FBIN(2417, 1),
		 /*Data[9].ctl_edges[2].bChannel*/FREQ2FBIN(2472, 1),
		 0},

		{/*Data[10].ctl_edges[0].bChannel*/FREQ2FBIN(2412, 1),
		 /*Data[10].ctl_edges[1].bChannel*/FREQ2FBIN(2417, 1),
		 /*Data[10].ctl_edges[2].bChannel*/FREQ2FBIN(2472, 1),
		 0},

		{/*Data[11].ctl_edges[0].bChannel*/FREQ2FBIN(2422, 1),
		 /*Data[11].ctl_edges[1].bChannel*/FREQ2FBIN(2427, 1),
		 /*Data[11].ctl_edges[2].bChannel*/FREQ2FBIN(2447, 1),
		 /*Data[11].ctl_edges[3].bChannel*/FREQ2FBIN(2462, 1)}
	},


//OSP_CAL_CTL_DATA_2G   ctl_power_data_2g[OSPREY_NUM_CTLS_2G];

#if AH_BYTE_ORDER == AH_BIG_ENDIAN
    {

	    {{{0, 60}, {1, 60}, {0, 60}, {0, 60}}},
	    {{{0, 60}, {1, 60}, {0, 60}, {0, 60}}},
	    {{{1, 60}, {0, 60}, {0, 60}, {1, 60}}},

	    {{{1, 60}, {0, 60}, {0, 60}, {0, 60}}},
	    {{{0, 60}, {1, 60}, {0, 60}, {0, 60}}},
	    {{{0, 60}, {1, 60}, {0, 60}, {0, 60}}},

	    {{{0, 60}, {1, 60}, {1, 60}, {0, 60}}},
	    {{{0, 60}, {1, 60}, {0, 60}, {0, 60}}},
	    {{{0, 60}, {1, 60}, {0, 60}, {0, 60}}},

	    {{{0, 60}, {1, 60}, {0, 60}, {0, 60}}},
	    {{{0, 60}, {1, 60}, {1, 60}, {1, 60}}},
	    {{{0, 60}, {1, 60}, {1, 60}, {1, 60}}},

    },
#else
	{
	    {{{60, 0}, {60, 1}, {60, 0}, {60, 0}}},
	    {{{60, 0}, {60, 1}, {60, 0}, {60, 0}}},
	    {{{60, 1}, {60, 0}, {60, 0}, {60, 1}}},

	    {{{60, 1}, {60, 0}, {60, 0}, {60, 0}}},
	    {{{60, 0}, {60, 1}, {60, 0}, {60, 0}}},
	    {{{60, 0}, {60, 1}, {60, 0}, {60, 0}}},

	    {{{60, 0}, {60, 1}, {60, 1}, {60, 0}}},
	    {{{60, 0}, {60, 1}, {60, 0}, {60, 0}}},
	    {{{60, 0}, {60, 1}, {60, 0}, {60, 0}}},

	    {{{60, 0}, {60, 1}, {60, 0}, {60, 0}}},
	    {{{60, 0}, {60, 1}, {60, 1}, {60, 1}}},
	    {{{60, 0}, {60, 1}, {60, 1}, {60, 1}}},
	},
#endif

//static    JET_MODAL_EEP_HEADER   modal_header_5g=

	{

		    0x110,			//  ant_ctrl_common;                         // 4   idle, t1, t2, b (4 bits per setting)
		    0x22222,		//  ant_ctrl_common2;                        // 4    ra1l1, ra2l1, ra1l2, ra2l2, ra12
		    {0x000,0x000,0x000,0x000},	//  ant_ctrl_chain[JET_MAX_CHAINS];       // 6   idle, t, r, rx1, rx12, b (2 bits each)
		    {0,0,0,0},			//   xatten1_db[JET_MAX_CHAINS];           // 3  //xatten1_db for merlin (0xa20c/b20c 5:0)
		    {0,0,0,0},			//   xatten1_margin[JET_MAX_CHAINS];          // 3  //xatten1_margin for merlin (0xa20c/b20c 16:12
			68,				//    temp_slope;
			0,				//    voltSlope;
		    {0,0,0,0,0}, // spur_chans[OSPREY_EEPROM_MODAL_SPURS];  // spur channels in usual fbin coding format
		    {-1,0,0,0},			//    noise_floor_thresh_ch[JET_MAX_CHAINS]; // 3    //Check if the register is per chain
            {0,0,0,0},       // obdboffst_aux1                       // 3    //obdboffst5G_aux1
            {0,0,0,0},       // obdboffst_aux2                       // 3    //obdboffst5G_aux2
            {0,0,0,0},      // tempslopeExt
            {0},            // reserved
			0,											// quick drop
		    0,				//   xpa_bias_lvl;                            // 1
		    0x0e,			//   tx_frame_to_data_start;                    // 1
		    0x0e,			//   tx_frame_to_pa_on;                         // 1
		    3,				//   txClip;                                     // 4 bits tx_clip, 4 bits dac_scale_cck
		    0,				//    antenna_gain;                           // 1
		    0x2d,			//   switchSettling;                        // 1
		    -30,			//    adcDesiredSize;                        // 1
		    0,				//   txEndToXpaOff;                         // 1
		    0x2,			//   txEndToRxOn;                           // 1
		    0xe,			//   tx_frame_to_xpa_on;                    // 1
		    28,				//   thresh62;                              // 1
            0x0cf0e0e0,		//	 paprd_rate_mask_ht20					// 4
            0x00,           //   	 paprd_rate_mask_4ss_ht20			// 4
            0x6cf0e0e0,		//	 paprd_rate_mask_ht40					// 4
            0x00,           //       paprd_rate_mask_4ss_ht40           // 4
		    0,				//   switchcomspdt;                         // 1
			0,				// bit: 0,1:chain0, 2,3:chain1, 4,5:chain2
			0,				//  rf_gain_cap
			0,				//  tx_gain_cap
			{0},            //futureModal[1];
            0x0,            // xtal_ppm_target, PPM target
            3,              // xtal_ppm_tolerance, PPM tolerance, default +/- 3 PPM
	},

	{			// base_ext2
		0x13,	// temp_slope_low
		0x13,	// temp_slope_high
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0}
	},

//static    A_UINT8            cal_freq_pier_5g[OSPREY_NUM_5G_CAL_PIERS]=
	{
		    //pPiers[0] =
		    FREQ2FBIN(5180, 0),
		    //pPiers[1] =
		    FREQ2FBIN(5220, 0),
		    //pPiers[2] =
		    FREQ2FBIN(5320, 0),
		    //pPiers[3] =
		    FREQ2FBIN(5400, 0),
		    //pPiers[4] =
		    FREQ2FBIN(5500, 0),
		    //pPiers[5] =
		    FREQ2FBIN(5600, 0),
		    //pPiers[6] =
		    FREQ2FBIN(5725, 0),
            //pPiers[7] =
            FREQ2FBIN(5825, 0)
	},

//static    OSP_CAL_DATA_PER_FREQ_OP_LOOP cal_pier_data_5g[JET_MAX_CHAINS][OSPREY_NUM_5G_CAL_PIERS]=

	{
		{{0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},    {0,0,0,0,0},  {0,0,0,0,0}},
		{{0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},    {0,0,0,0,0},  {0,0,0,0,0}},
		{{0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},    {0,0,0,0,0},  {0,0,0,0,0}},
		{{0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},    {0,0,0,0,0},  {0,0,0,0,0}},

	},

//static    CAL_TARGET_POWER_LEG cal_target_freqbin_5g[OSPREY_NUM_5G_20_TARGET_POWERS]=

	{
			FREQ2FBIN(5180, 0),
			FREQ2FBIN(5220, 0),
			FREQ2FBIN(5320, 0),
			FREQ2FBIN(5400, 0),
			FREQ2FBIN(5500, 0),
			FREQ2FBIN(5600, 0),
			FREQ2FBIN(5725, 0),
			FREQ2FBIN(5825, 0)
	},

//static    OSP_CAL_TARGET_POWER_HT  cal_target_power_5g_ht20[OSPREY_NUM_5G_20_TARGET_POWERS]=

	{
			FREQ2FBIN(5180, 0),
			FREQ2FBIN(5240, 0),
			FREQ2FBIN(5320, 0),
			FREQ2FBIN(5500, 0),
			FREQ2FBIN(5700, 0),
			FREQ2FBIN(5745, 0),
			FREQ2FBIN(5725, 0),
			FREQ2FBIN(5825, 0)
	},

//static    OSP_CAL_TARGET_POWER_HT  cal_target_power_5g_ht40[OSPREY_NUM_5G_40_TARGET_POWERS]=

	{
			FREQ2FBIN(5180, 0),
			FREQ2FBIN(5240, 0),
			FREQ2FBIN(5320, 0),
			FREQ2FBIN(5500, 0),
			FREQ2FBIN(5700, 0),
			FREQ2FBIN(5745, 0),
			FREQ2FBIN(5725, 0),
			FREQ2FBIN(5825, 0)
	},


//static    CAL_TARGET_POWER_LEG cal_target_power_5g[OSPREY_NUM_5G_20_TARGET_POWERS]=


	{
        //6-24,36,48,54
	    {{20,20,20,10}},
	    {{20,20,20,10}},
	    {{20,20,20,10}},
	    {{20,20,20,10}},
	    {{20,20,20,10}},
	    {{20,20,20,10}},
	    {{20,20,20,10}},
	    {{20,20,20,10}},
	},

//static    OSP_CAL_TARGET_POWER_HT  cal_target_power_5g_ht20[OSPREY_NUM_5G_20_TARGET_POWERS]=

	{
        //0_8_16,1-3_9-11_17-19,
        //      4,5,6,7,12,13,14,15,20,21,22,23
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0,0,0,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0,0,0,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0,0,0,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0,0,0,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0,0,0,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0,0,0,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0,0,0,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0,0,0,0,0}},
	},

//static    OSP_CAL_TARGET_POWER_HT  cal_target_power_5g_ht40[OSPREY_NUM_5G_40_TARGET_POWERS]=
	{
        //0_8_16,1-3_9-11_17-19,
        //      4,5,6,7,12,13,14,15,20,21,22,23
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0,0,0,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0,0,0,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0,0,0,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0,0,0,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0,0,0,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0,0,0,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0,0,0,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0,0,0,0,0}},
	},

//static    A_UINT8            ctl_index_5g[OSPREY_NUM_CTLS_5G]=

	{
		    //pCtlIndex[0] =
		    0x10,
		    //pCtlIndex[1] =
		    0x16,
		    //pCtlIndex[2] =
		    0x18,
		    //pCtlIndex[3] =
		    0x40,
		    //pCtlIndex[4] =
		    0x46,
		    //pCtlIndex[5] =
		    0x48,
		    //pCtlIndex[6] =
		    0x30,
		    //pCtlIndex[7] =
		    0x36,
            //pCtlIndex[8] =
            0x38
	},

//    A_UINT8   ctl_freqbin_5G[OSPREY_NUM_CTLS_5G][OSPREY_NUM_BAND_EDGES_5G];

	{
	    {/* Data[0].ctl_edges[0].bChannel*/FREQ2FBIN(5180, 0),
	    /* Data[0].ctl_edges[1].bChannel*/FREQ2FBIN(5260, 0),
	    /* Data[0].ctl_edges[2].bChannel*/FREQ2FBIN(5280, 0),
	    /* Data[0].ctl_edges[3].bChannel*/FREQ2FBIN(5500, 0),
	    /* Data[0].ctl_edges[4].bChannel*/FREQ2FBIN(5600, 0),
	    /* Data[0].ctl_edges[5].bChannel*/FREQ2FBIN(5700, 0),
	    /* Data[0].ctl_edges[6].bChannel*/FREQ2FBIN(5745, 0),
	    /* Data[0].ctl_edges[7].bChannel*/FREQ2FBIN(5825, 0)},

	    {/* Data[1].ctl_edges[0].bChannel*/FREQ2FBIN(5180, 0),
	    /* Data[1].ctl_edges[1].bChannel*/FREQ2FBIN(5260, 0),
	    /* Data[1].ctl_edges[2].bChannel*/FREQ2FBIN(5280, 0),
	    /* Data[1].ctl_edges[3].bChannel*/FREQ2FBIN(5500, 0),
	    /* Data[1].ctl_edges[4].bChannel*/FREQ2FBIN(5520, 0),
	    /* Data[1].ctl_edges[5].bChannel*/FREQ2FBIN(5700, 0),
	    /* Data[1].ctl_edges[6].bChannel*/FREQ2FBIN(5745, 0),
	    /* Data[1].ctl_edges[7].bChannel*/FREQ2FBIN(5825, 0)},

	    {/* Data[2].ctl_edges[0].bChannel*/FREQ2FBIN(5190, 0),
	    /* Data[2].ctl_edges[1].bChannel*/FREQ2FBIN(5230, 0),
	    /* Data[2].ctl_edges[2].bChannel*/FREQ2FBIN(5270, 0),
	    /* Data[2].ctl_edges[3].bChannel*/FREQ2FBIN(5310, 0),
	    /* Data[2].ctl_edges[4].bChannel*/FREQ2FBIN(5510, 0),
	    /* Data[2].ctl_edges[5].bChannel*/FREQ2FBIN(5550, 0),
	    /* Data[2].ctl_edges[6].bChannel*/FREQ2FBIN(5670, 0),
	    /* Data[2].ctl_edges[7].bChannel*/FREQ2FBIN(5755, 0)},

	    {/* Data[3].ctl_edges[0].bChannel*/FREQ2FBIN(5180, 0),
	    /* Data[3].ctl_edges[1].bChannel*/FREQ2FBIN(5200, 0),
	    /* Data[3].ctl_edges[2].bChannel*/FREQ2FBIN(5260, 0),
	    /* Data[3].ctl_edges[3].bChannel*/FREQ2FBIN(5320, 0),
	    /* Data[3].ctl_edges[4].bChannel*/FREQ2FBIN(5500, 0),
	    /* Data[3].ctl_edges[5].bChannel*/FREQ2FBIN(5700, 0),
	    /* Data[3].ctl_edges[6].bChannel*/0xFF,
	    /* Data[3].ctl_edges[7].bChannel*/0xFF},

	    {/* Data[4].ctl_edges[0].bChannel*/FREQ2FBIN(5180, 0),
	    /* Data[4].ctl_edges[1].bChannel*/FREQ2FBIN(5260, 0),
	    /* Data[4].ctl_edges[2].bChannel*/FREQ2FBIN(5500, 0),
	    /* Data[4].ctl_edges[3].bChannel*/FREQ2FBIN(5700, 0),
	    /* Data[4].ctl_edges[4].bChannel*/0xFF,
	    /* Data[4].ctl_edges[5].bChannel*/0xFF,
	    /* Data[4].ctl_edges[6].bChannel*/0xFF,
	    /* Data[4].ctl_edges[7].bChannel*/0xFF},

	    {/* Data[5].ctl_edges[0].bChannel*/FREQ2FBIN(5190, 0),
	    /* Data[5].ctl_edges[1].bChannel*/FREQ2FBIN(5270, 0),
	    /* Data[5].ctl_edges[2].bChannel*/FREQ2FBIN(5310, 0),
	    /* Data[5].ctl_edges[3].bChannel*/FREQ2FBIN(5510, 0),
	    /* Data[5].ctl_edges[4].bChannel*/FREQ2FBIN(5590, 0),
	    /* Data[5].ctl_edges[5].bChannel*/FREQ2FBIN(5670, 0),
	    /* Data[5].ctl_edges[6].bChannel*/0xFF,
	    /* Data[5].ctl_edges[7].bChannel*/0xFF},

	    {/* Data[6].ctl_edges[0].bChannel*/FREQ2FBIN(5180, 0),
	    /* Data[6].ctl_edges[1].bChannel*/FREQ2FBIN(5200, 0),
	    /* Data[6].ctl_edges[2].bChannel*/FREQ2FBIN(5220, 0),
	    /* Data[6].ctl_edges[3].bChannel*/FREQ2FBIN(5260, 0),
	    /* Data[6].ctl_edges[4].bChannel*/FREQ2FBIN(5500, 0),
	    /* Data[6].ctl_edges[5].bChannel*/FREQ2FBIN(5600, 0),
	    /* Data[6].ctl_edges[6].bChannel*/FREQ2FBIN(5700, 0),
	    /* Data[6].ctl_edges[7].bChannel*/FREQ2FBIN(5745, 0)},

	    {/* Data[7].ctl_edges[0].bChannel*/FREQ2FBIN(5180, 0),
	    /* Data[7].ctl_edges[1].bChannel*/FREQ2FBIN(5260, 0),
	    /* Data[7].ctl_edges[2].bChannel*/FREQ2FBIN(5320, 0),
	    /* Data[7].ctl_edges[3].bChannel*/FREQ2FBIN(5500, 0),
	    /* Data[7].ctl_edges[4].bChannel*/FREQ2FBIN(5560, 0),
	    /* Data[7].ctl_edges[5].bChannel*/FREQ2FBIN(5700, 0),
	    /* Data[7].ctl_edges[6].bChannel*/FREQ2FBIN(5745, 0),
	    /* Data[7].ctl_edges[7].bChannel*/FREQ2FBIN(5825, 0)},

	    {/* Data[8].ctl_edges[0].bChannel*/FREQ2FBIN(5190, 0),
	    /* Data[8].ctl_edges[1].bChannel*/FREQ2FBIN(5230, 0),
	    /* Data[8].ctl_edges[2].bChannel*/FREQ2FBIN(5270, 0),
	    /* Data[8].ctl_edges[3].bChannel*/FREQ2FBIN(5510, 0),
	    /* Data[8].ctl_edges[4].bChannel*/FREQ2FBIN(5550, 0),
	    /* Data[8].ctl_edges[5].bChannel*/FREQ2FBIN(5670, 0),
	    /* Data[8].ctl_edges[6].bChannel*/FREQ2FBIN(5755, 0),
	    /* Data[8].ctl_edges[7].bChannel*/FREQ2FBIN(5795, 0)}
	},

//static    OSP_CAL_CTL_DATA_5G   ctlData_5G[OSPREY_NUM_CTLS_5G]=

#if AH_BYTE_ORDER == AH_BIG_ENDIAN
	{
	    {{{1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {0, 60}}},

	    {{{1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {0, 60}}},

	    {{{0, 60},
	      {1, 60},
	      {0, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60}}},

	    {{{0, 60},
	      {1, 60},
	      {1, 60},
	      {0, 60},
	      {1, 60},
	      {0, 60},
	      {0, 60},
	      {0, 60}}},

	    {{{1, 60},
	      {1, 60},
	      {1, 60},
	      {0, 60},
	      {0, 60},
	      {0, 60},
	      {0, 60},
	      {0, 60}}},

	    {{{1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {0, 60},
	      {0, 60},
	      {0, 60}}},

	    {{{1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60}}},

	    {{{1, 60},
	      {1, 60},
	      {0, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {0, 60}}},

	    {{{1, 60},
	      {0, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {0, 60},
	      {1, 60}}},
	},
#else
	{
	    {{{60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 0}}},

	    {{{60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 0}}},

	    {{{60, 0},
	      {60, 1},
	      {60, 0},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1}}},

	    {{{60, 0},
	      {60, 1},
	      {60, 1},
	      {60, 0},
	      {60, 1},
	      {60, 0},
	      {60, 0},
	      {60, 0}}},

	    {{{60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 0},
	      {60, 0},
	      {60, 0},
	      {60, 0},
	      {60, 0}}},

	    {{{60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 0},
	      {60, 0},
	      {60, 0}}},

	    {{{60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1}}},

	    {{{60, 1},
	      {60, 1},
	      {60, 0},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 0}}},

	    {{{60, 1},
	      {60, 0},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 0},
	      {60, 1}}},
	},
#endif

    // CAL_DATA_PER_CHIP    chipCaldata
    {0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0,0,0}, 0, 0, {0,0,0,0,0,0,0,0}},

    10,     // int8_t   aux12_lboffset;

    100,    // int8_t   aux12_hboffset;

    2,      // u_int8_t nart_version;

    2,      // u_int8_t ref_file_version;

    3,      // u_int8_t board_id; JET01(QCN5502,xPA)=3, JET02(QCN5500,iPA)=4

	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

};

#ifndef ART_BUILD
static osprey_eeprom_t ar9300_template_generic=
#else
static ar9300_eeprom_t ar9300_template_generic=
#endif
{

	2, //  eeprom_version;

    ar9300_eeprom_template_generic, //  template_version;

    {0,2,3,4,5,6}, //mac_addr[6];

    //static  A_UINT8   custData[OSPREY_CUSTOMER_DATA_SIZE]=

	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

    //static OSPREY_BASE_EEP_HEADER base_eep_header=

	{
		    {0,0x1f},	//   reg_dmn[2]; //Does this need to be outside of this structure, if it gets written after calibration
		    0x77,	//   txrx_mask;  //4 bits tx and 4 bits rx
		    {AR9300_OPFLAGS_11G | AR9300_OPFLAGS_11A, 0},	//   op_cap_flags;
		    0,		//   rf_silent;
		    0,		//   blue_tooth_options;
		    0,		//   device_cap;
		    5,		//   device_type; // takes lower byte in eeprom location
		    OSPREY_PWR_TABLE_OFFSET,	//    pwrTableOffset; // offset in dB to be added to beginning of pdadc table in calibration
			{0,0},	//   params_for_tuning_caps[2];  //placeholder, get more details from Don
            0x0c,     //feature_enable; //bit0 - enable tx temp comp
                             //bit1 - enable tx volt comp
                             //bit2 - enable fastClock - default to 1
                             //bit3 - enable doubling - default to 1
                             //bit4 - enable internal regulator - default to 0
            0,       //misc_configuration: bit0 - turn down drivestrength
			3,		// eeprom_write_enable_gpio
			0,		// wlan_disable_gpio
			8,		// wlan_led_gpio
			0xff,		// rx_band_select_gpio
			0,			// txrxgain
            0,		//   swreg
	},


	//static OSPREY_MODAL_EEP_HEADER modal_header_2g=
	{

		    0x110,			//  ant_ctrl_common;                         // 4   idle, t1, t2, b (4 bits per setting)
		    0x22222,		//  ant_ctrl_common2;                        // 4    ra1l1, ra2l1, ra1l2, ra2l2, ra12
		    {0x150,0x150,0x150},	//  ant_ctrl_chain[OSPREY_MAX_CHAINS];       // 6   idle, t, r, rx1, rx12, b (2 bits each)
		    {0,0,0},			//   xatten1_db[OSPREY_MAX_CHAINS];           // 3  //xatten1_db for merlin (0xa20c/b20c 5:0)
		    {0,0,0},			//   xatten1_margin[OSPREY_MAX_CHAINS];          // 3  //xatten1_margin for merlin (0xa20c/b20c 16:12
			36,				//    temp_slope;
			0,				//    voltSlope;
		    {0,0,0,0,0}, // spur_chans[OSPREY_EEPROM_MODAL_SPURS];  // spur channels in usual fbin coding format
		    {-1,0,0},			//    noise_floor_thresh_ch[OSPREY_MAX_CHAINS]; // 3    //Check if the register is per chain
            {0,0,0},       // obdboffst_aux1                       // 3    //obdboffst2G_aux1 for dragonfly (0x16044/0x16444/0x16844, 5:3)
            {0,0,0},       // obdboffst_aux2                       // 3    //obdboffst2G_aux2 for dragonfly (0x16044/0x16444/0x16844, 2:0)
            {0,0,0,0,0},   // reserved
			0,											// quick drop
		    0,				//   xpa_bias_lvl;                            // 1
		    0x0e,			//   tx_frame_to_data_start;                    // 1
		    0x0e,			//   tx_frame_to_pa_on;                         // 1
		    3,				//   txClip;                                     // 4 bits tx_clip, 4 bits dac_scale_cck
		    0,				//    antenna_gain;                           // 1
		    0x2c,			//   switchSettling;                        // 1
		    -30,			//    adcDesiredSize;                        // 1
		    0,				//   txEndToXpaOff;                         // 1
		    0x2,			//   txEndToRxOn;                           // 1
		    0xe,			//   tx_frame_to_xpa_on;                        // 1
		    28,				//   thresh62;                              // 1
		    0x0c80C080,			//   paprd_rate_mask_ht20 		// 4
            0x0080C080,			//   paprd_rate_mask_ht40
		    0,				//   switchcomspdt;                         // 2
			0,				// bit: 0,1:chain0, 2,3:chain1, 4,5:chain2
			0,				//  rf_gain_cap
			0,				//  tx_gain_cap
			{0,0,0,0,0}    //futureModal[5];
	},

	{
			0,					    // ant_div_control
			{0,0},					// base_ext1
			0,						// misc_enable
			{0,0,0,0,0,0,0,0},		// temp slop extension
		0,									// quick drop low
		0,									// quick drop high
	},

	//static A_UINT8 cal_freq_pier_2g[OSPREY_NUM_2G_CAL_PIERS]=
	{
		FREQ2FBIN(2412, 1),
		FREQ2FBIN(2437, 1),
		FREQ2FBIN(2472, 1)
	},

	//static OSP_CAL_DATA_PER_FREQ_OP_LOOP cal_pier_data_2g[OSPREY_MAX_CHAINS][OSPREY_NUM_2G_CAL_PIERS]=

	{	{{0,0,0,0,0,0},  {0,0,0,0,0,0},  {0,0,0,0,0,0}},
		{{0,0,0,0,0,0},  {0,0,0,0,0,0},  {0,0,0,0,0,0}},
		{{0,0,0,0,0,0},  {0,0,0,0,0,0},  {0,0,0,0,0,0}},
	},

	//A_UINT8 cal_target_freqbin_cck[OSPREY_NUM_2G_CCK_TARGET_POWERS];

	{
		FREQ2FBIN(2412, 1),
		FREQ2FBIN(2484, 1)
	},

	//static CAL_TARGET_POWER_LEG cal_target_freqbin_2g[OSPREY_NUM_2G_20_TARGET_POWERS]
	{
		FREQ2FBIN(2412, 1),
		FREQ2FBIN(2437, 1),
		FREQ2FBIN(2472, 1)
	},

	//static   OSP_CAL_TARGET_POWER_HT  cal_target_freqbin_2g_ht20[OSPREY_NUM_2G_20_TARGET_POWERS]
	{
		FREQ2FBIN(2412, 1),
		FREQ2FBIN(2437, 1),
		FREQ2FBIN(2472, 1)
	},

	//static   OSP_CAL_TARGET_POWER_HT  cal_target_freqbin_2g_ht40[OSPREY_NUM_2G_40_TARGET_POWERS]
	{
		FREQ2FBIN(2412, 1),
		FREQ2FBIN(2437, 1),
		FREQ2FBIN(2472, 1)
	},

	//static CAL_TARGET_POWER_LEG cal_target_power_cck[OSPREY_NUM_2G_CCK_TARGET_POWERS]=
	{
		//1L-5L,5S,11L,11S
        {{36,36,36,36}},
	 	{{36,36,36,36}}
	 },

	//static CAL_TARGET_POWER_LEG cal_target_power_2g[OSPREY_NUM_2G_20_TARGET_POWERS]=
	{
        //6-24,36,48,54
		{{32,32,28,24}},
		{{32,32,28,24}},
		{{32,32,28,24}},
	},

	//static   OSP_CAL_TARGET_POWER_HT  cal_target_power_2g_ht20[OSPREY_NUM_2G_20_TARGET_POWERS]=
	{
        //0_8_16,1-3_9-11_17-19,
        //      4,5,6,7,12,13,14,15,20,21,22,23
		{{32,32,32,32,28,20,32,32,28,20,32,32,28,20}},
		{{32,32,32,32,28,20,32,32,28,20,32,32,28,20}},
		{{32,32,32,32,28,20,32,32,28,20,32,32,28,20}},
	},

	//static    OSP_CAL_TARGET_POWER_HT  cal_target_power_2g_ht40[OSPREY_NUM_2G_40_TARGET_POWERS]=
	{
        //0_8_16,1-3_9-11_17-19,
        //      4,5,6,7,12,13,14,15,20,21,22,23
		{{32,32,32,32,28,20,32,32,28,20,32,32,28,20}},
		{{32,32,32,32,28,20,32,32,28,20,32,32,28,20}},
		{{32,32,32,32,28,20,32,32,28,20,32,32,28,20}},
	},

//static    A_UINT8            ctl_index_2g[OSPREY_NUM_CTLS_2G]=

	{

		    0x11,
    		0x12,
    		0x15,
    		0x17,
    		0x41,
    		0x42,
   			0x45,
    		0x47,
   			0x31,
    		0x32,
    		0x35,
    		0x37

    },

//A_UINT8   ctl_freqbin_2G[OSPREY_NUM_CTLS_2G][OSPREY_NUM_BAND_EDGES_2G];

	{
		{FREQ2FBIN(2412, 1),
		 FREQ2FBIN(2417, 1),
		 FREQ2FBIN(2457, 1),
		 FREQ2FBIN(2462, 1)},

		{FREQ2FBIN(2412, 1),
		 FREQ2FBIN(2417, 1),
		 FREQ2FBIN(2462, 1),
		 0xFF},

		{FREQ2FBIN(2412, 1),
		 FREQ2FBIN(2417, 1),
		 FREQ2FBIN(2462, 1),
		 0xFF},

		{FREQ2FBIN(2422, 1),
		 FREQ2FBIN(2427, 1),
		 FREQ2FBIN(2447, 1),
		 FREQ2FBIN(2452, 1)},

		{/*Data[4].ctl_edges[0].bChannel*/FREQ2FBIN(2412, 1),
		/*Data[4].ctl_edges[1].bChannel*/FREQ2FBIN(2417, 1),
		/*Data[4].ctl_edges[2].bChannel*/FREQ2FBIN(2472, 1),
		/*Data[4].ctl_edges[3].bChannel*/FREQ2FBIN(2484, 1)},

		{/*Data[5].ctl_edges[0].bChannel*/FREQ2FBIN(2412, 1),
		 /*Data[5].ctl_edges[1].bChannel*/FREQ2FBIN(2417, 1),
		 /*Data[5].ctl_edges[2].bChannel*/FREQ2FBIN(2472, 1),
		 0},

		{/*Data[6].ctl_edges[0].bChannel*/FREQ2FBIN(2412, 1),
		 /*Data[6].ctl_edges[1].bChannel*/FREQ2FBIN(2417, 1),
		 FREQ2FBIN(2472, 1),
		 0},

		{/*Data[7].ctl_edges[0].bChannel*/FREQ2FBIN(2422, 1),
		 /*Data[7].ctl_edges[1].bChannel*/FREQ2FBIN(2427, 1),
		 /*Data[7].ctl_edges[2].bChannel*/FREQ2FBIN(2447, 1),
		 /*Data[7].ctl_edges[3].bChannel*/FREQ2FBIN(2462, 1)},

		{/*Data[8].ctl_edges[0].bChannel*/FREQ2FBIN(2412, 1),
		 /*Data[8].ctl_edges[1].bChannel*/FREQ2FBIN(2417, 1),
		 /*Data[8].ctl_edges[2].bChannel*/FREQ2FBIN(2472, 1),
		 0},

		{/*Data[9].ctl_edges[0].bChannel*/FREQ2FBIN(2412, 1),
		 /*Data[9].ctl_edges[1].bChannel*/FREQ2FBIN(2417, 1),
		 /*Data[9].ctl_edges[2].bChannel*/FREQ2FBIN(2472, 1),
		 0},

		{/*Data[10].ctl_edges[0].bChannel*/FREQ2FBIN(2412, 1),
		 /*Data[10].ctl_edges[1].bChannel*/FREQ2FBIN(2417, 1),
		 /*Data[10].ctl_edges[2].bChannel*/FREQ2FBIN(2472, 1),
		 0},

		{/*Data[11].ctl_edges[0].bChannel*/FREQ2FBIN(2422, 1),
		 /*Data[11].ctl_edges[1].bChannel*/FREQ2FBIN(2427, 1),
		 /*Data[11].ctl_edges[2].bChannel*/FREQ2FBIN(2447, 1),
		 /*Data[11].ctl_edges[3].bChannel*/FREQ2FBIN(2462, 1)}
	},


//OSP_CAL_CTL_DATA_2G   ctl_power_data_2g[OSPREY_NUM_CTLS_2G];

#if AH_BYTE_ORDER == AH_BIG_ENDIAN
    {

	    {{{0, 60}, {1, 60}, {0, 60}, {0, 60}}},
	    {{{0, 60}, {1, 60}, {0, 60}, {0, 60}}}, 
	    {{{1, 60}, {0, 60}, {0, 60}, {1, 60}}},

	    {{{1, 60}, {0, 60}, {0, 60}, {0, 60}}},
	    {{{0, 60}, {1, 60}, {0, 60}, {0, 60}}},
	    {{{0, 60}, {1, 60}, {0, 60}, {0, 60}}},

	    {{{0, 60}, {1, 60}, {1, 60}, {0, 60}}},
	    {{{0, 60}, {1, 60}, {0, 60}, {0, 60}}},
	    {{{0, 60}, {1, 60}, {0, 60}, {0, 60}}},

	    {{{0, 60}, {1, 60}, {0, 60}, {0, 60}}},
	    {{{0, 60}, {1, 60}, {1, 60}, {1, 60}}},
	    {{{0, 60}, {1, 60}, {1, 60}, {1, 60}}},
        
    },
#else
	{
	    {{{60, 0}, {60, 1}, {60, 0}, {60, 0}}},
	    {{{60, 0}, {60, 1}, {60, 0}, {60, 0}}}, 
	    {{{60, 1}, {60, 0}, {60, 0}, {60, 1}}},

	    {{{60, 1}, {60, 0}, {60, 0}, {60, 0}}},
	    {{{60, 0}, {60, 1}, {60, 0}, {60, 0}}},
	    {{{60, 0}, {60, 1}, {60, 0}, {60, 0}}},

	    {{{60, 0}, {60, 1}, {60, 1}, {60, 0}}},
	    {{{60, 0}, {60, 1}, {60, 0}, {60, 0}}},
	    {{{60, 0}, {60, 1}, {60, 0}, {60, 0}}},

	    {{{60, 0}, {60, 1}, {60, 0}, {60, 0}}},
	    {{{60, 0}, {60, 1}, {60, 1}, {60, 1}}},
	    {{{60, 0}, {60, 1}, {60, 1}, {60, 1}}},
	},
#endif

//static    OSPREY_MODAL_EEP_HEADER   modal_header_5g=

	{

		    0x110,			//  ant_ctrl_common;                         // 4   idle, t1, t2, b (4 bits per setting)
		    0x22222,		//  ant_ctrl_common2;                        // 4    ra1l1, ra2l1, ra1l2, ra2l2, ra12
		    {0x000,0x000,0x000},	//  ant_ctrl_chain[OSPREY_MAX_CHAINS];       // 6   idle, t, r, rx1, rx12, b (2 bits each)
		    {0,0,0},			//   xatten1_db[OSPREY_MAX_CHAINS];           // 3  //xatten1_db for merlin (0xa20c/b20c 5:0)
		    {0,0,0},			//   xatten1_margin[OSPREY_MAX_CHAINS];          // 3  //xatten1_margin for merlin (0xa20c/b20c 16:12
			68,				//    temp_slope;
			0,				//    voltSlope;
		    {0,0,0,0,0}, // spur_chans[OSPREY_EEPROM_MODAL_SPURS];  // spur channels in usual fbin coding format
		    {-1,0,0},			//    noise_floor_thresh_ch[OSPREY_MAX_CHAINS]; // 3    //Check if the register is per chain
            {0,0,0},       // obdboffst_aux1                       // 3    //obdboffst5G_aux1
            {0,0,0},       // obdboffst_aux2                       // 3    //obdboffst5G_aux2
            {0,0,0,0,0},   // reserved
			0,											// quick drop  
		    0,				//   xpa_bias_lvl;                            // 1
		    0x0e,			//   tx_frame_to_data_start;                    // 1
		    0x0e,			//   tx_frame_to_pa_on;                         // 1
		    3,				//   txClip;                                     // 4 bits tx_clip, 4 bits dac_scale_cck
		    0,				//    antenna_gain;                           // 1
		    0x2d,			//   switchSettling;                        // 1
		    -30,			//    adcDesiredSize;                        // 1
		    0,				//   txEndToXpaOff;                         // 1
		    0x2,			//   txEndToRxOn;                           // 1
		    0xe,			//   tx_frame_to_xpa_on;                        // 1
		    28,				//   thresh62;                              // 1
  			0x0cf0e0e0,		//	 paprd_rate_mask_ht20						// 4
  			0x6cf0e0e0,		//	 paprd_rate_mask_ht40						// 4
		    0,				//   switchcomspdt;                         // 2
			0,				// bit: 0,1:chain0, 2,3:chain1, 4,5:chain2
			0,				//  rf_gain_cap
			0,				//  tx_gain_cap
			{0,0,0,0,0}    //futureModal[5];
	},

	{			// base_ext2
		0,
		0,
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0}
	},						

//static    A_UINT8            cal_freq_pier_5g[OSPREY_NUM_5G_CAL_PIERS]=
	{
		    //pPiers[0] =
		    FREQ2FBIN(5180, 0),
		    //pPiers[1] =
		    FREQ2FBIN(5220, 0),
		    //pPiers[2] =
		    FREQ2FBIN(5320, 0),
		    //pPiers[3] =
		    FREQ2FBIN(5400, 0),
		    //pPiers[4] =
		    FREQ2FBIN(5500, 0),
		    //pPiers[5] =
		    FREQ2FBIN(5600, 0),
		    //pPiers[6] =
		    FREQ2FBIN(5725, 0),
    		//pPiers[7] =
    		FREQ2FBIN(5825, 0)
	},

//static    OSP_CAL_DATA_PER_FREQ_OP_LOOP cal_pier_data_5g[OSPREY_MAX_CHAINS][OSPREY_NUM_5G_CAL_PIERS]=

	{
		{{0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},    {0,0,0,0,0},  {0,0,0,0,0}},
		{{0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},    {0,0,0,0,0},  {0,0,0,0,0}},
		{{0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},  {0,0,0,0,0},    {0,0,0,0,0},  {0,0,0,0,0}},

	},

//static    CAL_TARGET_POWER_LEG cal_target_freqbin_5g[OSPREY_NUM_5G_20_TARGET_POWERS]=

	{
			FREQ2FBIN(5180, 0),
			FREQ2FBIN(5220, 0),
			FREQ2FBIN(5320, 0),
			FREQ2FBIN(5400, 0),
			FREQ2FBIN(5500, 0),
			FREQ2FBIN(5600, 0),
			FREQ2FBIN(5725, 0),
			FREQ2FBIN(5825, 0)
	},

//static    OSP_CAL_TARGET_POWER_HT  cal_target_power_5g_ht20[OSPREY_NUM_5G_20_TARGET_POWERS]=

	{
			FREQ2FBIN(5180, 0),
			FREQ2FBIN(5240, 0),
			FREQ2FBIN(5320, 0),
			FREQ2FBIN(5500, 0),
			FREQ2FBIN(5700, 0),
			FREQ2FBIN(5745, 0),
			FREQ2FBIN(5725, 0),
			FREQ2FBIN(5825, 0)
	},

//static    OSP_CAL_TARGET_POWER_HT  cal_target_power_5g_ht40[OSPREY_NUM_5G_40_TARGET_POWERS]=

	{
			FREQ2FBIN(5180, 0),
			FREQ2FBIN(5240, 0),
			FREQ2FBIN(5320, 0),
			FREQ2FBIN(5500, 0),
			FREQ2FBIN(5700, 0),
			FREQ2FBIN(5745, 0),
			FREQ2FBIN(5725, 0),
			FREQ2FBIN(5825, 0)
	},


//static    CAL_TARGET_POWER_LEG cal_target_power_5g[OSPREY_NUM_5G_20_TARGET_POWERS]=


	{
        //6-24,36,48,54
	    {{20,20,20,10}},
	    {{20,20,20,10}},
	    {{20,20,20,10}},
	    {{20,20,20,10}},
	    {{20,20,20,10}},
	    {{20,20,20,10}},
	    {{20,20,20,10}},
	    {{20,20,20,10}},
	},

//static    OSP_CAL_TARGET_POWER_HT  cal_target_power_5g_ht20[OSPREY_NUM_5G_20_TARGET_POWERS]=

	{
        //0_8_16,1-3_9-11_17-19,
        //      4,5,6,7,12,13,14,15,20,21,22,23
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0}},
	},

//static    OSP_CAL_TARGET_POWER_HT  cal_target_power_5g_ht40[OSPREY_NUM_5G_40_TARGET_POWERS]=
	{
        //0_8_16,1-3_9-11_17-19,
        //      4,5,6,7,12,13,14,15,20,21,22,23
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0}},
	    {{20,20,10,10,0,0,10,10,0,0,10,10,0,0}},
	},

//static    A_UINT8            ctl_index_5g[OSPREY_NUM_CTLS_5G]=

	{
		    //pCtlIndex[0] =
		    0x10,
		    //pCtlIndex[1] =
		    0x16,
		    //pCtlIndex[2] =
		    0x18,
		    //pCtlIndex[3] =
		    0x40,
		    //pCtlIndex[4] =
		    0x46,
		    //pCtlIndex[5] =
		    0x48,
		    //pCtlIndex[6] =
		    0x30,
		    //pCtlIndex[7] =
		    0x36,
    		//pCtlIndex[8] =
    		0x38
	},

//    A_UINT8   ctl_freqbin_5G[OSPREY_NUM_CTLS_5G][OSPREY_NUM_BAND_EDGES_5G];

	{
	    {/* Data[0].ctl_edges[0].bChannel*/FREQ2FBIN(5180, 0),
	    /* Data[0].ctl_edges[1].bChannel*/FREQ2FBIN(5260, 0),
	    /* Data[0].ctl_edges[2].bChannel*/FREQ2FBIN(5280, 0),
	    /* Data[0].ctl_edges[3].bChannel*/FREQ2FBIN(5500, 0),
	    /* Data[0].ctl_edges[4].bChannel*/FREQ2FBIN(5600, 0),
	    /* Data[0].ctl_edges[5].bChannel*/FREQ2FBIN(5700, 0),
	    /* Data[0].ctl_edges[6].bChannel*/FREQ2FBIN(5745, 0),
	    /* Data[0].ctl_edges[7].bChannel*/FREQ2FBIN(5825, 0)},

	    {/* Data[1].ctl_edges[0].bChannel*/FREQ2FBIN(5180, 0),
	    /* Data[1].ctl_edges[1].bChannel*/FREQ2FBIN(5260, 0),
	    /* Data[1].ctl_edges[2].bChannel*/FREQ2FBIN(5280, 0),
	    /* Data[1].ctl_edges[3].bChannel*/FREQ2FBIN(5500, 0),
	    /* Data[1].ctl_edges[4].bChannel*/FREQ2FBIN(5520, 0),
	    /* Data[1].ctl_edges[5].bChannel*/FREQ2FBIN(5700, 0),
	    /* Data[1].ctl_edges[6].bChannel*/FREQ2FBIN(5745, 0),
	    /* Data[1].ctl_edges[7].bChannel*/FREQ2FBIN(5825, 0)},

	    {/* Data[2].ctl_edges[0].bChannel*/FREQ2FBIN(5190, 0),
	    /* Data[2].ctl_edges[1].bChannel*/FREQ2FBIN(5230, 0),
	    /* Data[2].ctl_edges[2].bChannel*/FREQ2FBIN(5270, 0),
	    /* Data[2].ctl_edges[3].bChannel*/FREQ2FBIN(5310, 0),
	    /* Data[2].ctl_edges[4].bChannel*/FREQ2FBIN(5510, 0),
	    /* Data[2].ctl_edges[5].bChannel*/FREQ2FBIN(5550, 0),
	    /* Data[2].ctl_edges[6].bChannel*/FREQ2FBIN(5670, 0),
	    /* Data[2].ctl_edges[7].bChannel*/FREQ2FBIN(5755, 0)},

	    {/* Data[3].ctl_edges[0].bChannel*/FREQ2FBIN(5180, 0),
	    /* Data[3].ctl_edges[1].bChannel*/FREQ2FBIN(5200, 0),
	    /* Data[3].ctl_edges[2].bChannel*/FREQ2FBIN(5260, 0),
	    /* Data[3].ctl_edges[3].bChannel*/FREQ2FBIN(5320, 0),
	    /* Data[3].ctl_edges[4].bChannel*/FREQ2FBIN(5500, 0),
	    /* Data[3].ctl_edges[5].bChannel*/FREQ2FBIN(5700, 0),
	    /* Data[3].ctl_edges[6].bChannel*/0xFF,
	    /* Data[3].ctl_edges[7].bChannel*/0xFF},

	    {/* Data[4].ctl_edges[0].bChannel*/FREQ2FBIN(5180, 0),
	    /* Data[4].ctl_edges[1].bChannel*/FREQ2FBIN(5260, 0),
	    /* Data[4].ctl_edges[2].bChannel*/FREQ2FBIN(5500, 0),
	    /* Data[4].ctl_edges[3].bChannel*/FREQ2FBIN(5700, 0),
	    /* Data[4].ctl_edges[4].bChannel*/0xFF,
	    /* Data[4].ctl_edges[5].bChannel*/0xFF,
	    /* Data[4].ctl_edges[6].bChannel*/0xFF,
	    /* Data[4].ctl_edges[7].bChannel*/0xFF},

	    {/* Data[5].ctl_edges[0].bChannel*/FREQ2FBIN(5190, 0),
	    /* Data[5].ctl_edges[1].bChannel*/FREQ2FBIN(5270, 0),
	    /* Data[5].ctl_edges[2].bChannel*/FREQ2FBIN(5310, 0),
	    /* Data[5].ctl_edges[3].bChannel*/FREQ2FBIN(5510, 0),
	    /* Data[5].ctl_edges[4].bChannel*/FREQ2FBIN(5590, 0),
	    /* Data[5].ctl_edges[5].bChannel*/FREQ2FBIN(5670, 0),
	    /* Data[5].ctl_edges[6].bChannel*/0xFF,
	    /* Data[5].ctl_edges[7].bChannel*/0xFF},

	    {/* Data[6].ctl_edges[0].bChannel*/FREQ2FBIN(5180, 0),
	    /* Data[6].ctl_edges[1].bChannel*/FREQ2FBIN(5200, 0),
	    /* Data[6].ctl_edges[2].bChannel*/FREQ2FBIN(5220, 0),
	    /* Data[6].ctl_edges[3].bChannel*/FREQ2FBIN(5260, 0),
	    /* Data[6].ctl_edges[4].bChannel*/FREQ2FBIN(5500, 0),
	    /* Data[6].ctl_edges[5].bChannel*/FREQ2FBIN(5600, 0),
	    /* Data[6].ctl_edges[6].bChannel*/FREQ2FBIN(5700, 0),
	    /* Data[6].ctl_edges[7].bChannel*/FREQ2FBIN(5745, 0)},

	    {/* Data[7].ctl_edges[0].bChannel*/FREQ2FBIN(5180, 0),
	    /* Data[7].ctl_edges[1].bChannel*/FREQ2FBIN(5260, 0),
	    /* Data[7].ctl_edges[2].bChannel*/FREQ2FBIN(5320, 0),
	    /* Data[7].ctl_edges[3].bChannel*/FREQ2FBIN(5500, 0),
	    /* Data[7].ctl_edges[4].bChannel*/FREQ2FBIN(5560, 0),
	    /* Data[7].ctl_edges[5].bChannel*/FREQ2FBIN(5700, 0),
	    /* Data[7].ctl_edges[6].bChannel*/FREQ2FBIN(5745, 0),
	    /* Data[7].ctl_edges[7].bChannel*/FREQ2FBIN(5825, 0)},

	    {/* Data[8].ctl_edges[0].bChannel*/FREQ2FBIN(5190, 0),
	    /* Data[8].ctl_edges[1].bChannel*/FREQ2FBIN(5230, 0),
	    /* Data[8].ctl_edges[2].bChannel*/FREQ2FBIN(5270, 0),
	    /* Data[8].ctl_edges[3].bChannel*/FREQ2FBIN(5510, 0),
	    /* Data[8].ctl_edges[4].bChannel*/FREQ2FBIN(5550, 0),
	    /* Data[8].ctl_edges[5].bChannel*/FREQ2FBIN(5670, 0),
	    /* Data[8].ctl_edges[6].bChannel*/FREQ2FBIN(5755, 0),
	    /* Data[8].ctl_edges[7].bChannel*/FREQ2FBIN(5795, 0)}
	},

//static    OSP_CAL_CTL_DATA_5G   ctlData_5G[OSPREY_NUM_CTLS_5G]=

#if AH_BYTE_ORDER == AH_BIG_ENDIAN
	{
	    {{{1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {0, 60}}},

	    {{{1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {0, 60}}},

	    {{{0, 60},
	      {1, 60},
	      {0, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60}}},
	    
	    {{{0, 60},
	      {1, 60},
	      {1, 60},
	      {0, 60},
	      {1, 60},
	      {0, 60},
	      {0, 60},
	      {0, 60}}},

	    {{{1, 60},
	      {1, 60},
	      {1, 60},
	      {0, 60},
	      {0, 60},
	      {0, 60},
	      {0, 60},
	      {0, 60}}},

	    {{{1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {0, 60},
	      {0, 60},
	      {0, 60}}},

	    {{{1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60}}},

	    {{{1, 60},
	      {1, 60},
	      {0, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {0, 60}}},

	    {{{1, 60},
	      {0, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {1, 60},
	      {0, 60},
	      {1, 60}}},
	}
#else
	{
	    {{{60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 0}}},

	    {{{60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 0}}},

	    {{{60, 0},
	      {60, 1},
	      {60, 0},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1}}},
	    
	    {{{60, 0},
	      {60, 1},
	      {60, 1},
	      {60, 0},
	      {60, 1},
	      {60, 0},
	      {60, 0},
	      {60, 0}}},

	    {{{60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 0},
	      {60, 0},
	      {60, 0},
	      {60, 0},
	      {60, 0}}},

	    {{{60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 0},
	      {60, 0},
	      {60, 0}}},

	    {{{60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1}}},

	    {{{60, 1},
	      {60, 1},
	      {60, 0},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 0}}},

	    {{{60, 1},
	      {60, 0},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 1},
	      {60, 0},
	      {60, 1}}},
	}
#endif
};

#endif
