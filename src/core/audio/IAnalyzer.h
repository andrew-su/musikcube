//////////////////////////////////////////////////////////////////////////////
// Copyright � 2007, Daniel �nnerby
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

#include <core/config.h>
#include <core/ITrack.h>
#include <core/audio/IBuffer.h>

//////////////////////////////////////////////////////////////////////////////
namespace musik { namespace core { namespace audio {
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////
///\brief
///The main interface for a analyzer plugin
///
///A analyzer plugin will be executed from the Indexer
///after all tags has been read. The plugin will first be
///called with the Start method, and if that method returns true
///a audio::Stream will be opened and the whole track will be
///decoded and passed on to the Analyze method (or until the Analyze method
///returns false). Finally the End method will be called where the
///the plugin can make changes to the tracks metadata.
//////////////////////////////////////////
class  IAnalyzer {
    public:
        //////////////////////////////////////////
        ///\brief
        ///Destroy the object
        //////////////////////////////////////////
        virtual void    Destroy() = 0;

        //////////////////////////////////////////
        ///\brief
        ///Start analyzing the track. Returns true if
        ///the analyzing should continue.
        //////////////////////////////////////////
        virtual bool    Start(musik::core::ITrack *track) = 0;

        //////////////////////////////////////////
        ///\brief
        ///Analyze a buffer
        //////////////////////////////////////////
        virtual bool    Analyze(musik::core::ITrack *track,IBuffer *buffer) = 0;

        //////////////////////////////////////////
        ///\brief
        ///Called when the whole track has been analyzed.
        ///If this call makes changes to the track it should
        ///return true.
        //////////////////////////////////////////
        virtual bool    End(musik::core::ITrack *track) = 0;
};

//////////////////////////////////////////////////////////////////////////////
} } }
//////////////////////////////////////////////////////////////////////////////
