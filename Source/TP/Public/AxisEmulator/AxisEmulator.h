// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TP_API AxisEmulator
{
public:
	AxisEmulator();
	~AxisEmulator();

	float UpdateValue(float Value);

private:
	float Sensitivity = 0.1f;
	float Gravity = 0.02f;
	float DeadZone = 0.05f;
	float CurrentValue = 0.f;
};
