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
#include <core/xml/Parser.h>
#include <core/xml/ParserNode.h>
#include <expat/expat.h>

#include <fstream>

using namespace musik::core::xml;

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////
///\brief
///Constructor
///
///\param socket
///pointer to boost::asio socket to be used by the Parser
///
///The Parser will continously parse XML and 
///read from the socket when there is no buffer
///left in the parser.
//////////////////////////////////////////
Parser::Parser(boost::asio::ip::tcp::socket *socket)
 :level(0)
 ,xmlParser(NULL)
 ,socket(socket)
 ,xmlParserStatus(XML_Status::XML_STATUS_OK)
 ,readBufferLength(0)
 ,currentEventType(0)
 ,exit(false)
{
    // Set node stuff
    this->parser        = this;

    this->xmlParser    = XML_ParserCreate(NULL);
    XML_SetUserData(this->xmlParser,this);
    XML_SetElementHandler(this->xmlParser,&Parser::OnElementStart,&Parser::OnElementEnd);
    XML_SetCharacterDataHandler(this->xmlParser,&Parser::OnContent);
}

//////////////////////////////////////////////////////////////////////////////

Parser::~Parser(void){
    XML_ParserFree(this->xmlParser);
}

//////////////////////////////////////////////////////////////////////////////

void Parser::OnElementStart(void *thisobject,const char *name, const char **atts){
    ((Parser*)thisobject)->OnElementStartReal(name,atts);
}
void Parser::OnElementStartReal(const char *name, const char **atts){
    this->xmlFound  = true;
    this->level++;
    // First, lets create the new Node and add to the nodeLevels
    Node::Ptr node(new Node());
    node->name    = name;

    // fix attributes
    for (int i(0);atts[i];i+=2){
        node->attributes[atts[i]] = atts[i + 1];
    }

    // Set the parent node
    if(!this->currentNodeLevels.empty()){
        node->parent    = this->currentNodeLevels.back();
    }
    this->currentNodeLevels.push_back(node);


    this->currentEventType  = EventTypes::NodeStart;
    XML_StopParser(this->xmlParser,true);
}

//////////////////////////////////////////////////////////////////////////////

void Parser::OnElementEnd(void *thisobject,const char *name){
    ((Parser*)thisobject)->OnElementEndReal(name);
}

void Parser::OnElementEndReal(const char *name){
    this->xmlFound  = true;
    this->level--;
    if(this->currentNodeLevels.size()>0){
        if(this->currentNodeLevels.back()->name == name){

            this->currentNodeLevels.back()->status   |= Node::Status::Ended;
            this->currentNodeLevels.pop_back();

            this->currentEventType  = EventTypes::NodeEnd;
            XML_StopParser(this->xmlParser,true);
        }else{
            // Wrong endtag expected, mallformated input
            this->Exit();
            XML_StopParser(this->xmlParser,true);
        }
    }else{
        // below root level, mallformated input
        this->Exit();
        XML_StopParser(this->xmlParser,true);
    }
}

//////////////////////////////////////////////////////////////////////////////

void Parser::OnContent(void *thisobject,const char *content,int length){
    ((Parser*)thisobject)->OnContentReal(content,length);
}

void Parser::OnContentReal(const char *content,int length){
    if(this->currentNodeLevels.size()>0){
        this->currentNodeLevels.back()->content.append(content,length);
    }
}

//////////////////////////////////////////////////////////////////////////////

void Parser::ReadFromSocket(){
    boost::system::error_code error;

    // This is a likely place where a thread will wait
    this->readBufferLength  = this->socket->read_some(boost::asio::buffer(this->readBuffer),error);

    if(error){
        // Connection closed or some other error occured
        this->Exit();
    }

    // Log
//    std::ofstream logFile("mc2_Parser.log",std::ios::app);
//    logFile.write(this->readBuffer.c_array(),this->readBufferLength);
//    logFile << std::endl;

}

//////////////////////////////////////////////////////////////////////////////

void Parser::ContinueParsing(){
    this->xmlFound  = false;
    while(!this->xmlFound && !this->exit){
        switch(this->xmlParserStatus){
	        case XML_Status::XML_STATUS_SUSPENDED:
		        this->xmlParserStatus	= XML_ResumeParser(this->xmlParser);
		        break;
	        case XML_Status::XML_STATUS_OK:
		        this->ReadFromSocket();
		        this->xmlParserStatus	= XML_Parse(this->xmlParser,this->readBuffer.c_array(),(int)this->readBufferLength,0);
		        break;
            case XML_Status::XML_STATUS_ERROR:
                this->Exit();
                break;
        }
    }
}

void Parser::Exit(){
    this->exit  = true;
    for(std::vector<Node::Ptr>::iterator node=this->currentNodeLevels.begin();node!=this->currentNodeLevels.end();++node){
        (*node)->status  = Node::Status::Ended;
    }
}


//////////////////////////////////////////////////////////////////////////////

std::string Parser::CurrentNodeLevelPath(bool getParent){
    std::string nodeLevels;
    bool first(true);
    int i(0);
    for(std::vector<Node::Ptr>::iterator node=this->currentNodeLevels.begin();node!=this->currentNodeLevels.end();++node){
        ++i;
        if(!getParent || this->currentNodeLevels.size()!=i){
            if(first){
                first=false;
                nodeLevels += (*node)->name;
            }else{
                nodeLevels += "/"+(*node)->name;
            }
        }
    }
    return nodeLevels;
}

//////////////////////////////////////////////////////////////////////////////

Node::Ptr Parser::LastNode(){
    if(this->currentNodeLevels.empty()){
        return Node::Ptr();
    }
    return this->currentNodeLevels.back();
}


