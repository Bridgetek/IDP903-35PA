/**
 * @file EVE_CoDl.h
 * @brief EVE's co-processor command
 *
 * The purpose of this header is to provide a simplified interface to display list instructions.
 * The functions do not match 1:1 with the display list instructions. Some instructions are combined
 * to simplify compatibility between platforms. (For example, BITMAP_SIZE and BITMAP_SIZE_H.)
 * All functions write to the display list through EVE_CoCmd_dl.
 *
 * If EVE_DL_OPTIMIZE is set to 1 in EVE_Config, these functions will ignore duplicate calls.
 * To bypass optmization, call `EVE_CoCmd_dl` directly (within a subroutine or saved context.)
 *
 * If ESD_DL_END_PRIMITIVE is set to 0 in EVE_Config, the END() instruction will be avoided.
 *
 * Compatibility:
 * - EVE_CoDl_vertexFormat and EVE_CoDl_vertex2f implement fallback functionality for
 *   VERTEX_FORMAT, VERTEX_TRANSLATE_X, and VERTEX_TRANSLATE_Y on FT80X series.
 * - EVE_CoDl_bitmapSize calls BITMAP_SIZE_H on FT81X series and higher.
 * - EVE_CoDl_bitmapLayout calls BITMAP_LAYOUT_H on FT81X series and higher.
 *
 * @author Bridgetek
 *
 * @date 2018
 *
 * MIT License
 *
 * Copyright (c) [2019] [Bridgetek Pte Ltd (BRTChip)]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef EVE_CO_DL__H
#define EVE_CO_DL__H

/*************
** INCLUDES **
*************/

#include "EVE_CoCmd.h"

/***********
** MARCOS **
***********/

#define EVE_VERTEX2F_MIN -16384L
#define EVE_VERTEX2F_MAX 16383L
#define EVE_VERTEX2II_MIN 0UL
#define EVE_VERTEX2II_MAX 511UL

/**********************
** GLOBAL PROTOTYPES **
**********************/
/**
 * @param phost Pointer to Hal context
 */
void EVE_CoDlImpl_resetDlState(EVE_HalContext *phost);
/**
 * @param phost Pointer to Hal context
 */
void EVE_CoDlImpl_resetCoState(EVE_HalContext *phost);

/**********************
 *   INLINE FUNCTIONS
 **********************/

/**
 * @param phost Pointer to Hal context
 */
static inline void EVE_CoDl_display(EVE_HalContext *phost)
{
	EVE_CoCmd_dl(phost, DISPLAY());
}

/**
 * @brief Fixed point vertex with subprecision depending on current vertex format
 *
 * @param phost Pointer to Hal context
 * @param x
 * @param y
 */
static inline void EVE_CoDl_vertex2f(EVE_HalContext *phost, int16_t x, int16_t y)
{
	EVE_CoCmd_dl(phost, VERTEX2F(x, y));
}

/**
 * @param phost Pointer to Hal context
 * @param x
 * @param y
 * @param handle
 * @param cell
 */
inline static void EVE_CoDl_vertex2ii(EVE_HalContext *phost, uint16_t x, uint16_t y, uint8_t handle, uint8_t cell)
{
	EVE_CoCmd_dl(phost, VERTEX2II(x, y, handle, cell));
}

/**
 * @param phost Pointer to Hal context
 * @param addr
 */
inline static void EVE_CoDl_bitmapSource(EVE_HalContext *phost, uint32_t addr)
{
	EVE_CoCmd_dl(phost, BITMAP_SOURCE(addr));
}

/**
 * @param phost Pointer to Hal context
 * @param addr
 * @param flash
 */
inline static void EVE_CoDl_bitmapSource_ex(EVE_HalContext *phost, uint32_t addr, bool flash)
{
	EVE_CoCmd_dl(phost, BITMAP_SOURCE2(flash, addr));
}

/**
 * @brief Specify clear color RGB
 *
 * @param phost Pointer to Hal context
 * @param c clear color
 */
inline static void EVE_CoDl_clearColorRgb_ex(EVE_HalContext *phost, uint32_t c)
{
	EVE_CoCmd_dl(phost, CLEAR_COLOR_RGB(0, 0, 0) | (c & 0xFFFFFF));
}

/**
 * @brief Specify clear color RGB
 *
 * @param phost Pointer to Hal context
 * @param r
 * @param g
 * @param b
 */
inline static void EVE_CoDl_clearColorRgb(EVE_HalContext *phost, uint8_t r, uint8_t g, uint8_t b)
{
	EVE_CoCmd_dl(phost, CLEAR_COLOR_RGB(r, g, b));
}

/**
 * @brief Specify clear alpha channel
 *
 * @param phost Pointer to Hal context
 * @param alpha
 */
inline static void EVE_CoDl_clearColorA(EVE_HalContext *phost, uint8_t alpha)
{
	EVE_CoCmd_dl(phost, CLEAR_COLOR_A(alpha));
}

/**
 * @brief Specify clear color: Alpha (bits 31:24) + RGB (bits 23:0)
 *
 * @param phost Pointer to Hal context
 * @param c clear color
 */
inline static void EVE_CoDl_clearColorArgb_ex(EVE_HalContext *phost, uint32_t c)
{
	EVE_CoDl_clearColorRgb_ex(phost, c);
	EVE_CoDl_clearColorA(phost, c >> 24);
}

/**
 * @brief Set current tag.
 *
 * @warning: Must be returned to 255 after usage, to ensure next widgets don't draw with invalid tag
 * @param phost Pointer to Hal context
 * @param s
 */
inline static void EVE_CoDl_tag(EVE_HalContext *phost, uint8_t s)
{
	EVE_CoCmd_dl(phost, TAG(s));
}

/**
 * @brief Specify color RGB
 *
 * @param phost Pointer to Hal context
 * @param c
 */
inline static void EVE_CoDl_colorRgb_ex(EVE_HalContext *phost, uint32_t c)
{
	uint32_t rgb = c & 0xFFFFFF;
#if EVE_DL_OPTIMIZE
	if (rgb != EVE_DL_STATE.ColorRGB)
	{
#endif
		EVE_CoCmd_dl(phost, COLOR_RGB(0, 0, 0) | (c & 0xFFFFFF));
#if EVE_DL_OPTIMIZE
		EVE_DL_STATE.ColorRGB = rgb;
	}
#endif
}

/**
 * @brief Specify color RGB
 *
 * @param phost Pointer to Hal context
 * @param r
 * @param g
 * @param b
 */
inline static void EVE_CoDl_colorRgb(EVE_HalContext *phost, uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t rgb = ((uint32_t)r << 16) | ((uint32_t)g << 8) | ((uint32_t)b);
	EVE_CoDl_colorRgb_ex(phost, rgb);
}

/**
 * @brief Specify alpha channel
 *
 * @param phost Pointer to Hal context
 * @param alpha
 */
inline static void EVE_CoDl_colorA(EVE_HalContext *phost, uint8_t alpha)
{
#if EVE_DL_OPTIMIZE
	if (alpha != EVE_DL_STATE.ColorA)
	{
#endif
		EVE_CoCmd_dl(phost, COLOR_A(alpha));
#if EVE_DL_OPTIMIZE
		EVE_DL_STATE.ColorA = alpha;
	}
#endif
}

/**
 * @brief Specify color: Alpha (bits 31:24) + RGB (bits 23:0)
 *
 * @param phost Pointer to Hal context
 * @param c
 */
inline static void EVE_CoDl_colorArgb_ex(EVE_HalContext *phost, uint32_t c)
{
	EVE_CoDl_colorRgb_ex(phost, c);
	EVE_CoDl_colorA(phost, c >> 24);
}

/**
 * @brief Specify bitmap handle, see BITMAP_HANDLE
 *
 * @param phost Pointer to Hal context
 * @param handle
 */
inline static void EVE_CoDl_bitmapHandle(EVE_HalContext *phost, uint8_t handle)
{
#if EVE_DL_OPTIMIZE
	if (handle != EVE_DL_STATE.Handle)
	{
#endif
		EVE_CoCmd_dl(phost, BITMAP_HANDLE(handle));
#if EVE_DL_OPTIMIZE
		EVE_DL_STATE.Handle = handle;
	}
#endif
}

/**
 * @brief Specify cell number for bitmap, see CELL
 *
 * @param phost Pointer to Hal context
 * @param cell
 */
inline static void EVE_CoDl_cell(EVE_HalContext *phost, uint8_t cell)
{
#if EVE_DL_OPTIMIZE
	if (cell != EVE_DL_STATE.Cell)
	{
#endif
		EVE_CoCmd_dl(phost, CELL(cell));
#if EVE_DL_OPTIMIZE
		EVE_DL_STATE.Cell = cell;
	}
#endif
}

/**
 * @param phost Pointer to Hal context
 * @param format
 * @param linestride
 * @param height
 */
static inline void EVE_CoDl_bitmapLayout(EVE_HalContext *phost, uint8_t format, uint16_t linestride, uint16_t height)
{
	EVE_CoCmd_dl(phost, BITMAP_LAYOUT_H(linestride >> 10, height >> 9));
	EVE_CoCmd_dl(phost, BITMAP_LAYOUT(format, linestride, height));
}

/**
 * @param phost Pointer to Hal context
 * @param filter
 * @param wrapx
 * @param wrapy
 * @param width
 * @param height
 */
static inline void EVE_CoDl_bitmapSize(EVE_HalContext *phost, uint8_t filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height)
{
	EVE_CoCmd_dl(phost, BITMAP_SIZE_H(width >> 9, height >> 9));
	EVE_CoCmd_dl(phost, BITMAP_SIZE(filter, wrapx, wrapy, width, height));
}

/**
 * @param phost Pointer to Hal context
 * @param func
 * @param ref
 */
static inline void EVE_CoDl_alphaFunc(EVE_HalContext *phost, uint8_t func, uint8_t ref)
{
	EVE_CoCmd_dl(phost, ALPHA_FUNC(func, ref));
}

/**
 * @param phost Pointer to Hal context
 * @param func
 * @param ref
 * @param mask
 */
static inline void EVE_CoDl_stencilFunc(EVE_HalContext *phost, uint8_t func, uint8_t ref, uint8_t mask)
{
	EVE_CoCmd_dl(phost, STENCIL_FUNC(func, ref, mask));
}

/**
 * @param phost Pointer to Hal context
 * @param src
 * @param dst
 */
static inline void EVE_CoDl_blendFunc(EVE_HalContext *phost, uint8_t src, uint8_t dst)
{
	EVE_CoCmd_dl(phost, BLEND_FUNC(src, dst));
}

/**
 * @param phost Pointer to Hal context
 */
static inline void EVE_CoDl_blendFunc_default(EVE_HalContext *phost)
{
	EVE_CoDl_blendFunc(phost, SRC_ALPHA, ONE_MINUS_SRC_ALPHA); // TODO: Add default calls for all display list state instructions
}

/**
 * @param phost Pointer to Hal context
 * @param sfail
 * @param spass
 */
static inline void EVE_CoDl_stencilOp(EVE_HalContext *phost, uint8_t sfail, uint8_t spass)
{
	EVE_CoCmd_dl(phost, STENCIL_OP(sfail, spass));
}

/**
 * @param phost Pointer to Hal context
 * @param size
 */
inline static void EVE_CoDl_pointSize(EVE_HalContext *phost, int16_t size)
{
#if EVE_DL_OPTIMIZE
	if (size != EVE_DL_STATE.PointSize)
	{
#endif
		EVE_CoCmd_dl(phost, POINT_SIZE(size));
#if EVE_DL_OPTIMIZE
		EVE_DL_STATE.PointSize = size;
	}
#endif
}

/**
 * @param phost Pointer to Hal context
 * @param width
 */
inline static void EVE_CoDl_lineWidth(EVE_HalContext *phost, int16_t width)
{
#if EVE_DL_OPTIMIZE
	if (width != EVE_DL_STATE.LineWidth)
	{
#endif
		EVE_CoCmd_dl(phost, LINE_WIDTH(width));
#if EVE_DL_OPTIMIZE
		EVE_DL_STATE.LineWidth = width;
	}
#endif
}

/**
 * @param phost Pointer to Hal context
 * @param s
 */
static inline void EVE_CoDl_clearStencil(EVE_HalContext *phost, uint8_t s)
{
	EVE_CoCmd_dl(phost, CLEAR_STENCIL(s));
}

/**
 * @param phost Pointer to Hal context
 * @param s
 */
static inline void EVE_CoDl_clearTag(EVE_HalContext *phost, uint8_t s)
{
	EVE_CoCmd_dl(phost, CLEAR_TAG(s));
}

/**
 * @param phost Pointer to Hal context
 * @param mask
 */
static inline void EVE_CoDl_stencilMask(EVE_HalContext *phost, uint8_t mask)
{
	EVE_CoCmd_dl(phost, STENCIL_MASK(mask));
}

/**
 * @param phost Pointer to Hal context
 * @param mask
 */
static inline void EVE_CoDl_tagMask(EVE_HalContext *phost, bool mask)
{
	EVE_CoCmd_dl(phost, TAG_MASK(mask));
}

/**
 * @param phost Pointer to Hal context
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformA(EVE_HalContext *phost, uint16_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_A(v));
#if EVE_DL_OPTIMIZE
	EVE_DL_STATE.BitmapTransform = true;
#endif
}

/**
 * @param phost Pointer to Hal context
 * @param p
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformA_ex(EVE_HalContext *phost, bool p, uint16_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_A_EXT(p, v));
#if EVE_DL_OPTIMIZE
	EVE_DL_STATE.BitmapTransform = true;
#endif
}

/**
 * @param phost Pointer to Hal context
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformB(EVE_HalContext *phost, uint16_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_B(v));
#if EVE_DL_OPTIMIZE
	EVE_DL_STATE.BitmapTransform = true;
#endif
}

/**
 * @param phost Pointer to Hal context
 * @param p
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformB_ex(EVE_HalContext *phost, bool p, uint16_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_B_EXT(p, v));
#if EVE_DL_OPTIMIZE
	EVE_DL_STATE.BitmapTransform = true;
#endif
}

/**
 * @param phost Pointer to Hal context
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformC(EVE_HalContext *phost, uint32_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_C(v));
#if EVE_DL_OPTIMIZE
	EVE_DL_STATE.BitmapTransform = true;
#endif
}

/**
 * @param phost Pointer to Hal context
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformD(EVE_HalContext *phost, uint16_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_D(v));
#if EVE_DL_OPTIMIZE
	EVE_DL_STATE.BitmapTransform = true;
#endif
}

/**
 * @param phost Pointer to Hal context
 * @param p
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformD_ex(EVE_HalContext *phost, bool p, uint16_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_D_EXT(p, v));
#if EVE_DL_OPTIMIZE
	EVE_DL_STATE.BitmapTransform = true;
#endif
}

/**
 * @param phost Pointer to Hal context
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformE(EVE_HalContext *phost, uint16_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_E(v));
#if EVE_DL_OPTIMIZE
	EVE_DL_STATE.BitmapTransform = true;
#endif
}

/**
 * @param phost Pointer to Hal context
 * @param p
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformE_ex(EVE_HalContext *phost, bool p, uint16_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_E_EXT(p, v));
#if EVE_DL_OPTIMIZE
	EVE_DL_STATE.BitmapTransform = true;
#endif
}

/**
 * @param phost Pointer to Hal context
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformF(EVE_HalContext *phost, uint32_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_F(v));
#if EVE_DL_OPTIMIZE
	EVE_DL_STATE.BitmapTransform = true;
#endif
}

/**
 * @param phost Pointer to Hal context
 * @param p
 * @param a
 * @param b
 * @param c
 * @param d
 * @param e
 * @param f
 */
static inline void EVE_CoDl_bitmapTransform_ex(EVE_HalContext *phost, bool p, uint16_t a, uint16_t b, uint32_t c, uint16_t d, uint16_t e, uint32_t f)
{
	EVE_CoDl_bitmapTransformA_ex(phost, p, a);
	EVE_CoDl_bitmapTransformB_ex(phost, p, b);
	EVE_CoDl_bitmapTransformC(phost, c);
	EVE_CoDl_bitmapTransformD_ex(phost, p, d);
	EVE_CoDl_bitmapTransformE_ex(phost, p, e);
	EVE_CoDl_bitmapTransformF(phost, f);
#if EVE_DL_OPTIMIZE
	EVE_DL_STATE.BitmapTransform = true;
#endif
}

/**
 * @param phost Pointer to Hal context
 */
static inline void EVE_CoDl_bitmapTransform_identity(EVE_HalContext *phost)
{
#if EVE_DL_OPTIMIZE
	if (EVE_DL_STATE.BitmapTransform)
	{
		/* Setting matrix can be skipped if already identity, since it's a no-op */
#endif
		EVE_CoDl_bitmapTransformA_ex(phost, 0, 256);
		EVE_CoDl_bitmapTransformB_ex(phost, 0, 0);
		EVE_CoDl_bitmapTransformC(phost, 0);
		EVE_CoDl_bitmapTransformD_ex(phost, 0, 0);
		EVE_CoDl_bitmapTransformE_ex(phost, 0, 256);
		EVE_CoDl_bitmapTransformF(phost, 0);
#if EVE_DL_OPTIMIZE
		EVE_DL_STATE.BitmapTransform = false;
	}
#endif
}

/**
 * @param phost Pointer to Hal context
 * @param x
 * @param y
 */
static inline void EVE_CoDl_scissorXY(EVE_HalContext *phost, uint16_t x, uint16_t y)
{
#if EVE_DL_OPTIMIZE && EVE_DL_CACHE_SCISSOR
	if (EVE_DL_STATE.ScissorX != x || EVE_DL_STATE.ScissorY != y)
#endif
	{
		EVE_CoCmd_dl(phost, SCISSOR_XY(x, y));
#if EVE_DL_CACHE_SCISSOR
		EVE_DL_STATE.ScissorX = x;
		EVE_DL_STATE.ScissorY = y;
#endif
	}
}

/**
 * @param phost Pointer to Hal context
 * @param width
 * @param height
 */
static inline void EVE_CoDl_scissorSize(EVE_HalContext *phost, uint16_t width, uint16_t height)
{
#if EVE_DL_OPTIMIZE && EVE_DL_CACHE_SCISSOR
	if (EVE_DL_STATE.ScissorWidth != width || EVE_DL_STATE.ScissorHeight != height)
#endif
	{
		EVE_CoCmd_dl(phost, SCISSOR_SIZE(width, height));
#if EVE_DL_CACHE_SCISSOR
		EVE_DL_STATE.ScissorWidth = width;
		EVE_DL_STATE.ScissorHeight = height;
#endif
	}
}

/**
 * @param phost Pointer to Hal context
 * @param dest
 */
static inline void EVE_CoDl_call(EVE_HalContext *phost, uint16_t dest)
{
	EVE_CoCmd_dl(phost, CALL(dest));
}

/**
 * @param phost Pointer to Hal context
 * @param dest
 */
static inline void EVE_CoDl_jump(EVE_HalContext *phost, uint16_t dest)
{
	EVE_CoCmd_dl(phost, JUMP(dest));
}

/**
 * @param phost Pointer to Hal context
 * @param prim
 */
static inline void EVE_CoDl_begin(EVE_HalContext *phost, uint8_t prim)
{
#if EVE_DL_OPTIMIZE
	/* For continuous primitives, reset the active primitive. */
	uint8_t oldPrim = phost->DlPrimitive;
	switch (oldPrim)
	{
	case LINE_STRIP:
	case EDGE_STRIP_R:
	case EDGE_STRIP_L:
	case EDGE_STRIP_A:
	case EDGE_STRIP_B:
		oldPrim = 0;
		break;
	default:
		break;
	}
	if (prim != oldPrim)
	{
#endif
		EVE_CoCmd_dl(phost, BEGIN(prim));
#if EVE_DL_OPTIMIZE
		phost->DlPrimitive = prim;
	}
#endif
}

/**
 * @param phost Pointer to Hal context
 * @param r
 * @param g
 * @param b
 * @param a
 */
static inline void EVE_CoDl_colorMask(EVE_HalContext *phost, bool r, bool g, bool b, bool a)
{
	EVE_CoCmd_dl(phost, COLOR_MASK(r, g, b, a));
}

/**
 * @param phost Pointer to Hal context
 */
inline static void EVE_CoDl_end(EVE_HalContext *phost)
{
#if EVE_DL_END_PRIMITIVE || !EVE_DL_OPTIMIZE
#if EVE_DL_OPTIMIZE
	if (phost->DlPrimitive != 0)
	{
#endif
		EVE_CoCmd_dl(phost, END());
#if EVE_DL_OPTIMIZE
		phost->DlPrimitive = 0;
	}
#endif
#endif
}

/**
 * @brief Save EVE context, see SAVE_CONTEXT
 *
 * @param phost Pointer to Hal context
 */
inline static void EVE_CoDl_saveContext(EVE_HalContext *phost)
{
#if (EVE_DL_OPTIMIZE) || (EVE_DL_CACHE_SCISSOR)
	uint8_t nextState;
#endif
	EVE_CoCmd_dl(phost, SAVE_CONTEXT());
#if (EVE_DL_OPTIMIZE) || (EVE_DL_CACHE_SCISSOR)
	nextState = (phost->DlStateIndex + 1) & EVE_DL_STATE_STACK_MASK;
	phost->DlState[nextState] = phost->DlState[phost->DlStateIndex];
	phost->DlStateIndex = nextState;
#endif
}

/**
 * @brief Restore EVE context, see RESTORE_CONTEXT
 *
 * @param phost Pointer to Hal context
 */
inline static void EVE_CoDl_restoreContext(EVE_HalContext *phost)
{
	EVE_CoCmd_dl(phost, RESTORE_CONTEXT());
#if (EVE_DL_OPTIMIZE) || (EVE_DL_CACHE_SCISSOR)
	phost->DlStateIndex = (phost->DlStateIndex - 1) & EVE_DL_STATE_STACK_MASK;
#endif
}

/**
 * @param phost Pointer to Hal context
 */
static inline void EVE_CoDl_return(EVE_HalContext *phost)
{
	EVE_CoCmd_dl(phost, RETURN());
}

/**
 * @param phost Pointer to Hal context
 * @param m
 */
static inline void EVE_CoDl_macro(EVE_HalContext *phost, uint16_t m)
{
	EVE_CoCmd_dl(phost, MACRO(m));
}

/**
 * @param phost Pointer to Hal context
 * @param c clear color
 * @param s clear stencil
 * @param t clear tag
 */
static inline void EVE_CoDl_clear(EVE_HalContext *phost, bool c, bool s, bool t)
{
	EVE_CoCmd_dl(phost, CLEAR(c, s, t));
}

/**
 * @param phost Pointer to Hal context
 * @param frac
 */
static inline void EVE_CoDl_vertexFormat(EVE_HalContext *phost, uint8_t frac)
{
#if EVE_DL_OPTIMIZE
	if (frac != EVE_DL_STATE.VertexFormat)
	{
#endif
		EVE_CoCmd_dl(phost, VERTEX_FORMAT(frac));
#if EVE_DL_OPTIMIZE
		EVE_DL_STATE.VertexFormat = frac;
	}
#endif
}

/**
 * @brief Set palette source, see PALETTE_SOURCE command
 *
 * @param phost Pointer to Hal context
 * @param addr
 */
inline static void EVE_CoDl_paletteSource(EVE_HalContext *phost, uint32_t addr)
{
#if EVE_DL_OPTIMIZE
	if (addr != EVE_DL_STATE.PaletteSource)
	{
#endif
		EVE_CoCmd_dl(phost, PALETTE_SOURCE(addr));
#if EVE_DL_OPTIMIZE
		EVE_DL_STATE.PaletteSource = addr;
	}
#endif
}

/**
 * @param phost Pointer to Hal context
 * @param x
 */
static inline void EVE_CoDl_vertexTranslateX(EVE_HalContext *phost, int16_t x)
{
	EVE_CoCmd_dl(phost, VERTEX_TRANSLATE_X(x));
}

/**
 * @param phost Pointer to Hal context
 * @param y
 */
static inline void EVE_CoDl_vertexTranslateY(EVE_HalContext *phost, int16_t y)
{
	EVE_CoCmd_dl(phost, VERTEX_TRANSLATE_Y(y));
}

/**
 * @param phost Pointer to Hal context
 */
static inline void EVE_CoDl_nop(EVE_HalContext *phost)
{
}

/**
 * @brief Fixed point vertex using 4 bits subprecision
 *
 * @param phost Pointer to Hal context
 * @param x
 * @param y
 */
inline static void EVE_CoDl_vertex2f_4(EVE_HalContext *phost, int16_t x, int16_t y)
{
	if (x > EVE_VERTEX2F_MAX || y > EVE_VERTEX2F_MAX) /* Support display up to 2048 px */
	{
		EVE_CoDl_vertexFormat(phost, 3);
		EVE_CoDl_vertex2f(phost, x >> 1, y >> 1);
	}
	else
	{
		EVE_CoDl_vertexFormat(phost, 4);
		EVE_CoDl_vertex2f(phost, x, y);
	}
}

/**
 * @brief Fixed point vertex using 2 bits subprecision
 *
 * @param phost Pointer to Hal context
 * @param x
 * @param y
 */
inline static void EVE_CoDl_vertex2f_2(EVE_HalContext *phost, int16_t x, int16_t y)
{
	EVE_CoDl_vertexFormat(phost, 2);
	EVE_CoDl_vertex2f(phost, x, y);
}

/**
 * @brief Fixed point vertex using 0 bits subprecision, or integer point vertex
 *
 * @param phost Pointer to Hal context
 * @param x
 * @param y
 */
inline static void EVE_CoDl_vertex2f_0(EVE_HalContext *phost, int16_t x, int16_t y)
{
	EVE_CoDl_vertexFormat(phost, 0);
	EVE_CoDl_vertex2f(phost, x, y);
}

#endif /* EVE_CO_DL__H */

/* end of file */
