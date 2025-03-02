// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingMatrix.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include <fstream>
#include <sstream>
#include <array>

using std::ifstream;
using std::istringstream;

vector<string> CraftingMatrix::m_resources;
unordered_map<string, unordered_map<string, string>> CraftingMatrix::m_matrix;

void CraftingMatrix::Initialize()
{
    UE_LOG(LogTemp, Display, TEXT("Initializing Crafting Matrix"));

    FString ProjectDir = FPaths::ProjectDir();
    FString FullPath = FPaths::ConvertRelativePathToFull(ProjectDir + "Data/CraftingMatrix.csv");
    ifstream file(TCHAR_TO_UTF8(*FullPath));

    if (!file.is_open())
    {
        std::array<char, 256> buffer;
        strerror_s(buffer.data(), buffer.size(), errno);
        UE_LOG(LogTemp, Error, TEXT("Could not open CraftingMatrix.csv: %s"), UTF8_TO_TCHAR(buffer.data()));
        return;
    }

    string line;

    // Get the first line: contains all the keys of the crafting matrix
    getline(file, line);
    istringstream iss(line);
    string resourceKey1, resourceKey2, resourceVal;
    getline(iss, resourceKey1, ','); // first entry is empty
    while (getline(iss, resourceKey1, ','))
    {
        m_matrix[resourceKey1] = unordered_map<string, string>();
        m_resources.push_back(resourceKey1);
    }

    // Now every future line will start with the secondary key, followed by the values
    int resourceKey2Idx = 0;
    while (getline(file, line))
    {
        int resourceKey1Idx = 0;
        istringstream iss2(line);
        getline(iss2, resourceKey2, ',');
        while (getline(iss2, resourceVal, ','))
        {
            if (resourceKey1Idx > resourceKey2Idx) break; // Only fill the lower triangle of the matrix
            if (resourceVal.empty()) resourceVal = NONE;
            resourceKey1 = m_resources[resourceKey1Idx];
            m_matrix[resourceKey1][resourceKey2] = resourceVal;
            m_matrix[resourceKey2][resourceKey1] = resourceVal;
            UE_LOG(LogTemp, Display, TEXT("Crafting Matrix: %s + %s = %s"), UTF8_TO_TCHAR(resourceKey1.c_str()), UTF8_TO_TCHAR(resourceKey2.c_str()), UTF8_TO_TCHAR(resourceVal.c_str()));
            resourceKey1Idx++;
        }
        resourceKey2Idx++;
    }
}

string CraftingMatrix::GetCraftingResult(string resource1, string resource2)
{
    check(m_matrix.find(resource1) != m_matrix.end() && "Resource1 does not exist in the crafting matrix");
    check(m_matrix[resource1].find(resource2) != m_matrix[resource1].end() && "Resource2 does not exist in the crafting matrix");

    return m_matrix[resource1][resource2];
}