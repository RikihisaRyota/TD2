#pragma once

#include <sstream>
#include <vector>

#include "Vector3.h"

class CSV {
public:
	struct Date {
		Vector3 position;
		uint32_t type;
	};

public:
	/// <summary>
	/// データの読み込み
	/// </summary>
	void LoadCSV(std::string fileName);

	/// <summary>
	/// データの更新
	/// </summary>
	std::vector<Date> UpdataDateCommands();

private:
	// 発生コマンド
	std::stringstream dateCommands_;
};
