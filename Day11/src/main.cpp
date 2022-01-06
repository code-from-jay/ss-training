#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <cassert>

class Sentence
{
	char* sentence;
	std::vector<int> vsentence;
public:
	Sentence(const char* sent)
	{
		deep_copy_str(&sentence, sent);

		generate_vector();
	}

	Sentence(const Sentence& other)
	{
		deep_copy_str(&sentence, other.sentence);

		vsentence = std::vector<int>(other.vsentence);
	}

	Sentence(Sentence&& other)
	{
		// Just take the other class's pointer
		// Which is the equivalent of just doing:
		//   this->sentence = other.sentence;
		//   other.sentence = nullptr;
		this->sentence = std::exchange(other.sentence, nullptr);
		vsentence = std::move(other.vsentence);
	}

	Sentence& operator=(const Sentence& other)
	{
		if (sentence) delete[] sentence;
		deep_copy_str(&sentence, other.sentence);

		vsentence = std::vector<int>(other.vsentence);
		
		return *this;
	}

	Sentence& operator=(Sentence&& other)
	{
		if (sentence) delete[] sentence;

		sentence = std::exchange(other.sentence, nullptr);
		vsentence = std::move(other.vsentence);

		return *this;
	}

	inline char* data()
	{
		return this->sentence;
	}

	int num_words() const
	{
		return vsentence.size();
	}

	std::string get_word(int n)
	{
		int endpos;
		int startpos = vsentence[n];
		int len = strlen(sentence);

		for (endpos = startpos+1; endpos < len && sentence[endpos] != ' '; ++endpos);

		/**
		 * Learned something new when I was adding warning/optimization flags,
		 * turns out the C++ compiler is smart enough to automatically
		 * move/copy/construct temporary variables to the return location.
		 *
		 * Thus making my explicit std::move unneeded.
		 */
		return std::string(sentence + startpos, endpos - startpos);
	}

	void replace(char* sent)
	{
		if (sentence) delete[] sentence;
		deep_copy_str(&sentence, sent);
	}

	~Sentence()
	{
		if (sentence) delete[] sentence;
	}

private:
	static void deep_copy_str(char** destination, const char* source)
	{
		size_t len = strlen(source);
		*destination = new char[len+1];
		strcpy(*destination, source);
	}

	void generate_vector()
	{
		vsentence.push_back(0);
		int len = strlen(sentence);

		for (int i = 0; i < len; ++i)
		{
			if (sentence[i] == ' ')
			{
				// Skip any extra spaces
				int temp = i+1;
				while (sentence[temp] == ' ')
					temp++;
				// -1 because the loop will automatically increment i
				i = temp-1;
				vsentence.push_back(temp);
			}
		}
	}
};

int main(void)
{
	Sentence a{"Quick brown fox jumps over the lazy dog."};
	assert(a.num_words() == 8);
	assert(a.get_word(0) == "Quick");

	Sentence b{a}; // Copy constructor
	assert(b.num_words() == 8);
	assert(b.get_word(1) == "brown");

	Sentence c{"Some string that's going to vanish."};
	assert(c.num_words() == 6);
	assert(c.get_word(5) == "vanish.");
	c = a; // Copy assignment
	assert(c.num_words() == 8);
	assert(c.get_word(5) == "the");

	Sentence d{std::move(c)}; // Move constructor
	assert(c.num_words() == 0);
	assert(d.num_words() == 8);

	Sentence e{"Another string that's going to go."};
	assert(e.num_words() == 6);
	e = std::move(b); // Move assignment
	assert(b.num_words() == 0);
	assert(e.num_words() == 8);

	return 0;
}
