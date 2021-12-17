namespace DDX.Enums
{
    public enum Protection
    {
        PageNoAccess = 0x1,
        PageReadOnly = 0x2,
        PageReadWrite = 0x4,
        PageWriteCopy = 0x8,
        PageExecute = 0x10,
        PageExecuteRead = 0x20,
        PageExecuteReadWrite = 0x40,
        PageExecuteWriteCopy = 0x80,
        PageGuard = 0x100,
        PageNoCache = 0x200,
        PageWriteCombine = 0x400,
        PageTargetsNoUpdate = 0x40000000,
        PageTargetsInvalid = 0x40000000,
    }
}
