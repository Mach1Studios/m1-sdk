#pragma once

#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

#include <math.h>

#include "2094_common_definitions.xml.h"
#include "xml/pugixml.hpp"

#ifndef PI
#    define PI 3.14159265358979323846f
#endif

using namespace std;

class ADMParser {

  private:
    struct ADMBlock {
        std::string id;
        float rtime = 0;
        float duration = 0;
        float x = 0;
        float y = 0;
        float z = 0;
    };

    struct ADMAudioChannel {
        std::string id;
        std::vector<ADMBlock> blocks;
        bool valid;
    };

    struct ADMAudioObject {
        std::string id;
        std::vector<std::string> audioTracks;
        std::vector<ADMAudioChannel> audioChannels;
    };

    struct ADMAudioContent {
        std::string id;
        std::vector<ADMAudioObject> audioObjects;
    };

    struct ADMAudioProgramme {
        std::string id;
        std::vector<ADMAudioContent> audioContents;
    };

    struct ADMDocument {
        std::vector<ADMAudioProgramme> audioProgrammes;
    };

    static float parseTimeString(std::string str) {
        float t = 0;
        std::smatch m;
        std::regex_search(str, m, std::regex("([0-9]{2}):([0-9]{2}):([0-9]{2}\\.[0-9]+)"));
        if (m.size() == 4) {
            t = 60 * 60 * std::stof(m[1]) + 60 * std::stof(m[2]) + std::stof(m[3]);
        }
        return t;
    }

    static bool FindAudioChannel(pugi::xml_document &doc, string rootTag, string audioPackFormatIDRef, vector<ADMAudioChannel> &admAudioChannels) {
        bool bFound = false;

        for (pugi::xml_node xmlAudioPackFormat : doc.child(rootTag.c_str()).child("coreMetadata").child("format").child("audioFormatExtended").children("audioPackFormat")) {
            if (xmlAudioPackFormat.attribute("audioPackFormatID").as_string() == audioPackFormatIDRef) {
                for (pugi::xml_node xmlAudioChannelFormatIDRef : xmlAudioPackFormat.children("audioChannelFormatIDRef")) {
                    string audioChannelFormatIDRef = xmlAudioChannelFormatIDRef.text().as_string();
                    for (pugi::xml_node xmlAudioChannelFormat : doc.child(rootTag.c_str()).child("coreMetadata").child("format").child("audioFormatExtended").children("audioChannelFormat")) {
                        if (xmlAudioChannelFormat.attribute("audioChannelFormatID").as_string() == audioChannelFormatIDRef) {
                            ADMAudioChannel admAudioChannel;
                            admAudioChannel.id = audioChannelFormatIDRef;

                            int typeLabel = xmlAudioChannelFormat.attribute("typeLabel").as_int();
                            if (typeLabel == 1 || typeLabel == 3) {
                                admAudioChannel.valid = true;

                                for (pugi::xml_node xmlAudioBlockFormat : xmlAudioChannelFormat.children("audioBlockFormat")) {

                                    ADMBlock admBlock;
                                    admBlock.id = xmlAudioBlockFormat.attribute("audioBlockFormatID").as_string();
                                    admBlock.rtime = parseTimeString(xmlAudioBlockFormat.attribute("rtime").as_string());
                                    admBlock.duration = parseTimeString(xmlAudioBlockFormat.attribute("duration").as_string());

                                    bool cartesian = xmlAudioBlockFormat.child("cartesian").text().as_int();
                                    if (cartesian) {
                                        for (pugi::xml_node position : xmlAudioBlockFormat.children("position")) {
                                            std::string coordinate = position.attribute("coordinate").as_string();
                                            float val = position.text().as_float();
                                            if (coordinate == "X") {
                                                admBlock.x = val;
                                            } else if (coordinate == "Y") {
                                                admBlock.y = val;
                                            } else if (coordinate == "Z") {
                                                admBlock.z = val;
                                            }
                                            // std::cout << "position: " << coordinate << " = " << position.text().as_float() << "\n";
                                        }
                                    } else {
                                        float azimuth = 0;
                                        float elevation = 0;
                                        float distance = 0;
                                        for (pugi::xml_node position : xmlAudioBlockFormat.children("position")) {
                                            std::string coordinate = position.attribute("coordinate").as_string();
                                            float val = position.text().as_float();
                                            if (coordinate == "azimuth") {
                                                azimuth = val;
                                            } else if (coordinate == "elevation") {
                                                elevation = val;
                                            } else if (coordinate == "distance") {
                                                distance = val;
                                            }
                                            // std::cout << "position: " << coordinate << " = " << position.text().as_float() << "\n";
                                        }
                                        admBlock.x = distance * cos(elevation * PI / 180) * cos(azimuth * PI / 180);
                                        admBlock.y = distance * cos(elevation * PI / 180) * sin(azimuth * PI / 180);
                                        admBlock.z = distance * sin(elevation * PI / 180);
                                    }
                                    admAudioChannel.blocks.push_back(admBlock);
                                }
                            } else {
                                admAudioChannel.valid = false;
                            }
                            admAudioChannels.push_back(admAudioChannel);
                            bFound = true;
                        }
                    }
                }
            }
        }
        return bFound;
    }

    ADMDocument admDocument;

  public:
    struct KeyPoint {
        long long sample;
        float x;
        float y;
        float z;

        bool operator<(const KeyPoint &b) const {
            return sample < b.sample;
        }

        KeyPoint() {
            sample = 0;
            x = 0;
            y = 0;
            z = 0;
        }

        KeyPoint(long long sample, float x, float y, float z) {
            this->sample = sample;
            this->x = x;
            this->y = y;
            this->z = z;
        }
    };

    typedef std::map<std::string, std::vector<KeyPoint>> AudioTracks;

    bool sessionDataFound = false;

    bool findSessionDetails(pugi::xml_document &doc) {
        pugi::xml_node xmlAudioSessionTitle = doc.child("ebuCoreMain").child("coreMetadata").child("title");
        std::string dctitle = xmlAudioSessionTitle.child("dc:title").text().as_string();
        std::cout << "Title: " << dctitle << "\n";

        pugi::xml_node xmlAudioSessionCreator = doc.child("ebuCoreMain").child("coreMetadata").child("creator");
        std::string organizationName = xmlAudioSessionCreator.child("organisationDetails").child("organisationName").text().as_string();
        std::cout << "Creator: " << organizationName << "\n";

        pugi::xml_node xmlAudioSessionDescription = doc.child("ebuCoreMain").child("coreMetadata").child("description");
        std::string dcdesc = xmlAudioSessionDescription.child("dc:description").text().as_string();
        std::cout << "Description: " << dcdesc << "\n";

        pugi::xml_node xmlAudioSessionDate = doc.child("ebuCoreMain").child("coreMetadata").child("date").child("created");
        std::string createdDate = xmlAudioSessionDate.attribute("startDate").as_string();
        std::cout << "Date: " << createdDate << "\n";
        sessionDataFound = true;

        return sessionDataFound;
    }

    bool ParseString(const char *string, AudioTracks &audioTracks) {
        pugi::xml_document doc;

        if (doc.load_string(string)) {
            return ParseXMLDoc(doc, audioTracks);
        }

        return false;
    }

    bool ParseFile(const char *filename, AudioTracks &audioTracks) {
        pugi::xml_document doc;

        if (doc.load_file(filename)) {
            return ParseXMLDoc(doc, audioTracks);
        }

        return false;
    }

    bool ParseXMLDoc(pugi::xml_document &doc, AudioTracks &audioTracks) {

        pugi::xml_document docCommonDefinitions;

        if (docCommonDefinitions.load_buffer(COMMON_DEFINITIONS_XML, COMMON_DEFINITIONS_XML_SIZE, pugi::encoding_auto)) {
            admDocument = ADMDocument();

            if (!sessionDataFound) {
                findSessionDetails(doc);
            }

            for (pugi::xml_node xmlAudioProgramme : doc.child("ebuCoreMain").child("coreMetadata").child("format").child("audioFormatExtended").children("audioProgramme")) {
                ADMAudioProgramme admAudioProgramme;
                admAudioProgramme.id = xmlAudioProgramme.attribute("audioProgrammeID").as_string();

                for (pugi::xml_node xmlAudioContentIDRef : xmlAudioProgramme.children("audioContentIDRef")) {
                    string audioContentIDRefID = xmlAudioContentIDRef.text().as_string();
                    for (pugi::xml_node xmlAudioContent : doc.child("ebuCoreMain").child("coreMetadata").child("format").child("audioFormatExtended").children("audioContent")) {
                        if (xmlAudioContent.attribute("audioContentID").as_string() == audioContentIDRefID) {
                            ADMAudioContent admAudioContent;
                            admAudioContent.id = audioContentIDRefID;

                            for (pugi::xml_node xmlAudioObjectIDRef : xmlAudioContent.children("audioObjectIDRef")) {
                                string xmlAudioObjectIDRefValue = xmlAudioObjectIDRef.text().as_string();
                                for (pugi::xml_node xmlAudioObject : doc.child("ebuCoreMain").child("coreMetadata").child("format").child("audioFormatExtended").children("audioObject")) {
                                    if (xmlAudioObject.attribute("audioObjectID").as_string() == xmlAudioObjectIDRefValue) {
                                        ADMAudioObject admAudioObject;
                                        admAudioObject.id = xmlAudioObjectIDRefValue;

                                        for (pugi::xml_node xmlAudioTrackUIDRef : xmlAudioObject.children("audioTrackUIDRef")) {
                                            admAudioObject.audioTracks.push_back(xmlAudioTrackUIDRef.text().as_string());
                                        }

                                        string audioPackFormatIDRef = xmlAudioObject.child("audioPackFormatIDRef").text().as_string();

                                        // find audio channel
                                        if (!FindAudioChannel(doc, "ebuCoreMain", audioPackFormatIDRef, admAudioObject.audioChannels)) {
                                            FindAudioChannel(docCommonDefinitions, "ituADM", audioPackFormatIDRef, admAudioObject.audioChannels);
                                        }
                                        admAudioContent.audioObjects.push_back(admAudioObject);
                                    }
                                }
                            }
                            admAudioProgramme.audioContents.push_back(admAudioContent);
                        }
                    }
                }
                admDocument.audioProgrammes.push_back(admAudioProgramme);
            }

            std::map<std::string, long long> audioTracksSamplerates;
            for (pugi::xml_node audioFormatExtended : doc.child("ebuCoreMain").child("coreMetadata").child("format").child("audioFormatExtended")) {
                for (pugi::xml_node xmlAudioTrackUID : audioFormatExtended.children("audioTrackUID")) {
                    string audioTrackUID = xmlAudioTrackUID.attribute("UID").as_string();
                    audioTracksSamplerates[audioTrackUID] = xmlAudioTrackUID.attribute("sampleRate").as_llong();
                }
            }

            audioTracks.clear();
            for (auto audioProgramme : admDocument.audioProgrammes) {
                for (auto audioContent : audioProgramme.audioContents) {
                    for (auto audioObject : audioContent.audioObjects) {

                        if (audioObject.audioChannels.size() == audioObject.audioTracks.size()) { // skip object with not equal channels and tracks count
                            for (int i = 0; i < audioObject.audioChannels.size(); i++) {
                                for (int j = 0; j < audioObject.audioChannels[i].blocks.size(); j++) {
                                    ADMParser::ADMBlock block = audioObject.audioChannels[i].blocks[j];

                                    if (audioTracks.find(audioObject.audioTracks[i]) == audioTracks.end()) {
                                        audioTracks[audioObject.audioTracks[i]] = std::vector<KeyPoint>();
                                    }

                                    ADMParser::KeyPoint point;
                                    point.x = block.x;
                                    point.y = block.z;
                                    point.z = block.y;
                                    point.sample = (long long)(block.rtime * audioTracksSamplerates[audioObject.audioTracks[i]]);
                                    audioTracks[audioObject.audioTracks[i]].push_back(point);
                                }
                            }
                        }
                    }
                }
            }

            for (auto &audioTrack : audioTracks) {
                std::sort(audioTrack.second.begin(), audioTrack.second.end());
            }

            return true;
        }

        return false;
    }
};
