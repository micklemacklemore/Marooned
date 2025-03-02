// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <unordered_map>
#include <string>
#include <vector>

using std::string;
using std::unordered_map;
using std::vector;

/**
 * 
 */
class MAROONED_API CraftingMatrix
{
public:
	static void Initialize();
	static string GetCraftingResult(string resource1, string resource2);
	static constexpr const char* NONE = "None";

private:
	CraftingMatrix() = delete;
	~CraftingMatrix() = delete;

	static vector<string> m_resources;
	static unordered_map<string, unordered_map<string, string>> m_matrix;
};