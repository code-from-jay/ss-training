#include <string>
#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <random> // std::default_random_engine, std::uniform_int_distribution
#include <functional> // std::bind
#include <chrono> // std::chrono::system_clock

using std::chrono::system_clock;

int roll()
{
	static auto now = system_clock::now().time_since_epoch().count();
	static std::default_random_engine gen{now};
	static std::uniform_int_distribution<int> dist{1, 100};
	
	return dist(gen);
}

struct Student
{
	std::string studentId;
	int score;

	static bool comparator(const Student& x, const Student& y)
	{
		return x.score < y.score;
	}
};

class MergeSort
{
public:
	void sort(std::vector<Student>& students)
	{
		merge_sort(students, 0, students.size()-1);
	}

private:
	void merge_sort(std::vector<Student>& students, int left, int right)
	{
		if (left < right)
		{
			int middle = left + (right - left) / 2;
			merge_sort(students, left, middle);
			merge_sort(students, middle+1, right);
			merge(students, left, middle, right);
		}
	}

	void merge(std::vector<Student>& students, int left, int middle, int right)
	{
		int leftSliceLen = (middle - left) + 1;
		int rightSliceLen = (right - middle);

		std::vector<Student> leftSlice
		{
			students.begin()+left,
			students.begin()+left+leftSliceLen
		};
		std::vector<Student> rightSlice
		{
			students.begin()+middle+1,
			students.begin()+middle+1+rightSliceLen
		};

		int i = 0, j = 0, k = left;
		for (; i < leftSliceLen && j < rightSliceLen; ++k)
		{
			if (Student::comparator(leftSlice[i], rightSlice[j]))
			{
				students[k] = leftSlice[i];
				++i;
			}
			else
			{
				students[k] = rightSlice[j];
				++j;
			}
		}

		for (; i < leftSliceLen; ++i, ++k) // Handle extra elements in leftSlice
		{
			students[k] = leftSlice[i];
		}

		for (; j < rightSliceLen; ++j, ++k)
		{
			students[k] = rightSlice[j];
		}
	}
};

int main(void)
{
	std::vector<Student> students =
	{
		Student{"Valentine", roll()}, Student{"Ralph", roll()},
		Student{"Landis", roll()}, Student{"Chesley", roll()},
		Student{"Shelton", roll()}, Student{"Arick", roll()},
		Student{"Lyle", roll()}, Student{"Shawn", roll()},
		Student{"Lacy", roll()}, Student{"Ainsleigh", roll()}
	};

	std::cout << "Unsorted Students\n-----------------\n";
	for (const auto& student : students)
	{
		std::cout
			<< "Name: " << student.studentId
			<< ", Score: " << student.score
			<< "\n";
	}

	MergeSort ms;
	ms.sort(students);

	assert(std::is_sorted(students.begin(), students.end(), Student::comparator));

	std::cout << "\nSorted Students\n-----------------\n";
	for (const auto& student : students)
	{
		std::cout
			<< "Name: " << student.studentId
			<< ", Score: " << student.score
			<< "\n";
	}
}
