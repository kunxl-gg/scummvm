/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ags/shared/debugging/out.h"
#include "common/scummsys.h"
#include "common/debug.h"
#include "ags/shared/core/platform.h"
#include "ags/shared/gfx/gfx_def.h"
//#include "ags/shared/debugging/assert.h"
// File not present??
#include "common/scummsys.h"
#include "ags/lib/allegro/color.h"
#include "ags/shared/gfx/bitmap.h"
#include "ags/shared/gfx/image.h"
#include "ags/lib/allegro/surface.h"
#include "ags/shared/debugging/debug_manager.h"
#include "ags/globals.h"
#include "graphics/managed_surface.h"
#include "graphics/pixelformat.h"

#ifdef __aarch64__
#define OPT_NEON
#include "ags/lib/allegro/surface_simd_neon.h"
#elif defined(__x86_64__) || defined(__i686__)
#define OPT_SSE
#include "ags/lib/allegro/surface_simd_sse.h"
#endif

namespace AGS3 {

namespace GfxDef = AGS::Shared::GfxDef;
using namespace AGS::Shared;

// Comment this out if you don't want the console to be clogged with info durning tests
#define VERBOSE_TEST_GFX

void Test_GfxSpeed(bool opt, int blenderModeStart, int blenderModeEnd) {
	_G(_bitmap_simd_optimizations) = opt;
#ifdef VERBOSE_TEST_GFX
	if (opt) debug("SIMD optimizations: true\n");
	else debug("SIMD optmizations: false\n");
#endif
	Bitmap *benchgfx32 = BitmapHelper::CreateBitmap(100, 100, 32);
	Bitmap *benchgfx16 = BitmapHelper::CreateBitmapCopy(benchgfx32, 16);
	Bitmap *benchgfx8 = BitmapHelper::CreateBitmap(100, 100, 8);
	Bitmap *dest32 = BitmapHelper::CreateBitmap(100, 100, 32);
	Bitmap *dest16 = BitmapHelper::CreateBitmap(100, 100, 16);
	Bitmap *dest8 = BitmapHelper::CreateBitmap(100, 100, 8);
	int benchRuns[] = {1000, 10000, 100000};
	int blenderModes[] = {kRgbToRgbBlender, kSourceAlphaBlender, kArgbToArgbBlender, kOpaqueBlenderMode, kTintLightBlenderMode};
	const char *modeNames[] = {"RGB to RGB", "Source Alpha", "ARGB to ARGB", "Opaque", "Tint with Light"};
	Bitmap *destinations[] = {dest32, dest16, dest8};
	Bitmap *graphics[] = {benchgfx32, benchgfx16, benchgfx8};
	int bpps[] = {32, 16, 8};
	if (blenderModeEnd >= sizeof(blenderModes) / sizeof(blenderModes[0])) blenderModeEnd = (sizeof(blenderModes) / sizeof(blenderModes[0])) - 1;
	for (int dest = 0; dest < 3; dest++) {
		for (int gfx = 0; gfx < 3; gfx++) {
			if (dest == 2 && gfx != 2) continue;
			for (int mode = blenderModeStart; mode <= blenderModeEnd; mode++) {
				for (int runs = 0; (size_t)runs < sizeof(benchRuns)/sizeof(int); runs++) {
					uint32 start, end;
					_G(_blender_mode) = (AGS3::BlenderMode)blenderModes[mode];
#ifdef VERBOSE_TEST_GFX
					if (runs == 2) debug("Dest: %d bpp, Gfx: %d bpp, Blender: %s, Stretched: false, Iters: %d\n", bpps[dest], bpps[gfx], modeNames[mode], benchRuns[runs]);
#endif
					start = std::chrono::high_resolution_clock::now();
					for (int i = 0; i < benchRuns[runs]; i++)
						destinations[dest]->Blit(graphics[gfx], 0, 0, kBitmap_Transparency);
					end = std::chrono::high_resolution_clock::now();
#ifdef VERBOSE_TEST_GFX
					if (runs == 2) debug("exec time (mills): %u\n\n", end - start);
					if (runs == 2) debug("Dest: %d bpp, Gfx: %d bpp, Blender: %s, Stretched: true, Iters: %d\n", bpps[dest], bpps[gfx], modeNames[mode], benchRuns[runs]);
#endif
					start = std::chrono::high_resolution_clock::now();
					for (int i = 0; i < benchRuns[runs]; i++)
						destinations[dest]->StretchBlt(graphics[gfx], Rect(0, 0, 99, 99), kBitmap_Transparency);
					end = std::chrono::high_resolution_clock::now();
#ifdef VERBOSE_TEST_GFX
					if (runs == 2) debug("exec time (mills): %u\n\n", end - start);
#endif
				}
			}
		}
	}
	
	delete benchgfx32;
	delete benchgfx16;
	delete benchgfx8;
	delete dest32;
	delete dest16;
	delete dest8;
}



void Test_DrawingLoops() {
		
}
void Test_BlenderModes() {
	constexpr int depth = 2;
	Graphics::ManagedSurface owner(16, 16, Graphics::PixelFormat(4, 8, 8, 8, 8, 16, 8, 0, 24));
	BITMAP dummy(&owner);
	Graphics::ManagedSurface owner16(16, 16, Graphics::PixelFormat(2, 5, 6, 5, 0, 11, 5, 0, 0));
	BITMAP dummy16(&owner16);
	for (int blenderMode = (int)kSourceAlphaBlender; blenderMode <= (int)kTintLightBlenderMode; blenderMode++) {
		_G(_blender_mode) = (BlenderMode)blenderMode;
		for (int srcR = 0; srcR < 255; srcR += (1 << (8 - depth))) {
			for (int srcG = 0; srcG < 255; srcG += (1 << (8 - depth))) {
				for (int srcB = 0; srcB < 255; srcB += (1 << (8 - depth))) {
					for (int srcA = 0; srcA < 255; srcA += (1 << (8 - depth))) {
						for (int destR = 0; destR < 255; destR += (1 << (8 - depth))) {
							for (int destG = 0; destG < 255; destG += (1 << (8 - depth))) {
								for (int destB = 0; destB < 255; destB += (1 << (8 - depth))) {
									for (int destA = 0; destA < 255; destA += (1 << (8 - depth))) {
										for (uint32 alpha = 0; alpha < 255; alpha += (1 << (8 - depth))) {
											// First run the non-vectorized version of the code
											uint32 controlCol, simdCol, pixelDummy;
											uint16 control2bppCol, simd2bppCol, pixelDummy2bpp;
											uint8 a = destA, r = destR, g = destG, b = destB;
											pixelDummy = (a << 24) | (r << 16) | (g << 8) | b;
											dummy.blendPixel(srcA, srcR, srcG, srcB, a, r, g, b, alpha, false, (byte *)&pixelDummy);
											controlCol = b | (g << 8) | (r << 16) | (a << 24);

											uint8 a16 = 0xff, r16 = destR >> 3, g16 = destG >> 2, b16 = destB >> 3;
											r16 = (r16 << 3) | (r16 >> 2);
											g16 = (g16 << 2) | (g16 >> 4);
											b16 = (b16 << 3) | (b16 >> 2);
											uint8 srcR16 = srcR >> 3, srcG16 = srcG >> 2, srcB16 = srcB >> 3;
											srcR16 = (srcR16 << 3) | (srcR16 >> 2);
											srcG16 = (srcG16 << 2) | (srcG16 >> 4);
											srcB16 = (srcB16 << 3) | (srcB16 >> 2);
											pixelDummy2bpp = (destB >> 3) | ((destG >> 2) << 5) | ((destR >> 3) << 11);
											dummy16.blendPixel(0xff, srcR16, srcG16, srcB16, a16, r16, g16, b16, alpha, false, (byte *)&pixelDummy2bpp);
											r16 >>= 3; g16 >>= 2; b16 >>= 3;
											control2bppCol = b16 | (g16 << 5) | (r16 << 11);
											{
#ifdef OPT_NEON
												uint32x4_t src = vdupq_n_u32(srcB | (srcG << 8) | (srcR << 16) | (srcA << 24));
												uint32x4_t dest = vdupq_n_u32(destB | (destG << 8) | (destR << 16) | (destA << 24));
												uint32x4_t alphas = vdupq_n_u32(alpha);
												simdCol = vgetq_lane_u32(blendPixelSIMD(src, dest, alphas), 0);
#else
												__m128i src = _mm_set1_epi32(srcB | (srcG << 8) | (srcR << 16) | (srcA << 24));
												__m128i dest = _mm_set1_epi32(destB | (destG << 8) | (destR << 16) | (destA << 24));
												__m128i alphas = _mm_set1_epi32((int)alpha);
												simdCol = _mm_cvtsi128_si32(blendPixelSIMD(src, dest, alphas));
#endif
											}
											{
#ifdef OPT_NEON
												uint16x8_t src = vdupq_n_u16((srcB >> 3) | ((srcG >> 2) << 5) | ((srcR >> 3) << 11));
												uint16x8_t dest = vdupq_n_u16((destB >> 3) | ((destG >> 2) << 5) | ((destR >> 3) << 11));
												uint16x8_t alphas = vdupq_n_u16((uint16)alpha);
												simd2bppCol = vgetq_lane_u16(blendPixelSIMD2Bpp(src, dest, alphas), 0);
#else
												__m128i src = _mm_set1_epi16((srcB >> 3) | ((srcG >> 2) << 5) | ((srcR >> 3) << 11));
												__m128i dest = _mm_set1_epi16((destB >> 3) | ((destG >> 2) << 5) | ((destR >> 3) << 11));
												__m128i alphas = _mm_set1_epi16((uint16)alpha);
												simd2bppCol = (uint16)(_mm_cvtsi128_si32(blendPixelSIMD2Bpp(src, dest, alphas)) & 0xffff);
#endif
											}
#ifdef VERBOSE_TEST_GFX
											debug("src argb: %d, %d, %d, %d dest argb: %d, %d, %d, %d a: %d", srcA, srcR, srcG, srcB, destA, destR, destG, destB, alpha);
#endif
											switch ((BlenderMode)blenderMode) {
												case kSourceAlphaBlender: debug("blenderMode: kSourceAlphaBlender"); break;
												case kArgbToArgbBlender: debug("blenderMode: kArgbToArgbBlender"); break;
												case kArgbToRgbBlender: debug("blenderMode: kArgbToRgbBlender"); break;
												case kRgbToArgbBlender: debug("blenderMode: kRgbToArgbBlender"); break;
												case kRgbToRgbBlender: debug("blenderMode: kRgbToRgbBlender"); break;
												case kAlphaPreservedBlenderMode: debug("blenderMode: kAlphaPreservedBlenderMode"); break;
												case kOpaqueBlenderMode: debug("blenderMode: kOpaqueBlenderMode"); break;
												case kAdditiveBlenderMode: debug("blenderMode: kAdditiveBlenderMode"); break;
												case kTintBlenderMode: debug("blenderMode: kTintBlenderMode"); break;
												case kTintLightBlenderMode: debug("blenderMode: kTintLightBlenderMode"); break;
											}
#ifdef VERBOSE_TEST_GFX
											debug("controlCol %x argb: %d, %d, %d, %d", controlCol, a, r, g, b);
											debug("simdCol %x argb: %d, %d, %d, %d", simdCol, (simdCol >> 24), ((simdCol >> 16) & 0xff), ((simdCol >> 8) & 0xff), (simdCol & 0xff));
											debug("control2bppCol %x rgb: %d, %d, %d", control2bppCol, r16, g16, b16);
											debug("simd2bppCol %x rgb: %d, %d, %d", simd2bppCol, (simd2bppCol >> 11), ((simd2bppCol >> 5) & 0x3f), (simd2bppCol & 0x1f));
#endif
											int tolerance, tolerance16;
											switch ((BlenderMode)blenderMode) {
												// These need to be IDENTICAL for lamplight city to work
												// It would be nice to get tolerance16 down to 0 though...
												case kRgbToRgbBlender:
												case kArgbToRgbBlender:
												case kSourceAlphaBlender:
												case kAlphaPreservedBlenderMode:
												tolerance = 0;
												tolerance16 = 1;
												break;

												// These can be 1 or 2 off, as long as they look the same
												case kArgbToArgbBlender:
												case kRgbToArgbBlender:
												case kOpaqueBlenderMode:
												case kAdditiveBlenderMode:
												tolerance = 1;
												tolerance16 = 1;
												break;

												case kTintBlenderMode:
												case kTintLightBlenderMode:
												tolerance = 2;
												tolerance16 = 1;
												break;
											}
											assert(std::abs((int)a - (int)(simdCol >> 24)) <= tolerance);
											assert(std::abs((int)r - (int)((simdCol >> 16) & 0xff)) <= tolerance);
											assert(std::abs((int)g - (int)((simdCol >> 8) & 0xff)) <= tolerance);
											assert(std::abs((int)b - (int)(simdCol & 0xff)) <= tolerance);
											assert(std::abs((int)b16 - (int)(simd2bppCol & 0x1f)) <= tolerance16);
											assert(std::abs((int)g16 - (int)((simd2bppCol >> 5) & 0x3f)) <= tolerance16);
											assert(std::abs((int)r16 - (int)(simd2bppCol >> 11)) <= tolerance16);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void Test_GfxTransparency() {
	// Test that every transparency which is a multiple of 10 is converted
	// forth and back without loosing precision
	const size_t arr_sz = 11;
	const int trans100[arr_sz] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
	int trans255[arr_sz] = { 0 };
	int trans100_back[arr_sz] = { 0 };

	for (size_t i = 0; i < arr_sz; ++i) {
		trans255[i] = GfxDef::Trans100ToLegacyTrans255(trans100[i]);
		trans100_back[i] = GfxDef::LegacyTrans255ToTrans100(trans255[i]);
		assert(trans100[i] == trans100_back[i]);
	}
}

void Test_Gfx() {
	Test_GfxTransparency();
#if defined(OPT_NEON) || defined(OPT_SSE)
	//Test_DrawingLoops();
	//Test_BlenderModes();
	// This could take a LONG time
	bool has_simd = _G(_bitmap_simd_optimizations);
	if (has_simd) Test_GfxSpeed(true, 0, kTintLightBlenderMode);
	Test_GfxSpeed(false, 0, kTintLightBlenderMode);
	_G(_bitmap_simd_optimizations) = has_simd;
#endif
}

} // namespace AGS3
