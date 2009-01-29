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
namespace win32cpp{
    class Splitter;
    class TabView;
    class Window;
    class TopLevelWindow;
}
namespace musik { namespace cube {
    class LibraryWindowView;
	class SourcesView;
	class SourcesController;
    class TracklistController;
} }
//////////////////////////////////////////////////////////////////////////////

#include <core/Library/Base.h>
#include <core/MessageQueue.h>
#include <win32cpp/Timer.hpp>
#include <win32cpp/Types.hpp>

#include <boost/shared_ptr.hpp>
#include <vector>

//////////////////////////////////////////////////////////////////////////////

using namespace win32cpp;

namespace musik { namespace cube {

//////////////////////////////////////////////////////////////////////////////

class LibraryWindowController : public EventHandler
{
	public:
		LibraryWindowController(LibraryWindowView& view);
		~LibraryWindowController();

	protected:  
		void OnViewCreated(Window* window);
		void OnDestroyed(Window* window);

        void UpdateLibraryTabs();

		void OnLibraryMessage(const char* identifier,void* data);

		LibraryWindowView& view;

		typedef boost::shared_ptr<SourcesController> SourcesControllerPtr;
		typedef std::map<SourcesView*,SourcesControllerPtr> SourcesMap;

		SourcesMap libraries;
	public:

		SourcesController* CurrentSourceController();

    private:
        TracklistController *nowPlayingController;
};

//////////////////////////////////////////////////////////////////////////////

} }     // musik::cube
