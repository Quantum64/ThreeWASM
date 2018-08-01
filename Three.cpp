#include <vector>
#include <string>

enum class Operation {
	Add,
	Subtract,
	Multiply,
	Floordiv,
	Celidiv,
	Mod,
	Power,
	Bitwise,
	ShiftRight,
	ShiftLeft,
	Or
};

// Header
std::wstring getSign(Operation operation);
bool canProcess(Operation operation, int u, int z);
int process(Operation operation, int u, int z);
std::vector<Operation> getOperations(unsigned int iteration);

// Source

extern "C" const wchar_t* calculate(const char* string, long deapth) {
	std::string input(string);
	const unsigned int count = input.length();
	std::vector<unsigned char> digits;
	digits.reserve(count);
	for (unsigned int index = 0; index < count; index++) {
		digits.push_back(input[index] - '0');
	}

	for (unsigned int iteration = 0; iteration < 4; iteration++) {
		if (iteration == 0) {
			bool even = true;	
			for (char digit : digits) {
				if ((digit % 2) == 1) {
					even = false;
					break;
				}
			}
			if (even) {
				iteration++;
			}
		}
		std::vector<Operation> operations = getOperations(iteration);
		const unsigned int operationCount = operations.size();
		unsigned int* permutation = new unsigned int[count]();
		unsigned int currentRow;
		unsigned long long maxAttempts = 1;
		const long maxAttemptsPerIteration = deapth;
		for (unsigned int index = 0; index < count; index++) {
			maxAttempts *= operationCount;
			if (maxAttempts < 0 || maxAttempts > maxAttemptsPerIteration) {
				maxAttempts = maxAttemptsPerIteration;
				break;
			}
		}
		for (unsigned long long j = 0; j < maxAttempts; j++) {
			currentRow = 0;
			int current = operations[permutation[0]] == Operation::Multiply ? 1 : 0;
			while (currentRow < count) {
				Operation currentOperation = operations[permutation[currentRow]];	
				if (!canProcess(currentOperation, current, digits[currentRow])) {
					current = 0;
					break;
				}
				current = process(currentOperation, current, digits[currentRow]);
				currentRow++;
			}
			if (current == 3) {
				std::vector<Operation> finalOperations;
				finalOperations.reserve(operationCount);
				for (unsigned int digit = 0; digit < count; digit++) {
					finalOperations.push_back(operations[permutation[digit]]);
				}
				std::wstring result = L"";
				for (unsigned int row = 0; row < count; row++) {
					Operation operation = finalOperations[row];
					if (operation != Operation::Add && operation != Operation::Subtract) {
						result = result + L")";
						result = L"(" + result;
					}
					if (row == 0 && (operation == Operation::Multiply || operation == Operation::Add)) {
						result = result + std::to_wstring(digits[row]);
					}
					else {
						result = result + getSign(operation) + std::to_wstring(digits[row]);
					}
					if (operation == Operation::Floordiv) {
						result = result + L"⌋";
						result =  L"⌊" + result;
					}
					if (operation == Operation::Celidiv) {
						result = result + L"⌉";
						result = L"⌈" + result;
					}
				}

				delete[] permutation;
				result = result + L"=3";
				return result.c_str();
			}
			currentRow = 0;
			while (currentRow < count) {
				if (permutation[currentRow] < operationCount - 1) {
					permutation[currentRow]++;
					break;
				}
				else {
					permutation[currentRow] = 0;
				}
				currentRow++;
			}
		}
		delete[] permutation;
	}
	return L"No solution found!";
}

std::wstring getSign(Operation operation) {
	switch (operation) {
	case Operation::Subtract:
		return L"-";
	case Operation::Multiply:
		return L"*";
	case Operation::Mod:
		return L"%";
	case Operation::Add:
		return L"+";
	case Operation::Power:
		return L"^";
	case Operation::Bitwise:
		return L"&";
	case Operation::ShiftRight:
		return L">>";
	case Operation::ShiftLeft:
		return L"<<";
	case Operation::Or:
		return L"|";
	case Operation::Floordiv:
		return L"/";
	case Operation::Celidiv:
		return L"/";
	default:
		break;
	}
	return L"";
}

bool canProcess(Operation operation, int u, int z) {
	switch (operation) {
	case Operation::Floordiv:
		return u != 0 && z != 0;
	case Operation::Celidiv:
		return u != 0 && z != 0;
	case Operation::Power:
		return u != 0 && z >= 0 && u > -10 && u < 10;
	case Operation::Mod:
		return u > 0 && z > 0;
	default:
		break;
	}
	return true;
}

int process(Operation operation, int u, int z) {
	switch (operation) {
	case Operation::Subtract:
		return u - z;
	case Operation::Multiply:
		return u * z;
	case Operation::Mod:
		return u % z;
	case Operation::Add:
		return u + z;
	case Operation::Power:
	{
		int r = 1;
		for (int i = 0; i < z; i++) {
			r *= u;
		}
		return r; 
	}
	case Operation::Bitwise:
		return u & z;
	case Operation::ShiftRight:
		return u >> z;
	case Operation::ShiftLeft:
		return u << z;
	case Operation::Or:
		return u | z;
	case Operation::Floordiv:
		return u / z - (((u > 0) ^ (z > 0)) && (u % z));
	case Operation::Celidiv:
		return u / z + (((u < 0) ^ (z > 0)) && (u % z));
	default:
		break;
	}
	return 0;
}

std::vector<Operation> getOperations(unsigned int iteration) {
	std::vector<Operation> operations;
	switch (iteration) {
	case 3:
		operations.push_back(Operation::Bitwise);
		operations.push_back(Operation::ShiftLeft);
		operations.push_back(Operation::ShiftRight);
	case 2:
		operations.push_back(Operation::Mod);
	case 1:
		operations.push_back(Operation::Power);
		operations.push_back(Operation::Floordiv);
		operations.push_back(Operation::Celidiv);
	}
	operations.push_back(Operation::Add);
	operations.push_back(Operation::Subtract);
	operations.push_back(Operation::Multiply);
	return operations;
}