#pragma once
#include <Vector3.h>
#include <map>
#include <string>
#include <variant>
class GlobalVariables {
public:
	static GlobalVariables* GetInstance();

	void CreateGroup(const std::string& groupName);

	void SetValue(const std::string& groupName, const std::string& key, int32_t value);

	void SetValue(const std::string& groupName, const std::string& key, float value);

	void SetValue(const std::string& groupName, const std::string& key, const Vector3 value);

private:
	GlobalVariables();
	~GlobalVariables();
	GlobalVariables(const GlobalVariables& obj) = delete;
	GlobalVariables& operator=(const GlobalVariables& obj) = delete;

private:
	struct Item {
		std::variant<int32_t, float, Vector3> value;
	};

	struct Group {
		std::map<std::string, Item> items;
	};

	std::map<std::string, Group> datas_;
};