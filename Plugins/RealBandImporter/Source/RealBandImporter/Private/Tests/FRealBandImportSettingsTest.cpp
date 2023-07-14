//Copyright(C) 2022 VAXPORT all rights reserved.
#pragma once

#include "../Settings/RealBandImportSettings.h"
#include "Misc/AutomationTest.h"

#define LOCTEXT_NAMESPACE "RealBandImportSettingsTests"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(RealBandImportSettingsTest, "RealBandGroup.ImporterSettings.Placeholder Test", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)


bool RealBandImportSettingsTest::RunTest(const FString& Parameters)
{
    // Make the test pass by returning true, or fail by returning false.
    return true;
}
#undef LOCTEXT_NAMESPACE