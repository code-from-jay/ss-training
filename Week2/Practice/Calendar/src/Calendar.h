#ifndef CALENDAR_H
#define CALENDAR_H

class Calendar
{
	int** m_calendar;
	bool m_set = false;
	int m_month;
	int m_year;
public:
	Calendar(int month, int year);
	~Calendar();
	
	int** get() const;
	void print() const;
	void set(int month, int year);
};

#endif
