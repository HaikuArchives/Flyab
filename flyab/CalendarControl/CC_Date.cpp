//CC_Date.cpp
     
#ifndef CC_DATE_H
#define CC_DATE_H

class CC_Date
{
	private:
     		int day;
     		int month;
     		int year;
    		void inkrement();
     		bool isLeapYear(int j) const;
     		bool isValid(int t, int m, int j) const;
    	public:
     		CC_Date(int t, int m, int j);
     		void setDate(int t, int m, int j);
     		void ausgabe() const;

		int month_days() const;
		int get_day() const;
		int get_month() const;
		int get_year() const;
		int get_weekday(int d) const;
		int get_realdate(int x) const;

     		bool operator<(const CC_Date &d) const;
     		void operator++();
     		CC_Date &operator+=(int t);
};
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////

      
//#include <iostream>
#include <math.h>

using namespace std;


CC_Date::CC_Date(int t, int m, int j)
{
	setDate(t,m,j);
}
  
bool CC_Date::isLeapYear(int j) const
{
     	if(j%400==0 || (j%4==0 && j%100!=0))
       	return true;
     	return false;
}
  
bool CC_Date::isValid(int t, int m, int j) const
{	
     	if(m<1 || m>12)
                                     
    	return false;
	
  	int day_in_month[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  	if(isLeapYear(j))
    	day_in_month[2]++;

  	if(t<1 || t>day_in_month[m])
    	return false;

	return true;
	
}

void CC_Date::setDate(int t, int m, int j)
{
	

  	if(isValid(t,m,j))
  	{
    		day = t;
    		month = m;
    		year = j;
  	}
  	else
  	{
		
    		day = get_realdate(1);
    		month = get_realdate(2);
    		year = get_realdate(3);
  	}
}

void CC_Date::ausgabe() const
{
  	cout << "ausgabe: " << day << "." << month << "." << year << endl;
}

////modifits
int CC_Date::month_days() const
{
	int day_in_month[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };	
	return day_in_month[month];
}

int CC_Date::get_weekday(int d) const
{
	/*Weekdays:
		0 = So
		1 = Mo
		2 = Di
		3 = Mi
		4 = Do
		5 = Fr
		6 = Sa
	See the weekday Calc from Gaußsche , url =  "http://de.wikipedia.org/wiki/Gau%C3%9Fsche_Wochentagsformel" */

	int c = year/100;
	int y = year%100;
	
	int m;
	switch (month)
	{
		case 1:
			m = 11;
			y = y-1;
			break;
		case 2:
			m = 12;
			y = y-1;
			break;
		 default:
    			m = month - 2;
		     	break;
	}

	double mc = 2.6*m-0.2;
	int A = d+(int)mc+y+(y/4)+(c/4)-2*c;
    	int w = A%7;
	if (w < 0) w += 7;	
	return w;
}

int CC_Date::get_day() const
{
  	return day; 
}

int CC_Date::get_month() const
{
  	return month; 
}

int CC_Date::get_year() const
{
  	return year; 
}

int CC_Date::get_realdate(int x) const
{
	
	struct tm *tmnow;
	time_t tnow;
	time(&tnow);
	tmnow = localtime(&tnow);
	
	int rd = tmnow->tm_mday;
	int rm = tmnow->tm_mon + 1;
	int ry = tmnow->tm_year + 1900;
	
	switch (x)
	{
		case 1:
			return rd;
			break;
		case 2:
			return rm;
			break;
		case 3:
			return ry;
			break;
		
		 default:
		     	break;
	}
}

/////

bool CC_Date::operator<(const CC_Date &d) const
{
  	if(year<d.year) return true;
  	if(year==d.year && month<d.month) return true;
  	if(year==d.year && month==d.month && day<d.day) return true;
  	return false;
}

void CC_Date::inkrement()
{
  	day++;
  	if(!isValid (day,month,year))
 	{
    		day = 1;
    		month++;
    		if(!isValid(day,month,year))
    		{
			month = 1;
         		year++;
       		}
     	}
}
 
void CC_Date::operator++()
{
	inkrement();
}
  
CC_Date &CC_Date::operator+=(int t)
{
	for(int i=0; i<t; i++)
	inkrement();
     	return *this;
}
