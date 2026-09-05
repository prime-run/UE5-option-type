# UE5 Option Type

An implementation of rust's Option type in Unreal Engine (5.1 +) with BP support.

## Setup

Because the payload is boxed safely using `FInstancedStruct`, you must add the `StructUtils` module to your project's `[YourProjectName].Build.cs` file:

```csharp
PublicDependencyModuleNames.AddRange(new string[] { 
    // ...
    "StructUtils" 
});

```

## C++ Usage

### Creating an Option

Return `FVYOption` from your functions using the `MakeSome` or `MakeNone` helpers. It supports any primitive, `UObject*`, or `USTRUCT`.

```cpp
#include "VYOption.h"

FVYOption GetHealth(bool bIsAlive)
{
    if (bIsAlive)
    {
        return FVYOption::MakeSome<int32>(100);
    }
    
    return FVYOption::MakeNone();
}

```

### Evaluating an Option

Use `TryGet<T>()` to safely extract the payload. It returns `false` if the Option is `None` or if the requested type doesn't match the stored type.

```cpp
FVYOption HealthOpt = GetHealth(true);
int32 HealthValue = 0;

if (HealthOpt.TryGet<int32>(HealthValue))
{
    UE_LOG(LogTemp, Log, TEXT("Health is: %d"), HealthValue);
}
else
{
    UE_LOG(LogTemp, Warning, TEXT("Health data unavailable (None)"));
}

```

## Blueprint Usage

The `VYOption` struct is fully exposed to Blueprints, supporting native pins for primitives/objects and generic wildcard pins for custom structs.

### Returning an Option

1. Set your function's Return Node output type to **VYOption**.
2. Feed either a **Make Some** or **Make None** node into the return value.

### Handling an Option

Evaluate the returned `VYOption` using one of two methods:

* **Match Nodes (Execution):** Splits your execution flow into **Some** and **None** branches. The **Some** execution pin safely provides the unwrapped data on its output pin.
* **Unwrap Nodes (Pure):** A pure evaluation node without execution pins. It returns a boolean (`true` if Some) alongside the unwrapped data. It is guaranteed not to crash and will output safe, zero-initialized default data if called on a `None` value.
