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

#include "plib.h"

#define D2R (M_PI/180.0)
#define R2D (180.0/M_PI)
#define REV(x)	((x)-floor((x)/360.0)*360.0)

static char month[][15]={"January","February","March","April","May","June",
		  "July","August","September","October","November","December"};

static char rashi[][15]={"Mesha","Vrishabha","Mithuna","Karka","Simha","Kanya","Tula",
		   "Vrischika","Dhanu","Makara","Kumbha","Meena"};

static char day[][15]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

static char tithi[][15]={"Prathame","Dwithiya","Thrithiya","Chathurthi","Panchami",
		  "Shrashti","Saptami","Ashtami","Navami","Dashami","Ekadashi",
		  "Dwadashi","Thrayodashi","Chaturdashi","Poornima","Prathame",
		  "Dwithiya","Thrithiya","Chathurthi","Panchami","Shrashti",
		  "Saptami","Ashtami","Navami","Dashami","Ekadashi","Dwadashi",
		  "Thrayodashi","Chaturdashi","Amavasya"};

static char karan[][15]={"Bava","Balava","Kaulava","Taitula","Garija","Vanija",
		  "Visti","Sakuni","Chatuspada","Naga","Kimstughna"};

static char yoga[][15]={"Vishkambha","Prithi","Ayushman","Saubhagya","Shobhana",
		 "Atiganda","Sukarman","Dhrithi","Shoola","Ganda","Vridhi",
		 "Dhruva","Vyaghata","Harshana","Vajra","Siddhi","Vyatipata",
		 "Variyan","Parigha","Shiva","Siddha","Sadhya","Shubha","Shukla",
		 "Bramha","Indra","Vaidhruthi"};

static char nakshatra[][20]={"Ashwini","Bharani","Krittika","Rohini","Mrigashira","Ardhra",
		      "Punarvasu","Pushya","Ashlesa","Magha","Poorva Phalguni","Uttara Phalguni",
		      "Hasta","Chitra","Swathi","Vishaka","Anuradha","Jyeshta","Mula",
		      "Poorva Ashada","Uttara Ashada","Sravana","Dhanishta","Shatabisha",
		      "Poorva Bhadra","Uttara Bhadra","Revathi"};

static double Ls, Lm, Ms, Mm;

//Calculate Aynamsa
static double calc_ayanamsa(double d)
{
	double t, o, l, ayan;
	
	t = (d+36523.5)/36525;
	o = 259.183275-1934.142008333206*t+0.0020777778*t*t;
	l = 279.696678+36000.76892*t+0.0003025*t*t;
	ayan = 17.23*sin((o)*D2R)+1.27*sin((l*2)*D2R)-(5025.64+1.11*t)*t;
	//Based on Lahiri
	ayan = (ayan-80861.27)/3600.0;

	return ayan;
}

//Longitude of Sun
static double sun_long(double d)
{
	double w, a, e, M, E, x, y, r, v, tmp;

	w = 282.9404+4.70935e-5*d;
	a = 1.000000;
	e = 0.016709-1.151e-9*d;
	M = REV(356.0470+0.9856002585*d);
	Ms = M;
	Ls = w+M;

	tmp = M*D2R;
	E = M+R2D*e*sin(tmp)*(1+e*cos(tmp));

	tmp = E*D2R;
	x = cos(tmp)-e;
	y = sin(tmp)*sqrt(1-e*e);

	r = sqrt(x*x + y*y);
	v = REV(R2D*atan2(y,x));

	return REV(v+w);
}

//Longitude of Moon
static double moon_long(double d)
{
	double N, i, w, a, e, M, E, Et, x, y, r, v, xec, yec, zec, D, F, tmp, tmp1, tmp2, lon; 
	
	N = 125.1228-0.0529538083*d;
 	i = 5.1454;
    	w = REV(318.0634+0.1643573223*d);
    	a = 60.2666;
    	e = 0.054900;
    	M = REV(115.3654+13.0649929509*d);
	Mm = M;
	Lm = N+w+M;

	//Calculate Eccentricity anamoly
	tmp = M*D2R;
	E = M+R2D*e*sin(tmp)*(1+e*cos(tmp));

	tmp = E*D2R;
	Et = E-(E-R2D*e*sin(tmp)-M)/(1-e*cos(tmp));

	do {
		E = Et;
		tmp = E*D2R;
		Et = E-(E-R2D*e*sin(tmp)-M)/(1-e*cos(tmp));
	} while(E-Et>0.005);

	tmp = E*D2R;
	x = a*(cos(tmp)-e);
	y = a*sqrt(1-e*e)*sin(tmp);

	r = sqrt(x*x + y*y);
	v = REV(R2D*atan2(y,x));

	tmp = D2R*N;
	tmp1 = D2R*(v+w);
	tmp2 = D2R*i;
	xec = r*(cos(tmp)*cos(tmp1)-sin(tmp)*sin(tmp1)*cos(tmp2));
	yec = r*(sin(tmp)*cos(tmp1)+cos(tmp)*sin(tmp1)*cos(tmp2));
	zec = r*sin(tmp1)*sin(tmp2);

	//Do some corrections
	D = Lm - Ls;
	F = Lm - N;

	lon = R2D*atan2(yec,xec);

	lon+= -1.274*sin((Mm-2*D)*D2R);
    	lon+= +0.658*sin((2*D)*D2R);
    	lon+= -0.186*sin((Ms)*D2R);
    	lon+= -0.059*sin((2*Mm-2*D)*D2R);
    	lon+= -0.057*sin((Mm-2*D+Ms)*D2R);
    	lon+= +0.053*sin((Mm+2*D)*D2R);
    	lon+= +0.046*sin((2*D-Ms)*D2R);
    	lon+= +0.041*sin((Mm-Ms)*D2R);
    	lon+= -0.035*sin((D)*D2R);
   	lon+= -0.031*sin((Mm+Ms)*D2R);
    	lon+= -0.015*sin((2*F-2*D)*D2R);
    	lon+= +0.011*sin((Mm-4*D)*D2R);

	return REV(lon);
}

//Calculate Panchanga
void calculate_panchanga(int dd, int mm, int yy, double hr, double zhr, struct panchanga *pdata)
{
	double d, ayanamsa, slon, mlon, tmlon, tslon;
	int n;

	//Calculate day number since 2000 Jan 0.0 TDT
	d = (367*yy-7*(yy+(mm+9)/12)/4+275*mm/9+dd-730530);

	//Calculate Ayanamsa, moon and sun longitude
	ayanamsa = calc_ayanamsa(d);
	slon = sun_long(d+((hr-zhr)/24.0));
	mlon = moon_long(d+((hr-zhr)/24.0));

	//Calculate Tithi and Paksha
	tmlon = mlon+((mlon<slon)?360:0);
	tslon = slon;
	n = (int)((tmlon-tslon)/12);
	strcpy(pdata->dtithi,tithi[n]);
	(n<=14)?strcpy(pdata->dpaksha,"Shukla"):strcpy(pdata->dpaksha,"Krishna");
		
	//Calculate Nakshatra
	tmlon = REV(mlon+ayanamsa);
	strcpy(pdata->dnakshatra,nakshatra[(int)(tmlon*6/80)]);

	//Calculate Yoga
	tmlon = mlon+ayanamsa;
	tslon = slon+ayanamsa;
	strcpy(pdata->dyoga,yoga[(int)(REV(tmlon+tslon)*6/80)]);

	//Calculate Karana
	tmlon = mlon+((mlon<slon)?360:0);
	tslon = slon;
	n = (int)((tmlon-tslon)/6);
	if(n==0) n=10;
	if(n>=57) n-=50;
	if(n>0 && n<57) n=(n-1)-((n-1)/7*7);
	strcpy(pdata->dkarana,karan[n]);
		
	//Calculate the rashi in which the moon is present
	tmlon = REV(mlon+ayanamsa);
	strcpy(pdata->drashi,rashi[(int)(tmlon/30)]);
}
