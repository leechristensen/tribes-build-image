//----------------------------------------------------------------------------
// ObjectComponents
// Copyright (c) 1994, 1997 by Borland International, All Rights Reserved
//
// $Revision:   2.16  $
//
// Low level OLE Utility class definitions
//----------------------------------------------------------------------------
#if !defined(OCF_OLEUTIL_H)
#define OCF_OLEUTIL_H

#if !defined(OCF_DEFS_H)
# include <ocf/defs.h>
#endif
#if !defined(OCF_EXCEPT_H)
# include <ocf/except.h>
#endif
#if !defined(SERVICES_CSTRING_H)
# include <services/cstring.h>
#endif
#if !defined(SERVICES_EXCEPT_H)
# include <services/except.h>
#endif
#if !defined(SERVICES_OLE2INC_H)
# include <services/ole2inc.h>
#endif

//
// Force this pointers and vtable pointers far on interfaces
//
#if !defined(_ICLASS)
# if defined(BI_COMP_BORLANDC)
#   if defined(BI_PLAT_WIN16)
#     define _ICLASS  __huge
#   else
#     define _ICLASS
#   endif
# else
#   define _ICLASS  FAR
# endif
#endif

#define _IFUNC STDMETHODCALLTYPE

#if defined(BI_NAMESPACE)
namespace ClassLib {
#endif

class   TRegList;

#if defined(BI_NAMESPACE)
} // namespace ClassLib
#endif

#if defined(BI_NAMESPACE)
namespace OCF {
#endif

//
// class TComRef
// ~~~~~ ~~~~~~~
// Encapsulation of OLE interface pointers
//
template<class T> class TComRefBase {
  public:
            operator T*()         {return I;}
    virtual operator T**()        {Clear(); return &I;}
    int     operator !() const    {return I==0;}
    void operator delete(void* p) {((TComRefBase<T>*)p)->I=0;}
  protected:
    TComRefBase(const TComRefBase<T>& i) : I(i.I) {if (I) I->AddRef();}
    TComRefBase(T* i) : I(i) {}
    TComRefBase()     : I(0) {}
    virtual ~TComRefBase() {Clear();}

    void Clear() {if (I) {I->Release(); I = 0;}}
    T* I;
  private:

    // Prohibit use of operator new
    //
    void* operator new(size_t) {return 0;}
};

template<class T> class TComRef : public TComRefBase<T> {
  public:
    TComRef() : TComRefBase<T>() {}
    TComRef(T* iface) : TComRefBase<T>(iface) {}
    TComRef(const TComRef<T>& i) : TComRefBase<T>(i) {}
    TComRef<T>& operator =(T* iface) {Clear(); I = iface; return *this;}
    TComRef<T>& operator =(const TComRef<T>& i)
                 {Clear(); if (i.I) {I = i.I; I->AddRef();} return *this;}
    T* operator->()       {return I;}      // Could throw exception if I==0
    T* operator->() const {return I;}      // Could throw exception if I==0
};

//
// class TBSTR
// ~~~~~ ~~~~~
// BASIC global string encapsulation
//
class TBSTR {
  public:
    TBSTR();
    TBSTR(BSTR p);
   ~TBSTR();

     // Operators
    //
    BSTR      operator =(BSTR p);
    TBSTR&    operator =(const TBSTR& p);
              operator  BSTR();
              operator  BSTR*();
    int       operator !() const;
    void      operator ~();

  protected:
    BSTR      P;

  private:

    // Prohibit use of new/delete operators
    //
    void* operator new(size_t) {return 0;}
    void  operator delete(void*) {}
};

#if defined(BI_NAMESPACE)
} // namespace OCF
#endif

//
//
//
inline TBSTR::TBSTR() : P(0)
{}

//
//
//
inline TBSTR::TBSTR(BSTR p) : P(p)
{}

//
//
//
inline TBSTR::~TBSTR()
{
  // NOTE: OK to pass 0 to 'SysFreeString'
  //
  ::SysFreeString(P);
}

//
//
//
inline BSTR
TBSTR::operator =(BSTR p)
{
  ::SysFreeString(P);
  return P = p;
}

//
//
//
inline TBSTR&
TBSTR::operator =(const TBSTR& p)
{
  ::SysFreeString(P);
  P = p.P;
  return *this;
}

//
//
//
inline
TBSTR::operator BSTR()
{
  return P;
}

//
//
//
inline
TBSTR::operator BSTR*()
{
  if(P)
   ::SysFreeString(P);
  return &P;
}

//
//
//
inline int
TBSTR::operator !() const
{
  return P==0;
}

//
//
//
inline void
TBSTR::operator ~()
{
  ::SysFreeString(P);
  P = 0;
}

#if defined(BI_NAMESPACE)
namespace OCF {
#endif

// Conversion routines
//
void OcHiMetricToPixel(const SIZEL& sizeHM, SIZEL& sizePix);
void OcPixelToHiMetric(const SIZEL& sizePix, SIZEL& sizeHM);

//
// class TOleAllocator
// ~~~~~ ~~~~~~~~~~~~~
// Object to initialize OLE and access memory manager
//
class TOleAllocator {
  public:
    TOleAllocator();             // Gets allocator, does not initialize OLE
    TOleAllocator(IMalloc* mem); // Use 0 to initialize with OLE task allocator
   ~TOleAllocator();
    void*     Alloc(unsigned long size);
    void      Free(void far* pv);
    IMalloc*  Mem;
  private:
    bool      Initialized;
};

#if defined(BI_NAMESPACE)
} // namespace OCF
#endif

//
//
//
inline
TOleAllocator::TOleAllocator(IMalloc* mem) : Initialized(true)
{
  OLECALL(OleInitialize(mem), "OleInitialize");
  OLECALL(CoGetMalloc(MEMCTX_TASK, &Mem), "CoGetMalloc");
}

//
//
inline
TOleAllocator::TOleAllocator() : Initialized(false)
{
  OLECALL(CoGetMalloc(MEMCTX_TASK, &Mem), "CoGetMalloc");
}

//
//
//
inline
TOleAllocator::~TOleAllocator()
{
  if (Mem)
    Mem->Release();
  if (Initialized)
    ::OleUninitialize();
}

#if defined(BI_NAMESPACE)
namespace OCF {
#endif

//
// class TOleInit
// ~~~~~ ~~~~~~~~
// Simple wrapper of TOleAllocator used exclusively for initializing OLE
//
class TOleInit : public TOleAllocator {
  public:
    TOleInit() : TOleAllocator(0){};
};

// To maintain compatibility with initial release of v5.0 of OCF
//
typedef TOleInit  TOleStart;

//
// class TClassId
// ~~~~~ ~~~~~~~~
// GUID/IID/CLSID management
//
class far TClassId {
  public:
    enum TGenerate {
      Generate
    };
    TClassId(const GUID& guid);
    TClassId(const char* idtxt);
    TClassId(const TClassId& copy);
    TClassId(TGenerate);
    TClassId& operator =(const TClassId& copy);
    TClassId(const GUID far& guid, int offset);
   ~TClassId();
    operator GUID&();
    operator const char*();

  protected:
    TClassId();
    GUID  Guid;
    const char* Text;
};

#if defined(BI_NAMESPACE)
} // namespace OCF
#endif

//
//
inline TClassId::TClassId(const GUID& guid) : Text(0) {
  Guid = guid;
}

//
//
inline TClassId::TClassId(const TClassId& copy) : Guid(copy.Guid), Text(0) {}

// Default constructor - Derived class must set GUID
//
inline TClassId::TClassId() : Text(0) {}

//
//
inline TClassId::operator GUID&() {
  return Guid;
}

//
//
inline TClassId& TClassId::operator =(const TClassId& copy) {
  Guid = copy.Guid;
  Text=0;
  return *this;
}

#if defined(BI_NAMESPACE)
namespace OCF {
#endif

//
// class TBaseClassId
// ~~~~~ ~~~~~~~~~~~~
// Base GUID for a components sequence of GUIDs
//
class TBaseClassId : public TClassId {
  public:
    TBaseClassId(TRegList& regInfo, int maxAlloc = 256);
    TBaseClassId(const GUID& guid, int maxAlloc = 256);
    TBaseClassId(const char* idtxt, int maxAlloc = 256);
    TBaseClassId(TGenerate, int maxAlloc = 256);
    TClassId operator [](int offset);
    int   AllocId();
    int   GetOffset(const GUID far& guid); // returns -1 if not within range

  private:
    int   Offset;  // Last offset given out by AllocId(), creator reserves 0
    int   MaxAlloc;
};

//
// Fast GUID compare functions, assume that low word has already matched OK
//
void CmpGuid12 (IUnknown* This, REFIID req, REFIID ref, void** pif);
void CmpGuidOle(IUnknown* This, REFIID req, void** pif);

//
// class TUnknown
// ~~~~~ ~~~~~~~~
// Standard implementation of a controlling IUnknown for an object, to be
// inherited with other COM interfaces into an implementation class whose
// IUnknown implementation delegates to TUnknown::Outer.
//
class _ICLASS TUnknown {
  public:
    operator         IUnknown&() {return *Outer;}
    operator         IUnknown*() {Outer->AddRef(); return Outer;}
    IUnknown*       SetOuter(IUnknown* outer = 0);
    IUnknown*       GetOuter() {return Outer;}
    unsigned long   GetRefCount();
    unsigned long   AdjustRefCount(int i);
    IUnknown&       Aggregate(TUnknown& inner);  // returns outermost IUnknown

  protected:
    TUnknown();
    virtual ~TUnknown();

    IUnknown&       ThisUnknown() {return I;}
    IUnknown*       Outer;
    virtual HRESULT QueryObject(const GUID& iid, void** pif);

  private:
    class _ICLASS TUnknownI : public IUnknown {
      public:
        HRESULT       _IFUNC QueryInterface(const GUID far& iid, void** pif);
        unsigned long _IFUNC AddRef();
        unsigned long _IFUNC Release();
        TUnknownI() : RefCnt(0), Inner(0) {}
       ~TUnknownI();
        unsigned  RefCnt;  // Object's reference count, remains 1 if aggregated
        TUnknown* Inner;   // Chain of TUnknown-aggregated objects
      private:
        TUnknown& Host();
    } I;
  friend class TUnknownI;
};

#if defined(BI_NAMESPACE)
} // namespace OCF
#endif

inline IUnknown* TUnknown::SetOuter(IUnknown* outer)
{
  if (outer) {
    Outer = outer;
    if (I.Inner)
      I.Inner->SetOuter(outer);
  }
  return &I;
}

inline TUnknown& TUnknown::TUnknownI::Host()
{
  return *(TUnknown*)((char*)this-(int)(&((TUnknown*)0)->I));
}

inline unsigned long TUnknown::AdjustRefCount(int i)
{
  return (unsigned long)(I.RefCnt += i);
}


//------------------------------------------------------------------------
//
// Macros to generate COM mixin classes for use with TUnknown
//      Each base class must have an inline QueryInterface wrapper defined.
//
// DECLARE_COMBASES1(classname, base)    // declares support for 1 interface
// DECLARE_COMBASES2(classname, base, base)      // mixes in for 2 interfaces
//  .......
// DEFINE_COMBASES1(classname, base)  // implements IUnknown for 1 interface
// DEFINE_COMBASES2(classname, base, base)   // implements for 2 interfaces
//  ......
//
// Macros to generate inline QueryInterface wrappers for interfaces/delegates
//    (low GUID word may specify an explicit integer or an IID_whatever.Data1)
//    For efficiency they are void functions; check result pointer for success
//
// DEFINE_QI_BASE(cls, low)      // Inherited COM interface with IUnknown base
// DEFINE_QI_OLEBASE(cls, low)   // Inherited OLE interface using an OLE GUID
// DEFINE_QI_DERIVED(cls, base, low)     // COM interface derived from another
// DEFINE_QI_OLEDERIVED(cls, base, low)  // OLE interface derived from another
// DEFINE_QI_DELEGATE(cls, member)       // Inherited object with COM member
// DEFINE_QI_DELEGATE_TEST(cls, member)  // Object whose COM member may be 0
// DEFINE_QI_MEMBER(name, member)        // Used inside object with COM member
// DEFINE_QI_MEMBER_TEST(name, member)   // Where object's COM member may be 0
//
// Macro to invoke inline QueryInterface wrappers and return if successful
//    A list of these is generated by the DEFINE_COMBASESn(...) macro
//
// COMBASEQUERY(name) // Calls name_QueryInterface(), returns OK if success
//
// Macro to provide standard IUnknown implementation which forwards to Outer
//    This macro is automatically invoked by the DEFINE_COMBASESn(...) macro
//
// IUNKNOWN_FORWARD(cls, outer) // cls IUnknown implementation forwards to outer
//
// Example of user-written COM class inheriting from macro-generated mixin
//
//  class TSample : public classname {
//   public:
//    TSample(IUnknown* outer, IUnknown*far* pif) { *pif = SetOuter(outer); }
//  };
//
//------------------------------------------------------------------------

#define COMBASES_D(cls, bases)     \
  class cls : public TUnknown, bases { \
  protected:                        \
    virtual HRESULT _IFUNC QueryInterface(const GUID far& iid, void far*far* pif); \
    virtual unsigned long _IFUNC AddRef();  \
    virtual unsigned long _IFUNC Release(); \
    virtual HRESULT QueryObject(const GUID far& iid, void far*far* pif); \
            HRESULT QueryBases(const GUID far& iid, void far*far* pif) \
                               {return cls::QueryObject(iid, pif);} \
  }; // QueryBases() is an inline wrapper to this QueryObject() implementation

#define COMQRY1BASES(i1)             protected i1
#define COMQRY2BASES(i1,i2)          protected i1, COMQRY1BASES(i2)
#define COMQRY3BASES(i1,i2,i3)       protected i1, COMQRY2BASES(i2,i3)
#define COMQRY4BASES(i1,i2,i3,i4)    protected i1, COMQRY3BASES(i2,i3,i4)
#define COMQRY5BASES(i1,i2,i3,i4,i5) protected i1, COMQRY4BASES(i2,i3,i4,i5)

#define DECLARE_COMBASES1(cls,i1) \
  COMBASES_D(cls,COMQRY1BASES(i1))
#define DECLARE_COMBASES2(cls,i1,i2) \
  COMBASES_D(cls,COMQRY2BASES(i1,i2))
#define DECLARE_COMBASES3(cls,i1,i2,i3) \
  COMBASES_D(cls,COMQRY3BASES(i1,i2,i3))
#define DECLARE_COMBASES4(cls,i1,i2,i3,i4) \
  COMBASES_D(cls,COMQRY4BASES(i1,i2,i3,i4))
#define DECLARE_COMBASES5(cls,i1,i2,i3,i4,i5) \
  COMBASES_D(cls,COMQRY5BASES(i1,i2,i3,i4,i5))

// Macro which can be used by classes which inherit from TUnknown for
// IUnknown methods support - This macro defines the methods inline
//
#define TUNKNOWN_STANDARD_IUNKNOWN_METHODS \
    ulong   _IFUNC   AddRef()       {return GetOuter()->AddRef();}    \
    ulong   _IFUNC   Release()      {return GetOuter()->Release();}   \
    HRESULT _IFUNC   QueryInterface(const GUID& iid, void** iface)    \
                     {return GetOuter()->QueryInterface(iid, iface);}


// Macro which can be used by classes which inherit from TUnknown for
// IUnknown methods support - This macros defines the methods out-of-line
//
#define IUNKNOWN_FORWARD(cls, outer) \
  HRESULT _IFUNC cls::QueryInterface(const GUID far& iid, void far* far* pif)\
                                 { return outer->QueryInterface(iid, pif); } \
  unsigned long _IFUNC cls::AddRef()  { return outer->AddRef(); } \
  unsigned long _IFUNC cls::Release() { return outer->Release(); }

#define COMBASES_I(cls, tests) \
  IUNKNOWN_FORWARD(cls, Outer) \
  HRESULT cls::QueryObject(const GUID far& iid, void far*far* pif) \
         { *pif = 0; tests return ResultFromScode(E_NOINTERFACE); }

#define COMBASEQUERY(cls) \
  cls##_QueryInterface(this,iid,pif); if(*pif) return NOERROR;

#define COMQRY1TEST(i1)             COMBASEQUERY(i1)
#define COMQRY2TEST(i1,i2)          COMBASEQUERY(i1) COMQRY1TEST(i2)
#define COMQRY3TEST(i1,i2,i3)       COMBASEQUERY(i1) COMQRY2TEST(i2,i3)
#define COMQRY4TEST(i1,i2,i3,i4)    COMBASEQUERY(i1) COMQRY3TEST(i2,i3,i4)
#define COMQRY5TEST(i1,i2,i3,i4,i5) COMBASEQUERY(i1) COMQRY3TEST(i2,i3,i4,i5)

#define DEFINE_COMBASES1(cls,i1) \
  COMBASES_I(cls,COMQRY1TEST(i1))
#define DEFINE_COMBASES2(cls,i1,i2) \
  COMBASES_I(cls,COMQRY2TEST(i1,i2))
#define DEFINE_COMBASES3(cls,i1,i2,i3) \
  COMBASES_I(cls,COMQRY3TEST(i1,i2,i3))
#define DEFINE_COMBASES4(cls,i1,i2,i3,i4) \
  COMBASES_I(cls,COMQRY4TEST(i1,i2,i3,i4))
#define DEFINE_COMBASES5(cls,i1,i2,i3,i4,i5) \
  COMBASES_I(cls,COMQRY5TEST(i1,i2,i3,i4,i5))

#define DEFINE_QI_BASE(cls, low) \
  inline void cls##_QueryInterface(cls* obj, REFIID iid, void FAR* FAR* pif) \
  {if (iid.Data1==low) CmpGuid12(obj, iid, IID_##cls, pif);}

#define DEFINE_QI_OLEBASE(cls, low) \
  inline void cls##_QueryInterface(cls* obj, REFIID iid, void FAR* FAR* pif) \
  {if (iid.Data1==low) CmpGuidOle(obj, iid, pif);}

#define DEFINE_QI_DERIVED(cls, base, low) \
  inline void cls##_QueryInterface(cls* obj, REFIID iid, void FAR* FAR* pif) \
  {if (iid.Data1==low) CmpGuid12(obj, iid, IID_##cls, pif); \
  else base##_QueryInterface(obj, iid, pif);}

#define DEFINE_QI_OLEDERIVED(cls, base, low) \
  inline void cls##_QueryInterface(cls* obj, REFIID iid, void FAR* FAR* pif) \
  {if (iid.Data1==low) CmpGuidOle(obj, iid, pif); \
  else base##_QueryInterface(obj, iid, pif);}

#define DEFINE_QI_DELEGATE(cls, member) \
  inline void cls##_QueryInterface(cls* obj, REFIID iid, void FAR* FAR* pif) \
  {obj->member->QueryInterface(iid, pif);}

#define DEFINE_QI_DELEGATE_TEST(cls, member) \
  inline void cls##_QueryInterface(cls* obj, REFIID iid, void FAR* FAR* pif) \
  {if (obj->member) obj->member->QueryInterface(iid, pif);}

#define DEFINE_QI_MEMBER(name, member) \
  void name##_QueryInterface(void far*, REFIID iid, void FAR* FAR* pif) \
  {member->QueryInterface(iid, pif);}

#define DEFINE_QI_MEMBER_TEST(name, member) \
  void name##_QueryInterface(void far*, REFIID iid, void FAR* FAR* pif) \
  {if (member) member->QueryInterface(iid, pif);}

#define DEFINE_COMBASE_DELEGATE(cls, ifc, member) \
  struct _ICLASS cls { \
    cls() : member(0) {} \
    ifc* member; \
    DEFINE_QI_MEMBER_TEST(cls, member) \
  };

#endif  //  OCF_OLEUTIL_H
