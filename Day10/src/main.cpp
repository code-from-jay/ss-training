#include <string>
#include <iostream>
#include <cassert>

class Person
{
public:
	std::string name;
	int count;

	static Person* persons[10];
	static Person* getPerson(std::string name)
	{
		int emptyIndex = -1;
		for (int i = 0; i < 10; ++i)
		{
			if (persons[i])
			{
				if (persons[i]->name == name)
				{
					Person* person = new Person(std::move(name));
					delete persons[i];
					persons[i] = person;
					return person;
				}
			}
			else
			{
				emptyIndex = i;
			}
		}

		assert(emptyIndex != -1); // No more room!
		persons[emptyIndex] = new Person(std::move(name));
		return persons[emptyIndex];
	}
private:
	Person(std::string name) : name(std::move(name)), count(1)
	{
		for (int i = 0; i < 10; ++i)
		{
			if (persons[i] && persons[i]->name == this->name)
			{
				this->count = persons[i]->count + 1;
				break;
			}
		}
	}
};

// Not allowed to initialize this inside of the class :y
Person* Person::persons[10] = {};

int main(void)
{
	std::string names[5] = { "bob", "james", "janet", "bob", "stella" };

	for (int i = 0; i < 5; ++i)
	{
		Person* person = Person::getPerson(names[i]);
		if (i != 0)
			std::cout << ", ";
		std::cout << person->name << ": " << person->count;
	}
	std::cout << "\n";

	for (int i = 0; i < 10; ++i)
	{
		// Cleanup
		if (Person::persons[i])
			delete Person::persons[i];
	}
	return 0;
}
