// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingMatrix.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

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
        UE_LOG(LogTemp, Error, TEXT("Could not open CraftingMatrix.csv: %s"), UTF8_TO_TCHAR(strerror(errno)));
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

    // Now every future line will start with the key, followed by the values
    while (getline(file, line))
    {
        int resourceKey1Idx = 1;
        istringstream iss2(line);
        getline(iss2, resourceKey2, ',');
        while (getline(iss2, resourceVal, ','))
        {
            resourceKey1 = m_resources[resourceKey1Idx];
            m_matrix[resourceKey1][resourceKey2] = resourceVal;
            m_matrix[resourceKey2][resourceKey1] = resourceVal;
            UE_LOG(LogTemp, Display, TEXT("Crafting Matrix: %s + %s = %s"), UTF8_TO_TCHAR(resourceKey1.c_str()), UTF8_TO_TCHAR(resourceKey2.c_str()), UTF8_TO_TCHAR(resourceVal.c_str()));
            resourceKey1Idx++;
        }
    }
}