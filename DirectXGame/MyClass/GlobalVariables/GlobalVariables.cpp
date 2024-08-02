#include "GlobalVariables.h"

GlobalVariables* GlobalVariables::GetInstance() { 
	static GlobalVariables instance;
	return &instance;
}

void GlobalVariables::CreateGroup(const std::string& groupName) { 
	datas_[groupName];
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value) { 
	Group& group = datas_[groupName];
	Item newItem{};
	newItem.value = value;
	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {
	Group& group = datas_[groupName];
	Item newItem{};
	newItem.value = value;
	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3 value) {
	Group& group = datas_[groupName];
	Item newItem{};
	newItem.value = value;
	group.items[key] = newItem;
}

GlobalVariables::GlobalVariables() {}

GlobalVariables::~GlobalVariables() {}
