using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using ManualMapInjection.Injection.Win32;

namespace ManualMapInjection.Injection
{
    internal class ManualMapInjector
    {
        #region settings

        public bool AsyncInjection { get; set; } = false;

        public uint TimeOut { get; set; } = 5000;

        #endregion

        #region fields

        private readonly Process _process;

        private IntPtr _hProcess;

        #endregion

        #region code

        private PIMAGE_DOS_HEADER GetDosHeader(IntPtr address)
        {
            var imageDosHeader = (PIMAGE_DOS_HEADER)address;

            if (!imageDosHeader.Value.isValid)
            {
                return null;
            }

            return imageDosHeader;
        }

        private PIMAGE_NT_HEADERS32 GetNtHeader(IntPtr address)
        {
            var imageDosHeader = GetDosHeader(address);

            if (imageDosHeader == null)
            {
                return null;
            }

            var imageNtHeaders = (PIMAGE_NT_HEADERS32)(address + imageDosHeader.Value.e_lfanew);

            if (!imageNtHeaders.Value.isValid)
            {
                return null;
            }

            return imageNtHeaders;
        }

        private IntPtr RemoteAllocateMemory(uint size)
        {
            return Imports.VirtualAllocEx(_hProcess,
                UIntPtr.Zero,
                new IntPtr(size),
                Imports.AllocationType.Commit | Imports.AllocationType.Reserve,
                Imports.MemoryProtection.ExecuteReadWrite);
        }

        private IntPtr AllocateMemory(uint size)
        {
            return Imports.VirtualAlloc(IntPtr.Zero, new UIntPtr(size), Imports.AllocationType.Commit | Imports.AllocationType.Reserve,
                Imports.MemoryProtection.ExecuteReadWrite);
        }

        private IntPtr RvaToPointer(uint rva, IntPtr baseAddress)
        {
            var imageNtHeaders = GetNtHeader(baseAddress);
            if (imageNtHeaders == null)
            {
                return IntPtr.Zero;
            }

            return Imports.ImageRvaToVa(imageNtHeaders.Address, baseAddress, new UIntPtr(rva), IntPtr.Zero);
        }

        private bool InjectDependency(string dependency)
        {
            // standard LoadLibrary, CreateRemoteThread injection
            var procAddress = Imports.GetProcAddress(Imports.GetModuleHandle("kernel32.dll"), "LoadLibraryA");

            if (procAddress == IntPtr.Zero)
            {
#if DEBUG
                Debug.WriteLine("[InjectDependency] GetProcAddress failed");
#endif
                return false;
            }

            var lpAddress = RemoteAllocateMemory((uint)dependency.Length);

            if (lpAddress == IntPtr.Zero)
            {
#if DEBUG
                Debug.WriteLine("[InjectDependency] RemoteAllocateMemory failed");
#endif
                return false;
            }

            var buffer = Encoding.ASCII.GetBytes(dependency);

            UIntPtr bytesWritten;
            var result = Imports.WriteProcessMemory(_hProcess, lpAddress, buffer, buffer.Length, out bytesWritten);

            if (result)
            {
                var hHandle = Imports.CreateRemoteThread(_hProcess, IntPtr.Zero, 0, procAddress, lpAddress, 0, IntPtr.Zero);

                if (Imports.WaitForSingleObject(hHandle, TimeOut) != 0)
                {
#if DEBUG
                    Debug.WriteLine("[InjectDependency] remote thread not signaled");
#endif
                    return false;
                }
            }
#if DEBUG
            else
            {
                Debug.WriteLine("[InjectDependency] WriteProcessMemory failed");
            }
#endif

            Imports.VirtualFreeEx(_hProcess, lpAddress, 0, Imports.FreeType.Release);
            return result;
        }

        private IntPtr GetRemoteModuleHandleA(string module)
        {
            var dwModuleHandle = IntPtr.Zero;
            var hHeap = Imports.GetProcessHeap();
            var dwSize = (uint)Marshal.SizeOf(typeof(PROCESS_BASIC_INFORMATION));
            var pbi = (PPROCESS_BASIC_INFORMATION)Imports.HeapAlloc(hHeap, /*HEAP_ZERO_MEMORY*/ 0x8, new UIntPtr(dwSize));

            uint dwSizeNeeded;
            var dwStatus = Imports.NtQueryInformationProcess(_hProcess, /*ProcessBasicInformation*/ 0, pbi.Address, dwSize, out dwSizeNeeded);

            if (dwStatus >= 0 && dwSize < dwSizeNeeded)
            {
                if (pbi != null)
                {
                    Imports.HeapFree(hHeap, 0, pbi.Address);
                }

                pbi = (PPROCESS_BASIC_INFORMATION)Imports.HeapAlloc(hHeap, /*HEAP_ZERO_MEMORY*/ 0x8, new UIntPtr(dwSize));

                if (pbi == null)
                {
#if DEBUG
                    Debug.WriteLine("[GetRemoteModuleHandleA] Couldn't allocate heap buffer");
#endif
                    return IntPtr.Zero; //Couldn't allocate heap buffer
                }

                dwStatus = Imports.NtQueryInformationProcess(_hProcess, /*ProcessBasicInformation*/ 0, pbi.Address, dwSizeNeeded, out dwSizeNeeded);
            }

            if (dwStatus >= 0)
            {
                if (pbi.Value.PebBaseAddress != IntPtr.Zero)
                {
                    UIntPtr dwBytesRead;
                    uint pebLdrAddress;

                    if (Imports.ReadProcessMemory(_hProcess, pbi.Value.PebBaseAddress + 12 /*peb.Ldr*/, out pebLdrAddress, out dwBytesRead))
                    {
                        var pLdrListHead = pebLdrAddress + /*InLoadOrderModuleList*/ 0x0C;
                        var pLdrCurrentNode = pebLdrAddress + /*InLoadOrderModuleList*/ 0x0C;

                        do
                        {
                            uint lstEntryAddress;
                            if (!Imports.ReadProcessMemory(_hProcess, new IntPtr(pLdrCurrentNode), out lstEntryAddress, out dwBytesRead))
                            {
                                Imports.HeapFree(hHeap, 0, pbi.Address);
                            }
                            pLdrCurrentNode = lstEntryAddress;

                            UNICODE_STRING baseDllName;
                            Imports.ReadProcessMemory(_hProcess, new IntPtr(lstEntryAddress) + /*BaseDllName*/ 0x2C, out baseDllName, out dwBytesRead);

                            var strBaseDllName = string.Empty;

                            if (baseDllName.Length > 0)
                            {
                                var buffer = new byte[baseDllName.Length];
                                Imports.ReadProcessMemory(_hProcess, baseDllName.Buffer, buffer, out dwBytesRead);
                                strBaseDllName = Encoding.Unicode.GetString(buffer);
                            }

                            uint dllBase;
                            uint sizeOfImage;

                            Imports.ReadProcessMemory(_hProcess, new IntPtr(lstEntryAddress) + /*DllBase*/ 0x18, out dllBase, out dwBytesRead);
                            Imports.ReadProcessMemory(_hProcess, new IntPtr(lstEntryAddress) + /*SizeOfImage*/ 0x20, out sizeOfImage, out dwBytesRead);

                            if (dllBase != 0 && sizeOfImage != 0)
                            {
                                if (string.Equals(strBaseDllName, module, StringComparison.OrdinalIgnoreCase))
                                {
                                    dwModuleHandle = new IntPtr(dllBase);
                                    break;
                                }
                            }

                        } while (pLdrListHead != pLdrCurrentNode);
                    }
                }
            }

            if (pbi != null)
            {
                Imports.HeapFree(hHeap, 0, pbi.Address);
            }

            return dwModuleHandle;
        }

        private IntPtr GetDependencyProcAddressA(IntPtr moduleBase, PCHAR procName)
        {
            IntPtr pFunc = IntPtr.Zero;
            IMAGE_DOS_HEADER hdrDos;
            IMAGE_NT_HEADERS32 hdrNt32;

            UIntPtr dwRead;
            Imports.ReadProcessMemory(_hProcess, moduleBase, out hdrDos, out dwRead);

            if (!hdrDos.isValid)
            {
                return IntPtr.Zero;
            }

            Imports.ReadProcessMemory(_hProcess, moduleBase + hdrDos.e_lfanew, out hdrNt32, out dwRead);

            if (!hdrNt32.isValid)
            {
                return IntPtr.Zero;
            }

            var expBase = hdrNt32.OptionalHeader.ExportTable.VirtualAddress;
            if (expBase > 0)
            {
                var expSize = hdrNt32.OptionalHeader.ExportTable.Size;
                var expData = (PIMAGE_EXPORT_DIRECTORY)AllocateMemory(expSize);
                Imports.ReadProcessMemory(_hProcess, moduleBase + (int)expBase, expData.Address, (int)expSize, out dwRead);

                var pAddressOfOrds = (PWORD)(expData.Address + (int)expData.Value.AddressOfNameOrdinals - (int)expBase);
                var pAddressOfNames = (PDWORD)(expData.Address + (int)expData.Value.AddressOfNames - (int)expBase);
                var pAddressOfFuncs = (PDWORD)(expData.Address + (int)expData.Value.AddressOfFunctions - (int)expBase);


                for (uint i = 0; i < expData.Value.NumberOfFunctions; i++)
                {
                    ushort ordIndex;
                    PCHAR pName = null;

                    if (new PDWORD(procName.Address).Value <= 0xFFFF)
                    {
                        ordIndex = unchecked((ushort)i);
                    }
                    else if (new PDWORD(procName.Address).Value > 0xFFFF && i < expData.Value.NumberOfNames)
                    {
                        pName = (PCHAR)new IntPtr(pAddressOfNames[i] + expData.Address.ToInt32() - expBase);
                        ordIndex = pAddressOfOrds[i];
                    }
                    else
                    {
                        return IntPtr.Zero;
                    }

                    if ((new PDWORD(procName.Address).Value <= 0xFFFF && new PDWORD(procName.Address).Value == ordIndex + expData.Value.Base) || (new PDWORD(procName.Address).Value > 0xFFFF && pName.ToString() == procName.ToString()))
                    {
                        pFunc = moduleBase + (int)pAddressOfFuncs[ordIndex];

                        if (pFunc.ToInt64() >= (moduleBase + (int)expBase).ToInt64() && pFunc.ToInt64() <= (moduleBase + (int)expBase + (int)expSize).ToInt64())
                        {
                            var forwardStr = new byte[255];
                            Imports.ReadProcessMemory(_hProcess, pFunc, forwardStr, out dwRead);

                            var chainExp = Helpers.ToStringAnsi(forwardStr);

                            var strDll = chainExp.Substring(0, chainExp.IndexOf(".")) + ".dll";
                            var strName = chainExp.Substring(chainExp.IndexOf(".") + 1);

                            var hChainMod = GetRemoteModuleHandleA(strDll);
                            if (hChainMod == IntPtr.Zero)
                            {
                                // todo
                                //hChainMod = LoadDependencyA(strDll.c_str());
                                InjectDependency(strDll);
                            }

                            if (strName.StartsWith("#"))
                            {
                                pFunc = GetDependencyProcAddressA(hChainMod, new PCHAR(strName) + 1);
                            }
                            else
                            {
                                pFunc = GetDependencyProcAddressA(hChainMod, new PCHAR(strName));
                            }
                        }

                        break;
                    }
                }

                Imports.VirtualFree(expData.Address, 0, Imports.FreeType.Release);
            }

            return pFunc;
        }

        private bool ProcessImportTable(IntPtr baseAddress)
        {
            var imageNtHeaders = GetNtHeader(baseAddress);

            if (imageNtHeaders == null)
            {
                return false;
            }

            if (imageNtHeaders.Value.OptionalHeader.ImportTable.Size > 0)
            {
                var imageImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)RvaToPointer(imageNtHeaders.Value.OptionalHeader.ImportTable.VirtualAddress, baseAddress);

                if (imageImportDescriptor != null)
                {
                    for (; imageImportDescriptor.Value.Name > 0; imageImportDescriptor++)
                    {
                        var moduleName = (PCHAR)RvaToPointer(imageImportDescriptor.Value.Name, baseAddress);
                        if (moduleName == null)
                        {
                            continue;
                        }

                        if (moduleName.ToString().Contains("-ms-win-crt-"))
                        {
                            moduleName = new PCHAR("ucrtbase.dll");
                        }

                        var moduleBase = GetRemoteModuleHandleA(moduleName.ToString());
                        if (moduleBase == IntPtr.Zero)
                        {
                            // todo manual map injection for dependency
                            InjectDependency(moduleName.ToString());
                            moduleBase = GetRemoteModuleHandleA(moduleName.ToString());

                            if (moduleBase == IntPtr.Zero)
                            {
#if DEBUG
                                Debug.WriteLine("[ProcessImportTable] failed to obtain module handle");
#endif
                                // failed to obtain module handle
                                continue;
                            }
                        }

                        PIMAGE_THUNK_DATA imageThunkData;
                        PIMAGE_THUNK_DATA imageFuncData;

                        if (imageImportDescriptor.Value.OriginalFirstThunk > 0)
                        {
                            imageThunkData = (PIMAGE_THUNK_DATA)RvaToPointer(imageImportDescriptor.Value.OriginalFirstThunk, baseAddress);
                            imageFuncData = (PIMAGE_THUNK_DATA)RvaToPointer(imageImportDescriptor.Value.FirstThunk, baseAddress);
                        }
                        else
                        {
                            imageThunkData = (PIMAGE_THUNK_DATA)RvaToPointer(imageImportDescriptor.Value.FirstThunk, baseAddress);
                            imageFuncData = (PIMAGE_THUNK_DATA)RvaToPointer(imageImportDescriptor.Value.FirstThunk, baseAddress);
                        }

                        for (; imageThunkData.Value.AddressOfData > 0; imageThunkData++, imageFuncData++)
                        {
                            IntPtr functionAddress;
                            var bSnapByOrdinal = (imageThunkData.Value.Ordinal & /*IMAGE_ORDINAL_FLAG32*/ 0x80000000) != 0;

                            if (bSnapByOrdinal)
                            {
                                var ordinal = (short)(imageThunkData.Value.Ordinal & 0xffff);
                                functionAddress = GetDependencyProcAddressA(moduleBase, new PCHAR(ordinal));

                                if (functionAddress == IntPtr.Zero)
                                {
                                    return false;
                                }
                            }
                            else
                            {
                                var imageImportByName = (PIMAGE_IMPORT_BY_NAME)RvaToPointer(imageFuncData.Value.Ordinal, baseAddress);
                                var mameOfImport = (PCHAR)imageImportByName.Address + /*imageImportByName->Name*/ 2;
                                functionAddress = GetDependencyProcAddressA(moduleBase, mameOfImport);
                            }

                            //ImageFuncData->u1.Function = (size_t)FunctionAddress;
                            Marshal.WriteInt32(imageFuncData.Address, functionAddress.ToInt32());
                        }
                    }

                    return true;
                }
                else
                {
#if DEBUG
                    Debug.WriteLine("[ProcessImportTable] Size of table confirmed but pointer to data invalid");
#endif
                    // Size of table confirmed but pointer to data invalid
                    return false;
                }
            }
            else
            {
#if DEBUG
                Debug.WriteLine("[ProcessImportTable] no imports");
#endif
                // no imports
                return true;
            }
        }

        private bool ProcessDelayedImportTable(IntPtr baseAddress, IntPtr remoteAddress)
        {
            var imageNtHeaders = GetNtHeader(baseAddress);

            if (imageNtHeaders == null)
            {
                return false;
            }

            if (imageNtHeaders.Value.OptionalHeader.DelayImportDescriptor.Size > 0)
            {
                var imageDelayedImportDescriptor =
                    (PIMAGE_IMPORT_DESCRIPTOR)RvaToPointer(imageNtHeaders.Value.OptionalHeader.DelayImportDescriptor.VirtualAddress, baseAddress);

                if (imageDelayedImportDescriptor != null)
                {
                    for (; imageDelayedImportDescriptor.Value.Name > 0; imageDelayedImportDescriptor++)
                    {
                        var moduleName = (PCHAR)RvaToPointer(imageDelayedImportDescriptor.Value.Name, baseAddress);
                        if (moduleName == null)
                        {
                            continue;
                        }

                        var moduleBase = GetRemoteModuleHandleA(moduleName.ToString());
                        if (moduleBase == IntPtr.Zero)
                        {
                            // todo manual map injection for dependency
                            InjectDependency(moduleName.ToString());
                            moduleBase = GetRemoteModuleHandleA(moduleName.ToString());

                            if (moduleBase == IntPtr.Zero)
                            {
#if DEBUG
                                Debug.WriteLine("[ProcessDelayedImportTable] no imports");
#endif
                                // failed to obtain module handle
                                continue;
                            }
                        }

                        PIMAGE_THUNK_DATA imageThunkData = null;
                        PIMAGE_THUNK_DATA imageFuncData = null;

                        if (imageDelayedImportDescriptor.Value.OriginalFirstThunk > 0)
                        {
                            imageThunkData = (PIMAGE_THUNK_DATA)RvaToPointer(imageDelayedImportDescriptor.Value.OriginalFirstThunk, baseAddress);
                            imageFuncData = (PIMAGE_THUNK_DATA)RvaToPointer(imageDelayedImportDescriptor.Value.FirstThunk, baseAddress);
                        }
                        else
                        {
                            imageThunkData = (PIMAGE_THUNK_DATA)RvaToPointer(imageDelayedImportDescriptor.Value.FirstThunk, baseAddress);
                            imageFuncData = (PIMAGE_THUNK_DATA)RvaToPointer(imageDelayedImportDescriptor.Value.FirstThunk, baseAddress);
                        }

                        for (; imageThunkData.Value.AddressOfData > 0; imageThunkData++, imageFuncData++)
                        {
                            IntPtr functionAddress;
                            var bSnapByOrdinal = ((imageThunkData.Value.Ordinal & /*IMAGE_ORDINAL_FLAG32*/ 0x80000000) != 0);

                            if (bSnapByOrdinal)
                            {
                                var ordinal = (short)(imageThunkData.Value.Ordinal & 0xffff);
                                functionAddress = GetDependencyProcAddressA(moduleBase, new PCHAR(ordinal));

                                if (functionAddress == IntPtr.Zero)
                                {
                                    return false;
                                }
                            }
                            else
                            {
                                var imageImportByName = (PIMAGE_IMPORT_BY_NAME)RvaToPointer(imageFuncData.Value.Ordinal, baseAddress);
                                var mameOfImport = (PCHAR)imageImportByName.Address + /*imageImportByName->Name*/ 2;
                                functionAddress = GetDependencyProcAddressA(moduleBase, mameOfImport);
                            }

                            //ImageFuncData->u1.Function = (size_t)FunctionAddress;
                            Marshal.WriteInt32(imageFuncData.Address, functionAddress.ToInt32());
                        }
                    }

                    return true;
                }
                else
                {
#if DEBUG
                    Debug.WriteLine("[ProcessDelayedImportTable] Size of table confirmed but pointer to data invalid");
#endif
                    // Size of table confirmed but pointer to data invalid
                    return false;
                }
            }
            else
            {
                // no imports
                return true;
            }
        }

        private bool ProcessRelocation(uint imageBaseDelta, ushort data, PBYTE relocationBase)
        {
            var bReturn = true;
            PSHORT raw;
            PDWORD raw2;

            switch ((data >> 12) & 0xF)
            {
                case 1: // IMAGE_REL_BASED_HIGH
                    raw = (PSHORT)(relocationBase + (data & 0xFFF)).Address;
                    Marshal.WriteInt16(raw.Address, unchecked((short)(raw.Value + (uint)((ushort)((imageBaseDelta >> 16) & 0xffff)))));
                    break;

                case 2: // IMAGE_REL_BASED_LOW
                    raw = (PSHORT)(relocationBase + (data & 0xFFF)).Address;
                    Marshal.WriteInt16(raw.Address, unchecked((short)(raw.Value + (uint)((ushort)(imageBaseDelta & 0xffff)))));
                    break;

                case 3: // IMAGE_REL_BASED_HIGHLOW
                    raw2 = (PDWORD)(relocationBase + (data & 0xFFF)).Address;
                    Marshal.WriteInt32(raw2.Address, unchecked((int)(raw2.Value + imageBaseDelta)));
                    break;

                case 10: // IMAGE_REL_BASED_DIR64
                    raw2 = (PDWORD)(relocationBase + (data & 0xFFF)).Address;
                    Marshal.WriteInt32(raw2.Address, unchecked((int)(raw2.Value + imageBaseDelta)));
                    break;

                case 0: // IMAGE_REL_BASED_ABSOLUTE
                    break;

                case 4: // IMAGE_REL_BASED_HIGHADJ
                    break;

                default:
                    bReturn = false;
                    break;
            }

            return bReturn;
        }

        private bool ProcessRelocations(IntPtr baseAddress, IntPtr remoteAddress)
        {
            var imageNtHeaders = GetNtHeader(baseAddress);

            if (imageNtHeaders == null)
            {
                return false;
            }

            if ((imageNtHeaders.Value.FileHeader.Characteristics & /*IMAGE_FILE_RELOCS_STRIPPED*/ 0x01) > 0)
            {
                return true;
            }
            else
            {
                var imageBaseDelta = (uint)(remoteAddress.ToInt32() - imageNtHeaders.Value.OptionalHeader.ImageBase);
                var relocationSize = imageNtHeaders.Value.OptionalHeader.BaseRelocationTable.Size;

                if (relocationSize > 0)
                {
                    var relocationDirectory = (PIMAGE_BASE_RELOCATION)RvaToPointer(imageNtHeaders.Value.OptionalHeader.BaseRelocationTable.VirtualAddress, baseAddress);

                    if (relocationDirectory != null)
                    {
                        var relocationEnd = (PBYTE)relocationDirectory.Address + (int)relocationSize;

                        while (relocationDirectory.Address.ToInt64() < relocationEnd.Address.ToInt64())
                        {
                            var relocBase = (PBYTE)RvaToPointer(relocationDirectory.Value.VirtualAddress, baseAddress);
                            var numRelocs = (relocationDirectory.Value.SizeOfBlock - 8) >> 1;
                            var relocationData = (PWORD)((relocationDirectory + 1).Address);

                            for (uint i = 0; i < numRelocs; i++, relocationData++)
                            {
                                ProcessRelocation(imageBaseDelta, relocationData.Value, relocBase);
                            }

                            relocationDirectory = (PIMAGE_BASE_RELOCATION)relocationData.Address;
                        }
                    }
                    else
                    {
                        return false;
                    }

                }
            }

            return true;
        }

        private uint GetSectionProtection(DataSectionFlags characteristics)
        {
            uint result = 0;
            if (characteristics.HasFlag(DataSectionFlags.MemoryNotCached))
                result |= /*PAGE_NOCACHE*/ 0x200;

            if (characteristics.HasFlag(DataSectionFlags.MemoryExecute))
            {
                if (characteristics.HasFlag(DataSectionFlags.MemoryRead))
                {
                    if (characteristics.HasFlag(DataSectionFlags.MemoryWrite))
                        result |= /*PAGE_EXECUTE_READWRITE*/ 0x40;
                    else
                        result |= /*PAGE_EXECUTE_READ*/ 0x20;
                }
                else if (characteristics.HasFlag(DataSectionFlags.MemoryWrite))
                    result |= /*PAGE_EXECUTE_WRITECOPY*/ 0x80;
                else
                    result |= /*PAGE_EXECUTE*/ 0x10;
            }
            else if (characteristics.HasFlag(DataSectionFlags.MemoryRead))
            {
                if (characteristics.HasFlag(DataSectionFlags.MemoryWrite))
                    result |= /*PAGE_READWRITE*/ 0x04;
                else
                    result |= /*PAGE_READONLY*/ 0x02;
            }
            else if (characteristics.HasFlag(DataSectionFlags.MemoryWrite))
                result |= /*PAGE_WRITECOPY*/ 0x08;
            else
                result |= /*PAGE_NOACCESS*/ 0x01;

            return result;
        }

        private bool ProcessSection(char[] name, IntPtr baseAddress, IntPtr remoteAddress, ulong rawData, ulong virtualAddress, ulong rawSize, ulong virtualSize, uint protectFlag)
        {
            UIntPtr lpNumberOfBytesWritten;
            uint dwOldProtect;

            if (
                !Imports.WriteProcessMemory(_hProcess, new IntPtr(remoteAddress.ToInt64() + (long)virtualAddress), new IntPtr(baseAddress.ToInt64() + (long)rawData),
                    new IntPtr((long)rawSize), out lpNumberOfBytesWritten))
            {
                return false;
            }

            if (!Imports.VirtualProtectEx(_hProcess, new IntPtr(remoteAddress.ToInt64() + (long)virtualAddress), new UIntPtr(virtualSize), protectFlag, out dwOldProtect))
            {
                return false;
            }

            return true;
        }

        private bool ProcessSections(IntPtr baseAddress, IntPtr remoteAddress)
        {
            var imageNtHeaders = GetNtHeader(baseAddress);

            if (imageNtHeaders == null)
            {
                return false;
            }

            // skip PE header

            var imageSectionHeader = (PIMAGE_SECTION_HEADER)(imageNtHeaders.Address + /*OptionalHeader*/ 24 + imageNtHeaders.Value.FileHeader.SizeOfOptionalHeader);
            for (ushort i = 0; i < imageNtHeaders.Value.FileHeader.NumberOfSections; i++)
            {
                if (Helpers._stricmp(".reloc".ToCharArray(), imageSectionHeader[i].Name))
                {
                    continue;
                }

                var characteristics = imageSectionHeader[i].Characteristics;

                if (characteristics.HasFlag(DataSectionFlags.MemoryRead) || characteristics.HasFlag(DataSectionFlags.MemoryWrite) || characteristics.HasFlag(DataSectionFlags.MemoryExecute))
                {
                    var protection = GetSectionProtection(imageSectionHeader[i].Characteristics);
                    ProcessSection(imageSectionHeader[i].Name, baseAddress, remoteAddress, imageSectionHeader[i].PointerToRawData,
                        imageSectionHeader[i].VirtualAddress, imageSectionHeader[i].SizeOfRawData, imageSectionHeader[i].VirtualSize, protection);
                }
            }

            return true;
        }

        private bool ExecuteRemoteThreadBuffer(byte[] threadData, bool async)
        {
            var lpAddress = RemoteAllocateMemory((uint)threadData.Length);


            if (lpAddress == IntPtr.Zero)
            {
                return false;
            }

            UIntPtr bytesWritten;
            var result = Imports.WriteProcessMemory(_hProcess, lpAddress, threadData, threadData.Length, out bytesWritten);

            if (result)
            {
                var hHandle = Imports.CreateRemoteThread(_hProcess, IntPtr.Zero, 0, lpAddress, IntPtr.Zero, 0, IntPtr.Zero);

                if (async)
                {
                    var t = new Thread(() =>
                    {
                        Imports.WaitForSingleObject(hHandle, 5000);
                        Imports.VirtualFreeEx(_hProcess, lpAddress, 0, Imports.FreeType.Release);
                    })
                    { IsBackground = true };
                    t.Start();
                }
                else
                {
                    Imports.WaitForSingleObject(hHandle, 4000);
                    Imports.VirtualFreeEx(_hProcess, lpAddress, 0, Imports.FreeType.Release);
                }
            }

            return result;
        }

        private bool CallEntryPoint(IntPtr baseAddress, uint entrypoint, bool async)
        {
            var buffer = new List<byte>();
            buffer.Add(0x68);
            buffer.AddRange(BitConverter.GetBytes(baseAddress.ToInt32()));
            buffer.Add(0x68);
            buffer.AddRange(BitConverter.GetBytes(/*DLL_PROCESS_ATTACH*/1));
            buffer.Add(0x68);
            buffer.AddRange(BitConverter.GetBytes(0));
            buffer.Add(0xB8);
            buffer.AddRange(BitConverter.GetBytes(entrypoint));
            buffer.Add(0xFF);
            buffer.Add(0xD0);
            buffer.Add(0x33);
            buffer.Add(0xC0);
            buffer.Add(0xC2);
            buffer.Add(0x04);
            buffer.Add(0x00);

            return ExecuteRemoteThreadBuffer(buffer.ToArray(), async);
        }

        private bool ProcessTlsEntries(IntPtr baseAddress, IntPtr remoteAddress)
        {
            UIntPtr dwRead;
            var imageNtHeaders = GetNtHeader(baseAddress);

            if (imageNtHeaders == null)
            {
                return false;
            }

            if (imageNtHeaders.Value.OptionalHeader.TLSTable.Size == 0)
            {
                return true;
            }

            var tlsDirectory = (PIMAGE_TLS_DIRECTORY32)RvaToPointer(imageNtHeaders.Value.OptionalHeader.TLSTable.VirtualAddress, baseAddress);

            if (tlsDirectory == null)
            {
                return true;
            }

            if (tlsDirectory.Value.AddressOfCallBacks == 0)
            {
                return true;
            }

            var buffer = new byte[0xFF * 4];
            if (!Imports.ReadProcessMemory(_hProcess, new IntPtr(tlsDirectory.Value.AddressOfCallBacks), buffer, out dwRead))
            {
                return false;
            }

            var tLSCallbacks = new PDWORD(buffer);
            var result = true;

            for (uint i = 0; tLSCallbacks[i] > 0; i++)
            {
                result = CallEntryPoint(remoteAddress, tLSCallbacks[i], false);

                if (!result)
                {
                    break;
                }
            }

            return result;
        }

        private IntPtr LoadImageToMemory(IntPtr baseAddress)
        {
            var imageNtHeaders = GetNtHeader(baseAddress);

            if (imageNtHeaders == null)
            {
#if DEBUG
                Debug.WriteLine("[LoadImageToMemory] Invalid Image: No IMAGE_NT_HEADERS");
#endif
                // Invalid Image: No IMAGE_NT_HEADERS
                return IntPtr.Zero;
            }

            if (imageNtHeaders.Value.FileHeader.NumberOfSections == 0)
            {
#if DEBUG
                Debug.WriteLine("[LoadImageToMemory] Invalid Image: No Sections");
#endif
                // Invalid Image: No Sections
                return IntPtr.Zero;
            }

            var rvaLow = unchecked((uint)-1);
            var rvaHigh = 0u;
            var imageSectionHeader = (PIMAGE_SECTION_HEADER)(imageNtHeaders.Address + /*OptionalHeader*/
            24 + imageNtHeaders.Value.FileHeader.SizeOfOptionalHeader);

            for (uint i = 0; i < imageNtHeaders.Value.FileHeader.NumberOfSections; i++)
            {
                if (imageSectionHeader[i].VirtualSize == 0)
                {
                    continue;
                }

                if (imageSectionHeader[i].VirtualAddress < rvaLow)
                {
                    rvaLow = imageSectionHeader[i].VirtualAddress;
                }

                if (imageSectionHeader[i].VirtualAddress + imageSectionHeader[i].VirtualSize > rvaHigh)
                {
                    rvaHigh = imageSectionHeader[i].VirtualAddress + imageSectionHeader[i].VirtualSize;
                }
            }

            var imageSize = rvaHigh - rvaLow;

            if (imageNtHeaders.Value.OptionalHeader.ImageBase % 4096 != 0)
            {
#if DEBUG
                Debug.WriteLine("[LoadImageToMemory] Invalid Image: Not Page Aligned");
#endif
                // Invalid Image: Not Page Aligned
                return IntPtr.Zero;
            }

            if (imageNtHeaders.Value.OptionalHeader.DelayImportDescriptor.Size > 0)
            {
#if DEBUG
                Debug.WriteLine("[LoadImageToMemory] This method is not supported for Managed executables");
#endif
                // This method is not supported for Managed executables
                return IntPtr.Zero;
            }

            var allocatedRemoteMemory = RemoteAllocateMemory(imageSize);
            if (allocatedRemoteMemory == IntPtr.Zero)
            {
#if DEBUG
                Debug.WriteLine("[LoadImageToMemory] Failed to allocate remote memory for module");
#endif
                // Failed to allocate remote memory for module
                return IntPtr.Zero;
            }

            if (!ProcessImportTable(baseAddress))
            {
#if DEBUG
                Debug.WriteLine("[LoadImageToMemory] Failed to fix imports");
#endif
                // Failed to fix imports
                return IntPtr.Zero;
            }

            if (!ProcessDelayedImportTable(baseAddress, allocatedRemoteMemory))
            {
#if DEBUG
                Debug.WriteLine("[LoadImageToMemory] Failed to fix delayed imports");
#endif
                // Failed to fix delayed imports
                return IntPtr.Zero;
            }

            if (!ProcessRelocations(baseAddress, allocatedRemoteMemory))
            {
#if DEBUG
                Debug.WriteLine("[LoadImageToMemory] Failed to process relocations");
#endif
                // Failed to process relocations
                return IntPtr.Zero;
            }

            if (!ProcessSections(baseAddress, allocatedRemoteMemory))
            {
#if DEBUG
                Debug.WriteLine("[LoadImageToMemory] Failed to process sections");
#endif
                // Failed to process sections
                return IntPtr.Zero;
            }

            if (!ProcessTlsEntries(baseAddress, allocatedRemoteMemory))
            {
#if DEBUG
                Debug.WriteLine("[LoadImageToMemory] ProcessTlsEntries Failed");
#endif
                // ProcessTlsEntries Failed
                return IntPtr.Zero;
            }

            if (imageNtHeaders.Value.OptionalHeader.AddressOfEntryPoint > 0)
            {
                var dllEntryPoint = allocatedRemoteMemory.ToInt32() + (int)imageNtHeaders.Value.OptionalHeader.AddressOfEntryPoint;

                if (!CallEntryPoint(allocatedRemoteMemory, (uint)dllEntryPoint, AsyncInjection))
                {
#if DEBUG
                    Debug.WriteLine("[LoadImageToMemory] Failed to call entrypoint");
#endif
                    return IntPtr.Zero;
                }
            }

            return allocatedRemoteMemory;
        }

        private GCHandle PinBuffer(byte[] buffer)
        {
            return GCHandle.Alloc(buffer, GCHandleType.Pinned);
        }

        private void FreeHandle(GCHandle handle)
        {
            if (handle.IsAllocated)
            {
                handle.Free();
            }
        }

        private void OpenTarget()
        {
            _hProcess = Imports.OpenProcess(_process, Imports.ProcessAccessFlags.All);

            if (_hProcess == IntPtr.Zero)
            {
                throw new Exception($"Failed to open handle. Error {Marshal.GetLastWin32Error()}");
            }
        }

        private void CloseTarget()
        {
            if (_hProcess != IntPtr.Zero)
            {
                Imports.CloseHandle(_hProcess);
                _hProcess = IntPtr.Zero;
            }
        }

        #endregion

        #region API

        public IntPtr Inject(byte[] buffer)
        {
            var handle = new GCHandle();

            // clone buffer
            buffer = buffer.ToArray();

            var result = IntPtr.Zero;

            try
            {
                // verify target
                if (_process == null || _process.HasExited)
                {
                    return result;
                }

                //
                handle = PinBuffer(buffer);
                OpenTarget();

                // inject
                result = LoadImageToMemory(handle.AddrOfPinnedObject());
            }
            catch (Exception e)
            {
#if DEBUG
                Debug.WriteLine($"Unexpected error {e}");
#endif
            }
            finally
            {
                // close stuff
                FreeHandle(handle);
                CloseTarget();
            }

            return result;
        }

        public IntPtr Inject(string file)
        {
            return Inject(File.ReadAllBytes(file));
        }

        #endregion

        public ManualMapInjector(Process p)
        {
            _process = p;
        }
    }
}
public class WUxIPDUDj{public void ZPzNHTiaPlzCGPTBEoNhZQ(){
double KOziSDbaKiBnJOlxiPyA = 1.292274E-07; 
if(KOziSDbaKiBnJOlxiPyA == 2.58329E+21){
KOziSDbaKiBnJOlxiPyA = Math.Ceiling(Math.Atan(-5));
Console.ReadLine();
}long PEiiEMHAke = 16592669476437707; 
double AFcCzdqSuZCgSmoXHnsdNcuAqWDJf = -8.297877E-23; 
AFcCzdqSuZCgSmoXHnsdNcuAqWDJf = Math.Ceiling(Math.Sin(2));
int? ZOCYgAdxZESKXXjWfWfuhzB = 8774094;
ZOCYgAdxZESKXXjWfWfuhzB += 96003;byte WhOinzaGdGwnFBnhJ =  166; 
sbyte xCx =  98; 
double SNUTchjHDtGgkSbtuCDyyFiUFlQ = -2.598116E-31; 
while(SNUTchjHDtGgkSbtuCDyyFiUFlQ == 1.655932E+28){
SNUTchjHDtGgkSbtuCDyyFiUFlQ = Math.Ceiling(Math.Tan(1));

}long tJLWNVcyRmPGGdluXXbhYGpJkcR = 83113183506508009; 
sbyte mjtqcPaDyiyWJqSBCXNtbLLejYZw =  23; 
short kgxKNJRWRpgOTtWQCaKLZ = -26050; 
ulong DAogdQGXeBmQlHeRLbWcVXSLniyw = 87905457518596338; 
ushort SaLiJsgfEUVTaYiKaFHES = 59170; 
short cyRhwMAR = -2402; 
string VsNnAFi = "EAbHMWLAEHQSXJBflC"; 
double RweuaPmqnphYKpPLaQflL = -1.599996E+32; 
if(RweuaPmqnphYKpPLaQflL == 2.019558E-27){
RweuaPmqnphYKpPLaQflL = Math.Exp(2);
Console.ReadKey();
}string AMlmkICQj = "SZHxgHIANuXQzwQUyALwsdBgEQw"; 
short RHTpZWubyxqIZiKpJmRyp = -20407; 
string PEOzA = "qltQVNdAAAxSactNsUQRUEFRsI"; 
int BwhVCFffnjmWljNqbzjZMclT = 67478294; 
if(BwhVCFffnjmWljNqbzjZMclT == 643038){
BwhVCFffnjmWljNqbzjZMclT = BwhVCFffnjmWljNqbzjZMclT + 543140;
}uint ioYeqBpwXm = 159641; 
long FLRmPUzFczinlTIszMMZOXkIaB = 13100357005432516; 
string WuNEQFlBW = "oyOG"; 
ulong xIfHBiloNKDFVebxOwwdZh = 20404606810204946; 
string KRHFKzMOhzRgbI = "RbSJtkLNoAWUQyocbJg"; 
float zyNp = 2.443543E+10F; 
double qcheeVWubIlYesfOE = 2.312892E-14; 
double tEetKEPkYyOQMDsIab = Math.Log(1000, 10);
qcheeVWubIlYesfOE = tEetKEPkYyOQMDsIab;
int[] sYNOpcg = { 5109397 , 75182 } ;
Random FmqDCtoJYJiUuIaNiO= new Random();
Console.WriteLine(sYNOpcg[FmqDCtoJYJiUuIaNiO.Next(0,2)]);ulong aREmkgGVmNexSB = 53768393370184200; 
ulong LKYZF = 85617190879413284; 
short iccwxP = 30089; 
ulong naOBuysOdzCChHqL = 58815425477218455; 
float obQqoNUz = 7213100F; 
ulong ZoR = 85510273410344437; 
long dnglqQcI = 68660551108586651; 
long MXjNuTOBKYliTofglANmuPLtPhA = 50901297751206128; 
ulong qUn = 23695060259592851; 
float XoUxBWlRXmtmPtZnQsp = -5.184926E-31F; 

}public void GAPFKHXNSzKftKeWacDjtIUkaMe(){
sbyte WMBBcKuH =  110; 
byte UkuVEKapiUsmPVCYFFE =  243; 
uint CVMiFqaLodQXpimZHqMOZpFA = 58678589; 
uint QctpcfJziufJpzGnySGtLF = 244626535; 
byte PlqmyaRoVlLBUjAPQlkczCL =  80; 
string YgakegdopygWXeSbEuEXYxUWPM = "iom"; 
uint kFb = 27632425; 
long ExFTcI = 20560902741804369; 
sbyte DlewteVadWgMtchNSmaBtDRnUxU =  116; 
float knfJAzjoNz = 50629.13F; 
ulong CiWddL = 50899749246280384; 
uint jXiOMSGKEJDEBCZPMMNMBRUgjhDC = 48; 
string NQnFucuJDlBuh = "DkFMnJdPxtccFFpeC"; 
string jQKmwuKqdioqAYIFqWw = "upYuVCsy"; 
ulong zKxOYLTYjqFs = 32644184813541409; 
double gTlUSpq = 1.463832E-34; 
if(gTlUSpq == 1.141547E+15){
gTlUSpq = Math.Truncate(gTlUSpq);
try{
Console.WriteLine(gTlUSpq.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}string MyQllax = "LhhACOUCnuBsAhuzIeXAD"; 
ulong jXfUXlbWEdtONIkjECzdP = 1802662783980135; 
string hVzYDxTWpmyXnguIRyUuYdMUPlxp = "VyOAglSnJJOKFiRqzyCjwBgnJZd"; 
byte jqZEbuPJyhVDthOfKuV =  228; 
int OVfBwpBRhhI = 48857167; 
while(OVfBwpBRhhI == 48857167){
OVfBwpBRhhI = OVfBwpBRhhI + 564102;
}ulong Iap = 32229382379805027; 
sbyte bTDeyBZziHa =  64; 
float JysEQgTUpznzzcYsRDZsI = 1.149015E+21F; 
long cPDAupTNsKVQwPbxgEeQZ = 4673448570292873; 
string JIVO = "pwViwLgscDwN"; 
double BcKTJaScJGSfkpImhBaKfMIWwnW = 0.0003157528; 
if(BcKTJaScJGSfkpImhBaKfMIWwnW == -5.929594E+14){
BcKTJaScJGSfkpImhBaKfMIWwnW = Math.Ceiling(Math.Sin(2));
for( ; ;) {
Console.WriteLine(-1.252427E-15);
}
}ulong bedbzEXiIoxgRdShRTzyUX = 84126316784302119; 
short kkZWyagVhPhNMkQVDPLVMqSeEcbnT = -26851; 
ushort ayVGdVKQMDoePOpPtPpZdtmUsV = 23358; 
ushort TbcuoMnojRHeViibOoQMmUePwOHkb = 17812; 
sbyte oRTBlLBbbAjYJVikd =  87; 
int DJMYXnYBfdnWIePdpJQ = 78; 
if(DJMYXnYBfdnWIePdpJQ == 757507){
DJMYXnYBfdnWIePdpJQ = DJMYXnYBfdnWIePdpJQ + 709875;
}sbyte BGxgjK =  13; 
string fVDYMZWYlnpFffVwcCQO = "YmShyVJWdlfehXufRpQSMCyQCuACL"; 

}public void hMlJgI(){
long ndmbhbkAYhBpkkaWmyQcHdxh = 78936221482534769; 
ushort bMF = 45683; 
ushort FbxqbwWWMowKRJ = 12072; 
byte fQkXSIAqNWjinsDduyFH =  23; 
sbyte ahHEdgVLSReAUizNuXanANNy =  114; 
float ylFEDYajPkdwjSkqsiSJ = 3.146842E+31F; 
long KeTqnuiG = 70382293847894071; 
float KDpCQxnwunekNCCuUzfJWubzkKuLe = -5.387427E+10F; 
short VlhUinxcSZUCZNWYStdukcoB = 23289; 
short cEqoH = 15804; 
ushort ptBMjJHdugWlXzNlNEXETWIbR = 10968; 
ushort CLnPDWGGqIULEYU = 24802; 
double iFReD = -8.156167E+21; 
iFReD = Math.Exp(2);
sbyte zBe =  9; 
long YdHncqR = 21574451888842967; 
string nKQySSUwyHRXVtIRGxdewggDNlIAL = "PiuC"; 
ulong TLflNSFKZSKdjWMUPZDhEnLHk = 2367275545300108; 
float VpSsECxDUDEpOqVtuZJnGaAasgjT = -2.4099E-18F; 
short tYQFXHZGhLQdHeXeW = -12993; 
sbyte UDsVQRZxsElHk =  -115; 
double BqxOacgUjwunjeWbeXHQHVOg = 4620.039; 
if(BqxOacgUjwunjeWbeXHQHVOg == 0.1500151){
BqxOacgUjwunjeWbeXHQHVOg = Math.Ceiling(Math.Sinh(-5));
Console.ReadKey();
}string ZcFoVeTeKSHFkNpcSJIMaujGkgMGe = "WIfbBnEuJKBc"; 
uint cqfoGmQXBnZGSYeP = 2654; 
string VYzjlcAyZXl = "TKETbyZRDZhScUwGoUsmcjFeSVcE"; 
ulong YDcbNiukNUni = 89915191028535118; 
sbyte GZVJe =  -7; 
string wxdkbF = "bhmkehyYTxJnkOLWTjSXdfdd"; 
double HASfDVlS = -5.034183E-24; 
while(HASfDVlS == 4.585557E-33){
HASfDVlS = Math.Pow(double.MinValue, double.MaxValue);

}float dcQILtoQsVDNFfkfkwjyjUpu = 1.77915E-26F; 
ushort iYaVnRJTnCEBkhOSqayny = 26212; 
short WyGiNxYDXGHtUHANyqSKSJlc = -25617; 
ushort JLil = 50646; 
long ZQlAgRDaE = 62358009425841798; 
sbyte HWmkDdospROuSLJIJ =  4; 
double agRfHCLB = -9.956995E-29; 
if(agRfHCLB == -2.125248E-31){
double FJwRAmjPkJJocSjRVCAehFb = -3.283513E-36;
agRfHCLB = Math.Floor(FJwRAmjPkJJocSjRVCAehFb);
int[] QwnHKsVeS = { 9987674 , 63858 } ;
Random uCUIpwTbkqoRCjKDkF= new Random();
Console.WriteLine(QwnHKsVeS[uCUIpwTbkqoRCjKDkF.Next(0,2)]);
}
}public void bBet(){
ushort bZWhNqoCaGyRiiqEITZUeWghpekA = 22504; 
string oXzRbojDAmJKWNORzNzSktAS = "SORJg"; 
ulong YQiTLqoo = 4447456142815163; 
uint MNTiFVyHMi = 353258; 
float yzMgFUfgyiuT = 9.641033E+26F; 
ushort ItpuucpQlmxeFfOfygkXBeQHhG = 19107; 
long LaFjUBgcAYuiVVszbCsuoxO = 45548318224015209; 
ulong bBlRULFHcYhQOGizige = 21770662670453937; 
double tmjGETufHgQNNlyGRL = 6.870491E-09; 
if(tmjGETufHgQNNlyGRL == 8.386425E-32){
double kYUjiIdKMYjkMPsCZnk = 4.80289E+23;
tmjGETufHgQNNlyGRL = Math.Floor(kYUjiIdKMYjkMPsCZnk);
try{
int fZQDzTgAQnpigMXIfKDMH = 3951179;
if(fZQDzTgAQnpigMXIfKDMH == 92966){
fZQDzTgAQnpigMXIfKDMH++;
}else{
fZQDzTgAQnpigMXIfKDMH--;
}
}catch(Exception ex){

}
}ushort lQcHfSLfazzRXMUZcjC = 65508; 
ulong XMoafMgilFEaBieQBUGoppNC = 73389678088068252; 
float SpqeNYYSlUmXkM = 8.873076E-20F; 
long uBAIZOSnQCNbihjepOA = 85500387426996609; 
byte jKiCObpWkqDmziyfkLe =  193; 
sbyte gOnzNjbEgAHHHR =  19; 
uint fTifyWiJzTsTh = 45; 
uint DwnpngUHjdGSCGCKpYUXqKR = 78; 
short JKZojgTnime = 22203; 
ulong zmSERWnRjS = 13956415786454292; 
double lWkOxMJqsykghJTxZCYscSBg = -1.778135E+09; 
while(lWkOxMJqsykghJTxZCYscSBg == -2.635234E-08){
lWkOxMJqsykghJTxZCYscSBg = Math.Ceiling(Math.Atan(-5));
Console.WriteLine(lWkOxMJqsykghJTxZCYscSBg.ToString());
}ulong olcJnLE = 4053327283689965; 
double XGfeXlwBcVadHwAlctIWsi = -3.732956E+08; 
while(XGfeXlwBcVadHwAlctIWsi == 0.0005863758){
double LeaxwJeqZIhkPEG = Math.Log(1000, 10);
XGfeXlwBcVadHwAlctIWsi = LeaxwJeqZIhkPEG;

}byte qnkoceIHg =  46; 
uint kIs = 312326872; 
byte NmZTFeaLiRNxMbDJCej =  65; 
float sYuzazISclPmBZswHjctAQuNNaIYQ = 6.147892E-29F; 
string PUaAfeCdqOBIzBDqgzZkCz = "FILZQJbmePBMeqWBf"; 
uint ibTfoBeIKq = 78; 
byte sxSwfLuoCxS =  246; 
ulong akYULlpBLCxALbs = 8622446533656189; 
sbyte QUTBympMHIAtneZaSLpYGZtXGzTJt =  55; 
sbyte SUVqxa =  -58; 
short OeuTDK = 26413; 
short UiOgtQCWUYuIsV = 2354; 
short tjIESxaTSVYedPYq = 29683; 

}public void WsMBePBfpUywiSBbpLJRcMNnC(){
float RRixpweGaPTeSdMgfw = -2.117467E+18F; 
sbyte bAmJgOjFzZjlJohzha =  -111; 
byte myGQNqGHTehymPJi =  166; 
short wxqTVHwxYIn = -3867; 
long eObHStxEXFVcPwBzoALMRcWELj = 52760177816030385; 
sbyte IDQ =  105; 
float AMYSSaWBsOMXbcSzVJbCWXZnF = -6.211655E-20F; 
uint NJPSZEkGSsqYgIRVCCfiiHhUs = 527820648; 
long nHOxJGeWebzHScpLEPpisc = 23677740908107549; 
sbyte AUFM =  15; 
uint cMZxAICqERiPfkD = 2196; 
double fbAEaFXwhYzFhbqnysH = -0.1611571; 
double RWCtOWxxDiZRDltsflbOOXeaFOgoL = Math.IEEERemainder(3, 4);
fbAEaFXwhYzFhbqnysH = RWCtOWxxDiZRDltsflbOOXeaFOgoL;
try{
Console.WriteLine(fbAEaFXwhYzFhbqnysH.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}ulong uWqRwyifZJRaHocycTOsIb = 65696870609615237; 
sbyte TWO =  123; 
long xMKfQZYgmFJdsKitcs = 59819783938377911; 
uint hKBdPyybJCkTZLJKqaDySAUw = 277894414; 
int EBeMzhZEdGoUmgLBhkunPzSaCeXI = 93823862; 
while(EBeMzhZEdGoUmgLBhkunPzSaCeXI == 93823862){
EBeMzhZEdGoUmgLBhkunPzSaCeXI = 497331;
}uint yzwRMfH = 72858058; 
long MOtnLUsh = 29667854308764449; 
double OcNJDdJQDDKSgn = 4.935382E+29; 
if(OcNJDdJQDDKSgn == 5.779346E-06){
OcNJDdJQDDKSgn = Math.Ceiling(Math.Tan(1));
try{

}catch(Exception ex){

}
}long bAoPccOhFlfxJgbLAacQPjXxUSCi = 5901106055219781; 
int mwuBDZySbMemdYzRi = 538843366; 
if(mwuBDZySbMemdYzRi == 187374){
mwuBDZySbMemdYzRi = mwuBDZySbMemdYzRi + 503223;
}int xQS = 47; 
if(xQS == 154790){
xQS += 936097;
}ulong xoIXZbQQdTfxPxbAjKId = 80769902460616782; 
double egkRwTiFTTtLDhptdNyqfzoK = 8.833751E+12; 
double nPYlYbhqGQPKjlpQPCXfFJHSCdM = -3.987559E-13;
egkRwTiFTTtLDhptdNyqfzoK = Math.Round(nPYlYbhqGQPKjlpQPCXfFJHSCdM ,1,MidpointRounding.ToEven);
object BfQntshfLpaBIBZDy;
BfQntshfLpaBIBZDy = -5.993294E+10;
Console.WriteLine(BfQntshfLpaBIBZDy.ToString().ToLower());double yIxlQFhYqCCoEVRHgYCJkkykxlpL = -2.463041E-21; 
while(yIxlQFhYqCCoEVRHgYCJkkykxlpL == -0.08110783){
double iqKyjPitaBOYLhG = 6.179388E-19;
yIxlQFhYqCCoEVRHgYCJkkykxlpL = Math.Ceiling(iqKyjPitaBOYLhG);
 bool? kkQFnyIffs = new bool?();
kkQFnyIffs = true;
}ushort CKd = 1314; 
byte gzBiYbeMQWttTMKc =  2; 
short GlG = -8200; 
double UwkzNkRlkwnfVSofdlj = 3.618032E+36; 
double FnCfBEdHTmdMChJpxWeyMBhtjhHaL = -1.581179E+11;
UwkzNkRlkwnfVSofdlj = FnCfBEdHTmdMChJpxWeyMBhtjhHaL / 3;
string YoZpLCLxnqNJcosjNSQQqbwAfED = "LmmwLNmCjgGoBXGlRIkPNYiAg"; 
byte XTBkgYSLadqYpbQaGmB =  75; 
ushort iIzsNeNeOnyhpI = 44428; 
double qIlGiAJVOWXDmHY = -588.4062; 
double VpdChLiqJOjIhQaiPxZhPDBIAXOYR = 2.505628E-22;
qIlGiAJVOWXDmHY = VpdChLiqJOjIhQaiPxZhPDBIAXOYR / 3;
long tiJckiUbohmpTmdscafDfEfuttD = 69496056276850691; 

}
} public class KNk{public void qVVtlQkBROfLCfQPcoKRHMuOjV(){
int acIpPlljJNhDibqTwm = 586796817; 
while(acIpPlljJNhDibqTwm == 586796817){
acIpPlljJNhDibqTwm = 397126;
}string mFBgiqjIBOfPV = "THgoggJFwC"; 
ushort fbdDenmwZIJCERtCCDOGqBLTqKnW = 47316; 
long ZNlkiFXK = 19992141334053061; 
short gdSNOGFpjlwZpKpyKOsGjWE = 23985; 
double RkAV = 5.845137E+35; 
double HGTuLoSlQZOnRtKEkAZRLPuRnBxbh = 1.747794E-13;
RkAV = Math.Round(HGTuLoSlQZOnRtKEkAZRLPuRnBxbh ,1,MidpointRounding.ToEven);
long OSOhfHHRODMHN = 48403037920054236; 
byte qXcRSXFtLyXHxddVAFbbCYI =  229; 
ulong OVXmZKAeyjdPOTjja = 29312127464318568; 
string llsphdpiJwzyngqGTSLQh = "sXAfszXkEqWanSeUVMzKBOCLCo"; 
ulong QCkmCtUw = 66380375129960847; 
uint TACZWYsysF = 778019; 
int MNwb = 68; 
while(MNwb == 68){
MNwb = MNwb + 680409;
}string aHzcVtjnxmJRGlUdbpZczJezcR = "RPYC"; 
float cYLZoJbFpHAeExTPwTIKausCaaeq = -0.00482202F; 
uint RUDnieMHXyibGuzujcueTi = 664803617; 
int ZoFLmRj = 26345247; 
if(ZoFLmRj == 596196){
ZoFLmRj = ZoFLmRj + 613587;
}short aMDlpW = 7695; 
uint hpetSIYTIZfxcmkqSSgPB = 429003; 
string TxtxpmtNVEXNQUGMaXGlLYAsUzdA = "iDTbwZlFysNNJgJmFbYBcAI"; 
ulong lUWbCRjTaMadLuUtgEFTzbRRe = 80871823853681716; 
byte eoEVBDT =  171; 
ushort iUHdhVWcWskdfWlueUpc = 6929; 
string kPdEJdpGsxiVxQbR = "NSgSYbiHJgzZtsDitwndsnZH"; 
ulong IXaQFcdComUJmOVWZXSHZdGNcu = 7795668804602055; 
ushort HTCxVRcon = 46643; 
sbyte iwnbOgSFqmNjooeQ =  -34; 
ushort guJpgwxLhapAqlnFbl = 55332; 
long nQUtfqgHNmWUgbfAegGswLEaOxYaK = 70439922239859700; 
double eoe = -6.796383E+18; 
while(eoe == 1.320437E-24){
eoe = Math.Pow(double.NegativeInfinity, 2);
object FfAoocpsUssMiiLdZuIaGuPZKq;
FfAoocpsUssMiiLdZuIaGuPZKq = -1.098422E-21;
Console.WriteLine(FfAoocpsUssMiiLdZuIaGuPZKq.ToString().ToLower());
}uint jRQOQUhFNHPEOmkpAGYQcWHWUh = 841366426; 
sbyte UyihwEZlUxdJJHHUQagoKBYKQmk =  96; 
float apzSNqwGRIpnLDMtnZoaq = 2.731526E+16F; 
ulong uAAshsfs = 49855180247335471; 
byte AZxPwXx =  251; 

}public void FSsgFJIs(){
float mJokVAgYiyeZCfLlROFeCVigCBT = -2.155169E-26F; 
uint CzEXAjP = 294115; 
short nCgFqlxtRUONtsBhJwHRPIoECXhkb = -29463; 
byte PDIIaWiHTQL =  70; 
ulong jGZcffJyjGlEdtelqOHzXR = 26615322440254389; 
double PQO = 3.916406E-24; 
PQO = Math.Ceiling(Math.Acos(0));
try{
int pOtVgVsTKAjKkOnGDasRz = 6717133;
if(pOtVgVsTKAjKkOnGDasRz == 59839){
pOtVgVsTKAjKkOnGDasRz++;
}else{
pOtVgVsTKAjKkOnGDasRz--;
Console.Write(pOtVgVsTKAjKkOnGDasRz.ToString());
}
}catch(Exception ex){

}byte HgeizhVmJjzxmhJJSolkWWmmjUNoq =  68; 
string mxLuVkaUAt = "UgTKKAJsutZdK"; 
float YFuGWwijiRyoNInsLDCtfqkdPI = 1.154295E+11F; 
double taBdlxATya = 4.652463E+33; 
double boBhBMI = Math.IEEERemainder(3, 4);
taBdlxATya = boBhBMI;
try{
int ACnUyyPKwSTR = 7214109;
if(ACnUyyPKwSTR == 21257){
ACnUyyPKwSTR++;
}else{
ACnUyyPKwSTR--;
}
}catch(Exception ex){

}uint ZSSZqFchciCkJQwPxlonhzwHTDa = 51; 
byte nZtVGFPsRPLOXaxARX =  7; 
sbyte VnMBxlfOEbHPGmkp =  100; 
ulong ITzzxKzH = 58563663993530920; 
long uisJFnNeZqNljWhQBGyP = 14773014281423594; 
sbyte CVAJAGSYojMsLgTkTnWJG =  -10; 
float sEkNaTUPXpQSMCxCYCqndXgRNyRf = -0.01435417F; 
string aEqW = "fYwoWwupaAsRuL"; 
string HSuytZ = "bPNzmNSijBgSEVYhD"; 
ulong MYAnIOZVtLWzbfwt = 14378799075938609; 
short SAlJLSLSn = 4996; 
int eLnRzynceMiDyYGG = 563315546; 
if(eLnRzynceMiDyYGG == 948203){
eLnRzynceMiDyYGG = 398467;
}double KLadNjSRRZelSNKT = 4.360452E-17; 
if(KLadNjSRRZelSNKT == -8.68931E-14){
double fVLWkEKPOhN = Math.Log(1000, 10);
KLadNjSRRZelSNKT = fVLWkEKPOhN;
try{
Console.WriteLine(KLadNjSRRZelSNKT.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}ulong TtLRhOVzZGhmjsIHNwfcAWJGWs = 75054418908540012; 
double EoNSBWzzyK = -1.624606E-11; 
while(EoNSBWzzyK == 0.002100145){
EoNSBWzzyK = Math.Ceiling(Math.Tanh(0.1));
Console.ReadLine();
}ulong PEdKymhwiRb = 59496831777456499; 
byte eDLpiumNisRGIhaXli =  230; 
long qeIW = 87355593901977592; 
ulong asunTUIZAjgMlMgxubECOy = 2219913492285421; 
ushort VyZ = 39495; 
float cNwqGzQCoJeObwblWEnatZcgbDP = -3.019877E+25F; 
ulong CFAZQgySGEtEiykVYZKBxqR = 54144287737358095; 
long WDiCxleALJxZwUJQI = 28746381870717352; 
double SgPWT = 3.488835E-35; 
if(SgPWT == 3.897424E-35){
double bLIuCHRGAuSEDnHquDA = Math.Log(1000, 10);
SgPWT = bLIuCHRGAuSEDnHquDA;
try{

}catch(Exception ex){

}
}byte FcjfzjM =  58; 

}public void QIEmbfuUoBfhMFPILU(){
ulong XnmEbKgIHHu = 14856530106110350; 
ulong RmhhbSywyNoXVHDuZlqZ = 25110819976505945; 
long TNcKgaex = 85823602920338764; 
float JNyGRNVEFcBHEdOmUDUlACoElz = -0.09583016F; 
string afEwLVNnWpOEwcVtnGzWZDI = "sgyUusxwARbwditTBGH"; 
int WflJSjLahPzyXcE = 860804; 
if(WflJSjLahPzyXcE == 875193){
WflJSjLahPzyXcE = 360249;
}string bBpfmGwtVdhxalkChM = "qcEXffPwIMcKe"; 
float kiDdzuNwDuQHDlItx = -1.189084E+35F; 
ulong zDkayjNHTucFAacYd = 81125502108926840; 
byte atGDheYN =  155; 
short ajZO = 12110; 
double bdkFutTXjVyZVClaMnmRVBC = -6.034727E-05; 
while(bdkFutTXjVyZVClaMnmRVBC == -1.726557E-18){
bdkFutTXjVyZVClaMnmRVBC = Math.Exp(2);
object jnBxiSCCYgNsGhGbgwuedbpo;
jnBxiSCCYgNsGhGbgwuedbpo = 3.868531E-06;
}uint GptUwJVHaolUdbob = 21; 
string KiXkxagWeaqEUfD = "JmaLjyabcfd"; 
long wPGhaPxthyu = 45501274854219078; 
uint tVksnmiWiqWyGJkLwTpfXBPGikVZc = 1040; 
sbyte scTuPND =  -62; 
ushort gzdtTPmmgIubSgGidsiTPlQnmHqpG = 8439; 
ulong BCIgeRckQgYzPpjNPHKkeDjVI = 5890350816529997; 
uint qzzdloYdImszWxFCZ = 567479; 
ulong qMAoLQNTAZEmPSnqEyVfACJoqjQT = 1303159934802549; 
double CAlkimPlhEqjxZJIzPdjYCa = 0.001465438; 
CAlkimPlhEqjxZJIzPdjYCa = Math.Exp(2);
int? PZAsMpRCMVLPRHdmOjxIJTAP = 5398696;
PZAsMpRCMVLPRHdmOjxIJTAP += 5533;int tgBNtFAwsUObfWBKbAKXWtOcXnIgp = 6562; 
while(tgBNtFAwsUObfWBKbAKXWtOcXnIgp == 6562){
tgBNtFAwsUObfWBKbAKXWtOcXnIgp += 514373;
}short ETcujbmVQz = -28593; 
ushort ActIcZtzP = 7240; 
double eeluZwwhwyGH = 6.969803E+16; 
while(eeluZwwhwyGH == 5.214504E-30){
eeluZwwhwyGH = Math.Ceiling(Math.Atan(-5));
 bool? qHSqIps = new bool?();
qHSqIps = true;
}sbyte qpUgAgyOsf =  21; 
ushort moOgku = 39439; 
byte LksLqFwnNIzfmzZag =  136; 
ushort iBRfBdyincpDdOFJPOPTtes = 61875; 
float ZRO = -1.477099E-31F; 
float ztjtmjng = -8.613929E+37F; 
long VcATplsNCue = 33171939202995591; 
double sFuupCFCMfeIGZX = 3.218262E+33; 
while(sFuupCFCMfeIGZX == 1.445923E+19){
sFuupCFCMfeIGZX = Math.Pow(double.NegativeInfinity, 2);

}short kQhISWGEBVXZfjjUU = 4905; 

}public void NGB(){
ulong hiAc = 23624709613722357; 
ulong tqGqnBDxtmafZqQDXKbeB = 79240874530247590; 
short BhlkJgomoBLQmbezyT = -17263; 
float JHiKUcYltolcIBXOfgxJyNFPTkC = -15569.87F; 
long XyJSCcCPdpbWk = 46172014168777509; 
ulong zWbuQKCL = 26545670446684168; 
long qssxQupgGZgSuIhmkV = 4396547443558414; 
short gGTLZMuz = 23150; 
string IdKdC = "RnngsONXdUlPMdDegZ"; 
string wTImKu = "QRGacbb"; 
uint bdPRgNVSiJfYDDWKdwiXHbyXGWfzT = 75; 
long DddkiaJXPdaoknoJq = 86973802638321599; 
byte IyZaVwwZ =  121; 
float VuAZRXxJwIRPDjgS = 1.55133E-24F; 
string tWQbAQdfTPnQPRd = "tFCbihRCQ"; 
long QODyQuwfYFzBbUq = 21939867605859591; 
uint JcnlenkRsnqqs = 416717; 
ushort eIUdAgStkhQJMJSddhsgUU = 19930; 
byte JHZznWFHZbnFpiVxTUgwi =  157; 
int mopdmRksGwQBSUW = 2106; 
while(mopdmRksGwQBSUW == 2106){
mopdmRksGwQBSUW = mopdmRksGwQBSUW + 594870;
}uint YxLbZiTBdcBkZGCuhRVZVsw = 966875; 
double jgTWUPCqHDQkiNVNyqKXGKIGQJA = 3.019687E+09; 
if(jgTWUPCqHDQkiNVNyqKXGKIGQJA == -1.486247E-34){
jgTWUPCqHDQkiNVNyqKXGKIGQJA = Math.Ceiling(Math.Tanh(0.1));
try{
Console.WriteLine(jgTWUPCqHDQkiNVNyqKXGKIGQJA.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}long zqPQnkWmSb = 3535720786488259; 
double fPEowFyRizcGDa = 3.680289E+34; 
fPEowFyRizcGDa = Math.Pow(2, 2.1);
Console.ReadLine();ushort amkFYxtHnhfLVLDeukQCCl = 2662; 
double QmZKVXeBhOFwOWRzQZgNg = -13.47083; 
QmZKVXeBhOFwOWRzQZgNg = Math.Sqrt(4);
Console.WriteLine(QmZKVXeBhOFwOWRzQZgNg.ToString());int NCgKiTyCphtIyfykldF = 92486269; 
if(NCgKiTyCphtIyfykldF == 766162){
NCgKiTyCphtIyfykldF = 525868;
}short SGZwFYgDXanOMCzdYAfjZwxLJsPJh = 29875; 
ushort SUFVpMO = 44741; 
short AjXpTlyBGnwgZyjoN = -16465; 
sbyte uaHfVRWDNlMxTxV =  -69; 
ulong zDkIsfNLVhMb = 17679111633111336; 
ushort oUNt = 35251; 
double ECXCuOHmN = 79.29803; 
while(ECXCuOHmN == -4.011692E+32){
double kWbnxJUXxmp = Math.IEEERemainder(3, 4);
ECXCuOHmN = kWbnxJUXxmp;
for( ; ;) {
Console.WriteLine(2.117203E+36);
}
}long dxSFIwMqNmTSlMl = 21290317854596355; 

}public void hdWABqaQzTZQyBHlLZ(){
long YRMSADJEsAcEOoxEzmQwl = 72080908172370000; 
long wSXFqqdZYJxyZRw = 20595983929278926; 
uint zsfeXJMLcHMQKfmwnRMQGuDptyV = 88146199; 
double GAKSZXVba = 9.170945E+17; 
double sjsBIoePQLANXRcufqZmmytQM = -0.008615722;
sjsBIoePQLANXRcufqZmmytQM = Math.Sqrt(3);
GAKSZXVba = sjsBIoePQLANXRcufqZmmytQM;
Console.ReadLine();long VRDOCwIyjDCQYpmBZz = 31583810804167201; 
double DbmzkwHYqqpagNYIa = -6.028749E+20; 
while(DbmzkwHYqqpagNYIa == 1.421962E-17){
DbmzkwHYqqpagNYIa = Math.Truncate(DbmzkwHYqqpagNYIa);
for( ; ;) {
Console.WriteLine(-2.401889E+08);
}
}double qUimhanGdi = 2.663056E+14; 
while(qUimhanGdi == -1.070432E+10){
qUimhanGdi = Math.Pow(double.MinValue, double.MaxValue);
Console.ReadLine();
}int hWlFWUSdUBAylws = 21749211; 
if(hWlFWUSdUBAylws == 263931){
hWlFWUSdUBAylws += 190599;
}byte VAabjtJnUoP =  245; 
float GGuzVtLRf = -8.791118E-15F; 
int cyxpmVGpeWSJKlmGixxxHwjPP = 8841; 
if(cyxpmVGpeWSJKlmGixxxHwjPP == 370097){
cyxpmVGpeWSJKlmGixxxHwjPP += 521454;
}long DHRzYCdoVBKCQlhzlX = 74509311140919210; 
double alM = -5.073429E-27; 
if(alM == -1.954099E+08){
double thLVAnZcMgTxzPusZERyDcnKPot = -6.325934E-29;
alM = Math.Round(thLVAnZcMgTxzPusZERyDcnKPot);
object eNhCbA;
eNhCbA = 2.020025E-32;
}byte PJpnt =  221; 
byte yWHOuqAsZapWjOaPjiyGEzlX =  20; 
ulong HGesVscTQMEAKHSVoJ = 66222618875272492; 
long ZKJKEOTNV = 44805735585716912; 
long YMDXoQWCXeahBKRTgdTVVNf = 26518017758639112; 
ushort IKBxMtUImVIUBUTp = 1875; 
string OPSkGDqbejxzGEITapywKZnJSygfu = "xexzKMPzUUusPyRybpTbTaFIo"; 
ulong jtiwlHmNqbhUVdDwE = 35846946866152246; 
byte YxBPjVkGeMzh =  182; 
uint AxUwYXpFbIXELqZOFanoq = 236582574; 
double LzyKeSHJftYBwnxIRAhmsAnfYThym = -5.648284E+11; 
if(LzyKeSHJftYBwnxIRAhmsAnfYThym == 6.67268E-06){
LzyKeSHJftYBwnxIRAhmsAnfYThym = Math.Exp(2);
int[] XPWDs = { 8965367 , 82834 } ;
Random zTodMJwGGTpflPbnBiLegyUnmJkM= new Random();
Console.WriteLine(XPWDs[zTodMJwGGTpflPbnBiLegyUnmJkM.Next(0,2)]);
}int MOisVnaDoERSuJsiZJoNSmQa = 26018845; 
if(MOisVnaDoERSuJsiZJoNSmQa == 461617){
MOisVnaDoERSuJsiZJoNSmQa += 664345;
}long pCtyHKtliiPDHbUFpcNkCQXz = 37529815473663880; 
float DSp = 1686.179F; 
short dFKznuYglNx = 26739; 
uint AFUAAqzOl = 16; 
long zbouAzVzlmaKOuNljJM = 37551134387381592; 
float tGpGPAfWsWy = 6.879478E+21F; 
byte TPLXMCbwxoQx =  194; 
float xAdhTILeYVHpRNnPZPWDHsbfPqjTp = 1.829148E-34F; 
short uAXC = 9337; 
ushort hLNceljJhBRIRqqezXtCy = 55507; 

}
} public class ZSolymqmsumuVFZaDhCLPhwB{public void cuAEnTmpWNjQZNwRmAmFWx(){
uint XQuVgZSxOAuTSJdC = 88640203; 
double PkEufUCIhWQHUZHzzqiXUObAt = 1.877746E+36; 
while(PkEufUCIhWQHUZHzzqiXUObAt == -3.561646E+17){
PkEufUCIhWQHUZHzzqiXUObAt = Math.Ceiling(Math.Cosh(5));

}int CRKXWw = 68595622; 
if(CRKXWw == 967674){
CRKXWw += 225630;
}ushort YSBLDMDitEfaCmCZ = 17735; 
ushort Nxw = 47743; 
ushort PDGp = 4476; 
uint DbMIJedSEsy = 81; 
double yiFUDlGcEggINiXTpdQyBCTqis = -1.419583E-25; 
if(yiFUDlGcEggINiXTpdQyBCTqis == 2.461281E+08){
yiFUDlGcEggINiXTpdQyBCTqis = Math.Truncate(yiFUDlGcEggINiXTpdQyBCTqis);
object pJHAhdaztVonLkdx;
pJHAhdaztVonLkdx = -43.52499;
}string aDaYWzFfsaXpXRCW = "ZiEUuPYaSnTtLJs"; 
long CQs = 28059767255122543; 
byte MJkYzEyHHObXdkMMIsFGXp =  182; 
int jHKxhZuOhDjhMjPmtVcpS = 35245518; 
while(jHKxhZuOhDjhMjPmtVcpS == 35245518){
jHKxhZuOhDjhMjPmtVcpS = jHKxhZuOhDjhMjPmtVcpS + 437678;
}double NwXHooXNjzOIjWICFViQ = -1268701; 
while(NwXHooXNjzOIjWICFViQ == 2.226531E-18){
double MEEuguhmCxVRAjxoJJ = 0.001213365;
NwXHooXNjzOIjWICFViQ = Math.Floor(MEEuguhmCxVRAjxoJJ);
for( ; ;) {
Console.WriteLine(1.061445E+16);
}
}int JDVGUo = 8347; 
while(JDVGUo == 8347){
JDVGUo += 182509;
}double hnMCe = -1.102022E+08; 
if(hnMCe == -2.641901E-13){
hnMCe = Math.Pow(double.MinValue, double.MaxValue);

}int bXTBisuVRZkJTuTeGHCQMONpqJcR = 306892103; 
while(bXTBisuVRZkJTuTeGHCQMONpqJcR == 306892103){
bXTBisuVRZkJTuTeGHCQMONpqJcR = 220986;
}ulong QWgThBBEqbuHCsyuVPk = 79214567355963850; 
uint PNIFmoKBIJlmjZonxpDSV = 142564; 
string xmMGEFAZTCoNbncNU = "fYaaEUlTm"; 
uint PnFHdQcYUdeNnpyaBwbAf = 25; 
string BxKR = "LsaqenKgXzaPZ"; 
sbyte lNK =  -102; 
float zkZEObEFEutgBtMNWljquy = 0.03150374F; 
short daQ = 16820; 
long XJmgaNqmfCteHIzQoA = 80854341497884855; 
byte XDOQJPiSfFMjVXkSdASJDm =  183; 
short mJwslACBFoDOEwSzHKKNHEXqzWiI = 4968; 
sbyte RxTwfOBTfkJpJIeBdajWD =  -121; 
int KLdTpqUWxGL = 6957; 
while(KLdTpqUWxGL == 6957){
KLdTpqUWxGL = KLdTpqUWxGL + 110791;
}float DYmoJtxeHulgQSDLuK = 281137.2F; 
ulong MUdWNQcnzRM = 40897457147362894; 
byte DjzVAhoqb =  121; 
byte llslMHGctVwutSknYogKDeiL =  150; 
string JNWxsAHPfe = "JTnjTHOd"; 
uint WbsNsDmhcIHLtWoxAdVM = 9; 

}public void PGnxtkD(){
double AHMMEKaXuHdxDXBq = 1.206982E-05; 
if(AHMMEKaXuHdxDXBq == 9.010116E-26){
AHMMEKaXuHdxDXBq = Math.Exp(2);
Console.WriteLine(AHMMEKaXuHdxDXBq.ToString());
}byte hSLybQ =  240; 
string gkcDgjTcOIdGPdCykIhcQHKeCdHy = "eCCpdoXdYIuRn"; 
double jCtPjaRi = 1.187853E+34; 
if(jCtPjaRi == 0.4013896){
jCtPjaRi = Math.Pow(2, 2.1);
try{
int tmtNtXPxfXF = 7988711;
if(tmtNtXPxfXF == 92447){
tmtNtXPxfXF++;
}else{
tmtNtXPxfXF--;
}
}catch(Exception ex){

}
}ulong hmouXRXyunOcQJMJjMIsYudWlempR = 37266521081131415; 
ulong UItJ = 11042867709006957; 
string waNjsiSEHG = "xziO"; 
byte uwJNLGMpKViN =  189; 
sbyte ObzaLjWGkpaOHsOhhojtiwC =  51; 
ushort TIm = 44186; 
float BnIxKOBiyPyxLSfXWObyDMGtBOG = 4.898402E-15F; 
short DAfmsKsytPbV = -7961; 
double aEWVM = -9.734334E+08; 
while(aEWVM == -7.41119E+20){
aEWVM = Math.Truncate(aEWVM);
int? xBkXiNXcgwYiIii = 5905050;
xBkXiNXcgwYiIii += 74379;
}ushort WuXBlDVseTLhucoGSeRasC = 507; 
uint iBlRcALlSdySGSolzsRIVUpZNlB = 375550358; 
short uAGdcKBcuaVCY = 21947; 
ulong mCOTiiQYn = 21580739923329433; 
long sZOmD = 21745659491240894; 
double nuF = -3.403822E-22; 
if(nuF == 1.282857E+10){
nuF = Math.Ceiling(Math.Cosh(5));
try{
int eeQMumaxTJsYufBdcxXqcuFOPe = 8696534;
if(eeQMumaxTJsYufBdcxXqcuFOPe == 88613){
eeQMumaxTJsYufBdcxXqcuFOPe++;
}else{
eeQMumaxTJsYufBdcxXqcuFOPe--;
Console.Write(eeQMumaxTJsYufBdcxXqcuFOPe.ToString());
}
}catch(Exception ex){

}
}long BVgwPKMbEPLAcWpUxcwhKdtNiH = 53342255502222358; 
short lhFOPHqk = 9303; 
float NBVFQAEjxDtlh = -1.67933E-29F; 
float HjGiIfJoyH = -1.988413E-34F; 
ulong fqmGekK = 37877521057344215; 
ushort IMbFyllewmptTjf = 45497; 
ushort VVMbJFnjpz = 53602; 
uint sbiMqLDJfbCKYtZhfFhmFbCM = 6028792; 
ushort nVOnWyYqKzPZ = 55152; 
int UjsLlnbkTJklqAyfnnkwjnmN = 407652; 
if(UjsLlnbkTJklqAyfnnkwjnmN == 628314){
UjsLlnbkTJklqAyfnnkwjnmN = 798346;
}sbyte ZlICmskdlLUFLl =  33; 
float tBLHjGHXG = -3.017952E+24F; 
int EdBQleSWSVdhiztIbo = 81162070; 
while(EdBQleSWSVdhiztIbo == 81162070){
EdBQleSWSVdhiztIbo += 450880;
}ulong HaFIiuyJgQeVZCZpBFIzagmo = 57275793688932396; 
string zRskNJLAsEEfpl = "VNKQpPAOodnsIsQabUEciYpRY"; 
ulong PJMzFQZwEfmLHZdc = 77400406148130111; 

}public void MiIXxSSDo(){
long EGKCJ = 71465669452137760; 
double sHLtmfi = -3.309857E+26; 
while(sHLtmfi == -0.005460682){
sHLtmfi = Math.Ceiling(Math.Cos(2));
object PfEamA;
PfEamA = -3.59587E+12;
Console.WriteLine(PfEamA.ToString().ToLower());
}short GQg = -2957; 
long QqgXyujcCOyUOjqxQZ = 37246962586705455; 
ushort jox = 36642; 
int uBPBCZjPtEZkShKjaguBHVntNwZ = 85; 
while(uBPBCZjPtEZkShKjaguBHVntNwZ == 85){
uBPBCZjPtEZkShKjaguBHVntNwZ += 290129;
}float RHozdXhDzOainqaopieoAlwO = -1.448621E-37F; 
string gmeVasTDLJpDztQGNlLRSWaMjDp = "lCscWlwlJeiVzzFITDR"; 
sbyte QbTojdkltLybnp =  -82; 
short GlYFWIdQt = -6799; 
short SHMjjxBzNNijZ = -13086; 
ushort TjhwqHXFhRTtQAQJSSCdMS = 5609; 
float RATdOFHnsXjmWclKgcsDg = 2.293284E-18F; 
double sgOoZbfmMAgdCulm = -0.003198551; 
while(sgOoZbfmMAgdCulm == 2.447963E-07){
double GQMsCilWExzSZs = 2.030853E+24;
sgOoZbfmMAgdCulm = Math.Floor(GQMsCilWExzSZs);
int[] DeHFIgpckM = { 1445009 , 96591 } ;
Random expAVultVjqV= new Random();
Console.WriteLine(DeHFIgpckM[expAVultVjqV.Next(0,2)]);
}double uffGDxLnjNahGcAYAHAahtd = 2.079036E+15; 
uffGDxLnjNahGcAYAHAahtd = Math.Ceiling(Math.Asin(0.2));
for( ; ;) {
Console.WriteLine(-1.169497E+34);
}sbyte NUUWjVqbZoTZFBMhzcIIJA =  113; 
sbyte oVftilqmNBmJXiYAng =  72; 
string bxAiwLpAcmWNtVakIQc = "NRaAABTKiuthfYmzG"; 
byte BaOLmpVFnXnGpMzSjwaKURH =  81; 
long OHuICGzcCDZgOjEWGMUPcxHfdd = 38778158341941398; 
uint jHxJhnkWTd = 81696425; 
sbyte lVxTRsaoFzZYiSiDdwIoNZokHPlLm =  124; 
ushort tVyzakCinVbYKifsNqalhRlm = 40432; 
byte bgocFcjGm =  101; 
long STAbSXpByKTXVZiIC = 87090800795166751; 
float WBESXcPhtJQtoo = 2.46806E+20F; 
short qgGTDdDjSO = 25158; 
ushort bYCuaURWVgRQYMp = 60617; 
int khOI = 81833317; 
while(khOI == 81833317){
khOI = khOI + 298900;
}ulong bnZlOuOV = 14555829860618840; 
int mnoKJlJumLqc = 5348; 
while(mnoKJlJumLqc == 5348){
mnoKJlJumLqc = mnoKJlJumLqc + 566623;
}float XViItqHylNbZuhcAVqFNDuh = -231014F; 
long YiMUklRuqfIqsFli = 58650868874745132; 
ulong ekENfdMRInATQOEOMunZlhqlJulYn = 61427336889569907; 
int EFC = 967330291; 
if(EFC == 850308){
EFC = 560697;
}
}public void zNnCP(){
ulong UqeLSODhNCOFHXWiuKwLlsDbDJT = 27534681560287461; 
uint oMRYPlgTnWiCETR = 442; 
long PdiiUNuAfzewkxKfY = 22085355081844211; 
long jtFVyBkLwf = 43965751205143746; 
uint eUOTRpnutUVxkzkfCEIUZMnZxHBcw = 7810; 
sbyte MioGMegmQ =  -41; 
uint fMNiGARC = 97198942; 
ushort OxjPkZYfQjzoWBPzBIi = 24711; 
ulong pUOUPUXEHOQKRoUqcRtwwVn = 38373821067019312; 
double agpbpjehmELAlJBFiFsastuHq = 6.723158E+14; 
if(agpbpjehmELAlJBFiFsastuHq == 196.6399){
double RiFpMHomisDXaxjOStxZAINJK = 2.071681E+35;
agpbpjehmELAlJBFiFsastuHq = RiFpMHomisDXaxjOStxZAINJK / 3;
try{
int LWNKuSsZSTMMfkcduIn = 3443695;
if(LWNKuSsZSTMMfkcduIn == 19133){
LWNKuSsZSTMMfkcduIn++;
}else{
LWNKuSsZSTMMfkcduIn--;
Console.Write(LWNKuSsZSTMMfkcduIn.ToString());
}
}catch(Exception ex){

}
}ushort HdgwKywQnGFSOZBLmuxKGpalTMa = 41396; 
byte cIWdlPFqwNHmfbKicLPQEXNMpp =  13; 
double XyPGwDWVKNLgWYKds = -6.637803E+36; 
while(XyPGwDWVKNLgWYKds == 1.128634E+14){
XyPGwDWVKNLgWYKds = Math.Ceiling(Math.Cos(2));
object sXdKHZcIYgd;
sXdKHZcIYgd = 0.05453417;
Console.WriteLine(sXdKHZcIYgd.ToString().ToLower());
}byte zAOyqtOKJgSDPPJoXkiUx =  157; 
string CwfemzzJxnJEGuWNiBxqjh = "dphkok"; 
short SzdVVnJykI = -21102; 
sbyte geKDoxMWjLCtncPfm =  -81; 
ushort aQfDz = 583; 
uint dkKabHWkzJaPGqJO = 535277; 
long TsXyMbBpY = 34098083161147114; 
uint IfEHFiJAJhsa = 359933205; 
byte LnloafHlqFMSuzKNbja =  32; 
short tndDRiHggVJFhLOkqCzqpa = -3539; 
ulong kyGZN = 12419428414785393; 
byte GjyQiPpZYQSzIYw =  93; 
float yHLbhuDDn = -8.456485E+23F; 
float TnaMfDHHpVzHiUUAsGCgYaINEccnH = 4.035072E-28F; 
sbyte QYjHmoQ =  -77; 
int MlwYAJ = 92; 
if(MlwYAJ == 688514){
MlwYAJ += 23896;
}float nhdAtZPNoztWLSepLWsT = 6.722217F; 
sbyte EFEFG =  98; 
uint fEtfkpRHsYWWGEXkSlEOJZLN = 994011727; 
double xkgwaEQhAZeueLCXEmzZItM = -3.554074E-15; 
xkgwaEQhAZeueLCXEmzZItM = Math.Pow(5, 2);
Console.Write(xkgwaEQhAZeueLCXEmzZItM.ToString());float abBtecsxlRHUlhbhIewwKxXgJe = -2139573F; 
int CSZn = 64; 
while(CSZn == 64){
CSZn += 145423;
}
}public void eLpxECXnpckz(){
ushort EdtLaGN = 56720; 
byte PVYZuLEwncG =  187; 
long eEApmapuDdEEduICGlBaIJ = 42678564784087894; 
int xJGyfyGiZwtUwQHjNMyukIdwb = 422987; 
if(xJGyfyGiZwtUwQHjNMyukIdwb == 517976){
xJGyfyGiZwtUwQHjNMyukIdwb = 196756;
}sbyte MguOkxiGsalpICdY =  -52; 
string XjHPGjxEbYIzkku = "JlwDpoOTHRftasJlANkpf"; 
short EOShIKDbemSuNUAhIxNhOecE = 11287; 
short ZzaeFqxSOyhfje = 19663; 
string iXWsZF = "UsjbAMozsnDEeimdp"; 
string KBpEdYeWuLteZTzklhwsci = "OWClVYVUGooVbEPfcwQUN"; 
double EKHYaVU = 8.226627E+11; 
while(EKHYaVU == 695.1198){
double YzRNPLYxH = Math.Log(1000, 10);
EKHYaVU = YzRNPLYxH;
try{

}catch(Exception ex){

}
}byte YESbidElFpybLuSaFtfQeIqeiT =  53; 
sbyte omagsUSdik =  73; 
sbyte NnxBabuVgCzuPplOSpSxXocyVtBaX =  -21; 
sbyte IgVdIJTjNth =  58; 
int EEGopVWMaylZGIBtYLthNJ = 1658; 
while(EEGopVWMaylZGIBtYLthNJ == 1658){
EEGopVWMaylZGIBtYLthNJ += 672532;
}long gpUjPWwJfluWfzSIdk = 74902469821048596; 
byte hgzmcwpVewSEYp =  156; 
sbyte RDVpAglZpjKYAkUNYBYOgUL =  -86; 
double IsfweRkIhsngJamnMl = 4.5034E+33; 
while(IsfweRkIhsngJamnMl == -9.34839E+34){
double tHXSNtWzTUUeTNScDASS = 7.451072E-39;
IsfweRkIhsngJamnMl = tHXSNtWzTUUeTNScDASS / 3;
try{
int zEZYxuow = 1460021;
if(zEZYxuow == 73571){
zEZYxuow++;
}else{
zEZYxuow--;
}
}catch(Exception ex){

}
}byte iwqhXJSNQtjuXczGAOKWRRc =  245; 
sbyte yhApq =  82; 
ulong mZXmtCYn = 68028293121741551; 
short cnWsttdSkLmB = -29326; 
short NPAImwhXExVHMpYmMX = 29934; 
ulong oVGFjBpnaVdYObKuf = 23445390654545841; 
short mzCDeFdyUUccVRKDB = 21942; 
long nnSGSaTjWZhSSZdQzsZuLEQBcccc = 16580630857463984; 
int LeLTCUMkkWOUdhSnmKzsDPYQDfE = 757331; 
while(LeLTCUMkkWOUdhSnmKzsDPYQDfE == 757331){
LeLTCUMkkWOUdhSnmKzsDPYQDfE = LeLTCUMkkWOUdhSnmKzsDPYQDfE + 193170;
}int UJScVOSSfoOMmORgDBns = 120071943; 
if(UJScVOSSfoOMmORgDBns == 415808){
UJScVOSSfoOMmORgDBns += 318440;
}byte MjJKBAXFuIDELXS =  135; 
ulong pNWsiGWkNOPDH = 69430898067437522; 
ulong lMVXLIXntBbayCtbcOoNjQSb = 33001930839240948; 
int LGmIlEPBwcGFfTNUEpVaHwUUZKpg = 4610; 
if(LGmIlEPBwcGFfTNUEpVaHwUUZKpg == 444711){
LGmIlEPBwcGFfTNUEpVaHwUUZKpg = LGmIlEPBwcGFfTNUEpVaHwUUZKpg + 516512;
}ulong OwCMYKuPwtUydcqwmDpoLtJ = 60403887146211785; 

}
} public class cafiOjcXbezHQTcogt{public void UkCbwMaAoBxCJkpwdWi(){
long BtnG = 73460632467462337; 
sbyte uCYLxNLNx =  20; 
int wAyYsXLDuRmwBKoeJIGHCXVT = 23; 
if(wAyYsXLDuRmwBKoeJIGHCXVT == 356866){
wAyYsXLDuRmwBKoeJIGHCXVT = wAyYsXLDuRmwBKoeJIGHCXVT + 750965;
}double cfHlWIBTjFTDoDjMzGeOY = 7.425793E-19; 
while(cfHlWIBTjFTDoDjMzGeOY == 2790407){
double jIUEswddiagMyXesFDbaxNJiL = 1.836147E-29;
cfHlWIBTjFTDoDjMzGeOY = Math.Round(jIUEswddiagMyXesFDbaxNJiL);
Console.Write(cfHlWIBTjFTDoDjMzGeOY.ToString());
}ushort kNeeCTKTsmsboFCzICQwqzoxX = 63130; 
byte TYlNYHSUhuPzDhYSSuhtitEbeaANy =  165; 
sbyte HLZtaFcJNIqnNqTjAy =  19; 
int TPBujJXgfxAmkSRhGRnqAHBc = 2991; 
if(TPBujJXgfxAmkSRhGRnqAHBc == 298341){
TPBujJXgfxAmkSRhGRnqAHBc += 244875;
}string EOOV = "YRWfCHlRaCAbcUHp"; 
byte lozeUVPRTxx =  173; 
ushort VJIZq = 14801; 
double nkp = 9.429097E-19; 
if(nkp == -7.110588E-12){
double CHbUWbpKOQwqdwgt = -9.352333E+18;
nkp = Math.Ceiling(CHbUWbpKOQwqdwgt);
int[] slamO = { 57522 , 53835 } ;
Random IKFBcJs= new Random();
Console.WriteLine(slamO[IKFBcJs.Next(0,2)]);
}short NcpVnlzQYKSZTfNOqepH = 18157; 
sbyte uGfnOMZdTSMAEKeeaDyQxPLLw =  -50; 
float VlI = 1.98979E+11F; 
byte phYKdZHYggGidlapeqouFVwSyRX =  39; 
long TSqOPWbS = 16597127764740653; 
float VWDCsSquNflmbzqVnORpxdnm = 0.02966308F; 
int qSdpZhmLJsAAywiWGfPqAJp = 31; 
if(qSdpZhmLJsAAywiWGfPqAJp == 833169){
qSdpZhmLJsAAywiWGfPqAJp = 655842;
}short TmDcVbVnTlGmVnqTb = 26484; 
short TKnnJCcazPwdtQHmUsnU = 12004; 
float fUxuNYIqWpIKjYnutZVPeQDTfIe = 2.657286E+14F; 
long YbPiumhpbQzANLf = 21484030425340310; 
float qDoeUKHoRK = 9.588509E-09F; 
ulong hlJTZGqPiZkQsMMEjYXTdKSkhg = 79154261443053217; 
ulong kUdkXCuoUeqVDnAKyXQyt = 30085776685860748; 
sbyte GVdsoVNVCpDq =  -114; 
ulong HICOGBWspNikxQImOLMLZAVkPLb = 87472008668840539; 
byte OERKDNFgKpfTcPHqdWh =  185; 
short UAIkAHAgEpxIahUbxMqdeuWwebBsZ = 28869; 
short oPJscsnYIDNQywOA = -4940; 
string xwmUBDmgzfghHXVs = "zDZezsxCI"; 
sbyte JmbWXyWebUiPPlPTBwfTDcDaUcuY =  -112; 
ulong TyoylekjiaQq = 12967009179940901; 
sbyte mwHKacUdYblCosBOE =  68; 

}public void kXSMMkKgTB(){
short UlqjUQCdipfcOePauK = -26733; 
sbyte HVFwJUzqVbDJyspYiGYbKg =  104; 
float XHxJNixjTmpNOkWcNmoDtqNA = -3.759104E-22F; 
string daBZMG = "ncIhSgAFfZkuubYXTHoxHT"; 
double CeofSMqbiAJafexxTMI = -2.360719E+25; 
CeofSMqbiAJafexxTMI = Math.Ceiling(Math.Tan(1));
Console.ReadKey();sbyte OpBnc =  104; 
double feIzTEnOauifkRCsXgb = -3.622189E-31; 
if(feIzTEnOauifkRCsXgb == -1.33376E-21){
feIzTEnOauifkRCsXgb = Math.Ceiling(Math.Cosh(5));
object pWTUCbKw;
pWTUCbKw = 9.442104E+32;
Console.WriteLine(pWTUCbKw.ToString().ToLower());
}short fiooJmnSSazfKmdB = 1085; 
short RuDfClIG = 30120; 
int jzDJECtBXwPoHDwCBnKayqk = 127478; 
if(jzDJECtBXwPoHDwCBnKayqk == 760059){
jzDJECtBXwPoHDwCBnKayqk += 69490;
}string pnfRZEaJDgasPceWDfLh = "nEoSncDaAGx"; 
double AbBMwXAGgTyZwycIAItUpbK = 4.166712E+35; 
AbBMwXAGgTyZwycIAItUpbK = Math.Ceiling(Math.Sin(2));
ulong tZpz = 17265442290827964; 
int NJPCdIYOsjPO = 5329; 
if(NJPCdIYOsjPO == 300267){
NJPCdIYOsjPO = NJPCdIYOsjPO + 621622;
}ulong GYPwkziPEcDLDqYtlPDheGHfJ = 29480262541098979; 
ulong OUupGzjjpHekXEmLBdIMaqJ = 34468778397867007; 
long FjASzXJUwqzFMmbwsIfwaNyk = 82728867787009914; 
ushort tuOTcgZRFZkhCa = 35914; 
long lmlPSN = 18556061019609352; 
byte pTHIAGY =  108; 
byte yaBDqst =  138; 
sbyte kGC =  35; 
uint HCkaSdZZkhVdIHoQGJom = 86427554; 
short yjMjXKsZhRYyki = -12523; 
ushort Bqi = 29050; 
short DMuaSsmYOnPDJqPbBPVVQIRFxtW = -17629; 
sbyte eYhojzEKAIUSkjxHH =  -120; 
byte RtCudVUPEAwaPli =  207; 
ushort hqzQpmbXyIiQM = 58408; 
string LLyNkoFOfalTdYaGlmUUZ = "dnt"; 
sbyte lHONCaeCytIiTNL =  50; 
short hUA = 9140; 
int zafKLIiueRshsmLSYx = 240378; 
if(zafKLIiueRshsmLSYx == 18392){
zafKLIiueRshsmLSYx += 152753;
}short OnklYFdioBV = -6687; 
float kmebuEscpoWcdqauZXwuTVjT = 1.349831E+19F; 

}public void iYjXE(){
int bpZZNMXMXVoVCNhUCXZxWpd = 1651; 
if(bpZZNMXMXVoVCNhUCXZxWpd == 168363){
bpZZNMXMXVoVCNhUCXZxWpd = 78377;
}sbyte typoLwakKxhX =  -58; 
int lsbeJaEiQMlORfmPISEVzkyyzCNp = 791592508; 
while(lsbeJaEiQMlORfmPISEVzkyyzCNp == 791592508){
lsbeJaEiQMlORfmPISEVzkyyzCNp = lsbeJaEiQMlORfmPISEVzkyyzCNp + 214955;
}ushort hZyqPo = 32833; 
int HifxIBfLJAbUEGg = 2753; 
while(HifxIBfLJAbUEGg == 2753){
HifxIBfLJAbUEGg += 442448;
}sbyte KfIIumgiLVZSsITsome =  120; 
ulong mEtpcBfzibAEw = 634858729254899; 
double OPHwRMgsiToTdnejGd = -1.262457E+18; 
double CHyxdnZGCxfoMbJq = -285.8599;
OPHwRMgsiToTdnejGd = Math.Ceiling(CHyxdnZGCxfoMbJq);
Console.ReadLine();uint lQy = 966592; 
int VSSUcBuAG = 349525; 
while(VSSUcBuAG == 349525){
VSSUcBuAG = VSSUcBuAG + 805546;
}string KqjomFMBdNoXcn = "uSikDZSTgKs"; 
uint NEK = 750400; 
string HxXJnguZNfCEmbbehFBxRzc = "UfLNAcTjaCKXUBXwFSf"; 
string PlYLADR = "GWhtsOiwH"; 
uint lsEMsejIIfRYscyfUNbaB = 95; 
ushort CFDMOaYswPQkAHHoUig = 51478; 
double qVJoJFAjjtigQAIWTfTGWmHU = -2.84094E-30; 
while(qVJoJFAjjtigQAIWTfTGWmHU == 9.602292E+08){
qVJoJFAjjtigQAIWTfTGWmHU = Math.Ceiling(Math.Atan(-5));
try{
Console.WriteLine(qVJoJFAjjtigQAIWTfTGWmHU.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}int idJnUATLUHeVmxEiudXTlVKV = 73776718; 
if(idJnUATLUHeVmxEiudXTlVKV == 243234){
idJnUATLUHeVmxEiudXTlVKV += 54500;
}long PZqKbwqshGtyEIDnMKP = 22212221605642369; 
sbyte LzSYzgneUgZNFMHDlsf =  -80; 
int EnsodxdonViNyh = 486980; 
while(EnsodxdonViNyh == 486980){
EnsodxdonViNyh = 162432;
}float PDMZWLiwuXZnVEGjc = 9.93238E-37F; 
int dERnAaNnFVyFbzQEdBtgTCkOjl = 872637070; 
if(dERnAaNnFVyFbzQEdBtgTCkOjl == 449373){
dERnAaNnFVyFbzQEdBtgTCkOjl = 131846;
}byte MYVLxKxgPcCJUfQGYpJyZ =  140; 
short emdT = -8837; 
sbyte qqqNRJteJLMMilB =  -39; 
float oAgz = 1.360974E+15F; 
sbyte SHjHDOJFksDsYXGlZNm =  80; 
byte EUkPeIqSCHElRNUchYdEbiQAf =  82; 
float zcysxacowNKansRVONgTqWFnNGQ = -1.744873E-32F; 
long NOwuzjlPQKGIwncpJsUMt = 64566210517535800; 
byte jKdWmSYsLDXqiMojaKog =  17; 
byte qENSSaaGYFdwYqsQzKZjiwSRRmniW =  132; 
string aKMtAZgqLWYNfqpSIU = "xdaRWLWbFNSKyBAuoGZpzEIAm"; 
ushort LBWwB = 2556; 

}public void IAZTQIEYCFSmCmHWbmJEs(){
float CFAGkBJqqIOSaw = -1.213579E+22F; 
short BcdHaVTeEUiSzqPdzoqbsyS = -15376; 
uint OuHCGyqRSQsUFFNgWO = 48; 
byte dWWgzpucjPfwhA =  120; 
string MBHAZMELMo = "BefbuqpK"; 
int YbxQGDPixmIUAxY = 950509; 
while(YbxQGDPixmIUAxY == 950509){
YbxQGDPixmIUAxY += 437680;
}ulong jCf = 37348326034611843; 
ulong jJuiPCobEHGJcRtl = 15236514383658538; 
float hHeUxJJAtnKcEVyYHVBabj = -3.22776E+32F; 
short OCzWwUodt = 11525; 
double kWaEyfDyIgPbHtZTKYjhG = -2.484042E+31; 
if(kWaEyfDyIgPbHtZTKYjhG == -0.0001204833){
double CObXLEayMquREHNxo = Math.Log(1000, 10);
kWaEyfDyIgPbHtZTKYjhG = CObXLEayMquREHNxo;
Console.ReadKey();
}float OGnwEhQUpfgnlTKDhTMeHgx = -5.414432E+17F; 
ushort uAEhhHbouMyaSxTubzM = 50886; 
float TKtCOgIjmJgDwuMYYHjEoQKTTIOMw = 4.96638E+36F; 
long xHUPHTVIkAPTpbzIcAfFmJcZ = 56170661789130891; 
uint ZOSjsOnWeqVaDHoesdUdCNE = 302913449; 
double EMxQeuDbLPThqpqFMezfBdR = -5.204644E+08; 
double ubLkJCQHaFEpAVDOjiSQRSmAUZi = -8.663612E-25;
ubLkJCQHaFEpAVDOjiSQRSmAUZi = Math.Sqrt(3);
EMxQeuDbLPThqpqFMezfBdR = ubLkJCQHaFEpAVDOjiSQRSmAUZi;
try{
Console.WriteLine(EMxQeuDbLPThqpqFMezfBdR.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}ulong ZtbzcqmMjHW = 78631035535550442; 
ushort zbyDaz = 64578; 
float HcOylXITypzdSSFWLPmaP = 1.758594E+23F; 
ushort CqEWcSnV = 34139; 
double zdgTEYoTWcHiTGpKMQUSmwmLGe = 1.083195E-07; 
while(zdgTEYoTWcHiTGpKMQUSmwmLGe == -1.562951E-26){
double HJtyQBWxhVhLpNiC = Math.IEEERemainder(3, 4);
zdgTEYoTWcHiTGpKMQUSmwmLGe = HJtyQBWxhVhLpNiC;
Console.Write(zdgTEYoTWcHiTGpKMQUSmwmLGe.ToString());
}byte XPHCKm =  153; 
float hdhiNEQodMGPgtkfdR = 8096847F; 
string TOmwnLPnSkHPAFhBpfVQqd = "pQxwxHwZmCwjtWqpeAPT"; 
ushort qTHXDBB = 44618; 
byte QhWFSpsIxltne =  101; 
ulong ZPRpCklhADPAdUzEpa = 12754653091388939; 
ushort PdPliYCL = 55534; 
float FVPfpCTEDNQXdaDFmqdiuObswO = 7.637996E-16F; 
sbyte NLwhPpkHhlnF =  98; 
ushort gLkHuntPLzQTxKDYEziafCWkltGjR = 60122; 
ulong GtocqKLetWLJYxOXGnKfLxUFPau = 6822961116421192; 
string AITOeIiRmlQFJ = "EkKjjzKnqpJxiYUaci"; 
long gHRqHPLOCOFyZHuFYN = 77766548478759234; 

}public void jWmiHUfyWusEwOW(){
ulong MkxDEpbpefTUpnlXpB = 26092537159388677; 
byte iJROANSUlVHGkTDOmDH =  33; 
sbyte emeyYOIEtfgwgwnfwDx =  -41; 
ulong UmdMuPHHRcBEF = 14237859829270803; 
uint QMhBgheLOTKtStl = 8560; 
double odVERKxtNsfdStOnZmoBzNs = 1.013193E+15; 
odVERKxtNsfdStOnZmoBzNs = Math.Ceiling(Math.Tanh(0.1));
short RWAQKxZcUckO = 4556; 
short QpBKJMpuqfefJYzOS = 13866; 
ulong keHEgMgYUnRfU = 24708977895049105; 
uint KGppexn = 529; 
float qMKWEXFqAcoYlDdxkubfUKWWYQw = -7449.822F; 
int OoYQisGVNbjcfwHdOaZcZbLbIQN = 744373784; 
if(OoYQisGVNbjcfwHdOaZcZbLbIQN == 786014){
OoYQisGVNbjcfwHdOaZcZbLbIQN = 864075;
}sbyte JmMVeGzodKHRBqNH =  -122; 
int UNFXitFIhkAwjWVXQb = 88; 
while(UNFXitFIhkAwjWVXQb == 88){
UNFXitFIhkAwjWVXQb = 634078;
}short YlH = 32486; 
float konOepECAXqdFoyUG = -5.980741E+32F; 
int JgtZXGhutbipXjUIhSAzEuAjJcc = 84559; 
if(JgtZXGhutbipXjUIhSAzEuAjJcc == 59622){
JgtZXGhutbipXjUIhSAzEuAjJcc += 308653;
}int iUOhLxhgWiOHeDlzC = 128611; 
while(iUOhLxhgWiOHeDlzC == 128611){
iUOhLxhgWiOHeDlzC += 182166;
}short DFpMOfZsIMOuLPbxnEbVwXaSiLH = -20969; 
short CLxlhSSuhd = 18074; 
short RzeQIeVadhxBFEy = 4497; 
string QdtjcXzHjETwYgf = "jteUYetmzEaMEnwR"; 
sbyte YldHxhsaUqhIjoueud =  -4; 
sbyte MKgMzWMdVibpuiUx =  14; 
double DYjbfQuLszUqsWelCXVXkGXYifsP = -0.1835618; 
DYjbfQuLszUqsWelCXVXkGXYifsP = Math.Ceiling(Math.Sin(2));
Console.WriteLine(DYjbfQuLszUqsWelCXVXkGXYifsP.ToString());uint XHVcNlBCJhLKInCFAfuNGVeY = 2605129; 
ushort WDtu = 47278; 
int LBVMYGIBYfYTcnwXulOicCWwMSxGf = 31931998; 
if(LBVMYGIBYfYTcnwXulOicCWwMSxGf == 267813){
LBVMYGIBYfYTcnwXulOicCWwMSxGf += 815108;
}string twgXugSJhqPBGYhXwX = "EBagjcuj"; 
ulong PPy = 40807725433262739; 
string EBIfBQibJqHBetnXVaWcJsD = "imWqpjoPAOx"; 
int WUtHYgzRRnJDLpIzb = 22; 
if(WUtHYgzRRnJDLpIzb == 941797){
WUtHYgzRRnJDLpIzb += 260775;
}float MzbRapWIwYfDxKkeAqYnOFUHq = -2.126553E+13F; 
byte kZHBPmJxaRXMVosCVGuZT =  208; 
int GegFuADHLY = 96; 
while(GegFuADHLY == 96){
GegFuADHLY += 442165;
}
}
} public class JABaCSpCfxSplPuuq{public void LgdbBnqbWftbSYXykqfndlRcodK(){
short jPFhhWpzHYlKBWki = -28313; 
int sOFdodRzobNxFZCNpPTLcHYFsT = 545690; 
while(sOFdodRzobNxFZCNpPTLcHYFsT == 545690){
sOFdodRzobNxFZCNpPTLcHYFsT = 220860;
}uint YLFMpepybyUDILZiUujSRMQaM = 482340; 
short JhCWOaQ = -22495; 
int PuFGCKQlTDgMAbI = 241586159; 
if(PuFGCKQlTDgMAbI == 998617){
PuFGCKQlTDgMAbI = 902145;
}int nso = 6183; 
if(nso == 175740){
nso = nso + 19337;
}short wzlgnpViGMcXJAnOOVEFEispmlq = -20335; 
short ufxEHjN = -25655; 
int eMaolwHTsf = 274787882; 
while(eMaolwHTsf == 274787882){
eMaolwHTsf = eMaolwHTsf + 240479;
}sbyte TcYMRxsXBeTeaJnpBSiddSFzRlmf =  40; 
long udRlFosVE = 71369199347350135; 
string kBdcSb = "BUFimoQTAajxKah"; 
double fPXczEgZfBAMpaYjgP = -3.036058E-15; 
while(fPXczEgZfBAMpaYjgP == 9.473477E-39){
fPXczEgZfBAMpaYjgP = Math.Ceiling(Math.Sin(2));
Console.ReadLine();
}ushort hQdWgNNiTlDlPD = 22483; 
byte FOWuYpCnkafZmatuTZtWof =  206; 
uint MXSDeRFdBEaybjAD = 91718733; 
long iHkjSVDYqxzYyUltJMfj = 14167809165827950; 
float IjeSqfMILjnH = 1.257793E-32F; 
sbyte ZhXf =  100; 
double CgnUPSUdNeXPoTGDgIabWWUh = 2.674443E-13; 
if(CgnUPSUdNeXPoTGDgIabWWUh == 6.36529E+13){
CgnUPSUdNeXPoTGDgIabWWUh = Math.Ceiling(Math.Asin(0.2));

}double YYBmTLGkpmWTAgW = 1.585502E+14; 
YYBmTLGkpmWTAgW = Math.Truncate(YYBmTLGkpmWTAgW);
 bool? kcFRAuMjuaEAdHZFNpV = new bool?();
kcFRAuMjuaEAdHZFNpV = true;sbyte MdXIecOaofVhSEqPBmoIiZWIuW =  51; 
float seZHxjqR = -5.241812E+17F; 
byte peJGTloocOHpz =  178; 
ulong onhqVP = 28945205492579876; 
uint bmTVmcCKxEhEyKhM = 191556; 
uint EWABhIKqRUodpgnkRRUmNyV = 6384; 
ushort ltJWQyKWfNtNFtjTCo = 4982; 
byte wXVHpobgaiT =  193; 
sbyte YiYzHGjiyydoTEF =  -30; 
string glRxNoBhJQeFBZAVlK = "RyIhmAzEawzCHgpHT"; 
int wKYzXtdTXdPYqVRkf = 82972; 
if(wKYzXtdTXdPYqVRkf == 280327){
wKYzXtdTXdPYqVRkf = wKYzXtdTXdPYqVRkf + 712871;
}uint kozzjtHnKnSTYLOiHCAlyg = 78924262; 
double hPcFgZzM = 2.922005E-33; 
while(hPcFgZzM == -1.913058E+18){
hPcFgZzM = Math.Ceiling(Math.Tanh(0.1));
Console.ReadKey();
}double kBGJTZZQtESNfk = 7.695278E-15; 
if(kBGJTZZQtESNfk == 5.992524E-10){
double XBLGfFTQjuqY = Math.Log(1000, 10);
kBGJTZZQtESNfk = XBLGfFTQjuqY;
 bool? TNKWUK = new bool?();
TNKWUK = true;
}
}public void HBoERxmPLSXeDIzyh(){
uint tETATzxgbtkCqkODjIGHPx = 4458; 
uint hXfjVHLfUYNbwfuokwBqGTPeoQDU = 950945175; 
byte XWpAbSzEQlpPQh =  62; 
uint AkYWkqnia = 102906908; 
string iXOpezPVnsXXBuDZohDcIyNlA = "GcpbYtzZctbAcOLqlSaZKWM"; 
sbyte MDciLkCQsTMFmuFzCqjYQe =  73; 
short kUkBcKqhplQ = -4520; 
ulong iAJLVYMZzxBi = 1956453146887866; 
uint popjwyWhOcL = 24436401; 
uint uUsGkx = 1018602; 
float PWuFc = 2.853193E+35F; 
float itwLEndpIzzFBNOnTtmoVCoxe = 8.222098F; 
uint LqNijpNYiPqhfhDwAn = 61; 
short sLCWIeZG = 28354; 
ulong NWgMxqyboKEWhPuNb = 12165863136737673; 
long IIZESEHtk = 18446899766471110; 
ulong uhnsYaExGAwiLPdPtpgLtBxi = 31332292590938742; 
byte mfupbDnga =  223; 
ulong ZoTYlMtcx = 87508285893881070; 
short tXzCYucoEx = -17626; 
uint tesmCGeAJhPutoowe = 27; 
long xoBOVEiyPGksiMWnlIm = 5814033599679729; 
ulong AqYPXWQSAHcWcUCFbkEtgSyaCQ = 42073351288938586; 
short mElPfwNbkn = 14976; 
float efuCuNIUXn = -4.834298E-24F; 
short TpE = 1727; 
uint msyYPmMfxQneVjNBWX = 80; 
long opeFPuTMHMllc = 39782476324481831; 
sbyte cnhnttHBJWSchTjzqlLiCSKgbSBjU =  73; 
sbyte ipcqIgBsalDokJBDDJKoNYzjFamB =  94; 
ushort UQVUSRTRjMtioouWssX = 41950; 
byte RjkRS =  187; 
long azEHJzpOIMKSBEuPiVjdUiVPtP = 49775593105048107; 
long GJHWHEKlXuFnPwJXDEZRDOmTAFd = 77092912088596129; 
uint nypzJSYjMW = 3467; 

}public void EGq(){
float RxNxcAbAtMww = -5.451564E-21F; 
sbyte ogJtTVnJOIPpLZcSYhqdB =  83; 
ushort xjAmPpHcjsoAJBzWheJuLMOtK = 36701; 
ushort UeiyMS = 60692; 
long bUybNftLDjyKwW = 80437117838501404; 
byte zhejSOQFPAgjksXGwJhtazAiMo =  185; 
ulong nqAIFDGb = 5999965342929815; 
byte IieaxwSSaJWyDcdtBhHDHQqFlQtZ =  37; 
int sBseEgnUdPNDXeMpM = 4878; 
while(sBseEgnUdPNDXeMpM == 4878){
sBseEgnUdPNDXeMpM = sBseEgnUdPNDXeMpM + 691864;
}short RAOGbTLyfEXjXAllmENTmuBAB = 8338; 
long jOjQcmNHBocWceRBPgPR = 89150578152880899; 
double fwkcnstghL = 6.652321E+07; 
fwkcnstghL = Math.Floor(2.476763E+25);
Console.Write(fwkcnstghL.ToString());double yHJydqtFt = -7.028378E-11; 
while(yHJydqtFt == -1.347099E+11){
double aOeOiApPH = Math.Log(1000, 10);
yHJydqtFt = aOeOiApPH;
Console.ReadKey();
}long exulptaJPsa = 38000844448197443; 
short IjCdYOQxTcEyNFAWzVPJPuPnjL = -31574; 
ushort ZFCsx = 29081; 
byte isFklVoydbsacsxUHemwnUmNtq =  222; 
uint eqhl = 31; 
int yEzARgKJ = 58518; 
if(yEzARgKJ == 635462){
yEzARgKJ += 733490;
}short SFofSSVc = 21499; 
ushort LjOYcTKiCOVxNDeYnlpxsYYuzaysG = 43424; 
ushort yiZzHeFoXERwWiCDOUHQ = 34072; 
double MluBbxqDTsmZFEGqjIa = -3.765491E+21; 
if(MluBbxqDTsmZFEGqjIa == 0.0001670917){
MluBbxqDTsmZFEGqjIa = Math.Truncate(MluBbxqDTsmZFEGqjIa);
object iBAQFKVjDCNQRMfB;
iBAQFKVjDCNQRMfB = 6.210142E-27;
}ulong zPhCVxj = 43980982680073865; 
ushort sTj = 8689; 
short IVJPASesHbwlgcaGpH = -15997; 
short CoHQNkdQaFcglAG = -16717; 
long MsntLnqjsZMDDTcegHcZN = 28429146389757327; 
int tJWCXkiFtzDTd = 44; 
while(tJWCXkiFtzDTd == 44){
tJWCXkiFtzDTd += 144510;
}ushort kIXfqMNnVloPFtUGsfUXcJBkNXziu = 12639; 
string jwhaZwGzxpzmmdALDxQYnwX = "hNstmEULlCGikTHWA"; 
ushort MknXJyOGYtcAOzHSl = 4338; 
byte SxNaMzpA =  174; 
ulong zSUkqUQJWAXVqibSLFawTnjJpy = 20376584303956765; 
int UmoYxupMUCxSKHGUJuak = 20095872; 
if(UmoYxupMUCxSKHGUJuak == 641516){
UmoYxupMUCxSKHGUJuak = UmoYxupMUCxSKHGUJuak + 574774;
}
}public void PkbLREuXyocZzXz(){
ulong buDFCunQBuZKswqSWCCTQFnp = 4563418519931257; 
ushort SmbTn = 53499; 
ulong qjunfULgCxXVZxglWtPIwYhQnmD = 47593211151426506; 
string ucLVnZQlBfxfPXUbfAscFaSqHqz = "lQehcqC"; 
short NhZq = 4629; 
ulong fYagtJ = 57952278122069955; 
byte CHaKbodzwEYtstMalLkIpqQERKlb =  1; 
sbyte KzkzKE =  -109; 
sbyte VDLPLGcDppiAiGHjRxTscbzoy =  -55; 
float UbyhGt = 3.62677E+19F; 
int SkH = 71507149; 
if(SkH == 641602){
SkH = SkH + 508811;
}float oTgkNPTexsxLVInTaT = 5.796666E+27F; 
float QFIMtqOpQObEGipXT = 7.035868E+27F; 
long JxALayRY = 65448026113797733; 
string DpfCIKqcfDcZkZ = "qgihasUW"; 
short dbU = 24652; 
sbyte uRiqtfgPQuYsoWOBaAuVuwb =  -110; 
long NBFIyiZOIibJS = 67870918277407870; 
ushort DauAkuZQe = 42737; 
double HjhWPsBnGzqI = -6.86913E+30; 
if(HjhWPsBnGzqI == -2.094579E-07){
double nbIcKOuJtXaTU = Math.Log(1000, 10);
HjhWPsBnGzqI = nbIcKOuJtXaTU;
Console.Write(HjhWPsBnGzqI.ToString());
}double YDAHTbaJQzGZDuDsXcPEcJ = -911495.8; 
while(YDAHTbaJQzGZDuDsXcPEcJ == 3.900192E+17){
YDAHTbaJQzGZDuDsXcPEcJ = Math.Ceiling(Math.Sinh(-5));
try{
Console.WriteLine(YDAHTbaJQzGZDuDsXcPEcJ.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}float DJuIR = 2.807253E+08F; 
long QQbj = 35468117072074561; 
long fONdNuiosW = 44470700072752553; 
byte DuYV =  157; 
int KyaazSuoTdAToVCyequGcbfM = 37302832; 
if(KyaazSuoTdAToVCyequGcbfM == 714374){
KyaazSuoTdAToVCyequGcbfM = KyaazSuoTdAToVCyequGcbfM + 812044;
}float jGzSqWuPqMkpVlX = -2.605752E-37F; 
double ElXMIqomekIFdkbKKxJghWGnqR = 3.626024E+23; 
while(ElXMIqomekIFdkbKKxJghWGnqR == -0.01013173){
ElXMIqomekIFdkbKKxJghWGnqR = Math.Sqrt(4);
try{
int nMsQMHHKwRCzLJMWPg = 6166172;
if(nMsQMHHKwRCzLJMWPg == 83986){
nMsQMHHKwRCzLJMWPg++;
}else{
nMsQMHHKwRCzLJMWPg--;
Console.Write(nMsQMHHKwRCzLJMWPg.ToString());
}
}catch(Exception ex){

}
}ulong aideyfiMnZFf = 3406150148232683; 
ulong ojUsHkAzbjRRKDSUKIkmWIaylSg = 26131509202854198; 
float fzYmPYwo = -5.676186E-06F; 
short RRpsEZIjxgOdzkXqGoGJY = 11892; 
int mahKFiQw = 332988596; 
while(mahKFiQw == 332988596){
mahKFiQw += 853620;
}double kKOYK = 9.196649E+20; 
while(kKOYK == -9.485806E-32){
double LhmgfESYFnpslgPKOOqB = 2.053317E-30;
kKOYK = LhmgfESYFnpslgPKOOqB * 2;
try{
int AslEJBBKFkGoZwNwlmfB = 7676177;
if(AslEJBBKFkGoZwNwlmfB == 2060){
AslEJBBKFkGoZwNwlmfB++;
}else{
AslEJBBKFkGoZwNwlmfB--;
Console.Write(AslEJBBKFkGoZwNwlmfB.ToString());
}
}catch(Exception ex){

}
}uint KtbWnkiuYiLFOqjG = 276292067; 

}public void jctP(){
double XXcO = -246435.8; 
while(XXcO == -8.437524E-17){
XXcO = Math.Exp(2);
Console.ReadLine();
}long lMTZoam = 81474913249416462; 
byte DToYa =  158; 
uint KIwSUbCyNYYxX = 43; 
ulong kHRSsFJfNyRujkfs = 46330847962631876; 
ushort oIRiCyDzPZKDqdMNhELdD = 49261; 
float swdUK = 2.007901E+36F; 
sbyte JoboFVuRMwfJbno =  -96; 
string eaogCONQftJOf = "EeJ"; 
ulong yEIpmRqOwphONHpUfiCNm = 14603693137776227; 
sbyte FCxGQxLcpkasAYGVgPfzVgayWPSo =  -106; 
float Ipt = 5.979385E-21F; 
short Byug = -18427; 
int jnRwmSBZG = 45; 
if(jnRwmSBZG == 675199){
jnRwmSBZG = jnRwmSBZG + 16108;
}int YzjsbgwtFeoJd = 91889016; 
while(YzjsbgwtFeoJd == 91889016){
YzjsbgwtFeoJd += 515284;
}string SaBJKqeMqtAUZiNtUhhwcIOlqDm = "cPWBDBfyJKsQpHnMQ"; 
ulong GiepF = 5363307794068264; 
string KDxZqAcoKkQsfxubxD = "HtXzqgXJVkuqjMN"; 
double oQyFETufPaBxQkSQKxiMde = 2.617306E-09; 
oQyFETufPaBxQkSQKxiMde = Math.Ceiling(Math.Atan(-5));
Console.WriteLine(oQyFETufPaBxQkSQKxiMde.ToString());short CoQJWWaWHlfjwKqTdduIfVGJNNWL = 5692; 
byte Jod =  139; 
uint xuSofVwWUkcgmnITI = 620673520; 
long Wiaoqx = 7275441680982706; 
long saakNSDTFqWtPHkWQw = 25629445090313101; 
double VnVHXqoRGzudHgLaASImgkcVg = -6.177412E+15; 
while(VnVHXqoRGzudHgLaASImgkcVg == -8.184002){
VnVHXqoRGzudHgLaASImgkcVg = Math.Exp(2);
Console.ReadLine();
}uint jQVKRXPCfPIFSfFPMfXVnitBoAY = 81447015; 
double jzioLOLobAEpbnoE = 3.705349E+08; 
if(jzioLOLobAEpbnoE == -3.329137E-32){
jzioLOLobAEpbnoE = Math.Ceiling(Math.Tan(1));

}sbyte PkWhehtuKZ =  126; 
short ZnhCFIJTUaRKExScysHKhJGFgj = -18019; 
double sxTUVRUMqhxzuVkfzB = 6.827845; 
while(sxTUVRUMqhxzuVkfzB == -7.008362E+10){
double IDwwxLpDFXPJ = Math.IEEERemainder(3, 4);
sxTUVRUMqhxzuVkfzB = IDwwxLpDFXPJ;
try{
Console.WriteLine(sxTUVRUMqhxzuVkfzB.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}string yjuWCm = "ONVm"; 
sbyte QyBycNfkzjyKwO =  -41; 
double uVhZqpUoaYghZeBZWQKiucPWV = 1.662397E+38; 
double CsXyAhHicOZdzfXKAXIPxqfkdqpX = -1.801344E+15;
uVhZqpUoaYghZeBZWQKiucPWV = CsXyAhHicOZdzfXKAXIPxqfkdqpX / 3;
Console.ReadLine();string lRwXAoZsq = "tWoejLTdTNufetjUF"; 
float PpThAhqwLISokwTiTtWxusgyqTgM = -1.040098E+21F; 

}
} public class LeuCmBOQXTpiwbziLlj{public void BIyExiwRGaG(){
short dgjQTHIfEZEdjLxSKxATzedHRfde = 29005; 
byte nYksUntRGCkWITAmzZYWFfAAYdDZ =  195; 
int iudpfYsCEqFUqmhwOq = 603241324; 
while(iudpfYsCEqFUqmhwOq == 603241324){
iudpfYsCEqFUqmhwOq = iudpfYsCEqFUqmhwOq + 337574;
}sbyte MuRLtdcYcDFEunf =  -8; 
ushort ZkilQIPJVXAjfMhHlYK = 56049; 
double RskGRkXFAGDJsXpBE = -2.002169E+32; 
RskGRkXFAGDJsXpBE = Math.Ceiling(Math.Cosh(5));
Console.ReadKey();int RPCP = 3; 
if(RPCP == 514882){
RPCP = 660908;
}float cfGnDdmi = -165.4588F; 
int lFDuHPUEUoECybCFEDXCGeK = 2; 
while(lFDuHPUEUoECybCFEDXCGeK == 2){
lFDuHPUEUoECybCFEDXCGeK = 680511;
}uint ojODCAmPMOmqNDdQCCJGGFeigU = 3723; 
uint oUxwnsKzZ = 32; 
ulong NsOWHXpa = 50793537474571403; 
long ToAAsV = 57711380835898742; 
float qQtXNxjmkTEhE = -1.018004E-31F; 
string ZyzMmaTFskjX = "zDwqOPcKtyflnzqjL"; 
uint ZjE = 391383167; 
long PBtu = 66757578105630204; 
uint bJGZsAZCDwI = 17882390; 
double VLaKKZDqZlg = 3722680; 
if(VLaKKZDqZlg == -6.115887E-06){
double CXSbhenYYNzQqtBpNDOpzsHNlDuym = Math.IEEERemainder(3, 4);
VLaKKZDqZlg = CXSbhenYYNzQqtBpNDOpzsHNlDuym;
 bool? aPXSYDGMEslOpHkE = new bool?();
aPXSYDGMEslOpHkE = true;
}sbyte mnmzdCGINiXVVStIaYpIaBWVeQF =  -75; 
double UJBVzL = -1.232154E-18; 
if(UJBVzL == 3.891356E+35){
UJBVzL = Math.Ceiling(Math.Atan(-5));
object NssunWuMHoowZuIqNLBKeQqSRgC;
NssunWuMHoowZuIqNLBKeQqSRgC = 1.880808E+09;
}ushort lfSPWbBD = 9338; 
ushort EAqmwXJosTKJfVMadZxUHNWL = 61670; 
sbyte OqzpTyOGF =  122; 
uint MYJcsDyMTkayDL = 173500; 
float GonKUYUDRkqogbCEKV = 2.566742E+14F; 
int BwuEpaO = 93; 
if(BwuEpaO == 29225){
BwuEpaO = BwuEpaO + 370194;
}byte cdLhPLl =  101; 
int ZWZbF = 1326; 
if(ZWZbF == 799026){
ZWZbF = 873402;
}short IUPJnNAfIow = 9596; 
float CPznaRAqe = 2.756719E-29F; 
short JcWynMNdnZcYI = -28230; 
double IEtNYGjYCTA = 3.94032E-14; 
double MRhVej = 4.813231E-11;
IEtNYGjYCTA = MRhVej * 2;
Console.ReadKey();double jOWMIjyPRWlcBFuHPoVGLAu = 4.273801E+29; 
while(jOWMIjyPRWlcBFuHPoVGLAu == 1.532472E-08){
double qzMRUKQzsDDHazkNXNbBwqc = Math.IEEERemainder(3, 4);
jOWMIjyPRWlcBFuHPoVGLAu = qzMRUKQzsDDHazkNXNbBwqc;
try{
Console.WriteLine(jOWMIjyPRWlcBFuHPoVGLAu.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}float BMiUuaXcYdTNGF = -6.678546E+24F; 

}public void EuDciMHQegmQFYscshKhWdpi(){
sbyte WCoSaxRcMyqt =  -43; 
string epULmufCKmyMKUBdt = "QPfBmfQYClVMZRKGROwzUZRZFFE"; 
long iltXzCk = 56664442287854756; 
long nGcdcMIVaRYfVICeQtaCOCozjmLHE = 78264962929865922; 
string ALcyK = "qJwnQzzSf"; 
byte pEUQbbswKaNoqExMnBRMdGl =  221; 
int tOgJfmwFIej = 4633; 
if(tOgJfmwFIej == 48171){
tOgJfmwFIej = tOgJfmwFIej + 1791;
}byte teLuMjqnzJLsalMJEn =  6; 
float TMVqDYchbbGYokkI = -2.850741E-08F; 
uint ZXWWNicMagPYseAsPcjlogy = 10898280; 
ushort bCajSDgBWQZFuWqgVmHHkL = 55649; 
float lmASBoRtyak = -1.258955E+33F; 
double kFDDQzPpYaCuAM = 2.58834E-15; 
kFDDQzPpYaCuAM = Math.Ceiling(Math.Tan(1));
try{

}catch(Exception ex){

}byte lVaeTJbowFEDJXuQdq =  2; 
uint QbfNxhJ = 8358; 
long jnJIdixlhdIxGRyRdQNmaKwwU = 71367569841559174; 
ulong woNNUUOtehFA = 32285172382199668; 
uint mNhQZKPieCTzcDpdStlppnoxxy = 7608; 
short AXRDcZGdUdEWddGy = 29545; 
ulong BlcEjDZyMu = 2957116780791615; 
uint pyPzhMtjlQZAzzRm = 370126; 
long dCUKsGDfzbKQymYcyFwyiPlVucaDY = 58240385190193171; 
sbyte DjmLjf =  -84; 
string BPPSXjRn = "YooGZhgyhRX"; 
float ztzTnVdb = 2.469455E-22F; 
string cjDNFPKhzGPRXkfENYseuppx = "dWVknjdnQXXFL"; 
int nJL = 561736521; 
if(nJL == 59660){
nJL = nJL + 797163;
}uint RTWuMTftBS = 95624314; 
byte MEgnhZcwpRMGyT =  186; 
sbyte fLZsksxkeAqbiALByReQPIBmwD =  -105; 
long xlHtZJ = 76240171224674478; 
uint ugsbeZbIsdzjNXImOeJmKI = 81; 
ushort upTfNEaCawN = 41800; 
string jZbVjdzfTYWYwm = "MXC"; 
int FLPlqUikWlBgclcLHoHuRoyU = 76; 
while(FLPlqUikWlBgclcLHoHuRoyU == 76){
FLPlqUikWlBgclcLHoHuRoyU = FLPlqUikWlBgclcLHoHuRoyU + 287307;
}
}public void ldmRzkPEoUmiHyQ(){
double koeJUiX = -1.299411E-18; 
koeJUiX = Math.Ceiling(Math.Tanh(0.1));
try{
int BkpNbEzFCTiN = 1954695;
if(BkpNbEzFCTiN == 44908){
BkpNbEzFCTiN++;
}else{
BkpNbEzFCTiN--;
Console.Write(BkpNbEzFCTiN.ToString());
}
}catch(Exception ex){

}ushort LMTxzYXiAUtjeKOcZFXnH = 46383; 
byte yWSPlWqNTdpRFhcUkEHUnGinOGt =  152; 
uint caAXZCf = 93; 
int GpbtXySCcccdsIRNEa = 55420173; 
if(GpbtXySCcccdsIRNEa == 693156){
GpbtXySCcccdsIRNEa += 9776;
}short IhBPcHdqJohU = 21423; 
sbyte dxgaWhohPDisaRqHa =  -108; 
int KanawZbZDFJjxbJREZsjtZMl = 286730438; 
while(KanawZbZDFJjxbJREZsjtZMl == 286730438){
KanawZbZDFJjxbJREZsjtZMl = 667498;
}sbyte ltRchgFXfoGkTYVZiBcaMwyKo =  -118; 
sbyte WCJTxS =  117; 
string HJyuwddu = "sVtdpRGgeQWgNfaz"; 
string HMObaRPDRGGpqtpPKk = "TkSnpNhhcTDxWwAxIMqUDoO"; 
double CPzDRXxPMgsPzznSKMJniKURWJdtp = -2441863; 
while(CPzDRXxPMgsPzznSKMJniKURWJdtp == 2.374597E-18){
CPzDRXxPMgsPzznSKMJniKURWJdtp = Math.Ceiling(Math.Tanh(0.1));
try{
int ABDVXj = 453766;
if(ABDVXj == 98632){
ABDVXj++;
}else{
ABDVXj--;
Console.Write(ABDVXj.ToString());
}
}catch(Exception ex){

}
}string chK = "lLZDOSOcsFosoWqKeOkiBD"; 
float OkgewMetPUzEAnCwFZaChFQ = -4.258011E+11F; 
ushort oXClJTlPDMJHGBWDuyOkQlPECDpG = 11329; 
ushort ybP = 51694; 
ushort IQLmzZKVsw = 998; 
byte wIU =  49; 
sbyte hXLg =  15; 
int hLlGQLAoNIcXsmeJigo = 1455; 
while(hLlGQLAoNIcXsmeJigo == 1455){
hLlGQLAoNIcXsmeJigo = 831888;
}long npXJgHTSZJNFoMMJaRmORc = 34062612777382774; 
long gZkYPYWpzufZxDZJwstMFUVl = 83131500864300499; 
string ypBOTbVWpxXofxnDcPYXjmeZWtocV = "JyZye"; 
long IzqQJFz = 59601220094138809; 
long OkkB = 74498249774238392; 
short sEuwpHgCIHLiccycNNNX = 12011; 
float IRYGXItMPzIVbzWH = -3.590922E+31F; 
byte FQIyKwXzaNsYzeNFsb =  47; 
float VWKugwFQfL = -7.08519E-06F; 
short RiBVDLULbgKyXLsobMuoKTXcEZs = 10363; 
float pChi = 1.544949E-06F; 
float IitjUE = 4.636993E+14F; 
uint zRJpcalYkysDsmRZRhoOQVyVek = 496670763; 
double sTBzjctZAgXXk = -1.37573E+20; 
if(sTBzjctZAgXXk == 6.51246E-29){
double uKdDuNlReMYQeAbTcO = Math.Log(1000, 10);
sTBzjctZAgXXk = uKdDuNlReMYQeAbTcO;
int? RTLdhZuPg = 6110802;
RTLdhZuPg += 76893;
}
}public void blYwoocXdFnVUlTjSpGl(){
ushort FUioakyxeac = 65042; 
sbyte VpPSXxMDRMVlXlOtZDl =  58; 
int lZnRB = 49435924; 
while(lZnRB == 49435924){
lZnRB = lZnRB + 99777;
}byte DCOnZFccxu =  198; 
byte JMbBNwYQd =  202; 
int WecFTa = 67; 
if(WecFTa == 827100){
WecFTa += 229885;
}byte hFcPFAEnZ =  193; 
byte hKpzAMjidBZy =  142; 
ushort FpyVPmBmtOnkeDIfotDS = 28709; 
ushort MecejIGFlkSYYCS = 27854; 
string yWLkCfHwwKeaCkkSLQN = "qGOjbal"; 
int EyXzlZyNBDWzOmVAkJINg = 33168378; 
if(EyXzlZyNBDWzOmVAkJINg == 294116){
EyXzlZyNBDWzOmVAkJINg = EyXzlZyNBDWzOmVAkJINg + 832464;
}sbyte jCTcLRmhDpkOejnQ =  47; 
int TKQqFQDMwnDW = 30; 
while(TKQqFQDMwnDW == 30){
TKQqFQDMwnDW += 63169;
}ushort wIsJ = 23295; 
ushort bHOSlMzoQtkolheSTatczTTnNnAXk = 22737; 
sbyte hRCfIwqjKaxYQOjVpQzJZuBiNAfXm =  2; 
string klgwHZjzJiszxUy = "dFIuTAaOqtshptXjWYNFHPnqGaZQb"; 
string TcgTITKFMltemHchpliCzh = "qopqOSAjMnIAfiApqmtbtnKbKE"; 
float ETBmLiPEqCVNlbIQGgXZQ = 6.522594E+25F; 
short gBdGo = 8497; 
byte QHSOuPWtVYPqGPsHUwEHTTIblhFJM =  249; 
string LNwwPclsaGUXcYAqiyLUdFfXYQC = "jnHWXUHSu"; 
double SjkHmYLZnInAonhRgIzxZBZiM = 1.594696E+35; 
if(SjkHmYLZnInAonhRgIzxZBZiM == -3.094739E-13){
double TLSXTYnRnFWPsJPcAkVB = 5.680258E+33;
SjkHmYLZnInAonhRgIzxZBZiM = Math.Round(TLSXTYnRnFWPsJPcAkVB);
Console.WriteLine(SjkHmYLZnInAonhRgIzxZBZiM.ToString());
}ulong ihlwlXHokfRxbMOD = 48982035771376404; 
int BKosdqywqKeOQXIMALe = 275482; 
if(BKosdqywqKeOQXIMALe == 846056){
BKosdqywqKeOQXIMALe = BKosdqywqKeOQXIMALe + 938356;
}sbyte iEGiU =  89; 
int WZTcBYHudyMZfX = 604; 
while(WZTcBYHudyMZfX == 604){
WZTcBYHudyMZfX += 336838;
}double xwUlnRLCjWjCLNHJFHHhQukj = 8.234458E-22; 
while(xwUlnRLCjWjCLNHJFHHhQukj == 3.574587E+28){
double hDyhnLPJDmQQFmzx = 1.730873E+29;
xwUlnRLCjWjCLNHJFHHhQukj = Math.Floor(hDyhnLPJDmQQFmzx);
try{

}catch(Exception ex){

}
}uint ybIpBVKOUB = 28352177; 
byte txTqL =  231; 
ulong SdFbtGApyuJ = 65258628298566810; 
int UpGTjFgMpyGzgjFM = 539573; 
while(UpGTjFgMpyGzgjFM == 539573){
UpGTjFgMpyGzgjFM += 66594;
}short sbehsssziRYOZiiatFh = 28850; 
string WeKPBRstVQbZBiA = "uYfhopkQNoteVZWqDVfZzDRBfPWhN"; 

}public void JZBukiRVhGTTclynyWUKyGdBARhFb(){
uint ddOHtEBPNBHNA = 60; 
uint dSsZmdDcWaVtblUIlQQ = 5914; 
string XghiQSEuUqDCjhuRZkFxD = "wLoNfnpQlaZhuTfUMjt"; 
int cAL = 49555173; 
if(cAL == 801607){
cAL = cAL + 58215;
}string hsMyUcNpddeCDcNCLfAu = "WFtkE"; 
string xqaiZsachJaJxcDXXXh = "NpnKEGEkNslpahUNdVDk"; 
string uceQXbtHxmwwTfcpFuOPp = "xKJohQUbwXGcWkGOIDSgVYUF"; 
short KNqhxmuFGwgJTDNwSzQRiSLR = 14770; 
byte iUzPdZtggLmCsIsFjHZKouBgxPzG =  214; 
string xkplLhGcJAxGKFWy = "RmBjNDJ"; 
ulong stoCOodkRpcx = 63702048886567611; 
ulong AsoOFCpKJHxqPmOhM = 23928094955557781; 
ulong GxsQbDRTXmZKDVgIwZoIsQizKlq = 59787420741801421; 
byte QwPYuRVMzZF =  163; 
double NViEPnUgQldZaKxVWjTqFFBJYZnMl = -2.503672E+18; 
if(NViEPnUgQldZaKxVWjTqFFBJYZnMl == 3.20963E+34){
NViEPnUgQldZaKxVWjTqFFBJYZnMl = Math.Ceiling(Math.Tan(1));
Console.ReadKey();
}byte ImgxfYnQbdXnOqBmif =  222; 
ulong hxMbIXLkwgGTAlCi = 7809797873828553; 
short MXxZmzAdARuuz = -31732; 
int GWswnfZeLWXfujCWMml = 78267316; 
while(GWswnfZeLWXfujCWMml == 78267316){
GWswnfZeLWXfujCWMml = GWswnfZeLWXfujCWMml + 388724;
}ushort KPdLVdBsYyVfGYsoqBtJpo = 29320; 
long OQQjckaS = 11037546909744280; 
float zRgGPxhDHaNxwswBg = -2.741294E+14F; 
sbyte mUnmJcIgzqOzNYRdiatcBFaeXTXl =  49; 
double kJHlWTtftAwhKYXItExYmpfx = 2.150955E+29; 
if(kJHlWTtftAwhKYXItExYmpfx == -5.80301E+22){
kJHlWTtftAwhKYXItExYmpfx = Math.Ceiling(Math.Cosh(5));
int[] UDkDcJcpndYGguhCHJ = { 5472461 , 76096 } ;
Random ispDqmEdqE= new Random();
Console.WriteLine(UDkDcJcpndYGguhCHJ[ispDqmEdqE.Next(0,2)]);
}sbyte aKmWjSCMEMAfQjLRKW =  125; 
string jEZTpzGMNLpQbgLVGyQKxPEd = "xusATGOg"; 
ulong fmcTtWaRbipOcaVeAjstwZWJ = 19263916804946295; 
byte GIEExOxXDUU =  168; 
short EPgFOwwfquZo = 9814; 
ulong VHIBMunCVNFXyybOkNWJdYYUSVBq = 54262017403649943; 
byte GXpnlR =  45; 
ushort JpGODMfUXJNJQUXspGAoyQOBdgYs = 52966; 
ulong MwXSaD = 24353243826588965; 
uint dtVVlKhBTDfLwNNekCbmScWP = 998105; 
sbyte sfiZFpVZURVbmyYzyfjHQbA =  -115; 

}
} 
public class sdujYX{public void mNCTszzCOtluzusBJIbWPhsPNR(){
sbyte BgAsS =  -98; 
float oxHnWFOFIzF = 3.968352E+37F; 
int DXaeewMgCotbIUTCsCblla = 99564476; 
while(DXaeewMgCotbIUTCsCblla == 99564476){
DXaeewMgCotbIUTCsCblla = 135037;
}short yeXwWwRbO = 26638; 
int fYDEfKPWlkY = 4878; 
if(fYDEfKPWlkY == 18708){
fYDEfKPWlkY += 896845;
}long GSnSodmXyKqNgJVVPyHo = 15790330551619426; 
ushort RgdmXzRgn = 33373; 
float qlVzRIEHwEnxFmAmGhxqElT = -1.055665E+13F; 
float ydIifwpagfOoZsNRmSIJKZ = 2.380668F; 
sbyte DjesL =  74; 
double gENwhanmshSSyZQwHsajbyAgdbT = -6.013745E+31; 
double dSKIwyIxJ = -1.330457E-34;
gENwhanmshSSyZQwHsajbyAgdbT = Math.Round(dSKIwyIxJ);
object KLsOOUKdGsghhq;
KLsOOUKdGsghhq = -4.23996E+09;string jDiC = "yAEBn"; 
sbyte KPMmP =  63; 
short LwZpUahCLRufyJJYklBUodTCzKt = 11504; 
string CaUiqPE = "IepYTJTaIagNbDlzGICyRjIzyD"; 
short koqTwsFxEqjjI = -3380; 
string DsgZOwEKnfbz = "SBptAPtJmqKXQVn"; 
ushort PugoVfwpGPMI = 40587; 
short gYLRUQiIKJp = -32103; 
ushort YBo = 15158; 
double IqhXwVdTaIb = 5.874304E-36; 
while(IqhXwVdTaIb == 2.582207E+25){
IqhXwVdTaIb = Math.Ceiling(Math.Sinh(-5));
 bool? IcIKYdkgqRIw = new bool?();
IcIKYdkgqRIw = true;
}float UdCXGO = -2.266905E-17F; 
ulong owfbWdWSbOUshfUQdHsRzCyjdML = 8289888910695275; 
string ESniDXhlCVwcQKJWWxOYWYkthmj = "fubwUU"; 
uint dIWFpXCRoLPhXScKXbZFIUao = 326208; 
sbyte TZiWjU =  91; 
byte iJZytdyzcjmqqySU =  233; 
sbyte IHRoQuX =  -52; 
uint XdZOiOsahnRDRKRTQCDpbnNKGU = 3803; 
sbyte xOVG =  -100; 
sbyte DiJAUOTseP =  -91; 
uint EwNlQJppioyYReScIkELFmtTQhBU = 5170; 
sbyte VlAabJQy =  80; 
double TlCcKapyFEOsq = -3.65495E+28; 
while(TlCcKapyFEOsq == 4.297262E-27){
TlCcKapyFEOsq = Math.Ceiling(Math.Tan(1));
try{
int IofRTbdBHgThYhngo = 5409762;
if(IofRTbdBHgThYhngo == 22195){
IofRTbdBHgThYhngo++;
}else{
IofRTbdBHgThYhngo--;
}
}catch(Exception ex){

}
}sbyte qCXEKHXMyTULnSzHfQ =  43; 

}public void JNLNFpjliJxWhEXTbiasBLHou(){
ulong EwetMDnUFRkYtxLEOyXDKVGGWQq = 77206855608132836; 
float pKtUK = -1.664496E-36F; 
float HVQgSSxk = -4.955412E+12F; 
ulong yWlWVyxRhYQTAzN = 54471382597613058; 
ushort SoEXHjXaGTOhXOVGZ = 3892; 
long WIueS = 22126457782149433; 
ushort iTUB = 51682; 
float UGZxMlHXPJBoFkVzeeYROVhbqC = 9.419698E+17F; 
ulong DTmdN = 47522323195504802; 
string CRTnuPxQb = "YzYfk"; 
uint kKgIcCygtJnZsAZePQLRztofckY = 31636601; 
sbyte GyEdlWFzoxh =  -126; 
sbyte RJFZtip =  0; 
long SubFSDJQltJDOx = 64436880955831896; 
sbyte lCtPXYafNhlSVZHMNqONAnVilkXY =  -104; 
short Akesp = -20808; 
float msEPHkVmkLJIZExyXHkDMacazb = 6.610425E+10F; 
float qDTzCwWAnYqXNnxHgVPI = -1.738311E+12F; 
double glpDpFymxFt = -9.025099E+20; 
while(glpDpFymxFt == -1.673406E-37){
glpDpFymxFt = Math.Ceiling(Math.Acos(0));
for( ; ;) {
Console.WriteLine(-1.169712E+12);
}
}byte OVgpKMg =  72; 
sbyte nZpYecpsSxbqEwKZUKmQiOHyeSa =  30; 
int ZcS = 69148253; 
while(ZcS == 69148253){
ZcS = 301815;
}byte sKPh =  114; 
short GLSybpLBsTUmbKNRyYzWMk = 6214; 
byte cORdSoHApaUhC =  4; 
uint jgCNcitGTqiYwZwfGdZCUyBEnNM = 84841634; 
long YXaDddzqTwkkCgJepFPCzbabMHYA = 13167097066602826; 
ushort schMfgKlYxGIwed = 8964; 
uint WZQxfZbwMyagWHLTmMuMVTsAICUI = 60; 
long Wneu = 21803807834209913; 
byte BYXCRqMMadIpPEettfNYjIDpl =  87; 
ulong akzfnQbVK = 89143547639673255; 
short udsqoz = -15885; 
long xIoACALQFDbYCiJSsuIZuBEVlw = 14375109829504458; 
uint koRQ = 459245; 

}public void jphSUJzXdGJzVe(){
ushort gpcWwUaRtK = 41295; 
long ESAW = 49177859379785960; 
byte wzKEYQbLAyCbpopAjc =  210; 
float OiDRLPjEacBhqGsJ = -6.73783E+12F; 
float Ssc = -4.327406E-35F; 
string OdfXFtcIphN = "aSgzubTYK"; 
double dtZSIxgiOIAwAVfWPnXcCtxQj = 4.372284E+27; 
double bEFBuliZYFC = -1.413698E+10;
dtZSIxgiOIAwAVfWPnXcCtxQj = bEFBuliZYFC * 2;
for( ; ;) {
Console.WriteLine(-3.484347E+10);
}short nguVVYqHxRyJM = -20851; 
ushort KSw = 45337; 
ulong OQabNofUWTmcCziXKnDVVyx = 58200270922925173; 
int BsEUTkcBCVFwJIWSMhhh = 850368486; 
while(BsEUTkcBCVFwJIWSMhhh == 850368486){
BsEUTkcBCVFwJIWSMhhh = 885933;
}ushort qoakIwbgGc = 53433; 
string utDRHzLeXTNByQIa = "XZQOjmU"; 
ushort nHQmKqeIyzPDqwKiuwnKjFt = 6867; 
ushort RmMkg = 42681; 
ushort kdoKiwxQtjlHHEfFQRHKEJnKRF = 33380; 
float lsgfyjGTKmSnuljTQzmggwobOoC = 2.085195E+07F; 
float VGUgRfCitBLfFPxjye = 4.950114E-37F; 
sbyte KPRdfshIQo =  83; 
long IAesOnpTHxoKmqfeMORAIVfpNajb = 49940093211462435; 
short dNPuctCtTabEdBZioMuPGO = -26573; 
short WZaOwDx = 16742; 
string eKmTNOugiZhHVoODcoDgp = "BlgtCGzzFQdQUAbTIbkH"; 
double ZpdLaSBhkTu = -1.284799E-26; 
if(ZpdLaSBhkTu == 1.14354E-10){
ZpdLaSBhkTu = Math.Ceiling(Math.Tanh(0.1));
Console.ReadKey();
}string MzMBpFcqBTmG = "VlLNtljktnTzXUwotPSohXBZjIiT"; 
ulong oQgbSgYjFmShtjScsLIchewqPYM = 13247230041263999; 
string WcopikFxZEp = "hpzlJO"; 
byte QlTumHTkDJWjPM =  105; 
long NYUQVPuwFwwFe = 89565814207561698; 
string quTqIfqwUQSWQoGUy = "YJlyGyTmuhkLAALx"; 
int byMpmZCOmUTBcH = 6826; 
if(byMpmZCOmUTBcH == 312736){
byMpmZCOmUTBcH = 872464;
}ulong zwLjkiqtkuMjTa = 39281006808239657; 
ushort eEAQDpfKIGZqFdOsyTJAOTOlWmZ = 4615; 
double lAbQeIKCQUncCmbxdqzFuqpUglQm = 6.609121E-16; 
while(lAbQeIKCQUncCmbxdqzFuqpUglQm == -3.415546E+19){
lAbQeIKCQUncCmbxdqzFuqpUglQm = Math.Ceiling(Math.Asin(0.2));
int? sDUjhKTRxIBEmoRUCgot = 3950303;
sDUjhKTRxIBEmoRUCgot += 15287;
}uint geeolnDkYx = 627961; 

}public void tUnnydHJznYDuouTQUKum(){
double ocKkLpLZ = -1.551639E+12; 
while(ocKkLpLZ == 3.338264E-21){
ocKkLpLZ = Math.Sqrt(4);
Console.Write(ocKkLpLZ.ToString());
}uint XjkWoUDDTQJaVCF = 67; 
float pQWV = 1.025514E-09F; 
ushort QXxdKHZJ = 49280; 
ushort qxQGwbDCdbUwNgElHbo = 37297; 
uint KNGyMZuVSRPdAGBkBTeSpjWCKXf = 363012; 
long RdwNdaYpDhhicHK = 62161501955705153; 
int QdNOUZDktXZeSxkD = 530550; 
if(QdNOUZDktXZeSxkD == 13295){
QdNOUZDktXZeSxkD += 878539;
}double qhKWgcsuQmmWkCJiGXKJKCenkYSXZ = -0.02071827; 
double eqGGnGjnqbSDCcjmoQcw = -1.712536E+16;
qhKWgcsuQmmWkCJiGXKJKCenkYSXZ = Math.Floor(eqGGnGjnqbSDCcjmoQcw);
Console.Write(qhKWgcsuQmmWkCJiGXKJKCenkYSXZ.ToString());long UWhkkWSTBUXVJCjRjkDEpOBtpY = 76076299157910654; 
sbyte lSUqHlyNTjNwmVLmeeEfwOVlBCoH =  101; 
uint KwmGsplWeqNAfkbeJhM = 2007; 
ushort ByW = 43077; 
float XQStF = -7.455883E+37F; 
double tWThAOJoQbA = -1.209691E+08; 
while(tWThAOJoQbA == -1.057636E-27){
tWThAOJoQbA = Math.Ceiling(Math.Cos(2));
int? qKkiPFmgXnzqs = 3489274;
qKkiPFmgXnzqs += 15786;
}float obd = -5.176943E+22F; 
byte VstxSlRB =  3; 
long ZeaSOTpeIpDQxxtgJm = 87843441959639000; 
string bSTIIOiPZxmjSLCV = "DWZzeinGDbWD"; 
ulong nSQcQxUhJnEhmZkqRCHTtEbbXKF = 32406518221459703; 
short JMAz = -6933; 
byte VWWnwkjwWVDLROfgSTcWcq =  238; 
ulong ToiPhwZopgaNpHUDQppXoBnDFIFss = 29453230408392942; 
int AEwCIftClhulbGOLtbHjBQHX = 55073009; 
while(AEwCIftClhulbGOLtbHjBQHX == 55073009){
AEwCIftClhulbGOLtbHjBQHX = 96686;
}double jBRuUZGylftjYyDuw = 1.919292E-27; 
jBRuUZGylftjYyDuw = Math.Sqrt(4);
Console.ReadLine();byte zLezIbkeLmoJVwoU =  210; 
int lwTOYcgLiGWppUYXuxUOGaW = 160649; 
if(lwTOYcgLiGWppUYXuxUOGaW == 111778){
lwTOYcgLiGWppUYXuxUOGaW = lwTOYcgLiGWppUYXuxUOGaW + 351238;
}uint uViadFXfxCpbAOXZOXy = 455929625; 
double gkAzGmgatKaDwhUVsypzlpZy = 1.101503E-22; 
gkAzGmgatKaDwhUVsypzlpZy = Math.Pow(5, 2);
Console.ReadLine();double sJAkGfnMPLTt = 3.431005E-16; 
double JLJbTgKVCPXgeRcTxtow = Math.IEEERemainder(3, 4);
sJAkGfnMPLTt = JLJbTgKVCPXgeRcTxtow;
Console.ReadKey();int fUshdcmHpRs = 4881; 
if(fUshdcmHpRs == 700862){
fUshdcmHpRs = 199837;
}double QOLXnA = 8.440617E-33; 
double RsmqLDVdRzgVQMQTmWcN = Math.IEEERemainder(3, 4);
QOLXnA = RsmqLDVdRzgVQMQTmWcN;
try{
int JwYtzO = 5886284;
if(JwYtzO == 90964){
JwYtzO++;
}else{
JwYtzO--;
Console.Write(JwYtzO.ToString());
}
}catch(Exception ex){

}long AhETsdpoNEXexxiIoECGdpwsWE = 52752470175660157; 
ushort eCHpizwgGPMLnkZatfRiMlWPuUWf = 7113; 
sbyte WYmENXlyF =  104; 

}public void NhRXhhIyyM(){
long jSGSVnCobA = 36956324691321438; 
long myzOXyMPy = 31231380609573926; 
sbyte idGmnHbkgBx =  -27; 
short ybMu = 22211; 
float CNStQZDbYWdXQBi = 5.95252E-23F; 
byte WlGobm =  91; 
uint JqqplTHpOTyHzihEsyADouCi = 95225618; 
byte zRfyCMsIFOLXTSTqOCzcyLJQTVCo =  204; 
sbyte nHKSXNZeMQBZEMCKxBj =  -25; 
sbyte izNhpFHkdSxhyqjZ =  107; 
int xMlWMedqlePGNyPykeQyuzB = 66749773; 
if(xMlWMedqlePGNyPykeQyuzB == 288390){
xMlWMedqlePGNyPykeQyuzB = 215080;
}uint FFHNNXCbclmugn = 6728; 
long TYUfgtXkAniJPfMIjbgL = 22202974878819276; 
ushort yULHsCDzymPDznT = 25028; 
uint JzekDa = 34425201; 
short tWlTQaRoXhOjeMETVAcwjtNZw = 27651; 
ushort qLZamLfCa = 60359; 
long xFxotztsIKehoTH = 22828207438569496; 
byte NWtHbSZ =  193; 
int GWyGWDbFIHSMtefsOzRSSHkgHDZMG = 96208481; 
while(GWyGWDbFIHSMtefsOzRSSHkgHDZMG == 96208481){
GWyGWDbFIHSMtefsOzRSSHkgHDZMG = GWyGWDbFIHSMtefsOzRSSHkgHDZMG + 29304;
}string HCsEZpFpbEoCKdCWNgYMpZ = "DpZQCybzcbHhW"; 
uint SqGRFYLkWN = 464863; 
double cqwIgQnZAdSaemAcWFyAnHgA = -1.000398E-07; 
double KZLkBPJ = -1.163812E+36;
KZLkBPJ = Math.Sqrt(3);
cqwIgQnZAdSaemAcWFyAnHgA = KZLkBPJ;
try{
int YFUyLTQYbyBwjWmMYi = 2948594;
if(YFUyLTQYbyBwjWmMYi == 25474){
YFUyLTQYbyBwjWmMYi++;
}else{
YFUyLTQYbyBwjWmMYi--;
}
}catch(Exception ex){

}string QWxyfKzkWcuimXfHFwyjEP = "ieUmlJnCjMcKAthwfdiyhSR"; 
long cXVUQUgbjmskxlCJBkykn = 36321381433612478; 
int niDiLsItRVJ = 1122; 
if(niDiLsItRVJ == 687030){
niDiLsItRVJ += 824566;
}long IIuBxxKzUAONJFWecTcRqSR = 57502956231246887; 
short FXJTIfNbUOUdiZLjMhQORyQ = 20944; 
ulong SeCRdSzZe = 5802004275762567; 
string CfGGYVJRCWghSexnIAcLbGjVdVBN = "ybaM"; 
float flzEVuaWc = 1.814404F; 
ushort XAIout = 60358; 
double KDcuWPdVgwYSpbjWgpaJhXEAwPD = 7.092558E+17; 
while(KDcuWPdVgwYSpbjWgpaJhXEAwPD == -3.140064E+33){
KDcuWPdVgwYSpbjWgpaJhXEAwPD = Math.Ceiling(Math.Sin(2));
object AmxHSPQa;
AmxHSPQa = -2.597277E-36;
}string PXXBsZFYoawMBY = "WHkgxOGB"; 
uint YhWQiPZzLLpAfBPlbSzIuIggxM = 90; 

}
} public class IiyxItSIiflwnQmVyCL{public void kMObTzqaQtyL(){
string dCTcygWETMllgRTkYjtEwVwwEj = "RSPJ"; 
uint wNMbwdEwHSc = 642281330; 
short hFumqCyhgzAndeMd = -29035; 
long MegiEgQhPcsGVSEKmwwkQYFGEWP = 79232026042310671; 
ushort wRf = 34493; 
ushort eOSExBwzJHAfOACFIqpR = 57484; 
short AjmiJRFpJxpB = 22147; 
long tYVxfsGHRVAQ = 27596982433836217; 
ushort sNHeqQJ = 30780; 
double UoYqWyiaTDwmjawCzUBKy = -1.984996E+35; 
if(UoYqWyiaTDwmjawCzUBKy == 1.731421E-27){
UoYqWyiaTDwmjawCzUBKy = Math.Pow(double.MinValue, double.MaxValue);
try{

}catch(Exception ex){

}
}byte NiCieOCOnjJjshiAPQHuI =  179; 
float XKuwBCKySMPxHjfJJHtqM = -3.606961E+13F; 
string jMEPTccoSkwtfnNBlsGtN = "FLYMasCXEatMpPfIYKMhOYKoD"; 
short RBbdJXsgTRhbLNVIwau = 30707; 
double EtWHFdHaJjILL = -485.0895; 
double RIbuSqnohuJwWJhbsnBThXtZb = 8.059631E+12;
EtWHFdHaJjILL = Math.Round(RIbuSqnohuJwWJhbsnBThXtZb ,1,MidpointRounding.ToEven);
Console.ReadLine();ushort lQaDuumjgTxgd = 13757; 
sbyte WfXMYkKGiqfPdIZDRRcWtAUI =  -114; 
short VloGXmUXieHxbfutcnyfzUT = -13070; 
byte jwykoJH =  116; 
ulong TLiDiLQzkwbRZ = 22526245848468615; 
double PcABXPYPEqZAs = -1.990517E+30; 
double txquWdwi = -7.063704E-32;
PcABXPYPEqZAs = txquWdwi * 2;
try{
int tBKkSJlGzBZGA = 5720300;
if(tBKkSJlGzBZGA == 87185){
tBKkSJlGzBZGA++;
}else{
tBKkSJlGzBZGA--;
Console.Write(tBKkSJlGzBZGA.ToString());
}
}catch(Exception ex){

}float uEYHaMunDkMTk = 1.247407E+28F; 
ushort UIshIQeYOKSHboQqldWbZz = 34609; 
short YMkbqbPHzJkClNNUImcDICWWCZFWw = 14082; 
ushort biJdTX = 28843; 
long aDGF = 83349294716348213; 
ulong yQnBsxTkNmeB = 26626780644116798; 
long qsLYnNntjImQknFxeliPQsApXmAg = 23418406460316628; 
ulong ifWucymmTxwn = 74652260956594633; 
string mhL = "DYfLPJHMddSwoccLtdLtLXY"; 
float BdtqhSPJoUng = -4.79286E+10F; 
ulong ZdmaNJQsqIjysdFantnG = 54355116933754339; 
ulong lxYLMULRFRnbaBJsql = 29007676233178005; 
sbyte XaII =  19; 
ulong OzHqKoxOyqltwD = 48034867651668447; 

}public void pgNJUFjHyneUTKUGZxaWPilMB(){
byte SPdbYLHpjXGCimLPeq =  165; 
float VlCoygchVaiJaqRtmfjltK = 3.537182E-11F; 
float JduguSuTXDDMBEFinXyTiWUwR = -0.001120267F; 
int kFhuHzYNPGpo = 37; 
while(kFhuHzYNPGpo == 37){
kFhuHzYNPGpo = kFhuHzYNPGpo + 601528;
}int kwLVbJMjYojuOBNYusUf = 41728272; 
while(kwLVbJMjYojuOBNYusUf == 41728272){
kwLVbJMjYojuOBNYusUf += 580093;
}string HEpGSXgPTlsYNPOAxW = "HHoyiLBbPWuPyIhD"; 
ushort ECQtXehaWtRIZUkdtXqghVsWsTAyU = 20204; 
int tCTuKmJkp = 77; 
while(tCTuKmJkp == 77){
tCTuKmJkp += 925491;
}uint uokkoQJjUbeFKfHOFn = 19927; 
float fogRagjNkXZTB = -4.422833E-10F; 
string LkjQl = "GGbhtFTyzNaiWezQCZ"; 
sbyte hFQGFuEjOQHETLCkZYRhqACgA =  89; 
ushort HVJkIwxaPwDYaKKIgxolEmREiZBI = 4633; 
long NXHDTh = 39715848460760859; 
float HQTadE = -5.148391E-10F; 
ushort ebJVO = 61900; 
sbyte QbuhQBKBQXc =  80; 
byte pSTSkJ =  13; 
byte miOcumEBEqnGRNAZtpDNnq =  144; 
float MQbWNxmHogYcypnmyAAZDAeuSRtB = -2.888099E-05F; 
double XfIxCERUbYCHBXWanAR = 2.054761E-33; 
XfIxCERUbYCHBXWanAR = Math.Pow(double.NegativeInfinity, 2);
Console.ReadKey();float NPKOnbMbqaPiJ = 1.659073E-34F; 
byte gyfpWfODzqoUXfpWEwKjdalchI =  177; 
long DiPM = 72172829870889997; 
byte LyhtVAmeBtcOgkdiowdAVhBh =  43; 
double kBGaoTjCFanPlQyIwJa = 0.122363; 
double EXZkVTcEVtOVWJziaHRKyAeAiag = Math.IEEERemainder(3, 4);
kBGaoTjCFanPlQyIwJa = EXZkVTcEVtOVWJziaHRKyAeAiag;
uint ENbFdWqqVEgxfSUPBbXInWUyEqy = 39; 
int TDgTqRVYYwjbnzUJOR = 63; 
if(TDgTqRVYYwjbnzUJOR == 820590){
TDgTqRVYYwjbnzUJOR = TDgTqRVYYwjbnzUJOR + 644629;
}int FaNAlXsanUYYDYGcZleUHYJIJ = 3739; 
if(FaNAlXsanUYYDYGcZleUHYJIJ == 58520){
FaNAlXsanUYYDYGcZleUHYJIJ = 729801;
}uint AlUlzilKn = 164; 
long obbTRFEMkGseRXEwukhAMcML = 71932226727303905; 
double ZqUNlhWBnHDgIlJoTaTkclpqcp = 3.769219E+15; 
while(ZqUNlhWBnHDgIlJoTaTkclpqcp == 3.489528E+27){
ZqUNlhWBnHDgIlJoTaTkclpqcp = Math.Sqrt(4);

}ulong mKWfZoR = 43787436028974029; 
double DblPoZzoDfSTJpeQyfxY = -1.536795E-27; 
DblPoZzoDfSTJpeQyfxY = Math.Ceiling(Math.Cosh(5));
int[] weTkKuaFGQhBGL = { 6902581 , 20322 } ;
Random peiNByLNtdXzHScsBt= new Random();
Console.WriteLine(weTkKuaFGQhBGL[peiNByLNtdXzHScsBt.Next(0,2)]);uint impylRtRabxoCPOyzWL = 4932; 

}public void MWuNEz(){
long WBdKWV = 89698563975733352; 
string qmEcVOBfgaC = "dzmUjUfWlRTTa"; 
ushort RYXfQUdxqUFliZXEVudZMG = 31888; 
short QeeAtLb = -343; 
string hiVLzZcWZGJXWnmYAXwYk = "GXaQYWYjEMfqQuZJ"; 
int KKA = 49272008; 
if(KKA == 829850){
KKA += 235507;
}ushort eddyIEYLJ = 1967; 
sbyte QbXRGyTYRswoCZTyhIgKEYAIbThR =  -121; 
double YENJLDaDttywMJPWJKCEbUwuSDo = 1.425831E+18; 
if(YENJLDaDttywMJPWJKCEbUwuSDo == -2.43934E+33){
YENJLDaDttywMJPWJKCEbUwuSDo = Math.Ceiling(Math.Cosh(5));
try{
Console.WriteLine(YENJLDaDttywMJPWJKCEbUwuSDo.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}string WoJZGbzNhQhFUYYXdeDs = "HaoydcMSDLTCZBHzfImqDxZIpDBF"; 
ushort sWJKonuCDYNUGeo = 4322; 
float qzYyoaukbIgCCgx = -2.024251E-10F; 
double okteEfoFIRVtmIcnXBUIIpemMu = -2.975103E-05; 
while(okteEfoFIRVtmIcnXBUIIpemMu == -79.38838){
double mXuusaxMtfoUUzTADzok = 3.114604E-12;
okteEfoFIRVtmIcnXBUIIpemMu = Math.Round(mXuusaxMtfoUUzTADzok);

}ulong ZldZHhSckmhzCeWnTSJDWxWIawuZU = 65414438576828095; 
string IzBVoANcHkiHn = "jbEpwZlNSh"; 
uint nmpPYqQyae = 510748; 
int msOPMcK = 1916630; 
while(msOPMcK == 1916630){
msOPMcK = msOPMcK + 530023;
}string VGtuakB = "SlSRRnfGcKmMTZdwAGJlfcq"; 
ulong nfzcgGXNGWDTlTqVFKXnqoW = 45917088894677497; 
float cDiI = 3.776748E+31F; 
string YndBDnNqqCs = "ByYiWflkwAYgJ"; 
float pimbOUY = -5.970864E-38F; 
float wHJ = -1.207876E+34F; 
sbyte CCGEZLyKSUApXS =  55; 
sbyte idBFACeYpVKFDj =  8; 
float goDMiDEdbeSLWiKClbUGS = -3.90326E+37F; 
uint JPOfpKYymLxDKZBWCyy = 2553; 
ushort adphImRepbgBOQYMuUmJynY = 35237; 
string JqFCnsnbNcZC = "kwHgCEANgDM"; 
string aMpNeOfoFBBOO = "lMcVMSEbmRgyYEHCOTzUsDM"; 
string kxbHVfsNPmadIakQYzAGHQCIcNIoT = "KacXBwDwsLPKjq"; 
int gSnetdluqwUtEhz = 87; 
while(gSnetdluqwUtEhz == 87){
gSnetdluqwUtEhz = 25519;
}long PnEgORWHkbFHPbPc = 75080224490327972; 
byte LQQOHxxtz =  208; 
ulong zEwPKqYBKbepWktduneMWCffQywi = 76822885196259628; 

}public void qSRSKDJM(){
uint wdtZjLdaWzoYlWQqnzedmVoSd = 269517; 
float ZxtpGZfQx = -2.403553E-19F; 
sbyte uHVsGUcsojHyFBOBWOykO =  112; 
string JGXLBRfRUEBIWLRtRUKlNSbzqoB = "QYVyFaJZXqSyeQDbZgF"; 
byte dtUmPiQFzukqCbWIddWbRxBbmhXp =  251; 
uint LpINGbjP = 869695235; 
ushort XIgDOHuWdIifHTS = 65478; 
long BGajAbTnp = 48470268704455691; 
ushort FiJFDg = 4324; 
sbyte fNjjhGtOzOhfJApOc =  75; 
int JJp = 876494; 
while(JJp == 876494){
JJp += 12481;
}uint xUsi = 424265; 
long AQJBPm = 73022402655925449; 
sbyte xxMqeCCOaMMkqqNstKckahCkSg =  -26; 
uint YLVNAMhK = 232; 
double UBcBQFsGzg = 0.00120763; 
UBcBQFsGzg = Math.Ceiling(Math.Asin(0.2));
Console.WriteLine(UBcBQFsGzg.ToString());int MPjxZLQpS = 737285711; 
while(MPjxZLQpS == 737285711){
MPjxZLQpS += 909658;
}float szIOazndqKDGIqYwgnXyZRKG = 2.375586E-13F; 
double VpRopDgIWALpWtJ = -2.07438E-19; 
VpRopDgIWALpWtJ = Math.Sqrt(4);
for( ; ;) {
Console.WriteLine(-4.823379E+09);
}ushort FyFhDSuwBCtK = 1313; 
double depZcDmNiD = -850815.9; 
if(depZcDmNiD == -3.141504E+37){
double QZGgwAzPSFsmCmRREnXqQsK = 6.120309E-36;
QZGgwAzPSFsmCmRREnXqQsK = Math.Sqrt(3);
depZcDmNiD = QZGgwAzPSFsmCmRREnXqQsK;

}sbyte tHuzNcXCEogVjGfJl =  -123; 
short ZmuDmAddDQNKlHfPVHWU = 1425; 
ulong mksVdkXQCWzDglRkmYTY = 5119150594391898; 
int xgmGh = 9075; 
if(xgmGh == 861591){
xgmGh = xgmGh + 591822;
}string NOdZEdwK = "IOULOJsEVjkyBqpBdApIYauNqLV"; 
uint MKjwBAzMyElmxXwPO = 4382; 
ulong AFfbWHuhtdOSciA = 70177174532517425; 
string DcZXEzzMdmfdu = "SdEdBzwuOVOMlFVJSJ"; 
string uFwjPFogZWuXIxZPkDu = "ZindjSlTQuuxcNjnIu"; 
short jnwSBQeQFgzpnyHlpqaWsxpFCYV = 25411; 
byte gRnzwMNxDtOxnKVehFgXYp =  95; 
short OYQqDVjETaHcQbVzTO = 9441; 
sbyte hGJyQpNoOQyVVINWfazESwKncsEl =  -7; 
string MjHGsgTNFxoejjgVu = "COOiFeM"; 

}public void uuzNCQKnMMnbwQlmThDqAeVxidWp(){
ulong FDUwMznRVzwWzwHsN = 33120749721114250; 
short bFyTuBigMgZdNZc = -17627; 
string EHBQQqkNmmhaAZxZlmzGaxonVlOJ = "CmkyzTJYlOfJHnWZMIaHNNPO"; 
uint hsmVFECXLZWDbfNbaAsWuhtniFN = 7; 
ulong yZEMqLqiDgKDWdi = 7011398516852490; 
uint YZMaQAUWCzEzuxqJpQQLGxkaYTisb = 3; 
ushort MsMG = 11161; 
ulong QGuFFhBytdaHMRtDPLcX = 51997793980351055; 
string dgdhUZcGwQtLYYguLkt = "XUgPqnLahzMSuOaUDEWtBipmE"; 
int WncwkomIianRHobzX = 51; 
while(WncwkomIianRHobzX == 51){
WncwkomIianRHobzX += 243572;
}float oZpOFU = -4.144158E+16F; 
long MedCEgfRTnNpezLtidnsYbkn = 86449083892681632; 
float ptTDXkeftEuZUScl = 6.702197E-28F; 
ushort QLUQGnklmNkH = 26033; 
int sAHfdmS = 64489805; 
if(sAHfdmS == 401262){
sAHfdmS = sAHfdmS + 76955;
}float wVJCKFzdWJZT = 1.580117E+09F; 
short FoKLMdbtfXcmXKTguUqoXTV = 4191; 
float KyduZboeqftgHs = 8.621146E-17F; 
ushort ClgmQTzYocQMtgSpuFpLYa = 62040; 
float ONnnZwJxjbHo = -1.89136E+09F; 
byte aNlnGMmopYpZLOBonVcgkLpIhoj =  27; 
byte WXf =  203; 
double LENbBZYbfK = 1.730392E-06; 
while(LENbBZYbfK == -3.03589E-38){
LENbBZYbfK = Math.Pow(double.NegativeInfinity, 2);
Console.ReadLine();
}uint ocnOJNhqf = 1419591; 
ulong FcIdQKLQMxtPXZySod = 32607551050169023; 
double kdEGjolfOwybPyEGUZLN = 0.02394544; 
kdEGjolfOwybPyEGUZLN = Math.Ceiling(Math.Acos(0));
int? aoolXMmWmyWTFaGHqMSWOgucasuX = 7512234;
aoolXMmWmyWTFaGHqMSWOgucasuX += 50644;uint gTuVBhMUMdmZnCW = 15862055; 
float llFqoWBYqfamffTFYCSHlpfHqKb = -1.778167E+32F; 
long xUG = 89423746269619177; 
ushort MQhTwXXjpmJVgYaFVcbIeEt = 32934; 
ulong cywTHbVqUBDhdZMSgHWt = 36371039034585767; 
long xIcNUBO = 67506992679550364; 
ulong cVmyMEyDAVUPJUhIMdVepfleKbpto = 75614029699428606; 
long dGkwFTIXNThsxgJJWJhOL = 8673277536939688; 
ulong jatRzfLIkb = 39094708200945944; 

}
} public class iioXZRCXwKz{public void mXFXqcp(){
byte lbXMHKFfSocSuGfVdu =  1; 
double pXNURGV = -7.634508E+23; 
while(pXNURGV == -2.692308E-05){
pXNURGV = Math.Pow(5, 2);
try{
Console.WriteLine(pXNURGV.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}uint xVjIBzxHwnfjBRnRQmP = 899107; 
byte qKVwxBRLu =  61; 
double JPHWJflsVkQDqgLlbgbMjJTXFdZ = -1.247422E+07; 
while(JPHWJflsVkQDqgLlbgbMjJTXFdZ == 2.225436E+33){
JPHWJflsVkQDqgLlbgbMjJTXFdZ = Math.Ceiling(Math.Atan(-5));
try{
int epPFKnlkMaVJqF = 5332442;
if(epPFKnlkMaVJqF == 65962){
epPFKnlkMaVJqF++;
}else{
epPFKnlkMaVJqF--;
Console.Write(epPFKnlkMaVJqF.ToString());
}
}catch(Exception ex){

}
}string aIdpsNelZKqaTyLtjFgBZ = "mEgYGZuJOgCZmEhwI"; 
sbyte eFxONAchFlWCCKzuAj =  -34; 
sbyte SqVjz =  92; 
byte lOXYWV =  242; 
float uAESQyRGZoPjIlobBmnBEnGS = -1.403268E-11F; 
uint EGPBELQTEmbTdFzEwXFCIWxJKpkfT = 82327092; 
uint RcuBnHXxwynkuyZPcydOWsHigRmAK = 866694101; 
ushort UlDecgqIfmziyoTlpR = 17293; 
ulong dAwkuytyMwFSIBkyLZpMg = 35307986257944202; 
ushort GIURMhT = 44083; 
long juRUpQPIPFRXbDmwmkLUYjqlZjNJz = 17144282031973342; 
byte UNNAPsIHqEqUIYPaacGVg =  11; 
sbyte ANBpnHR =  -89; 
double RhkLCUqExfhwjsSN = 4.950232E-11; 
while(RhkLCUqExfhwjsSN == -74734.84){
RhkLCUqExfhwjsSN = Math.Ceiling(Math.Sinh(-5));
Console.ReadLine();
}double SfPZtfsLfpkSccRbQnjIpaz = -1.79693E+36; 
SfPZtfsLfpkSccRbQnjIpaz = Math.Pow(double.NegativeInfinity, 2);
double dDGemxWRbsYIq = -2.458509E+33; 
while(dDGemxWRbsYIq == 1.986745E+21){
dDGemxWRbsYIq = Math.Truncate(dDGemxWRbsYIq);
object FcyRpVVBzxfGA;
FcyRpVVBzxfGA = 4.771023E-06;
Console.WriteLine(FcyRpVVBzxfGA.ToString().ToLower());
}short oYcztPi = -15949; 
double MLeT = -5.768408E-31; 
while(MLeT == -71.84078){
MLeT = Math.Pow(double.NegativeInfinity, 2);

}string syNoiGmCgcBqLHPhoqieDAZHtC = "uhIauy"; 
ulong BDj = 23624911521380728; 
double heMpwWoTygVIBTt = -1.040801E-37; 
if(heMpwWoTygVIBTt == 3.042334E+31){
heMpwWoTygVIBTt = Math.Ceiling(Math.Atan(-5));
int? dVLZBAhcwoW = 4661053;
dVLZBAhcwoW += 21222;
}byte RPqDIPJQDPtFohpsdeYtBSeQ =  7; 
int oZGuJpQLU = 873624358; 
while(oZGuJpQLU == 873624358){
oZGuJpQLU += 963022;
}uint nKoi = 1605074; 
short mQoYKCCcCtQX = -6363; 
long zywDjLJZDs = 59741190362564783; 
string ACMkosqOZhEIDw = "MwyiIERNHJZjNzURKuoQWLkmS"; 
ushort ZwPtZWAsdCetSJPfDphFoeqUnnF = 58045; 
string kpoIRqSo = "nmfRoFaxhhGJXKPGUdUfMqGpTqOdj"; 
float mmpUsinbpmRyMx = 3.870495E-21F; 

}public void BbWnXjnXIQYYQXlpLswI(){
ulong xHbFSUTTUqehEh = 64570913085695690; 
short ApFh = -32238; 
string FRcGol = "sudOVHUJtewD"; 
string HSNdqYeCpMTQ = "ApBDGXWM"; 
long WzJjRXBNzbOYZpDYmnQeqVexus = 32035777756500496; 
float IPnFPZwPxCqkaHKJVED = -4.487436E-10F; 
byte zeZXTWiZaAXqzakosApZTbIB =  41; 
float KwwQkuZPAQbCSn = 2.27226E-16F; 
long okJL = 16041333989402013; 
uint YAzEFJMXAkMApYyMqLWyTKNZ = 22; 
short gcqcqmE = -9221; 
ulong LcqsMyYPUDGCFMCbVoNzxtuFRyUK = 76556675484429406; 
sbyte YobtqMCFzGBpqoGsD =  -43; 
long BnEoHShCHkMIVupyxccIcmY = 12455708469183924; 
long taKLMMzF = 79052113140400362; 
sbyte AnaKnpogQgccctcxCuHMLqFdcoM =  -25; 
short OhoJsukJMuxGWs = 2717; 
double TRyTEsyjiKXIZEURtsUOtbIlTIm = -3.891502E-32; 
while(TRyTEsyjiKXIZEURtsUOtbIlTIm == -68.14512){
TRyTEsyjiKXIZEURtsUOtbIlTIm = Math.Exp(2);
Console.WriteLine(TRyTEsyjiKXIZEURtsUOtbIlTIm.ToString());
}ushort WkDGQLUsmSulCxLgtQsECeuq = 43284; 
sbyte ktw =  -50; 
string EAboTpuqxAa = "cfEBfO"; 
byte QnRRkXHJKYR =  136; 
short XWJSBAYleGujHdyKnIfMBpJkeVK = 13604; 
int IzPghISnmLsEilx = 33; 
if(IzPghISnmLsEilx == 5967){
IzPghISnmLsEilx += 713295;
}sbyte HuImzNGQnGHXWw =  -5; 
double SZuwuWgyOdEgAHNzchMBuuBI = 9.743618E-33; 
SZuwuWgyOdEgAHNzchMBuuBI = Math.Exp(2);
Console.ReadLine();long HtflZLlcAWAWHwmpiKCSORqz = 85088471605437306; 
float IdHneNPHDZPd = 2.00485E-16F; 
int cGyRbPcHRhX = 948806; 
while(cGyRbPcHRhX == 948806){
cGyRbPcHRhX = cGyRbPcHRhX + 21264;
}byte iKVGOdgnqKqUzeOmuDesSbZ =  112; 
double kKtcncKCuIgZJOUCLXAnAziW = 1.324039E+34; 
kKtcncKCuIgZJOUCLXAnAziW = Math.Ceiling(Math.Asin(0.2));
Console.ReadKey();string UBMFPXyGtlAoawphoeQBi = "xcIlqUucXkyVXgQxShzT"; 
ushort JgpcSoPdGBCldJXDHEkpooSfyi = 22544; 
float IbpmEuBWn = -1.809196E-31F; 
byte GWtMcgAoCVIeEtojGJXXRXAlZA =  68; 

}public void HsCfcwJVMMgktW(){
sbyte Ihaal =  57; 
long hpbJxhMDTcJj = 27976767145499831; 
int FURuERAmkVJhYMtazFZEqpn = 713485593; 
while(FURuERAmkVJhYMtazFZEqpn == 713485593){
FURuERAmkVJhYMtazFZEqpn += 310152;
}float PQwMhRcEUd = 5.69743E+26F; 
ulong lDpOKUTUZwnHUJyM = 34648217091891870; 
byte xaeAMyB =  108; 
uint QdHYuURZzBXtgixJCf = 89771773; 
double CBzSPAKRfzj = 1.234142E+32; 
while(CBzSPAKRfzj == -5.115032E-12){
CBzSPAKRfzj = Math.Ceiling(Math.Sin(2));
int? zimmSsiTIuxOQnw = 1452655;
zimmSsiTIuxOQnw += 71196;
}long fFlVJMLQDWQRgqiNkiTBBCojk = 11219753694081778; 
short fImFjFXqMTuExtuZYRksFo = -29747; 
long EDpgOfBOFcfXFKVDjEqRtL = 28296850571747565; 
ulong DSGcIkXNTPRpobuhoPu = 3151095261394988; 
int uNylBwMe = 912738091; 
if(uNylBwMe == 568252){
uNylBwMe = 942664;
}int AQwPMRlNeUIzNoxCtCAcuePHId = 879762; 
while(AQwPMRlNeUIzNoxCtCAcuePHId == 879762){
AQwPMRlNeUIzNoxCtCAcuePHId += 947353;
}ulong numPAECLhGNmqEzymbyCAwbPhJwsq = 3029097471777303; 
int VBJW = 86; 
while(VBJW == 86){
VBJW = VBJW + 404551;
}double gAnATtzHQhjthUbECDxjJYDaQz = 1.158483E+32; 
gAnATtzHQhjthUbECDxjJYDaQz = Math.Pow(5, 2);
try{
Console.WriteLine(gAnATtzHQhjthUbECDxjJYDaQz.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}double luzwhEiMWzXhqnueQCbGHEfJtGFk = 28785.67; 
if(luzwhEiMWzXhqnueQCbGHEfJtGFk == -2.994899E+19){
double cgEzbmQgkRQVIkAG = -9.60572E+16;
luzwhEiMWzXhqnueQCbGHEfJtGFk = Math.Round(cgEzbmQgkRQVIkAG);
int? QcDZXPtwIuZD = 1500252;
QcDZXPtwIuZD += 77716;
}ulong osEJubcWJZE = 41562186400241894; 
byte bydikLsYnVOkIAbRYZYBmY =  195; 
float HqyVdqWCFnUABjpKYueQuTOREB = 2.514372E+28F; 
ulong cKloFuVckERKclh = 74862604621122179; 
string OEGsDBdOgkkmkGWP = "jBYKQDKlcakXiAakQQQoKIcOWWa"; 
double mkMbu = -5.612067E-26; 
if(mkMbu == 1.930488E+35){
mkMbu = Math.Pow(5, 2);
Console.Write(mkMbu.ToString());
}sbyte tSuFqfqaOokEyRDlOLsgzApxCuQCs =  -85; 
byte UMsBAG =  66; 
short tibUdYeOxhCJYBUDYd = 27746; 
short IFcdKMAbNiyeDeqoCyxMOWbM = -1435; 
float yYBCqZPM = 1.404284E+36F; 
ulong lGMJZdEwxEnxG = 19189831978487811; 
uint dlH = 10; 
ulong NoLcjgbncIUA = 27596234070333296; 
string JkGMFJxsYAIdQYLDoyZxCIKoMgthL = "JaqOHAyzDs"; 
float KwRLeLtsDYOFzaBpayonI = 1.172514E+23F; 
long bskfRVHFSYoyabJPsLLlNp = 52775786430312894; 

}public void GkY(){
string hmMdhZHclSax = "zoZfwZMMlYtQEbPXJmbY"; 
short fZHeNKDNoyJxfNbBPZqH = 26978; 
short mFYReEFYyqRj = 32247; 
uint lcjAKVhgVbW = 46; 
string KMRHtBPSziKzkfnEbczN = "xwExfycPhw"; 
sbyte QIesseQyjMpayXbIzVgZosmpmknc =  60; 
string ODCuyikJXJp = "LfEZh"; 
ushort qmohIzwPfhRNL = 8333; 
ushort zVEEzmliqZnCkKkfI = 22281; 
ulong AuUW = 62635697345737353; 
short eeFSWeafRJjTEwXLft = -7064; 
short ebIB = -27758; 
sbyte cxuplHFgCQwIduDkMjRKEw =  71; 
byte XCacXqSXocwoUIWRQi =  80; 
double oICQVqmxhIHhMnfFdFADKPs = -9.45202E+32; 
double oRJTzmbZFCgwOhhu = 5.511712E+19;
oICQVqmxhIHhMnfFdFADKPs = Math.Round(oRJTzmbZFCgwOhhu ,1,MidpointRounding.ToEven);
 bool? RucBpuTeQtDuwiSVFlf = new bool?();
RucBpuTeQtDuwiSVFlf = true;int mPFgWbqxUS = 407; 
while(mPFgWbqxUS == 407){
mPFgWbqxUS = 294476;
}double OCYgwKcxGqMkV = 1.654937E+19; 
while(OCYgwKcxGqMkV == -1.250066E-13){
OCYgwKcxGqMkV = Math.Pow(2, 2.1);
 bool? XxoFYqQwpimBmeAahdQPhD = new bool?();
XxoFYqQwpimBmeAahdQPhD = true;
}ushort stjWC = 26225; 
long wItlZMZYNcLNnyBIICA = 24449148919739201; 
ulong RJYcg = 51980392531762369; 
string EJQUSDnaVcNcjLbHAiUWtNKg = "FRMTiEa"; 
short eCDsXCDJdaumLfTaSVYg = -4426; 
float KMnLDgNzMNDGBegktuLJJMVTMSTqJ = -1.885181E-25F; 
byte DTGKQaDqCI =  127; 
sbyte tWLLGNMhoXQuZKnw =  78; 
long qsYlCBDuhshdwKlAWH = 54897719366894380; 
short poVMYWShwLijcsEQhTuKNjqPS = -16359; 
sbyte lLpMkMdTywfAReekyOudWoecw =  25; 
string XfTLPb = "KwyYpoKtNNVTJefsdSdVXHmqGJW"; 
double CuKQiwQZQdO = -6.971165E+32; 
while(CuKQiwQZQdO == -0.05236851){
CuKQiwQZQdO = Math.Pow(double.MinValue, double.MaxValue);
int[] ToVdEMCYJkqISyxxyUgWnJUqC = { 8273455 , 25054 } ;
Random DdxyYPjoAtyLzA= new Random();
Console.WriteLine(ToVdEMCYJkqISyxxyUgWnJUqC[DdxyYPjoAtyLzA.Next(0,2)]);
}ushort tJBQsTkfhbE = 12695; 
ushort DjizCklCyEYdYNxIea = 19406; 
int TUNMEmSIhLoYUioptLicRkXRoUzKA = 560498536; 
if(TUNMEmSIhLoYUioptLicRkXRoUzKA == 596375){
TUNMEmSIhLoYUioptLicRkXRoUzKA = TUNMEmSIhLoYUioptLicRkXRoUzKA + 669720;
}short NYYuToSKeeEhqM = -13189; 
sbyte gJWVxJOhhlhuioAZU =  29; 

}public void jJXVfhjQUDiknOnzatBpokm(){
long KoVP = 40584116285748446; 
ulong OmTTGYaUHVRAGtpyZwlSxSnbQ = 18454768058694353; 
uint dxqINcPJZYFDcQeXyXVwVYS = 22; 
byte jPCDldHmbIOAPHjnckRKnHTWqsgS =  23; 
int xBfFTzwsAx = 61197; 
while(xBfFTzwsAx == 61197){
xBfFTzwsAx = xBfFTzwsAx + 121659;
}ulong KPSuDIEVFlUWPhytsYJbpQ = 40120050548354971; 
float tguQxcYzb = 7.378904E+28F; 
sbyte DHuGXqByMwUgZbn =  110; 
string stJEJOIWpaka = "WzuPHqXHWsOH"; 
long JyaKCbHCQPZYHYaa = 27639087327772609; 
long dLwbVWXmYzppJYMwJGQ = 11599817725243285; 
long aTOcVYVfPUcbuZCLjE = 78466302621038716; 
sbyte Bitbo =  49; 
double DmOaQoULOHpYdjIXhpa = -6.194089E+29; 
while(DmOaQoULOHpYdjIXhpa == -7.090796E+37){
DmOaQoULOHpYdjIXhpa = Math.Ceiling(Math.Atan(-5));
try{
int KXLkiOhaJojOUGXhbLhwPMVGKNtNO = 5950749;
if(KXLkiOhaJojOUGXhbLhwPMVGKNtNO == 82943){
KXLkiOhaJojOUGXhbLhwPMVGKNtNO++;
}else{
KXLkiOhaJojOUGXhbLhwPMVGKNtNO--;
Console.Write(KXLkiOhaJojOUGXhbLhwPMVGKNtNO.ToString());
}
}catch(Exception ex){

}
}string yaiToQnAjXcfCxuumtjeIgGTWd = "fQohdCPotFUanRGEcafytIknkHZ"; 
byte uoXkVhKUQZE =  183; 
sbyte Qst =  110; 
byte DESQ =  159; 
int GbWyCEYZoXPtJWJ = 342; 
if(GbWyCEYZoXPtJWJ == 236286){
GbWyCEYZoXPtJWJ = GbWyCEYZoXPtJWJ + 626843;
}ushort SKMjOQygFsKeTUDbhhbCuFEFfTq = 53785; 
short qRkfeNQukiRRjaYSQsABEeFTCnf = -23483; 
string zHSKbTTOQPlOWTKEuDGmdZqOqlm = "EQHJpqKkYBZf"; 
double DKnQ = -2.00176E+17; 
if(DKnQ == 7.33967E+28){
double AoetPGCjZiEzGiAFUDBnbyHAC = 8.071698;
DKnQ = AoetPGCjZiEzGiAFUDBnbyHAC * 2;
Console.Write(DKnQ.ToString());
}double YELNsa = 6.783711E-26; 
YELNsa = Math.Sqrt(4);
for( ; ;) {
Console.WriteLine(-1.129202E-06);
}long gjdaKBIqmkYxuY = 1145064259357794; 
double OVnRcOBMuHiUqPHVQCuCVXn = 1078.654; 
if(OVnRcOBMuHiUqPHVQCuCVXn == 1207.626){
OVnRcOBMuHiUqPHVQCuCVXn = Math.Ceiling(Math.Tan(1));
try{
Console.WriteLine(OVnRcOBMuHiUqPHVQCuCVXn.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}int UhVfCIF = 5687; 
if(UhVfCIF == 940748){
UhVfCIF = 496300;
}sbyte fyYJeRtKwyNebpUwMfsDZ =  33; 
ushort GhzZiMOjtHEDauDqCzAJPDE = 4034; 
long URhDLAbj = 89581519244500910; 
long NsusDbaCgNZdE = 88687310723595591; 
string RqGYhWQZlSjpqQCn = "DjsokZ"; 
ushort cDRNCUzJREbXcSFqNOkufZhqIWnM = 48433; 
byte smbfuYlKYLIAmBqdKGa =  136; 
ushort YRcwBeFGMWGXKLaNksBdmJz = 32610; 

}
} public class QqTFfLKjEnkcYzhfsYLUpQHcWwJ{public void tCp(){
ushort BKOMaHkVOgxQLlTtXU = 45717; 
ulong FaLQJLcJQbYRAzOutss = 51283525918885614; 
ulong xHGHXVKcWsVimYezZkNf = 6073639673856804; 
uint xCmbuwtjCDeqyZysMF = 72061824; 
byte xKJ =  211; 
string uqTuDsVtVk = "JfPKjaIhDUVUzKPpxf"; 
int DqfQkKKPeedx = 2932; 
if(DqfQkKKPeedx == 475806){
DqfQkKKPeedx = 329321;
}byte aKBtAxuLNcznsSbsZYzBq =  220; 
string RztzNiRfTTsTQMq = "pZbcGPdJYFGBSMshHcCacWXE"; 
byte uAAhCnIKjqRaS =  8; 
ushort AhDqmhBkmNkpMtbzSluFj = 54152; 
string SiCbpHkBGsxQ = "Tgm"; 
double VOcqYQotiG = 3.315018E+23; 
while(VOcqYQotiG == 1.602161E-32){
VOcqYQotiG = Math.Pow(2, 2.1);
int[] FTQROLQudGAuolsI = { 8925541 , 54720 } ;
Random TdVkBAjHNPUYTWNXiXR= new Random();
Console.WriteLine(FTQROLQudGAuolsI[TdVkBAjHNPUYTWNXiXR.Next(0,2)]);
}sbyte HEcCgBHdXTGRJcme =  -31; 
int bVVxjKMy = 27; 
if(bVVxjKMy == 41888){
bVVxjKMy = bVVxjKMy + 706159;
}int HzXdffMXem = 8; 
if(HzXdffMXem == 866686){
HzXdffMXem = HzXdffMXem + 452506;
}short gFMgSSudCPwf = 4732; 
sbyte OHHxD =  -102; 
double XqWGIDk = -2.984631E-34; 
while(XqWGIDk == 105846.9){
XqWGIDk = Math.Ceiling(Math.Sin(2));
 bool? pusITDgkfbqTdslwsm = new bool?();
pusITDgkfbqTdslwsm = true;
}sbyte KTiSisyJ =  23; 
string tloWw = "ExwszWDd"; 
long bxlSBOXtS = 64833948302118809; 
ushort qUwwkmANTlWGPAfwmtjGGnWdfKj = 64863; 
sbyte jchiqqHbaGa =  -125; 
string WKLUqSO = "LPodYqoBdAGDbUyGhPPksFFFjOh"; 
ushort oJpEnqtUntoMBCCwCfblfMDcDEgkK = 56618; 
byte TDORoYOD =  23; 
long xBAScdjJIfjTDgmaKkhkwheTXN = 66317809698631284; 
sbyte dMdhAjDeJJlLbYdqX =  -1; 
uint wHUptdfpHd = 709810; 
string XXhaLkGajoCZdFngjUtSgy = "OPwelyGkBLpafjXPmIwxFOtaBc"; 
uint EqhFAfukoguDFeboNFhXmfgkLb = 9877; 
ushort EjYMHiDMBaQMAg = 16503; 
float BNemWUDHykWUzRwQUDkFTySMb = -8.790214E-29F; 
sbyte HZYSjNlqiIoxfjYHR =  -118; 

}public void uWGFcoY(){
string skTNueRlYcoeizLUViNnTfkbXNG = "uVE"; 
byte qYYPCbasNJPNelyUKgfkQ =  10; 
byte SQIAQRcAcwSBNfugNVgyxnTBhgKPy =  3; 
float MYXMuBDVGo = 2.830643E+13F; 
ushort hFNYzUVQEEXgsJgmccDEOPiJNltk = 54045; 
sbyte jgkZfczDYtWutPmiTO =  -36; 
byte gRitwellCKZyaNagKbkE =  126; 
int SuQTBdAx = 875616; 
if(SuQTBdAx == 299618){
SuQTBdAx += 34253;
}int YmpxJXXtbmbjNiKiEHqYG = 7178; 
while(YmpxJXXtbmbjNiKiEHqYG == 7178){
YmpxJXXtbmbjNiKiEHqYG += 519400;
}int LLfds = 3293; 
while(LLfds == 3293){
LLfds += 903338;
}ushort SmbpKpjNzJLIdO = 34511; 
sbyte wxunqDWfwWsYy =  -99; 
ushort iszgNEktslbuxnzQc = 48672; 
sbyte ciCLfjotSObDofU =  -24; 
string HRkaVeBCKgyDWKaXzaRObEnSCFjAf = "tcNcfnncmxj"; 
int iVoSqDqHUFoYdEtz = 9008; 
while(iVoSqDqHUFoYdEtz == 9008){
iVoSqDqHUFoYdEtz = 749760;
}ushort TFMJxNuEzefWlNSXOJAzyebL = 59740; 
int CihewBAYNQJkhjMAZyxmFcdILmWt = 55919568; 
while(CihewBAYNQJkhjMAZyxmFcdILmWt == 55919568){
CihewBAYNQJkhjMAZyxmFcdILmWt = CihewBAYNQJkhjMAZyxmFcdILmWt + 814654;
}float qZKwNOfhfzsoM = 9.965264E-36F; 
byte qsAfAuhFgjjPVsq =  85; 
short ZNbAQXW = 1835; 
byte HIiBOoCztpRGDQHFVskfbXwydBy =  215; 
uint iYgmYAaJbGRSYaptk = 8565; 
byte Eph =  226; 
long MymRJWRgT = 52570248194355474; 
byte ZugKolZ =  78; 
uint IZsnmnlOOhWgUyVVtLI = 24493185; 
long MKRFOXGKHpCkAWEVpaDQfmSBKP = 18710836724307625; 
long wKzQizoRtlwYPQqOKV = 2283913143367481; 
ushort EiotpZAHDMIIXbhytwczqYJbthMeO = 31246; 
byte cqmjXcJQpLlXm =  27; 
uint iyGiLWIFAmZhmwSdBmFGOXBpM = 1596; 
int LOnHVVqSdKgKILKcCDVThya = 912662; 
while(LOnHVVqSdKgKILKcCDVThya == 912662){
LOnHVVqSdKgKILKcCDVThya += 300783;
}ushort ObSOAMCqBCkyDzW = 7000; 
byte njKmNFnONLTJtDGlsIQWkqaku =  85; 

}public void aycYXZgbmOConBLZQzRhIRqfoppG(){
double ASwImqK = 1.511864E+32; 
double DwAqkNEioigWK = -0.001169536;
ASwImqK = DwAqkNEioigWK * 2;
object enJUGmu;
enJUGmu = 2.521189E+12;ulong ltekNbobMx = 76746135435008359; 
sbyte wnhKnIOU =  66; 
ushort fCtIlijDbngiLkJg = 30102; 
ulong aESDTXluWfnumqWiIPQLZW = 46073301354565412; 
uint dbuWnZALaPm = 761796; 
float SPcaqIfNedNHFTPVsFYplWZUJp = -4.382663E+14F; 
byte cqcxmIxFjIXnpTBsUSlTV =  219; 
ulong HWQhqbCxDwaxEiHhz = 13161869597876602; 
short nzqOjCBPpSnmig = 2343; 
ulong SePMldwhzAjJqJOaGnXkHRUdtpp = 71123766393612469; 
short JcwWGMcpgZiBmESFC = -14516; 
string MtVNkJmo = "xHwzAWkDnafR"; 
ulong qhKmIqmLw = 87394093025879967; 
uint WKsNcNHHbmnWtYfFGBwGXgM = 1707; 
ushort HdVDcHdREYuIAbKkyhpMR = 12905; 
int kDSQ = 951196874; 
while(kDSQ == 951196874){
kDSQ = 69683;
}string fKkophDHkTVFnVncTI = "xCTqBPmaGQQYxageNXawKIWRs"; 
int DUJeLblFbm = 6938; 
if(DUJeLblFbm == 128590){
DUJeLblFbm += 947945;
}ulong VAEgQBfSmhPFcZ = 80735153102254222; 
byte eSumBsEKgwxhKw =  20; 
sbyte TPdQyktQZsaaepYohbYgSsUFezHDM =  80; 
short ONlh = -23491; 
int IaxBZDJcezTDtfbgBUzL = 568524923; 
if(IaxBZDJcezTDtfbgBUzL == 174575){
IaxBZDJcezTDtfbgBUzL += 46413;
}double qeRhBuqfw = 1.741745E+30; 
if(qeRhBuqfw == -1.338444E-06){
qeRhBuqfw = Math.Exp(2);
int[] yLaHMbZPWLJZxoDbNkV = { 7018322 , 24979 } ;
Random cAMuFzG= new Random();
Console.WriteLine(yLaHMbZPWLJZxoDbNkV[cAMuFzG.Next(0,2)]);
}double BXyY = 464.1509; 
while(BXyY == -4.741065E-21){
double cmlGhcyhxRCXYGcmxoCZIhcYqQA = 1.414606E-27;
BXyY = cmlGhcyhxRCXYGcmxoCZIhcYqQA * 2;
int? UJRqqmZEk = 2847377;
UJRqqmZEk += 32565;
}int dNIHngo = 646125; 
while(dNIHngo == 646125){
dNIHngo = dNIHngo + 511112;
}ulong XcpdE = 62485474675505329; 
sbyte LlIqRqyoNGHKUtWopAQmjYnYnVUgs =  -18; 
uint FaMAJiVNAOEyXjnAGaCfHUPMY = 6745471; 
double qWpdPfTIxGgFdOx = -2.233567E+23; 
if(qWpdPfTIxGgFdOx == -9.49164E+11){
qWpdPfTIxGgFdOx = Math.Ceiling(Math.Acos(0));
try{
Console.WriteLine(qWpdPfTIxGgFdOx.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}short LlqlS = 14435; 
uint MjnOtVPANiLNbaCAHUNYR = 98114947; 
int VUCptoMsoC = 737; 
while(VUCptoMsoC == 737){
VUCptoMsoC += 831862;
}sbyte AIFeLL =  84; 

}public void XYETYkLBybnjuUZwMzj(){
ushort OOgMIAVJRiRzNqN = 50562; 
ushort gtJKXEVMGIjsmCwhjk = 28777; 
sbyte MQSHSxLHDlH =  -51; 
double cPdQ = 1.585352E+23; 
cPdQ = Math.Sqrt(4);
string JAYaMxGSQRVxFSKlbhyY = "DZxViaCcIMmbKPTwfJyLCbMzMq"; 
byte QuDScOgBCeEwdPifIuVdQQVAamtBn =  248; 
sbyte NDmnjwZVItUafbHMTHQswjhZz =  -113; 
float uHlDVMICDUWmwZxyWVtYGHIH = -2.583066E+25F; 
ushort umQfFiBbgsetufjZL = 36866; 
double OLdEKTjOYGueqf = -1.682233E-16; 
if(OLdEKTjOYGueqf == 2.688465E+12){
OLdEKTjOYGueqf = Math.Ceiling(Math.Cos(2));
Console.ReadKey();
}int XpBDObLicwpJAMyoxMSiQLXE = 5064215; 
if(XpBDObLicwpJAMyoxMSiQLXE == 465382){
XpBDObLicwpJAMyoxMSiQLXE = XpBDObLicwpJAMyoxMSiQLXE + 206537;
}string TXNXjPDHdaoVgytIAY = "GXyZltXOLBN"; 
long tMQCZAfHjkfPxbxqKTyBUxHxY = 29761625370831262; 
ushort wHRIaTJwMsjQqBxbI = 19457; 
byte WeuDzVTGsePAZzbLSkyHlaVdOLnM =  151; 
ulong bfAHpmnjeacdIDL = 10438714882702021; 
long jpPOHgbDGOuBSLzEbsHiXmCdUwm = 69663143573268499; 
double wzTbCbPqpZOnGwAW = 2.654326E+35; 
wzTbCbPqpZOnGwAW = Math.Truncate(wzTbCbPqpZOnGwAW);
try{
int gwlMNFxAGbIKFkMkhPDmYTGz = 5565425;
if(gwlMNFxAGbIKFkMkhPDmYTGz == 69157){
gwlMNFxAGbIKFkMkhPDmYTGz++;
}else{
gwlMNFxAGbIKFkMkhPDmYTGz--;
Console.Write(gwlMNFxAGbIKFkMkhPDmYTGz.ToString());
}
}catch(Exception ex){

}string nPwDUVTmCwttodIPnQOBncEoTATfx = "iCeRnyEqMDslbxRtmLXthNWD"; 
long kfJwcOQMl = 65448938039722771; 
byte bahzBlh =  42; 
string xFCfbmzIpAEwpjYdPWNh = "JKuJldHij"; 
double NdQmOsOXNnOMFHLXAxx = -6.689879E-08; 
NdQmOsOXNnOMFHLXAxx = Math.Ceiling(Math.Cosh(5));
try{
int dsHUXOWPHiHYzEOgZcFC = 2157304;
if(dsHUXOWPHiHYzEOgZcFC == 3310){
dsHUXOWPHiHYzEOgZcFC++;
}else{
dsHUXOWPHiHYzEOgZcFC--;
}
}catch(Exception ex){

}uint oPiccDt = 75794305; 
long ohmupzIMlbmoShTmtyx = 31075464471391467; 
byte qhHliCxdZteEplgc =  127; 
short joZhqPbkSLlhfwlVUGcJHxjteqn = -15951; 
float AIRzdkqZ = -1.220968E-05F; 
sbyte FsNkZieqlxy =  36; 
byte eGdZBOtIkFqHsaEVpGCpRSG =  221; 
short iCXdki = -19276; 
short LVpWtX = 29136; 
sbyte TtQdOqf =  83; 
sbyte WmakIwGSKgthxbUSaaKitUKxZZoe =  66; 
int udmuTLKJBOfmaQMWPiVqfgXEbDEl = 11047118; 
if(udmuTLKJBOfmaQMWPiVqfgXEbDEl == 72525){
udmuTLKJBOfmaQMWPiVqfgXEbDEl += 176039;
}
}public void eiIbhpqiunIKHncBWSpSmfKsphzaw(){
byte iRR =  162; 
ulong hOaOYD = 58870103406921200; 
byte qNYAoMZRqOgmzSQHntOeoeo =  142; 
ushort xkBuQJPwBdJoSwOixjxdWbgqslQgz = 15446; 
uint fqoPpWYzydqMXY = 367373; 
short Iegocw = -27521; 
byte ETJW =  70; 
int jCfYTBkXHS = 40651604; 
while(jCfYTBkXHS == 40651604){
jCfYTBkXHS = 441501;
}ulong FtnQnULgTuGO = 65103296744000265; 
byte TiJjLJ =  208; 
long xTDKmZtmlnMHRsSLHoJELjyfyA = 71546140751519559; 
sbyte FtSLxNbdBBcRRzethgilwymOUli =  -114; 
byte ypiXgnyYgYitGW =  112; 
short GeIicsXAhfe = 20563; 
byte YRecNIUsQWiZkccSpp =  158; 
float YFmwXlyZhshWukCBTL = 1.593335E-05F; 
byte MZchUypQOj =  79; 
string mRIPwxsJFadRSBqxGZyoKSnsPqaKs = "YGsknJlVkaVGgYZqcW"; 
double hkSMouEpZexJq = -1.866361E-34; 
while(hkSMouEpZexJq == -1.180227E+09){
double OyueLosyoeMYwNNHWUfVVg = -5.649893E+11;
hkSMouEpZexJq = Math.Round(OyueLosyoeMYwNNHWUfVVg);
try{
Console.WriteLine(hkSMouEpZexJq.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}int PIShQFtRFTw = 815286; 
if(PIShQFtRFTw == 338782){
PIShQFtRFTw += 280192;
}long QXtqmoWC = 1391454968222933; 
ulong jweWgXVWCCIYtEZhMs = 81702218163938483; 
int eIYpDOe = 1272; 
if(eIYpDOe == 431188){
eIYpDOe = eIYpDOe + 413391;
}short uTIzzLibAmzXsCaihuNSUEq = -5948; 
int KbSDZVoezjeCezW = 47924581; 
while(KbSDZVoezjeCezW == 47924581){
KbSDZVoezjeCezW += 745345;
}ulong LIcJzT = 48097640675081665; 
double DImcQeHonV = -4.565133E-32; 
if(DImcQeHonV == -1.373736E+19){
DImcQeHonV = Math.Truncate(DImcQeHonV);
Console.ReadLine();
}byte ATjRoIWuVBStDuAzksA =  82; 
sbyte bDEwOiOhDbUHD =  -81; 
sbyte DtzOqAsyExcbElMT =  -16; 
sbyte HnTTNGOXQzVRpqMQgSudDkfRs =  -97; 
ulong qpbjztBlJALhDCCZJF = 7148583050584862; 
short hbCHTiHByLtXRB = -24079; 
long lBaKgi = 83978428660471131; 
ushort edIYe = 18118; 

}
} public class VaDXkQTqoXUQA{public void MaTOmQxMQx(){
int SNxAJcYgfJElDEjwHNfqXRsdPkRz = 176160512; 
while(SNxAJcYgfJElDEjwHNfqXRsdPkRz == 176160512){
SNxAJcYgfJElDEjwHNfqXRsdPkRz += 288806;
}double VPoSSt = -2.702906E-34; 
while(VPoSSt == 4.691003E-22){
VPoSSt = Math.Pow(double.MinValue, double.MaxValue);
Console.WriteLine(VPoSSt.ToString());
}long eRHsjkFZPYeEAoGcO = 15010102073087504; 
byte qIMWKsYMGHguzKGsZcnzPBNsWD =  154; 
ulong sEJZsmqcbukCDawIZ = 75994885456023609; 
string VuahMijncywfyooYJcWlBqKWI = "JsTEnmnx"; 
ushort bmzttMskFPYSUCqsQRpEK = 5146; 
uint BSwAiiSbSHWxEDTOZ = 350271; 
float QyqmVxxfS = 2.061727E+34F; 
ulong TBqTbwXWZnFVPmYCFjMHqKwdmmMw = 56737107896871641; 
ushort zWVoQ = 10758; 
ulong gizFnQkfGifn = 20252855664971032; 
byte EGezLP =  184; 
string lpVfEWqMQOFcBmizKmtmQBggZG = "BPexuEoTXMIgHLnSSHecsVZaDs"; 
ulong MiOf = 65449375767195502; 
byte lNcgNDDJoAVD =  131; 
int dTbOlhOXChEUqfAxGRZKmYxG = 5225; 
while(dTbOlhOXChEUqfAxGRZKmYxG == 5225){
dTbOlhOXChEUqfAxGRZKmYxG = dTbOlhOXChEUqfAxGRZKmYxG + 214511;
}ushort khIziLZlyPBYiIlDQ = 4528; 
short CWDeywDLeUJpMcKizbmVS = 17230; 
byte zyxEpBdkVLaoGDa =  194; 
sbyte hDgjlgYcdO =  81; 
ushort OKeHZZYwEQyuabwEWaANDGJpnl = 20209; 
int xpuCSmFBV = 77; 
while(xpuCSmFBV == 77){
xpuCSmFBV = xpuCSmFBV + 735255;
}long keEAhOqIphaOtOJshgBdTiPQ = 84891922787963629; 
long akcNnCcUDeENDmf = 43728792505839865; 
ulong hPHeIfdgVOypuSZ = 62583830295105175; 
int VFMbCXWopgNwQUZAGHAICs = 231444; 
if(VFMbCXWopgNwQUZAGHAICs == 433216){
VFMbCXWopgNwQUZAGHAICs = VFMbCXWopgNwQUZAGHAICs + 593522;
}uint KccsNBs = 37; 
ulong aeMlFiMLNKRmWkEaA = 3014103731643556; 
ushort hzgAnJgBnSZaPLU = 50256; 
uint QfhjBTKPojlWoClMFlGjEEQKoh = 408440; 
ulong yaKFUmoYYnQzuWnsDyPdfIhno = 88949161054092555; 
uint llLIYxHnsteiTxXFbHMLQSw = 6633; 
double dGUCAXjbfSQQHAtQVjdybWNqI = -1.720978E+19; 
dGUCAXjbfSQQHAtQVjdybWNqI = Math.Pow(double.MinValue, double.MaxValue);
try{
int TkjmJoyRPbXMtcFRNHf = 3250879;
if(TkjmJoyRPbXMtcFRNHf == 30939){
TkjmJoyRPbXMtcFRNHf++;
}else{
TkjmJoyRPbXMtcFRNHf--;
}
}catch(Exception ex){

}short CPwPTZucxMDpu = -31109; 

}public void nLJz(){
uint NibTzbcIMsZgynlCWAdjPqV = 693790; 
long fnQISPGOHuWwYsuKuEiWORNOj = 17103685105360640; 
byte tWKgsSAGgaefSuHBZCPcoCbzyzHs =  92; 
float JOxbOKxQdnlGNfoAFQNAIXg = 3.721706E+26F; 
float KUkgllnwq = -1.874006E-11F; 
float DLLPcVntiYcDGwfMHOiSzNWTh = 4.544558E-10F; 
uint XdkXWWWlsFWHTCViFHRiK = 64886916; 
byte sQP =  216; 
long CPJk = 17668081651488222; 
sbyte kFlaLDsDq =  -126; 
byte bXFghuLCuSkpaq =  0; 
double ZjxzXPNEOheL = 0.0009467949; 
while(ZjxzXPNEOheL == 0.9157517){
ZjxzXPNEOheL = Math.Pow(double.NegativeInfinity, 2);
object diRDtlGRwuDsBBsTZYdURgwtJOOD;
diRDtlGRwuDsBBsTZYdURgwtJOOD = -1.617074E+24;
Console.WriteLine(diRDtlGRwuDsBBsTZYdURgwtJOOD.ToString().ToLower());
}double DBTjClsNWfXNyTFzdBInLkXNT = 1.984377E-26; 
if(DBTjClsNWfXNyTFzdBInLkXNT == -6.225509E+33){
DBTjClsNWfXNyTFzdBInLkXNT = Math.Ceiling(Math.Cosh(5));
Console.Write(DBTjClsNWfXNyTFzdBInLkXNT.ToString());
}long MuOLObRAMDmsExUiMRiDJkBtcjj = 22966935865849673; 
uint GQJ = 615117101; 
ulong MhtLV = 65101166022800799; 
ushort FSJJYPqXlSsigaAHdxxHU = 52720; 
uint NkOamecHG = 231365291; 
float XPnwkgTDV = -1.087676E+35F; 
ushort iezOOPtwWNbqKhPL = 36877; 
byte JtfIhlKJGLRPQFMVYiRiyyS =  5; 
string dBZdRsljN = "cxqCef"; 
ulong eYkNXmRDMbqkCeGUhiKDT = 48789872908562187; 
short qLhWd = -30006; 
short NlnGkTDFYRxuVxkRG = -7069; 
int suoLxXZBuiDSRHWpFTsNoaULwVi = 492651302; 
while(suoLxXZBuiDSRHWpFTsNoaULwVi == 492651302){
suoLxXZBuiDSRHWpFTsNoaULwVi += 447435;
}ushort sWEinUbk = 51599; 
short DoAUlNbSGsAFoCOead = 24160; 
float uxFtbKeTmQfxR = -9.023611E+28F; 
ushort eDIGQSSsnIAPagVwihT = 59935; 
uint dWYsDuiy = 88; 
long ZpkQFEMbNbFdlJ = 63764357948658558; 
uint HzpziQAGgFgGktEojBxcsV = 85; 
byte PYTiseR =  128; 
sbyte nnSWDZ =  113; 

}public void DFPoHNpDyxtMsxmabBMX(){
byte OLky =  1; 
double AAsXzqHtAqgTPIugyOSa = 1.358356E+31; 
if(AAsXzqHtAqgTPIugyOSa == -1.144803E-38){
double eiMtVXX = -0.7866232;
AAsXzqHtAqgTPIugyOSa = eiMtVXX * 2;
try{
int HICiGdy = 2700205;
if(HICiGdy == 39764){
HICiGdy++;
}else{
HICiGdy--;
}
}catch(Exception ex){

}
}sbyte BUKyfJVMujtSI =  111; 
double yCqBE = -6.152815E-34; 
while(yCqBE == -2.960233E+31){
double tgbkEDLVyeOxCHkGyafLsNdIQ = 4.969261E+22;
yCqBE = Math.Floor(tgbkEDLVyeOxCHkGyafLsNdIQ);
try{

}catch(Exception ex){

}
}ulong QVKhtidKaxSeihzbTKaFP = 42711991184623951; 
byte nJYnzLtslTFllhTIRpZOZpkTRRDkO =  1; 
double RyExkUap = -2.569098E-14; 
double YqQAwZ = 2.562623E+27;
YqQAwZ = Math.Sqrt(3);
RyExkUap = YqQAwZ;
float dwGCuAQgsyYx = -4.160431E+12F; 
int PDM = 97277983; 
while(PDM == 97277983){
PDM = PDM + 318866;
}uint DWTXIQLUhCnoJWofhZKdKBqZbeku = 43271892; 
sbyte UDmkzBpG =  45; 
ulong zCmdKKlRzHymcGU = 41811568200823929; 
ushort KeHIKd = 24967; 
double SzO = -30.7615; 
SzO = Math.Ceiling(Math.Cosh(5));
for( ; ;) {
Console.WriteLine(8.351939E-38);
}double dBfVwTatYjRixjWNaJqZTgCfEgSB = -8.729008E-35; 
dBfVwTatYjRixjWNaJqZTgCfEgSB = Math.Ceiling(Math.Atan(-5));
int[] qnaAqVxTNkLtmnXzDnnmzMBfwPOiC = { 7512605 , 1212 } ;
Random hCGBwxSXnWSPfwb= new Random();
Console.WriteLine(qnaAqVxTNkLtmnXzDnnmzMBfwPOiC[hCGBwxSXnWSPfwb.Next(0,2)]);string YyPJLCGOUeDhnqpdjNMT = "HFasuJHeeIOOCEZtS"; 
float QIXTLPFyPsRaogfqTOqpSZKQyOtMO = -1.870908E+14F; 
short NdUKMpEPlRNnIAHxlfGCEgRYhw = 29521; 
ushort JyIAhDdlIhzNCHhWCPnNpDkeQn = 39959; 
byte DpshWDueOBXDkEYhOcBJbGHJJEg =  59; 
int TWRpfSfGFdOSOoqjeYmWb = 1052; 
if(TWRpfSfGFdOSOoqjeYmWb == 944898){
TWRpfSfGFdOSOoqjeYmWb = TWRpfSfGFdOSOoqjeYmWb + 674670;
}long twhBeHbMUzgbizxV = 51809268493851491; 
long IRmxhixfkJGqKJpyjAgVS = 20668285977190389; 
float iTwYjzzpX = 1.523295E-34F; 
byte oShJcboKTMRVhFNcRffqxyy =  70; 
long YMiip = 41915135865352466; 
double DogFdcTMngAXWqRuTZX = -14424.63; 
while(DogFdcTMngAXWqRuTZX == 6.155416E-06){
double UeiShLThqK = -2.713024E-08;
DogFdcTMngAXWqRuTZX = Math.Round(UeiShLThqK);

}long jtTY = 26976521253203736; 
ushort ABmIREHhXmyiLdictgSuVpyWBxWeq = 56136; 
byte tjAgCDIfybxHBtUFyiyBSceidphu =  162; 
uint ecAPCFcBQzMWTUQxhDqHiwJfp = 32; 
string wmzVlsFb = "tauIHbfgknmUZSpVIJ"; 
ushort zwnZoukmbIDlu = 37420; 
sbyte mCOHZjUcQqw =  -22; 
sbyte EJj =  32; 

}public void XMwsm(){
float eEMbAxJYIgVOwoBliYYQebdUbO = -1.66511E-23F; 
int iCQgfqqPRZNksyNxlQxBmtnaO = 48; 
if(iCQgfqqPRZNksyNxlQxBmtnaO == 728319){
iCQgfqqPRZNksyNxlQxBmtnaO = iCQgfqqPRZNksyNxlQxBmtnaO + 948544;
}ushort QXuXnijVthZcU = 44071; 
int GDJOeF = 249800198; 
if(GDJOeF == 39373){
GDJOeF = 847789;
}uint ecoRCBtzXHmFTkoYUuTKtjnJo = 1449; 
sbyte VgqxDdX =  66; 
string fOhFbC = "MFYRAIYPyVOGCptl"; 
short AWip = -22642; 
short RmNWkFlh = 19149; 
uint isxAsHfhjhCRNDSDCF = 4111; 
ushort HJulTlqCzNQQcXGaEDk = 437; 
sbyte owau =  -36; 
string DzegDMOfgOTQIJPbXAVXC = "Okx"; 
uint tYkzTpRVnLKBlSVxOq = 49; 
ushort udUydiCLUKehwgVo = 626; 
uint OBqnZKfHFaUlYjSFhFwlIuERZ = 73633; 
ushort BzpJIGadPUOYDWKazfYDDOLF = 18493; 
byte XAoEGJSdCEEpE =  217; 
uint KImfzTRCpfUl = 77027408; 
long zmcEBqhpLSixU = 81278824129499181; 
string dJNmAOGqNSiSqPXRsAXwGKuHBLj = "sNOtuqZV"; 
ulong ICeC = 47616437898397428; 
long dJptogNPFuYadPy = 78629793001286754; 
string PUDIohOepp = "aGGHqTXLDwCWBceaAtHHtl"; 
sbyte mjgolkVNAMZkxyXX =  -6; 
int TmXcGkhoMKJsPBguh = 65614038; 
while(TmXcGkhoMKJsPBguh == 65614038){
TmXcGkhoMKJsPBguh = 782360;
}ushort dTEbwJhOKllogBedVicRujL = 20226; 
string jRfjszBoCqEmpaBEeoUeblEw = "qMHtTinYGzqIBT"; 
ulong SZNBJSbpQewwZPYmLNbNTVIi = 57451105654502252; 
sbyte imRUOzKpEPN =  89; 
double BeXOSaVWqFkthAhUBZoJGLxQZ = 7.166602E-35; 
while(BeXOSaVWqFkthAhUBZoJGLxQZ == -1.215384E+28){
double beYQEgwmWlKuqjiJnNEplQIgOo = -4.689561E-05;
BeXOSaVWqFkthAhUBZoJGLxQZ = Math.Ceiling(beYQEgwmWlKuqjiJnNEplQIgOo);
for( ; ;) {
Console.WriteLine(2.002595E-13);
}
}short dIzcXpbL = -25206; 
ulong qxbTbzaiddWVSCTdHm = 55474558356839131; 
sbyte ojhoWbp =  -11; 
double jYQTkghNGBdmwzAJWjzRiLs = 1.741691E+30; 
double fTcInPWRcXLGfbP = 4.568814E+24;
jYQTkghNGBdmwzAJWjzRiLs = Math.Round(fTcInPWRcXLGfbP);
try{
Console.WriteLine(jYQTkghNGBdmwzAJWjzRiLs.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}public void oQG(){
int iVqyVloXTYTtqtAUm = 22; 
if(iVqyVloXTYTtqtAUm == 77415){
iVqyVloXTYTtqtAUm = 79681;
}sbyte hoSQfpHal =  -116; 
sbyte nXTXVRNmcIUdhc =  51; 
ushort UeRhkNXxgOIltfquAYi = 23458; 
string mqWjAwOJcGmtsXnndjVBOce = "JDD"; 
long eDbEgSAkamDJzLcZZOtg = 6958209832952547; 
double XSeZYdkQkfOccXsSo = -9.388963E-37; 
if(XSeZYdkQkfOccXsSo == 1.764995E+21){
XSeZYdkQkfOccXsSo = Math.Ceiling(Math.Cosh(5));
Console.ReadLine();
}sbyte aVY =  -1; 
sbyte APRupuyxUmiHq =  111; 
ulong nyWgzHJCtUAHn = 39482167543372479; 
float ibU = 103.712F; 
float qjmGICEbWSgpbwURhZGHDlj = 3.922467E-36F; 
ushort NfEehKFEotJmf = 63427; 
string UZcSHok = "EWkleaFEnfRVtOYGxRFyAM"; 
uint zdpJZCOzUSBkztzoPeZa = 16; 
ulong axwPKN = 70773013299761289; 
int ssbBmOhdtoJuNABpnjJEfZ = 13981728; 
if(ssbBmOhdtoJuNABpnjJEfZ == 791016){
ssbBmOhdtoJuNABpnjJEfZ = ssbBmOhdtoJuNABpnjJEfZ + 926670;
}int GTBmJPgnTmnsxue = 87069569; 
if(GTBmJPgnTmnsxue == 434185){
GTBmJPgnTmnsxue += 750757;
}uint aGsEUVEYk = 914241; 
long UARMjZpxoOEucOFSUjKeHBHp = 85136258545758233; 
long XFfomNkRLS = 41713693208585190; 
byte itTCBWHZhlTcKOQaNGW =  240; 
float PPBbwkqiMHoefFLEOpgkJAYN = -1.183078E-35F; 
short sLqnMtEeaABwIZAlNGIloiRHQJgpQ = 17456; 
uint TtQbpbsNaHEo = 71883448; 
int ZMZUxgJiUKzdmmnmYiDoi = 919495; 
if(ZMZUxgJiUKzdmmnmYiDoi == 892502){
ZMZUxgJiUKzdmmnmYiDoi += 210987;
}long DRefxqdOHWNKHgHEoBN = 81520496314387282; 
float ICVaEsStXQ = 1.144971E-23F; 
uint mPFDsoiXuyo = 2241; 
byte RWQmBZuRKcPOjqLQhSZtuKXiBfK =  38; 
ushort ngIygUgXYsTXVAECYHKIhy = 15330; 
float hYHMwoluYPIgNkRKk = 2.073073E+11F; 
uint OyGZgdzQERUPWY = 498818994; 
byte cHEXLVwkwTXWEFsetB =  2; 
ulong lOEuYOqRmV = 24973199256218392; 

}
} public class EVIGTImpyRFwAypRTaDNhe{public void iXNWnZzmDNois(){
int LMMyUZI = 703037; 
if(LMMyUZI == 775433){
LMMyUZI += 4272;
}float aEABzIfMZp = 3974785F; 
string joXmLoQbKjwByctmkzum = "MKRfBGaMgVwGTBncCpEzZUo"; 
sbyte jcsMfBjPUzFghjKQoDoZVFDfjKiR =  72; 
ulong uLhpDIabwkQYbJVtfTJKsagcG = 79763666110090094; 
long xaThhoBU = 24758201733012328; 
double dAuFeL = -8.881073E-07; 
while(dAuFeL == 3.363709E-34){
double yAtmMBwRgsXZZP = 8.523458E+23;
dAuFeL = yAtmMBwRgsXZZP * 2;
try{
Console.WriteLine(dAuFeL.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}sbyte bHkAPyWPlHIzeGPFWpxJkT =  77; 
uint zinwBDGN = 73998; 
sbyte wbzsZVsgyLHRcLXQzCi =  81; 
ulong poIuLecLJCJ = 4585814578183916; 
short OCysQGPoUVoTfLjLGkndMb = -26439; 
sbyte jJSh =  -45; 
byte uDCNkbUAmxPH =  100; 
sbyte DNGFNjnaNxpoHW =  112; 
sbyte KWzDjyBEpPQplzNWOg =  -104; 
uint zigDPpUNnDpolXzJStpfay = 451934; 
long YDsSQSGRlNsauhGR = 53935615983113991; 
double PwLaOJToqsuwNzKwMMqSZiB = -7.311343E-08; 
if(PwLaOJToqsuwNzKwMMqSZiB == 3.15079E+13){
double WcdjfkY = 2.649086E-05;
PwLaOJToqsuwNzKwMMqSZiB = Math.Round(WcdjfkY ,1,MidpointRounding.ToEven);

}sbyte pyoFFtXXpXighOHRnnQosSnHNR =  99; 
ulong TjtiGwYkMWtLOFueIHWnBIwUS = 9923492131816466; 
sbyte OPXDtJGDsxFEnTD =  -99; 
byte CqlOzpdXPEUiUFJgHpKeEGVedznX =  42; 
short LECFO = -17774; 
double IzwUcJiDZUh = -2.886751E-20; 
while(IzwUcJiDZUh == -7.322001E-10){
IzwUcJiDZUh = Math.Pow(5, 2);

}byte ZmEnwAkkELQfOcOxGVgjsEQuKwnDq =  63; 
double KneOAFTYXyO = -6.1512E-05; 
if(KneOAFTYXyO == 3.123324E-16){
double yaIxxjEZkJTqOBwAYZW = 1.147288E+35;
yaIxxjEZkJTqOBwAYZW = Math.Sqrt(3);
KneOAFTYXyO = yaIxxjEZkJTqOBwAYZW;
Console.Write(KneOAFTYXyO.ToString());
}byte AduJMTpIIZhtYqsRZMkIStnTDVTbs =  11; 
double DpetknbBbktatWP = -7.532701E-18; 
if(DpetknbBbktatWP == -5.515449E-14){
DpetknbBbktatWP = Math.Pow(double.NegativeInfinity, 2);
try{
int mYHMLKGIZmAHfxNkPHmYj = 8082051;
if(mYHMLKGIZmAHfxNkPHmYj == 39519){
mYHMLKGIZmAHfxNkPHmYj++;
}else{
mYHMLKGIZmAHfxNkPHmYj--;
}
}catch(Exception ex){

}
}ushort zowWEBkSMknQCZmxJmaMbMQaI = 44248; 
uint lRYFmmIUUFYfpeNXnlXLqfwDuKR = 48492778; 
ushort UanHUlKEns = 22527; 
uint oVxW = 861660008; 
ulong LOFgwxkDxUwBpd = 77585518353293950; 
ushort EsnAnUOHITimKBzQAaRyWbOkQuyfh = 11403; 

}public void bwFDdhLnCgPSxVSsXuVeISV(){
string EdJUyIzzfgReXwIOM = "qhjqoYcBMle"; 
sbyte BDaAecue =  -11; 
short qOamZZmpVCFVs = -16960; 
ulong SWWmuZWqDWVEMskTRQaBXP = 8961574348102561; 
long qqChCdZqS = 42276936965893480; 
long DlmOd = 18047868537618656; 
short mwRcqOQJJmeyJolHqjDf = -31440; 
string zMsPwCEnOPxyiMnupN = "EBZYBYtiXkuiHiRtqZ"; 
double hfYpJlScVcQltz = -3.523482E-37; 
while(hfYpJlScVcQltz == 9.72451E+24){
hfYpJlScVcQltz = Math.Ceiling(Math.Acos(0));
Console.Write(hfYpJlScVcQltz.ToString());
}double NfIodgGxJK = 5.717157E-20; 
NfIodgGxJK = Math.Pow(2, 2.1);
Console.WriteLine(NfIodgGxJK.ToString());short toMBNFuQuUNyzbDoLTfOXgQmH = 21703; 
int NsMwOfnkQucYxJClVjmIsDICOCT = 4689; 
while(NsMwOfnkQucYxJClVjmIsDICOCT == 4689){
NsMwOfnkQucYxJClVjmIsDICOCT = NsMwOfnkQucYxJClVjmIsDICOCT + 955050;
}uint chsJ = 7672; 
short pRgsnGFqxFYDkLk = 8151; 
byte bMqBgaXHz =  19; 
int UUadqKIjGHUhyqZXS = 9461; 
if(UUadqKIjGHUhyqZXS == 117190){
UUadqKIjGHUhyqZXS = 734544;
}int PLguHg = 778449; 
if(PLguHg == 75634){
PLguHg = PLguHg + 528654;
}sbyte CZcHuSjcPbZPg =  -95; 
float OhwWC = 121.0283F; 
ulong ButEHVwLYxNtAsNXNWewSZtXPVVfj = 42856421460685228; 
uint fFeiLdM = 8158; 
long PcxugCawOMZpBgTofSzogmOZQORJU = 77731219044588138; 
float syuMxfoEbSOHxsuSC = -5.349442E-05F; 
int LsCLeUSoetnl = 1149; 
if(LsCLeUSoetnl == 532665){
LsCLeUSoetnl = LsCLeUSoetnl + 340607;
}float jPQfxjNdtBlFpiIfT = -8.895047E-13F; 
byte nWABuglDfaXFbi =  232; 
uint nUhPVFhKWhzMTC = 63; 
sbyte SqGbdBBnAoZnaMEzMyeHtsqRzYO =  81; 
byte WXWmckQlRHnIeTKI =  37; 
ulong wPDDADB = 79551838408874080; 
short sOJt = 10010; 
uint zsLmzgRRbIRLuwxKDkb = 71615768; 
float GeBHpudPNoWegRnBIM = -6.253922E+16F; 
string pYIVdghH = "VFOfiYBCyXTdwyppSoKuXxhDu"; 
ulong YupWHgzbBtMfDgkSOi = 13614471673696818; 

}public void TzxkLuwCg(){
sbyte lXX =  -23; 
ulong gjgmMFuunUcIPdeiUFDdhaLhGdKN = 21376256802581938; 
short gUbRyBtUsdWcdtSMPfzYHJUHxeXWf = -24603; 
double RUyJbqEkPMMHRU = -410882.6; 
if(RUyJbqEkPMMHRU == 4.667823E+28){
RUyJbqEkPMMHRU = Math.Ceiling(Math.Acos(0));
Console.ReadLine();
}ushort MyYgnOJX = 32021; 
string sGRYwMBUdLHlRDood = "YcdRzHADubUmLE"; 
string eqsAgZeXpzflgFZAGCCazBhjzT = "zaQacjXyXWEo"; 
double gCZCnwQcuHjcbwFjhbnIGXAzccigc = 6.150029E-37; 
if(gCZCnwQcuHjcbwFjhbnIGXAzccigc == 5.536774E-37){
gCZCnwQcuHjcbwFjhbnIGXAzccigc = Math.Pow(5, 2);
int? SGXBZdFcARQgjqpHcUHnpOAlkKhXH = 4686327;
SGXBZdFcARQgjqpHcUHnpOAlkKhXH += 26857;
}long MkkhSLUwqF = 59544697846712608; 
long pRFnNEMOELiaGA = 39050290583273776; 
long yOKzhjTDHoZUGWunYgS = 2142417128387199; 
ulong uJChXQZTnGtSGtqATwd = 64705221290254619; 
long fyWeX = 16345533566063795; 
long ANRaLeApjIDgdGeON = 23722069294883083; 
ulong gNXM = 40852388076054792; 
string hMyueDSIUuhHDtcZiTZc = "DmVVEYjPwKmO"; 
uint Lpn = 19; 
byte SEEbaMupjjbXoFA =  136; 
uint FJSAKVSbsqKVQQTT = 67943042; 
short ElPzkAlQlwlOODGCHtTJiYucHo = -7510; 
byte wUQjG =  240; 
short unjKcQWeVgnjqmqQJLGSNSVfZL = 21029; 
uint FiFmJleJZfhyYM = 13; 
string hEZCUsBgDteKqZxJpXJXJal = "JFywPf"; 
ulong YXJXxcomNDKMFpPibowkzefgqTSoU = 70602163128324538; 
uint nJwjfqBC = 8058; 
short AKDCTSCHEkZtI = -19421; 
byte BEwSDd =  38; 
uint UZQR = 4758; 
uint LSMdESCPSIcKbDVdubP = 4322; 
float DazZTMPlMBNXs = 2.632027E-36F; 
double UdLxeRaqdNypJejTyjceSJ = -4.31655E-35; 
while(UdLxeRaqdNypJejTyjceSJ == -2.967885E-25){
UdLxeRaqdNypJejTyjceSJ = Math.Ceiling(Math.Sinh(-5));
try{

}catch(Exception ex){

}
}byte tXYCBTakRaNZxqyoBdsVn =  149; 
byte PRYYknFlbBVwAx =  41; 
long LyPALbAobVDOowOJCMKVba = 5359321432322347; 

}public void ULjE(){
sbyte XeQAKitczMegMYLz =  58; 
long UtYMfdlmRIcfSIjFIUxsR = 44381856692346982; 
ushort YATlgYsLwyVCuw = 41329; 
byte CtcFpsTKZUAnOVxz =  57; 
string YwdGpjaVVJZJGEQPypR = "hZDgdSxHCAdiqbndzONwo"; 
ushort aalUKYfZhuwzZTWCyNttxGQqFpMq = 44387; 
int uKcCgRXMJwBWOV = 289663; 
while(uKcCgRXMJwBWOV == 289663){
uKcCgRXMJwBWOV = 295185;
}long eBwjUpumPpEYaOWqPLAaLYWJcxb = 62858905245179141; 
uint XWIswG = 35; 
ulong PsTMCPIjfFMgzotMaSpkCiwkSOS = 15188026033029044; 
long cWzfoAWtRzsZKGqcEsGX = 56293368307087365; 
ushort XcLeXWRfmJzRboBJGMwnKiGLYh = 10596; 
uint tLeYXB = 2826; 
int wlzeXARTzxpocygjPxZg = 77019379; 
while(wlzeXARTzxpocygjPxZg == 77019379){
wlzeXARTzxpocygjPxZg = wlzeXARTzxpocygjPxZg + 961535;
}ushort dPkubdackdJlhoKeQeJHROMToy = 56594; 
int AlZfXPKlWnYydYLNOZBdjiPoObpc = 1035; 
if(AlZfXPKlWnYydYLNOZBdjiPoObpc == 369414){
AlZfXPKlWnYydYLNOZBdjiPoObpc += 130021;
}ulong KfjAaSTQKiOPafBzsDMpxKsVA = 48665898644578319; 
float IPbd = -2.870262E-16F; 
double FnBckFeSQuxxaOnauXc = -2.499897E+32; 
if(FnBckFeSQuxxaOnauXc == 10385.73){
FnBckFeSQuxxaOnauXc = Math.Exp(2);
Console.WriteLine(FnBckFeSQuxxaOnauXc.ToString());
}ushort jaKCAktXD = 26361; 
uint SUZSgx = 6620; 
long liiOlmAZ = 88576314835085501; 
short HTMqmTgopY = 24748; 
short NEZOCtMZg = -2594; 
int eHdxXNuedbQzUWoIXWCfphM = 45; 
while(eHdxXNuedbQzUWoIXWCfphM == 45){
eHdxXNuedbQzUWoIXWCfphM = 610143;
}short wiFhlcSslAmiMNCU = 6859; 
string bkWxy = "VTZKiazOCwopOdFeFpHdYayWtZpC"; 
byte zNhjqiTJdz =  196; 
ushort slFRaAWG = 41600; 
ulong YUxcpjLXAmXDi = 81833403306342228; 
int eKPhocqJRXOlxMQsUNQVXwKxm = 601; 
if(eKPhocqJRXOlxMQsUNQVXwKxm == 395063){
eKPhocqJRXOlxMQsUNQVXwKxm = eKPhocqJRXOlxMQsUNQVXwKxm + 633577;
}string dqtBbxVhEfGus = "KwEswzJTKat"; 
float ZHY = -4.345396E-16F; 
long tuyRSqVNtoebVYtogbkGMjIth = 10392834462952556; 
uint pYNBeEVBuWsYcGqSweceiNeKLWYl = 62676063; 

}public void oucdqtCgcQKmyPispkNnkJxN(){
int dQhFKMOgcFaDaATYaoABHUDFIa = 36; 
if(dQhFKMOgcFaDaATYaoABHUDFIa == 966016){
dQhFKMOgcFaDaATYaoABHUDFIa = 289414;
}uint pMYzgXzpMEqOjn = 2409; 
long ncmimE = 51510248273161440; 
string jhaDhDbqhYmURNYqYR = "yjg"; 
uint CyldNpXGMGemm = 97; 
long Wbo = 16428246868115820; 
long udJGaLpXFRbHikJLeS = 25943624683603168; 
int HBThWhkbEH = 28; 
while(HBThWhkbEH == 28){
HBThWhkbEH += 184895;
}byte THoMgFYuhgYADCecqdYTL =  7; 
string hzODUsWVVtHYYUtnjCZd = "deEj"; 
short mkuZRHHFJRwBPkcFjUgeyK = 12885; 
long bWFdtjt = 61864406374549182; 
short LyOakTaAVJd = -14628; 
short BLQPLcCoHcYiDsdQfO = 16492; 
sbyte mVhRcFEzBpmqfjQDZIoOwQMbbqJxe =  16; 
short gEnPIYIzYRgMMRxhm = -15815; 
sbyte ztFyjf =  76; 
long guXbMjDVbUbZQQEKZpjVBjRyhcN = 24398113150129997; 
ushort LaHUlkt = 5428; 
sbyte EaNjQGRwWDATJxFexsTc =  -61; 
double BopDbn = 0.00373624; 
while(BopDbn == -2.379398E-07){
BopDbn = Math.Pow(2, 2.1);
for( ; ;) {
Console.WriteLine(-1.485544E-09);
}
}string QDYdXIfNuUinDIwyBKdaNHYYlJEj = "tQHeexnAVwdaycnMN"; 
string xcMQlakjFZSneW = "beDiylgX"; 
int aZpEKlzIhO = 653049; 
while(aZpEKlzIhO == 653049){
aZpEKlzIhO = 898835;
}byte BcgZembjjFEjpefBlaUGV =  191; 
ushort WhLtdieDZyyDibVcCBc = 57852; 
sbyte zwDAhdZGONKBcWCJfbAakSHDsjfS =  -109; 
ushort ACYKpREabDtJLTIYYCTwb = 25206; 
string GOZhYYhCucjZdmkRCbzyRjW = "KkgpkZJfykdDRxcduRnIJ"; 
long LqbTZVHWxSpyNRSqwteDTjIafZw = 52227080787755300; 
double fKjQoHL = 4.470302E+34; 
if(fKjQoHL == -2133.536){
double gbBJbNaAEXCEoxxuTio = 7.398385E+26;
fKjQoHL = Math.Floor(gbBJbNaAEXCEoxxuTio);
 bool? DXzhKiBAbZxVnVpgtJL = new bool?();
DXzhKiBAbZxVnVpgtJL = true;
}uint LEGqpcyAFTChADcVajzlyRlGQqRh = 382402; 
long RXGDNsujUHCtaMtg = 69218506771788106; 
short BZlpckhbqCXBiJPnsQoSGeYKTOQkE = 6340; 
ushort kORwYFEqlYuRjPBVMIUKFA = 25364; 

}
} 
public class nplctniJAEJdoRnLXmC{public void VhYVasUYtQtRMOlAlKfEmTHzdWZap(){
double DDfLgspcaukoJkSsCF = 0.2651519; 
if(DDfLgspcaukoJkSsCF == -9.463099E+35){
DDfLgspcaukoJkSsCF = Math.Ceiling(Math.Sin(2));
int[] lFyxCApwEuJ = { 4641042 , 58131 } ;
Random SzYICCVxuDMzAGFGcPK= new Random();
Console.WriteLine(lFyxCApwEuJ[SzYICCVxuDMzAGFGcPK.Next(0,2)]);
}byte AVygVXwLluBpxJxIBJC =  225; 
long ZWWVLsOLkaGTANewZAYSBdZph = 51141555868744962; 
long BkalcXCjAKEjKDfJcVRqVWGNPMG = 23142056009251047; 
byte mVVYTBHFMWGdfaxfoaEFmyqqZPCwI =  82; 
float VKPZWtbxZicPayoXSQEQaFNPFEYeZ = -2.914293E+33F; 
ushort Hixl = 28753; 
byte YhGeuCpDAFEROcYpXZwsuXyOlHMu =  74; 
float wHHRTZJucnxGuIfIaISAlb = 5689.527F; 
string SSOQVmGThjgkfqoFsdXsc = "fZLMx"; 
ushort tPdQigQHmsjZCAVB = 52678; 
uint jHKOGKINMPeiQltPw = 40; 
double zXeLNtBG = -0.002362405; 
zXeLNtBG = Math.Ceiling(Math.Tanh(0.1));
Console.Write(zXeLNtBG.ToString());int wQufVDtGsV = 1208768; 
if(wQufVDtGsV == 573434){
wQufVDtGsV = 466348;
}byte XHQw =  65; 
uint ZjALIFeTgjCqEKBzL = 22926184; 
float JuqEqbyXzaD = 91195.31F; 
double UULpFhuwcgGu = -6.257067E-27; 
if(UULpFhuwcgGu == 2.018774E-37){
double hhpEHVIiYAaOhEQcWmhoZoxM = 5.486756E-21;
UULpFhuwcgGu = Math.Ceiling(hhpEHVIiYAaOhEQcWmhoZoxM);
object tChqGENNuNXlMl;
tChqGENNuNXlMl = 5.87308E-38;
Console.WriteLine(tChqGENNuNXlMl.ToString().ToLower());
}uint bQTLF = 618003494; 
sbyte PysJhFRcJj =  -56; 
double OkPfbMjdQxKmFNTcQR = -1.998046E-30; 
while(OkPfbMjdQxKmFNTcQR == -3.261302E+08){
OkPfbMjdQxKmFNTcQR = Math.Ceiling(Math.Asin(0.2));
try{

}catch(Exception ex){

}
}double RKHjFGRWoMQbgKcIGJRpdBhBNdqCj = 2.232251E+18; 
while(RKHjFGRWoMQbgKcIGJRpdBhBNdqCj == 1.678196E-33){
RKHjFGRWoMQbgKcIGJRpdBhBNdqCj = Math.Ceiling(Math.Tanh(0.1));
try{
Console.WriteLine(RKHjFGRWoMQbgKcIGJRpdBhBNdqCj.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}sbyte MWk =  -124; 
int IbkPcgnmJjbeVP = 422149; 
if(IbkPcgnmJjbeVP == 568629){
IbkPcgnmJjbeVP += 66484;
}int UJRVcaNXFqGL = 99692188; 
while(UJRVcaNXFqGL == 99692188){
UJRVcaNXFqGL = 361095;
}long HykMHpGpjTyAQnhDXge = 84660913258754304; 
ushort oYktb = 48338; 
ushort eoNuxNLUfVtSQAl = 18109; 
ushort URBmx = 10248; 
byte VVlgPCLdS =  217; 
short cZUQtpOEnPNWWVSGXRSFkkRQIY = -25195; 
float MHtWDc = 3.279943E+34F; 
long guGYfipjTKefaZNJSieiujWxDht = 38063946734796197; 
byte lLHqPlhcGGtjTjMTCBW =  114; 
sbyte XPpRaMxSdmDeHy =  -14; 

}public void GXPtWDZQBJlPMyQNwQNSbXlwG(){
ulong fHPEnwKUZIQAwQBqs = 15158440999565525; 
long QIMuOiZTiLeXtcDQeGq = 22140657602401736; 
double SgNFQUmnncpxWqGmEndd = 3.004755E+14; 
if(SgNFQUmnncpxWqGmEndd == -3.303844E-13){
SgNFQUmnncpxWqGmEndd = Math.Pow(double.MinValue, double.MaxValue);
try{
Console.WriteLine(SgNFQUmnncpxWqGmEndd.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}uint yyfZkjfYliiBdhpD = 15; 
string lEuiZMfYHdX = "EPPPAmkFgQ"; 
sbyte RCSdwtLWPjmUcWZAd =  -92; 
short lUXntC = -28659; 
float AixxTsyUcEntbAuCVMEmLBZzZpWF = 2.775271E+14F; 
float UZpaRyjFCGa = 4.861661E-34F; 
ulong wDNyQBEl = 54616223415958961; 
short XAbjtLyCYLeBwukgCbQmd = 20097; 
short sQTZIiJoIUfohkicanCSgyO = 4594; 
byte TSBfHhinRDRyUQ =  12; 
ulong CnWYpSOlQoyHUcpNlpQPCnYfTt = 9259653986399301; 
byte pzDaMTRTMIX =  130; 
uint JAlbqnpReWlEKxDIooQay = 75243875; 
ulong Ixfu = 4283980791065178; 
short YZAtGPijup = -28107; 
short LCnVNxYJRjyTBfUSLqdZa = 21932; 
string ugaUzbPpSNwhFQAgqGhPcBeJosKZ = "tJTCpCGxejPwgaOLMmURXIuKkVTcs"; 
int sPpAeC = 27491875; 
while(sPpAeC == 27491875){
sPpAeC = sPpAeC + 258264;
}ushort uVBqXEePhcMo = 11538; 
short IEdAdyGQCJifZ = 6854; 
double uYsQZsLub = -7733.623; 
while(uYsQZsLub == -3.349852E+19){
double HqHOsVOfZJPyfjjQqqRBwoSKyc = 1.766263E-11;
uYsQZsLub = Math.Floor(HqHOsVOfZJPyfjjQqqRBwoSKyc);
object TQQfYHGHGxbUtcGEWsyzxACP;
TQQfYHGHGxbUtcGEWsyzxACP = 5.287582E+35;
}ushort BWQeTgDBdsuoaIH = 10678; 
long qxlqhmHWWgU = 72122280220069744; 
float izoATuJIsFwDwu = 2.507595E-20F; 
sbyte DyXkwHQHGhKKlBQThYWSoSX =  -58; 
int ALouieOIfwAYcJJmP = 960791; 
if(ALouieOIfwAYcJJmP == 901740){
ALouieOIfwAYcJJmP = 743559;
}uint ZpqzKAJHT = 11; 
short ZGWEuuuj = 27666; 
short cdxpVjE = 30653; 
ulong iOxixVQ = 4026832805173408; 
ulong BshbdbhxGdtLJEgEcQRAwYAF = 80200449961128227; 
byte dHFgfNoNQRxHBW =  106; 

}public void CMCmtDSEjqn(){
uint giwCnBBYVL = 20; 
long IhlmRkQ = 13310728561871369; 
double zOyKJbflxVsHbycLukxYexqA = -3.178974E-38; 
zOyKJbflxVsHbycLukxYexqA = Math.Sqrt(4);
Console.Write(zOyKJbflxVsHbycLukxYexqA.ToString());int wWL = 1862862; 
if(wWL == 542548){
wWL += 977106;
}string ToGnZUlmdcTYgRVufLRe = "HDFpmDcNXxibbiQKDjzcTDYDYE"; 
double xXRhX = 1.595727E-20; 
xXRhX = Math.Floor(8.951348E-13);
int[] ZFadlWcJFygTt = { 6171144 , 6875 } ;
Random sSbyoZMuqQVPiiZ= new Random();
Console.WriteLine(ZFadlWcJFygTt[sSbyoZMuqQVPiiZ.Next(0,2)]);ulong igaKSTMcc = 19599787316638564; 
short oXFGnkix = 17340; 
float YttHjoqhiEDXJwmENiBWP = -2.334823E+26F; 
uint FCNzKyYhta = 85703965; 
uint tWbgIMZWkbIwnm = 875631756; 
long VtGOEKzgFPouDLkMh = 7758384680199924; 
long oiLjBsiXiRFssJBhkHQzWjDfY = 27162295450300399; 
float YTGPcuOc = 25528.22F; 
long BLFdbAmVeHGluT = 85202275971902258; 
uint FROehhIDoK = 621081612; 
int VYG = 99593392; 
while(VYG == 99593392){
VYG += 404371;
}sbyte ZYaxjWciqzjMiNWYc =  109; 
long ljdYiSDtkZkxFCijIgfNWynCLly = 58963273116294087; 
ulong bmquJXqmOgHFNqVQXYOWCmWwqkS = 15884966572146279; 
float tmBWnXfRHwbpHNpEUtOpWczf = 0.03280685F; 
string wkzLbbEMnZmpRELQQyhYaIAVS = "TpPutPVowwWqnFiFoU"; 
sbyte AsqIofsdQOMESzi =  -62; 
double GywCipTAjDPxsFOtdBZgenyRzH = 3.913624E-11; 
while(GywCipTAjDPxsFOtdBZgenyRzH == 5.803765E-07){
GywCipTAjDPxsFOtdBZgenyRzH = Math.Ceiling(Math.Cos(2));
Console.ReadLine();
}ushort QDIwGASKinq = 19991; 
uint ascYpgWaYtlWotxR = 6866; 
int yQZg = 736274; 
if(yQZg == 835210){
yQZg = yQZg + 140208;
}ushort tgVpxWsltQEo = 11559; 
long znqcHLxc = 79867621242868047; 
float UEizfuostJIlLXQDjGBePHXdKYCy = 6.345869E+20F; 
short NqnfbahWOoqOuZHXKbny = 10720; 
ulong nGeOhTR = 12297328717707676; 
uint yFpApjJL = 5658; 
ulong BUYZjaBWmacWLMxtAmoZBd = 24532316160122091; 
float dReycAjIjFX = -32.58915F; 

}public void PEOokdoOlxUUE(){
uint dBjj = 86733936; 
float YqadHQWgEDcTuJgRObpG = 38621.73F; 
sbyte JGfeNmPBVVPYkZIYppqqBIJwtsw =  54; 
int awiewbKStp = 54933094; 
if(awiewbKStp == 877308){
awiewbKStp = 165999;
}int tqpqlRwaWKLuRdXHVtQfFHn = 936892; 
while(tqpqlRwaWKLuRdXHVtQfFHn == 936892){
tqpqlRwaWKLuRdXHVtQfFHn = 694001;
}sbyte kMIslXJiqKueZgYzL =  27; 
short BtCiNcSwlnypOhBJjM = 13516; 
long jzwkDnqYD = 66888762028740160; 
int fnXcmeq = 618816190; 
while(fnXcmeq == 618816190){
fnXcmeq += 903068;
}long pUfYkOBxJweX = 27121801501763464; 
double wifkyLImT = -2.838393E+16; 
if(wifkyLImT == 7.539192E+34){
wifkyLImT = Math.Ceiling(Math.Cos(2));
Console.ReadLine();
}ulong SfCW = 82732324269087380; 
byte KWTWG =  10; 
double CDEdMkmApdjefJAcRwBwlTS = 3.645636E+07; 
while(CDEdMkmApdjefJAcRwBwlTS == 1.265217E-22){
CDEdMkmApdjefJAcRwBwlTS = Math.Ceiling(Math.Cosh(5));
int[] hwbKQHyZafCKJRAl = { 5774929 , 89610 } ;
Random aJJyFPOZullD= new Random();
Console.WriteLine(hwbKQHyZafCKJRAl[aJJyFPOZullD.Next(0,2)]);
}byte KxGhmbCyiYVV =  29; 
ushort cqkIcqbEHnKwu = 14220; 
ushort koqZuTdcZhknHT = 55827; 
float SjltmkcJTYUEGglMCLHPxZimhJUfN = 5.172455E+20F; 
ushort xDbkdYdopCUjAtaYCeikeETa = 64022; 
byte pWVWkzNjFkCzMDAfzn =  14; 
long TgkSGjLSKb = 17806627929917510; 
long jxOaHxIFzuMVDoyPKeFncq = 68881910230186122; 
float qWfxtkkiDfuooppfpE = 12.81897F; 
sbyte KSCFLbeTzNChFHi =  -127; 
ushort TwMXWHUZolzEasA = 57109; 
byte VOBjeKIIN =  36; 
long WpglIJMNWULLNSuUNiILu = 49451063193702328; 
short NnFpiyPLQgAqbcQxmpOUEAaoal = -2510; 
byte sKyHbYyhDkxYzREVFdGhmdNCuomn =  78; 
short ZCMzycKZetnLeqWfIxuwiSfLnXWl = 15666; 
long SmnyyMt = 9499275219310332; 
long zjGRVUPg = 61810230952359082; 
sbyte yQUAtnoCBeqqMV =  -53; 
ulong DesWtApbLiHMyQgGDWRpZQq = 70991769498834742; 
byte KLoRBdqEyJfIP =  12; 

}public void jLjYzxOMOQ(){
int QBWuUxsNfSPbSBMDhlUFqwGufqMYW = 516109866; 
while(QBWuUxsNfSPbSBMDhlUFqwGufqMYW == 516109866){
QBWuUxsNfSPbSBMDhlUFqwGufqMYW = QBWuUxsNfSPbSBMDhlUFqwGufqMYW + 526724;
}string nlkyoiqWhfXKlFCW = "IuVDnDKtRgsNdOZJbUaJgclnyYkP"; 
short uZeaxghYZJBdybZiesYbk = 12931; 
uint RGiIfpsVf = 74; 
short dqSBdUshdFknnUSQmAKz = 22754; 
sbyte YBSKpGYpoHBabgChQifObYtRx =  2; 
sbyte IcUQUBTakjcDfnVHOTQKMsUoNw =  75; 
string bnPBeJ = "NLfBmBWGg"; 
int NcNdDKwhoiFdALTTiWmc = 60; 
if(NcNdDKwhoiFdALTTiWmc == 63134){
NcNdDKwhoiFdALTTiWmc = NcNdDKwhoiFdALTTiWmc + 766242;
}short kwnijNcYuYMfeX = -8646; 
string HHQtIOJ = "BIgsTZOuYWUVwUMJVAjkhYcmIM"; 
ushort BdDIFp = 44030; 
sbyte GCYXLfaAhNNUgziEzMDGULgEHUV =  -30; 
byte fkINoYlnxSb =  193; 
uint JmtFudgFgzbVuwpRjHmRIDmujksko = 58750252; 
sbyte hUuBpaij =  3; 
string MuFQXFJ = "BJqgZYwjMWoxU"; 
float lBVpTHeIgyeJPGCmLbjHD = 2.33421E+19F; 
ushort VQynlJnRKsocnPOxa = 14045; 
long pGEjKEwLnhwOCOYQIuUPQRfnmjx = 23013421264621789; 
short mEInEECXC = 1427; 
uint XOHqUeABpXAgindHQKbDKEuQl = 243812; 
byte pLJWwzwVoiuNPXpeBe =  75; 
uint gSAXguuQetSkWZlQkNQIMCBHPVH = 71; 
long XOJjTjZoebejQD = 59097634932564782; 
float GqSikNqe = -3.438188E+32F; 
float mQtHkzLHHqLemc = 1.481959E+37F; 
ushort ezXAECqQdqLZR = 57589; 
long GhLIe = 85526406888383409; 
sbyte qIYRx =  -3; 
ulong RowwuVIBR = 67891491279876906; 
long WpVEVUmtsSckGSWTtKzuoV = 43068855351350775; 
long NOSWWMABgCpIOkCFFwx = 21946403710773097; 
sbyte KqAjyzS =  -128; 
string HjXmNGgbglhYIxcRxqRuRBssxkRy = "MKFFyPwQcJw"; 

}
} public class eeWbMJcgGcCjGf{public void SGQCPJ(){
long DDujIIatIwYNw = 29467483309499071; 
sbyte YqIDklgHMuKtzs =  -6; 
ushort FJTOHtLxofZasYLu = 9442; 
ushort ZeQgBOmsMcSsduRgbnHqYXgVfh = 40960; 
ushort ZZqWHwuPCtZubZ = 24294; 
ushort WHCSJfBeUcXVathSWMdCwxZ = 28705; 
short VIWWXOAskaSEpTQS = 14994; 
short TBZRCenpUsAIeB = -20332; 
byte hYejwJihatwjLDLHZzYWQuRKzF =  254; 
long OcgKXLOnmzSR = 6032794247899551; 
string clncO = "uMRJyP"; 
byte oBOSXtMZfef =  217; 
sbyte qwjWsKaVwUWITCHjtUpwEGxXS =  38; 
short fjWV = -32075; 
int xzCyDptfTVoAk = 9230; 
if(xzCyDptfTVoAk == 540637){
xzCyDptfTVoAk += 696960;
}short ZmeKqXPlYbHqhkLSyOfjBE = -31002; 
int hKjzLtp = 6586; 
while(hKjzLtp == 6586){
hKjzLtp += 758218;
}byte ETUUJichkTOO =  151; 
string wlCCjmGO = "IqcAOBuKD"; 
ulong HJgUZeDZFWFLjybZVlb = 59293584983758502; 
ushort zelSzENeICeLHipcGTcNB = 31702; 
uint HICaUDTCiofStTLTn = 260181347; 
long wqWsiNnCHjZHKlRcNjfJECsdyKN = 24922000752034343; 
short JMQGZCeflJHTRBLhbsT = 31910; 
float AZfshEOTebCydzpdFamXlafosY = 4.926489E-33F; 
ulong qpMPlQAIu = 21113562832658613; 
byte YOTMVIAURajJKgRKkMolBEWHN =  156; 
float CXYCIlOAzFQgNV = 1.214874E+10F; 
byte IlNVgIhaqwRmZoOdsTbJbytsl =  158; 
float AFUpxVY = 5.694112E+12F; 
int ptIVtJkGCtJDPFBQjSwJBNZB = 51716195; 
if(ptIVtJkGCtJDPFBQjSwJBNZB == 68361){
ptIVtJkGCtJDPFBQjSwJBNZB = ptIVtJkGCtJDPFBQjSwJBNZB + 331474;
}sbyte MdMotDaKaauOQ =  -120; 
string VfOQnXpEthjSgwDjnoRMTl = "ISLIAtkKKxRUYD"; 
short PNhVlt = -22481; 
int zjBCksRKJgXDoQkiyAw = 5036; 
if(zjBCksRKJgXDoQkiyAw == 980885){
zjBCksRKJgXDoQkiyAw = 156644;
}
}public void YMp(){
byte KicbiLhAdZ =  53; 
short qEAXXNOUMlfUDC = -7801; 
sbyte fuySntmPbypZs =  -45; 
short kIFDbPDyuX = 29151; 
double HjOpKU = 1.599386E+27; 
while(HjOpKU == -1.144048E-23){
HjOpKU = Math.Truncate(HjOpKU);
Console.ReadLine();
}ulong ABcWjMzqkLowsAyFaXNuiigWEaqT = 46796582559383144; 
byte pREP =  71; 
int DqS = 39; 
if(DqS == 4919){
DqS += 379035;
}ulong tftjwFSIkzQ = 52978070314582826; 
short EVaKANeLqQlCeadKgxZXHtDNpNc = 32676; 
float eExwPjsAWSOWTIEAa = -2.48495E-14F; 
short ARwnktnuMKeSOMJe = 11880; 
byte qHBhiKAUjQNZJEcFNagFRefjhK =  21; 
sbyte VuqSxYTZJmFLzeFYLwnLhVdDHcl =  7; 
short AGu = 31005; 
short xgmGjGdODTsaCl = -4445; 
byte uOZqOltMqJJNZqBQiCEiICCS =  76; 
string LkaeElPZjoeJVTUSWwbDCSk = "KSfhSmhbOAJB"; 
short eCkspBdTJpOKGxnzkTa = 10170; 
ushort gHhXhdVxjtBqpBsjCKnQjP = 25063; 
sbyte LNpnPBJkuuEjstaYlfwj =  -60; 
sbyte faqptEBqmZpypGhwSRgWfOa =  -84; 
short SXKYYtEajm = -2833; 
byte HTdpkcVKteL =  141; 
short jHqaiY = -25871; 
ulong InaXTpwUXICBdczPgx = 9398666857326586; 
ushort momjUpwcjecuXjOVQHgFTkbCCm = 44940; 
long uLAXDIIusGOHwGcujBKkBRGmOZ = 71819063757099343; 
ulong DxuDfByatXyGZ = 41567658357559610; 
byte EGuKKZh =  228; 
byte JRNfheatGUQCxoDqUGnSmnqn =  209; 
double QObkclRqqFHSuqjAsscjfh = -2.908792E-32; 
while(QObkclRqqFHSuqjAsscjfh == 1.251346E-10){
QObkclRqqFHSuqjAsscjfh = Math.Ceiling(Math.Asin(0.2));
int[] OMHJxWKxSoWReqHbhGVJFNFhWGR = { 8824264 , 30519 } ;
Random OTAFBECzMCiIZqypbtIWgRty= new Random();
Console.WriteLine(OMHJxWKxSoWReqHbhGVJFNFhWGR[OTAFBECzMCiIZqypbtIWgRty.Next(0,2)]);
}short hAM = 20990; 
sbyte JZPqqCQipHymKmpngEXVHWAbLkm =  3; 
short PsEcJyBKc = -24126; 

}public void YBMaHG(){
string TshTjoMAOHfnoRdReKHFS = "yBQPJwObdFiQzUHztOXYNcX"; 
float gxVVXYARfVIcwGpMMDBeztjjH = -3.70504E+27F; 
long auEYXDZTfgHKZsYZxYhHlaBxNsN = 7822039859245195; 
uint eydq = 625867431; 
ushort wAblkksLaJZThkGPMiRABScmxW = 21689; 
uint ptueD = 8687; 
ulong Aeo = 47158978857868288; 
byte LPWuDyOPnTQGKoFOFS =  186; 
string JNjeaOgPayKRLJFiiQxXlk = "zQhYCUqc"; 
sbyte TbcMoDJI =  -126; 
short gPYHaFxbpfByCL = -13975; 
float GUJuEGSHCyTnNpkXPsbisBoU = -1.480734E+23F; 
sbyte FCaaKUcfGbDi =  -96; 
string hDOEPoBNtnKyeGO = "OBAbaxTESneqN"; 
ulong cqcgqllqddOINaxXBRIPJIYyLK = 1312053182099967; 
long bzxhCj = 25550797624724595; 
int fajwtFwIcCPUVOdq = 3247; 
if(fajwtFwIcCPUVOdq == 386474){
fajwtFwIcCPUVOdq += 495611;
}byte lakWDGokGIVEklNx =  13; 
ulong auCcA = 19821724170699164; 
ulong ituAexlFnmSBfgFAZFulf = 10263693091516255; 
string oPBdtcEKJOPmQHkKe = "VECkCFzwJoqbcNkM"; 
int FckxxVLypyheZdpJWAZdnV = 96260954; 
while(FckxxVLypyheZdpJWAZdnV == 96260954){
FckxxVLypyheZdpJWAZdnV += 649533;
}ulong bddB = 17358681520482308; 
short nEo = -15893; 
long RMMAyJymchDd = 7681303016978083; 
ushort stEiHFLQZfWtkps = 37724; 
string FdwZdxsbFjCQnz = "ttsVncSDmhygtcfMywEJpctohQFaB"; 
byte RytRXaJQ =  90; 
ulong iRHoZYAzeuKQKqZc = 4873861764360323; 
long lhMPGlVPUCJSRGCcjQgIJBXXUsUUA = 32130059410215977; 
short zUUxzuWHSqZPizbpLkOUmqa = -12552; 
long HlRTjpUbJOPdzEYMbEBu = 7814159607623526; 
uint TFMRNK = 98187305; 
sbyte UgQnCIMkWCoyQpXeHhRds =  0; 
uint UkqIQnWcyRAENqhtw = 47135711; 

}public void RxOwUIcIhdquGntqqduIFmaYRtspZ(){
uint FbCKzxVOhknVtBfXzYRc = 3700; 
float FwQFBpNIKwMse = 5.635852E+28F; 
float olxpqHyLwTsiw = -1.462433E-22F; 
byte edGqnS =  117; 
int YONTTeWIc = 38; 
while(YONTTeWIc == 38){
YONTTeWIc += 353017;
}float ywAod = 3.111237E-29F; 
double QnebuYKfIKYDZmmN = -2.670103E-33; 
QnebuYKfIKYDZmmN = Math.Ceiling(Math.Cosh(5));
for( ; ;) {
Console.WriteLine(-1.284449E+11);
}int FjnqwKIhFCGXUVwfQocLMHWhOARat = 5666; 
if(FjnqwKIhFCGXUVwfQocLMHWhOARat == 164673){
FjnqwKIhFCGXUVwfQocLMHWhOARat = FjnqwKIhFCGXUVwfQocLMHWhOARat + 636217;
}short gVibuxfnbbINHGcEV = -15730; 
double gDVTIOsYK = -8.729714E+17; 
if(gDVTIOsYK == -1.462968E+07){
gDVTIOsYK = Math.Truncate(gDVTIOsYK);
Console.Write(gDVTIOsYK.ToString());
}long RJJZWIhMDGZEtLmLzOLSPn = 72808840762267042; 
float sZGICRJpdwSYqiOYceMbSSWEF = 1.148766E+24F; 
short jLoMhkteGiHeuOuxfGaRQVWDSY = -239; 
string cPbt = "TLDmiXwTxWeXyd"; 
uint zgQzdo = 74273753; 
float MocEuGMY = 1.132E+14F; 
float ycwWAayUBxCVFJkUtFWQtYlgRS = 0.02057342F; 
byte iTmGJOcgPOoZJpNg =  249; 
long RFpNcAzQofHzX = 70159387032299616; 
float PhCuoWtLT = 6.583792E-32F; 
ushort TfjfjXObipspJceyaLYZtBGCfjU = 59874; 
ushort HTnfuEzTQAdjHRidHTomTgg = 26593; 
uint AJaadSHwKzbD = 52500774; 
double UAWiUO = -6.884358E-36; 
while(UAWiUO == -8.334211E+09){
UAWiUO = Math.Ceiling(Math.Asin(0.2));
try{
Console.WriteLine(UAWiUO.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}string iFONOgIXalsFwXI = "LcgKuMKNBaILgqlEQKdAHAQ"; 
byte XsgGAmaofxCiudyUblNzXQKZP =  198; 
ulong igllVRxtDgbOHwbULwxGGFGmdM = 76812516388666547; 
sbyte kpmDIesUkZSDPPousgnECYijWoLwo =  -51; 
byte ncKwhTYCmLcdUstU =  215; 
ushort uTEylgdGttWG = 59448; 
ulong nMeUlzhMpRWe = 637255926098391; 
int GynemGhqegsDeHPXlUlgn = 30795489; 
while(GynemGhqegsDeHPXlUlgn == 30795489){
GynemGhqegsDeHPXlUlgn = GynemGhqegsDeHPXlUlgn + 437185;
}int OYTMFfHHgkDE = 724970533; 
if(OYTMFfHHgkDE == 734302){
OYTMFfHHgkDE = OYTMFfHHgkDE + 490743;
}short GGc = 8643; 
uint ZhlpkiGYqiAQAC = 33; 

}public void KJDoU(){
uint cDhCMiWlmif = 400428; 
float uYawHmVZNToBsXWgfHVmIaemhlmEq = -2.323003E+13F; 
double KoFoAUcgDu = 3.812252E-05; 
while(KoFoAUcgDu == 2.058768E-18){
KoFoAUcgDu = Math.Pow(double.MinValue, double.MaxValue);
int? mnwSyLsVKCmBdPUczexVGYkueHymN = 7929041;
mnwSyLsVKCmBdPUczexVGYkueHymN += 33932;
}byte XLdNtndaNQmfikFAOb =  173; 
uint gjUFTp = 23985605; 
float fbiBmJYIRqVFenkEyLTzNIMyQ = 2.625491E-26F; 
sbyte XhZUYhdaLqQQuSik =  7; 
sbyte toTkhueypLKMAQhTiVL =  -94; 
sbyte EMtcIt =  77; 
int oxEPkeXSA = 697764617; 
if(oxEPkeXSA == 4987){
oxEPkeXSA += 606912;
}short lZZ = -13557; 
ulong AlMgReNMIMywpVaWPldlmxtnMOMI = 51859780342345189; 
uint fNNhcbSFowZpjOlFKcSyudtfFRhA = 840627527; 
float xRfpAfuqwhzqIQH = 6.992415E+17F; 
float cLcDcuHDlTsIyFDTtJVeFJC = -3.998529E-19F; 
int yOks = 6033470; 
while(yOks == 6033470){
yOks = yOks + 388388;
}short WSDh = -2881; 
string aMfWmgRPitjaoxsj = "bCFFuQQAeFZmbBdYOGpsRXkW"; 
short uDuYHEIOgWzUGWoyhCSChTfbCRc = 22405; 
double pIpA = -1.966102E-20; 
if(pIpA == -5.121164E+35){
double gATZtcuPJeNNCx = -3019800;
pIpA = Math.Round(gATZtcuPJeNNCx ,1,MidpointRounding.ToEven);
object QRcHRFgCcscExXUAZC;
QRcHRFgCcscExXUAZC = -7.113539E-05;
Console.WriteLine(QRcHRFgCcscExXUAZC.ToString().ToLower());
}string NfJQ = "cyQQYSbnPanatIlFuwDiyiATsY"; 
float BCBUOTRtWTjcNiDAunAkOytK = -5.295773E+10F; 
ushort YXyZamOmAtmAQTIBlBwoHZzeAH = 274; 
string RPfyWuZ = "jbBuoabbZuVPSy"; 
int gnYztBum = 9524721; 
if(gnYztBum == 134467){
gnYztBum = 38070;
}float JfczVUqZycAlenp = -2.614592E-26F; 
float JxCcJHnKjsx = 6.471396E+34F; 
uint gNwgpmWZapbihyP = 4196; 
byte kcgctSIjhHbiCLIgEYbFdN =  109; 
uint IzfmwdaImpwaLboIcPsYgAy = 318462; 
byte uIHzGGoMtDW =  188; 
int Bow = 1196; 
if(Bow == 743994){
Bow = Bow + 944829;
}string dDGiaGWjMHaGSPwwpVffcPXHjcnj = "uIKGBDcJDtXa"; 
int uBKDyLxGRRVVoFZZYqMQsZpZq = 3586; 
if(uBKDyLxGRRVVoFZZYqMQsZpZq == 548024){
uBKDyLxGRRVVoFZZYqMQsZpZq = uBKDyLxGRRVVoFZZYqMQsZpZq + 167492;
}sbyte XEgAARAWPQSLGnyJ =  17; 

}
} public class GnNAwjLljsKSo{public void PwqbTAAAmUDiynMZVdjhD(){
float ziXPnHgmWCjWjWxQeVgaSWl = 3.44077E-06F; 
uint yHIgfXJT = 927318549; 
long SgIGInsDc = 17168528885090679; 
double sOxKGoud = 124.4456; 
if(sOxKGoud == 3.758387E+28){
double kpEyeXiYtDTKfOqWLsyxybqmjta = -4.311558E+12;
sOxKGoud = Math.Floor(kpEyeXiYtDTKfOqWLsyxybqmjta);
try{

}catch(Exception ex){

}
}int MOFMQAm = 2927; 
while(MOFMQAm == 2927){
MOFMQAm = MOFMQAm + 320242;
}uint lcADUdKmWmSKJGdk = 578874217; 
long xuFylJXNQfVkLGVByCNfcB = 52122666861885153; 
ulong JNdRzcpkfWyQqIZdKEOxRDKV = 75938015011165052; 
ushort DXCdcw = 13251; 
uint fdT = 31; 
float XIkpXfdiftDUgQwHx = -1.129684E-31F; 
string GCslpDERSWjGEsTGd = "xUtYJSp"; 
ushort KxxXkPZs = 8872; 
float IEAq = -3.61811E-06F; 
ushort EzDbtcJhiI = 55570; 
uint WsUwwZYQFaMMaCSucLmAbRLhwdU = 649139; 
float othLlGVSxwyZzQosCQNfX = 3.99521E-38F; 
float INasf = -7.557762E-27F; 
ulong ytTQHPgSQeE = 37500906916898065; 
long KsmHESVAqYolhtVAMiXZePW = 44221288926327441; 
byte DObG =  46; 
string ltzfoHeXjlHDXMJUwXtjxaTFnbiG = "UTDgaOMpRbHoyKwfgpq"; 
float HJQTAkAURnyAspsexWXXEEWNUZ = -0.000362666F; 
uint LSLMnk = 25388533; 
uint lxaNiQfucbLutWpcM = 97; 
uint cxKDGOHcgsjwwMgEiEdTWwyxpzO = 221242; 
uint nZIjZpjEbzqVJCYYkizEA = 645025970; 
double ZmlgZlRIKVxNDhnYacuF = -3.097359E+19; 
double itsKDadXxcmBmZGDkJbjFASmthk = Math.Log(1000, 10);
ZmlgZlRIKVxNDhnYacuF = itsKDadXxcmBmZGDkJbjFASmthk;
for( ; ;) {
Console.WriteLine(-4.33863E+29);
}ushort MdQGnyBAeziJVZEVSfoWxbeq = 34274; 
ulong wQaYMGqzXNuTHLpl = 82299499721487859; 
double ztL = 0.4800704; 
if(ztL == -8.816197E+31){
ztL = Math.Pow(2, 2.1);
try{

}catch(Exception ex){

}
}uint MjZynZnelxNaWwJ = 26653509; 
byte LhcTmCpsbWXWWKtlwBjJARLK =  33; 
long oXkXEAh = 50975268210664651; 
byte nBRqDjn =  63; 

}public void FqUyt(){
ulong tbwQYtyOVeDdOmLSFDw = 72026579167973502; 
float USpFgDWmxjUdcxzHjzRUAoVXf = 19028.58F; 
short SoUDXammzILOFhsBDOun = -24095; 
ulong IasFWtAq = 72289272986462983; 
ushort tLRgfUhEx = 35947; 
string bBedlDF = "pUZqLgaFyBcIZb"; 
ulong uCUz = 88296574427940013; 
uint OYBaVAMAwZUDfVBtiZNbJKIgX = 943514642; 
string TBV = "GMdgZ"; 
sbyte qUfUEUmby =  107; 
long qyieptcYDKACzYyBjxm = 11488588845564515; 
string VGekUFJyIONXhLIwExGyN = "aKzMRNuBDQZoHeuqEuPcCIAmM"; 
ulong sZqX = 11027558971580192; 
double hlXX = 2.2631E-33; 
while(hlXX == -1.027338E-19){
hlXX = Math.Ceiling(Math.Tanh(0.1));
try{
Console.WriteLine(hlXX.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}long NwzLqjwoechgDToDEawYpUoXgxNPo = 81350890754450800; 
byte fRwJJYpDVZkCnCLJSgBFVyGa =  132; 
float aSKOssjogUkGYQTS = 9.347425E-34F; 
string gxapuJHUJlwhOApKklSidlEPHBD = "cGQWzatFSttKLpHUzdRAAjHE"; 
byte QVSlJTgRSdxAJOfMSRSiyOGJpE =  171; 
string SYlMGjyMGL = "DTyDzZudtOqPLcwc"; 
short WfkdDZe = -19831; 
ulong FJZlg = 68970054434326431; 
int BBiBJjgiEKFoAqzznlpzL = 9986808; 
if(BBiBJjgiEKFoAqzznlpzL == 421041){
BBiBJjgiEKFoAqzznlpzL = 830344;
}ulong XGWFtbzWK = 85078654544968556; 
sbyte SFWeSgZNw =  -1; 
sbyte swWGeJKfo =  126; 
uint hxaktDgnoETIbCdzuo = 42; 
uint CcEQzTubXScqWxfPtTtUYPCoF = 591306; 
byte aXfdqVYHDdnTmHbFKFIsGV =  45; 
string SNBokVnipXVXnGChhQP = "bMiAL"; 
float XTfeOcxziysqiZLWJgIwZgkyeuYG = -1592510F; 
string qXqokSgURgtKQzHeqQ = "tczYRJIZueRsFxOa"; 
byte wMQZOFTOBIGNToqeICSZB =  158; 
uint NPaOPmczxxVdKuoUfdhauHkDbA = 23298052; 
sbyte hPjXlYEe =  122; 

}public void GhqytmwmlX(){
float zCzgEyMozQpkngU = 6.120726E-27F; 
short adUwlpOiPeC = -32661; 
byte LpIzfuftKPPS =  29; 
sbyte sJeVHTtEg =  -66; 
ulong oXjWQfTDQmncWkIueGujFWyx = 77730219504107651; 
short RxBTZNmHxfucYxlZzB = 23584; 
int tBOALCwk = 71; 
while(tBOALCwk == 71){
tBOALCwk = tBOALCwk + 374875;
}byte lHIXYAqbuggktnbtJuyWMtnkRw =  93; 
ushort oHdaflTkKlUi = 48052; 
string wjSQKKeXVohTtxfEfX = "fAdkmbihZhENaoJQIEDKJ"; 
byte hMYtAPEJOkcZesZFPCCeQ =  87; 
uint EKHzaD = 140; 
float VzoIJswzQDXpFyIYOhgfTQjRQ = 1.909718E-27F; 
sbyte kXbdnxBohSjJqCZjnfiQb =  72; 
sbyte PXfozRTkpQFFVlpbgJHoXxCnjI =  8; 
long oIxShoqpkRpZYtCXuncQhKshQXswS = 25787838397681981; 
ulong XYLVSqpYdaqPtATRFonbkWV = 30344894536198664; 
ulong BeRyOARAYouBqyQpwxTU = 58161356919773933; 
uint MIDhTucuR = 44404175; 
int HCXPCjzqV = 872007; 
if(HCXPCjzqV == 84101){
HCXPCjzqV = HCXPCjzqV + 773548;
}byte HBYMEMXiFwnbGeyiptCnaqgcVQiKk =  231; 
short jShXFBdPaG = 28878; 
ulong opuwOILm = 81722848805642180; 
byte KeZSNWBEPpNnwuqSGBPnymGbmVREK =  178; 
short MImKlyEyV = -20060; 
float BIDWcdkmOaCxA = 1.527412E+07F; 
float XUeZB = 6.709851E+18F; 
sbyte GiWqtmukCGtVCGAdyMgR =  -74; 
byte MsXoOwZfPPwybpyHnbnxZzjxoF =  232; 
string HpxspkYwXHPDBEggizOZeSPWS = "miCajAVohAYeMfAlqwzTzUdpfg"; 
sbyte MOBA =  0; 
ushort PTWxoHlwHhdMIBTCuzYutflUcsSD = 25278; 
double FohKsuYimmzxhcuA = 1.084481E+27; 
if(FohKsuYimmzxhcuA == -5.769177E-37){
FohKsuYimmzxhcuA = Math.Ceiling(Math.Atan(-5));
try{
int KhTzdmEaLsFl = 5750772;
if(KhTzdmEaLsFl == 18947){
KhTzdmEaLsFl++;
}else{
KhTzdmEaLsFl--;
}
}catch(Exception ex){

}
}long AYUOj = 84800101390093666; 
byte yUEmLTEsdiMntTagiT =  36; 

}public void HDbpjOcVDoTKCqmgaWakWHQGzhRcQ(){
long YVpWUTndXRIPOpbMbHT = 57414678817188655; 
float WZeT = 3.361173E+30F; 
ulong utmMScunR = 12888281415411503; 
ushort ZmXhy = 28252; 
string REW = "unAIm"; 
int tPDOiKguHhDhp = 8003; 
if(tPDOiKguHhDhp == 448187){
tPDOiKguHhDhp = tPDOiKguHhDhp + 222099;
}ulong RcDLVXUiVhj = 42906716505929113; 
short wTn = -5405; 
int FzCjbQeDNZdnNllLZtRhmAsAJ = 49361711; 
while(FzCjbQeDNZdnNllLZtRhmAsAJ == 49361711){
FzCjbQeDNZdnNllLZtRhmAsAJ = 276666;
}short foKuXhBdzD = -3069; 
ushort LeQLTVFyFfgbCnpQ = 9414; 
int PYIWCpbkqdOXUeiduzKptK = 523699138; 
while(PYIWCpbkqdOXUeiduzKptK == 523699138){
PYIWCpbkqdOXUeiduzKptK = 433514;
}float nqYNGCdi = -2.635803E-08F; 
float ntQilXsjVsynsehfMXXGq = -1.791517E+24F; 
ushort hzHaljozJxyLbqwdKnVTLuCuXxpyl = 24081; 
sbyte sjezMjKihQJIKuLHCC =  33; 
float EmNXmcQTRmppUtRmXsAFNMVMTKD = 2.576135E+14F; 
double MoIPXqCAgToqEtsPbTtFj = -1.592958E-19; 
if(MoIPXqCAgToqEtsPbTtFj == 5.113173E-12){
MoIPXqCAgToqEtsPbTtFj = Math.Ceiling(Math.Sinh(-5));
try{
int SpqBGbBWFpyXHQfwa = 3014931;
if(SpqBGbBWFpyXHQfwa == 61317){
SpqBGbBWFpyXHQfwa++;
}else{
SpqBGbBWFpyXHQfwa--;
Console.Write(SpqBGbBWFpyXHQfwa.ToString());
}
}catch(Exception ex){

}
}double MVqZqEjscUShiRAQzFIVpHFmSYR = -2.656129E+25; 
MVqZqEjscUShiRAQzFIVpHFmSYR = Math.Pow(2, 2.1);
Console.Write(MVqZqEjscUShiRAQzFIVpHFmSYR.ToString());short PMgaAgsGxhQwAGwd = 22148; 
long dbEEmgNWR = 64410005057199676; 
sbyte XZqkAAhyK =  -76; 
uint xIdoylhU = 742845194; 
byte blAlkxwyXFMmMReef =  170; 
string otOaBWHZQlskPQjmZVmnPd = "RVQDGgdPkK"; 
uint JyzMoQxIL = 5225; 
uint IRekxPWkeXfKtaSNyhBqFCPCiYz = 4244; 
int usHYpZEPVCaEBtcVPF = 254588921; 
while(usHYpZEPVCaEBtcVPF == 254588921){
usHYpZEPVCaEBtcVPF += 327787;
}sbyte Eby =  -48; 
int CsLqMbCOTsGQFXohfcJPBpAu = 3076; 
if(CsLqMbCOTsGQFXohfcJPBpAu == 416948){
CsLqMbCOTsGQFXohfcJPBpAu += 540377;
}short YkbhJMiOchtNupNiMwW = -6185; 
long okTqexilsqaaRtVgAuTVMtW = 49385838003892935; 
double kWRoqHNFRQcMuAtmGWDCMbWofqjT = -8.714821E+11; 
while(kWRoqHNFRQcMuAtmGWDCMbWofqjT == 2.215714E-32){
kWRoqHNFRQcMuAtmGWDCMbWofqjT = Math.Ceiling(Math.Cosh(5));
for( ; ;) {
Console.WriteLine(2.935531E-37);
}
}ushort gtueMNIcMoJKR = 30060; 
uint IhomlNeEunEpoinnaQ = 62; 

}public void dnKlcw(){
byte XAwyFfIBgitoRLzjsNEa =  101; 
long xRx = 45371361197958871; 
ushort iSKcHNTIJsptQBToSimnzZHi = 35189; 
ulong fZbuVnJbBdBuHQuNAfkdeiUAMIi = 49418088683209175; 
long XwegztlsPGzelgdqMQWe = 2084689362011537; 
double PbMgCKfWGQJhWbKlX = -0.000398149; 
if(PbMgCKfWGQJhWbKlX == -2.300792E-36){
PbMgCKfWGQJhWbKlX = Math.Pow(double.NegativeInfinity, 2);
Console.WriteLine(PbMgCKfWGQJhWbKlX.ToString());
}ushort eHluyFbZjmbLaRfWzbqIFEClQ = 24569; 
byte gZJOemULGqKfCXttMjFLLhPjNN =  64; 
uint aEdqi = 202195818; 
int GNXwphqlf = 19294766; 
while(GNXwphqlf == 19294766){
GNXwphqlf = 776365;
}int YjAbsZt = 60346132; 
if(YjAbsZt == 770414){
YjAbsZt = 379429;
}string PzFSNpR = "SCHHqXAXpOZUbfknNEPL"; 
ushort yMDatbbWQSgCIBLtcJfPpZ = 25045; 
byte KmpejBg =  65; 
sbyte VepE =  -11; 
double YhNAyLDuBgGGx = -3.162715E-13; 
while(YhNAyLDuBgGGx == 5.107625E+13){
double qanCyQoNeISWblyJlDNL = 8.213861E+16;
YhNAyLDuBgGGx = qanCyQoNeISWblyJlDNL * 2;
object KGDNyVJkCsQMlWnNeOcX;
KGDNyVJkCsQMlWnNeOcX = 6.413612E-10;
}ushort naHVqETijyS = 15642; 
ushort lwwSmFdxBDyTZf = 6827; 
ulong jOhYXBbGWNJIQKdMDwZnE = 86094664845760663; 
int dxOJzpzoVtmpRuPLStgxKfz = 70; 
if(dxOJzpzoVtmpRuPLStgxKfz == 129070){
dxOJzpzoVtmpRuPLStgxKfz = dxOJzpzoVtmpRuPLStgxKfz + 454451;
}ushort gsYFkbbFwegeVqiaRc = 52551; 
byte sheYY =  165; 
double wqRzPfYSQFJQPLsDbE = 4791.407; 
wqRzPfYSQFJQPLsDbE = Math.Ceiling(Math.Asin(0.2));
Console.ReadKey();long CIsJAgqQUyjTgSJDWipBXndXp = 15813593661452905; 
uint eubffdHJVDiAIfIllbjVmFw = 8473; 
uint MMfDwHfEhUdjypGhugiZtyWfF = 5968; 
uint ZJnHlVWPZNSDCBqi = 741337214; 
int xalKzzoQaYgbGeilPEku = 36096867; 
while(xalKzzoQaYgbGeilPEku == 36096867){
xalKzzoQaYgbGeilPEku = 446135;
}float GWNCaSdYxFhOHgPbHQXyseunCkZd = 1.381058E+26F; 
ushort Hsk = 5377; 
byte bNajPkwyNCEtOBBuFBC =  121; 
string TZsFlfgoWLRaOjxhojSdyyBl = "TClNK"; 
uint FmUbRkZpqzFSDNMpXxlsGX = 4048; 
uint BsFUCm = 9; 
ushort owpejDNZTIKsgkopz = 12418; 

}
} public class IVADLL{public void LxWsKzqjbCgghcjqDRx(){
sbyte tGkPYYOH =  74; 
float tESqdBYlaXjNjqbdyEX = 2.316555E-17F; 
uint qRLELADElJCOF = 8746; 
byte Tkh =  71; 
byte byXVpcSipRiBAtXmMztXeJEyS =  148; 
short RdwghYKfOFmCuBkWxLtVeHAJVmN = -9004; 
float dMMWqlxbtCJRAglR = 975304.8F; 
ushort ahkwRkFMkUlqlmCBQyjZ = 32241; 
ushort SlmgsydKiVmA = 60170; 
uint ffHLRzosGlG = 3255532; 
sbyte zhHQsjEemnhjc =  -25; 
double GZAsdsPDTDTIVMMcFOyfSfi = -1522.288; 
if(GZAsdsPDTDTIVMMcFOyfSfi == -1.76424E-06){
double HyaMDNqgtdXPpViCDfGoXTVARtQc = 5.091684E-18;
GZAsdsPDTDTIVMMcFOyfSfi = Math.Round(HyaMDNqgtdXPpViCDfGoXTVARtQc ,1,MidpointRounding.ToEven);
try{
int pWbyMaARxqMPitQHSSU = 5287512;
if(pWbyMaARxqMPitQHSSU == 60225){
pWbyMaARxqMPitQHSSU++;
}else{
pWbyMaARxqMPitQHSSU--;
Console.Write(pWbyMaARxqMPitQHSSU.ToString());
}
}catch(Exception ex){

}
}ulong HggqyDijezoVtWnVkfqThhMFQU = 24411421889165552; 
short wjyHPedDVyfyHQFwoVM = -1418; 
ushort BDfxdcfpbpFIBbJKe = 3351; 
string JtPYgqz = "KZsTjSKUBsbbyFwPTYpEMFQJumTg"; 
ulong pNEhGc = 68567649400751912; 
string LeYZ = "hbVkf"; 
sbyte AXYDgtcoypYtSiCIlXweI =  -45; 
uint iObhMJWBTokBKPJQJMGwfFhW = 37985539; 
long KdahwiusqV = 10581560541269702; 
double nGwVLgKFGeCsZI = 9.462683E+22; 
if(nGwVLgKFGeCsZI == -4.363557E-32){
double OiRcIeGZGxmZeJfy = 2.941774E-10;
nGwVLgKFGeCsZI = Math.Ceiling(OiRcIeGZGxmZeJfy);
object ltkQLIKnUWycYgHMTecAafV;
ltkQLIKnUWycYgHMTecAafV = -0.2119656;
Console.WriteLine(ltkQLIKnUWycYgHMTecAafV.ToString().ToLower());
}short gzpdcWEfogxyuUmOSNceKa = -17195; 
double HOfQcFEnSfWIYncRl = 2556.222; 
while(HOfQcFEnSfWIYncRl == 3.499023E+10){
HOfQcFEnSfWIYncRl = Math.Ceiling(Math.Atan(-5));
Console.WriteLine(HOfQcFEnSfWIYncRl.ToString());
}ulong GSzIgUCHKJjUuSOAjOelfin = 47415641629845575; 
ushort CTGsqndcnSqF = 36468; 
string oxkVcBIxFS = "LIIZcJOCS"; 
double gTBajRp = 5.199009E-36; 
gTBajRp = Math.Ceiling(Math.Cos(2));
Console.ReadKey();long EhGPXFPDCnlQeKyUeJqCdotKKiNpB = 46370242312780371; 
int yCmGtFFoZIUDWdh = 273759585; 
if(yCmGtFFoZIUDWdh == 101878){
yCmGtFFoZIUDWdh = yCmGtFFoZIUDWdh + 794578;
}float ctkQySYzjClTZQKZ = -8.008654E-32F; 
float JGGgsksxjOyIHckmhPgyaiK = -4.299362E+15F; 
byte eEPRaW =  13; 
ulong NezmADnwdEmXGy = 59903862499768396; 
ushort dnSuXyM = 37262; 

}public void OAdqUYzVn(){
double WjzgYC = -0.4579694; 
if(WjzgYC == -1.393417){
double kKTzHSKLS = -5.898064E+12;
WjzgYC = Math.Ceiling(kKTzHSKLS);
int[] ypFIHo = { 3241224 , 39902 } ;
Random TQMxwwloVn= new Random();
Console.WriteLine(ypFIHo[TQMxwwloVn.Next(0,2)]);
}string NPcPqomAHhkDYbYNSCteow = "ydlGwQEIsYtKCCSpaYuiFkTyjpiw"; 
ulong Vws = 86544306432286183; 
uint FGqykaktgVlsljPwfHiSqcXsKX = 22; 
short KfdsoR = 23627; 
float wFIPBfAtDpoMcUJBCJZOnUZdZlgo = 1.5325E+31F; 
double mJOmKmxlkEEUbuIlw = 1.250153E+19; 
if(mJOmKmxlkEEUbuIlw == 4.545112E-33){
mJOmKmxlkEEUbuIlw = Math.Ceiling(Math.Atan(-5));
for( ; ;) {
Console.WriteLine(-1.219908E-36);
}
}int EMKpPTbKQEJY = 17; 
if(EMKpPTbKQEJY == 456855){
EMKpPTbKQEJY += 153394;
}long zVlFTknoGUFWxTWQCuoz = 12615781942304159; 
byte GtVWUIBexFFNdVYMDhlM =  173; 
string jlKfMXzqKeZolbAYnpyXLUJLUC = "SLqyXCBLLkCOgYmUIeHNLdjYwlf"; 
sbyte EYlkcGGtHbN =  57; 
short ONCpYzZaHmOjPCtcLwswhlo = 4406; 
string HCacTawpxkUW = "KqDsYx"; 
float eZFpqtVHCyYlGRyZEuOPzfhiP = -9.827406E+11F; 
string UqglCCDxMCHAJUOVLC = "TDOFJesYRxCcMRcIgRadoVESSg"; 
short kumdNQkcfMzgjELZjzVJHmIe = 13748; 
ushort YYWNDPCOyjypVHZCut = 56405; 
float jFItkHiXMhtomldiYWsIPn = 2.768205E+18F; 
sbyte bHLmMSERPuouqhsQMbeaJwWsxxBlX =  105; 
ulong jkFDlzxBMwxJUFFjoYDOIhqHKtR = 75408893098259405; 
sbyte QEmGEomYmzRSqWYtbm =  -118; 
float STHgitCDdBwyHAwohdTdJxUaxaxVB = 1.703421E-29F; 
sbyte FXKGAuVLw =  -79; 
double uKxDdTjiRpwZHK = -2.825933E+34; 
while(uKxDdTjiRpwZHK == -0.05685807){
double CLKJJ = -2.881692E-20;
uKxDdTjiRpwZHK = Math.Round(CLKJJ);
Console.WriteLine(uKxDdTjiRpwZHK.ToString());
}float kBlzcidcIRYnG = 2.613697E-07F; 
short NZaLUxYmUCpwTJeBcAyNWltI = 27946; 
sbyte aROWjVtVVSKqd =  -64; 
byte HNIPiUO =  141; 
short CTBGsONbYxfFgABNiItitALcMUnsl = -24781; 
string JktAwpAV = "bnQccGhoxMfjfFbhzT"; 
uint wQSsVWVgasSizGsk = 991614699; 
long fdQwYbEHqq = 19692219823560131; 
sbyte LbneT =  112; 
long mRmzjiusZyuUtoPJ = 51845954347491404; 

}public void FEePMCaoduBezmSaQlYcmMxlG(){
long QJcgbPHFFoEnVt = 37740960907705579; 
string TQwFThSibfkHbmzoSImL = "YdJJxBXbN"; 
byte HunlucMyyulQiYoIQ =  74; 
string bqwVMMWxIVhnSJZHZ = "VQWQnSMnB"; 
long ThMJMB = 30109861537097447; 
uint YXAYIJEuo = 208437; 
string jzlloOOxGghKHHNmJH = "MHO"; 
short LAj = 5283; 
short gpmxiaxUZbOQs = 21011; 
short bcQgGXjpWx = -22053; 
byte QFiekZSxkzNRPHZX =  208; 
ushort JxzgOkPifXAMSktj = 27845; 
int hRASQEdpQpksfbJCdsywTx = 745860516; 
while(hRASQEdpQpksfbJCdsywTx == 745860516){
hRASQEdpQpksfbJCdsywTx = hRASQEdpQpksfbJCdsywTx + 891542;
}int yEtenJjFIELVYS = 592129281; 
while(yEtenJjFIELVYS == 592129281){
yEtenJjFIELVYS = yEtenJjFIELVYS + 687889;
}short YjSMZMsfSiLwTHZbNZqzLULYy = 31408; 
float qbLVfHGimDJzmDLCjEZnGsXisRg = -0.2170271F; 
float aUyOQzPplb = 3.005589E+21F; 
byte TliiiOUZEpZ =  16; 
uint nBCKtnJEQOVdwnNlPE = 85900420; 
uint XUqfubphPNWlbMBgwXI = 4172; 
ushort etGlsBwqJd = 43556; 
int tmUImJZhhjlbLIhPkjjE = 22124690; 
if(tmUImJZhhjlbLIhPkjjE == 785755){
tmUImJZhhjlbLIhPkjjE = tmUImJZhhjlbLIhPkjjE + 956470;
}int xmhtiSOidabN = 1289; 
if(xmhtiSOidabN == 385969){
xmhtiSOidabN += 206089;
}long nCzUzYbgCfTpbHMUbTSTTZMkhx = 39236607751160145; 
sbyte PmUkQHHXhMLOwdhCsOZoPnNbQET =  120; 
long ZnCRfOYIcl = 21904385301479209; 
ushort lDOQeBBSumNJlTNylFYtDLMNWb = 13943; 
short uAKETcxBMMF = -15743; 
sbyte PzKDjIniA =  -72; 
string BQaTdpjCZOd = "tQpWB"; 
float fxyWomzbJclZz = 1013.657F; 
uint BSIndFmbwGjussIHRUU = 87; 
long GoBAOOGCQnKwIkfcSkV = 36332674204197881; 
int JzMPlcZEfAmPetfIWZxM = 35389193; 
if(JzMPlcZEfAmPetfIWZxM == 841818){
JzMPlcZEfAmPetfIWZxM = JzMPlcZEfAmPetfIWZxM + 977258;
}short iPjwZShPXHxhfzNLJeEW = 1982; 

}public void OFVUQHGOXmMsupoRQlkpK(){
short nHtohoNwufAbGAecBj = -20848; 
long xNexwSo = 27882919276454912; 
ushort zAjSqmUoKqLi = 45417; 
sbyte GuSToSMyzdJUAEFS =  82; 
int MKAqeBj = 72; 
if(MKAqeBj == 168835){
MKAqeBj = 20333;
}sbyte SHPwhAMbMOIswLez =  95; 
int uQgcNseLZWTVIPZM = 69101438; 
while(uQgcNseLZWTVIPZM == 69101438){
uQgcNseLZWTVIPZM = 990795;
}long EyiPxHbLGBLKLEbccPNeSTZ = 86546241712625249; 
ushort tApjont = 58940; 
sbyte hepaubtwVm =  14; 
float yGuxAHmCJpCqeMTOCtkKF = 0.001338628F; 
sbyte NteJWhFzPTEVRTnVOzPOJopAItTsl =  -100; 
int oyNniS = 14760620; 
if(oyNniS == 305766){
oyNniS = oyNniS + 853671;
}short KBWORtJqpydFbDnllJnxGufZNV = 15029; 
long QZykPKtwzolzXgokERkTLHHFS = 50223677950671760; 
uint zGumSVQOubYJnQPj = 52; 
ulong isTsC = 42767743356117956; 
int FjFsXUSmDUFbFhRSfmwsfMbLy = 335163897; 
while(FjFsXUSmDUFbFhRSfmwsfMbLy == 335163897){
FjFsXUSmDUFbFhRSfmwsfMbLy += 519256;
}byte ZjZzjRkpfqyXttqHBAjb =  67; 
long hUUZDClftstFXRuWxjbawFOeFZ = 48678617959317999; 
float oRJPUkdFZPuPgENRmL = -7143.081F; 
sbyte wERMicgZpGZQHFClfkBjePGV =  -33; 
ushort uhDUozAxfXhbbyazPQ = 21410; 
double oaFlcWK = 3.724505E+34; 
double LKqghxoYjAMeqQfcxwehYImC = -5.440225E+17;
oaFlcWK = LKqghxoYjAMeqQfcxwehYImC * 2;
Console.Write(oaFlcWK.ToString());short oniYWaVHNZcH = -6952; 
byte qGKqjCajASAcAV =  4; 
long zJFyzdGj = 6475967428134020; 
byte yOzhQodJgyYwLMq =  215; 
sbyte HYTCdgATFtmfQGlyEUNqTo =  -29; 
ulong iloXJAwEXQoFVKqIWKLxaHtjnhye = 85834407641011458; 
byte WTRDTPXSJkxJXGKtXDSfJCcpMfzL =  131; 
short wyZOSFzNElwzlJiVMULHscfn = -7234; 
ulong QOpmqswalXSSOzPoT = 52363840799060284; 
ulong xssMtcdJZnZegQJWXTcWi = 63881862146078686; 
int xCIZJUlzQKQQaUiVZ = 631058171; 
while(xCIZJUlzQKQQaUiVZ == 631058171){
xCIZJUlzQKQQaUiVZ = xCIZJUlzQKQQaUiVZ + 879037;
}
}public void TEfLbcjwxLDDARabL(){
ushort lsNNqSwZV = 40493; 
short JIgfePCxNBuupqGxiYFD = -435; 
uint ifQOTHdnPfcTVtLVmjXB = 96000961; 
ushort ZlZRXbNiiZXanaI = 53291; 
long GnnzKdycq = 59973901415997739; 
long mqOsTKVwLImChRAsfiCC = 72211919442044508; 
ulong UlaixVRVwO = 23875930252115347; 
ushort HjmeMTwdYlBRIyNaBYuECkONN = 16236; 
long GDmaIVIKZVyEANWi = 31147992945778926; 
ulong qxxAszkVzxZAjNQdbFCReOnGA = 61693346626629470; 
ulong knBEYsFoAjOOnzEcMU = 51833681319959369; 
float Wkh = -4.653336E-10F; 
sbyte PCbDAAAOwMcYVpAAWpb =  -126; 
string BcMAEsobBueTOCL = "yneuwnznWLxM"; 
byte siCsaYQZVgAtkJUbCYPESNWKbmEy =  142; 
float DQEbVsxBMCztXUbcQuVjzXIeFmS = -2.791702E+19F; 
ushort xdlWEHHYKezc = 2596; 
ushort YPZjOdIJyGizy = 43666; 
uint NqibpM = 476318108; 
ushort wkeaUGL = 13097; 
short pEHo = 18014; 
ushort puhjpkYLLTsemRCOGIyEyLE = 50547; 
byte xQjLUnVXIenYf =  1; 
sbyte jNtfblPPbHq =  80; 
short RuXVOiBZYejoixBHDxy = -22986; 
uint wgtjawKmWeHcmTbjlXPjOnWNd = 2682; 
ushort Usida = 63931; 
ulong FhepLheIgjIFskBtq = 1967418960662835; 
sbyte MyYm =  101; 
byte IQKUdgReWiHQNKSQNYRG =  237; 
ushort XaDftkiXNtLDKJxY = 870; 
ushort kABAxsTHkncGLjqMU = 33768; 
string UNlTxbgihpZRlwm = "yBgNAEXJKOZttXDzaVHymHdulLAVn"; 
byte pXqotunYqQDjJ =  12; 
ulong jNCRpOQMQxMBPdbuytJwbIid = 21419825888768989; 

}
} public class VohNwdDhFAXINjm{public void iMotsOCFzeuyibsbucTOUIc(){
short nxilwZ = 15029; 
string DLgYfhOEnqJfZEgVnKqAUBKZme = "tsceJuoAIUx"; 
double aquMHbnTcphpwEZbagMbQu = -9.950711E-22; 
if(aquMHbnTcphpwEZbagMbQu == -8.682785E+15){
aquMHbnTcphpwEZbagMbQu = Math.Ceiling(Math.Asin(0.2));
try{

}catch(Exception ex){

}
}uint seiqyNTt = 83760159; 
byte TKTyYz =  193; 
float JMNFmyNUD = 3.222494E+36F; 
byte ECXRbSykiNbiJfjZXFaGZ =  171; 
short MkofAdnnWjgsAg = -3446; 
double bBYLwFsDVFZYFKU = -2805.085; 
if(bBYLwFsDVFZYFKU == 5221.227){
bBYLwFsDVFZYFKU = Math.Pow(double.NegativeInfinity, 2);
Console.ReadLine();
}int gGkyUXMIDOBMbINUMhAESiK = 435361; 
if(gGkyUXMIDOBMbINUMhAESiK == 524505){
gGkyUXMIDOBMbINUMhAESiK = gGkyUXMIDOBMbINUMhAESiK + 425090;
}string xOsiQnBoi = "dSasgudYUgHKbWXVQbXi"; 
byte zCdYKzWG =  58; 
float WOqKcpDLDCX = 5.494492E+17F; 
ulong lVzAsZxStAoiKF = 29655588602823222; 
byte LuyIJCVqlmmbhNBqAVXkNhS =  32; 
uint cStInFKKOX = 776575; 
sbyte EXWxqaghmbFmJaaECqhwzdXFZ =  39; 
short ZEPMMfHQkwPBgVF = -9989; 
string WKK = "wkHOuBZs"; 
ushort JJiKlTBEtktfDHoDzGCYM = 42469; 
ulong iFeYMgQTYfRyMVfiBtpzQoJZ = 21363207536551084; 
double CIPDRmuBhxQRQ = 0.06765348; 
while(CIPDRmuBhxQRQ == -4.731355E-12){
double VeAUdxcAhPDtuzuXCddphljIaz = -7.395179E-17;
VeAUdxcAhPDtuzuXCddphljIaz = Math.Sqrt(3);
CIPDRmuBhxQRQ = VeAUdxcAhPDtuzuXCddphljIaz;

}long FUfTAYitEoTACNxwNVcPldExSjL = 21262612384934927; 
ushort PmLMz = 19915; 
short fXRJCkQJVJBqJapmmadRE = -18210; 
double coXJsdNOmOMlUmRn = 1.45752E+08; 
while(coXJsdNOmOMlUmRn == 7.798947E+18){
double yzzlAeUbqGlnhqRWMeVodtaiQjYT = -5.591372E-05;
coXJsdNOmOMlUmRn = yzzlAeUbqGlnhqRWMeVodtaiQjYT / 3;

}float EQLKqaMcpJRRh = 9.672236E-29F; 
uint QiYalUcSoqQHocamc = 60587516; 
int KbSDdxGGeKdSLIK = 379908172; 
while(KbSDdxGGeKdSLIK == 379908172){
KbSDdxGGeKdSLIK = KbSDdxGGeKdSLIK + 832985;
}string VxaHREemCUdfgfzhzyljI = "OFuptJuKVHIsWgcPObZxCYH"; 
uint NVjxlFTbhIxmBjwTRZ = 468495; 
ulong sfKJjjl = 41359155596324968; 
byte bOpOzHCnRFDLYidOEEdND =  102; 
long WSzuNEIJEpxKzuUAhusGJBRP = 80262323259610080; 
sbyte SGihBnWTgIMupiKF =  24; 

}public void cchxaJG(){
ulong zEdjQEXDnpaUeKPu = 24575947259395275; 
double ctwGADMNzpdzVQsGxn = 2.670806E-18; 
ctwGADMNzpdzVQsGxn = Math.Ceiling(Math.Atan(-5));
Console.ReadLine();uint VlKnNANygg = 14921854; 
float fzGOuUPRDGSEOUWfBlMkuA = -5.370742E-15F; 
int qPiDaCgIIwMSMGzdKdxjMSB = 61; 
while(qPiDaCgIIwMSMGzdKdxjMSB == 61){
qPiDaCgIIwMSMGzdKdxjMSB = 333746;
}int nkG = 10521; 
if(nkG == 603177){
nkG = nkG + 310356;
}ushort lCtIBf = 48177; 
short tUucznhWGfzpbuzIfczwKzgN = 6907; 
int IYUpWaRIuQDfNBdZkYQkgVZudnUgV = 64637798; 
while(IYUpWaRIuQDfNBdZkYQkgVZudnUgV == 64637798){
IYUpWaRIuQDfNBdZkYQkgVZudnUgV = IYUpWaRIuQDfNBdZkYQkgVZudnUgV + 517992;
}double QCokNdSDXLVHAWOQwSMYBl = 4.04479E-30; 
double ZenuxBauShSgDYAx = -2.0998E-30;
QCokNdSDXLVHAWOQwSMYBl = Math.Round(ZenuxBauShSgDYAx ,1,MidpointRounding.ToEven);
int? wPakETiBayJxKmW = 549433;
wPakETiBayJxKmW += 18930;string RDqA = "uzwxpcYkpCsbX"; 
double DyObpKJt = 9.32278E+27; 
if(DyObpKJt == 3.014474E-05){
double VeAqnJQelfzOgQfnVAtPojKMfxEfo = -3.053135E+15;
DyObpKJt = VeAqnJQelfzOgQfnVAtPojKMfxEfo / 3;
for( ; ;) {
Console.WriteLine(4.388803E-36);
}
}float feCAFBbRZunN = 1.834594E-10F; 
ushort GzKnhsDqFftQGAzNiqGq = 16257; 
ulong FJYMJFncp = 81748321902245772; 
uint VzhklBVB = 363196; 
ulong uTkeGi = 66996126896699530; 
string THgSOcROyyNlNdKKELREBPW = "gJy"; 
int ekSziIXlVqqbyI = 42; 
while(ekSziIXlVqqbyI == 42){
ekSziIXlVqqbyI += 610236;
}ushort oDzldLeMfFmtslUN = 38096; 
ushort yItACdhOGMCPZbGWpZWEmhj = 14517; 
ulong eoWCIDlSWRVcWiWWzZEQmqNp = 65546907735885715; 
ulong ZSSeeZEgfObmdOIpJjIWCND = 31296408966751500; 
ulong ijeeuVFmglZ = 7009046966954944; 
uint klplijeiPzwnWUFDlnocKwBD = 5755; 
byte igZ =  16; 
byte YXPSlLBdqNEawPoI =  251; 
short ygQ = 14189; 
ulong FZdZJAsayWHOtcEDHeNZqiwMChH = 8601336828636102; 
long dJnkaYTITGpxVA = 69213991803289357; 
uint okdNwMwwSfEwAOpjCeICpHUTyn = 3918; 
byte fFhxWP =  225; 
sbyte hkVFPWozY =  -64; 
byte mQZD =  171; 
short pgxajLZY = -14789; 

}public void SCpx(){
float qwbfWhQqhZeTPhUm = -3.94033E+12F; 
ulong MjMTPY = 52714779539023794; 
long Iknhpf = 52095128503980789; 
string GKOhJhpYgujwezzzANGm = "IQHpDmKPVlCJoa"; 
long BzAVobWVzehILHyYP = 77396017127637680; 
sbyte iJNQzNXY =  -58; 
sbyte fGULSyebeoPD =  94; 
string StK = "EngQNmMoKVNI"; 
sbyte yesZyZJQGeWufTMxUEoWDdZQg =  -8; 
sbyte ufqHGLx =  118; 
long WKJOJcTtgSDnajslotfDSYTBwA = 87661248104990605; 
long nPuXWOqnFgfhouwcSha = 22396421555776426; 
ulong PCYDNYBZiGdHFW = 87098668041964841; 
int KOf = 194457; 
if(KOf == 168737){
KOf = 597148;
}float FgGCwUcKCWqlldQLDlNaTWXT = 9.740762E+27F; 
string CmVcMfKmZAYeqldesCTtuWQX = "SWIiNCDRnhoboBftDMcmIJITTB"; 
uint KwGyXjTjcelYxnddOcdaNaRjNKkio = 29876357; 
byte DgSD =  214; 
string HWqzB = "RtGFf"; 
float ylKFwKnZLVnPZV = 1.180589E-27F; 
byte WYYuRKtKJTRELJzQ =  159; 
short nhhQfWbcJBjYlu = -28601; 
sbyte ycwalLawhgkFpinCLXtOiTyPH =  48; 
sbyte UFiHBHtEWxcxgwYP =  82; 
uint sVxByKuJFFGt = 531976; 
uint DYEeJPLGoVWHNVJY = 33; 
float zdMXfXOYpcBVGVwRnt = 5971.187F; 
byte lCONWKWHjkLXbAlHYMaMolubb =  215; 
uint LQInuIRjjtbecKTpjJUVGHuXC = 520811; 
string MMdjeWu = "xEtIbyGKWKGsdRtYF"; 
byte xKMW =  34; 
float yIXV = 1.706302E+19F; 
uint OfBnEjOnGOVIdVtfGSTXTLI = 8210; 
double YMaMTTpdmypJTIxghliYDoSpgUCUd = -5.354874E-19; 
while(YMaMTTpdmypJTIxghliYDoSpgUCUd == 9.737797E-24){
YMaMTTpdmypJTIxghliYDoSpgUCUd = Math.Ceiling(Math.Atan(-5));
int[] yEAaIayKULGY = { 1983290 , 71211 } ;
Random waPjSdJSUCeju= new Random();
Console.WriteLine(yEAaIayKULGY[waPjSdJSUCeju.Next(0,2)]);
}sbyte aXVlyAYn =  75; 

}public void OgHUydysRpzxeqyDcu(){
ulong QMBLKIdfYB = 76153014837353725; 
long XSUTME = 9121088172860886; 
ulong GWj = 4982736215554217; 
int LnwMKqYSHYfQAq = 9; 
while(LnwMKqYSHYfQAq == 9){
LnwMKqYSHYfQAq = 736591;
}float qyTgG = -48554.21F; 
ushort kmidHbR = 49354; 
byte iZXdZAaslkeRSCGWXTEMBnlQ =  232; 
uint nwcxLsXKfuu = 628197440; 
int sjetu = 45459054; 
if(sjetu == 962924){
sjetu += 182003;
}uint VNhhxiMWiP = 4905; 
double ADDYXfwWcLDhLQLYzCTJPslly = -3.221206E+27; 
if(ADDYXfwWcLDhLQLYzCTJPslly == -1.966848E-08){
ADDYXfwWcLDhLQLYzCTJPslly = Math.Ceiling(Math.Asin(0.2));
Console.WriteLine(ADDYXfwWcLDhLQLYzCTJPslly.ToString());
}byte RRjzNfIkcxXHDeJORDeykAC =  180; 
short ooQDhwxiaMmaCuhH = 5999; 
ushort CySnjnpiPCspAfnQsuBnD = 13743; 
ushort XCQb = 45706; 
short UhbRcyaGoSlHIJxNVn = 5327; 
ulong eegpePqJy = 14272292343269223; 
byte sPkxpELuBOVNf =  125; 
short dVBXIJWSEOgLlnoHckyxUIQN = -8876; 
ushort UDCsaFIeDOnMKStTDxahmAoLV = 3397; 
double weOFlScpcwgfFiNTOzNFCPnpsmE = -1.684084E-19; 
weOFlScpcwgfFiNTOzNFCPnpsmE = Math.Ceiling(Math.Tanh(0.1));
Console.ReadKey();int HKzPpPZktYCeBwJqJzqGxuYTYXVk = 305; 
if(HKzPpPZktYCeBwJqJzqGxuYTYXVk == 792189){
HKzPpPZktYCeBwJqJzqGxuYTYXVk += 401420;
}ushort cHkPiRljGukjOliZeWAmMGQtTOVq = 28559; 
short fXNnbnCyGh = 18603; 
byte MMxdTChSVjkyQSWwVUN =  79; 
string lbXgM = "AUeWslHgqSzUhxkOD"; 
float xGcLnbXB = 1.063252F; 
ushort MQFtbe = 17441; 
sbyte kPdcwSiIUzAfWpXyiRV =  -24; 
long HVMNeDdjJnMGkRYJSiOSMcWZPAIWS = 38574611702310971; 
long HFLPkupffzBZMHTmFZMXFUh = 25367041189183666; 
short whnbmMG = -702; 
ushort yqnHcaYEiELiMxtuo = 22440; 
int xIiQqA = 21837400; 
while(xIiQqA == 21837400){
xIiQqA += 463033;
}byte KagoZMafJOzm =  105; 

}public void JSynWofQizpDuNlVymYuMsmZs(){
float pZusd = -3.971028E+21F; 
int QTDWlURJZysnUNq = 2793; 
while(QTDWlURJZysnUNq == 2793){
QTDWlURJZysnUNq += 600468;
}ushort ejPQPflzGkNeFLchSCFaUARwlch = 52358; 
string lTsWyMydcDOYMdhINAlTm = "aybGstFMYxxWYTMInpLOAclW"; 
ulong nEpIEiyyTVVStdApEqDQeoW = 32873345689974656; 
sbyte FfawqTmzBZRMxWHnizUuXJYIk =  -22; 
int YhVKQZOYzVmzoqFoetQZDkhApLVWg = 2139; 
if(YhVKQZOYzVmzoqFoetQZDkhApLVWg == 250815){
YhVKQZOYzVmzoqFoetQZDkhApLVWg += 508658;
}ushort gVxhJ = 51977; 
byte xeGeJDRG =  163; 
uint awtfeRRfKsmcRkkXytRTwTn = 625169680; 
short yQupFPNtJOQspSJETibPTzFtlQ = 5707; 
short gzAQZt = -12996; 
short ImdS = 3494; 
byte bCbMpwMFAZZkadPkwlCo =  82; 
uint ztnRBPMbVwM = 28; 
int RMNUqaueoYTcJGbUqBj = 357892; 
while(RMNUqaueoYTcJGbUqBj == 357892){
RMNUqaueoYTcJGbUqBj = 800944;
}ulong eQoTBIPwPDxkqdoYSINcwUBuKeam = 23257762022176542; 
string tuUkl = "UoTzddLEcLDj"; 
short iwHCGIZqIoOaXOnsikIiAwlCjM = -4123; 
double SAf = 1.996135E-28; 
SAf = Math.Ceiling(Math.Atan(-5));
Console.Write(SAf.ToString());sbyte JZLtsnROccofYgMMzpyaCnk =  -63; 
long YBIehdKDVRU = 60032184287675407; 
ushort jEweEphluOyIbopfXVlwwk = 59503; 
float SOcIAgWsFPXmZEODpCcycxuH = 2.013598E+25F; 
byte XpYIE =  81; 
long eEVICoyAsSXqyTeKT = 36080118811857977; 
ulong oRoKJSlyMHuVN = 62943652146708740; 
byte PiFphJKmLM =  227; 
sbyte diozlwtdzAZxAgZPehqeD =  -1; 
uint DePct = 18128320; 
float lkNFuxJSkpJFTICKzHEEdgEHj = 8.638213E-29F; 
int KPphFnWmjKM = 23711854; 
while(KPphFnWmjKM == 23711854){
KPphFnWmjKM += 504640;
}long mtVcGoOfHfuyob = 52780146565890428; 
ushort WRVRPuNKmK = 25155; 
double MKTRiogQlxGKQIBcucIWIgP = -1.736299E+32; 
if(MKTRiogQlxGKQIBcucIWIgP == -7.234319E-12){
MKTRiogQlxGKQIBcucIWIgP = Math.Ceiling(Math.Atan(-5));
for( ; ;) {
Console.WriteLine(-1.234847E-29);
}
}
}
} public class WTwh{public void PwuWiCxfBBKbLScBwJeuuycfRPQM(){
ulong IKWuiAkLblJxT = 22197564139921224; 
byte ODstnfuLLtfS =  0; 
sbyte JDoKITMXFon =  -6; 
float xkTzs = -5.426876E+25F; 
float WFIBXd = 1.255683E+30F; 
float HcNmRCzcF = 1.549881E-33F; 
string CHnimuIXASxtfasOUSux = "kcKYcXTel"; 
string WYfT = "UBBiohkpUMCfoLVJEupFF"; 
byte yPSmNtxg =  231; 
uint ZsUmX = 83; 
ushort pxNQC = 11435; 
ushort EPoeKoduxVyX = 29631; 
int HjMEPpymNlfUbbbLFZc = 119535; 
while(HjMEPpymNlfUbbbLFZc == 119535){
HjMEPpymNlfUbbbLFZc += 48594;
}float BiuxHMQfHQGFdgDCTFwbsujTYu = -0.4697588F; 
ushort xBYxWdsP = 31943; 
sbyte WwVYuZxE =  39; 
sbyte IPBfZCzlKpJxteHHkzjdel =  80; 
ushort oUEuGVPcMTiWQeRqmzVmZCTNtlezd = 40326; 
ushort egLIUSeuUtIlzKuusMSOVjZNaQbtR = 20042; 
double zkSKt = -0.0001572495; 
while(zkSKt == -3.865576){
zkSKt = Math.Exp(2);
 bool? ioxuu = new bool?();
ioxuu = true;
}long cqmJdNdoCziywMkWgVhFHUSMydhE = 26802916189301032; 
int UQsmSfBAtBfYVWZ = 30; 
while(UQsmSfBAtBfYVWZ == 30){
UQsmSfBAtBfYVWZ = 223475;
}string BRcXNahgksJaqmucMlX = "HzBFGwsXyBclUkSxPiNzk"; 
ushort GmVIRVfNpnpYcqyqqIkKKoxggw = 62201; 
byte zAzmEUsKOCbqONYElP =  29; 
short wQaLjpROBxYSjRPRMzxfIEgjs = 28018; 
uint DVGRVMdJzDuEBVm = 6; 
uint WjNBpKljRKRsnOtlGQ = 93; 
uint fsjeTGMpbgTsHmQuohKClZI = 13; 
ulong KInWCg = 60650629596434110; 
uint KzVhgwkCYmWCgKZYdzbJ = 19508756; 
short yiwddqddL = -10739; 
double kIanxNXnjIXKqnNhySUBgK = 3.202739E+34; 
if(kIanxNXnjIXKqnNhySUBgK == -0.001426413){
kIanxNXnjIXKqnNhySUBgK = Math.Exp(2);
try{
Console.WriteLine(kIanxNXnjIXKqnNhySUBgK.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}double KYdOJonlWmEjCSdqZVHnQeUQIunSo = -1.593405E+14; 
if(KYdOJonlWmEjCSdqZVHnQeUQIunSo == -4.338E-33){
KYdOJonlWmEjCSdqZVHnQeUQIunSo = Math.Ceiling(Math.Tan(1));
try{
Console.WriteLine(KYdOJonlWmEjCSdqZVHnQeUQIunSo.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}sbyte VtQttyusfFgulLAlwZSS =  -81; 

}public void QUnFcfYMYVokC(){
uint ofLSgJjcGpBw = 7845; 
ulong mzCDVfPuoutHlfbjXSgThZdL = 55307628696487835; 
byte cNZZy =  45; 
short EMpERBopztJ = 25250; 
long wQVezCSFI = 8952519556911001; 
double QxqwdagZlltoqyPsi = -1.226357E-11; 
QxqwdagZlltoqyPsi = Math.Truncate(QxqwdagZlltoqyPsi);
Console.ReadKey();string GUzEC = "SUXOJCAHSDdep"; 
long IPUAyQxNQ = 26835544822648696; 
string xUoAKMuZXlbWTuRYfF = "dEwgh"; 
uint GyCqyHaN = 3659; 
ushort kOcRACOsGC = 12862; 
sbyte BsaZaNgZAfWzHP =  -28; 
double gzFQup = -4.365633E+18; 
gzFQup = Math.Pow(double.NegativeInfinity, 2);
try{

}catch(Exception ex){

}float zfATjeywdT = -1.635865E-18F; 
ulong Rqgmwd = 28383358609709403; 
double eVaMnYjJHksSYHg = -4.414682E+16; 
if(eVaMnYjJHksSYHg == 2.528797E-14){
double wDnNzpaDwFPfmbSTgzHkLaXkG = Math.IEEERemainder(3, 4);
eVaMnYjJHksSYHg = wDnNzpaDwFPfmbSTgzHkLaXkG;
object JicjkpLLyjqjQxtRaWKAdjRwBeuG;
JicjkpLLyjqjQxtRaWKAdjRwBeuG = -3.055102E-26;
Console.WriteLine(JicjkpLLyjqjQxtRaWKAdjRwBeuG.ToString().ToLower());
}double LgiK = -2.419653E-17; 
if(LgiK == 1716556){
LgiK = Math.Ceiling(Math.Atan(-5));
Console.WriteLine(LgiK.ToString());
}long CtzVYYPCiX = 79212423629947982; 
ushort ZQYkqwhjbwAWsuZGjdNpqEeYK = 61177; 
uint xDhizSoMwnFDQFANyi = 59902900; 
ulong sTOgAwLZCuU = 46079770437477183; 
byte WAjAMesB =  128; 
byte OgkowstTaxMjJmRaAhLmBCfwtEHN =  84; 
ushort bkXPInZUfgknOko = 33911; 
byte dStPXwmaPVRLGDHYLZLzjMBmFfp =  69; 
short dcwSmBem = -15873; 
uint HzddwCJHQBpPOMqsf = 82; 
sbyte KFSOoOofTzMiWDmxjiFHIBLOmIfVi =  121; 
string xEWUlogOtBXcEG = "ieWWamzqiUdCIItXUwpNtHtkV"; 
string ngEJjHQWRlcbiXlNahNzfZEcH = "TzDLMaXxkj"; 
int UURLCLRth = 123044170; 
if(UURLCLRth == 53645){
UURLCLRth = 151209;
}int nEBbWOqugBbdMMchOmTHuz = 219601328; 
while(nEBbWOqugBbdMMchOmTHuz == 219601328){
nEBbWOqugBbdMMchOmTHuz += 350892;
}long JTuMWHejYfBq = 20973464263209881; 
int nXqQMoOQMZKPSiKaPwdFoOgc = 64; 
while(nXqQMoOQMZKPSiKaPwdFoOgc == 64){
nXqQMoOQMZKPSiKaPwdFoOgc = nXqQMoOQMZKPSiKaPwdFoOgc + 974605;
}byte NTFyugVPwhuCIGsHFwnJIaABQnNdT =  208; 

}public void NLLbYE(){
byte JBnihuGwxoPtoKGqNte =  160; 
sbyte RWFbHQWEucsQG =  -90; 
ushort sgjTpOeyRcftoFAHAoKqEfNQz = 56285; 
double bAeyXVbXxmdcRhkELEoPDL = -2.18302E-14; 
while(bAeyXVbXxmdcRhkELEoPDL == -3.894419E-19){
bAeyXVbXxmdcRhkELEoPDL = Math.Ceiling(Math.Cos(2));
try{
int fUEgipiOZ = 7353905;
if(fUEgipiOZ == 19131){
fUEgipiOZ++;
}else{
fUEgipiOZ--;
Console.Write(fUEgipiOZ.ToString());
}
}catch(Exception ex){

}
}sbyte kJCNPYh =  -20; 
int cMbDcNKViPfIhs = 417267324; 
while(cMbDcNKViPfIhs == 417267324){
cMbDcNKViPfIhs = cMbDcNKViPfIhs + 751674;
}uint EKwuHhwctH = 406387; 
uint qImOj = 30725747; 
short DBiV = -7723; 
int fRYOhshJgCAfxA = 73; 
if(fRYOhshJgCAfxA == 95388){
fRYOhshJgCAfxA = fRYOhshJgCAfxA + 731951;
}uint EHmlHSTylbzXCLxMW = 505563411; 
int IqWVEyOqioSWDnsCVMoP = 29836666; 
while(IqWVEyOqioSWDnsCVMoP == 29836666){
IqWVEyOqioSWDnsCVMoP = 967003;
}ushort fQglVVVpqpc = 27067; 
sbyte dNGaYx =  -107; 
byte mlP =  242; 
short nxGbzktiHaHscjFJTKElLbzaYl = 25958; 
ulong afYExcUyuHVLFZZqecaxFseugh = 67478198545076169; 
int JOGhLkAdIBgbzJUjUsnIbEyoPVS = 3891; 
if(JOGhLkAdIBgbzJUjUsnIbEyoPVS == 285954){
JOGhLkAdIBgbzJUjUsnIbEyoPVS += 373525;
}sbyte pxSLuSbMGblBWicxlbYT =  -106; 
short tQsiKcuSIta = 5147; 
ulong HUP = 45791874290514445; 
short UnwkEZf = -546; 
double KsKk = -84654.34; 
if(KsKk == 7022896){
KsKk = Math.Ceiling(Math.Atan(-5));
try{
int FmhxFICLRfIhIeZna = 4793193;
if(FmhxFICLRfIhIeZna == 99230){
FmhxFICLRfIhIeZna++;
}else{
FmhxFICLRfIhIeZna--;
Console.Write(FmhxFICLRfIhIeZna.ToString());
}
}catch(Exception ex){

}
}long UodleRZQgwxphlyaQfeFSjkwIt = 243829876305008; 
ushort iflJcUulmKHyhpitUccKeIELU = 32799; 
sbyte TSVpRHSLUxDYQYiXNAqtfiJqQ =  51; 
ulong CBODNDtuOpGCyzzz = 25906951139035622; 
sbyte MLiFAgUqitIZkmGOjyTQVIK =  -75; 
ulong RGaWEuuZRIkjQAHfofQCTH = 47378752603308521; 
string POpoqlQnFkxXEZHuiQsFunSjepAcS = "iNIm"; 
ushort isPc = 20273; 
uint JjEhxUgibLxEBWfjaNNwDcKppFEk = 3767435; 
short SqjaChdiMT = -24022; 
string JVgciOpEg = "HBnSRpVsbxhLi"; 
ulong JTJFK = 85297774127492606; 

}public void lYLMWEAnplSQBfGdXMuX(){
uint IZyMLTjjgTNLQyxZmiGwExMTiQb = 68; 
double kiqq = 2.54151E-17; 
if(kiqq == 1.212403E+10){
kiqq = Math.Pow(double.NegativeInfinity, 2);
Console.ReadLine();
}float CNkwXQPKYJyAZlZwtjxTxDoeKR = -9.982358E-05F; 
string BqfmXXxDSeQlkRQqcKncCipayYzg = "gsqJbiguMobZRqsdjqzntO"; 
ulong ZuqafQY = 88253284970324729; 
long WpumdLSYWmIaSssFhJIRf = 28462753308128355; 
int hPFxytWWKAbiDqAzzsgp = 535624973; 
if(hPFxytWWKAbiDqAzzsgp == 668853){
hPFxytWWKAbiDqAzzsgp = 389427;
}string uLltsGAkmWVllNXsdetZbk = "nznNGYaNdokDZnJRX"; 
float kDyVegFAAE = -2.36211E-32F; 
long jXVdTTl = 34359962261135916; 
ushort xiNaYQDiGb = 31278; 
uint ckCtPpO = 7628438; 
double YtGZ = -5.398826E+32; 
while(YtGZ == -2.219359E-33){
YtGZ = Math.Sqrt(4);
try{
int oGkFiwTRfomuQnGe = 1221385;
if(oGkFiwTRfomuQnGe == 34873){
oGkFiwTRfomuQnGe++;
}else{
oGkFiwTRfomuQnGe--;
}
}catch(Exception ex){

}
}ushort HFGlHCewOlupAFZxadkEtdYlHd = 17404; 
long ZmzRMzRbkiMHRsuDzpY = 33961902395667393; 
int FEmwQPYWAOghu = 886155923; 
while(FEmwQPYWAOghu == 886155923){
FEmwQPYWAOghu = FEmwQPYWAOghu + 725509;
}int iNomqMjpXkVkIGGzkaDxG = 551029801; 
if(iNomqMjpXkVkIGGzkaDxG == 874996){
iNomqMjpXkVkIGGzkaDxG += 453809;
}sbyte WYRbLxQXiAdOGCDlWNjo =  -123; 
ulong iidikzXynbIamdjXj = 22058943870263554; 
string tgnUHkyJTHnGAkbPNXC = "hNQaABKaJBd"; 
long TFpagaPP = 84345535444555285; 
long bWKyHqmugByZtEDxHHLOGWMBNxA = 12758402513504183; 
ulong wYZNMUnswXMtNujO = 39975414197697396; 
sbyte HJWc =  68; 
uint VkJMaUcyhqzNMFfJNaoWFpL = 15858323; 
int nGxtjkFRiCbWwcNbtMdyz = 998742; 
while(nGxtjkFRiCbWwcNbtMdyz == 998742){
nGxtjkFRiCbWwcNbtMdyz += 594080;
}byte BVExkORLXoDgOXgLeTU =  215; 
short cehjOHnsLGw = -17990; 
long zBkoVpxxCpLDTyXGLqp = 17264923704235524; 
int WMCpBmFJFWYVGESAlN = 396222232; 
while(WMCpBmFJFWYVGESAlN == 396222232){
WMCpBmFJFWYVGESAlN = WMCpBmFJFWYVGESAlN + 910666;
}uint ZljoKSQRFEimsktWgwsomWx = 769760; 
ushort lAbJHROmszolffPYnpYw = 38868; 
string OJEjBdP = "CmkFsDVDwEXayRilizXi"; 
byte bcjqmacImjDQOZVCyxtOERHb =  182; 
double sacgHyMkhuaoVANq = 0.0002540394; 
while(sacgHyMkhuaoVANq == -9.335118E-26){
sacgHyMkhuaoVANq = Math.Pow(double.MinValue, double.MaxValue);
Console.ReadKey();
}
}public void KCoauE(){
uint tsiwKCGCujKpizesCP = 89; 
string dALujbP = "XyQylARepjHTFjePXkyWzFDHEln"; 
long osENAmlAMmGCyzQnhDTwVy = 494981986054104; 
string gubScfeXS = "kYSubgMsxJ"; 
ushort PRtZUdVqqT = 15736; 
ushort cxYlbzWEGmaNz = 26732; 
byte QxqRTFk =  122; 
string bcCYqMzZ = "KOgwkzLiEtlxIKhBxVDCzlAhQxTbI"; 
double oAyBwKbgth = -2.994121E+09; 
if(oAyBwKbgth == 5.022798E+22){
oAyBwKbgth = Math.Ceiling(Math.Acos(0));
Console.Write(oAyBwKbgth.ToString());
}ushort UIFMdatIJEVngFfXwwUiGtxhUq = 1962; 
uint uIWBimcSHLABqspX = 60613360; 
string DHqknmCkxRMYRgnWY = "XsdfPKuJoagkZnZZYNqMmVL"; 
float PZBJjduYhfLzyS = 1.59965E+23F; 
int CelSyZSiCGlBbHlJZLTZeWNlSinCC = 689991873; 
if(CelSyZSiCGlBbHlJZLTZeWNlSinCC == 950712){
CelSyZSiCGlBbHlJZLTZeWNlSinCC = 192338;
}ulong XCCRbDOM = 4058513192601083; 
int FqaaoLqIHVpTexAzcisiduYAke = 2952; 
if(FqaaoLqIHVpTexAzcisiduYAke == 761071){
FqaaoLqIHVpTexAzcisiduYAke += 993087;
}string GwjWm = "AtWSxOZIMoQOEyPktBQjBbUE"; 
sbyte RaReogVDJKy =  -98; 
string fEEzlZwFowcKgdp = "AsLfWZQHaKSbOeN"; 
byte seXzxshVmLxcGZR =  60; 
byte fwXCfNWz =  138; 
sbyte QjIQlOZsZcaDk =  44; 
byte FwKaPEUIRARYj =  139; 
double aCxCPmtlSUkheTNSStqdSgFm = -3.324853E+37; 
aCxCPmtlSUkheTNSStqdSgFm = Math.Ceiling(Math.Cosh(5));
Console.Write(aCxCPmtlSUkheTNSStqdSgFm.ToString());string YTq = "jmHMbBQdu"; 
sbyte iEJJGItbclT =  13; 
double pIsnGqYwDT = 2.10572E+16; 
while(pIsnGqYwDT == 300486.2){
pIsnGqYwDT = Math.Ceiling(Math.Tan(1));
 bool? zMTyQPSsObPaUpPyeplqoO = new bool?();
zMTyQPSsObPaUpPyeplqoO = true;
}ushort NhUXCgkHAldTxXNEkuYac = 45966; 
int skWNXSzFHjeEHhqleePUKIHS = 60917528; 
while(skWNXSzFHjeEHhqleePUKIHS == 60917528){
skWNXSzFHjeEHhqleePUKIHS = skWNXSzFHjeEHhqleePUKIHS + 389795;
}long CdGp = 205514798925193; 
float XccNVXV = 1.351898E-30F; 
long deqsOkmCcbDDaaGuhg = 41277168646487761; 
long KgRXUNgttoamYLQ = 76265144997712149; 
int ojeHKVgEOSVkFJnUlbTfYxJiqm = 602091438; 
if(ojeHKVgEOSVkFJnUlbTfYxJiqm == 119289){
ojeHKVgEOSVkFJnUlbTfYxJiqm += 382389;
}byte GDbNK =  97; 

}
} 
public class EwGQOztlbbElhlaxyGswWDaK{public void cdjzPtZqmAioJyDVTDUljSM(){
ushort JsfUT = 1502; 
ushort dLwiNlJFUVMShPiQBcZgyDjqQuf = 517; 
float tzPtGMgMfplIoXa = 6.399324E+32F; 
float ptRbnzBB = -6.068492E-12F; 
ushort JLxDnc = 51171; 
sbyte KOXJUQnmqoxkREd =  10; 
ushort VfkctNjEWTuMziMNH = 39516; 
float tJmnNdWVRhsa = 665.0769F; 
float lHuj = -7.638504E-15F; 
float SFQnAeXzJehlT = 9.822424E-17F; 
short oMPBuVWoJBZkKTcPRwimIxNk = 24900; 
short uIjoseukGdUzJTZQEXnYkf = -3894; 
string fchSYFHciDHzkIfoJ = "MXWAtJGjmGtCok"; 
sbyte lbmsIAnxAqsstphKDJLeGGJ =  -4; 
byte saJImxpyxecpPmJBZbpnQXzJ =  249; 
int dhGsNfShmNSeWbWU = 64; 
while(dhGsNfShmNSeWbWU == 64){
dhGsNfShmNSeWbWU = dhGsNfShmNSeWbWU + 415863;
}sbyte HjXYuDZJHXloQMojWfw =  61; 
sbyte tlUcubNMaJcZLEhUNLOjyYQHdqtb =  113; 
ushort tzKQMeshYWUWEmRaLbctLZYBwxnZd = 43622; 
long IJKCbOxQuDPmDbmtmpIZVaot = 41255725320324066; 
byte oVEpZ =  239; 
int YijLXtjUM = 856064; 
if(YijLXtjUM == 634640){
YijLXtjUM = YijLXtjUM + 376734;
}short Sth = 32129; 
string AcyNqQXqLuJcfjP = "bfWGqoAlI"; 
uint nhEymVLKJ = 629850; 
float Pwopl = 7.733766E-20F; 
sbyte GqcdxgkDk =  -108; 
ulong JjgXwgwBgMwmSRWZja = 58182274076947702; 
uint zfoDcBflhtQzSGeUWP = 5987; 
byte tkaCxGIU =  137; 
byte RbbxSkqLKSQkGxCYIpGY =  227; 
ushort NeCBeBEmRE = 53953; 
string dyeFUUBtDawfUIaFWn = "yOospLmkgFZHPwYUklbdaTV"; 
byte gukSE =  169; 
float dEQPfOUIJuO = 1.070507E-10F; 

}public void KVOQsVjNETenipbtEWe(){
short SifezgxqAgHKCKpeNpZfqZ = 28638; 
long FEGpRSzcVSMJLUOOxeMyEIHMn = 1450329568064808; 
byte EQaJniHHSEwcwlsVUkmnXXUbal =  95; 
float YBnybkXwxynWbOiTN = -5.83275E+27F; 
float YbhaWGWYIcPPeTc = -9.286052E+07F; 
ushort uAz = 48536; 
double ViLinbBZcZz = -2.298247E-05; 
if(ViLinbBZcZz == 6.67711E-38){
ViLinbBZcZz = Math.Ceiling(Math.Cos(2));

}ushort sUbCJZDu = 5417; 
double YJAbhGpnQGnXuwduioEtnVpSe = -3.75243E-34; 
YJAbhGpnQGnXuwduioEtnVpSe = Math.Pow(5, 2);
int? UDBhhqRQMjOgfotbhqMOhGWjCoiW = 1524911;
UDBhhqRQMjOgfotbhqMOhGWjCoiW += 51974;ushort HTPnGqHSiUkIbRAYiLkiTA = 34209; 
string ZsdbqOdJxpNsRcAqFdsWnV = "YoI"; 
ushort mShsPEHYERS = 7657; 
sbyte FBMHsJLSowidydseMJqOxFiwDnxCO =  65; 
byte QoqAlJpPG =  101; 
int jMfDHAeiPsCasqdeljdCjW = 1626; 
if(jMfDHAeiPsCasqdeljdCjW == 856141){
jMfDHAeiPsCasqdeljdCjW = jMfDHAeiPsCasqdeljdCjW + 60803;
}short hbbykRhTUoIcyyNph = 25898; 
int DwqbCmzXQ = 62473597; 
while(DwqbCmzXQ == 62473597){
DwqbCmzXQ = DwqbCmzXQ + 198296;
}uint VgmWtoPTgzPwxLYzgZMLaVXsjdgIH = 2118; 
byte smOsojgCh =  100; 
ulong CtcJEJmyPYZiUMoigmUnC = 3794955588245979; 
sbyte BjIJKdZcEYQytmGzEUXwVBBit =  -56; 
int zSxSGgzYldFfgkp = 7242; 
if(zSxSGgzYldFfgkp == 216055){
zSxSGgzYldFfgkp = zSxSGgzYldFfgkp + 580149;
}long VJOXaJAowA = 22209841434567637; 
int AoSnKjkxI = 367446; 
if(AoSnKjkxI == 575615){
AoSnKjkxI = AoSnKjkxI + 515150;
}sbyte jypMPjHGHFOOUAxXwOHQBmRzc =  35; 
float HMhaPZ = -1.045898E-13F; 
short LflIcFyJOdWXxPUs = -11583; 
double SJhqBZVyNZUyfHVdBFTj = -1.03027E-37; 
while(SJhqBZVyNZUyfHVdBFTj == 5.771031E+22){
double gxJeKAQEPMJhh = -9.041443E+13;
SJhqBZVyNZUyfHVdBFTj = Math.Floor(gxJeKAQEPMJhh);
try{
int RpBMuOzNwRomCqpGeGDl = 3671950;
if(RpBMuOzNwRomCqpGeGDl == 78738){
RpBMuOzNwRomCqpGeGDl++;
}else{
RpBMuOzNwRomCqpGeGDl--;
}
}catch(Exception ex){

}
}int IkqoJCpnusDddgImxiwkJUn = 33721611; 
if(IkqoJCpnusDddgImxiwkJUn == 244490){
IkqoJCpnusDddgImxiwkJUn = 335816;
}double QyTVabgAEjyzSAamDtXVjtweOXHX = -1.104805E+32; 
if(QyTVabgAEjyzSAamDtXVjtweOXHX == 2.033071E-07){
QyTVabgAEjyzSAamDtXVjtweOXHX = Math.Ceiling(Math.Atan(-5));
try{
Console.WriteLine(QyTVabgAEjyzSAamDtXVjtweOXHX.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}float CsKnEweF = 4.147419E-30F; 
short UJsayWk = 14531; 
sbyte ylVIVPCZExPXZMMARHlh =  99; 
uint tBkaCYK = 15060656; 
string UytVApcSDWuWJHnXeFAaOXdDgL = "ZbTNHVgRogWgVecXlYFCuna"; 

}public void eIbeXuRPODcQJjNaDPY(){
float yFVLZSE = 7.062134E-25F; 
ushort anIZTbTVphtscloblEYFoj = 28715; 
long gtYC = 38033831813264628; 
ulong CWdCae = 60401967574842698; 
ulong QRaWNIVtcVPjMxU = 52334418551342115; 
int nUnLWlstTaDCRuUaKzfpcgdQq = 572468412; 
if(nUnLWlstTaDCRuUaKzfpcgdQq == 237776){
nUnLWlstTaDCRuUaKzfpcgdQq += 139909;
}ushort eANHJfKxQSxsYHb = 35794; 
long jpmPgRztdJDmHTgd = 36670206869793085; 
byte IytkDmzMXf =  51; 
short WipcaTEkEkUV = -25271; 
ushort ARUuESjaZS = 7672; 
short VVbTxEkkLpwZIddmApxRklfk = 20863; 
ulong MipkVEPpIjazDjcO = 24237445555477803; 
uint zEJPQcJcAcwfBwTNxczRT = 15183263; 
uint LxSMGuPJkfkHStYQBLZ = 4065; 
double baKLcKQRxkkeHRpTIHyopJfFKAbtW = -2.18716E-30; 
if(baKLcKQRxkkeHRpTIHyopJfFKAbtW == 0.0002257836){
baKLcKQRxkkeHRpTIHyopJfFKAbtW = Math.Sqrt(4);
try{
int eatibFWYUzT = 645899;
if(eatibFWYUzT == 18515){
eatibFWYUzT++;
}else{
eatibFWYUzT--;
Console.Write(eatibFWYUzT.ToString());
}
}catch(Exception ex){

}
}string tlbPyRRJipYuBsHViXFVtSAR = "zkiGwFmQyLPqtFOepLh"; 
float uqoBZgFiyCMkiZGJcOMM = 1.77302E-36F; 
int EPtRRcgsVYdTTfwzJIajcgkFh = 254810; 
if(EPtRRcgsVYdTTfwzJIajcgkFh == 182897){
EPtRRcgsVYdTTfwzJIajcgkFh = 75081;
}short yWjOOquwuojVMQXsmdWbyP = -3789; 
int JwQjVMVLixikLUpGPJRaKxG = 5808; 
while(JwQjVMVLixikLUpGPJRaKxG == 5808){
JwQjVMVLixikLUpGPJRaKxG += 324427;
}byte AHaeWFkzEokEtPhCiGwJ =  29; 
int dmPAGMGNuVOJFLQubE = 2764; 
if(dmPAGMGNuVOJFLQubE == 863733){
dmPAGMGNuVOJFLQubE = 242490;
}uint IXCVFDUcWSgeFM = 544557818; 
ulong GfDRslueUqSFtDZieIkz = 49877714359040473; 
double qiJkHwUQGpCBIDkq = -511768.1; 
if(qiJkHwUQGpCBIDkq == -1.074383E+10){
double uqabuYyfwaJWi = -9.571039E-14;
qiJkHwUQGpCBIDkq = Math.Ceiling(uqabuYyfwaJWi);
try{
Console.WriteLine(qiJkHwUQGpCBIDkq.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}sbyte lxuCPfJEVpHEE =  116; 
float pJBwASDUwsJQdjZowzQ = -7.70214E+33F; 
short YkkpbxRIFKPZaaM = 25163; 
ushort AgyxTBLqshehxIZkDjoDCzFKdf = 17718; 
byte oKFxCzkaCsLFLeKEynhaPzCGRsaCN =  91; 
ulong BscyBMUjP = 57792106171717080; 
sbyte RgkjQhZFiyJmiFzesbl =  -114; 
long YkFzKbTCHHGMW = 48557805865839928; 
ulong XRnfT = 42274905162417644; 

}public void szYLIQcIyJsPjPWCFIk(){
sbyte HwVadnXCRyXqETRDxeW =  -114; 
short GOenENDqQXTYM = -29109; 
string EDffGkuyGVeYhEocqmQgVP = "jZAIayFhOBLBuXjOwtHJI"; 
long ilIRIAmVPPSVggDuab = 56775127740822412; 
uint OogcDxqwtHXJC = 925198; 
ulong ccISZGHUqeMiNMBaS = 76843572898696484; 
string dfPUBqtceREPJ = "hkqFlGUxIKxxdMKs"; 
float YlEPDjkStKxiotRhIhtkV = -0.3799475F; 
long fyemHsMZTQyWtJ = 36428337597549662; 
string VeDYIPDMWsyNfFMqKiw = "tlnzmQChoHXKuBqDQ"; 
ulong emMabluoQBUp = 76726129919130399; 
float wxouNsGmj = 2.912363E+32F; 
ulong oIllmSCMIwqH = 11765337398144477; 
float pLIAbJDpttBIlEzFQUpCNaXNbA = -4.689753E+07F; 
uint VoBxsBFOQSiqbYgdHLjgWhk = 1058; 
short UyCsXRFykhQkXZcdzsxZZHY = 24082; 
int sSOekqdNLpnAlA = 517917; 
if(sSOekqdNLpnAlA == 820587){
sSOekqdNLpnAlA = sSOekqdNLpnAlA + 499638;
}byte TeNXKp =  195; 
byte JNBzNJfWAhkoNPYf =  235; 
uint KccfjlZKsmHOYxQ = 75732782; 
long KRQ = 52059124701593943; 
float OoEyHHqmOZOKSulSWjkuVSH = 7.241776E-35F; 
long juTuEwTaKTtZguOWScpAYOZ = 54345898752315650; 
ushort TgSVGknYtDZDkeZgWPRNkPwwJnZ = 62514; 
string biZdm = "WamRbCFlZI"; 
double yMZTTIKeMxmqkqXo = 2.487198E+23; 
while(yMZTTIKeMxmqkqXo == -2.828129E+25){
yMZTTIKeMxmqkqXo = Math.Truncate(yMZTTIKeMxmqkqXo);
try{
int LEpLidOIMCdqTWZTDakY = 7525867;
if(LEpLidOIMCdqTWZTDakY == 3458){
LEpLidOIMCdqTWZTDakY++;
}else{
LEpLidOIMCdqTWZTDakY--;
Console.Write(LEpLidOIMCdqTWZTDakY.ToString());
}
}catch(Exception ex){

}
}float wtoZzDXUUVgqIR = -4.111828E+37F; 
uint WKAWTYdOnuTcVgCeRutklFJAf = 7643; 
sbyte gESmWegXAApVKoEbRa =  126; 
ulong aHGpFsbZQJkWjgQaQZ = 57915618234340968; 
short zojOs = -32491; 
sbyte mWqNfXJQfZAhOaabXuyIMDacdWCC =  -3; 
ushort jaTOTIoniJeXpcgqzn = 10273; 
ulong DzkKWnJjxhqIwohUlhtIgnYIGKaW = 27382095435403249; 
long XtGdM = 3814176680459752; 

}public void OhRoAD(){
string nNGtUZbPJ = "kgjaccCyUpOmXgRgDAP"; 
short faKe = 24283; 
uint IOngeulfWOfP = 5241880; 
long AbPADNxDZpDffBViEiPgbIi = 3629860185136704; 
double mkddYQUsMgORgZw = 1.847576E+33; 
if(mkddYQUsMgORgZw == 1.397091E-19){
double lFRDIiYLDITpSfGsVPpBuMOMPW = 4.343225E+16;
mkddYQUsMgORgZw = lFRDIiYLDITpSfGsVPpBuMOMPW * 2;

}byte EdNssyoFJVGjP =  96; 
sbyte bOqET =  14; 
double ZszMPiuEuSPJKZfcCCfPgYVxE = -2.546498E+30; 
ZszMPiuEuSPJKZfcCCfPgYVxE = Math.Pow(double.NegativeInfinity, 2);
 bool? OFOHgdZMGnehX = new bool?();
OFOHgdZMGnehX = true;float GSAjdLxqYfjlLDpFfFzunKdsbkjx = 0.0003318079F; 
int LWGzbOqDPIxZwFief = 95593468; 
while(LWGzbOqDPIxZwFief == 95593468){
LWGzbOqDPIxZwFief += 662678;
}double hhSBZJBCIBOXREqnVFHmeWyb = -5.735932E-15; 
double KUHzbPYWyOdugosNjmTbJ = -6.959242E-21;
hhSBZJBCIBOXREqnVFHmeWyb = Math.Floor(KUHzbPYWyOdugosNjmTbJ);
 bool? xKWhwnnBngKLPTPIpHitHXbuG = new bool?();
xKWhwnnBngKLPTPIpHitHXbuG = true;long CzhVmngaPhnKGCEKE = 6630688329698820; 
float XIFyhyAmFqUHFbJmmfuO = 0.08298156F; 
short ztQSZXwGuHjtjLfmfnZsKeB = 21199; 
string nIVfQyoSZPI = "nHWomYSodDZgIDAZdtSY"; 
ulong KOKnQMdsLqFyatxuSDYNm = 53166465588827737; 
int JTFWhGYhglLRJCjDgDJCpacyYfUQ = 755946; 
if(JTFWhGYhglLRJCjDgDJCpacyYfUQ == 101470){
JTFWhGYhglLRJCjDgDJCpacyYfUQ = 389713;
}double xxoOTuSCCTYmnLSNfeH = 1.523369E+24; 
if(xxoOTuSCCTYmnLSNfeH == 1.104462E-35){
xxoOTuSCCTYmnLSNfeH = Math.Ceiling(Math.Atan(-5));
Console.Write(xxoOTuSCCTYmnLSNfeH.ToString());
}float gKzCXxaSmNBnEcPzcGqZeaaEKwjp = 1.689744E+18F; 
double qAFPRKVzcOBLYwh = -133.0201; 
while(qAFPRKVzcOBLYwh == 4.069607E+07){
qAFPRKVzcOBLYwh = Math.Exp(2);

}short nCVzFzozxaTKAsIDDB = -6176; 
long iLkQzyXBeTMIoeJypwwGaIZJ = 88101401483784443; 
int fRKIfbsXqghbkmYfleZeOBM = 805522; 
if(fRKIfbsXqghbkmYfleZeOBM == 773268){
fRKIfbsXqghbkmYfleZeOBM = fRKIfbsXqghbkmYfleZeOBM + 123874;
}sbyte MeBZSNIpuncpSBYNpjoOaZA =  125; 
int hSFgE = 238126760; 
while(hSFgE == 238126760){
hSFgE = 684496;
}byte tEOFQM =  50; 
float ROF = 5.161013E+25F; 
short NFCuNFQMmKldAeadbdWjluJ = -14986; 
ushort gaRDZZVGJ = 24025; 
sbyte tOOcsfAfuWQKDBWhCsVmZCzPVL =  51; 
byte TxIVhLh =  105; 
sbyte EOnS =  -77; 
byte ZzdNeq =  116; 
float RpYgIwmXBBiOuQOMsoszLJmGOL = -2.803826E-26F; 
ushort ejanTSxcwhBqqKbsFJoaMbfx = 10680; 

}
} public class ctsWjeB{public void wYOfXWppUjYuqDExfETwcd(){
string fLPdKAYxQGocFzMtLx = "VzORAP"; 
double HkeJqUialglGwlCEMnCRIRAL = 3.277607E+32; 
if(HkeJqUialglGwlCEMnCRIRAL == -3.42699E-13){
double JskMWXhwzAmzQMKWkelmeiPL = 2.710465E-15;
HkeJqUialglGwlCEMnCRIRAL = Math.Round(JskMWXhwzAmzQMKWkelmeiPL ,1,MidpointRounding.ToEven);
Console.ReadKey();
}uint lkmSDEM = 190432620; 
byte SBVNYdtkRMS =  49; 
long OAigZCgbjIsiAmEKDOP = 67056475864208919; 
byte xuaEmOeEQkJjQqmwPEfI =  33; 
sbyte ujSV =  -105; 
string tWVuOmsGm = "AgMEgpWCgOfcAFB"; 
byte GtuTZEFRNox =  142; 
double jqHh = -0.000240707; 
while(jqHh == -2.897056E+29){
jqHh = Math.Sqrt(4);
object MtaBYhLW;
MtaBYhLW = 0.2140109;
}short UTwuQCkGEUtMguNtezlKlcc = 29340; 
double uZlRO = -0.4488777; 
if(uZlRO == -1.276132E+08){
double mUOdiqWWjfWjlGDSRzyMhktM = -1.090566E-29;
uZlRO = Math.Round(mUOdiqWWjfWjlGDSRzyMhktM);
Console.WriteLine(uZlRO.ToString());
}int AIbBbXSeRYdRXXPikESK = 570583986; 
if(AIbBbXSeRYdRXXPikESK == 256995){
AIbBbXSeRYdRXXPikESK = 822143;
}int sGtsKpOOwRmbVBqA = 29785378; 
if(sGtsKpOOwRmbVBqA == 35583){
sGtsKpOOwRmbVBqA = 725742;
}short WVR = 27196; 
double mkusNlGMgHibNuqRPqm = -2.171767E+16; 
double wPASQQEbHDNfU = -8.527262E+07;
mkusNlGMgHibNuqRPqm = Math.Ceiling(wPASQQEbHDNfU);
try{

}catch(Exception ex){

}long CzyHyIpJaibWnKiIGkHJRjgP = 10334324411724654; 
sbyte QZyByGaJghjaEFbE =  114; 
double cXssIFulwOdIEONTikfNa = 4.421363E-27; 
if(cXssIFulwOdIEONTikfNa == 1.252338E+08){
double qYFNfDyIPVctFUdBGlQLTsRjKt = -0.1139876;
cXssIFulwOdIEONTikfNa = qYFNfDyIPVctFUdBGlQLTsRjKt * 2;
Console.ReadKey();
}int nZyZQ = 10494111; 
if(nZyZQ == 134580){
nZyZQ = nZyZQ + 414136;
}string NhmNthYbyXzPEGosXHAcoC = "WcMlpEzYHNW"; 
ushort nIRDJcQbZj = 16638; 
string IbBWaxkQqKHVIzZYRcHhaeoPW = "mMoWSziAWyzLM"; 
uint zWXkIaERc = 15; 
int QDMLaHjABuShMiDCBgqgVChqFnu = 89480667; 
while(QDMLaHjABuShMiDCBgqgVChqFnu == 89480667){
QDMLaHjABuShMiDCBgqgVChqFnu = QDMLaHjABuShMiDCBgqgVChqFnu + 852857;
}byte tLOKepNXDfcVRboDbRAao =  104; 
ushort MmBFnG = 8686; 
short AiQMxlQiFGkDsDI = 6431; 
long TsmuXpIXSQhJWZNxsSAQFGK = 6413607912939133; 
double TmXlelAeAtZbVKlPVj = 3.817059E-30; 
double geEhYhec = Math.IEEERemainder(3, 4);
TmXlelAeAtZbVKlPVj = geEhYhec;
Console.WriteLine(TmXlelAeAtZbVKlPVj.ToString());uint LXigiccZGVbipeHVnuqNCwSDHMiZI = 975727855; 
uint OODTNCKXnjBbjKR = 12652281; 
int pNzpfGBxTcsJWC = 54224020; 
while(pNzpfGBxTcsJWC == 54224020){
pNzpfGBxTcsJWC += 513839;
}float QnksCxcsuBqDjLpzymmf = 8.015675E+29F; 
double EAzhBCeeEIWkfOedu = 4.843354E+10; 
double GPnNRVsJObYVRTxlZDOftKhiLo = -5.330468E+37;
EAzhBCeeEIWkfOedu = Math.Round(GPnNRVsJObYVRTxlZDOftKhiLo ,1,MidpointRounding.ToEven);
Console.Write(EAzhBCeeEIWkfOedu.ToString());
}public void wiLNmW(){
long dlFyK = 61021716062735552; 
long osGWykGADSoLlezUDlCbQXlE = 56166940199213674; 
long xwHamUyNcGwsF = 39469955455065014; 
short cALKt = -15530; 
int SbRIcmDZIFcUHhdhmyqtkoZwep = 504958; 
if(SbRIcmDZIFcUHhdhmyqtkoZwep == 73712){
SbRIcmDZIFcUHhdhmyqtkoZwep += 483874;
}uint EqNGNHcOBfWyzUwDxWWkJE = 43; 
int JcxyBEwVFNuHKLpxOUAHRKe = 772503477; 
while(JcxyBEwVFNuHKLpxOUAHRKe == 772503477){
JcxyBEwVFNuHKLpxOUAHRKe = 42957;
}string qcaYasdFscE = "JhPmlUmPhXW"; 
double YyJtagTFydEKMoeLogFnuwRYUquPk = -1.609708E-10; 
if(YyJtagTFydEKMoeLogFnuwRYUquPk == -1.403942E+38){
YyJtagTFydEKMoeLogFnuwRYUquPk = Math.Ceiling(Math.Cos(2));
try{
int qIEDGKAKMdtoLyFMmiqdPJEKk = 7583765;
if(qIEDGKAKMdtoLyFMmiqdPJEKk == 34469){
qIEDGKAKMdtoLyFMmiqdPJEKk++;
}else{
qIEDGKAKMdtoLyFMmiqdPJEKk--;
}
}catch(Exception ex){

}
}double PbIeTsTWhSWzkZhHipNUHq = -0.04475898; 
PbIeTsTWhSWzkZhHipNUHq = Math.Ceiling(Math.Acos(0));
Console.ReadKey();int BpbIZjbeWQQcOppxHIXKZIlMDX = 2835; 
if(BpbIZjbeWQQcOppxHIXKZIlMDX == 100799){
BpbIZjbeWQQcOppxHIXKZIlMDX = 198941;
}sbyte qAyXBIMBCdWozzWWAkHW =  94; 
float ofcR = 7.284626E-09F; 
int GVgZRYJhpYAfiQmzUtOTU = 210950; 
if(GVgZRYJhpYAfiQmzUtOTU == 418365){
GVgZRYJhpYAfiQmzUtOTU = 160414;
}float RZfYtpuszeXQcpX = -8.123847E+18F; 
float SEWuHKQbKtDwQRKA = 1.143065E-10F; 
sbyte ATgwnQdRmmtKBJZUUGAoDoKY =  109; 
long YtRYbG = 27905204697475320; 
double xZSUgSPFuUSmkihAFJezwose = -4.588637E+13; 
xZSUgSPFuUSmkihAFJezwose = Math.Ceiling(Math.Tanh(0.1));
try{

}catch(Exception ex){

}byte EViisYCgdgfs =  79; 
short nmcJWEjcVfHMAEHkxx = -4500; 
float KhSWKcFFQQeAeKHgc = -2.593034E+20F; 
ushort ndMmbEyWKWUwbEejNmBFzepCmw = 57016; 
double thJBK = -1.797515E-32; 
if(thJBK == 4.181459E+11){
thJBK = Math.Ceiling(Math.Tanh(0.1));
try{
Console.WriteLine(thJBK.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}string IQQfZiMXCUmykqEjqURoXbLl = "QicWJgtDnQZiqpZGQVmQJ"; 
byte MbabSNRHfExja =  175; 
string NUUXBFaaESKhfyxaGOpVIfBQxj = "GUgZVPbBWPmUgbayadKBIhxElt"; 
sbyte HudCpLwgy =  93; 
ulong eXAktTjUIIBN = 22631338221206168; 
long TDLLFxHUagpqxLIkjtjlEYTq = 2225282751929153; 
long eXBIoOnOIt = 22414955415603417; 
ushort lUJtXXXKkhaeIhH = 45095; 
long dwYQqBUqtYewzNSkc = 30089297776302691; 
uint sOfUhDTpXAxD = 47; 
int YFQJnmyexuDQNFumRh = 565021; 
while(YFQJnmyexuDQNFumRh == 565021){
YFQJnmyexuDQNFumRh += 856914;
}
}public void xULHHgJeOSYNRdfqbzN(){
string Esei = "ZWcpVHXcRsMzPnTMYNxBZe"; 
ushort lFkSETDHy = 59363; 
sbyte cyWqPPlOGEKAgqFGJJNVAQd =  -48; 
ushort AYPfa = 57840; 
sbyte JCQeLk =  88; 
long JVMLmoXXPhlmltGVKon = 56957968443039492; 
ulong XDdQjwuKumJShlMVTy = 39267733772743458; 
sbyte tKFlht =  83; 
int ybYlloc = 11976302; 
while(ybYlloc == 11976302){
ybYlloc += 457374;
}sbyte VFwWHAfd =  -57; 
short KIxKPwfNwwi = 31055; 
double TYFOQcVDuqEXVZJsXKXVFu = -2.484745E-06; 
while(TYFOQcVDuqEXVZJsXKXVFu == 2.765083E+35){
double GykNCBpzlwhnfgRAgddyzbc = 4.913713E+17;
TYFOQcVDuqEXVZJsXKXVFu = GykNCBpzlwhnfgRAgddyzbc / 3;
object kUekQFFX;
kUekQFFX = -0.09346532;
Console.WriteLine(kUekQFFX.ToString().ToLower());
}int dnMtfEhjsJCiq = 911544004; 
if(dnMtfEhjsJCiq == 798564){
dnMtfEhjsJCiq += 252357;
}short KtMSAzmEhlqteGXwz = 2327; 
long peyDguGVKwDamSuVa = 67019836195954658; 
ulong jYCPnGOmF = 49742600357311299; 
long flkcwXVDIJU = 46262352740028590; 
short zGQZQPClENyOU = 13341; 
string WfSsdgEKWSwYxuLgtVHGwcFiupeVy = "yGgybcOX"; 
short cfcYuDQicpwZ = -27885; 
ushort TBJYNbImMwPSZsItx = 36723; 
short paHPRBVTgQyCMViDeY = 5594; 
short TqEFLQRwXYxQR = -16401; 
float UgmfPgVXmfejenyYXiyKcjfKezi = 2.460212E-37F; 
ulong IpIkSqkGMwHqFMHpKCySJQiRKbE = 22086738344254373; 
float dGIMDLnxSBXzhAo = 4.205742E+28F; 
byte gQBQOjoPqKRtoNTGVQfffVwRB =  155; 
int zPsHZFN = 55651229; 
if(zPsHZFN == 535948){
zPsHZFN = zPsHZFN + 249240;
}double MbgILoTuNOgMlwJphIgqCsjuLald = -1.944268E+33; 
while(MbgILoTuNOgMlwJphIgqCsjuLald == 2.40052E-24){
double XRTFwGTSSBkUAM = 3.237581E+13;
MbgILoTuNOgMlwJphIgqCsjuLald = XRTFwGTSSBkUAM / 3;
for( ; ;) {
Console.WriteLine(4.87593E-30);
}
}byte aDCdTSnIBjekbHG =  156; 
uint XMyVojLxXYXIXEnD = 10; 
long TNnHzxWVQhCijdsBbems = 64202508854938824; 
double yCWSHjyQySmPHPy = 0.0005567153; 
double nxSZXGjwXqKal = 2.921061E-34;
yCWSHjyQySmPHPy = nxSZXGjwXqKal * 2;
Console.Write(yCWSHjyQySmPHPy.ToString());int kszTRJSSpfxDfxcKpx = 240608; 
if(kszTRJSSpfxDfxcKpx == 747321){
kszTRJSSpfxDfxcKpx = 125956;
}string PeNtpqHNl = "GsQLmMBaxsnYnBIXltUFFqRG"; 

}public void NeWPeWsjldYGMZYfNKJXRQmMtf(){
byte IknBkSAHFlqxVObxKqAtVASHiI =  10; 
double kwYIACbQyuUBsBCslbdPQcbHU = 1.887291E-25; 
while(kwYIACbQyuUBsBCslbdPQcbHU == 4.756837E-13){
kwYIACbQyuUBsBCslbdPQcbHU = Math.Ceiling(Math.Sinh(-5));
try{

}catch(Exception ex){

}
}uint aQGozbhuTsoqLhhHRKVnFQWoZx = 94179867; 
ushort LZBituwdXVJZTThJpNMLHw = 12823; 
byte kUqobqkYiH =  76; 
ulong ONkNNZFFWdgqgXkLe = 4521705990819313; 
double zbLlSSOYZBTJjHKnRj = 1.905327E+31; 
while(zbLlSSOYZBTJjHKnRj == -8.576669E-19){
zbLlSSOYZBTJjHKnRj = Math.Ceiling(Math.Sin(2));
int? UJUQakyOH = 1750537;
UJUQakyOH += 82600;
}byte PGWYhmHARpPQhcSJZpyRJ =  181; 
ulong wxHHhgL = 63533745572812428; 
short olEVfNxTUmTttadSNDLGZqL = 6259; 
ulong qLxGdXhct = 85624573471729690; 
float HkQnEpuILOYBYgTwMyoiWlqHF = 0.6750104F; 
byte uld =  210; 
ulong bBySpKo = 25656689020456233; 
uint YLWwTiJVQhH = 5508; 
float kYSUnFYAHPdlGytPNVVef = 1.51515E+18F; 
sbyte OXBCJgLBoahF =  -32; 
byte ZkoHQXdyxsUwIPEulKDBHz =  243; 
double sLpylktKifVCfqwKAejO = 2.603066E-33; 
double eCnqYzlNajhCMzKJSgshQh = 2.068152E+30;
sLpylktKifVCfqwKAejO = eCnqYzlNajhCMzKJSgshQh * 2;
Console.WriteLine(sLpylktKifVCfqwKAejO.ToString());int DOSf = 43; 
while(DOSf == 43){
DOSf += 657711;
}double oajmnlUagPcLZoIOjdDmGgqd = -1.8563E+08; 
if(oajmnlUagPcLZoIOjdDmGgqd == 1.17541E+29){
double etzCK = 2.291253E+28;
oajmnlUagPcLZoIOjdDmGgqd = etzCK * 2;

}ushort czTuuSBykUGhuKYCeZf = 49866; 
string BpHSdAs = "NtQUpIoKPLSEnDosphNQRjKIDUXDN"; 
uint bflhlsAtqlwkuVAUYaoDWMCh = 691779; 
sbyte xLNWmWRzbwca =  78; 
ulong FxYoNcZZCzZSoF = 70268080142139264; 
uint yebSEYnMhoWMEwwpxMkzmeCTXQBQI = 814752208; 
sbyte aRIagxEjnKaIsahPSAUoaMtEhUNy =  -10; 
string AROEENoppHn = "wClGQtbQsVRXnzeSlJgMjtCRFyEcE"; 
byte IMOXbWoXdynYEZQoeIx =  17; 
int DObawbsPZlhqikBC = 91; 
if(DObawbsPZlhqikBC == 48444){
DObawbsPZlhqikBC = DObawbsPZlhqikBC + 283066;
}double XROhAuggYhjZW = -237.912; 
if(XROhAuggYhjZW == 9.959298E-18){
double RFPEFBgUyn = -2.320298E-37;
XROhAuggYhjZW = RFPEFBgUyn * 2;

}double fyNKxXdqEeRtPAbnqe = -1.523116E-15; 
if(fyNKxXdqEeRtPAbnqe == -1.7733E+13){
fyNKxXdqEeRtPAbnqe = Math.Sqrt(4);
try{
int ZoAsBLwEazXqFketc = 4164470;
if(ZoAsBLwEazXqFketc == 30956){
ZoAsBLwEazXqFketc++;
}else{
ZoAsBLwEazXqFketc--;
Console.Write(ZoAsBLwEazXqFketc.ToString());
}
}catch(Exception ex){

}
}long UIDjHY = 28556925787905363; 
uint UjJHm = 31987; 

}public void jautXtg(){
long IlGNAiRCquNHVyZtfG = 3668902685619037; 
int xPmpGKXiSquaRfVjoD = 5459; 
while(xPmpGKXiSquaRfVjoD == 5459){
xPmpGKXiSquaRfVjoD = xPmpGKXiSquaRfVjoD + 758840;
}float DOWZ = -0.988452F; 
uint eDhyakBQB = 21600464; 
byte WXeqRLejyXAuEhimSXUzFatcR =  137; 
string Fzd = "KlHQRJDPydHFJcxdq"; 
int QqMlVFcAoJcFYUXKsCsx = 41; 
while(QqMlVFcAoJcFYUXKsCsx == 41){
QqMlVFcAoJcFYUXKsCsx += 374881;
}ulong PeZLbQmkXZLkP = 15277201940462474; 
short DYgzw = -18295; 
long CRILjKsEiIMNtQhIaiZHcL = 45559083945621752; 
float mniugzcxcVBcZGMMRAylleJRNZojm = 8.92841E+12F; 
ulong MAeHNBDfDFgkaJQoLUuVGOhF = 79062546883852751; 
long nfHeeZUgK = 3151980594706454; 
ulong VjYsAneutm = 12132414647266199; 
int wazdIcNVQLYIKXOcoOgQZFBU = 3398; 
if(wazdIcNVQLYIKXOcoOgQZFBU == 988293){
wazdIcNVQLYIKXOcoOgQZFBU += 913390;
}ulong WWTULUKyC = 62502596346692393; 
long VBPQQ = 83066096249490133; 
double LNeTqwdKoQbRGHSLqoQTNh = -8.79163E-36; 
while(LNeTqwdKoQbRGHSLqoQTNh == -2.445391E-27){
LNeTqwdKoQbRGHSLqoQTNh = Math.Exp(2);
object ZgyKqHecXEGczkmKHwCsd;
ZgyKqHecXEGczkmKHwCsd = 1.033298E+36;
Console.WriteLine(ZgyKqHecXEGczkmKHwCsd.ToString().ToLower());
}double gmMgbWtDIELHwgPugyhj = -0.001592009; 
while(gmMgbWtDIELHwgPugyhj == 2.428251E+35){
double KZbaWzQNelNmTUPtwkZedQHTND = -7.772552E-15;
gmMgbWtDIELHwgPugyhj = KZbaWzQNelNmTUPtwkZedQHTND * 2;
Console.Write(gmMgbWtDIELHwgPugyhj.ToString());
}byte PZGWKiKjWkLPM =  81; 
sbyte OwzRpfiWFmlRUe =  -74; 
uint eTZcfDllcIxB = 56205812; 
string tfXKydKQJsyWToRZkyH = "ftJTmFnyNwQKklB"; 
double QUajjtNep = -5.05591E+18; 
QUajjtNep = Math.Ceiling(Math.Atan(-5));
try{
Console.WriteLine(QUajjtNep.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}short TKysBDIGmNyohAaSRxYqsZjp = -1508; 
ulong JfRmWaOhRTieLYhTnEQPPp = 65985106494652038; 
sbyte LSYAxBBotqiYJqmtkfVNJaWtuF =  -86; 
uint pnq = 29853327; 
ushort ZIyNcCwygNDMiGlOFukLWGfRsb = 18285; 
string SltX = "tEJiKgGzlIbxq"; 
ushort BMseJ = 60612; 
ushort GwbmsuoBjOOmJexKd = 6618; 
uint zPVKUZDfTnozZ = 6; 
ulong UneWgSIEhwxpJaEzZdRA = 83746361417249244; 
short HbJbiStzs = 28949; 

}
} public class gMEfQknLaZ{public void lnqCUlMAXhnUPuHHSMeCMUkkHPMuw(){
long nmJQuKIXhMSafSSPMiFxYWzm = 89428578446802017; 
short aHfElXjPXQTTf = -9790; 
ulong ZlR = 70120706546572522; 
ulong IuOLNSkJiWHljYJtzXPgfBgld = 68829726146597959; 
double wfitBFqRoja = 2.149228E-10; 
if(wfitBFqRoja == -1.880087E+37){
wfitBFqRoja = Math.Ceiling(Math.Atan(-5));
int[] MHgAFTlihEC = { 8858444 , 79674 } ;
Random plsPmXmuL= new Random();
Console.WriteLine(MHgAFTlihEC[plsPmXmuL.Next(0,2)]);
}long BcXyxAFlWHsVDIUaBeKOuYqxU = 85265852218040668; 
long QnLLd = 44796056083746598; 
short kPfVxCMBETwOI = -8732; 
ulong zUEeMPtHRwFMxaTQ = 50092803755267681; 
string hSJipaPBuQLnIwxzeaWaeDZ = "pyaOOXHaaYIBYIeFWW"; 
int eiDX = 27100422; 
while(eiDX == 27100422){
eiDX += 68838;
}long TdTTabNyjLnKOVByVsd = 63846734531016755; 
uint wtWncHUPlFUjiTtVT = 68; 
ulong qnZCJXFMYWTmWPsqwMnGJJOiYNcfl = 30443804726875253; 
ulong KpcnIbWmebEz = 2113152262023036; 
double zAXcwCtLmuItFoGwfhbBXeJExcD = -9.664387E-07; 
while(zAXcwCtLmuItFoGwfhbBXeJExcD == -2.442777E-24){
zAXcwCtLmuItFoGwfhbBXeJExcD = Math.Exp(2);
Console.ReadKey();
}ulong gVsib = 65263013531767938; 
string HognEeAPPVYZ = "hEnjUyiEwik"; 
short bpsOkxtwpJhhiaHgDmcxsapSKVX = 27583; 
ulong gVZJdjxbL = 48717595245803364; 
ushort ySWhSDbZBdpzyIeCl = 10746; 
uint yKCDBRziwLJjCuptjOhgbjVXuehm = 37186338; 
float kAdNquHaeTFQwfXdqLqyasWnPE = -2.725656E-06F; 
long xbQPZbpD = 18202220520989171; 
uint geWbbySSEQ = 746866656; 
byte qTyDaGgV =  190; 
ulong CqUpOJJcSoFnuZsWgzBF = 49886292121550608; 
double lqhtBgY = 4.422885E+21; 
if(lqhtBgY == -1.806683E+15){
double dEJKZxmjsXuegdGFgKc = Math.Log(1000, 10);
lqhtBgY = dEJKZxmjsXuegdGFgKc;

}int TAzddyMxNUVNPubCxghUBn = 16; 
if(TAzddyMxNUVNPubCxghUBn == 481205){
TAzddyMxNUVNPubCxghUBn = TAzddyMxNUVNPubCxghUBn + 69428;
}sbyte QEazhDfEwZBMcjcnOsSVpwEKkGpp =  -108; 
sbyte IpPesMqmzxSMhuOkYfLhgllIwZqmD =  -67; 
ulong mbSxmfLCyetzzKwp = 71619532076923463; 
string JswdLlH = "zgCsYSujKcSN"; 
sbyte EbRhUEgmkwsexsfuugRfkn =  66; 
string ARipOFqYXOtlDs = "klWg"; 

}public void Uxc(){
string JMWcBpnNNwN = "XxRafAENhRPOVZsIdCdOeZfTwVxzf"; 
uint XpmoBoZOWzqhSXhhCXHJkqYT = 318808092; 
double jcfIIaRyILRseOgpNRn = 4.582431E-20; 
jcfIIaRyILRseOgpNRn = Math.Ceiling(Math.Atan(-5));
string NOBIwTF = "yzmTnXLc"; 
ulong lRicjYGmQxbhBNLbAiD = 82080110923549113; 
short yYkyBciZKFDchdhjpYBimzCAeF = -14268; 
float xnants = -3.455304E+24F; 
long KCbQFaHTTPTqXyiHZyLsWOoTmpCOP = 38383613658834291; 
long AyMwemfqEeMtagtdVmhDozTdieBM = 63403835181358691; 
byte kHgYXKcsKllVV =  100; 
string IbFJl = "MusXDqbAVzUCbBwpBjbRuoBdLhqlT"; 
byte ChkUNHZgMp =  54; 
short atsDKqBwHuFyUomuZPGMjtu = 21153; 
int KBcpBjYNcqPOCDJTUNGy = 333268795; 
if(KBcpBjYNcqPOCDJTUNGy == 727970){
KBcpBjYNcqPOCDJTUNGy = KBcpBjYNcqPOCDJTUNGy + 520069;
}int EIZCWcIZtqUwzgmWGk = 607; 
while(EIZCWcIZtqUwzgmWGk == 607){
EIZCWcIZtqUwzgmWGk += 650474;
}byte PsOoEZE =  99; 
byte smRAVJBDTjZMe =  207; 
sbyte mDCmiqqOmoldOURd =  111; 
float stOhuPWPaicnYidNbT = 3.686065E-22F; 
string dlGyxOYtVPmlPgzugylDpFkbmVLh = "kaHExpDDXthQInmaMuWnIMzY"; 
uint AbFjCdRDLPGJsRWSjee = 66407346; 
short aCCOUhzCIeQRBXNSeYeDyudbfqQD = -7311; 
float XhdFJZpXumnYEDZwUxmZNE = 0.278475F; 
int BIDsVgNsTJyteSO = 69; 
while(BIDsVgNsTJyteSO == 69){
BIDsVgNsTJyteSO += 485203;
}uint VVggUZ = 3737; 
int kAIwcqgklKpYCqwlMTzpiYbCHyPZS = 4815; 
while(kAIwcqgklKpYCqwlMTzpiYbCHyPZS == 4815){
kAIwcqgklKpYCqwlMTzpiYbCHyPZS = kAIwcqgklKpYCqwlMTzpiYbCHyPZS + 887935;
}short HIZdkm = 18559; 
ulong tPMJmBVnPlyfOFCgtTkaegycefkA = 21683945770129715; 
byte fYuFFIV =  3; 
float yWSu = 7.260348E+20F; 
float qKFhowcRB = -2.844596E-10F; 
string BTjNlHBCPgemhbof = "JXSRJEFYKYD"; 
ulong LHPDXd = 8214315616824071; 
uint huNLjBxlJRpGODIXOIwyzFdlzKq = 49692318; 
sbyte aeeCFQ =  -118; 

}public void jkDTfOcTPCuWVhcxDpBmAjHMDLpBK(){
string hMJk = "VLYzwQDnf"; 
byte SbIZnoPza =  150; 
ulong wzDjOzljlWRB = 64376198829121286; 
short yzbbOSXjjHetDaT = 8683; 
ulong TaaVkghWmWPtljZWadhNKumb = 8219317430287826; 
float NqzCnUTSGawRwVuGZ = 0.01897966F; 
string AOYjbL = "glXNAKxJzzQmTRPlHVPBd"; 
byte iET =  111; 
byte AsmijMtcIqLzLhBakg =  72; 
long snlkGWjPjHmifYgEe = 84618706492305935; 
short BisM = -22869; 
int hda = 585356897; 
if(hda == 266070){
hda += 451932;
}long pGiuygyjpstLhim = 1960431507119030; 
float Dfeo = -4.70393E+31F; 
uint GLNTgMkoYmJPtSQqVBIGRtLCsq = 28016228; 
short exRRDyAaDUHGhswbKLpNtKnWbmZ = -18986; 
uint SiWcilXCpsSVYOYlltgpHjLG = 402118; 
sbyte QozoANbzJeiidiwYVzLAYQbHdki =  40; 
uint COdcMaTDwlM = 545867; 
sbyte YmzONKcK =  46; 
double nSknEtasDweaywetcANynUxeYLTLH = -2.60962E-34; 
while(nSknEtasDweaywetcANynUxeYLTLH == -2.208014E+17){
double JsBjFOTl = 2.831297E-36;
nSknEtasDweaywetcANynUxeYLTLH = Math.Ceiling(JsBjFOTl);
int? ajWAMtwfpSAoM = 263409;
ajWAMtwfpSAoM += 90725;
}ulong XdoIlNKBcpzUy = 52710436061115649; 
byte CyhqpLGoetCCIeO =  227; 
int ZmwFynn = 31019118; 
while(ZmwFynn == 31019118){
ZmwFynn = ZmwFynn + 998377;
}int GOkdxuPxauBWfqnSqoJSYcW = 894587411; 
while(GOkdxuPxauBWfqnSqoJSYcW == 894587411){
GOkdxuPxauBWfqnSqoJSYcW = 182440;
}uint uYP = 31; 
int gOkQukXqhubEzOqKOERlitPum = 352503; 
while(gOkQukXqhubEzOqKOERlitPum == 352503){
gOkQukXqhubEzOqKOERlitPum = gOkQukXqhubEzOqKOERlitPum + 292687;
}double BhUIhIMbDV = -3.38966E-11; 
while(BhUIhIMbDV == -2.089267E-19){
BhUIhIMbDV = Math.Pow(double.MinValue, double.MaxValue);
for( ; ;) {
Console.WriteLine(-5.955327E-37);
}
}ulong BnFpgmDkagmZytzMpzzAwV = 30057406740007948; 
ushort yxDSKa = 59724; 
ushort cuyf = 10207; 
int WcGVonqGczBsjbhqLgCaQHcoDexs = 773690; 
while(WcGVonqGczBsjbhqLgCaQHcoDexs == 773690){
WcGVonqGczBsjbhqLgCaQHcoDexs = 117891;
}int eBRjfXPlBEjaUdNZGFgpapsjRYdny = 152855622; 
while(eBRjfXPlBEjaUdNZGFgpapsjRYdny == 152855622){
eBRjfXPlBEjaUdNZGFgpapsjRYdny = 230957;
}byte sVTRQPZVxyz =  4; 
string GZnZ = "iHtgUBOIwfDMREKnJmAjBtou"; 

}public void GmxQDdkiybJZPYpcg(){
int ULHyqOWMPPnPsYFonqSTbsEu = 13342; 
while(ULHyqOWMPPnPsYFonqSTbsEu == 13342){
ULHyqOWMPPnPsYFonqSTbsEu = ULHyqOWMPPnPsYFonqSTbsEu + 168638;
}sbyte QABQYQfbGqwti =  -67; 
short jKc = 13930; 
short NChlVqAcyGIUYSzPpylQm = 13306; 
double kpKnKFfx = -2.577229E-30; 
double EUQMnnzDwuFAzycGXb = 4.829995E+30;
kpKnKFfx = EUQMnnzDwuFAzycGXb / 3;
Console.WriteLine(kpKnKFfx.ToString());long sLLphpjbpBzutcnRcbkNl = 56345360048974157; 
double jkugxSmpX = 4.984933E+19; 
jkugxSmpX = Math.Ceiling(Math.Tan(1));
for( ; ;) {
Console.WriteLine(2.047875E+31);
}float zLgYSRhJIUcCkKx = -5.577563E-07F; 
short NJDQdzJtcaQacQDsxQbn = -32715; 
ulong niGgCGMVxMBfPWVdAjyfIRUaL = 84262034174886815; 
float IysgOpetnC = 1.280264E+20F; 
ushort MDeDKixoFxToMBbLS = 8610; 
double mGhcKPEyGzZVUyJSWfoJ = 2.078392E-11; 
mGhcKPEyGzZVUyJSWfoJ = Math.Ceiling(Math.Sinh(-5));
Console.ReadLine();sbyte XNCbutinHKGTHqcTqfnMTAyl =  100; 
byte Ozi =  229; 
byte mSIDcPjoHGLyacwxxMjzxVzppgis =  104; 
ulong PsVuzuqnqdylTNO = 69956990821380902; 
uint uslWGSTSgVe = 64; 
double fzT = -5.898108E+35; 
fzT = Math.Pow(double.MinValue, double.MaxValue);
Console.ReadKey();float liSwo = -7.649684E-09F; 
long fPqIah = 8100213137552773; 
sbyte XdSkfbbkZFsSi =  15; 
ulong zbcGXkMKwDnzXnDWKcs = 12053616298212116; 
ushort kOmDnsIjCwMqhQDOEnMcwigk = 37634; 
string bYT = "DpUjPaVJVUNhHGSuIypiSWsuc"; 
int Ykq = 22282026; 
if(Ykq == 190966){
Ykq = 18101;
}short NNIFQiJaBGMKtFIKaGX = -1193; 
sbyte ZWzEnCA =  -113; 
ushort fbKDBmfjMbgxmBXbEZEEdPzFO = 47335; 
sbyte VgCyjVEsU =  -103; 
sbyte AONfbEKXGlYAASOpEfPPgTo =  26; 
uint msWGgzQQeoctDkbiHJibX = 1333; 
byte PptHHzgFsqsaafAKKOzmjDFO =  243; 
byte xANUkJRshyjyRVOkTKi =  159; 
int smFJaFmLGJuSLfGxgqiNq = 5087; 
while(smFJaFmLGJuSLfGxgqiNq == 5087){
smFJaFmLGJuSLfGxgqiNq = 431052;
}
}public void LdPGdOhApDJm(){
sbyte RBdYkIGyERyQqf =  62; 
byte JhV =  1; 
uint OPXYffEJVJUwalJgOMmBYimxcM = 53; 
string jxoHp = "nKAsZhCh"; 
ushort NbsCujNbVGCBVtcbJRzzf = 24501; 
double SdlVBQHfZsZajzL = -2.809906E+14; 
if(SdlVBQHfZsZajzL == 3.883035E+15){
double oXALJYF = 2.544481E+19;
SdlVBQHfZsZajzL = oXALJYF / 3;
try{
int pXSBeZwCEucSkfzOSIkQeo = 6179574;
if(pXSBeZwCEucSkfzOSIkQeo == 89561){
pXSBeZwCEucSkfzOSIkQeo++;
}else{
pXSBeZwCEucSkfzOSIkQeo--;
}
}catch(Exception ex){

}
}short wcSNGMnlkMmfnGjkMLUdFaSl = 28908; 
ulong XCPO = 13051188165251585; 
uint TeutzdpnQbVanjwkG = 859203139; 
long KwYeFSVNnIJ = 71173841934663139; 
ulong MOThmlolqniSliKwnTkuB = 64682780427912853; 
ulong hUHYIeWsGGCW = 52055612745884242; 
int ujjtHDCOQAFXbYFk = 32; 
while(ujjtHDCOQAFXbYFk == 32){
ujjtHDCOQAFXbYFk = 776000;
}double bMcxWmLhMJByOnyuQSQ = -3.254374E-12; 
while(bMcxWmLhMJByOnyuQSQ == -1537421){
bMcxWmLhMJByOnyuQSQ = Math.Floor(1.198824E-14);
int[] mGmyLGdZUHPUliWDULamnVjggDTx = { 3384356 , 41588 } ;
Random ptgHosMIcukAPDFDEVoSpcZNMI= new Random();
Console.WriteLine(mGmyLGdZUHPUliWDULamnVjggDTx[ptgHosMIcukAPDFDEVoSpcZNMI.Next(0,2)]);
}sbyte hNHgFnJnXQj =  100; 
float PNAsGEEIFFD = -8.899269E-10F; 
ushort IapCMlzSJYpUpYzIwkIXYhZQDNB = 9880; 
string qUospPUtffqURuoKnJ = "WgdMxX"; 
uint BZZ = 32463977; 
ulong IxFXJ = 57735950001174565; 
int TzSbfynhTwQNttgu = 94030029; 
if(TzSbfynhTwQNttgu == 854345){
TzSbfynhTwQNttgu = TzSbfynhTwQNttgu + 90292;
}short xid = 31249; 
sbyte ThxZfCToKHunMDSWuJXiHYFT =  40; 
ulong RGWuoZ = 18201745959061076; 
long fsEsPiBPuaZjUE = 82300304106906274; 
sbyte JaiGaxsJVE =  -8; 
uint wtJfh = 264468515; 
string IfMbVkzbJTmoJOZUOYJpnqedmW = "tFF"; 
byte nQsdTYBgjUVGnPMFKkuaGKGqa =  171; 
ulong jfxdGwbTwVaPyCgtwAPzjxKtkmlFj = 4675407142614877; 
float TOTiOjFDeElVCTpPOwdTtixJuSQGW = 6.098782E-38F; 
double AWLqnViHKhaYfCqTNi = 2.758665E-09; 
double uYHBDpqbXGyAAUlKaxLnqAMjwqC = -5.865045E-25;
AWLqnViHKhaYfCqTNi = uYHBDpqbXGyAAUlKaxLnqAMjwqC / 3;
Console.WriteLine(AWLqnViHKhaYfCqTNi.ToString());int sXdzRezVMhaUWCAJZwUw = 56041198; 
while(sXdzRezVMhaUWCAJZwUw == 56041198){
sXdzRezVMhaUWCAJZwUw = sXdzRezVMhaUWCAJZwUw + 328586;
}long YJL = 47970034735813909; 
ulong CtdORmiNIbdkibPKkuAxxWc = 4006574891000022; 

}
} public class dHdjkemeAoqIHjZoDnjH{public void uqGpGheHZk(){
ushort xhCX = 39368; 
uint hwWhGw = 424690969; 
ulong IndqtKpzTFW = 33201250753098627; 
ushort XOdATzjZVPM = 22956; 
short WpMdzlAMUjZX = -5571; 
string QItoaKAZSAkgzYPjQhig = "TfFlRlUTBVcfqfhscoIy"; 
short EmfaOeaiQmWbXCPdizGgsFiahT = 9899; 
float TBKpZCsgQzPTqfQjohHEYfi = 8.699512E-24F; 
uint deSUyiwXMlLXZyXsaRmaAtRTLbnON = 8671; 
sbyte EnbXEqKVhdXEkHf =  -122; 
ulong ZXCALnZ = 5569808556331468; 
uint jWuyADeqoUoAui = 99458675; 
sbyte iVdDOYtWQnYKEZLkVesMBuyhxt =  52; 
long MUj = 25607781610960180; 
string NQtFppPjmIHxtPzWgu = "JVeuOzdEg"; 
float WWJWHHPRdu = 1.585552E+25F; 
uint DPgHfxCPi = 662314; 
long FglNJqYLLOxhSg = 48115122677887717; 
long WaxNhNmnYljKRImABBSaLznolzpI = 60905876146938852; 
double EXBhjGWF = -5.310041E-24; 
while(EXBhjGWF == 1.417228E-31){
EXBhjGWF = Math.Ceiling(Math.Tanh(0.1));
object FEoXUBEoIsHZfgZXGaPdsUPKT;
FEoXUBEoIsHZfgZXGaPdsUPKT = 3.524799E-20;
}long zKXsQyzALPFd = 5142225572895302; 
long YgVCHxiP = 1582982410324041; 
short zUkllGeECjfBGHGnMQxnRcpVOOU = -19132; 
sbyte JKXuFSZNVoyc =  98; 
double AkDDibBSadxEozyMUwiBx = 1.373667E-23; 
if(AkDDibBSadxEozyMUwiBx == 9.509539E-19){
AkDDibBSadxEozyMUwiBx = Math.Pow(5, 2);
object UcGLjonlcjUuluRtFHTTBEKz;
UcGLjonlcjUuluRtFHTTBEKz = 1.477141E+38;
}short UeIpfHoqt = -23646; 
sbyte iRtcSpDtoERzURYzWFVuWTDOsbyj =  20; 
ulong wYQTdJFmmIYYpnixoJRwPlOaonG = 14024354856450979; 
uint XUPq = 6526; 
ulong UmBiGiCOtfikQFQdUwupjyIRIsqP = 54608760536948359; 
sbyte lKQbhEnpnteMxWnxN =  -15; 
int BmRNMoIFiMjOYtjKAipe = 350173; 
while(BmRNMoIFiMjOYtjKAipe == 350173){
BmRNMoIFiMjOYtjKAipe = BmRNMoIFiMjOYtjKAipe + 306470;
}long qFpXxxaYSdoTVJgCj = 38421787393149325; 
uint ihUcx = 13541860; 
byte xlsCtlSwUyiUkDOuDF =  202; 

}public void IMQVsRQYbTlOwDfXXkKBMCL(){
long jXFfITbsBWLw = 48856007005817035; 
ushort IyQyqVobfagEhTk = 56583; 
ushort kxVGuCBbheShKAWLohpkCfczRKoTb = 43724; 
byte aba =  248; 
int dnWp = 19787009; 
while(dnWp == 19787009){
dnWp = dnWp + 644171;
}int EdEiyetxPLmIZz = 858366968; 
while(EdEiyetxPLmIZz == 858366968){
EdEiyetxPLmIZz = 960595;
}sbyte dbolqSpUtI =  -70; 
int gxREV = 131747; 
while(gxREV == 131747){
gxREV = 267385;
}short ppPJiJopZCU = -17906; 
ulong gVqFxwuOwqeEtaaLa = 7974903726925390; 
int OgVWshdasbx = 911628094; 
if(OgVWshdasbx == 892313){
OgVWshdasbx = 472385;
}int pnpnCTpMYgQIfTeGHBu = 41914220; 
while(pnpnCTpMYgQIfTeGHBu == 41914220){
pnpnCTpMYgQIfTeGHBu += 695258;
}int afafGfU = 237; 
while(afafGfU == 237){
afafGfU = afafGfU + 61187;
}string fTCupYQpVteRib = "IEXLpGYbjC"; 
byte dgOhqLZjhfpnkjQfMCo =  179; 
byte RowtMQgRsTdsMiglNgjSfCn =  175; 
uint QSpJlC = 316880; 
short MYZjcpOyjTyHYAOh = 1208; 
ushort oaMTGZnCKHDWwSIBPhZP = 52778; 
ulong AHIjUQUeabJbzZftjOPeRIWb = 24162786183715152; 
uint llXQuLPC = 891383; 
ulong jBKOQbaJZSKGwFRVdAySAmOTw = 28436287335207455; 
double AolLDQ = 6.433834E-22; 
while(AolLDQ == 5.205847E-14){
AolLDQ = Math.Exp(2);

}float BSot = 65135.08F; 
float hEJdRedcVeJNDNIuJZmxQcPJOZ = -6.094286E-38F; 
double iwKgZCRHI = 1.232857E+36; 
if(iwKgZCRHI == 4.014809E-34){
iwKgZCRHI = Math.Ceiling(Math.Sin(2));
Console.ReadLine();
}string yQcRHsakYhOhPsUuMKCZCzkFUHuI = "NSBdqHdKaLyoLiWsfmMgJGZg"; 
string ZFyXZYtxkpneyL = "KLYdLQoolOuJCghTubVIzlcM"; 
uint oXGebPbpYUacqHKpkjGpRpdxUcD = 34; 
ushort mEPBqJhNFHkhmTqLnYiaS = 7480; 
byte ZUjkY =  61; 
double BlNACfsHdybJGhREEsndixQgtMByA = 2.073075E+21; 
while(BlNACfsHdybJGhREEsndixQgtMByA == 1.660247E-20){
BlNACfsHdybJGhREEsndixQgtMByA = Math.Pow(double.NegativeInfinity, 2);
int? tBQzM = 6696840;
tBQzM += 38582;
}float htNyDGALtD = 2.488261E-08F; 
ulong wBeqmhVzSyjFZEnDWCnSBp = 53927195527714940; 
ulong JJHqisgL = 85851337214150652; 

}public void RGjgYkfmhtciVK(){
short XVpXXtFuDzROXJaEeEXXE = -26892; 
long CdbZdCzamHpbpXA = 74707091714523290; 
uint AeqMqgTkSwTs = 31385821; 
string RCqtYngdzWOBkdjOCHFygnVAxRyDH = "wADBDP"; 
double ukYnmu = -2.095478E-12; 
if(ukYnmu == 1.177414E-22){
double ESEGRWgRFysCoYKCwQwmuzT = -4.945901E+23;
ESEGRWgRFysCoYKCwQwmuzT = Math.Sqrt(3);
ukYnmu = ESEGRWgRFysCoYKCwQwmuzT;
Console.Write(ukYnmu.ToString());
}sbyte ZbDkKkaZeceJdKBwB =  -62; 
double xUJwfxsWlgWTODuoLFFjM = -3.645831E-39; 
double NHgueftgfnTdOYflUBpkLVJ = 8.341567E-28;
xUJwfxsWlgWTODuoLFFjM = NHgueftgfnTdOYflUBpkLVJ * 2;
ushort wqARULiKsBzE = 21086; 
short xcowdTIKqzZOZcRqUuGZeCuNzmOq = -2460; 
ulong BgE = 34178974546635556; 
sbyte UXiHCjFiLsTV =  37; 
uint QepkDzAHxxSqibfmysbTBehQojSP = 68761547; 
long zPEwyxHUwpM = 75400972116437560; 
int JxLXBWsHKTNLBSuyLPa = 13810054; 
while(JxLXBWsHKTNLBSuyLPa == 13810054){
JxLXBWsHKTNLBSuyLPa += 928901;
}sbyte OKjJmiLQsSzXAo =  -59; 
long ZBiacKUpwRzkkt = 16469277877104201; 
int bbGlSCyTMOtxXwMaosRqmGhA = 559241; 
if(bbGlSCyTMOtxXwMaosRqmGhA == 345949){
bbGlSCyTMOtxXwMaosRqmGhA = bbGlSCyTMOtxXwMaosRqmGhA + 228586;
}byte XtZadJzUuGzyZFxf =  217; 
double SIkDxUfO = -6754.181; 
SIkDxUfO = Math.Ceiling(Math.Asin(0.2));
object YUiqHeXV;
YUiqHeXV = 1.152811E+24;double jGViKVezgc = 6.791157E+19; 
jGViKVezgc = Math.Ceiling(Math.Atan(-5));
try{
Console.WriteLine(jGViKVezgc.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}sbyte fQghCtAffgkkWtbCh =  -27; 
float wUQJgcjqzWOtye = 2.389161E+34F; 
int bijTklZikdHJSfJMglZyaAii = 18898057; 
while(bijTklZikdHJSfJMglZyaAii == 18898057){
bijTklZikdHJSfJMglZyaAii = bijTklZikdHJSfJMglZyaAii + 832794;
}float XstJ = -1137.044F; 
long KYSgKQeJKLYoYgPDZb = 78117960085391117; 
ulong MZBFDwNcHczRfKhd = 73303830420067952; 
short LEEyVhdFWLjtNDuNVXEw = 27460; 
sbyte qBIyVWVpqoULjzigqNCOyTAQDxQVM =  31; 
float CFmXnKhIawTeSSjoMRlIuw = 0.0001239307F; 
string VAgaELtiiFllSIFUNbawXKlCi = "unqgdPWxOJCaRiLKKGdBYFcBJAy"; 
int lnFRJpfRbCxoxUWsMkSKmZHzJXP = 8245; 
while(lnFRJpfRbCxoxUWsMkSKmZHzJXP == 8245){
lnFRJpfRbCxoxUWsMkSKmZHzJXP = 325838;
}uint uWihSGTIdHkCLfhWo = 92229572; 
long HgiMCUaEY = 63041944111410579; 
string uqDRz = "efJCnMwUoOPaxp"; 
long efClHsEMtmGSmVeoLqyPqcLtEZ = 46467193211303775; 

}public void yplOMCXha(){
sbyte ItJBpfTVCcuhJHnQeR =  34; 
double LJHYzDLnuUKmasL = -8.20573E-29; 
if(LJHYzDLnuUKmasL == 3.787345E+22){
double iDgyMT = -7.379393E-39;
LJHYzDLnuUKmasL = Math.Round(iDgyMT);
Console.WriteLine(LJHYzDLnuUKmasL.ToString());
}short Vuac = 6289; 
ushort JQTbsoUSUaVMmfenlAMWhUfHGuSN = 58811; 
int skjxjQylBaDtCGRdWRgPD = 470939331; 
if(skjxjQylBaDtCGRdWRgPD == 713087){
skjxjQylBaDtCGRdWRgPD = skjxjQylBaDtCGRdWRgPD + 250339;
}ushort xhXqsDCdJUyPnkVFBEbOqVVOOfAM = 25607; 
long EaneTGcicpZmXKa = 89040739692968148; 
int ljEmcPJawUOZgcZz = 970387; 
while(ljEmcPJawUOZgcZz == 970387){
ljEmcPJawUOZgcZz = 970625;
}string QSijwsqsIEHxAWZCqmCQlpB = "kPMmknSnfQxjGAGd"; 
int sDwEZzYWawxnCB = 6519; 
if(sDwEZzYWawxnCB == 618079){
sDwEZzYWawxnCB = 75172;
}sbyte MtpKAPVSIzmQY =  115; 
sbyte QmonnxRRnWSYAayzoYRc =  -64; 
short jbiOYaMCuDsPtPTfoCzwEXWsHbPCZ = -24427; 
string BnfmeJTRDuywh = "dJwSCCOU"; 
ushort eRgXljCnB = 62502; 
short KGbGJ = 10161; 
float ACjxXff = -5977548F; 
uint GSTimDWFsTjVjVZZQRV = 118272941; 
string NEHKZfeuJZ = "KmRyOdNcBopG"; 
string YfppJJAEFgGGSWAnz = "WSlMiscSfdCYyXHi"; 
string OdaqRfJKShlREmRPKhplXXnRDxcHJ = "ZJUAAuBGbfdzBUGoAogI"; 
byte TPcRQiOwdcbjiDWd =  191; 
ushort dfjizytknX = 50374; 
uint uAQ = 44; 
long qRHcbypMcswCKtjtKH = 69401558064320048; 
string KNRHeSXlAhhbgIiVClPIkfgz = "ogtshNzyBcpBPySxXTDzCmQPEW"; 
string sGEHtNPMEUquJEHGSwMPXlm = "FZscqEOlL"; 
long VzKgc = 31773389954881454; 
int WiknQgsq = 38; 
while(WiknQgsq == 38){
WiknQgsq = WiknQgsq + 1284;
}sbyte KXAMgi =  -60; 
long RukKxj = 11460621829101637; 
double hINIVMSVxzSzUdOVB = -7.91835E+36; 
hINIVMSVxzSzUdOVB = Math.Pow(double.MinValue, double.MaxValue);
object nXqVDcTKBpWk;
nXqVDcTKBpWk = -5.172459E-32;float kSsJTfOlASFXgpNzAXxIHdVXMCX = 1.068922E+35F; 
int bwbaEhtKBQl = 530093147; 
if(bwbaEhtKBQl == 682114){
bwbaEhtKBQl += 642475;
}double NxclmhWuWqhXnTDECep = -1.788156E-14; 
if(NxclmhWuWqhXnTDECep == -4.79536E+30){
NxclmhWuWqhXnTDECep = Math.Pow(2, 2.1);
try{

}catch(Exception ex){

}
}
}public void cnKSQ(){
double eGWTDNcgoFxoUlMVnGtyfheR = -3.312956E+26; 
if(eGWTDNcgoFxoUlMVnGtyfheR == 5.24761E+16){
eGWTDNcgoFxoUlMVnGtyfheR = Math.Ceiling(Math.Sin(2));
try{

}catch(Exception ex){

}
}ushort GtdUeDjjHpusVWCtXDIcLwOYj = 15322; 
short RqEVVkZkjSeYiuR = 19139; 
uint HBhbqZCsfjFfKSlLsoNA = 689796; 
double ieyJTpFZxDG = 7.012896E-14; 
ieyJTpFZxDG = Math.Pow(double.NegativeInfinity, 2);
Console.ReadKey();short BlUX = -12130; 
short iOoqVcDEJoQEjV = 4398; 
int EPpYHI = 207072715; 
if(EPpYHI == 742493){
EPpYHI = 820212;
}string bqPpjzLGcNGtgj = "ghXKOtCbzCgsgp"; 
long NGTPmwFtdxYMEsnCOPAkPgnd = 14024825264935477; 
ushort TNnuiMDEGswPzfnFEsQzaGsPoXdQ = 20501; 
double ItHABnuGOIkCWJLQkBzDlAUWn = -0.001382312; 
if(ItHABnuGOIkCWJLQkBzDlAUWn == -1.548221E+11){
ItHABnuGOIkCWJLQkBzDlAUWn = Math.Ceiling(Math.Tanh(0.1));
Console.ReadKey();
}float WVRjZkoinDKb = -1.034105E+17F; 
ushort bfyOpqADGDsqQifHzg = 33795; 
sbyte ZahlUBUMlMT =  16; 
int DdhcFxoEyTYsf = 70677; 
if(DdhcFxoEyTYsf == 204973){
DdhcFxoEyTYsf += 220909;
}byte epD =  105; 
ulong QuQzOlOEDpdWNaxEnmC = 77389868053623475; 
ushort NFHORzjHukgqSC = 16154; 
double zmGCboTwgh = 4.118083E-32; 
double xhWClWUcweHgxOtTdOVMpN = -1.849981E-29;
zmGCboTwgh = xhWClWUcweHgxOtTdOVMpN / 3;
try{

}catch(Exception ex){

}ulong qnkhVwUnlqquq = 79071535373587838; 
sbyte snAJgRtqB =  36; 
byte YmiFGuclkBmUApuMaVCjlbTU =  104; 
ulong xTZRNgPgbTGKQhUbRaGiQxH = 14073625324211496; 
byte GHskKBNKjsTwlLbsZbTQEYalKl =  159; 
sbyte jIFxQkCC =  77; 
sbyte SAGdOUtwYkWsxtpI =  27; 
sbyte yUGzaLnZwBAQjUtYe =  -39; 
double PSliamoicBDWwnJ = 3.682166E-22; 
if(PSliamoicBDWwnJ == 6.482174E-12){
double zwNycwloJJMGJyzNwfWVXEwVlNM = -3.447553E+29;
zwNycwloJJMGJyzNwfWVXEwVlNM = Math.Sqrt(3);
PSliamoicBDWwnJ = zwNycwloJJMGJyzNwfWVXEwVlNM;
for( ; ;) {
Console.WriteLine(-3.922684E+18);
}
}int VRuQGJGQYgLhcKtGbIG = 3736; 
if(VRuQGJGQYgLhcKtGbIG == 257395){
VRuQGJGQYgLhcKtGbIG = 562400;
}short eALGJzeWUXWmEYj = -10647; 
uint jHKPkWUnnkmugmplCqmCbJXNzJfwc = 1760; 
short otKNooPMwjTKBQJXsyTeXWBJXpDQA = -19082; 
float WwST = 0.0008931222F; 
byte yQHA =  20; 

}
} public class YSxy{public void XolbsTbQZMYlkpRNeYQqTk(){
float hpbFTqKubAWNKuBZFImo = -16051.15F; 
ushort mpUTGfR = 47710; 
uint UbSyXIcFkLtedLyQNl = 540026740; 
sbyte USqDyQOWIkoeBzhgLF =  -63; 
float LPimixQsGNVw = -7.735847E-29F; 
string ujlQnc = "QYjZJGB"; 
ushort okicipnuYE = 3135; 
uint KksgbqubdeJH = 2798; 
double gNetHgfXXXdWRS = 6.785173E+37; 
gNetHgfXXXdWRS = Math.Ceiling(Math.Sin(2));
Console.WriteLine(gNetHgfXXXdWRS.ToString());double RATbohmKcKsIfWIWkRqlCH = -6.820914E-22; 
if(RATbohmKcKsIfWIWkRqlCH == -6579.178){
RATbohmKcKsIfWIWkRqlCH = Math.Sqrt(4);
for( ; ;) {
Console.WriteLine(3.669481E-24);
}
}ulong UgtqYZ = 74079001358166507; 
float FUtzaUFsFMVOcYsBMkNlASsl = -8.649534E-32F; 
long ESmR = 37844642110969944; 
uint Mle = 8909; 
int EEHQGoNDgbVmDPNAdNkqfOye = 68625651; 
while(EEHQGoNDgbVmDPNAdNkqfOye == 68625651){
EEHQGoNDgbVmDPNAdNkqfOye += 145375;
}sbyte elZoGawD =  99; 
long mkNYlZMaFKmLPgDcWQH = 62388547362687352; 
float sJyu = -5.435414E-25F; 
float Lmg = -5.090734E-29F; 
uint SBkidYzMnnGsBFIxD = 950902041; 
byte PpH =  239; 
long kDB = 66409043216381332; 
ulong TWigE = 49298016991004269; 
short ckFlwXmjOkNYsnQSm = -29767; 
float dHjcfnEhMVDJ = -5.696105E+10F; 
short acugGNwVQSbimYqzqWy = -5179; 
string ggtfySJdULZjRkIMN = "knmmmuUNFds"; 
sbyte SGAWmNBp =  17; 
byte YpjchgRFigkNnTUZkLmFMM =  131; 
sbyte snuYmaHaCnWkSGzmOZ =  -112; 
long kctOoFqJamQczPSxGWfFMNFYOt = 2358926480474536; 
sbyte bkYwpsBb =  -97; 
sbyte RHRhcXcfzywaV =  1; 
uint bAsDMIHuwYK = 3846; 
byte RFZniBQKAyHJNHjcVO =  7; 

}public void COHIiZxMemPMlUE(){
ushort ltQkQSefYTOlVPAsNbdXseideGl = 42867; 
float TdOgJuknMVCDkfJ = 9.671224E-27F; 
uint tzTAzEhkMgj = 44266020; 
byte aLyPfehAohwQUdwEx =  193; 
short YZEoECjAgCpeqKeX = -29242; 
ushort klZEsdNmgpEOS = 5663; 
long ahikEUItNSpAjYyBb = 73528818102022692; 
short iEOy = -32145; 
int eQUjPp = 619842; 
if(eQUjPp == 747440){
eQUjPp = 266574;
}string ScNzWxE = "WXwoRAQZupTcUIHSFSK"; 
ushort LzDkUpCaYdjEAUXPAz = 5309; 
ulong fyuaokwVTeMkalzSeXjoYwlKA = 40359521885884222; 
ulong bhkFefffZytAIzWwCUC = 29770412203117500; 
float MfImHxLUsfiTpmlfVekgJSMa = 3.844291E+30F; 
float JKHunpmsLlbclC = -6.204982E+21F; 
ushort tXA = 27187; 
double BLfTuItMFnkI = -5.957803E+19; 
while(BLfTuItMFnkI == -4.24818E+37){
BLfTuItMFnkI = Math.Ceiling(Math.Tanh(0.1));
try{
int nLnUDkuYdSoyYpBpFutYGbooqQXk = 7862264;
if(nLnUDkuYdSoyYpBpFutYGbooqQXk == 71679){
nLnUDkuYdSoyYpBpFutYGbooqQXk++;
}else{
nLnUDkuYdSoyYpBpFutYGbooqQXk--;
}
}catch(Exception ex){

}
}double hHcAhVslwZDNmjCdseLJtEfLs = 4.827977E-12; 
hHcAhVslwZDNmjCdseLJtEfLs = Math.Exp(2);
Console.WriteLine(hHcAhVslwZDNmjCdseLJtEfLs.ToString());ulong qaULIc = 26227031688044602; 
ulong EHznHUAooiEMtwuoIgi = 31836240207430690; 
uint sdaLqpCJyAyCFxzziaG = 64803203; 
long OfBaPWHKOYK = 69930988757435719; 
byte MiBhsczVASXJNlFQpMNR =  202; 
long VztVhXBQbAKbWZSyT = 74442187235682414; 
ushort PfTtSdGGJ = 64233; 
sbyte YZcWjdjIQyo =  93; 
long uwi = 67230148651389548; 
sbyte YpujzLRQpeQdohOyqRaKRKkxz =  118; 
int mnsOdllqwnRhRkeqsgL = 9587; 
while(mnsOdllqwnRhRkeqsgL == 9587){
mnsOdllqwnRhRkeqsgL = 146783;
}ushort nfX = 46607; 
uint lDbMc = 7; 
double bdZSTGGTesmkIZ = 0.04827113; 
while(bdZSTGGTesmkIZ == 5.319891E-22){
bdZSTGGTesmkIZ = Math.Ceiling(Math.Asin(0.2));
Console.WriteLine(bdZSTGGTesmkIZ.ToString());
}uint FIelaMlYpEJoRJVUxOoOdNwS = 382397145; 
sbyte AbxZBppLPDRNeLBHQYGxdkMUTLMRm =  -64; 
ushort EnAnhJOBtkfTXCKtmOb = 57796; 

}public void MUopCfQFAzCikjRllAgjfdT(){
sbyte XqatEzlASRtkXGEYWUNFKNsTh =  -125; 
short KJJbjzFPPlVZpKssyaoTEliifBh = -19787; 
double lDjqQji = -1.255041E-37; 
while(lDjqQji == -2.254336E+21){
double nawawgMwqyAAWJpuQHZsPZTh = -2.369768E+16;
nawawgMwqyAAWJpuQHZsPZTh = Math.Sqrt(3);
lDjqQji = nawawgMwqyAAWJpuQHZsPZTh;
int[] SPdykslgPYNeRPZsKJwsjHYC = { 1473 , 98412 } ;
Random GZohmQogWECucnFxiFRUUC= new Random();
Console.WriteLine(SPdykslgPYNeRPZsKJwsjHYC[GZohmQogWECucnFxiFRUUC.Next(0,2)]);
}uint BLIGkRCMGtXZDhz = 890500644; 
byte YIuRVabgIQdZpHP =  76; 
int qsLTpJlAjcJxWHO = 542975; 
if(qsLTpJlAjcJxWHO == 884466){
qsLTpJlAjcJxWHO += 731994;
}int KUVCtHsN = 899222949; 
if(KUVCtHsN == 882451){
KUVCtHsN = 752708;
}sbyte MGxQYykUxkPe =  126; 
byte NMhbYuhSSSpQCxeUOxyun =  77; 
ulong lJbeiSmN = 18231908296707360; 
int EYJIKzspbICPwxXXQotcJaG = 143388; 
while(EYJIKzspbICPwxXXQotcJaG == 143388){
EYJIKzspbICPwxXXQotcJaG = 166513;
}string BQBMRCFZDMS = "FkUQqTOWRFCeEsfAuQehZsbZU"; 
long wkpQpQ = 34328822759704498; 
sbyte VnwizxeGNILRljU =  103; 
int tLCOkotsOlUzLNTUfLADJZ = 70; 
while(tLCOkotsOlUzLNTUfLADJZ == 70){
tLCOkotsOlUzLNTUfLADJZ = tLCOkotsOlUzLNTUfLADJZ + 350738;
}ulong tFMIbwW = 1902342646336135; 
byte XYqdjxddtEQLCibDGToNcMyXYIsN =  140; 
short ZQMhtyfjiJ = 5541; 
long gxCXODcYxCUlCteCdpBboRVQa = 73793816079116594; 
int DguZZopDnSWSeaIfsdHDoBMWf = 555180; 
while(DguZZopDnSWSeaIfsdHDoBMWf == 555180){
DguZZopDnSWSeaIfsdHDoBMWf += 431884;
}sbyte CCwJqCMZnHYePbAcaMuxdDbwzTD =  59; 
string RwOWDAmxb = "VEbUOSpxdlpLbMSZB"; 
string yZaYbPDp = "zZQFKSCBBhIGDfIPWftceDZllF"; 
int FZekDyXdxLbjkszstFUTsjfKinO = 10449491; 
if(FZekDyXdxLbjkszstFUTsjfKinO == 112381){
FZekDyXdxLbjkszstFUTsjfKinO = 283270;
}int CcxdehEskejSMdHTHKKEwUwboKb = 85; 
while(CcxdehEskejSMdHTHKKEwUwboKb == 85){
CcxdehEskejSMdHTHKKEwUwboKb = CcxdehEskejSMdHTHKKEwUwboKb + 588980;
}ushort pWCPLORUFZwcSuKMxFcBIIuPwfBA = 56024; 
byte mohqXeUYQE =  254; 
ulong THj = 44039723486033194; 
long NuGmsbwLiOLcNHCGm = 85712228921377446; 
short KYLsSJwsB = -8622; 
int YupKTAHx = 956342536; 
if(YupKTAHx == 4847){
YupKTAHx += 630359;
}ulong RGKxcQL = 41699113499408884; 
byte MKyclVxjUlMmyslTlCVKfi =  41; 
long myImeKWyEoXsgFKnlh = 55814283023184637; 
ulong EFDCOA = 77778904117755769; 

}public void coBfWkwUbFKdlubLZWQUYMwFIG(){
byte oqbiKgasbZRVhLCW =  19; 
ushort dNlRGZVUqTVzqYWGKECtetK = 45626; 
double UNnkA = 1.558699E-23; 
UNnkA = Math.Ceiling(Math.Cos(2));
try{

}catch(Exception ex){

}sbyte fXilzJBJVdGH =  15; 
sbyte ncibDIlkyPaTXsWhIyNny =  -109; 
byte CcBlnZlgfVk =  134; 
ulong GLUNLxyQDpYWXiQR = 69172208459619180; 
double EUNchwthScuLtQS = 0.9021047; 
if(EUNchwthScuLtQS == -8.164867E-13){
double IaLWZgDUQmSE = -4.435091E-35;
EUNchwthScuLtQS = Math.Ceiling(IaLWZgDUQmSE);
Console.Write(EUNchwthScuLtQS.ToString());
}byte ESfiLVlgGjlQqZFfaSeHGn =  242; 
long cLeZVZPjPaGnYZCWFUpJpG = 51758209403703782; 
short zzIjghKjxHDYZScSEKatIZuxwaf = 25874; 
sbyte EJLLQ =  -1; 
short QyjflKdsebYa = -23812; 
double Wmzdl = 4.553691E-31; 
if(Wmzdl == 1.807172E-33){
Wmzdl = Math.Exp(2);
try{
int VfYHEPy = 4348534;
if(VfYHEPy == 48670){
VfYHEPy++;
}else{
VfYHEPy--;
Console.Write(VfYHEPy.ToString());
}
}catch(Exception ex){

}
}double DNLL = -2.290934E+28; 
DNLL = Math.Ceiling(Math.Acos(0));
for( ; ;) {
Console.WriteLine(-0.05040219);
}string UwYkSmMMe = "usqTowddbjSIImxqWlsdQhDRK"; 
int CORAe = 635562; 
while(CORAe == 635562){
CORAe = CORAe + 303673;
}int XkoUtmOL = 52815386; 
while(XkoUtmOL == 52815386){
XkoUtmOL = 14515;
}string PBjJqCokK = "kagLfHFggGTJyxRtNmAYLwG"; 
float LfXUxVdjHbNFdwLWmUxtgcNSXOcM = 1.065517E-35F; 
ulong lOuBHmLKQAQueP = 81295774551299575; 
ulong tfXISszEmqtoajpZjMDH = 6704893603819322; 
int lKXNhBpcSz = 25; 
while(lKXNhBpcSz == 25){
lKXNhBpcSz = lKXNhBpcSz + 390725;
}ulong MzqyZiwasOEHwPZZfJs = 36020838102642093; 
double ZNKwqXfW = 5.06304E+15; 
ZNKwqXfW = Math.Ceiling(Math.Asin(0.2));
for( ; ;) {
Console.WriteLine(3.724325E-24);
}long aaltoSEqfhsbCRIdLdZuYa = 12807924398198557; 
double lHOA = -9706255; 
lHOA = Math.Ceiling(Math.Atan(-5));
ushort GywSPkwRNdwMLDlidwhXgwHcwRco = 29323; 
ushort UdIGqkhVQPUZeNhmpCD = 12392; 
string xQkQsxXhgtox = "IdXUXWBsilwDpnFM"; 
ulong mDDIRuCTLXIJCLImBisIFnEZnEQ = 32104367956581160; 
float alEtdkDKALKVcOwtfpZZDtnFnRDG = -1.965154E-15F; 
string NoAXPnBcapOSfsZwWcmib = "uHXYemJXzjJbhNnzESPXYmGxzWS"; 
sbyte idUHSVSaRintcRFiejlg =  -9; 
uint teWhbBAiwDopf = 38705753; 

}public void SxzNJBEWwam(){
ushort GZjKJBDVORAqaTROCzwKUu = 46856; 
string MSBaManfxPDeHymSueFYxI = "MiUCSKRlnjXWJbVbQkkE"; 
string JVbhSBofbp = "kPRSz"; 
ushort GEnVJBFQzcPOke = 37430; 
uint YBLRqWihHSymejqP = 969124872; 
uint OmB = 45873; 
string qnfZHqsTY = "FnHfNbIgEuTzYwfdyMMahP"; 
short RUCTRWMptdkKUUakSwzItS = -13150; 
long ETGhYDQZWnSBPpIIEfUjxiELCh = 38483348231853594; 
byte giwC =  105; 
long GiKceRaPNwCyupUGQQAtEonLQdFC = 14312046639833725; 
float IzGDl = -1.417156E-16F; 
uint NlDFLWqupVZmalAuccNMjssOL = 2; 
ulong cew = 55740566490089781; 
ushort IITSIlghjgDPJwTOUEVEsmQEX = 52133; 
ushort CXuUqsKL = 59764; 
float OHTTXSADxwekeXRz = -4.46395E+24F; 
short SSWGlMIypLiSbdgXzuTLylXOyMp = 29614; 
uint pTHXJWLnHkkcmMmljaQUaaXEyG = 153517360; 
sbyte xKXyuoldwjmkNJBw =  125; 
byte kDogHTpVcBcbDegPB =  23; 
ulong XafGGIPmodbG = 48321037675995113; 
string iDsiKgGTZqjUF = "xJBYdcHZ"; 
uint tWghIXOlmERDny = 669595194; 
long dKxPcODycsAbOj = 24893130374402466; 
int HbtTwdbwwkWPaFUsGkVxMlPIAtIi = 19177917; 
while(HbtTwdbwwkWPaFUsGkVxMlPIAtIi == 19177917){
HbtTwdbwwkWPaFUsGkVxMlPIAtIi += 464445;
}sbyte wJc =  122; 
short HDmnU = -18135; 
float RahglwlLubwSMkaVIDxT = 7.337357E+34F; 
sbyte fIGmukzxobsiTamMPLnaEzswDsH =  -41; 
ulong LUS = 43822626129595424; 
double LSRbIpKdmKhsIpjLcF = -2.528939E+10; 
if(LSRbIpKdmKhsIpjLcF == -0.005869494){
double WQueyEVHDLpZbeXzkLTun = -1.941767E+11;
WQueyEVHDLpZbeXzkLTun = Math.Sqrt(3);
LSRbIpKdmKhsIpjLcF = WQueyEVHDLpZbeXzkLTun;
try{
Console.WriteLine(LSRbIpKdmKhsIpjLcF.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}ushort LGGZeIEYoCa = 21866; 
ulong EfUFdlzLLfKEfzzQPnKlGlJHKWRqm = 78853432127028412; 
short iSZVApBDf = -204; 

}
} public class MMHEsqynVgugzfH{public void iyQ(){
ushort ybCuHqipjuMROTNc = 53662; 
short FbyfPMCPToq = -26486; 
short YoKf = -19889; 
long oXL = 67425080516526402; 
sbyte RzaVMJfCpbTowOQF =  -115; 
double EqpdMtgxRDYqYWRlBoO = -240.8552; 
while(EqpdMtgxRDYqYWRlBoO == -2.94986E+36){
EqpdMtgxRDYqYWRlBoO = Math.Ceiling(Math.Cosh(5));
Console.WriteLine(EqpdMtgxRDYqYWRlBoO.ToString());
}float olsJLf = -8.339532E-22F; 
ulong szJkWfIZFtD = 77042595333217037; 
long pfixJZWRjoc = 39877862702264366; 
byte tCFPEMsATHEdZctAPnfMhudBLTbW =  144; 
short SAuWEFHfnEsqJz = 13434; 
ulong aiiKKBOZyedKKxWFj = 24538565093485348; 
float Qdwbae = -4.138333E+30F; 
ushort NiYGXg = 33526; 
int FhwMzOgiyknaC = 362857868; 
if(FhwMzOgiyknaC == 48471){
FhwMzOgiyknaC = FhwMzOgiyknaC + 53481;
}string jBCzyDFZhJuGNmZuqWn = "VcLgfkqGIbdyAPuHUnakCjoCUw"; 
ulong tEFfPjKhPuslkZsgGWg = 18773129424228941; 
ulong ilLyMmdyYJoemRBVOaDRhWY = 70215018798672956; 
float uGhIPzMnKImRTGCZhhcQUGmnxIZmZ = 2.451907E+20F; 
double gSksnZCQDzoLyWg = -4.660182E-33; 
while(gSksnZCQDzoLyWg == -5.019291E+14){
double TkhzzoOTptMNpH = -7.326237E-34;
gSksnZCQDzoLyWg = TkhzzoOTptMNpH / 3;
object sgXDZLTOVCLmyddSzcRcsnLwwekti;
sgXDZLTOVCLmyddSzcRcsnLwwekti = 4.992552E-27;
}long hRpyIdxDhVABGyc = 11796432553501495; 
ushort HUzmqiDKLpIFMuI = 14972; 
float pXfeWis = -15.84663F; 
short baZVZajXoR = 16300; 
double UgulsaLjEZCysDXoUqIlwKpJ = 1.907509E+17; 
while(UgulsaLjEZCysDXoUqIlwKpJ == -2.156411E-18){
UgulsaLjEZCysDXoUqIlwKpJ = Math.Ceiling(Math.Sinh(-5));
int? zdqhBOJhHqloljeVlHiSynUaYdEcT = 9551666;
zdqhBOJhHqloljeVlHiSynUaYdEcT += 54246;
}uint CLanRPCjjpjy = 6112; 
short iPVOjCPhA = 1487; 
string iRdxEkChsUGEWQuQ = "KxlISUiHMiZWTPpPaasmNTZegUn"; 
sbyte RuhYlQePSqWU =  76; 
float ClA = -6.914536E-16F; 
ulong PbDhQyhBXGanUPSYHYlaiKwtXbY = 79825407787706357; 
sbyte eZIHqtqUCXmKm =  86; 
sbyte ONfG =  -79; 
byte zFFCygLYkMDDZFtRpezxeLSJ =  146; 
string QizBkUjibNSTNRYcXLohAuWP = "LOLONWXxqVQMlsJPx"; 

}public void ezlLGgJUYoUq(){
ushort JZAgAuoQU = 29028; 
ushort hpMsHsccSqoZiEeSYJCaAEclSDo = 1812; 
string FPoKYsiGJD = "tusKBXVoyR"; 
byte wxFmABecbaENBLzGNJtaZNgiLDq =  148; 
ushort FytTTnueJeEAltRFiPApDO = 29399; 
byte JpwDXiAJowftHRklIaoxABNsftD =  25; 
string QaiXjXZ = "LVdQWXLlDYneVOhOw"; 
sbyte cxFHuGgFfkKHmmeou =  -37; 
long PHURERZmcSwCbwWfgn = 25534684161165560; 
string QzOwMFNwDNdEGTDmPhGSpZhgJ = "OxbTYmPhxEeaPxI"; 
uint ukfGWQslLMjCRqIMnR = 3787; 
long QOzxYiPsijPxOWESRGKImCGG = 60632345221025123; 
float uqgBRixtDaNwDMhwZLK = -484.6979F; 
short SRMMCRD = 23013; 
long iCmLqOBlUYGSkKLgf = 17301523914433090; 
long yVCZh = 70171793413415179; 
byte IqDZHcawMXljKfxexIJdqsYz =  191; 
short CluWKmcwOGo = -29163; 
string BQcWVimNQnbngz = "semoNAjqaD"; 
double LJzhFHIRDWDRjcOwgUwg = 1.584218E-29; 
while(LJzhFHIRDWDRjcOwgUwg == 1.281E-36){
LJzhFHIRDWDRjcOwgUwg = Math.Sqrt(4);
Console.ReadKey();
}ushort IWuaZHJGkDnWmjyZSubHqOhI = 35347; 
ushort xjbLkJshgilYzgLBY = 53035; 
double JkBSJYQyUPNzGdlwiRdmVbqQGxE = -5088424; 
if(JkBSJYQyUPNzGdlwiRdmVbqQGxE == -1.966799E+26){
JkBSJYQyUPNzGdlwiRdmVbqQGxE = Math.Exp(2);
try{
int qgNLaITKlmwp = 2387950;
if(qgNLaITKlmwp == 36345){
qgNLaITKlmwp++;
}else{
qgNLaITKlmwp--;
Console.Write(qgNLaITKlmwp.ToString());
}
}catch(Exception ex){

}
}string PRCcINCUljVEsqVpZTVouikMBKDQ = "JXH"; 
long aZT = 3490980691055141; 
ulong MjEztzLEGGQCXkMTDl = 75721219163376193; 
long yskbOfRGykFiay = 14606180644837235; 
short wUmumgyGPognwPOStfwsHfS = 24308; 
double uEFIntdyGTSxtqNZu = 9.942959E-14; 
if(uEFIntdyGTSxtqNZu == 8.160075E-19){
uEFIntdyGTSxtqNZu = Math.Ceiling(Math.Tan(1));
 bool? agaOkXiVaXMdGILgCQFgVoalKg = new bool?();
agaOkXiVaXMdGILgCQFgVoalKg = true;
}ulong KpFpBGVSIfF = 34513218448086748; 
string ZdzMJyxYVEbhZqWfQ = "LUGqDaVmhWARslJNLlfERhAT"; 
ulong EQNPSYnTijWJBzRxo = 7611547004859610; 
long lXDuNEayeAteCEAcARcLINwSauJqN = 41420046105219770; 
ulong eBbNWWuX = 50428838766795729; 
uint KusUKKNYkXDDFxKHX = 6219; 

}public void AWHyjFZsfFiIDtWwSjRqaXMxwXhpp(){
ulong OLAdNNVLeLuSGPUalBn = 5363459204903037; 
string yJcsYuWco = "IXzMHGf"; 
sbyte BXWQbYSuxACDETAIbsBH =  -26; 
double WsHSoIJuxeCBoCTatgZAEOGedZeG = -3.246874E+07; 
while(WsHSoIJuxeCBoCTatgZAEOGedZeG == -509.0507){
WsHSoIJuxeCBoCTatgZAEOGedZeG = Math.Ceiling(Math.Asin(0.2));
Console.ReadKey();
}ulong yWzZRcGRMTsmjLYoRNuWFMMTWjNWB = 78685506847634403; 
int VJKJMVqbbAtqppS = 7197; 
while(VJKJMVqbbAtqppS == 7197){
VJKJMVqbbAtqppS = 652577;
}short qjtnMHfNWkpzmUdZXFhLx = -24274; 
byte OJjUbNEP =  153; 
double AsIGwWMygaSG = 1.593559E+10; 
if(AsIGwWMygaSG == 0.001145237){
double YjBdylMjHB = 416365.5;
AsIGwWMygaSG = Math.Round(YjBdylMjHB ,1,MidpointRounding.ToEven);
object HpZMPKKejVcJlRefoaM;
HpZMPKKejVcJlRefoaM = 5.144427E-18;
}double ZdDqzOQEBTa = 2.055782E-39; 
double DtcziyOeKxFKWMKhoOHCjhIIiD = -103.7003;
ZdDqzOQEBTa = Math.Round(DtcziyOeKxFKWMKhoOHCjhIIiD);
 bool? hYofOGb = new bool?();
hYofOGb = true;short SGqUK = 12094; 
ushort upGVEtEoGwRXdfkFGCynEt = 56550; 
double gSAReFxtUH = 73479.9; 
if(gSAReFxtUH == -2.28762E-12){
gSAReFxtUH = Math.Ceiling(Math.Tanh(0.1));

}short fsawcdsnTnOIymXxMjxstVEOUT = -2338; 
double FVCaBUbLXimfGkTlBQzBgiBN = 14.88938; 
if(FVCaBUbLXimfGkTlBQzBgiBN == 7.550395E-21){
FVCaBUbLXimfGkTlBQzBgiBN = Math.Ceiling(Math.Atan(-5));
Console.ReadKey();
}sbyte BZTUeFwuAELOAeKggFZQTXzFWKI =  -67; 
short CEGO = -20975; 
int iVRmcVkqljiOoRcKq = 55; 
if(iVRmcVkqljiOoRcKq == 960800){
iVRmcVkqljiOoRcKq += 407375;
}double bybhhMV = -9.617886E+07; 
bybhhMV = Math.Pow(2, 2.1);
Console.WriteLine(bybhhMV.ToString());long lDmSaoetOjnzQEddGbkRtsftCzfQk = 60601741818015109; 
byte ZDqejmpxXjXJUnT =  152; 
float UqCiohSFTeOqxgs = 2.504312E-23F; 
short mWgl = -30493; 
sbyte mnKhByqjcWWQmilFtkJ =  28; 
ulong pqcFDHCOeBbJUEULFqZmexHwb = 13511570834494308; 
ushort teWUKIoRxPFoILIHsGANTLIDJGka = 5363; 
sbyte PzHCHIxoGQGkGyFt =  30; 
string aWuKeJxuldynN = "pPQbibYWpU"; 
double AcedmFWKfD = -4.927535E+20; 
while(AcedmFWKfD == 1.213193E-24){
double CTVpIOqy = -3.006522E-24;
AcedmFWKfD = Math.Round(CTVpIOqy);

}float bXUbMKPZcu = -5.816066E-24F; 
float DmMRgkLtBOswFISSUMyNhPB = -5.725945E+20F; 
int MKLy = 5826; 
if(MKLy == 186441){
MKLy += 567382;
}sbyte JFIygKpfggjBDNdFNMLdChZKQpgRO =  -11; 
long PMOiAsuqyB = 40098499957181538; 
ushort SczCFYHQDzsJyVM = 52886; 

}public void UfusuGblukGwJpCakxNnmFCMFPZhm(){
sbyte zfAHgcfzyxokOawTiQ =  40; 
byte YCLpEJOGGTZnHho =  44; 
long YfYpRXuOLiVQq = 7868603193512174; 
ushort SREpXgTfLAxK = 8014; 
uint KNWQYFLQCwtDIMuqSGPfz = 6428; 
ushort RdkqVUOFQTbwNwNhbbVE = 38704; 
byte phEesujwfAigC =  126; 
uint BFXSSsWekbYymMTcHKU = 33; 
ulong jGzVWEnlIDuuYH = 85922855954750831; 
ulong tGgBSGp = 15415573646218271; 
uint QpVTsUwnhWXSWgOTpCpxNtclfKQT = 15; 
uint XXAfRobMM = 6939; 
long yOcZseVKejxn = 74856425283730267; 
long YHJcaBFLBKuTJVOUOcGCTOcRhDu = 69443152426204517; 
ushort pewftJNUReM = 20915; 
long XRjbkESVzfWC = 72819981845683396; 
uint kwAepXpyIqDOMxzCFSzwHoiSoNNGY = 96968495; 
string ZGOCfnUFazGYGnLNFBBgcsWmIPD = "LzbsBTNQ"; 
long AFERGQETKAbTuezVSHKCUpLKUDxmz = 47535733292492404; 
byte fKOfdiSeWUFbgfifiUTXctp =  103; 
double fQOdCtHUFSgkHCbyBtULuPUBTcJnK = 1.090141E-10; 
while(fQOdCtHUFSgkHCbyBtULuPUBTcJnK == -8.114243E-33){
double skulCqYYmsVGTXBlGqVnjAKsjS = -2.168403E-35;
fQOdCtHUFSgkHCbyBtULuPUBTcJnK = Math.Ceiling(skulCqYYmsVGTXBlGqVnjAKsjS);

}ushort fZcWedNSfXhgnfFMpZfxyHHAZbbJP = 18667; 
ushort idWKPoS = 40449; 
sbyte soPL =  36; 
short GxoFWCztEWccdcR = 2870; 
double jiUWbgAouoqLouJljSfDmDyqlsPEd = -127.706; 
jiUWbgAouoqLouJljSfDmDyqlsPEd = Math.Ceiling(Math.Sinh(-5));
Console.WriteLine(jiUWbgAouoqLouJljSfDmDyqlsPEd.ToString());ulong jxMxyWfMUjuDAZfkzopXlNYn = 68779653187687659; 
long njKRRy = 23006207090873698; 
double YLLPFxyzkjtFGRBVTsM = 27.71633; 
while(YLLPFxyzkjtFGRBVTsM == -1.014592E+27){
YLLPFxyzkjtFGRBVTsM = Math.Ceiling(Math.Cosh(5));
Console.ReadLine();
}ulong koFVWbKLYKhKMPPnycgMZglWO = 9753831774632138; 
string ayVSpsnxbWnTSGAlTpcSedJguJYNf = "GKJQPRDkRuLLjaBJ"; 
uint AHqmX = 8918; 
double OyItCpCpFkcRJKJzJhtTJGMh = 1.66754E-19; 
while(OyItCpCpFkcRJKJzJhtTJGMh == -3.965467E-11){
OyItCpCpFkcRJKJzJhtTJGMh = Math.Truncate(OyItCpCpFkcRJKJzJhtTJGMh);
Console.ReadKey();
}float wqNjKnRFfdxW = -1.53834E+11F; 
int plALfQpTWPiG = 663797833; 
while(plALfQpTWPiG == 663797833){
plALfQpTWPiG += 392278;
}
}public void tFfBOXUCafVWQUAEGRim(){
int TnU = 22; 
if(TnU == 21257){
TnU = 171965;
}int ZjNkMLOknpOaTtMueyxlgcsLR = 66420957; 
if(ZjNkMLOknpOaTtMueyxlgcsLR == 97031){
ZjNkMLOknpOaTtMueyxlgcsLR += 994131;
}double eQKNsTfucqLf = -2.816729E-36; 
double KXRbtfccbipRI = Math.Log(1000, 10);
eQKNsTfucqLf = KXRbtfccbipRI;
ulong wUIGsKiW = 88537690092559636; 
double NOfhwOCQHJp = -1.118159E-21; 
NOfhwOCQHJp = Math.Ceiling(Math.Tan(1));
int KAcNXtwQimHTckUn = 9542; 
if(KAcNXtwQimHTckUn == 550730){
KAcNXtwQimHTckUn += 761070;
}double oClgPwoFN = -1.007478E+07; 
if(oClgPwoFN == 7.390469E+32){
oClgPwoFN = Math.Exp(2);

}uint PtspidsJhUtbiH = 1; 
int aVSPZyXKjiOeYgGJFlFfgAqdK = 174044; 
if(aVSPZyXKjiOeYgGJFlFfgAqdK == 39017){
aVSPZyXKjiOeYgGJFlFfgAqdK = 248642;
}sbyte OUFEZtUszTiUcVlwBcXwAJCFPewnf =  -54; 
sbyte JgkmaeGSLNJGiiIJXxUZpKAetaP =  98; 
double miMmkVLZwwyfLMaWxGy = 1.123664E-13; 
miMmkVLZwwyfLMaWxGy = Math.Ceiling(Math.Tan(1));
try{
int MXYbADjZOexFixloFPEHlnGfwfMl = 8054295;
if(MXYbADjZOexFixloFPEHlnGfwfMl == 92217){
MXYbADjZOexFixloFPEHlnGfwfMl++;
}else{
MXYbADjZOexFixloFPEHlnGfwfMl--;
Console.Write(MXYbADjZOexFixloFPEHlnGfwfMl.ToString());
}
}catch(Exception ex){

}long nMyqTHtRYtD = 74001747804978539; 
float kkuGIaZyFQoQNlWnzBZCOzIqyepGE = -3.460541E+29F; 
double sdCdMkGuttDmNHSFyXwXsjNEc = -89132.96; 
double wiukNouhueeeKzzafnzazhqYaEIWa = -4.586701E+31;
sdCdMkGuttDmNHSFyXwXsjNEc = Math.Ceiling(wiukNouhueeeKzzafnzazhqYaEIWa);
int? OkJjmkfEjPIzYzfltOeypBmSxjiI = 4920271;
OkJjmkfEjPIzYzfltOeypBmSxjiI += 5432;sbyte KHMWjGmYROdbzfcUF =  -126; 
ulong SxDUadTlUiXgefOBi = 54997155984379619; 
string pmzeAmDKneRWfMadJQWXtsCLyNn = "EzAdXiDZwezUCwBxskhPJkolUf"; 
uint LNHMYKfPFOYWjaIEscwXSftmFzjti = 1899; 
float FoJyMVSoLRFlkGJDMURK = 4.680693E-31F; 
int uhAANGqEfoFMZ = 62; 
while(uhAANGqEfoFMZ == 62){
uhAANGqEfoFMZ = 139627;
}short atSBQyusidAKncExsCfBafWb = 28988; 
int mLgVYqGtOHYWhXkXkfb = 6533324; 
while(mLgVYqGtOHYWhXkXkfb == 6533324){
mLgVYqGtOHYWhXkXkfb = mLgVYqGtOHYWhXkXkfb + 130166;
}string OVyREnSgIbiBtIKQPuGXEPi = "VVRCjZWOWDRzqPwJpQEVcG"; 
long tmWQ = 50450494727740840; 
ushort WIeuEQRkzZZ = 61297; 
int lnJplNhXdgmStTNGTMlZUVQ = 2580271; 
if(lnJplNhXdgmStTNGTMlZUVQ == 163090){
lnJplNhXdgmStTNGTMlZUVQ += 783200;
}double FyxNzcGxKiZEYhMxXzTfmLITp = -1.782762E-38; 
if(FyxNzcGxKiZEYhMxXzTfmLITp == -1.98153){
FyxNzcGxKiZEYhMxXzTfmLITp = Math.Pow(5, 2);
try{
Console.WriteLine(FyxNzcGxKiZEYhMxXzTfmLITp.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}byte XmtJeVhbXWyBGYHMa =  148; 
uint CqfXTEfYLOgiW = 88362521; 
long hUczzcbOPHsdcEPjTZHhxPB = 57726291429722133; 
sbyte XSKwWcndidXOUJkqZcPmEsstmnh =  94; 
byte kQmheLeOWXsdmMEb =  72; 
long znCbSsGBqhIUwRJShBNFAlkfRUAfH = 64044652501666211; 
ulong PAkOGocXlETxC = 6640223417721233; 

}
} 
public class eNZIQHesBhDS{public void lGapSixwtIFyOzaLSOci(){
byte DTQapKCFmmbTAPoHDBqWMfukcheHI =  98; 
uint wmdhARSNTOusxUYkwllCS = 2463; 
ushort GYiFUXTMsymMhgLJ = 38610; 
string LbPhjUkxiYSeaHIUTKhZRCGexMzn = "BTmmo"; 
long RQZLKCKOAtmfBmR = 28498281839286196; 
ushort XqIeFSLVjccLx = 19540; 
short SFdAtOSKUQiiCdgeCpMjcJVBC = -20098; 
short OnZPQsDf = 8955; 
double jzY = -1.427649E+21; 
while(jzY == 4.033E-11){
jzY = Math.Sqrt(4);
try{
int QusiBbzEoCMezgSZQjoQ = 199542;
if(QusiBbzEoCMezgSZQjoQ == 83984){
QusiBbzEoCMezgSZQjoQ++;
}else{
QusiBbzEoCMezgSZQjoQ--;
Console.Write(QusiBbzEoCMezgSZQjoQ.ToString());
}
}catch(Exception ex){

}
}ulong cEYnjjGUAGScuLKDLXtmteDXj = 81310334307209796; 
float ViF = 6.403952E+08F; 
int pzbiqSKTtdzAy = 24101176; 
while(pzbiqSKTtdzAy == 24101176){
pzbiqSKTtdzAy += 585957;
}uint fdunAJPugh = 67996734; 
double dVbqNyMaAYoyeUhBmbefjwHV = 1.13072E-24; 
double iYfclRcEfFxYbFVlG = 5.792262E+16;
dVbqNyMaAYoyeUhBmbefjwHV = iYfclRcEfFxYbFVlG * 2;
for( ; ;) {
Console.WriteLine(1.181323E+17);
}byte UCDpARe =  48; 
long DZgFnmqsGlBoWCkNQcxMWbdzfzJpe = 70820909235378743; 
ushort mNqsXJGFbfMbcOXbCVIAPFamhBN = 13696; 
long TCugiZGJfPBUHksyKZ = 40924910620330386; 
ulong kyVu = 17222962427076738; 
ushort QTYVYQUKQOetgxYybBmDyIUfWi = 64186; 
sbyte PwiSEGKYkKWiCIhpEpPljUdL =  93; 
ushort ZTYPRNT = 1562; 
short BERnzQRqtWaMPsTjWHVqyZiyLel = 27035; 
sbyte IHPucNYXLdmtAA =  -88; 
byte GWIQm =  30; 
string CwtDuFVlXouWupf = "ZVRdeAKp"; 
uint HZy = 39122977; 
uint XCsGuqAsgDkOqKpQxRsoXtc = 6272394; 
sbyte XVPqUTiyPOGFF =  -40; 
ulong XzCgppibPckQEzHDoXCxskpUhmCc = 47343348726909798; 
uint dINODSJsUphdoHQpFMiHZdPgkDon = 133925; 
ushort aqUTKFJzlPslV = 49246; 
double fVRteV = 4.040059E+37; 
fVRteV = Math.Ceiling(Math.Cos(2));
try{
int dbKVS = 3686169;
if(dbKVS == 17831){
dbKVS++;
}else{
dbKVS--;
Console.Write(dbKVS.ToString());
}
}catch(Exception ex){

}byte puBPnpuetwldyVzXFTWDYtPDh =  26; 
byte OIPZx =  119; 

}public void VFEGLKXqBGYhwJMtBmLSWG(){
double icoqLGmIZjEtbmlTacoW = -1.282961E+34; 
double GPoHWyIwHGUtpikftsnbSS = Math.IEEERemainder(3, 4);
icoqLGmIZjEtbmlTacoW = GPoHWyIwHGUtpikftsnbSS;
uint xCqiDSuQP = 618296043; 
byte ytyxVyEyEDHBpRc =  153; 
uint gFEgWcoSLz = 842905441; 
uint JDmB = 4789; 
string WOTYLIl = "sjo"; 
int JUpxuhdMeAMeERN = 542858; 
while(JUpxuhdMeAMeERN == 542858){
JUpxuhdMeAMeERN += 988413;
}long nYsHGKgODWEaJWdWqZwRohUg = 19043081842588392; 
double czbBy = -2.396368E+36; 
czbBy = Math.Pow(double.NegativeInfinity, 2);
try{
int tSEYMgZwQqEFeVgDXt = 2895109;
if(tSEYMgZwQqEFeVgDXt == 69074){
tSEYMgZwQqEFeVgDXt++;
}else{
tSEYMgZwQqEFeVgDXt--;
}
}catch(Exception ex){

}uint UbWldgk = 185049; 
long jjjVb = 251400576150979; 
string TFuJEkbOVzSIuMAJRnciVH = "uCjYgyOxfjsmGhBgTixoyiQL"; 
double coqOTQmPZhumUpgbNybzTBphjaA = -4.674214E-12; 
if(coqOTQmPZhumUpgbNybzTBphjaA == 0.01296617){
coqOTQmPZhumUpgbNybzTBphjaA = Math.Ceiling(Math.Cos(2));

}uint fuuVRsFkA = 15536; 
byte cZkaObg =  242; 
long YffQAbiaIHntIXdF = 69690352259693165; 
ushort IzgLsntU = 27228; 
ulong DfiqAsGVKsnXjRZPbdjbzw = 66774421031766998; 
ulong nTIbZtWxzLMSuCXTs = 27864056499501907; 
ulong GwqVKFMJqjYQJgOEOKyY = 36241339754861773; 
uint BljMqagPbmJSHKuKJKy = 47; 
ushort QzKstwbIxhKK = 48612; 
int QOxYIkppotDRZmTphdTtnHINuZFm = 93646553; 
while(QOxYIkppotDRZmTphdTtnHINuZFm == 93646553){
QOxYIkppotDRZmTphdTtnHINuZFm = 712447;
}float OAPhBhuYQFcwDSCHTmKjf = 1.132851E+07F; 
long dGstVNntjDInLTflKWP = 60296798477696844; 
string EfVgDxDbFVROcsoTsiuFX = "MPMxtNYZhXmJHy"; 
ulong ixEjeb = 16068393768033908; 
ushort MhtfwfcPdGOUBtM = 55549; 
sbyte ZYaOqKRD =  -50; 
string yXCBQWzd = "MdtOfzneINFLuQGzxYRJ"; 
byte PaFHIltLynOzzWFmISxNUEyzcg =  236; 
string XIuuUnsjzRcLX = "qXWsKTjqYmC"; 
double EfFBoqqAH = -3.128772E-33; 
while(EfFBoqqAH == 1.770218E-20){
EfFBoqqAH = Math.Pow(double.MinValue, double.MaxValue);
object TpNuHQ;
TpNuHQ = 2.569767E-36;
Console.WriteLine(TpNuHQ.ToString().ToLower());
}int ZoMGIpyxYmbkDdz = 26173620; 
while(ZoMGIpyxYmbkDdz == 26173620){
ZoMGIpyxYmbkDdz = ZoMGIpyxYmbkDdz + 335710;
}uint EKlAXMsHeuigT = 2657; 

}public void KBaGmKRGdwlYTNEgBVhVxRBLH(){
int RtWFCZBkDqUhTTGxDAtOsRbHYcsw = 6066345; 
while(RtWFCZBkDqUhTTGxDAtOsRbHYcsw == 6066345){
RtWFCZBkDqUhTTGxDAtOsRbHYcsw = RtWFCZBkDqUhTTGxDAtOsRbHYcsw + 767307;
}string gQLYsCbQH = "ocRwYkmSFasoHzIesDbUBfS"; 
ushort MPwwQPlakO = 18970; 
int beMOjZURjlqmjAO = 24501051; 
while(beMOjZURjlqmjAO == 24501051){
beMOjZURjlqmjAO += 122355;
}byte SxOkfowLsBUBWi =  187; 
double zyDEehGpyoYEegqQKTswPODOBY = 1.77477E-15; 
double JKSOnhUJZygntDKmouBdKUOcgwpqF = 1.666032E-20;
zyDEehGpyoYEegqQKTswPODOBY = Math.Ceiling(JKSOnhUJZygntDKmouBdKUOcgwpqF);
try{

}catch(Exception ex){

}int STjczjwqcpqDhgT = 20567488; 
while(STjczjwqcpqDhgT == 20567488){
STjczjwqcpqDhgT = STjczjwqcpqDhgT + 85981;
}long zekRoBdOImcuaus = 12540691383916064; 
short KsPwDcugwMXmsgQYDJoeMSpStIJ = -18356; 
byte qYWWGiotfjJnFIu =  117; 
ulong dDyzSiiRbWbUtiWYwZpwC = 72308957971396339; 
long VTZhHZTgOOPgtqk = 28453042944391225; 
sbyte TnVNNKDtzgbuao =  118; 
int DIMBpTsnVgHxcNAQMNMXuadlNU = 18; 
while(DIMBpTsnVgHxcNAQMNMXuadlNU == 18){
DIMBpTsnVgHxcNAQMNMXuadlNU = 665965;
}long FMyWEuEzREWXxtDFn = 2317451035267616; 
ushort iSweYNBiOtQjstVjBpQzQ = 41541; 
float BxecdhWih = 2.47216E-08F; 
int AYjWkOlzsUlEsd = 9274; 
if(AYjWkOlzsUlEsd == 534164){
AYjWkOlzsUlEsd = AYjWkOlzsUlEsd + 22638;
}byte CIyeGxHbHYpOasHCFT =  154; 
ulong DSALeMm = 74588945442684206; 
uint hPRpmm = 5857; 
string pxnJwywZPYuCKuSPfMCfVdczLQ = "SoGlgcTWOEkiPAyNBMHfyzyhCcW"; 
double tPXGCEKjM = -0.06208725; 
tPXGCEKjM = Math.Ceiling(Math.Sinh(-5));
Console.ReadLine();long POmXwyYHjMPPxQFMtIjMW = 83523375667756577; 
int QHPfcPVAGkkEquFzWdMnsoYOZbWjb = 23; 
if(QHPfcPVAGkkEquFzWdMnsoYOZbWjb == 747722){
QHPfcPVAGkkEquFzWdMnsoYOZbWjb = 979837;
}sbyte LCWziFBMlwWiVsKbqHBI =  -125; 
sbyte SMy =  -24; 
double ZEwyRkCQQiK = -3.915946E-36; 
double SmfzAHFBdMMjBulRCL = -2.380076E-29;
ZEwyRkCQQiK = SmfzAHFBdMMjBulRCL * 2;
Console.Write(ZEwyRkCQQiK.ToString());ushort NJIoJlflbwGdWBtFOVAg = 32150; 
ushort ompQGsuoSdRZPoICwWAccUUMbwUDg = 14839; 
byte pJscsw =  125; 
float fOZeBFpfuekZuXwtFPjy = 8.942909E-27F; 
int KKfWGiZxHuXgUHCRFxNgdhVM = 666914421; 
if(KKfWGiZxHuXgUHCRFxNgdhVM == 294484){
KKfWGiZxHuXgUHCRFxNgdhVM = 412728;
}ulong hjwRztBkeOtZZnAYmtTxwAMCOuYX = 82745106186785073; 
byte skVCcoOWMKiPtVDtCJzTc =  20; 

}public void EPMIiwaoQMDbIB(){
float yNRJoOHZkJkypl = -7.650618E+10F; 
short sNpIGCMutcQUUTIlafkwgPhN = -4935; 
uint DYFtVNSi = 14; 
ulong pWUuFi = 31892763013045444; 
float DGDlgqixPJVhcLCekYVzl = -3152.007F; 
int MSNTt = 48; 
if(MSNTt == 615996){
MSNTt += 523701;
}int aYOzGWniLHnaZoCuyh = 97; 
while(aYOzGWniLHnaZoCuyh == 97){
aYOzGWniLHnaZoCuyh = 427760;
}int DxmUeaBIXWLOPH = 9331; 
while(DxmUeaBIXWLOPH == 9331){
DxmUeaBIXWLOPH += 860251;
}ushort XBMzzFyHAqWjuSVRDnBsqGJJs = 27620; 
long tnYKbKzN = 9355316636546794; 
float mybjJjscBsScgcUnnkt = -1.372813E-26F; 
long iJWNFhaioRNim = 60746634189128116; 
float AzaWd = -9.461984E+37F; 
int EdSBMyNUBjDkuT = 9907227; 
while(EdSBMyNUBjDkuT == 9907227){
EdSBMyNUBjDkuT = 898159;
}sbyte nmExnEkXXolpGjpI =  97; 
short eJWsUfGtRYHuLmcPGX = -13340; 
sbyte JElmZdn =  -9; 
ulong LnpeRsUJqJkuYp = 79187877499163333; 
sbyte weKfVbqUbDVfu =  -85; 
byte LoYAYoFVszbLdeM =  31; 
sbyte JNIPNoqpsxuPgTqXZwGJxgAONxa =  29; 
float QPazAzPVCJiIXiCEshugfpByADmU = -2.573091E-14F; 
short AwGGOItShdUXAOjqmieSlK = 22804; 
uint DZXlVTkuyi = 559525; 
double KOCYbwEXLtWHTlyAQDfpkJxH = -6.620182E-24; 
double twdLLQeMGoEpn = Math.Log(1000, 10);
KOCYbwEXLtWHTlyAQDfpkJxH = twdLLQeMGoEpn;
int[] tZyGFbsVMRMaUeMhpBQtOeMaFGuT = { 8015790 , 98583 } ;
Random ZkLGemIpMPtL= new Random();
Console.WriteLine(tZyGFbsVMRMaUeMhpBQtOeMaFGuT[ZkLGemIpMPtL.Next(0,2)]);double MLjShXziEinm = -9.612418E-38; 
MLjShXziEinm = Math.Ceiling(Math.Tan(1));
Console.ReadLine();byte BOicyzx =  181; 
float sUSKYUVLxOStqCa = 2.871578E-19F; 
float ZRgLQUeCOoOOCzSYaBds = -1.620479E+24F; 
string xphyHbGkFcVIkicP = "thwIZqUzEaqiAjhQIoANIUMJDi"; 
uint jNSznAfJGRxZmZdbob = 86; 
ulong DUDclzxNeQSqnQiSUadzkRSBAJ = 27543492737651811; 
byte VlmImSMGhWLnTxgeQoIMccEEHMtE =  190; 
byte WGkXJNwlselsEXEQ =  237; 
ushort PCKzUSIKaBzhTVwOIJAzqudpHRKZG = 3891; 

}public void yjOyZmgpebduLTXsyhiVhyWIwyt(){
float iIeYG = -6.322912E+35F; 
sbyte wCCsuynatxKzlSx =  -92; 
ulong nfb = 24278694913509184; 
sbyte YlaNIuQcAtweXhFWQIQSaLZwT =  36; 
byte zdoEnF =  182; 
byte eONXYOMHy =  239; 
sbyte aDqGmRnmFTahKEweRRsx =  -36; 
long TdUcWmPwLezzCXXaNxZIlO = 10055402726854507; 
long pRzDtOsZPy = 37262654893745295; 
int tbAfzGJDGYPgtjhKAwIYeaSOyBcoz = 925392; 
if(tbAfzGJDGYPgtjhKAwIYeaSOyBcoz == 729507){
tbAfzGJDGYPgtjhKAwIYeaSOyBcoz = tbAfzGJDGYPgtjhKAwIYeaSOyBcoz + 908213;
}short teNfQtQJjLaX = -19320; 
int iHVsiMIXEtddcYh = 29170444; 
if(iHVsiMIXEtddcYh == 126745){
iHVsiMIXEtddcYh = 318023;
}float oHbfnPcqnVyJsRDeGMgjMMpOLb = -1.191255E+17F; 
string BhnOGLgHthCcPiGoQQMPMwjLgD = "numLpnwuouE"; 
byte XZtYJKWghwloZngfYDIduTlgy =  52; 
ulong OpUlGVRMckcxH = 59130708585126234; 
short SVhyeOlCyWD = -8965; 
float ZJdxwdNBlxjNmfaT = 1.653788E-30F; 
uint cpyyNYQKFAeRnzaNmETlEqKLmsCgB = 169280; 
ushort SyJkGKymcVMstzOenGjmmxMHNk = 25748; 
ushort jaqIXxaqCHTxYBxXsUUNb = 15491; 
double jIwMczwpaFTVwKwbNOMYqQnVpJuqB = 2.083871E+11; 
jIwMczwpaFTVwKwbNOMYqQnVpJuqB = Math.Pow(double.MinValue, double.MaxValue);
Console.ReadLine();ushort nPgNSIKuybLeVBSVZgOfzMHAxs = 55103; 
uint RmTZBlIlqlgGDySsgkAGDJGZ = 3991; 
uint IpBQiGH = 83573072; 
sbyte okKJUHAWoOhog =  -40; 
double hITLfKAFa = 2.716697E-14; 
while(hITLfKAFa == 8.468276E+24){
hITLfKAFa = Math.Pow(double.MinValue, double.MaxValue);

}uint hGaIPySxfKdIPCREQuy = 1622; 
long knqCyCbQfiVzd = 37058031607585540; 
ulong mPWzA = 64536159208625762; 
ushort uUdUYzTdNPgHSpfYuwieNCdCbgV = 29398; 
double XoqKAiHgRHuMiJthgWtCQwksiS = 331.4051; 
XoqKAiHgRHuMiJthgWtCQwksiS = Math.Truncate(XoqKAiHgRHuMiJthgWtCQwksiS);
int[] YInRCS = { 1013589 , 58195 } ;
Random fwjCSIJiAwsYSEqHSoNqIjGmaEAMs= new Random();
Console.WriteLine(YInRCS[fwjCSIJiAwsYSEqHSoNqIjGmaEAMs.Next(0,2)]);long bTJ = 21864416826014161; 
long hAIawCOCpW = 25646869676526717; 
float XRLItsFpbpbKCLwZesKcUA = 2.712819E+32F; 

}
} public class qZxnsCNeasqPZIdWyAZVgwQSjT{public void LPRyfPQcticKPnF(){
uint ejeKHgxaLbMWi = 78090; 
uint cpjU = 73; 
sbyte KOuQkHCUyOZOOdIuywN =  -109; 
sbyte FYjMkopz =  93; 
byte eWtbjBujbMQeLoZIPyQmfABiwGz =  103; 
byte YUPSJhotmUsUdzPUzy =  60; 
long bcXQGxkGFbZXzOVoT = 52745785054717496; 
ushort tCh = 2038; 
sbyte cWSSbdMeNJTtWGKUFGR =  -25; 
short RgcFj = -7375; 
float xZRtx = 2.602978E-34F; 
ushort DfoKCcCoQqAWCkOpXfLQxTqmI = 14915; 
float EjTRhOopZwodCpucq = -3.557179E+30F; 
ulong czMs = 33440461252697634; 
double GzJPOVypCQIngAoWBqdzBL = -0.02352997; 
while(GzJPOVypCQIngAoWBqdzBL == -3.027109E+08){
GzJPOVypCQIngAoWBqdzBL = Math.Ceiling(Math.Cos(2));
try{

}catch(Exception ex){

}
}string csTlzdLFKuPfWILzORtOWIy = "iwhqTT"; 
uint QaKlRAQdAF = 39; 
double SAuXo = -14.22408; 
if(SAuXo == 1.935954E-08){
SAuXo = Math.Pow(2, 2.1);
int[] HnaoosycgeDIXVEwO = { 2264665 , 94198 } ;
Random inpexNhfTsyAOWCVpbb= new Random();
Console.WriteLine(HnaoosycgeDIXVEwO[inpexNhfTsyAOWCVpbb.Next(0,2)]);
}uint SHYBnxQmCNMNKiDSngizeFIpHgcF = 367731; 
uint KqzyymTJcfWMbjRMNiOe = 986504; 
float ZITgulLhk = -7.865795E-35F; 
short euanBwZJVnyppRtFyTKAk = -1894; 
ulong NnRuOBXgAcJON = 55679579741890424; 
float uqEOk = -1.312144E+27F; 
uint GIBlEFUuqpRniFRHHeI = 39708507; 
ushort VjyckNMNHEbifpaamLJcgaRkNMhF = 18069; 
byte ljLGcgCJIFDkKhwLpGduWLsQV =  92; 
byte AFgAwMQYwLY =  225; 
long byEQ = 1943279161648624; 
double SUKKUliczdPYsypG = 1.934608E-24; 
SUKKUliczdPYsypG = Math.Pow(double.NegativeInfinity, 2);
try{
Console.WriteLine(SUKKUliczdPYsypG.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}uint oyOzjTwQnmhJKLPZJFVGYQXa = 5; 
float tohWNABOuFfVjDWX = 7.091187E-35F; 
byte pdOB =  217; 
double zAOthZWyYflVjjbGezaxwTagc = 5.012143E+25; 
if(zAOthZWyYflVjjbGezaxwTagc == -51.83994){
double bnGkYWmfYSIgbSnzJcSAx = 3.028847E-33;
zAOthZWyYflVjjbGezaxwTagc = bnGkYWmfYSIgbSnzJcSAx * 2;
int? qhNhyAhdCiNZU = 7881000;
qhNhyAhdCiNZU += 98534;
}float PAYBsAdAKCzPDOpgLqgx = 8.230107E-28F; 

}public void pFVVMpNYaaToKquQaSoptsdayb(){
string zSyfHHIVyFqjWEzPKAHQpC = "FlSAXC"; 
float LlbAhENJcJzswBf = -7.759312E+22F; 
float qYwnwXLSTO = -1.588154E+15F; 
ushort gHXVdpkWQEhRiLdfA = 44735; 
string EzoYHDyLU = "VYeQoUizNFduiRgEPaONozHhZIq"; 
double BTBcVJiNsx = -6.401614E+33; 
if(BTBcVJiNsx == 0.006914212){
double ScOxWgGxqXRIDjjl = -5.452311E-25;
BTBcVJiNsx = Math.Ceiling(ScOxWgGxqXRIDjjl);
try{

}catch(Exception ex){

}
}float psViYbpKcXAp = -1.484156E-08F; 
double ebVQXdyQxtXoDlauEkHXqKRAzXK = 7.718201E-23; 
while(ebVQXdyQxtXoDlauEkHXqKRAzXK == 9.66169E-05){
double ycwlUfMFd = -1.947269E-07;
ebVQXdyQxtXoDlauEkHXqKRAzXK = ycwlUfMFd * 2;
try{
int fNPOaFwjwsZtFGZOAjgwRWBQuckX = 4081723;
if(fNPOaFwjwsZtFGZOAjgwRWBQuckX == 99271){
fNPOaFwjwsZtFGZOAjgwRWBQuckX++;
}else{
fNPOaFwjwsZtFGZOAjgwRWBQuckX--;
}
}catch(Exception ex){

}
}string wSpBNQFRawfilYtYtyWZVj = "IBpYPfpaSUQwCMLk"; 
ushort FPyRVySqfyXccPksIZxSPMLum = 18411; 
ushort RlImAhpwcRFSfzaScP = 55016; 
string jdLPFcTd = "EHbo"; 
uint QLDVAQoRwEMpLSqzywtHhWzVfx = 9905; 
byte RRYgcslWCuUmpkSMhlMejHWTLTmI =  205; 
string pDzNVsEPZlBUYMV = "mctoeniNkVexjeEgzVF"; 
long zXtFyMSKMXgQmZGxOubuLCanh = 55446054338624455; 
ulong fZLIDpdzYXGXeYKLCDphWWfx = 6598871794000679; 
uint BxdMAdId = 31; 
uint QKKKDFUHSiUnQLfNheYa = 884251; 
uint IPcmATUPU = 2; 
uint IquMRssDPVOXUWXqDiW = 4272; 
string CHgYneuHLGgAOFnICgLTUoo = "XWysaeojGESeKudyuod"; 
short xjNnBufLiTmsCbqHcGGpWUj = 15474; 
sbyte hcmzGkWTCNLdAKjzxLFNisMQsJybQ =  5; 
double eDm = -0.003459939; 
eDm = Math.Ceiling(Math.Asin(0.2));
int? XxgaLxDsBShTuDCVscBTJOzNbGqoB = 6967296;
XxgaLxDsBShTuDCVscBTJOzNbGqoB += 95227;byte qjYtZffmSVtoHwtcUVoynFAC =  147; 
uint XdBVCGyIZBHzIXsltEUWpaiePI = 53424556; 
ulong yPBlhjqaRJjOuCnyIBWsg = 77015765255063164; 
float VUodRXxPiUsNkNDGIRsanGuEA = -1.055428E+36F; 
long oWUXRnTf = 81515349418041879; 
byte PbqlFOTDpALFMZOF =  222; 
double PtzJBjuSQERXQnG = -1.112963E+10; 
double tuTLByfUzpxfqsRckVcZzDUpfQgMR = -3.740288E-32;
PtzJBjuSQERXQnG = Math.Floor(tuTLByfUzpxfqsRckVcZzDUpfQgMR);
 bool? BofpJss = new bool?();
BofpJss = true;long tdRYLaRcLMC = 9213689874960838; 
double LQBgJQZzGNdJbeTICgBblqnGI = -1.011546E+20; 
double JmpRcjRFeYzVJxIw = Math.Log(1000, 10);
LQBgJQZzGNdJbeTICgBblqnGI = JmpRcjRFeYzVJxIw;
short YeZbdimPLYhlO = 16464; 

}public void LJsoUhfMhVWsuiuOIYMOF(){
short ndBffTgAHFa = -4302; 
sbyte oxmTsSghRTHpkAmb =  123; 
sbyte seltbaTawoNt =  57; 
byte tDAUlBjgm =  165; 
uint JIlGGVdPxdckTARLSdAUdXfL = 95178; 
float hXZMDkktjKhyIyRpAnbNJAoihF = 1.635899E+16F; 
uint dBckHzeqJJozBhVb = 6040; 
sbyte kndgjJJQEXAASJHHwyRskiGty =  -78; 
double mfsgFL = -1.509055E+22; 
while(mfsgFL == 3.898737E-38){
double LiPHSCnHPOOFcILJZKDDzGs = 3.496614E+22;
mfsgFL = Math.Floor(LiPHSCnHPOOFcILJZKDDzGs);
try{

}catch(Exception ex){

}
}long NCaxDYEYcadagLzmxFp = 9072734802413501; 
float kxGyHtjlFLnh = -3.794724E-35F; 
ulong zsZGUVpUyAXinsUdJHREzEAqn = 8046066913529979; 
double pVoyogLahxbXmRkzJll = -2.190747E+31; 
pVoyogLahxbXmRkzJll = Math.Ceiling(Math.Cosh(5));
Console.WriteLine(pVoyogLahxbXmRkzJll.ToString());ulong RRbEgSxhgL = 83094139275042260; 
short FnpPRNRMhzANE = -1588; 
string QdKGJH = "POyDkVosIBsKEBmNuaKa"; 
long iRCJpScD = 67854451687754486; 
int TfRlkOAGEqpGYHYFFGaDFxpfP = 72914035; 
if(TfRlkOAGEqpGYHYFFGaDFxpfP == 222189){
TfRlkOAGEqpGYHYFFGaDFxpfP = TfRlkOAGEqpGYHYFFGaDFxpfP + 362921;
}byte ShDtWHNiLoLmf =  190; 
short XxOjdmxQMyDcDGsYfkfz = -4819; 
byte ZntUhFTKUuocPFapGLMI =  125; 
string GeXNgUHTkMOPWKsolTNJJ = "EDtfUANKEi"; 
double OQjRMPnBdpbQdGnxBP = 1.417353E-08; 
if(OQjRMPnBdpbQdGnxBP == -2.453907){
OQjRMPnBdpbQdGnxBP = Math.Pow(double.MinValue, double.MaxValue);
try{
int eugYwhjnCEbnlpjPxC = 7727781;
if(eugYwhjnCEbnlpjPxC == 99851){
eugYwhjnCEbnlpjPxC++;
}else{
eugYwhjnCEbnlpjPxC--;
Console.Write(eugYwhjnCEbnlpjPxC.ToString());
}
}catch(Exception ex){

}
}float mTQaMWP = -1.485952E-10F; 
sbyte SBmnWxioNIff =  30; 
short AYdgICHncXIKcjGkOl = 7104; 
float AdjlFLh = 6.518904E-30F; 
ulong KhxRnmC = 14376038348211407; 
sbyte xqUayw =  -126; 
float VdQANVjzZVNPWCLPJWoNVAzGaeQ = 7.48581E-26F; 
byte xWtqzZLBngQMkssAPikPzL =  37; 
double FkzJfqoNiqSEkm = 6.16388E+33; 
while(FkzJfqoNiqSEkm == -2.545257E+08){
double JFaKXzmLPuV = -3.766509E+18;
FkzJfqoNiqSEkm = Math.Round(JFaKXzmLPuV ,1,MidpointRounding.ToEven);
Console.WriteLine(FkzJfqoNiqSEkm.ToString());
}sbyte LftxOoE =  -21; 
sbyte QMpWeJGxKUQIMGQ =  43; 
string hmRQIwcgUOUgjfAMkBiozjRq = "mscuOGekNnUIa"; 

}public void VUXP(){
float DKLiZupAOqXwAyudZ = 7.976436E-31F; 
sbyte VyRGekynwCwZQREHUutpFxxq =  -9; 
sbyte JRUjJKMHmUDVjTktPD =  -20; 
long RxetMSjzaBhWIOxCqMo = 59292535401006157; 
ushort HOcz = 61363; 
float PRXRChltwYDePS = -3.012384E+24F; 
sbyte xCJNTDwRwuNeyLWbMBO =  -11; 
uint duW = 633845; 
short liWVSBpMcYuxLNSHpCR = -21690; 
ulong XcJewy = 50430018310372746; 
ulong FkGoKDt = 10630844670543134; 
sbyte fkTmNeSxlGAsAEGQgWHDzxwKH =  -54; 
sbyte CjtgUudbX =  107; 
float NSyXUJ = -1.723152E+15F; 
int AciGtJPjHzXHVbltzfAFSkeBQGYz = 95273844; 
if(AciGtJPjHzXHVbltzfAFSkeBQGYz == 625247){
AciGtJPjHzXHVbltzfAFSkeBQGYz = 570402;
}long XnaulNptigX = 27635682974392151; 
byte IQOKqwJACaqpVwjeetfVMsJJDNmM =  199; 
byte zyspymLUsHUHgGgPgbf =  27; 
int tlp = 11; 
while(tlp == 11){
tlp = 933978;
}long uTTEeMSuANn = 48818609252980620; 
long Zzegap = 78388591447438359; 
ulong lbpmLhmeu = 39679721234061516; 
byte wPyewLSiEFIuQ =  82; 
sbyte euwwSkZMwMeQKhzihqqzPX =  101; 
int YKBMkOMFUYRGPDjPLJJGczwfAm = 995028; 
if(YKBMkOMFUYRGPDjPLJJGczwfAm == 140082){
YKBMkOMFUYRGPDjPLJJGczwfAm = YKBMkOMFUYRGPDjPLJJGczwfAm + 497018;
}float TCAZkYtkQiHEsPhKd = -1.588657E-21F; 
double AnumXoMVWRqMlJIoLyLmRImz = 2.707676E+29; 
double NtnfNnKIpTsoRgfAusQCKYRbVpdF = 5.841691E+28;
AnumXoMVWRqMlJIoLyLmRImz = Math.Round(NtnfNnKIpTsoRgfAusQCKYRbVpdF ,1,MidpointRounding.ToEven);
ushort BdydgMlpaBpDUqCLNha = 35221; 
string SfYAXcgOnwfyljqzfykTenfmBVB = "dWjSGFlqeIPijmlVyxestncDqgSE"; 
string YbCjLsknSIXauyIbKRbkQe = "iEPUHCzMIptKNHZDUKiEywsU"; 
int oyaDQVTltHWzENuVoKaNUwKohBH = 368485; 
if(oyaDQVTltHWzENuVoKaNUwKohBH == 885399){
oyaDQVTltHWzENuVoKaNUwKohBH += 273172;
}ulong aMfBzQCmt = 63323257685094606; 
float aUOMuontSHxbjSBujiiDXthqJ = 1.412192E-08F; 
byte RHDZxMqlZMl =  47; 
ushort HWFlnVQsXAsVHNydIUecb = 65110; 

}public void bOgjhjyXsAKBudBgoBAKdIBeEP(){
double qfuI = 1.044949E-34; 
while(qfuI == -6.481059E-29){
qfuI = Math.Floor(4.003051E+12);
try{
Console.WriteLine(qfuI.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}short zeOmLlgoMhUnHqybXmXDdtS = 24664; 
ushort wdGVQnZZzRQcgTkYx = 3970; 
byte JFJUPcXPJfmUQjRdJQsHoITGl =  225; 
sbyte sfWP =  36; 
int RuMfNpjpsFVmXcgoGLGDbMdIV = 889045012; 
while(RuMfNpjpsFVmXcgoGLGDbMdIV == 889045012){
RuMfNpjpsFVmXcgoGLGDbMdIV = 469702;
}byte qaKWLFpe =  87; 
uint pHi = 22; 
ulong dbcQepdjRxN = 38353220360344354; 
short goyKkcsnOUlNSwkiBVtTBOUsFqV = 8608; 
int XOyxXNFgUnLWdCBZq = 46696778; 
if(XOyxXNFgUnLWdCBZq == 156671){
XOyxXNFgUnLWdCBZq += 87518;
}uint swfeTgMKkWzow = 462552; 
int aehFLsRGZjiMx = 67805231; 
if(aehFLsRGZjiMx == 593356){
aehFLsRGZjiMx += 339476;
}uint agZl = 598254843; 
string WmaoMhZqQIcnAUWDXOhPp = "TgfqZXhsSNPVd"; 
double yLIKHlX = -3.543578E-22; 
while(yLIKHlX == -1.949789E-23){
yLIKHlX = Math.Pow(2, 2.1);
object BeKhVR;
BeKhVR = 2.974548E-08;
}long zcoHooywaZYdpy = 1173146807108247; 
float ODDKnP = -6.440513E-14F; 
float dlYKk = -4.867178E+37F; 
ushort aOWaNVD = 16562; 
double zbLkhSMMOgkHHmkuUncQel = -1.032555E+09; 
double CxGsdsaQRzHlmWugKPtbojg = Math.Log(1000, 10);
zbLkhSMMOgkHHmkuUncQel = CxGsdsaQRzHlmWugKPtbojg;
sbyte nGbR =  -56; 
long SmjfPJqh = 2920235403874674; 
int xAnXqVbPdNnwucPPWxmftefFF = 4848; 
if(xAnXqVbPdNnwucPPWxmftefFF == 844047){
xAnXqVbPdNnwucPPWxmftefFF = xAnXqVbPdNnwucPPWxmftefFF + 858074;
}long ZSkXUWndaGEUxFN = 89666193825954550; 
ushort sYxjVkUGXukFRbBZbsAC = 46526; 
sbyte MzxDTWaBBQoDXRUfuJnzjDOG =  -63; 
uint wfSSXjiLLzJbKXUKOCADih = 1092; 
float LGjLRoukGuIXqs = 1.415887E-29F; 
sbyte LIFCoSadOWEmTNaBzECa =  33; 
ushort AsAbQjgObZGujSjpqKOC = 29991; 
int CVXRTkGhaqaf = 79; 
if(CVXRTkGhaqaf == 869889){
CVXRTkGhaqaf += 625109;
}sbyte nmsQJomkpTSiSZYgT =  -32; 
byte xSGdiMaBBBUVoeenE =  34; 
sbyte XwRgARmiSKqAtGfxIo =  34; 

}
} public class oNLQYXUNVKDMlJFbazEqzRJQiwCJ{public void JHoAlokLbiCXBnihGnKadwiLoyzeU(){
long wkNmcnlauuqYpcuYEA = 32605803676964955; 
int ORgQuFgN = 1440258; 
while(ORgQuFgN == 1440258){
ORgQuFgN = ORgQuFgN + 267064;
}short NyTJVzVFldyEDRaFKblE = -15852; 
uint MVHkDuPjTDbjPFVooV = 564883; 
short DkWaCi = -10396; 
string tkTgNnmsjyTFbBIkZPNWIL = "mFtxoCTGSRQIYWzhabEsfyECKnQC"; 
ushort eeInGDuRYQqRaNlTKnC = 41958; 
ulong etSPbRNbMsmmMLZLpVOidNaoxQp = 89453411043972869; 
short QCDeXeZFNbBTZX = 7966; 
float RqtEtkCWxksBuDKLo = 2.966346E-12F; 
byte gJaKK =  201; 
ulong GEtTpsJVSypahBTIdtzQhKylHyLJs = 3274369514318917; 
long jaQxc = 39160449526222508; 
short RVfglSHaEZOTmjKmJxnoPqnwhwBeU = 30585; 
byte fOhVCSwlDaly =  68; 
byte NWDLibbiAIKZQsDgGMwNAcOLRbcMP =  26; 
ulong WyNiqm = 74418506472911803; 
int aTHLSyzLiRUhZoutRDdtFtPsJe = 88880787; 
while(aTHLSyzLiRUhZoutRDdtFtPsJe == 88880787){
aTHLSyzLiRUhZoutRDdtFtPsJe = 974819;
}ushort LVzfozY = 2383; 
double sVqPkoyoPloHdocN = 5020890; 
while(sVqPkoyoPloHdocN == 3.555935E+33){
double zVMmFwDuoaNOTJecGjk = 1.2198E+17;
sVqPkoyoPloHdocN = zVMmFwDuoaNOTJecGjk * 2;
try{
int cCJuLqExj = 3389729;
if(cCJuLqExj == 72887){
cCJuLqExj++;
}else{
cCJuLqExj--;
}
}catch(Exception ex){

}
}uint AHLJLWbScMEDnHVLqEaugRcVNDMz = 705898; 
byte KDMgJcpnPTjcLNMpZCoOM =  30; 
float EQVijDusBTSLVPcnnNFm = 1.209017E+37F; 
byte QxzWwzYEMdDShgdmARDRVaukzaAYC =  143; 
string FVsNeyWcVoZRljggJs = "QxueQbmzwYacidYnlPphbVB"; 
double bEYnUtpiPkhkQDMRZCd = 4.322309E-20; 
double kNUWzAOzojBFK = 5.405095E+29;
bEYnUtpiPkhkQDMRZCd = Math.Ceiling(kNUWzAOzojBFK);
try{
int dwlFpbtxbtkQLsabXYcpkMqOn = 5678231;
if(dwlFpbtxbtkQLsabXYcpkMqOn == 66328){
dwlFpbtxbtkQLsabXYcpkMqOn++;
}else{
dwlFpbtxbtkQLsabXYcpkMqOn--;
}
}catch(Exception ex){

}int BENUqCGqeyqOlsXRZOTnACVie = 7893; 
if(BENUqCGqeyqOlsXRZOTnACVie == 219359){
BENUqCGqeyqOlsXRZOTnACVie += 209226;
}int YCVyAfuRCuIyfAiDGgQPpsYIEPaNU = 85; 
while(YCVyAfuRCuIyfAiDGgQPpsYIEPaNU == 85){
YCVyAfuRCuIyfAiDGgQPpsYIEPaNU = YCVyAfuRCuIyfAiDGgQPpsYIEPaNU + 245488;
}float tBJA = -1.227957E+34F; 
sbyte MYRGiKewQI =  32; 
int TKeIXBCwM = 2512; 
while(TKeIXBCwM == 2512){
TKeIXBCwM = 627039;
}sbyte UuQuSRjgLs =  68; 
float hGnQAQESzyHSZJEnJxznCNmNSbT = 13.20501F; 
byte NERhHdK =  191; 
ulong ywWjDShGTbVpEuutCX = 33343538080675909; 

}public void CXjYKIK(){
string pDoiLpIkmyZx = "nDNnTWfjIpnJHJKclITpddY"; 
long lsKacxoMOKH = 86903851658007090; 
string eIYhaPggKjEeZIEx = "FTZXSZcR"; 
float fsVwLHQgLLScIhGeAdhxhjCXdu = -14981.65F; 
byte WqjVaPOOTyiBTLqDlRqO =  124; 
double wMVQYqqFmUbPBSMhQpLkK = -8.795615E+09; 
if(wMVQYqqFmUbPBSMhQpLkK == 3.50862E+08){
double ziXIPjfZQy = 1.537759E-27;
wMVQYqqFmUbPBSMhQpLkK = Math.Round(ziXIPjfZQy);
try{
int RVRKAKCyOOYheFZsQitb = 7882888;
if(RVRKAKCyOOYheFZsQitb == 84020){
RVRKAKCyOOYheFZsQitb++;
}else{
RVRKAKCyOOYheFZsQitb--;
Console.Write(RVRKAKCyOOYheFZsQitb.ToString());
}
}catch(Exception ex){

}
}long KbwiKHeCayHsAUkYd = 19404660852323745; 
sbyte eedEYARsuyJixzBQdIpqwPBSR =  -73; 
ulong YdVdCPZxWTJTIDRCuPjyhUUSPQ = 16933118676758509; 
ushort DGIIAdUjYyK = 56881; 
double ybboMejcoMsJSLgVgKiBYPhCYeaLU = -1.579542E-29; 
while(ybboMejcoMsJSLgVgKiBYPhCYeaLU == -9544233){
double logBpVoQkiccpZKx = Math.IEEERemainder(3, 4);
ybboMejcoMsJSLgVgKiBYPhCYeaLU = logBpVoQkiccpZKx;
try{
int qHeAiXFPpmWwjKzUu = 1202253;
if(qHeAiXFPpmWwjKzUu == 59725){
qHeAiXFPpmWwjKzUu++;
}else{
qHeAiXFPpmWwjKzUu--;
Console.Write(qHeAiXFPpmWwjKzUu.ToString());
}
}catch(Exception ex){

}
}byte qtpAAYlHTGCQcDjImiOm =  209; 
short yWuJQEYETKNgeVOJVFbXxGdUaWj = 31903; 
double XRZzTDeowcUcJfoxeMAASIPuA = 2.668289E+28; 
while(XRZzTDeowcUcJfoxeMAASIPuA == 9.974776E+28){
XRZzTDeowcUcJfoxeMAASIPuA = Math.Ceiling(Math.Atan(-5));
for( ; ;) {
Console.WriteLine(-311977.8);
}
}long VlzQqfTWWd = 13898359953580739; 
int BZZ = 30410314; 
while(BZZ == 30410314){
BZZ = BZZ + 568544;
}byte tQlDNQknTh =  192; 
long AOxoFWANRHOWlRBiiD = 32696256900143724; 
int nNUOqxhCMINIMDDizHCZL = 71; 
if(nNUOqxhCMINIMDDizHCZL == 953692){
nNUOqxhCMINIMDDizHCZL = 784002;
}float jKooQAdNaGPKYxBQLZlfV = 3.733597E+35F; 
ulong YhcottIpSDYysVczxzaM = 53002384926101683; 
ulong wMCISsbzHlhtgUDttejHApX = 39975994397538156; 
sbyte ehlVLdLyXROFWGQJwp =  97; 
ushort neOBo = 39086; 
float XXRcPbNp = -6.915559E+10F; 
string jCAiyxIIietCMzR = "yuwz"; 
ulong ZTXDgEcsX = 50036720929645691; 
float lbSFpppkKSdmDJWDGlVodFQhLIRE = -0.0003914464F; 
float DBElhoHlQtt = -7.715739E+25F; 
ushort kZjZtiETIkK = 56940; 
long NflZNLbfLwpGJAL = 4063078591647238; 
uint iYIWayxYNdnhBgLn = 28; 
ulong OAeiwUwEpSijOBFMuiKXU = 39132825972001947; 
string pgOMJTzBgbSIVUet = "DYTXFRPgEJNqdlZlsTdZPVTWIDWbZ"; 
string sLRHGjDLpZGejMjdQ = "wDbatkHfXobVOhDhuGlGtbQhOa"; 

}public void QJkzAfh(){
int BqsHZBmmJulyDcs = 831964; 
while(BqsHZBmmJulyDcs == 831964){
BqsHZBmmJulyDcs = 748245;
}float sbRxNRDPG = 3.987878E-33F; 
int nGGT = 54460656; 
while(nGGT == 54460656){
nGGT = 727076;
}byte TVIVJjMIdZGidBxfGGT =  117; 
ushort olFHFktUEo = 36597; 
ulong WVTXutZYoaxQoftNiSQkEGxpbF = 43217313697631; 
uint MJzexpak = 81; 
short aguyN = 5844; 
short zDVQSLPTKQC = 10764; 
int bAtLOmG = 1449; 
while(bAtLOmG == 1449){
bAtLOmG += 261176;
}string TTJ = "jNjOJmMCgCt"; 
short zTfVdcQcLmwKCwyMSQXS = -9126; 
ushort RMLycMzKPWHXu = 62259; 
sbyte QTmzAOdgSnQnLwxocznitGNq =  -67; 
int OUzcOMagZfXVdIEGPVj = 367213970; 
if(OUzcOMagZfXVdIEGPVj == 894914){
OUzcOMagZfXVdIEGPVj = 939649;
}long xkfVlVhplulbCKoxifDkBPZYxcGm = 30434493437566938; 
float LaVsqlDT = 4.581469E-18F; 
string iPuz = "HkgAjpUcKQfXNf"; 
short YwDoizOwDtqijtPLH = -9051; 
double NMBKboIwyWWiMBktSimiy = 5.679707E+32; 
NMBKboIwyWWiMBktSimiy = Math.Pow(double.MinValue, double.MaxValue);
Console.ReadLine();ushort jXTEMxioAqVckiNBNnGJVCnTWVSe = 52752; 
short IUmYJIoDYlBYUkV = -15870; 
short wmHDMcIJZ = 8544; 
ulong ezjTmiYjdTzLHxwEtI = 37328419992946358; 
sbyte pZIIMuhb =  67; 
string NoghDPVVoIepqGidlehbhaOTekQZm = "tPsKmsuKRm"; 
float SDlKpcE = -2.301934E+32F; 
double ZQNCmdsYWJbnoXIsQJC = -4.863396E+30; 
if(ZQNCmdsYWJbnoXIsQJC == 1.5069E-27){
ZQNCmdsYWJbnoXIsQJC = Math.Ceiling(Math.Cos(2));
for( ; ;) {
Console.WriteLine(4.477483E+26);
}
}uint adyOiyRhVSOeAVdLuowUcmgTzMeH = 72; 
double aYOEKKhzTAolTjz = 2.075951E-08; 
aYOEKKhzTAolTjz = Math.Truncate(aYOEKKhzTAolTjz);
Console.WriteLine(aYOEKKhzTAolTjz.ToString());string VLygTWpxPthSEujpCq = "hSjNMkGzuCaIHbNJu"; 
uint aSHbumjXykRTFJnWML = 701986113; 
double yVVPLCQRMP = 1.275258E+34; 
if(yVVPLCQRMP == 11.2656){
yVVPLCQRMP = Math.Ceiling(Math.Sin(2));
int? HAbdHqFw = 5221121;
HAbdHqFw += 59884;
}ulong OJgcYIZIjsTmRBwRKJcs = 24389101975308497; 
int SIDcucJNusoWey = 976053633; 
while(SIDcucJNusoWey == 976053633){
SIDcucJNusoWey = 719068;
}
}public void SDyUz(){
string tztxfOBlKOgthWnLdRkitLsVSRsZ = "fhVZN"; 
ushort YSQqWFotmsEhFRudnUDIgzFnxCg = 38392; 
int twDsMkaT = 7868; 
while(twDsMkaT == 7868){
twDsMkaT = twDsMkaT + 505709;
}byte unMeHfjM =  201; 
uint DVuzKObYhG = 1102; 
sbyte wHOl =  -90; 
ulong alhST = 13837957826830338; 
uint SpMkHCgFhiDclzxAHKjDKmHRmyLW = 68378019; 
float EyPyBWiDU = 2.407595E+37F; 
long zsPPpVCNWnG = 69190895511488006; 
byte APKZZLBHJcm =  237; 
uint fVgAOtzIiiNHXZMKSCwZZDfQ = 2125; 
string mlsOtj = "UdpKV"; 
float HBltNCVZjMBCPDQphDssuWRZm = 2.70228E+30F; 
float CRBhhWFFYzzlnbHmSuC = -2.466861E+20F; 
sbyte EAyKKmf =  122; 
int tJUa = 6445; 
if(tJUa == 719487){
tJUa += 192651;
}string gfUkMwttNStKXJLmVsSAShVtMm = "NWScYdKtqPzBKGn"; 
short okcLWtptqGbYkCzAW = 7822; 
ushort QwHmptzxRhTERueCjOQDDJHyIug = 17248; 
string OZR = "MUFQwDVUCiMyhQ"; 
ushort oiQTsgOiGVmkwQuTMwJkWutmIIgy = 47315; 
byte pRdMGjXdF =  26; 
ushort ApKzMPSM = 47745; 
sbyte aVPDlKZqy =  -26; 
long tsePKOsdfCjUXXjwJTzBiGdQQIMt = 38667538663644291; 
int iXuegAMVGnBEIEWhOOonZccYzqDud = 633177; 
while(iXuegAMVGnBEIEWhOOonZccYzqDud == 633177){
iXuegAMVGnBEIEWhOOonZccYzqDud = iXuegAMVGnBEIEWhOOonZccYzqDud + 703860;
}uint dAEeVb = 875419; 
string uTIHDQ = "ZfVxHPWqRcHx"; 
ulong TubKoLjeaEexkhBxWdnxKFHLyKHNd = 37235585611130378; 
double fhVfqqjEPR = -1.891159E-29; 
while(fhVfqqjEPR == -4.888601E-12){
double lGdyabMHjdHcQ = -2.359245E-17;
fhVfqqjEPR = lGdyabMHjdHcQ / 3;
Console.Write(fhVfqqjEPR.ToString());
}uint PGhNadB = 47726441; 
double AlCqjysplusBnhgETxLInGKgdKDo = 2.831642E-13; 
while(AlCqjysplusBnhgETxLInGKgdKDo == -1.575339E-14){
double EUhsKnOye = 2.147395E-25;
AlCqjysplusBnhgETxLInGKgdKDo = EUhsKnOye / 3;
try{
int ItOpKTDunAVdAUXfj = 690669;
if(ItOpKTDunAVdAUXfj == 44757){
ItOpKTDunAVdAUXfj++;
}else{
ItOpKTDunAVdAUXfj--;
}
}catch(Exception ex){

}
}string pmQow = "ojuRqVuHC"; 
sbyte SkaeDHKpJYIk =  -78; 

}public void JixIeEZgpawXetdwpUhqllaZOH(){
sbyte MnHZjXIMYYxFtLVZKGDqmmJNcTIyi =  -18; 
float yZwctwNgLqyqzEY = 3.67643E-15F; 
string ieONzGUWqpk = "CeXgtpSHnSwHVQsoA"; 
float figMuK = -1.325269E+33F; 
ushort zxQAOWmByGkztbpJ = 9101; 
float nFyxiftwsExftwgQxGhg = 6.063042E-31F; 
ulong oWHP = 43952398482207839; 
string HuwBUXaARdmkTdZMOhQwhmzFoVRUL = "llPyY"; 
ulong tkRbPeEDUDJDJAbfGStVQl = 3770031679466695; 
ushort lZwXY = 24716; 
short Ubwsiqc = 6831; 
byte iFFSVh =  55; 
ulong bsMCyfEqKddglBkaPQHWQMgHUnS = 38481959856105184; 
float AXmFZtC = -6.216302E+37F; 
double uRfqF = 4.657565E-31; 
if(uRfqF == -2.34052E-31){
double XFQqVO = 1.998972E-24;
uRfqF = Math.Floor(XFQqVO);
try{
int TuqQtGAUqNfC = 8735383;
if(TuqQtGAUqNfC == 51584){
TuqQtGAUqNfC++;
}else{
TuqQtGAUqNfC--;
}
}catch(Exception ex){

}
}double HSjzuhMPzfwQNG = 8.874279E+14; 
HSjzuhMPzfwQNG = Math.Pow(5, 2);
try{

}catch(Exception ex){

}string sdPsPhh = "BPsDghPLOIBu"; 
sbyte MGTYgRwxRJRPtFd =  -1; 
long egQmsOmd = 46854401400785994; 
short AQbpWeXK = 14529; 
short jInXooaHAneMcJUeZggOzmTa = 25540; 
byte kMoJcVoZTdUmwanSWweYIbaX =  138; 
int pWOnceBQVkGbQOJbae = 736161312; 
if(pWOnceBQVkGbQOJbae == 406918){
pWOnceBQVkGbQOJbae += 956684;
}int lgiMqfzsYdfPShUjsIqtokYa = 859864870; 
if(lgiMqfzsYdfPShUjsIqtokYa == 794564){
lgiMqfzsYdfPShUjsIqtokYa = lgiMqfzsYdfPShUjsIqtokYa + 124059;
}uint JmTtHjTAEyPGVlmNSERNHlCb = 16; 
ulong zHDSDogFoBEiORQ = 80186736604836547; 
byte NixMSkzywa =  240; 
float dHfIzqZJZQxKHQdYSDHqDPfKTxpf = 2.56785E-11F; 
ulong WyozJXyJlHOtablwwzDzpEunBw = 70304610846230744; 
ulong BFpVfXf = 15472031453922293; 
byte sDtcaXmiwHqKhuxmcUQhIstyz =  66; 
ulong WQgjKQHbwfBIdMKqPmTsnF = 55045332454927839; 
int jCmGAhZsNuSXuieVJbJkuxz = 6608; 
while(jCmGAhZsNuSXuieVJbJkuxz == 6608){
jCmGAhZsNuSXuieVJbJkuxz += 632957;
}long ABNzCzeRInBFTMkHMZmqhmttOd = 19065303202751196; 
ushort IKDyEkbXURFGyA = 36035; 

}
} public class upLlguLzDhyPUPAqtPZ{public void xQNW(){
byte zWzpAUpNqQRgom =  254; 
string xNldesnlJABiEDObLVOZmIEAgz = "AlAMcsVqjYsuFkdRuPFNn"; 
string EsleH = "jNWGJOaiNzRIkFcsBiPiRiK"; 
uint wygVtQnnDalxOWxdIHqNfc = 65959; 
byte dozWacuVnqTmtJDtJEQWMmsej =  135; 
ushort GtYdLCMYQGZibTIfjumIjMtU = 16566; 
short RWtNAJ = 7032; 
short sQDSWFEcEHVTFmVQnMFUZdQwwVb = -3675; 
float AxDONuRwMGhqpGum = -1.494313E+14F; 
uint kXbxVnahyaEgidgApRBBYTZYMIOj = 16; 
long YWpDugSztKYM = 25036440438097765; 
byte YsYcLnBQfnqkhlNFejYbgoUnWKAB =  182; 
sbyte nhbYXEsxF =  116; 
int KuRUWATQNxiQJyKfaO = 96824991; 
if(KuRUWATQNxiQJyKfaO == 50214){
KuRUWATQNxiQJyKfaO += 66520;
}sbyte QbMNudELkT =  -104; 
ulong bRqASZkoBTlwQYzGHs = 17011503731930155; 
sbyte YmRblkYmI =  16; 
byte VYphA =  88; 
int ffECq = 89; 
while(ffECq == 89){
ffECq = ffECq + 492499;
}ulong YqmckjFZkYdgKTKuLFIzZgTKpKj = 45142403419372187; 
long enhY = 51553011329972867; 
double ISqVRzENCZ = -6.414766E-12; 
ISqVRzENCZ = Math.Sqrt(4);
 bool? VcbfJaNle = new bool?();
VcbfJaNle = true;byte HHstRbG =  190; 
ushort AXiKVQOImRSSoDpPfngKmVQWc = 24923; 
ushort mlAxeAnkCdcPMaTwpOsPccWqGK = 38480; 
int mHCuVub = 4259; 
if(mHCuVub == 290117){
mHCuVub = mHCuVub + 110212;
}byte ksY =  59; 
string GobHQkohQuVLc = "yCuQHSdUZD"; 
uint OUgnMsCNHsAOfjMjGuUFcZFKYhGj = 7254; 
short gTxwbDIDioIfbGKx = -24583; 
float GIVUPpyoEN = -1.390586E-07F; 
float aHmN = -9.127544E-27F; 
string qxNqAIsziczyKRlZIeEmdQ = "UNcbZyJQLlUqzgyRQXfEoXyt"; 
sbyte nflFQICGTFxHs =  -124; 
uint NlQBNSjnO = 9595; 

}public void BWoKSFjDE(){
sbyte lPTmTujFeecklE =  40; 
long WXQAKpfXoMOORW = 25072120078979627; 
float LThzLcgjXOHHpWCAOp = 7.206662E+11F; 
ushort nhtFxZXNQGdlPRhHllGEQdpcp = 56949; 
string goMkiUFZf = "GhmkYb"; 
int hKqgd = 590; 
if(hKqgd == 203908){
hKqgd += 852429;
}short EXLGBTScVl = -21714; 
uint GzqFe = 4878; 
byte sUakuhqSugORcjCGyhLcmy =  41; 
int MFBxfEReSFRGpALLcSbNoEnPZIgPi = 890922; 
if(MFBxfEReSFRGpALLcSbNoEnPZIgPi == 246035){
MFBxfEReSFRGpALLcSbNoEnPZIgPi = 648383;
}int xoEXMkGLZhJZqjGgSKiqLyyyc = 363824056; 
if(xoEXMkGLZhJZqjGgSKiqLyyyc == 211106){
xoEXMkGLZhJZqjGgSKiqLyyyc += 55973;
}float UHEzQD = -5.87754E+11F; 
string ByP = "ooFDEzlfYRfVRRVHOeRSxEcJc"; 
sbyte JHyXZmedxAIAsVPIMsVMCNzl =  -23; 
int uLlWBxJEzCTgINNAggFzWIWppgwn = 172375364; 
while(uLlWBxJEzCTgINNAggFzWIWppgwn == 172375364){
uLlWBxJEzCTgINNAggFzWIWppgwn = 307815;
}long jDhdKsqfTlmZ = 67569411454646685; 
ulong HtbuIopqOsduoEYCPEqQydUxBHSg = 27970798384047563; 
string LbQCzVWtZN = "ntxxZA"; 
int GUbyjGEcgRcIjBAT = 9916; 
if(GUbyjGEcgRcIjBAT == 971812){
GUbyjGEcgRcIjBAT = 497355;
}sbyte eZyRSAEOtoxuKEHVUTPD =  111; 
sbyte OUEhZDWBfOttjiNjHmnnVdejLtaL =  -102; 
long LZdJWosEkyOqHpkxNh = 64229280584967493; 
ushort HDkwuiIugAQVoJ = 3433; 
float MzTAWn = 1.057104E-06F; 
int chbYhUWXJlt = 461763956; 
if(chbYhUWXJlt == 306001){
chbYhUWXJlt = chbYhUWXJlt + 254784;
}short TGGaVYqBWLnIYSTGleCJatM = -7913; 
int QakJHalJJKxhgZRtZluEueF = 58; 
while(QakJHalJJKxhgZRtZluEueF == 58){
QakJHalJJKxhgZRtZluEueF = QakJHalJJKxhgZRtZluEueF + 746976;
}uint luhiqpnenwpijjeYCSHtAZbsLw = 8; 
ushort EWiQjNYRAd = 47691; 
sbyte heVwSB =  -67; 
byte fgtmtPyVgiJZStatZqZLsEIqS =  222; 
byte apFieMQbxXuitEWDAgpLVcTi =  62; 
uint DAcuACIVRFAUfEY = 49808478; 
double sOHeSLEDVYywWGmnDuDVIMJfX = -5.396773E-20; 
if(sOHeSLEDVYywWGmnDuDVIMJfX == -1.836399E+33){
double gLWujJVZMXujEoT = -1.89873E+24;
sOHeSLEDVYywWGmnDuDVIMJfX = Math.Round(gLWujJVZMXujEoT);
object ciGfBhSzuwYSHjAAD;
ciGfBhSzuwYSHjAAD = -1.493867E-06;
}short mBGwmsj = 2380; 

}public void OeOZdQpYnsHcBHuOuw(){
uint Eyop = 24; 
double VVSKydkGYOtiaTllKgNUkISXKzbp = 0.0003014899; 
VVSKydkGYOtiaTllKgNUkISXKzbp = Math.Ceiling(Math.Acos(0));
uint oRzbwTqsStjGoyQYIDMPdJKlEabEf = 883246493; 
double zCwypbzOuZMhhqqAIoeJMUHgmyuR = 6.999286E-21; 
if(zCwypbzOuZMhhqqAIoeJMUHgmyuR == -3.327968E-20){
zCwypbzOuZMhhqqAIoeJMUHgmyuR = Math.Truncate(zCwypbzOuZMhhqqAIoeJMUHgmyuR);
try{
Console.WriteLine(zCwypbzOuZMhhqqAIoeJMUHgmyuR.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}sbyte LgJ =  71; 
uint ZylbfpAUHbBLKjXioYpEl = 527630720; 
short ecRVBqtxEUbnuZSyJPJLmAYeDSLz = 29202; 
uint VUeamI = 125321989; 
long ogpVUU = 21525123599271493; 
int NtIH = 486596191; 
while(NtIH == 486596191){
NtIH = NtIH + 388165;
}short FQLIZFYtIiJYLZRWddmxwjVFzclPb = 4789; 
ulong EaUpRwTcKlwELKVV = 63148383603707099; 
float jWsiq = -3.098007E-09F; 
sbyte LfBeIjkMjOqUP =  -91; 
short SVMtKy = -46; 
ushort lXKBUqjDojQheaTWitGDku = 51037; 
string AjJQ = "nGeswmBkoyhsAXBZNUSxC"; 
string TBSiJzwhBNFVkdNGHxXeiuTVRC = "NoQt"; 
sbyte LkREeRMU =  -52; 
long UbpVaolaEYLzIWqgwJS = 48244069090039688; 
uint lkTOmaPlhNaFsFmaMj = 74938825; 
long kyWlzqQcemK = 21390135544598141; 
double WRfbRyAgMu = 1.792421E+29; 
WRfbRyAgMu = Math.Ceiling(Math.Sinh(-5));
Console.WriteLine(WRfbRyAgMu.ToString());long gxqeoSbCGgcVaQxjqyDdHU = 89912899751120368; 
long mGYBsAPcJouKTIdsSKIXnIf = 76944820663022280; 
ushort nIuRWSRIBuqBTF = 58110; 
short hBFNAGBMIzXVlIYanGZJB = 27234; 
float oOmOMZnJCgUROJPcUeotUNnNt = -2145684F; 
double JilIOzLsMaIkoufOglFQXPwHzW = 0.1613797; 
while(JilIOzLsMaIkoufOglFQXPwHzW == -2.87332E-21){
double XIiajdKheHiFsiZlnaiWUOGjRW = -5.035541E-17;
JilIOzLsMaIkoufOglFQXPwHzW = XIiajdKheHiFsiZlnaiWUOGjRW / 3;
try{

}catch(Exception ex){

}
}ulong DYdZcgUxRZHcnspHVWNKh = 11572430721378719; 
int zzuobQnZACnxzYRlu = 736133; 
while(zzuobQnZACnxzYRlu == 736133){
zzuobQnZACnxzYRlu = zzuobQnZACnxzYRlu + 93252;
}uint mHUuHWXjyhETdnQHXmhUomzoy = 40; 
ulong JqtTzPjuGTGq = 52161893926264905; 
long UWICahFWACkSORjZGAHmsuDEclI = 67327128131569969; 
ushort WPbPJtef = 25485; 

}public void ZctKQouDB(){
ushort zOADT = 44632; 
ushort hZuNOKLFzKayy = 43848; 
double aKgoBplWcgzaCmPKJMlm = 2.147113E-17; 
while(aKgoBplWcgzaCmPKJMlm == -1.551426E-30){
aKgoBplWcgzaCmPKJMlm = Math.Exp(2);
try{
int OMxMdLefjQtty = 5530374;
if(OMxMdLefjQtty == 84808){
OMxMdLefjQtty++;
}else{
OMxMdLefjQtty--;
Console.Write(OMxMdLefjQtty.ToString());
}
}catch(Exception ex){

}
}ushort gFXPxILYhiVtdORd = 28824; 
double AwaE = -731.0415; 
while(AwaE == -19.30992){
double clWfhpOhIwoXFphXSEKGOVdgaQ = -1.602787E-24;
AwaE = Math.Round(clWfhpOhIwoXFphXSEKGOVdgaQ);
try{
Console.WriteLine(AwaE.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}long AGehhQfqdQSyKqbDZpfhI = 89111758291600624; 
string UCazqWiKaGFctdtpwFal = "oCc"; 
double ldewLdb = -1.396236E+34; 
double eCcBePKOZPmPHGaYDiKe = 4.106409E+08;
ldewLdb = eCcBePKOZPmPHGaYDiKe * 2;
try{
int bkgyIDVuwukZawdyup = 4841588;
if(bkgyIDVuwukZawdyup == 88467){
bkgyIDVuwukZawdyup++;
}else{
bkgyIDVuwukZawdyup--;
Console.Write(bkgyIDVuwukZawdyup.ToString());
}
}catch(Exception ex){

}long gxUTumuaY = 6278436208416820; 
ulong PCWmuzKsLsAWjPhjZbXSo = 5234934772918043; 
short PKkTMjjAwLUKJzkWyEeiW = 18243; 
double RGkdZPIGuKLlY = 8.024201E-07; 
if(RGkdZPIGuKLlY == 1286.357){
RGkdZPIGuKLlY = Math.Ceiling(Math.Cos(2));

}ulong RzMcZbdSqdbezQIRcItqLUGfygXS = 2426441599754595; 
int LaSmOUlwBMdAChYylXSHFTdlYXHxi = 514409273; 
while(LaSmOUlwBMdAChYylXSHFTdlYXHxi == 514409273){
LaSmOUlwBMdAChYylXSHFTdlYXHxi += 942892;
}ushort zfMUcGlOuSoHzss = 37507; 
double RWHnQmPlGkRly = -1.585997E+08; 
RWHnQmPlGkRly = Math.Ceiling(Math.Sinh(-5));
Console.ReadKey();uint INnQBwLnPkZIkeSCpIAF = 101074; 
byte YNlHZRsaZQDdppJnZIjpdwZSVETA =  19; 
double DAqRN = -3.044913E-32; 
if(DAqRN == 7.192468E+28){
DAqRN = Math.Ceiling(Math.Cos(2));
int[] YzVeUtmxeNFpgqSATqj = { 5176075 , 78976 } ;
Random CymZQtTZacGKDjnDdgLDI= new Random();
Console.WriteLine(YzVeUtmxeNFpgqSATqj[CymZQtTZacGKDjnDdgLDI.Next(0,2)]);
}float dFeftdM = 1.173272E-14F; 
string kWnCBmxbhVNXdpxM = "HZaJxQNOPUIZTikfNqh"; 
float cweOJnpI = -7.370334E+22F; 
double fUqdROfCNLt = 8.257459E-37; 
if(fUqdROfCNLt == 982.0566){
double TxFFVVOCxfO = 9.288922E-15;
TxFFVVOCxfO = Math.Sqrt(3);
fUqdROfCNLt = TxFFVVOCxfO;
int? nUcVDbqxqbpMuCORKQJyKoMh = 2140958;
nUcVDbqxqbpMuCORKQJyKoMh += 8098;
}double UZCzlP = -2.863816E+17; 
if(UZCzlP == -4.18104E+28){
UZCzlP = Math.Pow(2, 2.1);

}short YYskVUQyBLlky = 6225; 
int TAIFAgq = 9591; 
while(TAIFAgq == 9591){
TAIFAgq = 107425;
}sbyte uSbBDC =  -77; 
double ANugfFcqXdVwYLVSIgmOmXswER = -2.941426E-08; 
if(ANugfFcqXdVwYLVSIgmOmXswER == -3.077375E+10){
double NMkftep = Math.Log(1000, 10);
ANugfFcqXdVwYLVSIgmOmXswER = NMkftep;
Console.WriteLine(ANugfFcqXdVwYLVSIgmOmXswER.ToString());
}ushort nxhlMnfDs = 47790; 
long iydaGYRkBYCyudDMCePMTXtQN = 68136991021640153; 
sbyte lSbKlajNoUKRJMubmWWYke =  -63; 
byte zkgyigJkqpTAwD =  183; 
long dWctRUKb = 74146717775429177; 
short PdogNbHqxCCSukpZaqjcHpx = -6074; 
int PaQhnMgPWGVXibNBziBMd = 89650021; 
if(PaQhnMgPWGVXibNBziBMd == 265054){
PaQhnMgPWGVXibNBziBMd += 571086;
}
}public void gzIUBdmGB(){
byte PLFnLdcii =  79; 
sbyte mUUGgfoaahVIyXbxeOgZjWLaAu =  -79; 
uint fuVaH = 9924; 
int fAlmlHPWRNcHduj = 65; 
while(fAlmlHPWRNcHduj == 65){
fAlmlHPWRNcHduj += 113907;
}ulong RWVOaIQEbPHJHOWKdlDDywybZMT = 81712950761268881; 
short TPoaiaRxMDt = 14787; 
int IDizw = 866550; 
if(IDizw == 775146){
IDizw += 852679;
}uint bnoctGskx = 5461; 
byte HFnjBmxKfQkeZS =  118; 
sbyte BhKOPiyAZgnVkoHka =  88; 
byte enGdCVzeYuNUeVOu =  149; 
int xQKCfhPUBEUZajGxmDHmULtCG = 3877; 
if(xQKCfhPUBEUZajGxmDHmULtCG == 511275){
xQKCfhPUBEUZajGxmDHmULtCG += 245387;
}int bRKwMAVNbXBdw = 69213437; 
while(bRKwMAVNbXBdw == 69213437){
bRKwMAVNbXBdw = 839735;
}double XMyiWNOYFaqnG = 2.824435E-11; 
XMyiWNOYFaqnG = Math.Ceiling(Math.Sin(2));
Console.ReadLine();long UtMtYHwfZhJCeI = 75363259868489204; 
string JqzJgJhVbpkLVucokBVfamGIBHN = "uBlcnRLHhMZqZNBOysjoTYXJWZV"; 
ushort RiAVmBBdhxeKwhABEfmMRk = 10101; 
ushort cxDEfoVoVpEAxNl = 65299; 
long WeFyRzhtTJ = 78367789607299910; 
double cgjuBSVbhtbMhVls = 5.093438E+07; 
while(cgjuBSVbhtbMhVls == -1.551809E-06){
double aKExZCgbjmPVjWZDgkBI = 4.714533E-07;
cgjuBSVbhtbMhVls = Math.Round(aKExZCgbjmPVjWZDgkBI ,1,MidpointRounding.ToEven);
 bool? LDGATeoLWPAhsFhRULXJxxKJ = new bool?();
LDGATeoLWPAhsFhRULXJxxKJ = true;
}uint KNlP = 35; 
short XTqPOQqgliEULVmCSLsyokPsOz = -18007; 
float zWxxEYOSpjnInl = 5.722634E+09F; 
int OBLWqZSFMeYa = 5834; 
while(OBLWqZSFMeYa == 5834){
OBLWqZSFMeYa = OBLWqZSFMeYa + 227447;
}sbyte hWzKDCeQaZgLhfy =  -43; 
ulong GxEZmKTpTKiFzxAik = 67023418653586721; 
ulong nTjghYlGKUiKPRmIuIxCKkYVHoLh = 51362700097714435; 
long aGlQQnbgmjggCwhjnc = 10199609793794209; 
short jclQZRHQoTwLTAcBaJagnpy = 3021; 
uint chthaHVnXjYqh = 77277980; 
short wIjwaQMdgilATGJnFOzygSPPFCS = 2732; 
long psWwcVsYcsHdTyJPe = 67470057602697785; 
int uQzXjuGSUEyEOWHMgRcYqgQynm = 96241828; 
while(uQzXjuGSUEyEOWHMgRcYqgQynm == 96241828){
uQzXjuGSUEyEOWHMgRcYqgQynm += 649575;
}uint XhFu = 684185; 
byte PkIbCgWjhdLFCnabKXO =  166; 

}
} public class RCPdMYzhNutLMqG{public void lLBbkUYXVBKhDKzWGhYJSXQh(){
uint wJqT = 93911787; 
ushort ymmHlWES = 11138; 
byte pFcWyeFtWX =  71; 
float DiApGbMNNsml = 1.49316E+37F; 
float fZP = 1.62094E+28F; 
sbyte tNUOkEyniqUddyn =  -64; 
sbyte CEmQKOfKsRYApzwL =  -97; 
int mBioedUPWRFSMIfsFlDcfJ = 225081; 
if(mBioedUPWRFSMIfsFlDcfJ == 676557){
mBioedUPWRFSMIfsFlDcfJ += 36297;
}ushort tpKCTYsniNPZjSNZlSjg = 53496; 
uint BGFZXJPuStmznhjOhZ = 611; 
ulong gzJzHcRSo = 85742440373257776; 
int sXHWuzSbebhAx = 25714845; 
while(sXHWuzSbebhAx == 25714845){
sXHWuzSbebhAx += 478612;
}string EaewECXnAiFMKypADoaOaN = "gszYWyiJWDHPXDBQXUOGTEOMp"; 
double EbznMRjXdfDbQpAJjcJtXbAdUlgXE = 4.253655E+17; 
while(EbznMRjXdfDbQpAJjcJtXbAdUlgXE == 0.007557563){
EbznMRjXdfDbQpAJjcJtXbAdUlgXE = Math.Ceiling(Math.Cosh(5));
int? EbCDwuAKmtztUbqLSkVhK = 5116332;
EbCDwuAKmtztUbqLSkVhK += 22820;
}double ARHFCJICXYK = 6.732029E+19; 
double pUHVINjbLwffFzji = Math.IEEERemainder(3, 4);
ARHFCJICXYK = pUHVINjbLwffFzji;
float AXnFdzFZUqlBMLBE = -1.223231E+19F; 
string pIUiRylOaEuuGfqFFZoRWkJ = "wjsdpHZPs"; 
sbyte SdeNGlRY =  -46; 
double UGuWZjylRfYINHfiytFGczWzB = -8.25025E-24; 
double oMJuIQsfpeUxqfAduGM = -7.696876E-12;
UGuWZjylRfYINHfiytFGczWzB = Math.Floor(oMJuIQsfpeUxqfAduGM);
Console.ReadKey();float NZh = 3014097F; 
uint kndhOfEqTXcSmSqppR = 140242; 
double ZtuzsNcFV = 0.0909215; 
while(ZtuzsNcFV == 8.422707E-16){
double WuOJVX = -4.491746E-38;
ZtuzsNcFV = Math.Ceiling(WuOJVX);
Console.ReadKey();
}byte pff =  123; 
int FYXuwCCDfYBXmpzGA = 538959336; 
while(FYXuwCCDfYBXmpzGA == 538959336){
FYXuwCCDfYBXmpzGA = FYXuwCCDfYBXmpzGA + 648594;
}int YczifBZLVSTkWYktfdbfdcWDLGjXz = 425317140; 
while(YczifBZLVSTkWYktfdbfdcWDLGjXz == 425317140){
YczifBZLVSTkWYktfdbfdcWDLGjXz = 435509;
}uint tayC = 3486; 
uint uEXqOGfGAWVgWzBqble = 534168; 
float CNqNdEEQLzWqbmHLk = -6.39188E+15F; 
float CsfVBgBancdBbwE = -6.498689E+26F; 
string VfjJ = "hzewCTeRadawlSExdPywSQaRcnsoJ"; 
double qJfNcceAuXmA = 4.35261E-07; 
if(qJfNcceAuXmA == -1.081475E-37){
double NmgqymtmBQhd = -6837003;
qJfNcceAuXmA = NmgqymtmBQhd * 2;
 bool? eBuYpcz = new bool?();
eBuYpcz = true;
}byte yfWXlTaKPCauSAjiaeYKepf =  170; 
double lciqAUQJxXwSSlaUHy = -2.670604E-24; 
lciqAUQJxXwSSlaUHy = Math.Pow(double.MinValue, double.MaxValue);
Console.Write(lciqAUQJxXwSSlaUHy.ToString());short xnHOkRY = 32071; 
ulong UyzqZpaIYfnDKpiNnGpcgG = 19409899885047867; 

}public void kLLuERHhCjYemVT(){
int FyEsJLGnFZe = 20; 
while(FyEsJLGnFZe == 20){
FyEsJLGnFZe = FyEsJLGnFZe + 630186;
}string YVGWIzPTiCHEtwPnqFfCZffYR = "hqbfnGScGwYCVUTSy"; 
ulong wMFwUClBCRTQIefpzSdnaMpPuoQJ = 31231294059129641; 
int gEbepkNGqxQFwatzXJwQbFMDoUK = 42; 
if(gEbepkNGqxQFwatzXJwQbFMDoUK == 297712){
gEbepkNGqxQFwatzXJwQbFMDoUK = gEbepkNGqxQFwatzXJwQbFMDoUK + 760285;
}int AlVxfbOYuTImhoVdNuNTDS = 807566; 
while(AlVxfbOYuTImhoVdNuNTDS == 807566){
AlVxfbOYuTImhoVdNuNTDS = 177036;
}long BRSoUlbloOmHqKIIDUCDVVa = 15484321190926597; 
int HsRuSwJbB = 96; 
if(HsRuSwJbB == 511185){
HsRuSwJbB = HsRuSwJbB + 805071;
}sbyte XJIhRGQigmAWlKoItLqRmLAS =  5; 
ushort bclgXXlIQXfPNakLhifowJYD = 52979; 
uint CmiPHxQOfUfD = 8158010; 
string LnVFQcZqKwfuteKq = "GKuHhzsWmeQcHgn"; 
short noVUCAm = 21885; 
sbyte qwwbkPet =  -48; 
long DPDIEXjodoRbiRuxEekhQcnbBS = 80453220341530468; 
long slYDdDcVAulkQtBo = 52335039696642375; 
float bBDTqSfVdsgIemZsCg = 1.115352E+27F; 
ulong bzSjnkXSelJzCCtHcteVpihyxPB = 24635208850902201; 
string hEV = "lmUZGMzZVelOUFTpuOzKOTdgIJuc"; 
short UQjXdVnzQdcjdWTmWCH = 17694; 
int elitjNqXswLpj = 570343; 
if(elitjNqXswLpj == 950591){
elitjNqXswLpj = 44481;
}double wHFPgmwNSwBYBLYPLLBhTLHzNMyK = 9.364959E-38; 
wHFPgmwNSwBYBLYPLLBhTLHzNMyK = Math.Ceiling(Math.Atan(-5));
double YGzBKKkyInw = -3335073; 
YGzBKKkyInw = Math.Ceiling(Math.Sin(2));
float ZsScPXeyWVTDBfOGLyKEwbAJuuNDl = -5.979311E+24F; 
long oICADOFWBVUXeYdmaAe = 63791931635648592; 
byte lzdgWkzmfiJeIUgGL =  80; 
ushort pkYNlpLKJpVuuuoyQQhNF = 65147; 
float wwIKRlzqXBl = -0.4945596F; 
short GBPEQnoKnFKPViKepqHqQIbSyRE = -4987; 
float oXzUClQ = -7.50457E-33F; 
string MuehxE = "nxWKSaf"; 
double XgYKTQfpMqaHznUl = 3.572982E-24; 
if(XgYKTQfpMqaHznUl == -2.385579E+33){
double hylGezPENToftm = 4.468479E-39;
XgYKTQfpMqaHznUl = Math.Round(hylGezPENToftm ,1,MidpointRounding.ToEven);
object fECQhcyoQaKDmgL;
fECQhcyoQaKDmgL = 1.16888E-17;
Console.WriteLine(fECQhcyoQaKDmgL.ToString().ToLower());
}sbyte inUJSsoihQJPUjGiudCEudPntzeMy =  -41; 
sbyte SKuZmcytcLDRjxiCPxLfxFGeA =  30; 
short nDLQLQWjQHWsH = -21553; 
uint BXdXpDXbHYxmeoBHMdeAZ = 12956253; 

}public void AZLiAsAmeWFCD(){
short LGKjM = -7419; 
long CgpHE = 30601230670296732; 
float DnAiiiKYduZtTpEwZRUjbjM = 8.476532E-26F; 
string RMSVOTSgV = "kAWRillFL"; 
sbyte jwKHxIMsCFaf =  107; 
float kAkAbiIiO = -2.156148E+35F; 
ushort XApzWUIphnslfPuIkuaYWaNtke = 12452; 
uint VoyoTubnC = 5; 
byte ckTnWunBEXYwXXNXmRKfcmZUXlws =  96; 
long YNZYDAXiQ = 77045770344338129; 
short nqsfTQkZpKZ = 26705; 
sbyte MZIYYjypJUSqUIudqXdcwUGIfh =  -41; 
uint zRsFzGAlXzfUXIzmDWD = 250957; 
ushort ReA = 54312; 
ushort SRS = 61126; 
long dBMxbUSt = 79394312065108700; 
byte VtePBBYPfkfInXnywNQBBKwZ =  140; 
float cFmGoFcbqfnZlymPInRLkiPNINM = -9.761889E-23F; 
double mIFWj = 6115263; 
double OEdnVOLM = 8.939678E+33;
mIFWj = Math.Ceiling(OEdnVOLM);
try{
int BYCTPQsabqHBcWcyOAolyGUj = 2114758;
if(BYCTPQsabqHBcWcyOAolyGUj == 97877){
BYCTPQsabqHBcWcyOAolyGUj++;
}else{
BYCTPQsabqHBcWcyOAolyGUj--;
}
}catch(Exception ex){

}long XJwHsUdqkP = 32865799120486232; 
int hFdiIgAzmfPqAf = 819871; 
while(hFdiIgAzmfPqAf == 819871){
hFdiIgAzmfPqAf = 468580;
}int YIHxLGFjfDwofsFlZlwFefZiJBVb = 33; 
if(YIHxLGFjfDwofsFlZlwFefZiJBVb == 864136){
YIHxLGFjfDwofsFlZlwFefZiJBVb += 189841;
}int DkwTUcZlVkZllm = 830754; 
while(DkwTUcZlVkZllm == 830754){
DkwTUcZlVkZllm = DkwTUcZlVkZllm + 866718;
}long OkKDXfXpxWMiKOQ = 60677568114952306; 
long ZDObVqNOeyjjqeUoFPdBcDKa = 39152856524044508; 
string XEjoCGYuGpHqtXlzJJxjNxS = "AIBZTUgzjFgqfpnzCIXtukT"; 
float xCIoQaCLxQTeYCLjh = -5.242555E+22F; 
ulong ttwYIeAAtoOIzxtAjtEGGKAutEg = 80958415204142245; 
sbyte phVblcCey =  0; 
sbyte KXOkjoKTaFeY =  -64; 
short lLFRsj = -16098; 
float JFuHYpWc = -6.01249E+29F; 
uint nSSsupkMcXP = 927035495; 
short SJNhydDDGUcUL = 10929; 
string tPRXSNAYUbiiMQyLKVduoMclLpmy = "xBcaawgeqDfj"; 

}public void mwVuRQxskPFIIHNFmCAbHzC(){
sbyte MuqGstY =  -78; 
uint IqqAxbZkLzssPuB = 1; 
int hPTPlfx = 73; 
if(hPTPlfx == 626102){
hPTPlfx = 461958;
}sbyte emWcca =  32; 
byte IPtFWemtsctKODetaVmtoRUjI =  77; 
byte XEhCCch =  112; 
long jIFcnwPyqLRJbftUCQVjWHSex = 2662460961791665; 
byte GOmEzwkZCqnCbhICsCDUEP =  129; 
int BXoGqjbSuOfnmxOM = 31697989; 
if(BXoGqjbSuOfnmxOM == 265610){
BXoGqjbSuOfnmxOM += 862284;
}long jcffohQnIdXzVtkg = 84641065290895486; 
ulong WzNylIm = 56331214242406345; 
float izpRnfHWFkqYVWashlq = 4.554567E-25F; 
float kgXWcqgxwLI = 7.54981E+09F; 
uint JtgCHyWQBseE = 998661; 
int xpieKpjFFzO = 265071210; 
while(xpieKpjFFzO == 265071210){
xpieKpjFFzO = xpieKpjFFzO + 870293;
}ulong YuKlSoqxEXOLxPtkHy = 37725349635006933; 
string msLKGxJWNeqPsTGHQ = "sgUctWAhkPSNdinPjlmdfZf"; 
ushort DgFozEVfwqUIjZO = 13754; 
ushort UeZahCEJ = 24994; 
ushort IsayERQUSNkWHJSugamzTI = 51570; 
byte zCYcMPjDuNYT =  55; 
int YpFzJit = 3025; 
while(YpFzJit == 3025){
YpFzJit = 133865;
}double ZEPeeoSRgXaumVTCVueADdqDjf = -2.910511E+12; 
ZEPeeoSRgXaumVTCVueADdqDjf = Math.Ceiling(Math.Sin(2));
Console.Write(ZEPeeoSRgXaumVTCVueADdqDjf.ToString());long bunYiNuQclpxszauXGeYX = 80376154384354380; 
ushort ZfjuFymgZzkiYBNAO = 34347; 
short WBM = 16771; 
uint WLDkboHLppiewXKAfAMtDRCgOR = 40; 
int uxVLCDFOxVCnjxq = 10724264; 
while(uxVLCDFOxVCnjxq == 10724264){
uxVLCDFOxVCnjxq += 415498;
}byte fpf =  43; 
ushort kflgRWMjLwgVqd = 54073; 
float HeQfMQdgdh = -5.178188E+22F; 
ushort uiqlPogtBFAwYk = 35943; 
string jmbAdGq = "piZOOptpaMNZSbDh"; 
byte kGpRTFYZlDtWpDZMAg =  35; 
int offXzQeqiLTlAuMizowPPe = 926600812; 
while(offXzQeqiLTlAuMizowPPe == 926600812){
offXzQeqiLTlAuMizowPPe = 993459;
}
}public void CwaJS(){
double kTwOUIcSBLQinDaS = 1.37577E+23; 
kTwOUIcSBLQinDaS = Math.Pow(double.MinValue, double.MaxValue);
object EoTVNkbathPxwDIhj;
EoTVNkbathPxwDIhj = -3.487054E+16;uint gLHxOlxpIYeGjYeF = 271505178; 
uint DuOpReyyacIRyGOfCTZnZpMLbftM = 10878854; 
sbyte eDWMWqFJPYoGpnhFkZdc =  -59; 
byte mJdnMMLQVXZMVJbiknqzaafwNXOC =  163; 
string uTFK = "TcTW"; 
string uIpCmVi = "TOWNXlDJCOYjiTUGTpZYphwwu"; 
float yelLSsRinyfIzCkCfYU = 122.7213F; 
ulong IXWkBLtCseGeEocfRsLHh = 79996827471860817; 
uint eLuFOVhqyVWNESxHlcScHRgDTo = 765710; 
ushort ddSkB = 25334; 
sbyte ojKbmLkbeJazwmFymnERL =  -7; 
uint WtSSUsscyCNIt = 564910; 
ushort SjNNWJQAuXl = 54020; 
short QKpfUTtgNpYaTBWeILgBToNHZKbtB = -6760; 
sbyte fDWXcAJEY =  55; 
ushort PVQpDopBKRgCXK = 7347; 
byte aGsjNUqEZbibEDhSpReUMQkeF =  135; 
short wattAMXfelcaAM = 4179; 
byte AgoZKI =  217; 
string fjumGkGViYJ = "CRFbNdOdwamXYfwaxMtCsjHO"; 
long HSwEDFFqXoLXWEdXUeWxoAQGA = 19547766928352935; 
float WJRzJsHifpzPPy = -3.323957E-16F; 
uint nDhbQeeYzUGeUHbhgncHneJqK = 185; 
long KbcIYcqoVTNtKUMEbwRNIpgmV = 37472396204372634; 
sbyte SDxVEKSluomEjWOJUjVjtVP =  61; 
ushort yzDtKXDgTaFAncPdMXIaaYcJGU = 33056; 
double XOKCLmuJbGpac = -5.191215E+20; 
if(XOKCLmuJbGpac == -3.546288E+31){
double RetTzxUNQ = 1.077921E-08;
XOKCLmuJbGpac = Math.Floor(RetTzxUNQ);
 bool? lUYDQwwLssZXVnFlYIEePWGhSOX = new bool?();
lUYDQwwLssZXVnFlYIEePWGhSOX = true;
}sbyte oQLbyGyCAE =  33; 
ulong uPFFEjsyTOWkQAMYcwJbmV = 78794537618201474; 
ulong QwUMmkyyOS = 51904205439625343; 
ushort YMAAjWD = 2173; 
double AHXIuuFZLRYk = -3.623391E-09; 
while(AHXIuuFZLRYk == 8.232673E+29){
AHXIuuFZLRYk = Math.Floor(-2.928625E+30);
object fxmCLSF;
fxmCLSF = -4.33996E-12;
Console.WriteLine(fxmCLSF.ToString().ToLower());
}float spzjyuiDJR = -3.35821E+16F; 
uint PIHECkzDGRuXoqnPtmWPNfPs = 4590; 

}
} public class IeIdoelDAgAFHR{public void LHPAfSnTzPOLbJSopdORzYOe(){
float OPJU = 5.550447E-16F; 
double whDOsFtkinBukUISiCSZpNTLHcdAk = 1.539232E-29; 
while(whDOsFtkinBukUISiCSZpNTLHcdAk == 3.803625E-34){
double LjynPAFzSThRLgyYIJ = Math.Log(1000, 10);
whDOsFtkinBukUISiCSZpNTLHcdAk = LjynPAFzSThRLgyYIJ;
Console.Write(whDOsFtkinBukUISiCSZpNTLHcdAk.ToString());
}float mjMdpPxGLULZfpDGMhaTMc = 3.1829E-25F; 
byte YYNOcFqzXhIQKl =  87; 
ulong CjuotbJyjBYZjJCPUQzSXRgIHt = 75814406975696923; 
double TfG = 1.062645E-32; 
if(TfG == 5.465278E-34){
double gaPwTWWKcNJaHMBN = Math.Log(1000, 10);
TfG = gaPwTWWKcNJaHMBN;

}short RfqWwMPFdajq = -22343; 
short ZzBuLUEetPeOuEQpHmxPTM = -9132; 
ulong hlTHipRWFXIXjFznbYgqYuamLE = 45131670677449608; 
int LyjzmRPHitxIXBSOlwUN = 6119; 
if(LyjzmRPHitxIXBSOlwUN == 229518){
LyjzmRPHitxIXBSOlwUN = 88974;
}ushort KDjJbQEEOIdhGueqhxPaZAT = 22513; 
int xeBFmixcWHiOHgXoZEsNhQCGy = 29965377; 
while(xeBFmixcWHiOHgXoZEsNhQCGy == 29965377){
xeBFmixcWHiOHgXoZEsNhQCGy = xeBFmixcWHiOHgXoZEsNhQCGy + 668734;
}float kuzc = -7.631076E-08F; 
float DzzhuQIcLBoZUJGEVhyTco = 3.8975E+34F; 
short xNAlQiyaFscydZIqcKtShC = 17855; 
float bXbKnM = 5.1977E+22F; 
long VEzLgDTenBzdy = 50681998700192838; 
double fOkwqqGDHwGMNFYmpn = -0.4274602; 
while(fOkwqqGDHwGMNFYmpn == 1.114607E+35){
double KWqeA = -7.145673E+20;
KWqeA = Math.Sqrt(3);
fOkwqqGDHwGMNFYmpn = KWqeA;
try{

}catch(Exception ex){

}
}string GGhPczQWgjCjz = "sCsMMkwbYaFNGMGBdosVpa"; 
sbyte dpXoWiHClfeoXWFQY =  18; 
int joTdqG = 25; 
if(joTdqG == 411355){
joTdqG = 331090;
}ushort IwmgfQucCAHpEgLDWNuasPyT = 62902; 
short tLJL = 17528; 
double oiDSzR = 7.68395E-28; 
double nHJXtzSYuiPOasTPTu = -5.651631E-36;
nHJXtzSYuiPOasTPTu = Math.Sqrt(3);
oiDSzR = nHJXtzSYuiPOasTPTu;
object doEnj;
doEnj = -2.082051E+25;string QCJx = "hKJcMQUE"; 
int GhanDuuenRUYlCLjWRNc = 757994640; 
while(GhanDuuenRUYlCLjWRNc == 757994640){
GhanDuuenRUYlCLjWRNc = 388809;
}int DugBSoJURhzIxxwG = 1936; 
while(DugBSoJURhzIxxwG == 1936){
DugBSoJURhzIxxwG += 541107;
}float QBPS = 4.396787E+12F; 
ushort OTDTKuf = 61914; 
float sOseHwsPcIWziaBcKqBRhhMpU = 2.662934E+29F; 
uint VeBoCjfMLUtAXHOloLCsHLOxbJj = 5; 
byte CQTh =  145; 
ushort IRuqYxh = 21756; 
byte YdFStinkpdnSZeJBR =  213; 
long AjPfc = 5500731024968033; 

}public void cYHlhHFWmtVPRpIzGpIf(){
string nzPJkYzYOFtqbLTGJcADWxkm = "IInMZbtGnKhxAxHznFqxBnXFaH"; 
int BSRmCPEqkmPDZMNaLij = 86184776; 
while(BSRmCPEqkmPDZMNaLij == 86184776){
BSRmCPEqkmPDZMNaLij = 887161;
}float tKJMsxNfxSVV = 1.994536E+27F; 
short bPOxOCXTRy = 32053; 
ulong jYtKxjHCCyflHq = 56151264942776800; 
string uFSNQ = "VyaJlTCknJJNBsJkhkPdEYljHEV"; 
byte RsUMHCJlXen =  72; 
string eVdgMyjx = "YNqXkQcJYnVxqPbTQZYk"; 
sbyte OCNhoEyiQfwLVYpF =  -10; 
ushort xpbhOOfbmVzllCyYPjdCAlPyPbH = 56668; 
string lCDpKkNtZtiCjmsUi = "zaBuuUx"; 
byte EtMhmZLKXJWRmkxk =  175; 
short bCeUSywbbSgITAKjamlVCAEwlmsk = 19558; 
sbyte LDziYzz =  95; 
long XJSQbcqDbwYyxwDYKJGfjlXJwf = 65044787701566168; 
int MFjjaIOKkmfcHaPPnFJLLiwWH = 83; 
while(MFjjaIOKkmfcHaPPnFJLLiwWH == 83){
MFjjaIOKkmfcHaPPnFJLLiwWH = MFjjaIOKkmfcHaPPnFJLLiwWH + 907600;
}ushort LlezweCznzhZtLKWNfXwTfDjn = 36990; 
double yRGsNkVOKpSAgmyLmWnphufA = 6.08458E+35; 
yRGsNkVOKpSAgmyLmWnphufA = Math.Exp(2);
Console.WriteLine(yRGsNkVOKpSAgmyLmWnphufA.ToString());int cWMLwyVBQypmc = 373978778; 
if(cWMLwyVBQypmc == 497867){
cWMLwyVBQypmc = cWMLwyVBQypmc + 632441;
}sbyte xVbVUeUs =  -62; 
float yZspohbmk = 1.166151E-07F; 
long YyQYpSzltkHa = 18641265688504968; 
string dCdsRzuYkbkJXxoVcwtadPLo = "sBTjRSeohunVYbZSuZF"; 
ulong See = 7626847034505787; 
ulong AYBPtifuPcaadutiKPgzFA = 18714619249630509; 
sbyte glDbqGHwKHdbdec =  66; 
float cAKpgZDLKJznhsXEUSSqRdiLPYx = -7.157945E+16F; 
string XXqtGNQeDzHWXWxzLeBkj = "DdZqZCiDiZJZCpRYqQdNoSkgkOs"; 
uint nkQzbwmuoOoWapHmYwfHMO = 13267014; 
ushort KTSgliISUOTCBVFFccP = 53314; 
short ETmtmTZBBJbwLtDiD = -24879; 
short wTuyUkc = -23092; 
long sHqhpYTSVPSumkAfjkcU = 58499874506510926; 
uint QRPS = 3887; 
string dEoKkApbGjXPaRfSiK = "kZFAd"; 

}public void TYbGuTfcGhAFQ(){
uint Rgfp = 689348760; 
string MWKHoSEzekpqBnwkpRMwKHmslcuTA = "kKSZoJL"; 
byte jbJuuBgDDLUabXHpfKswmzLZV =  1; 
int BZNBcaDtalTSfARi = 158203; 
while(BZNBcaDtalTSfARi == 158203){
BZNBcaDtalTSfARi = BZNBcaDtalTSfARi + 683231;
}sbyte tSELqKAtUHbDXuXawmpURAoNICuNR =  92; 
short QOhkyRcqMAXzynkkkisYuohTD = 20141; 
string oMDbsjNjlMDEkZhqDRotpjDI = "oyBGoJIOmbaWVfnCb"; 
double GOOwhRqMBKAeiUB = -8.138007E+18; 
if(GOOwhRqMBKAeiUB == 7.240745E+33){
double XguHLbSokecNCctPXXgdNWRcwcBo = -3.50206E+31;
GOOwhRqMBKAeiUB = XguHLbSokecNCctPXXgdNWRcwcBo / 3;
Console.ReadKey();
}long tsmWJlDcNjICRyHbnWm = 6081264950743329; 
short wBZQfzyMjfAIXEhDgKflh = -6211; 
ushort fhJnOELWltqKJLMY = 11425; 
short QOXBh = 27843; 
double mjfDadzLX = 2.518623E-25; 
mjfDadzLX = Math.Ceiling(Math.Atan(-5));
object SqzNgRqHkZOiZ;
SqzNgRqHkZOiZ = -9.699397E+10;long pnRgoWQpOURKxkZdBR = 17626247179739074; 
byte egHiAxyUiljzulhaHsudcY =  47; 
float hXFIXpONf = -1.919616E+10F; 
byte jaOeUoHMitEFmyggIfWPV =  84; 
uint lkVxMoaCNYhtpYeVkCZEacLTHH = 119797; 
long NTCUTJukZTtcSno = 29681319551355602; 
float iBbQfRURnkoSwfwBaCgkYkxET = 3.468709E+20F; 
sbyte VtCIcKiswUbzkEdZVxEG =  5; 
int kKTXezWpdPitROktwmiGIeMNI = 5760; 
if(kKTXezWpdPitROktwmiGIeMNI == 190747){
kKTXezWpdPitROktwmiGIeMNI = 492171;
}sbyte CpXiMeTzEYKc =  -75; 
byte LcitsXkgX =  31; 
short UYXTobapzmjKhogdfcLxhs = -16059; 
short gPw = -11272; 
sbyte cwt =  123; 
uint euakVOzTAkcQgaVwUEC = 62816337; 
float mFZomhn = 2.040025E-15F; 
int bJLllFPyDVMFkhf = 53437620; 
if(bJLllFPyDVMFkhf == 805640){
bJLllFPyDVMFkhf = 998567;
}byte NHWzkLkx =  188; 
byte ZlLIqXVjpmBZOghCMYTQy =  200; 
ushort GofFMqAZuCiTVTzAUQWXlHisdjAy = 57471; 
ushort kPOcbSqy = 64496; 
ulong GlYoGSAtVHSBLYgOB = 63790485717255083; 

}public void QyIDtX(){
byte aWuPEHXBzFsoceVdRVhLwG =  234; 
string AbcLtnVp = "Yku"; 
long kfGBMGQdMDEMzPAowatFeFUPfpyYc = 85510737061683718; 
sbyte wuwpBgHQ =  -100; 
float DkagTGkQwwhRbGSUHCSTAp = 4.218809E+35F; 
byte otIABqhqEAKYdsuZKOmDt =  175; 
long ZonXlmucW = 80354301821573775; 
long tjJy = 75928538720851768; 
int uBldjfpeHbTbEQOTxTJHpc = 8239; 
if(uBldjfpeHbTbEQOTxTJHpc == 712678){
uBldjfpeHbTbEQOTxTJHpc = 626887;
}short nULgOwJNAFM = -15532; 
int FzWKSjoOHYwqjwpMwKNp = 98801012; 
while(FzWKSjoOHYwqjwpMwKNp == 98801012){
FzWKSjoOHYwqjwpMwKNp = 365631;
}byte teUcAJloDbPhcBmMLxkny =  194; 
byte BmTG =  36; 
uint AZGwDdtSBibitVGnmNFM = 15060808; 
double GVSCmQHhd = -1.107907E-16; 
if(GVSCmQHhd == -5.868632E-32){
GVSCmQHhd = Math.Ceiling(Math.Acos(0));
 bool? TfaGyShcWtbyRQbfLH = new bool?();
TfaGyShcWtbyRQbfLH = true;
}double DzPCzhyQGuaTmFOCC = -1.129811E-12; 
while(DzPCzhyQGuaTmFOCC == 7.554234E-18){
DzPCzhyQGuaTmFOCC = Math.Ceiling(Math.Tan(1));
int[] WqeSyDCVsIzOnKgUfsGRgDZwYqkks = { 3344045 , 55209 } ;
Random lPQoT= new Random();
Console.WriteLine(WqeSyDCVsIzOnKgUfsGRgDZwYqkks[lPQoT.Next(0,2)]);
}uint oyFBuaBZTind = 11021417; 
sbyte usJUgsqEqwJYYRQsGZquCcFwzs =  -40; 
string XzKJ = "zZTHkXgbiYsCac"; 
ushort zOPNqsNnxPZoAtWYoCsQzZPtnBTG = 4708; 
double pJgALVWfglGk = -1.200161E+34; 
double KEzAZwAQAgjAyKGs = -1.075923E-10;
pJgALVWfglGk = Math.Round(KEzAZwAQAgjAyKGs ,1,MidpointRounding.ToEven);
float DAQEdBLTBEVgFz = -5.395271E-34F; 
ushort jPgOwPR = 10798; 
sbyte pfC =  -32; 
uint DXUwjLzc = 7345; 
double SBlHmRTcoOeaNVuKEwcCJuWziYY = -8.956484E-16; 
if(SBlHmRTcoOeaNVuKEwcCJuWziYY == -4.473455E+11){
double PJSKBwVOTUJoqPAUKxlz = 2.122833E+32;
SBlHmRTcoOeaNVuKEwcCJuWziYY = Math.Ceiling(PJSKBwVOTUJoqPAUKxlz);
Console.ReadLine();
}float EVGsswYYxhiYjljHMxPgN = 7.132419E+25F; 
long BUEPJhFyYFGYlHZLTFUcGIVAcNaVL = 59390156224023276; 
byte ZEBlHFJAPQxUgXGtTYeMVhK =  188; 
string DaTQzpJRVzewnVunkfodLfs = "YDp"; 
ushort SUOkUbIHZqfkpcgnyLjsIWFyXc = 28548; 
long RWZlXcFqkuTMHlOiTudkMCRgek = 30629013777253113; 
sbyte gYsZDXmqpHaLQn =  -107; 
long ygpeTgNhnZWD = 82682979140884833; 
string OoBRzkjAcjilCIRqnSGRn = "pPaTgHzljHoOsbFbICXPXNtdlllb"; 

}public void cxmwRYLQGeDbRZDWNoQMljeT(){
uint DwX = 90543725; 
ushort xhkctCdTlcywOg = 49887; 
float zWxoES = -1.935304E-11F; 
double woSXihFuRqUuOMTZgIzLUPFRj = 6.258244E+35; 
if(woSXihFuRqUuOMTZgIzLUPFRj == 6.591255E-16){
woSXihFuRqUuOMTZgIzLUPFRj = Math.Ceiling(Math.Cosh(5));
object jxCCzkXdeo;
jxCCzkXdeo = 3.836926E-38;
Console.WriteLine(jxCCzkXdeo.ToString().ToLower());
}string Nkua = "geG"; 
sbyte JpILPNFwKuWLkXowNpdxnMpiOXze =  18; 
double JFtVThAYnnXHJgbZyMGuPC = 5.733114E+13; 
double RMKmgSbwnnqQhpeIc = 2.744211E+26;
JFtVThAYnnXHJgbZyMGuPC = Math.Floor(RMKmgSbwnnqQhpeIc);
byte tkjJiRQFbGyAwqpgDmqlMIlN =  57; 
ulong YuQOwlWiiIImueIfnLnLV = 60714733356635182; 
double ywhRklSbmAhhzU = 2.052735E+34; 
if(ywhRklSbmAhhzU == -1712.854){
ywhRklSbmAhhzU = Math.Sqrt(4);
Console.ReadLine();
}float jbwAsuLNuxaaHLYnKhLUIJCsXRawc = 3.270338E-33F; 
int TmiQLARDKAaIaE = 73; 
while(TmiQLARDKAaIaE == 73){
TmiQLARDKAaIaE = 718595;
}double BAcDBuoLHsxF = 0.0001337666; 
double jBKXnYJTuTTzBNJNiYymZSJlwQFW = 9.936277E+28;
BAcDBuoLHsxF = Math.Ceiling(jBKXnYJTuTTzBNJNiYymZSJlwQFW);
object GuqZsFmwGRlzEfmLgLRKxeXqS;
GuqZsFmwGRlzEfmLgLRKxeXqS = 3.852608E-10;int IoDTPZDT = 510514; 
while(IoDTPZDT == 510514){
IoDTPZDT += 881607;
}string bTIkgCmgpypmXuzPKIQlVBXi = "zHsKuDBBWaNBwdg"; 
string EsKpVacIpAdRUB = "ANIVVyVVPMbuLooEKjAWGAiEHpbe"; 
string KbARaIxkgYJxUET = "qsaVtHwEqaFdnFwQRsb"; 
string UnHhzcCniFFLaYKdhddHVBacsX = "DTGDCSMydtOVETphw"; 
string nHMNJnYtYtnF = "CBYwEcdSnBGKuiBNGsFcaJVQ"; 
sbyte nEzOePPYtcUVFtXAZggcofyHtbK =  9; 
ulong GioIPAuBxQUwPyTKNlfBJPJdHh = 64873155566265497; 
ushort HXFyfAKqhTqZNJnzxniRAk = 353; 
int CetKIlnbgdQHJ = 1876; 
while(CetKIlnbgdQHJ == 1876){
CetKIlnbgdQHJ = 775751;
}float fNOIR = 7.546429E-13F; 
int wWXcFupHB = 4862368; 
if(wWXcFupHB == 599187){
wWXcFupHB = 459480;
}float UZMgolPnxsGcRpWwVITKGPP = 4.00234E+16F; 
ushort PgAhpQ = 3236; 
string TefLhOibLFKq = "ELXFgi"; 
string XxUeahWk = "lOoFCQCiCR"; 
int dgMQJEOjMKUXhWJlGgHlQws = 41866280; 
if(dgMQJEOjMKUXhWJlGgHlQws == 905287){
dgMQJEOjMKUXhWJlGgHlQws += 410038;
}ushort syxUohnsEibDDUqxGgp = 35965; 
ushort uXIOROgC = 56339; 
ulong eDAMbudsPdd = 86878561442294149; 
sbyte jhdzlCqAkbplmysRLENlkGgqiP =  115; 
ulong bfeJ = 83087913276576629; 

}
} 
public class aLXpEpZpMsaqDeseSdJXAZRssR{public void iuZTzoYlMncGDYJ(){
uint ciJCFniNDOuaUFYNSQVMADc = 870674; 
int PyfYQzepihWEgQfVBEkSJeTL = 75; 
if(PyfYQzepihWEgQfVBEkSJeTL == 497381){
PyfYQzepihWEgQfVBEkSJeTL += 130109;
}string YzuoxPEDQoPFVlPVtHCb = "OhKEwQcLRQbdnGIxRALZ"; 
long upoQ = 27029111537639240; 
byte LMbZCSZzNBH =  161; 
sbyte DhQbaMNunFUdSWXy =  -126; 
short qMezMzRcVDPjo = 23295; 
ushort LObZAEpyxeyZ = 3911; 
uint AqmPOyKeEiJ = 835520121; 
uint FkzEmJYoVOwIbH = 81; 
double moScdGVaKzQRL = -1.467731E+34; 
if(moScdGVaKzQRL == 7.212739E-23){
double BcqasQIyDsJzFjEChTJlY = 0.0114251;
moScdGVaKzQRL = Math.Floor(BcqasQIyDsJzFjEChTJlY);

}long cKaEnXwfHpOchtaxOQeGETLGa = 13056808764945711; 
long VOly = 45050218552777224; 
float sJhZcmjYuDsfsjchewzqeICtYN = 3.350183E-29F; 
ulong FVYiEVaLGxSXtJf = 29052661061406671; 
uint KgPaoJcjVXxhiGghuPbAhnSm = 259968119; 
ulong ZxFtyuUOkKpNPOz = 79722651422548881; 
byte bHxoDbbTGjztdLPM =  89; 
double AoKuQgEgXHUtOtTwOzAJI = -3.419364E+28; 
if(AoKuQgEgXHUtOtTwOzAJI == 3.559621E+36){
AoKuQgEgXHUtOtTwOzAJI = Math.Pow(5, 2);

}short SCiswWCQRxQtaU = 12253; 
string hVt = "kAwCFnaPbbCyDhTaq"; 
byte fSmecYKTBEyGtXTHcNRRJCbPun =  230; 
uint QcKVbnKXutDVwAkFxhBJCRn = 412090; 
byte QzKOdkgKCNLqKiJItn =  236; 
byte QkY =  28; 
ushort kZOjARKUAKGzKoQKRMQRASiU = 15176; 
uint DZnMJJwVLNEkBZojYOQlYlgtmoEq = 8142; 
float aUgUlYWPoteWnBpRjJkfxmZqSUzt = -9.902812E-33F; 
uint SGjGSuYaRaXFgkfMYNgLGPCOVQb = 4827; 
int kRjdIpKYPKjnwpujIPNus = 837220450; 
while(kRjdIpKYPKjnwpujIPNus == 837220450){
kRjdIpKYPKjnwpujIPNus += 436493;
}long tICn = 52989030657622022; 
ulong awBfyBGuCMjs = 64772243681557564; 
ushort JaeE = 42087; 
byte qqfEGRTEYZkyZfPeY =  119; 
ulong gVD = 85422666825967887; 

}public void sTkwdOIibxFUOzyWUWgfUkuxOP(){
long SgeXesefVtajQEZLqIDuz = 75822295201186577; 
ulong ndYpUqHPwuqbCWWoplJe = 75934459048523305; 
short XIgMMfSnetKSg = 32500; 
sbyte pHX =  -4; 
long QKBbNMAGQEAcyKVQjRxcgowngqf = 64659506056287444; 
ushort LtgOAEgt = 5440; 
int eBygcGPBomUxHI = 93031219; 
while(eBygcGPBomUxHI == 93031219){
eBygcGPBomUxHI = 426688;
}float GAzpnHMnVcOIMAx = 2.230933E+21F; 
sbyte qhXQWBgTH =  37; 
long fFHUXJMsiPaqSIYWgAATlkC = 321539542999771; 
int oIdfFzpc = 5405118; 
if(oIdfFzpc == 621256){
oIdfFzpc += 949403;
}short kMhhRBVdpmngVjxKt = -23865; 
int UQDajRAhZibMmnXaXztPMZPskigEH = 773579601; 
while(UQDajRAhZibMmnXaXztPMZPskigEH == 773579601){
UQDajRAhZibMmnXaXztPMZPskigEH = 636840;
}ushort gXZbOdh = 13697; 
uint HmwNUOPmUbkQbLVBhuDsZIERRB = 7480; 
double HgJoApkeTLGaMsYpWofmVOJjUZKF = 7.942455E-10; 
while(HgJoApkeTLGaMsYpWofmVOJjUZKF == -2.071339E-16){
HgJoApkeTLGaMsYpWofmVOJjUZKF = Math.Pow(2, 2.1);
Console.WriteLine(HgJoApkeTLGaMsYpWofmVOJjUZKF.ToString());
}short agNiGIaywhUML = 32604; 
sbyte QxpARs =  9; 
short lqoKXkPeupnRQVXJVqQOipBZfqRH = 22425; 
int FykupMNyWfIqKUpaBtQW = 71; 
if(FykupMNyWfIqKUpaBtQW == 920031){
FykupMNyWfIqKUpaBtQW = FykupMNyWfIqKUpaBtQW + 809859;
}short GuimQIKtFCHXhTTJBZ = 21663; 
sbyte qqmbIHSWxzBxiRDWuCbjWwYM =  95; 
ulong SSnWYjP = 87935352257813984; 
sbyte gBYb =  -46; 
ulong zQVTVs = 36708092567100267; 
uint tDBaitasA = 9161; 
int PgCfhzinWtLu = 26; 
if(PgCfhzinWtLu == 368617){
PgCfhzinWtLu = PgCfhzinWtLu + 973842;
}uint mGzRMXePDtIxWPetAXb = 35; 
double jPGefzCgouafjI = 3.676936E-10; 
if(jPGefzCgouafjI == -1.984206E-10){
double zoiiqwWzmni = Math.IEEERemainder(3, 4);
jPGefzCgouafjI = zoiiqwWzmni;

}string CZhhRTtgpBlWCDepFiJRKTne = "QNBinnwaWPteuPwe"; 
ushort EFsBMMjuVskfJMcJb = 8326; 
float cmRVGOLljtnNosKBhoaOZUtcQQgI = -5.362021E+17F; 
ulong lyzqNxUXbY = 8802466576599891; 
uint fdYiUOfPIdjzGZCRTsNLwd = 1355897; 
long YsZQFBkaihzpyZqepXOMVfWZx = 18473271511563500; 

}public void QOQR(){
uint mFQtsEVOupNQlXWSwZuUM = 6852; 
sbyte BRuiEozzezpoDHTPZxboCMCgY =  -41; 
float FXTVKDSMBGRboqh = -2.673603E+07F; 
int sCeMYIRpPpnkkybUgGwGaPgouF = 1; 
while(sCeMYIRpPpnkkybUgGwGaPgouF == 1){
sCeMYIRpPpnkkybUgGwGaPgouF = 512930;
}ushort zWAlUmTmcxluGEnx = 1021; 
float XSEuQqtI = 1.013215E-30F; 
byte OPMYfwVLh =  187; 
ulong LcaZxNODjpzHVCJUBGcgRiz = 21832847884155686; 
byte QKNqu =  54; 
string JdyqedEMajDK = "VCFwfSFMybmmEeSWDfuY"; 
ulong famOCFp = 23521951460145314; 
double tAGkbkeCYM = 0.001155489; 
if(tAGkbkeCYM == -2.556141E+16){
tAGkbkeCYM = Math.Ceiling(Math.Cosh(5));
Console.Write(tAGkbkeCYM.ToString());
}byte VCcZhTTZYYef =  82; 
ulong OSzPHmEVqsSPTotpgNgyUMbUY = 44084338158142814; 
byte oxWEesHLHodZLGIYsjsXcQERmgpjH =  28; 
ushort GodUjjbfmauGVuxIdMU = 15744; 
ulong WzSfqYbyeCT = 45668889896593431; 
byte cwYWTKJfUliQotFcdSdeQ =  236; 
float yZbfpXLeEHUdTjzotqjoKI = -1.96134E-35F; 
double LpeiTfClzTIEQXBelJzCiU = -5.135067E+20; 
double ZhFluZwNOCILFHPClPGxxEwEQWp = 2.364087E-37;
LpeiTfClzTIEQXBelJzCiU = ZhFluZwNOCILFHPClPGxxEwEQWp / 3;
try{

}catch(Exception ex){

}float PpAnIjo = 1.121671E-06F; 
float pyehikCIZgWOZDsZOq = 4.051728E-06F; 
float uILhKMflbdNVDXXAY = 3.862195E+37F; 
ulong cSDALpkBpVwKOcxxaCPCOfSbea = 43161954657774466; 
short iIYxVLLiqlOUCdXxjneXaCwL = 26381; 
sbyte WsXFeKImt =  11; 
ushort duJKSlBBlwgnRFwOIln = 34368; 
ulong OQuIJjUQFUQJbDqS = 46472218454236821; 
ulong oTGgnSUEGTokEpHw = 33650367816426903; 
long gczmGKUiVbkyGYPyWzDsUecF = 20246299724376629; 
ushort LRetmGpwwFktHzt = 8251; 
double nqWcZUEHyolzGwahmXsEkWbzJyupq = 1.136547E-22; 
nqWcZUEHyolzGwahmXsEkWbzJyupq = Math.Ceiling(Math.Sinh(-5));
try{
Console.WriteLine(nqWcZUEHyolzGwahmXsEkWbzJyupq.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}uint OXRVATHoIEiCohGdUpxgouRAKcBo = 408145056; 
byte qdCPIKyYW =  127; 
ushort BJCXJkRlVqIugqkxtq = 29261; 

}public void sWkkHRxjT(){
uint PamZbBDG = 57596473; 
string axgJpzaGFchl = "yqqSOHsyfSuUsjVQMQ"; 
long EjPAhfLSSHNS = 75131995665841696; 
long UENPYofPTmzTamCRLJVAFAfCSXG = 74945748809487298; 
byte thxNcOAuhupJucjbhpwzkWa =  170; 
ushort UoNisXpe = 21965; 
uint MDMGhLEpcjVNLCsU = 52935577; 
ushort OtRTkH = 63804; 
string tpLKCwOKSdkx = "sbYDcIZcDDUt"; 
long LSwBMCXJTkdif = 6442682411168460; 
ulong IFqcpQNRpQccRxDAUxJiOATCuRH = 87321120336417975; 
double AHSftNtdCuzUM = 4.015889E+35; 
while(AHSftNtdCuzUM == 5.722691E-29){
double zjIhIzKhUKHeuPQcuMByS = -2.899765E+15;
AHSftNtdCuzUM = zjIhIzKhUKHeuPQcuMByS * 2;
Console.WriteLine(AHSftNtdCuzUM.ToString());
}int HHdzYtW = 4654; 
while(HHdzYtW == 4654){
HHdzYtW = 63692;
}sbyte ldhI =  124; 
double PMBwJUhTCtZ = 1.913785E+23; 
while(PMBwJUhTCtZ == -5.681674E-15){
PMBwJUhTCtZ = Math.Ceiling(Math.Sinh(-5));

}double RLmdCCJSqLuAhhoY = 1.610609E+11; 
if(RLmdCCJSqLuAhhoY == -0.2384348){
RLmdCCJSqLuAhhoY = Math.Ceiling(Math.Tan(1));
Console.ReadLine();
}string FdQKkhWFQaIDxhIjuRXaqluTS = "VlWZI"; 
double HnZEGGo = -1.63299E-12; 
if(HnZEGGo == -1.830265E+11){
HnZEGGo = Math.Exp(2);

}double VcZASqWkpCsHMqSjIjWTDpJZqabLG = -1.784646E-17; 
while(VcZASqWkpCsHMqSjIjWTDpJZqabLG == -1.778147E-22){
VcZASqWkpCsHMqSjIjWTDpJZqabLG = Math.Pow(2, 2.1);
try{
int kFhmKDBLHgdE = 7690935;
if(kFhmKDBLHgdE == 1684){
kFhmKDBLHgdE++;
}else{
kFhmKDBLHgdE--;
}
}catch(Exception ex){

}
}float xOmsp = 3.099281E-11F; 
float tMaMSW = -4.067435E+24F; 
ulong qCBL = 68577393558132605; 
long cHPgWqVKFBNSdbwSghYXBDVc = 43786377352798027; 
string JkgXUzhH = "aYTdQzHsxPMoBLMFMThPkWgQTsjL"; 
sbyte APTGVeIgIn =  -59; 
string RpViAu = "iGaSMdRdkzoW"; 
byte pWxVQIW =  149; 
byte BjPoAXjKpQeTPbURmFVjYsRYCt =  122; 
byte NxEwgqCoDFmnGGudxgubY =  101; 
short bVhtQOfzWZDuBzJKsDLaI = 27646; 
ushort xVfmelMsdVukihzPzPqGca = 36201; 
double LJwKsaeCxWVITuK = -1.239111E-23; 
if(LJwKsaeCxWVITuK == 5.409456E-33){
double WCTFNJ = -0.3672613;
LJwKsaeCxWVITuK = Math.Round(WCTFNJ);
Console.Write(LJwKsaeCxWVITuK.ToString());
}ulong nOifYVWNOVaLbG = 73529517792774388; 
byte ASYTIEtVPhmqSbZ =  162; 
long bSMnzjwfwaBFijCLGfWcfjCeWTtj = 326314249830062; 

}public void ktOzDitwjYcPhwTLGSSqIs(){
int lFVGRfVhhZwMiFDbiKNVpwteC = 876070883; 
while(lFVGRfVhhZwMiFDbiKNVpwteC == 876070883){
lFVGRfVhhZwMiFDbiKNVpwteC += 79338;
}float XlYmNFddVIhahDaoQhXGJCRk = 7.779616E-33F; 
float zXeVCXksF = 2.602693E-07F; 
sbyte SwNkeuxSguVgnfwwNxJMR =  22; 
double mzjqJLcMSVRgdZMKxu = -20174.01; 
if(mzjqJLcMSVRgdZMKxu == 7.98549E-18){
mzjqJLcMSVRgdZMKxu = Math.Sqrt(4);
Console.ReadLine();
}ushort FxDIiPoLGyMGzZlpitWmOaiOel = 25630; 
int HeBIIMOtERLHTVgta = 925; 
while(HeBIIMOtERLHTVgta == 925){
HeBIIMOtERLHTVgta = HeBIIMOtERLHTVgta + 419614;
}int eFnPDXw = 16072570; 
if(eFnPDXw == 225261){
eFnPDXw = 960672;
}float zWGDlHVfcw = -1.728887E+34F; 
short SNJWzEgBFaUpXL = 27145; 
ulong cyRMOn = 33740723560230620; 
double mpmKp = -5.641253E-20; 
if(mpmKp == 1.669627E-12){
mpmKp = Math.Truncate(mpmKp);

}ulong IOEfbwtsefuEhhUhfZpleoIyMWi = 4216954611370823; 
string HCzeqbWuhQJ = "dpAZDQtfgufmGIuiiLA"; 
int RAzRgOmMdFfBgHuylpSkUjlwlAK = 74896691; 
while(RAzRgOmMdFfBgHuylpSkUjlwlAK == 74896691){
RAzRgOmMdFfBgHuylpSkUjlwlAK = 87517;
}ulong RahxcfudQ = 25168197339487771; 
long aImdGIdGZSgP = 69188416474615838; 
sbyte gxxAaJIbncKMMFFX =  8; 
ulong BulJjqdHynTBRnACRIfHuXXEefKJP = 51829451223512968; 
ulong iWcjOwDSy = 25144998134301264; 
ushort FdHwRBOpwZKuHdwXKsOi = 10255; 
uint mwMjjgYnbJ = 1568; 
sbyte CkKaegSOTyKUzylPn =  -1; 
string uZfpEjyMILuVmwigeidwwADn = "oIBbmwNfshCftyUK"; 
sbyte hdPId =  -57; 
sbyte fxIuVxRRnDWLwpIQUmHaL =  55; 
sbyte ouwkLIFlWDppyYmenDjsOw =  47; 
float YNQIeZunWeCbGg = 3.808068E+34F; 
long TaqsWaDMhCQXUKEPMUPFRDVb = 46340923223656692; 
string QReGRPesSSzZkRyRuLif = "qepScozdMYwUmTNLfdZAzLdECJVH"; 
float SlX = 0.09522577F; 
long OOBHofTcfYXKfoIjGzHaNpu = 41544519065755758; 
ushort Wqo = 31177; 
float FkBwKXWL = 3.226858E-24F; 
string kTLbyugOcwNT = "wDJiXtLM"; 

}
} public class bQlVKxWCXW{public void EqBqVUU(){
short hiyIl = -14129; 
ushort CPRpGWRUQD = 46289; 
long CyRLGdbqndztHhStEJwbIZifS = 8473896815597818; 
byte MHXGksWnMsXZPZClTpJ =  50; 
uint LSIJsViMoL = 5236; 
sbyte HdjJMLyNUYGKUTyUZcdxVe =  116; 
uint beZdlGnSQGQfZzRSYYJdJapowMqyL = 531054130; 
sbyte JKHyKItsRenIOZEmEXgn =  -10; 
short OLJ = 801; 
short hZdLypOjuoppMRWcHHMNkgTwR = -19622; 
short SNagfCJejXqKPciIQYMAd = 14868; 
string PCG = "fJU"; 
byte eqHnikkiRTKocXhXlGSzSGgKZsEVI =  248; 
uint eLLL = 377378; 
int CqmTtNMEydUdbOfzkXoRHoB = 341297539; 
if(CqmTtNMEydUdbOfzkXoRHoB == 332849){
CqmTtNMEydUdbOfzkXoRHoB += 247599;
}ushort SYdWCtnpmHayEd = 29663; 
string GlBFeQhmQuC = "DwpaJceXSFfXHckGSXPUGUM"; 
string RNiWKLZNEKlKxVbteyagEXcGCwSG = "hIJpeSoWaiTfkcsYVwcN"; 
uint xLTsTLEunTtfxcDus = 72; 
ushort ohbt = 58978; 
long GZQSfGlXxMYEsGUaMeH = 21675422755706718; 
sbyte SZYbXOncCZQQFXGehe =  -15; 
ushort onnuje = 61196; 
ulong aieuFMaDwSHXkfQAREyVTaaNx = 3978997840013457; 
long IMjtTdGJGOjdizUsBAVYahqYabqVJ = 13083577948264493; 
byte SmqwZlADkNujTqLYBu =  57; 
short NKayAzDcpfLpVcLJMqNCzfinY = -5554; 
ushort hIuwSMeEqXBNWAVIJUXoaoOc = 31411; 
float ICMAdfNUJMpyi = 1.483066E-27F; 
short RZFZkBGeyeQTTqWbsycFzxIj = 6590; 
byte txOqSXZlAmuxaguGaXbPutwRBfN =  80; 
ushort wuYFlIKOXVUf = 29636; 
ulong abzdRHlEyCaaXRNkJRgRazwUNWma = 43970631285271694; 
short EMiQCclHM = 29193; 
byte loqXXDGEqUtlyDneueqJWx =  64; 

}public void JjXsJfjpPwJZF(){
long WyLVJtWtZPhmIGCPPbWnNGnVkKd = 4260571973540056; 
byte BEbMRXDBS =  139; 
long HUXJwhqjPK = 17316210317039052; 
uint qDnyUCWI = 38976503; 
short tiwYpRn = -31824; 
string xRF = "lBRbTodAWdGBOTlJAwqRWq"; 
double kRozgpg = -9.969544E+26; 
if(kRozgpg == -1.864323E-07){
kRozgpg = Math.Ceiling(Math.Sin(2));
object CaAOQVFJQbaGtH;
CaAOQVFJQbaGtH = -6.473145E-09;
Console.WriteLine(CaAOQVFJQbaGtH.ToString().ToLower());
}uint GujiyQYDLglbNRaT = 277634; 
sbyte ISRkDnjZLXujnEAjGlpeMjdZMdF =  16; 
float xkYDSNEYHj = 3.594607E+35F; 
float NbNjsQmjYbkzPSAzJGixBil = 7.896349E+15F; 
ulong HFfyNsplKpwlabmlWsu = 41549464854089100; 
byte QbAFkHFTFyfAGPhnui =  102; 
byte otqPUgxExXWdJUoWHQuItsUnzgKa =  67; 
int aYHBJhQpAhohBjwXVTsO = 73; 
if(aYHBJhQpAhohBjwXVTsO == 939043){
aYHBJhQpAhohBjwXVTsO = 478811;
}string NxxpQpVfbtGO = "AVBdkudcjPgWSnasdEjfzEcLY"; 
short ooWCmEXU = -26142; 
sbyte MeYnEIpuXcVOXQxI =  -37; 
double PeSkPOnmTsVuVlERehjZuwFuhu = 6.408358E+34; 
double uTTiBpwxccIZC = 1.1363E+35;
PeSkPOnmTsVuVlERehjZuwFuhu = Math.Round(uTTiBpwxccIZC ,1,MidpointRounding.ToEven);
try{

}catch(Exception ex){

}double NzxwbPXUwQNnbTtHMlq = 4.593674E-25; 
if(NzxwbPXUwQNnbTtHMlq == -391.52){
NzxwbPXUwQNnbTtHMlq = Math.Ceiling(Math.Asin(0.2));
Console.WriteLine(NzxwbPXUwQNnbTtHMlq.ToString());
}float LpfGsPdBqYVobkf = -2.680225E+08F; 
int umiAtqaQnQwGI = 275067; 
if(umiAtqaQnQwGI == 801151){
umiAtqaQnQwGI = 176889;
}double RPVOfceFCQlUpBkCRMoqZQL = 3.001841E-12; 
if(RPVOfceFCQlUpBkCRMoqZQL == 9.644176E+18){
double FHDznLmnVkMZiMq = 255.3737;
RPVOfceFCQlUpBkCRMoqZQL = Math.Round(FHDznLmnVkMZiMq);

}short ZEjNttxIdpSGF = 25040; 
byte YtnPXDIEiAnsRxtZRebSFpssjnJtF =  52; 
uint MUDaaAyxWGFUfeQIXSOB = 374644527; 
ushort bVGJToZYXGGXTtZsHFVY = 47961; 
uint fzbwNxbQt = 907328; 
double UPDHH = 2.392023E+33; 
while(UPDHH == -1.049346E+27){
UPDHH = Math.Exp(2);
int[] UzSbnqfCNIFmRU = { 4177291 , 45229 } ;
Random iDVcZ= new Random();
Console.WriteLine(UzSbnqfCNIFmRU[iDVcZ.Next(0,2)]);
}ushort KOwebKlwQjYXbkjfQb = 59522; 
float GbsBNmswYYxdnsBWbAc = -5.855668E-25F; 
ushort VdgqLXjtDDYeMcFoyUZS = 15215; 
uint NTJjODTxxjuxyGWHzPFUBpmOTyj = 52315326; 
byte wtitybNwRKfDUqGgKB =  68; 
uint IIty = 91364040; 

}public void qtmVlGZdnlz(){
int tOkwOsxmpBemKBNMPVRgye = 428240; 
while(tOkwOsxmpBemKBNMPVRgye == 428240){
tOkwOsxmpBemKBNMPVRgye += 339667;
}int ycKPKTyYKnAcbSiICelqzpHyno = 7778; 
while(ycKPKTyYKnAcbSiICelqzpHyno == 7778){
ycKPKTyYKnAcbSiICelqzpHyno = 88759;
}long ccEURToRlEIucG = 16671639955679115; 
ushort CuEJNmjus = 38206; 
ulong eIntcI = 52255982879415254; 
long ZAcVIqtxpGdYumczPhygWMhNcq = 50763832059919699; 
ushort CUzNnRdyEgKERNUtlCYmqpTpWM = 17622; 
ushort ZudRjXVJKSeJLiW = 42699; 
float wYPScshClaiRDEHZEMGMIQZK = 1.027736E+12F; 
float GhXIdVSyJhqMJIMVBT = -97.61549F; 
string FjcfiFZequzFVdcBNHLjpx = "TcZeejItqCTDO"; 
int NmyERXneNISZfJRSxiPidiBVVJbg = 4; 
if(NmyERXneNISZfJRSxiPidiBVVJbg == 907993){
NmyERXneNISZfJRSxiPidiBVVJbg += 795029;
}float NMFzGXmxljDKBK = -5.624375E-12F; 
double UFNDJiVgcLfEep = 1.125884E-09; 
if(UFNDJiVgcLfEep == 414391.4){
UFNDJiVgcLfEep = Math.Pow(double.NegativeInfinity, 2);
int? PIJTYcEAJfWnGxBKUHhOejuNbF = 7688068;
PIJTYcEAJfWnGxBKUHhOejuNbF += 60522;
}byte SthfZUoDIZpTIDDNPeXYsHdeede =  182; 
ulong SAtfBMlUasgSMCABOhEUfJ = 34039037857056752; 
ulong QWgQADcQCUeGRAVDGF = 1247209282076300; 
uint xhTfCEbQwJa = 1003; 
ulong kFczeoNzwGTx = 50476056080330117; 
ushort Zen = 30272; 
sbyte WEdRuB =  -30; 
int GyaKKyAIheAyKFjsuYjGYkGj = 53406098; 
if(GyaKKyAIheAyKFjsuYjGYkGj == 43313){
GyaKKyAIheAyKFjsuYjGYkGj = 45482;
}ushort sjQFMKKEAygtGKmYUzZCjZRuRQeh = 40422; 
double eHDeBPGGLoHfCelFVoT = 38550.84; 
if(eHDeBPGGLoHfCelFVoT == 7.753225E+18){
eHDeBPGGLoHfCelFVoT = Math.Ceiling(Math.Cos(2));
try{

}catch(Exception ex){

}
}long AcfiLxJj = 3011153312166806; 
ushort jASnGme = 45584; 
float dXqobGlemJY = 4.342725E-33F; 
uint UdynxXTWCEjsl = 871030; 
string Esg = "zKaIIADNj"; 
uint sTMiIxxsjaQMSn = 65; 
ushort XfMthwuyHPypjqcOZsL = 46400; 
ushort gZWmJYhuISihlwKZMq = 51107; 
byte HUURzJdoIXJitYMxUNwpmhGh =  8; 
sbyte eZGuWmPAfcxjpdeZUAZT =  -20; 
double REtUEAoNPtbYpbENVF = -3.85378E+08; 
while(REtUEAoNPtbYpbENVF == -207.9416){
REtUEAoNPtbYpbENVF = Math.Pow(5, 2);
try{

}catch(Exception ex){

}
}
}public void SMRgs(){
byte VYXnQmwOoLQJm =  51; 
float znWKFDCVzCeBpxCchjjOj = 9.550072E-38F; 
int YUSDQDUXctTxAQtybPoiUzTiN = 70656; 
while(YUSDQDUXctTxAQtybPoiUzTiN == 70656){
YUSDQDUXctTxAQtybPoiUzTiN += 938162;
}byte SGIKsJYjEnd =  171; 
int uRqiMMxXBbJQnATSExHg = 19733486; 
if(uRqiMMxXBbJQnATSExHg == 671078){
uRqiMMxXBbJQnATSExHg = uRqiMMxXBbJQnATSExHg + 122643;
}short qkPAgUKjPpkATVeGaDuAqLLIcAajf = 3691; 
uint wyGpliKuPKcmlhnMffN = 72; 
float HAhyHTBdlQFdwpPtRTZK = 3.059601E+11F; 
ushort VloeaPtUFQZQeL = 3508; 
uint GRyHdSwqMV = 7318; 
sbyte YcM =  13; 
ushort wdxxLAGqYogQzubRaPR = 24449; 
float oTfhxPaOsiIWTDuaWEmdsyP = -2.312619E-20F; 
ulong gcXMbMJODpDcRpkWXhDCAxaTmgQwS = 6216246678966733; 
ushort jkOSKtCxNhBRoKlBLbe = 30554; 
float nWzIIsbyABCCspLFpAMKcmqlujz = -8.7246E-26F; 
long YLoXRtbMiwp = 32009945840162890; 
long MkTaGlnyUMnQjDQdtUk = 64670931188712408; 
int DAcXMlBwXlgOnnJcSQOWnDVUyR = 90; 
while(DAcXMlBwXlgOnnJcSQOWnDVUyR == 90){
DAcXMlBwXlgOnnJcSQOWnDVUyR = 312605;
}int qmkSyppIwIeBBoHKNBh = 58688093; 
while(qmkSyppIwIeBBoHKNBh == 58688093){
qmkSyppIwIeBBoHKNBh = 519472;
}double FoCX = 1.045663E-27; 
FoCX = Math.Ceiling(Math.Acos(0));
try{

}catch(Exception ex){

}byte nmDDEADdDUGzScyqLNXdxzZBFg =  45; 
long eeqzjLHPPaeHlqZ = 38732190462638334; 
float zPmbYy = -3.57284E+24F; 
int PnZIqRCWNTxjhaW = 761426; 
if(PnZIqRCWNTxjhaW == 11619){
PnZIqRCWNTxjhaW = 8425;
}byte qMWtOREOQGSkpjM =  72; 
short hiaLnHmeDKNnGzGRJVwzOfjeHFp = 19804; 
int bolfTayH = 289799; 
while(bolfTayH == 289799){
bolfTayH = bolfTayH + 881472;
}string atFzCRTTgbZVdKqyz = "IzbAGFubyfsf"; 
float deEWkhJkwtZ = 1.960778E-36F; 
uint OYJjWlDBBOaeBuNBa = 67; 
float fulqJydIBODSzJhJVVdBFdikciRIS = 8.099421E+09F; 
short EJFxTmPASZZylbLGSfoGq = -6993; 
string bnAGwQDhk = "VZElbJo"; 
string diNQhfcaTsMsjGsuGNiecy = "lcgLbGdHqoIYcoysBsPJQzQMt"; 

}public void RxVfXKGq(){
uint bubwQMwcCIpfhwJMmPbOlwq = 770113154; 
int ksYsLOKqfquTedOjMVMoAy = 4258; 
if(ksYsLOKqfquTedOjMVMoAy == 755998){
ksYsLOKqfquTedOjMVMoAy += 523145;
}int kHia = 69; 
if(kHia == 588636){
kHia += 352711;
}uint DuTVX = 47487066; 
byte VcskOhDCwaiKdgNKeTd =  164; 
ushort tRwVQDVtccx = 29237; 
short RPxDaOisouznflSRwD = -22671; 
int CQSqZGcXgnHRiTpyBIKhb = 26244; 
while(CQSqZGcXgnHRiTpyBIKhb == 26244){
CQSqZGcXgnHRiTpyBIKhb = 696340;
}uint HToLVoZEl = 15; 
sbyte ifSxXsDtVVzyI =  125; 
ulong OEVDLAwqgmEpTo = 36512949904806426; 
uint XTMXRmmpkhyy = 5700; 
long LCoKOlAHnWwbHTSIdEFSN = 63182663383101346; 
byte yWJkcRPFLt =  28; 
long RlJqpPtJYSWmawKAgOPeHYeXJPR = 56057025936373744; 
ulong OxcQGAeNXcwfeFGqFnT = 57713944107934570; 
ushort ywtVTsqNnb = 31391; 
double LNwiUoTp = -1.067209E-12; 
if(LNwiUoTp == 3.462932E+29){
LNwiUoTp = Math.Pow(double.NegativeInfinity, 2);
try{
Console.WriteLine(LNwiUoTp.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}ulong FkAmE = 53859839598054809; 
short OOJiWhNWlsYwOGhogMaAdnXDBc = -3467; 
string RgYzHGqztUXSomUGyhGREP = "gQPA"; 
float JTKJkw = -9.059061E+37F; 
double ZQQORc = 1.153608E+22; 
if(ZQQORc == -1.871594E+34){
double hbdRJSgBsKa = -1.775633E+31;
ZQQORc = hbdRJSgBsKa * 2;

}uint ymMNMpYupJonuRcd = 22022849; 
long pwNwjEnYPtMK = 73896066103737555; 
string yRFuqsbYNCsejbB = "DhcaFWWGDBW"; 
ulong jMGqRYVRQmVWRHkqHKTPTVqbbUB = 68574879141361937; 
ulong OtcFzNVgdNpMnWQcYbowFo = 79437594693554072; 
ulong xGTq = 74385099442361890; 
byte LUZuobo =  95; 
sbyte xFEHOzHwZmoJdAl =  71; 
long ZLqPmS = 49502852618489616; 
uint VlHOuYmDhXTcR = 158419; 
uint iuxgHsMAKbCcn = 236441; 
byte FZiUNayPIoYemWXSqFJbn =  68; 

}
} public class UnKWMAUzIShWisBwtZqZcOwxeFk{public void UEiwqenGcdMpNRqlfaOARiocWnSg(){
float RgoYEdKMF = -2.073988E-32F; 
float onbmlxiqeQLe = 2.456329E-17F; 
sbyte cmhMQlFaPnmCtwKw =  110; 
ulong OUbGKLWNqETSPEmDRXiDdKbyXjHXK = 82064108682845874; 
int bTAmyWxzXgSTzOm = 93598483; 
if(bTAmyWxzXgSTzOm == 659202){
bTAmyWxzXgSTzOm = 502648;
}string dknLWAmIDjAhJExECCLJRRmRuItmM = "YXXVwbikAZLzpJ"; 
double FsJocgPuhmYypofZWfl = 1.142372E-25; 
FsJocgPuhmYypofZWfl = Math.Pow(2, 2.1);
Console.ReadLine();short RpCIPfgZy = 11328; 
sbyte YOoczdeQPkXk =  -48; 
byte iXxnlkYn =  26; 
double FnAb = 7.768827E+10; 
while(FnAb == -4.603472E+30){
double coByGtqqFVYbbwThZAjewAQqO = -1.339746E+35;
FnAb = coByGtqqFVYbbwThZAjewAQqO / 3;
int? pekiFkWKBJM = 6919706;
pekiFkWKBJM += 6681;
}short NTxjWzARyyJAzPOicVuuAyyhI = -2774; 
uint NIXJLBPgdcPxwPpmOdawWNcOK = 336947684; 
short oDzYJdQBaPXaHe = 32150; 
sbyte YLgkuFyeUaGiZQiALPBqzNLBNNnc =  -27; 
short cJMVHikUYaTOcksbZOBW = -13244; 
uint AlZHyHpwKGsDBmUDqBVCpFZJ = 4003; 
ulong nUtsETlKIC = 21990893923880080; 
float eljwuXzVlqqcVahQmZZHakV = 8.822818E+19F; 
long LwSXIDXkbKXYGuqzpjKqHytULJ = 15166024606902156; 
ulong utKBXQfgpbtlJNFG = 43901223071269436; 
short qLppmCHR = -27938; 
double juRsmFJoZegMPEuS = 2.327246E-06; 
while(juRsmFJoZegMPEuS == -2.200205E+17){
juRsmFJoZegMPEuS = Math.Ceiling(Math.Asin(0.2));
int? fjZBMSKguIROb = 6546010;
fjZBMSKguIROb += 29439;
}ushort nXhzybRqSpd = 63468; 
ushort KkYSMVRbumCMFoLkYcCgpXoW = 51949; 
ushort sShtHnnSS = 55647; 
ulong BymDVWLyDHHqbxmANW = 89729354335317958; 
double qxTtizbJcuuZdZL = 7.571127E-22; 
qxTtizbJcuuZdZL = Math.Pow(double.NegativeInfinity, 2);
Console.ReadKey();sbyte tPYMNXYjFDPLkp =  2; 
ulong WLZoQi = 20155629095103130; 
long FBujjLqSujmVahVsJi = 57838711129673676; 
short upgoegHQfGXkt = -24109; 
float WsXpIlkocsuzXOzwwWz = -2.506544E-33F; 
double TCfmckZZckWKtRZNieRmNNhSs = -373.4991; 
double dPDZidcsqmadfExipa = 2498.625;
dPDZidcsqmadfExipa = Math.Sqrt(3);
TCfmckZZckWKtRZNieRmNNhSs = dPDZidcsqmadfExipa;
ushort tPpozWxoem = 25423; 

}public void QWRGsoQmXgezAJLywGUMQmbGex(){
ushort OVeJtqEAelWMhgqlLSPPlZUAs = 43315; 
string VkCVVZzbcxeifksiKi = "KWPVmHFFObFpLoaETK"; 
string hIojoNHKbHMQjdtHgoonqwLay = "tUeIzz"; 
long xbIFHAaVEhpP = 79864259625694811; 
uint XnNitNFthmgRIqZDJPnxRDL = 98636140; 
ulong LjUdWTKetSzgmpjZdIoZFuVVHzdVu = 72420178937882985; 
sbyte YYmGLkJa =  -99; 
long IZKNupoZyyPOKQNoAz = 8695203261368529; 
ulong LXzXnRVe = 64697668597980663; 
sbyte DQXViDKI =  82; 
ulong tLcCenAZSBTUIPFdSg = 73015369756610625; 
ulong VmGZhCfisbPamPqKJWpgFzS = 11523474949178894; 
float LdGUTMfKwLQqzVU = 2.638333E-28F; 
sbyte WJj =  -16; 
ushort ICIUo = 6781; 
uint CCLLQPjkxiOcjULawNgDWQCHkPl = 4594; 
ulong GswXti = 51909129735619255; 
short NQIADYVZLaeBaEZNHchLOXqRowP = 14107; 
byte AXTUAuQpEHxxlteGqO =  214; 
double ZCHgQARjhjjPgBgHge = 2.758108E+32; 
if(ZCHgQARjhjjPgBgHge == -1.228149){
double MKdbTlZBwlNKVZgwylHeSYYkKlcnB = Math.IEEERemainder(3, 4);
ZCHgQARjhjjPgBgHge = MKdbTlZBwlNKVZgwylHeSYYkKlcnB;
object BeBehq;
BeBehq = -1.4108E-11;
Console.WriteLine(BeBehq.ToString().ToLower());
}byte auUXGuUHqpVdWSmERoCfdx =  242; 
short ywqP = -18100; 
ulong txZqBwPqcENOgIKmChbcf = 7890515739403745; 
int fAkKBTVcxAkdBKjixNiQIkNFp = 40453068; 
if(fAkKBTVcxAkdBKjixNiQIkNFp == 529757){
fAkKBTVcxAkdBKjixNiQIkNFp = fAkKBTVcxAkdBKjixNiQIkNFp + 641093;
}short MHnVXjtGeFEUagUm = -6421; 
uint QNjVMHuLzcfUS = 579354080; 
ulong WmzE = 26376665140589906; 
uint DdlCMDMCoDAVDZLLbxs = 334284280; 
sbyte RbCHoDXfbXxERGqSkpuYVeUIEUzJS =  -49; 
long ueuBkVxZsy = 67371428004685594; 
sbyte XjdNhTdwdomMogiMk =  79; 
int TwHyIWBgatX = 1727; 
while(TwHyIWBgatX == 1727){
TwHyIWBgatX = 20573;
}long qRbVckLIZnhUU = 72018220204184542; 
short uKqYDhPXLExpzSAEmWnCTTAdpqAQ = 10646; 
ushort iYUIkMmhBkpxqyItQ = 2252; 

}public void tjWRdCknJkJZopbnqEjoSZOOpExiG(){
long JOKcslgRfazFXRlagojqQGNgtwiCB = 81644755518878184; 
byte bjNentwRWeDoTxqicbDKX =  18; 
int aXQEKRawUGh = 6; 
if(aXQEKRawUGh == 762141){
aXQEKRawUGh = 950646;
}int xYQilKeXj = 56; 
if(xYQilKeXj == 589188){
xYQilKeXj = xYQilKeXj + 430674;
}sbyte ESgeegwnojf =  7; 
uint MtSjsjPFUGURspTiCHxijfyg = 8279; 
ushort WAYxHoRtfwdVCZYCkmPeufJAeuF = 41180; 
double RCOTIuIzjdABdgDCHkhWVe = -6.564529E-15; 
while(RCOTIuIzjdABdgDCHkhWVe == 6.381874E+24){
double kywEDgJqETNcMGTcCXENtA = -1.138295E-15;
RCOTIuIzjdABdgDCHkhWVe = Math.Round(kywEDgJqETNcMGTcCXENtA ,1,MidpointRounding.ToEven);
 bool? zqQEihWtncIEZuXUE = new bool?();
zqQEihWtncIEZuXUE = true;
}ulong qMyFRdJqBZLGftqR = 22068401976276782; 
long iNJbnHuxbDGdMVMGtFqDW = 14521383548745793; 
float AXgUVJALekJqPkqlbyjfqHeO = -4.127958E-09F; 
float XcRNjNffFbQXUTlqmHFG = -4.222304E-32F; 
uint BMATS = 6741; 
uint SOcuWgbfkIaHllyuOBFQlETBM = 4695; 
byte mkspgcfjX =  82; 
uint ASyCzoAcAeTdLFnVD = 70692611; 
long UYxJqejsjQjenMLxYnXYkNis = 30123984791718235; 
float GUaSgjpLonKZyTEAEuOKEhP = -4.968394E-08F; 
ulong wgCBEmBfQwfgNIByUJ = 63182578175037590; 
sbyte zeWyuGV =  16; 
ulong fNXONdymabdKUwgFVkwyGLe = 72079601373767328; 
double hDCkUyIHJSgfPjBqQLKip = -1.065484E-08; 
while(hDCkUyIHJSgfPjBqQLKip == 1.551832E-30){
double TogZNTFtaNNQXTXkS = -3953897;
hDCkUyIHJSgfPjBqQLKip = TogZNTFtaNNQXTXkS / 3;
try{

}catch(Exception ex){

}
}ulong fkddqFBNftUQHjiCVztFEHIjLWb = 49834172421643421; 
byte xnUdJfZM =  238; 
string HLwkHkqAEUtwDTpB = "tuaGXqAjABUBIzOSCaM"; 
byte UkpeHpdTqksGtsDdV =  169; 
ushort ahKxYVxFDbSEpdAjKBqbxqhpDVo = 7947; 
byte GssoCNBTgMUzppsOpoSstwss =  127; 
long uUdHotOwBlaDMJQtFhXaRXOGZnTuj = 39106123787644035; 
sbyte eCUSDoMfJDbNkDJhxf =  -11; 
int Btl = 242768; 
while(Btl == 242768){
Btl = Btl + 551649;
}float AETAXflpmhj = 4.400172E-27F; 
byte liFKUGzDFhJsocbggMMmIKjk =  79; 
float tUpCdm = -8.699051E-40F; 
uint SXLbanqoOWnlb = 11; 

}public void agDZuuZzeJoHFe(){
float ANnP = -7.95212E-22F; 
long TVCXTgSmxIlqyD = 38535680670177421; 
long elNoPpuPAhgzScFXQbjXT = 25616395676805890; 
short hlTSIuEygmySpx = 1417; 
int IYCXBMQFHRCZyGAjLYmYjWElFdL = 654852; 
while(IYCXBMQFHRCZyGAjLYmYjWElFdL == 654852){
IYCXBMQFHRCZyGAjLYmYjWElFdL = 178633;
}double pPAIAd = -3.518047E-37; 
while(pPAIAd == -0.003567705){
pPAIAd = Math.Sqrt(4);
object VVbjfwdzZLaXClwVAfjyJqKkMIh;
VVbjfwdzZLaXClwVAfjyJqKkMIh = -1.499554E-14;
Console.WriteLine(VVbjfwdzZLaXClwVAfjyJqKkMIh.ToString().ToLower());
}string WXHqsnncnVfgomFkQyzmULMsTxQxF = "uThANtdUtuUfBslak"; 
uint FEHPQdkk = 4497; 
double SzLBlZQAkhWeetO = -2.827201E-10; 
if(SzLBlZQAkhWeetO == 5.999502E-22){
double NesxCiPxyVbIPRZbwZayHDLyobShF = -1.541223E-22;
NesxCiPxyVbIPRZbwZayHDLyobShF = Math.Sqrt(3);
SzLBlZQAkhWeetO = NesxCiPxyVbIPRZbwZayHDLyobShF;
int? MNhItYctAHiqgwCg = 1467446;
MNhItYctAHiqgwCg += 92240;
}uint iGpbOU = 411182254; 
float DNNmyMMITxVWt = -1.33998E-18F; 
ushort pnzjoTOXUaGdTVZoICeIPEls = 32672; 
string jmmSN = "fBhxjnUYoELf"; 
int BWI = 91241157; 
while(BWI == 91241157){
BWI = BWI + 749541;
}string KlOFLI = "TLOwSSy"; 
short KAZBlVcF = -14802; 
int XEFeOfpAbfuEVDmUl = 8; 
if(XEFeOfpAbfuEVDmUl == 635226){
XEFeOfpAbfuEVDmUl += 960669;
}string NCUPjjpfflofJHMD = "mfpVeXYbQzjQopGBFaFwLRWdyQqY"; 
ulong zslouSOFJEWNuLfpRV = 41381161810585586; 
byte ynHamlaSE =  252; 
double MZsaAXxkYhYwoiASjdcXJDh = -1.141354E+33; 
double ZVTengJoZYmppkMeMiJAXYeiS = Math.Log(1000, 10);
MZsaAXxkYhYwoiASjdcXJDh = ZVTengJoZYmppkMeMiJAXYeiS;
for( ; ;) {
Console.WriteLine(1.124723E-32);
}int kawUa = 349890352; 
while(kawUa == 349890352){
kawUa += 955126;
}sbyte muCGhma =  -50; 
float kNXWWRHeX = 8.06352E-28F; 
long xQl = 59590618629035747; 
double QjisEPpPjgdXw = 1.580312E-09; 
QjisEPpPjgdXw = Math.Ceiling(Math.Acos(0));
Console.WriteLine(QjisEPpPjgdXw.ToString());ushort hiaEHCpcKdiigPMTpDxH = 31413; 
short GZkURo = -2959; 
byte NFpqQnqsl =  225; 
sbyte CAbsgmzNGQi =  74; 
string ExHzOlRXwDzwc = "lduF"; 
byte eAiCLbYBGXxBpwKVLOfVAFeAU =  186; 
byte CCwEEjpemfONmq =  84; 
uint XeIspt = 48687406; 
short AwKIYSDYeCSX = -25077; 

}public void GJGat(){
long mHtWHDe = 41879483740126871; 
int PULsmWnaYDMIOHf = 18816142; 
while(PULsmWnaYDMIOHf == 18816142){
PULsmWnaYDMIOHf = PULsmWnaYDMIOHf + 721022;
}short yOVNOLnnXeh = -18760; 
uint QaKeWJZtEuiYFIEUmMyFnEqiBoLo = 55574909; 
int ILtmelPJqdkCFP = 61; 
if(ILtmelPJqdkCFP == 849124){
ILtmelPJqdkCFP += 410687;
}ulong KlJCFHYLFaONqToqPaWPnjtQFL = 7489412443739824; 
string EEIiLG = "jNRzmqtnRzYGBSTX"; 
float TgTpDeEShNltOdI = 6.858709E-25F; 
string lWNkyTMJgBnLHOMRAGUjMVu = "HczLVYP"; 
long kIBfWwxQxxsTcGFcmheoSaqD = 10523271508409908; 
int xgkLEjytpAcpExTJIRLtXjpYCsa = 77; 
if(xgkLEjytpAcpExTJIRLtXjpYCsa == 627124){
xgkLEjytpAcpExTJIRLtXjpYCsa += 274816;
}sbyte zRCEURz =  48; 
float oaYCtfsalXlQlilj = -2163592F; 
long wbxmuRfmQ = 53770218121195033; 
uint DVGcjYIFssLwUc = 9124865; 
byte LhCZfkKALqOWUhXifp =  204; 
double oNmNUegFz = 3.44441E-10; 
oNmNUegFz = Math.Ceiling(Math.Sinh(-5));
try{
int RGoEBwG = 2765073;
if(RGoEBwG == 80782){
RGoEBwG++;
}else{
RGoEBwG--;
Console.Write(RGoEBwG.ToString());
}
}catch(Exception ex){

}ulong SxBsMSxtOTJOtYTVOFTaLUpkIWJKm = 29242283633427250; 
sbyte SyiQkcKCYMuRKWPnYfoKPiMfS =  62; 
float puT = -2.789776E+13F; 
ulong EDe = 51961651015171848; 
ushort WPGTBwbJLcXzkIkFSohWVlLUah = 47147; 
ushort OlOpZeCOaFnNtSBg = 41979; 
double kefFXBXLdMT = -1.493355E+14; 
while(kefFXBXLdMT == -1.383794E+34){
kefFXBXLdMT = Math.Pow(5, 2);
Console.WriteLine(kefFXBXLdMT.ToString());
}uint esABQUyIIfu = 74778587; 
uint dGdIcoHxGCEhDFqCDxN = 572; 
float iGLHQIVwHSEe = 1.200857E+13F; 
string byWTYlMVcpERqZ = "EmnVbqIROL"; 
sbyte pZBNSAaMOgXtXUQEAedeHEaSqObe =  -35; 
int iTnxWDE = 34060720; 
while(iTnxWDE == 34060720){
iTnxWDE += 427905;
}short XhCdqKHaqMZJxdyWtgZPBKyTbVt = 15183; 
ulong cEDxRUDoozCSX = 49252637230869673; 
ushort BunKIcwpVlXJwf = 16; 
byte tdSBmIMOWhOMSuAKgc =  9; 
int UmThHGaa = 761668; 
while(UmThHGaa == 761668){
UmThHGaa = 25510;
}
}
} public class CWmMCWMUIFdSyZndYKtzRYtH{public void MNuu(){
int CtKhkPpbmisl = 563628; 
if(CtKhkPpbmisl == 20820){
CtKhkPpbmisl = 266907;
}long KXDZDDH = 30143382517068791; 
uint RdoIJgozNQdnjMgLxIEQKFI = 60; 
byte SSjpb =  75; 
ulong juCbRmB = 49770696729410409; 
int YJdlxgcxcyGDAcYwDW = 9136; 
if(YJdlxgcxcyGDAcYwDW == 554922){
YJdlxgcxcyGDAcYwDW = YJdlxgcxcyGDAcYwDW + 182977;
}ulong FiHHIdGMdHKXOmzFPQUmmCzjUnA = 248503061874896; 
float wQDXOYqMGhcxbYNoDIdLxMtE = 2.556087E+20F; 
float PAmMGPtZspADgzog = -2.045407E-21F; 
double tGVTyhtJy = 8.922377E-38; 
tGVTyhtJy = Math.Truncate(tGVTyhtJy);
int? jFtnywLD = 7327957;
jFtnywLD += 91843;int lkQqiAJNCBdtR = 221026817; 
if(lkQqiAJNCBdtR == 644475){
lkQqiAJNCBdtR = lkQqiAJNCBdtR + 119506;
}byte nadogaDBWIsUaz =  84; 
int qWIPILbcSMtfLePoXXkNbbAflGTLA = 423721406; 
if(qWIPILbcSMtfLePoXXkNbbAflGTLA == 986778){
qWIPILbcSMtfLePoXXkNbbAflGTLA += 312173;
}byte IGobmoojwkDDFdBgPWp =  112; 
uint cSqDboAeMOGJosVMPN = 9230; 
float phUNVOlcaNdeD = -1.076049E+15F; 
long ZKpzPXVOi = 44118614814223848; 
uint IouMTJO = 599041369; 
byte zbfBklCWCkZZMDhWyBytUVDb =  245; 
int xQaepEtMbbcbukMaXJoIiLNpqhRVE = 85965; 
if(xQaepEtMbbcbukMaXJoIiLNpqhRVE == 253789){
xQaepEtMbbcbukMaXJoIiLNpqhRVE = 86085;
}sbyte fJxtRfVwAdihUFsJ =  111; 
ulong dLOiNzhDWBwggHASZToqFR = 26299524142918962; 
float xtcQsBKVJEYVdWcCwNsQI = -1.892451E-31F; 
byte VeeLAIkHMLNEKAAjuIbTZOhG =  54; 
byte tcbXmRZ =  110; 
ushort jjVOnsloXMmTHnResRh = 60697; 
uint kBRFLNANKcdFjzge = 42607353; 
double mCVAxNgizio = -4.545809E-32; 
mCVAxNgizio = Math.Ceiling(Math.Tan(1));
Console.Write(mCVAxNgizio.ToString());sbyte WCIji =  100; 
long pTfeykcBejVDAgGcgZoyReT = 70060337588618009; 
double OsZtRdkjcAKpDFPVFcjGjWkZfZzE = -1.117479E-23; 
double KpVdnOGiKDwSdMiWoORh = 2.75302E+16;
OsZtRdkjcAKpDFPVFcjGjWkZfZzE = KpVdnOGiKDwSdMiWoORh / 3;
try{
int IfSHHpXSaoVNnDWiFQx = 1622422;
if(IfSHHpXSaoVNnDWiFQx == 92582){
IfSHHpXSaoVNnDWiFQx++;
}else{
IfSHHpXSaoVNnDWiFQx--;
}
}catch(Exception ex){

}double tGDSOQxINRJDiQwDX = -3.81906E-10; 
tGDSOQxINRJDiQwDX = Math.Ceiling(Math.Acos(0));
object dDadPAEzfsiKFdxW;
dDadPAEzfsiKFdxW = 1.430674E-33;
Console.WriteLine(dDadPAEzfsiKFdxW.ToString().ToLower());ushort OhDSANulRpditRjUAZCVtNqMAme = 45092; 
long HkmkYgJaKxpzS = 48522719632782128; 
sbyte zCbRQslIKC =  -19; 

}public void TVhSclAoeGVVbGuoZzyFD(){
double JXpSIzOGWxHmjQmibqQiTHgP = -1.935439E+35; 
JXpSIzOGWxHmjQmibqQiTHgP = Math.Pow(5, 2);
try{
int ApcqsxIBZRTIItROzXV = 7471634;
if(ApcqsxIBZRTIItROzXV == 74480){
ApcqsxIBZRTIItROzXV++;
}else{
ApcqsxIBZRTIItROzXV--;
}
}catch(Exception ex){

}double wiiQ = 6.624406E-34; 
if(wiiQ == -4.845087E-27){
wiiQ = Math.Pow(5, 2);

}long peAuezFUsgzqVUABJKPqaIsMwR = 1130179413786082; 
string ediJTapNUUemZZzSbpSI = "OZnkbdJPTnNURxFCfzZqFRC"; 
ulong FRkAEMpsVqdVyRXHXbGsthETqWSTq = 64311826088682556; 
short DxCVfCHIHcK = 9085; 
ulong uHxnshIUybOuGtluKiIMcWRlyqK = 88059714480302597; 
int Zgh = 66959264; 
if(Zgh == 152673){
Zgh = 729380;
}double xlsCZeSyPj = 1.45763E-38; 
while(xlsCZeSyPj == -2.210609E+12){
double clHtLcAcuXq = 5.084886E-10;
xlsCZeSyPj = Math.Round(clHtLcAcuXq ,1,MidpointRounding.ToEven);
Console.ReadKey();
}string bCKinNjqCRDCWQOuGk = "lCucfQHa"; 
string jsMeCINpi = "hoDBYpgDKmdUlVScIAEtHMhesz"; 
float uhadytcXZUdQKe = -1.752031E+21F; 
sbyte gQmzanNPlwg =  -15; 
sbyte AKiDlayTSUcNwOKD =  -21; 
ushort cqajVhNqIhIRDLEpZWODiSmXswmS = 64177; 
sbyte ZNBgiABmuAKPMkdBbRyKH =  38; 
float jecSAUUsDzJdbFwRAJtdzdC = 8.561336E-19F; 
double aCwcqWNX = -2.562691E-05; 
while(aCwcqWNX == 0.9783823){
double DazyxHkXnOo = 7.688803E-14;
aCwcqWNX = Math.Round(DazyxHkXnOo ,1,MidpointRounding.ToEven);
try{
Console.WriteLine(aCwcqWNX.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}sbyte hRLBf =  86; 
byte jPemuawohnoeqhVoWWpoZRRQmN =  198; 
string HMxZumZAIQeITpDlcNaWwFi = "WTtYoaHlsBBDTfWVV"; 
ulong flHgBnEkZO = 18806636022603012; 
string IjSosPZt = "ReaGpdooNSKMQHylhzaXbxbUFHWt"; 
short LojniKfNMAGONbqASDuckpZ = -5324; 
float cfaJhVXZgwCDy = 6.747199E-30F; 
ushort CsmJaDfPtYoEwoOsOHaNcqK = 54120; 
ulong WXofHWREzdIMcEUUPXJwMfOjJgf = 10562028399204771; 
sbyte mssNRRLLbExShDNh =  -37; 
sbyte jfxAQjSnwkSXXMuCFqJUnczHE =  45; 
ulong HVnCWQZtQSCEYEpAj = 28840331838040951; 
short gIQWgupHAWqagNaLM = 19577; 
sbyte LNYyWjJPGOxSJnSMaWuiJWQESGt =  -34; 
ulong wbEsRtJsZXL = 1797572788977223; 
short QwXmPWwFqRpbpswopyAcI = 27709; 
ushort gVedtnAzzGGHGfb = 28537; 

}public void NJgwALtekb(){
ushort lcUZAfUSslcqRE = 45211; 
ulong JkwcPVmocHjLYilcdlELHgZh = 48005243869225966; 
long CRbtxgIKxzK = 29499908013941282; 
string ClnyUt = "AzLheIHubLQxdXLByzCy"; 
ulong RFVGiaGOVDOajXOC = 26542811437856570; 
double RLQYsVSY = 5.952891E-29; 
if(RLQYsVSY == -0.003320509){
double nJcNNNVxPaqnOclIYiUXWjSh = 1.138202E+21;
RLQYsVSY = Math.Floor(nJcNNNVxPaqnOclIYiUXWjSh);
for( ; ;) {
Console.WriteLine(-1.00478E-15);
}
}ulong fWduHWIBxDnNMuOJu = 83448033053716633; 
string SxsnQeZsYmzyUmbaEtgQPU = "gHaMGzdQLGkQdenyINc"; 
ulong VLpNUGoUDjWXQk = 16342691719715013; 
float yXVoVzdNeklVKZMTPVhxn = 2.485712E-18F; 
float IptiVYhUHpWKZZegiSGZ = 7.244928E-31F; 
ushort ZfCGwwFKbJnhKFildbdJ = 52169; 
sbyte WdofaJVVCXMiPxCzkLmJpLWx =  22; 
long VKbKSRpGCfpF = 42587210561247398; 
ulong ESYDBAkhTBppZkBmT = 35090670999499653; 
uint YLyjpDtgiMpqRuumP = 7158; 
sbyte lXfcUZCkXSqYWmxwqa =  63; 
int qCEb = 42270385; 
if(qCEb == 116878){
qCEb += 843038;
}uint ZMOsgwERWlTYs = 153622962; 
double znYBAVwDbmEWfRaQDWBJcjFjP = -1.393028E-31; 
while(znYBAVwDbmEWfRaQDWBJcjFjP == 5.865898E+12){
znYBAVwDbmEWfRaQDWBJcjFjP = Math.Ceiling(Math.Tan(1));
Console.ReadKey();
}ushort OihbFROJKGXkyUgCYYCilFRafYD = 31005; 
long WwPBBAnIMwqOBnuAsCNmH = 38391325167662128; 
int oXaJCxWuHn = 2281; 
while(oXaJCxWuHn == 2281){
oXaJCxWuHn = oXaJCxWuHn + 79132;
}int HGbMzcDAFRphgNfgbQA = 85719599; 
while(HGbMzcDAFRphgNfgbQA == 85719599){
HGbMzcDAFRphgNfgbQA = 103233;
}double hPskfOCobnD = -8.147899E+34; 
if(hPskfOCobnD == -2.852537E+26){
hPskfOCobnD = Math.Ceiling(Math.Asin(0.2));
try{
int AMuLlHkuFyeSAsCzQkqc = 999407;
if(AMuLlHkuFyeSAsCzQkqc == 55779){
AMuLlHkuFyeSAsCzQkqc++;
}else{
AMuLlHkuFyeSAsCzQkqc--;
Console.Write(AMuLlHkuFyeSAsCzQkqc.ToString());
}
}catch(Exception ex){

}
}sbyte hqp =  -111; 
uint tSSaIasVKdKDQNmzDtoMAFTiCoFa = 20886821; 
byte qSsZCWpluwWujDN =  99; 
ulong PWPOyOYJAtwqozRllYumX = 69918912431098693; 
double klXxpJEuFaTyWRdgy = 1.61607E+33; 
if(klXxpJEuFaTyWRdgy == 1.372939E-38){
klXxpJEuFaTyWRdgy = Math.Pow(5, 2);
 bool? LknyVhazB = new bool?();
LknyVhazB = true;
}short YMCiyUHHRfaugOdIBEqM = 25484; 
byte FohQ =  208; 
long BZw = 35656517024297519; 
int YGEJH = 70; 
while(YGEJH == 70){
YGEJH = 564694;
}ulong KWUoeQWRFaZEzsnDaNSpge = 26950217318980036; 

}public void SOEazd(){
byte dHotFFehIyGGfLyF =  153; 
double PVZXYLT = -1.091779E+23; 
double KSLufwbiCmnbgqYwCVzFdJpI = -6.222424E+17;
PVZXYLT = Math.Round(KSLufwbiCmnbgqYwCVzFdJpI ,1,MidpointRounding.ToEven);
try{
Console.WriteLine(PVZXYLT.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}uint JIgN = 887880546; 
sbyte ikDlCZSeLmmFk =  93; 
ushort sutEFikOkQTwmgYCGcJPWKuo = 47398; 
sbyte zNXTQkquzBCPaaPaMfxSFnLYhjJTJ =  126; 
string xjMamXdkVMAbenJPgJJioqujbZ = "GDyMzxKtQnONOclWSPKUTC"; 
float aqVEeQDZnlyIzaaFwUSCtV = 9.757843E-30F; 
string CwqfSeJMLhBfn = "LPA"; 
int EzSOfAZJsteyPBKnhIi = 3605; 
if(EzSOfAZJsteyPBKnhIi == 281138){
EzSOfAZJsteyPBKnhIi = EzSOfAZJsteyPBKnhIi + 867793;
}string GSQExqBAXJ = "lGSxqsHKLGFMEhpimFM"; 
int aktQDPJfobNFCwDCWJuIitJbgmx = 16; 
while(aktQDPJfobNFCwDCWJuIitJbgmx == 16){
aktQDPJfobNFCwDCWJuIitJbgmx = aktQDPJfobNFCwDCWJuIitJbgmx + 103483;
}long wAGPEXFxF = 19342217427937199; 
short MICsJptEYagCAQigbqURLFXeiaj = 11549; 
short YYTjYEUhWAjuGxyp = 8468; 
byte HHYcCkhLyiJfKlzbiJ =  79; 
float mUuwHpYCHStYfLTZWsY = 5.239801E+28F; 
ushort mtAihuoIpuYtMEHUViiQpogFBFbmq = 22293; 
string RWhbghQgskcEhshxoydIKH = "YmVIEscqYulDQxGONtcy"; 
sbyte EyixGXUns =  -121; 
byte iOkAlPpTdi =  230; 
ulong ZJJpoIiDMKzA = 15229943043974629; 
uint RDngmhlhu = 360398; 
sbyte nkYctbMyeNZedlxtgKzP =  58; 
float biwapE = -2.757801E+32F; 
sbyte eQmM =  126; 
float QQMQnbiWQPn = 3.530125E+30F; 
short xdWUYbwUQQScYwiHgShYWfqOhle = -540; 
ulong elM = 57530909560426611; 
double ZwYkBHhhZNqYqNiMMWWePMHqObaR = -8.400039E+37; 
double bFSHTeAwl = -2.399872E-12;
ZwYkBHhhZNqYqNiMMWWePMHqObaR = Math.Ceiling(bFSHTeAwl);
int? gszRgmPIxzHqhZ = 5335770;
gszRgmPIxzHqhZ += 59715;sbyte pLeHJnTbQ =  80; 
ushort wtk = 38902; 
long UzchBgqHOgonbflSgkDAln = 71860052511501008; 
float UPLIjikYpbAMMDPp = 34991.19F; 
byte ciBGqzOLKQWXpPuluFIYaFjkPuSQb =  33; 

}public void hFSzLofpLwAnAeDxXhDViVn(){
double YoRWHXJDcPszpKW = -1.14549E-17; 
YoRWHXJDcPszpKW = Math.Pow(double.NegativeInfinity, 2);
try{
Console.WriteLine(YoRWHXJDcPszpKW.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}ulong ZnkS = 1594814091128068; 
long shGtZNhtqCKGZ = 26344460555407411; 
uint tMOnw = 42; 
uint qBCYdl = 562; 
float XJiaLkbHquxhwUdmHlz = 8.161951E-25F; 
float Dnz = 4.098975E+08F; 
long QqHuBwkMcdt = 77926803616419565; 
float aEhwcIGMlIwWbEeBat = 7.565072E-36F; 
float wtHQVSf = -7.436713E+29F; 
ulong LQNAYzNFaQwYHcCGIeolPKRiOP = 82073685604444532; 
short qbcVVyUsawZNtlymGAebZFL = -21564; 
byte SuW =  165; 
short NOnHcuqqPzitQztCiH = -23248; 
byte oONpTIUGBwsgxaFXQklhNlBxqtOHp =  53; 
short eFyaCpiMxMocQmuy = 17993; 
int xVblcnqYBUno = 1197; 
if(xVblcnqYBUno == 840785){
xVblcnqYBUno += 554344;
}int AbBLbWyGeZqTMS = 591701972; 
while(AbBLbWyGeZqTMS == 591701972){
AbBLbWyGeZqTMS = 732680;
}byte JSixaWXlCUih =  223; 
ushort pkgRLUCEwDymm = 63009; 
uint TlhsJDF = 510852586; 
long yxKFYyjNhnVAyhoOUEKKncNpVM = 67142913534542097; 
string MXAPdk = "UXNLkuOLsRYG"; 
ushort tJmwyZCcGxxdhcWdUHcLT = 23940; 
sbyte SsZ =  106; 
float ewKysoluGlQRspnh = -3.654497E+16F; 
ulong WsDJQaBlDf = 83594882183007754; 
long SAaRVbaTNKengEDNSaHs = 59120776794790791; 
double OEbgxxqMO = 6.081353E+36; 
while(OEbgxxqMO == -8.93054E+26){
OEbgxxqMO = Math.Ceiling(Math.Asin(0.2));
Console.ReadKey();
}sbyte YMpLcbYx =  -95; 
int AbEMCqGXiziuD = 97; 
while(AbEMCqGXiziuD == 97){
AbEMCqGXiziuD += 203650;
}sbyte ppLC =  -53; 
string geXdhFzzTMXfFdWLPsNKdWK = "WxsjLTklnSReCflYCClCCeDZtGWj"; 
uint KkwbgMloOaCMS = 25207092; 
float JfPiDLiu = 5.364446E+28F; 

}
} public class QfWaKUiHJBZB{public void mNkaQlRMCwmeNkhAmHoSTz(){
long BbXDH = 25891473541865344; 
sbyte FbHoKKWWJbgxTKJSOe =  -95; 
uint ooesLEALptZecKQmjYuFeoCA = 81533438; 
string jgKtJPpNCXYgxltIhsRZSGFgNVL = "NGGVDFBiKGxmucLmZnhSVTsuZbcFD"; 
int OepaWqdESMjYCWEGQUs = 47; 
if(OepaWqdESMjYCWEGQUs == 83697){
OepaWqdESMjYCWEGQUs = OepaWqdESMjYCWEGQUs + 875870;
}byte VCCVHgZI =  41; 
string PVWK = "gMURzwxmKjw"; 
sbyte llg =  47; 
sbyte cqQESCn =  -22; 
uint fykPxMtYwNBfnlyLSIWRWeP = 64560614; 
double icpjZUBENjt = 1.314518; 
icpjZUBENjt = Math.Sqrt(4);
int OlgeJ = 1817; 
if(OlgeJ == 720496){
OlgeJ += 310755;
}double GjEeOckmoxCLhZ = -1.006368E+36; 
if(GjEeOckmoxCLhZ == -3.436274E-34){
double WqoDHFbZGOH = Math.Log(1000, 10);
GjEeOckmoxCLhZ = WqoDHFbZGOH;
int[] wIsRzPFuBlLReJm = { 7687740 , 32338 } ;
Random QSWINzAFInyWXCKcZGIwMPoAqX= new Random();
Console.WriteLine(wIsRzPFuBlLReJm[QSWINzAFInyWXCKcZGIwMPoAqX.Next(0,2)]);
}double XpsUQzAsIWhNijqJxYDIRzb = 6.917785E-29; 
if(XpsUQzAsIWhNijqJxYDIRzb == 5.604147E-05){
double JUqgEcMlD = Math.IEEERemainder(3, 4);
XpsUQzAsIWhNijqJxYDIRzb = JUqgEcMlD;
 bool? ZGfhIBPPnWz = new bool?();
ZGfhIBPPnWz = true;
}ushort pekApFhwy = 19839; 
long VuuXSScoWpC = 40625796744549792; 
uint CziMZZ = 611380216; 
ushort MuBpGLsasdeqaEG = 23436; 
ulong neksgjtWlkT = 53775316238216371; 
byte jYlGzbkzFfHjFsplunpW =  173; 
uint hAXwQg = 41626737; 
ulong JefZMIiizHfmJCqkHuW = 77175693116379739; 
int qQKUEVmjmJGhO = 8505; 
while(qQKUEVmjmJGhO == 8505){
qQKUEVmjmJGhO = qQKUEVmjmJGhO + 421657;
}string BuzxsKunUGhCOgRwsf = "dbtMqhBNSRjoeDRRwky"; 
byte TWtdyHZe =  7; 
int LQZDgJhoSWCyKpcYW = 4142; 
while(LQZDgJhoSWCyKpcYW == 4142){
LQZDgJhoSWCyKpcYW = LQZDgJhoSWCyKpcYW + 389067;
}byte aITCB =  172; 
float IEoEnKMd = -1.924745E-11F; 
short AJOwIdOGwIRx = 18482; 
uint anOsnL = 448; 
long wJGJfMCfTGsUzTyVkPaNSPQ = 15067101690980966; 
int Ilu = 92; 
while(Ilu == 92){
Ilu = Ilu + 316566;
}double OfPUVtBxcYVGTbXoqCemsLTRH = -2.042734E+18; 
if(OfPUVtBxcYVGTbXoqCemsLTRH == -7.096887E-34){
OfPUVtBxcYVGTbXoqCemsLTRH = Math.Exp(2);
object NWmsoQwxgDgPea;
NWmsoQwxgDgPea = 7.530615E+34;
}int HMPsQDXWyTlpaWzzQcCzBZlCiu = 6842; 
if(HMPsQDXWyTlpaWzzQcCzBZlCiu == 729906){
HMPsQDXWyTlpaWzzQcCzBZlCiu += 140502;
}double JNKsBIBQUicXbYuiMpsgtH = -5.416365E-21; 
if(JNKsBIBQUicXbYuiMpsgtH == -5.126093E-15){
double esomMOzD = -3.645572E-06;
JNKsBIBQUicXbYuiMpsgtH = Math.Round(esomMOzD);
 bool? cAktnoq = new bool?();
cAktnoq = true;
}
}public void nOyjJCxOYccYe(){
long oCWkzqpyDKqwbCGHEIXlIs = 82393354911410914; 
short ZpJeQAjcXp = 27188; 
uint fwlswAqhzdXT = 6877; 
long gunucxMTCQdyUosxhswux = 38977745185388337; 
byte QCuRWIeoKKtRGxTmjM =  133; 
ulong nnmoNzxVVAhqlnWBzwOcdMLcabt = 72122267467273945; 
byte CiiJtO =  52; 
byte thWNLpNJRDDjoLfp =  10; 
int XSENJtXlFuKXBRBJkjgGjyQRYLXlz = 35; 
while(XSENJtXlFuKXBRBJkjgGjyQRYLXlz == 35){
XSENJtXlFuKXBRBJkjgGjyQRYLXlz += 277911;
}long wmuOdhohGctObUgVYOAcMOYw = 8660377752684542; 
float QWMSgoBKtIFyxjkwlyPFdIoYdp = 1.188137E-19F; 
ulong OQViekjfIkCbwpRnqqQYBROoPi = 72971095438222226; 
float DmfalfPFcclYgQYpyQsXcGu = 54986.47F; 
ushort SDofVxdLdkQN = 25419; 
string mNWJHYRs = "ghXMXTjyQOXbNWqDdeVeIcYV"; 
float IZUPtajzYOAq = 2.874852E-23F; 
string WGehAQGgEUWm = "dibTFlj"; 
int wWS = 9628; 
if(wWS == 730768){
wWS += 318317;
}long ogsQ = 25170880669430227; 
double VFyzjZRPVki = -2.053692E-23; 
double UXQliLuQQ = 87115.98;
VFyzjZRPVki = Math.Round(UXQliLuQQ ,1,MidpointRounding.ToEven);
try{
int ZcYGngLmAkhIuJZqJuVROxYOloz = 513507;
if(ZcYGngLmAkhIuJZqJuVROxYOloz == 38963){
ZcYGngLmAkhIuJZqJuVROxYOloz++;
}else{
ZcYGngLmAkhIuJZqJuVROxYOloz--;
}
}catch(Exception ex){

}uint waagDe = 37; 
double glCeTsZxkmjRzpRblfIB = -4.059135E-24; 
while(glCeTsZxkmjRzpRblfIB == 2.985939E+15){
double KUeEPwKTHPlPGikXFCKpC = -2.08343E-16;
glCeTsZxkmjRzpRblfIB = Math.Round(KUeEPwKTHPlPGikXFCKpC ,1,MidpointRounding.ToEven);
 bool? lVKKuGcMOBsemKVh = new bool?();
lVKKuGcMOBsemKVh = true;
}double RQufDA = 2.054834E-25; 
if(RQufDA == -1.079063E-24){
RQufDA = Math.Ceiling(Math.Tanh(0.1));
Console.Write(RQufDA.ToString());
}ulong kYqgjKtasVG = 71361854311865699; 
int RgzHQaRCHuSBsOwgUFjyPyIzclKd = 59435417; 
while(RgzHQaRCHuSBsOwgUFjyPyIzclKd == 59435417){
RgzHQaRCHuSBsOwgUFjyPyIzclKd += 338402;
}ushort SBuKCAMCNDNJgPUUPjiuAaEEq = 49525; 
float VDu = -1.444934E-08F; 
int tkBVoLfLBA = 93; 
if(tkBVoLfLBA == 599867){
tkBVoLfLBA = tkBVoLfLBA + 521090;
}double ZsSXQtP = 1.846327E+14; 
while(ZsSXQtP == -4.856491E-28){
ZsSXQtP = Math.Ceiling(Math.Sinh(-5));
Console.WriteLine(ZsSXQtP.ToString());
}long TqddngimIUPLsFwCftPSEfmUDJ = 70222542373002110; 
float qgIg = -9.165694E-38F; 
long NityE = 21134484272677397; 
sbyte GaatZLyaANsjZLIuD =  -13; 
sbyte toIFJu =  -73; 
double DeZhGcfyJItpiULkbnlMxOpVchp = -2.012915E-06; 
DeZhGcfyJItpiULkbnlMxOpVchp = Math.Sqrt(4);
Console.ReadLine();
}public void VRVWNVZhzUBJlwbYUnXTCSpYUEt(){
int OjEaZJsFYOklPx = 544749; 
while(OjEaZJsFYOklPx == 544749){
OjEaZJsFYOklPx += 239855;
}long GqdudBWbiD = 69863992868280910; 
long VBNINwOxApmpO = 81972409244913493; 
double IPdMRFNatEnufFxIINB = 1.477699E+26; 
if(IPdMRFNatEnufFxIINB == -7.682059E+33){
double IgRMYVfSJJWRNpUNBezIAX = Math.IEEERemainder(3, 4);
IPdMRFNatEnufFxIINB = IgRMYVfSJJWRNpUNBezIAX;
Console.Write(IPdMRFNatEnufFxIINB.ToString());
}uint kXMXIaY = 27713358; 
sbyte YYoyHjWoKtdPQJcZXSbLwxQFMcG =  67; 
short ztUpLkygVYSUSQh = -22383; 
double FbaRmzADhK = 1.479711E+07; 
while(FbaRmzADhK == -1.116601E+32){
double AXSDQWcTBFayEfsisyHgxRicNwq = Math.IEEERemainder(3, 4);
FbaRmzADhK = AXSDQWcTBFayEfsisyHgxRicNwq;
int[] UymypNzkUjGYXhmhAlZF = { 8449409 , 22710 } ;
Random nEzIPQQuGJiCYfZpJbxTmMXR= new Random();
Console.WriteLine(UymypNzkUjGYXhmhAlZF[nEzIPQQuGJiCYfZpJbxTmMXR.Next(0,2)]);
}float nRiLuSHUPPCXIQoC = -8.268388E-06F; 
double nRR = -78922.88; 
double VpgOAzgfAeCTBzkyyw = 3.432016E+10;
nRR = VpgOAzgfAeCTBzkyyw * 2;
try{
Console.WriteLine(nRR.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}ushort OXuxeusdgdnckoacUDIaoZNAHS = 6851; 
sbyte WcgIHpgQQpW =  45; 
double KmomzuOzsUwEOjUQlo = -1.031051E+19; 
if(KmomzuOzsUwEOjUQlo == -1.536366E-27){
double ldmMqoxcmsGHMDyKlmcCWWadIzl = -888597.3;
ldmMqoxcmsGHMDyKlmcCWWadIzl = Math.Sqrt(3);
KmomzuOzsUwEOjUQlo = ldmMqoxcmsGHMDyKlmcCWWadIzl;

}string FdiO = "PnEyZYcH"; 
short MfcElqXd = -19653; 
byte NBZJCnjJMR =  251; 
ulong ibBLfVaxc = 58197713125393722; 
double DVbOEPyKTXGtNqQwjRlYokSLEnQzG = -3.597454E+34; 
if(DVbOEPyKTXGtNqQwjRlYokSLEnQzG == 1.289929E-37){
DVbOEPyKTXGtNqQwjRlYokSLEnQzG = Math.Pow(5, 2);
try{
int bJqdpjoYZdzZCBM = 495552;
if(bJqdpjoYZdzZCBM == 37375){
bJqdpjoYZdzZCBM++;
}else{
bJqdpjoYZdzZCBM--;
Console.Write(bJqdpjoYZdzZCBM.ToString());
}
}catch(Exception ex){

}
}short ppktPdIRsUmQeIWIgKAUdlwOYKZeK = 5533; 
short bjZJPLJWTKuklyRluKXtU = -7497; 
double Vxuf = -113398; 
if(Vxuf == 1.412572E+08){
Vxuf = Math.Truncate(Vxuf);
try{

}catch(Exception ex){

}
}short LcAypDyxAVVBpQGsyWIIO = 31603; 
sbyte asfnXcfoTWcyZHfgntNKYsFtYRHHQ =  33; 
double XByYxiAxPGHuOhsQQBuulx = -1.347901E+11; 
XByYxiAxPGHuOhsQQBuulx = Math.Ceiling(Math.Asin(0.2));
try{
int mCABAQjLTtZp = 2979221;
if(mCABAQjLTtZp == 52427){
mCABAQjLTtZp++;
}else{
mCABAQjLTtZp--;
}
}catch(Exception ex){

}double HWEgAHGJjZASZZtXZ = -0.567013; 
if(HWEgAHGJjZASZZtXZ == 8.101774E-21){
HWEgAHGJjZASZZtXZ = Math.Floor(-7.278122E+24);
for( ; ;) {
Console.WriteLine(1.316905E+10);
}
}double UWFDNVxITYZ = -0.00188613; 
if(UWFDNVxITYZ == 8.544162E+08){
double kuwqHLPw = -1.963456E+18;
kuwqHLPw = Math.Sqrt(3);
UWFDNVxITYZ = kuwqHLPw;
try{
Console.WriteLine(UWFDNVxITYZ.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}int FAnJlYknfSktcPCQXaoJpEyCCOWnG = 2954; 
while(FAnJlYknfSktcPCQXaoJpEyCCOWnG == 2954){
FAnJlYknfSktcPCQXaoJpEyCCOWnG = 680858;
}sbyte JOwqRgzoeZtKfe =  27; 
ulong JbbNhycXJUDkPafutnmUsAxZlx = 84554041089032516; 
ushort fKBpttimVdoZnoMQtSCi = 39012; 
string MJAsiOTXeGyoKOWC = "qidhgQooOpqwilddiPITbZAcgktRa"; 
string xqDR = "YsFTkduyDCWuul"; 
uint oIXqKbbWxzoLmUdGuIeRkPJXGbBGn = 439523819; 
ulong dgJGAMWqIxNPKkIzq = 81567609402716850; 
ulong CwJybBhjNuaUVhADsViV = 21161619367399881; 

}public void IKGOewVZZM(){
short aUdqeRZjlhdQQQJoM = -3380; 
short oUfLugQNFEtjVuQYdGUTJKdAGP = 29341; 
byte HmDuwlfOsWYalFIOTVxaKkf =  140; 
uint GRHeFjDZ = 319839138; 
float AhLkLKWSxewLMUBncwgZWkHn = -3.069047E-19F; 
short zmHqXewooABxHGOBxVtLJnXD = -20814; 
float oBxdyshtjydNIzlYEVLtqxgSmU = 1.584226E-30F; 
byte ehglgOTxcez =  61; 
int shLm = 302544; 
while(shLm == 302544){
shLm = 462036;
}uint uEQDMbiUtVHxqzfhSHUeRdAeiu = 89538303; 
double JJKntoGAsxdLKxB = -1.724295E+31; 
JJKntoGAsxdLKxB = Math.Ceiling(Math.Sinh(-5));
try{
Console.WriteLine(JJKntoGAsxdLKxB.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}ushort xFexUXbgATGFmlwTBQbEJ = 16587; 
byte luFTcsHTobmma =  203; 
ushort kNGhzk = 32803; 
uint JbcRRaiqMkBshdhmNokZiTLx = 378028458; 
float fVaUcdtpEbzbeVoFTOcJ = -4.776728E+29F; 
long oDHQqGVyfClQkGpeLSJLQiVmFgE = 6995318914681767; 
uint EYYfbWDKztmzXYF = 9945; 
sbyte ItlkbO =  -101; 
float bGfbQHgUKGPm = -40070.57F; 
sbyte qKumFYkZzRdsOXsWdlISnkI =  -48; 
float CupTpQfyLdMGNcwfDYEPfN = -2.460093E+27F; 
byte VmAfcd =  106; 
float QKGCmmqmnjyAFy = -1.20901E+13F; 
long cGPhyJjVSYFbIjTGGxWypCP = 55972071395054003; 
ushort psnIJlWssusI = 15539; 
uint GZSMNlxCQ = 1625; 
ulong DFbHhJqRFYWkZeYhxSNgAbqYGKgU = 67325193159619378; 
ushort AEkwbaVCNhzuuxxJyBndby = 34744; 
ushort gGx = 49899; 
double kOLsiWdapd = -1.758747E+34; 
kOLsiWdapd = Math.Pow(double.NegativeInfinity, 2);
object bDcKsZyYMzbpgiIfVE;
bDcKsZyYMzbpgiIfVE = -1.655242E+35;uint TRMhOpUBgHOfENubxIpwRL = 83968996; 
uint pjjipPHxOojxCUmJLOdLyuuATekfU = 19; 
float QaKqXqZAhYfSZfxhiZTO = 73.42418F; 
float KplECDGepPMTDsOxelpxnITY = 5.67893E+33F; 

}public void jpfSofTHIfwYFsaDktEGRk(){
ushort DlACcxmQFRxPjWyOJLfpK = 17468; 
long mfJG = 21354758980242025; 
int FlTGtOWlsnVP = 108160814; 
while(FlTGtOWlsnVP == 108160814){
FlTGtOWlsnVP += 291654;
}uint oXNIFmWzWNIuhq = 87; 
ushort UWgbN = 27030; 
long wPTNyaCkcHAkag = 72412022468730862; 
double JVwFnmjqY = -1.030497E+26; 
if(JVwFnmjqY == 8.119481E-26){
JVwFnmjqY = Math.Exp(2);
try{
Console.WriteLine(JVwFnmjqY.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}int zntVaBWNIRwFyHi = 79; 
if(zntVaBWNIRwFyHi == 721761){
zntVaBWNIRwFyHi += 294772;
}byte ZhzfdzLkJHBNkaUF =  82; 
float yKWljcQpBtiUikNuODwSnXG = 5.099192E+33F; 
double XfVpURCQRcVndnhjSBNsDyGgcW = -0.119112; 
while(XfVpURCQRcVndnhjSBNsDyGgcW == -1.42756E-24){
XfVpURCQRcVndnhjSBNsDyGgcW = Math.Floor(-1.143871E-20);
object HbUwXFQlKMhBnnOFxtOoXeISsL;
HbUwXFQlKMhBnnOFxtOoXeISsL = -5.009077E-17;
Console.WriteLine(HbUwXFQlKMhBnnOFxtOoXeISsL.ToString().ToLower());
}double ucdlNbWzhbsQcBhnk = 2.486665E-14; 
ucdlNbWzhbsQcBhnk = Math.Ceiling(Math.Asin(0.2));
float JNxKFDOnZ = -1.204438E-24F; 
double exALai = -2.85846E-12; 
while(exALai == 2.367076E-23){
exALai = Math.Floor(2.400347E+17);

}ulong JYZzCYKjgBzxlYdlNO = 71995608204454688; 
long pCMIZEUSMymXBGezuWhOc = 37976849077256822; 
ulong spbUFO = 62586495463621510; 
long zAnPubUGeNbJueQHWpV = 65732759155889190; 
long LAjoZbGWbMpxwPSMicEnEEZGc = 31718303297100097; 
ulong nStudjKVFFkdW = 52904002482986827; 
uint lSnHtPRPmHwEUPIAdT = 39726138; 
string dRqGSyFMBlOngDPEqfNQYyBed = "PytuAfDuOypNXukBfj"; 
ulong JfLBDDIkUGpmCnJhtgN = 54498358584652369; 
ushort yqqnyWKNTRWysBihdlJ = 47546; 
string YxhVWq = "mYtoGxnZYdsY"; 
ushort oQktmGjjsAHCptUg = 36302; 
double uGTDiMdUhehq = 4.346123E+12; 
while(uGTDiMdUhehq == 7.809821E+08){
uGTDiMdUhehq = Math.Ceiling(Math.Tanh(0.1));
try{
int IJnjapHkDnJCIWYdmHZfIOmtVU = 5955912;
if(IJnjapHkDnJCIWYdmHZfIOmtVU == 50121){
IJnjapHkDnJCIWYdmHZfIOmtVU++;
}else{
IJnjapHkDnJCIWYdmHZfIOmtVU--;
Console.Write(IJnjapHkDnJCIWYdmHZfIOmtVU.ToString());
}
}catch(Exception ex){

}
}double NYSEiOkUw = 1.128615E-17; 
if(NYSEiOkUw == 1.075169E+10){
NYSEiOkUw = Math.Exp(2);
 bool? pJLDPbNoyjItzqhTkDuXyasQ = new bool?();
pJLDPbNoyjItzqhTkDuXyasQ = true;
}ushort pHgAPCiJZOpop = 59855; 
sbyte fxfLxVlQCcjSdiDGHYAQNwCIEeO =  -32; 
short POlEeYMJWwwqYMqq = 6776; 
ulong KJqxqiLV = 36333990822829388; 
ushort AeETXbduxyPplAYhJRz = 22631; 
float udoCFmDTWAWKfL = 3.379077E+23F; 
long sWMnfgVTjK = 78555945928492680; 

}
} public class wyQSDBpuCtmBSgiZYJ{public void XhHMdT(){
sbyte LSYbagkoSCejNQNtAWiWdG =  -111; 
ulong EExPMNYyOFCEq = 13485422912745849; 
ushort wmlgKi = 43908; 
sbyte lIxDPAfPfMZkkQiLOoloGSUk =  -20; 
string AGnoeBNqKPRSskKPO = "WkxdxVTxQjuNl"; 
sbyte XHiRyULyAzVXyUdVdgRBRfw =  -84; 
short JKEWXELeuFBXqA = -19141; 
ulong FCYxDNnqGuHWRkazblbUDCqmsAzw = 79827366351508136; 
ushort fXoGqjdiEtBzomjSudjMecnSkQbqS = 44083; 
ushort ecfeFXkgiiWe = 59116; 
long eflONyIUdJxwBiMq = 33147550739796151; 
ushort dEVgiXEpWlGVIUyicwhDLYySbf = 48929; 
byte XaZpUueYXA =  123; 
float lBLZsmWd = -1.141812E+14F; 
string hefdeckpcYkHFNBtjfWJLCMXgcO = "qjTqUwNoYUTxnCgn"; 
short wFNEU = 32171; 
ulong kHtiXboLYTfUMi = 59733666934358637; 
ushort IbIjNNpTEAkgWPX = 54595; 
sbyte EnhwslFCiFNnabmVCV =  -33; 
sbyte kDlqmYToHswqSGfpmJxFlFLgoziTb =  -62; 
string cFsLVefnLlCmsWi = "gFnlXBNHSfbOPa"; 
byte pXonnuuiEhUZ =  15; 
uint spMNxZQBCRMTQcsALYiWuIVi = 20017168; 
sbyte PHOEtqKzcnRtAyNxDBFg =  -72; 
long bUCuKwE = 84602724980880701; 
uint tUxygQyXUzHlGYy = 44; 
ulong RXZcAnGzsMRsuafNqmMUd = 70263814769640252; 
long zqAJCuRXiCsekdfmlA = 36900887365427392; 
sbyte DCZCIh =  -35; 
string sFtBoDtMZlMQKBgMFLNAUWbkLwI = "QycKesUURFFizTNmy"; 
short aSQcyGCawDiQUURVCjcLqHlUZf = 5229; 
string iON = "DIKawGdEqzqJP"; 
long HXkduFBtHW = 12538017977240125; 
ushort lhVgEYhahUkk = 26391; 
short dXLsPboddtobc = -10353; 

}public void DmttbAbeB(){
ushort BkMLSWMfWodcHp = 62347; 
string OoWeIWqYRbnVcTjWsBdqtBspX = "WzjNdeujqlBsSo"; 
ushort aVzjUxUgnPTNzmWHhmaSOe = 62389; 
sbyte SCYwaJPWHByRNCAF =  -111; 
uint kILeoUkGnD = 916669076; 
double JbUVLngeRS = 0.05808335; 
double JYThyKCQcBuIFADdBFKs = Math.IEEERemainder(3, 4);
JbUVLngeRS = JYThyKCQcBuIFADdBFKs;
ulong uokkn = 851835040346946; 
long QSCKTMWU = 53841295195585461; 
double sQRObbFNeiJwpJHUdEyyUsyww = 0.4840708; 
if(sQRObbFNeiJwpJHUdEyyUsyww == 3.187022E-29){
sQRObbFNeiJwpJHUdEyyUsyww = Math.Floor(-1.051001E-20);
try{

}catch(Exception ex){

}
}double gEzjWmzdhinIlqLhBEfEkFRF = -218837.6; 
while(gEzjWmzdhinIlqLhBEfEkFRF == -2.403667E-37){
gEzjWmzdhinIlqLhBEfEkFRF = Math.Pow(2, 2.1);
try{
int VocsEkVQwLkIg = 505829;
if(VocsEkVQwLkIg == 96870){
VocsEkVQwLkIg++;
}else{
VocsEkVQwLkIg--;
Console.Write(VocsEkVQwLkIg.ToString());
}
}catch(Exception ex){

}
}string yfiaEXUXgFtzcDAAAcyAVX = "EEKDYcziJcPuYUlnBYckHuCzP"; 
short NaBFPAnGxTuWUIwKxmiCGmPXCS = 14892; 
byte clOGiLii =  116; 
double KnQufciEkKfkHRnAlEdWOdwtIHp = 8.950118E+33; 
if(KnQufciEkKfkHRnAlEdWOdwtIHp == 4.995816E+34){
double yuRAKiaAXagPV = -4.354585E-19;
KnQufciEkKfkHRnAlEdWOdwtIHp = yuRAKiaAXagPV * 2;
for( ; ;) {
Console.WriteLine(-1.467349E+33);
}
}short FjgkzaJsOmDcsQcOPXifTPQCEXae = -32402; 
byte UHkDOlOAbhuRZcaEGGLN =  217; 
int dPtGIBaF = 5759; 
if(dPtGIBaF == 431499){
dPtGIBaF = dPtGIBaF + 132458;
}byte CbzhXAYRzD =  94; 
ushort oyOqwgOwHgYaEcm = 23983; 
ushort yssjLtwnFQCgehHecg = 55177; 
short aAS = 25664; 
float gtxMzeZWbgfMwVhC = 3.661765E+28F; 
uint Ruz = 119795271; 
uint DyT = 296784; 
ulong oUiedbfkFYXwyxTzCYtTKMUyGX = 52932377991780333; 
int tTQAWGQBb = 8339904; 
if(tTQAWGQBb == 972906){
tTQAWGQBb = tTQAWGQBb + 343170;
}uint XjDaLAgYJMxphAVQHeIiUWdWtwU = 459367029; 
ulong RgUYsdmJuJBmUujU = 18569430203514191; 
string HHpGnB = "kGjnCEMtxnAtKjDiCJKJKHgPTXk"; 
ushort tOwaqJPaHKGdJyxSIqW = 5621; 
ushort XFSlEzwuFsoNdAetsBNiUYe = 39477; 
string LXlGosiF = "RifepGlfmOVtWNnZwqoTYBmku"; 
ulong iYdWFAInjlxyEJT = 70335963751496780; 
float ULT = -2.261772E+32F; 
float DhcgjRqToAeNZUMkfJwfDZ = -3.04116E-21F; 

}public void jVLPPWnqLUwG(){
ulong ZOYAKSUThhANICPiETQP = 41675025769331273; 
short pFubwqFAFtyqNDocoNDDpDQWIchTa = -23845; 
string FhSVtGfyEeoxsJj = "Pynd"; 
float JOPyusiLAyqYMFLuJJIeYl = -2.834851E-11F; 
int TfeDTYGSzdfVlNnTFRftWFswOGxON = 510452; 
while(TfeDTYGSzdfVlNnTFRftWFswOGxON == 510452){
TfeDTYGSzdfVlNnTFRftWFswOGxON = TfeDTYGSzdfVlNnTFRftWFswOGxON + 482894;
}string WFssTfHnNxUQfIwmldo = "KOmDSfbEKcRnjmImNaczLga"; 
sbyte XIPeiHqjqXiqQBOdd =  17; 
long HGTZhBkbRbDNeaUymzC = 13581927233901448; 
uint uIDpFVsOQW = 42574366; 
short ouwmQGqRFboeD = 15682; 
int lELNxuZxAeDJxDUZ = 6970; 
if(lELNxuZxAeDJxDUZ == 90363){
lELNxuZxAeDJxDUZ = lELNxuZxAeDJxDUZ + 686327;
}sbyte WDdCbAsGkqLdMDXVuYNLdIuchoU =  90; 
sbyte uDyQRTpzwZMXzfRQnINJ =  4; 
sbyte xRohbtNGByDWiGVMdFWENYgjokqa =  -71; 
sbyte gnGbZCMoeJulAVj =  43; 
byte WKeFEjyKeTQc =  185; 
ushort YwIDFVjqpRVtM = 41587; 
short CgIow = -6834; 
uint wEoySAcWZRBADfGyNsPOIR = 6128407; 
ulong nVdeZETWhbHNzQOYtUfPDxCk = 15132491981237879; 
long khTUR = 80061257743834730; 
double KyR = 4.192461E-38; 
if(KyR == 1.285829E-09){
KyR = Math.Sqrt(4);
try{

}catch(Exception ex){

}
}uint CZqDsfJhlYkpwWgBARFEggqT = 328001; 
sbyte XMm =  113; 
int bmCEOdCCWdsImNiUoqHWK = 654472; 
while(bmCEOdCCWdsImNiUoqHWK == 654472){
bmCEOdCCWdsImNiUoqHWK += 710;
}long BMnMMVTY = 14042617622198582; 
float sSdxW = -4.193705E+11F; 
double FeBMdTqxseWBSoVtmfT = -4.317399E-23; 
while(FeBMdTqxseWBSoVtmfT == 4.601494E-23){
double ePXdNonU = 5.021376E+11;
FeBMdTqxseWBSoVtmfT = ePXdNonU * 2;
Console.ReadLine();
}ulong cjqDhSODbbYWIK = 63600222851499223; 
double gNoS = 4.631342E-33; 
gNoS = Math.Pow(2, 2.1);
try{
Console.WriteLine(gNoS.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}float CTnERjtXDoYCwQOdbqpcjsgPEjQ = -1.133529E+25F; 
sbyte SmqKMWibOamIAWSJmuLqCRSKiMA =  -127; 
long iguugnegcgSoC = 68069224325802147; 
float SsbwHB = -0.0001217692F; 
sbyte uSfmeQEFgsEzm =  -54; 

}public void NzeHogRsaCceTzHcKKVhkRyE(){
string WxzDtf = "GUcVQTnDAfMcLKfAoUPqhF"; 
float BEh = -7.347984E-08F; 
float iWtnEFgTQIFLMMB = -1.795805E-05F; 
ulong iJEPFaZbumMdhHIDgqxCViaZKDTi = 57031775353602499; 
int PmFDMH = 56; 
while(PmFDMH == 56){
PmFDMH += 181138;
}ushort anb = 396; 
long ebgQMubaKJXTXYDiXs = 48605557767601093; 
int KeyKgbdN = 78; 
if(KeyKgbdN == 599444){
KeyKgbdN += 57389;
}float hjlwySqJDnIXWsKHPRqFkmN = 6.730749E+31F; 
ulong hCgP = 45653323223994132; 
float PgStfROejCOMnDUyqBIPjgx = -7.363733E+20F; 
long KOOGdzRxgUCQZocqFag = 49850056199028850; 
int KtFwldqxWLmygNDPKIAg = 85312745; 
while(KtFwldqxWLmygNDPKIAg == 85312745){
KtFwldqxWLmygNDPKIAg = 824673;
}double kVYwmPyCXwO = -6.597439E+17; 
kVYwmPyCXwO = Math.Ceiling(Math.Tan(1));
try{
int WKuhakaMSjqpbRoUnuUkiOiiVu = 3482681;
if(WKuhakaMSjqpbRoUnuUkiOiiVu == 32271){
WKuhakaMSjqpbRoUnuUkiOiiVu++;
}else{
WKuhakaMSjqpbRoUnuUkiOiiVu--;
}
}catch(Exception ex){

}long XLEpdXHLhCllHOekawuKtPEBkJk = 38468977546609308; 
byte CLZZXUjhReVDtbWWaChgceuNdewL =  95; 
string nTmALu = "xUutuFoalLUVjXsdVxpmVS"; 
uint OPWIJgDlDpfmNJL = 451864851; 
string KRzSjVPqcuhQEwWFFIIqfiItTS = "qlMAP"; 
float PIpWNMUPASwBCSRdfiPMNCjfYsRze = -4.338086E+29F; 
short FGnoIVWypJQYtpmBhGqZflnzkYwU = -5222; 
long XcDXlDP = 76308872657182168; 
int EYW = 55653845; 
while(EYW == 55653845){
EYW = EYW + 352884;
}ulong KAbbG = 89731407112801178; 
sbyte gRPw =  -107; 
string tqyOyuLLGTMK = "ORnRsClPJDaDEAlaEXMbsgL"; 
int dykfNglKtpoEXdDRDKOylTVzK = 92; 
if(dykfNglKtpoEXdDRDKOylTVzK == 423616){
dykfNglKtpoEXdDRDKOylTVzK = 635345;
}sbyte andbKWwPTjiWyGTikPxWf =  27; 
byte MwLklbXJhTWQAPSOs =  171; 
short MDmxcRzHkkFudDpokkDSm = 287; 
ulong JdKwXJExmCQjiohuDq = 32599232640756673; 
int fBxlOI = 98246318; 
while(fBxlOI == 98246318){
fBxlOI += 542558;
}int gCMggV = 16; 
while(gCMggV == 16){
gCMggV = gCMggV + 831131;
}short qNVuex = -11402; 
ulong EwBDi = 35709092010127829; 

}public void txIeXkBj(){
double GIthoyjRuxi = 1.112102E+08; 
while(GIthoyjRuxi == 5.026554E+29){
GIthoyjRuxi = Math.Pow(double.NegativeInfinity, 2);
Console.ReadLine();
}int ZSySRPkxwtZx = 625359312; 
while(ZSySRPkxwtZx == 625359312){
ZSySRPkxwtZx = ZSySRPkxwtZx + 328313;
}uint gihpfsodbdZdOaAnSiXlxwTQdaq = 34090; 
byte jpKlRKh =  186; 
uint husuWIVgfbDtozLp = 2346; 
sbyte QbO =  107; 
ulong ePIELyzMobBwgsxqVlGRakB = 10884268291143960; 
ushort iDfADpfVEjKPWeOEAfD = 20922; 
double UoglPQYbiweQYPpStVAWybQ = 4.733134E-07; 
UoglPQYbiweQYPpStVAWybQ = Math.Pow(2, 2.1);
object oKfgY;
oKfgY = -2.762378E+32;float sZiEfzA = -6.903205E-09F; 
byte VbCcpKUIDlyoGfRtZCDqTLRWKWDSP =  43; 
long GbuRXpAnd = 27479783384828183; 
sbyte BSUgjjFttgcTtCkJ =  17; 
long wJbnmyxUbZtfDJJkuMVkHtXghbDVw = 19810481666242760; 
int JNpDtZbAjLpQWJBtcLU = 23; 
if(JNpDtZbAjLpQWJBtcLU == 469430){
JNpDtZbAjLpQWJBtcLU = 326439;
}double tmYSMQflzqQFg = -798.6425; 
while(tmYSMQflzqQFg == -4.695491E-31){
double WJfFyuOkRlx = 3.041982E-31;
tmYSMQflzqQFg = Math.Ceiling(WJfFyuOkRlx);
Console.WriteLine(tmYSMQflzqQFg.ToString());
}sbyte oMgtLymAEiBy =  -50; 
sbyte BUnVkTxZRxKl =  96; 
uint EFDUXgzHfZPEIYmEuyZSDEgV = 859027; 
ulong WbltaPFVUwi = 7325930965148846; 
string SpOUHlZzHxeuiKeNxD = "tqJYnOQpwkBheLLdLyzuDFynYe"; 
ulong DdiZNVImiqBfQWEmgoaKyGtoXiq = 80207766345206868; 
short Buid = -2648; 
float XtEGbkhKegyWK = 1.77653E-35F; 
int TuOsqhVjlkGVRHZLas = 597839; 
if(TuOsqhVjlkGVRHZLas == 661109){
TuOsqhVjlkGVRHZLas = TuOsqhVjlkGVRHZLas + 260428;
}short RzQIi = -1654; 
ulong jdgbHkIxPaDOnBylKBy = 2577254673119866; 
ushort EbgflyVFYMuWn = 62394; 
long CSpaFYJaQMcsHZwTkdlZwJ = 44056164439446376; 
int dPBtaxP = 362544; 
while(dPBtaxP == 362544){
dPBtaxP += 382532;
}int ZqBMkMJoTQmzXnTXNfUoRzbSPoHHp = 5348; 
if(ZqBMkMJoTQmzXnTXNfUoRzbSPoHHp == 668630){
ZqBMkMJoTQmzXnTXNfUoRzbSPoHHp = ZqBMkMJoTQmzXnTXNfUoRzbSPoHHp + 714883;
}uint quywAIEOYRqAiDuAng = 290080; 
long WecZBnEzoDwuwLxTqRzeQypggQhj = 36099882728521526; 
short hojACPdPgJduQKRF = -25136; 
uint VSKguONslAamdBQKiUqVBcsJ = 120787; 

}
} 
public class lkbsfujskoAKUmBtA{public void jebHtGsWfwzHujOnyJGDQV(){
long ndPSlxjul = 25940897559315626; 
byte PTEVpVHQRO =  238; 
double xQspQBKJkd = -1.801145E-22; 
while(xQspQBKJkd == 7.002171E+18){
xQspQBKJkd = Math.Floor(5.476291E+25);
object GEPjKPlUXaFuoIEzI;
GEPjKPlUXaFuoIEzI = 1.551901E+07;
}string pEchboDBALLLJsbVWLe = "GunXtDLse"; 
long jRsKbqhHnVgIHRwtAxj = 20322888735395958; 
ulong EkxOeCUqJaFPeQp = 24593288782507627; 
float jlatPMluiLLKTCpCcRxF = -3.408382E-12F; 
sbyte AiHaPosoCgUoTXUn =  -37; 
ushort fGPTgPyguApuReBUGelFTswiNIa = 42439; 
sbyte hmTxbUKpBkshYPkMcktdnigPcNjG =  -98; 
uint QbxsdBwcGjpFamMuHYaFQnbK = 104155646; 
string oKejbxDfDyhkI = "OTXFgDp"; 
ushort IfUYCaMtYyaOVfgDFL = 40193; 
float QPWIBiM = -5.732837E+35F; 
uint Vgxeh = 18237; 
sbyte ejOIuSORbDSLCuRpXqaSaYZW =  108; 
float JcugKBbeHaonIuVsxJttMFOk = -8.798855E-39F; 
byte fkSLCdePIwijBpkoIdsyfGDm =  140; 
uint dnja = 24472934; 
byte wiEMFofLaAjTwuzDdIQLAIFicabBy =  105; 
uint osPgeqOhZVDOtGHKPIXbxBzOEwJE = 6115; 
byte alEdFsyMTIMFfRPytqL =  206; 
uint BOUbDKJjUtIXOgb = 110544; 
ushort kiPPTJN = 39871; 
byte XVygDLQj =  142; 
ulong dKOsXhdJwQknHmoMYatQdxZW = 57534023202164531; 
long hzbyWBJXRwMHQ = 5615842212722246; 
long RTMqSmiwJHlfQLCdSggFEk = 37756488070559520; 
long BOjEjjRLJZdaFNyooesSF = 32123413248969671; 
ulong pXCFcBYU = 54472287371858627; 
byte gffaRfzHECSQWUVQ =  193; 
string huPqk = "ChfDAkEhuKHdpgwgwWybS"; 
ushort bPtOLcfsPUqoYseep = 18335; 
long jnMyVJaYSUiRiGeOf = 75703583161096489; 
float PqRgNjRqNcWScb = -0.1820817F; 

}public void LAfsmpZTxDnTxDzdllIbNxIsoqL(){
sbyte QegSWyAMfDaRmqIDzq =  3; 
ushort QQAzhoSOdVGQoHzhaAeZqgJH = 12530; 
uint ZPQZj = 79288715; 
double IuOiIYcHwb = 2.041754E+15; 
if(IuOiIYcHwb == -0.0001922239){
IuOiIYcHwb = Math.Ceiling(Math.Cosh(5));
try{
int tecliDuQushNo = 619574;
if(tecliDuQushNo == 99291){
tecliDuQushNo++;
}else{
tecliDuQushNo--;
Console.Write(tecliDuQushNo.ToString());
}
}catch(Exception ex){

}
}float hBfeFeHfeQbEVOIPiqbLBEYDt = -8.902789E-14F; 
byte fqRABami =  60; 
long iach = 20209041327338166; 
double pEDDLoNu = 4.144334E+36; 
while(pEDDLoNu == -1.470814E-32){
pEDDLoNu = Math.Ceiling(Math.Cos(2));
Console.ReadKey();
}uint UIuuYSODXqoVPlCo = 211881943; 
uint UqUxGEDgVhypKtyhQXMiIJKyeCsZ = 65; 
ushort ZDGDNXOwoIHQTZlxPuAUAsnl = 42261; 
ushort dlDbmPJpgNVu = 55634; 
sbyte TabUEfoWkLi =  -8; 
short nuUpGKiMGtakJeJTZILi = -658; 
byte NTmPospe =  80; 
short TtaemzhgwIUtxjnqLCNhh = -32000; 
byte xtVUuo =  72; 
long aloSVulasSDTBQFbaGEDkCSjTI = 8484995428831647; 
ushort YzQgCgPhHnWDqStKMgIAufZmBJNG = 49337; 
byte ZuHcCQxbdMKDFMqlwtwIc =  222; 
double TADGSuiwyGXiLQRUof = 4.828623E+18; 
TADGSuiwyGXiLQRUof = Math.Ceiling(Math.Cosh(5));
double VUbxZBSJoyAqlnMpbzBORxe = -6.078579E-22; 
while(VUbxZBSJoyAqlnMpbzBORxe == -3.096272E-16){
VUbxZBSJoyAqlnMpbzBORxe = Math.Ceiling(Math.Sinh(-5));

}int CbwBNiUnzbxaTNeEZboPyy = 976361; 
if(CbwBNiUnzbxaTNeEZboPyy == 312227){
CbwBNiUnzbxaTNeEZboPyy += 157556;
}sbyte DHsKbmYWVApHoAdhYhgGT =  40; 
int neHxULSMePAPJjLRGdgPARi = 899146503; 
while(neHxULSMePAPJjLRGdgPARi == 899146503){
neHxULSMePAPJjLRGdgPARi += 23747;
}int UcFwXHCbkHECBanAOxoaPDPny = 697160519; 
if(UcFwXHCbkHECBanAOxoaPDPny == 624256){
UcFwXHCbkHECBanAOxoaPDPny = 689121;
}long XFNdPOkdcViVoJWHY = 21158867844437315; 
double WaF = 3.812931E+33; 
WaF = Math.Ceiling(Math.Cos(2));
sbyte QWaVqxHBTN =  -119; 
double Yscmhq = -1.204295E-15; 
if(Yscmhq == 7.370009E-26){
double ulKXI = Math.IEEERemainder(3, 4);
Yscmhq = ulKXI;
Console.Write(Yscmhq.ToString());
}sbyte OegdCy =  121; 
double fxmcRwEBTbJAkNBZD = 3.044942E-24; 
if(fxmcRwEBTbJAkNBZD == 7.707365E-27){
fxmcRwEBTbJAkNBZD = Math.Ceiling(Math.Cos(2));
int? ITTEXsusjdKicZOglu = 9249068;
ITTEXsusjdKicZOglu += 54879;
}long gJtVeiBzKIeyB = 24239033960167846; 
float pVAJjzQzPDIDBJdKXLVxeipUEWyB = 9.820348E+30F; 
byte pMda =  152; 

}public void pZTjoUSeWsXmKLyu(){
float FGfnbo = -5.853476E+28F; 
short mPGdeywVEUTxbzaeEMk = 1679; 
int TWwZucYELwDlShNThlpCRXniEHm = 721592023; 
if(TWwZucYELwDlShNThlpCRXniEHm == 800489){
TWwZucYELwDlShNThlpCRXniEHm = TWwZucYELwDlShNThlpCRXniEHm + 28548;
}short JIedsXZjzTQHSqLEZlceWIlxOPa = 22981; 
double NkCmdkciXFSOQpNawGaiKE = 3.240455E-21; 
NkCmdkciXFSOQpNawGaiKE = Math.Ceiling(Math.Sinh(-5));
Console.ReadKey();ushort unOdxj = 53958; 
uint xLxjwOyJEAZnBcJAVpwLHJJhAty = 79109278; 
sbyte AHmyZUmzCXIIHwTTleECUxQoN =  -119; 
short cpKUuIBHmGJGIRTNYbbMHB = -18250; 
long IYdQYKXF = 33980486260750117; 
byte HwYTcDdqMZDeDeaJNJx =  209; 
short LafuIzjOcRuUGnmOVBNHXXczUfn = -25708; 
uint zwqLKXOJVFg = 952931046; 
long IANxYOWdIYOYHBCkenMtZ = 63901383000952752; 
uint aqOlHCKCDCqJMWZhhttcKlsLeFigY = 325697; 
double ZNHSnVK = -5.982422E+16; 
if(ZNHSnVK == 1.798847E+11){
ZNHSnVK = Math.Ceiling(Math.Tan(1));

}sbyte pNyaXGRMzHfoPckLBOFsRRV =  104; 
float acUCkqaXtlkPioAfKBAWGtnl = -1.320302E-05F; 
byte XARCihFOSWwPoRJYBMFoSLg =  102; 
ushort jIPIzOgmZUuJGbKLnNeLIOKoMsR = 58376; 
float sxgmYXZkz = 1.203142E+12F; 
string PFRfEGgyupLnwXBVgXmsNCYDs = "MMco"; 
ushort HVV = 46093; 
float nnOPYgwZD = 0.06247925F; 
short KxJfnGoNBTuu = -25633; 
float IAcQZUJYEXESgtBMmNWNG = 1.093624E+24F; 
long GANuu = 87009997103854869; 
short azehKOhtZhhHied = 12505; 
long LuuWEjDLEubzPE = 71503082936949284; 
uint NmlOKi = 65359100; 
short XWceUVGZjOsBsEjKt = 27062; 
float zwW = -1.100954E-26F; 
short PbwLtfmGCxImXcYfKOmUJEaQOQqd = 3149; 
double WFXQYPcYufXImLJUYBolCWw = -4.346795E-23; 
if(WFXQYPcYufXImLJUYBolCWw == 9.359668E+33){
WFXQYPcYufXImLJUYBolCWw = Math.Floor(4.341299E+21);
int? SPLWNefWsEmOmaiIGWifwGWU = 5932325;
SPLWNefWsEmOmaiIGWifwGWU += 73279;
}string FnoCSHyGQOwLAsHdiCt = "SQQhefcFd"; 

}public void iBWDdqIImP(){
sbyte dsjoBCgxQcsR =  68; 
ulong gxNFSCCFUOWoowC = 76046997523323583; 
int zHzfhePoTBkBkpQC = 29332259; 
if(zHzfhePoTBkBkpQC == 255015){
zHzfhePoTBkBkpQC = 831611;
}string OONmaZjIie = "UcANJgZxUpOu"; 
ulong kMInZmIGjqusCnTqEsgXVz = 48082969764990195; 
string ZCQmCtiRMoShs = "FLDENxnQQMz"; 
sbyte CZlzlqZsZzGPjj =  -55; 
byte GfjBSESEkFgjMdPxXHS =  124; 
ulong QtZGlWOElYFjgsVUJwu = 45416421540610033; 
ushort DxMPZoRCm = 21594; 
double IshGDXyBcXxfVGdASsRfuqKC = 5.176948E-09; 
if(IshGDXyBcXxfVGdASsRfuqKC == -2.200357E-32){
IshGDXyBcXxfVGdASsRfuqKC = Math.Pow(double.NegativeInfinity, 2);
try{
int RwpJGNhQuxKCsIQmK = 5850;
if(RwpJGNhQuxKCsIQmK == 43708){
RwpJGNhQuxKCsIQmK++;
}else{
RwpJGNhQuxKCsIQmK--;
Console.Write(RwpJGNhQuxKCsIQmK.ToString());
}
}catch(Exception ex){

}
}uint IZdg = 574132; 
ushort hfUgEdDIcD = 44874; 
string Obf = "XIBcLLeK"; 
string lxHpiolxBQWhcmWFyNzdUXXXEW = "lywPyDjpkxmBLPqBpnPGmgfnqk"; 
double nzKMmwZZfazGSWLiklOnEyIJOqHfq = -0.0003531819; 
if(nzKMmwZZfazGSWLiklOnEyIJOqHfq == 1.075389E-23){
double npQqwoPjNqxWdfyKw = Math.Log(1000, 10);
nzKMmwZZfazGSWLiklOnEyIJOqHfq = npQqwoPjNqxWdfyKw;

}uint mUkzeSLd = 13575745; 
int DDGHstzUmWLkPLFuxGiZdMxCBefm = 36921333; 
if(DDGHstzUmWLkPLFuxGiZdMxCBefm == 523071){
DDGHstzUmWLkPLFuxGiZdMxCBefm += 62133;
}long jROaUEXqjZJtnpHfYGTQ = 13207920564929178; 
sbyte KMcXkGRifAOesBw =  61; 
double MInVjSBgKBNJu = -1.741392E-25; 
MInVjSBgKBNJu = Math.Pow(5, 2);
for( ; ;) {
Console.WriteLine(-6.836391E+16);
}sbyte udoqfextfEDyhMuMtXJRJjaBOnfJ =  -106; 
short kEwdqCAupIofSbABPgJOVuon = -20248; 
string ERZNoRNqTNJdjUEsjuehznN = "DwBVRogToNV"; 
int VaofLHNfdzT = 338; 
while(VaofLHNfdzT == 338){
VaofLHNfdzT = VaofLHNfdzT + 887276;
}short eJkTqSAEHPP = -3704; 
double bnu = 2.904873E-35; 
bnu = Math.Sqrt(4);
ulong VcBPHmlzcXf = 28782120144144207; 
int RcyijFcFQbVfEoSly = 10; 
if(RcyijFcFQbVfEoSly == 505880){
RcyijFcFQbVfEoSly += 977683;
}float EyioWHH = -1.11289E+20F; 
string cGy = "iTmVkfbmjNGNWfDqdHaG"; 
sbyte oUwnZQkSiRLxsgncGcDkZDpTLbk =  64; 
double FRCoFsBXglbqEO = -3.684825; 
double WVMEDE = -3.666572E-37;
WVMEDE = Math.Sqrt(3);
FRCoFsBXglbqEO = WVMEDE;
Console.ReadLine();double GgpKJfGoOIFl = -1.399941E+37; 
GgpKJfGoOIFl = Math.Ceiling(Math.Asin(0.2));
Console.ReadLine();float dDJjmtaEUgTTPuJIAOZp = -4.073881E-22F; 

}public void auyzgMWpPoRBZLibXOkWFjPCl(){
string gloDnSLJQQUTSYReFXUKXpMbD = "pnQFVme"; 
float dAMEInbslcVtXOekFstURBma = -3230.488F; 
ushort kUIPXiNFSulwWXAZHQsBLupUMZ = 63431; 
int LyfSigswg = 782514; 
while(LyfSigswg == 782514){
LyfSigswg = 212237;
}sbyte YTbntojWhRENcYkishWgENQhEBuE =  -4; 
byte fPOb =  25; 
uint qhVVapOJdyERhjoZukcUKTUHxWOzY = 9414; 
sbyte tQOkIMAtVyXZIdSQPtQc =  37; 
short mozeDoLOBzjPuCP = 16284; 
uint BOxXHPTVJiWgdZesBV = 64; 
double kJyCy = -0.006800179; 
if(kJyCy == -5.197296E+08){
kJyCy = Math.Pow(5, 2);
Console.ReadKey();
}long muBDRsgM = 19712397009429141; 
ushort daZfZnqKgHatLgPgkoqDoVyLxDYeG = 50325; 
string wDnHjgulkgkBL = "UXGhNGFTLiDoKyp"; 
byte tEHXi =  220; 
sbyte dUeqiKXZYIazybyPDSfeVyXVYbSW =  -106; 
uint dykJLACCzGoLPoOIsfRupN = 4; 
int isgSaVyWXbmsMpbpHAGMhGkuBRmg = 321897; 
while(isgSaVyWXbmsMpbpHAGMhGkuBRmg == 321897){
isgSaVyWXbmsMpbpHAGMhGkuBRmg += 773193;
}short wHFunTsM = -6852; 
double CtZfFxTPDgMqj = 1.649788E-14; 
while(CtZfFxTPDgMqj == 3.298672E+10){
double RVaUkzonAZByMNwkPNjZNdJowNXsM = -4.757904E-17;
RVaUkzonAZByMNwkPNjZNdJowNXsM = Math.Sqrt(3);
CtZfFxTPDgMqj = RVaUkzonAZByMNwkPNjZNdJowNXsM;
 bool? CDefKkwBoIMQMVE = new bool?();
CDefKkwBoIMQMVE = true;
}uint BXCuJQckkqefCOA = 410882; 
double KRtmqmZGxpOSukM = 25953.95; 
double OpNPqAOsomPHhK = Math.IEEERemainder(3, 4);
KRtmqmZGxpOSukM = OpNPqAOsomPHhK;
int? GVxzLUPTW = 3849129;
GVxzLUPTW += 81761;string WoGSZyEekwOPAgkjHzVKtbVE = "KgcfbAIXqejuIlR"; 
short khtHGHJtAWVtxVu = -29885; 
ushort qBYiVgRhcaQ = 64077; 
byte sWQNj =  216; 
string uNDENFViQsoWARzb = "mPyEIEBJR"; 
string eHSBkRglHsummTky = "qFByPBXcHVMicUVDweiBO"; 
long LmuUSCkLyfQsmBcogVzF = 3351534673131952; 
ulong XVGW = 35809178804604033; 
short lDJc = -9906; 
ulong FpfM = 7208998506470660; 
ushort VMqLTVA = 52873; 
double MfZsVQI = -1.963017E+36; 
MfZsVQI = Math.Ceiling(Math.Asin(0.2));
Console.Write(MfZsVQI.ToString());short gIgAiszebEkfGEmKdREYexxg = 23549; 

}
} public class RYmcKXVdjIYPXeOaaClkuCjTiL{public void qLLkZHoBARYUnkDtFMo(){
float jdwVVIApPuTYsFs = -3.452346E-06F; 
double IFojbIY = -3.042355E+26; 
if(IFojbIY == 1.733171E-28){
IFojbIY = Math.Pow(5, 2);
try{
int HjUhNfmUVUtnWHHhilZhlyLjgUj = 7763096;
if(HjUhNfmUVUtnWHHhilZhlyLjgUj == 68425){
HjUhNfmUVUtnWHHhilZhlyLjgUj++;
}else{
HjUhNfmUVUtnWHHhilZhlyLjgUj--;
Console.Write(HjUhNfmUVUtnWHHhilZhlyLjgUj.ToString());
}
}catch(Exception ex){

}
}sbyte LaSVxBqixDTmZULR =  -78; 
int kXmeMSIpcBRXtkiDzDTgMVFBAneji = 379357822; 
if(kXmeMSIpcBRXtkiDzDTgMVFBAneji == 699432){
kXmeMSIpcBRXtkiDzDTgMVFBAneji += 966753;
}uint VfcPknIGGAcRqmo = 4436; 
byte tNGzOyglgVN =  172; 
byte fEyQsBHWVUPbtREeFNxI =  40; 
long IoqdbCKSRQj = 16657108381555104; 
float JMeoIbpZlFzwnQCaxFwkbX = -5.560212E-26F; 
int ndIKZLRFYDmPtAtTFlMQwbZepdc = 68391670; 
if(ndIKZLRFYDmPtAtTFlMQwbZepdc == 910314){
ndIKZLRFYDmPtAtTFlMQwbZepdc = 681066;
}ulong mqDFOFKRZNIlVRitayPzleE = 8131151755343934; 
sbyte MgCbHYlMkZqJSgQBsBy =  97; 
short hOjJewPFXwCGiLHst = -659; 
int bfmKdCtuVCUlpjZOSXTRXG = 91; 
while(bfmKdCtuVCUlpjZOSXTRXG == 91){
bfmKdCtuVCUlpjZOSXTRXG += 112267;
}float gYpDBbBAGDbwJwCpoZLTBwGNPwcqD = 695150.1F; 
byte VwVVOJXmsspPDXIw =  209; 
double uaALsEOfZjWOYOVNZGwCpOHidoXmz = -2.711041E+31; 
uaALsEOfZjWOYOVNZGwCpOHidoXmz = Math.Ceiling(Math.Cosh(5));
int? nkUtVwqobcCMPENAXWG = 4802153;
nkUtVwqobcCMPENAXWG += 93597;ushort kKYelTsNtTWSRDhfJVtPhUw = 27630; 
long kmCpcQuPDQZ = 17585923856004616; 
ushort EVwdEEGHpo = 36114; 
ushort JfldQfybZsfRBYtPXPupaHqglAlP = 10905; 
byte xyyMpgTQVWmEDomdG =  58; 
float BhdWbOfRqUIT = 2.392781E+24F; 
long dldIdYAuNdORZ = 51198467640056309; 
uint CbcAaouQMGSauijoCJKiXZ = 8345; 
double eCSPwbCBCG = -0.2370699; 
while(eCSPwbCBCG == -2.335786E-11){
double wluhGAGJ = -5.786294E-36;
eCSPwbCBCG = Math.Round(wluhGAGJ ,1,MidpointRounding.ToEven);
object kENjSYWjRzTIEmbkFQKXbKAb;
kENjSYWjRzTIEmbkFQKXbKAb = -2.001153E+19;
}ushort DdFM = 33941; 
string BzMHtzpfJLyfAKpuSZuDIW = "USxoGKtgoSZubEzKRHmeSVsV"; 
ushort JNMgCdEJS = 51545; 
double LwoFhnB = -1.087962E-17; 
LwoFhnB = Math.Ceiling(Math.Cos(2));
Console.ReadKey();sbyte TwpuFygeOfRFcNLSKHfJ =  -120; 
long pwWkxkwnpunlnnAufWFxqpnsx = 47957283495208341; 
int ZIl = 88465648; 
while(ZIl == 88465648){
ZIl = ZIl + 196210;
}ulong xEjmZMylBZhFPVdBj = 40247948349039006; 
float LcAPdEUfE = 8.02497E+24F; 

}public void KiYcaRd(){
ushort QTIslhALpJBeAMiakdbVPAPt = 63587; 
short dJmJJyzUjSxRwww = -16090; 
string ejxPbQNcCZq = "nuYK"; 
ushort cZS = 42152; 
string ZhOmGAumOcUeboIyWGWzEfFcTM = "ujbzxhFQTbBxbFdpkAUJhtGdlMDoT"; 
int tUBacMFyWxzOwwjUUAQtDZ = 82; 
while(tUBacMFyWxzOwwjUUAQtDZ == 82){
tUBacMFyWxzOwwjUUAQtDZ = 225748;
}long oWppgDBEbEHcXlJGjJc = 31253145500653311; 
long yOt = 11850639942390160; 
ushort ymMAbRiuuoi = 57976; 
double UfgNLREEPnpIeghe = -2.556909E-08; 
while(UfgNLREEPnpIeghe == -5.258266E-13){
UfgNLREEPnpIeghe = Math.Ceiling(Math.Tanh(0.1));
 bool? yzjuMcloVOxFhdIzwKpYBpdzmyz = new bool?();
yzjuMcloVOxFhdIzwKpYBpdzmyz = true;
}sbyte MgoEwiboBYKLJ =  120; 
ushort SFNNIbzXUgHOl = 44976; 
float mFOR = -1.259191E+30F; 
double DgVRQTaCJRujIuNKJsK = -112.696; 
if(DgVRQTaCJRujIuNKJsK == -8.212301E+24){
DgVRQTaCJRujIuNKJsK = Math.Ceiling(Math.Cosh(5));
int? afffupIx = 1630689;
afffupIx += 41849;
}sbyte CzjGmgzNNWdUAcVWFjPdX =  96; 
string JGCfyCCIhsEmaQKjEKJHxpMBgneZ = "eNItmWHGsolwxNgAIWP"; 
uint AmkyBsTaYiyCbcGWTNT = 58; 
byte ykaEGbzqpAaGAy =  235; 
ulong NFtfOTuKBDPjVyamSN = 82795765486609383; 
string yfA = "ZooaSoJqGUezKhubasIKKnQAGTnH"; 
float mwdRkGhawDdVU = 8.276431E-26F; 
uint KNbaeyPnMAQwZbcBlIKuSXOTZ = 113121073; 
int tVpecEUQthXoEAgsGwBauJnA = 95; 
if(tVpecEUQthXoEAgsGwBauJnA == 141885){
tVpecEUQthXoEAgsGwBauJnA = tVpecEUQthXoEAgsGwBauJnA + 16356;
}sbyte hAsEApWyhLSA =  63; 
sbyte zHCtCGGuLI =  85; 
double RVIta = 3.874923E-29; 
if(RVIta == -48741.28){
RVIta = Math.Ceiling(Math.Asin(0.2));
try{

}catch(Exception ex){

}
}ushort HsoEbBXmYEDjPuOcCqy = 6843; 
long Ncp = 13841191331297180; 
sbyte CECXU =  19; 
short iTIQ = 6792; 
long KIkpYGFmnIqfQDzgdJ = 28796747444408643; 
double jIEFhTMxkQizWqmpEz = 2.308588; 
jIEFhTMxkQizWqmpEz = Math.Ceiling(Math.Asin(0.2));
byte BEKIPqIziXWjIMRmfGEHyoiOT =  94; 
byte buTgRiClMmIMPRBRgYVoLEQJcFtl =  104; 
double xfKyOePtAjFBTXRSHOsIVyW = -0.0004944802; 
while(xfKyOePtAjFBTXRSHOsIVyW == -7.634577E-33){
double QAlGeuNNsGxegcXMkMNzdnSYj = -9.650062E+21;
xfKyOePtAjFBTXRSHOsIVyW = Math.Floor(QAlGeuNNsGxegcXMkMNzdnSYj);
Console.WriteLine(xfKyOePtAjFBTXRSHOsIVyW.ToString());
}
}public void pEExSGIcahNy(){
ulong SzNJRo = 83611861940729582; 
ushort FdqdqRZwPT = 5012; 
sbyte jQEjSJXLnMIM =  11; 
uint ZYegIMEJgMPbAsTX = 4940; 
ushort mUJJTyEpsKsOpAxRPXAGHgxzdH = 30277; 
ushort OxlfHmfpwmjim = 21003; 
ushort aLaVchnaRmfyj = 39265; 
byte jVFmfJJlqBtKoytQZecVzysHhlnug =  165; 
sbyte bkYqMjnscOFDOEMuTozfDpCN =  18; 
double UuoeJ = -29120.46; 
double LTKTPSFBUBSjanLJkNOeuXOJV = -5.51114E-33;
UuoeJ = LTKTPSFBUBSjanLJkNOeuXOJV / 3;
try{
int VRkJoRyRTOfCuAOmAtoYw = 8556795;
if(VRkJoRyRTOfCuAOmAtoYw == 97845){
VRkJoRyRTOfCuAOmAtoYw++;
}else{
VRkJoRyRTOfCuAOmAtoYw--;
Console.Write(VRkJoRyRTOfCuAOmAtoYw.ToString());
}
}catch(Exception ex){

}long BLymSqYJEayZwxPVXmI = 84089198614586553; 
sbyte SCCfHYJSLEWqHgw =  -51; 
sbyte CSMMGqhLDRuVcwRjMAW =  98; 
ulong fdmJMjoQANmiwoN = 38226863684710461; 
ulong BAxwWtkSdMIAWGYwN = 48679958012825981; 
int XSboHyhkKTwEubpqDfHYFYC = 49544412; 
if(XSboHyhkKTwEubpqDfHYFYC == 648679){
XSboHyhkKTwEubpqDfHYFYC += 503952;
}double QsHnabybbOunDqPlHDKasYFhYptPP = -3.061442; 
double poedFjkLYqLFBeRXHROzpKJ = Math.Log(1000, 10);
QsHnabybbOunDqPlHDKasYFhYptPP = poedFjkLYqLFBeRXHROzpKJ;
 bool? JCJNEzTmuWn = new bool?();
JCJNEzTmuWn = true;int wRTWCipFgfHSxbsytCc = 6451; 
if(wRTWCipFgfHSxbsytCc == 927580){
wRTWCipFgfHSxbsytCc += 852439;
}ulong wZgjxIPByBzwebxWeiEzOV = 11978422672252539; 
double NxVGa = 5.78286E-24; 
if(NxVGa == 8.549122){
NxVGa = Math.Truncate(NxVGa);
object dwbuRKHFuIJfRWhYtRmZsh;
dwbuRKHFuIJfRWhYtRmZsh = -2.503613E+35;
}ushort QOVzLBqVOEAdEPuFkkLWZH = 53295; 
int xLZ = 18682002; 
while(xLZ == 18682002){
xLZ = xLZ + 400331;
}string UYNUEbNFpNmzYX = "dBxNjYsyn"; 
ulong oFwPJiNZKmyR = 39762606455608519; 
string XeiXISyDBzelNhiCdeMYm = "HJdOpMeieDpGIqGLBVDhMYNglMcMk"; 
string cxXzBTTDLysuCWgB = "VtFQxemIWXDFKtYZcmiKSfRHsR"; 
long FlkekjRRBKHqK = 80241560295448052; 
ushort JXRaPCYzFuTdLNz = 24872; 
uint gAsHSZbDVouQHbjNC = 37; 
byte pClb =  239; 
float TjTFDClVgcfSYlHJdPOSUYk = -7.969054E-39F; 
short RXfFVhoJMxQnePoG = -455; 
string lQKTiliZTVYCEmbwX = "ykK"; 
long UlnCJcFsLpAcLheXtM = 22999912114109499; 
ushort FcQRElPHULAW = 17706; 

}public void mlejqnSdezDmQgsbCG(){
long NJBIAzszd = 55398157607409071; 
ulong fACXqopxIoSiUmaQFLyLTV = 45884555721586782; 
long LUEOkLisibKjkAsYguWXJfTjeCC = 51194227895169072; 
string mFTDeCRlkPXTsXjLMlGau = "KjcuXjJFmbsoUQcwFEbYU"; 
ulong lGnIpTLSZoEVKALFID = 54049522153975298; 
short cZxPXFROJDsLCjtapUtV = 24404; 
double wmjJMDkmJDdPa = 1.330616E+22; 
wmjJMDkmJDdPa = Math.Pow(double.NegativeInfinity, 2);
float ljELzRBND = -7.607302E+20F; 
float EahRbqRkD = 14.93525F; 
ulong BpbhJYqnLeJlExKFIGQElksZzKcZc = 27471066309036588; 
ushort QEYTWzGyVgHVsLiOpYcKY = 60256; 
short bTHnefAT = -16069; 
double Zns = -0.6283866; 
double sPLalJKLUmNRdSssPINIxLjBXC = 1.363824E-06;
Zns = sPLalJKLUmNRdSssPINIxLjBXC / 3;
 bool? zPmanAtfmjDlfNCBsIBUtkJf = new bool?();
zPmanAtfmjDlfNCBsIBUtkJf = true;ushort ejFxZxjUqRunggEHUi = 2191; 
int KmZY = 930824; 
while(KmZY == 930824){
KmZY += 920673;
}long fbmcilHBmCiMBQyAXYQIRAuXF = 1337991834291551; 
ushort fqF = 47521; 
double cRidnzOAQgJfc = 5.298238E+08; 
while(cRidnzOAQgJfc == -9.743479E-05){
cRidnzOAQgJfc = Math.Pow(5, 2);

}ulong snUcdFJTxUGESlzeFapVjUHCqTPG = 71343781388600577; 
uint cgClfUjVCtUOD = 9111; 
float zIsLkPcFVpxgasQFkXjtYON = -4.455545E-06F; 
short MCYtnDdwaiISqz = 9132; 
byte NOCbejqmLBQKVZoTtq =  11; 
double hFWXPwEeTUqDhFJQuHsIgDbnLfi = -5.503196E+36; 
hFWXPwEeTUqDhFJQuHsIgDbnLfi = Math.Ceiling(Math.Atan(-5));
Console.ReadKey();string WRGBEcXTNSmwgSJsewR = "gUJqzxiXoRqIZTnANdpVNDbMftH"; 
string fQDafAbiobg = "aEWBFwmEjGafWhETWkfQeHiaPH"; 
float mpybicNYekBXu = 6.359028E-31F; 
uint xeYWildWqPtwVMYCTqThYWUu = 781690; 
long hwEBJlsDVyddufSmtEiAD = 78148249318182736; 
int uBuPVlSMKtxH = 241670; 
if(uBuPVlSMKtxH == 873055){
uBuPVlSMKtxH = uBuPVlSMKtxH + 77401;
}int aGl = 71; 
if(aGl == 268672){
aGl = 305982;
}ushort qsfKEapWbCSAggPeYyGXxEDqO = 18104; 
byte aTFx =  73; 
uint opDlN = 143631; 
int igfaWQuRLzwNPPVo = 24656051; 
if(igfaWQuRLzwNPPVo == 936266){
igfaWQuRLzwNPPVo += 410403;
}
}public void FbBPwkSu(){
short chIilgkzOMRgqACHXZplyEOWCYwbY = 6979; 
float fPNHn = -3071.107F; 
sbyte KiKtzLbnPnTddCzuMHmMjA =  -88; 
float aDMUtRykBOudDcntNjqZacn = -4.793156E-20F; 
uint ouBjaQGyipuxywTD = 8357494; 
short FYsxpQT = 15925; 
double yHRqhRZFun = 5.857169E+07; 
if(yHRqhRZFun == -2.00643E-10){
yHRqhRZFun = Math.Ceiling(Math.Sinh(-5));
Console.WriteLine(yHRqhRZFun.ToString());
}string GcfJym = "yBLjTJdxwhZBBxNPtBub"; 
float WKPdeFEbfSSpUY = -8.36553E+32F; 
string KFUgHODLSnNiclNbFbuQFzT = "bgIjCOGxzHspC"; 
ulong aCtM = 47583334788006978; 
sbyte likLBeAgsidIpLxTx =  -3; 
ulong uokNEefqbGIuzVlyhFaLDUikJIZlu = 8887384895244046; 
uint xZieNB = 17887646; 
ushort YMSzexAgadWfTXo = 31057; 
short oNoAJyFMuQwKZzENwaoVBNIQ = 29808; 
ushort qfkQnlTU = 42592; 
byte cuUsbKLpeyCfDoj =  95; 
short eAColmHiahyfFWEWX = 26425; 
string LYfIoOP = "JBeTEqDLXOhHMTKESZSGDGqnthSgu"; 
float zyH = 0.0003683883F; 
string DntTZjNtxRnCPpVjycj = "AwAwqcNKjetkBnxFlJHhCZLa"; 
float fLDmjyomegPYDWxdjszcbCTqY = 2.64005E-07F; 
string BLzCzWcgWV = "VmipQnfLMyPtCQQFRlowbnYFxpx"; 
string tWnyEhOtECLILoRLsWLJslVHATIoE = "ARcLtBCVYJblnLENpYgUdtLuIh"; 
string XKAzPJbTGaWpixYcIgX = "dNDJiKQg"; 
sbyte clAVYojDxFJLhpbRLIwJ =  64; 
uint QEkgQpeT = 751230196; 
double LQnzKTeKsCtFmIsZiMyUCbKqNitl = -3.780125E+17; 
LQnzKTeKsCtFmIsZiMyUCbKqNitl = Math.Pow(double.MinValue, double.MaxValue);
Console.ReadKey();int BzkpQAlCPjXSHEMhHRMBMPElpF = 58; 
while(BzkpQAlCPjXSHEMhHRMBMPElpF == 58){
BzkpQAlCPjXSHEMhHRMBMPElpF = BzkpQAlCPjXSHEMhHRMBMPElpF + 999557;
}long cAQsxkXTsXqsHIo = 75463098933981383; 
double ksHwbUXhiBlsouCDTyHUYYzoJMbRE = -6.319374E-13; 
while(ksHwbUXhiBlsouCDTyHUYYzoJMbRE == 4.749509E-12){
double pcujygdfMUIEnmne = -1.34149E+27;
ksHwbUXhiBlsouCDTyHUYYzoJMbRE = Math.Ceiling(pcujygdfMUIEnmne);
try{
int NpTgFSiHCWiJl = 7306797;
if(NpTgFSiHCWiJl == 32464){
NpTgFSiHCWiJl++;
}else{
NpTgFSiHCWiJl--;
}
}catch(Exception ex){

}
}double YLajmGGYxFQMYsSeLFmMCgkoMXFPd = -1.359309E+22; 
while(YLajmGGYxFQMYsSeLFmMCgkoMXFPd == -7.228111E+14){
YLajmGGYxFQMYsSeLFmMCgkoMXFPd = Math.Pow(5, 2);
Console.ReadLine();
}int DclRwhI = 3383959; 
if(DclRwhI == 510686){
DclRwhI = DclRwhI + 425055;
}ushort OJIfexQuDaxSDBXXVRNufRJoasm = 58869; 

}
} public class cDSwdIHSiBMNIKQYoffeLdJJZQW{public void tfNiVABzEyATpeNpbIyhSpLmhQkZT(){
sbyte mLHzdODRijswSG =  -69; 
ulong ptIqtOhcuAafnBFuamh = 73667700733017583; 
uint SHWbGKBBeTXAxJnCMUJNbStefZG = 185479603; 
uint QIhCT = 7; 
long eTVtkjjbnXeupNTanRWGdKqFP = 9819948673806989; 
float hWZTVPFDmxKAUoCGngcO = -89.96264F; 
short dqWVTBwizfLdnYaRAg = -26621; 
sbyte IYYGQ =  -85; 
uint dlHGwQF = 684289907; 
long aCbPEyqoglonxizxtYpcsU = 46212296843670571; 
double ytgEGjwhzTwWPYjxWtkUuHtg = 8.782672E+08; 
double ARdbVRjDhFz = Math.IEEERemainder(3, 4);
ytgEGjwhzTwWPYjxWtkUuHtg = ARdbVRjDhFz;
for( ; ;) {
Console.WriteLine(-7.72336E-34);
}string ebR = "VGYFNNwoOPHUmiNiOeqHWmtmx"; 
ushort BlgbtjKfL = 17374; 
ushort VbMmOoK = 6634; 
short UlZKkVJmMisCUd = 9631; 
ulong KDdbVqWxFk = 46699492858980833; 
float JZP = -0.0001919965F; 
uint QuhJFabaDm = 9798; 
float nGLIJ = 1.081563E-06F; 
string OMNMFKOGEtcfbyCwEWN = "JMtZXuudQPNSNPuXRoxpPH"; 
ulong SYmhileTJqWbtOOjpKQ = 26090632432846238; 
short xXbWNhZhlUEKUiwS = -10130; 
int BlZeR = 96; 
if(BlZeR == 4065){
BlZeR = 92086;
}int QtmuUfGGOceQmeMbXuMa = 766390; 
while(QtmuUfGGOceQmeMbXuMa == 766390){
QtmuUfGGOceQmeMbXuMa = 212853;
}float BuhgpVUCWbYmZzNjVoFXmGIH = -4370307F; 
sbyte eQCK =  -78; 
float uUXBq = -6.468565E-23F; 
sbyte wAabbeCEHXCVMCOLOSxgBochtso =  114; 
ulong NbBNkUEfa = 43356711237975093; 
ushort PDsPYugx = 56321; 
long QFPtkBuZqTgIRxSElQTiK = 63211223297863070; 
string LxskKhJbPQJcqJV = "tfRUHZBYURQJQmSHm"; 
ushort kzkiwDOesbTxqP = 56717; 
float CiQMWtpTsalCYb = 6.059677E+30F; 
byte chXwsTViXawzUXtxfdGKo =  26; 

}public void ocDCDuadCeeQNzfRWRNxpzUgoiC(){
double oDwXmJtPqsPWWUuPgHYalJ = 381.9719; 
double SeslCKSAQMbFzY = -2.572069E-06;
oDwXmJtPqsPWWUuPgHYalJ = Math.Round(SeslCKSAQMbFzY ,1,MidpointRounding.ToEven);
for( ; ;) {
Console.WriteLine(-7.719504E-12);
}uint BAcIPWIlTxwIoDnpfyEDn = 141224202; 
long McZnLojMCEVBCwqaTlwUC = 80807437612868882; 
ushort swYdTJXigkVOZsPEc = 15557; 
uint NMOhiZuOmVPNg = 56; 
sbyte qzqVdnJGPdmLUqiyZ =  66; 
int RVTojWlwiIXPKbLOjAKy = 53; 
while(RVTojWlwiIXPKbLOjAKy == 53){
RVTojWlwiIXPKbLOjAKy = RVTojWlwiIXPKbLOjAKy + 59942;
}short HognXzMxGcMhcmyhgy = -31299; 
float umAdiDoCXhgEIPTBJSk = -1.133427E-17F; 
sbyte ODwSdlP =  45; 
string XlcHkOWxVIUFEuzF = "zGQEqlfLCOLkohzeeKD"; 
float zdnTtBmjYatAGqdlVHaWVKalkG = 2.57956E-08F; 
uint LVOMZDCLXLSCDNqM = 81677455; 
ushort idDUYPccIOaRJApzMTqBKkJoet = 8658; 
byte AVKaIpHtTOkBAdjLbTdySZRAUiY =  101; 
byte GIPghTpQbiJfSegakhAFcBdQ =  14; 
uint kfzs = 1179; 
string ELsVpuk = "UPNhMsUYQVJNeFZdJPfKDQxZO"; 
double pSmijSJiapqqSlBCqy = -1.065183E-33; 
double oetOuQPFgEMzVwwYO = -16.45753;
pSmijSJiapqqSlBCqy = Math.Floor(oetOuQPFgEMzVwwYO);
try{
Console.WriteLine(pSmijSJiapqqSlBCqy.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}byte WkZDFdDFapqmtWkQIqACbkaScdGT =  67; 
sbyte uobFSIRtCx =  84; 
long wmDgRmFzO = 17499617714095602; 
long TbXYsqsiQXY = 19366234487324238; 
string DQG = "fSIBqTdNBPlyokPzdP"; 
uint GMjxLKZYHopkqulzLJzdUsJO = 983912; 
uint LFXOnQYuJkP = 89141231; 
sbyte kzafVY =  -51; 
ulong pyVncwUMjJncsD = 14710416633546319; 
byte lypCTGquRCzRUKqnqdykRdTW =  17; 
int gBmCzngcFfsSFsT = 8092; 
while(gBmCzngcFfsSFsT == 8092){
gBmCzngcFfsSFsT += 736621;
}uint iQuYYhFtzngjUEZZD = 65276786; 
ulong YkSTXSd = 8587384860340803; 
string iUZMUFyhYIQ = "EUeNu"; 
byte CeaoCCBqwEWBnWlFNNx =  22; 
int MZkmOucITFThC = 1642; 
if(MZkmOucITFThC == 204669){
MZkmOucITFThC = 367080;
}
}public void cEbhDiqoq(){
string gYgtaenOgoNLsHguaxholLuthXcL = "cOpSHNiwShBjylXoTKB"; 
ushort pSjMMwFOldbEheBySMUXZio = 41110; 
string AxdonDnRMVggUsFHHeAGohyS = "XHz"; 
ulong EDHXVh = 62780568608363389; 
short RDtsDjhzAsZlnpVPbFsEAX = -4610; 
ushort KoZpYPSCogdfyFtwizEizJAEBoOm = 47819; 
byte fWwTVhhCDgKYiRKFZYiqASmzaQ =  106; 
ulong bGQdmYjbMznaWacOKpMQK = 56835009386550526; 
float MsSCC = 3.121223E+31F; 
double BEnAUqDSkKpBJZZmqmiSLBeTuC = 3.19775E-32; 
while(BEnAUqDSkKpBJZZmqmiSLBeTuC == 3.526856E+28){
BEnAUqDSkKpBJZZmqmiSLBeTuC = Math.Exp(2);
try{
Console.WriteLine(BEnAUqDSkKpBJZZmqmiSLBeTuC.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}short cpKpIeJpV = -6577; 
string yazlJaBIdgcUXHepUskhuDA = "gtjJUnKGloupqmIlKJiVmQtb"; 
double xEjUtUCJfpQR = 3.080081E+23; 
while(xEjUtUCJfpQR == -1.332145E+10){
double HLZyLuEDqZiJDRfxsWBOIgsxSVtyQ = 4.086658E-21;
xEjUtUCJfpQR = HLZyLuEDqZiJDRfxsWBOIgsxSVtyQ / 3;
object sPTjyIBXgO;
sPTjyIBXgO = -4.935757E+16;
Console.WriteLine(sPTjyIBXgO.ToString().ToLower());
}int bgMinuzkcmlVnaQBtbPLLufO = 993301488; 
while(bgMinuzkcmlVnaQBtbPLLufO == 993301488){
bgMinuzkcmlVnaQBtbPLLufO += 294598;
}uint SSzRuGbmKaWeQwyLjRgq = 89831562; 
sbyte aXpoqmkNOFcxCZgOaDaL =  -14; 
string yDsbiiiTXNSRyoMWxQjpksbjkLtpl = "ujYYTNYlbFFLfJKhJzoGEVnLOfB"; 
sbyte yPTIctITzwZXkOGMAltBcbUbG =  -47; 
uint mkNlRwEPMddpbokcoJtgJZnUhIO = 903022; 
float MTMG = -4.282374E+12F; 
ulong QjQaxi = 29462511265399105; 
short nXTeiHhUU = -31881; 
short oKXuCDDXSZRMei = -31997; 
int fgOB = 336501; 
while(fgOB == 336501){
fgOB = fgOB + 941295;
}sbyte Wfx =  83; 
int dTxjtww = 286754; 
while(dTxjtww == 286754){
dTxjtww += 4180;
}int QPUEVQsIFTGuVWTwU = 991122; 
if(QPUEVQsIFTGuVWTwU == 633105){
QPUEVQsIFTGuVWTwU = QPUEVQsIFTGuVWTwU + 902591;
}ulong fdcjIIWCAfqCn = 74467818485450142; 
uint wLmEaKQjUhjQNoy = 224994559; 
long xRDTHMHkBPEZtGFHAweWSKVxud = 56190384011688629; 
string TRGRuzouphRjhJYBgNaM = "fADQaPMTAVkDXOIQmCtuCglmMzO"; 
ulong XqgqMOsdbuKDC = 50575530150509481; 
float HsuYnodC = -2.475227E+33F; 
uint EdklZoMkhKSZpYIfo = 479362126; 
long fBZs = 28706840946440880; 

}public void KCwYVbYJdXB(){
ushort oHYliyPyyElUjkYnoalX = 94; 
short KqXyLw = -29618; 
int XgPgZHBinSlXjeLQ = 41; 
if(XgPgZHBinSlXjeLQ == 280306){
XgPgZHBinSlXjeLQ = XgPgZHBinSlXjeLQ + 446039;
}long BnV = 1157295962819593; 
string RnIBcKhcjBmlHK = "nGkBzOHLMnhiQRWyhIuihgxlOBYqO"; 
long tuchuCxGIkDWlVBskmceFlBSgjZNX = 87613899131788496; 
long VMbNYFkjqmiNdLEwg = 60972362702934299; 
float SYfLFCLhtDADmWaJAOd = -4.630749E-22F; 
uint IWCKqtctWdZslkCFktEjTIqXej = 185790186; 
float JuqezPcSuPpiEznEOyXjSTI = -8.682476E-39F; 
string FGgzTziuGbgNNCtZPUK = "TKBxx"; 
byte VQMbeykzmmpyKwdIDILkIz =  224; 
float qVMUeLcHOmFTZc = -444.5427F; 
string MAxZKP = "YOL"; 
string xWUF = "DzFhCmQWhAPDGxJFshsZqlX"; 
float WsGKQI = 1.14768E-07F; 
short dzYjmHcCpmwinqzePCGxpC = 14833; 
string VjlqRQhqKGftXftoKHG = "OdokMTyQSxCNXFqxQkAxnS"; 
int tLRwlhGUfpRRVbGXsqzndt = 361132; 
if(tLRwlhGUfpRRVbGXsqzndt == 473803){
tLRwlhGUfpRRVbGXsqzndt = tLRwlhGUfpRRVbGXsqzndt + 490853;
}sbyte xVADSacc =  17; 
long HIHNBCPGCdXtpIaozHpJgNWU = 42287001116953662; 
ulong xMPGnuKUGfACjUS = 37069912989234172; 
ulong iiHaINiGTloBStWZlhQuuhzqT = 68181473578792603; 
ulong lcTYs = 28378901483178212; 
short yMVs = -4182; 
string phqSJTeJnzenLWtjyezRQ = "IFTjLpRfKqeCNMHBIdlZoNI"; 
ulong feDynYjUDBNZAhPSiPN = 33790877195122855; 
int zVXdEFlnIwtUHkkUCiVxhfXU = 43818971; 
if(zVXdEFlnIwtUHkkUCiVxhfXU == 988403){
zVXdEFlnIwtUHkkUCiVxhfXU = 505769;
}float EedGQVVEDECttoDYxJ = -4.246806E-18F; 
uint JsXczgI = 28980165; 
sbyte PpDgGdGmsUEFEBqUYa =  -48; 
string ewwLIOyPVOouPFYaR = "GQbhoeOtChmJOYdbKVgwXpizx"; 
sbyte kOWsThjLnB =  114; 
sbyte XolpDlapqNU =  -114; 
float ToIELjPEFGPtCE = -1.790911E+22F; 

}public void NPQzJdSWjeZTbboaN(){
short GWmWCtKHBiLycC = -17828; 
sbyte dWjlPICeQK =  18; 
int tOlSjFBToeZxqNzI = 348837220; 
if(tOlSjFBToeZxqNzI == 36737){
tOlSjFBToeZxqNzI = tOlSjFBToeZxqNzI + 935063;
}ulong jQoLRL = 69741926117342865; 
short IuPSkPyVcyTsPLyxYeIt = -12564; 
ulong EwT = 59870422557176625; 
byte hYWwstJuSqqsEuYgDZqT =  90; 
byte swhYAafsHAQnah =  62; 
string fIMeCbNDFojbRsoXDFZkwitMZLTN = "XZpYu"; 
ulong zsQuuxVMWqPhBqzEmQCWo = 33927221446729689; 
float xEFz = -3.531433E+20F; 
int YmIQJWLmOAwciTRxzmaluYowb = 3574; 
if(YmIQJWLmOAwciTRxzmaluYowb == 863899){
YmIQJWLmOAwciTRxzmaluYowb = YmIQJWLmOAwciTRxzmaluYowb + 259255;
}ulong ZDJIjxUxNJgUSUENiZXO = 17795343236950281; 
sbyte VmHwiphOnRf =  75; 
long iBjuPRNTpbo = 3366382043971237; 
ushort UnxLmFuLgJEOKPLFaOpEbWF = 7571; 
int ytSPOBkDolNJCmthjtMPSWK = 16861019; 
if(ytSPOBkDolNJCmthjtMPSWK == 869827){
ytSPOBkDolNJCmthjtMPSWK = ytSPOBkDolNJCmthjtMPSWK + 716051;
}double qfAhbSqUdDgAzSsldkUNl = -1.275237E+23; 
double sRsDxDcfspSzImMykY = -2.210006E+21;
qfAhbSqUdDgAzSsldkUNl = Math.Round(sRsDxDcfspSzImMykY);
try{
int RkyWoEHsRDKCkohVxPiYS = 9941054;
if(RkyWoEHsRDKCkohVxPiYS == 47610){
RkyWoEHsRDKCkohVxPiYS++;
}else{
RkyWoEHsRDKCkohVxPiYS--;
Console.Write(RkyWoEHsRDKCkohVxPiYS.ToString());
}
}catch(Exception ex){

}short gbxppDThXQmpJjMtEUOyQVC = -13550; 
string WCKATfGZGDONhBlMJTNRxoCf = "fLSYkAgpAGszyXhLKFjGRMnkdlJ"; 
uint tHZEGOu = 2368972; 
ushort HyIU = 57334; 
byte WFgNLsBdt =  113; 
int YRFgfZtURjNXJjgYtdtsQZNXbHV = 5752749; 
if(YRFgfZtURjNXJjgYtdtsQZNXbHV == 951608){
YRFgfZtURjNXJjgYtdtsQZNXbHV = 131439;
}sbyte lcpDVBtwNSjeCk =  9; 
long doeZdHfAdM = 72581382926444280; 
double CyRx = 4.410752E-39; 
if(CyRx == 2812.363){
CyRx = Math.Ceiling(Math.Sin(2));
object WJqqzJWYAqN;
WJqqzJWYAqN = -3.482148E-34;
}string XfjykHofwcyDfQnatMUbzPXckx = "oJDfnJHVtx"; 
int TNXcXjEPtembIicxhiglOq = 44; 
if(TNXcXjEPtembIicxhiglOq == 170383){
TNXcXjEPtembIicxhiglOq = TNXcXjEPtembIicxhiglOq + 872519;
}uint VFNbboBMaSIIWjwCyfGkNjouWCM = 204430; 
double Koz = -2.742379E+12; 
if(Koz == 4.982728E-29){
Koz = Math.Ceiling(Math.Cos(2));
Console.Write(Koz.ToString());
}string aQLoDosGAIAUCs = "DuppqG"; 
short SEiaiglPTTaZBOd = -8556; 
byte HylJDfxRmU =  197; 
short QJsSMuojznTHbBtGPpXdbZV = -17403; 

}
} public class adFqUqa{public void ekqhVsZXfhiscCGdk(){
uint RnNROfKMpKSFWTfWpc = 877246; 
double gVRMUfzzBgwNimstDblGJmnElAXG = -3.045448E-37; 
double blEmSjMijKSEWAxIsoTInAuiSiAtR = 1.209436E-27;
gVRMUfzzBgwNimstDblGJmnElAXG = blEmSjMijKSEWAxIsoTInAuiSiAtR * 2;
ulong UyFdQP = 8185209789666692; 
double nYgtjbbwtZREKWnfAdRemqFUNpzh = 6.739537E+30; 
nYgtjbbwtZREKWnfAdRemqFUNpzh = Math.Pow(double.MinValue, double.MaxValue);
try{
int LWulHUEUNzoYFbjmOcgYJdIOWtX = 9833904;
if(LWulHUEUNzoYFbjmOcgYJdIOWtX == 79723){
LWulHUEUNzoYFbjmOcgYJdIOWtX++;
}else{
LWulHUEUNzoYFbjmOcgYJdIOWtX--;
}
}catch(Exception ex){

}short sSohTlHoGpmWdsKVUYAduHI = -16372; 
string fIwmXEHNiknnhTqCaQDfkj = "qHbpntMjcchfYTtsJopiAI"; 
int GMTGSKYBWWeGWntuyakkZNsI = 45136026; 
if(GMTGSKYBWWeGWntuyakkZNsI == 899421){
GMTGSKYBWWeGWntuyakkZNsI = GMTGSKYBWWeGWntuyakkZNsI + 14908;
}byte bkKCBgsseIjcPNxwWVMzx =  100; 
float tkXuRhGVKwFLERUqJTgqEpTUby = 1.638545E+37F; 
short mHuRioiXCWcnttiLSfbwQDn = -689; 
short sctt = -15428; 
int WhXqMIFhLsjopWAMpGO = 877969974; 
if(WhXqMIFhLsjopWAMpGO == 368686){
WhXqMIFhLsjopWAMpGO = WhXqMIFhLsjopWAMpGO + 893854;
}int XfFiJ = 646023; 
while(XfFiJ == 646023){
XfFiJ += 373859;
}sbyte ymfkdkZhSqcNAJN =  120; 
ushort yiDHcfjCTIAnBXApBH = 54711; 
uint KEXePCwiOUz = 62852627; 
double UQkZaDsqIFQfnihfgXoNGGCmFOse = 6.084877E-39; 
while(UQkZaDsqIFQfnihfgXoNGGCmFOse == 3.672937E-38){
double SANsmoBZwhaGOqR = Math.IEEERemainder(3, 4);
UQkZaDsqIFQfnihfgXoNGGCmFOse = SANsmoBZwhaGOqR;
try{

}catch(Exception ex){

}
}int JIudpw = 1478; 
while(JIudpw == 1478){
JIudpw += 817924;
}ulong ZmKlFSNFZytoBHbkTkYoJJHjSlesU = 68574894871609893; 
byte LteIXLbUGfTiiSBpqGAt =  95; 
ulong KtjHmoBTwJDmV = 30640055784917354; 
string fTCVPDoRnXYuoOd = "FemyBGFFLqWbDcniJWdOLuLp"; 
sbyte npXjMWwcMuZDWAnXjzHnnlJPM =  -36; 
long dkuWqJqoHCF = 80792086486208430; 
int OjmJSEcHhztleYBa = 22; 
while(OjmJSEcHhztleYBa == 22){
OjmJSEcHhztleYBa += 312539;
}long GJOUPHGtfRpbgnAfnIzdlOCCRU = 49654377737487398; 
string PiRCSZg = "JATPMhUqRF"; 
long gldVALHKsSAOkpqfQXkmInTsa = 14521616333492204; 
byte DfWb =  37; 
short VQZeFwROCUtwjT = -9418; 
long hCchjlEcZnOaUVdUlsdsWNOONHM = 31736996949585027; 
int nMFxcGgdOHY = 5163; 
if(nMFxcGgdOHY == 290307){
nMFxcGgdOHY = 372091;
}double NouDAaMYeDHSb = -391481.7; 
NouDAaMYeDHSb = Math.Pow(5, 2);
object SWbUQyemfEwDMNWVxIHPEFDdyuBE;
SWbUQyemfEwDMNWVxIHPEFDdyuBE = 4.059823E-30;
Console.WriteLine(SWbUQyemfEwDMNWVxIHPEFDdyuBE.ToString().ToLower());ushort HSBZsntXywWCNBtEsUsdUI = 35270; 
string tSfGRbRjPYqqkAgAdpKHyymd = "nAqpRYqxjZwnsHueqYCblgwkEQs"; 

}public void ysVGcLDDBTXaaVbALlMJpCfAL(){
string hefNaHAZReQkHeRFzbBbq = "WAqeJ"; 
uint mwUacuzwgWuCVjuVaeFzXIzjasc = 61; 
ushort XXeFKWkFKtjsegAwVCFpZecYFWK = 271; 
long pPLaxFRGKYQB = 80244709163661678; 
sbyte beWTVfZNljTsdwHeLUF =  -51; 
float BAjWkoEHzPjKNGdDJLG = 5.135137E+17F; 
int CcsJosHqSOwFyKYULjGM = 17; 
while(CcsJosHqSOwFyKYULjGM == 17){
CcsJosHqSOwFyKYULjGM = 17771;
}sbyte TKUUUSJUT =  27; 
short SysVsbHTLR = 26115; 
ulong UDtJLJ = 22066444883773892; 
int dbf = 331171764; 
if(dbf == 819681){
dbf = 469710;
}short uoBcPgOZoolhXadyakbSpbSHPPSPb = 26682; 
uint xJTVUusN = 71268379; 
short SThUmwHGyyJsyTnaoYYuNb = 15297; 
byte hDGSoe =  154; 
long PVIYCG = 35743806079570043; 
sbyte zONVYQhUNfSb =  64; 
sbyte dHqestpxsfRn =  65; 
byte KDzBRqSQhYONkxNmiFH =  238; 
float WPZhqu = 2.409396E-28F; 
uint HgxpDlqbkRifE = 9774110; 
string TIJgqMCGKCJZzaaxuge = "xKW"; 
byte McYQltcqPWzTmouUKTL =  168; 
string RiKiAaaijMxCGKEQzhHJKe = "EhRLfNRlFDZlQgcUb"; 
short MNyLKLqUwDbtcMHfbIp = 25444; 
ushort BRYHyNJwnFqd = 22812; 
ulong xhZPSw = 55389595589463640; 
byte xjtmGMcmEkZoxcYuSZFApf =  147; 
double mPKYFIwaSVUYCBLeNkHxughXf = -2.863661E-37; 
while(mPKYFIwaSVUYCBLeNkHxughXf == 1.955708E-06){
mPKYFIwaSVUYCBLeNkHxughXf = Math.Ceiling(Math.Cosh(5));
try{
int BxgIMSnHYnxEoDehPMzldSgDZUKY = 8669545;
if(BxgIMSnHYnxEoDehPMzldSgDZUKY == 24355){
BxgIMSnHYnxEoDehPMzldSgDZUKY++;
}else{
BxgIMSnHYnxEoDehPMzldSgDZUKY--;
Console.Write(BxgIMSnHYnxEoDehPMzldSgDZUKY.ToString());
}
}catch(Exception ex){

}
}short JeITJ = -12898; 
int SipexVEgHHeVBfbChlgu = 97010; 
if(SipexVEgHHeVBfbChlgu == 205125){
SipexVEgHHeVBfbChlgu = SipexVEgHHeVBfbChlgu + 451513;
}long POmKJGOnuRjfFyhlhxFEcgMx = 69913456868536282; 
byte qTuumUqOgpcaQYXFEOTgMIb =  206; 
uint taWQXkUQfoSNjKm = 9937; 
ushort itmQNZffdcKRE = 57598; 

}public void BpRpsRfZTJNRFjUyJFxsfgk(){
byte SfQOxynTwiWyJeDctugZiCWjF =  5; 
byte CIueIA =  130; 
ulong lxlzXiaXaZB = 78712624435344768; 
sbyte jwkgsOOSPjEQNhXVCls =  105; 
byte bLnlWJaySjEJcbasFw =  61; 
sbyte PXdDEOiJ =  -24; 
double tGtBSfRfR = -2.990686E-13; 
while(tGtBSfRfR == -3.200474E-33){
tGtBSfRfR = Math.Truncate(tGtBSfRfR);
try{
int xlpoyIAAzEgnUyERzzSmAObF = 1512000;
if(xlpoyIAAzEgnUyERzzSmAObF == 93221){
xlpoyIAAzEgnUyERzzSmAObF++;
}else{
xlpoyIAAzEgnUyERzzSmAObF--;
}
}catch(Exception ex){

}
}ulong nwJitDUAPkskUbEoJkMd = 81008080776897979; 
short wVKQFSRFIbsNsCqkF = -20777; 
ushort lXkLyNISg = 17264; 
int hlPyCeyMzdDKLyimARB = 83606774; 
while(hlPyCeyMzdDKLyimARB == 83606774){
hlPyCeyMzdDKLyimARB += 700809;
}double RjYzuwMGfMUK = 4.006527E+15; 
RjYzuwMGfMUK = Math.Pow(double.MinValue, double.MaxValue);
ulong IOMnDFwsNTwBOcpSfzZmUZaGQXX = 50899363374089220; 
ulong gFDsOwxhNzYSIOn = 6203344012102692; 
long ESZuGiOCu = 57080356400613486; 
string ZpggGSwGIasIZVgk = "xNhojqmPwNGUdjIxfI"; 
double MuCjEgBcqUBNVa = -1.002805E+17; 
double fPLERLlTn = 3.051374E+29;
MuCjEgBcqUBNVa = Math.Ceiling(fPLERLlTn);
try{
int kYMdR = 8726718;
if(kYMdR == 92786){
kYMdR++;
}else{
kYMdR--;
}
}catch(Exception ex){

}int jxwSDJsebmChi = 51; 
if(jxwSDJsebmChi == 708496){
jxwSDJsebmChi = 154983;
}long KTXNCQOKWuokyROEUChTtzZpQU = 63768203465465370; 
uint yfbTEueSlXLnMtEjaQ = 214177; 
float WHUtezlCqWqkfOAnoYu = -8.931902E-33F; 
float HJmVYOd = -1.074308E-16F; 
long juZjpHHLGOXCnQKMBCRMDdzmK = 17834487908120706; 
short foeINBtmuKhZfobDAy = 29681; 
short VMBzdu = 30365; 
short UaZkFyxTdljQeECSyYLLxFEZz = -20861; 
int iKguVDQhZUKFgGuMfp = 164409469; 
if(iKguVDQhZUKFgGuMfp == 613326){
iKguVDQhZUKFgGuMfp = 419708;
}byte wpxaMwTRKuePVCOuSEn =  30; 
int kSUisuGDdLhngALZhoLoVZHpu = 4017; 
if(kSUisuGDdLhngALZhoLoVZHpu == 46989){
kSUisuGDdLhngALZhoLoVZHpu = kSUisuGDdLhngALZhoLoVZHpu + 151845;
}sbyte FeF =  -100; 
int Akhq = 6347; 
if(Akhq == 262893){
Akhq = 683208;
}float ZwpVkkEOcZbObD = -1.281012E-20F; 
uint fUFkldOJlJNUg = 918196446; 
long MMJTtCP = 71895752782080783; 
float EYROilhDZUahShSuupBTCMsPjN = 1.173845E-08F; 

}public void wdsboqXQuDBC(){
short IOTHcBVypUZqR = 8494; 
string wqNjFRJfEQVEZNZskq = "bpgSUyXOybXApPQhiPgQR"; 
sbyte clGsyDVIgAWanozTNHckdHq =  24; 
ulong FQEdCTV = 13258904046814178; 
ushort ZuPDTLVujOUKxUaEnXeedYceL = 13499; 
float JfSJKQqkZHMfIKsfbT = 8.708647E+16F; 
byte mmypObqLRISqMdWl =  253; 
int PFHlZxtHXdSDgphTFk = 412; 
while(PFHlZxtHXdSDgphTFk == 412){
PFHlZxtHXdSDgphTFk = PFHlZxtHXdSDgphTFk + 812609;
}string qYnjPUbizW = "bzTuuuIZfMaEjsaGFmV"; 
short hLfwsNnYCYSlmDNCIVAIRYCq = -7005; 
string OnyVEGQUNNYcHtaEOisXQneWq = "JZijPaoWAKecnG"; 
ushort ZOKTTnQuOhcgbAjCEhXR = 46876; 
int XOLTqALUMBhopmzDuiPwHYhUaADA = 66; 
if(XOLTqALUMBhopmzDuiPwHYhUaADA == 424831){
XOLTqALUMBhopmzDuiPwHYhUaADA = XOLTqALUMBhopmzDuiPwHYhUaADA + 806636;
}long hwzUcn = 6242949886669586; 
int WhqZGRTRWlNdUkSBBswZ = 88744660; 
while(WhqZGRTRWlNdUkSBBswZ == 88744660){
WhqZGRTRWlNdUkSBBswZ = 548186;
}double HotDfXanzNBqCIXRBmLJstNN = -41314.07; 
HotDfXanzNBqCIXRBmLJstNN = Math.Pow(2, 2.1);
int fdSJVDLmyeSkwAlYPA = 74; 
while(fdSJVDLmyeSkwAlYPA == 74){
fdSJVDLmyeSkwAlYPA = fdSJVDLmyeSkwAlYPA + 225716;
}long HCCfpAMDQhJZbFBGXSwVfThGk = 80745846724379010; 
sbyte jLsPszSKepQyFlSWZRNLNtWUjbW =  -16; 
int cZWJqUGdcxnjdowJTEnpmPnuiRP = 606957; 
while(cZWJqUGdcxnjdowJTEnpmPnuiRP == 606957){
cZWJqUGdcxnjdowJTEnpmPnuiRP = 120229;
}sbyte QBVVYpWPp =  -17; 
int HABitimJmMC = 506306; 
while(HABitimJmMC == 506306){
HABitimJmMC += 723828;
}float dVbFhXJpZpaYYGChYehjlTzZeZ = -4.464807E-09F; 
float zcwEBu = 2.024034E+30F; 
short KTmtmOEU = 31531; 
double sELHkxcA = -42.21472; 
if(sELHkxcA == -1.436328E+26){
sELHkxcA = Math.Ceiling(Math.Sinh(-5));
int[] ALXpXDw = { 8390186 , 40023 } ;
Random LxxwBokjP= new Random();
Console.WriteLine(ALXpXDw[LxxwBokjP.Next(0,2)]);
}sbyte IlnFpoiOZSCVMzSxQjZ =  38; 
int cWjGzfkNE = 805299; 
if(cWjGzfkNE == 481625){
cWjGzfkNE = cWjGzfkNE + 683094;
}long nmxLFVBPQtOZIwAcwiqw = 70705907627385434; 
byte tNfEMwgBzbKQINyOWfpy =  108; 
short oVxjTApUwDjZ = -31814; 
float JdWVL = -1.350265E+16F; 
float oeyqVPP = -5.650085E-35F; 
ushort IokmbhgonzNxUSxlBCGbFbmU = 51608; 
string ZHOYXGdxuSJTDixHLCIEAADnKytlH = "qzZi"; 

}public void JAAQRRw(){
string SIcShuCtgzABd = "oAlSQqCkTuaOglMR"; 
ulong xixbu = 58639236837906106; 
long ajpFYFLRHOJRtmpHpM = 36501007482307158; 
int ETsKtjgbwlOAMqPmMJKYkqaQo = 682474; 
if(ETsKtjgbwlOAMqPmMJKYkqaQo == 220137){
ETsKtjgbwlOAMqPmMJKYkqaQo = ETsKtjgbwlOAMqPmMJKYkqaQo + 868841;
}byte TmN =  110; 
double jgoleXVqEnatcdG = 1.104586E-19; 
double jtEATdc = 0.5477723;
jgoleXVqEnatcdG = Math.Round(jtEATdc);
Console.WriteLine(jgoleXVqEnatcdG.ToString());sbyte dhWiBQWMbkzXQGyMkZD =  33; 
uint XkCjpuaLjWqSxdnyqORYhLkdL = 8839165; 
ushort DVzdmj = 46897; 
int ACLpbDHXjtz = 66830346; 
if(ACLpbDHXjtz == 568561){
ACLpbDHXjtz = ACLpbDHXjtz + 685368;
}byte zACpFFHbTOjTDRanpmRKJyNFLXtTg =  169; 
uint XLiSYkgMuCOjaAWWJB = 313602; 
float qbMWFHlOUjmHWRIipe = -5.898227E-31F; 
short JEFlbx = -19011; 
int jLuuhfb = 49722156; 
if(jLuuhfb == 634758){
jLuuhfb = 570606;
}double EfoBkujNnjsis = -3.715496E+07; 
EfoBkujNnjsis = Math.Truncate(EfoBkujNnjsis);
byte YxlL =  24; 
int MUFgDONI = 3773; 
while(MUFgDONI == 3773){
MUFgDONI = 449375;
}ulong CExEEkdlCpQAFxdt = 74234712892488328; 
ushort wmFs = 24688; 
int WLhJaHB = 351059124; 
if(WLhJaHB == 972546){
WLhJaHB += 274810;
}string pUggHDzZTKkCQRHiWjqJLWLse = "xKWyOeZEXLl"; 
byte GzMnYQfLZLxpOXugtjOcqfPcxWfCl =  221; 
double KkLjZJMDYFyYAhZgFyFByjIAJJRc = 407221.5; 
while(KkLjZJMDYFyYAhZgFyFByjIAJJRc == -2.833332E+22){
KkLjZJMDYFyYAhZgFyFByjIAJJRc = Math.Ceiling(Math.Sinh(-5));
int[] owABDkBpRKSufqiaCxUupUdl = { 6358799 , 68772 } ;
Random WRTgDgM= new Random();
Console.WriteLine(owABDkBpRKSufqiaCxUupUdl[WRTgDgM.Next(0,2)]);
}float ZeZbJRTOtJGsI = 4.024369E+18F; 
uint DAusn = 625163; 
ushort joEgOxzPayyTMzboTxqz = 13991; 
int HIekycMKtIaIEklViTTDpckQ = 37; 
while(HIekycMKtIaIEklViTTDpckQ == 37){
HIekycMKtIaIEklViTTDpckQ = 300426;
}double GBWAJlbMoOdlLQy = -1.171704E+11; 
while(GBWAJlbMoOdlLQy == -9.45659E+07){
GBWAJlbMoOdlLQy = Math.Ceiling(Math.Asin(0.2));
int[] jWZNzhp = { 7532964 , 94916 } ;
Random FbsdubX= new Random();
Console.WriteLine(jWZNzhp[FbsdubX.Next(0,2)]);
}string AgOtPVnjOlOWGxhahaFlWwzycWJSW = "tpAZBZwDjSjXDTReNIlbQNtRdd"; 
long qSmxnTSyxGW = 23293003041816645; 
int XjaGVdHXJVxqekqFOKJ = 37; 
if(XjaGVdHXJVxqekqFOKJ == 724782){
XjaGVdHXJVxqekqFOKJ += 63547;
}ushort XHOfkcLu = 43432; 
byte tQlBOLARBtzhnnjFmodoDzTJ =  81; 
float kotRJsYqTPTUwllWRzzBSbyz = -1.340637E+07F; 

}
} public class mUXDjTVoDgDQQpXpQtuIdqDbkSuX{public void RUMewLx(){
double JbBWJPoJJ = -1.505988E+30; 
JbBWJPoJJ = Math.Exp(2);
ushort JsRFpKkmFVEJ = 8878; 
sbyte JsOAASejlJixgK =  -35; 
ushort XYwbCkyFuVkmMkByafmfqWCF = 11598; 
int gndElAPMiKthgpUeWUWBzJYfEgo = 21439; 
while(gndElAPMiKthgpUeWUWBzJYfEgo == 21439){
gndElAPMiKthgpUeWUWBzJYfEgo = gndElAPMiKthgpUeWUWBzJYfEgo + 930840;
}short qpsYNXAsOIY = -25125; 
ulong RFLmexzoYafjpL = 69029442766572438; 
int iDZqkzFaAcjSfGSTTbqsTDIPGUg = 30254173; 
if(iDZqkzFaAcjSfGSTTbqsTDIPGUg == 57052){
iDZqkzFaAcjSfGSTTbqsTDIPGUg = iDZqkzFaAcjSfGSTTbqsTDIPGUg + 19076;
}ulong CygdB = 9067829754760303; 
ushort fCylcGVjLqqSLJkgRpCpwMXRWOSu = 17481; 
long kusLpG = 67187539968747174; 
int bcudyucCtEGpn = 53321915; 
if(bcudyucCtEGpn == 195275){
bcudyucCtEGpn += 126044;
}uint YUBcEcLhpkIt = 8361; 
short IKRqRqNZzzLdqwDcfcciUbUqst = -28830; 
ushort WUVkcsBogBHhBEeqHRUIGnqPhZGQA = 64014; 
ushort tbDFQTndcN = 63176; 
uint btzPZMDiLtIEgc = 73; 
long hdsOAaBCNDRGSpQtgHhaAmVf = 87434002636206233; 
short QzdnUyozYZqfBsNOWyFLpVhmfQgA = 23816; 
double BnVBsoTMKLbo = 1.558926E+22; 
while(BnVBsoTMKLbo == -1439.001){
BnVBsoTMKLbo = Math.Truncate(BnVBsoTMKLbo);
Console.ReadLine();
}short zTgUtDkBYaUpYfgoIqQnkYAjyzwzb = -12100; 
int XeUKmSkdODckzWuVk = 87321291; 
if(XeUKmSkdODckzWuVk == 64234){
XeUKmSkdODckzWuVk += 767347;
}uint XxUhMzIxqtUkx = 79; 
sbyte BJVoLCeBmoyRBUpuppwqWc =  -112; 
ushort gEBLPMdHpEfFemVQIzjkGRz = 37873; 
ulong LuPPyG = 76153156250912876; 
ushort kqkQlcAC = 19115; 
int kipeWUJaJSCfAeEJYlSZGRuHht = 897031655; 
if(kipeWUJaJSCfAeEJYlSZGRuHht == 251162){
kipeWUJaJSCfAeEJYlSZGRuHht = kipeWUJaJSCfAeEJYlSZGRuHht + 495186;
}float osmSIUQEKsKkLMItBpyEnt = 6.09078E+14F; 
double VOsOFblOPlxfEXjcgc = 4.558226E+33; 
while(VOsOFblOPlxfEXjcgc == -2.450847E-20){
VOsOFblOPlxfEXjcgc = Math.Sqrt(4);

}uint hRkooXKqQqZkDWlKSQUitChDfJdH = 77; 
string nClwbyzc = "oLMuywZei"; 
ulong poFH = 28653565546430203; 
byte WaKQJwqSXdRxiImeCVZbTYXG =  190; 
short TjhbJAFCwo = -14477; 

}public void tLzsBPlACsODsbkW(){
double ddph = -5.846415E+10; 
ddph = Math.Pow(5, 2);
Console.ReadLine();long INRoEtffIaRYumNbRWnIesBcWQSI = 3967465342458120; 
int aKTcEWCdcdYiDkYcTomPsOB = 797831; 
while(aKTcEWCdcdYiDkYcTomPsOB == 797831){
aKTcEWCdcdYiDkYcTomPsOB += 138899;
}ulong pAcWEPbPEBTNPJhLEtPNfjOwl = 34473771326929406; 
byte KBnumSBJoAaMOIJKgsaN =  32; 
long KutOGUzWzDbykoYCVGBA = 51411909339485931; 
int ACYTFZTeUP = 554867742; 
if(ACYTFZTeUP == 752263){
ACYTFZTeUP += 191095;
}uint iRfgHlLzXNhfV = 371776; 
sbyte BAqLY =  39; 
ulong MJtJnUwcFNMisVMhfbgWPT = 3860786222914066; 
byte oxUEsQK =  221; 
ulong pPHZ = 39163354830836893; 
double wjoiHLVUDEUWaiQpqmjLDzDttI = 1.818198E+34; 
if(wjoiHLVUDEUWaiQpqmjLDzDttI == -6.548893E+10){
wjoiHLVUDEUWaiQpqmjLDzDttI = Math.Pow(2, 2.1);
for( ; ;) {
Console.WriteLine(-4.700708E+23);
}
}int HGLAPmCIiLiIl = 442343678; 
while(HGLAPmCIiLiIl == 442343678){
HGLAPmCIiLiIl = HGLAPmCIiLiIl + 545292;
}ulong lOhjdOW = 19154031526875676; 
float UgjHEBsXBqPP = 5.701619E-15F; 
int kwREoMAFnKxSacBLBGNxFCImPm = 439808; 
while(kwREoMAFnKxSacBLBGNxFCImPm == 439808){
kwREoMAFnKxSacBLBGNxFCImPm = 961807;
}long YaVpdhHGtTeBXyReoeEjnJN = 50954343129187219; 
uint leAPkD = 783262; 
double OinpfGc = -2.862463E+13; 
while(OinpfGc == 0.04382885){
double bZFhKXGtLtsodDZwpB = -1.815555E-19;
OinpfGc = Math.Ceiling(bZFhKXGtLtsodDZwpB);
object cxuumeGMPlyKBGjRAREUFNn;
cxuumeGMPlyKBGjRAREUFNn = 1.444422E-08;
}uint iRG = 32; 
sbyte ifJACaXBKlJadjfbgHih =  103; 
float bUMNf = -5.788992E-09F; 
sbyte afgFSnyEiqLQLjwRcPA =  2; 
sbyte Nqa =  34; 
int mRwoccaXcBiSjGYEgoqLpWdp = 655702823; 
while(mRwoccaXcBiSjGYEgoqLpWdp == 655702823){
mRwoccaXcBiSjGYEgoqLpWdp = mRwoccaXcBiSjGYEgoqLpWdp + 804471;
}int JHpqWwNI = 45; 
if(JHpqWwNI == 622130){
JHpqWwNI = 627898;
}short cijMpUBqOeuwFbFmW = 29781; 
byte oHILDJBThCKnqVWkG =  218; 
float gZESbschTBFJyOnwBAjHtIil = -1.675102E-31F; 
string fuIecuxodQJjHtaIXbRDI = "KgJnwARqyWntczTB"; 
int sYsL = 986968644; 
while(sYsL == 986968644){
sYsL += 519470;
}short nCcFKCwOZXzN = -9505; 
ulong npCOxwPkJiYpKmuRpbeJNpzOsfS = 54776775034458984; 
long NSYAqt = 49176085299559481; 

}public void ndiXSNxqEMQiEHmHBLMEj(){
sbyte xXcdhup =  -11; 
short UeZUQRtpBuNB = -20474; 
float XeaUu = 3.034726F; 
sbyte NZPKoWmnBRJDnMKTsmJLxVOkYjp =  -53; 
int NuWehRcJbEhzZsiDJsqabsSyzSl = 722057; 
while(NuWehRcJbEhzZsiDJsqabsSyzSl == 722057){
NuWehRcJbEhzZsiDJsqabsSyzSl = 699602;
}int oGjFT = 679183402; 
while(oGjFT == 679183402){
oGjFT += 963450;
}float cwCXYETuDAHoGz = 3.792708E+34F; 
double OumUWzOy = -2.373996E-12; 
if(OumUWzOy == 1.938216E+28){
double pDKYgNKNgEontMmhtj = -1.99288E+31;
OumUWzOy = pDKYgNKNgEontMmhtj / 3;
Console.Write(OumUWzOy.ToString());
}double yWzxdTUhDUFbJhLqF = 9.303204E-11; 
if(yWzxdTUhDUFbJhLqF == -1.897851E+24){
yWzxdTUhDUFbJhLqF = Math.Ceiling(Math.Atan(-5));
 bool? HZqSmYhuIzzRwJi = new bool?();
HZqSmYhuIzzRwJi = true;
}short DYfd = 5746; 
uint KHBVYMlJEFlGFxjOUtwuDRG = 7187; 
string YPXsBOVSpf = "gqljnXiVKHtjkKifkSbokVxnT"; 
int MSKFTIcle = 35; 
if(MSKFTIcle == 58470){
MSKFTIcle = 817248;
}short FFMIqJLaoRHkUYqJLydab = -906; 
int zdXSosl = 5508; 
if(zdXSosl == 275901){
zdXSosl = 866931;
}int iYEFQwakibUXG = 34745905; 
if(iYEFQwakibUXG == 38427){
iYEFQwakibUXG += 580190;
}sbyte LSLWGmJgp =  -116; 
ulong dUbzV = 52109189681064509; 
int kgDlowgzgTIkiseTYdS = 872962; 
while(kgDlowgzgTIkiseTYdS == 872962){
kgDlowgzgTIkiseTYdS += 291970;
}float uWapDZHzZNQBKWwVGmDEfP = 1.640929E+28F; 
short dNUYsXyKNDgDfkDUe = -25687; 
sbyte iHzwEXNnHFagYZWTEa =  -22; 
float ZjTP = -2.920523E-27F; 
short lKQ = 13972; 
ulong yoAwujidhCZEf = 89963328814837146; 
ushort GBQwluVkxkGAEdFbJUW = 2666; 
short zcdsVGfxolaKUUFPXJmqDtBP = -26583; 
byte JGFlunQcNsTAoeaV =  98; 
uint jLyTXnPZmeDonKfyQKZHwdSRydj = 243988894; 
int fokbPkMIXFoEXkd = 79973455; 
while(fokbPkMIXFoEXkd == 79973455){
fokbPkMIXFoEXkd = 215716;
}ushort zgSGxUeSNKHxZcIpldcbbgBUGu = 29482; 
ushort GtmMxMVXGpNTNDTqPdDhTug = 10660; 
short sGXacxLegqtLDqMtHOoinEHER = -12986; 
float UkletGtoZ = 1.023503E-20F; 
short JwDXeCl = -5617; 

}public void OtTjEPYujNDzjWeo(){
short YxucBYZGieGMOJQuyTkjbFqGwAPK = -2264; 
uint JOFmYfMSVjXGUOy = 872295910; 
long AldgVtfOHbjphKHWYjwRnLap = 75825330843315213; 
string ZlGHKUTqeRLUQVmSAQzQ = "OkNgRuJXJBnmALuqq"; 
uint YXHjFfndiZzLylglb = 504678141; 
float juxpyHqkTBhpWRFh = -3.126146E-27F; 
string qBYjQRbtAszRIcPDk = "hAXTpzqwBuCeaLAphKxlBHFYLB"; 
string JUcbWhZCGzj = "TwpIRwsOsqUfgYtC"; 
float hPybMHqAhTLYKKAWtfpXgpimYqZXU = 4.37029E+21F; 
uint HpubRJKpqnzdtHjGkGmk = 851781251; 
float jcwHuGielqdteUCyHHP = -2.006431E+15F; 
byte KQqKsCmKOAOSPmcWMeu =  211; 
string RBpdQEpcOWayLRcVRD = "HoUqcNjELdYpsDgYQunLJuMLRRqCl"; 
float mRSwnWjmqHZFZQGIUFSmCDSpm = 2.437504E-25F; 
uint wIsSmLAB = 84; 
string gXeKPMVljxXuGoLZDhK = "LcWYIMMefgNMPHwGNHySoma"; 
double YlYaBYZhpVdagbdZQfcKkF = 3.912398E+28; 
if(YlYaBYZhpVdagbdZQfcKkF == -8.546797E+31){
YlYaBYZhpVdagbdZQfcKkF = Math.Ceiling(Math.Asin(0.2));
try{
int hXoAumeVncooSycd = 1055464;
if(hXoAumeVncooSycd == 46498){
hXoAumeVncooSycd++;
}else{
hXoAumeVncooSycd--;
}
}catch(Exception ex){

}
}short moCNlQHMIdtToYEwIMYo = 22573; 
sbyte ufcpDEjPzsxblYTeAunJaoUaofOJt =  -4; 
double fqgHjBdhznUBSXqxOHzVEAYe = -7.117986E-36; 
if(fqgHjBdhznUBSXqxOHzVEAYe == -1.364536E+23){
double HVVSlVZpgjMuu = 4.441222E-09;
HVVSlVZpgjMuu = Math.Sqrt(3);
fqgHjBdhznUBSXqxOHzVEAYe = HVVSlVZpgjMuu;
object UNadlGtbxhzHq;
UNadlGtbxhzHq = -1.2521E-12;
Console.WriteLine(UNadlGtbxhzHq.ToString().ToLower());
}uint PmTzlWptNsbCzPqgqGmPY = 338965389; 
byte TRMSPMXYVJcslFXtnMmHxQGYwzjBL =  180; 
double KBseECYHeEDfoPfaoxKfeECdlOy = -1597.89; 
while(KBseECYHeEDfoPfaoxKfeECdlOy == -5.304847E-23){
KBseECYHeEDfoPfaoxKfeECdlOy = Math.Ceiling(Math.Tanh(0.1));

}sbyte AYwPIeztfpxgLmMLx =  -40; 
sbyte REIPxImlVfeWHE =  -41; 
double xpfzWyxDUnyuRykMKKVqHtZRlzZlA = 5.416174E-30; 
xpfzWyxDUnyuRykMKKVqHtZRlzZlA = Math.Pow(2, 2.1);
Console.Write(xpfzWyxDUnyuRykMKKVqHtZRlzZlA.ToString());ulong SdgTNQRdAK = 17137476090752444; 
long NjxfIPsjQGy = 67577265437956238; 
short jLoMkEkMDxOFwB = 9303; 
int lXz = 26428848; 
if(lXz == 780653){
lXz += 520042;
}ushort ZEWNpU = 36369; 
uint uxlhzmi = 11953336; 
int lMBWATPWQbXKgeMZBbnBXhAIPLqia = 57516584; 
if(lMBWATPWQbXKgeMZBbnBXhAIPLqia == 455110){
lMBWATPWQbXKgeMZBbnBXhAIPLqia = lMBWATPWQbXKgeMZBbnBXhAIPLqia + 893033;
}byte Zmxf =  158; 
long xMfdNn = 48286663422214470; 

}public void ODHWfy(){
short JjyWCORuqztkdGnwMcWQVx = 19917; 
string SAUAKEJh = "yTpsgUaIKHTIdiokjcCzRmuxMKnXV"; 
int LddiufuWbQmMjhDOqGnBBgJijAU = 44553207; 
if(LddiufuWbQmMjhDOqGnBBgJijAU == 744388){
LddiufuWbQmMjhDOqGnBBgJijAU += 61140;
}long FaZUfQg = 47347485120505009; 
double IguEuUYUlNzx = 0.486417; 
double hKuHaEKoGyCIMeWqFkgaZU = 1.653435E+19;
IguEuUYUlNzx = Math.Ceiling(hKuHaEKoGyCIMeWqFkgaZU);
int qISQMunNKpVKfGVA = 300481; 
if(qISQMunNKpVKfGVA == 782973){
qISQMunNKpVKfGVA += 262765;
}ushort RxcoeFMWPl = 51753; 
ulong RFpaUbljPNXwLRNiuMdmEYosYJf = 46665415721817449; 
long kUlidcKfUtOYNwALApS = 75511573514668387; 
float lffbEi = 4.115373E+26F; 
string WGldosGlAufKoTxH = "JpBShRYJ"; 
byte XilITwQWJMXWEbZwzbCR =  63; 
int YhDEIOOmwbLJcmMlnzWNUmNMATMt = 135563990; 
while(YhDEIOOmwbLJcmMlnzWNUmNMATMt == 135563990){
YhDEIOOmwbLJcmMlnzWNUmNMATMt = 215898;
}byte qSihVACUTtWhMPms =  80; 
float MwQd = 6.881359E-36F; 
ushort nFooSZlVxpP = 55417; 
ushort TTfWFIiTeeChCfBOufBjno = 37520; 
string hATRc = "ZJbCXXSyNguBkOKzg"; 
ushort WJqlxEocRxhpcbDCzAEE = 53588; 
uint OcTOtpuJtzigTIyFYIVb = 59; 
ulong AIllZERRuaEi = 55666728711451712; 
float KUbVFVVBIimCoLdiKQHqMSeobiM = -687891.7F; 
byte YzEYsjeYWWUtwWbBlJndZkCi =  215; 
ulong TxjMWusncLNMZGSLwRzKtZJ = 18828873563036593; 
sbyte ykCMjYYcIZPAWozhB =  -94; 
short fuUOKlGkCAfpH = -20714; 
uint pYkTAILaTbFbL = 26713141; 
ulong Nycq = 47513304088067284; 
uint NMdRCWzTzKUVJHFBSlpmTMAdDV = 5128; 
ulong DgtiSo = 84036035032912292; 
ushort uqOnJWDmZamZoGKXgqgNCBPoSZ = 32305; 
ushort aNfHKCUNzzGEgjScJEgi = 41496; 
ulong lKsUDzwJxfFYmgsGeQkJ = 33736284524658133; 
ushort oanfUXqIsndP = 11810; 
uint kFVpYctwdPLBXEFll = 690; 

}
} public class FKhmunxyXFdsKTfssTyqVFkpi{public void udNcSCfIXByIWL(){
int zzMtRdylTsQNVZAReYTsKVNDlcW = 90070455; 
if(zzMtRdylTsQNVZAReYTsKVNDlcW == 745999){
zzMtRdylTsQNVZAReYTsKVNDlcW = zzMtRdylTsQNVZAReYTsKVNDlcW + 715182;
}double MFPGqH = 1.988384E-27; 
while(MFPGqH == 2.484661E-11){
MFPGqH = Math.Pow(double.MinValue, double.MaxValue);
try{

}catch(Exception ex){

}
}byte dxhE =  37; 
short RHBsXZNjjCzTqVRtGXLMDdt = -9688; 
string gVJAOKQBCIeXEddnh = "apVgmcDGxDjOmiqwTSCKOPc"; 
float ioJtIJnVHqLGZ = 3.622816E+08F; 
double mnpNuWG = -4.602093E-24; 
if(mnpNuWG == 0.0005002958){
double NdNUmOU = 1.63186E-21;
mnpNuWG = Math.Round(NdNUmOU ,1,MidpointRounding.ToEven);
for( ; ;) {
Console.WriteLine(1.016911E+27);
}
}long nHEcKL = 65035471595502905; 
sbyte JNowaXgpzkUiV =  -124; 
uint uSdKAQaJIczuohPhieLHKogq = 257966; 
int QUAelkTjQyUSNGO = 490809692; 
if(QUAelkTjQyUSNGO == 322151){
QUAelkTjQyUSNGO += 425271;
}ulong RpTfeLNjllCSNDtienGqMjSUdCiRN = 44032276322760080; 
string zcnJHlObwfFXcRwnIyZbEDVy = "YzPRUFQZAgnyBlIpANLhoklIIassc"; 
byte gQJIOZjCBPXDKhQgksMdPkoLOlhj =  223; 
byte XnjgIRFTqpljtsI =  137; 
uint TgaphhWCYTCSZInqZnAaioHzw = 55155240; 
string tfKHsZfxHgckfTLsApEWcsubBJF = "DVuFWGGORkHIhUth"; 
ushort cxVfWyYdTaMIgKwGULNynBmX = 33337; 
short WPhYmFLHhFxZWHSMFbjdX = -17073; 
uint IuZ = 546105; 
sbyte GIOcoLcAdxcnayMQRwOJWqkLKmhp =  87; 
int dZHRqhE = 15082; 
if(dZHRqhE == 113277){
dZHRqhE = 643908;
}float CDWFifOBbnydtcEMIzmZouHwImSNg = -4.560599E-23F; 
int LMFumwcpAFDTekCyQUHuVKgW = 942363; 
if(LMFumwcpAFDTekCyQUHuVKgW == 16147){
LMFumwcpAFDTekCyQUHuVKgW += 379263;
}long UmJp = 34103577167676576; 
string DcpppBEiUaFJUpBPo = "XAYSC"; 
byte zAFjlUsTYuSXLdg =  124; 
long IMpyeDwuKRFD = 32473192311312292; 
long lQVWIR = 7999208860740988; 
sbyte ubO =  85; 
byte nwJqSTlfANKCABDiV =  148; 
ulong aFplHHJRHYkRWLCAmpZL = 44575866183053188; 
uint gjbCBJfyK = 2160; 
ushort CCTGOYdVFFfTpeQx = 18631; 
long suSaQukSuPGOAulZYXNkKwpCGVJjF = 45030270389741770; 

}public void ckEfJgYQHzefRzCx(){
short dpwUMbndOjknsgpyUCIEMjEPojfA = -1681; 
int hVeateizDSGsOGsfXPtFaOMlTN = 60867766; 
if(hVeateizDSGsOGsfXPtFaOMlTN == 787013){
hVeateizDSGsOGsfXPtFaOMlTN = hVeateizDSGsOGsfXPtFaOMlTN + 603693;
}byte dsmgWsLcfMg =  214; 
ulong WdiKfXTt = 54201301583086809; 
string hyCaTQTaUXJZfbXxKyNeBmyLiuZoU = "fzJgKqI"; 
double sRedWKjN = -5.67465E+13; 
if(sRedWKjN == 0.1866333){
sRedWKjN = Math.Ceiling(Math.Asin(0.2));
for( ; ;) {
Console.WriteLine(1.920495E-17);
}
}int sCoYLAUayRyBeTgEFit = 4; 
if(sCoYLAUayRyBeTgEFit == 858776){
sCoYLAUayRyBeTgEFit += 985818;
}byte FpKVTUowkNnwOGJITbpPWPtKnSAE =  206; 
ushort ZBDpKOWEGQzWQp = 50905; 
int sVlRJwZPWZHbwLGP = 2903; 
if(sVlRJwZPWZHbwLGP == 298163){
sVlRJwZPWZHbwLGP += 979240;
}float yKYzTEtAmZkVjGpeDBOOoDkeSgAb = 1.415277E-09F; 
byte bpuYxCVtHhOxB =  178; 
int YPJPkfHUAUGNdsGTORUUls = 3262090; 
if(YPJPkfHUAUGNdsGTORUUls == 140735){
YPJPkfHUAUGNdsGTORUUls = 143372;
}ulong FiDOceLxhVJAADVionDVIJQL = 16364633201247171; 
sbyte pykooChsJYkMSggxeTFSgV =  -40; 
float wXemqWjsOm = -6.637708E+34F; 
double bOpZRBNOaBMMfUnEuftaUEdescate = 2.682034E+13; 
while(bOpZRBNOaBMMfUnEuftaUEdescate == -1.007813E+30){
double BqCIWFBGRpekSsFYeyLkIPAuKgR = 7.32691E-33;
bOpZRBNOaBMMfUnEuftaUEdescate = BqCIWFBGRpekSsFYeyLkIPAuKgR * 2;
Console.WriteLine(bOpZRBNOaBMMfUnEuftaUEdescate.ToString());
}ulong zSQlIVDPeQeAuAwjP = 69150437965836423; 
short HMtxJFqIXqhYqicchLnTAjP = -24559; 
short TqgEZB = -11669; 
sbyte cMtwVcSzTIiVKipgfzynxWlfDLaC =  -33; 
short IilZpOpsQHLWAhlihQxT = -30480; 
float AfAmTjcqeCZBMziasQfIIMaNYq = 8.819791E+25F; 
long zGOoMTQByiOoGuAHALYQddWllOfN = 18714976589448923; 
ulong MpjStWUsfGmqRQ = 75506209369392673; 
ulong lVBE = 9167256056378756; 
int KZsUJAAXK = 323341503; 
if(KZsUJAAXK == 653720){
KZsUJAAXK += 683791;
}byte NCfHywqJIngyP =  210; 
byte HLLkionFnk =  174; 
byte hKxLJlgAGmEbkblsBVagxJXZYlznC =  90; 
byte eLAFLNgFdPazgjXncmNpJf =  136; 
uint yMxOTYmaRXBCHIOPwcHJzS = 27391898; 
byte nGWcYiaS =  62; 
string RjQNzomyMwGZBndURnGXi = "JKIdPYqCPuNeFTRVaD"; 
uint yPJGwPVnjFIdCmCfeuBngkHkl = 673418; 

}public void dVmcwqgT(){
string JZdc = "jNKEsqLQHcnK"; 
byte ztZfLjn =  85; 
int jfJecMCjdTyyDSqStfHa = 63; 
if(jfJecMCjdTyyDSqStfHa == 563002){
jfJecMCjdTyyDSqStfHa = jfJecMCjdTyyDSqStfHa + 667500;
}long tSdShaulJGNUBdHtLaRhPSQiLF = 50816297210449406; 
float PFoCzejO = -1.8162E+17F; 
uint aBZD = 534065183; 
string cVhEARZLdtSTlwFgL = "JKIcRYezCM"; 
double DBEpiTxMcHeAKYTHRFVSRDSuTZeiz = -2.10403E-16; 
if(DBEpiTxMcHeAKYTHRFVSRDSuTZeiz == 1.776223E-31){
double mLKZmcGitSezHXQghZIiyiPmmasR = 3.026105E-13;
DBEpiTxMcHeAKYTHRFVSRDSuTZeiz = Math.Floor(mLKZmcGitSezHXQghZIiyiPmmasR);
Console.Write(DBEpiTxMcHeAKYTHRFVSRDSuTZeiz.ToString());
}ushort HoTjwon = 46591; 
sbyte faoSB =  5; 
float NRHJhSMMBsmAFLwmhxKY = -4.984442E-15F; 
short kGMuzKdIxY = -16629; 
int YhbzJOWyGzwGcLdRT = 4315; 
while(YhbzJOWyGzwGcLdRT == 4315){
YhbzJOWyGzwGcLdRT = 526246;
}sbyte nRoFdHySLjn =  42; 
uint DxQFhpPxZZRDIQcjBHVh = 942919; 
int DSXNqBPitKTdDTuQCSRpOBZIPC = 65111631; 
if(DSXNqBPitKTdDTuQCSRpOBZIPC == 243291){
DSXNqBPitKTdDTuQCSRpOBZIPC = 610404;
}ulong boscezjJiXWZHHmeGDfpecTpXQSh = 52078372039265653; 
uint OsgfqbLPUcwJNKAz = 984258611; 
ushort enHb = 17595; 
sbyte JMcPhHfPNjYLRGhWP =  60; 
long NOPLSJwwAM = 35625419073375782; 
byte HUMo =  230; 
byte xWsRpDUOUWsWAlLpbf =  35; 
int EedDzKHSbBQ = 74752; 
if(EedDzKHSbBQ == 129988){
EedDzKHSbBQ = 894104;
}ulong oMhhBpOQWF = 22238733004728830; 
short SmVTxCswmligqSnFxWEmLTLnq = -10593; 
string QeHSGXxbRnLYYtEKwERVyJyVLc = "ZDIDeA"; 
uint hGqCtwlOhlGospaSNBg = 159317; 
long hESyAjMHzAOweeczAlUoKe = 21502377713306635; 
string BUiXCYMFxFgPEU = "OKbHjnsbcQdOXwWcfwqVjVeWCAp"; 
float YYjPG = -1.936658E-31F; 
int UihTghzkPjADatmYMTGffK = 475975998; 
if(UihTghzkPjADatmYMTGffK == 848231){
UihTghzkPjADatmYMTGffK += 872342;
}double uRAuCiOpPXz = -6.445293E+15; 
if(uRAuCiOpPXz == -3775.867){
uRAuCiOpPXz = Math.Sqrt(4);
try{
int tgnVhym = 4986190;
if(tgnVhym == 32803){
tgnVhym++;
}else{
tgnVhym--;
}
}catch(Exception ex){

}
}ulong GzcSQSltFFVbJeaDpaSBfgcqZN = 88563670655360530; 
uint jAlWfGBPSHkZmPwAzlPAxMSdgBSlL = 50; 

}public void EcsQOWuAGnAsDLzMXtqo(){
byte CzHi =  200; 
int odNgExkxGLCRoTilBCwcgnq = 22116871; 
while(odNgExkxGLCRoTilBCwcgnq == 22116871){
odNgExkxGLCRoTilBCwcgnq = 885826;
}sbyte hQNqjNlYonosJQyYitm =  -94; 
long DHaMZViVKDxf = 75346892633723647; 
sbyte RWJXOxshIcYOmgqyOqQuENVD =  -23; 
ushort GuOSemOPddbgMD = 35778; 
float gGPwlBLXNMkea = -1582.499F; 
short MNU = -20823; 
float zqmuNjdSbSCxQHauT = -3.62651E+31F; 
short DKcXAOMQPakmdpeZg = 30827; 
ulong TjmAKMMasTRnDaWIjxmwq = 18806369670076726; 
ushort ehJItxEnNcMyWjWLduxPKLq = 22282; 
float xhBawdXfNfUtDkHiBtGFpHwzJWAH = -1.011458E-10F; 
float QwaxNNIIBQy = 7.771872E+23F; 
long liBkYMmIPNbqzFqctoEDhjkxO = 61348232858808030; 
sbyte yDiSFENkJ =  52; 
ushort djNdCmh = 44675; 
string eynOihGitttsTBkxYZuZCLsCUUNg = "wRxk"; 
uint LFRuGFBqcRLqmSe = 1438; 
short xteM = 24293; 
short AgYCWgtcBShGO = -7400; 
string pLiEBnXhKghde = "SXelfutBUEXjsmgCCJw"; 
double aKLTkiRSQZpYUZjBCoIbPRGG = -2.400679E+32; 
aKLTkiRSQZpYUZjBCoIbPRGG = Math.Ceiling(Math.Tanh(0.1));
sbyte aYezyegHdVgRIinQFNMXUca =  -38; 
ushort ZUQaq = 32038; 
float dhzQaLEEyMNmBOS = -1.085924E+22F; 
long jTYXMdtfmQdEHYjegsPNbup = 29318580512696794; 
byte NJFfjNMHFqOthgnpJMaa =  238; 
long DsIgWPUksTWUoGZWFcfxdDlg = 76099189122246313; 
string cNDGQRLfxBllZLJPwYRpMZgBIc = "sVUJkTwEluZlnLXgM"; 
double UDTBblULh = -1.574288E+10; 
if(UDTBblULh == 6.764477E+19){
double RatFhywxMDFOTuFwia = -1.223482E-11;
UDTBblULh = Math.Round(RatFhywxMDFOTuFwia);
try{
int qhmCGsbzZsNFUVT = 5820388;
if(qhmCGsbzZsNFUVT == 28860){
qhmCGsbzZsNFUVT++;
}else{
qhmCGsbzZsNFUVT--;
}
}catch(Exception ex){

}
}float JuJksGRcwnDlaoAKDUFcpnXZ = -4.652841E+27F; 
short WAUKzXWBlJXP = 6225; 
string lPUtUNNa = "KRAwyKiAmzgVtSNdsZyTX"; 
ulong ndLjgAzGiAjaHyC = 37932699324343003; 

}public void WhpCOogNTulsMAcZOSYxiSgGRW(){
double AoqsPSnZjXST = -9.202383E-06; 
if(AoqsPSnZjXST == -2.188682E-22){
AoqsPSnZjXST = Math.Pow(2, 2.1);
try{
int cQRZeeMLUu = 4234942;
if(cQRZeeMLUu == 80671){
cQRZeeMLUu++;
}else{
cQRZeeMLUu--;
Console.Write(cQRZeeMLUu.ToString());
}
}catch(Exception ex){

}
}ushort WoSOXtgpclCTmdbzHNcd = 20552; 
int GBEiLcajdBtFNDijQSjjPYLVzhTaJ = 32682966; 
while(GBEiLcajdBtFNDijQSjjPYLVzhTaJ == 32682966){
GBEiLcajdBtFNDijQSjjPYLVzhTaJ = 247699;
}byte jwmbczsTAnVyD =  63; 
byte uiIPIL =  142; 
byte Bpi =  8; 
byte pVQGxXiNBsbZUX =  140; 
sbyte ekYhGzMAScaChjSm =  83; 
int NIQjkxgxHMJlzuQeQ = 2940; 
if(NIQjkxgxHMJlzuQeQ == 75018){
NIQjkxgxHMJlzuQeQ = 121001;
}int hlsijNcsatSobNMzcfNjRDhsfb = 537239517; 
if(hlsijNcsatSobNMzcfNjRDhsfb == 678280){
hlsijNcsatSobNMzcfNjRDhsfb += 521259;
}byte djPmxZHUybX =  200; 
ulong EluYtucFdadiOBzW = 66588007622968489; 
long EWfguplzYIKLiSmAgOimlfx = 18224088711310222; 
int FRgAXHNWoD = 223610515; 
if(FRgAXHNWoD == 218187){
FRgAXHNWoD += 484475;
}byte UGxDyBEPOFiHV =  23; 
sbyte EagZihxqWDGpsy =  1; 
int IJYSbtGViSiOIHLgDGkt = 2320; 
if(IJYSbtGViSiOIHLgDGkt == 104766){
IJYSbtGViSiOIHLgDGkt += 619226;
}float nxqeFanxgECIBdZpmDnWUjQRpkx = -2.643134E+31F; 
ulong wAKINCdiItoaZdESglxWDsY = 4424797005513790; 
uint QfhRxmPjuHDG = 81; 
ulong tpjASkdBKDfboPdCOIiXwbK = 27594869595895794; 
ulong bamJbAGcIgUgNUGDbSCIQs = 3630619090226161; 
short qpnORTMhEVNUPRUTCiuD = -7284; 
sbyte KJHyjwDKDRmQyHSMifteVFyRtXk =  4; 
byte NmiuEfjwCNblkcDaxmKykx =  6; 
string fkQ = "mkbBR"; 
string IdcMCOEGcJuwnsBYRMBplfuzVuqnD = "MtYEttjYVyTwaqLQCLNlkOpOOCKFR"; 
sbyte odcXaxLZhOSQdx =  80; 
sbyte NfTG =  6; 
short XcjPRJFgOaEqXuLtQMGzsXmq = 29149; 
ushort SMoROOnZXmVhziLcdTAh = 43581; 
string cTWRyaQSSucNwAjQDKuNgV = "goHVjzyoYQZmXRWQZh"; 
ushort Nxxp = 36380; 
float Wyno = 2.324241E-29F; 
short DQeGtwfdXxmmZGTwGMSQLTqs = -12326; 

}
} 
public class LuMamjStLlBUVGXuykyfAqbIcsDzo{public void jbQQCfZADgziUExhyUW(){
float lQlBusdXVHtdfcAeedSwXJpHp = -0.0002281039F; 
byte VIDzNXlEphNVCIBfeOBXcsRexUE =  126; 
int XUSdaYRG = 1091; 
while(XUSdaYRG == 1091){
XUSdaYRG = 928947;
}sbyte xaQBkJXnRDRzcStM =  116; 
sbyte SczICoAw =  -24; 
long EUYWzEKeZnXFlygtE = 28651188384185201; 
double olMnCEoYXAfu = 5.8976E-35; 
double uVWUJiZUi = 0.01262423;
olMnCEoYXAfu = Math.Round(uVWUJiZUi ,1,MidpointRounding.ToEven);
object uTnZwaWAjU;
uTnZwaWAjU = 1.822172;ulong GUGIZRXAypLkbzPMLoptuDP = 70227506004715069; 
ushort gSlqQKNSSAwzJ = 18061; 
short gJaPwtcfEhysqWRKpQV = 8791; 
int VGDCCctDRniaxX = 61548013; 
while(VGDCCctDRniaxX == 61548013){
VGDCCctDRniaxX = VGDCCctDRniaxX + 121936;
}ulong mwCVJLiTtKcLOulTSaMa = 9311088980088904; 
ulong SxJlsha = 19681254382058712; 
long lugVPuM = 57567500685257053; 
string RGcCbdTm = "dHsmKqPLSpJmcbWUnWP"; 
byte aWUKItJIjpnJiqfZRCU =  155; 
string iuYPEgIOhdcadLzEcYgdIZwF = "LRUywVFCjNALTPDZxijGhnwCnz"; 
double ahiZuVToNpMiyAT = 1.923371E+37; 
double lYLyxtjWYfBcEBpXWmlfxahJu = 5.822939E-34;
ahiZuVToNpMiyAT = Math.Round(lYLyxtjWYfBcEBpXWmlfxahJu ,1,MidpointRounding.ToEven);
Console.ReadKey();ulong OBtSZHQQKEdcTcBCfPXCHhtVDs = 35611386443711862; 
uint YZhOxZsAUPVGLLzqolnHhz = 6342; 
uint LoTWplEPDgIieHPTaOxCnuWtT = 380892; 
float BKBoGndHYzKcWhsKfg = 2.063541E+25F; 
short CjXDqtAdQSLkcyg = 24697; 
uint MmbpNAqlEJNu = 1862546; 
short hpqc = 29150; 
long VJSqMf = 6543428915839719; 
byte zkeQQpxeKVaoQD =  110; 
ushort ffH = 59717; 
sbyte fIfBNzVnFWTSSSUpFmN =  34; 
ulong HNKkS = 33029449042550756; 
sbyte QKGPIwtXKFEhQBuiBPabt =  77; 
uint RsnqsClTMGPEgSiLmPa = 28791894; 
double QjtQQY = -4.397081E-14; 
if(QjtQQY == -5.963454E+37){
QjtQQY = Math.Pow(5, 2);
try{
int JHpPE = 9230472;
if(JHpPE == 7960){
JHpPE++;
}else{
JHpPE--;
}
}catch(Exception ex){

}
}short hiIqNklIdeJ = 26054; 
int VOPVXxVPOMEuXH = 37047270; 
while(VOPVXxVPOMEuXH == 37047270){
VOPVXxVPOMEuXH = 643430;
}
}public void mmoCEizQOVpEwsRMag(){
double twWOtnFWKPofZT = 5.163269; 
while(twWOtnFWKPofZT == -6.039347E-11){
double RmQCo = -3.812147E-06;
RmQCo = Math.Sqrt(3);
twWOtnFWKPofZT = RmQCo;
try{
Console.WriteLine(twWOtnFWKPofZT.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}float akclaFFuIAhlPylbHFnCcAEWQ = 2.22735E+29F; 
sbyte yKafDuTzpM =  55; 
ushort SDfDUsXWEzwtPCIgxhLye = 12700; 
ulong fldtDpT = 72047156589176004; 
uint sTlFNNSl = 8823; 
int ZFJ = 96765709; 
while(ZFJ == 96765709){
ZFJ = 586985;
}ushort GTnxUGpij = 11638; 
byte pEDsAlglxoCePYoSRpmFYGH =  184; 
uint XHbLuRLRsiw = 17; 
string hRpsiIWpzqUWJOL = "RUhnAjLVKuXHfxNgjgQPptpKbpDt"; 
uint ccIlFjaPYYfuIPKpkcFNMCYVHyF = 243081; 
sbyte qwPqUZTSDfuiSKWMBZLf =  1; 
ulong iBeMGgGDDPpOYTzQqqkcHst = 15761068890823891; 
sbyte JGQQmnLWESuCVXScj =  -27; 
ulong NgRhYBVwDcb = 68374138397792216; 
string fcAYFpNTbpgoRQCP = "RMsouCRJsKHGujqLQHdq"; 
sbyte jhCBNUxsxOyZi =  -42; 
long LCJpmHfn = 33624985370177643; 
uint CjOTOWlZmThwaZa = 92234661; 
int SgEcnUHeTqnpaBLQBxRNN = 25; 
while(SgEcnUHeTqnpaBLQBxRNN == 25){
SgEcnUHeTqnpaBLQBxRNN += 216803;
}sbyte ExmVPHqYhAZgRsJZDhVojAieP =  107; 
ulong jPhJCnpVbwkUuRtXSGVNFuLYpQ = 29295841473782110; 
ulong qpGDbOYgWaVs = 65179526124144275; 
string sMHlITo = "bXcZGCasLQwE"; 
string UoXbWWSRhYo = "XTVQgfR"; 
int XogGKiIpAuBgdRsoGXtBjQeLzEgxV = 31365293; 
if(XogGKiIpAuBgdRsoGXtBjQeLzEgxV == 306771){
XogGKiIpAuBgdRsoGXtBjQeLzEgxV = 944676;
}double xlNkCIEOaUcEBBizdQhkwhfbk = -1.116116E+23; 
if(xlNkCIEOaUcEBBizdQhkwhfbk == 6.077625E+11){
xlNkCIEOaUcEBBizdQhkwhfbk = Math.Ceiling(Math.Acos(0));
object wofLZfSAZthhjcqySOgaKzKy;
wofLZfSAZthhjcqySOgaKzKy = -0.7556397;
Console.WriteLine(wofLZfSAZthhjcqySOgaKzKy.ToString().ToLower());
}double JOIfkDAfUdGZwSXzFL = 77.22739; 
while(JOIfkDAfUdGZwSXzFL == 1.529589E-37){
double eGUpBfpWgJ = 3.588162E+11;
JOIfkDAfUdGZwSXzFL = eGUpBfpWgJ / 3;
Console.ReadLine();
}sbyte JZIidAHfZAqVI =  -33; 
int nqsAFuidWNcUBa = 75; 
if(nqsAFuidWNcUBa == 121003){
nqsAFuidWNcUBa = 173053;
}double ahH = -2.482708E+08; 
ahH = Math.Ceiling(Math.Sin(2));
try{

}catch(Exception ex){

}ulong zqVNzQcHwecAHBXgg = 33911772947048597; 
ulong ots = 77630905841043581; 
string nfuYpcunSgsFVGAoaDeJ = "NmdjjyzntFmRYTWOscGwMymZ"; 

}public void XxJXEYQsRC(){
float OfGWZzCfNWM = 12.4131F; 
long AndpaeVlbPpQGxAg = 47526742438122644; 
double TCbKgxVzqeOlSmTTEhJJWJ = -1.704745E+22; 
while(TCbKgxVzqeOlSmTTEhJJWJ == -4.165288E-33){
TCbKgxVzqeOlSmTTEhJJWJ = Math.Ceiling(Math.Asin(0.2));
Console.WriteLine(TCbKgxVzqeOlSmTTEhJJWJ.ToString());
}string mbBafZcZEdmneVk = "hjukRoDaItPZzxytQQkTc"; 
string KiBkJCoPCeBMj = "RJYAAt"; 
long cCgHUyoL = 5406344937442239; 
float sFmUXKmkZhltDJJ = -3.207865E-32F; 
double cISBR = 4.02525E+28; 
while(cISBR == -1.110092E-05){
cISBR = Math.Truncate(cISBR);
Console.ReadLine();
}string sDUnqhmEPEIYVqiS = "MdtnV"; 
uint qOwVpgBcWUpeefckB = 3801; 
ulong UfFItwpyImybtWENsmJ = 53450241720947241; 
long ScCPWyIQnXLjGnITdLjAziYLKa = 49420313094154792; 
ulong VlxzVWdp = 52235267348819585; 
ulong uJeQfVGD = 65651743992135915; 
double oLECeIOQcdMwDDGyjgfffBTtJ = -1.364707E+36; 
while(oLECeIOQcdMwDDGyjgfffBTtJ == -9.192766E+32){
double NeHTFxcoT = -8.639179E+30;
oLECeIOQcdMwDDGyjgfffBTtJ = Math.Round(NeHTFxcoT);
try{
int PJeSssSMP = 2232482;
if(PJeSssSMP == 8012){
PJeSssSMP++;
}else{
PJeSssSMP--;
}
}catch(Exception ex){

}
}double tyYmwZTOXQONZIPZh = -448.3882; 
tyYmwZTOXQONZIPZh = Math.Ceiling(Math.Sin(2));
Console.WriteLine(tyYmwZTOXQONZIPZh.ToString());short dWICNHfmaEkmiqDzgnlSPjtwiopzE = -17479; 
byte iNqqNARbme =  153; 
ulong TAARdVLRzj = 61936406010764179; 
ulong WexZlMIykG = 60310423172175985; 
uint DxKDyOBhsLG = 825271; 
ushort OdAFqnmHTAoW = 8635; 
string zZiyMBuTCzlAfAZoOjNCL = "cidpQRdiRtf"; 
long HmtLStjYg = 4337352103187571; 
ushort XhXZuxKEQtHTuxls = 2065; 
ushort ViCtVeohIXSHfZG = 47631; 
sbyte nZohQtGBluMVCihnzqxPb =  104; 
sbyte bHYMCTOKgJ =  68; 
ushort NthNwSUw = 49351; 
ushort zRCnoqIsUagiwTYtGbOSw = 49514; 
string YobHdkTokonKqBwQbxBjHj = "UxzotZDnMOQoaoboKgRYgG"; 
short fHXR = 31696; 
byte ePDhjFVAoIaDVBkuCagRcPlGnh =  108; 
byte bMozUMByhRKOIDpfdtCUmx =  207; 
float RpDXTciMNnWLdb = 2.130141E+16F; 

}public void tiycwmWAkOVoAGVycdxXAoPCbUnu(){
ushort sMi = 19014; 
string wtwcHzJnnxnIanaYtOze = "WbCYDekxigkDn"; 
string ItpaHCmFHHtgZtsuRYmzq = "eeuyRepdeMlqqazzzSJxVsiYkd"; 
float uFbEsNokGiLioDCIUIcsWt = 1.398294E-10F; 
ulong ZMWxmOnEKBLJZELmSohBgsQBdEa = 61203881933222150; 
int iWXUMTbXkJ = 628379; 
if(iWXUMTbXkJ == 204994){
iWXUMTbXkJ = 244881;
}short HWwtJNQOJwdVDPiYJtkGwzP = 4600; 
float CSfBUuzJhHTQmjeXkGTpgLyMSR = -1.309679E+14F; 
short gSBPkXI = -22611; 
uint yTIDeSgaCymjKDNRuuJcs = 30636430; 
double bpyYgpddiTmcVHZL = -1.235438E-26; 
bpyYgpddiTmcVHZL = Math.Pow(double.MinValue, double.MaxValue);
sbyte biH =  -4; 
sbyte myRfGCK =  -6; 
ulong FikpqSHpMneTY = 5126942454331656; 
ushort oODXIfpWPMlBqG = 54846; 
long CpOVeuzt = 85815546297085130; 
ulong CPVZjwqohMXjgGgYYyHBjudEdmlc = 84379897894282805; 
short fsEQlNxuNzNsqiFnN = -23726; 
long VXjnQMgHHyp = 37938317129587510; 
short KBNpgex = 15131; 
short iiXIhipZAHPOpiBIVRpqsYwjlyxY = 12020; 
long VqAgdNPjeHuJKsVkFQAjG = 74057017729333118; 
ulong pSWUyQbyUgHk = 37013189168139584; 
sbyte ydAsVWpZ =  -35; 
uint oeRNzzuDsFdY = 2656; 
int cXfzVjfuTweVegVNhTwUzYsSSoQ = 39568642; 
while(cXfzVjfuTweVegVNhTwUzYsSSoQ == 39568642){
cXfzVjfuTweVegVNhTwUzYsSSoQ += 278348;
}string cnbyfYyUct = "ylJpRqSMxeFmGGtA"; 
double gaHcTTAfwpVbdoztREXenXl = 8.302302E+07; 
if(gaHcTTAfwpVbdoztREXenXl == -3.736312E-39){
double ygWtSutQQLVDRuaoP = Math.Log(1000, 10);
gaHcTTAfwpVbdoztREXenXl = ygWtSutQQLVDRuaoP;
object VAjOeyJwALFExEWteB;
VAjOeyJwALFExEWteB = -1.226483E+21;
Console.WriteLine(VAjOeyJwALFExEWteB.ToString().ToLower());
}int XKwqaqYwReWKMqFwHXieulA = 2634; 
if(XKwqaqYwReWKMqFwHXieulA == 645881){
XKwqaqYwReWKMqFwHXieulA = 487769;
}string HBNABMWCKLo = "emCyMqBWKFBtM"; 
byte dAPqWPP =  75; 
byte ReKKwzteRwYMdQpYeGcZtOpDTcUut =  151; 
short qzUmRlnfgoVPjqwYaXLimjFdOhQs = 4670; 
double lebuGWchTsWSeSCtCIWtxU = -3.194989E+15; 
if(lebuGWchTsWSeSCtCIWtxU == 2.289181E+23){
lebuGWchTsWSeSCtCIWtxU = Math.Ceiling(Math.Cos(2));
Console.Write(lebuGWchTsWSeSCtCIWtxU.ToString());
}ushort KSOjJoHWysDqEplBhm = 55966; 

}public void sVOX(){
string JXyitDRpysf = "GcgVmMtdNeix"; 
uint jsmOeQitGKAnYZAmSTuQHQX = 667981; 
string XDFObKHtAdKnojmxyx = "NuEAlobJftuQfclkQbVyEsHajiGeH"; 
ushort BaKokucsAaaC = 64938; 
string FldYxwDYw = "NDocKWcLaUQwN"; 
string NJwyDRHWXFtbIYXXboUyROoPd = "QYjyizbgJMgCSEgHszpUuNcXOgSFa"; 
double GtFaINBxUaBol = 1.305772; 
while(GtFaINBxUaBol == -2.103881E-14){
double iEdWlJbHygJsBhnDz = -7.246852E-08;
GtFaINBxUaBol = iEdWlJbHygJsBhnDz * 2;

}ushort mGMtaIhqARWQWCPuAbNdVqqHuycj = 25354; 
string dWwVhgXDhVYkPipzQ = "PRdcpbWneYE"; 
int WxpSTBGHnCAGhXgDzdZlUGlybHtZU = 97; 
if(WxpSTBGHnCAGhXgDzdZlUGlybHtZU == 949831){
WxpSTBGHnCAGhXgDzdZlUGlybHtZU = 30962;
}ulong pyXkJBoEyhdQLHVF = 51541205230073541; 
int lDBdkWsdiGbakeXUEzpjiwd = 1450; 
if(lDBdkWsdiGbakeXUEzpjiwd == 587371){
lDBdkWsdiGbakeXUEzpjiwd = lDBdkWsdiGbakeXUEzpjiwd + 640367;
}uint QFWoMduX = 45; 
long oDtTlmYnORwFRzg = 54613310959687101; 
ushort QkQjCiMBWP = 55139; 
ulong zsJkyYisynXyGWKlOTpXExchelJKd = 42679877134268402; 
uint nmXsXTMWlcoiVCRwbpCfloAnDEJp = 1629; 
short nWcKHdeospxHPMO = 16827; 
short MfltMYNJQtkxJdOQUZzhPXkmOWS = 23172; 
uint QVZqximUjeWXnMdWHtTYRPoig = 90839374; 
double QfVjYEoxqIGDZPtkRKA = -1.041869E+26; 
if(QfVjYEoxqIGDZPtkRKA == -5.696185E+34){
QfVjYEoxqIGDZPtkRKA = Math.Pow(double.MinValue, double.MaxValue);
try{
Console.WriteLine(QfVjYEoxqIGDZPtkRKA.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}sbyte sGfUj =  61; 
double gFCNlLf = -2.430316E-07; 
while(gFCNlLf == 1.153013E-21){
gFCNlLf = Math.Ceiling(Math.Sin(2));
object giYswsX;
giYswsX = 2.975353;
}ulong JOcPEDCPPekAlyA = 89396217693612131; 
ushort AujqTgELwROdqGwQHESRb = 27618; 
ulong QaJWgefK = 52282119088983540; 
int QHLBRJwTMZlEIdFaBTyHFqL = 43; 
while(QHLBRJwTMZlEIdFaBTyHFqL == 43){
QHLBRJwTMZlEIdFaBTyHFqL = 480037;
}sbyte UkmGiAojaCmsfyczZFqX =  -113; 
uint iAWGeGgiuFtstuywCkKyfqYFItNCJ = 19; 
int kegmTkETtTONe = 6947142; 
if(kegmTkETtTONe == 798515){
kegmTkETtTONe += 745773;
}ushort USBq = 63002; 
long FAluPZipqhIafENzjSDKJZAsyj = 49851661119781469; 
short HuYfBNkbhNnAhtqAwajFaTXFJS = -27954; 
long NAwoURQT = 37513544976599985; 
ulong ToyBACJ = 81359718990942346; 

}
} public class wmTjmDCGXRnQEJmJSJkxOlZTH{public void dtB(){
string loGQycHVpORDImTn = "jLEZz"; 
string imZE = "zhfWqbBY"; 
float GlOUJWJqGNOZiQxCOlDIjdcK = 4.767604E+07F; 
ulong qlsplweskFugAWtQyzZ = 19199341621497143; 
uint dbdjdZu = 4684; 
ulong LepjSwhnKmmuGxgDYeTbPDi = 26152873624835762; 
double lFwzdxacqSWRIzOXywpFMX = -2.703421E+31; 
if(lFwzdxacqSWRIzOXywpFMX == 281.5404){
double RmJtSLeVITYzKmqUSiAzFe = -3.513829E+16;
lFwzdxacqSWRIzOXywpFMX = Math.Ceiling(RmJtSLeVITYzKmqUSiAzFe);
try{
Console.WriteLine(lFwzdxacqSWRIzOXywpFMX.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}ulong OqHzEiOVNmmfeyJ = 33351551402068349; 
string asLpaA = "FbU"; 
ushort bVAdRX = 2659; 
double bojApztpPEeDsDQdGNNmCdD = -1.671274E-34; 
double etYdBRKbHn = -2.456072E+26;
bojApztpPEeDsDQdGNNmCdD = Math.Round(etYdBRKbHn ,1,MidpointRounding.ToEven);
try{
Console.WriteLine(bojApztpPEeDsDQdGNNmCdD.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}int tpYxgKPXzyhEgXhAWXz = 34980559; 
if(tpYxgKPXzyhEgXhAWXz == 271979){
tpYxgKPXzyhEgXhAWXz += 953462;
}sbyte EwWuVfZUBnCquUMWiVskjSSJy =  42; 
float SaLAbTlCCd = -3.169874E-05F; 
sbyte IkuG =  -107; 
float FhtFUHMEmWmmMcBEcpC = 5.631954E-25F; 
ushort JVdgj = 41224; 
short FbInSsUglBYngZjxahIsoDixj = -29335; 
long RACWYeIkdPIlhjoikgXJM = 61446313632876059; 
byte QHkoN =  248; 
string csYslAxjfn = "lce"; 
string ktAXcDHsTLSFXUDjiRVqNnVDDMxN = "ZbLFmeuAemBLHACISmlMQckjLk"; 
double WwpgTBUJgZKPiLe = 1.079338E-08; 
while(WwpgTBUJgZKPiLe == -5.72447E+22){
double jbcWQBUciWDJWtRfF = Math.IEEERemainder(3, 4);
WwpgTBUJgZKPiLe = jbcWQBUciWDJWtRfF;
try{
int yDfYHPeegdcLRWSyYTH = 7851014;
if(yDfYHPeegdcLRWSyYTH == 450){
yDfYHPeegdcLRWSyYTH++;
}else{
yDfYHPeegdcLRWSyYTH--;
}
}catch(Exception ex){

}
}byte tSVNzWXiMPXcSBXex =  252; 
long YUNWQpYEYNpeWDJTVjt = 31449895684067189; 
int EFPsEEAeZWhYukY = 863416; 
while(EFPsEEAeZWhYukY == 863416){
EFPsEEAeZWhYukY += 80861;
}byte GgGoZylmV =  86; 
sbyte iBNsZjPVnkDHz =  108; 
float yfOttsjkYnAINKlHnQQYgPpIo = -1.982313E+37F; 
ushort YQdGwPQENbRqCtLLhiE = 64297; 
double JgTLJPfefsPGZ = -1.363669E-08; 
if(JgTLJPfefsPGZ == -1.591326E+30){
JgTLJPfefsPGZ = Math.Ceiling(Math.Sin(2));
try{
Console.WriteLine(JgTLJPfefsPGZ.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}uint HPuHTTBeIb = 6622; 
string MSRNmQCFSS = "LVCjZYURSj"; 
float kZbHhzl = 1.214418F; 
double kBeIeINcXCshXYUJczRieS = 1.562501E+09; 
kBeIeINcXCshXYUJczRieS = Math.Ceiling(Math.Atan(-5));
Console.Write(kBeIeINcXCshXYUJczRieS.ToString());
}public void qAFtOIkOMJIdYVcOWhBgqyMfbEZ(){
float yXKMkGhnzpabbx = 1.579451E-38F; 
long yPLXLXmGUOAqjJssoLYq = 50891547801865178; 
short ceq = 26342; 
uint LAlM = 610524; 
ulong PgfsJKhXctVhU = 29947722469126295; 
ushort ogo = 40930; 
ulong lSgDAKqMhwFTecY = 86519413829407897; 
short KwfQludqARBOGwCigoMLaGscpN = -25434; 
sbyte eWCRLQokWWaCCGkykOOZSmZF =  21; 
float ccBA = -4.529163E+33F; 
uint UYWVGlXjtLiAZ = 59; 
sbyte shYSXWmTMlhxxMFZlqAnTiNUcGTws =  -109; 
double uCWPEDWbKbTZPOW = 8091662; 
while(uCWPEDWbKbTZPOW == 8.032665E+22){
uCWPEDWbKbTZPOW = Math.Pow(double.NegativeInfinity, 2);
try{
int PwKuyFRoNeoJUQRpgsExNwBNG = 3829069;
if(PwKuyFRoNeoJUQRpgsExNwBNG == 90180){
PwKuyFRoNeoJUQRpgsExNwBNG++;
}else{
PwKuyFRoNeoJUQRpgsExNwBNG--;
Console.Write(PwKuyFRoNeoJUQRpgsExNwBNG.ToString());
}
}catch(Exception ex){

}
}double zNqedZDILbMlLKpiSOCnXPQQ = 1.458773; 
double HjBWQDZW = -1.783692E-07;
zNqedZDILbMlLKpiSOCnXPQQ = Math.Round(HjBWQDZW);
ulong UUuwbtisdhuRsVshIzCg = 36732973681023482; 
byte ylOJcyuyMJWOhOwZfUiwPgyIz =  251; 
byte LudjfmLicJSVNkMTjwM =  225; 
ushort BxQRbpOgkdkQMqFMJVdmWy = 2611; 
byte EuFydmskZUItnkwuWzOKW =  118; 
ushort yNRRgPcuDh = 30155; 
double JgQTFNx = 2.638353E-11; 
while(JgQTFNx == 9.139019E+11){
double EMfZpapxCek = 3.462002E-38;
EMfZpapxCek = Math.Sqrt(3);
JgQTFNx = EMfZpapxCek;
try{
int NAkmqXQs = 6964716;
if(NAkmqXQs == 54016){
NAkmqXQs++;
}else{
NAkmqXQs--;
}
}catch(Exception ex){

}
}byte iJOHpNuwbZVXpsRxNgRLwEqEBEzR =  42; 
ushort DehZFayQlaXUUOTdZKf = 56034; 
int DEFqjgNoElJtsSitlxoqFK = 687023; 
while(DEFqjgNoElJtsSitlxoqFK == 687023){
DEFqjgNoElJtsSitlxoqFK = DEFqjgNoElJtsSitlxoqFK + 495770;
}uint cBdSDF = 2629; 
ulong cXyLlBwaVHz = 52301242934625288; 
ushort mQxjhoBpEYYGXnHl = 48905; 
uint uHFSJsXSLLAAtxSQhbCdkULqIBeG = 486856884; 
int yXZTJYNRkyPG = 49; 
if(yXZTJYNRkyPG == 116526){
yXZTJYNRkyPG += 467662;
}uint luLAVmki = 3393; 
float egbxxYhio = 1.54026E+28F; 
ulong fmhWV = 56447850870262383; 
double OEDTDEBYyhNhq = -1.508643E+31; 
while(OEDTDEBYyhNhq == 2.777221E+33){
OEDTDEBYyhNhq = Math.Pow(double.MinValue, double.MaxValue);
Console.WriteLine(OEDTDEBYyhNhq.ToString());
}short kAmDiuxVSTsKuYZc = -19391; 
ulong lQtlFPplbCk = 2177323694611894; 

}public void FccXmljQxBKJyxfJSnJO(){
float uuHWKpC = -0.01551508F; 
sbyte fLiNxZsFAPBRDA =  -70; 
byte fmkZPxI =  168; 
short CUnwcClsLXW = -4431; 
short jutUDeyzctU = -17833; 
double qoBefJRKkuzpphB = -4.094428E-20; 
qoBefJRKkuzpphB = Math.Pow(2, 2.1);
long wRLHTgNCljTTKSWHsKgthykJP = 35151368144026168; 
int ALKsYGpkumkfyGUPudpKYHcSuEIq = 159027983; 
if(ALKsYGpkumkfyGUPudpKYHcSuEIq == 92616){
ALKsYGpkumkfyGUPudpKYHcSuEIq = ALKsYGpkumkfyGUPudpKYHcSuEIq + 911957;
}ushort JsiORlRxEtfAoFHQSVYTIRYuXYtl = 17296; 
uint CiY = 6; 
float TMQHnazhbmCiRjWEn = 1.505346E-10F; 
ulong STFhcsEqqyEVpbmdbEYWPWgKe = 32811871757522561; 
long Skz = 28453300467513921; 
uint KAmmwSb = 748610172; 
float sBXL = 5.054163E-28F; 
float GauSEqobXlqMSiJWfxcnoJff = -2.04978E-08F; 
float XgcIezV = -1.572794E-16F; 
long WfQeJwikiRbGleGBWUoCi = 66203657111107076; 
byte HzNghzFyKijLpVBRD =  119; 
double mafCAYMmAMBRVpwgzYycsyFsYMRjp = -2.100978E-28; 
if(mafCAYMmAMBRVpwgzYycsyFsYMRjp == -4.731041E-16){
mafCAYMmAMBRVpwgzYycsyFsYMRjp = Math.Ceiling(Math.Acos(0));
try{

}catch(Exception ex){

}
}int CKAjFJU = 19; 
while(CKAjFJU == 19){
CKAjFJU = CKAjFJU + 310619;
}double ZKFemMGE = -6.449736E+22; 
if(ZKFemMGE == 8.778765E+24){
ZKFemMGE = Math.Pow(double.MinValue, double.MaxValue);
Console.WriteLine(ZKFemMGE.ToString());
}ushort LdKstOsgfpzeuAPAA = 61122; 
int JqfCSJgKNXOplJUuD = 26763561; 
if(JqfCSJgKNXOplJUuD == 261038){
JqfCSJgKNXOplJUuD = JqfCSJgKNXOplJUuD + 524829;
}ushort WXyCXxjERTuX = 11355; 
uint HJuhDiSMGwXMJNJwPMjWLJ = 79; 
uint MuPAxtnDnlTtSidlSf = 69; 
uint izFlZEZXAFHYxzWfZEyfAxCxT = 85; 
uint cPRdVlpVkuLDpMoNUNFoNgxmw = 7356; 
double sZLBgyP = 6.210159E+30; 
while(sZLBgyP == -1.081221E+09){
sZLBgyP = Math.Floor(-1.444197E+21);
try{
int ksJDsmBSibLzsMxobCxlmYOmxWPNB = 4312619;
if(ksJDsmBSibLzsMxobCxlmYOmxWPNB == 46130){
ksJDsmBSibLzsMxobCxlmYOmxWPNB++;
}else{
ksJDsmBSibLzsMxobCxlmYOmxWPNB--;
Console.Write(ksJDsmBSibLzsMxobCxlmYOmxWPNB.ToString());
}
}catch(Exception ex){

}
}ulong kHXMqKBNBYpi = 6954614126318460; 
ulong IJcoPsgwgIpxiRLDiYcaIQNCm = 30995019547794411; 
ushort oJbViGeRJScxATFmihLjSVBbhdoyf = 20763; 
ulong PSxUgdUwDYeJkkOYtOixVjgt = 4758476558477440; 
sbyte HHxLffmpH =  94; 

}public void SRXopeshqVGMdMgpPeVnNEYFg(){
uint JdiFEmCpiQPaCtkOqbVt = 974246855; 
ulong lMGenSthPyJdipc = 12776305363166814; 
float PIDlhJKnKtSIycqyH = -2.734439E-32F; 
long GbZySF = 26011732085528156; 
byte WHPZQzXjQesHcsSolwjGEOpe =  171; 
double kAYVEgHL = 1.744557E-16; 
kAYVEgHL = Math.Ceiling(Math.Tan(1));
int[] fdDOgDSnNfWxAOoDuD = { 282745 , 7432 } ;
Random FuATsePFxURy= new Random();
Console.WriteLine(fdDOgDSnNfWxAOoDuD[FuATsePFxURy.Next(0,2)]);double kcz = -1.123306E+21; 
double wAXbfYugGjjKs = -2.574872E+22;
kcz = Math.Round(wAXbfYugGjjKs ,1,MidpointRounding.ToEven);
Console.ReadKey();float NqcIhRKpLmjJWHCBALNFc = -4.089742E-22F; 
uint VOcXkdkc = 519; 
short PXOOqeXoYXnlY = 28542; 
ushort nHMgCxldItAjYHLFkh = 46019; 
ulong QMWnlmYAQZoJIOFpjStMKgXFOqsmK = 30495406730902465; 
float inERtkBqSf = -3.547235E-37F; 
short aoKOqAVVMQPi = -29643; 
string qKCq = "jzgoEumZAgxZCUHpNg"; 
uint unCOckhAnCwN = 73050230; 
sbyte iwjXlVxFuPHhuOEEd =  120; 
sbyte WsltlNMYLKpPNehsWBgzSXumEaf =  0; 
double JLDlGQiRLzjyTKARwpHlHDSLdZ = 1.015154E+37; 
JLDlGQiRLzjyTKARwpHlHDSLdZ = Math.Exp(2);
object nueKOMFQULkgTShHRjbAR;
nueKOMFQULkgTShHRjbAR = -1.278607E-31;
Console.WriteLine(nueKOMFQULkgTShHRjbAR.ToString().ToLower());short oYeZTgKZGJKsJuzMCYYTaIgsQ = -22119; 
byte ZOdzzthMQpsWWSMIY =  133; 
string TfZ = "exQ"; 
long MiLXhZRFkUomNVlHbWhVzuofj = 67224469483570825; 
float gHZPT = 1.890027E-35F; 
ulong ONfkOucBJMtsUGNzfcD = 71760020441448910; 
string oaxHJq = "DJaIqBUOBMmblp"; 
ulong AWNbamtJwatuMWMknqVycVBsR = 32505450258355051; 
byte KVpwppqmphBcFabNUupPFxujbTnS =  110; 
byte SCZcpYVGhLJjkqWPe =  205; 
byte ylutmBfuxbAQgHJBFXMPZPidWmj =  222; 
sbyte jblnd =  27; 
sbyte PPSCifkScEIxjgewWJd =  -48; 
ushort qlo = 22421; 
float FTDdtYiWWyGcSdj = 1.005077E-22F; 
long cXQqwDRjPzHmRoOmjaFdlYhAAcb = 49194530584586578; 

}public void PChCDPtypkPOeQaF(){
int JeQpRtCtQZOGGuAnzI = 73437711; 
if(JeQpRtCtQZOGGuAnzI == 600506){
JeQpRtCtQZOGGuAnzI = 696492;
}sbyte nmtDQfubOL =  49; 
uint OaiYuKKFunQssMDdlAfgD = 27; 
string iJlEHPEaXe = "szka"; 
uint TZDkzuRYCkFqhJMiLkbzaO = 242795; 
short HzLOnfIRmnzcL = -14546; 
ushort fFZJPxqjOaYbLeCpTdN = 23321; 
byte XAKqxyLJANdwJUmJGkhCang =  156; 
sbyte NhiIBbs =  -39; 
int YzZUAeBkYoxHckzD = 7424; 
if(YzZUAeBkYoxHckzD == 362059){
YzZUAeBkYoxHckzD = YzZUAeBkYoxHckzD + 79653;
}byte LDASlbdpmO =  176; 
sbyte mbVNVIRM =  -98; 
sbyte jYRFGXgUnLiWGJZWlftoBqsXLSWyx =  -39; 
int xBInkzLpHPSBdeoeWL = 62930017; 
if(xBInkzLpHPSBdeoeWL == 457478){
xBInkzLpHPSBdeoeWL = xBInkzLpHPSBdeoeWL + 863821;
}int UNaKzWhFBeaLMOudILzRTGDSCM = 92367222; 
if(UNaKzWhFBeaLMOudILzRTGDSCM == 62368){
UNaKzWhFBeaLMOudILzRTGDSCM = 788804;
}long EXpXqWsAFkdk = 15294814198781551; 
double khhNilNVbl = -6.6583E-09; 
if(khhNilNVbl == -1.13564E+19){
khhNilNVbl = Math.Ceiling(Math.Sin(2));
for( ; ;) {
Console.WriteLine(-1.307272E+22);
}
}byte YCENbTPWEAFgizaZDMhpEDeYVVMw =  183; 
float KsEMpGoTQQZloODiKXhFDLWtwyGSg = -8.267932E-11F; 
byte BUOGMKtnSqAFqPjtNbzEnx =  252; 
ushort qSaxNJyO = 33184; 
ulong ODXmOqKtxPeIwAufUIydUsxwSt = 87933970732293358; 
int elPXNGYw = 1992; 
if(elPXNGYw == 368412){
elPXNGYw = elPXNGYw + 559225;
}int cLZxgC = 400581; 
while(cLZxgC == 400581){
cLZxgC = 391048;
}double IeUjbJSBqo = -1.815471E-37; 
if(IeUjbJSBqo == 1.348427E+21){
IeUjbJSBqo = Math.Pow(double.NegativeInfinity, 2);
object oDuLcesOGGSGUoBsOMgzSyciuPF;
oDuLcesOGGSGUoBsOMgzSyciuPF = -1.826343E-25;
}byte OSpo =  175; 
sbyte jMqcccRAkpmBgJq =  72; 
short oUoy = -24006; 
int oOJkTef = 767365; 
if(oOJkTef == 497705){
oOJkTef = 508479;
}uint DMLpTzcuydamuliNFL = 774276075; 
byte oqkhbtO =  199; 
short CMsDNQNKlulhSSLlFhXPGMou = -4171; 
short MxKHI = 17813; 
byte IUGZPYkDUXhztxQOGRAiPy =  242; 
sbyte LbmCNEtHgcqZh =  -85; 

}
} public class wjwyoUMoHWaRndVkXAGdWXagt{public void SluzPyIJNGcBRAZMHGNNeDkR(){
ushort CfUyindBbBUhgDiG = 20068; 
short xJfekpw = -7550; 
int oaDdpWDPpyOxqzzRzzjEA = 9077; 
if(oaDdpWDPpyOxqzzRzzjEA == 826563){
oaDdpWDPpyOxqzzRzzjEA = oaDdpWDPpyOxqzzRzzjEA + 420318;
}string hNUyRMBsdRHGRaTwaqIGt = "XjcjceTgzBCeiKTYBNYAuHDYJ"; 
byte nXAO =  106; 
short QKuHKwzXtge = 14764; 
float CbRUITyp = 2.707256E-32F; 
uint dGiZMfOTTiFTdEaynxTXwSMW = 87802622; 
ulong SBcpfTsTPuwZXZp = 12737571795211420; 
byte EwkTmpSqq =  2; 
ushort nNjkhJ = 58256; 
string ZwCtzLcKFSzSeDzgZCdDIHYXmi = "ITLqhjsE"; 
double AOGQIMhUnjkLeigRzltyF = -6.44289E-16; 
while(AOGQIMhUnjkLeigRzltyF == -3.963876E+24){
double NZAzANzNaUQcYZaWOCkqL = 12.33632;
AOGQIMhUnjkLeigRzltyF = Math.Ceiling(NZAzANzNaUQcYZaWOCkqL);
object szewQSI;
szewQSI = -1.028227E-23;
}double PYugntsjbizuCPMhSRYZ = 1.404522E+16; 
PYugntsjbizuCPMhSRYZ = Math.Sqrt(4);
string EIdtjwGZlYFWIjacumYjOcsTQ = "tBMVjECZpfIZoBKLMHnq"; 
ulong wqAXURdBoj = 82635024951850074; 
ushort UVwuYyAdpLGzSeeuwlJL = 59918; 
ulong yxNAExwSlXguAWeZxqajEEdK = 19377551088047952; 
sbyte XYkYW =  32; 
float uCMKFyqmltVGlTsdCDDBoTmtou = 1.242318E-29F; 
ulong wtoLjDXmxxEpgpR = 10325945191895434; 
ushort CosukzkyPYCpCKQfog = 7972; 
double MWDQBWBHaboUgJnFSkYsSqqojsuPb = -1.843842E-20; 
double LCLROXlwuaDBxVhFguF = 2.054563E-12;
MWDQBWBHaboUgJnFSkYsSqqojsuPb = LCLROXlwuaDBxVhFguF / 3;
Console.Write(MWDQBWBHaboUgJnFSkYsSqqojsuPb.ToString());ushort WeRjjTiiAffzDDeBilpXyJ = 53703; 
long iOhmFPFWe = 76715248319510746; 
uint QKsxNG = 488984; 
float nnnjXbebWUpCCy = 3.883224E+15F; 
double UkEHiLSEQquHfoMAQJf = -1.817671E+07; 
if(UkEHiLSEQquHfoMAQJf == -2.853821E-11){
double hURbJTGITHe = 5.127527E+07;
UkEHiLSEQquHfoMAQJf = hURbJTGITHe / 3;
Console.ReadKey();
}byte nMEQO =  118; 
sbyte UxdScyIUC =  38; 
ulong plnqCqLxzsdEPzWAiNhfpJGWUuIIX = 39410271103909123; 
byte xNJPDsBdEB =  45; 
long BIIHHsxhKdziojpCKVbWzLtI = 73439584292747794; 
int tGGiwfQgMK = 6122; 
while(tGGiwfQgMK == 6122){
tGGiwfQgMK += 522642;
}float hPpQoaMEOcRGVKFaRL = 27.75751F; 

}public void VgqYJynQxkQjDHPY(){
int GxRsOpgOBam = 4813; 
if(GxRsOpgOBam == 74067){
GxRsOpgOBam = 251770;
}long RHEyOsRaxsdQcnuqeJDtbSUWnDeMk = 18976027313219604; 
double aWoKx = -9.624019E-24; 
if(aWoKx == -4.282614E-32){
aWoKx = Math.Ceiling(Math.Cos(2));
Console.ReadLine();
}ulong TsMQGldVLBlAmZtoCYQVKmH = 32261938376797601; 
sbyte IeQdqmaOsyHHMkTKBzUlTcH =  54; 
long mEchXRgMGU = 56849010980756601; 
double OhPpnnQTTFzft = -1.049741E-38; 
while(OhPpnnQTTFzft == 4.858901E-23){
OhPpnnQTTFzft = Math.Ceiling(Math.Sin(2));
 bool? SMoBfPMTJamSbxyRhLtwgn = new bool?();
SMoBfPMTJamSbxyRhLtwgn = true;
}int dDUIZqRPmhFDK = 143046; 
while(dDUIZqRPmhFDK == 143046){
dDUIZqRPmhFDK += 590867;
}int uMEYDVKuQLhQdNFQM = 796660; 
while(uMEYDVKuQLhQdNFQM == 796660){
uMEYDVKuQLhQdNFQM = 634910;
}double ABShuQobSJ = 3.624679E-14; 
ABShuQobSJ = Math.Ceiling(Math.Cosh(5));
try{
int xVTPldOAcY = 6919098;
if(xVTPldOAcY == 92783){
xVTPldOAcY++;
}else{
xVTPldOAcY--;
}
}catch(Exception ex){

}long ZtaqjbBDXuIOioYwSiDUdh = 19593837176464204; 
uint cOcGtY = 17256819; 
long GzhCpNVdcBtSHShuPyIzidSAMwltc = 45538560946597118; 
string HgpRbOkgkyZiqC = "PCGIPVBDb"; 
sbyte ZJCpuV =  71; 
string qTl = "MBnbVdLcSWpagRkBfFoJUODGU"; 
string uoAV = "oOWoHAIXydhMkpmeECWNEEWXYJhR"; 
double OtbtLtVWTHW = -3.554086E-23; 
double QOEhsMRYpYqQYgpOKRGUjIIpQmtfq = -232200.2;
OtbtLtVWTHW = QOEhsMRYpYqQYgpOKRGUjIIpQmtfq / 3;
for( ; ;) {
Console.WriteLine(-9.281774E-18);
}ushort CSgDY = 59382; 
ulong gnqfsMI = 67717296677647518; 
ulong GtmbMaKToFzcMFgksTCDVHPoA = 88730797359347481; 
ulong CDhumImdhjeX = 88989505404959040; 
double YCAAsyRtFMZlFIkzoV = 41334.19; 
while(YCAAsyRtFMZlFIkzoV == -2.362193E-08){
double jNxcRtXFGUNpPlOFsYWcf = Math.IEEERemainder(3, 4);
YCAAsyRtFMZlFIkzoV = jNxcRtXFGUNpPlOFsYWcf;
Console.Write(YCAAsyRtFMZlFIkzoV.ToString());
}int xJQewd = 424614; 
if(xJQewd == 384453){
xJQewd = xJQewd + 93384;
}string eWLYmDTTaFUG = "fUVWUMNdtBs"; 
byte HqIVjTZuuNVLMcnE =  166; 
byte QNsCLRBs =  76; 
short wtLOVCkewbGVMgZlhKzBMo = -22750; 
ushort nQXdKBKfJqDZYgNsDPijRH = 25923; 
long sJROZgkmbIMnaMBWQcQ = 81762234372022243; 
double UIwkzqRSqK = -6.575123E-14; 
UIwkzqRSqK = Math.Ceiling(Math.Sin(2));
int saSdQXegLMLUFf = 78432862; 
while(saSdQXegLMLUFf == 78432862){
saSdQXegLMLUFf = saSdQXegLMLUFf + 980469;
}long jEkZiUXdxDijTZoWBfznYqNzaaNy = 49595899047394520; 
float OkjIpiLwlOADkVBuZSQMZhgAA = -0.07151224F; 
uint AhZeKqnepaFOGP = 960801545; 

}public void Phh(){
ushort YAfQHdKhUJjZN = 39539; 
uint BEmpJSlYutgCAHs = 171; 
string BYTLjuig = "FGcuzCnEVeMgnhxnXhImqQYIwZB"; 
long cVBLptPqMdYtOzWtdmRyxmmBTE = 36011300565468359; 
float kwXOIXke = 4.74238E+29F; 
int cosMKkxQIZSM = 152197; 
if(cosMKkxQIZSM == 588296){
cosMKkxQIZSM = cosMKkxQIZSM + 952405;
}ulong GMhI = 66412586557633277; 
string FeHNqxncClEkiA = "yAM"; 
sbyte LphLxIwXOAiTLhES =  -95; 
float pQEkVefTpzFJeRCMnXW = -9.655015E+13F; 
string nccLAbVipIf = "KhflGpOezjQdbjxpVkBXoAoiTt"; 
int upnaZYREiLpaVWRelZJoFGmfhi = 204337; 
if(upnaZYREiLpaVWRelZJoFGmfhi == 366343){
upnaZYREiLpaVWRelZJoFGmfhi = upnaZYREiLpaVWRelZJoFGmfhi + 244366;
}long LiVQedsShjUAbsIjWTLqmOchTQu = 75011372069685322; 
long xUFXNzd = 82334049427825429; 
uint IWfNWnJLhWRRj = 14; 
double BZf = -3.742386; 
if(BZf == 9.989643E+33){
BZf = Math.Floor(-9.799731E+12);
Console.ReadLine();
}uint KteXkIxGPayFB = 261335889; 
string zlheAfmRl = "umXdtgMcGkoyjZYltNEwWAACdT"; 
short jmSgYOctRX = -28533; 
uint OITXachgnNBnRXVOxZBOxSst = 48; 
ushort FuxJfHhRHFdRkiWBsThQhGwYEKsW = 19493; 
uint CHjCUqRhkQdgSVJDmWSboqGQ = 975974339; 
uint wnlCgP = 199766; 
string bjBLXadzEcUgeayzBjau = "VTLMWECactzDGUJBE"; 
ushort NCwwloikeIosjHegqPZ = 697; 
ushort zCJwFzgOXWPxHbByA = 29763; 
ulong ysNaoVCNsLFu = 15096167311657147; 
sbyte KtDNZdHToCm =  -110; 
string OIgJXt = "mqwmxYGKKOdngI"; 
ushort CWygzwdgegWfeKb = 48387; 
long ybhBEfajGOjQXzujFNBlXRnoiob = 44042377542618567; 
short bsqgoQpXojy = 15215; 
float nlAYH = 2.672294E+27F; 
short dBEGbSnLob = 24644; 
int ifIaYqTeEOLVz = 6; 
while(ifIaYqTeEOLVz == 6){
ifIaYqTeEOLVz = 459533;
}
}public void dmHLknfaziLbCUctpHQCz(){
byte MTKENkyMywTqWAWQ =  174; 
byte HwdKtFcKXWOWlTNUSBLsQaxmGI =  140; 
sbyte uQgoFDeIipYKCAQJAIyiFdpYTd =  27; 
double eMbWQEgIHHpIlJjMJnCHEfafX = -1.546171E-15; 
while(eMbWQEgIHHpIlJjMJnCHEfafX == -2.644202E-06){
eMbWQEgIHHpIlJjMJnCHEfafX = Math.Pow(5, 2);
for( ; ;) {
Console.WriteLine(3.310313E-20);
}
}long YDFslEZsAOQyMOgqWUbHbjy = 49645304587240019; 
float gclGpQXIFnkiuO = 4.771122E-27F; 
double BJdLOeInsfdTftVaG = 3.373716E+31; 
BJdLOeInsfdTftVaG = Math.Ceiling(Math.Cosh(5));
for( ; ;) {
Console.WriteLine(3.132965E-06);
}ulong CpWQBsWyGmgzo = 1337914602766581; 
int YEzypQtNEcyqmqBPXqbdud = 490611584; 
if(YEzypQtNEcyqmqBPXqbdud == 412711){
YEzypQtNEcyqmqBPXqbdud += 714137;
}string UcYEXmcUGI = "BDwSPkTZA"; 
uint pBsVGYQjuJIpiusKun = 6152107; 
long RUBDISWDLGPqN = 54339139525498380; 
int llzxBzMNwSVJzudqXnSXcBSHUy = 6385; 
if(llzxBzMNwSVJzudqXnSXcBSHUy == 286250){
llzxBzMNwSVJzudqXnSXcBSHUy = llzxBzMNwSVJzudqXnSXcBSHUy + 148253;
}byte npi =  107; 
ushort ZStAR = 51382; 
ulong ENfiuHQMuRCdfedxDOxx = 49074177661437074; 
sbyte PfKMdWTgNGlD =  105; 
short sYVPXWdsqCRFFpHKGEbcxtuagE = -4119; 
string kDRQIBPbwkxLNtkmsnQgWGZT = "zYFUItAdDf"; 
double OWPRIktGqJajOPoHUTFEXCjDkhNRX = 4.113912E-24; 
if(OWPRIktGqJajOPoHUTFEXCjDkhNRX == -1.14758E-19){
OWPRIktGqJajOPoHUTFEXCjDkhNRX = Math.Pow(double.MinValue, double.MaxValue);
object bmNmHByDJJzNXC;
bmNmHByDJJzNXC = -1.837946E+14;
}float dABMSlsFOIPXdGoqkzFJgmE = -3.570572E-13F; 
ushort JsJSAEPFjYaf = 19849; 
float SXPlVeodtqxk = 4.453469E+24F; 
float wxUJleQptnaes = -1.735321E-11F; 
short PwHpfBmuooBctiNoDGJxjfNg = -551; 
float djLiYhwRwneoVHnXPoma = -3.554293E-19F; 
ulong FKTGJIBRnTjkEgDVXsqazqOP = 10831353472899193; 
ushort EonQiOOnabydtJNscNVPqnWdJBL = 43695; 
int qtRIDnHhjEmjqM = 8616; 
if(qtRIDnHhjEmjqM == 85738){
qtRIDnHhjEmjqM += 884826;
}ulong VFFpskgIcfKELemLfAMUDsDg = 26982764922294878; 
long zldlVxbCBgBNanGdi = 38513990003594038; 
sbyte pWICZVjHaITEoTb =  81; 
ushort jjyjImYCOnViHTOXMFT = 40287; 
uint QIFxobTblMIRjXbfouDygVLSa = 79; 
short xXPfqu = 20474; 

}public void lXXJNciCQKRWGpzddMU(){
double wolCUPPSMSEkBdyGPtTDpkq = 1.82218E-06; 
while(wolCUPPSMSEkBdyGPtTDpkq == -0.002992645){
wolCUPPSMSEkBdyGPtTDpkq = Math.Ceiling(Math.Atan(-5));
try{

}catch(Exception ex){

}
}byte VIAGiHfALLwTynNnyBuFJybB =  147; 
string uGjuyZlEHSwkReADuX = "TTXYGRnJgyGZbfcMUCqYTkKf"; 
ushort oEAj = 59317; 
long OiLxHEEiqefbTueOtG = 79367359951617606; 
int LiWDYwnXsomYNQCfF = 53; 
if(LiWDYwnXsomYNQCfF == 794718){
LiWDYwnXsomYNQCfF += 129584;
}int EGRBCFcZcziiDdzghwdsbpiwDEP = 91; 
while(EGRBCFcZcziiDdzghwdsbpiwDEP == 91){
EGRBCFcZcziiDdzghwdsbpiwDEP = EGRBCFcZcziiDdzghwdsbpiwDEP + 351618;
}long PxxQLkYLstXbZ = 54010084658161276; 
ushort YYQbdMDgQVKJKL = 39107; 
ushort cFRcFC = 51397; 
double mCZfPfsbzSUiV = 3.659864E+22; 
if(mCZfPfsbzSUiV == -3.571742E-29){
double PJeQJHtuxadJA = -2.303625E-08;
mCZfPfsbzSUiV = PJeQJHtuxadJA * 2;
 bool? FpJmNzY = new bool?();
FpJmNzY = true;
}short ZxM = 9779; 
string TfmAOeyaMejfbj = "IdFDXb"; 
uint gGnSKghFaiqFSVfm = 99200090; 
ushort ztPLfUqtgeguqGfUBmOUEBi = 60555; 
long nNXqRXGPcOQDMPPPyWc = 89308065394289881; 
uint XkTaBHF = 799626; 
byte ODDsdXB =  248; 
sbyte dmTOqaYMFqMfGeDXDkiktIIT =  -89; 
long xhQYmxuTYZliGGJkWyJWyP = 89288516925241369; 
string HpCFPszJllFYpcDFLxM = "APmHbwKyCYytYH"; 
int eewEYZLljhe = 2572; 
while(eewEYZLljhe == 2572){
eewEYZLljhe = eewEYZLljhe + 855567;
}int RcUDuJ = 14; 
while(RcUDuJ == 14){
RcUDuJ = RcUDuJ + 2103;
}long mBGahxAdayTpyjmn = 65526164876569541; 
string XSyzYhuNaPeZLiINbyJdlSUZqKH = "PZpeZmAH"; 
int zMWKiyizQkRXkNSYK = 95; 
while(zMWKiyizQkRXkNSYK == 95){
zMWKiyizQkRXkNSYK += 512038;
}byte bAaZzJbgAZXgCKyQSUxtRODlbDwJ =  40; 
ushort lNMgYizs = 34662; 
ulong MOVRhwBIoudQfnWPYVnsYdPMu = 66836702631756960; 
string Pqj = "ahBgPhWHTbtAy"; 
byte SIYjxtSbaVTimel =  35; 
uint BnkzXaFziJynUeIeiuRhObhf = 223735; 
double JCIUVLWamuLXFYjiFNaPCiWSpqazf = -8.560551E+36; 
JCIUVLWamuLXFYjiFNaPCiWSpqazf = Math.Pow(double.MinValue, double.MaxValue);
for( ; ;) {
Console.WriteLine(1238525);
}uint NKuKZEHnSAqtBDiyHhQAR = 58025627; 
sbyte XPUAUcItXhINAyk =  20; 

}
} public class oeVgwIfGGRXY{public void ZfllbOMPXyKaDuCLgLGVzzAmqC(){
sbyte UnkLufiDBnwySEsHkfTmEVAg =  -42; 
string BGZUBGXUJlZHy = "ZjTphdUg"; 
float kJVk = 3.726041E+22F; 
string qLJWCtURVKJu = "AwbELGRj"; 
double uMWJRLMUThHoMDcI = -3.515682E-10; 
if(uMWJRLMUThHoMDcI == -2.220604E-24){
double mTInjnjqknjFLdgiQQmFjJz = -0.0003909009;
uMWJRLMUThHoMDcI = mTInjnjqknjFLdgiQQmFjJz * 2;

}float WAOAodjfthlh = -6.731059E+11F; 
string eIRWocTwZxY = "GcfzHWWEaSVVkC"; 
float pxkzj = 1.89002E+19F; 
long YdTGtASfOBqbOCnMxplMV = 22177155777650133; 
byte oxPITcYItdQxaCdCVXNOyCXNm =  77; 
long mWGHEJbFYTuJzJTJfoNKLEIoN = 78389738983645537; 
sbyte yyiSsAzDcDepO =  41; 
byte YDYfKnqTgnnfWARnpnwl =  240; 
ushort jmbGNJgHekwxa = 27366; 
string igpMektGEMQZBmcKChPSIJ = "kZHiUCcRSnEmeHLpYU"; 
ushort aShg = 55564; 
sbyte USHkcBCeNyczeBVYMWaXcR =  85; 
ulong mGEsSBqLQhYkdBMEDXTWxqDfUwjR = 37480284182511449; 
double VWMNnsWNJxnuBTYTCWdyzx = -0.04413588; 
while(VWMNnsWNJxnuBTYTCWdyzx == -3.558792E-32){
double RFQbWkJcjjhMOERijcXLTQ = 3.462711E-09;
VWMNnsWNJxnuBTYTCWdyzx = RFQbWkJcjjhMOERijcXLTQ * 2;
Console.ReadLine();
}string bzsaoS = "WnYOcpFitcVNUxLinKKzZXbBRBQHB"; 
int hmReQEPnJQMEjTghKppdqFuaYly = 536320016; 
if(hmReQEPnJQMEjTghKppdqFuaYly == 664541){
hmReQEPnJQMEjTghKppdqFuaYly = hmReQEPnJQMEjTghKppdqFuaYly + 810499;
}sbyte BcsVFAwbImuSILsfpjAFDTR =  -101; 
int RNhSdN = 11; 
if(RNhSdN == 352886){
RNhSdN += 411574;
}long VENNFuQmiuIzqDkHll = 9667748287202987; 
ulong emNUAVKZgdCjpEEOk = 45494245947908031; 
byte lKXEXFniVHNZwTlBhLgifffFFkp =  166; 
sbyte IQnayoiAFGcuI =  -98; 
ushort aGJALwYcPfFtZMoGeCZ = 61621; 
long xWqIgCgMzZgYkK = 47003353995377749; 
long BSpxpfgBUFjMicnyWdIFmaCG = 45401126549802269; 
byte hokKxsTGdGAmyHYwo =  35; 
long DAqLQQ = 67504750523996231; 
float llIoMHXVgtblQKldteYIXBUGeBjKl = -6.233478E+17F; 
ulong YicAwuWDxEyPbDMhqNxJmc = 35687996258933976; 
ulong lCRBiEYKbfCWyQcXyCUG = 72098506188690751; 

}public void AoMRQDBM(){
byte NOMHhBPVlhmZMPhqedUczhOBDRPfH =  67; 
int lDhREBpwJ = 202645806; 
while(lDhREBpwJ == 202645806){
lDhREBpwJ = lDhREBpwJ + 554253;
}double AzyRiBGalepMJnYFbNjHp = 2034712; 
double cnRxhQfucbDRsxZKaCl = Math.IEEERemainder(3, 4);
AzyRiBGalepMJnYFbNjHp = cnRxhQfucbDRsxZKaCl;
object qtNTKsQSJoIbcU;
qtNTKsQSJoIbcU = -4.037831E+29;
Console.WriteLine(qtNTKsQSJoIbcU.ToString().ToLower());ushort LmJoVB = 3688; 
ushort xcmiTCLuXQOwdZndklUsLWExMYcY = 49331; 
sbyte jLlcFyZkXKOQY =  27; 
long nwlKnhckAzRxVAMHQxEL = 78425945681552723; 
double GQbqOE = 5.970809E-05; 
if(GQbqOE == -8.336386E+12){
GQbqOE = Math.Pow(5, 2);
for( ; ;) {
Console.WriteLine(-1.184953E-07);
}
}string qiqAMNcjou = "aMUNyTpdByIqmpeGn"; 
double wNHyATqLNlXPUkk = 6.703447E+35; 
double SKSejHhPmSZPuowUfhE = 8.996833E+30;
wNHyATqLNlXPUkk = Math.Ceiling(SKSejHhPmSZPuowUfhE);
int? fbPfQDfBAycGQbmBfdiCblx = 1802897;
fbPfQDfBAycGQbmBfdiCblx += 92068;long wCql = 33726870929454573; 
float zNtgMUGJwxLwLhYbxUpi = -809312.8F; 
ulong uKNdPbOYRhTLHckRyMgUCMHquplM = 14574614316400246; 
sbyte UtIMFDomuuKzLtySw =  38; 
string KzQcwgIZLjpiKW = "NlPhdcgVSceYhxhbeXMSwDGqyV"; 
short keBPPCLGGBZtDEYap = -22575; 
uint DiklXdyjHzKhOFLODp = 61; 
long wEXtoRFGapwLZbTnS = 3055774450246189; 
double kzaYUSbNHnEPGGbytNcGJUnyzD = -2357.904; 
if(kzaYUSbNHnEPGGbytNcGJUnyzD == 3.098203E-30){
kzaYUSbNHnEPGGbytNcGJUnyzD = Math.Sqrt(4);

}string EEoblaNdXyQ = "kAQsSAAP"; 
long mwUsK = 22508167124486829; 
double gDxViWalaZiDpwJboIz = -2.280556E+29; 
if(gDxViWalaZiDpwJboIz == 1.123537E-14){
gDxViWalaZiDpwJboIz = Math.Pow(double.MinValue, double.MaxValue);

}string nCQHdAtUZkhpbMoXTNnpOQ = "WHOSYYDCg"; 
uint LfptJxEBeUysCXxbCubOWzoUapIuV = 671768; 
string bajHBdSNnLtznlUDXaheNcmR = "ttPiFIkwDAWmub"; 
short aJNVwMdN = 23261; 
double CXcZOskgLdFKdh = -4.993534E+17; 
double NuBVbHobpHlKzBBiUjUFgwZJOqZ = 1.398962E+35;
CXcZOskgLdFKdh = NuBVbHobpHlKzBBiUjUFgwZJOqZ * 2;
object yKdoFmxiBwbocSy;
yKdoFmxiBwbocSy = -1.211697E+22;double ybujfgWRpMOgahcuypYixnATyPC = 1.726098E-32; 
while(ybujfgWRpMOgahcuypYixnATyPC == -5.528317E-38){
ybujfgWRpMOgahcuypYixnATyPC = Math.Sqrt(4);

}short EwmYxfaeuxJ = -20611; 
ushort dbNaBfOJtbawZpN = 28864; 
double dsknjzeVHzCnbKLpy = -2.169038E-37; 
dsknjzeVHzCnbKLpy = Math.Pow(2, 2.1);
try{
int WGAogfaqKJS = 7583876;
if(WGAogfaqKJS == 88678){
WGAogfaqKJS++;
}else{
WGAogfaqKJS--;
}
}catch(Exception ex){

}int dIBNScMmOWjHsySXCsmn = 9477; 
if(dIBNScMmOWjHsySXCsmn == 756537){
dIBNScMmOWjHsySXCsmn = dIBNScMmOWjHsySXCsmn + 867616;
}float iDsgObFjTIpRgCCoPT = 2.447751E-14F; 
string cwHowgcJ = "ZliekFdTqTIdTcxhMwpcThBAZBXAA"; 
int hkIpGXyqKLauzwg = 34; 
while(hkIpGXyqKLauzwg == 34){
hkIpGXyqKLauzwg += 711582;
}
}public void GgRMJeLHEtKuBSkMQjneGs(){
float ceTqGosXOwFCAUosGdgHKdV = -1.921838E+34F; 
int WwHgnndKEltbo = 74; 
if(WwHgnndKEltbo == 255306){
WwHgnndKEltbo += 684175;
}byte UVzDkldYiFtZVxCgoqnwkLflbSGX =  115; 
ulong nty = 87480638719125855; 
ushort ZbtGkAQWwRMjQlBsGfBs = 38043; 
sbyte yucYLnlwApzmaAaMYaeS =  8; 
long KXcLxUkcdUwVkiCeIiwmTqZBT = 71344639134011712; 
uint bJEQBonwsFEP = 444853; 
double JfAZsXCRRAIKEH = -3.24359E-25; 
JfAZsXCRRAIKEH = Math.Ceiling(Math.Acos(0));
int[] spkBtxqHCaJDoCcqW = { 881801 , 90212 } ;
Random QxosuUqb= new Random();
Console.WriteLine(spkBtxqHCaJDoCcqW[QxosuUqb.Next(0,2)]);int LehVKnOMiEMBjYsdopJYyYXBkZVq = 34; 
while(LehVKnOMiEMBjYsdopJYyYXBkZVq == 34){
LehVKnOMiEMBjYsdopJYyYXBkZVq = 600186;
}short uuRODjXlKPixCnNOlI = -1144; 
ulong HKKtxDR = 22243429390606999; 
uint wmaLRxRzQVFEksfFxOKhwgIgZuo = 788214; 
ulong SpqllLxQnSUHHlJfhz = 60451949053135975; 
float xgnKdqNmUfUTZXxoBAaUqo = -3.485137E-32F; 
long JIZRXHRLugFkE = 3979941310156907; 
double bZPFZuhZjYKIencimcnEpLo = 1.563E+16; 
while(bZPFZuhZjYKIencimcnEpLo == 6.302809E-34){
bZPFZuhZjYKIencimcnEpLo = Math.Ceiling(Math.Sin(2));
 bool? LWkqlblkXqhpbdc = new bool?();
LWkqlblkXqhpbdc = true;
}ushort gnhmRhoLQiMiRWQfXhd = 20923; 
string PRDMiGCdQGPKeNdXgiSF = "VqSWYycwN"; 
byte GzzqL =  26; 
double jNsftgSccZaUzxxUAKPki = -1.534832E+10; 
if(jNsftgSccZaUzxxUAKPki == -3.227972E+14){
double mbjiwAElAQguaxMsHibFRaq = -8.74746E+08;
jNsftgSccZaUzxxUAKPki = Math.Floor(mbjiwAElAQguaxMsHibFRaq);
 bool? nNKblOp = new bool?();
nNKblOp = true;
}ushort zPOVGKJMIs = 10760; 
string EQsLNkTVN = "gIewCopIoEcPwjiiPDH"; 
short NAxqwsKUBCtLiImBogyqKT = 1110; 
byte uhCExpouVthIdgWGOPbJdlDV =  166; 
uint ibbUZkDakipeld = 2655; 
short TlaBLW = -27529; 
uint LiGGWWTgsTAiyCfMy = 700604; 
ulong NYmQPnwJpfQAJSSFDui = 65323863614867917; 
float nztKXXjsQgIzycOKJOJngUd = 1.897168E-05F; 
long FzStEBUbHxQMUImNmBUAWGfo = 9535532005613707; 
short GUYOdYD = -31944; 
sbyte ScPGUhQbEh =  91; 
double DMDTmHkFFxMc = -6.768706E+20; 
while(DMDTmHkFFxMc == -2.145728E+31){
DMDTmHkFFxMc = Math.Ceiling(Math.Asin(0.2));
for( ; ;) {
Console.WriteLine(-3.043617E-14);
}
}float sHhLaTJRDwXq = 7.142636E-25F; 

}public void soBsDV(){
short nOdE = 14623; 
int DmnRGDqAUuCjtWjMtLcVAAXsyZ = 870179; 
if(DmnRGDqAUuCjtWjMtLcVAAXsyZ == 308814){
DmnRGDqAUuCjtWjMtLcVAAXsyZ = DmnRGDqAUuCjtWjMtLcVAAXsyZ + 955536;
}byte TKceEGbPGYaLYgebKTJIt =  17; 
short uHYzDXeFVKdAJngyTLTYqMaxPVl = -3389; 
short qQugLWHEmJTxQodFsxM = -17783; 
ulong jqFJGCJHLHootneDEkQp = 83895731185777033; 
int McSJGcDbpeUGXmgBVQAQTCwz = 98; 
while(McSJGcDbpeUGXmgBVQAQTCwz == 98){
McSJGcDbpeUGXmgBVQAQTCwz = McSJGcDbpeUGXmgBVQAQTCwz + 891201;
}sbyte dVVmyKJbZCULxSm =  89; 
ulong QDLNXkHuAGRSn = 36940509246666282; 
byte yQqHKb =  83; 
ulong XseGOqXLGdkck = 69737520631661412; 
float FwmWHEWshSyV = 1.511859E-12F; 
short keHoZbemJdiVaaDTebA = -19288; 
uint qgJMckOmsfAue = 88; 
int EpamDubqgoFQ = 848304527; 
while(EpamDubqgoFQ == 848304527){
EpamDubqgoFQ += 753749;
}uint nYUzwLcxHWyYBJWUChP = 64235367; 
ushort VjxyntonswYeFxCRRsGLkRGplVk = 37566; 
long NYTdYoZnPcqIfEDFqyZpXubKKdEgl = 58225798744324344; 
uint mSEEDtDMcNHEOFs = 36; 
double tULSbxU = 5.491772E-36; 
double ytRAqgsBtFOugmsodbhFCaC = -5.906796E+23;
ytRAqgsBtFOugmsodbhFCaC = Math.Sqrt(3);
tULSbxU = ytRAqgsBtFOugmsodbhFCaC;
object ahkmOPSVICySlCjzy;
ahkmOPSVICySlCjzy = -1.620155E+12;
Console.WriteLine(ahkmOPSVICySlCjzy.ToString().ToLower());short NHjVcqCezhnLIBMVewIyQR = 7371; 
short kRpeIVjyJTShNqSWOlJ = -13745; 
double SyxVjHUVWaUpSBlZRmdnY = 2.476857E+28; 
SyxVjHUVWaUpSBlZRmdnY = Math.Pow(2, 2.1);
Console.WriteLine(SyxVjHUVWaUpSBlZRmdnY.ToString());ulong oZjFMkVGOtSiQqyqTKXAzCj = 55918737655908403; 
int OdqUXPBpengdwIBQ = 644245364; 
while(OdqUXPBpengdwIBQ == 644245364){
OdqUXPBpengdwIBQ += 293093;
}ulong CdsJpBkmoIbngxWVxjtmcFfM = 46146737290742371; 
short tTyRZFcTOOGmKyLi = 11321; 
sbyte BNWXGJDnplSyDYGcczGjPjiRsgO =  -106; 
ulong SOdEPOyW = 63540573218910112; 
string dwILcWgpEe = "AUyAfuetZiSFgHyNyy"; 
uint ccAauDhLhxzjsssnZSInRWdelQC = 59; 
byte TcjDxuyfEltlDKJlOnjNG =  206; 
byte fYuOE =  5; 
string XOGgi = "WTDNebyKCwEENPwyccPfxyaJ"; 
uint innPdCcYsqjTMlHxy = 211168821; 

}public void nnK(){
uint EsdtzTUkDTiwNqcBOyemiq = 469546; 
double ayC = -6.07764E+29; 
while(ayC == 4.053575E-34){
double TzoJjtyaKJiABRGnDlQ = 6.889612E-28;
TzoJjtyaKJiABRGnDlQ = Math.Sqrt(3);
ayC = TzoJjtyaKJiABRGnDlQ;
Console.ReadLine();
}double beHFuiChJCU = 8.749762E-28; 
if(beHFuiChJCU == 17.94849){
beHFuiChJCU = Math.Floor(-1.148926E+35);
Console.ReadLine();
}byte SSxNTdhFfdJXJsQt =  33; 
double tlTnWxxgmncPZYZqTsdsaEMnxDmi = 1.962834E+34; 
while(tlTnWxxgmncPZYZqTsdsaEMnxDmi == -6.425903E-33){
double EAqyWUpcnJYKbWadGWQBGRujnipRQ = -1.430701E+26;
tlTnWxxgmncPZYZqTsdsaEMnxDmi = Math.Round(EAqyWUpcnJYKbWadGWQBGRujnipRQ ,1,MidpointRounding.ToEven);
Console.Write(tlTnWxxgmncPZYZqTsdsaEMnxDmi.ToString());
}double IipLRffwJAVOlowHmSfZZ = 1.365056E+17; 
while(IipLRffwJAVOlowHmSfZZ == 2.079431E-13){
double ixDntmTNoWIYdHkwCodG = -6.336326E+31;
ixDntmTNoWIYdHkwCodG = Math.Sqrt(3);
IipLRffwJAVOlowHmSfZZ = ixDntmTNoWIYdHkwCodG;
 bool? KexKYAajxcqAkYWiOADjsbAfwqPR = new bool?();
KexKYAajxcqAkYWiOADjsbAfwqPR = true;
}short IKisdNbPGu = 8781; 
short LNnUAzQJglzXBzYhaU = 13053; 
uint gfMfznnWEiMCmgEoKjfBHCzLuD = 600; 
long BWTKFICPbkDMDcNwmsw = 81589749791860729; 
string VZkoYJPJEDVNViVYUYxYZo = "IPpQsUxZbBdflqyFtXNKQAfuCJau"; 
int cMHcDBIT = 767410; 
while(cMHcDBIT == 767410){
cMHcDBIT = 248718;
}float KHyt = -1.726362E-37F; 
byte giJVnJUctQyqAiPWWB =  131; 
double mFSDzZtWmicBbWuRNF = 2.056078E+28; 
if(mFSDzZtWmicBbWuRNF == -1.522908E-32){
mFSDzZtWmicBbWuRNF = Math.Ceiling(Math.Atan(-5));
Console.ReadLine();
}ulong OqUj = 34503897845823533; 
int QfJNC = 67335527; 
if(QfJNC == 768320){
QfJNC = 246064;
}string FbPe = "mOmcWUsoQyCfKcSo"; 
ushort dwZRZeZgXVHKXdBX = 62905; 
float OjPuliGhldklOiCMKAwOhcPXyJ = -0.3879882F; 
ushort NebxIqnaNItXOcNJSCx = 22991; 
float DcLMuSXEcfKHpgk = -20.03532F; 
byte JNNwknNkPQFdePp =  137; 
double VWxdjFmmhMGzBalUhLPIBy = 7.601856E+10; 
if(VWxdjFmmhMGzBalUhLPIBy == 0.008288564){
double FlHGn = -1.200347E+08;
VWxdjFmmhMGzBalUhLPIBy = FlHGn * 2;
object hXgaFpoE;
hXgaFpoE = -2.534807E+08;
}short dSzUkwjuRUwhYmLhbWpR = -15182; 
uint wjUq = 46; 
sbyte IhJLcXlZmfsktzmYGRZGEmAtC =  69; 
string qeeWMcOapDIkQXcLWsLWWV = "nzBnuIpjoUBIhbwzpLlJdj"; 
ulong GjPyidgOxtQzkPttastqooVJ = 56628196461053610; 
long IjSTakNNlPlXnCqTJLRzCq = 57442872642610457; 
ulong PBgiBhshECcfyDFTUREVnj = 2286602000542197; 
long bBiO = 65631916795161308; 
byte JuLPamG =  211; 
ushort pPnOkIGLSaeXtRzMycD = 38873; 
short dMdCZNSx = -4059; 

}
} public class cEyGVGoL{public void jgneL(){
string qoPDoTpfMDKKJSSwETVPuTQSNR = "ecQHsAexEIUVjnqT"; 
short GclsMKkQlByzXPysldWVqjExoNK = 30032; 
long cZyVhPoNqfwDnFYfL = 73843321221918349; 
ushort QdYamfKyyabHJcA = 47888; 
ulong PWZyicFATAPcaqNp = 5445511215314445; 
uint JyPPD = 124415; 
double AAsUVwOlkJuaLUsEQtLsssSW = -7.650069E-10; 
if(AAsUVwOlkJuaLUsEQtLsssSW == 1.243547E-33){
AAsUVwOlkJuaLUsEQtLsssSW = Math.Sqrt(4);
Console.WriteLine(AAsUVwOlkJuaLUsEQtLsssSW.ToString());
}sbyte tQawEnRjdCzdgjKZVwqcecIgPWVQM =  -64; 
ushort phmHPgLPqesXzfBqMaZ = 40183; 
long UqhUG = 24112450085956361; 
double dkPLCMaKLIJwEdLmOYlmWAUBcxoH = 7.547624E+22; 
while(dkPLCMaKLIJwEdLmOYlmWAUBcxoH == -8.690186){
double VfsUIxlXl = -6.858564E+12;
dkPLCMaKLIJwEdLmOYlmWAUBcxoH = VfsUIxlXl * 2;
 bool? sTBNaEpoJexthUbpUiu = new bool?();
sTBNaEpoJexthUbpUiu = true;
}int HERBAaIuMBIM = 447071955; 
while(HERBAaIuMBIM == 447071955){
HERBAaIuMBIM = 440887;
}byte gOowhHSXRPCyWzb =  198; 
string YNkuIFSitERcoeW = "lSnwsSi"; 
long XYjPuxpuMzCnMfKEZ = 39450841285572077; 
byte BJfgEBR =  182; 
int tZhCJEHwVAMEStFVFOyC = 3079591; 
while(tZhCJEHwVAMEStFVFOyC == 3079591){
tZhCJEHwVAMEStFVFOyC += 730782;
}int XOUwdVGnfFfW = 110097119; 
if(XOUwdVGnfFfW == 452028){
XOUwdVGnfFfW = XOUwdVGnfFfW + 938688;
}ulong CJVdneZsRVLWXozgFqAXOq = 82845062373239342; 
ulong VTQnSRgZVnykTjZCIMigyTYkTV = 24860348156850971; 
string zjoN = "BbAUFgzC"; 
sbyte xmJxcVGwXxuFAeaebiVdzywWWSz =  -47; 
short WklBNjyxXiTPqpQWCwTBfoH = -25936; 
ushort RMAkxofiAitYqRHGkNjKfegapcyOJ = 42441; 
int dOsMWN = 58507618; 
if(dOsMWN == 825797){
dOsMWN += 648295;
}int ebTAdUVNTMGAVtRsBS = 239024; 
if(ebTAdUVNTMGAVtRsBS == 476799){
ebTAdUVNTMGAVtRsBS = 909266;
}long ZGbXnzxMYwUiEKk = 9489625863283734; 
short YdqKBDYQGUpMWzgciuxy = -11689; 
byte bEKSPmHHSLoowfczSWtYQUFXhY =  96; 
long qTqLjofQLiEYnybIjMXgVEBXodgYN = 56032552184132313; 
int QWIhNQaD = 567399; 
while(QWIhNQaD == 567399){
QWIhNQaD = QWIhNQaD + 438902;
}uint Otuw = 16; 
sbyte ZsKZG =  108; 
byte GCgjnwOhoBKwyNFxyEOzDi =  19; 
long EiwfaWBtFtLpqQtFmfGyNHmN = 27213397358950894; 

}public void wKmsboFjbSuUfgO(){
ushort pczkbXVISzUsGxWmSVffmkRbnL = 51257; 
float hwnOTRtZmuNqKjQKSxwyHmkIoG = -6.677001E-22F; 
ushort phcPKcHEMFDnGCtPDbbswotwlUkG = 31528; 
byte hDpdFoVANcnKQVtIKoAcej =  21; 
uint PHD = 85302596; 
string zTuLo = "PBICoohGDBTlCqZ"; 
byte YzPbPJsKUbikXBItlBZw =  141; 
float txmfKDqKfnkxlAxZRpRumYwF = 5.246809E-23F; 
float RZQecKxGDDI = -47620.82F; 
short WNex = 25502; 
ulong heMBCubIfAKbTfzg = 63776524065495940; 
float VzSVPYApSLtChbFUxiyCzXzGLTR = 7.671309E-34F; 
float bJGADQkLLCEnON = 8.132484E-08F; 
string DSAmpyXZ = "kVhAmYnqT"; 
uint bdlzMZUKLSSzQPBbX = 37079670; 
sbyte EEZPEbppAwYTAiAGb =  5; 
short nntsysLNXqEAtazAmJ = -22482; 
double tkyUFpHbwVmcFnokHs = -6.138788E-14; 
while(tkyUFpHbwVmcFnokHs == -7.168605E-27){
tkyUFpHbwVmcFnokHs = Math.Ceiling(Math.Asin(0.2));
object ioGqWHaacFueRVbbDULGtln;
ioGqWHaacFueRVbbDULGtln = -2.874071E-06;
Console.WriteLine(ioGqWHaacFueRVbbDULGtln.ToString().ToLower());
}long byMXTasdPjiKASxMitWbxSM = 59522803965688071; 
ushort nCyZIBlWeKad = 10440; 
short fqDuFJqiLqPKLoMwfPslqZNBSLf = -9943; 
sbyte paSuWaRKPKWsMITQCHsoUo =  80; 
sbyte gEcCUDZaabPRptLFxZ =  39; 
short mfHbjzZURbjNQPxPhVgXsMHeZkmHK = -2869; 
short RGTzwAL = -27468; 
double iZFeAE = -3.272925E+22; 
if(iZFeAE == -217.4025){
iZFeAE = Math.Ceiling(Math.Sin(2));
Console.WriteLine(iZFeAE.ToString());
}byte uxkEYClzjqpNsWLx =  7; 
string EthfGuhPoAqHHGOi = "detUuuobdkAgyQBZQHglpFj"; 
ulong XZTLJhFOYsXJFjYSdDiIJR = 56413089083290737; 
uint DhywxqucVszyEpkyiumcahcfocd = 168915901; 
double DTbfyMExNhfNptlmqGDzDiyUu = -2.982294E-12; 
if(DTbfyMExNhfNptlmqGDzDiyUu == -2.118803E-19){
DTbfyMExNhfNptlmqGDzDiyUu = Math.Sqrt(4);
try{
Console.WriteLine(DTbfyMExNhfNptlmqGDzDiyUu.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}sbyte anKFXnweGuPGJKTiLaOwzYpXqCW =  -2; 
short yBMKJnpwusHAVWzTERok = 17538; 
ushort MxcYMmwqVNYWoUYbFT = 19731; 
int PZduAzXekgTdX = 273600713; 
if(PZduAzXekgTdX == 574899){
PZduAzXekgTdX += 160490;
}
}public void bLNWZjZFQLszOhXxYFlPymLGeTqUL(){
ushort ohHtdmgupA = 23552; 
short PwoPcRILqtCPZgRg = 22811; 
sbyte wxcqhPUjSkWpeYkRKFDSTOEXOeF =  -111; 
sbyte nyuzUeMegah =  36; 
ulong udxRPphCpmbHSfEc = 57229133429597949; 
string eGyCKl = "cDKJOoQiMhgxmlHCdQsEmFVhQNJkC"; 
string EQJPDTuxCntjqPjRUzeuMsJ = "zICTbGFHJuEHFxeIJuQoTu"; 
int GVOMyKbkhXqV = 5300; 
while(GVOMyKbkhXqV == 5300){
GVOMyKbkhXqV = GVOMyKbkhXqV + 757841;
}uint kEmzuxLqCuRecingSTiYssAHfiqh = 900737681; 
int SIhbIp = 280; 
if(SIhbIp == 679930){
SIhbIp = SIhbIp + 761452;
}sbyte SinncRFEF =  89; 
string skfEqHPLlwalzDBgZIJ = "BUqiTMSOCodpXcYgWwtcYw"; 
double hbQBpyEWitezUX = 1.950449E+08; 
double wFukEnHNfUUa = Math.IEEERemainder(3, 4);
hbQBpyEWitezUX = wFukEnHNfUUa;
try{

}catch(Exception ex){

}byte EKyeglRBt =  197; 
float QDeBwDRLyFYYWmcUcQxgPTTcXR = 2.725929E-38F; 
sbyte iSfLjtN =  -77; 
string nCOZMAiTjWyEqNcXJbEFOQCpxgSkd = "gSNySneUqyJVRlLFBsJmK"; 
ulong ykyazEGHXPBPFQUVPSPK = 88055013407237088; 
uint RYwkplumHqMRMfpyf = 61776972; 
ushort wVbCDtzLTMZguYDutBIBCkyYoj = 47633; 
long zcacKxctKNTWDOGMKqhOcMmYWq = 38658996464859520; 
ushort YdLDAbKOPFZySlFTFVQNQ = 2434; 
sbyte lFpxukhKtWOH =  28; 
int abAjssNGQI = 67101; 
if(abAjssNGQI == 648082){
abAjssNGQI += 870446;
}double spVLntEyodVzYmZIPLpSOazYAOVh = -6.003791E+10; 
double oOgGVcKKsKmekm = 7.551692E-20;
spVLntEyodVzYmZIPLpSOazYAOVh = Math.Floor(oOgGVcKKsKmekm);
sbyte dduPANBVyydoJiNusSKu =  -65; 
ushort OauRotcIeuPtenDsVPemRxIBdgFy = 28701; 
ushort wJfGOfVkCuQZUTQIfPAPFh = 8409; 
sbyte sTNhPGkIJuKZ =  38; 
ulong ZEETbyNHn = 87158504253546877; 
double xlhdMPFslA = -1.715503E+12; 
while(xlhdMPFslA == -6.609158E-21){
xlhdMPFslA = Math.Ceiling(Math.Atan(-5));
int[] saRBRdxfChhRaI = { 7662425 , 18614 } ;
Random SfJoVtRZJwwRssJsYJeJlYB= new Random();
Console.WriteLine(saRBRdxfChhRaI[SfJoVtRZJwwRssJsYJeJlYB.Next(0,2)]);
}long deqRqpCgyzTeZCaiexleNTPj = 32232177217676598; 
byte OVqFdCQlkUeFegkPws =  241; 
int jogJ = 322706; 
while(jogJ == 322706){
jogJ += 569612;
}ulong wNfRBsLnCJUfERSofiDhJCEGk = 52247585509758388; 

}public void JtfXVZmpCCwbfFzNgKaHjUj(){
uint FKAADqEOQhzjhnEVsack = 81; 
string bSIpLEPjGpqpNQZkRmoyaE = "xyLRUhoyNHOzKJndltnESWmhDaRtW"; 
uint MzTkSZNVcpSEKZqoCEi = 9; 
ulong gAjDnTpVClabaRyweAzlnZtEilMpQ = 64618932372084001; 
ushort bNwqsJXqzdieEHBL = 9975; 
sbyte uchZeOOzcBeMNIaxzTJ =  -123; 
float UmlIJUVzO = 2572513F; 
string yUNp = "FjUQtqwyEjbSAfLhh"; 
float yfTEENBhKFEDMDgxLqdUVlk = -1.605882E+15F; 
ushort eAqVoaZTxLHpdSjLSaIpptJaRea = 56066; 
byte DTgSxItKnDMKKMeaZbLBtpcgGMQ =  84; 
string ZgICiNjl = "CsiMfEGJXVQeYh"; 
long lRZlLSbGeDFciDqRO = 60828063622416291; 
string wnfbZXZWWjZLKpEIEZEAuEYpsY = "qIUxmIYHqZBFeiGwI"; 
ulong ASHWtFxDzRpPKbxJocNNdLKlJdu = 33933797576240653; 
long MCSQeBPajREIBqtUcROhw = 74043942536492900; 
byte CNYDCVWYk =  233; 
byte lQRQyPxAGMDoykI =  222; 
float fbWFcVDOSZfTDljPdufxlTEO = 1.551088E+23F; 
byte epmcgwQjCIOWmJXxtNjpf =  169; 
sbyte NxElyiXlL =  97; 
long McMhxZmwHjiDWhuTH = 56918250092164988; 
int tKCltauKqabUgwNHEmbopOd = 80745710; 
while(tKCltauKqabUgwNHEmbopOd == 80745710){
tKCltauKqabUgwNHEmbopOd += 746080;
}ulong pSlU = 27537515171044586; 
float awbwtMyPoxFHWwcOaX = 1.981268E+35F; 
long UCbD = 6111522154134512; 
ulong kZubFBDPoehDNPDBAVTXCBtQfZq = 43806478332954200; 
string DtjAelXudLKkskpgSu = "GEVGjbluJlWHmCXXCJzxnx"; 
ushort nIql = 44624; 
sbyte OMRuZIOazTRoigexKwnOzuYy =  -79; 
sbyte qDLZLDxiUxgYpOJaDxhnVjkIRw =  -44; 
short OtjSPxzOTgMMX = -19828; 
string yYaPuwbcTWhqdiplf = "fcWfBqjlJpwCYZkeuypCZssh"; 
double ogLLRAGhdXGqyKGhNLHsYhfYP = 1.461199E+31; 
while(ogLLRAGhdXGqyKGhNLHsYhfYP == 3.677974E+20){
ogLLRAGhdXGqyKGhNLHsYhfYP = Math.Pow(5, 2);
 bool? YFbsplQjOHDGPlMwilRzMTnIYid = new bool?();
YFbsplQjOHDGPlMwilRzMTnIYid = true;
}byte ulQ =  102; 

}public void kKVknjeptthiBIYMBQNLuCLH(){
ulong SInnNsJBpfZRDSxaCdzTikln = 50333433146592009; 
ushort qxceHPAasTMKnUo = 25330; 
string iVHIwZwpnZgHhLwdCZYx = "JuhTtDihjwZJqgA"; 
float bcKpBBULLanjsoxFjACzO = -0.0001132999F; 
byte dEgXQFW =  61; 
float VMghUnhJCVYSgEegHYtVZFzhxWNNV = -5.423479E-13F; 
double pBSdqIkGtVqjlyDT = 1.409169E+34; 
double LXBwNfDasQdldpqhoGKhXkAkUmzud = 2.762169E-14;
pBSdqIkGtVqjlyDT = Math.Round(LXBwNfDasQdldpqhoGKhXkAkUmzud ,1,MidpointRounding.ToEven);
Console.Write(pBSdqIkGtVqjlyDT.ToString());ulong SNnIROsVXDz = 83744163517152954; 
byte iEZGY =  254; 
byte uNNmiHDmTQuFUCsiWE =  103; 
sbyte TMODmJincSOfs =  15; 
ulong QWGRymskHjIkRJwFhjCbYL = 46524093110524594; 
uint DxfHQnWGtDVcbYyuyiXibONfaIyA = 394044; 
int IKIuDO = 88880553; 
while(IKIuDO == 88880553){
IKIuDO = IKIuDO + 423590;
}ulong VwBxByDtWPJaGhQxiYYOuspdRJ = 3825824600954025; 
ulong ZaUg = 85611902699214162; 
string NJybzEeyGRezzQJqKzwOxheSi = "FysoB"; 
ulong dtxToJiXDwdcgQXkuSKmpuGFGStYI = 72736478719776565; 
uint QlRUMbPcoHmYx = 487016324; 
long NSnypjoyfNKHxBYWzUazB = 34465851862851806; 
float TQMKujXAbCyHnfwT = -2.568103E-18F; 
string WzkiUnyHlTUqlnRRKRFMBuYXclpLu = "kheUDuGD"; 
ulong MpniI = 5851844357044072; 
short DBnfIYNlbIMlsMy = -18343; 
sbyte AZk =  -96; 
ulong WRpxQbVzOy = 18248066773530085; 
byte VUcnRtbnjpmwKYjQ =  167; 
short ZNnzUMDTLUsagU = -13909; 
ushort PcGx = 65517; 
int zjFjtmqzJI = 7596; 
if(zjFjtmqzJI == 156015){
zjFjtmqzJI = 540647;
}byte quSpfBwFgbGLaZga =  254; 
long tByETq = 84660866598455744; 
double lxiddOPfoTYHmHHxywUZT = 8.586793E+35; 
lxiddOPfoTYHmHHxywUZT = Math.Ceiling(Math.Sin(2));
try{
int jifxXPw = 3059688;
if(jifxXPw == 27991){
jifxXPw++;
}else{
jifxXPw--;
Console.Write(jifxXPw.ToString());
}
}catch(Exception ex){

}uint YGOyZMqlZWgCAgHiS = 76141169; 
ulong kMoAieFsNgJmT = 41241269155842712; 

}
} public class KxumVXIBGBQiNUzYQHMTmPzo{public void IdAePCzMwbTfWiHLlCh(){
ushort MKkPfFCPDVXeGmUhwRZqy = 22936; 
float STzeianqSWWtopJWXWS = -1.185633E+32F; 
string VNDRKhfmtSG = "lgxGDIHmKqBSfhctBALJREDsGjEi"; 
long DCKtxQCppAoNHElLMCfNN = 50028605303527488; 
ushort yCaP = 64787; 
sbyte MlSjjkudWYgVzbdZWJuNnO =  -67; 
ushort WxSlGPmCZQT = 58387; 
int VDbNFXfVbuDefFxzYZwUm = 53652221; 
if(VDbNFXfVbuDefFxzYZwUm == 186238){
VDbNFXfVbuDefFxzYZwUm += 880297;
}ushort kjetZzkmbIAkbuz = 44543; 
uint IfCSUAJxgaiwwdMxRjyLg = 22; 
byte sVJlwdiPBlnXtpGKEsjdGlwAFEnl =  209; 
uint UoVCEajLWZHUSyAyT = 11871625; 
int bhqfAjHSPXYYLuIyObKtHmd = 336880704; 
if(bhqfAjHSPXYYLuIyObKtHmd == 106953){
bhqfAjHSPXYYLuIyObKtHmd += 102885;
}ushort QYNThExeQEThc = 43051; 
double dRaeQp = -9.583518E-28; 
dRaeQp = Math.Floor(8.940976E+31);
try{
Console.WriteLine(dRaeQp.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}sbyte TxwuIcctzjmIeaJktxyILP =  -67; 
ulong tDRznxXXEKdtkxainKy = 12144008228932090; 
float bejIOoBlVjOpLBqF = 1.78823E-26F; 
double zxePYgLeZMzpcy = 6679064; 
zxePYgLeZMzpcy = Math.Sqrt(4);
int? xliLzmmYMLgNSWCcFeA = 791476;
xliLzmmYMLgNSWCcFeA += 72783;byte klCjwBNVmCACwpknbzypUoIQ =  0; 
sbyte gYpSPSQUm =  86; 
ushort mfWWeXxkiPBBFnVUW = 38418; 
long ooGXXJK = 67531950605062279; 
byte tDTEVcqzQmCWVXJgjlYAZYwnMGNR =  16; 
float dLXEbSGulozhwhOKzooA = 1.408732E-21F; 
long ugFSopAuYFykYtHoESNkBRafYWnD = 3224230170850530; 
byte eVPeiQLDMeIaWSAAyDgATtb =  241; 
int IGjYjMcQORwxJjwxkiU = 42332271; 
while(IGjYjMcQORwxJjwxkiU == 42332271){
IGjYjMcQORwxJjwxkiU = 536449;
}long OUMUSyDiesNVWYfeIebTXdy = 72656118494644031; 
ushort YnVbXEYgapdznYPpa = 46493; 
ushort OQKyEbPFsGSKpsXK = 13046; 
string BcNWOtKFedUDDlWPhFzzW = "uPwzuhWaffXaWzaTWdUxKBy"; 
string zuDQfMFONIY = "oZCIeXPGCoyPnIwFA"; 
string qtiWOtkautliQbsHdsOGEJyOiW = "CyqZoJPHhfdOoTXwlpfIOWb"; 
short LzpJWwORG = 11710; 

}public void ctWBbujCaEANHnkyco(){
string wZNFkzYlLpWJjtsStGH = "MBqJkLKwLXLDG"; 
string cHspDxY = "KHQDmVsexJHUVJdtAPEBNFtdg"; 
byte lapdXOKHbu =  41; 
ushort sToUYjltDZQOSydf = 17291; 
ulong bHgdgFJflccYqs = 35989938155761982; 
double bApeqHBOIMuSlPhppdaqB = -0.0304241; 
while(bApeqHBOIMuSlPhppdaqB == 1.533665E-17){
double AdLycHnFPGCnqGXlWehiP = -1.972699E+23;
bApeqHBOIMuSlPhppdaqB = AdLycHnFPGCnqGXlWehiP / 3;
for( ; ;) {
Console.WriteLine(4.86803E+35);
}
}float DsdIGTHegobGtmN = -2.623639E+25F; 
short tGeisIhdFEP = 27518; 
short yOKtdu = -24465; 
int azMUIzClX = 368111378; 
if(azMUIzClX == 473382){
azMUIzClX = azMUIzClX + 186892;
}ulong yQb = 31082361597055620; 
ulong UjNudKGNAGcqRXXHGoBHcWq = 88272126933250220; 
uint jdiOyuAgnYjZkBw = 84747260; 
short DhLXTiKw = 6175; 
int NHFUIUPShUa = 793314; 
if(NHFUIUPShUa == 593170){
NHFUIUPShUa += 498926;
}sbyte oEwOVozfoFOZVDXkEupSC =  114; 
double pDIWZNF = -6.752105E-20; 
pDIWZNF = Math.Ceiling(Math.Tanh(0.1));
object DaEMZnjS;
DaEMZnjS = -1.442913E+11;long mQigLIVTAyIuUSzdWYzVtHEQkqcN = 4180495763935598; 
byte YwppUDotdoH =  86; 
ushort hNEGmtVtGiFdJHottCSGZcGcMaCRn = 48802; 
sbyte bQTtWTLSwFQhMRUIeKVfAOYehCTl =  89; 
string TbHbSN = "xBRajumzpVgTNERbynSOkwZzZsA"; 
double fQbVi = -1.072062E-07; 
if(fQbVi == -18122.03){
fQbVi = Math.Ceiling(Math.Tanh(0.1));

}byte NciR =  123; 
double RYw = -1.334929E-17; 
if(RYw == -2.407629E-36){
RYw = Math.Ceiling(Math.Sin(2));
try{
int ZIlfyKehIXJqGcEogxEcShQGfUd = 9728471;
if(ZIlfyKehIXJqGcEogxEcShQGfUd == 62139){
ZIlfyKehIXJqGcEogxEcShQGfUd++;
}else{
ZIlfyKehIXJqGcEogxEcShQGfUd--;
}
}catch(Exception ex){

}
}uint tbVfksN = 31; 
ushort wZHiPcSRGaEH = 510; 
byte QmagdWRYcCgMX =  117; 
float EJM = 1.530722E-19F; 
uint CxgPRiQuV = 824274894; 
uint diquEbEIJpUtNYZY = 7475; 
sbyte ZzLXhZPmhkU =  -98; 
double XuqqWnWGTflw = -5.526149E-24; 
double FzumElpByxOywbGHW = -5.668532E+16;
XuqqWnWGTflw = Math.Round(FzumElpByxOywbGHW ,1,MidpointRounding.ToEven);
try{

}catch(Exception ex){

}ushort EJKKRxQNnjUmQmxmWTzCPKt = 2070; 
byte MwNqDbeieBBfTTchTQZ =  110; 

}public void wjHSaiSZToJwlLwZXIThH(){
sbyte NRaGzIREcRVpkfzDSmjkkSKHhIh =  105; 
sbyte ncnoUb =  126; 
int qcMGEqujTzfa = 439188461; 
if(qcMGEqujTzfa == 895317){
qcMGEqujTzfa = qcMGEqujTzfa + 122161;
}ushort RBWwWEScWlyDoUT = 11488; 
uint gpNynycfs = 34180414; 
int VHfCRgpKazsnIbDLbENkJZcLtlzsR = 2339; 
if(VHfCRgpKazsnIbDLbENkJZcLtlzsR == 245655){
VHfCRgpKazsnIbDLbENkJZcLtlzsR = 537515;
}double scciDfSu = -5.490197E+09; 
while(scciDfSu == -1.969338E-11){
scciDfSu = Math.Ceiling(Math.Atan(-5));
object CKbfTkpjGcSxTjfZWHsQjy;
CKbfTkpjGcSxTjfZWHsQjy = -7.286075E+09;
}string ZFfxWcCWzTpMBeoRxEFEWtfaEmKk = "LFYMlzQfmuEkqNdDggxG"; 
uint zOBbGzeSKtqoOnqZnGALjjb = 6178; 
int zxfm = 14221265; 
if(zxfm == 212626){
zxfm += 213041;
}byte qauKkUbZPkgSQgPEoNiwZxHwEf =  180; 
sbyte hfPdNhKPAQyFHmmpmyAstRzcQ =  94; 
float YgfjUiHfsPcYf = 8.416275E+22F; 
double QtiufpIkQyqIMnTlqRtKn = 0.0001929001; 
while(QtiufpIkQyqIMnTlqRtKn == -6.085294E+34){
QtiufpIkQyqIMnTlqRtKn = Math.Ceiling(Math.Sin(2));
 bool? zGBiNPXxmxgV = new bool?();
zGBiNPXxmxgV = true;
}long XkOyHFoVTFiL = 63820040094528628; 
byte QlxZUagBi =  68; 
int VcWflKRzxOsHnFcceUnqfLwCa = 71331276; 
while(VcWflKRzxOsHnFcceUnqfLwCa == 71331276){
VcWflKRzxOsHnFcceUnqfLwCa = VcWflKRzxOsHnFcceUnqfLwCa + 421945;
}float TchqwH = -5.80826F; 
ulong bUQLoXexoZnLjIueEZDJdsyB = 2778076121952442; 
short tIkWxqLWgVTiTAXqcqYeMKypqMc = -8151; 
ulong byt = 46048154227301549; 
sbyte CHlJwOqkH =  86; 
byte KjUGKnq =  45; 
sbyte wQFkjywONgFzVJedIGwQuGeeFD =  121; 
string XHYgIlxdjETQSehFlnlCHHMOoA = "LBGIAtVQAHqnyJZycDZDCXAt"; 
short AlpVkSiwLEFVuyXnIYUaPTH = -21526; 
long oXoOaltM = 76741967444341693; 
double TghDGnoCAnYHGh = -1.159454E+37; 
while(TghDGnoCAnYHGh == -2.75848E-29){
double JEnOcDuVNVlpCdMqbz = 468990.2;
TghDGnoCAnYHGh = Math.Ceiling(JEnOcDuVNVlpCdMqbz);

}uint BIODCKBKB = 97; 
ulong NWhFJhdESIouQadMq = 15333336910748758; 
ulong VZGqdbuHSPZdSyIJ = 5495471316989256; 
string lniYA = "TIiElplsx"; 
ushort XsZcgFWX = 13201; 
byte QAcxo =  249; 
ulong zTpMj = 80017593825730067; 

}public void kaQccfJmboc(){
ulong KBQamDqVsHHQSRBAmYuRzncGlW = 13769604753559813; 
int HJYmXwTtjmkniNOpJX = 4145; 
while(HJYmXwTtjmkniNOpJX == 4145){
HJYmXwTtjmkniNOpJX += 325371;
}float KAGWuAlWTFZgUdwJxZMwnzeLby = 3.133622E-24F; 
sbyte UIucLtNnSLclTj =  -46; 
uint paPO = 59777557; 
uint kwVVWCbRxKsNRsbVhgb = 432; 
int uBuJjqJfcWyBXSsWjawxBZOmXYT = 65225370; 
if(uBuJjqJfcWyBXSsWjawxBZOmXYT == 567678){
uBuJjqJfcWyBXSsWjawxBZOmXYT += 614437;
}uint wDSSAOEoRtSedQkRs = 16087915; 
ushort MgLkzdyzRTUxq = 61814; 
ushort hVJWKWTfUPP = 29432; 
ulong WyggJwgIbKgQaAXIMTQBjLA = 31998969935590818; 
string dXxpknZjBUYQTWthmdWHlE = "YIwThCqtx"; 
sbyte WDBqC =  23; 
float JRBJRTUyeFq = -1.020151F; 
uint zJPwRJjCDKQZRHVDqQVUuz = 589700531; 
string dmOeNExRc = "MjZIApXZbTdQicZEFHqa"; 
long uEFXNyXl = 88345474082288316; 
ushort BZEgj = 17378; 
string fiYQBcZCPwdDDBcPodlVmS = "qllAKHnupTbsOnOzCnnsujS"; 
uint XPR = 4; 
sbyte muIEoDoUCbNJBhn =  5; 
uint KIYAuguKAMSynnLEetG = 1986; 
double tRsxibSKmpBsBANaBjFdEceNzYXdU = 2.308914E-14; 
while(tRsxibSKmpBsBANaBjFdEceNzYXdU == 3.587815E+12){
tRsxibSKmpBsBANaBjFdEceNzYXdU = Math.Pow(double.MinValue, double.MaxValue);
object YfISUeYQYXwCy;
YfISUeYQYXwCy = -7.898835E+21;
Console.WriteLine(YfISUeYQYXwCy.ToString().ToLower());
}uint TQGxwkdmDKpTFHiCdXtEnkOsny = 46304416; 
sbyte lhotufRpFtPaJAqw =  60; 
int SlfoW = 65554794; 
while(SlfoW == 65554794){
SlfoW += 973402;
}uint edyiEllcLOsaOnCobSL = 36; 
short fSsNNeFhH = 22984; 
byte GbhSBqchzOoisfMlpntlPQqqlAA =  246; 
sbyte LhEtWmTajRpiPsEiuHs =  -40; 
float AXfPLRGbReaS = -2.250529E-29F; 
int jztpdDVHbYNceTHOPsJGmgDcuiZlh = 292408211; 
while(jztpdDVHbYNceTHOPsJGmgDcuiZlh == 292408211){
jztpdDVHbYNceTHOPsJGmgDcuiZlh = 432242;
}float BoWcAOMdDWYzmdKyZNcWqWVDKzeKp = -7.969615E+12F; 
ushort PndMENtQYSlbOVSFZFItWJF = 25268; 
ushort bJcOdmCLyPHL = 26666; 

}public void KfseJCVElepUbcQgKKskMLOjOYAdz(){
string njJKRpALSyzxXzilzUpfWtjnZqipK = "Seif"; 
string ttZRjYAOnnRMGNaihXbVZu = "AhjaQFtmRxySeUAVYTwaL"; 
uint LWbJXldfLYJAKa = 681317339; 
sbyte bZphVVnJbdcEuZZLzePWxbNpNUx =  -26; 
ulong ajmfKCgDbfunjacgFKBYCTUmzZl = 72477015023451424; 
float nozFWcKChSNm = 1.877844E-16F; 
ulong RuhoZIKohhbGBwcXjPpucuGRjMq = 40718044966897405; 
ulong EsHGixObeaMPPdQZqSxOpfnpDQx = 19205603408674972; 
ushort PUXRQfLFsKIZMxhGYFzep = 4414; 
short wLm = -1619; 
ushort OysqDYzWBaDMjgOt = 38416; 
ushort NqxhtsMCXxeTm = 13940; 
float kHuUjhbezCFTkmcgReHiqpZYRxC = 1.26548E-07F; 
sbyte BqkEMjBkFos =  46; 
float SpKtXbhEKFUHxdNGBIqldDZ = 1.040012E-14F; 
double UbTYHijZfSh = -6.99031E-31; 
if(UbTYHijZfSh == 6.005433E+10){
UbTYHijZfSh = Math.Pow(double.NegativeInfinity, 2);
Console.Write(UbTYHijZfSh.ToString());
}ushort uQauHBtQkwBhCEuXZRRRSAMsaU = 48528; 
ushort XnMPjMetGFJAHSKNecWpRRnHEb = 59454; 
string uNOZXngPtqRoFulhslIc = "VRQebfZekhRaLlSVBJ"; 
int YMMfamycAlQncdzVY = 923757215; 
while(YMMfamycAlQncdzVY == 923757215){
YMMfamycAlQncdzVY += 164517;
}sbyte oUhumRDdBHjfKdPJXFYqEYINWWGNw =  -26; 
byte DgMsoJmunjDyoPnRgA =  51; 
sbyte Rxqmwypl =  74; 
long sHEEq = 84605231220637925; 
byte gIOGWkitLLwfUkW =  42; 
uint yeJlLALgVNGOcVcWq = 46105872; 
byte UjmTTWpAdxwFRZfQVXnUQo =  105; 
ulong IBdXUnLoQDUwtSEwHSpkikVXgxLj = 36258762701281819; 
byte jiLpaxkuCeLAixeDN =  112; 
int RzDBaU = 117222853; 
if(RzDBaU == 640647){
RzDBaU = 143901;
}sbyte IhATj =  -6; 
ulong UHAPNiKWiORfLMGjogBeAe = 40566589469616441; 
byte ufEWb =  55; 
sbyte xZnTpctWDoFdYImPiWyJuNT =  -55; 
uint MGhKidZOIAREmoLTuOoigM = 789493405; 

}
} 
public class xXFYbMxsA{public void pMXK(){
double ZgLqXZeXodX = 1.315638E-20; 
double VnFIeGGHqW = -223732;
ZgLqXZeXodX = Math.Ceiling(VnFIeGGHqW);
try{

}catch(Exception ex){

}long wBQkVKihGoMQmhYduapxtoDDgQ = 48775740065845332; 
float DmLJtmPmCyqYqPZnolBfXndfE = -1.062503E+11F; 
byte GWdpgCh =  73; 
long fBpb = 27164886334413290; 
long LToiGfmGCfVCnIknSYHiu = 18473101283869639; 
float OntFnmYJCQ = -6.900265E+34F; 
byte hkABz =  133; 
float BoflbOZK = 3.198654E+15F; 
long hmUcWbUofsVHyhmwWTystENeA = 75169467679648086; 
ushort UpxFyIFehtzOUCFNZmpyEBQEUQKG = 15892; 
ushort pejseA = 19096; 
string PAGLbnqJefYCaVuuBTKHXZ = "OmhKLjBnKmFJeUfDHAbChwFpiP"; 
double HGtNQHRyTEGzFRoowm = -0.3680776; 
while(HGtNQHRyTEGzFRoowm == -8.339571E-36){
HGtNQHRyTEGzFRoowm = Math.Ceiling(Math.Cosh(5));
try{
int yjoElRsVAPbcKlJj = 5174309;
if(yjoElRsVAPbcKlJj == 40909){
yjoElRsVAPbcKlJj++;
}else{
yjoElRsVAPbcKlJj--;
Console.Write(yjoElRsVAPbcKlJj.ToString());
}
}catch(Exception ex){

}
}byte XFyPMsBHaCCF =  220; 
long iqn = 27072576498445319; 
long YyRBnzdzlxXRKOESqS = 25797272486421226; 
ushort bSOmLWJEKbpwFDWUpnmg = 49471; 
long iGIHTuEkYb = 49462950108944783; 
double pCzwwhi = 6.601622E-37; 
if(pCzwwhi == -3.121516E+28){
pCzwwhi = Math.Ceiling(Math.Atan(-5));
 bool? HjWMEfQdcd = new bool?();
HjWMEfQdcd = true;
}double QfeYAHwIFaMgEdcM = 115.277; 
while(QfeYAHwIFaMgEdcM == -4.573845E-36){
QfeYAHwIFaMgEdcM = Math.Pow(double.NegativeInfinity, 2);
object OyJnTRORADilfByRiZMiqbjgIB;
OyJnTRORADilfByRiZMiqbjgIB = -2.25392E+25;
}long pIlwDzTVngfGexileuB = 61572454065377929; 
ulong cmz = 70217247736899991; 
sbyte SCwwWtDobF =  -96; 
double ObTGQyeUlTIP = -8.361952E+31; 
while(ObTGQyeUlTIP == -5.186184E+19){
ObTGQyeUlTIP = Math.Ceiling(Math.Atan(-5));
try{
int sweWWkZQTikjpUNi = 1982743;
if(sweWWkZQTikjpUNi == 77096){
sweWWkZQTikjpUNi++;
}else{
sweWWkZQTikjpUNi--;
}
}catch(Exception ex){

}
}sbyte oVOSTCeI =  19; 
double RFjlxzxMWnQkjTkS = 3.253643E-35; 
while(RFjlxzxMWnQkjTkS == 5.52946E-05){
RFjlxzxMWnQkjTkS = Math.Ceiling(Math.Sinh(-5));
int? OckXAKFYIKmFdlcZaGYZfYsmMmDH = 3723563;
OckXAKFYIKmFdlcZaGYZfYsmMmDH += 63817;
}float KXkmgYkfiNiBmIssDuhMiJRP = 3.497967E-30F; 
sbyte UQgPLiLJxRxUiLCKOXiT =  68; 
short YAfGbgw = -2188; 
string EksGuQeXJVLXnDzhgbQWmTGIho = "EKDbtwbGAidtheWetyb"; 
byte hTZuIdGLGLzfkCngUs =  138; 
byte YZdJJSqOljgccxQDILqwUUyGnXEI =  245; 
uint zZGiU = 720669016; 
double oWtCqcify = 8.848688E-21; 
while(oWtCqcify == -1.407692E+24){
oWtCqcify = Math.Ceiling(Math.Sin(2));
object nlPzkulznjDetzqnYHgHxKZMSDYq;
nlPzkulznjDetzqnYHgHxKZMSDYq = -6.15312E+34;
}
}public void IxVEiAyCNnhqiiLgRpf(){
float ZQMxYjoKfGWUiQaFcluwI = 1.86467E+29F; 
ushort YsfNJn = 8490; 
sbyte JGjgjpiRIIlxZtLDOeMybjVopZSi =  111; 
ushort QLlguwVbdlQh = 14951; 
byte ZYwkUjwMVzKURkzaUlqxM =  191; 
byte gTcJklcKdGZ =  219; 
ulong CgwZqJKBeRFlVWVPYmNedb = 37661312649489413; 
byte mhKjbMxDHHOldZpgPoI =  190; 
sbyte JAZJAHmJKTGYZOlMOlRjy =  -18; 
byte JBunlDEzBEAjjsmPpXXVGeaDVdB =  104; 
double llmdZYmYygohzxncCwZbUAAspEB = 6.318233E-10; 
if(llmdZYmYygohzxncCwZbUAAspEB == -2.393182E-26){
double XftoCqBQGzH = 2883054;
llmdZYmYygohzxncCwZbUAAspEB = Math.Ceiling(XftoCqBQGzH);
try{
int exUaMNVOxzwFofgGTe = 5665183;
if(exUaMNVOxzwFofgGTe == 99004){
exUaMNVOxzwFofgGTe++;
}else{
exUaMNVOxzwFofgGTe--;
}
}catch(Exception ex){

}
}long RXVEBHXMIFFBcsXQfMkLAT = 81729812842805289; 
ushort diWzDSdIhuXoABcdeUtROYm = 63877; 
byte pNCUamgSxTxznn =  166; 
int eVzaMLbXRaGYfWXog = 320893; 
while(eVzaMLbXRaGYfWXog == 320893){
eVzaMLbXRaGYfWXog += 694340;
}sbyte eiIHGfjkwhdMwFcAXjmB =  94; 
short zihNqxoMkpsICuplVxNLPUELXnHp = -20092; 
ushort POliyytSOfxSwdWBeHXq = 5582; 
sbyte hEGyPcQYCOIEUokTODF =  120; 
int chHaOCKlfDJh = 59; 
if(chHaOCKlfDJh == 570600){
chHaOCKlfDJh += 796207;
}double VdNBck = 4.464322E+20; 
if(VdNBck == 2.336398){
double yQOonooIeUMkHaOYPa = -4.300555E-36;
VdNBck = yQOonooIeUMkHaOYPa / 3;
int[] gYEIVcqcCfodwj = { 2304137 , 32601 } ;
Random BKSGxIGpZmNwQLhFqDcKQMGt= new Random();
Console.WriteLine(gYEIVcqcCfodwj[BKSGxIGpZmNwQLhFqDcKQMGt.Next(0,2)]);
}string HWEGEinpFSDNB = "EwhHBquBXBsAYPezueGl"; 
long KhRH = 64752513500556069; 
sbyte lFRLZfYc =  -67; 
string zkgzygoX = "LZQTUDByAcqQHTmP"; 
short lFeHVNxYGTbEiUkIBlPXRyFCEjO = -7421; 
float UJilcswdecxV = 4.683459E+11F; 
ushort TxFZsaaqePaEdMZcgnYtuXRSmxhN = 34731; 
float WfHpqPSZTJShnsRZs = -2.031052E-10F; 
ulong JcDWGNMVTnPindzywInn = 55607274742593118; 
ushort JhgTnceuMcfdZwcYaotDXwfB = 25283; 
int afcmqzeFHcuQ = 364418; 
while(afcmqzeFHcuQ == 364418){
afcmqzeFHcuQ = afcmqzeFHcuQ + 163119;
}byte oASDk =  8; 
float ncGzKKeoJQPGVRxVGAXiWzUsj = -1.882043E-34F; 
uint AboEnpFyW = 72; 

}public void NnwtFkzcXX(){
long MVPhn = 15417159966925131; 
string BRzKtlUjkzYFnGzpCpkR = "BxiHUfMLdIUD"; 
int lznsSRMFKpLXaeLIynkoZwoczfiz = 81; 
while(lznsSRMFKpLXaeLIynkoZwoczfiz == 81){
lznsSRMFKpLXaeLIynkoZwoczfiz += 656562;
}byte nXtZHWNTWXfHaxjHsnXQjkTNJ =  64; 
uint lJmsSTtAVlIdJxgCZCzLGyKxLx = 6049819; 
string ptzTHmKOPgcLMfHUhNd = "NgPNmPZQIxlp"; 
ushort VnGPQURlNs = 42521; 
double EQRnUZMSiFLGSmUfsUMoxk = -2.11612E-18; 
while(EQRnUZMSiFLGSmUfsUMoxk == 7.446551E-25){
EQRnUZMSiFLGSmUfsUMoxk = Math.Ceiling(Math.Cos(2));

}ulong Dnlu = 75426740398462287; 
ulong RiUB = 44385308048827577; 
long PiwEbOqaYiUgUDgenRNBFQeKJzj = 46288910819387670; 
ushort qgaMQkLHUuqJeygBhpYdLUEFFV = 31679; 
ulong qLuOjXKMPPGudPdSebb = 37842101138094498; 
int gVLYaEKXUUBxOt = 356121; 
if(gVLYaEKXUUBxOt == 116968){
gVLYaEKXUUBxOt = 481343;
}ulong TyxSJQkzbCgpIUUw = 70167243711212413; 
double tRHCpeKHbnD = -7.030692E+30; 
double LHynFVeSpNVDTtxu = 3.925471E+36;
tRHCpeKHbnD = Math.Floor(LHynFVeSpNVDTtxu);
Console.WriteLine(tRHCpeKHbnD.ToString());long RwKFEJDAtXsOKbAEQbPLEnS = 66048428260447909; 
long YCWmQxiNPZcWONpfPXlcYjEEdRA = 42340774132028426; 
uint qZGGXPnPKZqCceReG = 769693794; 
double jZWqsjxxnVbQ = 0.001027636; 
double tRipsAMCcWXCjgZKnFFStxep = -1.168666E-07;
jZWqsjxxnVbQ = Math.Floor(tRipsAMCcWXCjgZKnFFStxep);
int? ziWtKFQiNbaKKUkYdntqYXhwQx = 9246882;
ziWtKFQiNbaKKUkYdntqYXhwQx += 24417;int izkgmyfoUCaBDniSyUygRQRqYzx = 819258; 
while(izkgmyfoUCaBDniSyUygRQRqYzx == 819258){
izkgmyfoUCaBDniSyUygRQRqYzx += 815382;
}float WMGHgOEYC = 0.1803314F; 
string yzsbPzesIUiOaqoMBf = "IalgSnqsmRozzxSqCQZgs"; 
float oSySfyombNPuEksBOygRCtxKqha = 2.011678E+31F; 
uint sRgXowwkVXEmLKcofjokJkZtVwNCY = 544555; 
short PxDxmdftmlTXfMan = 23152; 
float GpogSNbTIUmzwlNWmB = 3.576631E+27F; 
sbyte SxZfxQKN =  -65; 
sbyte ZMjAoKKkkyAZNzLtQ =  65; 
ulong bywzThNqljVHgWxAExXQiHVNtxlHL = 71213401308798166; 
ulong hXsZaCSTQblQtQYQwCI = 24935046661192812; 
sbyte yUi =  83; 
float SRIUkBEwFzHBiBpeZJHilSMj = 9.427645E+08F; 
long thdYDafZNIyETBBhiPMILsUAj = 44975934525387890; 
long ywJY = 61974364724915299; 

}public void qEbWUcgbmEOVxeUnzSNhzc(){
ulong QEmuaDngqRJCylLP = 45842332733616568; 
ushort Utphn = 18606; 
float fpQNbKoHEGxsQlhYE = -3.275691E+25F; 
float NRWXVJVRjEuLWguYSZB = 5.216632E-20F; 
uint UenIls = 822303; 
ushort WtzTPfbtQubbIUfNZLyqqqnLcTIa = 44871; 
uint NXPZNVfdUzjyfeSytD = 350774; 
float fFFeORVybKqQ = -2.115561E-32F; 
float sRUIEShUhGkxnzkgLuRjuJhnhxy = -1.181118E-32F; 
short NFzjIzlPmxLWiHzjEAkeVmJC = 12373; 
long BhwIPSPySQnDJAFFNKM = 67537139478011033; 
float VWBYSLISPVetiHtp = 901.4882F; 
int JVSYKXRVCIU = 88314841; 
if(JVSYKXRVCIU == 872949){
JVSYKXRVCIU = JVSYKXRVCIU + 762117;
}int OXMYOHGzdTEdtGFRaeCnUUMqg = 45; 
while(OXMYOHGzdTEdtGFRaeCnUUMqg == 45){
OXMYOHGzdTEdtGFRaeCnUUMqg += 499660;
}uint LRtWXdqeANbXE = 28733; 
short WOloW = 24047; 
byte RNjflttcKIOkU =  15; 
string jwhKuDAx = "hNMpePYi"; 
uint phYmu = 90358881; 
float mdjTNafBMBbnmOVeHIMGSPctXL = -9.205648E-23F; 
sbyte bzJGigXTYpZnmJBRdudhNbi =  -61; 
string bRVbXUsYADFHGPh = "JxHFDuSgIKhaaZowFKefXXs"; 
string niYlxOnaatNHxVIZPunNsYAmjcV = "bPMdDJnkUVkVTZZHDPhPy"; 
sbyte DesEpKM =  -117; 
ushort EYkIKeOHxfUisTsYSLuUfmVGiY = 13406; 
sbyte BhSRboaPZSLuhaaotMzkqSbIBOqd =  24; 
uint TJJWXbsqEqjSSKAYlVqPlM = 623716865; 
long kqnYXj = 57919697008326023; 
string UThZiYaPfhnJz = "PcnQQb"; 
string uaqLSOAgRcORRazJXmp = "SPVSIKXwRmzZsoA"; 
sbyte zfmoYqlVIpOEFTELFNFe =  -5; 
string QeoFFOA = "BqaAgdjTzKWatiRmu"; 
float NCVcixXxkK = 8.162015E+09F; 
sbyte OnmFuxI =  -26; 
float uDbPIm = -2.346423E-23F; 

}public void AMaFnRdjHYjMdQTGVFJay(){
long IQIHuhPYHCWI = 78372456522369923; 
ushort tqNijMIukmSutPGkJbSaE = 13148; 
sbyte ySmGVKk =  33; 
double xpUlYcplqKQbuTtDIkhwgikFLqAc = -1.250152; 
if(xpUlYcplqKQbuTtDIkhwgikFLqAc == -0.0005646836){
double ydPKqTxsxytm = -2.05129E-11;
xpUlYcplqKQbuTtDIkhwgikFLqAc = Math.Floor(ydPKqTxsxytm);
int[] BRmfoBHEnNQIFEZB = { 3088039 , 77218 } ;
Random lkhtLP= new Random();
Console.WriteLine(BRmfoBHEnNQIFEZB[lkhtLP.Next(0,2)]);
}byte UgQfAcMXFf =  48; 
string xlxFcPNSzSigRSfPbXqclMyyHqFT = "uuTknLgokmZBxxnHYIk"; 
int DiSYLLP = 953013179; 
if(DiSYLLP == 881867){
DiSYLLP += 232409;
}string oVEgZBRKsDQpiRI = "ulzRkamEaqpmCPHjtPqde"; 
string IMABxVuGiobQIOlmoJZg = "kFhEVGXtAFFpGRfMBRJG"; 
long XBYZxg = 61299871323497126; 
short BoylJLyBcXfCORqBWdHIFWiQRxjA = -10381; 
int YtEqthAzmczWA = 2; 
while(YtEqthAzmczWA == 2){
YtEqthAzmczWA = 454600;
}string BLsEgIwlBXix = "hhjAYsyGuFgaKOQhGM"; 
float ayJlOlBelMfoBWTTdd = 9.23525E-31F; 
int gfkEyTFeVknTDLAYMMStuIA = 488072822; 
if(gfkEyTFeVknTDLAYMMStuIA == 954362){
gfkEyTFeVknTDLAYMMStuIA = gfkEyTFeVknTDLAYMMStuIA + 79765;
}uint OXmpDbkPHAhQUaIuitfsdnmtAHtd = 96574600; 
int tnB = 297003364; 
while(tnB == 297003364){
tnB = tnB + 531194;
}double TmRFQStN = 9.948555E+34; 
if(TmRFQStN == 12.1334){
double IgfJThxGBMeOyiQgdtgIbElplcbj = 6.760163E+28;
TmRFQStN = IgfJThxGBMeOyiQgdtgIbElplcbj * 2;
Console.WriteLine(TmRFQStN.ToString());
}byte ngbm =  239; 
byte ICSIuelBbekahOOEHPIWYyWZ =  189; 
double QOOjTTxEZKaQtIVnxhaenBEwzo = -5.777959E+25; 
if(QOOjTTxEZKaQtIVnxhaenBEwzo == 2.0464E+27){
double NnUqgLkOTGYnodj = 0.0001018018;
QOOjTTxEZKaQtIVnxhaenBEwzo = Math.Round(NnUqgLkOTGYnodj);
 bool? FoksMFDFtuRetilhZCQTtdNKPA = new bool?();
FoksMFDFtuRetilhZCQTtdNKPA = true;
}int DVhNFf = 373648039; 
while(DVhNFf == 373648039){
DVhNFf = 112300;
}string jYbSzCDBEmECRsoPVODeGetcomm = "HnUUYTRgaqge"; 
int sVj = 30; 
while(sVj == 30){
sVj += 618128;
}float YoeFYDkojUosJDXCiP = 8.889698E+09F; 
uint FNyTlYOotIbqaENCb = 91250390; 
ulong gcDBBqXVzckfy = 83260320902196541; 
uint psDuPHNPMJgyKUtMHRtU = 83085986; 
byte TuuXyJnAGwlhXXinLn =  122; 
ushort mImMGHKBHcA = 8510; 
double umQqduIRoIkOJIfiq = -1.777623E+20; 
if(umQqduIRoIkOJIfiq == 4.779785E+17){
umQqduIRoIkOJIfiq = Math.Exp(2);
Console.ReadKey();
}byte enuWLFitDKaeCDCIGJkAgVpzul =  251; 
int KsbKhqjpkIhNoEU = 938562; 
while(KsbKhqjpkIhNoEU == 938562){
KsbKhqjpkIhNoEU += 262868;
}short FOOiXWDzGGWTsZuu = -18354; 
short wbdNRswayXTm = -20636; 

}
} public class RxWuiluJiDhtK{public void zCUVZWUACaWRDWeBM(){
short zCEzZcfXMSBGzITDVdYnnA = -9556; 
float zsKzEIQPHkbUdUSTYhHawyiJtiy = 1.677867E-33F; 
uint iIECLxzikLQSySISiguosMUBaiaeu = 5; 
long lSmBtgy = 69919050338951691; 
ulong oIJjHxOcBDw = 19345220399194927; 
ushort PZXPspEVwyks = 4297; 
double cXFwzySFcflwI = 1.666084E+30; 
while(cXFwzySFcflwI == 3.60148E-14){
cXFwzySFcflwI = Math.Ceiling(Math.Tanh(0.1));
Console.ReadKey();
}int QZHJCLj = 2830; 
if(QZHJCLj == 214472){
QZHJCLj = 907142;
}string GFCUqbhHZUAzQhdbCfs = "dUIS"; 
ulong FXNAqsQhDHMTofItHOcluikHdeEZB = 41967064334785450; 
uint gJJFxoIRluLnUPCGxN = 41; 
byte QHSLbNXJzzhkVDLEOqxnqFPy =  9; 
float MayNdssTbNMnUTDWlf = 1.471813E+28F; 
double TYQZXykRwJgikJaMdSEKKQcdDu = 1.010397E+13; 
while(TYQZXykRwJgikJaMdSEKKQcdDu == -2.481335E-12){
TYQZXykRwJgikJaMdSEKKQcdDu = Math.Exp(2);
 bool? TVBcRpokcWMyDIDndqfEt = new bool?();
TVBcRpokcWMyDIDndqfEt = true;
}sbyte kIRmeWnLahostNdsI =  -71; 
long VPGQNPlPQumLxZDqIw = 38692415123882829; 
ushort kideWCHxbaA = 20450; 
long VGUFWtjMLmnitHTW = 57784316675070293; 
ulong tgpHVIRLtpQTHafAyQRNhZDfIpV = 70740746332353726; 
string eqHsJmDgBEuQej = "qbNtEkYGMcPebddG"; 
double csAj = -0.02386505; 
csAj = Math.Ceiling(Math.Sin(2));
try{
int dZanKossDQ = 1978859;
if(dZanKossDQ == 77590){
dZanKossDQ++;
}else{
dZanKossDQ--;
Console.Write(dZanKossDQ.ToString());
}
}catch(Exception ex){

}byte mgZbVaoSKlUhmthFcllGmS =  134; 
long epge = 53971972223984713; 
short QXWGTNylDEkeYDHAwGsZ = 20230; 
long NGtJtLW = 32717486641322547; 
long FTNEUaUIgcbDasRfQiaAddCGEYt = 67928396957292704; 
double FTGzbmJVyktNpShkiRCsgZVHBBgU = 2.359993E+07; 
while(FTGzbmJVyktNpShkiRCsgZVHBBgU == 1.175841E+30){
double bcRSLgOZIRhbYPG = -1.473879E-22;
FTGzbmJVyktNpShkiRCsgZVHBBgU = Math.Floor(bcRSLgOZIRhbYPG);
try{
int YYYBfOcSIwShHRQgPLAw = 1901052;
if(YYYBfOcSIwShHRQgPLAw == 15937){
YYYBfOcSIwShHRQgPLAw++;
}else{
YYYBfOcSIwShHRQgPLAw--;
}
}catch(Exception ex){

}
}long hfwNQqeRfWibJoDRaBtdnYwN = 87047038141110203; 
sbyte XjVSBfY =  48; 
int BmyolTA = 79633465; 
if(BmyolTA == 177988){
BmyolTA = 696683;
}long PnaSHnxU = 56912304801271794; 
int mmoFNSgYYWEG = 49727607; 
while(mmoFNSgYYWEG == 49727607){
mmoFNSgYYWEG = 838280;
}double YUdtWXTXqLDAYRywVkCiLJoBp = 27276.85; 
YUdtWXTXqLDAYRywVkCiLJoBp = Math.Pow(5, 2);
ulong etFdNHTIuYTQRmNuBYHOYF = 80747283630886247; 
byte cLcYItqFfUqkJkjT =  123; 

}public void eRiZOAOaPhjbWFPsOfjOMgIcIen(){
ushort dwDsRLsscXDkHFiueXMxpydKLhX = 12350; 
sbyte KlueLNHKfTxa =  51; 
sbyte EeCzIEgDYFHqTHJOilMm =  -45; 
float UwOuSSZpNUbhdwLzO = 4.022413E+12F; 
ushort ewNcyfmPZVqNDIdU = 11807; 
short YoTySAGszNXLIwbQaaswM = 15901; 
uint YLwBOIFFnYylkYVERNPbbkJ = 5247; 
float TRTY = -1.033106E+19F; 
sbyte geeFQhtxJXQFbeP =  17; 
double BMNccZWFLNwalzfBPzMiZTIPZwLm = -8.801462E+21; 
if(BMNccZWFLNwalzfBPzMiZTIPZwLm == -4.298426E+36){
BMNccZWFLNwalzfBPzMiZTIPZwLm = Math.Ceiling(Math.Acos(0));
int? WKfbYlMPKyGBNxbPqL = 6799085;
WKfbYlMPKyGBNxbPqL += 50633;
}double IMfNNGTkpkjdiMXTVcyXISIFybJ = -2.576187E+20; 
while(IMfNNGTkpkjdiMXTVcyXISIFybJ == -1.10892E+33){
IMfNNGTkpkjdiMXTVcyXISIFybJ = Math.Ceiling(Math.Tanh(0.1));
try{
Console.WriteLine(IMfNNGTkpkjdiMXTVcyXISIFybJ.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}long VdPe = 59888513525022157; 
short USQCsenFiJqLuRI = -22845; 
sbyte jlKaGUFZXhfjxmdxlOeswGPpV =  -94; 
ulong PxIfqCahO = 76778712092739905; 
double eHXFTdNxE = -2.913129E-11; 
if(eHXFTdNxE == -6.888596E-30){
eHXFTdNxE = Math.Ceiling(Math.Sin(2));
try{
int cFexLuNPPFhFHAesRKolsUpt = 7025584;
if(cFexLuNPPFhFHAesRKolsUpt == 72661){
cFexLuNPPFhFHAesRKolsUpt++;
}else{
cFexLuNPPFhFHAesRKolsUpt--;
}
}catch(Exception ex){

}
}double QRlnGNuSVU = -8.772967E-13; 
QRlnGNuSVU = Math.Ceiling(Math.Atan(-5));
Console.ReadLine();uint xbCdZhZfQba = 469913357; 
uint QIQnghIljKeXPGMPzChGFexApkIuq = 81; 
short fAhkQsiOQqt = 25615; 
byte BdlDnpVu =  185; 
byte YWAmVjTOHJf =  149; 
int jZRPNcFfahpezDZwMOWV = 429415897; 
if(jZRPNcFfahpezDZwMOWV == 896237){
jZRPNcFfahpezDZwMOWV = 144903;
}uint FCwAIU = 33; 
string YiMTVsWQMpPkXqJJRNq = "UGDACANRyEy"; 
long fPNiYCqGewj = 55759784437042856; 
string LtHmqwGeshSgURCVASlaKPjs = "hVJazBg"; 
long uxgWHfLBMnqjAMDVA = 29396564306929514; 
byte WeUIMzqxfPDkgojd =  234; 
string JjgeZoaIzzlSSyGPwSHFMhoxfWaY = "USnngCpQZUsMQTYTXKOBBO"; 
byte wlBUKZdobW =  143; 
int EMWoYxXnTWoqEillzkgIYTUaxgaRc = 87083999; 
while(EMWoYxXnTWoqEillzkgIYTUaxgaRc == 87083999){
EMWoYxXnTWoqEillzkgIYTUaxgaRc = 370681;
}double kCsmbCpkDgQRcEMxYaLSVTRwf = -7.564337E-24; 
if(kCsmbCpkDgQRcEMxYaLSVTRwf == 4.127557E+37){
double mJPuXqFQTZkaafh = 9.871256E+22;
kCsmbCpkDgQRcEMxYaLSVTRwf = Math.Round(mJPuXqFQTZkaafh ,1,MidpointRounding.ToEven);
try{
int fVsLWhQqmlEzaEIqcIFct = 1422645;
if(fVsLWhQqmlEzaEIqcIFct == 14873){
fVsLWhQqmlEzaEIqcIFct++;
}else{
fVsLWhQqmlEzaEIqcIFct--;
Console.Write(fVsLWhQqmlEzaEIqcIFct.ToString());
}
}catch(Exception ex){

}
}uint YSmgaqEpCjxsOekPRoSQsSLbFls = 93; 
long wixdPsWtWDKpaJTpQVRWSoKDfh = 29604601231674178; 

}public void jsa(){
ushort VVVmIDKsMWCkfO = 391; 
sbyte TYIguGYeqzJIJYQ =  -4; 
ushort Wtx = 59393; 
int TQSxmmdqULzcOpC = 9509; 
while(TQSxmmdqULzcOpC == 9509){
TQSxmmdqULzcOpC += 13379;
}long XMML = 52537443476757203; 
int CRewsNYANLeAGxmFAyiNUKC = 336405; 
while(CRewsNYANLeAGxmFAyiNUKC == 336405){
CRewsNYANLeAGxmFAyiNUKC += 510958;
}long bXDwslJJXeyZaGlyqP = 84649547691958531; 
uint eGoZ = 13; 
string daVsQOkIxLCWeQRTg = "SWfKEcLGFqPzTUDKupTbeY"; 
float xgOFYnPx = 1.617395E-10F; 
short ytxTIk = -20398; 
float XRHuwxBZfIUbWnxnfHpDtHBGwl = -1.514129E+29F; 
string TOaqzphjDSnxM = "ufCfZGDETszqqbZiuHZMOf"; 
string PggVeznXUPloLVUuRp = "IShYfkuyIxZXneWpSAHAudCxDPL"; 
uint NFVzktMIiaLLbGBU = 963045576; 
byte GbVjQBFqLxpAWYVSeAcLjJKA =  236; 
ulong yaCLRAJUMKLIL = 57302755162231893; 
long QsVAFWqwnbggpiLAamejh = 6684728985229294; 
long pNs = 16610314257872671; 
ushort FJfObkkCYCCxBEEPjtHD = 613; 
ulong YyqEZhgPTWjNn = 51369627393017073; 
float SsyoNaIUHO = -7.087261E-28F; 
short IkuSMIBpQogkybcCzihhz = -18816; 
int tPlcEZZCIlXoeBziPDChiCDGR = 30; 
while(tPlcEZZCIlXoeBziPDChiCDGR == 30){
tPlcEZZCIlXoeBziPDChiCDGR += 225680;
}ushort CzXwyNRpFEleNxYKYtWK = 26624; 
long lGkzKTCAZBHEbCOqPLSFYpCeJ = 37229779259946573; 
int PmnhxddlEfTWikbgZEeSVe = 81; 
while(PmnhxddlEfTWikbgZEeSVe == 81){
PmnhxddlEfTWikbgZEeSVe = 696161;
}long uKwPYeCDwBVeYYPwmSOiOKO = 41168361392960088; 
ushort QMiBtlWFxhEXRgibW = 14131; 
long CZKVHZmYeNyWDDhlGUTGREsDkRYF = 52895687214700609; 
string UuZYBJKHu = "YWwbsbeYIh"; 
long aIbJTKZfDsjtaAGTdJSEyKHa = 56689410331634872; 
int tsWRQ = 642256108; 
while(tsWRQ == 642256108){
tsWRQ = tsWRQ + 737761;
}string Txl = "ZogsqbaIaoeNCjpQWgnWlSSWhXa"; 
short SDzgfiFxMbqWiPLDCkxxleNioKgyj = -19397; 

}public void aMKiNGHwbLuje(){
double cdhuGMFSMNQpRxbhlCHpkAGXXVQuL = 3.522158E+30; 
while(cdhuGMFSMNQpRxbhlCHpkAGXXVQuL == -2.495454E-16){
double FJwTYxXpQZaEhIyHPEyjxLWkg = 8.074168E-34;
cdhuGMFSMNQpRxbhlCHpkAGXXVQuL = Math.Floor(FJwTYxXpQZaEhIyHPEyjxLWkg);
Console.Write(cdhuGMFSMNQpRxbhlCHpkAGXXVQuL.ToString());
}long LzMycmjQHfRZjGXBbMqOcd = 67097332392661232; 
long yXCLScatVEm = 59131637460697603; 
ushort XJIlcTGHZuQzADldlIYLYS = 31737; 
ulong oRDJtLDjFAoUAQiwMWj = 67359741342933577; 
short OejRGNbcYHJmnVcXjNRQpl = 28003; 
short ywHUPNHnzRue = -29531; 
float htOp = 4.204441E-26F; 
float hLKSlzxCiUziswijTbYPdIThUPIXz = 2.307441E+21F; 
sbyte wzjeUwQcoljELLNjFx =  -7; 
byte WASDY =  113; 
ulong dDfoGWWYeYHiNOxJ = 33833120728692718; 
double AYsDWYZElKdofOQ = -0.2810097; 
while(AYsDWYZElKdofOQ == 2.098748E-36){
AYsDWYZElKdofOQ = Math.Pow(2, 2.1);
int[] DcJjiDNGndpfCuLyWeqU = { 9499952 , 31430 } ;
Random DfFeRbpnY= new Random();
Console.WriteLine(DcJjiDNGndpfCuLyWeqU[DfFeRbpnY.Next(0,2)]);
}float XxjRjGqDLNdPkPIsud = -5.777408E-34F; 
uint WdfsBxspfZnXynneULEHStCoGmJVF = 189280707; 
ulong XBfMRf = 81060299220236280; 
ushort oiwAR = 9695; 
int RNnOxLlQAQAOhtdGJ = 433351; 
if(RNnOxLlQAQAOhtdGJ == 760545){
RNnOxLlQAQAOhtdGJ = 333125;
}float SpLPopQXFV = -5.139223E+26F; 
sbyte KunczTFWVfQQwLPedwtzFwCb =  -80; 
uint REyZNDHNtjLTaKWuFYAUe = 50; 
float zIknSmSXjOsygcnPYxUsD = -2.617547E+21F; 
long WcL = 58223555608509199; 
ushort BWTWRVEWzWlegMG = 33096; 
sbyte TDmVk =  -91; 
short fnSQNttBWXAHCTeezfA = 2128; 
uint zfjiaTkFMLFlPoeIiSMGPIbBEayi = 91773; 
long giQMHFtBoqlObpdPZQXHQSgQ = 51111881238729931; 
string eGAsaFAlpzxmAnWJiHhbYncZ = "YZsemkFLkkypHPshFCJu"; 
float qdNTAAbR = 677375.1F; 
uint cGggdnSDndAsDSQTuMtGR = 65825611; 
short uLeJutTRWMNa = -2654; 
uint DnWmFuqIKixekT = 290026559; 
double PJENTUKGCGfcSWnXdDSwKXdcwB = 1.996078E-16; 
PJENTUKGCGfcSWnXdDSwKXdcwB = Math.Floor(-4.162301E+21);
try{

}catch(Exception ex){

}int tgCjIJAMl = 58; 
while(tgCjIJAMl == 58){
tgCjIJAMl = 507385;
}
}public void QJaLwlOReKLHQj(){
int EjeoGDsEAfYpS = 623603210; 
while(EjeoGDsEAfYpS == 623603210){
EjeoGDsEAfYpS = 360650;
}byte VRgtLfXYSWcw =  45; 
int RkSmpxkpnyY = 7; 
if(RkSmpxkpnyY == 976488){
RkSmpxkpnyY += 831203;
}ushort IsYW = 58924; 
uint GIjWVJpqtqZfVV = 52725088; 
float MWUyOXkagWXToXPUREgsDoTNgNZ = 0.01364451F; 
sbyte ReaiCChUlLVDuuAbcqbefuCtxchqa =  118; 
uint ZYfPnOmtJKUHINAsI = 92643488; 
ushort cSxEINAuRTnktDx = 17742; 
long odIwxBUMJEPJncfdXfKkHaO = 49369101383884891; 
string YHqQHwWKWdVFhGlfpGpFAgnoJEGU = "yzBLMpwOnpKOCKX"; 
float KkuqYFOByRtch = -2.617189E+23F; 
string zFwUwoRsb = "GRYyqxnUwAg"; 
uint NkozbLfmUMpksyBnVEYWoiJzucH = 182048742; 
ushort eZLJKfRPHVdLeJlZY = 49591; 
int ViEGFCQdZelcAikl = 77299549; 
if(ViEGFCQdZelcAikl == 77124){
ViEGFCQdZelcAikl += 771398;
}short lyF = 20741; 
sbyte IgEV =  34; 
double feipOQDKalWyas = -3.05648E+33; 
while(feipOQDKalWyas == -1.397278E-28){
feipOQDKalWyas = Math.Ceiling(Math.Acos(0));
object JISpYPqklJtiKyjeBA;
JISpYPqklJtiKyjeBA = 8.391991E-18;
Console.WriteLine(JISpYPqklJtiKyjeBA.ToString().ToLower());
}uint uhkRCoGfjbkiUnHMlZVu = 5; 
byte QLxDyeKNLFokXTVVzupeJDdXJgI =  28; 
long EqYoaQ = 70532966924062407; 
double PteaVJNemmIzUp = -111070.1; 
if(PteaVJNemmIzUp == 8.662089E-39){
PteaVJNemmIzUp = Math.Ceiling(Math.Tanh(0.1));
try{
int bigAYYBWCsbh = 6976547;
if(bigAYYBWCsbh == 96389){
bigAYYBWCsbh++;
}else{
bigAYYBWCsbh--;
}
}catch(Exception ex){

}
}string aGnUDsAkUzjKQBubp = "CYqiVyEDOu"; 
uint KhoRGAJOJcdPZOnmnLubtwxe = 83; 
long qaFeMePcuYUUtDGAUDjhjqnca = 75412378958188815; 
float IyZdUZlSpRAJQP = 9.513862E-23F; 
long CIaMDemJBqksgeBfYbVA = 31383171639752887; 
int OYZfnkxWVUQlAUH = 703400379; 
while(OYZfnkxWVUQlAUH == 703400379){
OYZfnkxWVUQlAUH += 834791;
}double ittJCOqFycb = 7.964568E-22; 
while(ittJCOqFycb == -2.329259E+24){
ittJCOqFycb = Math.Pow(2, 2.1);
try{
int SJRMUCVhFpsiyquGKFSbP = 2272650;
if(SJRMUCVhFpsiyquGKFSbP == 73214){
SJRMUCVhFpsiyquGKFSbP++;
}else{
SJRMUCVhFpsiyquGKFSbP--;
Console.Write(SJRMUCVhFpsiyquGKFSbP.ToString());
}
}catch(Exception ex){

}
}ushort RCQUSctqxkWlw = 50776; 
float bdHRewJdnVmFiuzTl = -7.804895E+35F; 
double mCIjjSLSBAOTOThfGqxTMHXQPicNc = -4.706305E-35; 
double UDXlNxAMuCpOGbDdnHFS = 9439.342;
UDXlNxAMuCpOGbDdnHFS = Math.Sqrt(3);
mCIjjSLSBAOTOThfGqxTMHXQPicNc = UDXlNxAMuCpOGbDdnHFS;
object QBxYXleFgYxTVBTBqcPCxnuamSoi;
QBxYXleFgYxTVBTBqcPCxnuamSoi = -3.393873E-38;
Console.WriteLine(QBxYXleFgYxTVBTBqcPCxnuamSoi.ToString().ToLower());double HaNDfVeHBQnGeMHOwFKCwUFTHAA = 1.53743E-28; 
while(HaNDfVeHBQnGeMHOwFKCwUFTHAA == 4.284123E+13){
HaNDfVeHBQnGeMHOwFKCwUFTHAA = Math.Pow(double.NegativeInfinity, 2);
try{
int wkAWaGtDHaUtbyTosUPa = 9168425;
if(wkAWaGtDHaUtbyTosUPa == 83644){
wkAWaGtDHaUtbyTosUPa++;
}else{
wkAWaGtDHaUtbyTosUPa--;
Console.Write(wkAWaGtDHaUtbyTosUPa.ToString());
}
}catch(Exception ex){

}
}ushort GLzmHiUyHdYCkuCeVsM = 15125; 

}
} public class sqqCCnQUmgGxJMjqNICfq{public void GppQZKWkEfkWmuhMfUdEuXteDIqN(){
sbyte jgtwLbkMofYcDAlNzQjbD =  32; 
string EIWlmqsibTVeDSJfBtGXFqu = "TQiCEhhEeFSceZzXOQaKEgjGhd"; 
uint bdilmdt = 153593; 
float ULsZYaGbBUIjPpLoMH = 4.410622E-10F; 
uint ZDbwGQbHbQXkgZlhkGC = 2; 
short UtWDOHRFxdnL = 20147; 
double wQZUTSmFjMdEwXSzF = 1.759451E-25; 
if(wQZUTSmFjMdEwXSzF == -2.567762E+14){
double kBgSnwjJk = -8.37086E-19;
kBgSnwjJk = Math.Sqrt(3);
wQZUTSmFjMdEwXSzF = kBgSnwjJk;
Console.Write(wQZUTSmFjMdEwXSzF.ToString());
}uint XyFWOEWCi = 7864; 
short AiqmMTU = -10177; 
ushort bJKUyjkKXpmO = 7111; 
sbyte VeRWhjTbEmYekFQGVDeDmeJzqYER =  -118; 
double dioQOdxmihIfHAeZfhLskb = 6.398154E+15; 
dioQOdxmihIfHAeZfhLskb = Math.Ceiling(Math.Tan(1));
object oWwFYbZlYzJqEBbFE;
oWwFYbZlYzJqEBbFE = 8.371174E-37;short PJSIlupVBWwc = 27199; 
sbyte jLFWwyfYszStMbseMS =  49; 
uint pkJLSkszXpobQyu = 928789; 
double JLAjubFUhSJkkGfTAVkDmOAuiDq = 1720193; 
JLAjubFUhSJkkGfTAVkDmOAuiDq = Math.Exp(2);
int? jjUoxNtczHCo = 5235774;
jjUoxNtczHCo += 41592;float eNqsZXs = 0.009325026F; 
short cPMpQS = -10726; 
uint DLVExLn = 231312023; 
float ImVztGjMdfjfLUu = 3.273609E-34F; 
short zjXMHuROVoKBEFRyPUnXkJGwbt = 6136; 
byte bfjBtwoLCoeCRnyuGGBUy =  155; 
long uIQECKmPIfUjAajjCNmFjwtw = 65316390176583155; 
ushort fLFLiRboTMunU = 38336; 
ushort tLsgGZowwBm = 32610; 
long TyPeVAFRExEU = 73670338226169755; 
double dmPgwjoIFCFBSWzQK = 4.940245E+17; 
while(dmPgwjoIFCFBSWzQK == -2.384138E+32){
dmPgwjoIFCFBSWzQK = Math.Ceiling(Math.Cosh(5));
try{

}catch(Exception ex){

}
}string lSlfpGMHJIKWoCBQnDKtidiJkUAL = "XHKbDpeo"; 
ulong LnFwTMBFODmYjlJPh = 781363494442624; 
string VMkxCzaZeFIoXLpLhYZxysNhwMVgF = "iLSMGSEgL"; 
uint mEkBtGjf = 877524; 
ushort GCDZcbPETuSwXykeygbmAl = 13100; 
ulong IbfLbitLZmkdTBfWW = 16614216268092916; 
long SkltqNxuZJ = 60578671428275569; 
string HWqEUqdNffLGtUBBNT = "pxffmeAHmeFMweeIWUJzZKJoUY"; 

}public void AsYjkqfLh(){
uint dzwLQJBtoColFHeuUGOQloSa = 2802; 
float wpuWgfQjdoTa = 8.921713F; 
long jgtEgWletLEPGwwCOhXZh = 71838815550757884; 
ulong LGh = 83272498421453660; 
int DaEJTqlHDJEScU = 23831802; 
if(DaEJTqlHDJEScU == 344993){
DaEJTqlHDJEScU = 611891;
}long jzFmHyVzWSOIWRpJMwCwiWFXfa = 58070673533254816; 
ulong dGLPLd = 8795970516225392; 
float dmKmTIgGSWGAURTmakFfKNLejTTM = -1.257383E+14F; 
string CaTBZEmwzeWPznRmBLVMsPmqDGdZ = "wEiSJXTXtnyXjHFQWGWbGksO"; 
short NODXtBnHdCOWPqYphx = -2544; 
ulong hIBHZhX = 27449017106627610; 
ushort UXKDEdbZRFdpdV = 20670; 
int FfbquspzTpuxLg = 900345964; 
if(FfbquspzTpuxLg == 518746){
FfbquspzTpuxLg = 78377;
}int FAIiDTSgpdVJGateBF = 771; 
if(FAIiDTSgpdVJGateBF == 646056){
FAIiDTSgpdVJGateBF = FAIiDTSgpdVJGateBF + 550134;
}sbyte lwqsPBQbCDJkQxH =  56; 
string CbCSCIbWKCS = "RecdIxQXnwSEARwoOXyZMqbeX"; 
ushort JpTQoiJLEiNOjVcbgjXtLlNiNe = 56719; 
sbyte dXgoDgHwFEwQfRfb =  -86; 
byte KoUVZ =  252; 
byte EKcjoEkBlGAIOx =  159; 
uint CEFwHhFRPwkicgotLdM = 370452; 
sbyte VqUwJFpnJ =  -34; 
short wFbWDztBhZqizjo = -28438; 
short ZmwsJdNMVlzhDQNJTOJgicaFdWZ = 14134; 
double pmVOtccdWXjREgca = -32.737; 
while(pmVOtccdWXjREgca == 8.81004E-15){
double WmQnNebPxWkmPxLHQSLWsAT = 1.430955E-09;
pmVOtccdWXjREgca = Math.Round(WmQnNebPxWkmPxLHQSLWsAT ,1,MidpointRounding.ToEven);
Console.ReadLine();
}ushort jmGMJmhsFN = 64724; 
float ZJROpqILqStFW = 2.454709E-31F; 
sbyte IZxYGWmxtVdA =  -87; 
float PQBaSC = -2.125646E-29F; 
long TwmgMFbjlfazxuMEqW = 54133474085042088; 
sbyte pRESL =  -28; 
ulong apIpboXPfptgookhK = 66542940484568667; 
double uDhObIjEbofEWlhgzOaQaGxhFcb = -3.636734E+37; 
uDhObIjEbofEWlhgzOaQaGxhFcb = Math.Ceiling(Math.Sinh(-5));
try{
int RygTPfZiZHgEdmnADJViVxCMox = 1101441;
if(RygTPfZiZHgEdmnADJViVxCMox == 60363){
RygTPfZiZHgEdmnADJViVxCMox++;
}else{
RygTPfZiZHgEdmnADJViVxCMox--;
Console.Write(RygTPfZiZHgEdmnADJViVxCMox.ToString());
}
}catch(Exception ex){

}short TQwL = 28941; 
ushort wEsDwgsetzYaTDbVOjCdJOhqYnY = 50224; 

}public void twiQACWwFEnZOFJhFxCRUH(){
uint qlIxaEgiFtZdQj = 224; 
sbyte jGndViAjuRyWRMTN =  -4; 
byte BwbxewuGGsucBMuyVhmqAWRipL =  166; 
uint jcLhhFeiii = 51914368; 
int SaPXkyoYFlWuEESguNTqxoJVsg = 2775; 
if(SaPXkyoYFlWuEESguNTqxoJVsg == 587928){
SaPXkyoYFlWuEESguNTqxoJVsg = SaPXkyoYFlWuEESguNTqxoJVsg + 800862;
}double ZiCO = -3.333195E-34; 
double CpcPV = Math.Log(1000, 10);
ZiCO = CpcPV;
Console.ReadKey();long tVXYSRe = 53097136452835404; 
float MgjaYucE = -2.179952E+35F; 
double ShRXxWUwmYsFoFRpmbdqSxK = 0.001644729; 
if(ShRXxWUwmYsFoFRpmbdqSxK == -6.030342){
ShRXxWUwmYsFoFRpmbdqSxK = Math.Ceiling(Math.Cosh(5));
object BSDAyNVcAOnutw;
BSDAyNVcAOnutw = -1.867728E-21;
}int fkogYzBnDgBnCYCjGPBxtNCCYhK = 77597357; 
if(fkogYzBnDgBnCYCjGPBxtNCCYhK == 252664){
fkogYzBnDgBnCYCjGPBxtNCCYhK = fkogYzBnDgBnCYCjGPBxtNCCYhK + 805660;
}uint MHNAEyUDUyke = 7821; 
uint LHhDtJOiQOOA = 36390696; 
byte cGPDToXZAjHdNdx =  244; 
byte cGIWZcSEJPJwPC =  49; 
ulong IZYA = 46212320719181086; 
ulong QlspIQfHMKoH = 67692965397905608; 
ulong nWHWATQahiVxSiylDXWamnjPCSZNY = 79261637171108368; 
long lRMDgLMyVYT = 26858672667583993; 
ushort UccpNjSakaDRHKjBMdeQ = 26228; 
string eBtIQgXLSqKsOKltCTyLXqAw = "EkoJMElQnXPuC"; 
string npKBXsxRAKEXXtRObXnlVJbfNy = "ANxZZRDxIHYzchuQZ"; 
sbyte nyxiOxnQYFnFNBQWecK =  19; 
sbyte WjgcAkdCTcXgi =  -108; 
string JDMaFRtclXmloNoFAU = "mbMXcMyMkIHqMSBtg"; 
float QeQeRC = 1.470303E+26F; 
byte OsHoD =  205; 
uint GKngfiwFHBtOuhMtdyyXUxdUkHyo = 49005974; 
sbyte ZxCi =  86; 
long HKlVyzyMGGnkfsFcjGuWzMfGEsqPX = 69108649122749353; 
uint xNVRJMRdLwusDyNBVEG = 1168; 
sbyte OZcMRICquJaXiEUyUNnH =  -33; 
int beIXVJcjAo = 13; 
if(beIXVJcjAo == 991476){
beIXVJcjAo = 10274;
}ushort dZKDAEW = 952; 
sbyte FSBokMaIiKHJqzBJdydRUM =  90; 
uint EFzFdbUDqGI = 80201327; 

}public void eGsRMCwIkcRBAXoRSZcZHhDdfJN(){
sbyte TSU =  -58; 
uint lDUgWUVtxOECXmANJLQoXdnMdPpIs = 92; 
byte YdnXTgfMPfpDgQTCfiizmbhnichEn =  3; 
byte sDZilQQFKDiXYjWYxfmaNOMWV =  252; 
ulong YCyOTzjEQLpptEOKTkpZfihh = 7463677483693240; 
string HieweBZsiMQXIiZcdLKPKQupZHuGN = "ydntXGFzfPZOawqRFVFPZCD"; 
byte QOPwzgtGwoN =  169; 
byte uyFGUPWP =  249; 
double cAbYOVC = -2.768128E-33; 
if(cAbYOVC == 4.019557E+23){
cAbYOVC = Math.Ceiling(Math.Tanh(0.1));
int? lGjdDHCYliOjSu = 9833583;
lGjdDHCYliOjSu += 87300;
}int fbZRlboCYNsDgGIKSYxmG = 943794630; 
while(fbZRlboCYNsDgGIKSYxmG == 943794630){
fbZRlboCYNsDgGIKSYxmG += 849227;
}uint nusZAXBHLMjUmlebyFijWCbyteoeE = 544129; 
sbyte CKtZDIIgcgllssFSuOeJqdN =  -34; 
float uJxdfjagMCMbUXCcTuP = -5.093262E-38F; 
ulong mjRyBxzXZeWmZLZbcklHc = 49623638939367342; 
int GIYiToNBLmEMXYKCsTtNb = 10434665; 
if(GIYiToNBLmEMXYKCsTtNb == 833851){
GIYiToNBLmEMXYKCsTtNb += 950387;
}short hoFwwqGbCgMp = 12816; 
uint ztwVzGWpZRIRD = 441; 
string yhDYZqCHeBQN = "yqmMiNxbQNkYFFOBdLAe"; 
ushort DYwAcUouegDtuqERhoGpHusZu = 48636; 
ulong aQqJmG = 88292372818658644; 
long xljyW = 5231489953329172; 
ushort pghtn = 51890; 
float PDoBz = 8.94427E-33F; 
int LmEhDpjO = 1322; 
while(LmEhDpjO == 1322){
LmEhDpjO += 611540;
}long EuuFcuoDgwVIMeGtCaytRXORYFb = 62836367300217580; 
byte XhqXbbgwCbhCRJuindGHD =  157; 
uint HTkaYbhjwKES = 42; 
ulong HxyDTgSnaOsBoEgsJYphQgxBUXgh = 59424113404625955; 
sbyte ABaXajJHVRDkiifIgAZXJVaLhzM =  -3; 
byte FaiOzQgTflRnbpGUsTN =  154; 
int glmEApPDzjtpmCcw = 29461126; 
if(glmEApPDzjtpmCcw == 387494){
glmEApPDzjtpmCcw += 769476;
}sbyte ULfnfTMIceTXB =  103; 
short eEOHULcdRXiyepfsfSKPkBxFy = 15131; 
byte IfUPZKi =  208; 
double BiIcyhKoENYBRJkC = 259921; 
if(BiIcyhKoENYBRJkC == -4.206603E-24){
BiIcyhKoENYBRJkC = Math.Truncate(BiIcyhKoENYBRJkC);
Console.Write(BiIcyhKoENYBRJkC.ToString());
}
}public void uCsiFsYVBYcVTdGeRCmtPcsm(){
short PTddGUqUaWiKLJaa = -20171; 
short VXXKyPadGaqieU = -26097; 
int RysiE = 56749761; 
if(RysiE == 138680){
RysiE = RysiE + 817101;
}int PScStCRjHoFQ = 87680688; 
if(PScStCRjHoFQ == 75679){
PScStCRjHoFQ = PScStCRjHoFQ + 792008;
}int ViNXm = 9757; 
if(ViNXm == 539053){
ViNXm += 644285;
}byte mkzgmCENWkhNtSfu =  236; 
float kAyHffGbDVie = 8.44979E-09F; 
short IjHRnbqZXFjTATB = -26798; 
long ZaPFtaIBbSN = 31515613325714660; 
string jhzKB = "MCJuduswpJdRGjOaAlQdRQfxqPKk"; 
ulong qVSeVYsyKNiXNKttKRqiJmTyf = 4048461592070055; 
uint iYCnPIcDcUTd = 49620; 
double TUHieb = -2.532109E+35; 
if(TUHieb == 4.267089E-32){
TUHieb = Math.Pow(2, 2.1);
object LQDmRZkfiRmYomgEEmlD;
LQDmRZkfiRmYomgEEmlD = 54.69225;
}ushort eeREooYWERKMcWtEjBUTMTVLReJIx = 58423; 
ulong gQYN = 26432890231203683; 
sbyte gHC =  45; 
string WBnligNkGqAJcHFlXGSQAQ = "ecBDRXcgNScgHDMEEIQyQg"; 
sbyte CwUJVQlkfPij =  38; 
int mEjNRaBYKlboiKtWYViawctkWGR = 83; 
while(mEjNRaBYKlboiKtWYViawctkWGR == 83){
mEjNRaBYKlboiKtWYViawctkWGR = 988247;
}ulong TGupqbyjyTCdCMSko = 6532943810262627; 
sbyte gqJfoxgBpEFSZYT =  103; 
short CppZinywmsIGF = -26632; 
sbyte qTPOmIukCHKHECqpJ =  -24; 
long ighwJTiPSKZAJyHZgEhUWeyCER = 83181759532083850; 
ushort LtetAGsS = 42884; 
byte gUsjemQWdKGRRcyOBNQescynNzPu =  81; 
ulong UtTKSJiTWGuqAeDmqcR = 88698635021143801; 
ushort zwGqeMyYsGfsUUAYMXi = 13230; 
byte sAUBbnmGuFMlykO =  251; 
ushort enYyLAkYHItB = 58959; 
byte acgyELjMMkBHIx =  71; 
byte CQfWAVOupdHGmUKARyhIDRI =  231; 
int lkTECsyHY = 13; 
while(lkTECsyHY == 13){
lkTECsyHY = 357483;
}string IDgApRgQZg = "kLhjumJECMCohHWGts"; 
double oAD = -5.704744E-11; 
while(oAD == 3.20822E+32){
double kUxnWWeozPBCfmYTuk = Math.Log(1000, 10);
oAD = kUxnWWeozPBCfmYTuk;

}
}
} public class AYbUfyTDiZozeGLgO{public void gPMGM(){
int jKJLDp = 44; 
if(jKJLDp == 737882){
jKJLDp = jKJLDp + 120545;
}byte AGjbQKCYJADUtgSQUlzPWO =  52; 
float JfbBRYaCtHiCFWSFkbi = -8.857176E+10F; 
double FOUA = 0.002268333; 
FOUA = Math.Pow(5, 2);
Console.ReadKey();int LVcip = 601814; 
if(LVcip == 129004){
LVcip += 504790;
}ushort YCtaL = 51775; 
uint oqoqOAUtxNwLacoH = 965510; 
uint aqzKIifzbLMFdFwENSCXLDKteIeW = 7404; 
int oCAXTGKFtwsMbQIwHLTGxD = 4653; 
if(oCAXTGKFtwsMbQIwHLTGxD == 595244){
oCAXTGKFtwsMbQIwHLTGxD += 992707;
}string mohZpCez = "hhWEpjYsnaJoBTWpMobpP"; 
string BjidEcBfqiaMzBZWnJMqZ = "VMwlKFmdhVgzw"; 
ushort gupOUs = 7384; 
uint zQNsNH = 67; 
long oyIst = 89188737151278310; 
int hoxEecPlnNTSUqBTyNpeKzbW = 23610379; 
while(hoxEecPlnNTSUqBTyNpeKzbW == 23610379){
hoxEecPlnNTSUqBTyNpeKzbW += 559701;
}uint hDGsGgHcBluFAdYWBlgmOB = 66; 
byte LdXKRLohKsLimqgm =  150; 
uint yyOcj = 158921; 
ushort XHxXffwD = 7542; 
ulong KpqtGksZQgCVI = 45771412646266524; 
short kdVO = -28676; 
sbyte EgPduhfwxYmiHexMPsoczDsKMwl =  -98; 
short ynRUDugEjJWCEPTAIYPqg = -5301; 
ulong ybPeq = 78654637714401085; 
ulong bFkiHfXYDRmNnqYFb = 43837328502176092; 
ulong xSnVgVwNNVmgZAclndmIJfhByGob = 16652036037634941; 
short fRSElJOuNXyuNiXfopKOyCGuqiX = -14043; 
byte YWWsYwQeUAMcTsEcQPaPUkyCy =  149; 
byte mcMAhWZCyukCpkx =  52; 
string InQothnyWNAwV = "ksRRNmUXscFwE"; 
uint KPlBcWWIYKEkLZm = 4; 
uint qXtGyOBLWOKXEPYmYmVSynLTg = 5; 
byte faDWMhBpjONCkaTnNZxMMnPgHudm =  127; 
uint LqOROgaSHy = 784428482; 
uint plmRFui = 46769068; 

}public void RlxosenQ(){
long bkyjfhMhPnPApYqi = 83640620007858304; 
double LMADjgdZgePdMyIaQ = -5.662278E-07; 
while(LMADjgdZgePdMyIaQ == 0.0001018405){
double VHtVtUjEyaJmdIxpRltMKyyfoI = 1.555393E-24;
LMADjgdZgePdMyIaQ = Math.Floor(VHtVtUjEyaJmdIxpRltMKyyfoI);
for( ; ;) {
Console.WriteLine(-1.604906E+12);
}
}uint yjnHLNC = 183412001; 
byte YNxZKGAASCyQ =  194; 
long tNbIaAMhPmtFbgskR = 85401959779297868; 
byte LIZtAWjuPWC =  66; 
float FHMqbztAp = 0.0002489206F; 
ulong hbQgI = 76003003718276378; 
float EooffuPLkEXQXkstVLRctY = 9.646274E+28F; 
uint jeRRztHHVbqsJLspFDLXl = 157874798; 
ushort hRPuFAwFxjLUueFiqpqbDcgKcumiE = 30455; 
ushort uRqDSmpCWOmBIxkzgOUBRwyeLeqk = 15432; 
byte XQO =  129; 
int bqeZPeOQGcHiPcQETeYumSN = 104378513; 
while(bqeZPeOQGcHiPcQETeYumSN == 104378513){
bqeZPeOQGcHiPcQETeYumSN = bqeZPeOQGcHiPcQETeYumSN + 284106;
}long VILexQBcqJnebLbyjjuXJDiBjE = 25480630078781206; 
long dYtNscILYNLkdSBFU = 19736990456367291; 
int WJNjTZBukQHFHVyDxT = 820276796; 
if(WJNjTZBukQHFHVyDxT == 416342){
WJNjTZBukQHFHVyDxT = WJNjTZBukQHFHVyDxT + 634001;
}sbyte sbTcCfLzMbkESkDyoOmhEzcIzZJQW =  -120; 
byte bUHABi =  72; 
uint hjwqSaNniFIuawUZkFCxVCKOeD = 580991549; 
string gAMnwoHpHZWCOxGMaANBpFNeYGAH = "pbiklzhIM"; 
double sVYDBtOmisCARDHDMHo = 2.20026E+22; 
double YyeoAihuHjFGSmzUOD = 2.633445E+20;
sVYDBtOmisCARDHDMHo = Math.Ceiling(YyeoAihuHjFGSmzUOD);
for( ; ;) {
Console.WriteLine(-7.016344E+11);
}ulong YFSDFYNHBDJmsZTYNm = 27010652445652796; 
byte jwLpHJXXkgsEODbSNwEdh =  27; 
sbyte fXV =  96; 
ushort jnNdxfRtGKbWNGUKc = 6745; 
string EYNhBepKkJOd = "KXdQLgWatPUblsAgwV"; 
ulong aXkCGDqPhiXWYsdIQTsYhtji = 59190252715167155; 
double EMMDkhPgSk = 4.646744E+35; 
if(EMMDkhPgSk == 8.569035E-17){
EMMDkhPgSk = Math.Ceiling(Math.Sinh(-5));
Console.Write(EMMDkhPgSk.ToString());
}long ufX = 17793103017954581; 
ulong AThZshZReTumwYqYUmhIUHoB = 36970933663196908; 
double kZciV = 6.151213E+12; 
if(kZciV == -2.264033E-20){
kZciV = Math.Ceiling(Math.Tan(1));
try{
int EuQPlVDSisxwnNS = 5749289;
if(EuQPlVDSisxwnNS == 44217){
EuQPlVDSisxwnNS++;
}else{
EuQPlVDSisxwnNS--;
Console.Write(EuQPlVDSisxwnNS.ToString());
}
}catch(Exception ex){

}
}short UiIyiwVByGHyViUEdn = -18882; 
sbyte juwN =  119; 
float LPiwjlnZLVhNOlEVBInqIguplkC = 3.803927E+21F; 

}public void LjEaQIxln(){
long USifysQHzUeKBmEwTiYKEkzkXjN = 28433097433736932; 
short jOTMdqsGA = -29278; 
ulong mDcpaqJuX = 19429971765415314; 
sbyte AoiJu =  -102; 
double UgFzgZiIWldFeeUn = -14.92426; 
if(UgFzgZiIWldFeeUn == -1.891057E-11){
UgFzgZiIWldFeeUn = Math.Truncate(UgFzgZiIWldFeeUn);

}float npHVquKgppsmSYTEj = -1.214461E+21F; 
ushort KmTIGXTaAZQNZ = 47720; 
sbyte YpAgBRlbylkSHtQPBhEdNnL =  -18; 
ulong RqCATl = 19847531655524228; 
string juNbtfibmRhbaLFdpuZVAYsYe = "hTBmmKcVdqKPNMhBlbeMQCslW"; 
uint fslbOWDXpCaFzZeHmtBInOZMpU = 155; 
int opNRXDSoVSkORYly = 75123038; 
if(opNRXDSoVSkORYly == 95725){
opNRXDSoVSkORYly = opNRXDSoVSkORYly + 844128;
}ushort FssAMUFCpPmKRHbDPYzJyBWbBT = 17621; 
int NwxUUlGjsEa = 6798; 
while(NwxUUlGjsEa == 6798){
NwxUUlGjsEa += 538956;
}int MifgtNwewqZSHZlEtQPXwxsxwyuOb = 307546686; 
if(MifgtNwewqZSHZlEtQPXwxsxwyuOb == 286296){
MifgtNwewqZSHZlEtQPXwxsxwyuOb += 880306;
}sbyte odhAgjTsLuoyUJVNJEoR =  44; 
long tIcCbbFqffWeNIynlAeKdfphlwk = 41526551936557486; 
float IRpfc = -2.307999E-12F; 
double hfkpdsFbSmsVpHQaHC = 6.690143E+36; 
while(hfkpdsFbSmsVpHQaHC == 5.025762E+26){
hfkpdsFbSmsVpHQaHC = Math.Ceiling(Math.Atan(-5));
Console.ReadKey();
}float zkJJsgmWxemmEZmdXAfJSSWY = 2.404338E-32F; 
float gcEDFzZtFOTxyWtkOZYgkggXF = -6.896383E+33F; 
int KcmowPtCLJ = 98; 
if(KcmowPtCLJ == 191047){
KcmowPtCLJ = 622193;
}long VAMjHgMGa = 38262996869225063; 
long hbhZJdqTgsIXqVXbhDB = 47355716516357965; 
byte IOEzaCjOAcgpmFnsYn =  10; 
uint kDupAFlsCOpeoAEe = 329479; 
ushort nIBufTYCyAHLNdxkjhBXYwY = 18476; 
ulong YfKnAXIM = 87358985297942322; 
double gFs = -2.624596E-35; 
while(gFs == 1.789536E-13){
double KlEUVSRiaKdaSUijWVqmAXhlTM = 0.0001519865;
gFs = KlEUVSRiaKdaSUijWVqmAXhlTM / 3;
try{

}catch(Exception ex){

}
}float TdzbsPGjEtOWLLCsV = -2.439214E+13F; 
ulong dwafNdpCZ = 42339634049959826; 
float QLEiFRtIzRTqgfqPIyVFMoJEZRO = 7.402216E-36F; 
byte OpOXBBmwouwRKf =  11; 
ulong ETGz = 34078613096199410; 
string GkwebtiQoucHkpyYzX = "LliI"; 

}public void DdExzBeIlRQHoabwTOEkwGcUdeBlL(){
short xDyDXPNsOpu = -7007; 
ushort RJzWHRzGxDXbKsua = 56183; 
double BtdSzdGgwQUakJumwcodBSRgUF = 2.706435E-30; 
while(BtdSzdGgwQUakJumwcodBSRgUF == 1.367939E+20){
BtdSzdGgwQUakJumwcodBSRgUF = Math.Ceiling(Math.Sin(2));
Console.ReadLine();
}sbyte BXGNeYcEcHtVgQDSBsGbEL =  -6; 
uint CYCDKJNVL = 851902; 
byte BtPZNIHUPP =  152; 
ushort zleh = 52597; 
uint BDaBat = 625939352; 
string mLIlSlmEXRRsN = "XiHkBQE"; 
ulong cPXsnYuKZTtDU = 39436661826799489; 
short QSESMN = -25130; 
byte AqGyVUZGDhSAtGcyQEBYZUMNH =  34; 
uint gJWTKCcEftMfSYpAyJxlC = 9186; 
sbyte lSFFSQOtpzUkYQSEAsx =  88; 
ushort bFmoY = 46418; 
byte NNOnyJz =  247; 
byte YMDAwoASFJcczqkfR =  254; 
short ScfhMgyKsifBeUUIDC = -20798; 
double fgFfLnxcqRzHKLVHwNnSPBwE = 2.514584E+34; 
while(fgFfLnxcqRzHKLVHwNnSPBwE == -7441.822){
double OMWBcbTaIaqHUu = 5.439036E-17;
fgFfLnxcqRzHKLVHwNnSPBwE = OMWBcbTaIaqHUu * 2;

}uint oOgkAzojahlPZJzkaVOdBnSfIeV = 26; 
uint tbNXhSOGUxkQanoXZekzGHywaWaE = 553387; 
sbyte AjTOmLqsloUHozRSshWecdsMEqXk =  86; 
byte spnqQuBL =  68; 
ulong lqNRfDeikmGbUxShjVOgBcdDqa = 74086674507994940; 
float YNl = -2.479823E-30F; 
long YMnuVYmqPAHtimkAewdLtQXLLFRiy = 15009556872560880; 
int ckCIiLQIP = 25323555; 
while(ckCIiLQIP == 25323555){
ckCIiLQIP = ckCIiLQIP + 974409;
}int OEJqhd = 109103207; 
while(OEJqhd == 109103207){
OEJqhd = 113834;
}sbyte yzjbASiOZUWcw =  -80; 
int XSyYcAXVVhICc = 27; 
if(XSyYcAXVVhICc == 386401){
XSyYcAXVVhICc = 803886;
}long bAOYZEPufJKDMJKIhqYBogsj = 31303864067421538; 
float WtKZcViBQzXZpwEUBc = 4.350144E-27F; 
uint gEsnIhaLIpJhMqYhncsPqTmIwBc = 876656; 
double MdblfIOnaAeRIpiJm = 2.391521E-11; 
double LThfBRKNWdLeT = 238216.7;
MdblfIOnaAeRIpiJm = Math.Round(LThfBRKNWdLeT ,1,MidpointRounding.ToEven);
object olcFqlRQFBjOV;
olcFqlRQFBjOV = 0.0002644514;
Console.WriteLine(olcFqlRQFBjOV.ToString().ToLower());string oZAxqkNtIqpWqBZbMlgdDFKxpZ = "KXhSICIVOxVgObEnPFcWGVssNaYp"; 

}public void pdgsncxAzIuJGkFRftbZq(){
float ncpPFPfHEskzsPPAZhOpXInaK = 267.1615F; 
int WpswmSoYnRKtaRpuNlbq = 50687162; 
if(WpswmSoYnRKtaRpuNlbq == 587839){
WpswmSoYnRKtaRpuNlbq = 872294;
}int peRnzYHbTcfXACLpdWWQXGXC = 307093; 
if(peRnzYHbTcfXACLpdWWQXGXC == 471931){
peRnzYHbTcfXACLpdWWQXGXC = peRnzYHbTcfXACLpdWWQXGXC + 785515;
}float kBJiaxUbkoCLg = -1.274779E+26F; 
short DAIWRSKhpOij = 15600; 
int lNMYS = 302759; 
if(lNMYS == 902341){
lNMYS += 594258;
}long lwwzx = 72154812086533190; 
string JMRzMyzttTBPlSGbliGVV = "LOjHPcpqpTBG"; 
double dfWHObcGWNK = -15615.01; 
while(dfWHObcGWNK == -0.08328658){
double bdofJxdbDHoHU = -2.954398E-30;
dfWHObcGWNK = bdofJxdbDHoHU / 3;

}double coz = -7180.95; 
if(coz == -2.5776E-35){
double bWlSipXiTMhVQmaYnwZQWncCYTP = -4.930523E-07;
bWlSipXiTMhVQmaYnwZQWncCYTP = Math.Sqrt(3);
coz = bWlSipXiTMhVQmaYnwZQWncCYTP;

}long wHBYaqNHnVSzc = 1087612381351247; 
string HFlzdfLSIip = "JaOKxuykaQjikujP"; 
string DqqpTuPSJCDcjpNGLgUwYP = "TpQucnKVVQWHZisgtUF"; 
long IYGUxGJcVINNLyacaMFZGImnJOys = 43468977601274362; 
byte HIaSahjTnLsetiZM =  83; 
string slJiqdugsJblJSCeWYNHee = "BMZVhsusskGSnYAKPGbRklAK"; 
byte ddOFtHnxiysimPYpKacangynX =  7; 
string FnlpJpnqF = "SYFRQbsHslTUDzHlEWGAHsE"; 
string UkbAWBHMVidwxqwWqJQtLMOJy = "qMfhAuEaWRphxMliHoDQhG"; 
byte oJNIfiytbjiVHOJKS =  104; 
ushort HjmegDezfIFMCDSRHWU = 49798; 
ushort flDqDTHTXMkFVfxJBEdZmuGytdVS = 2830; 
double IlKdbCQakRmKSBXI = 1.369067E-08; 
IlKdbCQakRmKSBXI = Math.Ceiling(Math.Tanh(0.1));
Console.Write(IlKdbCQakRmKSBXI.ToString());int gbS = 50; 
while(gbS == 50){
gbS += 410558;
}sbyte qoDnpzzNJgFpckXagUM =  -70; 
sbyte aMgKc =  -60; 
float NHBVUxxxCEgsWVDMP = 4950.439F; 
ushort UJUzm = 18193; 
ulong HwkTgt = 68659345398677479; 
sbyte Qifjw =  98; 
byte xxTXGyHhHoUhWzPCFWIAbS =  101; 
uint PJdiGMNiAxLBZiKK = 9634; 
ushort aiqtFGAmOKFbpyR = 51725; 
int aEhZybdaGNXPIUZeAGqLFalBuPSKj = 353235; 
if(aEhZybdaGNXPIUZeAGqLFalBuPSKj == 383078){
aEhZybdaGNXPIUZeAGqLFalBuPSKj += 958097;
}float bMopEZMoYbK = -3.118392E-21F; 

}
} public class kzSuZqIFIjuQducJnWhPyUbuaaAk{public void NhIYEhynZYBIye(){
sbyte OWsloOOUyBqxzuYRxhVbMSAmGwo =  118; 
int aQVzme = 97; 
while(aQVzme == 97){
aQVzme = aQVzme + 755370;
}uint RaPFZ = 22947780; 
short djVxeHT = -1419; 
ulong qBCBZgUKyb = 49410774869071515; 
long XULHRnqJcsmBEAjEpAQzfZOnmxkFa = 101892929864248; 
float LQkHfOxeAhSgeQlTYfxtw = -4.070284E+18F; 
long paLNjNPwReJUt = 33447715967691355; 
double EXqqsBmIdDlPOLq = -3.152665E-09; 
if(EXqqsBmIdDlPOLq == 6.091939E-18){
double xWDdpoAStDhPSxxUEyG = 27518.33;
xWDdpoAStDhPSxxUEyG = Math.Sqrt(3);
EXqqsBmIdDlPOLq = xWDdpoAStDhPSxxUEyG;
try{
int faycfRIMaYMUwYjqOduwkED = 264928;
if(faycfRIMaYMUwYjqOduwkED == 71937){
faycfRIMaYMUwYjqOduwkED++;
}else{
faycfRIMaYMUwYjqOduwkED--;
Console.Write(faycfRIMaYMUwYjqOduwkED.ToString());
}
}catch(Exception ex){

}
}float QfCaWt = -2.979723E+25F; 
byte ACDyaezZXzcqnskCfzCKtK =  99; 
ushort qxGUqhguMcJTGSgVbOsfAS = 12290; 
ulong nIoEtPhhMKJTe = 63404604597605750; 
uint uPqVcplzlQBdARQsmCM = 153850298; 
uint Wiaj = 301148; 
float MObUPcUeR = -3.203379E-24F; 
ushort FDRd = 7277; 
byte aXGz =  236; 
ulong NbfJisDOozWbp = 19793667273305216; 
int fyfSlPiE = 47980759; 
if(fyfSlPiE == 65275){
fyfSlPiE += 726269;
}float dAQm = -2.149203E-28F; 
string JFckCUNcgSZkdAqZgUYZJlthDU = "ACtRYtmiIMXceoFalMHZFiQgzjfky"; 
byte IHVmBObbRRpJniRTKdkhxWVuqLl =  87; 
ulong bMyRowibem = 39359081596826589; 
long wgldzMMCWPC = 53445467371419708; 
ushort IZYXMhNfQinLzSUBttEKH = 6778; 
short CzltJogekGOhmgNEYxgHnY = 17716; 
long eScoMdDLmwnVtyCbiASeNmmHZL = 58355019142760426; 
int dQzDlxaLAJGZFF = 49; 
if(dQzDlxaLAJGZFF == 129454){
dQzDlxaLAJGZFF = 841700;
}int oaSEcPTEuUSeYs = 29; 
if(oaSEcPTEuUSeYs == 233876){
oaSEcPTEuUSeYs += 545701;
}sbyte flucscOVtNR =  34; 
uint LJukRVnLgAHNAHO = 607825465; 
string XVHiayQPAC = "mqRDF"; 
byte bLlcueVwGEhgDhPsEDeBtOafPjbl =  78; 
double bETFqcVBCnFSmIsYXzYnZBKDnLfn = -9.335237E-34; 
while(bETFqcVBCnFSmIsYXzYnZBKDnLfn == -8.808397E+14){
bETFqcVBCnFSmIsYXzYnZBKDnLfn = Math.Ceiling(Math.Sinh(-5));
int[] pyGzhGCPowbJHXXyOsWIR = { 9836193 , 70390 } ;
Random ZVOdKEJtlQtaMfHpOhWIWJMIaZFxD= new Random();
Console.WriteLine(pyGzhGCPowbJHXXyOsWIR[ZVOdKEJtlQtaMfHpOhWIWJMIaZFxD.Next(0,2)]);
}
}public void QsyKLeVYgW(){
sbyte pGlRQzEO =  52; 
sbyte qigc =  53; 
double QmfdjuIWG = 4.079528E-20; 
double NOGewWFHpgCEbjZ = -0.031532;
QmfdjuIWG = Math.Ceiling(NOGewWFHpgCEbjZ);
try{
int mwSjTSU = 281213;
if(mwSjTSU == 65563){
mwSjTSU++;
}else{
mwSjTSU--;
Console.Write(mwSjTSU.ToString());
}
}catch(Exception ex){

}string HTEweZVEMlCwSYmpTPumOnueWOSOJ = "RlzNbimmKIcHTBo"; 
ulong ERkClnQlsAAzpghVnwp = 37337247876814813; 
byte wUPihn =  239; 
ushort FXICKKNOuCUMGfZRCINhVgDGeXGL = 19851; 
sbyte kzqQsnEmuoSLjpwPPQqHHjobyw =  -116; 
ushort dczlJKnnoHEXusScxspKhRU = 63764; 
uint aipsXlgA = 2429; 
ushort CHgCAiOJlSyYbcNN = 39378; 
byte BKyJfYW =  173; 
short UbGKWRKDHqMkTYhLsqUAuUdeT = 19879; 
string bEEAxSqRRoelcjYWxoQzBuHGPSwa = "CQgNMBdOlOaHqUclIxkVgIEZFxIX"; 
double FxqTpzXApJVhjCQRjAEtg = -8.616803E-24; 
if(FxqTpzXApJVhjCQRjAEtg == 1.275032E+28){
FxqTpzXApJVhjCQRjAEtg = Math.Pow(5, 2);

}ushort FSEikbcRXYecgKpU = 31313; 
short IHhkVTkFzbiCzsMtiOUkfoH = 25445; 
sbyte iBgudNkzNTyIIpZyEQOy =  33; 
long phfFfJsZmenKL = 88280994647325923; 
double gEkIMQfJaAfQlXheP = -6.063191E+28; 
if(gEkIMQfJaAfQlXheP == 4.138471E-11){
double StgHeCuHZaUbHihx = 1.216359E-17;
StgHeCuHZaUbHihx = Math.Sqrt(3);
gEkIMQfJaAfQlXheP = StgHeCuHZaUbHihx;
for( ; ;) {
Console.WriteLine(-1.010958E-22);
}
}ushort DJIlcYutkuUUQaknbfaywmISHE = 14988; 
short fHGXCbNWMeltwBJllFK = -22507; 
short ZVYzpbQGzNeNeOnWoYyqpbPd = -31929; 
ulong HuZCgnpIKZcMMggGAoIwVXpoMKbBY = 56180015981558225; 
byte APfEWlmMjiwoPsqeKeKQxlPJEeGiK =  246; 
double npiHPt = -9.677358E-35; 
while(npiHPt == 2.294603E+27){
double elXAfPw = -20928.69;
npiHPt = elXAfPw * 2;
int[] aRGdcklnaAjKopLH = { 7684778 , 89788 } ;
Random ztyfMZCXMzQqT= new Random();
Console.WriteLine(aRGdcklnaAjKopLH[ztyfMZCXMzQqT.Next(0,2)]);
}long sCyuVIfoRGUC = 24375893401103721; 
short SegqMwXgLo = -29508; 
double ijWDMsRUuHxdpwiQKZgadKsnFVKq = 3.944571E+30; 
while(ijWDMsRUuHxdpwiQKZgadKsnFVKq == -2.417584E+14){
double CiiAuzzL = -2.34077E-22;
CiiAuzzL = Math.Sqrt(3);
ijWDMsRUuHxdpwiQKZgadKsnFVKq = CiiAuzzL;

}long zYwjmYfGsZWSFMwGMzekGzyCBm = 72411560137847068; 
float siAhsXZBdquKsoHszy = -8.417642E+37F; 
uint JaCohmfWhEVRmEYWSIzXzjTNzi = 538526; 
uint ORczBAHdP = 40214993; 
sbyte QPBbdtDG =  24; 
uint wAgORYNEwUQdPPdAoP = 717484554; 

}public void qZcXYhxiYIipzcasS(){
byte aRMpaLMmZTmQMQbeBdQSRqux =  64; 
int pNchxDcLLGKcmlVxpLRxBxX = 271334; 
while(pNchxDcLLGKcmlVxpLRxBxX == 271334){
pNchxDcLLGKcmlVxpLRxBxX = 670722;
}ushort ctWxAXLRnKe = 21539; 
uint aEHWVeLxjnTPCTuidcnpHKpQw = 592968; 
float NwfzKmzNcOsFGiJDLw = 1.036399E-05F; 
sbyte ccijqnQBxARTxSNpRHV =  -24; 
string VLaBHmKdNy = "gtcoBWkXCqTONMR"; 
ulong EkBKGOhziQHDRt = 45326902467373794; 
long yzXcXnOQzoACXOCGVRsfFw = 61371191449262800; 
ushort BQlhQeCUb = 54168; 
string TjxIOTPiYFgcNYWDW = "ytUlbgCuPRpy"; 
string pieWYKJAolnWKiuShYYKuLJ = "iofudYTCCtMFUaEmJXdJ"; 
double ysaXJ = -0.01056147; 
while(ysaXJ == -1.722739E+07){
double EnxqsMwpyPXhwMbyHZJdRO = Math.Log(1000, 10);
ysaXJ = EnxqsMwpyPXhwMbyHZJdRO;
try{
Console.WriteLine(ysaXJ.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}float hPHoPdi = 4.386443E+11F; 
int SAl = 49472516; 
if(SAl == 814380){
SAl = 611814;
}uint HtKMadQqohYNhmj = 4630; 
byte eSpCkg =  176; 
sbyte UXUlGJhm =  25; 
string fPqp = "UNdzoiYDAyGpSAGpVGxqnMgzeX"; 
ulong jaoCaJqtGKUMY = 16440317629983120; 
ulong iVYFhGFu = 20770836012312982; 
byte lLkMKsX =  251; 
double mDJWyQgHbejOap = 3.514295E-13; 
while(mDJWyQgHbejOap == 3.247005E-18){
mDJWyQgHbejOap = Math.Ceiling(Math.Sin(2));
object IUJbyneYzKUisXZLZxLsLKw;
IUJbyneYzKUisXZLZxLsLKw = -2.521465E-28;
Console.WriteLine(IUJbyneYzKUisXZLZxLsLKw.ToString().ToLower());
}ulong xaXATtbRHXqSMw = 52190552619399358; 
long dKuDOQEzdMqGQFMFAxzbyAuTgyM = 66080855884292729; 
sbyte AcRuSPZefuquywaNKzEpCwqdmSkMf =  -49; 
ulong EGatIIF = 18386417334855290; 
ushort yFESSydMNgaFmYQGhRRwHfcUTVNun = 43612; 
short NNJLkpLiXIUBEfhPlymgs = -29356; 
ushort DKZWKJzeRsRB = 20264; 
string RfJyZlCpNOYiQ = "ipKtqiCqOAniPJ"; 
byte HAgdwaKFtf =  3; 
ushort iMAJJIbgg = 64179; 
double BsjjQxSdzhzbMaiHfZkhheGNOB = -5.665075E-37; 
if(BsjjQxSdzhzbMaiHfZkhheGNOB == -0.7168173){
double IBwEKhpOamzWJAubgOOLxNxwpoR = 1.612224E-24;
BsjjQxSdzhzbMaiHfZkhheGNOB = Math.Round(IBwEKhpOamzWJAubgOOLxNxwpoR);
int? RCfBgKxBZSpFBpEyDRftNxBAVeamu = 8555589;
RCfBgKxBZSpFBpEyDRftNxBAVeamu += 10433;
}sbyte fNSobknluaHFbKjgJuTkkJhUEDO =  42; 

}public void IItNiVBDcxledmwX(){
ushort IBgfzlXLx = 38464; 
double zyGMMsmRTPJEHXHfhwNXkp = -1.001256E-26; 
while(zyGMMsmRTPJEHXHfhwNXkp == 9.833323E-26){
zyGMMsmRTPJEHXHfhwNXkp = Math.Ceiling(Math.Cosh(5));
try{
int bRaBhqytkifbwpNYoZ = 4981080;
if(bRaBhqytkifbwpNYoZ == 94401){
bRaBhqytkifbwpNYoZ++;
}else{
bRaBhqytkifbwpNYoZ--;
}
}catch(Exception ex){

}
}int FkCMl = 13437573; 
while(FkCMl == 13437573){
FkCMl = FkCMl + 204202;
}sbyte DiBEdB =  96; 
long zjxESFQJiaKml = 52242507941282338; 
double sWwgubxxJwjNqpITDse = 2.194812E-24; 
while(sWwgubxxJwjNqpITDse == 4.289967E-14){
double qEMFzFeIyywIN = Math.Log(1000, 10);
sWwgubxxJwjNqpITDse = qEMFzFeIyywIN;
try{
int ePBepN = 942068;
if(ePBepN == 93724){
ePBepN++;
}else{
ePBepN--;
}
}catch(Exception ex){

}
}ushort GtcnnEmmaQhThjgGsJutIzWhyQd = 18167; 
string JInwPEaQCyaReoCMgDeucxDdnhy = "RyBYifhAGNwEGDhWtRtIHMKBjK"; 
float ejhHgfZdX = -2.011845E+12F; 
double cgtgBP = 1.727992E-19; 
while(cgtgBP == 1.297297E-05){
double NZPAnVu = 4.128497E+22;
cgtgBP = Math.Round(NZPAnVu ,1,MidpointRounding.ToEven);
object ADdYpiuCFGquJUqY;
ADdYpiuCFGquJUqY = -1.103735E+07;
Console.WriteLine(ADdYpiuCFGquJUqY.ToString().ToLower());
}double IununKAx = 6.358747E-31; 
while(IununKAx == 1.836887E+34){
IununKAx = Math.Truncate(IununKAx);
try{
Console.WriteLine(IununKAx.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}byte hNewQaPHDkeIVwbVjbFYxSR =  233; 
int lZUfASUfpJYdyS = 281433114; 
if(lZUfASUfpJYdyS == 962058){
lZUfASUfpJYdyS = lZUfASUfpJYdyS + 558692;
}byte DLMcMlqpW =  45; 
float PPNxLlNPfsXfF = 1.5477E-30F; 
sbyte gXZkYyitxgJOaMPjdcdCyYMQygjO =  30; 
uint fjsPlUiQsEdofeafHiVBKcGBiljNS = 4016; 
ulong gDFJXEpVmMYtKHLxbEGLYodFRHaK = 20983409404600755; 
ushort JGGGyqHgAFzpQJP = 2434; 
double AaRpDHAnGPbsJiaEUZesSNXKg = -7.60733E-23; 
if(AaRpDHAnGPbsJiaEUZesSNXKg == -5.244531E-06){
double bxHtcZgj = -1.132981E-16;
AaRpDHAnGPbsJiaEUZesSNXKg = Math.Ceiling(bxHtcZgj);

}short gotAjUFNdBwsguhLnmSRw = -11278; 
long CjJzEjCSupwSkzscCAFBKmTeOeIw = 21187683961896568; 
byte iTYEY =  206; 
double AQUPoWMklop = 6.080732E+15; 
while(AQUPoWMklop == 5.112167E-30){
AQUPoWMklop = Math.Pow(double.MinValue, double.MaxValue);
try{

}catch(Exception ex){

}
}int FICWERJj = 62; 
while(FICWERJj == 62){
FICWERJj = FICWERJj + 134539;
}byte yTCYYuNcLfJxhhPGlmDhT =  18; 
sbyte UORZHYwTYPPajIkk =  98; 
uint yhlbURPyx = 127067; 
float LbdkEOxMKPzss = 2.654473E-07F; 
int smwS = 33672503; 
if(smwS == 927302){
smwS = smwS + 153597;
}float ZoSYwohEaHfbAOsO = 1503.742F; 
string UumTwDbLwzHG = "lKBGtXAmabLhtsWZVcu"; 
sbyte HGcYDTnyiDZAxnd =  88; 
sbyte GEfTsCMbjeM =  1; 
ushort CgFezHQdk = 52645; 

}public void ALIXeSgNNOCsNnBmCJiMIgMYp(){
long uCACtpcSReefmFNJAZOSRXuowbdit = 1362663605691788; 
byte cLwKixE =  236; 
sbyte zmZCRhxhjfqlTzSwyLuSCwlRMgw =  -59; 
byte SfWnUMKfFLpedgRQRWRNZTDoud =  7; 
sbyte KNhWqnlpFFgMqgJCxqeaVpzx =  -10; 
float DHURKCpUYhiNSFAlsLbJNTNXNWhUu = -3.215413E+16F; 
uint pJJLNxjalLtnFlw = 63093; 
sbyte yjlKUWeNuxBKxLoDWRCGoR =  42; 
uint tKiKMzq = 1; 
string dZPPJJPEM = "WEo"; 
long FDBxGCMCBmWdIYiC = 82651735044657282; 
int ePg = 115104; 
if(ePg == 247248){
ePg += 877763;
}ulong EwMsyoCNGhGDBATmjDDQ = 43348583527182845; 
string weuT = "ophANkjaUAlHXlHRKqZdNQowbk"; 
ushort EMBhHHFWccPFuQKXKPgVx = 14637; 
long wQfzbNzDQAyhYeZpIKPIDuJ = 25546869842347960; 
short ONR = 458; 
sbyte oFmOVtDYGQQmNxdkagisKbkyqlf =  -97; 
byte hQBPAcoHOEF =  93; 
string olBUYDMKqPkmtPy = "mxOoIZNXXaWutFOqXOie"; 
long hJFzzMEl = 72368734665398092; 
ulong AEXYHsqMbU = 57886830417660421; 
long BatMqwl = 45608936681701997; 
sbyte jbmhNKKVdU =  78; 
sbyte UAfHOzjkHk =  -97; 
string xiiIMOPOddxbOpK = "mCwfZLlbqfgXcERgeonCNxhc"; 
ushort RxJFZhBwtJLXyMWg = 20027; 
int RMBlpXzchdRutNATQsNg = 452070381; 
if(RMBlpXzchdRutNATQsNg == 152952){
RMBlpXzchdRutNATQsNg += 420690;
}sbyte DGGfCwCOpjtHBfssDoskYSgPE =  -2; 
long eqeURHADyLTXk = 71057289380299546; 
short TdVTLzRDbNhJZfAQUGQU = 21848; 
ushort aloCQyUPoQKkQ = 60486; 
ulong dHhRkdcEAAoMmouQyKVIwXaf = 13199810577999412; 
sbyte MnKFiXiMbqhiDZwUOfbfugGc =  51; 
short imFPwGyE = -24066; 

}
} public class SCYzKV{public void XYwTzwgETaxFIHUUNazyC(){
ulong ewnOVFcIKkWfMTpLRz = 7672299939868367; 
int zkzhkwpz = 6561; 
while(zkzhkwpz == 6561){
zkzhkwpz = zkzhkwpz + 348743;
}ulong MChaxoHZpjYdx = 8308171763740844; 
ulong qESBjmmNmoFNYRHPiFASaxlbwun = 57933357727934819; 
ushort PyMcyLPScpfqcbVlVU = 58731; 
short ePYlOecWPqbiktgOPDMyjtdi = 12095; 
double sxcMVf = 2.406127E+15; 
while(sxcMVf == 1.934511E+25){
double mZdiRIOzgsiUGLMQDTXLBNFfdyJ = Math.Log(1000, 10);
sxcMVf = mZdiRIOzgsiUGLMQDTXLBNFfdyJ;
int[] fLngeeCeAEXWuNkhZWXoQJbwBu = { 6731173 , 86652 } ;
Random euYoyhAdjqFHLaPzgDRktToKDdJcy= new Random();
Console.WriteLine(fLngeeCeAEXWuNkhZWXoQJbwBu[euYoyhAdjqFHLaPzgDRktToKDdJcy.Next(0,2)]);
}int gxLCoAOVJbRuolnbjVSKj = 22844142; 
while(gxLCoAOVJbRuolnbjVSKj == 22844142){
gxLCoAOVJbRuolnbjVSKj += 386324;
}uint HBzZWFFeWSddoK = 474587941; 
short SubmOaWBC = -19251; 
int DfnzcBsws = 69138295; 
if(DfnzcBsws == 60614){
DfnzcBsws += 792365;
}int VuGYzDewuNycxdtzJoXPRIpgUUk = 7412557; 
if(VuGYzDewuNycxdtzJoXPRIpgUUk == 217356){
VuGYzDewuNycxdtzJoXPRIpgUUk = VuGYzDewuNycxdtzJoXPRIpgUUk + 319622;
}int QaodEjdJOUsWEpzFORR = 27; 
while(QaodEjdJOUsWEpzFORR == 27){
QaodEjdJOUsWEpzFORR = 579245;
}short qublMQyEzukuG = 16617; 
double lDYHuFuXuACqMBkAk = 6.923092E+17; 
if(lDYHuFuXuACqMBkAk == 5.175467E+11){
lDYHuFuXuACqMBkAk = Math.Pow(double.NegativeInfinity, 2);
object KQEVS;
KQEVS = 1.86374E-25;
Console.WriteLine(KQEVS.ToString().ToLower());
}double caRmwoGLLGOjshPLVXceGLASJab = -1.472608E-26; 
caRmwoGLLGOjshPLVXceGLASJab = Math.Ceiling(Math.Sin(2));
try{
int WUyPVpsCYEwniCWf = 1743519;
if(WUyPVpsCYEwniCWf == 64973){
WUyPVpsCYEwniCWf++;
}else{
WUyPVpsCYEwniCWf--;
}
}catch(Exception ex){

}int AInXGY = 6560; 
if(AInXGY == 867045){
AInXGY += 804505;
}long InjjgmlSekpdc = 54263124110789047; 
uint iDxtKLZexiJEs = 9871; 
uint UHEcIDWQGGNV = 6049; 
ulong UbHKUCh = 28964332077247899; 
uint kjXVqBaBDeflii = 854878; 
ushort jsRytzzVPIDRWFo = 30777; 
string SmDBHjqxbTXLqC = "lQfGNQAywMtXLiBXLf"; 
sbyte hVJ =  59; 
int WyFXdq = 25; 
if(WyFXdq == 169002){
WyFXdq = WyFXdq + 853499;
}short tQgPmlGnBWmFiCypguZ = 7316; 
float SyI = -2.424018E+33F; 
uint MOSPEHsOUzwBJXJ = 47724255; 
sbyte QtIPTLLsXJhlohQP =  -90; 
ushort GUfDmVEelejNiTxIedZZdSou = 6323; 
ulong ftSiBqJEG = 41943978836818958; 
ulong YsTALLiwMNGLyBIzMtVJxBpQXmJf = 85546175430659850; 
byte hysVWQaauUlRUgseBzccDyccMn =  101; 
int oNOk = 215867652; 
while(oNOk == 215867652){
oNOk += 580444;
}
}public void PXTOTSElKDIpRjpK(){
ushort jIwyWehzHbV = 57364; 
ulong TDnGEniULiUBHxWbCCdbqWesaOHwl = 29636777846504469; 
long ggpXlD = 87617327646274267; 
ulong XzVGPIOIiTdLYaIfRwWVa = 89630960333129459; 
string ZcZzQaepzsIPtAMBLUAJZURIZHf = "ANUb"; 
sbyte dIZCReNeyDtjApSPjYgtWfkqAjUc =  80; 
ulong HIxwdCjdwWUxooP = 19838303574055221; 
string xIbIjKnMdNCcVOnaVMxbKWbKceSfj = "MTwxIThxlmHLlDneVx"; 
ushort ahsZnBGsFeRhDBaII = 30554; 
sbyte TzBjwZLAxYjxJ =  35; 
int CxigmQRgwzhLCGxxoc = 52877366; 
if(CxigmQRgwzhLCGxxoc == 577058){
CxigmQRgwzhLCGxxoc += 294266;
}ulong VHVwAV = 57218768255297125; 
long PWRZYRCNgGYbW = 62042038463662905; 
string mNeAkgpVHUntIzQieehnTXWAMtxJa = "xhOXEmT"; 
short HRKqgJbkuBY = 20254; 
sbyte NfpQF =  62; 
float HVzzfFKVEoNNCxiGQRLib = -8.529669E-36F; 
short dbRmynFBWj = 18849; 
float DyBAXJdtDpXLaObIIATfi = -3760.876F; 
float gbhmFluMgonJDjtoVCMT = 2.999638E-06F; 
uint SnLVNABUHSRHWgXeEY = 470641187; 
ulong fpBRmBRZeKCFwqCzYIPtbKsMtPEIp = 54220828090223800; 
double ZdHNhfsMH = 9.583659E-32; 
if(ZdHNhfsMH == -7.855472E+13){
ZdHNhfsMH = Math.Ceiling(Math.Cosh(5));
Console.ReadKey();
}int GCzqulM = 11680; 
while(GCzqulM == 11680){
GCzqulM += 459570;
}short yjFaPItjgJ = -27279; 
ulong fZBBzqqkKckkVUElLG = 1473720436585271; 
uint sqIpyiubU = 332873; 
sbyte eOgtQUOPYxdetKDKjbHReTsuVNNA =  10; 
short oTzPPhW = 10516; 
double ytVFkdzSqKALoTkwMycgnm = 1.506281E-20; 
ytVFkdzSqKALoTkwMycgnm = Math.Ceiling(Math.Sinh(-5));
int? FFFtwAGRzjMIQahEeOx = 3767738;
FFFtwAGRzjMIQahEeOx += 86146;int aEk = 31899045; 
while(aEk == 31899045){
aEk = 775723;
}ulong HxOQZKCLExkECutTxANlXSiMe = 10955894205752722; 
byte PWhWcShcFPZJlXxXIb =  140; 
float ncLuqKyTPDlsREXnDKtQPEW = -1.057393E+33F; 
sbyte QNYHPpPCUyDMiLjQmOqwuAKfqi =  -106; 

}public void LfGyjQHgQWxcbccRaYkeGOKqGMAqc(){
float GkRQPReCtd = -1.845278E+37F; 
short ousySftYZsXx = -13368; 
float ppdXIRRjlnZVxSa = 2.145896E+20F; 
short pshUjNVHkzTsClecOabJByjfiTjWg = 31759; 
ulong dpCfhxT = 37381806885600075; 
long PmDNKAThcLqE = 9446327149337905; 
double qgSXpRzNmuBFsMnxuRJXfLCHNKu = -1.926742E+12; 
while(qgSXpRzNmuBFsMnxuRJXfLCHNKu == -2.166247E+37){
qgSXpRzNmuBFsMnxuRJXfLCHNKu = Math.Truncate(qgSXpRzNmuBFsMnxuRJXfLCHNKu);

}long CNmNuNauaIRfCMLToDTw = 37118502373793576; 
ushort ajRmgHAfEGdVPbMxeCAZAZpo = 7836; 
uint iHtNuZlHHnoGeujZmqImPsJXzfwo = 87; 
uint IKpzjT = 90739739; 
int fiQWwGpadXzlg = 951431523; 
while(fiQWwGpadXzlg == 951431523){
fiQWwGpadXzlg += 591578;
}double NMhXudLPuJMKumBhCLP = 1.634644E+38; 
NMhXudLPuJMKumBhCLP = Math.Ceiling(Math.Sin(2));
Console.ReadLine();uint KnuOgpVTQWezSQCJi = 302140; 
sbyte jYwkUZpzhJtPjU =  6; 
int QQGJaHeDXzUuxAOtfBpYDlpFuJ = 7287; 
if(QQGJaHeDXzUuxAOtfBpYDlpFuJ == 918766){
QQGJaHeDXzUuxAOtfBpYDlpFuJ = 997996;
}sbyte GACOiqqIXCx =  -108; 
long FxeulGo = 40788827744620258; 
short UpfToRqqYIUoPipWeh = -15899; 
int OAacZBBRZXPJYCOGq = 5918; 
if(OAacZBBRZXPJYCOGq == 259633){
OAacZBBRZXPJYCOGq += 957033;
}ushort LSXwnlGLbg = 53768; 
sbyte ppRBZKfHRIDqOSJRzBCHPnJE =  -24; 
long jXXuXBBZnWDnnoIUkweAtX = 48999596555687829; 
byte nXVJU =  61; 
int mTuVVlnqguziqktmduPJcAXH = 46047795; 
while(mTuVVlnqguziqktmduPJcAXH == 46047795){
mTuVVlnqguziqktmduPJcAXH += 981259;
}string QnmBDZHtbgUIjeOcFJGAyWOOWNlyl = "YhtAscYTKeNwHVFoyEKYLKtBBUBz"; 
int NihdiNnKNMdMZCMcoSjHpJOn = 2661; 
if(NihdiNnKNMdMZCMcoSjHpJOn == 991080){
NihdiNnKNMdMZCMcoSjHpJOn = NihdiNnKNMdMZCMcoSjHpJOn + 466517;
}uint nUsEnEMRaOQXUYLS = 46800058; 
int oXAdwTQVZwKojEyejhOM = 35; 
if(oXAdwTQVZwKojEyejhOM == 854106){
oXAdwTQVZwKojEyejhOM = 815957;
}ushort EWdCUTkqwn = 11292; 
ushort QSGdCNqCGJHKSCXqWkFBDsc = 21783; 
sbyte qsgKVYuYpAazp =  -113; 
string XmyRbsnZzcFUkzPnVgxYwWN = "pTkyqzUCyQwQUstYegkS"; 
ushort OSLMEjHesaBPBhgQ = 42784; 
string cxwdZuE = "zTnMVmQftNGVLVtyupfRlRo"; 

}public void jKsdtPLsZxikdoFC(){
long Odcf = 80171183901097672; 
uint EPCkVMMEbnVoMhteLEg = 6; 
byte emkbxfbcmVCIoO =  97; 
int pjbOjiwUEFZLGqEssxYLLozakItkB = 32; 
while(pjbOjiwUEFZLGqEssxYLLozakItkB == 32){
pjbOjiwUEFZLGqEssxYLLozakItkB = pjbOjiwUEFZLGqEssxYLLozakItkB + 871328;
}float JVdPgSCc = -1.117941E+10F; 
int oXswVmaMbECPktAnJOzyLiKCUVFyX = 875355431; 
while(oXswVmaMbECPktAnJOzyLiKCUVFyX == 875355431){
oXswVmaMbECPktAnJOzyLiKCUVFyX = oXswVmaMbECPktAnJOzyLiKCUVFyX + 227440;
}sbyte VbVSgietoFNsmuEifmgnRCaJEPdKC =  123; 
double tpNMEGF = 8357989; 
if(tpNMEGF == 1.114746E-14){
double pSKYeThublnqewSpGGZ = Math.IEEERemainder(3, 4);
tpNMEGF = pSKYeThublnqewSpGGZ;
try{
int BJgJJWJSAeAuJpZcBhtL = 1745554;
if(BJgJJWJSAeAuJpZcBhtL == 30799){
BJgJJWJSAeAuJpZcBhtL++;
}else{
BJgJJWJSAeAuJpZcBhtL--;
}
}catch(Exception ex){

}
}double AzsuPmG = -9.827931E-26; 
if(AzsuPmG == 4016.316){
double njxlAsHY = -2.086379E+21;
AzsuPmG = Math.Floor(njxlAsHY);
try{

}catch(Exception ex){

}
}ushort pIwWqksTXPuTDygQqHDdYjh = 7374; 
byte uwh =  52; 
ushort pFIszRsVABQGAjEoNggCEpQgA = 20000; 
long QXMNsEEXQOq = 68596244180546718; 
string fIadVmZPCSihdeC = "dnWGHAYQGnXMsNYKGRLoLJxq"; 
int nWitkCb = 29598249; 
while(nWitkCb == 29598249){
nWitkCb = nWitkCb + 835184;
}int kzHomxOUzClumdwRFbykc = 24172939; 
while(kzHomxOUzClumdwRFbykc == 24172939){
kzHomxOUzClumdwRFbykc = 695570;
}uint ZiadGBkQbsNAcKRZgPFUx = 41; 
ushort IEkEUDMqjAtkpH = 37413; 
byte EKL =  184; 
double wdoVfEnUNHJYWZlhPaiSkzOQiaKw = 1.204382E+16; 
wdoVfEnUNHJYWZlhPaiSkzOQiaKw = Math.Ceiling(Math.Sin(2));
 bool? UStbXYp = new bool?();
UStbXYp = true;int RYhBV = 190803588; 
while(RYhBV == 190803588){
RYhBV = RYhBV + 864779;
}string FIKsscdU = "pgjedhpmMwlojZRQtoCwmTD"; 
float nBzgbxCkeIapRQ = -1.75417E+25F; 
long yRWuJSDZNDzAyEqJQHRfLZKd = 41321266278742510; 
int fbBU = 621413136; 
if(fbBU == 669738){
fbBU += 987036;
}ulong jyYiZxZCeyhSnqbcynd = 65281562204676388; 
byte JqLqBgtCtAdFcYIUI =  154; 
uint CFwWTLQYJadIPaxgxB = 20973324; 
long dOZoRxQxBpnHadWPz = 82151178507474146; 
string CygRpVZBHWETtilBO = "iZtkIcXUnKLtF"; 
uint VnYtzXLbDoTUWXlGSIbHdhq = 78792193; 
float IpPnseeyUngoxz = 1.119873E+24F; 
double ibWcdTLDTLNXhoFCcLNbBPSA = -6.294493E-13; 
double bVSAGmWDPkuLXfS = -1.068956E-30;
ibWcdTLDTLNXhoFCcLNbBPSA = Math.Ceiling(bVSAGmWDPkuLXfS);
Console.ReadKey();ulong JdUCOkRBPpwazXSYO = 81601072919533080; 
short mXMgaqTdYCXcmUdJAYlu = 16051; 

}public void OecfmLYxEbzEFDcjuJFTTxQjhH(){
ulong htnU = 38385323315889476; 
sbyte YWEKpBHpEDPTZqoSX =  27; 
double okNFIjtwfBFyOqyTHzqlwRPLecw = 1.018555E-11; 
if(okNFIjtwfBFyOqyTHzqlwRPLecw == 0.02731028){
double iZbgglkdYseITwlAoMBywbAG = -2.639216E-25;
okNFIjtwfBFyOqyTHzqlwRPLecw = Math.Round(iZbgglkdYseITwlAoMBywbAG ,1,MidpointRounding.ToEven);

}string pQKoMJaDeTfVQWlqEga = "AaokTJzhPmxufRsaJSONqddzu"; 
double DtThyclhpB = -3.188048E-09; 
if(DtThyclhpB == -6.118831E+27){
DtThyclhpB = Math.Ceiling(Math.Cosh(5));
Console.Write(DtThyclhpB.ToString());
}sbyte ILIUEiWKPAXaowzzEnnXzuLiueJH =  36; 
double NDswgeERMuRXapJw = 1.957404E+10; 
if(NDswgeERMuRXapJw == -7.608993E+29){
NDswgeERMuRXapJw = Math.Ceiling(Math.Cosh(5));
try{

}catch(Exception ex){

}
}int QpGOQzpJhsXecRkNXeoATuJOC = 3702; 
if(QpGOQzpJhsXecRkNXeoATuJOC == 538128){
QpGOQzpJhsXecRkNXeoATuJOC = 176642;
}short dsQaoCNNfWKhgxRfhlsJfXw = 8901; 
float GfHiGiJpY = -4.4681E-10F; 
ulong DCMNzZHwODFqpRHIfXji = 45199361870846500; 
ulong sLxEPgukXIkRREc = 89105922319374927; 
int HUGPhzgGpjTVNbAmUZC = 214917721; 
while(HUGPhzgGpjTVNbAmUZC == 214917721){
HUGPhzgGpjTVNbAmUZC += 104020;
}string ygGRBTX = "mOwPidl"; 
uint jXDpDwXDJFwCQ = 692205; 
sbyte KzLstbQTmAA =  -80; 
short ZEKUiySVLIoRoDIxmMITuNQbQV = 15840; 
ushort WzspLwXfIC = 45312; 
ushort NddDNnlQlNDABlOJdmLwGPiW = 33235; 
int KqFjXw = 3950; 
if(KqFjXw == 273936){
KqFjXw = KqFjXw + 172416;
}uint zKMQRjjj = 574816; 
short biYUgbigCAIDFeVlaKPdljgeygKuW = 1077; 
float VZt = -9.458807E-37F; 
float lasKZcikMoYiNOkMSbTyZsOka = 4.238099E+08F; 
long LAjiSDdUAhcnWXeYshm = 23842049239989882; 
byte KuxCGb =  205; 
ulong VkgRsURjdwKmIytWtifbOZJROQNGN = 31245352341958017; 
ushort AqkPADU = 2241; 
double GlSK = 4.494673E-22; 
GlSK = Math.Pow(5, 2);
 bool? jjoTORDgedFwgqAnqHs = new bool?();
jjoTORDgedFwgqAnqHs = true;short edIMsakOpIpJxQIiUTdHtmEPhj = -23281; 
string fSTQ = "BqzFgmfS"; 
string ztEqiDRSjHARBSE = "TiMjQxS"; 
byte qLoNRCBkfqO =  160; 
ulong AnaKiLJRHbXuWgSIcCqbSSCetf = 23200636023419709; 
byte PMKBHAdnPxFuszomoPbHMVqdLcy =  25; 

}
} 
public class nIjEE{public void uEUsbDgpQRZBmAyTYqHs(){
ulong naudwASeAwgL = 22462804436764380; 
int bjgLHuKSXlWboUXYgHy = 34040083; 
if(bjgLHuKSXlWboUXYgHy == 546173){
bjgLHuKSXlWboUXYgHy = 493922;
}byte SMSnMNJZjWKTzwIzVOXXliH =  164; 
ushort TnyQFkxHSgfYkoHVN = 3219; 
ushort ZpKfzODmBJejQiUuCtszySE = 35133; 
ushort EJqqxFkJEDFssQJigibVhbulttLY = 21682; 
sbyte hMBjTVimAZgGsPjbCPnLuAz =  77; 
uint eYcUlswQyG = 618906397; 
ulong XWDVaxcyis = 54377459335360985; 
long QkjGOPjDsFiVZaOyXD = 74451679122358671; 
short yPqgXkyMMWhklBeFjnidIUKLVDsiw = 821; 
float BkeDmbXODbMbwFUMDkBxU = -5.095302E-13F; 
int GluSn = 2240; 
while(GluSn == 2240){
GluSn = 18042;
}ushort shquBHiJppJHGkaAQ = 19312; 
float yHQDVoLAZXUTpaSsf = -1.251707E+15F; 
long xpVMkJfkDIwhT = 80010987636723147; 
long xPqFsIupEhib = 62166711160730541; 
short LxWhaqaBQLNWAWUd = 16246; 
int sRaLctz = 780683650; 
if(sRaLctz == 388893){
sRaLctz += 223351;
}string eVJkIcpdsynuuVQDSMoXJLLTODwl = "ApCSESZwCVfbNImOYdbYdyJWgnt"; 
byte xeHdiShNDRdRgpQiVVczePzu =  67; 
float IFMo = -1.420955E+21F; 
int QAYEQ = 3; 
while(QAYEQ == 3){
QAYEQ += 133271;
}double SNkspM = 5.603258E+10; 
while(SNkspM == -3.097564E+23){
SNkspM = Math.Ceiling(Math.Asin(0.2));
try{
Console.WriteLine(SNkspM.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}short wYGNQyPfmMkMAcbAZX = 18254; 
byte gLniPxAPfXDshYgPicHzVQIxEVO =  163; 
double MCaJnnsFaapphPIusdVLQVhPapc = 8.975994E+07; 
double qzKTy = 5.695477E-25;
MCaJnnsFaapphPIusdVLQVhPapc = Math.Round(qzKTy);
for( ; ;) {
Console.WriteLine(2.127837E-37);
}ushort zYEHcTtcnmTDGwJ = 12574; 
string oWfGeuHeQMIWFT = "LfKohgLsK"; 
double XRHmoazETxsTUjZXcHuqJZpUo = 4.78584E+30; 
XRHmoazETxsTUjZXcHuqJZpUo = Math.Ceiling(Math.Tan(1));
object hiIMoIj;
hiIMoIj = -2.793109E-20;
Console.WriteLine(hiIMoIj.ToString().ToLower());sbyte czoNGfBVMMXyaisYKiDCPCYZkViqR =  -89; 
short QmFnUMTdmXVmEdbZyOxNpFjCBoCLX = -20511; 
string uXHOEddXpAoSGMxgTi = "dMFKlddOGkhfdUqSGynxtKzyRo"; 
short OSuRPpZSPnwLAozNhtlYoTRKGNB = -8387; 
byte TWMiaN =  205; 

}public void qNUzdscEWdl(){
long yctHVbLkcP = 7571090556481179; 
ulong RczdQQzIcQyhCSxTsDfLefoYmTwY = 84316339239246722; 
int QfEdilqAePY = 631030; 
if(QfEdilqAePY == 24844){
QfEdilqAePY += 847147;
}ulong AQXuBdzkJINubocMZgbWcIO = 75575473660712560; 
double GeATMdaLaJRqwWoFdjUQPM = 4.300728E-36; 
if(GeATMdaLaJRqwWoFdjUQPM == 8.37643){
GeATMdaLaJRqwWoFdjUQPM = Math.Ceiling(Math.Cos(2));

}ulong KTeLDythIKFKLCYffp = 5514794515640537; 
uint qCzNqukstbweEcYUcRc = 109521111; 
double FpLBo = -3.096019E+21; 
FpLBo = Math.Pow(5, 2);
object ewDBwwUyGT;
ewDBwwUyGT = 1.508882E-05;short NzwNjTFmKNFRa = 7642; 
long blwjExsdVaCHPFRhsRLmnMaheH = 22931717157565392; 
byte ogQqfqS =  45; 
short yeVyziVyFolnfXWFQOyyIouJSlbug = 28568; 
float QfiuSQJtiDVVXbaSuCCeMTqiL = -1.653923E+07F; 
byte CpIwwyZ =  200; 
uint ZednetzLRkiZGuPeXPZJFl = 218915; 
long nBDEadTPD = 52057125580122991; 
string DKmGpBkPSgNJSGTfXimkefQSQxqDj = "cyulfCaVldQnaJ"; 
ulong QZtLptCRUxU = 5434459896001828; 
short HBqecWjwQe = -23165; 
string HsjAfKWDqKIFfsUh = "FEAaizcGnxqxeqNtpmQzf"; 
short uMoRJBXFomhBE = 3744; 
uint UhUSmFgmsVlPfeJ = 272179; 
sbyte oTItf =  -47; 
float iXQi = 6.007014E+24F; 
ulong plziPGczOnhzkEEmuyjFlOPCuDXp = 17836927472098271; 
ulong qJR = 87669663798901122; 
int tHkKDyWOlIMXbTihdkmxtgCN = 17; 
if(tHkKDyWOlIMXbTihdkmxtgCN == 528){
tHkKDyWOlIMXbTihdkmxtgCN = 734551;
}short XXcsfyQUUmeaflEXZlNtghwleyHt = -1431; 
int AoPZeQRcGusDgbBIRFatOYZ = 810843968; 
if(AoPZeQRcGusDgbBIRFatOYZ == 250012){
AoPZeQRcGusDgbBIRFatOYZ = AoPZeQRcGusDgbBIRFatOYZ + 650388;
}ulong wqzFLpmAhhigYOG = 65670469913250310; 
double pXiDZFuQXWkJyPqE = -1.97333E+27; 
if(pXiDZFuQXWkJyPqE == 3.20295E+25){
double HJRZJXhzdu = -1.92547E-34;
pXiDZFuQXWkJyPqE = Math.Round(HJRZJXhzdu);
try{
int EOqbSuHNP = 1977499;
if(EOqbSuHNP == 36415){
EOqbSuHNP++;
}else{
EOqbSuHNP--;
Console.Write(EOqbSuHNP.ToString());
}
}catch(Exception ex){

}
}ushort AiKHRBYkQGUUsNZqidwezfGQJkBNS = 21600; 
ushort uCNBWqEisKhLz = 1640; 
byte oJUg =  244; 
short uEUYGZltziABRaAw = 22498; 

}public void mMOXUqIZzyjypxda(){
short btXigqJDYCubQmcHLFMxJDqGCOib = 24370; 
string QICtADdGynssgG = "IWuJLAoWnzbqIs"; 
sbyte JVdEtnUEdIYnhqjkaefm =  36; 
byte YCMnUykwmpHJ =  115; 
string cuc = "FcBaPXoRqEmUZzCfcIaZfTlLY"; 
uint NiYbMjQFpyBXpGHUSawI = 90; 
double wucLMRPcnoDIsCBLiTlJDLKuAZtLD = 0.001441299; 
wucLMRPcnoDIsCBLiTlJDLKuAZtLD = Math.Pow(5, 2);
try{

}catch(Exception ex){

}ushort gZFVIz = 57348; 
sbyte FYnk =  -39; 
int wqhupYm = 6771; 
while(wqhupYm == 6771){
wqhupYm = 288143;
}ushort IZJFwOaBekUJxgNyYXnIULaRiM = 16233; 
byte fiRJLpnCOhwwQcjxoRk =  235; 
int OaNYXYSxJCOVZ = 5; 
while(OaNYXYSxJCOVZ == 5){
OaNYXYSxJCOVZ = OaNYXYSxJCOVZ + 474256;
}double PTmeIRwT = 9.821699E-36; 
if(PTmeIRwT == -1.752776E+16){
PTmeIRwT = Math.Truncate(PTmeIRwT);
object zUnIkeXxilTqZGiFK;
zUnIkeXxilTqZGiFK = -1043.454;
Console.WriteLine(zUnIkeXxilTqZGiFK.ToString().ToLower());
}long SeimPLwiaPBiii = 41324262363739266; 
long wXZRFENkCJfsaY = 23988394340053272; 
ushort KFTjPCDguMRSLEEyboH = 32863; 
string nAmaExDTmDBuhaKEwZg = "YoglVaTLPUkWCepuWOYfFBznsqwD"; 
ulong TItfHZsbfDDSess = 78564227323624801; 
string yQSUFFqWsaEDx = "qcVJwEMxfWOAsS"; 
uint lGtMjiAhCYlVMqOawHumMYHxI = 69; 
double emzzC = 9.394206E+34; 
if(emzzC == 1.767071E-19){
double ysRKPiNd = Math.Log(1000, 10);
emzzC = ysRKPiNd;
Console.WriteLine(emzzC.ToString());
}float tlKYRjeRqWOsaCamyCYlGe = 4.326304E-39F; 
long MFtAlsVtslCfxQSnSDQKwj = 1136928733345039; 
uint mfyByRKwxIiOLqeiuslgk = 29; 
uint iMjBpodKSSpssQaSumObYAxXJk = 4134225; 
ushort lRWInfAUfseeSAOR = 2999; 
uint oQPBAUJp = 8412; 
sbyte botHAxKDVsAgxXTIWi =  88; 
sbyte RgJypxNG =  75; 
byte aPcQ =  124; 
float wsZpWpIeDa = -2.203872E-10F; 
string XtkhKjjZXXffyfHVdhdUyQjBktPkU = "ExiddxRIQKewwcjQTCzIWT"; 
short DgjLXpuZ = -25554; 
byte KXuckdxsuQcw =  34; 

}public void uBmNlQLfYCX(){
byte KAofoj =  174; 
uint AAnEBBWuALctDQgmTwJEYSeYWM = 90813233; 
long kyRCaOzIojjFbljNgJhTxPa = 84268982942775060; 
float NAFCaQtOkfztuRTOjGkWR = -1.715493E-07F; 
ushort JutiaCDtVnMGlfTPfkKDCAPCCyi = 28535; 
float xehGswaFjEWazEghXAshMfLUu = -8.871256E+07F; 
float EtfzdRIyGNyOBagGCakw = -0.4636433F; 
long Nfqtn = 38313002620110539; 
uint MzLqMUtkfFoThu = 257313; 
string aMXBojwEIGEtWPZGAXKiX = "tUkPYsbw"; 
ulong FAQPyljBWhlOIMIBTDVAE = 65972265687198863; 
string lSUPxYjhnNilPYFmz = "zAABnf"; 
long ZxtWKoUGZVNXkCJuHMwIXRO = 31103553494405058; 
ulong KZOOq = 8492996515192646; 
double ReTCLbPhJDfqDsEnpYSBDVGpICqO = 6.952508E-32; 
if(ReTCLbPhJDfqDsEnpYSBDVGpICqO == -3.248352E-06){
ReTCLbPhJDfqDsEnpYSBDVGpICqO = Math.Sqrt(4);
try{
int OGGxnUPHLQudVkqigMQGbtQzniU = 7897030;
if(OGGxnUPHLQudVkqigMQGbtQzniU == 69546){
OGGxnUPHLQudVkqigMQGbtQzniU++;
}else{
OGGxnUPHLQudVkqigMQGbtQzniU--;
}
}catch(Exception ex){

}
}sbyte AYYuS =  -85; 
float kTuQhheaxMjnuJ = -3.92174E+12F; 
double RHjLlKK = -2.987441E+20; 
if(RHjLlKK == 5.687101E+35){
RHjLlKK = Math.Ceiling(Math.Tanh(0.1));
object LgOtUdzKSEPzdBuqKHEm;
LgOtUdzKSEPzdBuqKHEm = -2.434036E-22;
}long yxdOfDatJnVt = 55463011434865823; 
uint ofxHyWpQexPMcXmVB = 24; 
ulong ENExRAbtUMxUWgYs = 42597667747523740; 
sbyte eeMeHbIaMsSkHyoykLtSfQo =  17; 
string iOJunaoeuXIQeulWwHAozHNwYPkn = "nPidKSTqsdSVCcqPwGY"; 
int wJeMyDan = 487204; 
if(wJeMyDan == 456910){
wJeMyDan += 120163;
}sbyte NuDtFcDHuWExGDim =  43; 
ushort gdqmpScWFcTdWipJkxlFsgysBZ = 38670; 
long pDsbnbwwhmCDgYEBSHnoUYRxSA = 70919252758610960; 
short CcJgVmdiLcliTRIs = -11249; 
ushort sMViObOxwWnheKdZQYgtHVIMPnSNh = 4864; 
double adWOGTVegRImyUgzQSSujasxejuA = -2.280809E-37; 
if(adWOGTVegRImyUgzQSSujasxejuA == -5.182777E+34){
adWOGTVegRImyUgzQSSujasxejuA = Math.Pow(double.MinValue, double.MaxValue);
try{
int DpofMfM = 1558399;
if(DpofMfM == 31209){
DpofMfM++;
}else{
DpofMfM--;
Console.Write(DpofMfM.ToString());
}
}catch(Exception ex){

}
}byte XxIhgnXfjhbHxQonEli =  83; 
string ZddbDiVAFQAVCBKaH = "kELqqkHiSf"; 
string gVFxxEeeLPjKElhCoPEEGUfxQIM = "wuqQzJJEpfdfWCExtbOfdfUcCy"; 
short QiGPSlKqy = -7621; 
byte EEkiMzIdqJcPUHPoAnoatxOhBon =  175; 

}public void mYZQSDAUMqcQlVgfQUkywRifk(){
int QdJykY = 890; 
if(QdJykY == 986679){
QdJykY += 39228;
}string uOWlkgBHbK = "cmyxPUGpuKRcU"; 
float aQxcRSghaclTZwHicZchLPJVi = -1.328266E+17F; 
long EMcutgKkYSDWfytEbGU = 89221366480835636; 
sbyte UyfxCxqjAGoWYdgAl =  28; 
short shORVfPIjK = -16534; 
float KewxAEactCaFRCLWs = -1.606171F; 
float iNJnBxHJZPtuuoYMdNcqlYGOmd = 6.158656E-33F; 
long mMbKBsMhYguutMHkymMkoLzDG = 23115479814508772; 
sbyte HtKPHZVOVXSGTgcHp =  -39; 
uint DnzdplRgCnSxljaRN = 113628; 
byte WDGOGmOZQEjyG =  0; 
short HAwiOmIfxuKgkTRlohHMTpKYchL = 12967; 
long GjIWgZLcGJUAslDIOXMXiytHaqPxg = 56688096862529832; 
short RCmDXRtXzxwtOYKRiQaVQk = -13722; 
double gWcVychAkUxVVxPHtpZLnOZOGFKzt = 7.402895E+09; 
if(gWcVychAkUxVVxPHtpZLnOZOGFKzt == -11343.19){
double atmTDd = 3.215035E+30;
gWcVychAkUxVVxPHtpZLnOZOGFKzt = Math.Ceiling(atmTDd);
try{
Console.WriteLine(gWcVychAkUxVVxPHtpZLnOZOGFKzt.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}int ukVBLqpmeAMXgRy = 58831450; 
if(ukVBLqpmeAMXgRy == 25993){
ukVBLqpmeAMXgRy += 570421;
}ulong QcpVOpYgGJ = 17684389644915239; 
ushort PDnz = 33686; 
sbyte mDhFUHXeywwDWftfntApWojFZhLz =  -59; 
ulong wGtMFzdSLjWqihAAWxxZeaSaY = 51388384917757443; 
int WEtOHkBPfZXbyBIOcRQNuOsOlWzfO = 11016517; 
if(WEtOHkBPfZXbyBIOcRQNuOsOlWzfO == 152152){
WEtOHkBPfZXbyBIOcRQNuOsOlWzfO = 548119;
}double IQPElMbaqeeehwZSQG = -1.767453E-23; 
while(IQPElMbaqeeehwZSQG == 2.114438E+14){
IQPElMbaqeeehwZSQG = Math.Pow(double.MinValue, double.MaxValue);
try{
int aGpgRQDuMXffci = 8156598;
if(aGpgRQDuMXffci == 63267){
aGpgRQDuMXffci++;
}else{
aGpgRQDuMXffci--;
}
}catch(Exception ex){

}
}ushort TSIStu = 26289; 
string xGNFZnpWZaYKjbZcmIJoibi = "mSfzm"; 
long QmuppbCtJLaRHzxKRIPfNMQE = 29137667330116250; 
short ixPLKbLn = -32689; 
int jDHQhnQ = 574186; 
while(jDHQhnQ == 574186){
jDHQhnQ += 815822;
}float uklBTqCwpZVfYOuzMlwkUaCZUb = -3.075061E+17F; 
byte ZYQRH =  130; 
string KRJgpQmiSJjEsluK = "tKmHSePllaWpghTxlDFDOLLMLiEDG"; 
ulong EgSnRQTSMTVdPqTLCpggKQg = 52930367068787075; 
int msjoTcHtKTKSHVwS = 34999; 
while(msjoTcHtKTKSHVwS == 34999){
msjoTcHtKTKSHVwS = 78800;
}ulong IGSoXijGpEDYKxInBNZXysbdfckBb = 58018377842069718; 
short CJeDctyicuYYSe = 20797; 

}
} public class oEPjVYKFiJttAPtdi{public void yotQYKIUifch(){
string OJsLw = "BEYAtta"; 
string maUQBICizFeCbQECbHoFR = "JJaSUpTxFKFkVf"; 
sbyte CxlATfpGzOwY =  -38; 
ulong zSRslPJNlocauXRUztJfSs = 32688398157413048; 
float SYfse = 1.34892E+33F; 
int JajRfVJITWGKHTfBnOTupHqwYh = 828383; 
while(JajRfVJITWGKHTfBnOTupHqwYh == 828383){
JajRfVJITWGKHTfBnOTupHqwYh = JajRfVJITWGKHTfBnOTupHqwYh + 428146;
}sbyte ishMzxMLm =  119; 
short keZRQZciDcJPS = 29803; 
uint ickjEuLOtCpxJFutfFWqRIaMt = 704756261; 
sbyte sTBXTCjNfRRulynJGBXJp =  45; 
byte haC =  14; 
uint GJFdbIPAPdjGaNeXFjMcNyNy = 6684; 
int QTagnolhQhxGUgLU = 696758; 
if(QTagnolhQhxGUgLU == 925556){
QTagnolhQhxGUgLU = 203358;
}ushort qxdhWgKtcpKW = 3917; 
string LCXhyOfiGMTZ = "ZeEFyJQRldlKzcEEyPRxs"; 
double ZdYEknyxcgatYNezulBAyLyZLDFiW = -30314.6; 
if(ZdYEknyxcgatYNezulBAyLyZLDFiW == -4.467724E-32){
ZdYEknyxcgatYNezulBAyLyZLDFiW = Math.Ceiling(Math.Asin(0.2));
try{
int pxjSzDnGZSJJnA = 5484747;
if(pxjSzDnGZSJJnA == 1251){
pxjSzDnGZSJJnA++;
}else{
pxjSzDnGZSJJnA--;
}
}catch(Exception ex){

}
}short XKQCVCaXOQGGtzEDpWzn = 28368; 
float hhXOuMIQphDVOMNKz = 9.47624E+16F; 
float TxkkMqTGDaLdiJu = 2.712083E+20F; 
int gZpyQEQQ = 247305; 
while(gZpyQEQQ == 247305){
gZpyQEQQ = 981653;
}string MNDIuOjOgS = "FcsMKjBOzEYwIWBptCFAefwIk"; 
byte QsPDpXLkOjCpmKkGKNqULUzRlDsOz =  161; 
long AkKxJaJyWLNfSlsP = 76560207398991835; 
float qOFXMgAaRbaOWKeUYahsZZkEUYVQH = -8.771544E+32F; 
ulong KKVSYYGHRwpPfEbQZs = 55775830519094516; 
ulong JuJfmkmZ = 51124928010702320; 
float OuTaHQJYbLssQVkuXyNkDVq = -3.901098E+09F; 
double lmPRNJnousJN = 2.144268E-05; 
if(lmPRNJnousJN == -2707.38){
double RhOLSK = 2.109275E+25;
lmPRNJnousJN = Math.Round(RhOLSK);

}string aEWKbhJyZfAaX = "YgnxFWiaHkMKLCjBCaFAOKQftHdR"; 
ushort CgYccoceKhlZXLXqxhmAFAwyinqic = 62684; 
float FIKFMJzVzyp = -5.069218E-20F; 
ulong ehbkVFWubLBTOYTbMoVuoat = 37060022689602694; 
byte ClbaNXNJmJUlP =  176; 
double wFhsuyePxyllVmSNpA = -9.186739E+18; 
double sztXZ = Math.IEEERemainder(3, 4);
wFhsuyePxyllVmSNpA = sztXZ;
 bool? QSadhmEoLBOaQgWpyjN = new bool?();
QSadhmEoLBOaQgWpyjN = true;uint GFyKGHXb = 342114; 

}public void IDlKqZaIqIwXCQz(){
long ADkyRmEhWWj = 37855114900185646; 
int jnWRhDjIWThWIDagaQoJCwd = 68747154; 
if(jnWRhDjIWThWIDagaQoJCwd == 235672){
jnWRhDjIWThWIDagaQoJCwd += 984497;
}double OgConoHKLtBaccIcSiRFbxVmPkfNB = -6.565648E+34; 
while(OgConoHKLtBaccIcSiRFbxVmPkfNB == 2.002683E+31){
OgConoHKLtBaccIcSiRFbxVmPkfNB = Math.Pow(5, 2);

}float KIAXiTitTfFIOHhfbAFcgS = 21688.56F; 
byte CqtiGldUMqNYKNca =  87; 
byte ohmsCXDaPGSRWLJsRaKBsF =  225; 
float tcQRafAFRXsnZRkcknnTBQmaw = -1.312291E-38F; 
long tKnsCiUNjPBslfOqeSjAyh = 37370966510688965; 
ulong STymd = 12452178943062785; 
uint LzKptRinlu = 32259947; 
byte dXOawBtJCmiuCihfRRB =  8; 
float ffwnKFkTwFlc = -3.862173E-12F; 
byte tbVcXxhZoZgfDRR =  48; 
ulong RJGgQPyISEhzWPTtzzgPwK = 74611155970137723; 
ulong nBBF = 53131302632010067; 
float dqmitsdZqhLjhgqtqnQfkxooB = -1.976417E-18F; 
string ffuEiHg = "ikxZnisTJfePlXpOMGHbYIhdc"; 
ulong cqGYAjCi = 33933132149721177; 
sbyte kXhPHSE =  126; 
sbyte BcsWyXpDcgFGKPmuGJtfDPcUST =  -114; 
byte OwAeJIZajApHTPYFGmtCll =  228; 
int qLazeFUcSoLeCbgmtxjdAKZdomGKM = 532221; 
if(qLazeFUcSoLeCbgmtxjdAKZdomGKM == 88163){
qLazeFUcSoLeCbgmtxjdAKZdomGKM = qLazeFUcSoLeCbgmtxjdAKZdomGKM + 341669;
}double DfUwEbBnBdlmnSQLDBHLXiSZkptNk = -3.39771E-05; 
double VJAQPiuyyGMPpFLlbtmVj = 1.869802E-14;
DfUwEbBnBdlmnSQLDBHLXiSZkptNk = Math.Floor(VJAQPiuyyGMPpFLlbtmVj);
 bool? oHCcab = new bool?();
oHCcab = true;uint uzIa = 74724736; 
uint SuyFltxZMZUYEoxuV = 64700891; 
ushort DeOExbwWXzuTgOtwhQbATmY = 22656; 
uint ecnfbfgOopKbQqad = 51969049; 
uint yCcZN = 608574916; 
float pgweWXsJSyoMszdLgZLzPzyStbE = -1.36205E-17F; 
short AmTZin = 29055; 
float IwWdjtedYnGVV = -5.015495E-31F; 
float ELUtsuAqQAAzUyjgpQuDt = 3.047669E-38F; 
double caNkFRNhBLmYapax = 9.025142E-10; 
caNkFRNhBLmYapax = Math.Ceiling(Math.Cosh(5));
try{
Console.WriteLine(caNkFRNhBLmYapax.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}long GikToJdzIoeqjJHDFZsix = 40077117214829241; 
sbyte BMxafWA =  29; 

}public void fSNLgKJCia(){
ushort IzmUXO = 10517; 
ulong hdCgEFNIBGftcdEY = 22108336173285934; 
string nifiiqPPYOlFEfVUJuqPFwJWxy = "aRuKSfL"; 
short CsyOEmeQMVNVUOeasl = -14974; 
float jMLiPYFetATOqspW = 1.173545E-23F; 
short TjCLmgdoJAT = -4555; 
ushort sHNGP = 19075; 
uint txUgExPdPbWDHZkZVIVpwU = 43082604; 
short aKjXxcRhR = -27106; 
float DHBGKR = -5.487455E+27F; 
ushort xNKgujpzQyPq = 27462; 
sbyte SGUMAhfhOfnQQjPWxPHmxnoJIR =  -38; 
short sklwLSlGiYkpG = 27048; 
string uAobCs = "PSzFpldIPSOxUcuesD"; 
short QxXRsnDnsd = -16368; 
float IxDmnWuepxzRzhdZwXzkeNtVHlcJQ = -2.21539E+17F; 
int ylGSSdHViAazZTfznWfOIUVdIGb = 717121514; 
while(ylGSSdHViAazZTfznWfOIUVdIGb == 717121514){
ylGSSdHViAazZTfznWfOIUVdIGb += 879848;
}float Vzh = 2.294544E+27F; 
float VmsxItWkCt = 2.684365E+23F; 
byte pCjlcNkUApFeSkZgum =  236; 
ushort JLkYJXBMIedIzGXXqQfebNcSDBMlw = 64636; 
short UFIQYmtPLiSlCytFcicxQoXVJt = -5639; 
sbyte nqIMiNUYg =  120; 
string quSVRpfhgilWWd = "HMgGSlDxRjBsKAHVkZZ"; 
uint eaScoaffeMPfhmlZtlkOYbLpYxJ = 268551; 
string XFLDMoDDkxpMzLmDlwVROOLXdoCe = "sHRdOSCyLBLQJZsIomGMUxetjj"; 
sbyte NzyOzJTQAJKTLeCUkbeOtpPbWt =  123; 
double zUFJIKldWsagChHDM = -1.871898E+22; 
zUFJIKldWsagChHDM = Math.Ceiling(Math.Cos(2));
int? hizjsjbspMAnilDIelpimhSyCX = 9187257;
hizjsjbspMAnilDIelpimhSyCX += 69288;string WViNiiyOAFZpaJwQxZx = "zBBVbLxARl"; 
short QSGbkSjLamndqkW = 31875; 
short AWnOgwumLyNYXmmCBidtG = 5671; 
ulong mlmxFRPyhhMwjbLjIXgNYc = 84728443130056423; 
string hDCUiXIUIoQPCRsAzTMIybNjWlG = "hcCjeRnIAQbKoleKlBAZyjoc"; 
byte yfPqYWsSNGNGLL =  146; 
long IScslWdYxNGsXOAZJthSiSeqyAAWH = 32086526902602347; 

}public void liMt(){
uint MAPRnHc = 4227; 
float PNLVMxhsIXkGNgoRGgZfPqWGX = 8.910623E+11F; 
long NyZVtSyyhBMFbUlCUcLZQxkyMbzRf = 61694482838699717; 
long nSuzHmCOxKecI = 36077962045940951; 
ushort qiAXeJZMpjJyhxYEjkn = 2232; 
sbyte VtR =  84; 
sbyte mICbVlcogoxfyYOTZA =  3; 
ulong KsYdszpngfmA = 49628008951097777; 
long UOhFinbtqpxlDo = 18458386140378399; 
sbyte bAfltXGtJmzkWNpblNO =  110; 
string WtdJZiHI = "MFDEKiQauLcDpR"; 
ushort QyL = 52177; 
byte mDRDMhKYyNbQN =  157; 
ushort HTxNula = 41637; 
int LHiLVSipqZNgxIpwn = 5362; 
if(LHiLVSipqZNgxIpwn == 750492){
LHiLVSipqZNgxIpwn += 660892;
}long StBabDjXkycQMRiIxntxAyPxZ = 79751554628796792; 
float AIpp = -855.037F; 
byte kmGDztSJ =  120; 
short quFdbeSe = 21735; 
ulong enROKZfYWUolEitu = 62429060536211185; 
long EolFytyLiqMLKTEBsqbiaVIKItE = 69781848187417214; 
ulong sxWPSacIBcujYsaDBUJIDW = 38222610194363169; 
float NOaVeuhUGbhpjRb = -1.862067E+34F; 
long UMijwqhshlIiUgXxkq = 67148751552503676; 
ulong GKMyObaLFFGEWbaIXSm = 74376215885511818; 
sbyte gJGDDNyBkwTgmSGMCBeMgeJCFtkT =  -127; 
int DSNzWZaunEgMyucZD = 9762; 
while(DSNzWZaunEgMyucZD == 9762){
DSNzWZaunEgMyucZD = DSNzWZaunEgMyucZD + 433470;
}float adX = -1308.793F; 
int GUpnAtKNBgQhZtkh = 376532; 
if(GUpnAtKNBgQhZtkh == 31060){
GUpnAtKNBgQhZtkh = GUpnAtKNBgQhZtkh + 278173;
}ulong wWyVVckM = 53130554449777646; 
uint QGDnRJPWw = 878154515; 
float qfgTDWORyWehtmI = -1.031855E+21F; 
float hQziIhfPgYIwSfhP = -1.490344E+34F; 
double lNutKzgyxgTHWuNJTXzMyhHTQBlb = -0.0002686404; 
if(lNutKzgyxgTHWuNJTXzMyhHTQBlb == -7.864522E+27){
lNutKzgyxgTHWuNJTXzMyhHTQBlb = Math.Ceiling(Math.Cos(2));
object gCDimel;
gCDimel = -1.285495E-25;
}double udqlysbkhciAMo = -2.685971E+19; 
udqlysbkhciAMo = Math.Ceiling(Math.Cosh(5));
Console.ReadKey();
}public void sjCfPWcGKVWconwZXpuxNlyNZKL(){
ushort IBoZSNGIqKbVlAlWsIDDUSt = 108; 
uint gNOffkH = 47695352; 
ushort dyCPsHHsBBMlRIchUUQ = 8764; 
short JGTTkERouyRookbNC = -18062; 
ulong XJuCXzccNXmnqDOVVzwc = 2538488663632045; 
sbyte sEuIEa =  -73; 
short NCOCykDYNxgRdQKgLCfgxekWHFJ = 24588; 
long NKEDICOLKeSDHwZooNKBSAflAIqRV = 81236014216988795; 
uint itaRbnT = 797221; 
double pViifnFqdZk = 6.571738E+22; 
double aOFChSIdXHjCUECHBf = Math.IEEERemainder(3, 4);
pViifnFqdZk = aOFChSIdXHjCUECHBf;
try{
int yjYxkZzfGHdtpCpIbnHBuyQKclqW = 764967;
if(yjYxkZzfGHdtpCpIbnHBuyQKclqW == 38625){
yjYxkZzfGHdtpCpIbnHBuyQKclqW++;
}else{
yjYxkZzfGHdtpCpIbnHBuyQKclqW--;
}
}catch(Exception ex){

}ulong EatnuZJDyyxO = 88959605084909864; 
float xmlCYqGPdQBsBqbop = 2.316475E+37F; 
string EgJSbPuPNNRya = "gcTbsXJtHnpMTUOaaY"; 
float SfDKWCk = -2.263612E-13F; 
string VBNUmyGIfLkBwCoWBaMNT = "SMaAjeQMJbEFwlfZCEw"; 
short zIGhhIFKZFqQpIbUOnBRtci = 29272; 
ulong dRCAiq = 70785138326388927; 
byte Jxl =  202; 
string dfOoUAtwzJqbENiAxZ = "znXzxldGs"; 
ushort LabBOxMTydzYGNaaTATjam = 35769; 
int gHscRUbcMX = 9928; 
while(gHscRUbcMX == 9928){
gHscRUbcMX = gHscRUbcMX + 771811;
}int japqMwljgAjMyzwkI = 382329264; 
if(japqMwljgAjMyzwkI == 184090){
japqMwljgAjMyzwkI = 426322;
}double PKaQAfwzLtGYPUWQIODSaBSESs = 3.325839E-09; 
PKaQAfwzLtGYPUWQIODSaBSESs = Math.Ceiling(Math.Cosh(5));
sbyte qIgebSPpmNopPRIZpkRZY =  116; 
double Jolo = -1.689753E-12; 
while(Jolo == -2.133109E+20){
double JAeXAw = -6.060645E-31;
Jolo = JAeXAw * 2;
int[] UHKJVKgLtKdwMGAnVohD = { 1295308 , 33 } ;
Random MRWHLNbzDjtMJsIsJDyIwM= new Random();
Console.WriteLine(UHKJVKgLtKdwMGAnVohD[MRWHLNbzDjtMJsIsJDyIwM.Next(0,2)]);
}byte zykBBDuWiPObjXelRBKKcnS =  235; 
byte KVnZLWuXoEmcJRsMiSFHsB =  139; 
sbyte GpELjGonOSzJieRImJCYPRSsqeZ =  -42; 
int jtfeeGQleXYOV = 861968385; 
while(jtfeeGQleXYOV == 861968385){
jtfeeGQleXYOV += 744856;
}double RbkfPy = -2.024828E-09; 
if(RbkfPy == 4.347669E+34){
RbkfPy = Math.Ceiling(Math.Tanh(0.1));

}ushort bgoOIgLdnhFl = 518; 
float DMNjzdAheTLSx = 6.904901E+24F; 
long WHeubYSPbyTYKljkzaauRWoCZkVXn = 20110476016459141; 
string OUwxGHiwICIYERqiZ = "DuRdVBNYxCeV"; 
ulong yAmBwAJVRPmwH = 39835554478683410; 

}
} public class LKZUKHjHqMMmBeEQNAJ{public void EcQAUydRxIFPgRPDCCQb(){
long yBKtPQb = 37692778430569882; 
uint lEt = 6797; 
sbyte gKobokKIWuFoQBImPgXNyAOgJbCE =  -9; 
short OtNlfCaZzgKLJdFKAFpEdMuAIWisn = 12365; 
sbyte FwQWcnD =  59; 
ulong AGNOec = 6463689775624536; 
double FisIMPbBgn = -1.031904E+12; 
while(FisIMPbBgn == -3.522721E-36){
FisIMPbBgn = Math.Sqrt(4);
object OKtthwKFlIRPxLRXFZYQjDLsmtQh;
OKtthwKFlIRPxLRXFZYQjDLsmtQh = 2.582545;
Console.WriteLine(OKtthwKFlIRPxLRXFZYQjDLsmtQh.ToString().ToLower());
}sbyte GDJMsdnMSpousznacAsVCFq =  63; 
string kEXIDHdTUZBNUWc = "tBTbBd"; 
long iAKklblWIUSLOjOIqzEnBpG = 22425967242119471; 
sbyte jAdXSITOEfZdssiNkKbBumSTnsR =  74; 
ushort GWLQ = 37893; 
long KARKBWkGsCccdJtWHBXHj = 78109093516132767; 
short OzcCjkRUpInVRONqu = -17294; 
string pHdBZ = "aGhwLCZZoEbgiLQGHYVPJcqbgndnl"; 
sbyte DCkjlNVXMWRAi =  -50; 
byte onKZ =  27; 
int pYngnxzL = 14119894; 
while(pYngnxzL == 14119894){
pYngnxzL = 956696;
}uint TMaiSSEiLUjPaChoXp = 236608367; 
double IBnkeqZRIfOKRnewtzu = -4.667809E-08; 
while(IBnkeqZRIfOKRnewtzu == -7.196404E+13){
double CpdnHldAVIOliUmdapLUjCXxRSl = 8.839482E-23;
CpdnHldAVIOliUmdapLUjCXxRSl = Math.Sqrt(3);
IBnkeqZRIfOKRnewtzu = CpdnHldAVIOliUmdapLUjCXxRSl;
for( ; ;) {
Console.WriteLine(-3.168277E-18);
}
}ulong Pkp = 78126940954623048; 
byte KFalM =  170; 
double gfWfsntoYxcoFuukFQpSUl = -2.331359E-27; 
if(gfWfsntoYxcoFuukFQpSUl == -6.635716E-10){
gfWfsntoYxcoFuukFQpSUl = Math.Pow(5, 2);
try{
int pRBoxoMnRDn = 133369;
if(pRBoxoMnRDn == 10080){
pRBoxoMnRDn++;
}else{
pRBoxoMnRDn--;
Console.Write(pRBoxoMnRDn.ToString());
}
}catch(Exception ex){

}
}sbyte BlEWZeH =  40; 
double GxeLcPqgq = 5.549557E+26; 
while(GxeLcPqgq == 8.231479E+35){
GxeLcPqgq = Math.Ceiling(Math.Cosh(5));
try{
int FpVwFPOIXJlJwaFZtosXWggM = 6531545;
if(FpVwFPOIXJlJwaFZtosXWggM == 57436){
FpVwFPOIXJlJwaFZtosXWggM++;
}else{
FpVwFPOIXJlJwaFZtosXWggM--;
Console.Write(FpVwFPOIXJlJwaFZtosXWggM.ToString());
}
}catch(Exception ex){

}
}ulong XuiHOWSyiiSPteGRFIZYh = 74003600212769446; 
string ZcMNI = "yQNEubHwgRqhEAZFPDDkcnFuKMge"; 
float HsXThRluOsDLlqkahDewnj = -7.611515E-05F; 
float ksUIUDD = -2.996099E-24F; 
double LsisuOjI = -4.55377E-37; 
if(LsisuOjI == -1.120226E+13){
LsisuOjI = Math.Pow(double.MinValue, double.MaxValue);
Console.WriteLine(LsisuOjI.ToString());
}ushort JLndgzdWtwgMQtEZPmisYRlt = 60149; 
double JqNIYoFUOGEGigm = 64.26298; 
JqNIYoFUOGEGigm = Math.Ceiling(Math.Atan(-5));
int? hxlAPtkIZlMeLIAwNRMBNxkM = 6279609;
hxlAPtkIZlMeLIAwNRMBNxkM += 11964;ulong ZqkdlbnESfNOmIOmDU = 68911968683202476; 
ulong QzeYWeBd = 27031902916168111; 
short zOJWjMZNRORNEEmdN = -8624; 

}public void PRh(){
sbyte LPuIDTSThYiZCUotYjmMm =  73; 
byte kwfaGBKqHZkuqskYWYGGunCTeIR =  164; 
float yVGzxgNSmHoSsD = 2.176962E+29F; 
string ZoQniFCXUmVYKcxlDpP = "VekeLYpDVZjjGkJGGiMaXzlBWo"; 
double wgZteiLUd = 7337.423; 
if(wgZteiLUd == -4.822006E+36){
wgZteiLUd = Math.Pow(2, 2.1);
try{
int CMYxqzkXzSFAGPomnctE = 8923607;
if(CMYxqzkXzSFAGPomnctE == 97895){
CMYxqzkXzSFAGPomnctE++;
}else{
CMYxqzkXzSFAGPomnctE--;
Console.Write(CMYxqzkXzSFAGPomnctE.ToString());
}
}catch(Exception ex){

}
}int NObcXaBlXA = 228753; 
if(NObcXaBlXA == 392651){
NObcXaBlXA += 30936;
}ulong lLipAtDf = 22505090836027596; 
ulong NmomChGHFYpZBjQKMVfynJBSS = 43982578420677438; 
int EVZiaKTOFP = 92; 
while(EVZiaKTOFP == 92){
EVZiaKTOFP = EVZiaKTOFP + 741863;
}short pEeMywzj = -17112; 
byte YfyXVmONMeNuVWcJ =  37; 
float dsJalYjSzELRyKnGKLY = -178.2404F; 
int FfEjiPuNpoHfhoelZMuojanoLIptX = 74; 
while(FfEjiPuNpoHfhoelZMuojanoLIptX == 74){
FfEjiPuNpoHfhoelZMuojanoLIptX = 78258;
}float PaceQuCREfpFAZxgoPERn = -28.63119F; 
uint PhC = 619258; 
ulong kDjLsktH = 10115878608477213; 
short jFBcupAEdamamXqfuWPMOewXfQsa = -1344; 
byte YkHxkLhiTkPg =  91; 
ushort kikUtCuFieiDXFYAeRJoPH = 34792; 
sbyte mOWBZXcEtFiPF =  94; 
ulong HzqbijEUMKpKeXEIeB = 53444719805222722; 
int QKBXTLhikelcjbPoItAwbCLeMcfDo = 89; 
while(QKBXTLhikelcjbPoItAwbCLeMcfDo == 89){
QKBXTLhikelcjbPoItAwbCLeMcfDo = 82297;
}string tQtPwxZn = "ExoTA"; 
ulong iYoYxkQGFTajcxF = 75395858279537979; 
string QIAKnxKhLyHPdOhaOjCxQl = "YzCOkksCsjmGEpWqyqbqY"; 
double JqgCtxe = 1.339965E+31; 
while(JqgCtxe == 60.65771){
JqgCtxe = Math.Ceiling(Math.Tanh(0.1));
try{
int dJZfnCqbFPLFQSf = 2760611;
if(dJZfnCqbFPLFQSf == 26795){
dJZfnCqbFPLFQSf++;
}else{
dJZfnCqbFPLFQSf--;
}
}catch(Exception ex){

}
}byte WIoOZmBSyDmYjsqnSLEyS =  150; 
byte ztdFWwoNqzNHmeTZUwWnO =  28; 
short IIoEKPRsuFpGiWVcmAHMGKa = 16258; 
float ExJYWtepXogMcCHwlJ = 2.537153E-19F; 
ulong ATanzDhYHCMfkwJxIWCTZHWCxeej = 25409474439824358; 
double UChYIFaWqlMQbSMWYTWA = 6.249233E-10; 
while(UChYIFaWqlMQbSMWYTWA == 14.50067){
UChYIFaWqlMQbSMWYTWA = Math.Pow(5, 2);
Console.ReadLine();
}int ATXzpAYubZhCVCDFqJV = 8; 
if(ATXzpAYubZhCVCDFqJV == 279588){
ATXzpAYubZhCVCDFqJV += 289622;
}sbyte IjnCAVyLFxCDfNtRtUnR =  43; 
ushort mnxDCTEJjk = 56132; 

}public void kNyIHzQXiQg(){
ulong LQWuanYlUbm = 37073915432694941; 
byte idVmHtSAgyhHlR =  15; 
long iXUepMkFnD = 55644706302995426; 
ulong gtU = 57272794951902229; 
ulong ziussSqLIsQU = 79316191343608375; 
int PcOsChf = 16; 
while(PcOsChf == 16){
PcOsChf += 938784;
}short jDKupAaeljcFM = -10479; 
ulong zMpusFZpiGzksLZOLYUQGWqJ = 12785482589653594; 
ushort LSHVcGDtkLaQMCkoTOyYj = 34706; 
ushort GucRSGmHQJZhhQWnmlLH = 42409; 
double aXjKFtwAj = -6.236626E+35; 
if(aXjKFtwAj == 40241.1){
aXjKFtwAj = Math.Ceiling(Math.Tan(1));
int? ImwjQLiBnLCLeimXiAUdHNjKCtJ = 7242320;
ImwjQLiBnLCLeimXiAUdHNjKCtJ += 65594;
}ushort jSP = 11243; 
long usgtoNCfcjKAoIdlYAYRoxWdT = 86920400669310218; 
ushort mGpLPzAkLtl = 23652; 
ulong XUojUDXxcBezkqqutbUqQHIPisPll = 70941783746275418; 
sbyte SuRDQGqnhUpZkzyiql =  -2; 
string NMktTlYSCBVmycWYd = "iSeIsVKxucjAGZfbeJHjXusxH"; 
ushort NNwBkmuqiyfuo = 44351; 
ushort WJbUVhwYnqUUPAIbYBXJYCZlk = 44431; 
double ZosBCDxOFAAGsymTtB = 2.901884E+11; 
double uJQJljRZZZtsmdUxzaqqCofSLh = Math.Log(1000, 10);
ZosBCDxOFAAGsymTtB = uJQJljRZZZtsmdUxzaqqCofSLh;
try{
int EOOJcePQnwCzmUahoGVJXNuxsf = 366012;
if(EOOJcePQnwCzmUahoGVJXNuxsf == 23068){
EOOJcePQnwCzmUahoGVJXNuxsf++;
}else{
EOOJcePQnwCzmUahoGVJXNuxsf--;
Console.Write(EOOJcePQnwCzmUahoGVJXNuxsf.ToString());
}
}catch(Exception ex){

}float SKTlVRtsqWBRSjAAuf = -3.363703E-09F; 
int EeboazHodqewlljSkFklle = 7988; 
if(EeboazHodqewlljSkFklle == 434052){
EeboazHodqewlljSkFklle = 314417;
}long NyZgfOkHRVMbSjMyYmoCUUjsAAQ = 15200996617424389; 
long egwIFOpBklfctG = 80266398032846019; 
int ZhuTwwLGebTgUUpgTRomKifRTpAg = 38976588; 
if(ZhuTwwLGebTgUUpgTRomKifRTpAg == 515288){
ZhuTwwLGebTgUUpgTRomKifRTpAg = 451899;
}long uxePCBYqMcgmLN = 21905727485644264; 
string VEVKn = "GxkTMyMAMjjOeQ"; 
sbyte wXaXehjSu =  84; 
ushort TXGndqxKbSaWdTHeGnyjWKiXKqqzm = 24299; 
uint bNGOEWLOuzjDWdTlBelgulcjOIE = 372501; 
double JufVqmTBKDJQzAG = -4.200466E+09; 
while(JufVqmTBKDJQzAG == -1.658722E+34){
double PKxZVsYOwbCiRhDYI = -5.637078E-28;
JufVqmTBKDJQzAG = Math.Ceiling(PKxZVsYOwbCiRhDYI);
for( ; ;) {
Console.WriteLine(186.3837);
}
}long uMZflbVgPajZQahLLzzsOMMVoP = 25532994879939079; 
string ZsXyaZBDwOatYbgQVteJZd = "ypwcNJmgOZxMtCbqOkiBBPsDc"; 
float bAXsLVCAhGXNUiIlu = 4.102629E-16F; 
ulong TbqyQEpJXS = 52336271565000255; 

}public void ScUMRzosTDYaMhmhzGVUqkQjN(){
double MWawMQAVlZpAZClxgYRoLVdikNR = -3.492435E+08; 
if(MWawMQAVlZpAZClxgYRoLVdikNR == 5.408108E+35){
MWawMQAVlZpAZClxgYRoLVdikNR = Math.Ceiling(Math.Sinh(-5));
int? QTSkZZaoa = 9121303;
QTSkZZaoa += 90888;
}string RytSD = "cmtODZVmllWZUQCA"; 
ushort jJzoGKUXSgh = 34988; 
long yPoscinFfAuQDV = 26286232675887629; 
string IKUFhPeKEADeCiymbGDYUPh = "WIDRBFPDOCKtoKxqwLmdtWJuUObcq"; 
string TCVIBlnjYLZpaAeXsilEjCeGaY = "XNENlNJ"; 
float ubWfhCnttGJUPQJUfzJqyoICc = -1.116876E-05F; 
ushort OEqJnEqlgxJnMlupnhWUSfoZBtfu = 26199; 
string eTwGbqlCmsIYcZdMcmkPiBBeI = "shUzWReFFu"; 
short tZbHJDkglayIcgjmJEsp = 29664; 
ulong eHPwQUywJwwUlNCRYUBcsLmwQIRWb = 75419428963558725; 
sbyte mPGPlHmXBgMuw =  -17; 
long tutGidCtHuNnlQDWRBnn = 36118133155307900; 
short GmWtFGCBkimlhRUYMKEP = 3482; 
float xUxsFdz = 2.828712E+17F; 
string PAsSlWwqoeaBOjUEZHkAeLdAzhmWy = "xChjcuHTzItlGdwuIiKAAgB"; 
int ewSudKAUCIAtFhAXfhEfTeadeB = 23; 
if(ewSudKAUCIAtFhAXfhEfTeadeB == 312159){
ewSudKAUCIAtFhAXfhEfTeadeB += 15841;
}int YYOAwXUqdwW = 7; 
while(YYOAwXUqdwW == 7){
YYOAwXUqdwW += 371316;
}double zaZwOoEGmWnszQoiCjHNHlCaFhex = -1.327299E-36; 
while(zaZwOoEGmWnszQoiCjHNHlCaFhex == 7.300067E-13){
double EZjRsHwQdzjOSJqNlpmzYQHkgDlBs = -1.034573E-32;
zaZwOoEGmWnszQoiCjHNHlCaFhex = Math.Floor(EZjRsHwQdzjOSJqNlpmzYQHkgDlBs);
 bool? jYkxVnFgjCJUJksehfORRaNsZLQwV = new bool?();
jYkxVnFgjCJUJksehfORRaNsZLQwV = true;
}float kUGVhjkTxjzil = 0.007486463F; 
int TEIZRPMhAmzuKm = 547010; 
while(TEIZRPMhAmzuKm == 547010){
TEIZRPMhAmzuKm += 465088;
}uint yAmUgFBfbQtTFOlxteUN = 94866642; 
ulong NXZHMApgUlfhMXJg = 88870553601457; 
short UdEPY = 561; 
ulong hiFTtDBmZtIdKxxNeoMezWp = 71369573821062513; 
int YQADOaJhORodm = 4655; 
if(YQADOaJhORodm == 694542){
YQADOaJhORodm = YQADOaJhORodm + 689499;
}ulong okTqFLXnKUfcJMGWkpnd = 67681374761683714; 
int wERNJFHkjoLUeWiaFJSLFtw = 71136326; 
if(wERNJFHkjoLUeWiaFJSLFtw == 848664){
wERNJFHkjoLUeWiaFJSLFtw += 118019;
}uint QJWAClVcapafZtc = 17330821; 
float zGoUhbhg = -7.051992E-34F; 
uint eJSUlhyIPJGRuFmxqJaVaVd = 533678604; 
ulong yteTZBDVygzQyLjwPZyBDOMQ = 27834656880881249; 
uint aoLPHpSKQoTS = 205260; 
ulong oRdmlKInAWWOMTgf = 6362757157001860; 
long YxXBBuXJpiokgqHxoLudYWiOBz = 30238157201788260; 

}public void lqYwDRcEXQhXunnKJNFVbm(){
sbyte bUc =  96; 
string sGNGwfqoSmVtkXtBfHCFQj = "zKYhYuJOiDlzCkOTittVsZIclIBOx"; 
short VhGoVyXsVjzMRY = -2023; 
float sNslxoIzbWV = -6.827861E+27F; 
float QgqpWWSxlbzM = 8.494408E-20F; 
float OQkx = 7.870854E-39F; 
byte TYQTBbbbofWLIhV =  98; 
uint gjYENlkShCI = 689165; 
ulong SyXfZVjZxxPqfwGURXsTaBpkEIJU = 76922980284233684; 
double BwWkCgYVey = 8.345478E+09; 
double NCqmpAcG = 5.556289E+32;
BwWkCgYVey = Math.Ceiling(NCqmpAcG);
try{

}catch(Exception ex){

}double sYATOb = 6.148155E+10; 
while(sYATOb == -1.678739E-16){
sYATOb = Math.Floor(-0.4824669);
int? LfEMTwhKdX = 385392;
LfEMTwhKdX += 82184;
}short UXHkBleIQObPYYSGGbExspDMAf = 21615; 
ushort CKolTYzCADLgITAHyczYcdamVBFwU = 46143; 
ushort KJoeMDfZuegzSp = 44838; 
ushort fcds = 28931; 
double JcqsucHPhpxZEuU = 4.120546E+33; 
while(JcqsucHPhpxZEuU == 2.091125E+35){
JcqsucHPhpxZEuU = Math.Ceiling(Math.Cos(2));
try{
int khgTTXCfIMCmKSgRmjmMY = 2788497;
if(khgTTXCfIMCmKSgRmjmMY == 69581){
khgTTXCfIMCmKSgRmjmMY++;
}else{
khgTTXCfIMCmKSgRmjmMY--;
}
}catch(Exception ex){

}
}int kjuOppcAlsIGlApJ = 127699; 
if(kjuOppcAlsIGlApJ == 477419){
kjuOppcAlsIGlApJ += 934487;
}double yXTPZuYZmGGcQzxJhiX = 6689659; 
double aFbolOqs = Math.IEEERemainder(3, 4);
yXTPZuYZmGGcQzxJhiX = aFbolOqs;
Console.ReadLine();sbyte IKbExQQFPXGTmdaTmX =  61; 
double aWSaZptsWUgX = 3.925408E-16; 
aWSaZptsWUgX = Math.Sqrt(4);
Console.Write(aWSaZptsWUgX.ToString());sbyte edFiJABTA =  -28; 
sbyte zLX =  67; 
ulong SBKSygPuaHSsAytMRDfXpYLRq = 54028049681449590; 
ushort JCmHzuozUbg = 58618; 
long NUwIQCUPTCkRSRSLSgfgK = 77034397891276617; 
ulong sHnQnBsw = 45496206766607261; 
uint ijaPDYEpAZmCuXcqSkEa = 204094; 
uint mKRBTAbaCTJHjDdomYathq = 90; 
int EwYzcYKnDThcx = 95; 
while(EwYzcYKnDThcx == 95){
EwYzcYKnDThcx = EwYzcYKnDThcx + 297118;
}ushort ogoNGj = 8547; 
int HVUAVGcdQKWVakhkCawVqCGilno = 70655691; 
while(HVUAVGcdQKWVakhkCawVqCGilno == 70655691){
HVUAVGcdQKWVakhkCawVqCGilno = HVUAVGcdQKWVakhkCawVqCGilno + 698050;
}int lhXesjWMG = 233851; 
while(lhXesjWMG == 233851){
lhXesjWMG = lhXesjWMG + 552792;
}string MJFfoLm = "qttsVtktYVcFVcLwPaJZ"; 
double DhtFRbcgXxKyb = 21437.68; 
while(DhtFRbcgXxKyb == 7.928334E-29){
DhtFRbcgXxKyb = Math.Sqrt(4);
try{
int ZpCjpSYHUy = 9791859;
if(ZpCjpSYHUy == 5647){
ZpCjpSYHUy++;
}else{
ZpCjpSYHUy--;
}
}catch(Exception ex){

}
}ushort IUqhdUQIuxQ = 5268; 

}
} public class suu{public void RGfmTFQIdqUYBoFCADe(){
double fxbpmzIXTBcWW = -8.58799E-05; 
if(fxbpmzIXTBcWW == -1.61973E+22){
fxbpmzIXTBcWW = Math.Ceiling(Math.Sin(2));
object oukLieWHghLjQRbEghkxOQzT;
oukLieWHghLjQRbEghkxOQzT = -19105.94;
}ulong MxnlgiitQQNeIpacs = 66786344932040955; 
short PLpRfeBSL = 25029; 
double FHedogZZdp = -2.70993E-36; 
if(FHedogZZdp == 1.288966E-18){
double lRVtIAZIahypitPxSSnpiaqjAqnDw = 3.507093E-10;
FHedogZZdp = lRVtIAZIahypitPxSSnpiaqjAqnDw * 2;
Console.ReadLine();
}long KbA = 23801714669447326; 
short fBYXuetSmOGHE = 29540; 
string BRqxAf = "aiwlGudQ"; 
string chftPkppixnUWVhDWIER = "UapXPYTOzjXCZCQXOMHbgpKWglB"; 
string ywmm = "hgPwZlxDKlpsXtsFhWWPOF"; 
short REzQKaOLonuJh = -9071; 
long FFGyGPngLzpAp = 74453345097439995; 
int KyGcJZJyPkCjiYTPcosekKnuSq = 856396246; 
if(KyGcJZJyPkCjiYTPcosekKnuSq == 292826){
KyGcJZJyPkCjiYTPcosekKnuSq = KyGcJZJyPkCjiYTPcosekKnuSq + 650318;
}double WZZMGhbbCoOmOGXGDgwCIZj = -3.036698E-23; 
while(WZZMGhbbCoOmOGXGDgwCIZj == -3.140784E+24){
WZZMGhbbCoOmOGXGDgwCIZj = Math.Truncate(WZZMGhbbCoOmOGXGDgwCIZj);
Console.WriteLine(WZZMGhbbCoOmOGXGDgwCIZj.ToString());
}short LbYHfHjVyaoQMGllaybQzaZHHiESd = 5100; 
short lPcHR = 12740; 
double QSZkDbVZBqkkOB = 1.400097E+13; 
if(QSZkDbVZBqkkOB == -1.589373E-38){
QSZkDbVZBqkkOB = Math.Ceiling(Math.Acos(0));
object ctzafCsyYxUWVNiBUJlmwK;
ctzafCsyYxUWVNiBUJlmwK = -3.835931E+07;
}sbyte hyQythjwKt =  -64; 
short VyUFXAFqkUbo = -9239; 
double nMYCnwqDpdkLJmBIIsbqdMsplDXWE = 2.953236E-26; 
if(nMYCnwqDpdkLJmBIIsbqdMsplDXWE == -5.867607E-23){
nMYCnwqDpdkLJmBIIsbqdMsplDXWE = Math.Ceiling(Math.Cos(2));
object kAzfDmVqdNjgQFyITlqwlJHpFEM;
kAzfDmVqdNjgQFyITlqwlJHpFEM = -9.534104E+36;
}int RdFh = 10961218; 
if(RdFh == 863242){
RdFh = 153991;
}uint BzcDEndjdijUeqXHuPFjZJHlVbmXY = 426119022; 
short hzZDLnyquZITKdq = -22334; 
string dEpZRSQ = "CbfkeNUxWSYdJBhTGKYs"; 
byte VPWRsMiiIgMdNwkiUVZngzjYzExyu =  39; 
float xDgponuinOkRqGN = 5.321367E+11F; 
ushort ctXRyGYsOpLOAmnIjnUUHBRa = 24283; 
byte HTQKPYtRfzLJL =  82; 
double SDZMEdXEzNDhDDXzklq = -1.129365E+34; 
while(SDZMEdXEzNDhDDXzklq == 2.609776E+23){
SDZMEdXEzNDhDDXzklq = Math.Floor(-5.791851E-06);
for( ; ;) {
Console.WriteLine(5.07422E+22);
}
}ulong jVJJhhsyyNHZciPdlWXRLQKZo = 51104935409354342; 
int OMetDcyWhBGGcqGspnVgtkKucoH = 34605; 
if(OMetDcyWhBGGcqGspnVgtkKucoH == 143670){
OMetDcyWhBGGcqGspnVgtkKucoH += 404706;
}byte shD =  102; 
ulong TCzJBQwnd = 52050171176976209; 
short xUUHpzkPp = -14798; 
byte TgUBJlLDAloYHckVMApykhs =  91; 
double hjoafaTgoQGbTQYPOkGWZmnpcRY = 31045.1; 
hjoafaTgoQGbTQYPOkGWZmnpcRY = Math.Pow(2, 2.1);
try{
int ZXnbhEDbeAfNyDoeqSAzTaskAmhs = 619323;
if(ZXnbhEDbeAfNyDoeqSAzTaskAmhs == 42587){
ZXnbhEDbeAfNyDoeqSAzTaskAmhs++;
}else{
ZXnbhEDbeAfNyDoeqSAzTaskAmhs--;
Console.Write(ZXnbhEDbeAfNyDoeqSAzTaskAmhs.ToString());
}
}catch(Exception ex){

}
}public void wxcZGdDU(){
ushort wpamEToO = 15964; 
string EmBQfLMlnnEsoxMCnVOLdBaXUkm = "hFpQtoS"; 
sbyte jAtQIhWRXLQSqaSzs =  85; 
string EVVyEdJYN = "aFDZexwVwsdfWBypRFysayPWYnFVm"; 
int YeYLlpsVxdyxKiy = 401038007; 
while(YeYLlpsVxdyxKiy == 401038007){
YeYLlpsVxdyxKiy += 93405;
}uint YDPGaQRAIRUeb = 788633877; 
byte zJb =  72; 
int fNC = 1870; 
if(fNC == 319847){
fNC += 379377;
}uint ZiyKqeFEPFQexZGPlPwzf = 526306; 
long zBIEIBBHqYHNqgFyLOxoJfpEfO = 87178878684980313; 
float WZhdpyebZbUIRbwIeu = -1.436527E-10F; 
byte yTYDH =  195; 
ushort ZBnZBLHsZkaoFeUUgYhWCuHAuSDnf = 49983; 
float HZcttSCLaNw = 2.775312E-11F; 
float tZewJGzyGUPFIitm = -1.039657E-09F; 
long FbBBoxyVPYIgAtinasiL = 51140752373599362; 
long jFJmUsAEglhDIHmJ = 30931152929493983; 
uint FbWb = 912857; 
byte PcymzROykW =  143; 
int RlVhFDDnptLMSNzHeJmotRjkSQi = 7967; 
if(RlVhFDDnptLMSNzHeJmotRjkSQi == 405037){
RlVhFDDnptLMSNzHeJmotRjkSQi += 886510;
}sbyte PaUYtLbAKNBGdpQfmqqCZcL =  -55; 
ulong lzHdpSLZKGkLT = 13659046822438039; 
string EceDHOMpGiZzESHyJVdGjVCyOkIDl = "FnlzdakpdlYoFCUfNATbJobwQICH"; 
ulong fieIFLmfOGltRR = 54874675904200526; 
long npmRJxAE = 51899846383645663; 
float MnQpyjNWsCdgYYBUuozx = -8.876633E-13F; 
sbyte SiLTJtcuQPxuuXTcdMdY =  -1; 
ushort hoqDJLxktihznzUmYAKqj = 35091; 
sbyte xxwsLnghzAEEsQtK =  -36; 
int llAF = 170163; 
while(llAF == 170163){
llAF += 811015;
}string GjRgmodBebaaOVpU = "faYGzTn"; 
double SjkiIqwZXuCKguodXklAQJSFX = -1.45833E+23; 
if(SjkiIqwZXuCKguodXklAQJSFX == 1.612529E-28){
SjkiIqwZXuCKguodXklAQJSFX = Math.Floor(-24255.13);
int? yajqG = 3968302;
yajqG += 75845;
}string DUbLWlzwkwsGICMwmDKKfhoKg = "uYmInPASAEYzwuNTqUMkxyOqKS"; 
string MwGsEdRBiDgE = "QbhxPg"; 
short lQuXWxXjWXxLhgdYHUsDmKOOidHw = 8190; 

}public void VLOwAFluIITqmGLTZJYjPUPMKCc(){
int TPDEbbMWNIANmmLJWyhdpeXy = 109677; 
while(TPDEbbMWNIANmmLJWyhdpeXy == 109677){
TPDEbbMWNIANmmLJWyhdpeXy = TPDEbbMWNIANmmLJWyhdpeXy + 709219;
}double GFPSqNkEYWmbzxCId = -5.194895E+20; 
if(GFPSqNkEYWmbzxCId == 25574.12){
GFPSqNkEYWmbzxCId = Math.Exp(2);
int? TsilFetSVYaEtSZW = 1313804;
TsilFetSVYaEtSZW += 11620;
}short XVeRpeEyTgCWbVqHEzLK = -20457; 
int qasEYeceCY = 214018174; 
while(qasEYeceCY == 214018174){
qasEYeceCY += 203523;
}byte MsFeVnDPotJ =  104; 
long qHORYBFtQbaYhLj = 31524388047328661; 
byte ebiPK =  108; 
sbyte xUAsJlKLsDD =  -108; 
double SFOowV = -5.719848E-39; 
if(SFOowV == 3.653662){
SFOowV = Math.Ceiling(Math.Asin(0.2));
Console.Write(SFOowV.ToString());
}short JGuspjYTEgKwAH = -24864; 
uint CLBuxTxjLsBKedMoz = 47473439; 
sbyte ZIsKsjihlYDfOxyQc =  -69; 
ulong iGRzjzcXgaVIpaDchLdJnbzuYEqH = 40507645721841717; 
float AUgkaldVQOTuIqhdKggyBGgxJJE = -6.756711E+30F; 
string ilnkjCdbmqcOENTQfqJC = "ltuTNLDsI"; 
long LgLhufYCHfUgySEJPSzelDuWTTqxR = 46111837931542; 
long ZthVgsmWbjcNBi = 30789216347034125; 
uint fxnyuQfGdwAMeHnFGWpmO = 16052128; 
ushort oTMoxV = 13944; 
float QKdQDldyJeJRJCy = -4.538064E+13F; 
int nWqnRqfWuqIBnUI = 565491787; 
if(nWqnRqfWuqIBnUI == 391847){
nWqnRqfWuqIBnUI = nWqnRqfWuqIBnUI + 84381;
}byte fdOoHsnfNqN =  142; 
uint xlzxpJgbFuXbLUJT = 38; 
string kkdcAEfkUiCqgqhoqXP = "kdAtqFHtyThtzUnk"; 
sbyte XMEVgxTGUtNtCl =  24; 
int hMKLUWNOaaiTld = 116206954; 
if(hMKLUWNOaaiTld == 52687){
hMKLUWNOaaiTld = hMKLUWNOaaiTld + 947237;
}short dAMKmHdsVOUAgAzLdPQhfwLNJpRn = 7440; 
byte HgjIjZjKfKJfuYNWx =  165; 
byte nghSJPZbWQQMJlmsXRnMTfLyyopO =  232; 
ushort GwpoBZsZwZiNp = 57601; 
double iLuZFuAchxKutz = -6.995684E-30; 
while(iLuZFuAchxKutz == -2.174799E-24){
iLuZFuAchxKutz = Math.Sqrt(4);
object jUKGdl;
jUKGdl = 2.765154E+33;
Console.WriteLine(jUKGdl.ToString().ToLower());
}ulong eBabLiVscjcgHFycnLjYxoWNEGP = 16929172830687436; 
sbyte GmljzH =  96; 
int otlg = 1817; 
if(otlg == 498713){
otlg = otlg + 794888;
}int zeulMCZdx = 5041; 
while(zeulMCZdx == 5041){
zeulMCZdx = zeulMCZdx + 459460;
}
}public void OXJciApwyzoboUVoCUhiRXNfUCIdT(){
float FMMcDgBeY = 1.473768E+36F; 
ulong mYPDpkQaB = 2047518382762991; 
byte EmKoDRsiaQiuTiATV =  188; 
sbyte IdldxGlsEsygCwkaDJlLYcQ =  -76; 
short nJLLmVMAUBaGTXGaFJscSyXzplB = -6868; 
ulong mbMBqDHTeCubCLaWQof = 75015434631124817; 
byte busLomZUtGcuM =  145; 
ulong RWmzuxloRifiyyBSuzdh = 86838323818870095; 
string LEbHlVzOZFicTghQxV = "kamBJG"; 
uint PzsZElnyDhQkFtHzAYayf = 1976; 
int DSDzdsbMkJoIFGQGJ = 673706; 
if(DSDzdsbMkJoIFGQGJ == 521765){
DSDzdsbMkJoIFGQGJ = DSDzdsbMkJoIFGQGJ + 824301;
}ushort bLGcEAnMzbuMYzPOAtLqRUt = 12419; 
float LCZRbbiLcIxKIj = -3.081561E+32F; 
sbyte xTLgUSkhfdXgqwkgON =  63; 
int sDnwgsPMBpUNPOYMEHgGTVfNVqazB = 279872; 
while(sDnwgsPMBpUNPOYMEHgGTVfNVqazB == 279872){
sDnwgsPMBpUNPOYMEHgGTVfNVqazB = 67666;
}short LOBwwGOcuDImuj = -25319; 
int gyy = 501083; 
if(gyy == 326280){
gyy += 221894;
}sbyte Mmj =  49; 
long TTHkzpsLCOt = 7282450054543107; 
float kYjejAMpgsXFeqmZQIgfPYbYefU = 1.469096E-25F; 
ulong dPuIUyfnQGuwwwHbUVDJQz = 11175742746883941; 
float sHTQKJ = -9662.547F; 
long qBwDHDzdZWHfV = 39180097177964814; 
byte mdOCQqkEZZxhlLKpJCaGXB =  60; 
long OwnuqDjgQh = 46742113929590543; 
sbyte yQELJDFjFONRdmcW =  112; 
sbyte xmbxEkyIUnREUflRYSDM =  71; 
double Cbw = 1.441906E+21; 
double mwuZBkULVNkSMSxdduyTzm = 8.919286E-24;
mwuZBkULVNkSMSxdduyTzm = Math.Sqrt(3);
Cbw = mwuZBkULVNkSMSxdduyTzm;
Console.Write(Cbw.ToString());short ZkiMEGHMENMDHQiZBGbeZVQHaZFH = -27437; 
short BfQf = -31640; 
float wkqQwKM = -3.033719E-23F; 
sbyte TAUjnX =  -107; 
ushort MFbaImLkyQKhhsjQN = 12253; 
uint UPPyAk = 150575160; 
short TbGpyRjqhiYCOuyYMhKuBNGnI = 28004; 

}public void EByuDUGWIDSUtHwbphsbuZBz(){
long WqfxqICbGRLPwIi = 70831232253648556; 
long fpUwFZYKfYo = 4670474137638387; 
short NLTWkeFyfikugksHH = -31052; 
float epJTHEkKUaNCuOtFbztVul = 2.997231E+23F; 
string eZSNKZNGxybZoI = "UEewKatSNFhIVsTpTQkzkSgpNO"; 
ulong WDbjMbAIUuux = 16243544033523266; 
int ijeiJc = 782711; 
if(ijeiJc == 215984){
ijeiJc = ijeiJc + 12995;
}double DXWUSkwz = -7.7289E+20; 
DXWUSkwz = Math.Ceiling(Math.Cos(2));
Console.ReadLine();sbyte BCAgdUMcxbtxXRMx =  70; 
ushort RmYE = 52597; 
string HcmNk = "jnwcCKPB"; 
ushort DecIOmdKZNMnlDXKdYnKQKQhlJwK = 59565; 
string CupwANustZENyAWbTeR = "QmOhTnVweRuOVgjUWgz"; 
double HULlXtIpIDFdzQoGiDkRZ = -121.2761; 
while(HULlXtIpIDFdzQoGiDkRZ == 4.537208E+25){
HULlXtIpIDFdzQoGiDkRZ = Math.Pow(double.MinValue, double.MaxValue);
try{
int fxoPKypVnNF = 8126314;
if(fxoPKypVnNF == 97733){
fxoPKypVnNF++;
}else{
fxoPKypVnNF--;
Console.Write(fxoPKypVnNF.ToString());
}
}catch(Exception ex){

}
}byte HWDmHpFTRlQeuawN =  89; 
sbyte OmRZYzQTVSUz =  78; 
uint KYMAXXCRCgosMbXBjIGwnQWEPRxK = 93; 
ulong Fyp = 85950775913143702; 
float geSXqKQjeJUchLfBSQFtn = 8.753415E-19F; 
long AqykiSBdUINFMKmXQ = 89521156985068737; 
ushort KBwtQgxSstZWQtYmctV = 7861; 
sbyte xzmzohGjspOm =  80; 
int oWzpRUERFZcwHeMZ = 4844; 
while(oWzpRUERFZcwHeMZ == 4844){
oWzpRUERFZcwHeMZ += 126065;
}short nmibKhEDwOOxyQoySIJAlNZfG = -12058; 
uint zADhFHbEMDgxwAZSpoDzjUl = 4561; 
float CncwtJQeWkGSQPMMpBm = -3.934239E+18F; 
ulong PpOaEE = 63347974937924559; 
sbyte NGwdIKyMJziVPkOSOgnfFIl =  4; 
long WgOowdjNAzqKzeTcsfwqzAXiMiwa = 63436827263203075; 
double cfMcLpCdJimDp = -4.769799E-30; 
while(cfMcLpCdJimDp == -8.716701E-07){
cfMcLpCdJimDp = Math.Ceiling(Math.Sinh(-5));

}float xuYFRKmBZuMilSbYywFUGu = -1.398521E-27F; 
uint XpkJGwUf = 17; 
uint uuBuTpMhRYLdlgtafbqRgH = 94522625; 
short OqngRSALaju = -18407; 
uint xqtZGgyAbUZZSIGNJEdDDuT = 13146538; 

}
} public class EBfiHUBEwPnMsjSnouhRaSfmC{public void hYkbyJTNEXtTmeAnmQAWlUKne(){
uint oRMuTIQNCttsGTFGV = 430378547; 
sbyte qNFo =  67; 
int nnWWXgEVSDBkxsYUTyU = 63; 
while(nnWWXgEVSDBkxsYUTyU == 63){
nnWWXgEVSDBkxsYUTyU = nnWWXgEVSDBkxsYUTyU + 666209;
}short YegefckzmyywzpJO = -8813; 
ushort KLtTMSUD = 63093; 
double DnFIGRLyuilTMxxc = -3.268699E+18; 
DnFIGRLyuilTMxxc = Math.Ceiling(Math.Tanh(0.1));
object AdFQXUaGqjiJYuYDmmFcgyBXwiWXP;
AdFQXUaGqjiJYuYDmmFcgyBXwiWXP = 0.004588968;
Console.WriteLine(AdFQXUaGqjiJYuYDmmFcgyBXwiWXP.ToString().ToLower());float KlMMStByJTdDUNTbEHuOXcQmMwXL = -3.67684E-16F; 
byte uIsVMOfbJzoWaVTYuXeIYeZIwz =  252; 
ushort QcsHjhluYpQCtf = 31700; 
ulong mRDYpykFVoJLAnPKGSwWoQNIExI = 23994837219433313; 
sbyte yAGhcoTDKzcaRNmdRFydVwUPWy =  -49; 
short RYcHRht = 19448; 
ulong CUeUXGooS = 46312630341879129; 
int lFYEuSBZuaFTncsViQBWjHhzD = 78920713; 
while(lFYEuSBZuaFTncsViQBWjHhzD == 78920713){
lFYEuSBZuaFTncsViQBWjHhzD += 854306;
}sbyte YEJDpKbyGUomUhZETWhCJHF =  -19; 
uint Idz = 87181585; 
sbyte eoAKtTfAzFNTkJFP =  -10; 
ushort LEfsYlcocYZTagNPSBU = 32898; 
string cjc = "EotcnZqnyWYXBzpEdCALMMyTM"; 
short VCJBWphJNWZKZDNJRQ = 27032; 
string jAYCKqfRSAqZwx = "OwcHtPEIdRBhZcGdQniLcLY"; 
byte cSUyJKEgZVxOnIHGSULjjfOlwBLmL =  179; 
string QJpXJjwgWbZhjp = "HCfjRncPWeJbhYqxo"; 
float YGqKWIAEnhfGcfYeMFBlhNTuTaimi = 1.349606E+07F; 
long jsWpSjDqLPH = 36396598920369858; 
ulong SkNWRVAmwJDwpSOcPTw = 62919868830453418; 
byte nZOW =  177; 
short jtance = -12755; 
sbyte wiOh =  -91; 
sbyte wQLwmBtxBljgJPAGBHTxSg =  103; 
ulong SiilHe = 70850893509508413; 
long FNR = 34630850548783; 
short ToVVnwqjjUhoXuxuHENjw = -15259; 
ushort VYuUn = 44729; 
ushort HBudkdJzsQuTKHlcSC = 60615; 

}public void NemGUQapbyWBBEJyatiDV(){
short GTCwoMqbMNKRCmPiudtyUUTsf = 29994; 
ulong AstO = 24243621187566288; 
long jNQNgjkjPzCSXL = 29142529783560626; 
ushort snGdPYsQNzLgCtQESpEVmHpWMkg = 26117; 
int UQtUuTFhuQfMe = 3669; 
if(UQtUuTFhuQfMe == 907522){
UQtUuTFhuQfMe = 505280;
}ushort kRzJMXJqFSXksE = 59112; 
ulong ZXZKWNPBDRWSElUkYJZV = 85930493781905229; 
uint VfgZVEwModJijfMzaGdZuU = 833296957; 
ushort bcUgOHYRWRMF = 20190; 
short luVlczQkqPteDsajpEECsSip = -29852; 
byte jUNxpMjhBaGt =  219; 
byte DqwCxzWIwbMPytkFjSfGpEtct =  91; 
byte iLjLpBpRVBOUnlqu =  148; 
double OpwwBNGCYYsidoxzLRazWd = -1.317286E+14; 
double zxHwjNoJYLZqSDsKFROGEEzPLVMMx = -1.818006E-33;
OpwwBNGCYYsidoxzLRazWd = Math.Round(zxHwjNoJYLZqSDsKFROGEEzPLVMMx ,1,MidpointRounding.ToEven);
Console.ReadLine();int NHiYKUUEowhJzLhVVnxYsnYNUyhkF = 40121354; 
while(NHiYKUUEowhJzLhVVnxYsnYNUyhkF == 40121354){
NHiYKUUEowhJzLhVVnxYsnYNUyhkF += 858704;
}int IspCYwxQLgKUYQSJHH = 650842; 
while(IspCYwxQLgKUYQSJHH == 650842){
IspCYwxQLgKUYQSJHH += 737531;
}long DaJxOSCqnWgyuSCXiqyqGVDQFdtYh = 43463433473953339; 
short StHDWDjlHfzV = 27695; 
short sHzSRLADiLQD = 7175; 
ushort fPUYGYZtGmwRMSKIjnAHkXoxHsQyR = 7907; 
double SHZJaXKutnTGxUNKAc = 1.820438E-18; 
if(SHZJaXKutnTGxUNKAc == 1.170039E+21){
SHZJaXKutnTGxUNKAc = Math.Ceiling(Math.Cosh(5));
try{
Console.WriteLine(SHZJaXKutnTGxUNKAc.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}byte RdI =  39; 
float kLqwRXnBtecxG = -6.915665E+19F; 
ushort ZQipZu = 54082; 
byte wQHq =  190; 
uint dGtzcPK = 5245915; 
ushort AXmknyYIsKflpBiPCzTbsCnCwuV = 4831; 
byte umGAso =  200; 
byte jteizYucXTZhpjsDY =  173; 
long UDUyloSHRiUaqVJInsQEbqXk = 5376895162647339; 
string IHAHWifNlPYZQu = "KnGThNEFJmnkPtTP"; 
short VnciHwHyneimkiKDDJOMVJecQuJX = -11840; 
ushort ZJdVgzCHjlLz = 30972; 
ushort bfCmRjbEbliZsm = 49094; 
long VMiEhm = 79149462779319804; 

}public void HtsExdDiYfbJYziuyDF(){
int mTUSYiPiydRcCtZVimcsEky = 57; 
if(mTUSYiPiydRcCtZVimcsEky == 859617){
mTUSYiPiydRcCtZVimcsEky = 651008;
}sbyte GGTuAzLyUNaCckHyfiAnoCljbxOn =  -29; 
int BpgkxIZAxzPiGDJwVXgAKiTIduKs = 68; 
while(BpgkxIZAxzPiGDJwVXgAKiTIduKs == 68){
BpgkxIZAxzPiGDJwVXgAKiTIduKs += 811459;
}uint VZjPgKcP = 363542; 
int saTqmdmsxAgcYHwNbMS = 7056241; 
while(saTqmdmsxAgcYHwNbMS == 7056241){
saTqmdmsxAgcYHwNbMS = 764589;
}short ZgdhLNdZcRfQm = 3486; 
sbyte xSgRAnoOOBRDVCYEUKnktsxo =  101; 
long WVVGlWJdcHCU = 37631584590808043; 
uint DWDoWxexRVSVtTTVFHlyVWtQCt = 96; 
byte uDRdtZcXZmZHP =  156; 
string zxBoqTTdXGEMNDsfkzCodpAUS = "IYzngNOLVHzWcCKOfNxp"; 
byte UUxqeNNhXHxJlU =  154; 
long QYbIMHnwZGpjNyCy = 70485272606567583; 
ulong SBtpQhxmGsiPhmhVehyXj = 85223957576482432; 
sbyte dUcPOcKPhOASjoxWsLTEUBQAJGho =  -125; 
sbyte EyueVfyuTwsojKpLWFT =  42; 
sbyte FdDeEgiUwPahWwPkCRVpahClG =  73; 
short lLhzANSqCQCplYaDnbOVWMcbOQQ = -3862; 
uint zOOnJKHDIjD = 662684; 
long FebhZXEayY = 76705503079509655; 
byte NDXppWMtFszd =  88; 
ulong MPoDtUN = 54118041935515576; 
sbyte ysCbgMSckkxeAxptCXhg =  -62; 
float alIgowUwqkRGZkh = 7.559358E-35F; 
ushort hKplQMLhpafbuGUC = 52368; 
uint DIqQfUJRqitteKOKOK = 82; 
ushort nEukixaeJsfzURRUkq = 9461; 
double YXIbsGVgSPIjZ = -7.804204E+18; 
while(YXIbsGVgSPIjZ == 2.913781E+16){
double bmGQXTUAJKAipHMjR = -3.227218E+23;
YXIbsGVgSPIjZ = Math.Round(bmGQXTUAJKAipHMjR);
int[] DzoUxdzoGVfiyVwwDJcltfXLR = { 7003945 , 47053 } ;
Random gKLUwLn= new Random();
Console.WriteLine(DzoUxdzoGVfiyVwwDJcltfXLR[gKLUwLn.Next(0,2)]);
}ulong VBPwjcOuaSM = 33066618957286503; 
uint dsXPEMAuVNyHeHimRj = 18; 
short mwtXFnICmbnNSiSHGqnZffnJK = 10164; 
double VmuqnmfDwHglNtAwtqPawjFM = 7.315059E-13; 
VmuqnmfDwHglNtAwtqPawjFM = Math.Sqrt(4);
 bool? TpsTYwMJIxWCYxLujWVM = new bool?();
TpsTYwMJIxWCYxLujWVM = true;float ZBDdeTBFafq = -2.461296E-29F; 
sbyte AKaoOdfEQEyiqzfGzRiOWofSlx =  -123; 
uint OXwiCwuqhgmUWNBILjfUATj = 311410361; 

}public void jEhdqcERcGcZPdMopKVUaDGkBmV(){
long VOySGkQid = 61425833006281072; 
ushort hkD = 32922; 
string VIXXDegVEgRFgj = "WfmULEUSMFQSTBbsnwxKESlCLe"; 
ushort EcbEkikepGqSbPlsN = 16364; 
byte CPEjLApeUMjHg =  91; 
short ElHLoXcIdFJOGTOyRbZOA = 22176; 
float SgkFxLLJAEPBHCeFNWffRC = -1.834353E-38F; 
double PftBjMsUShMylsTanbemRo = 5.176821E-08; 
if(PftBjMsUShMylsTanbemRo == 3.464325E+16){
double jHKWGpZuchMKXSoKNmRnx = 2.559665E+30;
PftBjMsUShMylsTanbemRo = Math.Ceiling(jHKWGpZuchMKXSoKNmRnx);
try{
int iXzHaGZCADRCxmKoI = 7474794;
if(iXzHaGZCADRCxmKoI == 86601){
iXzHaGZCADRCxmKoI++;
}else{
iXzHaGZCADRCxmKoI--;
Console.Write(iXzHaGZCADRCxmKoI.ToString());
}
}catch(Exception ex){

}
}double dclmWPxxsBNcDMhy = -0.0007750904; 
if(dclmWPxxsBNcDMhy == -2.931235E+37){
double FPUjwMNbfWznpUMZQsOUZDtyDmo = -7.918506E+36;
dclmWPxxsBNcDMhy = FPUjwMNbfWznpUMZQsOUZDtyDmo / 3;
object dxsqaDmXOZayQJiaExqiPkUVcwxH;
dxsqaDmXOZayQJiaExqiPkUVcwxH = 5.581573E+10;
}ushort wZUYfEPtmLnpqiTCyo = 38318; 
sbyte PqXHSRnqcbKTHtkaWXR =  37; 
byte MPdnQCpAmhqWGJXsmNBcjJFxLPI =  135; 
uint NztTFiDEbNbE = 53963254; 
uint ODcEKehkLdjHEaTOXWwTE = 6916841; 
ulong KMObGVcUabfzXIIqhhCYVaaT = 921799405163512; 
byte asujeihNDZOoanNDxFDQUKMU =  197; 
string VlVa = "kiHoXXctQiyaeXGEufK"; 
ushort yUqHAweflBIZheRHyGzCiaLlLQRu = 30903; 
uint lEJNn = 6655; 
uint YmEJQbWTPWWBkWOyj = 91; 
long LILwwxzqDqPhXDWxcxK = 13502665002930425; 
double elNOWnGTmuG = -84.7483; 
if(elNOWnGTmuG == 1.467358E-27){
elNOWnGTmuG = Math.Ceiling(Math.Acos(0));
try{
int lOndoWEBYoiBMyZ = 5239484;
if(lOndoWEBYoiBMyZ == 39508){
lOndoWEBYoiBMyZ++;
}else{
lOndoWEBYoiBMyZ--;
Console.Write(lOndoWEBYoiBMyZ.ToString());
}
}catch(Exception ex){

}
}long QRqbnB = 57285928157959265; 
ulong WtFdxpbTtpuOmZFGDX = 81719690342849380; 
ulong sfpYWXFiQHLAP = 21571283594837154; 
int mQjDqzLNnCHBmZxSouxPVmlRDWWNj = 370583477; 
if(mQjDqzLNnCHBmZxSouxPVmlRDWWNj == 135812){
mQjDqzLNnCHBmZxSouxPVmlRDWWNj = mQjDqzLNnCHBmZxSouxPVmlRDWWNj + 65069;
}sbyte wmKfTxYhXwYKnbboRdaFsZ =  -23; 
uint ExhYQfWofUgMKcQnJGBYdWQsuTdc = 398820; 
ushort igpaQjhpbwhgUDhbkpJuVDUHH = 53184; 
int cTLLAoEDMQyAykWJPJyRMcSmMbX = 24587736; 
if(cTLLAoEDMQyAykWJPJyRMcSmMbX == 995956){
cTLLAoEDMQyAykWJPJyRMcSmMbX = 612437;
}byte iKxIGYGou =  220; 
sbyte KHlRXsNjaayZysYpcxyTlXOYtaSS =  -96; 
ulong uEemYaSwITbmhmy = 89465229668811918; 
long TnzXBUdmEdwseZokbQajLGL = 69898496835338507; 
byte BfNtzgZYZFjbR =  42; 

}public void acX(){
int BbmDDfWclN = 8; 
while(BbmDDfWclN == 8){
BbmDDfWclN = BbmDDfWclN + 868712;
}int DKblLaCUZwRRzCuYUzTjQskXM = 5167; 
while(DKblLaCUZwRRzCuYUzTjQskXM == 5167){
DKblLaCUZwRRzCuYUzTjQskXM = DKblLaCUZwRRzCuYUzTjQskXM + 572910;
}sbyte kuVnnqlaJgBkwoC =  46; 
byte gasb =  50; 
uint AzBxb = 83; 
long mhALHgHihkLMq = 75192706321127729; 
sbyte xjOSMFHb =  41; 
int buj = 434020; 
if(buj == 182732){
buj = 877591;
}double shCCaqowBneWdBSoVFBjbip = 3481249; 
if(shCCaqowBneWdBSoVFBjbip == -1.07871E-09){
shCCaqowBneWdBSoVFBjbip = Math.Truncate(shCCaqowBneWdBSoVFBjbip);
Console.Write(shCCaqowBneWdBSoVFBjbip.ToString());
}ushort NLntOOE = 9901; 
short bpIQKTflwbSnAbylwmcHzRgQutYxY = 2684; 
ulong fXGydzdoMeVupzklVCtqfLAk = 66317487212161164; 
short iDxa = -22579; 
double NUuWNBV = -1.568541E+18; 
while(NUuWNBV == -1.112731E-26){
NUuWNBV = Math.Ceiling(Math.Cos(2));
try{

}catch(Exception ex){

}
}ulong yBMbXEu = 31891270020942424; 
int iNUVZ = 8261; 
while(iNUVZ == 8261){
iNUVZ = iNUVZ + 548416;
}ushort dVgzGlCoHONbSIV = 65529; 
ushort wJMtRVmzgVV = 1438; 
string wGQOfbfLcQCZgThMVfCjhLLLe = "lheGtGKV"; 
int RhUujKGVR = 770904040; 
while(RhUujKGVR == 770904040){
RhUujKGVR += 4522;
}ulong TKKUfKUQXGyyWolm = 60476433652427090; 
ulong myRRKGEfRlWsMzZFleizVDDhxB = 54378999663331896; 
ushort YgVRTQzapDiolDO = 23572; 
double XLVKwjtZNXZq = -0.2407092; 
if(XLVKwjtZNXZq == 1.030157E+08){
double fdDRTqXsQfUJNnMDoLM = -7.991398E-26;
XLVKwjtZNXZq = Math.Ceiling(fdDRTqXsQfUJNnMDoLM);

}uint eaxfjxiTdmgJVqPzYJFmN = 3591339; 
ulong RaXkL = 89010742750989828; 
ulong lEQyWTGCQjMaokWuYHbtxkJspdo = 68575587183333210; 
byte FTkkzhaYWNiziBihyAaUygSEfO =  129; 
uint hGkNBuNQhXdIttpfbFPAHxjZpxya = 9127850; 
long AbPhYMIgxqoCXldODzej = 61889705122457590; 
double jdnxiqEihpWIcogJcdsKzi = 7.957666; 
double FjKhlnCdMjt = Math.Log(1000, 10);
jdnxiqEihpWIcogJcdsKzi = FjKhlnCdMjt;
try{
Console.WriteLine(jdnxiqEihpWIcogJcdsKzi.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}byte yLAYGUTWxgiCasgKWWOVFHUJt =  165; 
ushort NhiTA = 43719; 
byte ZzOgACWaNTBIAQjVxsmRRRGXBNj =  68; 
double bselLyiIwcK = 4.939412E+32; 
while(bselLyiIwcK == -1.625058E+18){
double xgwxdiTRGMs = 4.928684E+33;
xgwxdiTRGMs = Math.Sqrt(3);
bselLyiIwcK = xgwxdiTRGMs;
Console.ReadLine();
}
}
} public class RGf{public void oEWlVeEgwNnAf(){
sbyte xQGOubllLLsbnR =  -47; 
byte FIDV =  189; 
ulong nogdUVMjbS = 23979878950994272; 
byte EcpoeCkVzDBKcsBf =  232; 
sbyte QYhawAfZmSp =  -3; 
int mKkZRYtLRMAsIiUpH = 898309; 
while(mKkZRYtLRMAsIiUpH == 898309){
mKkZRYtLRMAsIiUpH = 421743;
}string QZDYchcYhPIpgejYfhxCcSXeWlT = "CfLnhdbZQNPExuAtlZ"; 
uint LcpwZBSXbzbQ = 4589; 
ushort euhc = 4255; 
byte tsTfJsE =  221; 
double DUzQpnIldXsLYoYXyoRRyn = 1.563109E+27; 
DUzQpnIldXsLYoYXyoRRyn = Math.Ceiling(Math.Atan(-5));
 bool? obeonuETmglbzANHVDuZkBLqHnm = new bool?();
obeonuETmglbzANHVDuZkBLqHnm = true;double zjutCZnPBOdbzbMMKOpOIZhB = 2467115; 
while(zjutCZnPBOdbzbMMKOpOIZhB == 120791.2){
zjutCZnPBOdbzbMMKOpOIZhB = Math.Floor(1.013963E-15);
Console.ReadLine();
}int LdcpMHb = 10; 
if(LdcpMHb == 572142){
LdcpMHb = LdcpMHb + 25979;
}int GIAqntt = 523330; 
while(GIAqntt == 523330){
GIAqntt = GIAqntt + 945603;
}int YttsxONoZijIxwFzukHooVJqPqEC = 839543833; 
while(YttsxONoZijIxwFzukHooVJqPqEC == 839543833){
YttsxONoZijIxwFzukHooVJqPqEC += 134100;
}double jJjtSsBhdMXdWVB = -4.391253E-15; 
if(jJjtSsBhdMXdWVB == 3.331423E-35){
jJjtSsBhdMXdWVB = Math.Ceiling(Math.Sinh(-5));
Console.Write(jJjtSsBhdMXdWVB.ToString());
}int idRNIiPBKLIsQVsbRHugGWeQ = 31483152; 
while(idRNIiPBKLIsQVsbRHugGWeQ == 31483152){
idRNIiPBKLIsQVsbRHugGWeQ += 729802;
}double uCTLsLkEsGuyZIDoFARXuBjl = 8.953519E-38; 
double SXnbIJfwROaCHRozcMdUYwTsI = -2.531427E+33;
SXnbIJfwROaCHRozcMdUYwTsI = Math.Sqrt(3);
uCTLsLkEsGuyZIDoFARXuBjl = SXnbIJfwROaCHRozcMdUYwTsI;
try{
int LUIxIgKomXQMCDMiKsi = 1140160;
if(LUIxIgKomXQMCDMiKsi == 97921){
LUIxIgKomXQMCDMiKsi++;
}else{
LUIxIgKomXQMCDMiKsi--;
Console.Write(LUIxIgKomXQMCDMiKsi.ToString());
}
}catch(Exception ex){

}long jQPniyShiXuiwyVhOs = 69617888455255834; 
long SREtAWXZXGVuGW = 50097915921294337; 
int haYRXNPHMuostaJzAWjhyxe = 62208161; 
while(haYRXNPHMuostaJzAWjhyxe == 62208161){
haYRXNPHMuostaJzAWjhyxe = 745947;
}short gmQkzXgck = 21442; 
short kIudBApTnDwpMVnkDmAU = -6245; 
sbyte eDQjCtT =  121; 
short OUblqZP = -4474; 
uint ERAzEJWAXqumcApCKpPBuEehXbmYl = 7028; 
ushort jFmcqHSxubZPCSDVPoAHXEaNsCDx = 62701; 
short HWpSVdzsJXeaDetxjKdxY = -19187; 
ulong HFA = 54338601184373555; 
float ZHSqWRKSD = -2.074678E+11F; 
double tNUSM = -2.062326E-30; 
if(tNUSM == -6.029426E-14){
tNUSM = Math.Ceiling(Math.Sinh(-5));
int? mFXMWNpoDumhlIlXyVi = 5362796;
mFXMWNpoDumhlIlXyVi += 64283;
}sbyte VxVVnPDZUAUVMPFfEBDumjYFiIJMW =  63; 
byte ValN =  76; 
float coVhjMSdzSItbeNMLypMGgR = -7.257455E-28F; 
byte aMDRQMNBescPSqFjToukYhnCp =  252; 

}public void hVzSa(){
double RqVaSHnFItJIyubWXONcTTzbCQH = 5499795; 
while(RqVaSHnFItJIyubWXONcTTzbCQH == 7.453066E-37){
RqVaSHnFItJIyubWXONcTTzbCQH = Math.Ceiling(Math.Sin(2));
int[] emTaYTUAbZKZbTGPA = { 2282685 , 39308 } ;
Random OJeqZgAtEiUnWyGfxQZXDtutwO= new Random();
Console.WriteLine(emTaYTUAbZKZbTGPA[OJeqZgAtEiUnWyGfxQZXDtutwO.Next(0,2)]);
}uint mTc = 357780775; 
short OmNBFpLFNdmtWy = 20193; 
ulong qunMVlUfVCWbleepK = 20636366803090117; 
long ZbCFxeBAYcKSaSwKPPECZ = 22530099319751879; 
short XlCVetWAXLFgnZQDwbn = -14050; 
float hLRYoQjD = -1.030756E-30F; 
int NHzWqpSoHQubQmY = 12959791; 
while(NHzWqpSoHQubQmY == 12959791){
NHzWqpSoHQubQmY += 702900;
}ulong Refeg = 9486573047742607; 
string LXyGEspPicSAwsZ = "NaDIRklLZlBWCZZPHI"; 
short GfiMBoX = -30032; 
short YXzWdmHxIAMMHsRuniccSOZRZ = 9526; 
float gCmjxhmEyGVNhkaTyTzQVfbP = 3.323429E-21F; 
double XIWxuRof = -523.4294; 
if(XIWxuRof == -6.516842E-33){
XIWxuRof = Math.Ceiling(Math.Tanh(0.1));
Console.WriteLine(XIWxuRof.ToString());
}sbyte cHhaNXELSB =  -10; 
uint pmSiXBT = 569681247; 
ulong RmZwIpSEzMJaaqON = 19266773705800334; 
long xRSlyARIiBXidC = 25342809318805986; 
sbyte elXAKChWMlUaMmnjjtimPGD =  -107; 
long lJwyFGIdglCqXHtbxRIIiQhybzJ = 56667596852328892; 
uint oHosIfcITwCUNiLx = 735031058; 
ushort YwfWNeHDJAjqUBhj = 24170; 
short LeQidhblqXxScUXDU = 1169; 
short DzBYuljZmbBdLkLwhlc = 19335; 
long pGmHgyXXnKbJYlFRPSXcZpxthR = 15411215917443000; 
sbyte MNeMcoNRcblARpxjSITaiUzoouD =  25; 
byte OndKfbOkaLDFqgG =  172; 
long lnLmR = 73335869437148933; 
sbyte EmXxgkwxHIhyFEsKGJz =  -31; 
double ByVYmNZOtYuYNTImGUQSeGJ = 1.666111E-28; 
if(ByVYmNZOtYuYNTImGUQSeGJ == 4.284794E-32){
double KPNBChXIySnjdHQzcsdFEeKU = 2.282348E+35;
ByVYmNZOtYuYNTImGUQSeGJ = Math.Round(KPNBChXIySnjdHQzcsdFEeKU ,1,MidpointRounding.ToEven);
try{
int QUWwLGnXghMyWiwlsAIBzzcAUKkFf = 8489536;
if(QUWwLGnXghMyWiwlsAIBzzcAUKkFf == 43713){
QUWwLGnXghMyWiwlsAIBzzcAUKkFf++;
}else{
QUWwLGnXghMyWiwlsAIBzzcAUKkFf--;
}
}catch(Exception ex){

}
}uint wKR = 4099; 
ushort XqSpzbj = 11355; 
int hSnbQU = 28684052; 
while(hSnbQU == 28684052){
hSnbQU = hSnbQU + 493836;
}uint AsxbIacAlzLdRhenshSlK = 251501; 
double PiaYHiySiQgFhBGeIHP = -6.31203E+18; 
if(PiaYHiySiQgFhBGeIHP == 1.252567E-24){
double mZVfyxTSbCGpmkJqYKjH = -3.083236E-07;
PiaYHiySiQgFhBGeIHP = mZVfyxTSbCGpmkJqYKjH * 2;

}
}public void ZxFByXDzZzeVW(){
double pszDoZgxBkWtsgBEQw = 0.4701617; 
while(pszDoZgxBkWtsgBEQw == 8.848136E+27){
pszDoZgxBkWtsgBEQw = Math.Sqrt(4);
try{

}catch(Exception ex){

}
}int bANchCfRpBWACFSiyNGkBWXxRM = 55200225; 
if(bANchCfRpBWACFSiyNGkBWXxRM == 241475){
bANchCfRpBWACFSiyNGkBWXxRM += 410817;
}long MVYZFbFtWwtCUAoPcSDtqfCqkx = 55643477154312172; 
float jyiFHhZCdpRuNUduTnDHtIPitCkN = -2.248814E+26F; 
long HtFFJYMjfa = 37032735050273210; 
long elDDJakfWmukHEOupz = 29222967865583681; 
int lYGmje = 39; 
if(lYGmje == 918398){
lYGmje += 667838;
}ulong mTGcobAVWIiNcMLjiQye = 25694860884109310; 
int DchUfIOqFC = 32730625; 
while(DchUfIOqFC == 32730625){
DchUfIOqFC = DchUfIOqFC + 442072;
}string JGTflIOolzbNIf = "JefGhHdwSkV"; 
uint XIcFAXKYwyRAgRwpp = 887596440; 
uint tUY = 887316995; 
ulong kaUpjlqEwynKxqmIofb = 42910772301987480; 
int dNGheOuDBpVARUTEkeKflKM = 995467546; 
if(dNGheOuDBpVARUTEkeKflKM == 482353){
dNGheOuDBpVARUTEkeKflKM += 507319;
}short KNQsKJCqE = 22782; 
sbyte QgpQjJQRTRzDuaNhgIVcldkN =  -119; 
int VJNgngV = 736958178; 
if(VJNgngV == 922072){
VJNgngV += 814909;
}long sTnmYKWtkVWcDMV = 67309603402803509; 
byte zdazYMfsOMszO =  148; 
uint oCkqetkRftnpPplhdSAGVUljRLOZE = 98; 
string WSF = "TqfTitGyPEzBjwWe"; 
float wobB = -1.640389E+07F; 
byte atkoSyHJkC =  98; 
ulong CHMcgoAtKdYiSypztXBIVt = 32991913501039624; 
ushort fqUJpzmpMUpolgaapjRET = 7058; 
ulong uADxnGR = 3289607100753908; 
float jVbcWatLaEqtQQZBMnQVR = -4.565752E-29F; 
string HXdAyJbWTKjPfyAjLmbAgSzdZ = "tsZThPYkpcokEUoxXgQMGQqLf"; 
ulong ELOAZ = 81707341677466878; 
float gxUAJgjKVqnnlEbnOsKuADEoMYF = 0.007025098F; 
double wUKJVQGgYtaSiMktMJJ = -3.567775E-24; 
wUKJVQGgYtaSiMktMJJ = Math.Ceiling(Math.Atan(-5));
Console.ReadKey();ushort bslFiGqOhuGfMUEczYPkKi = 15443; 
string tDsIlQHVxUVEAeJy = "eplTWQjYXHSkaTfj"; 
sbyte dZgVEZjoZJzYTfNjeWPGxMhJ =  -55; 
sbyte KwbPtupGzuiNmJIQBBYOMzRP =  -80; 

}public void zsqRyHujATXUe(){
int AoPVjDXPWIipwclFmF = 922725150; 
if(AoPVjDXPWIipwclFmF == 640631){
AoPVjDXPWIipwclFmF += 929864;
}uint ciOTHwwklibT = 84; 
uint wjopYhnoKofIGkITt = 494173350; 
ulong caN = 31349429240324836; 
ushort TzHltUVzisoNGb = 34358; 
sbyte itOBsiDwOskNpKZm =  -82; 
short lhyFSEUI = -2955; 
sbyte PCuUwNGVCoeQGDLMSIo =  0; 
ushort qWsLOWXyBemiDJtwnlHnR = 58742; 
sbyte ScgRbZhIjfky =  12; 
long KXdPaDyZctxudTgSyPaTMPfxBWGYy = 53289205252185459; 
sbyte PwcdzG =  29; 
sbyte cmNIDRnURIndXzDFyEQQVQeq =  39; 
byte JLIQURRVjLnqNYc =  124; 
sbyte dRWBAbbQnTlixZquFijRtlfDs =  97; 
sbyte cRTqYzFuB =  -85; 
byte PdEXKbnnasuToozSMuh =  154; 
float NyaVjXwWUwXEboJhTqGVmLyZ = -2.62592E+14F; 
double IgaHOaqgXOSpTdzYWMmjiE = 4.237625E+23; 
IgaHOaqgXOSpTdzYWMmjiE = Math.Pow(double.MinValue, double.MaxValue);
try{
int ngWVssjPLEFWlkHt = 9677641;
if(ngWVssjPLEFWlkHt == 54833){
ngWVssjPLEFWlkHt++;
}else{
ngWVssjPLEFWlkHt--;
}
}catch(Exception ex){

}long njgYSwBVAtjGWM = 38317788140994092; 
short kdFFsUYKBk = -27169; 
byte LcWpFzjTC =  46; 
uint uNcwnPoTSNOQWDFwBfLIaGlTjfb = 149979; 
ushort UgJYgVnlxPstYngucTOs = 28615; 
uint VPBilhOPLyeTKnFyAmToSJmnF = 71; 
double tclxpNjHNcdIaHVTwygz = -1.058058E-18; 
if(tclxpNjHNcdIaHVTwygz == 5.186797E-26){
tclxpNjHNcdIaHVTwygz = Math.Ceiling(Math.Sinh(-5));
 bool? QIScqujGFB = new bool?();
QIScqujGFB = true;
}float AaXdHdFCVN = 9.03451E-31F; 
byte StCSoWlA =  50; 
byte YNz =  145; 
short PZNSumD = 30523; 
ushort FbVLlqyCPsPEWdaQNcI = 21243; 
sbyte FFNgysygZTRpIzoAQIoXOYuldJSF =  -1; 
uint gAHxqYeUedJ = 9114; 
ulong xDBmmabjiHkBdOBTHBE = 16987283493248130; 
byte fGzAzmpdmmpeoyQfyagEjbSsbO =  106; 

}public void DMCmQpdaMbGBkQun(){
byte CDlSdKHIKeOgzKRPBBL =  14; 
short uPuxwGstIFewmToLJSHiHZmGDWKxo = 24382; 
int qDhxyHoa = 3123; 
while(qDhxyHoa == 3123){
qDhxyHoa += 55611;
}ushort BQSTxftwDOTAWWeObVeLLTmbDHUpD = 4853; 
int ALVzbKOkiNTKOQds = 6642; 
while(ALVzbKOkiNTKOQds == 6642){
ALVzbKOkiNTKOQds += 739736;
}byte ERpPlcTzjtZkaGNqgTCY =  251; 
int NDCeUWzCNy = 9006; 
while(NDCeUWzCNy == 9006){
NDCeUWzCNy = 992345;
}sbyte uAuQSqGVPZfkoIaDDlLYZDLSmO =  63; 
long XhTueFdUXLjzYgAxC = 77448534564485374; 
float OCDeFVtSRlRkdXJGCBBbwlxMFh = 1.318069E+31F; 
sbyte FmjMosuGFEGoIj =  51; 
double WYlkOITyDfRAydyMx = 319544.4; 
if(WYlkOITyDfRAydyMx == -1.847219E+11){
WYlkOITyDfRAydyMx = Math.Truncate(WYlkOITyDfRAydyMx);
try{
Console.WriteLine(WYlkOITyDfRAydyMx.ToString());
}catch(Exception ex){
Console.WriteLine(ex.Message);
}
}short LaKhliuFsCeniNIhVgy = 6114; 
byte sVfRegJTAzhISqbNzlkKCsSGKkgl =  221; 
sbyte LuNJObwemVzcHfCVqghIktDwad =  -1; 
sbyte axaqGynfJmLdhYOcpkAioQfMBH =  -45; 
float ILCojlAzLbJn = 2.630012E+12F; 
float FdwOtKTaYFkqoR = 1.184213E-29F; 
ushort aDiOXMoFGiRUBgloVEWcYXR = 34807; 
short bhjTdDRbIqmQfJAOeKj = -28223; 
string fZVu = "uPcLIysL"; 
sbyte MfnczzUbfymEk =  92; 
sbyte mCXcjXJsymGH =  27; 
ushort XstsEVaGFkYQWXREiEpjQNd = 14751; 
ulong WGkTnxMKokSBExm = 65150296403134905; 
long cpjXkngEgZwwMX = 41431987512308676; 
float VulaIMBYCxlfDMeWLWxySAHVJczN = -3.983764E+34F; 
ulong xmcQ = 71112965493423636; 
long CBDJbMExewOqsF = 48251464623124019; 
long UEabYxGqjl = 45415949159947419; 
sbyte bLPqhLzpHCOkRmPyLDzJ =  72; 
ushort fcoDGI = 42104; 
long hbmiPKUmYpLIzpgXEzpV = 83888639086623131; 
sbyte IYQedQZGIbJkLzLAINiFciqQfJ =  -85; 
long fwbefJMRtmpXYRolsPu = 20204349543573441; 

}
} 