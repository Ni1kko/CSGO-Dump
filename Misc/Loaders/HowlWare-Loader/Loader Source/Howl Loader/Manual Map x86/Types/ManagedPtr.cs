using System;
using System.Runtime.InteropServices;

namespace ManualMapInjection.Injection.Types
{
    internal class ManagedPtr<T> where T : struct
    {
        internal IntPtr Address { get; }

        internal T Value
        {
            get { return this[0]; }
        }

        private protected int? _structSize;

        internal int StructSize
        {
            get
            {
                if (_structSize == null)
                {
                    _structSize = Marshal.SizeOf(typeof(T));
                }

                return _structSize.Value;
            }
        }

        private protected static T GetStructure(IntPtr address)
        {
            return (T) Marshal.PtrToStructure(address, typeof(T));
        }

        internal T this[uint index]
        {
            get { return GetStructure(Address + (int) index * StructSize); }
        }

        public static ManagedPtr<T> operator +(ManagedPtr<T> c1, int c2)
        {
            return new ManagedPtr<T>(c1.Address + c2 * c1.StructSize);
        }

        public static ManagedPtr<T> operator ++(ManagedPtr<T> a)
        {
            return a + 1;
        }

        public static ManagedPtr<T> operator -(ManagedPtr<T> c1, int c2)
        {
            return new ManagedPtr<T>(c1.Address - c2 * c1.StructSize);
        }

        public static ManagedPtr<T> operator --(ManagedPtr<T> a)
        {
            return a - 1;
        }

        public static explicit operator ManagedPtr<T>(IntPtr ptr)
        {
            if (ptr == IntPtr.Zero)
            {
                return null;
            }

            return new ManagedPtr<T>(ptr);
        }

        public static explicit operator IntPtr(ManagedPtr<T> ptr)
        {
            return ptr.Address;
        }

        private protected GCHandle _handle;

        private protected bool _freeHandle;

        internal ManagedPtr(IntPtr address)
        {
            Address = address;
        }

        internal ManagedPtr(object value, bool freeHandle = true)
        {
            if (value == null)
            {
                throw new InvalidOperationException("Cannot create a pointer of type null");
            }

            try
            {
                _handle = GCHandle.Alloc(value, GCHandleType.Pinned);
            }
            catch (Exception)
            {
                throw new InvalidOperationException($"Cannot create a pointer of type {value.GetType().Name}");
            }

            _freeHandle = freeHandle;
            Address = _handle.AddrOfPinnedObject();
        }

        ~ManagedPtr()
        {
            if (_handle.IsAllocated && _freeHandle)
            {
                _handle.Free();
            }
        }
    }
}
