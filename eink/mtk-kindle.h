/*****************************************************************************
 * Copyright (C) 2016 MediaTek Inc.
 *
 * ----
 *
 * This is <linux/hwtcon_ioctl_cmd.h>, last updated from the PW5 kernel for FW 5.14.1.1
 *
 * NOTE: Upstream kernels available here: https://www.amazon.com/gp/help/customer/display.html?nodeId=200203720
 *
 * - Frankensteined to play nice w/ MXCFB constants -- NiLuJe
 *
 * ----
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 *
 * Accelerometer Sensor Driver
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 *****************************************************************************/

#ifndef __HWTCON_IOCTL_CMD_H__
#define __HWTCON_IOCTL_CMD_H__

#include <linux/types.h>

// Pull in the mxcfb stuff, as a lot of stuff hasn't actually changed (kudos, lab126!)
#include "mxcfb-kindle.h"

/* EPDC_FLAG_xx */
// What's not new matches Zelda
/*
#define EPDC_FLAG_ENABLE_INVERSION     0x01
#define EPDC_FLAG_FORCE_MONOCHROME     0x02
#define EPDC_FLAG_USE_CMAP             0x04
#define EPDC_FLAG_USE_ALT_BUFFER       0x100
#define EPDC_FLAG_TEST_COLLISION       0x200
#define EPDC_FLAG_GROUP_UPDATE         0x400
*/
#define MTK_EPDC_FLAG_USE_DITHERING_Y1 0x2000
#define MTK_EPDC_FLAG_USE_DITHERING_Y4 0x4000
#define MTK_EPDC_FLAG_USE_REGAL        0x8000
#define MTK_EPDC_FLAG_ENABLE_SWIPE     0x10000

/* temperature use sensor. */
/*
#define TEMP_USE_AMBIENT 0x1000
#define TEMP_USE_AUTO    TEMP_USE_AMBIENT
*/

/*
#define UPDATE_MODE_PARTIAL 0x0
#define UPDATE_MODE_FULL    0x1
*/

/*
#define AUTO_UPDATE_MODE_REGION_MODE    0
#define AUTO_UPDATE_MODE_AUTOMATIC_MODE 1
*/

/* gray scale */
/*
#define GRAYSCALE_8BIT          0x1
#define GRAYSCALE_8BIT_INVERTED 0x2
#define GRAYSCALE_4BIT          0x3
#define GRAYSCALE_4BIT_INVERTED 0x4
*/

enum MTK_WAVEFORM_MODE_ENUM
{
	MTK_WAVEFORM_MODE_INIT          = 0,
	MTK_WAVEFORM_MODE_DU            = 1,
	MTK_WAVEFORM_MODE_GC16          = 2,
	MTK_WAVEFORM_MODE_GC16_FAST     = 2,
	MTK_WAVEFORM_MODE_GL16          = 3,
	MTK_WAVEFORM_MODE_GL16_FAST     = 3,
	MTK_WAVEFORM_MODE_GL4           = 3,
	MTK_WAVEFORM_MODE_GL16_INV      = 3,
	MTK_WAVEFORM_MODE_GLR16         = 4,
	MTK_WAVEFORM_MODE_REAGL         = 4,
	MTK_WAVEFORM_MODE_GLD16         = 5,
	MTK_WAVEFORM_MODE_REAGLD        = 5,
	MTK_WAVEFORM_MODE_A2            = 6,
	MTK_WAVEFORM_MODE_DU4           = 7,
	MTK_WAVEFORM_MODE_LAST          = 7,
	MTK_WAVEFORM_MODE_GCK16         = 8,
	MTK_WAVEFORM_MODE_GLKW16        = 9,
	MTK_WAVEFORM_MODE_GC16_PARTIAL  = 10,
	MTK_WAVEFORM_MODE_GCK16_PARTIAL = 11,
	MTK_WAVEFORM_MODE_DUNM          = 12,
	MTK_WAVEFORM_MODE_P2SW          = 13,
	// Matches MXCFB
	MTK_WAVEFORM_MODE_AUTO          = 257,
};

// Matches MXCFB
enum hwtcon_update_scheme
{
	MTK_UPDATE_SCHEME_QUEUE           = 1,
	MTK_UPDATE_SCHEME_QUEUE_AND_MERGE = 2,
};

/*
#define WAVEFORM_TYPE_4BIT 0x1
#define WAVEFORM_TYPE_5BIT (WAVEFORM_TYPE_4BIT << 1)
*/

/*
enum mxcfb_dithering_mode
{
	EPDC_FLAG_USE_DITHERING_PASSTHROUGH = 0x0,
	EPDC_FLAG_USE_DITHERING_FLOYD_STEINBERG,
	EPDC_FLAG_USE_DITHERING_ATKINSON,
	EPDC_FLAG_USE_DITHERING_ORDERED,
	EPDC_FLAG_USE_DITHERING_QUANT_ONLY,
	EPDC_FLAG_USE_DITHERING_MAX,
};
*/

/*
 * SWIPE_DOWN = 0,
 * SWIPE_UP = 1,
 * SWIPE_LEFT = 2,
 * SWIPE_RIGHT = 3,
*/
struct mxcfb_swipe_data
{
	__u32 direction; /* UP, DOWN, LEFT or RIGHT */
	__u32 steps;     /* number of swipe steps */
};

// Matches !Zelda
/*
struct mxcfb_waveform_modes
{
	// waveform mode index for WAVEFORM_MODE_INIT
	int mode_init;
	// waveform mode index for WAVEFORM_MODE_DU
	int mode_du;
	// waveform mode index for WAVEFORM_MODE_GC4
	int mode_gc4;
	// waveform mode index for WAVEFORM_MODE_GC8
	int mode_gc8;
	// waveform mode index for WAVEFORM_MODE_GC16
	int mode_gc16;
	// waveform mode index for WAVEFORM_MODE_GC16_FAST
	int mode_gc16_fast;
	// waveform mode index for WAVEFORM_MODE_GC32
	int mode_gc32;
	// waveform mode index for WAVEFORM_MODE_GL16
	int mode_gl16;
	// waveform mode index for WAVEFORM_MODE_GL16_FAST
	int mode_gl16_fast;
	// waveform mode index for WAVEFORM_MODE_A2
	int mode_a2;
	// waveform mode index for WAVEFORM_MODE_DU4
	int mode_du4;
	// waveform mode index for WAVEFORM_MODE_REAGL
	int mode_reagl;
	// waveform mode index for WAVEFORM_MODE_REAGLD
	int mode_reagld;
	// waveform mode index for WAVEFORM_MODE_GL16_INV
	int mode_gl16_inv;
	// waveform mode index for WAVEFORM_MODE_GL4
	int mode_gl4;
};
*/

/*
struct mxcfb_rect
{
	__u32 top;
	__u32 left;
	__u32 width;
	__u32 height;
};
*/

/*
struct mxcfb_update_marker_data
{
	__u32 update_marker;
	__u32 collision_test;
};
*/

/*
struct mxcfb_alt_buffer_data
{
	__u32             phys_addr;
	__u32             width;  // width of entire buffer
	__u32             height; // height of entire buffer
	// region within buffer to update
	struct mxcfb_rect alt_update_region;
};
*/

struct mxcfb_update_data_mtk
{
	struct mxcfb_rect            update_region;
	/* which waveform to use for the update, du, gc4, gc8 gc16 etc */
	__u32                        waveform_mode;
	__u32                        update_mode; /* full update or partial update */
	/* Unique number used by both application
	 * and driver to identify an update
	 */
	__u32                        update_marker;
	int                          temp;        /* For testing only, currently not use */
	unsigned int                 flags;       /* one or more EPDC_FLAGs defined above */
	int                          dither_mode; /* one of the dither modes defined above */
	int                          quant_bit;   /* number of quantization bits for dithering */
	/* alternative buffer for update
	 * if the current frame buffer is not used.
	 * Flag EPDC_FLAG_USE_ALT_BUFFER should be set in this case
	 */
	struct mxcfb_alt_buffer_data alt_buffer_data;
	struct mxcfb_swipe_data      swipe_data;
#if 1
	/* start: lab126 added for backward compatible */
	/*Lab126: Def bw waveform for hist analysis*/
	__u32 hist_bw_waveform_mode;
	/*Lab126: Def gray waveform for hist analysis*/
	__u32 hist_gray_waveform_mode;
	__u32 ts_pxp;  /*debugging purpose: pxp starting time*/
	__u32 ts_epdc; /*debugging purpose: EPDC starting time*/
		       /* end: lab126 added */
#endif
};

/*
struct mxcfb_nightmode_ctrl
{
	int disable;       // 1: disable; 0, enable
	int start;         // reduced to level for gck16
	int stride;        // back to original level gradually: default
	int current_level; // current brighness setting
};
*/

struct mxcfb_panel_info
{
	char wf_file_name[100];
	int  vcom_value;
	/* temperature */
	int  temp;
	/* temperature zone */
	int  temp_zone;
};

/* ioctl commds */
#define HWTCON_IOCTL_MAGIC_NUMBER 'F'

/* Set the mapping between waveform types and waveform mode index */
#define MXCFB_SET_WAVEFORM_MODES_MTK _IOW(HWTCON_IOCTL_MAGIC_NUMBER, 0x2B, struct mxcfb_waveform_modes)

/* Set the temperature for screen updates.
 * If temperature specified is TEMP_USE_AMBIENT,
 * use the temperature read from the temperature sensor.
 * Otherwise use the temperature specified
 */
#define MXCFB_SET_TEMPERATURE_MTK _IOW(HWTCON_IOCTL_MAGIC_NUMBER, 0x2C, int32_t)

#define MXCFB_SET_AUTO_UPDATE_MODE_MTK _IOW(HWTCON_IOCTL_MAGIC_NUMBER, 0x2D, __u32)

/* Get the temperature currently used for screen updates.
 * If the temperature set by command FB_SET_TEMPERATURE
 * is not equal to TEMP_USE_AMBIENT,
 * return that temperature value.
 * Otherwise, return the temperature read from the temperature sensor
 */
#define MXCFB_GET_TEMPERATURE_MTK _IOR(HWTCON_IOCTL_MAGIC_NUMBER, 0x38, int32_t)

/* Send update info to update the Eink panel display */
#define MXCFB_SEND_UPDATE_MTK _IOW(HWTCON_IOCTL_MAGIC_NUMBER, 0x2E, struct mxcfb_update_data_mtk)

/* Set the scheme that the FB driver should use in handling the FB_SEND_UPDATE.
 * If the scheme is set to UPDATE_SCHEME_QUEUE,
 * all FB_SEND_UPDATE requests should be queued up
 * and processed indiviually in FIFO order.
 * If the scheme is set to UPDATE_SCHEME_QUEUE_AND_MERGE,
 * the driver should try to merge as many as possible
 * those requests before sending them
 * to IMGSYS or MMSYS
 * if the flags in the mxcfb_update_data_mtk
 * structure of those requests are identical.
 */
#define MXCFB_SET_UPDATE_SCHEME_MTK _IOW(HWTCON_IOCTL_MAGIC_NUMBER, 0x32, __u32)

/* Wait until the specified send_update request
 * (specified by mxcfb_update_marker_data) is
 * submitted to HWTCON to display or timeout (5 seconds)
 */
#define MXCFB_WAIT_FOR_UPDATE_SUBMISSION_MTK _IOW(HWTCON_IOCTL_MAGIC_NUMBER, 0x37, __u32)

/* Wait until the specified send_update request
 * (specified by mxcfb_update_marker_data) is
 * already completed (Eink panel updated) or timeout (5 seconds)
 */
#define MXCFB_WAIT_FOR_UPDATE_COMPLETE_MTK _IOWR(HWTCON_IOCTL_MAGIC_NUMBER, 0x2F, struct mxcfb_update_marker_data)

/* Copy the content of the working buffer to user space */
#define MXCFB_GET_WORK_BUFFER_MTK _IOWR(HWTCON_IOCTL_MAGIC_NUMBER, 0x34, unsigned long)

/* Check if the waveform supports advanced algorithms.
 * If yes, return WAVEFORM_TYPE_5BIT
 * Otherwise WAVEFORM_TYPE_4BIT
 */
#define MXCFB_GET_WAVEFORM_TYPE_MTK _IOR(HWTCON_IOCTL_MAGIC_NUMBER, 0x39, __u32)

/* get EPD material type from dts */
#define MXCFB_GET_MATERIAL_TYPE_MTK _IOR(HWTCON_IOCTL_MAGIC_NUMBER, 0x3A, __u32)

/* Set the front light control data for night mode.
 * For each screen update in night mode,
 * the front light needs to be set to the "start"
 * brightness level before the update,
 * and then raised back to the "current_level" gradually
 * with increment of "stride" every milli second.
 */
#define MXCFB_SET_NIGHTMODE_MTK _IOR(HWTCON_IOCTL_MAGIC_NUMBER, 0x4A, struct mxcfb_nightmode_ctrl)

/* Set the power down delay so the driver won't shut down the HWTCON immediately
 * after all the updates are done.
 * Instead it will wait until the "DELAY" time has elapsed to skip the
 * powerdown and powerup sequences if an update comes before that.
 */
#define MXCFB_SET_PWRDOWN_DELAY_MTK _IOW(HWTCON_IOCTL_MAGIC_NUMBER, 0x30, int32_t)

/* Get the power down delay set in MXCFB_SET_PWRDOWN_DELAY command */
#define MXCFB_GET_PWRDOWN_DELAY_MTK _IOR(HWTCON_IOCTL_MAGIC_NUMBER, 0x31, int32_t)

/* Pause updating the screen.
 * Any MXCFB_SEND_UPDATE request will be discarded.
 */
#define MXCFB_SET_PAUSE_MTK _IOW(HWTCON_IOCTL_MAGIC_NUMBER, 0x33, __u32)

/* Resume updating the screen. */
#define MXCFB_SET_RESUME_MTK _IOW(HWTCON_IOCTL_MAGIC_NUMBER, 0x35, __u32)

/* Get the screen updating flag set by MXCFB_SET_PAUSE or MXCFB_SET_RESUME */
#define MXCFB_GET_PAUSE_MTK _IOW(HWTCON_IOCTL_MAGIC_NUMBER, 0x34, __u32)

#define MXCFB_GET_PANEL_INFO_MTK _IOR(HWTCON_IOCTL_MAGIC_NUMBER, 0x130, struct mxcfb_panel_info)

/* Lightbox (aka halftone pattern) feature */
#define MXCFB_SET_HALFTONE_MTK _IOW('F', 0x4B, struct mxcfb_halftone_data)
#define MAX_HALFTONE_REGION    2
struct mxcfb_halftone_data
{
	struct mxcfb_rect region[MAX_HALFTONE_REGION];
	/* Lightbox (aka Halftone Pattern) Mode: 0:None, */
	/* 1:Turn on and change to default checker size */
	/* Others: Custom, checker size = mode - 1 */
	int               halftone_mode;
};
/* Lightbox (aka halftone pattern) feature */
#define MXCFB_WAIT_FOR_ANY_UPDATE_COMPLETE_MTK _IOWR(HWTCON_IOCTL_MAGIC_NUMBER, 0x37, __u32)
#define MAX_NUM_PENDING_UPDATES                64

/* Flag used in MXCFB_WAIT_FOR_ANY_UPDATE_COMPLETE. Caller of ioctl MXCFB_WAIT_FOR_ANY_UPDATE_COMPLETE
    can set the first element of the u32 array to this value to query if the command is supported by the FB
   driver. If supported, the return value will be 0, otherwise, -EINVAL */

/*  Expand definition:
          Return 0 to indicate REAGL_FEATURE_0 is supported
          MXCFB_WAIT_FOR_ANY_UPDATE_COMPLETE is supported and and engne can apply REAGL waveform in case of collision,
          return 3, indicate REAGL_FEATURE_1 is supported, Number 3 also means 3rd generation platform, Bellatrix, which can do REAGL with collision ,
          otherwise, -EINVAL
  */
#define FLAG_CHECK      0xffffffff
#define REAGL_FEATURE_0 0x00
#define REAGL_FEATURE_1 0x01

#define MXCFB_SET_UPDATE_FLAGS_MTK _IOW(HWTCON_IOCTL_MAGIC_NUMBER, 0x3B, __u32)
#define MXCFB_GET_UPDATE_FLAGS_MTK _IOWR(HWTCON_IOCTL_MAGIC_NUMBER, 0x3C, __u32)

/* fast mode flags */
#define UPDATE_FLAGS_MASK_PARAM (0xFF << 24)

#define UPDATE_FLAGS_FAST_MODE           (0x80 << 24)
#define UPDATE_FLAGS_FAST_MODE_PARAM     (0xFF)
#define UPDATE_FLAGS_MODE_FAST_FLAG      1                                  /*< 0b0000000000000001 */
#define UPDATE_FLAGS_MODE_FAST_FLAG_INIT (UPDATE_FLAGS_MODE_FAST_FLAG << 1) /*< 0b0000000000000010 */
#define UPDATE_FLAGS_MODE_FAST_FLAG_STOP (UPDATE_FLAGS_MODE_FAST_FLAG << 2) /*< 0b0000000000000100 */
#define UPDATE_FLAGS_MODE_FAST_FLAG_PAN  (UPDATE_FLAGS_MODE_FAST_FLAG << 3) /*< 0b0000000000001000 */
#define UPDATE_FLAGS_MODE_FAST_FLAG_KB   (UPDATE_FLAGS_MODE_FAST_FLAG << 4) /*< 0b0000000000010000 */
#define UPDATE_FLAGS_MODE_FAST_FLAG_HL   (UPDATE_FLAGS_MODE_FAST_FLAG << 5) /*< 0b0000000000100000 */

#define MXC_UPDATE_FAST_MODE_FLAG 0x1

#endif /* __HWTCON_IOCTL_CMD_H__ */
