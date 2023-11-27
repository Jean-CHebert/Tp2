// Fill out your copyright notice in the Description page of Project Settings.


#include "AxisEmulator/AxisEmulator.h"

AxisEmulator::AxisEmulator()
{
}

AxisEmulator::~AxisEmulator()
{
}

float AxisEmulator::UpdateValue(float Value)
{
	//The input Value is there to reduce the effect of the simulation on controller
	//Since it would be too ineffective at this point to rebind all controller inputs
	if (Value != 1 && Value != -1) {
		CurrentValue = Value;
		return CurrentValue;
	}
	if (Value == 1 || Value == -1) {
		CurrentValue = std::clamp(CurrentValue + Sensitivity * Value, -1.f, 1.f);
	}
	else if (CurrentValue >= DeadZone) {
		CurrentValue -= Gravity;
	}
	else if (CurrentValue >= DeadZone) {
		CurrentValue += Gravity;
	}
	else {
		CurrentValue = 0;
	}
	
	return CurrentValue;
}
