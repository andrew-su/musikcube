//////////////////////////////////////////////////////////////////////////////
//
// License Agreement:
//
// The following are Copyright � 2008, Daniel �nnerby
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

#include "pch.hpp"

#include <core/Query/Base.h>
#include <core/Library/Base.h>
#include <core/xml/ParserNode.h>
#include <core/xml/WriterNode.h>

using namespace musik::core;

Query::Base::Base(void) 
:status(0)
,options(0)
,uniqueId(0)
{
    // This will guarantee that the query id is uniq for each query, but copies will not.
    // This is usefull when canceling similar queries
    static unsigned int uniqueQueryId(0);
    uniqueQueryId++;
    this->queryId   = uniqueQueryId;

}

Query::Base::~Base(void){
}

//////////////////////////////////////////
///\brief
///Receive the query from XML
///
///\param queryNode
///Reference to query XML node
///
///\returns
///true when successfully received
//////////////////////////////////////////
bool Query::Base::ReceiveQuery(musik::core::xml::ParserNode &queryNode){
    return false;
}

//////////////////////////////////////////
///\brief
///Serialize query to XML
///
///\param queryNode
///Reference to query XML node
///
///\returns
///true when successfully send
//////////////////////////////////////////
bool Query::Base::SendQuery(musik::core::xml::WriterNode &queryNode){
    return false;
}

//////////////////////////////////////////
///\brief
///Receive results from XML
///
///\param queryNode
///Reference to query XML node
///
///\returns
///true when successfully received
//////////////////////////////////////////
bool Query::Base::ReceiveResults(musik::core::xml::ParserNode &queryNode,Library::Base *library){
    return false;
}

//////////////////////////////////////////
///\brief
///Serialize results to XML
///
///\param queryNode
///Reference to query XML node
///
///\returns
///true when successfully send
//////////////////////////////////////////
bool Query::Base::SendResults(musik::core::xml::WriterNode &queryNode,Library::Base *library){
    return false;
}

std::string Query::Base::Name(){
    return "Unknown";
}

void Query::Base::PostCopy(){
    static unsigned int uniqueQueryId(0);
    uniqueQueryId++;
    this->uniqueId  = uniqueQueryId;
}

