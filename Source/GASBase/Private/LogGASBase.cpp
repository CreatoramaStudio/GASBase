// Fill out your copyright notice in the Description page of Project Settings.

#include "LogGASBase.h"

DEFINE_LOG_CATEGORY(LogGASBase);

void FLogGASBase::Info(const FString& String)
{
	UE_LOG(LogGASBase, Display, TEXT("%s"), *String);
}

void FLogGASBase::Warning(const FString& String)
{
	UE_LOG(LogGASBase, Warning, TEXT("%s"), *String);
}

void FLogGASBase::Error(const FString& String)
{
	UE_LOG(LogGASBase, Error, TEXT("%s"), *String);
}