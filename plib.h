/*
 * Copyright (C) 2012 Santhosh N <santhoshn@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<unistd.h>

struct panchanga {
	char dtoday[16];
	char dyoga[64];
	char dnakshatra[64];
	char dtithi[64];
	char dkarana[64];
	char dpaksha[64];
	char drashi[32];
};

void calculate_panchanga(int dd, int mm, int yy, double hr, double zhr, struct panchanga *pdata);
