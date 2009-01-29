//////////////////////////////////////////////////////////////////////////////
//
// License Agreement:
//
// The following are Copyright � 2007, Casey Langen
//
// Sources and Binaries of: mC2, win32cpp
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright 
//      notice, this list of conditions and the following disclaimer in the 
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the author nor the names of other contributors may 
//      be used to endorse or promote products derived from this software 
//      without specific prior written permission. 
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE. 
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////////
// Forward declare
namespace musik{ namespace core{ namespace Query{
    class ListBase;
} } }
//////////////////////////////////////////////////////////////////////////////

#include <win32cpp/ListView.hpp>
#include <core/tracklist/Base.h>

//////////////////////////////////////////////////////////////////////////////

using namespace win32cpp;

namespace musik { namespace cube {

//////////////////////////////////////////////////////////////////////////////

class TracklistModel: public ListView::Model, public EventHandler
{
        // typedefs
public:     
    typedef ListView::Model base;
    typedef ListView::RowRendererRef RowRendererRef;
    typedef ListView::CellRendererRef CellRendererRef;
    typedef ListView::ColumnRef ColumnRef;

        // public API
    TracklistModel(
        musik::core::Query::ListBase *connectedQuery,
        musik::core::tracklist::Ptr setTracklist,
		unsigned int options);

    void ConnectToQuery(musik::core::Query::ListBase *connectedQuery);
    void OnRowActivated(int row);

    void OnPlayNow(win32cpp::ListView::RowIndexList& selectedRows);
    void OnEnqueue(win32cpp::ListView::RowIndexList& selectedRows);

    // ListView::Model implementation
    virtual uistring CellValueToString(int rowIndex, ColumnRef column);

    // instance data
protected:  
    void OnTrackMeta(std::vector<long> trackPositions);
    void OnTracks(bool cleared);
    void OnPositionChanged(long activeRow,long oldActiveRow);

    int currentPosition;
	unsigned int options;
public:     
    musik::core::tracklist::Ptr tracklist;    // FIXME: no public fields!

};

//////////////////////////////////////////////////////////////////////////////

} }     // musik::cube
