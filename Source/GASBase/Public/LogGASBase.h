// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogGASBase, Log, All);

class FLogGASBase
{
public:
	static void Info(const FString& String);

	static void Warning(const FString& String);

	static void Error(const FString& String);
};
