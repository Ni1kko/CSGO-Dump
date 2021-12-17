namespace DDX.Enums
{
    public enum AllocationType
    {
        Commit = 0x1000,
        Reserve = 0x2000,
        Reset = 0x80000,
        ResetUndo = 0x1000000,
        MemTopDown = 0x100000,
        MemPhysical = 0x400000,
        MemLargePages = 0x20000000,
    }
}
