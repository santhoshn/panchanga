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

//Fragments the string based on the given character and returns the number of fragments
int strsplit(char *str, char c)
{
	int n=0;
	char *tmp;

	for(tmp=str; *tmp; tmp++)
		if(*tmp==c) {
			*tmp=' ';
			n++;
		}

	return n+1;
}

void show_usage(int ret)
{
	fprintf(stderr,"Usage: panchanga -d dd/mm/yyy -t hh:mm -z [+/-]hh:mm\n");
	fprintf(stderr,"-d : Date in dd/mm/yyyy format.\n");
	fprintf(stderr,"-t : Time in hh:mm 24-hour format.\n");
	fprintf(stderr,"-z : Zone with respect to GMT in [+/-]hh:mm format.\n");
	exit(ret);
}

int main(int argc, char **argv)
{
	char date[20]={0}, time[20]={0}, zone[20]={0};
	double dd, hr, zhr;
	int mm, yy, mn, zmn, c;
	struct panchanga pdata;
	
	opterr = 0;
	while((c=getopt(argc,argv,"d:t:z:h"))!=-1) {
     		switch(c) {
			case 'd':
				strcpy(date,optarg);
				break;
			case 't':
				strcpy(time,optarg);
				break;
			case 'z':
				strcpy(zone,optarg);
				break;
			case 'h':
				show_usage(0);
			case '?':
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
				exit(1);
		      	default:
				abort();
		}
        }

	if(!date[0] || !time[0] || !zone[0])
		show_usage(1);

	//Extract date, time and zone, also do some preprocessing
	if(strsplit(date,'/')!=3)
		show_usage(1);
	sscanf(date,"%lf %d %d",&dd,&mm,&yy);

	if(strsplit(time,':')!=2)
		show_usage(1);
	sscanf(time,"%lf %d",&hr,&mn);
	hr+=mn/60.0;

	if(strsplit(zone,':')!=2)
		show_usage(1);
	sscanf(zone,"%lf %d",&zhr,&zmn);
	zhr+=(zhr<0.0)?-(zmn/60.0):(zmn/60.0);

	// Calculate Panchanga
	calculate_panchanga(dd, mm, yy, hr, zhr, &pdata);
	printf("Tithi     : %s, %s Paksha\n", pdata.dtithi, pdata.dpaksha);
	printf("Nakshatra : %s\n", pdata.dnakshatra);
	printf("Yoga      : %s\n", pdata.dyoga);
	printf("Karana    : %s\n", pdata.dkarana);
	printf("Rashi     : %s\n", pdata.drashi);
}
