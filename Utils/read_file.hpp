#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace Utils {

static std::vector<char> readDataFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error(std::string("Cannot open file: ").append(filename));
	}

	const size_t dataSize = file.tellg();
	std::vector<char> result(dataSize);
	file.seekg(0);

	file.read(result.data(), dataSize);

	return result;
}

static std::string readFile(const std::string& filename) {
	auto bytesData = readDataFile(filename);
	return std::string(bytesData.begin(), bytesData.end());
}

} // namespace Utils