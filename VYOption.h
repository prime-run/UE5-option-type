#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VYOption.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVYOption, Log, All);


USTRUCT(BlueprintType)
struct XXX_API FVYOption
{
	GENERATED_BODY()

public:
	FVYOption() = default;

	bool IsSome() const { return bIsSome && Payload.IsValid(); }

	bool IsNone() const { return !IsSome(); }

	void Reset()
	{
		bIsSome = false;
		Payload.Reset();
	}

	const UScriptStruct* GetStoredScriptStruct() const { return Payload.GetScriptStruct(); }

	template <typename T>
	static FVYOption MakeSome(const T& InValue)
	{
		FVYOption Option;
		Option.Payload.InitializeAs<T>(InValue);
		Option.bIsSome = true;
		return Option;
	}

	static FVYOption MakeNone()
	{
		return FVYOption();
	}

	template <typename T>
	bool TryGet(T& OutValue) const
	{
		if (IsSome())
		{
			if (const T* Found = Payload.GetPtr<T>())
			{
				OutValue = *Found;
				return true;
			}
		}
		return false;
	}

private:
	friend class UVYOptionLibrary;

	UPROPERTY()
	FInstancedStruct Payload;

	UPROPERTY()
	bool bIsSome = false;
};

USTRUCT()
struct FVYOptionBool
{
	GENERATED_BODY()

	UPROPERTY()
	bool Value = false;
};

USTRUCT()
struct FVYOptionInt
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Value = 0;
};

USTRUCT()
struct FVYOptionFloat
{
	GENERATED_BODY()

	UPROPERTY()
	double Value = 0.0;
};

USTRUCT()
struct FVYOptionString
{
	GENERATED_BODY()

	UPROPERTY()
	FString Value;
};

USTRUCT()
struct FVYOptionObject
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UObject> Value = nullptr;
};

USTRUCT()
struct FVYOptionClass
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UClass> Value = nullptr;
};

UENUM(BlueprintType)
enum class EVYOptionExecPin : uint8
{
	Some,
	None
};

UCLASS()
class VACANYYOU_API UVYOptionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Option|Make", meta = (DisplayName = "Make None", CompactNodeTitle = "None"))
	static FVYOption MakeNone();

	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Option|Make", meta = (CustomStructureParam = "Value", DisplayName = "Make Some (Struct)"))
	static FVYOption MakeSomeStruct(const int32& Value);
	DECLARE_FUNCTION(execMakeSomeStruct);

	UFUNCTION(BlueprintPure, Category = "Option|Make", meta = (DisplayName = "Make Some (Object)"))
	static FVYOption MakeSomeObject(UObject* Value);

	UFUNCTION(BlueprintPure, Category = "Option|Make", meta = (DisplayName = "Make Some (Class)"))
	static FVYOption MakeSomeClass(TSubclassOf<UObject> Value);

	UFUNCTION(BlueprintPure, Category = "Option|Make", meta = (DisplayName = "Make Some (Bool)"))
	static FVYOption MakeSomeBool(bool Value);

	UFUNCTION(BlueprintPure, Category = "Option|Make", meta = (DisplayName = "Make Some (Int)"))
	static FVYOption MakeSomeInt(int32 Value);

	UFUNCTION(BlueprintPure, Category = "Option|Make", meta = (DisplayName = "Make Some (Float)"))
	static FVYOption MakeSomeFloat(double Value);

	UFUNCTION(BlueprintPure, Category = "Option|Make", meta = (DisplayName = "Make Some (String)"))
	static FVYOption MakeSomeString(const FString& Value);

	UFUNCTION(BlueprintPure, Category = "Option|Query", meta = (DisplayName = "Is Some", CompactNodeTitle = "IsSome"))
	static bool IsSome(const FVYOption& Option);

	UFUNCTION(BlueprintPure, Category = "Option|Query", meta = (DisplayName = "Is None", CompactNodeTitle = "IsNone"))
	static bool IsNone(const FVYOption& Option);

	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Option|Match", meta = (CustomStructureParam = "OutValue", ExpandEnumAsExecs = "Branches", DisplayName = "Match (Struct)"))
	static void MatchStruct(const FVYOption& Option, EVYOptionExecPin& Branches, int32& OutValue);
	DECLARE_FUNCTION(execMatchStruct);

	UFUNCTION(BlueprintCallable, Category = "Option|Match", meta = (ExpandEnumAsExecs = "Branches", DisplayName = "Match (Object)"))
	static void MatchObject(const FVYOption& Option, EVYOptionExecPin& Branches, UObject*& OutValue);

	UFUNCTION(BlueprintCallable, Category = "Option|Match", meta = (ExpandEnumAsExecs = "Branches", DisplayName = "Match (Bool)"))
	static void MatchBool(const FVYOption& Option, EVYOptionExecPin& Branches, bool& OutValue);

	UFUNCTION(BlueprintCallable, Category = "Option|Match", meta = (ExpandEnumAsExecs = "Branches", DisplayName = "Match (Int)"))
	static void MatchInt(const FVYOption& Option, EVYOptionExecPin& Branches, int32& OutValue);

	UFUNCTION(BlueprintCallable, Category = "Option|Match", meta = (ExpandEnumAsExecs = "Branches", DisplayName = "Match (Float)"))
	static void MatchFloat(const FVYOption& Option, EVYOptionExecPin& Branches, double& OutValue);

	UFUNCTION(BlueprintCallable, Category = "Option|Match", meta = (ExpandEnumAsExecs = "Branches", DisplayName = "Match (String)"))
	static void MatchString(const FVYOption& Option, EVYOptionExecPin& Branches, FString& OutValue);

	UFUNCTION(BlueprintPure, CustomThunk, Category = "Option|Unwrap", meta = (CustomStructureParam = "OutValue", DisplayName = "Unwrap (Struct)"))
	static bool UnwrapStruct(const FVYOption& Option, int32& OutValue);
	DECLARE_FUNCTION(execUnwrapStruct);

	UFUNCTION(BlueprintPure, Category = "Option|Unwrap", meta = (DisplayName = "Unwrap (Object)"))
	static bool UnwrapObject(const FVYOption& Option, UObject*& OutValue);

	UFUNCTION(BlueprintPure, Category = "Option|Unwrap", meta = (DisplayName = "Unwrap (Bool)"))
	static bool UnwrapBool(const FVYOption& Option, bool& OutValue);

	UFUNCTION(BlueprintPure, Category = "Option|Unwrap", meta = (DisplayName = "Unwrap (Int)"))
	static bool UnwrapInt(const FVYOption& Option, int32& OutValue);

	UFUNCTION(BlueprintPure, Category = "Option|Unwrap", meta = (DisplayName = "Unwrap (Float)"))
	static bool UnwrapFloat(const FVYOption& Option, double& OutValue);

	UFUNCTION(BlueprintPure, Category = "Option|Unwrap", meta = (DisplayName = "Unwrap (String)"))
	static bool UnwrapString(const FVYOption& Option, FString& OutValue);

private:
	static FVYOption Generic_MakeSomeStruct(const FProperty* ValueProp, const void* ValuePtr);
	static bool Generic_UnwrapStruct(const FVYOption& Option, const FStructProperty* ValueProp, void* ValuePtr);
};
