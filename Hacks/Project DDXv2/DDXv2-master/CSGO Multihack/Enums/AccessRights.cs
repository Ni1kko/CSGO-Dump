namespace DDX.Enums
{
    public enum AccessRights
    {
        Terminate = 0x1,
        CreateThread = 0x2,
        VmOperation = 0x8,
        VmRead = 0x10,
        VmWrite = 0x20,
        DupHandle = 0x40,
        CreateProcess = 0x80,
        SetQuota = 0x100,
        SetInformation = 0x200,
        QueryInformation = 0x400,
        SuspendResume = 0x800,
        QueryLimitedInformation = 0x1000,
        Synchronize = 0x100000,
        AllAccess = 0x1F0FFF,
    }
}
