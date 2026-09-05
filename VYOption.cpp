#include "VYOption.h"
#include "UObject/UnrealType.h"

DEFINE_LOG_CATEGORY(LogVYOption);

FVYOption UVYOptionLibrary::MakeNone()
{
	return FVYOption();
}

FVYOption UVYOptionLibrary::MakeSomeStruct(const int32& Value)
{
	checkNoEntry();
	(void)Value;
	return FVYOption();
}

DEFINE_FUNCTION(UVYOptionLibrary::execMakeSomeStruct)
{
	Stack.MostRecentPropertyAddress = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);
	const FProperty* ValueProp = Stack.MostRecentProperty;
	const void* ValuePtr = Stack.MostRecentPropertyAddress;
	P_FINISH;

	P_NATIVE_BEGIN;
	*static_cast<FVYOption*>(RESULT_PARAM) = Generic_MakeSomeStruct(ValueProp, ValuePtr);
	P_NATIVE_END;
}

FVYOption UVYOptionLibrary::Generic_MakeSomeStruct(const FProperty* ValueProp, const void* ValuePtr)
{
	FVYOption Option;

	const FStructProperty* StructProp = CastField<const FStructProperty>(ValueProp);
	if (StructProp && StructProp->Struct && ValuePtr)
	{
		Option.Payload.InitializeAs(StructProp->Struct, static_cast<const uint8*>(ValuePtr));
		Option.bIsSome = true;
	}
	else
	{
		UE_LOG(LogVYOption, Warning, TEXT("Make Some (Struct): connected pin is not a valid struct type - returning None."));
	}

	return Option;
}

FVYOption UVYOptionLibrary::MakeSomeObject(UObject* Value)
{
	FVYOption Option;
	if (IsValid(Value))
	{
		FVYOptionObject Boxed;
		Boxed.Value = Value;
		Option.Payload.InitializeAs<FVYOptionObject>(Boxed);
		Option.bIsSome = true;
	}
	return Option;
}

FVYOption UVYOptionLibrary::MakeSomeClass(TSubclassOf<UObject> Value)
{
	FVYOption Option;
	if (Value)
	{
		FVYOptionClass Boxed;
		Boxed.Value = Value;
		Option.Payload.InitializeAs<FVYOptionClass>(Boxed);
		Option.bIsSome = true;
	}
	return Option;
}

FVYOption UVYOptionLibrary::MakeSomeBool(bool Value)
{
	FVYOption Option;
	Option.Payload.InitializeAs<FVYOptionBool>(FVYOptionBool{ Value });
	Option.bIsSome = true;
	return Option;
}

FVYOption UVYOptionLibrary::MakeSomeInt(int32 Value)
{
	FVYOption Option;
	Option.Payload.InitializeAs<FVYOptionInt>(FVYOptionInt{ Value });
	Option.bIsSome = true;
	return Option;
}

FVYOption UVYOptionLibrary::MakeSomeFloat(double Value)
{
	FVYOption Option;
	Option.Payload.InitializeAs<FVYOptionFloat>(FVYOptionFloat{ Value });
	Option.bIsSome = true;
	return Option;
}

FVYOption UVYOptionLibrary::MakeSomeString(const FString& Value)
{
	FVYOption Option;
	FVYOptionString Boxed;
	Boxed.Value = Value;
	Option.Payload.InitializeAs<FVYOptionString>(Boxed);
	Option.bIsSome = true;
	return Option;
}

bool UVYOptionLibrary::IsSome(const FVYOption& Option)
{
	return Option.IsSome();
}

bool UVYOptionLibrary::IsNone(const FVYOption& Option)
{
	return Option.IsNone();
}

bool UVYOptionLibrary::UnwrapStruct(const FVYOption& Option, int32& OutValue)
{
	checkNoEntry();
	(void)Option;
	(void)OutValue;
	return false;
}

DEFINE_FUNCTION(UVYOptionLibrary::execUnwrapStruct)
{
	P_GET_STRUCT_REF(FVYOption, Option);

	Stack.MostRecentPropertyAddress = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);
	const FStructProperty* ValueProp = CastField<FStructProperty>(Stack.MostRecentProperty);
	void* ValuePtr = Stack.MostRecentPropertyAddress;
	P_FINISH;

	P_NATIVE_BEGIN;
	*static_cast<bool*>(RESULT_PARAM) = Generic_UnwrapStruct(Option, ValueProp, ValuePtr);
	P_NATIVE_END;
}

bool UVYOptionLibrary::Generic_UnwrapStruct(const FVYOption& Option, const FStructProperty* ValueProp, void* ValuePtr)
{
	if (Option.IsSome() && ValueProp && ValueProp->Struct && ValuePtr)
	{
		if (ValueProp->Struct == Option.GetStoredScriptStruct())
		{
			ValueProp->Struct->CopyScriptStruct(ValuePtr, Option.Payload.GetMemory());
			return true;
		}

		UE_LOG(LogVYOption, Warning,
			TEXT("Unwrap (Struct): stored type '%s' does not match the requested type '%s'. Returning default value."),
			Option.GetStoredScriptStruct() ? *Option.GetStoredScriptStruct()->GetName() : TEXT("<none>"),
			*ValueProp->Struct->GetName());
	}

	return false;
}

void UVYOptionLibrary::MatchStruct(const FVYOption& Option, EVYOptionExecPin& Branches, int32& OutValue)
{
	checkNoEntry();
	(void)Option;
	(void)Branches;
	(void)OutValue;
}

DEFINE_FUNCTION(UVYOptionLibrary::execMatchStruct)
{
	P_GET_STRUCT_REF(FVYOption, Option);
	P_GET_ENUM_REF(EVYOptionExecPin, Branches);

	Stack.MostRecentPropertyAddress = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);
	const FStructProperty* ValueProp = CastField<FStructProperty>(Stack.MostRecentProperty);
	void* ValuePtr = Stack.MostRecentPropertyAddress;
	P_FINISH;

	P_NATIVE_BEGIN;
	Branches = Generic_UnwrapStruct(Option, ValueProp, ValuePtr) ? EVYOptionExecPin::Some : EVYOptionExecPin::None;
	P_NATIVE_END;
}

bool UVYOptionLibrary::UnwrapObject(const FVYOption& Option, UObject*& OutValue)
{
	OutValue = nullptr;
	if (Option.IsSome())
	{
		if (const FVYOptionObject* Boxed = Option.Payload.GetPtr<FVYOptionObject>())
		{
			OutValue = Boxed->Value;
			return IsValid(OutValue);
		}
	}
	return false;
}

void UVYOptionLibrary::MatchObject(const FVYOption& Option, EVYOptionExecPin& Branches, UObject*& OutValue)
{
	Branches = UnwrapObject(Option, OutValue) ? EVYOptionExecPin::Some : EVYOptionExecPin::None;
}

bool UVYOptionLibrary::UnwrapBool(const FVYOption& Option, bool& OutValue)
{
	OutValue = false;
	if (Option.IsSome())
	{
		if (const FVYOptionBool* Boxed = Option.Payload.GetPtr<FVYOptionBool>())
		{
			OutValue = Boxed->Value;
			return true;
		}
	}
	return false;
}

void UVYOptionLibrary::MatchBool(const FVYOption& Option, EVYOptionExecPin& Branches, bool& OutValue)
{
	Branches = UnwrapBool(Option, OutValue) ? EVYOptionExecPin::Some : EVYOptionExecPin::None;
}

bool UVYOptionLibrary::UnwrapInt(const FVYOption& Option, int32& OutValue)
{
	OutValue = 0;
	if (Option.IsSome())
	{
		if (const FVYOptionInt* Boxed = Option.Payload.GetPtr<FVYOptionInt>())
		{
			OutValue = Boxed->Value;
			return true;
		}
	}
	return false;
}

void UVYOptionLibrary::MatchInt(const FVYOption& Option, EVYOptionExecPin& Branches, int32& OutValue)
{
	Branches = UnwrapInt(Option, OutValue) ? EVYOptionExecPin::Some : EVYOptionExecPin::None;
}

bool UVYOptionLibrary::UnwrapFloat(const FVYOption& Option, double& OutValue)
{
	OutValue = 0.0;
	if (Option.IsSome())
	{
		if (const FVYOptionFloat* Boxed = Option.Payload.GetPtr<FVYOptionFloat>())
		{
			OutValue = Boxed->Value;
			return true;
		}
	}
	return false;
}

void UVYOptionLibrary::MatchFloat(const FVYOption& Option, EVYOptionExecPin& Branches, double& OutValue)
{
	Branches = UnwrapFloat(Option, OutValue) ? EVYOptionExecPin::Some : EVYOptionExecPin::None;
}

bool UVYOptionLibrary::UnwrapString(const FVYOption& Option, FString& OutValue)
{
	OutValue.Reset();
	if (Option.IsSome())
	{
		if (const FVYOptionString* Boxed = Option.Payload.GetPtr<FVYOptionString>())
		{
			OutValue = Boxed->Value;
			return true;
		}
	}
	return false;
}

void UVYOptionLibrary::MatchString(const FVYOption& Option, EVYOptionExecPin& Branches, FString& OutValue)
{
	Branches = UnwrapString(Option, OutValue) ? EVYOptionExecPin::Some : EVYOptionExecPin::None;
}
