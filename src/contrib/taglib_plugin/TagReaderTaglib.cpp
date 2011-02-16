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

#include "TagReaderTaglib.h"

#ifndef _HAVE_TAGLIB
#include <toolkit/tlist.h>
#include <toolkit/tfile.h>

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/audioproperties.h>

#include <mpeg/mpegfile.h>

#include <mpeg/id3v1/id3v1tag.h>
#include <mpeg/id3v1/id3v1genres.h>

#include <mpeg/id3v2/id3v2tag.h>
#include <mpeg/id3v2/id3v2header.h>
#include <mpeg/id3v2/id3v2frame.h>
#include <mpeg/id3v2/frames/attachedpictureframe.h>
#include <mpeg/id3v2/frames/commentsframe.h>

#include <taglib/ogg/oggfile.h>
#else //_HAVE_TAGLIB
#include <taglib/toolkit/tlist.h>
#include <taglib/toolkit/tfile.h>

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/audioproperties.h>

#include <taglib/mpeg/mpegfile.h>

#include <taglib/mpeg/id3v1/id3v1tag.h>
#include <taglib/mpeg/id3v1/id3v1genres.h>

#include <taglib/mpeg/id3v2/id3v2tag.h>
#include <taglib/mpeg/id3v2/id3v2header.h>
#include <taglib/mpeg/id3v2/id3v2frame.h>
#include <taglib/mpeg/id3v2/frames/attachedpictureframe.h>
#include <taglib/mpeg/id3v2/frames/commentsframe.h>

#include <taglib/ogg/oggfile.h>
#endif //_HAVE_TAGLIB

#include <vector>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


// Create a instance

TagReaderTaglib::TagReaderTaglib(void){
}

TagReaderTaglib::~TagReaderTaglib(void){
}

void TagReaderTaglib::Destroy() {
    delete this;
}

bool TagReaderTaglib::CanReadTag(const utfchar *extension){

    if(extension){
		std::wstring ext(extension);

        boost::algorithm::to_lower(ext);    // Convert to lower case

        if(	ext.compare(UTF("mp3")) == 0 ||
            ext.compare(UTF("ogg")) == 0 ||
            ext.compare(UTF("flac")) == 0 ||
            ext.compare(UTF("ape")) == 0 ||
            ext.compare(UTF("mpc")) == 0
                ) {
		    return true;
	    }
    }

	return false;

}

bool TagReaderTaglib::ReadTag(musik::core::ITrack *track){


    const utfchar *extension    = track->GetValue("extension");
    if(extension){
    	utfstring ext(extension);
        boost::algorithm::to_lower(ext);    // Convert to lower case

        if(ext==UTF("mp3"))
            if(this->GetID3v2Tag(track)){
                // Get the generic tag as well, just in case there is only a id3v1 tag.
                this->GetGenericTag(track);
                return true;
            }

        if(ext==UTF("ogg"))
            if(this->GetOGGTag(track))
                return true;

    }

    if(this->GetGenericTag(track))   // secondary: try generic way
        return true;

    return false;
}

bool TagReaderTaglib::GetOGGTag(musik::core::ITrack *track){
/*
    TagLib::Ogg::File file(track->GetValue("path"));
*/
    return false;
}


bool TagReaderTaglib::GetGenericTag(musik::core::ITrack *track){
    TagLib::FileRef oFile(track->URL());
    if(!oFile.isNull()){
        TagLib::Tag *tag	= oFile.tag();
	    TagLib::AudioProperties *oAudioProperties	= oFile.audioProperties();
	    if(tag){
            // TITLE
            if(!tag->title().isEmpty()){
			    this->SetTagValue("title",tag->title(),track);
		    }else{
			    this->SetTagValue("title",track->GetValue("filename"),track);
		    }

            // ALBUM
		    this->SetTagValue("album",tag->album(),track);
            // ARTISTS
            this->SetSlashSeparatedValues("artist",tag->artist(),track);
            // GENRES
            this->SetTagValue("genre",tag->genre(),track);
            // COMMENT
            this->SetTagValue("comment",tag->comment(),track);
            // TRACK
            if(tag->track()){
                this->SetTagValue("track",tag->track(),track);
            }
            // TRACK
            if(tag->year()){
                this->SetTagValue("year",tag->year(),track);
            }

            this->SetAudioProperties(oAudioProperties,track);

            return true;
        }
    }

    return false;

}


bool TagReaderTaglib::GetID3v2Tag(musik::core::ITrack *track){
    #ifdef UTF_WIDECHAR
        TagLib::ID3v2::FrameFactory::instance()->setDefaultTextEncoding(TagLib::String::UTF16);
    #else
    	TagLib::ID3v2::FrameFactory::instance()->setDefaultTextEncoding(TagLib::String::UTF8);
    #endif

    TagLib::MPEG::File oFile(track->URL());
	TagLib::ID3v2::Tag *oTagv2	= oFile.ID3v2Tag();
	if(oTagv2){

        TagLib::AudioProperties *oAudioProperties	= oFile.audioProperties();
		// Successfully found ID3V2-tag

		//////////////////////////////////////////////////////////////////////////////
		// Get ALL tags in a map
		TagLib::ID3v2::FrameListMap aAllTags	= oTagv2->frameListMap();

		if(!oTagv2->title().isEmpty()){
			this->SetTagValue("title",oTagv2->title(),track);
		}
		this->SetTagValue("album",oTagv2->album(),track);

		//////////////////////////////////////////////////////////////////////////////
		// YEAR
		if( !aAllTags["TYER"].isEmpty() )		// ID3 2.3
			this->SetTagValue("year",aAllTags["TYER"].front()->toString().substr(0,4),track);
		if( !aAllTags["TDRC"].isEmpty() )		// ID3 2.4
			this->SetTagValue("year",aAllTags["TDRC"].front()->toString().substr(0,4),track);


        // Split TRCK to track and totaltracks
        std::vector<utfstring> splitTrack;
        if(!aAllTags["TRCK"].isEmpty()){
        	TagLib::wstring tempTrack = aAllTags["TRCK"].front()->toString().toWString();
            boost::algorithm::split(splitTrack,tempTrack,boost::algorithm::is_any_of(UTF("/")));
            this->SetTagValue("track",splitTrack[0].c_str(),track);
            if(splitTrack.size()>1){
                this->SetTagValue("totaltracks",splitTrack[1].c_str(),track);
            }
        }

		this->SetTagValues("bpm",aAllTags["TBPM"],track);
		this->SetSlashSeparatedValues("composer",aAllTags["TCOM"],track);
		this->SetTagValues("copyright",aAllTags["TCOP"],track);
		this->SetTagValues("encoder",aAllTags["TENC"],track);
		this->SetTagValues("writer",aAllTags["TEXT"],track);
		this->SetTagValues("org.writer",aAllTags["TOLY"],track);
		this->SetSlashSeparatedValues("publisher",aAllTags["TPUB"],track);
		this->SetTagValues("mood",aAllTags["TMOO"],track);
		this->SetSlashSeparatedValues("org.artist",aAllTags["TOPE"],track);
		this->SetTagValues("language",aAllTags["TLAN"],track);
		this->SetTagValues("disc",aAllTags["TPOS"],track);
		this->SetTagValues("lyrics",aAllTags["USLT"],track);

		//////////////////////////////////////////////////////////////////////////////
		// GENRES
		if(!aAllTags["TCON"].isEmpty()){
			TagLib::ID3v2::FrameList genres	= aAllTags["TCON"];
			for(TagLib::ID3v2::FrameList::ConstIterator genreFrame=genres.begin();genreFrame!=genres.end();++genreFrame){
				TagLib::String genreString	= (*genreFrame)->toString();
				if( !genreString.isEmpty() ){
					// There is no need to worry about duplicates, since the Track will not save duplicates.
					// TODO: This should also handle old ID3v1 genre numbers
                    int numberLength(0);
                    bool isNumber(true);
                    for(TagLib::String::ConstIterator charIt=genreString.begin();isNumber && charIt!=genreString.end();++charIt){
                        isNumber = *charIt >= '0' && *charIt <= '9';
                        if(isNumber){
                            ++numberLength;
                        }
                    }

                    if(!isNumber && numberLength>0){
                        // Genre starting with a number
                        if(genreString.substr(numberLength,1)==" "){
                            // If the genre number is appended by a space, then this is a id3v1 genre number
                            int genreNumber = genreString.substr(0,numberLength).toInt();
                            if(genreNumber>= 0 && genreNumber<= 255)
                                this->SetTagValue("genre",TagLib::ID3v1::genre(genreNumber),track);

                            // Remove the prepending genre number
                            genreString = genreString.substr(numberLength+1);
                        }
                    }

                    if(isNumber){
                        // If the genre is only a number, look for id3v1 genres
                        int genreNumber = genreString.toInt();
                        if(genreNumber>= 0 && genreNumber<= 255)
                            genreString = TagLib::ID3v1::genre(genreNumber);
                    }

    				if( !genreString.isEmpty() ){
	    			    this->SetTagValue("genre",genreString,track);
                    }

                }
			}
		}

		//////////////////////////////////////////////////////////////////////////////
		// ARTISTS
		this->SetSlashSeparatedValues("artist",aAllTags["TPE1"],track);
		this->SetSlashSeparatedValues("artist",aAllTags["TPE2"],track);
		this->SetSlashSeparatedValues("conductor",aAllTags["TPE3"],track);
		this->SetSlashSeparatedValues("interpreted",aAllTags["TPE4"],track);

		//////////////////////////////////////////////////////////////////////////////
		// Audio properties
        this->SetAudioProperties(oAudioProperties,track);

		//////////////////////////////////////////////////////////////////////////////
		// Comments
		TagLib::ID3v2::FrameList	comments	= aAllTags["COMM"];
		for(TagLib::ID3v2::FrameList::Iterator commentFrame=comments.begin();commentFrame!=comments.end();++commentFrame){
			TagLib::ID3v2::CommentsFrame *comment	= dynamic_cast<TagLib::ID3v2::CommentsFrame*> (*commentFrame);

			TagLib::String temp	= comment->description();
			std::wstring description(temp.begin(),temp.end());

			if(description.empty()){
				this->SetTagValue("comment",comment->toString(),track);
			}else if(description.compare(UTF("MusicMatch_Mood")) == 0){
				this->SetTagValue("mood",comment->toString(),track);
			}else if(description.compare(UTF("MusicMatch_Preference")) == 0){
				this->SetTagValue("textrating",comment->toString(),track);
			}
		}

		//////////////////////////////////////////////////////////////////////////////
		// Thumbnail
		TagLib::ID3v2::FrameList pictures	= aAllTags["APIC"];
		if(!pictures.isEmpty()){
			// Thumbnail exists
			// Just get the front() picture
			TagLib::ID3v2::AttachedPictureFrame *picture = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(pictures.front());
			TagLib::ByteVector pictureData	= picture->picture();
			DBINT size	= pictureData.size();

			if(size>32){	// Noticed that some id3tags have like a 4-8 byte size with no thumbnail
				char *data	= pictureData.data();
				track->SetThumbnail(data,size);
			}

		}


		return true;
	}
/*	}

	// Try the "generic" way
	TagLib::FileRef	oFile(oMedia->sFileFullPath.c_str());
	TagLib::Tag		*oTag	= oFile.tag();
	TagLib::AudioProperties *oAudioProperties	= oFile.audioProperties();
	if(oTag && oAudioProperties){
		// Successfully found tags
		return this->getStandardTags(oMedia,oTag,oAudioProperties);
	}

	//oFile.audioProperties();
*/
	return false;
}

void TagReaderTaglib::SetTagValue(const char* key,const TagLib::String tagString,musik::core::ITrack *track){
	using namespace musik::core;
	std::wstring value(tagString.begin(),tagString.end());
    track->SetValue(key,UTF16_TO_UTF(value).c_str());
}

void TagReaderTaglib::SetTagValue(const char* key,const wchar_t* string,musik::core::ITrack *track){
	using namespace musik::core;
	std::wstring temp (string);
	track->SetValue(key,temp.c_str());
}

void TagReaderTaglib::SetTagValue(const char* key,const int tagInt,musik::core::ITrack *track){
    std::wstring temp = boost::str(boost::wformat(UTF("%1%"))%tagInt);
    track->SetValue(key,temp.c_str());
}


void TagReaderTaglib::SetTagValues(const char* key,const TagLib::ID3v2::FrameList &frame,musik::core::ITrack *track){
	using namespace musik::core;
	if(!frame.isEmpty()){
		for(TagLib::ID3v2::FrameList::ConstIterator value=frame.begin();value!=frame.end();++value){
			TagLib::String tagString	= (*value)->toString();
			if( !tagString.isEmpty() ){
				std::wstring value(tagString.begin(),tagString.end());
                track->SetValue(key,UTF16_TO_UTF(value).c_str());
			}
		}
	}
}

/*
bool TagReaderTaglib::getStandardTags(musik::core::ITrack &track,TagLib::Tag *oTag,TagLib::AudioProperties *oAudioProperties){
	oMedia->setArtist(oTag->artist().toCString(false));
	oMedia->setAlbum(oTag->album().toCString(false));
	oMedia->setGenre(oTag->genre().toCString(false));
	oMedia->setTitle(oTag->title().toCString(false));
	oMedia->iTrack		= oTag->track();
	oMedia->iYear		= oTag->year();
	oMedia->setComment(oTag->comment().toCString(false));

	if(oAudioProperties){
		oMedia->iDuration	= oAudioProperties->length();
	}

	return true;
}
*/
void TagReaderTaglib::SetSlashSeparatedValues(const char* key,TagLib::String tagString,musik::core::ITrack *track){
	if( !tagString.isEmpty() ){
		std::wstring value(tagString.begin(),tagString.end());
		std::vector<std::wstring> splitValues;

		boost::algorithm::split(splitValues,value,boost::algorithm::is_any_of(UTF("/")));

		for(std::vector<std::wstring>::iterator theValue=splitValues.begin();theValue!=splitValues.end();++theValue){
            track->SetValue(key,theValue->c_str());
		}
	}
}

void TagReaderTaglib::SetSlashSeparatedValues(const char* key,const TagLib::ID3v2::FrameList &frame,musik::core::ITrack *track){
	using namespace musik::core;
	if(!frame.isEmpty()){
		for(TagLib::ID3v2::FrameList::ConstIterator frameValue=frame.begin();frameValue!=frame.end();++frameValue){
			TagLib::String tagString	= (*frameValue)->toString();
/*			if( !tagString.isEmpty() ){
				std::wstring value(tagString.begin(),tagString.end());
				std::vector<std::wstring> splitValues;

				boost::algorithm::split(splitValues,value,boost::algorithm::is_any_of(_T("/")));

				for(std::vector<std::wstring>::iterator theValue=splitValues.begin();theValue!=splitValues.end();++theValue){
                    track->SetValue(key,theValue->c_str());
				}
			}*/
            this->SetSlashSeparatedValues(key,tagString,track);
		}
	}
}

void TagReaderTaglib::SetAudioProperties(TagLib::AudioProperties *audioProperties,musik::core::ITrack *track){
	if(audioProperties){
		std::wstring duration	= boost::str(boost::wformat(UTF("%1%"))%audioProperties->length());
		this->SetTagValue("duration",duration,track);

        int bitrate( audioProperties->bitrate() );
        if(bitrate){
			std::wstring temp( boost::str(boost::wformat(UTF("%1%"))%bitrate ) );
			this->SetTagValue("bitrate",temp,track);
        }

        int channels( audioProperties->channels() );
        if(channels){
			std::wstring temp( boost::str(boost::wformat(UTF("%1%"))%channels ) );
			this->SetTagValue("channels",temp,track);
        }
	}
}


