#include "Calendar.h"

#include <ctime>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <iostream>

const char* month_to_string(int month)
{
	static const char* month_strings[] =
	{
		"January", "February", "March",
		"April", "May", "June",
		"July", "August", "September",
		"October", "November", "December"
	};

	if (month < 1 || month > 12)
		return "Invalid";
	return month_strings[month - 1];
}

bool is_leap_year(int year)
{
	/**
	 * For a year to be a leap year, the year *must* be divisible by 4
	 * and if the year is divisible by 100, it must also be divisible by 400
	 * to be considered a leap year.
	 */
	return (year % 4 == 0 && (year % 100 != 0 || (year % 100 == 0 && year % 400 == 0)));
}

int days_in_month(int month, int year)
{
	static int days_in_month_arr[] =
	{
		31, 28, 31,
		30, 31, 30,
		31, 31, 30,
		31, 30, 31
	};

	if (month == 2 && is_leap_year(year))
		return days_in_month_arr[month - 1] + 1;
	return days_in_month_arr[month - 1];
}

Calendar::Calendar(int month, int year)
{
	this->set(month, year);
}

Calendar::~Calendar()
{
	int** ptr = this->m_calendar;
	while (*ptr)
	{
		delete[] *ptr;
		ptr++;
	}
	delete[] this->m_calendar;
	this->m_calendar = nullptr;
}

void Calendar::set(int month, int year)
{
	struct tm date = {0};
	date.tm_year = year - 1900;
	date.tm_mon = month - 1;
	date.tm_mday = 1;
	std::cout << "Year: " << date.tm_year << "\n";
	std::cout << "time_t: " <<  mktime(&date) << "\n";

	int days = days_in_month(month, year);
	int slots = days + date.tm_wday;
	int rows = (int)ceil(slots / 7.0);
	if (this->m_set)
	{
		int** ptr = this->m_calendar;
		while (*ptr)
		{
			delete[] *ptr;
			ptr++;
		}
		delete[] this->m_calendar;
		this->m_calendar = nullptr;
	}

	this->m_calendar = new int*[rows + 1];
	for (int i = 0; i < rows; ++i)
	{
		this->m_calendar[i] = new int[7];
		memset(this->m_calendar[i], 0, 7 * sizeof(int));
	}
	this->m_calendar[rows] = nullptr;

	int x = date.tm_wday;
	int y = 0;

	for (int i = 1; i <= days; ++i)
	{
		this->m_calendar[y][x++] = i;
		if (x > 6)
		{
			x = 0;
			++y;
		}
	}

	this->m_set = true;
	this->m_month = month;
	this->m_year = year;
}

int** Calendar::get() const
{
	return this->m_calendar;
}

void Calendar::print() const
{
	const char* month = month_to_string(this->m_month);
	int monthlen = strlen(month);
	int padlen = (20 - (monthlen+5))/2;
	printf("%*s%s %04d%*s\n", padlen, "", month, this->m_year, padlen, "");
	printf("Su Mo Tu We Th Fr Sa\n");
	int** ptr = this->m_calendar;
	while (*ptr)
	{
		int* inner_ptr = *ptr;
		for (int i = 0; i < 7; ++i)
		{
			if (i != 0)
				printf(" ");
			if (inner_ptr[i] == 0)
				printf("  ");
			else
				printf("%2d", inner_ptr[i]);
		}
		printf("\n");
		ptr++;
	}
	printf("\n");
}
