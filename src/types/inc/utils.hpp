/*++
Copyright (c) Microsoft Corporation

Module Name:
- utils.hpp

Abstract:
- Helpful cross-lib utilities

Author(s):
- Mike Griese (migrie) 12-Jun-2018
--*/

namespace Microsoft::Console::Utils
{
    bool IsValidHandle(const HANDLE handle) noexcept;

    // Function Description:
    // - Clamps a long in between `min` and `SHRT_MAX`
    // Arguments:
    // - value: the value to clamp
    // - min: the minimum value to clamp to
    // Return Value:
    // - The clamped value as a short.
    constexpr short ClampToShortMax(const long value, const short min) noexcept
    {
        return static_cast<short>(std::clamp(value,
                                             static_cast<long>(min),
                                             static_cast<long>(SHRT_MAX)));
    }

    std::wstring GuidToString(const GUID guid);
    GUID GuidFromString(const std::wstring wstr);
    GUID CreateGuid();

    std::string ColorToHexString(const COLORREF color);
    COLORREF ColorFromHexString(const std::string wstr);

    // Function Description:
    // - Fill the first 16 entries of a given color table with the Campbell color
    //   scheme, in the ANSI/VT RGB order.
    // Arguments:
    // - table: a color table with at least 16 entries
    // Return Value:
    // - <none>, throws if the table has less that 16 entries
    template<typename T>
    void InitializeCampbellColorTable(const gsl::span<T> table)
    {
        THROW_HR_IF(E_INVALIDARG, table.size() < 16);

        // clang-format off
        table[0]   = RGB( 12,   12,   12);
        table[1]   = RGB( 197,  15,   31);
        table[2]   = RGB( 19,   161,  14);
        table[3]   = RGB( 193,  156,  0);
        table[4]   = RGB( 0,    55,   218);
        table[5]   = RGB( 136,  23,   152);
        table[6]   = RGB( 58,   150,  221);
        table[7]   = RGB( 204,  204,  204);
        table[8]   = RGB( 118,  118,  118);
        table[9]   = RGB( 231,  72,   86);
        table[10]  = RGB( 22,   198,  12);
        table[11]  = RGB( 249,  241,  165);
        table[12]  = RGB( 59,   120,  255);
        table[13]  = RGB( 180,  0,    158);
        table[14]  = RGB( 97,   214,  214);
        table[15]  = RGB( 242,  242,  242);
        // clang-format on
    };
    void InitializeCampbellColorTableForConhost(const gsl::span<COLORREF> table);
    void SwapANSIColorOrderForConhost(const gsl::span<COLORREF> table);
    void Initialize256ColorTable(const gsl::span<COLORREF> table);

    // Function Description:
    // - Fill the alpha byte of the colors in a given color table with the given value.
    // Arguments:
    // - table: a color table
    // - newAlpha: the new value to use as the alpha for all the entries in that table.
    // Return Value:
    // - <none>
    constexpr void SetColorTableAlpha(const gsl::span<COLORREF> table, const BYTE newAlpha) noexcept
    {
        const auto shiftedAlpha = newAlpha << 24;
        for (auto& color : table)
        {
            WI_UpdateFlagsInMask(color, 0xff000000, shiftedAlpha);
        }
    }

    constexpr uint16_t EndianSwap(uint16_t value)
    {
        return (value & 0xFF00) >> 8 |
               (value & 0x00FF) << 8;
    }

    constexpr uint32_t EndianSwap(uint32_t value)
    {
        return (value & 0xFF000000) >> 24 |
               (value & 0x00FF0000) >> 8 |
               (value & 0x0000FF00) << 8 |
               (value & 0x000000FF) << 24;
    }

    constexpr unsigned long EndianSwap(unsigned long value)
    {
        return gsl::narrow_cast<unsigned long>(EndianSwap(gsl::narrow_cast<uint32_t>(value)));
    }

    constexpr GUID EndianSwap(GUID value)
    {
        value.Data1 = EndianSwap(value.Data1);
        value.Data2 = EndianSwap(value.Data2);
        value.Data3 = EndianSwap(value.Data3);
        return value;
    }

    GUID CreateV5Uuid(const GUID& namespaceGuid, const gsl::span<const gsl::byte> name);
}
