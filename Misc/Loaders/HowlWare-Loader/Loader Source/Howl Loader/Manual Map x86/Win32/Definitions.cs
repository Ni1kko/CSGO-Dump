using System;
using System.Runtime.InteropServices;

namespace ManualMapInjection.Injection.Win32
{
    internal enum MachineType : ushort
    {
        Native = 0,
        I386 = 0x014c,
        Itanium = 0x0200,
        x64 = 0x8664
    }

    internal enum MagicType : ushort
    {
        IMAGE_NT_OPTIONAL_HDR32_MAGIC = 0x10b,
        IMAGE_NT_OPTIONAL_HDR64_MAGIC = 0x20b
    }

    internal enum SubSystemType : ushort
    {
        IMAGE_SUBSYSTEM_UNKNOWN = 0,
        IMAGE_SUBSYSTEM_NATIVE = 1,
        IMAGE_SUBSYSTEM_WINDOWS_GUI = 2,
        IMAGE_SUBSYSTEM_WINDOWS_CUI = 3,
        IMAGE_SUBSYSTEM_POSIX_CUI = 7,
        IMAGE_SUBSYSTEM_WINDOWS_CE_GUI = 9,
        IMAGE_SUBSYSTEM_EFI_APPLICATION = 10,
        IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER = 11,
        IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER = 12,
        IMAGE_SUBSYSTEM_EFI_ROM = 13,
        IMAGE_SUBSYSTEM_XBOX = 14
    }

    internal enum DllCharacteristicsType : ushort
    {
        RES_0 = 0x0001,
        RES_1 = 0x0002,
        RES_2 = 0x0004,
        RES_3 = 0x0008,
        IMAGE_DLL_CHARACTERISTICS_DYNAMIC_BASE = 0x0040,
        IMAGE_DLL_CHARACTERISTICS_FORCE_INTEGRITY = 0x0080,
        IMAGE_DLL_CHARACTERISTICS_NX_COMPAT = 0x0100,
        IMAGE_DLLCHARACTERISTICS_NO_ISOLATION = 0x0200,
        IMAGE_DLLCHARACTERISTICS_NO_SEH = 0x0400,
        IMAGE_DLLCHARACTERISTICS_NO_BIND = 0x0800,
        RES_4 = 0x1000,
        IMAGE_DLLCHARACTERISTICS_WDM_DRIVER = 0x2000,
        IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE = 0x8000
    }

    [Flags]
    internal enum DataSectionFlags : uint
    {
        /// <summary>
        /// Reserved for future use.
        /// </summary>
        TypeReg = 0x00000000,

        /// <summary>
        /// Reserved for future use.
        /// </summary>
        TypeDsect = 0x00000001,

        /// <summary>
        /// Reserved for future use.
        /// </summary>
        TypeNoLoad = 0x00000002,

        /// <summary>
        /// Reserved for future use.
        /// </summary>
        TypeGroup = 0x00000004,

        /// <summary>
        /// The section should not be padded to the next boundary. This flag is obsolete and is replaced by IMAGE_SCN_ALIGN_1BYTES. This is valid only for object files.
        /// </summary>
        TypeNoPadded = 0x00000008,

        /// <summary>
        /// Reserved for future use.
        /// </summary>
        TypeCopy = 0x00000010,

        /// <summary>
        /// The section contains executable code.
        /// </summary>
        ContentCode = 0x00000020,

        /// <summary>
        /// The section contains initialized data.
        /// </summary>
        ContentInitializedData = 0x00000040,

        /// <summary>
        /// The section contains uninitialized data.
        /// </summary>
        ContentUninitializedData = 0x00000080,

        /// <summary>
        /// Reserved for future use.
        /// </summary>
        LinkOther = 0x00000100,

        /// <summary>
        /// The section contains comments or other information. The .drectve section has this type. This is valid for object files only.
        /// </summary>
        LinkInfo = 0x00000200,

        /// <summary>
        /// Reserved for future use.
        /// </summary>
        TypeOver = 0x00000400,

        /// <summary>
        /// The section will not become part of the image. This is valid only for object files.
        /// </summary>
        LinkRemove = 0x00000800,

        /// <summary>
        /// The section contains COMDAT data. For more information, see section 5.5.6, COMDAT Sections (Object Only). This is valid only for object files.
        /// </summary>
        LinkComDat = 0x00001000,

        /// <summary>
        /// Reset speculative exceptions handling bits in the TLB entries for this section.
        /// </summary>
        NoDeferSpecExceptions = 0x00004000,

        /// <summary>
        /// The section contains data referenced through the global pointer (GP).
        /// </summary>
        RelativeGP = 0x00008000,

        /// <summary>
        /// Reserved for future use.
        /// </summary>
        MemPurgeable = 0x00020000,

        /// <summary>
        /// Reserved for future use.
        /// </summary>
        Memory16Bit = 0x00020000,

        /// <summary>
        /// Reserved for future use.
        /// </summary>
        MemoryLocked = 0x00040000,

        /// <summary>
        /// Reserved for future use.
        /// </summary>
        MemoryPreload = 0x00080000,

        /// <summary>
        /// Align data on a 1-byte boundary. Valid only for object files.
        /// </summary>
        Align1Bytes = 0x00100000,

        /// <summary>
        /// Align data on a 2-byte boundary. Valid only for object files.
        /// </summary>
        Align2Bytes = 0x00200000,

        /// <summary>
        /// Align data on a 4-byte boundary. Valid only for object files.
        /// </summary>
        Align4Bytes = 0x00300000,

        /// <summary>
        /// Align data on an 8-byte boundary. Valid only for object files.
        /// </summary>
        Align8Bytes = 0x00400000,

        /// <summary>
        /// Align data on a 16-byte boundary. Valid only for object files.
        /// </summary>
        Align16Bytes = 0x00500000,

        /// <summary>
        /// Align data on a 32-byte boundary. Valid only for object files.
        /// </summary>
        Align32Bytes = 0x00600000,

        /// <summary>
        /// Align data on a 64-byte boundary. Valid only for object files.
        /// </summary>
        Align64Bytes = 0x00700000,

        /// <summary>
        /// Align data on a 128-byte boundary. Valid only for object files.
        /// </summary>
        Align128Bytes = 0x00800000,

        /// <summary>
        /// Align data on a 256-byte boundary. Valid only for object files.
        /// </summary>
        Align256Bytes = 0x00900000,

        /// <summary>
        /// Align data on a 512-byte boundary. Valid only for object files.
        /// </summary>
        Align512Bytes = 0x00A00000,

        /// <summary>
        /// Align data on a 1024-byte boundary. Valid only for object files.
        /// </summary>
        Align1024Bytes = 0x00B00000,

        /// <summary>
        /// Align data on a 2048-byte boundary. Valid only for object files.
        /// </summary>
        Align2048Bytes = 0x00C00000,

        /// <summary>
        /// Align data on a 4096-byte boundary. Valid only for object files.
        /// </summary>
        Align4096Bytes = 0x00D00000,

        /// <summary>
        /// Align data on an 8192-byte boundary. Valid only for object files.
        /// </summary>
        Align8192Bytes = 0x00E00000,

        /// <summary>
        /// The section contains extended relocations.
        /// </summary>
        LinkExtendedRelocationOverflow = 0x01000000,

        /// <summary>
        /// The section can be discarded as needed.
        /// </summary>
        MemoryDiscardable = 0x02000000,

        /// <summary>
        /// The section cannot be cached.
        /// </summary>
        MemoryNotCached = 0x04000000,

        /// <summary>
        /// The section is not pageable.
        /// </summary>
        MemoryNotPaged = 0x08000000,

        /// <summary>
        /// The section can be shared in memory.
        /// </summary>
        MemoryShared = 0x10000000,

        /// <summary>
        /// The section can be executed as code.
        /// </summary>
        MemoryExecute = 0x20000000,

        /// <summary>
        /// The section can be read.
        /// </summary>
        MemoryRead = 0x40000000,

        /// <summary>
        /// The section can be written to.
        /// </summary>
        MemoryWrite = 0x80000000
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct IMAGE_DOS_HEADER
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)] internal char[] e_magic; // Magic number
        internal UInt16 e_cblp; // Bytes on last page of file
        internal UInt16 e_cp; // Pages in file
        internal UInt16 e_crlc; // Relocations
        internal UInt16 e_cparhdr; // Size of header in paragraphs
        internal UInt16 e_minalloc; // Minimum extra paragraphs needed
        internal UInt16 e_maxalloc; // Maximum extra paragraphs needed
        internal UInt16 e_ss; // Initial (relative) SS value
        internal UInt16 e_sp; // Initial SP value
        internal UInt16 e_csum; // Checksum
        internal UInt16 e_ip; // Initial IP value
        internal UInt16 e_cs; // Initial (relative) CS value
        internal UInt16 e_lfarlc; // File address of relocation table
        internal UInt16 e_ovno; // Overlay number
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)] internal UInt16[] e_res1; // Reserved words
        internal UInt16 e_oemid; // OEM identifier (for e_oeminfo)
        internal UInt16 e_oeminfo; // OEM information; e_oemid specific
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)] internal UInt16[] e_res2; // Reserved words
        internal Int32 e_lfanew; // File address of new exe header

        private string _e_magic
        {
            get { return new string(e_magic); }
        }

        internal bool isValid
        {
            get { return _e_magic == "MZ"; }
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct IMAGE_FILE_HEADER
    {
        internal UInt16 Machine;
        internal UInt16 NumberOfSections;
        internal UInt32 TimeDateStamp;
        internal UInt32 PointerToSymbolTable;
        internal UInt32 NumberOfSymbols;
        internal UInt16 SizeOfOptionalHeader;
        internal UInt16 Characteristics;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct IMAGE_DATA_DIRECTORY
    {
        internal UInt32 VirtualAddress;
        internal UInt32 Size;
    }

    [StructLayout(LayoutKind.Explicit)]
    internal struct IMAGE_OPTIONAL_HEADER32
    {
        [FieldOffset(0)] internal MagicType Magic;

        [FieldOffset(2)] internal byte MajorLinkerVersion;

        [FieldOffset(3)] internal byte MinorLinkerVersion;

        [FieldOffset(4)] internal uint SizeOfCode;

        [FieldOffset(8)] internal uint SizeOfInitializedData;

        [FieldOffset(12)] internal uint SizeOfUninitializedData;

        [FieldOffset(16)] internal uint AddressOfEntryPoint;

        [FieldOffset(20)] internal uint BaseOfCode;

        // PE32 contains this additional field
        [FieldOffset(24)] internal uint BaseOfData;

        [FieldOffset(28)] internal uint ImageBase;

        [FieldOffset(32)] internal uint SectionAlignment;

        [FieldOffset(36)] internal uint FileAlignment;

        [FieldOffset(40)] internal ushort MajorOperatingSystemVersion;

        [FieldOffset(42)] internal ushort MinorOperatingSystemVersion;

        [FieldOffset(44)] internal ushort MajorImageVersion;

        [FieldOffset(46)] internal ushort MinorImageVersion;

        [FieldOffset(48)] internal ushort MajorSubsystemVersion;

        [FieldOffset(50)] internal ushort MinorSubsystemVersion;

        [FieldOffset(52)] internal uint Win32VersionValue;

        [FieldOffset(56)] internal uint SizeOfImage;

        [FieldOffset(60)] internal uint SizeOfHeaders;

        [FieldOffset(64)] internal uint CheckSum;

        [FieldOffset(68)] internal SubSystemType Subsystem;

        [FieldOffset(70)] internal DllCharacteristicsType DllCharacteristics;

        [FieldOffset(72)] internal uint SizeOfStackReserve;

        [FieldOffset(76)] internal uint SizeOfStackCommit;

        [FieldOffset(80)] internal uint SizeOfHeapReserve;

        [FieldOffset(84)] internal uint SizeOfHeapCommit;

        [FieldOffset(88)] internal uint LoaderFlags;

        [FieldOffset(92)] internal uint NumberOfRvaAndSizes;

        [FieldOffset(96)] internal IMAGE_DATA_DIRECTORY ExportTable;

        [FieldOffset(104)] internal IMAGE_DATA_DIRECTORY ImportTable;

        [FieldOffset(112)] internal IMAGE_DATA_DIRECTORY ResourceTable;

        [FieldOffset(120)] internal IMAGE_DATA_DIRECTORY ExceptionTable;

        [FieldOffset(128)] internal IMAGE_DATA_DIRECTORY CertificateTable;

        [FieldOffset(136)] internal IMAGE_DATA_DIRECTORY BaseRelocationTable;

        [FieldOffset(144)] internal IMAGE_DATA_DIRECTORY Debug;

        [FieldOffset(152)] internal IMAGE_DATA_DIRECTORY Architecture;

        [FieldOffset(160)] internal IMAGE_DATA_DIRECTORY GlobalPtr;

        [FieldOffset(168)] internal IMAGE_DATA_DIRECTORY TLSTable;

        [FieldOffset(176)] internal IMAGE_DATA_DIRECTORY LoadConfigTable;

        [FieldOffset(184)] internal IMAGE_DATA_DIRECTORY BoundImport;

        [FieldOffset(192)] internal IMAGE_DATA_DIRECTORY IAT;

        [FieldOffset(200)] internal IMAGE_DATA_DIRECTORY DelayImportDescriptor;

        [FieldOffset(208)] internal IMAGE_DATA_DIRECTORY CLRRuntimeHeader;

        [FieldOffset(216)] internal IMAGE_DATA_DIRECTORY Reserved;
    }

    [StructLayout(LayoutKind.Explicit)]
    internal struct IMAGE_OPTIONAL_HEADER64
    {
        [FieldOffset(0)] internal MagicType Magic;

        [FieldOffset(2)] internal byte MajorLinkerVersion;

        [FieldOffset(3)] internal byte MinorLinkerVersion;

        [FieldOffset(4)] internal uint SizeOfCode;

        [FieldOffset(8)] internal uint SizeOfInitializedData;

        [FieldOffset(12)] internal uint SizeOfUninitializedData;

        [FieldOffset(16)] internal uint AddressOfEntryPoint;

        [FieldOffset(20)] internal uint BaseOfCode;

        [FieldOffset(24)] internal ulong ImageBase;

        [FieldOffset(32)] internal uint SectionAlignment;

        [FieldOffset(36)] internal uint FileAlignment;

        [FieldOffset(40)] internal ushort MajorOperatingSystemVersion;

        [FieldOffset(42)] internal ushort MinorOperatingSystemVersion;

        [FieldOffset(44)] internal ushort MajorImageVersion;

        [FieldOffset(46)] internal ushort MinorImageVersion;

        [FieldOffset(48)] internal ushort MajorSubsystemVersion;

        [FieldOffset(50)] internal ushort MinorSubsystemVersion;

        [FieldOffset(52)] internal uint Win32VersionValue;

        [FieldOffset(56)] internal uint SizeOfImage;

        [FieldOffset(60)] internal uint SizeOfHeaders;

        [FieldOffset(64)] internal uint CheckSum;

        [FieldOffset(68)] internal SubSystemType Subsystem;

        [FieldOffset(70)] internal DllCharacteristicsType DllCharacteristics;

        [FieldOffset(72)] internal ulong SizeOfStackReserve;

        [FieldOffset(80)] internal ulong SizeOfStackCommit;

        [FieldOffset(88)] internal ulong SizeOfHeapReserve;

        [FieldOffset(96)] internal ulong SizeOfHeapCommit;

        [FieldOffset(104)] internal uint LoaderFlags;

        [FieldOffset(108)] internal uint NumberOfRvaAndSizes;

        [FieldOffset(112)] internal IMAGE_DATA_DIRECTORY ExportTable;

        [FieldOffset(120)] internal IMAGE_DATA_DIRECTORY ImportTable;

        [FieldOffset(128)] internal IMAGE_DATA_DIRECTORY ResourceTable;

        [FieldOffset(136)] internal IMAGE_DATA_DIRECTORY ExceptionTable;

        [FieldOffset(144)] internal IMAGE_DATA_DIRECTORY CertificateTable;

        [FieldOffset(152)] internal IMAGE_DATA_DIRECTORY BaseRelocationTable;

        [FieldOffset(160)] internal IMAGE_DATA_DIRECTORY Debug;

        [FieldOffset(168)] internal IMAGE_DATA_DIRECTORY Architecture;

        [FieldOffset(176)] internal IMAGE_DATA_DIRECTORY GlobalPtr;

        [FieldOffset(184)] internal IMAGE_DATA_DIRECTORY TLSTable;

        [FieldOffset(192)] internal IMAGE_DATA_DIRECTORY LoadConfigTable;

        [FieldOffset(200)] internal IMAGE_DATA_DIRECTORY BoundImport;

        [FieldOffset(208)] internal IMAGE_DATA_DIRECTORY IAT;

        [FieldOffset(216)] internal IMAGE_DATA_DIRECTORY DelayImportDescriptor;

        [FieldOffset(224)] internal IMAGE_DATA_DIRECTORY CLRRuntimeHeader;

        [FieldOffset(232)] internal IMAGE_DATA_DIRECTORY Reserved;
    }

    [StructLayout(LayoutKind.Explicit)]
    internal struct IMAGE_NT_HEADERS32
    {
        [FieldOffset(0)] [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)] internal char[] Signature;

        [FieldOffset(4)] internal IMAGE_FILE_HEADER FileHeader;

        [FieldOffset(24)] internal IMAGE_OPTIONAL_HEADER32 OptionalHeader;

        private string _Signature
        {
            get { return new string(Signature); }
        }

        internal bool isValid
        {
            get
            {
                return _Signature == "PE\0\0"
                    /*&& (OptionalHeader.Magic == PE.MagicType.IMAGE_NT_OPTIONAL_HDR32_MAGIC || OptionalHeader.Magic == PE.MagicType.IMAGE_NT_OPTIONAL_HDR64_MAGIC)*/;
            }
        }
    }

    [StructLayout(LayoutKind.Explicit)]
    internal struct IMAGE_NT_HEADERS64
    {
        [FieldOffset(0)] [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)] internal char[] Signature;

        [FieldOffset(4)] internal IMAGE_FILE_HEADER FileHeader;

        [FieldOffset(24)] internal IMAGE_OPTIONAL_HEADER64 OptionalHeader;

        private string _Signature
        {
            get { return new string(Signature); }
        }

        internal bool isValid
        {
            get
            {
                return _Signature == "PE\0\0"
                    /*&& (OptionalHeader.Magic == PE.MagicType.IMAGE_NT_OPTIONAL_HDR32_MAGIC || OptionalHeader.Magic == PE.MagicType.IMAGE_NT_OPTIONAL_HDR64_MAGIC)*/;
            }
        }
    }

    [StructLayout(LayoutKind.Explicit)]
    internal struct IMAGE_SECTION_HEADER
    {
        [FieldOffset(0)] [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)] internal char[] Name;

        [FieldOffset(8)] internal UInt32 VirtualSize;

        [FieldOffset(12)] internal UInt32 VirtualAddress;

        [FieldOffset(16)] internal UInt32 SizeOfRawData;

        [FieldOffset(20)] internal UInt32 PointerToRawData;

        [FieldOffset(24)] internal UInt32 PointerToRelocations;

        [FieldOffset(28)] internal UInt32 PointerToLinenumbers;

        [FieldOffset(32)] internal UInt16 NumberOfRelocations;

        [FieldOffset(34)] internal UInt16 NumberOfLinenumbers;

        [FieldOffset(36)] internal DataSectionFlags Characteristics;

        internal string Section
        {
            get { return new string(Name); }
        }
    }

    [StructLayout(LayoutKind.Explicit)]
    internal struct IMAGE_IMPORT_DESCRIPTOR
    {
        [FieldOffset(0)] internal uint Characteristics;

        [FieldOffset(0)] internal uint OriginalFirstThunk;

        [FieldOffset(4)] internal uint TimeDateStamp;

        [FieldOffset(8)] internal uint ForwarderChain;

        [FieldOffset(12)] internal uint Name;

        [FieldOffset(16)] internal uint FirstThunk;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    internal struct PROCESS_BASIC_INFORMATION
    {
        internal IntPtr ExitStatus;
        internal IntPtr PebBaseAddress;
        internal IntPtr AffinityMask;
        internal IntPtr BasePriority;
        internal UIntPtr UniqueProcessId;
        internal IntPtr InheritedFromUniqueProcessId;

        internal int Size
        {
            get { return (int) Marshal.SizeOf(typeof(PROCESS_BASIC_INFORMATION)); }
        }
    }

    [StructLayout(LayoutKind.Sequential, Pack = 0)]
    internal struct UNICODE_STRING
    {
        internal ushort Length;
        internal ushort MaximumLength;
        internal IntPtr Buffer;
    }

    [StructLayout(LayoutKind.Explicit)]
    internal struct IMAGE_THUNK_DATA
    {
        [FieldOffset(0)] internal uint ForwarderString; // PBYTE 

        [FieldOffset(0)] internal uint Function; // PDWORD

        [FieldOffset(0)] internal uint Ordinal;

        [FieldOffset(0)] internal uint AddressOfData; // PIMAGE_IMPORT_BY_NAME
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct IMAGE_EXPORT_DIRECTORY
    {
        internal UInt32 Characteristics;
        internal UInt32 TimeDateStamp;
        internal UInt16 MajorVersion;
        internal UInt16 MinorVersion;
        internal UInt32 Name;
        internal UInt32 Base;
        internal UInt32 NumberOfFunctions;
        internal UInt32 NumberOfNames;
        internal UInt32 AddressOfFunctions; // RVA from base of image
        internal UInt32 AddressOfNames; // RVA from base of image
        internal UInt32 AddressOfNameOrdinals; // RVA from base of image
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct IMAGE_IMPORT_BY_NAME
    {
        internal short Hint;
        internal char Name;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct IMAGE_BASE_RELOCATION
    {
        internal UInt32 VirtualAddress;
        internal UInt32 SizeOfBlock;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct IMAGE_TLS_DIRECTORY32
    {
        internal UInt32 StartAddressOfRawData;
        internal UInt32 EndAddressOfRawData;
        internal UInt32 AddressOfIndex; // PDWORD
        internal UInt32 AddressOfCallBacks; // PIMAGE_TLS_CALLBACK *
        internal UInt32 SizeOfZeroFill;
        internal UInt32 Characteristics;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct IMAGE_LOAD_CONFIG_DIRECTORY32
    {
        internal UInt32 Size;
        internal UInt32 TimeDateStamp;
        internal UInt16 MajorVersion;
        internal UInt16 MinorVersion;
        internal UInt32 GlobalFlagsClear;
        internal UInt32 GlobalFlagsSet;
        internal UInt32 CriticalSectionDefaultTimeout;
        internal UInt32 DeCommitFreeBlockThreshold;
        internal UInt32 DeCommitTotalFreeThreshold;
        internal UInt32 LockPrefixTable;                // VA
        internal UInt32 MaximumAllocationSize;
        internal UInt32 VirtualMemoryThreshold;
        internal UInt32 ProcessHeapFlags;
        internal UInt32 ProcessAffinityMask;
        internal UInt16 CSDVersion;
        internal UInt16 Reserved1;
        internal UInt32 EditList;                       // VA
        internal UInt32 SecurityCookie;                 // VA
        internal UInt32 SEHandlerTable;                 // VA
        internal UInt32 SEHandlerCount;
        internal UInt32 GuardCFCheckFunctionPointer;    // VA
        internal UInt32 Reserved2;
        internal UInt32 GuardCFFunctionTable;           // VA
        internal UInt32 GuardCFFunctionCount;
        internal UInt32 GuardFlags;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct FILETIME
    {
        internal uint DateTimeLow;
        internal uint DateTimeHigh;
    }
}