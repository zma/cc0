// #ifndef SYMBOLADDRESS_H
// #define SYMBOLADDRESS_H
// #include <stdint.h>
// #include <string>
// 
// struct SymbolAddress
// {
// public:
//     enum AddressKind
//     {
//         Absolute,
//         // relative to a section, e.g., label is relative to .text, and variable is relative to .data
//         SectionRelative,
//         // relative to the stack frame, e.g., local variables
//         FrameRelative
//     };
//     
//     enum SectionName
//     {
//         Text,
//         Data,
//         RData,
//         Bss
//     };
// public:
//     SymbolAddress();
//     ~SymbolAddress();
// public:
//     bool Resolved;
//     AddressKind Kind;
//     SectionName Section;
//     int64_t Address;
// };
// 
// #endif // SYMBOLADDRESS_H
