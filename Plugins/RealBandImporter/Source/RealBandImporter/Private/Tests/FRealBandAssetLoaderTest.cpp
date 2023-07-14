//Copyright(C) 2022 VAXPORT all rights reserved.
#pragma once

#include "../UI/FRealBandAssetLoader.h"
#include "Misc/AutomationTest.h"

#define LOCTEXT_NAMESPACE "FRealBandAssetLoaderTests"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRealBandAssetLoaderTest, "RealBandGroup.AssetLoader.Placeholder Test", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)


bool FRealBandAssetLoaderTest::RunTest(const FString& Parameters)
{
    // Make the test pass by returning true, or fail by returning false.
    return true;
}

#undef LOCTEXT_NAMESPACE