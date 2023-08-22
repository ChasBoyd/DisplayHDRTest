//********************************************************* 
// 
// Copyright (c) Microsoft. All rights reserved. 
// This code is licensed under the MIT License (MIT). 
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY 
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR 
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT. 
// 
//*********************************************************

DEFINE_GUID(GUID_BackgroundNoisePixelShader,   0x882aea52, 0x4067, 0x11ee, 0xbe, 0x56, 0x02, 0x42, 0xac, 0x12, 0x00, 0x02);
DEFINE_GUID(CLSID_CustomBackgroundNoiseEffect, 0x882af0d8, 0x4067, 0x11ee, 0xbe, 0x56, 0x02, 0x42, 0xac, 0x12, 0x00, 0x02);

// 882aea52 - 4067 - 11ee - be56 - 0242ac120002
// 882af0d8 - 4067 - 11ee - be56 - 0242ac120002

// Our effect contains one transform, which is simply a wrapper around a pixel shader. As such,
// we can simply make the effect itself act as the transform.
class BackgroundNoiseEffect : public ID2D1EffectImpl, public ID2D1DrawTransform
{
public:
    // Declare effect registration methods.
    static HRESULT Register(_In_ ID2D1Factory1* pFactory);

    static HRESULT __stdcall CreateBackgroundNoiseImpl(_Outptr_ IUnknown** ppEffectImpl);

    // Declare ID2D1EffectImpl implementation methods.
    IFACEMETHODIMP Initialize(
        _In_ ID2D1EffectContext* pContextInternal,
        _In_ ID2D1TransformGraph* pTransformGraph
        );

    IFACEMETHODIMP PrepareForRender(D2D1_CHANGE_TYPE changeType);

    IFACEMETHODIMP SetGraph(_In_ ID2D1TransformGraph* pGraph);

    // Declare ID2D1DrawTransform implementation methods.
    IFACEMETHODIMP SetDrawInfo(_In_ ID2D1DrawInfo* pRenderInfo);

    // Declare ID2D1Transform implementation methods.
    IFACEMETHODIMP MapOutputRectToInputRects(
        _In_ const D2D1_RECT_L* pOutputRect,
        _Out_writes_(inputRectCount) D2D1_RECT_L* pInputRects,
        UINT32 inputRectCount
        ) const;

    IFACEMETHODIMP MapInputRectsToOutputRect(
        _In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputRects,
        _In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputOpaqueSubRects,
        UINT32 inputRectCount,
        _Out_ D2D1_RECT_L* pOutputRect,
        _Out_ D2D1_RECT_L* pOutputOpaqueSubRect
        );

    IFACEMETHODIMP MapInvalidRect(
        UINT32 inputIndex,
        D2D1_RECT_L invalidInputRect,
        _Out_ D2D1_RECT_L* pInvalidOutputRect
        ) const;

    // Declare ID2D1TransformNode implementation methods.
    IFACEMETHODIMP_(UINT32) GetInputCount() const;

    // Declare IUnknown implementation methods.
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();
    IFACEMETHODIMP QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput);

    // Declare property getter/setter methods.
    HRESULT SetCenter(D2D1_POINT_2F center);
    D2D1_POINT_2F GetCenter() const;

    HRESULT SetWavelengthHalvingDistance(float frequency);
    float GetWavelengthHalvingDistance() const;

    HRESULT SetInitialWavelength(float phase);
    float GetInitialWavelength() const;

    HRESULT SetWhiteLevelMultiplier(float multiplier);
    float GetWhiteLevelMultiplier() const;

private:
    BackgroundNoiseEffect();
    HRESULT UpdateConstants();

    inline static float Clamp(float v, float low, float high)
    {
        return (v < low) ? low : (v > high) ? high : v;
    }

    inline static float Round(float v)
    {
        return floor(v + 0.5f);
    }

    // Prevents over/underflows when adding longs.
    inline static long SafeAdd(long base, long valueToAdd)
    {
        if (valueToAdd >= 0)
        {
            return ((base + valueToAdd) >= base) ? (base + valueToAdd) : LONG_MAX;
        }
        else
        {
            return ((base + valueToAdd) <= base) ? (base + valueToAdd) : LONG_MIN;
        }
    }

    // This struct defines the constant buffer of our pixel shader.
    // All distances are in pixels, we ignore DPI.
    struct
    {
        float dpi;
        D2D1_POINT_2F center;
        float initialWavelength;
        float wavelengthHalvingDistance;
        float whiteLevelMultiplier;
    } m_constants;

    Microsoft::WRL::ComPtr<ID2D1DrawInfo>      m_drawInfo;
    Microsoft::WRL::ComPtr<ID2D1EffectContext> m_effectContext;
    LONG                                       m_refCount;
    D2D1_RECT_L                                m_inputRect;
    float                                      m_dpi;
};
