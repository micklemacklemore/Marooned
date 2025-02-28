// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using std::unordered_map;
using std::string;
using std::ifstream;
using std::istringstream;
using std::vector;

/**
 * 
 */
class MAROONED_API CraftingMatrix
{
public:
	static void Initialize();

private:
	CraftingMatrix() = delete;
	~CraftingMatrix() = delete;

	static vector<string> m_resources;
	static unordered_map<string, unordered_map<string, string>> m_matrix;
};