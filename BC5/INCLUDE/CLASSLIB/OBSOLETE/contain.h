/*------------------------------------------------------------------------*/
/*                                                                        */
/*  CONTAIN.H                                                             */
/*                                                                        */
/*  Copyright Borland International 1991, 1993                            */
/*  All Rights Reserved                                                   */
/*                                                                        */
/*------------------------------------------------------------------------*/

#if !defined( __CONTAIN_H )
#define __CONTAIN_H

#define BI_OLDNAMES

#if !defined( __CLSTYPES_H )
#include "classlib\obsolete\ClsTypes.h"
#endif  // __CLSTYPES_H

#if !defined( __OBJECT_H )
#include "classlib\obsolete\Object.h"
#endif  // __OBJECT_H

#if !defined( __SHDDEL_H )
#include "classlib\ShdDel.h"
#endif  // __SHDDEL_H

#pragma option -Vo-
#if defined( __BCOPT__ ) && !defined( __FLAT__ ) && !defined( _ALLOW_po )
#pragma option -po-
#endif

_CLASSDEF(ostream)
_CLASSDEF(ContainerIterator)
_CLASSDEF(Container)

class _CLASSTYPE Container :
    public Object,
    public virtual Object::TShouldDelete
{

public:

    Container() : itemsInContainer(0) {}

    virtual void flush( DeleteType = DefDelete ) = 0;

    virtual int isEmpty() const
        {
        return itemsInContainer == 0;
        }

    virtual countType getItemsInContainer() const
        {
        return itemsInContainer;
        }

    virtual void forEach( iterFuncType, void _FAR * );
    virtual Object _FAR & firstThat( condFuncType, void _FAR * ) const;
    virtual Object _FAR & lastThat( condFuncType, void _FAR * ) const;

    virtual classType isA() const = 0;
    virtual _TCHAR _FAR *nameOf() const = 0;
    virtual hashValueType hashValue() const;
    virtual int isEqual( const Object& ) const;

    virtual void printOn( ostream& ) const;
    virtual void printHeader( ostream& ) const;
    virtual void printSeparator( ostream& ) const;
    virtual void printTrailer( ostream& ) const;

    virtual ContainerIterator _FAR & initIterator() const = 0;

    friend class ContainerIterator;

protected:

    unsigned itemsInContainer;

};

class _CLASSTYPE ContainerIterator
{

public:

    virtual ~ContainerIterator()
    {
    }

    virtual operator int() = 0;
    virtual Object _FAR & current() = 0;
    virtual Object _FAR & operator ++ ( int ) = 0;
    virtual Object _FAR & operator ++ () = 0;
    virtual void restart() = 0;

};

#if defined( __BCOPT__ ) && !defined( __FLAT__ ) && !defined( _ALLOW_po )
#pragma option -po.
#endif
#pragma option -Vo.

#endif
