#include "Calendar.h"

#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstdlib>

int main(int argc, char** argv)
{
	int month, year;
	if (argc == 1)
	{
		time_t now = time(NULL);
		struct tm* date = localtime(&now);
		month = date->tm_mon + 1;
		year = date->tm_year + 1900;
	}
	else if (argc == 3)
	{
		char* endptr;
		month = (int)strtol(argv[1], &endptr, 10);
		if (endptr == argv[1])
		{
			fprintf(stderr, "Invalid month!\n");
			return -1;
		}
		year = (int)strtol(argv[2], &endptr, 10);
		if (endptr == argv[2])
		{
			fprintf(stderr, "Invalid year!\n");
			return -1;
		}
	}
	else
	{
		printf("Usage: %s [month] [year]\n\nMonth and Year must either be ommitted or present, there can't be either or!\n", argv[0]);
		return 0;
	}

	Calendar cal(month, year);
	cal.print();
	return 0;
}
