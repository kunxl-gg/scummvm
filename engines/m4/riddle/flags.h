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

#ifndef M4_RIDDLE_FLAGS_H
#define M4_RIDDLE_FLAGS_H

#include "common/array.h"
#include "common/serializer.h"
#include "m4/m4_types.h"

namespace M4 {
namespace Riddle {

#define FLAGS_COUNT 512

enum {
	V000 =   0,
	V001 =   1,
	V002 =   2,
	V003 =   3,
	V004 =   4,
	V005 =   5,
	V006 =   6,
	V007 =   7,
	V008 =   8,
	V009 =   9,
	V010 =  10,
	V011 =  11,
	V012 =  12,
	V013 =  13,
	V014 =  14,
	V015 =  15,
	V016 =  16,
	V017 =  17,
	V018 =  18,
	V019 =  19,
	V020 =  20,
	V021 =  21,
	V022 =  22,
	V023 =  23,
	V024 =  24,
	V025 =  25,
	V026 =  26,
	V027 =  27,
	V028 =  28,
	V029 =  29,
	V030 =  30,
	V031 =  31,
	V032 =  32,
	V033 =  33,
	V034 =  34,
	V035 =  35,
	V036 =  36,
	V037 =  37,
	V038 =  38,
	V039 =  39,
	V040 =  40,
	V041 =  41,
	V042 =  42,
	V043 =  43,
	V044 =  44,
	V045 =  45,
	V046 =  46,
	V047 =  47,
	V048 =  48,
	V049 =  49,
	V050 =  50,
	V051 =  51,
	V052 =  52,
	V053 =  53,
	V054 =  54,
	V055 =  55,
	V056 =  56,
	V057 =  57,
	V058 =  58,
	V059 =  59,
	V060 =  60,
	V061 =  61,
	V062 =  62,
	V063 =  63,
	V064 =  64,
	V065 =  65,
	V066 =  66,
	V067 =  67,
	V068 =  68,
	V069 =  69,
	V070 =  70,
	V071 =  71,
	V072 =  72,
	V073 =  73,
	V074 =  74,
	V075 =  75,
	V076 =  76,
	V077 =  77,
	V078 =  78,
	V079 =  79,
	V080 =  80,
	V081 =  81,
	V082 =  82,
	V083 =  83,
	V084 =  84,
	V085 =  85,
	V086 =  86,
	V087 =  87,
	V088 =  88,
	V089 =  89,
	V090 =  90,
	V091 =  91,
	V092 =  92,
	V093 =  93,
	V094 =  94,
	V095 =  95,
	V096 =  96,
	V097 =  97,
	V098 =  98,
	V099 =  99,

	V100 = 100,
	V101 = 101,
	V102 = 102,
	V103 = 103,
	V104 = 104,
	V105 = 105,
	V106 = 106,
	V107 = 107,
	V108 = 108,
	V109 = 109,
	V110 = 110,
	V111 = 111,
	V112 = 112,
	V113 = 113,
	V114 = 114,
	V115 = 115,
	V116 = 116,
	V117 = 117,
	V118 = 118,
	V119 = 119,
	V120 = 120,
	V121 = 121,
	V122 = 122,
	V123 = 123,
	V124 = 124,
	V125 = 125,
	V126 = 126,
	V127 = 127,
	V128 = 128,
	V129 = 129,
	V130 = 130,
	V131 = 131,
	V132 = 132,
	V133 = 133,
	V134 = 134,
	V135 = 135,
	V136 = 136,
	V137 = 137,
	V138 = 138,
	V139 = 139,
	V140 = 140,
	V141 = 141,
	V142 = 142,
	V143 = 143,
	V144 = 144,
	V145 = 145,
	V146 = 146,
	V147 = 147,
	V148 = 148,
	V149 = 149,
	V150 = 150,
	V151 = 151,
	V152 = 152,
	V153 = 153,
	V154 = 154,
	V155 = 155,
	V156 = 156,
	V157 = 157,
	V158 = 158,
	V159 = 159,
	V160 = 160,
	V161 = 161,
	V162 = 162,
	V163 = 163,
	V164 = 164,
	V165 = 165,
	V166 = 166,
	V167 = 167,
	V168 = 168,
	V169 = 169,
	V170 = 170,
	V171 = 171,
	V172 = 172,
	V173 = 173,
	V174 = 174,
	V175 = 175,
	V176 = 176,
	V177 = 177,
	V178 = 178,
	V179 = 179,
	V180 = 180,
	V181 = 181,
	V182 = 182,
	V183 = 183,
	V184 = 184,
	V185 = 185,
	V186 = 186,
	V187 = 187,
	V188 = 188,
	V189 = 189,
	V190 = 190,
	V191 = 191,
	V192 = 192,
	V193 = 193,
	V194 = 194,
	V195 = 195,
	V196 = 196,
	V197 = 197,
	V198 = 198,
	V199 = 199,

	V200 = 200,
	V201 = 201,
	V202 = 202,
	V203 = 203,
	V204 = 204,
	V205 = 205,
	V206 = 206,
	V207 = 207,
	V208 = 208,
	V209 = 209,
	V210 = 210,
	V211 = 211,
	V212 = 212,
	V213 = 213,
	V214 = 214,
	V215 = 215,
	V216 = 216,
	V217 = 217,
	V218 = 218,
	V219 = 219,
	V220 = 220,
	V221 = 221,
	V222 = 222,
	V223 = 223,
	V224 = 224,
	V225 = 225,
	V226 = 226,
	V227 = 227,
	V228 = 228,
	V229 = 229,
	V230 = 230,
	V231 = 231,
	V232 = 232,
	V233 = 233,
	V234 = 234,
	V235 = 235,
	V236 = 236,
	V237 = 237,
	V238 = 238,
	V239 = 239,
	V240 = 240,
	V241 = 241,
	V242 = 242,
	V243 = 243,
	V244 = 244,
	V245 = 245,
	V246 = 246,
	V247 = 247,
	V248 = 248,
	V249 = 249,
	V250 = 250,
	V251 = 251,
	V252 = 252,
	V253 = 253,
	V254 = 254,
	V255 = 255,
	V256 = 256,
	V257 = 257,
	V258 = 258,
	V259 = 259,
	V260 = 260,
	V261 = 261,
	V262 = 262,
	V263 = 263,
	V264 = 264,
	V265 = 265,
	V266 = 266,
	V267 = 267,
	V268 = 268,
	V269 = 269,
	V270 = 270,
	V271 = 271,
	V272 = 272,
	V273 = 273,
	V274 = 274,
	V275 = 275,
	V276 = 276,
	V277 = 277,
	V278 = 278,
	V279 = 279,
	V280 = 280,
	V281 = 281,
	V282 = 282,
	V283 = 283,
	V284 = 284,
	V285 = 285,
	V286 = 286,
	V287 = 287,
	V288 = 288,
	V289 = 289,
	V290 = 290,
	V291 = 291,
	V292 = 292,
	V293 = 293,
	V294 = 294,
	V295 = 295,
	V296 = 296,
	V297 = 297,
	V298 = 298,
	V299 = 299,

	V300 = 300,
	V301 = 301,
	V302 = 302,
	V303 = 303,
	V304 = 304,
	V305 = 305,
	V306 = 306,
	V307 = 307,
	V308 = 308,
	V309 = 309,
	V310 = 310,
	V311 = 311,
	V312 = 312,
	V313 = 313,
	V314 = 314,
	V315 = 315,
	V316 = 316,
	V317 = 317,
	V318 = 318,
	V319 = 319,
	V320 = 320,
	V321 = 321,
	V322 = 322,
	V323 = 323,
	V324 = 324,
	V325 = 325,
	V326 = 326,
	V327 = 327,
	V328 = 328,
	V329 = 329,
	V330 = 330,
	V331 = 331,
	V332 = 332,
	V333 = 333,
	V334 = 334,
	V335 = 335,
	V336 = 336,
	V337 = 337,
	V338 = 338,
	V339 = 339,
	V340 = 340,
	V341 = 341,
	V342 = 342,
	V343 = 343,
	V344 = 344,
	V345 = 345,
	V346 = 346,
	V347 = 347,
	V348 = 348,
	V349 = 349,
	V350 = 350,
	V351 = 351,
	V352 = 352,
	V353 = 353,
	V354 = 354,
	V355 = 355,
	V356 = 356,
	V357 = 357,
	V358 = 358,
	V359 = 359,
	V360 = 360,
	V361 = 361,
	V362 = 362,
	V363 = 363,
	V364 = 364,
	V365 = 365,
	V366 = 366,
	V367 = 367,
	V368 = 368,
	V369 = 369,
	V370 = 370,
	V371 = 371,
	V372 = 372,
	V373 = 373,
	V374 = 374,
	V375 = 375,
	V376 = 376,
	V377 = 377,
	V378 = 378,
	V379 = 379,
	V380 = 380,
	V381 = 381,
	V382 = 382,
	V383 = 383,
	V384 = 384,
	V385 = 385,
	V386 = 386,
	V387 = 387,
	V388 = 388,
	V389 = 389,
	V390 = 390,
	V391 = 391,
	V392 = 392,
	V393 = 393,
	V394 = 394,
	V395 = 395,
	V396 = 396,
	V397 = 397,
	V398 = 398,
	V399 = 399,

	V400 = 400,
	V401 = 401,
	V402 = 402,
	V403 = 403,
	V404 = 404,
	V405 = 405,
	V406 = 406,
	V407 = 407,
	V408 = 408,
	V409 = 409,
	V410 = 410,
	V411 = 411,
	V412 = 412,
	V413 = 413,
	V414 = 414,
	V415 = 415,
	V416 = 416,
	V417 = 417,
	V418 = 418,
	V419 = 419,
	V420 = 420,
	V421 = 421,
	V422 = 422,
	V423 = 423,
	V424 = 424,
	V425 = 425,
	V426 = 426,
	V427 = 427,
	V428 = 428,
	V429 = 429,
	V430 = 430,
	V431 = 431,
	V432 = 432,
	V433 = 433,
	V434 = 434,
	V435 = 435,
	V436 = 436,
	V437 = 437,
	V438 = 438,
	V439 = 439,
	V440 = 440,
	V441 = 441,
	V442 = 442,
	V443 = 443,
	V444 = 444,
	V445 = 445,
	V446 = 446,
	V447 = 447,
	V448 = 448,
	V449 = 449,
	V450 = 450,
	V451 = 451,
	V452 = 452,
	V453 = 453,
	V454 = 454,
	V455 = 455,
	V456 = 456,
	V457 = 457,
	V458 = 458,
	V459 = 459,
	V460 = 460,
	V461 = 461,
	V462 = 462,
	V463 = 463,
	V464 = 464,
	V465 = 465,
	V466 = 466,
	V467 = 467,
	V468 = 468,
	V469 = 469,
	V470 = 470,
	V471 = 471,
	V472 = 472,
	V473 = 473,
	V474 = 474,
	V475 = 475,
	V476 = 476,
	V477 = 477,
	V478 = 478,
	V479 = 479,
	V480 = 480,
	V481 = 481,
	V482 = 482,
	V483 = 483,
	V484 = 484,
	V485 = 485,
	V486 = 486,
	V487 = 487,
	V488 = 488,
	V489 = 489,
	V490 = 490,
	V491 = 491,
	V492 = 492,
	V493 = 493,
	V494 = 494,
	V495 = 495,
	V496 = 496,
	V497 = 497,
	V498 = 498,
	V499 = 499,

	V500 = 500,
	V501 = 501,
	V502 = 502,
	V503 = 503,
	V504 = 504,
	V505 = 505,
	V506 = 506,
	V507 = 507,
	V508 = 508,
	V509 = 509,
	V510 = 510,
	V511 = 511
};

class Flags {
public:
	static int32 _flags[FLAGS_COUNT];

public:
	Flags();

	/**
	 * Save/load values to a save file
	 */
	void sync(Common::Serializer &s);

	size_t size() const {
		return FLAGS_COUNT;
	}
	int32 &operator[](uint idx) {
		assert(idx < FLAGS_COUNT);
		return _flags[idx];
	}

	/**
	 * Reset the flags
	 */
	void reset();
};

} // namespace Riddle
} // namespace M4

#endif
