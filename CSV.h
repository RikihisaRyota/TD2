#pragma once

#include <sstream>
#include <vector>

#include "GameScene.h"
#include "Vector3.h"

// CSVはGameSceneへ移植しました //

class CSV {
public:
	struct Data {
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
	std::vector<Data> UpdateDataCommands();

	/// <summary>
	/// データの書き込み
	/// </summary>
	void WritingData(std::string fileName, std::vector<Data> datas);

private:
	// 発生コマンド
	std::stringstream dateCommands_;
};
