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

#include "opt_ah.h"

#ifdef AH_SUPPORT_AR9300

#include "ah.h"
#include "ah_internal.h"

#include "ar9300/ar9300.h"

/* shorthands to compact tables for readability */
#define    OFDM    IEEE80211_T_OFDM
#define    CCK    IEEE80211_T_CCK
#define    TURBO    IEEE80211_T_TURBO
#define    XR    ATHEROS_T_XR
#define HT      IEEE80211_T_HT

#define AR9300_NUM_OFDM_RATES   8
#define AR9300_NUM_HT_SS_RATES  8
#define AR9300_NUM_HT_DS_RATES  8
#define AR9300_NUM_HT_TS_RATES  8
#define AR9300_NUM_HT_QS_RATES  8

/* Array Gain defined for TxBF */
#define AR9300_TXBF_2TX_ARRAY_GAIN  6  /* 2TX/SS 3 */
#define AR9300_TXBF_3TX_ARRAY_GAIN  10 /* 3TX/SS or 3TX/DS 4.8 */
#define AR9300_STBC_3TX_ARRAY_GAIN  10 /* 3TX/SS or 3TX/DS 4.8 */
#define AR9300_TXBF_4TX_ARRAY_GAIN  12 /* 4TX/SS or 4TX/DS or 4TX/TS 4.8 */
#define AR9300_STBC_4TX_ARRAY_GAIN  12 /* 4TX/SS or 4TX/DS or 4TX/TS 4.8 */

/* MCS RATE CODES - first and last */
#define AR9300_MCS0_RATE_CODE   0x80
#define AR9300_MCS31_RATE_CODE  0x9f

static inline void ar9300_init_rate_txpower_cck(struct ath_hal *ah,
       const HAL_RATE_TABLE *rt, u_int8_t rates_array[], u_int8_t chainmask);
static inline void ar9300_init_rate_txpower_ofdm(struct ath_hal* ah,
       const HAL_RATE_TABLE *rt, u_int8_t rates_array[], int rt_offset,
       u_int8_t chainmask);
static inline void ar9300_init_rate_txpower_ht(struct ath_hal *ah,
       const HAL_RATE_TABLE *rt, bool is40, u_int8_t rates_array[],
       int rt_ss_offset, int rt_ds_offset, int rt_ts_offset,
       int rt_qs_offset, u_int8_t chainmask);
#ifdef ATH_SUPPORT_TxBF
static inline void ar9300_init_rate_txpower_txbf(struct ath_hal *ah,
       const HAL_RATE_TABLE *rt, bool is40,
       int rt_ss_offset, int rt_ds_offset, int rt_ts_offset,
       int rt_qs_offset, u_int8_t chainmask);
#endif
static inline void ar9300_init_rate_txpower_stbc(struct ath_hal *ah,
       const HAL_RATE_TABLE *rt, bool is40,
       int rt_ss_offset, int rt_ds_offset, int rt_ts_offset,
       int rt_qs_offset, u_int8_t chainmask);
static inline void ar9300_adjust_rate_txpower_cdd(struct ath_hal *ah,
       const HAL_RATE_TABLE *rt, bool is40,
       int rt_ss_offset, int rt_ds_offset, int rt_ts_offset,
       int rt_qs_offset, u_int8_t chainmask);

#define AR9300_11A_RT_OFDM_OFFSET    0
HAL_RATE_TABLE ar9300_11a_table = {
    8,  /* number of rates */
    { 0 },
    {
/*                                                  short            ctrl */
/*             valid                 rate_code Preamble    dot11Rate Rate */
/*   6 Mb */ {  true, OFDM,    6000,     0x0b,    0x00, (0x80 | 12),   0 },
/*   9 Mb */ {  true, OFDM,    9000,     0x0f,    0x00,          18,   0 },
/*  12 Mb */ {  true, OFDM,   12000,     0x0a,    0x00, (0x80 | 24),   2 },
/*  18 Mb */ {  true, OFDM,   18000,     0x0e,    0x00,          36,   2 },
/*  24 Mb */ {  true, OFDM,   24000,     0x09,    0x00, (0x80 | 48),   4 },
/*  36 Mb */ {  true, OFDM,   36000,     0x0d,    0x00,          72,   4 },
/*  48 Mb */ {  true, OFDM,   48000,     0x08,    0x00,          96,   4 },
/*  54 Mb */ {  true, OFDM,   54000,     0x0c,    0x00,         108,   4 },
    },
};

HAL_RATE_TABLE ar9300_11a_half_table = {
    8,  /* number of rates */
    { 0 },
    {
/*                                                  short            ctrl */
/*             valid                 rate_code Preamble    dot11Rate Rate */
/*   6 Mb */ {  true, OFDM,    3000,     0x0b,    0x00, (0x80 |  6),   0 },
/*   9 Mb */ {  true, OFDM,    4500,     0x0f,    0x00,           9,   0 },
/*  12 Mb */ {  true, OFDM,    6000,     0x0a,    0x00, (0x80 | 12),   2 },
/*  18 Mb */ {  true, OFDM,    9000,     0x0e,    0x00,          18,   2 },
/*  24 Mb */ {  true, OFDM,   12000,     0x09,    0x00, (0x80 | 24),   4 },
/*  36 Mb */ {  true, OFDM,   18000,     0x0d,    0x00,          36,   4 },
/*  48 Mb */ {  true, OFDM,   24000,     0x08,    0x00,          48,   4 },
/*  54 Mb */ {  true, OFDM,   27000,     0x0c,    0x00,          54,   4 },
    },
};

HAL_RATE_TABLE ar9300_11a_quarter_table = {
    8,  /* number of rates */
    { 0 },
    {
/*                                                  short           ctrl */
/*            valid                 rate_code Preamble    dot11Rate Rate */
/*  6 Mb */ {  true, OFDM,    1500,     0x0b,    0x00, (0x80 |  3),   0 },
/*  9 Mb */ {  true, OFDM,    2250,     0x0f,    0x00,          4 ,   0 },
/* 12 Mb */ {  true, OFDM,    3000,     0x0a,    0x00, (0x80 |  6),   2 },
/* 18 Mb */ {  true, OFDM,    4500,     0x0e,    0x00,           9,   2 },
/* 24 Mb */ {  true, OFDM,    6000,     0x09,    0x00, (0x80 | 12),   4 },
/* 36 Mb */ {  true, OFDM,    9000,     0x0d,    0x00,          18,   4 },
/* 48 Mb */ {  true, OFDM,   12000,     0x08,    0x00,          24,   4 },
/* 54 Mb */ {  true, OFDM,   13500,     0x0c,    0x00,          27,   4 },
    },
};

HAL_RATE_TABLE ar9300_turbo_table = {
    8,  /* number of rates */
    { 0 },
    {
/*                                                 short            ctrl */
/*             valid                rate_code Preamble    dot11Rate Rate */
/*   6 Mb */ {  true, TURBO,   6000,    0x0b,    0x00, (0x80 | 12),   0 },
/*   9 Mb */ {  true, TURBO,   9000,    0x0f,    0x00,          18,   0 },
/*  12 Mb */ {  true, TURBO,  12000,    0x0a,    0x00, (0x80 | 24),   2 },
/*  18 Mb */ {  true, TURBO,  18000,    0x0e,    0x00,          36,   2 },
/*  24 Mb */ {  true, TURBO,  24000,    0x09,    0x00, (0x80 | 48),   4 },
/*  36 Mb */ {  true, TURBO,  36000,    0x0d,    0x00,          72,   4 },
/*  48 Mb */ {  true, TURBO,  48000,    0x08,    0x00,          96,   4 },
/*  54 Mb */ {  true, TURBO,  54000,    0x0c,    0x00,         108,   4 },
    },
};

HAL_RATE_TABLE ar9300_11b_table = {
    4,  /* number of rates */
    { 0 },
    {
/*                                                 short            ctrl */
/*             valid                rate_code Preamble    dot11Rate Rate */
/*   1 Mb */ {  true,  CCK,    1000,    0x1b,    0x00, (0x80 |  2),   0 },
/*   2 Mb */ {  true,  CCK,    2000,    0x1a,    0x04, (0x80 |  4),   1 },
/* 5.5 Mb */ {  true,  CCK,    5500,    0x19,    0x04, (0x80 | 11),   1 },
/*  11 Mb */ {  true,  CCK,   11000,    0x18,    0x04, (0x80 | 22),   1 },
    },
};


/* Venice TODO: round_up_rate() is broken when the rate table does not represent
 * rates in increasing order  e.g.  5.5, 11, 6, 9.
 * An average rate of 6 Mbps will currently map to 11 Mbps.
 */
#define AR9300_11G_RT_OFDM_OFFSET    4
HAL_RATE_TABLE ar9300_11g_table = {
    12,  /* number of rates */
    { 0 },
    {
/*                                                 short            ctrl */
/*             valid                rate_code Preamble    dot11Rate Rate */
/*   1 Mb */ {  true, CCK,     1000,    0x1b,    0x00, (0x80 |  2),   0 },
/*   2 Mb */ {  true, CCK,     2000,    0x1a,    0x04, (0x80 |  4),   1 },
/* 5.5 Mb */ {  true, CCK,     5500,    0x19,    0x04, (0x80 | 11),   2 },
/*  11 Mb */ {  true, CCK,    11000,    0x18,    0x04, (0x80 | 22),   3 },
/* Hardware workaround - remove rates 6, 9 from rate ctrl */
/*   6 Mb */ { false, OFDM,    6000,    0x0b,    0x00,          12,   4 },
/*   9 Mb */ { false, OFDM,    9000,    0x0f,    0x00,          18,   4 },
/*  12 Mb */ {  true, OFDM,   12000,    0x0a,    0x00,          24,   6 },
/*  18 Mb */ {  true, OFDM,   18000,    0x0e,    0x00,          36,   6 },
/*  24 Mb */ {  true, OFDM,   24000,    0x09,    0x00,          48,   8 },
/*  36 Mb */ {  true, OFDM,   36000,    0x0d,    0x00,          72,   8 },
/*  48 Mb */ {  true, OFDM,   48000,    0x08,    0x00,          96,   8 },
/*  54 Mb */ {  true, OFDM,   54000,    0x0c,    0x00,         108,   8 },
    },
};

HAL_RATE_TABLE ar9300_xr_table = {
    13,        /* number of rates */
    { 0 },
    {
/*                                                 short     ctrl */
/*            valid          rate_code Preamble    dot11Rate Rate */
/* 0.25 Mb */ {true,   XR,   250, 0x03,   0x00, (0x80 |  1),   0, 612, 612 },
/*  0.5 Mb */ {true,   XR,   500, 0x07,   0x00, (0x80 |  1),   0, 457, 457 },
/*    1 Mb */ {true,   XR,  1000, 0x02,   0x00, (0x80 |  2),   1, 228, 228 },
/*    2 Mb */ {true,   XR,  2000, 0x06,   0x00, (0x80 |  4),   2, 160, 160 },
/*    3 Mb */ {true,   XR,  3000, 0x01,   0x00, (0x80 |  6),   3, 140, 140 },
/*    6 Mb */ {true, OFDM,  6000, 0x0b,   0x00, (0x80 | 12),   4, 60,  60  },
/*    9 Mb */ {true, OFDM,  9000, 0x0f,   0x00,          18,   4, 60,  60  },
/*   12 Mb */ {true, OFDM, 12000, 0x0a,   0x00, (0x80 | 24),   6, 48,  48  },
/*   18 Mb */ {true, OFDM, 18000, 0x0e,   0x00,          36,   6, 48,  48  },
/*   24 Mb */ {true, OFDM, 24000, 0x09,   0x00,          48,   8, 44,  44  },
/*   36 Mb */ {true, OFDM, 36000, 0x0d,   0x00,          72,   8, 44,  44  },
/*   48 Mb */ {true, OFDM, 48000, 0x08,   0x00,          96,   8, 44,  44  },
/*   54 Mb */ {true, OFDM, 54000, 0x0c,   0x00,         108,   8, 44,  44  },
    },
};

#define AR9300_11NG_RT_OFDM_OFFSET       4
#define AR9300_11NG_RT_HT_SS_OFFSET      12
#define AR9300_11NG_RT_HT_DS_OFFSET      20
#define AR9300_11NG_RT_HT_TS_OFFSET      28
#define AR9300_11NG_RT_HT_QS_OFFSET      36
HAL_RATE_TABLE ar9300_11ng_table = {

    44,  /* number of rates */
    { 0 },
    {
/*                                                 short            ctrl */
/*             valid                rate_code Preamble    dot11Rate Rate */
/*   1 Mb */ {  true, CCK,     1000,    0x1b,    0x00, (0x80 |  2),   0 },
/*   2 Mb */ {  true, CCK,     2000,    0x1a,    0x04, (0x80 |  4),   1 },
/* 5.5 Mb */ {  true, CCK,     5500,    0x19,    0x04, (0x80 | 11),   2 },
/*  11 Mb */ {  true, CCK,    11000,    0x18,    0x04, (0x80 | 22),   3 },
/* Hardware workaround - remove rates 6, 9 from rate ctrl */
/*   6 Mb */ { false, OFDM,    6000,    0x0b,    0x00,          12,   4 },
/*   9 Mb */ { false, OFDM,    9000,    0x0f,    0x00,          18,   4 },
/*  12 Mb */ {  true, OFDM,   12000,    0x0a,    0x00,          24,   6 },
/*  18 Mb */ {  true, OFDM,   18000,    0x0e,    0x00,          36,   6 },
/*  24 Mb */ {  true, OFDM,   24000,    0x09,    0x00,          48,   8 },
/*  36 Mb */ {  true, OFDM,   36000,    0x0d,    0x00,          72,   8 },
/*  48 Mb */ {  true, OFDM,   48000,    0x08,    0x00,          96,   8 },
/*  54 Mb */ {  true, OFDM,   54000,    0x0c,    0x00,         108,   8 },
/*--- HT SS rates ---*/
/* 6.5 Mb */ {  true, HT,      6500,    0x80,    0x00,           0,   4 },
/*  13 Mb */ {  true, HT,     13000,    0x81,    0x00,           1,   6 },
/*19.5 Mb */ {  true, HT,     19500,    0x82,    0x00,           2,   6 },
/*  26 Mb */ {  true, HT,     26000,    0x83,    0x00,           3,   8 },
/*  39 Mb */ {  true, HT,     39000,    0x84,    0x00,           4,   8 },
/*  52 Mb */ {  true, HT,     52000,    0x85,    0x00,           5,   8 },
/*58.5 Mb */ {  true, HT,     58500,    0x86,    0x00,           6,   8 },
/*  65 Mb */ {  true, HT,     65000,    0x87,    0x00,           7,   8 },
/*--- HT DS rates ---*/
/*  13 Mb */ {  true, HT,     13000,    0x88,    0x00,           8,   4 },
/*  26 Mb */ {  true, HT,     26000,    0x89,    0x00,           9,   6 },
/*  39 Mb */ {  true, HT,     39000,    0x8a,    0x00,          10,   6 },
/*  52 Mb */ {  true, HT,     52000,    0x8b,    0x00,          11,   8 },
/*  78 Mb */ {  true, HT,     78000,    0x8c,    0x00,          12,   8 },
/* 104 Mb */ {  true, HT,    104000,    0x8d,    0x00,          13,   8 },
/* 117 Mb */ {  true, HT,    117000,    0x8e,    0x00,          14,   8 },
/* 130 Mb */ {  true, HT,    130000,    0x8f,    0x00,          15,   8 },
/*--- HT TS rates ---*/
/*19.5 Mb */ {  true, HT,     19500,    0x90,    0x00,          16,   4 },
/*  39 Mb */ {  true, HT,     39000,    0x91,    0x00,          17,   6 },
/*58.5 Mb */ {  true, HT,     58500,    0x92,    0x00,          18,   6 },
/*  78 Mb */ {  true, HT,     78000,    0x93,    0x00,          19,   8 },
/* 117 Mb */ {  true, HT,    117000,    0x94,    0x00,          20,   8 },
/* 156 Mb */ {  true, HT,    156000,    0x95,    0x00,          21,   8 },
/*175.5Mb */ {  true, HT,    175500,    0x96,    0x00,          22,   8 },
/* 195 Mb */ {  true, HT,    195000,    0x97,    0x00,          23,   8 },
/*--- HT QS rates ---*/
/*  26 Mb */ {  true, HT,     26000,    0x98,    0x00,          24,   4 },
/*  52 Mb */ {  true, HT,     52000,    0x99,    0x00,          25,   6 },
/*  78 Mb */ {  true, HT,     78000,    0x9a,    0x00,          26,   6 },
/* 104 Mb */ {  true, HT,    104000,    0x9b,    0x00,          27,   8 },
/* 156 Mb */ {  true, HT,    156000,    0x9c,    0x00,          28,   8 },
/* 208 Mb */ {  true, HT,    208000,    0x9d,    0x00,          29,   8 },
/* 234 Mb */ {  true, HT,    234000,    0x9e,    0x00,          30,   8 },
/* 260 Mb */ {  true, HT,    260000,    0x9f,    0x00,          31,   8 },
    },
};

#define AR9300_11NA_RT_OFDM_OFFSET       0
#define AR9300_11NA_RT_HT_SS_OFFSET      8
#define AR9300_11NA_RT_HT_DS_OFFSET      16
#define AR9300_11NA_RT_HT_TS_OFFSET      24
static HAL_RATE_TABLE ar9300_11na_table = {

    32,  /* number of rates */
    { 0 },
    {
/*                                                 short            ctrl */
/*             valid                rate_code Preamble    dot11Rate Rate */
/*   6 Mb */ {  true, OFDM,    6000,    0x0b,    0x00, (0x80 | 12),   0 },
/*   9 Mb */ {  true, OFDM,    9000,    0x0f,    0x00,          18,   0 },
/*  12 Mb */ {  true, OFDM,   12000,    0x0a,    0x00, (0x80 | 24),   2 },
/*  18 Mb */ {  true, OFDM,   18000,    0x0e,    0x00,          36,   2 },
/*  24 Mb */ {  true, OFDM,   24000,    0x09,    0x00, (0x80 | 48),   4 },
/*  36 Mb */ {  true, OFDM,   36000,    0x0d,    0x00,          72,   4 },
/*  48 Mb */ {  true, OFDM,   48000,    0x08,    0x00,          96,   4 },
/*  54 Mb */ {  true, OFDM,   54000,    0x0c,    0x00,         108,   4 },
/*--- HT SS rates ---*/
/* 6.5 Mb */ {  true, HT,      6500,    0x80,    0x00,           0,   0 },
/*  13 Mb */ {  true, HT,     13000,    0x81,    0x00,           1,   2 },
/*19.5 Mb */ {  true, HT,     19500,    0x82,    0x00,           2,   2 },
/*  26 Mb */ {  true, HT,     26000,    0x83,    0x00,           3,   4 },
/*  39 Mb */ {  true, HT,     39000,    0x84,    0x00,           4,   4 },
/*  52 Mb */ {  true, HT,     52000,    0x85,    0x00,           5,   4 },
/*58.5 Mb */ {  true, HT,     58500,    0x86,    0x00,           6,   4 },
/*  65 Mb */ {  true, HT,     65000,    0x87,    0x00,           7,   4 },
/*--- HT DS rates ---*/
/*  13 Mb */ {  true, HT,     13000,    0x88,    0x00,           8,   0 },
/*  26 Mb */ {  true, HT,     26000,    0x89,    0x00,           9,   2 },
/*  39 Mb */ {  true, HT,     39000,    0x8a,    0x00,          10,   2 },
/*  52 Mb */ {  true, HT,     52000,    0x8b,    0x00,          11,   4 },
/*  78 Mb */ {  true, HT,     78000,    0x8c,    0x00,          12,   4 },
/* 104 Mb */ {  true, HT,    104000,    0x8d,    0x00,          13,   4 },
/* 117 Mb */ {  true, HT,    117000,    0x8e,    0x00,          14,   4 },
/* 130 Mb */ {  true, HT,    130000,    0x8f,    0x00,          15,   4 },
/*--- HT TS rates ---*/
/*19.5 Mb */ {  true, HT,     19500,    0x90,    0x00,          16,   0 },
/*  39 Mb */ {  true, HT,     39000,    0x91,    0x00,          17,   2 },
/*58.5 Mb */ {  true, HT,     58500,    0x92,    0x00,          18,   2 },
/*  78 Mb */ {  true, HT,     78000,    0x93,    0x00,          19,   4 },
/* 117 Mb */ {  true, HT,    117000,    0x94,    0x00,          20,   4 },
/* 156 Mb */ {  true, HT,    156000,    0x95,    0x00,          21,   4 },
/*175.5Mb */ {  true, HT,    175500,    0x96,    0x00,          22,   4 },
/* 195 Mb */ {  true, HT,    195000,    0x97,    0x00,          23,   4 },
    },
};

#undef    OFDM
#undef    CCK
#undef    TURBO
#undef    XR
#undef    HT
#undef    HT_HGI

const HAL_RATE_TABLE *
ar9300_get_rate_table(struct ath_hal *ah, u_int mode)
{
    struct ath_hal_private *ahpriv = AH_PRIVATE(ah);
    HAL_CAPABILITIES *p_cap = &ahpriv->ah_caps;
    HAL_RATE_TABLE *rt;

    switch (mode) {
    case HAL_MODE_11A:
        rt = &ar9300_11a_table;
        break;
    case HAL_MODE_11A_HALF_RATE:
        if (p_cap->hal_chan_half_rate) {
            rt = &ar9300_11a_half_table;
            break;
        }
        return AH_NULL;
    case HAL_MODE_11A_QUARTER_RATE:
        if (p_cap->hal_chan_quarter_rate) {
            rt = &ar9300_11a_quarter_table;
            break;
        }
        return AH_NULL;
    case HAL_MODE_11B:
        rt = &ar9300_11b_table;
        break;
    case HAL_MODE_11G:
        rt =  &ar9300_11g_table;
        break;
    case HAL_MODE_TURBO:
    case HAL_MODE_108G:
        rt =  &ar9300_turbo_table;
        break;
    case HAL_MODE_XR:
        rt = &ar9300_xr_table;
        break;
    case HAL_MODE_11NG_HT20:
    case HAL_MODE_11NG_HT40PLUS:
    case HAL_MODE_11NG_HT40MINUS:
        rt = &ar9300_11ng_table;
        break;
    case HAL_MODE_11NA_HT20:
    case HAL_MODE_11NA_HT40PLUS:
    case HAL_MODE_11NA_HT40MINUS:
        rt = &ar9300_11na_table;
        break;
    default:
        HDPRINTF(ah, HAL_DBG_CHANNEL,
            "%s: invalid mode 0x%x\n", __func__, mode);
        return AH_NULL;
    }
    ath_hal_setupratetable(ah, rt);
    return rt;
}

static bool
ar9300_invalid_stbc_cfg(int tx_chains, u_int8_t rate_code)
{
    switch (tx_chains) {
    case 0:
        return true;

    case 1: /* Single Chain */
        if ((rate_code < 0x80) || (rate_code > 0x87)) {
            return true;
        } else {
            return false;
        }

    case 2: /* 2 Chains */
        if ((rate_code < 0x80) || (rate_code > 0x8f)) {
            return true;
        } else {
            return false;
        }

    case 3: /* 3 Chains */
        if ((rate_code < 0x80) || (rate_code > 0x97)) {
            return true;
        } else {
            return false;
        }

    case 4: /* 4 Chains */
        if ((rate_code < 0x80) || (rate_code > 0x9f)) {
            return true;
        } else {
            return false;
        }

    default:
        HALASSERT(0);
        break;
    } 

    return true;
}

#ifdef ATH_SUPPORT_TxBF
bool
ar9300_invalid_txbf_cfg(int tx_chains, u_int8_t rate_code)
{
    switch (tx_chains) {
    case 0:
        return true;
    case 1: /* Single Chain */
        if ((rate_code < 0x80) || (rate_code > 0x87)) {
            return true;
        } else {
            return false;
        }
    case 2: /* 2 Chains */
        if ((rate_code < 0x80) || (rate_code > 0x8F)) {
            return true;
        } else {
            return false;
        }
    case 3: /* 3 Chains */
        if ((rate_code < 0x80) || (rate_code > 0x97)) {
            return true;
        } else {
            return false;
        }
    case 4: /* 4 Chains */
        if ((rate_code < 0x80) || (rate_code > 0x9f)) {
            return true;
        } else {
            return false;
        }
    default:
        HALASSERT(0);
        break;
    } 

    return true;

}

void
ar9300_get_perrate_txbf_flags(struct ath_hal *ah,
                     u_int8_t txbf_disable_flag[][AH_MAX_CHAINS])
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    HAL_CHANNEL_INTERNAL *chan = AH_PRIVATE(ah)->ah_curchan;
    u_int mode = ath_hal_get_curmode(ah, chan);
    const HAL_RATE_TABLE *rt;
    int i, j, nchain = 0;


    /*
     * Note that this logic to disable TxBF applies only to higher 
     * power reference designs operating in  ETSI and MKK domains
     */
    if (!is_reg_dmn_etsi(ahp->reg_dmn) && !is_reg_dmn_mkk(ahp->reg_dmn)){
        return;
    }

    /*  Check whether we are in the right mode */
    if ((mode != HAL_MODE_11NA_HT20) && (mode != HAL_MODE_11NA_HT40PLUS) &&
        (mode != HAL_MODE_11NA_HT40MINUS) && (mode != HAL_MODE_11NG_HT20) &&
        (mode != HAL_MODE_11NG_HT40PLUS) && ( mode != HAL_MODE_11NG_HT40MINUS)){
        return;
    }
    
    /*
     * Disable TxBF where CDD is constrained by the upper limit. This
     * will likely be the case on high power reference designs. 
     */
    rt = ar9300_get_rate_table(ah, mode);
    HALASSERT(rt != NULL);

    for (i = 0 ; i < rt->rateCount; i++) { 
        for (j = 0; j < ar9300_get_ntxchains(ahp->ah_tx_chainmask); j++) {
            nchain = (j + 1);
            if ((ahp->txpower[i][j] == ahp->upper_limit[j]) ||
               (ar9300_invalid_txbf_cfg(nchain, rt->info[i].rate_code) == true)){
                txbf_disable_flag[i][j] = 1; /* Disable TxBF flag */ 
            }
            if (!(j) || (rt->info[i].rate_code < AR9300_MCS0_RATE_CODE ) ||
               (rt->info[i].rate_code > AR9300_MCS31_RATE_CODE) ||
               (ar9300_invalid_txbf_cfg(nchain, rt->info[i].rate_code) == true)){
                continue;
            }
            HDPRINTF(ah, HAL_DBG_POWER_MGMT,
                     " Index[%2d] Rate[0x%02x] Chains=%d TxBF=%3s\n",
                     i, rt->info[i].rate_code, nchain,
                     (txbf_disable_flag[i][j]?"OFF":"ON"));
        }
    }
    return;
}
#endif

int16_t
ar9300_get_rate_txpower(struct ath_hal *ah, u_int mode, u_int8_t rate_index,
                     u_int8_t chainmask, u_int8_t xmit_mode)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    int num_chains = ar9300_get_ntxchains(chainmask);

    switch (xmit_mode) {
    case AR9300_DEF_MODE:
        return ahp->txpower[rate_index][num_chains-1];

#ifdef  ATH_SUPPORT_TxBF
    case AR9300_TXBF_MODE:
        return ahp->txpower_txbf[rate_index][num_chains-1];
#endif
    
    case AR9300_STBC_MODE:
        return ahp->txpower_stbc[rate_index][num_chains-1];
       
    default:
        HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid mode 0x%x\n",
             __func__, xmit_mode);
        HALASSERT(0);
        break;
    }

    return ahp->txpower[rate_index][num_chains-1];
}

extern void
ar9300_adjust_reg_txpower_cdd(struct ath_hal *ah, 
                      u_int8_t power_per_rate[])
                      
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    int16_t twice_array_gain, cdd_power = 0;
    int i;

    /*
     *  Adjust the upper limit for CDD factoring in the array gain .
     *  The array gain is the same as TxBF, hence reuse the same defines. 
     */
    switch (ahp->ah_tx_chainmask) {

    case OSPREY_1_CHAINMASK:
        cdd_power = ahp->upper_limit[0];
        ahp->cdd_gain_comp[0] = 0;
        break;
  
    case OSPREY_2LOHI_CHAINMASK:
    case OSPREY_2LOMID_CHAINMASK:
        ahp->cdd_gain_comp[1] = ahp->upper_limit[1];
        twice_array_gain =
           (ahp->twice_antenna_gain >= ahp->twice_antenna_reduction)?
           -(AR9300_TXBF_2TX_ARRAY_GAIN) :
           ((int16_t)AH_MIN((ahp->twice_antenna_reduction -
           (ahp->twice_antenna_gain + AR9300_TXBF_2TX_ARRAY_GAIN)), 0));
        cdd_power = ahp->upper_limit[1] + twice_array_gain;
        cdd_power = AH_MAX(0, cdd_power);
        ahp->cdd_gain_comp[1] -= cdd_power;
        /* Adjust OFDM legacy rates as well */
        for (i = ALL_TARGET_LEGACY_6_24; i <= ALL_TARGET_LEGACY_54; i++) {
            if (power_per_rate[i] > cdd_power) {
                power_per_rate[i] = cdd_power; 
            } 
        }
            
        /* 2Tx/(n-1) stream Adjust rates MCS0 through MCS 7  HT 20*/
        for (i = ALL_TARGET_HT20_0_8_16_24; i <= ALL_TARGET_HT20_7; i++) {
            if (power_per_rate[i] > cdd_power) {
                power_per_rate[i] = cdd_power; 
            } 
        } 

        /* 2Tx/(n-1) stream Adjust rates MCS0 through MCS 7  HT 40*/
        for (i = ALL_TARGET_HT40_0_8_16_24; i <= ALL_TARGET_HT40_7; i++) {
            if (power_per_rate[i] > cdd_power) {
                power_per_rate[i] = cdd_power; 
            } 
        } 
        break;
        
    case OSPREY_3_CHAINMASK:
        ahp->cdd_gain_comp[2] = ahp->upper_limit[2];
        twice_array_gain =
            (ahp->twice_antenna_gain >= ahp->twice_antenna_reduction)?
            -(AR9300_TXBF_3TX_ARRAY_GAIN) :
            ((int16_t)AH_MIN((ahp->twice_antenna_reduction -
            (ahp->twice_antenna_gain + AR9300_TXBF_3TX_ARRAY_GAIN)), 0));
        cdd_power = ahp->upper_limit[2] + twice_array_gain;
        cdd_power = AH_MAX(0, cdd_power);
        ahp->cdd_gain_comp[2] -= cdd_power;
        /* Adjust OFDM legacy rates as well */
        for (i = ALL_TARGET_LEGACY_6_24; i <= ALL_TARGET_LEGACY_54; i++) {
            if (power_per_rate[i] > cdd_power) {
                power_per_rate[i] = cdd_power; 
            } 
        }
        /* 3Tx/(n-1)streams Adjust rates MCS0 through MCS 15 HT20 */
        for (i = ALL_TARGET_HT20_0_8_16_24; i <= ALL_TARGET_HT20_15; i++) {
            if (power_per_rate[i] > cdd_power) {
                power_per_rate[i] = cdd_power;
            }
        }

        /* 3Tx/(n-1)streams Adjust rates MCS0 through MCS 15 HT40 */
        for (i = ALL_TARGET_HT40_0_8_16_24; i <= ALL_TARGET_HT40_15; i++) {
            if (power_per_rate[i] > cdd_power) {
                power_per_rate[i] = cdd_power;
            }
        }

        break;

    case JET_4_CHAINMASK:
        ahp->cdd_gain_comp[3] = ahp->upper_limit[3];
        twice_array_gain =
            (ahp->twice_antenna_gain >= ahp->twice_antenna_reduction)?
            -(AR9300_TXBF_4TX_ARRAY_GAIN) :
            ((int16_t)AH_MIN((ahp->twice_antenna_reduction -
            (ahp->twice_antenna_gain + AR9300_TXBF_4TX_ARRAY_GAIN)), 0));
        cdd_power = ahp->upper_limit[3] + twice_array_gain;
        cdd_power = AH_MAX(0, cdd_power);
        ahp->cdd_gain_comp[3] -= cdd_power;
        /* Adjust OFDM legacy rates as well */
        for (i = ALL_TARGET_LEGACY_6_24; i <= ALL_TARGET_LEGACY_54; i++) {
            if (power_per_rate[i] > cdd_power) {
                power_per_rate[i] = cdd_power;
            }
        }
        /* 4Tx/(n-1)streams Adjust rates MCS0 through MCS 23 HT20 */
        for (i = ALL_TARGET_HT20_0_8_16_24; i <= ALL_TARGET_HT20_23; i++) {
            if (power_per_rate[i] > cdd_power) {
                power_per_rate[i] = cdd_power;
            }
        }

        /* 4Tx/(n-1)streams Adjust rates MCS0 through MCS 23 HT40 */
        for (i = ALL_TARGET_HT40_0_8_16_24; i <= ALL_TARGET_HT40_23; i++) {
            if (power_per_rate[i] > cdd_power) {
                power_per_rate[i] = cdd_power;
            }
        }

        break;

    default:
        HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                 __func__, ahp->ah_tx_chainmask);
        break;
    }

    return;
}

extern void
ar9300_init_rate_txpower(struct ath_hal *ah, u_int mode,
                      HAL_CHANNEL_INTERNAL *chan,
                      u_int8_t power_per_rate[], u_int8_t chainmask)
{
    const HAL_RATE_TABLE *rt;
    bool is40 = IS_CHAN_HT40(chan);

    rt = ar9300_get_rate_table(ah, mode);
    HALASSERT(rt != NULL);

    switch (mode) {
    case HAL_MODE_11A:
        ar9300_init_rate_txpower_ofdm(ah, rt, power_per_rate,
                              AR9300_11A_RT_OFDM_OFFSET, chainmask);
        break;
    case HAL_MODE_11NA_HT20:
    case HAL_MODE_11NA_HT40PLUS:
    case HAL_MODE_11NA_HT40MINUS:
        ar9300_init_rate_txpower_ofdm(ah, rt, power_per_rate,
                              AR9300_11NA_RT_OFDM_OFFSET, chainmask);
        ar9300_init_rate_txpower_ht(ah, rt, is40, power_per_rate,
                            AR9300_11NA_RT_HT_SS_OFFSET,
                            AR9300_11NA_RT_HT_DS_OFFSET,
                            AR9300_11NA_RT_HT_TS_OFFSET, 0, chainmask);
#ifdef  ATH_SUPPORT_TxBF
        ar9300_init_rate_txpower_txbf(ah, rt, is40,
                            AR9300_11NA_RT_HT_SS_OFFSET,
                            AR9300_11NA_RT_HT_DS_OFFSET,
                            AR9300_11NA_RT_HT_TS_OFFSET, 0, chainmask);
#endif
        ar9300_init_rate_txpower_stbc(ah, rt, is40,
                            AR9300_11NA_RT_HT_SS_OFFSET,
                            AR9300_11NA_RT_HT_DS_OFFSET,
                            AR9300_11NA_RT_HT_TS_OFFSET, 0, chainmask);
        /* For FCC the array gain has to be factored for CDD mode */
        if (is_reg_dmn_fcc(chan->conformance_test_limit)) {
            ar9300_adjust_rate_txpower_cdd(ah, rt, is40, 
                            AR9300_11NA_RT_HT_SS_OFFSET,
                            AR9300_11NA_RT_HT_DS_OFFSET,
                            AR9300_11NA_RT_HT_TS_OFFSET, 0, chainmask);
        }
        break;
    case HAL_MODE_11G:
        ar9300_init_rate_txpower_cck(ah, rt, power_per_rate, chainmask);
        ar9300_init_rate_txpower_ofdm(ah, rt, power_per_rate,
                              AR9300_11G_RT_OFDM_OFFSET, chainmask);
        break;
    case HAL_MODE_11B:
        ar9300_init_rate_txpower_cck(ah, rt, power_per_rate, chainmask);
        break;
    case HAL_MODE_11NG_HT20:
    case HAL_MODE_11NG_HT40PLUS:
    case HAL_MODE_11NG_HT40MINUS:
        ar9300_init_rate_txpower_cck(ah, rt, power_per_rate,  chainmask);
        ar9300_init_rate_txpower_ofdm(ah, rt, power_per_rate,
                              AR9300_11NG_RT_OFDM_OFFSET, chainmask);
        ar9300_init_rate_txpower_ht(ah, rt, is40, power_per_rate,
                            AR9300_11NG_RT_HT_SS_OFFSET,
                            AR9300_11NG_RT_HT_DS_OFFSET,
                            AR9300_11NG_RT_HT_TS_OFFSET,
                            AR9300_11NG_RT_HT_QS_OFFSET, chainmask);
#ifdef  ATH_SUPPORT_TxBF
        ar9300_init_rate_txpower_txbf(ah, rt, is40,
                            AR9300_11NG_RT_HT_SS_OFFSET,
                            AR9300_11NG_RT_HT_DS_OFFSET,
                            AR9300_11NG_RT_HT_TS_OFFSET,
                            AR9300_11NG_RT_HT_QS_OFFSET, chainmask);
#endif
        ar9300_init_rate_txpower_stbc(ah, rt, is40,
                            AR9300_11NG_RT_HT_SS_OFFSET,
                            AR9300_11NG_RT_HT_DS_OFFSET,
                            AR9300_11NG_RT_HT_TS_OFFSET,
                            AR9300_11NG_RT_HT_QS_OFFSET, chainmask);
        /* For FCC the array gain needs to be factored for CDD mode */
        if (is_reg_dmn_fcc(chan->conformance_test_limit)) {
            ar9300_adjust_rate_txpower_cdd(ah, rt, is40, 
                            AR9300_11NG_RT_HT_SS_OFFSET,
                            AR9300_11NG_RT_HT_DS_OFFSET,
                            AR9300_11NG_RT_HT_TS_OFFSET,
                            AR9300_11NG_RT_HT_QS_OFFSET, chainmask);
        }
        break;
    default:
        HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid mode 0x%x\n",
             __func__, mode);
        HALASSERT(0);
        break;
    }

}

static inline void
ar9300_init_rate_txpower_cck(struct ath_hal *ah, const HAL_RATE_TABLE *rt,
                         u_int8_t rates_array[], u_int8_t chainmask)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    /*
     * Pick the lower of the long-preamble txpower, and short-preamble tx power.
     * Unfortunately, the rate table doesn't have separate entries for these!.
     */
    switch (chainmask) {
    case OSPREY_1_CHAINMASK:
        ahp->txpower[0][0] = rates_array[ALL_TARGET_LEGACY_1L_5L];
        ahp->txpower[1][0] = rates_array[ALL_TARGET_LEGACY_1L_5L];
        ahp->txpower[2][0] = AH_MIN(rates_array[ALL_TARGET_LEGACY_1L_5L],
                                  rates_array[ALL_TARGET_LEGACY_5S]);
        ahp->txpower[3][0] = AH_MIN(rates_array[ALL_TARGET_LEGACY_11L],
                                      rates_array[ALL_TARGET_LEGACY_11S]);
        break;
    case OSPREY_2LOHI_CHAINMASK:
    case OSPREY_2LOMID_CHAINMASK:
        ahp->txpower[0][1] = rates_array[ALL_TARGET_LEGACY_1L_5L];
        ahp->txpower[1][1] = rates_array[ALL_TARGET_LEGACY_1L_5L];
        ahp->txpower[2][1] = AH_MIN(rates_array[ALL_TARGET_LEGACY_1L_5L],
                                  rates_array[ALL_TARGET_LEGACY_5S]);
        ahp->txpower[3][1] = AH_MIN(rates_array[ALL_TARGET_LEGACY_11L],
                                  rates_array[ALL_TARGET_LEGACY_11S]);
        break;
    case OSPREY_3_CHAINMASK:
        ahp->txpower[0][2] = rates_array[ALL_TARGET_LEGACY_1L_5L];
        ahp->txpower[1][2] = rates_array[ALL_TARGET_LEGACY_1L_5L];
        ahp->txpower[2][2] = AH_MIN(rates_array[ALL_TARGET_LEGACY_1L_5L],
                                   rates_array[ALL_TARGET_LEGACY_5S]);
        ahp->txpower[3][2] = AH_MIN(rates_array[ALL_TARGET_LEGACY_11L],
                                       rates_array[ALL_TARGET_LEGACY_11S]);
        break;
    case JET_4_CHAINMASK:
        ahp->txpower[0][3] = rates_array[ALL_TARGET_LEGACY_1L_5L];
        ahp->txpower[1][3] = rates_array[ALL_TARGET_LEGACY_1L_5L];
        ahp->txpower[2][3] = AH_MIN(rates_array[ALL_TARGET_LEGACY_1L_5L],
                                   rates_array[ALL_TARGET_LEGACY_5S]);
        ahp->txpower[3][3] = AH_MIN(rates_array[ALL_TARGET_LEGACY_11L],
                                       rates_array[ALL_TARGET_LEGACY_11S]);
        break;
    default:
        HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                 __func__, chainmask);
        break;
    }
}

static inline void
ar9300_init_rate_txpower_ofdm(struct ath_hal *ah, const HAL_RATE_TABLE *rt,
                          u_int8_t rates_array[], int rt_offset,
                          u_int8_t chainmask)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    int16_t twice_array_gain, cdd_power = 0;
    int i, j;
    u_int8_t ofdm_rt_2_pwr_idx[8] =
    {
        ALL_TARGET_LEGACY_6_24,
        ALL_TARGET_LEGACY_6_24,
        ALL_TARGET_LEGACY_6_24,
        ALL_TARGET_LEGACY_6_24,
        ALL_TARGET_LEGACY_6_24,
        ALL_TARGET_LEGACY_36,
        ALL_TARGET_LEGACY_48,
        ALL_TARGET_LEGACY_54,
    };

    /*
     *  For FCC adjust the upper limit for CDD factoring in the array gain.
     *  The array gain is the same as TxBF, hence reuse the same defines. 
     */
    for (i = rt_offset; i < rt_offset + AR9300_NUM_OFDM_RATES; i++) {

        /* Get the correct OFDM rate to Power table Index */
        j = ofdm_rt_2_pwr_idx[i- rt_offset]; 

        switch (chainmask) {
        case OSPREY_1_CHAINMASK:
            ahp->txpower[i][0] = rates_array[j];
            break;
        case OSPREY_2LOHI_CHAINMASK:
        case OSPREY_2LOMID_CHAINMASK:
            ahp->txpower[i][1] = rates_array[j];
            if (is_reg_dmn_fcc(ahp->reg_dmn)){
                twice_array_gain = (ahp->twice_antenna_gain >=
                ahp->twice_antenna_reduction)?
                -(AR9300_TXBF_2TX_ARRAY_GAIN) :
                ((int16_t)AH_MIN((ahp->twice_antenna_reduction -
               (ahp->twice_antenna_gain + AR9300_TXBF_2TX_ARRAY_GAIN)), 0));
                cdd_power = ahp->upper_limit[1] + twice_array_gain;
                cdd_power = AH_MAX(0, cdd_power);
                if (ahp->txpower[i][1] > cdd_power){
                    ahp->txpower[i][1] = cdd_power;
                }
            }
            break;
        case OSPREY_3_CHAINMASK:
            ahp->txpower[i][2] = rates_array[j];
            if (is_reg_dmn_fcc(ahp->reg_dmn)) {
                twice_array_gain =
                (ahp->twice_antenna_gain >= ahp->twice_antenna_reduction)?
                -(AR9300_TXBF_3TX_ARRAY_GAIN):
                ((int16_t)AH_MIN((ahp->twice_antenna_reduction -
                (ahp->twice_antenna_gain + AR9300_TXBF_3TX_ARRAY_GAIN)), 0));
                cdd_power = ahp->upper_limit[2] + twice_array_gain;
                cdd_power = AH_MAX(0, cdd_power);
                if (ahp->txpower[i][2] > cdd_power){
                    ahp->txpower[i][2] = cdd_power;
                }
            } 
            break;
        case JET_4_CHAINMASK:
            ahp->txpower[i][3] = rates_array[j];
            if (is_reg_dmn_fcc(ahp->reg_dmn)) {
                twice_array_gain =
                (ahp->twice_antenna_gain >= ahp->twice_antenna_reduction)?
                -(AR9300_TXBF_4TX_ARRAY_GAIN):
                ((int16_t)AH_MIN((ahp->twice_antenna_reduction -
                (ahp->twice_antenna_gain + AR9300_TXBF_4TX_ARRAY_GAIN)), 0));
                cdd_power = ahp->upper_limit[3] + twice_array_gain;
                cdd_power = AH_MAX(0, cdd_power);
                if (ahp->txpower[i][3] > cdd_power){
                    ahp->txpower[i][3] = cdd_power;
                }
            }
            break;
        default:
            HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                     __func__, chainmask);
            break;
        }
    }
}

static  u_int8_t mcs_rate_2_pwr_idx_ht20[32] =
    {
        ALL_TARGET_HT20_0_8_16_24,
        ALL_TARGET_HT20_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT20_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT20_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT20_4,
        ALL_TARGET_HT20_5,
        ALL_TARGET_HT20_6,
        ALL_TARGET_HT20_7,
        ALL_TARGET_HT20_0_8_16_24,
        ALL_TARGET_HT20_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT20_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT20_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT20_12,
        ALL_TARGET_HT20_13,
        ALL_TARGET_HT20_14,
        ALL_TARGET_HT20_15,
        ALL_TARGET_HT20_0_8_16_24,
        ALL_TARGET_HT20_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT20_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT20_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT20_20,
        ALL_TARGET_HT20_21,
        ALL_TARGET_HT20_22,
        ALL_TARGET_HT20_23,
        ALL_TARGET_HT20_0_8_16_24,
        ALL_TARGET_HT20_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT20_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT20_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT20_28,
        ALL_TARGET_HT20_29,
        ALL_TARGET_HT20_30,
        ALL_TARGET_HT20_31
    };

static   u_int8_t mcs_rate_2_pwr_idx_ht40[32] =
    {
        ALL_TARGET_HT40_0_8_16_24,
        ALL_TARGET_HT40_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT40_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT40_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT40_4,
        ALL_TARGET_HT40_5,
        ALL_TARGET_HT40_6,
        ALL_TARGET_HT40_7,
        ALL_TARGET_HT40_0_8_16_24,
        ALL_TARGET_HT40_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT40_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT40_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT40_12,
        ALL_TARGET_HT40_13,
        ALL_TARGET_HT40_14,
        ALL_TARGET_HT40_15,
        ALL_TARGET_HT40_0_8_16_24,
        ALL_TARGET_HT40_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT40_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT40_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT40_20,
        ALL_TARGET_HT40_21,
        ALL_TARGET_HT40_22,
        ALL_TARGET_HT40_23,
        ALL_TARGET_HT40_0_8_16_24,
        ALL_TARGET_HT40_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT40_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT40_1_3_9_11_17_19_25_27,
        ALL_TARGET_HT40_28,
        ALL_TARGET_HT40_29,
        ALL_TARGET_HT40_30,
        ALL_TARGET_HT40_31
    };

static inline void
ar9300_init_rate_txpower_ht(struct ath_hal *ah, const HAL_RATE_TABLE *rt,
                        bool is40,
                        u_int8_t rates_array[],
                        int rt_ss_offset, int rt_ds_offset,int rt_ts_offset,
                        int rt_qs_offset, u_int8_t chainmask)
{

    struct ath_hal_9300 *ahp = AH9300(ah);
    int i, j;
    u_int8_t mcs_index = 0;


    for (i = rt_ss_offset; i < rt_ss_offset + AR9300_NUM_HT_SS_RATES; i++) {
        /* Get the correct MCS rate to Power table Index */
        j = (is40) ? mcs_rate_2_pwr_idx_ht40[mcs_index] :
                          mcs_rate_2_pwr_idx_ht20[mcs_index];
        switch (chainmask) {
        case OSPREY_1_CHAINMASK:
            ahp->txpower[i][0] = rates_array[j];
            break;
        case OSPREY_2LOHI_CHAINMASK:
        case OSPREY_2LOMID_CHAINMASK:
            ahp->txpower[i][1] = rates_array[j];
            break;
        case OSPREY_3_CHAINMASK:
            ahp->txpower[i][2] = rates_array[j];
            break;
        case JET_4_CHAINMASK:
            ahp->txpower[i][3] = rates_array[j];
            break;
        default:
            HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                     __func__, chainmask);
            break;
        }
        mcs_index++;
    }

    for (i = rt_ds_offset; i < rt_ds_offset + AR9300_NUM_HT_DS_RATES; i++) {
        /* Get the correct MCS rate to Power table Index */
        j = (is40) ? mcs_rate_2_pwr_idx_ht40[mcs_index] :
                                       mcs_rate_2_pwr_idx_ht20[mcs_index];
        switch (chainmask) {
        case OSPREY_1_CHAINMASK:
            ahp->txpower[i][0] = rates_array[j];
            break;
        case OSPREY_2LOHI_CHAINMASK:
        case OSPREY_2LOMID_CHAINMASK:
            ahp->txpower[i][1] = rates_array[j];
            break;
        case OSPREY_3_CHAINMASK:
            ahp->txpower[i][2] = rates_array[j];
            break;
        case JET_4_CHAINMASK:
            ahp->txpower[i][3] = rates_array[j];
            break;
        default:
            HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                         __func__, chainmask);
            break;
        }
        mcs_index++;
    }

    for (i = rt_ts_offset; i < rt_ts_offset + AR9300_NUM_HT_TS_RATES; i++) {
        /* Get the correct MCS rate to Power table Index */
        j = (is40) ? mcs_rate_2_pwr_idx_ht40[mcs_index] :
                                  mcs_rate_2_pwr_idx_ht20[mcs_index];
        switch (chainmask) {
        case OSPREY_1_CHAINMASK:
            ahp->txpower[i][0] = rates_array[j];
            break;
        case OSPREY_2LOHI_CHAINMASK:
        case OSPREY_2LOMID_CHAINMASK:
            ahp->txpower[i][1] = rates_array[j];
            break;
        case OSPREY_3_CHAINMASK:
            ahp->txpower[i][2] = rates_array[j];
            break;
        case JET_4_CHAINMASK:
            ahp->txpower[i][3] = rates_array[j];
            break;
        default:
            HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                 __func__, chainmask);
            break;
        }
        mcs_index++;
    }

    if (ath_hal_Jet(ah)) {
        for (i = rt_qs_offset; i < rt_qs_offset + AR9300_NUM_HT_QS_RATES; i++) {
            /* Get the correct MCS rate to Power table Index */
            j = (is40) ? mcs_rate_2_pwr_idx_ht40[mcs_index] :
                mcs_rate_2_pwr_idx_ht20[mcs_index];
            switch (chainmask) {
                case OSPREY_1_CHAINMASK:
                    ahp->txpower[i][0] = rates_array[j];
                    break;
                case OSPREY_2LOHI_CHAINMASK:
                case OSPREY_2LOMID_CHAINMASK:
                    ahp->txpower[i][1] = rates_array[j];
                    break;
                case OSPREY_3_CHAINMASK:
                    ahp->txpower[i][2] = rates_array[j];
                    break;
                case JET_4_CHAINMASK:
                    ahp->txpower[i][3] = rates_array[j];
                    break;
                default:
                    HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                            __func__, chainmask);
                    break;
            }
            mcs_index++;
        }
    }
}

#ifdef  ATH_SUPPORT_TxBF
static inline void
ar9300_init_rate_txpower_txbf(struct ath_hal *ah, const HAL_RATE_TABLE *rt,
                        bool is40,
                        int rt_ss_offset, int rt_ds_offset, int rt_ts_offset,
                        int rt_qs_offset, u_int8_t chainmask)
{

    struct ath_hal_9300 *ahp = AH9300(ah);
    int i;
    int16_t twice_array_gain, txbf_power = 0;
    u_int8_t mcs_index = 0;
    

    /*
     *  Adjust the upper limit for TxBF factoring in the array gain 
     *  The same rule applies for all regulatory domains (FCC, ETSI and MKK)
     */
    switch (chainmask) {
    case OSPREY_1_CHAINMASK:
        txbf_power = ahp->upper_limit[0];
        break;
  
    case OSPREY_2LOHI_CHAINMASK:
    case OSPREY_2LOMID_CHAINMASK:
        twice_array_gain =
            (ahp->twice_antenna_gain >= ahp->twice_antenna_reduction)?
            -(AR9300_TXBF_2TX_ARRAY_GAIN) :
            ((int16_t)AH_MIN((ahp->twice_antenna_reduction -
            (ahp->twice_antenna_gain + AR9300_TXBF_2TX_ARRAY_GAIN)), 0));
            txbf_power = ahp->upper_limit[1] + twice_array_gain;
            txbf_power = AH_MAX(0, txbf_power);
        break;
        
    case OSPREY_3_CHAINMASK:
        twice_array_gain =
            (ahp->twice_antenna_gain >= ahp->twice_antenna_reduction)?
            -(AR9300_TXBF_3TX_ARRAY_GAIN) :
            ((int16_t)AH_MIN((ahp->twice_antenna_reduction -
            (ahp->twice_antenna_gain + AR9300_TXBF_3TX_ARRAY_GAIN)), 0));
        txbf_power = ahp->upper_limit[2] + twice_array_gain;
        txbf_power = AH_MAX(0, txbf_power);
        break;

    case JET_4_CHAINMASK:
        twice_array_gain =
            (ahp->twice_antenna_gain >= ahp->twice_antenna_reduction)?
            -(AR9300_TXBF_4TX_ARRAY_GAIN) :
            ((int16_t)AH_MIN((ahp->twice_antenna_reduction -
            (ahp->twice_antenna_gain + AR9300_TXBF_4TX_ARRAY_GAIN)), 0));
        txbf_power = ahp->upper_limit[3] + twice_array_gain;
        txbf_power = AH_MAX(0, txbf_power);
        break;

    default:
        HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                 __func__, chainmask);
        break;
    }

    for (i = rt_ss_offset; i < rt_ss_offset + AR9300_NUM_HT_SS_RATES; i++) {
        switch (chainmask) {
        case OSPREY_1_CHAINMASK:
            ahp->txpower_txbf[i][0] = ahp->txpower[i][0];
            break;

        case OSPREY_2LOHI_CHAINMASK:
        case OSPREY_2LOMID_CHAINMASK:
            ahp->txpower_txbf[i][1] = ahp->txpower[i][1];
            /* 2 TX/1 stream  TxBF gain adjustment */
            if (ahp->txpower_txbf[i][1] > txbf_power){
                ahp->txpower_txbf[i][1] = txbf_power;
            } 
            break;
        case OSPREY_3_CHAINMASK:
            /* 3 TX/1 stream  TxBF gain adjustment */
            ahp->txpower_txbf[i][2] = ahp->txpower[i][2];
            if (ahp->txpower_txbf[i][2] > txbf_power){
                ahp->txpower_txbf[i][2] = txbf_power;
            }
            break;
        case JET_4_CHAINMASK:
            /* 4 TX/1 stream  TxBF gain adjustment */
            ahp->txpower_txbf[i][3] = ahp->txpower[i][3];
            if (ahp->txpower_txbf[i][3] > txbf_power){
                ahp->txpower_txbf[i][3] = txbf_power;
            }
            break;
        default:
            HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                     __func__, chainmask);
            break;
        }
        mcs_index++;
    }

    for (i = rt_ds_offset; i < rt_ds_offset + AR9300_NUM_HT_DS_RATES; i++) {
        switch (chainmask) {
        case OSPREY_1_CHAINMASK:
            ahp->txpower_txbf[i][0] = ahp->txpower[i][0];
            break;
        case OSPREY_2LOHI_CHAINMASK:
        case OSPREY_2LOMID_CHAINMASK:
            ahp->txpower_txbf[i][1] = ahp->txpower[i][1];
            break;
        case OSPREY_3_CHAINMASK:
            ahp->txpower_txbf[i][2] = ahp->txpower[i][2];
            /* 3 TX/2 stream  TxBF gain adjustment */
            if (ahp->txpower_txbf[i][2] > txbf_power){
                ahp->txpower_txbf[i][2] = txbf_power;
            } 
            break;
        case JET_4_CHAINMASK:
            ahp->txpower_txbf[i][3] = ahp->txpower[i][3];
            /* 4 TX/2 stream  TxBF gain adjustment */
            if (ahp->txpower_txbf[i][3] > txbf_power){
                ahp->txpower_txbf[i][3] = txbf_power;
            }
            break;
        default:
            HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                 __func__, chainmask);
            break;
        }
        mcs_index++;
    }

    for (i = rt_ts_offset; i < rt_ts_offset + AR9300_NUM_HT_TS_RATES; i++) {
        switch (chainmask) {
            case OSPREY_1_CHAINMASK:
                ahp->txpower_txbf[i][0] = ahp->txpower[i][0];
            break;
            case OSPREY_2LOHI_CHAINMASK:
            case OSPREY_2LOMID_CHAINMASK:
                ahp->txpower_txbf[i][1] = ahp->txpower[i][1];
            break;
            case OSPREY_3_CHAINMASK:
                ahp->txpower_txbf[i][2] = ahp->txpower[i][2];
            break;
            case JET_4_CHAINMASK:
                ahp->txpower_txbf[i][3] = ahp->txpower[i][3];
            break;
            default:
                HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                     __func__, chainmask);
            break;
        }
        mcs_index++;
    }

    if (ath_hal_Jet(ah)) {
        for (i = rt_qs_offset; i < rt_qs_offset + AR9300_NUM_HT_QS_RATES; i++) {
            switch (chainmask) {
                case OSPREY_1_CHAINMASK:
                    ahp->txpower_txbf[i][0] = ahp->txpower[i][0];
                    break;
                case OSPREY_2LOHI_CHAINMASK:
                case OSPREY_2LOMID_CHAINMASK:
                    ahp->txpower_txbf[i][1] = ahp->txpower[i][1];
                    break;
                case OSPREY_3_CHAINMASK:
                    ahp->txpower_txbf[i][2] = ahp->txpower[i][2];
                    break;
                case JET_4_CHAINMASK:
                    ahp->txpower_txbf[i][3] = ahp->txpower[i][3];
                    break;
                default:
                    HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                            __func__, chainmask);
                    break;
            }
            mcs_index++;
        }
    }
}

#endif /* ATH_SUPPORT_TxBF */

static inline void
ar9300_init_rate_txpower_stbc(struct ath_hal *ah, const HAL_RATE_TABLE *rt,
                        bool is40,
                        int rt_ss_offset, int rt_ds_offset, int rt_ts_offset,
                        int rt_qs_offset, u_int8_t chainmask)
{

    struct ath_hal_9300 *ahp = AH9300(ah);
    int i;
    int16_t twice_array_gain, stbc_power = 0;
    u_int8_t mcs_index = 0;

    /* Upper Limit with STBC */
    switch (chainmask) {
    case OSPREY_1_CHAINMASK:
        stbc_power = ahp->upper_limit[0];
        break;
    case OSPREY_2LOHI_CHAINMASK:
    case OSPREY_2LOMID_CHAINMASK:
        stbc_power = ahp->upper_limit[1];
        break;
    case OSPREY_3_CHAINMASK:
        stbc_power = ahp->upper_limit[2];
        /* Only FCC requires that we back off with 3 transmit chains */
        if (is_reg_dmn_fcc(ahp->reg_dmn)) {
            twice_array_gain =
                (ahp->twice_antenna_gain >= ahp->twice_antenna_reduction)?
                -(AR9300_STBC_3TX_ARRAY_GAIN) :
                ((int16_t)AH_MIN((ahp->twice_antenna_reduction -
                (ahp->twice_antenna_gain + AR9300_STBC_3TX_ARRAY_GAIN)), 0));
            stbc_power = ahp->upper_limit[2] + twice_array_gain;
            stbc_power = AH_MAX(0, stbc_power);
        }
        break;
    case JET_4_CHAINMASK:
        stbc_power = ahp->upper_limit[3];
        /* Only FCC requires that we back off with 4 transmit chains */
        if (is_reg_dmn_fcc(ahp->reg_dmn)) {
            twice_array_gain =
                (ahp->twice_antenna_gain >= ahp->twice_antenna_reduction)?
                -(AR9300_STBC_4TX_ARRAY_GAIN) :
                ((int16_t)AH_MIN((ahp->twice_antenna_reduction -
                (ahp->twice_antenna_gain + AR9300_STBC_4TX_ARRAY_GAIN)), 0));
            stbc_power = ahp->upper_limit[3] + twice_array_gain;
            stbc_power = AH_MAX(0, stbc_power);
        }
        break;

    default:
        HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                 __func__, chainmask);
        break;
    }


    for (i = rt_ss_offset; i < rt_ss_offset + AR9300_NUM_HT_SS_RATES; i++) {
        switch (chainmask) {
        case OSPREY_1_CHAINMASK:
            ahp->txpower_stbc[i][0] = ahp->txpower[i][0];
            break;
        case OSPREY_2LOHI_CHAINMASK:
        case OSPREY_2LOMID_CHAINMASK:
            ahp->txpower_stbc[i][1] = ahp->txpower[i][1];
            break;
        case OSPREY_3_CHAINMASK:
            ahp->txpower_stbc[i][2] = ahp->txpower[i][2];
            /* 3 TX/1 stream  STBC gain adjustment */
            if (ahp->txpower_stbc[i][2] > stbc_power){
                ahp->txpower_stbc[i][2] = stbc_power;
            }
            break;
        case JET_4_CHAINMASK:
            ahp->txpower_stbc[i][3] = ahp->txpower[i][3];
            /* 4 TX/1 stream  STBC gain adjustment */
            if (ahp->txpower_stbc[i][3] > stbc_power){
                ahp->txpower_stbc[i][3] = stbc_power;
            }
            break;
        default:
            HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                     __func__, chainmask);
            break;
        }
        mcs_index++;
    }

    for (i = rt_ds_offset; i < rt_ds_offset + AR9300_NUM_HT_DS_RATES; i++) {
        switch (chainmask) {
        case OSPREY_1_CHAINMASK:
            ahp->txpower_stbc[i][0] = ahp->txpower[i][0];
            break;
        case OSPREY_2LOHI_CHAINMASK:
        case OSPREY_2LOMID_CHAINMASK:
            ahp->txpower_stbc[i][1] = ahp->txpower[i][1];
            break;
        case OSPREY_3_CHAINMASK:
            ahp->txpower_stbc[i][2] = ahp->txpower[i][2];
            /* 3 TX/2 stream  STBC gain adjustment */
            if (ahp->txpower_stbc[i][2] > stbc_power){
                ahp->txpower_stbc[i][2] = stbc_power;
	    }
            break;
        case JET_4_CHAINMASK:
            ahp->txpower_stbc[i][3] = ahp->txpower[i][3];
            /* 4 TX/2 stream  STBC gain adjustment */
            if (ahp->txpower_stbc[i][3] > stbc_power){
                ahp->txpower_stbc[i][3] = stbc_power;
	    }
            break;
        default:
            HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                     __func__, chainmask);
            break;
        }
        mcs_index++;
    }

    for (i = rt_ts_offset; i < rt_ts_offset + AR9300_NUM_HT_TS_RATES; i++) {
        switch (chainmask) {
        case OSPREY_1_CHAINMASK:
            ahp->txpower_stbc[i][0] = ahp->txpower[i][0];
            break;
        case OSPREY_2LOHI_CHAINMASK:
        case OSPREY_2LOMID_CHAINMASK:
            ahp->txpower_stbc[i][1] = ahp->txpower[i][1];
            break;
        case OSPREY_3_CHAINMASK:
            ahp->txpower_stbc[i][2] = ahp->txpower[i][2];
            break;
        case JET_4_CHAINMASK:
            ahp->txpower_stbc[i][3] = ahp->txpower[i][3];
            break;
        default:
            HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                     __func__, chainmask);
            break;
        }
        mcs_index++;
    }

    if (ath_hal_Jet(ah)) {
        for (i = rt_qs_offset; i < rt_qs_offset + AR9300_NUM_HT_QS_RATES; i++) {
            switch (chainmask) {
                case OSPREY_1_CHAINMASK:
                    ahp->txpower_stbc[i][0] = ahp->txpower[i][0];
                    break;
                case OSPREY_2LOHI_CHAINMASK:
                case OSPREY_2LOMID_CHAINMASK:
                    ahp->txpower_stbc[i][1] = ahp->txpower[i][1];
                    break;
                case OSPREY_3_CHAINMASK:
                    ahp->txpower_stbc[i][2] = ahp->txpower[i][2];
                    break;
                case JET_4_CHAINMASK:
                    ahp->txpower_stbc[i][3] = ahp->txpower[i][3];
                    break;
                default:
                    HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                            __func__, chainmask);
                    break;
            }
            mcs_index++;
        }
    }
    return;
}

static inline void
ar9300_adjust_rate_txpower_cdd(struct ath_hal *ah, const HAL_RATE_TABLE *rt,
                        bool is40,
                        int rt_ss_offset, int rt_ds_offset, int rt_ts_offset,
                        int rt_qs_offset, u_int8_t chainmask)
{

    struct ath_hal_9300 *ahp = AH9300(ah);
    int i;
    int16_t twice_array_gain, cdd_power = 0;
    u_int8_t mcs_index = 0;

    /*
     *  Adjust the upper limit for CDD factoring in the array gain .
     *  The array gain is the same as TxBF, hence reuse the same defines. 
     */
    switch (chainmask) {
    case OSPREY_1_CHAINMASK:
        cdd_power = ahp->upper_limit[0];
        break;
  
    case OSPREY_2LOHI_CHAINMASK:
    case OSPREY_2LOMID_CHAINMASK:
        twice_array_gain =
            (ahp->twice_antenna_gain >= ahp->twice_antenna_reduction)?
            -(AR9300_TXBF_2TX_ARRAY_GAIN) :
            ((int16_t)AH_MIN((ahp->twice_antenna_reduction -
            (ahp->twice_antenna_gain + AR9300_TXBF_2TX_ARRAY_GAIN)), 0));
        cdd_power = ahp->upper_limit[1] + twice_array_gain;
        cdd_power = AH_MAX(0, cdd_power);
        break;
        
    case OSPREY_3_CHAINMASK:
        twice_array_gain =
            (ahp->twice_antenna_gain >= ahp->twice_antenna_reduction)?
            -(AR9300_TXBF_3TX_ARRAY_GAIN) :
            ((int16_t)AH_MIN((ahp->twice_antenna_reduction -
            (ahp->twice_antenna_gain + AR9300_TXBF_3TX_ARRAY_GAIN)), 0));
        cdd_power = ahp->upper_limit[2] + twice_array_gain;
        cdd_power = AH_MAX(0, cdd_power);
        break;

    case JET_4_CHAINMASK:
        twice_array_gain =
            (ahp->twice_antenna_gain >= ahp->twice_antenna_reduction)?
            -(AR9300_TXBF_4TX_ARRAY_GAIN) :
            ((int16_t)AH_MIN((ahp->twice_antenna_reduction -
            (ahp->twice_antenna_gain + AR9300_TXBF_4TX_ARRAY_GAIN)), 0));
        cdd_power = ahp->upper_limit[3] + twice_array_gain;
        cdd_power = AH_MAX(0, cdd_power);
        break;

    default:
        HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                     __func__, chainmask);
        break;
    }


    for (i = rt_ss_offset; i < rt_ss_offset + AR9300_NUM_HT_SS_RATES; i++) {
        switch (chainmask) {
        case OSPREY_1_CHAINMASK:
            break;

        case OSPREY_2LOHI_CHAINMASK:
        case OSPREY_2LOMID_CHAINMASK:
            /* 2 TX/1 stream  CDD gain adjustment */
            if (ahp->txpower[i][1] > cdd_power){
                ahp->txpower[i][1] = cdd_power;
            } 
            break;
        case OSPREY_3_CHAINMASK:
            /* 3 TX/1 stream  CDD gain adjustment */
            if (ahp->txpower[i][2] > cdd_power){
                ahp->txpower[i][2] = cdd_power;
            }
            break;
        case JET_4_CHAINMASK:
            /* 4 TX/1 stream  CDD gain adjustment */
            if (ahp->txpower[i][3] > cdd_power){
                ahp->txpower[i][3] = cdd_power;
            }
            break;
        default:
            HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                     __func__, chainmask);
            break;
        }
        mcs_index++;
    }

    for (i = rt_ds_offset; i < rt_ds_offset + AR9300_NUM_HT_DS_RATES; i++) {
        switch (chainmask) {
        case OSPREY_1_CHAINMASK:
        case OSPREY_2LOHI_CHAINMASK:
        case OSPREY_2LOMID_CHAINMASK:
            break;
        case OSPREY_3_CHAINMASK:
        /* 3 TX/2 stream  TxBF gain adjustment */
            if (ahp->txpower[i][2] > cdd_power){
                ahp->txpower[i][2] = cdd_power;
            } 
            break;
        case JET_4_CHAINMASK:
        /* 4 TX/2 stream  TxBF gain adjustment */
            if (ahp->txpower[i][3] > cdd_power){
                ahp->txpower[i][3] = cdd_power;
            }
            break;
        default:
            HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                 __func__, chainmask);
            break;
        }
        mcs_index++;
    }

    for (i = rt_ts_offset; i < rt_ts_offset + AR9300_NUM_HT_TS_RATES; i++) {
        switch (chainmask) {
        case OSPREY_1_CHAINMASK:
        case OSPREY_2LOHI_CHAINMASK:
        case OSPREY_2LOMID_CHAINMASK:
            break;
        case OSPREY_3_CHAINMASK:
        /* 3 TX/2 stream  TxBF gain adjustment */
            if (ahp->txpower[i][2] > cdd_power){
                ahp->txpower[i][2] = cdd_power;
            }
            break;
        case JET_4_CHAINMASK:
        /* 4 TX/2 stream  TxBF gain adjustment */
            if (ahp->txpower[i][3] > cdd_power){
                ahp->txpower[i][3] = cdd_power;
            }
            break;
        default:
            HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                 __func__, chainmask);
            break;
        }
        mcs_index++;
    }

    if (ath_hal_Jet(ah)) {
        for (i = rt_qs_offset; i < rt_qs_offset + AR9300_NUM_HT_QS_RATES; i++) {
            switch (chainmask) {
                case OSPREY_1_CHAINMASK:
                case OSPREY_2LOHI_CHAINMASK:
                case OSPREY_2LOMID_CHAINMASK:
                    break;
                case OSPREY_3_CHAINMASK:
                    /* 3 TX/2 stream  TxBF gain adjustment */
                    if (ahp->txpower[i][2] > cdd_power){
                        ahp->txpower[i][2] = cdd_power;
                    }
                    break;
                case JET_4_CHAINMASK:
                    /* 4 TX/2 stream  TxBF gain adjustment */
                    if (ahp->txpower[i][3] > cdd_power){
                        ahp->txpower[i][3] = cdd_power;
                    }
                    break;
                default:
                    HDPRINTF(ah, HAL_DBG_POWER_MGMT, "%s: invalid chainmask 0x%x\n",
                            __func__, chainmask);
                    break;
            }
            mcs_index++;
        }
    }
    return;

}

void ar9300_disp_tpc_tables(struct ath_hal *ah)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    HAL_CHANNEL_INTERNAL *chan = AH_PRIVATE(ah)->ah_curchan;
    u_int mode = ath_hal_get_curmode(ah, chan);
    const HAL_RATE_TABLE *rt;
    int i, j, nchain = 0;

    /* Check whether TPC is enabled */
    if (!AH_PRIVATE(ah)->ah_config.ath_hal_desc_tpc) {
        ath_hal_printf(ah, "\n TPC Register method in use\n");
        return;
    }
    
    rt = ar9300_get_rate_table(ah, mode);
    HALASSERT(rt != NULL);

    ath_hal_printf(ah, "\n===TARGET POWER TABLE===\n");
    for (j = 0 ; j < ar9300_get_ntxchains(ahp->ah_tx_chainmask) ; j++ ) {
        for (i = 0; i < rt->rateCount; i++) {
            int16_t txpower[AR9300_MAX_CHAINS]; 
            txpower[j] = ahp->txpower[i][j];
            ath_hal_printf(ah, " Index[%2d] Rate[0x%02x] %6d kbps "
                       "Power (%d Chain) [%2d.%1d dBm]\n",
                       i, rt->info[i].rate_code, rt->info[i].rateKbps,
                       j + 1, txpower[j] / 2, txpower[j]%2 * 5);
        }
    }
    ath_hal_printf(ah, "\n");

#ifdef  ATH_SUPPORT_TxBF
    ath_hal_printf(ah, "\n\n===TARGET POWER TABLE with TxBF===\n");
    for (j = 0 ; j < ar9300_get_ntxchains(ahp->ah_tx_chainmask) ; j++ ) {
        nchain = (j + 1);
        for (i = 0; i < rt->rateCount; i++) {
            int16_t txpower[AR9300_MAX_CHAINS]; 
            txpower[j] = ahp->txpower_txbf[i][j];

            /* Do not display invalid configurations */
            if ((rt->info[i].rate_code < AR9300_MCS0_RATE_CODE) ||
                (rt->info[i].rate_code > AR9300_MCS31_RATE_CODE) ||
                ar9300_invalid_txbf_cfg(nchain, rt->info[i].rate_code) == true) {
                continue;
            }

            if ((is_reg_dmn_etsi(ahp->reg_dmn) ||
                 is_reg_dmn_mkk(ahp->reg_dmn)) && 
                ((mode == HAL_MODE_11NA_HT20) ||
                 (mode == HAL_MODE_11NA_HT40PLUS) ||
                 (mode == HAL_MODE_11NA_HT40MINUS) ||
                 (mode == HAL_MODE_11NG_HT20) ||
                 (mode == HAL_MODE_11NG_HT40PLUS) ||
                 (mode == HAL_MODE_11NG_HT40MINUS))){
                int txbf_disable_flag = 0;
                if ((ahp->txpower[i][j] == ahp->upper_limit[j]) ||
                    (ar9300_invalid_txbf_cfg(nchain, rt->info[i].rate_code)
                                                        == true)){
                    txbf_disable_flag = 1; 
                }
                ath_hal_printf(ah, " Index[%2d] Rate[0x%02x] %6d kbps "
                       "Power (%d Chain) [%2d.%1d dBm] TxBF=%3s\n",
                       i, rt->info[i].rate_code, rt->info[i].rateKbps,
                       nchain, txpower[j] / 2, txpower[j]%2 * 5,
                       (txbf_disable_flag?"OFF":"ON"));
            } else {
                ath_hal_printf(ah, " Index[%2d] Rate[0x%02x] %6d kbps "
                           "Power (%d Chain) [%2d.%1d dBm]\n",
                           i, rt->info[i].rate_code, rt->info[i].rateKbps,
                           nchain, txpower[j] / 2, txpower[j]%2 * 5);
            }
        }
    }
    ath_hal_printf(ah, "\n");
#endif

    ath_hal_printf(ah, "\n\n===TARGET POWER TABLE with STBC===\n");
    for ( j = 0 ; j < ar9300_get_ntxchains(ahp->ah_tx_chainmask) ; j++ ) {
        nchain = (j + 1);
        for (i = 0; i < rt->rateCount; i++) {
            int16_t txpower[AR9300_MAX_CHAINS]; 
            txpower[j] = ahp->txpower_stbc[i][j];

            /* Do not display invalid configurations */
            if ((rt->info[i].rate_code < AR9300_MCS0_RATE_CODE) ||
                (rt->info[i].rate_code > AR9300_MCS31_RATE_CODE) ||
                ar9300_invalid_stbc_cfg(nchain, rt->info[i].rate_code) == true) {
                continue;
            }

            ath_hal_printf(ah, " Index[%2d] Rate[0x%02x] %6d kbps "
                       "Power (%d Chain) [%2d.%1d dBm]\n",
                       i, rt->info[i].rate_code, rt->info[i].rateKbps,
                       nchain, txpower[j] / 2, txpower[j]%2 * 5);
        }
    }
    ath_hal_printf(ah, "\n");
}

/*
 * The followings are customer specific APIs for querying power limit.
 * Power limit is based on regulatory domain, chipset, and transmission rate.
 * Here we only consider EEPROM values, no array gain/CTL considered here.
 */

struct rate_power_tbl {
    u_int8_t    rateIdx;        /* rate index in the rate table */
    u_int32_t   rateKbps;       /* transfer rate in kbs */
    u_int8_t    rateCode;      /* rate for h/w descriptors */
    u_int8_t    txbf:   1,      /* txbf eligible */
                stbc:   1,      /* stbc eligible */
                chain1: 1,      /* one-chain eligible */
                chain2: 1,      /* two-chain eligible */
                chain3: 1,      /* three-chain eligible */
                chain4: 1;      /* Four-chain eligible */
    int16_t     txpower[AR9300_MAX_CHAINS];     /* txpower for different chainmasks */
#ifdef  ATH_SUPPORT_TxBF
    int16_t     txpower_txbf[AR9300_MAX_CHAINS];
#endif
    int16_t     txpower_stbc[AR9300_MAX_CHAINS];
};

u_int8_t *ar9300_get_tpc_tables(struct ath_hal *ah, u_int8_t *ratecount)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    HAL_CHANNEL_INTERNAL *chan = AH_PRIVATE(ah)->ah_curchan;
    u_int mode = ath_hal_get_curmode(ah, chan);
    const HAL_RATE_TABLE *rt;
    u_int8_t *data;
    struct rate_power_tbl *table;
    int i, j, nchain = 0;

    /* Check whether TPC is enabled */
    if (!AH_PRIVATE(ah)->ah_config.ath_hal_desc_tpc) {
        ath_hal_printf(ah, "\n TPC Register method in use\n");
        return NULL;
    }
    
    rt = ar9300_get_rate_table(ah, mode);
    HALASSERT(rt != NULL);

    data = (u_int8_t *)ath_hal_malloc(ah,
                       rt->rateCount * sizeof(struct rate_power_tbl));
    if (data == NULL)
        return NULL;

    OS_MEMZERO(data, rt->rateCount * sizeof(struct rate_power_tbl));
    /* return the actual rate count */
    *ratecount = rt->rateCount;
    table = (struct rate_power_tbl *)&data[0];


    for (j = 0 ; j < ar9300_get_ntxchains(ahp->ah_tx_chainmask) ; j++ ) {
        for (i = 0; i < rt->rateCount; i++) {
            table[i].rateIdx = i;
            table[i].rateCode = rt->info[i].rate_code;
            table[i].rateKbps = rt->info[i].rateKbps;
            switch (j) {
            case 0:
                table[i].chain1 = rt->info[i].rate_code <= 0x87 ? 1 : 0;
                break;
            case 1:
                table[i].chain2 = rt->info[i].rate_code <= 0x8f ? 1 : 0;
                break;
            case 2:
                table[i].chain3 = rt->info[i].rate_code <= 0x97 ? 1 : 0;
                break;
            case 3:
                table[i].chain4 = 1;
                break;
            default:
                break;
            }
            if ((j == 0 && table[i].chain1) ||
                (j == 1 && table[i].chain2) ||
                (j == 2 && table[i].chain3) ||
                (j == 3 && table[i].chain4))
                table[i].txpower[j] = ahp->txpower[i][j];
        }
    }

#ifdef  ATH_SUPPORT_TxBF
    for (j = 0 ; j < ar9300_get_ntxchains(ahp->ah_tx_chainmask) ; j++ ) {
        nchain = (j + 1);
        for (i = 0; i < rt->rateCount; i++) {

            /* Do not display invalid configurations */
            if ((rt->info[i].rate_code < AR9300_MCS0_RATE_CODE) ||
                (rt->info[i].rate_code > AR9300_MCS31_RATE_CODE) ||
                ar9300_invalid_txbf_cfg(nchain, rt->info[i].rate_code) == true) {
                continue;
            }

            if ((is_reg_dmn_etsi(ahp->reg_dmn) ||
                 is_reg_dmn_mkk(ahp->reg_dmn)) && 
                ((mode == HAL_MODE_11NA_HT20) ||
                 (mode == HAL_MODE_11NA_HT40PLUS) ||
                 (mode == HAL_MODE_11NA_HT40MINUS) ||
                 (mode == HAL_MODE_11NG_HT20) ||
                 (mode == HAL_MODE_11NG_HT40PLUS) ||
                 (mode == HAL_MODE_11NG_HT40MINUS))){
                int txbf_disable_flag = 0;
                if ((ahp->txpower[i][j] == ahp->upper_limit[j]) ||
                    (ar9300_invalid_txbf_cfg(j, rt->info[i].rate_code)
                                                        == true)){
                    txbf_disable_flag = 1; 
                }
                if (!txbf_disable_flag) {
                    table[i].txbf = 1;
                    table[i].txpower_txbf[j] = ahp->txpower_txbf[i][j];
                }
            } else {
                table[i].txbf = 1;
                table[i].txpower_txbf[j] = ahp->txpower_txbf[i][j];
            }
        }
    }
#endif

    for ( j = 0 ; j < ar9300_get_ntxchains(ahp->ah_tx_chainmask) ; j++ ) {
        nchain = (j + 1);
        for (i = 0; i < rt->rateCount; i++) {
            /* Do not display invalid configurations */
            if ((rt->info[i].rate_code < AR9300_MCS0_RATE_CODE) ||
                (rt->info[i].rate_code > AR9300_MCS31_RATE_CODE) ||
                ar9300_invalid_stbc_cfg(nchain, rt->info[i].rate_code) == true) {
                continue;
            }

            table[i].stbc = 1;
            table[i].txpower_stbc[j] = ahp->txpower_stbc[i][j];
        }
    }

    return data;
    /* the caller is responsible to free data */
}

#endif /* AH_SUPPORT_AR9300 */