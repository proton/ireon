/***********************************************************************
    filename: CEGUIPythonFunctor.h
    created:  Sat Jun 17 2006
    author:   Andrzej Krzysztof Haczewski <a.haczewski@gmail.com> (remove first dot)

    purpose:  Defines interface for PythonFunctor class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#ifndef _CEGUIPythonFunctor_h_
#define _CEGUIPythonFunctor_h_
#pragma warning(push, 1)
#include "CEGUIEventSet.h"
#include "CEGUIRefCounted.h"
#pragma warning(pop)
typedef struct _object PyObject;

// Start of CEGUI namespace section
namespace CEGUI
{

class PythonCallbackWrapper
{
    friend class PythonFunctor;

public:
    PythonCallbackWrapper(PyObject* handler);
    ~PythonCallbackWrapper();

    //bool runCallback(const EventArgs& args) const;

private:
    mutable void* callback;
};

/*!
\brief
    Functor class used for subscribing Lua functions to CEGUI events
*/
class PythonFunctor
{
public:
    PythonFunctor(PyObject* handler);
    PythonFunctor(const PythonFunctor& obj) : pythonCallback(obj.pythonCallback) {}
    bool	operator()(const EventArgs& e) const;

private:
    // no assignment possible
    PythonFunctor& operator=(const PythonFunctor& rhs);

    RefCounted<PythonCallbackWrapper> pythonCallback;

};

} // namespace CEGUI

#endif // end of guard _CEGUIPythonFunctor_h_
