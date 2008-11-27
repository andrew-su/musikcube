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

#include "pch.hpp"
#include <cube/MetadataFilterModel.hpp>

#include <cube/MetadataFilterController.hpp>
#include <cube/BrowseController.hpp>
#include <win32cpp/Utility.hpp>
#include <boost/algorithm/string.hpp>

using namespace musik::cube;

//////////////////////////////////////////////////////////////////////////////

/*ctor*/    MetadataFilterModel::MetadataFilterModel(MetadataFilterController *controller) : controller(controller)
{
    this->controller->parent->selectionQuery.OnMetadataEvent(this->controller->metadataKey.c_str()).connect(this,&MetadataFilterModel::OnMetadata);

    this->SetRowCount(1);
}

uistring    MetadataFilterModel::CellValueToString(int rowIndex, ListView::ColumnRef column)
{
    typedef boost::basic_format<uichar> format;

    if(rowIndex==0){
        return (format(_T("All (%1% %2%)")) % this->metadata.size() % this->controller->metadataKey).str();
    }

    if(rowIndex<=this->metadataFiltered.size()){
        return win32cpp::Escape(this->metadataFiltered[rowIndex-1]->value);
    }else{
        return uistring();
    }
}

void        MetadataFilterModel::OnMetadata(musik::core::MetadataValueVector* metadata,bool clear){
    if(clear){
        this->SetRowCount(0);
        this->metadata          = *metadata;
        this->metadataFiltered  = *metadata;
        this->filter.clear();
    }else{
        this->metadata.insert(this->metadata.end(),metadata->begin(),metadata->end());
        this->metadataFiltered.insert(this->metadataFiltered.end(),metadata->begin(),metadata->end());
    }
    this->SetRowCount((int)this->metadataFiltered.size()+1);
    this->InvalidateData(0);    // Invalidate the "All" count
}

void MetadataFilterModel::OnChar(wchar_t key){
    if(key){
        if(key==8){
            this->filter.resize(this->filter.size()-1);
        }else{
            this->filter    += key;
        }

        // Lets filter
        this->metadataFiltered.clear();
        for(musik::core::MetadataValueVector::iterator meta=this->metadata.begin();meta!=this->metadata.end();++meta){
            if(boost::ifind_first( (*meta)->value, this->filter) ){
                this->metadataFiltered.push_back(*meta);
            }
        }
    }
    this->SetRowCount(0);
    this->SetRowCount((int)this->metadataFiltered.size()+1);

    this->controller->SelectAllFiltered((int)this->metadataFiltered.size()+1);

}
