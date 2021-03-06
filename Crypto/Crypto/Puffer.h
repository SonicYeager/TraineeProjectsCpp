#pragma once

class Puffer
{
public:
	Puffer(char* content) 
		: mp_var(content)
	{}
	Puffer(char* content, bool keepPtr)
		: mp_var(content), keep(keepPtr)
	{}

	~Puffer() 
	{
		if (!keep) {
			delete[] mp_var;
}
	}

	auto GetChar() -> char*;
	auto SetChar() -> char*&;

private:
	bool keep = false;
	char* mp_var = nullptr;
};

