#include "bw64/bw64.hpp"
#include <vector>

struct ChannelDescType {
    int channelType;
    // 0001 = `DirectSpeakers`
    // 0003 = `Objects`
};

// channel number description
bw64::ChnaChunk fillChnaChunkADMDesc(std::vector<ChannelDescType> channels) {
    bw64::ChnaChunk chnaChunkAdm;

    for (int i = 0; i < channels.size(); ++i){
        //TODO: fix for >9 int values turning into hex
        std::ostringstream ss;
        ss << std::hex << i+1;
        std::string i_hex = ss.str();
    
        uint16_t trackIndex = i+1;
        std::string uid = "ATU_0000000"+i_hex;
        std::string trackRef = "AT_000"+std::to_string(channels[i].channelType)+"100"+i_hex+"_01";
        std::string enumerationType = (channels[i].channelType == 1) ? "1" : i_hex; // if part of the channel-bed then use the first AP index, if an object enumerate appropriately
        std::string packRef = "AP_000"+std::to_string(channels[i].channelType)+"100"+enumerationType;
        chnaChunkAdm.addAudioId(bw64::AudioId(trackIndex, uid, trackRef, packRef));
    }
    return chnaChunkAdm;
}

// default 8 channel Mach1 Spatial cube object-bed description
const char* axml_m1spatial_ChunkAdmString = R"(<?xml version="1.0" encoding="UTF-8"?>
<ebuCoreMain xmlns="urn:ebu:metadata-schema:ebuCore_2016" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="urn:ebu:metadata-schema:ebuCore_2016 ebucore.xsd" xml:lang="en">
    <coreMetadata>
        <format>
            <audioFormatExtended >
                <audioProgramme audioProgrammeID="APR_1001" audioProgrammeName="ADM_Master" start="00:00:00.00000" end="hh:mm:ss.fffff">
                    <audioContentIDRef>ACO_1001</audioContentIDRef>
                </audioProgramme>
                <audioContent audioContentID="ACO_1001" audioContentName="Atmos_Master_Content">
                    <audioObjectIDRef>AO_1001</audioObjectIDRef>
                    <audioObjectIDRef>AO_1002</audioObjectIDRef>
                    <audioObjectIDRef>AO_1003</audioObjectIDRef>
                    <audioObjectIDRef>AO_1004</audioObjectIDRef>
                    <audioObjectIDRef>AO_1005</audioObjectIDRef>
                    <audioObjectIDRef>AO_1006</audioObjectIDRef>
                    <audioObjectIDRef>AO_1007</audioObjectIDRef>
                    <audioObjectIDRef>AO_1008</audioObjectIDRef>
                    <dialogue mixedContentKind="0">2</dialogue>
                </audioContent>
                <audioObject audioObjectID="AO_1001" audioObjectName="Atmos_Obj_1" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_00031001</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_00000001</audioTrackUIDRef>
                </audioObject>
                <audioObject audioObjectID="AO_1002" audioObjectName="Atmos_Obj_2" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_00031002</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_00000002</audioTrackUIDRef>
                </audioObject>
                <audioObject audioObjectID="AO_1003" audioObjectName="Atmos_Obj_3" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_00031003</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_00000003</audioTrackUIDRef>
                </audioObject>
                <audioObject audioObjectID="AO_1004" audioObjectName="Atmos_Obj_4" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_00031004</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_00000004</audioTrackUIDRef>
                </audioObject>
                <audioObject audioObjectID="AO_1005" audioObjectName="Atmos_Obj_5" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_00031005</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_00000005</audioTrackUIDRef>
                </audioObject>
                <audioObject audioObjectID="AO_1006" audioObjectName="Atmos_Obj_6" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_00031006</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_00000006</audioTrackUIDRef>
                </audioObject>
                <audioObject audioObjectID="AO_1007" audioObjectName="Atmos_Obj_7" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_00031007</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_00000007</audioTrackUIDRef>
                </audioObject>
                <audioObject audioObjectID="AO_1008" audioObjectName="Atmos_Obj_8" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_00031008</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_00000008</audioTrackUIDRef>
                </audioObject>
                <audioPackFormat audioPackFormatID="AP_00031001" audioPackFormatName="Atmos_Obj_1" typeDefinition="Objects" typeLabel="0003">
                    <audioChannelFormatIDRef>AC_00031001</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioPackFormat audioPackFormatID="AP_00031002" audioPackFormatName="Atmos_Obj_2" typeDefinition="Objects" typeLabel="0003">
                    <audioChannelFormatIDRef>AC_00031002</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioPackFormat audioPackFormatID="AP_00031003" audioPackFormatName="Atmos_Obj_3" typeDefinition="Objects" typeLabel="0003">
                    <audioChannelFormatIDRef>AC_00031003</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioPackFormat audioPackFormatID="AP_00031004" audioPackFormatName="Atmos_Obj_4" typeDefinition="Objects" typeLabel="0003">
                    <audioChannelFormatIDRef>AC_00031004</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioPackFormat audioPackFormatID="AP_00031005" audioPackFormatName="Atmos_Obj_5" typeDefinition="Objects" typeLabel="0003">
                    <audioChannelFormatIDRef>AC_00031005</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioPackFormat audioPackFormatID="AP_00031006" audioPackFormatName="Atmos_Obj_6" typeDefinition="Objects" typeLabel="0003">
                    <audioChannelFormatIDRef>AC_00031006</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioPackFormat audioPackFormatID="AP_00031007" audioPackFormatName="Atmos_Obj_7" typeDefinition="Objects" typeLabel="0003">
                    <audioChannelFormatIDRef>AC_00031007</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioPackFormat audioPackFormatID="AP_00031008" audioPackFormatName="Atmos_Obj_8" typeDefinition="Objects" typeLabel="0003">
                    <audioChannelFormatIDRef>AC_00031008</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioChannelFormat audioChannelFormatID="AC_00031001" audioChannelFormatName="Atmos_Obj_1" typeLabel="0003" typeDefinition="Objects">
                  <audioBlockFormat audioBlockFormatID="AB_00031001_00000001" rtime="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <speakerLabel>Ltf</speakerLabel>
                    <cartesian>1</cartesian>
                    <position coordinate="X">-1.0000000000</position>
                    <position coordinate="Y">1.0000000000</position>
                    <position coordinate="Z">1.0000000000</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00031002" audioChannelFormatName="Atmos_Obj_2" typeLabel="0003" typeDefinition="Objects">
                  <audioBlockFormat audioBlockFormatID="AB_00031002_00000001" rtime="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <speakerLabel>Rtf</speakerLabel>
                    <cartesian>1</cartesian>
                    <position coordinate="X">1.0000000000</position>
                    <position coordinate="Y">1.0000000000</position>
                    <position coordinate="Z">1.0000000000</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00031003" audioChannelFormatName="Atmos_Obj_3" typeLabel="0003" typeDefinition="Objects">
                  <audioBlockFormat audioBlockFormatID="AB_00031003_00000001" rtime="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <speakerLabel>Ltr</speakerLabel>
                    <cartesian>1</cartesian>
                    <position coordinate="X">-1.0000000000</position>
                    <position coordinate="Y">-1.0000000000</position>
                    <position coordinate="Z">1.0000000000</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00031004" audioChannelFormatName="Atmos_Obj_4" typeLabel="0003" typeDefinition="Objects">
                  <audioBlockFormat audioBlockFormatID="AB_00031004_00000001" rtime="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <speakerLabel>Rtr</speakerLabel>
                    <cartesian>1</cartesian>
                    <position coordinate="X">1.0000000000</position>
                    <position coordinate="Y">-1.0000000000</position>
                    <position coordinate="Z">1.0000000000</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00031005" audioChannelFormatName="Atmos_Obj_5" typeLabel="0003" typeDefinition="Objects">
                  <audioBlockFormat audioBlockFormatID="AB_00031005_00000001" rtime="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <speakerLabel>Ltf</speakerLabel>
                    <cartesian>1</cartesian>
                    <position coordinate="X">-1.0000000000</position>
                    <position coordinate="Y">1.0000000000</position>
                    <position coordinate="Z">-1.0000000000</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00031006" audioChannelFormatName="Atmos_Obj_6" typeLabel="0003" typeDefinition="Objects">
                  <audioBlockFormat audioBlockFormatID="AB_00031006_00000001" rtime="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <speakerLabel>Rtf</speakerLabel>
                    <cartesian>1</cartesian>
                    <position coordinate="X">1.0000000000</position>
                    <position coordinate="Y">1.0000000000</position>
                    <position coordinate="Z">-1.0000000000</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00031007" audioChannelFormatName="Atmos_Obj_7" typeLabel="0003" typeDefinition="Objects">
                  <audioBlockFormat audioBlockFormatID="AB_00031007_00000001" rtime="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <speakerLabel>Ltr</speakerLabel>
                    <cartesian>1</cartesian>
                    <position coordinate="X">-1.0000000000</position>
                    <position coordinate="Y">-1.0000000000</position>
                    <position coordinate="Z">-1.0000000000</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00031008" audioChannelFormatName="Atmos_Obj_8" typeLabel="0003" typeDefinition="Objects">
                  <audioBlockFormat audioBlockFormatID="AB_00031008_00000001" rtime="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <speakerLabel>Rtr</speakerLabel>
                    <cartesian>1</cartesian>
                    <position coordinate="X">1.0000000000</position>
                    <position coordinate="Y">-1.0000000000</position>
                    <position coordinate="Z">-1.0000000000</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioStreamFormat audioStreamFormatID="AS_00031001" audioStreamFormatName="PCM_Atmos_Obj_1" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00031001</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00031001_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00031002" audioStreamFormatName="PCM_Atmos_Obj_2" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00031002</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00031002_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00031003" audioStreamFormatName="PCM_Atmos_Obj_3" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00031003</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00031003_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00031004" audioStreamFormatName="PCM_Atmos_Obj_4" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00031004</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00031004_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00031005" audioStreamFormatName="PCM_Atmos_Obj_5" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00031005</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00031005_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00031006" audioStreamFormatName="PCM_Atmos_Obj_6" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00031006</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00031006_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00031007" audioStreamFormatName="PCM_Atmos_Obj_7" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00031007</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00031007_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00031008" audioStreamFormatName="PCM_Atmos_Obj_8" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00031008</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00031008_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioTrackFormat audioTrackFormatID="AT_00031001_01" audioTrackFormatName="PCM_Atmos_Obj_1" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00031001</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00031002_01" audioTrackFormatName="PCM_Atmos_Obj_2" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00031002</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00031003_01" audioTrackFormatName="PCM_Atmos_Obj_3" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00031003</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00031004_01" audioTrackFormatName="PCM_Atmos_Obj_4" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00031004</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00031005_01" audioTrackFormatName="PCM_Atmos_Obj_5" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00031005</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00031006_01" audioTrackFormatName="PCM_Atmos_Obj_6" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00031006</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00031007_01" audioTrackFormatName="PCM_Atmos_Obj_7" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00031007</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00031008_01" audioTrackFormatName="PCM_Atmos_Obj_8" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00031008</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackUID UID="ATU_00000001" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011001_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00031001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000002" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011002_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00031002</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000003" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011003_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00031003</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000004" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011004_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00031004</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000005" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011005_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00031005</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000006" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011006_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00031006</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000007" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00031007_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00031007</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000008" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00031008_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00031008</audioPackFormatIDRef>
                </audioTrackUID>
            </audioFormatExtended>
        </format>
    </coreMetadata>
</ebuCoreMain>)";

// default 5.1.4 channel-bed description
const char* axml_5_1_4_ChunkAdmString = R"(<?xml version="1.0" encoding="UTF-8"?>
<ebuCoreMain xmlns="urn:ebu:metadata-schema:ebuCore_2016" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="urn:ebu:metadata-schema:ebuCore_2016 ebucore.xsd" xml:lang="en">
    <coreMetadata>
        <format>
            <audioFormatExtended >
                <audioProgramme audioProgrammeID="APR_1001" audioProgrammeName="ADM_Master" start="00:00:00.00000" end="hh:mm:ss.fffff">
                    <audioContentIDRef>ACO_1001</audioContentIDRef>
                </audioProgramme>
                <audioContent audioContentID="ACO_1001" audioContentName="Atmos_Master_Content">
                    <audioObjectIDRef>AO_1001</audioObjectIDRef>
                    <audioObjectIDRef>AO_1007</audioObjectIDRef>
                    <audioObjectIDRef>AO_1008</audioObjectIDRef>
                    <audioObjectIDRef>AO_1009</audioObjectIDRef>
                    <audioObjectIDRef>AO_100a</audioObjectIDRef>
                    <dialogue mixedContentKind="0">2</dialogue>
                </audioContent>
                <audioObject audioObjectID="AO_1001" audioObjectName="ChannelBed-01" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_00000001</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000002</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000003</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000004</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000005</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000006</audioTrackUIDRef>
                </audioObject>
                <audioObject audioObjectID="AO_1007" audioObjectName="Atmos_Obj_1" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_00031007</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_00000007</audioTrackUIDRef>
                </audioObject>
                <audioObject audioObjectID="AO_1008" audioObjectName="Atmos_Obj_2" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_00031008</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_00000008</audioTrackUIDRef>
                </audioObject>
                <audioObject audioObjectID="AO_1009" audioObjectName="Atmos_Obj_3" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_00031009</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_00000009</audioTrackUIDRef>
                </audioObject>
                <audioObject audioObjectID="AO_100a" audioObjectName="Atmos_Obj_4" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_0003100a</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_0000000a</audioTrackUIDRef>
                </audioObject>
                <audioPackFormat audioPackFormatID="AP_00011001" audioPackFormatName="AtmosCustomPackFormat1" typeLabel="0001" typeDefinition="DirectSpeakers">
                    <audioChannelFormatIDRef>AC_00011001</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011002</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011003</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011004</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011005</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011006</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioPackFormat audioPackFormatID="AP_00031007" audioPackFormatName="Atmos_Obj_1" typeDefinition="Objects" typeLabel="0003">
                    <audioChannelFormatIDRef>AC_00031007</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioPackFormat audioPackFormatID="AP_00031008" audioPackFormatName="Atmos_Obj_2" typeDefinition="Objects" typeLabel="0003">
                    <audioChannelFormatIDRef>AC_00031008</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioPackFormat audioPackFormatID="AP_00031009" audioPackFormatName="Atmos_Obj_3" typeDefinition="Objects" typeLabel="0003">
                    <audioChannelFormatIDRef>AC_00031009</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioPackFormat audioPackFormatID="AP_0003100a" audioPackFormatName="Atmos_Obj_4" typeDefinition="Objects" typeLabel="0003">
                    <audioChannelFormatIDRef>AC_0003100a</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011001" audioChannelFormatName="FrontLeft" typeLabel="0001" typeDefinition="DirectSpeakers">
                  <audioBlockFormat audioBlockFormatID="AB_00011001_00000001">
                    <speakerLabel>L</speakerLabel>
                    <position coordinate="azimuth">30.0</position>
                    <position coordinate="elevation">0.0</position>
                    <position coordinate="distance">1.0</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011002" audioChannelFormatName="FrontRight" typeLabel="0001" typeDefinition="DirectSpeakers">
                  <audioBlockFormat audioBlockFormatID="AB_00011002_00000001">
                    <speakerLabel>R</speakerLabel>
                    <position coordinate="azimuth">-30.0</position>
                    <position coordinate="elevation">0.0</position>
                    <position coordinate="distance">1.0</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011003" audioChannelFormatName="FrontCentre" typeLabel="0001" typeDefinition="DirectSpeakers">
                  <audioBlockFormat audioBlockFormatID="AB_00011003_00000001">
                    <speakerLabel>C</speakerLabel>
                    <position coordinate="azimuth">0.0</position>
                    <position coordinate="elevation">0.0</position>
                    <position coordinate="distance">1.0</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011004" audioChannelFormatName="LowFrequencyEffects" typeLabel="0001" typeDefinition="DirectSpeakers">
                  <frequency typeDefinition="lowPass">120.0</frequency>
                  <audioBlockFormat audioBlockFormatID="AB_00011004_00000001">
                    <speakerLabel>LFE</speakerLabel>
                    <position coordinate="azimuth">0.0</position>
                    <position coordinate="elevation">-30.0</position>
                    <position coordinate="distance">1.0</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011005" audioChannelFormatName="SurroundLeft" typeLabel="0001" typeDefinition="DirectSpeakers">
                  <audioBlockFormat audioBlockFormatID="AB_00011005_00000001">
                    <speakerLabel>Lrs</speakerLabel>
                    <position coordinate="azimuth">110.0</position>
                    <position coordinate="elevation">0.0</position>
                    <position coordinate="distance">1.0</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011006" audioChannelFormatName="SurroundRight" typeLabel="0001" typeDefinition="DirectSpeakers">
                  <audioBlockFormat audioBlockFormatID="AB_00011006_00000001">
                    <speakerLabel>Rrs</speakerLabel>
                    <position coordinate="azimuth">-110.0</position>
                    <position coordinate="elevation">0.0</position>
                    <position coordinate="distance">1.0</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00031007" audioChannelFormatName="Atmos_Obj_1" typeLabel="0003" typeDefinition="Objects">
                  <audioBlockFormat audioBlockFormatID="AB_00031007_00000001" rtime="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <speakerLabel>Ltf</speakerLabel>
                    <cartesian>1</cartesian>
                    <position coordinate="X">-1.0000000000</position>
                    <position coordinate="Y">1.0000000000</position>
                    <position coordinate="Z">1.0000000000</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00031008" audioChannelFormatName="Atmos_Obj_2" typeLabel="0003" typeDefinition="Objects">
                  <audioBlockFormat audioBlockFormatID="AB_00031008_00000001" rtime="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <speakerLabel>Rtf</speakerLabel>
                    <cartesian>1</cartesian>
                    <position coordinate="X">1.0000000000</position>
                    <position coordinate="Y">1.0000000000</position>
                    <position coordinate="Z">1.0000000000</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00031009" audioChannelFormatName="Atmos_Obj_3" typeLabel="0003" typeDefinition="Objects">
                  <audioBlockFormat audioBlockFormatID="AB_00031009_00000001" rtime="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <speakerLabel>Ltr</speakerLabel>
                    <cartesian>1</cartesian>
                    <position coordinate="X">-1.0000000000</position>
                    <position coordinate="Y">-1.0000000000</position>
                    <position coordinate="Z">1.0000000000</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_0003100a" audioChannelFormatName="Atmos_Obj_4" typeLabel="0003" typeDefinition="Objects">
                  <audioBlockFormat audioBlockFormatID="AB_0003100a_00000001" rtime="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <speakerLabel>Rtr</speakerLabel>
                    <cartesian>1</cartesian>
                    <position coordinate="X">1.0000000000</position>
                    <position coordinate="Y">-1.0000000000</position>
                    <position coordinate="Z">1.0000000000</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011001" audioStreamFormatName="PCM_FrontLeft" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011001</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011001_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011002" audioStreamFormatName="PCM_FrontRight" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011002</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011002_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011003" audioStreamFormatName="PCM_FrontCentre" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011003</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011003_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011004" audioStreamFormatName="PCM_LowFrequencyEffects" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011004</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011004_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011005" audioStreamFormatName="PCM_SurroundLeft" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011005</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011005_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011006" audioStreamFormatName="PCM_SurroundRight" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011006</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011006_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00031007" audioStreamFormatName="PCM_Atmos_Obj_1" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00031007</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00031007_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00031008" audioStreamFormatName="PCM_Atmos_Obj_2" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00031008</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00031008_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00031009" audioStreamFormatName="PCM_Atmos_Obj_3" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00031009</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00031009_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_0003100a" audioStreamFormatName="PCM_Atmos_Obj_4" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_0003100a</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_0003100a_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011001_01" audioTrackFormatName="PCM_FrontLeft" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011001</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011002_01" audioTrackFormatName="PCM_FrontRight" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011002</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011003_01" audioTrackFormatName="PCM_FrontCentre" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011003</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011004_01" audioTrackFormatName="PCM_LowFrequencyEffects" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011004</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011005_01" audioTrackFormatName="PCM_SurroundLeft" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011005</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011006_01" audioTrackFormatName="PCM_SurroundRight" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011006</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00031001_01" audioTrackFormatName="PCM_Atmos_Obj_1" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00031007</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00031002_01" audioTrackFormatName="PCM_Atmos_Obj_2" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00031008</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00031003_01" audioTrackFormatName="PCM_Atmos_Obj_3" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00031009</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00031004_01" audioTrackFormatName="PCM_Atmos_Obj_4" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_0003100a</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackUID UID="ATU_00000001" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011001_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000002" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011002_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000003" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011003_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000004" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011004_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000005" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011005_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000006" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011006_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000007" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00031007_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00031007</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000008" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00031008_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00031008</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000009" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00031009_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00031009</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_0000000a" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_0003100a_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_0003100a</audioPackFormatIDRef>
                </audioTrackUID>
            </audioFormatExtended>
        </format>
    </coreMetadata>
</ebuCoreMain>)";

// default 7.1.2 channel-bed description
const char* axml_7_1_2_ChunkAdmString = R"(<?xml version="1.0" encoding="UTF-8"?>
<ebuCoreMain xmlns="urn:ebu:metadata-schema:ebuCore_2016" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="urn:ebu:metadata-schema:ebuCore_2016 ebucore.xsd" xml:lang="en">
    <coreMetadata>
        <format>
            <audioFormatExtended >
                <audioProgramme audioProgrammeID="APR_1001" audioProgrammeName="ADM_7.1.2_Master" start="00:00:00.00000" end="hh:mm:ss.fffff">
                    <audioContentIDRef>ACO_1001</audioContentIDRef>
                </audioProgramme>
                <audioContent audioContentID="ACO_1001" audioContentName="Atmos_Master_Content">
                    <audioObjectIDRef>AO_1001</audioObjectIDRef>
                    <dialogue mixedContentKind="0">2</dialogue>
                </audioContent>
                <audioObject audioObjectID="AO_1001" audioObjectName="ChannelBed-01" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_00000001</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000002</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000003</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000004</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000005</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000006</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000007</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000008</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000009</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_0000000a</audioTrackUIDRef>
                </audioObject>
                <audioPackFormat audioPackFormatID="AP_00011001" audioPackFormatName="AtmosCustomPackFormat1" typeDefinition="DirectSpeakers" typeLabel="0001">
                    <audioChannelFormatIDRef>AC_00011001</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011002</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011003</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011004</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011005</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011006</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011007</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011008</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011009</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_0001100a</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011001" audioChannelFormatName="RoomCentricLeft" typeDefinition="DirectSpeakers" typeLabel="0001">
                    <audioBlockFormat audioBlockFormatID="AB_00011001_00000001">
                        <speakerLabel>RC_L</speakerLabel>
                        <cartesian>1</cartesian>
                        <position coordinate="X">-1.0000000000</position>
                        <position coordinate="Y">1.0000000000</position>
                        <position coordinate="Z">0.0000000000</position>
                    </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011002" audioChannelFormatName="RoomCentricRight" typeDefinition="DirectSpeakers" typeLabel="0001">
                    <audioBlockFormat audioBlockFormatID="AB_00011002_00000001">
                        <speakerLabel>RC_R</speakerLabel>
                        <cartesian>1</cartesian>
                        <position coordinate="X">1.0000000000</position>
                        <position coordinate="Y">1.0000000000</position>
                        <position coordinate="Z">0.0000000000</position>
                    </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011003" audioChannelFormatName="RoomCentricCenter" typeDefinition="DirectSpeakers" typeLabel="0001">
                    <audioBlockFormat audioBlockFormatID="AB_00011003_00000001">
                        <speakerLabel>RC_C</speakerLabel>
                        <cartesian>1</cartesian>
                        <position coordinate="X">0.0000000000</position>
                        <position coordinate="Y">1.0000000000</position>
                        <position coordinate="Z">0.0000000000</position>
                    </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011004" audioChannelFormatName="RoomCentricLFE" typeDefinition="DirectSpeakers" typeLabel="0001">
                    <audioBlockFormat audioBlockFormatID="AB_00011004_00000001">
                        <speakerLabel>RC_LFE</speakerLabel>
                        <cartesian>1</cartesian>
                        <position coordinate="X">-1.0000000000</position>
                        <position coordinate="Y">1.0000000000</position>
                        <position coordinate="Z">-1.0000000000</position>
                    </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011005" audioChannelFormatName="RoomCentricLeftSideSurround" typeDefinition="DirectSpeakers" typeLabel="0001">
                    <audioBlockFormat audioBlockFormatID="AB_00011005_00000001">
                        <speakerLabel>RC_Lss</speakerLabel>
                        <cartesian>1</cartesian>
                        <position coordinate="X">-1.0000000000</position>
                        <position coordinate="Y">0.0000000000</position>
                        <position coordinate="Z">0.0000000000</position>
                    </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011006" audioChannelFormatName="RoomCentricRightSideSurround" typeDefinition="DirectSpeakers" typeLabel="0001">
                    <audioBlockFormat audioBlockFormatID="AB_00011006_00000001">
                        <speakerLabel>RC_Rss</speakerLabel>
                        <cartesian>1</cartesian>
                        <position coordinate="X">1.0000000000</position>
                        <position coordinate="Y">0.0000000000</position>
                        <position coordinate="Z">0.0000000000</position>
                    </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011007" audioChannelFormatName="RoomCentricLeftRearSurround" typeDefinition="DirectSpeakers" typeLabel="0001">
                    <audioBlockFormat audioBlockFormatID="AB_00011007_00000001">
                        <speakerLabel>RC_Lrs</speakerLabel>
                        <cartesian>1</cartesian>
                        <position coordinate="X">-1.0000000000</position>
                        <position coordinate="Y">-1.0000000000</position>
                        <position coordinate="Z">0.0000000000</position>
                    </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011008" audioChannelFormatName="RoomCentricRightRearSurround" typeDefinition="DirectSpeakers" typeLabel="0001">
                    <audioBlockFormat audioBlockFormatID="AB_00011008_00000001">
                        <speakerLabel>RC_Rrs</speakerLabel>
                        <cartesian>1</cartesian>
                        <position coordinate="X">1.0000000000</position>
                        <position coordinate="Y">-1.0000000000</position>
                        <position coordinate="Z">0.0000000000</position>
                    </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011009" audioChannelFormatName="RoomCentricLeftTopSurround" typeDefinition="DirectSpeakers" typeLabel="0001">
                    <audioBlockFormat audioBlockFormatID="AB_00011009_00000001">
                        <speakerLabel>RC_Lts</speakerLabel>
                        <cartesian>1</cartesian>
                        <position coordinate="X">-1.0000000000</position>
                        <position coordinate="Y">0.0000000000</position>
                        <position coordinate="Z">1.0000000000</position>
                    </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_0001100a" audioChannelFormatName="RoomCentricRightTopSurround" typeDefinition="DirectSpeakers" typeLabel="0001">
                    <audioBlockFormat audioBlockFormatID="AB_0001100a_00000001">
                        <speakerLabel>RC_Rts</speakerLabel>
                        <cartesian>1</cartesian>
                        <position coordinate="X">1.0000000000</position>
                        <position coordinate="Y">0.0000000000</position>
                        <position coordinate="Z">1.0000000000</position>
                    </audioBlockFormat>
                </audioChannelFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011001" audioStreamFormatName="PCM_RoomCentricLeft" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011001</audioChannelFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011001_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011002" audioStreamFormatName="PCM_RoomCentricRight" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011002</audioChannelFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011002_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011003" audioStreamFormatName="PCM_RoomCentricCenter" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011003</audioChannelFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011003_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011004" audioStreamFormatName="PCM_RoomCentricLFE" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011004</audioChannelFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011004_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011005" audioStreamFormatName="PCM_RoomCentricLeftSideSurround" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011005</audioChannelFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011005_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011006" audioStreamFormatName="PCM_RoomCentricRightSideSurround" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011006</audioChannelFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011006_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011007" audioStreamFormatName="PCM_RoomCentricLeftRearSurround" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011007</audioChannelFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011007_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011008" audioStreamFormatName="PCM_RoomCentricRightRearSurround" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011008</audioChannelFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011008_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011009" audioStreamFormatName="PCM_RoomCentricLeftTopSurround" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011009</audioChannelFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011009_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_0001100a" audioStreamFormatName="PCM_RoomCentricRightTopSurround" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_0001100a</audioChannelFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                    <audioTrackFormatIDRef>AT_0001100a_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011001_01" audioTrackFormatName="PCM_RoomCentricLeft" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011001</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011002_01" audioTrackFormatName="PCM_RoomCentricRight" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011002</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011003_01" audioTrackFormatName="PCM_RoomCentricCenter" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011003</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011004_01" audioTrackFormatName="PCM_RoomCentricLFE" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011004</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011005_01" audioTrackFormatName="PCM_RoomCentricLeftSideSurround" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011005</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011006_01" audioTrackFormatName="PCM_RoomCentricRightSideSurround" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011006</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011007_01" audioTrackFormatName="PCM_RoomCentricLeftRearSurround" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011007</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011008_01" audioTrackFormatName="PCM_RoomCentricRightRearSurround" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011008</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011009_01" audioTrackFormatName="PCM_RoomCentricLeftTopSurround" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011009</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_0001100a_01" audioTrackFormatName="PCM_RoomCentricRightTopSurround" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_0001100a</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackUID UID="ATU_00000001" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011001_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000002" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011002_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000003" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011003_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000004" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011004_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000005" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011005_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000006" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011006_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000007" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011007_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000008" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011008_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000009" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011009_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_0000000a" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_0001100a_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
            </audioFormatExtended>
        </format>
    </coreMetadata>
</ebuCoreMain>)";

// default 7.1.4 channel-bed description
const char* axml_7_1_4_ChunkAdmString = R"(<?xml version="1.0" encoding="UTF-8"?>
<ebuCoreMain xmlns="urn:ebu:metadata-schema:ebuCore_2016" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="urn:ebu:metadata-schema:ebuCore_2016 ebucore.xsd" xml:lang="en">
    <coreMetadata>
        <format>
            <audioFormatExtended >
                <audioProgramme audioProgrammeID="APR_1001" audioProgrammeName="ADM_Master" start="00:00:00.00000" end="hh:mm:ss.fffff">
                    <audioContentIDRef>ACO_1001</audioContentIDRef>
                </audioProgramme>
                <audioContent audioContentID="ACO_1001" audioContentName="Atmos_Master_Content">
                    <audioObjectIDRef>AO_1001</audioObjectIDRef>
                    <audioObjectIDRef>AO_1009</audioObjectIDRef>
                    <audioObjectIDRef>AO_100a</audioObjectIDRef>
                    <audioObjectIDRef>AO_100b</audioObjectIDRef>
                    <audioObjectIDRef>AO_100c</audioObjectIDRef>
                    <dialogue mixedContentKind="0">2</dialogue>
                </audioContent>
                <audioObject audioObjectID="AO_1001" audioObjectName="ChannelBed-01" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_00000001</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000002</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000003</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000004</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000005</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000006</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000007</audioTrackUIDRef>
                    <audioTrackUIDRef>ATU_00000008</audioTrackUIDRef>
                </audioObject>
                <audioObject audioObjectID="AO_1009" audioObjectName="Atmos_Obj_1" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_00031009</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_00000009</audioTrackUIDRef>
                </audioObject>
                <audioObject audioObjectID="AO_100a" audioObjectName="Atmos_Obj_2" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_0003100a</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_0000000a</audioTrackUIDRef>
                </audioObject>
                <audioObject audioObjectID="AO_100b" audioObjectName="Atmos_Obj_3" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_0003100b</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_0000000b</audioTrackUIDRef>
                </audioObject>
                <audioObject audioObjectID="AO_100c" audioObjectName="Atmos_Obj_4" start="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <audioPackFormatIDRef>AP_0003100c</audioPackFormatIDRef>
                    <audioTrackUIDRef>ATU_0000000c</audioTrackUIDRef>
                </audioObject>
                <audioPackFormat audioPackFormatID="AP_00011001" audioPackFormatName="AtmosCustomPackFormat1" typeLabel="0001" typeDefinition="DirectSpeakers">
                    <audioChannelFormatIDRef>AC_00011001</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011002</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011003</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011004</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011005</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011006</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011007</audioChannelFormatIDRef>
                    <audioChannelFormatIDRef>AC_00011008</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioPackFormat audioPackFormatID="AP_00031009" audioPackFormatName="Atmos_Obj_1" typeDefinition="Objects" typeLabel="0003">
                    <audioChannelFormatIDRef>AC_00031009</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioPackFormat audioPackFormatID="AP_0003100a" audioPackFormatName="Atmos_Obj_2" typeDefinition="Objects" typeLabel="0003">
                    <audioChannelFormatIDRef>AC_0003100a</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioPackFormat audioPackFormatID="AP_0003100b" audioPackFormatName="Atmos_Obj_3" typeDefinition="Objects" typeLabel="0003">
                    <audioChannelFormatIDRef>AC_0003100b</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioPackFormat audioPackFormatID="AP_0003100c" audioPackFormatName="Atmos_Obj_4" typeDefinition="Objects" typeLabel="0003">
                    <audioChannelFormatIDRef>AC_0003100c</audioChannelFormatIDRef>
                </audioPackFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011001" audioChannelFormatName="FrontLeft" typeLabel="0001" typeDefinition="DirectSpeakers">
                  <audioBlockFormat audioBlockFormatID="AB_00011001_00000001">
                    <speakerLabel>L</speakerLabel>
                    <position coordinate="azimuth">30.0</position>
                    <position coordinate="elevation">0.0</position>
                    <position coordinate="distance">1.0</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011002" audioChannelFormatName="FrontRight" typeLabel="0001" typeDefinition="DirectSpeakers">
                  <audioBlockFormat audioBlockFormatID="AB_00011002_00000001">
                    <speakerLabel>R</speakerLabel>
                    <position coordinate="azimuth">-30.0</position>
                    <position coordinate="elevation">0.0</position>
                    <position coordinate="distance">1.0</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011003" audioChannelFormatName="FrontCentre" typeLabel="0001" typeDefinition="DirectSpeakers">
                  <audioBlockFormat audioBlockFormatID="AB_00011003_00000001">
                    <speakerLabel>C</speakerLabel>
                    <position coordinate="azimuth">0.0</position>
                    <position coordinate="elevation">0.0</position>
                    <position coordinate="distance">1.0</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011004" audioChannelFormatName="LowFrequencyEffects" typeLabel="0001" typeDefinition="DirectSpeakers">
                  <frequency typeDefinition="lowPass">120.0</frequency>
                  <audioBlockFormat audioBlockFormatID="AB_00011004_00000001">
                    <speakerLabel>LFE</speakerLabel>
                    <position coordinate="azimuth">0.0</position>
                    <position coordinate="elevation">-30.0</position>
                    <position coordinate="distance">1.0</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011005" audioChannelFormatName="SideLeft" typeLabel="0001" typeDefinition="DirectSpeakers">
                  <audioBlockFormat audioBlockFormatID="AB_00011005_00000001">
                    <speakerLabel>Lrs</speakerLabel>
                    <position coordinate="azimuth">90.0</position>
                    <position coordinate="elevation">0.0</position>
                    <position coordinate="distance">1.0</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011006" audioChannelFormatName="SideRight" typeLabel="0001" typeDefinition="DirectSpeakers">
                  <audioBlockFormat audioBlockFormatID="AB_00011006_00000001">
                    <speakerLabel>Rrs</speakerLabel>
                    <position coordinate="azimuth">-90.0</position>
                    <position coordinate="elevation">0.0</position>
                    <position coordinate="distance">1.0</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011007" audioChannelFormatName="BackLeftMid" typeLabel="0001" typeDefinition="DirectSpeakers">
                  <audioBlockFormat audioBlockFormatID="AB_00011007_00000001">
                    <speakerLabel>Lrs</speakerLabel>
                    <position coordinate="azimuth">135.0</position>
                    <position coordinate="elevation">0.0</position>
                    <position coordinate="distance">1.0</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00011008" audioChannelFormatName="BackRightMid" typeLabel="0001" typeDefinition="DirectSpeakers">
                  <audioBlockFormat audioBlockFormatID="AB_00011008_00000001">
                    <speakerLabel>Rrs</speakerLabel>
                    <position coordinate="azimuth">-135.0</position>
                    <position coordinate="elevation">0.0</position>
                    <position coordinate="distance">1.0</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_00031009" audioChannelFormatName="Atmos_Obj_1" typeLabel="0003" typeDefinition="Objects">
                  <audioBlockFormat audioBlockFormatID="AB_00031009_00000001" rtime="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <speakerLabel>Ltf</speakerLabel>
                    <cartesian>1</cartesian>
                    <position coordinate="X">-1.0000000000</position>
                    <position coordinate="Y">1.0000000000</position>
                    <position coordinate="Z">1.0000000000</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_0003100a" audioChannelFormatName="Atmos_Obj_2" typeLabel="0003" typeDefinition="Objects">
                  <audioBlockFormat audioBlockFormatID="AB_0003100a_00000001" rtime="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <speakerLabel>Rtf</speakerLabel>
                    <cartesian>1</cartesian>
                    <position coordinate="X">1.0000000000</position>
                    <position coordinate="Y">1.0000000000</position>
                    <position coordinate="Z">1.0000000000</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_0003100b" audioChannelFormatName="Atmos_Obj_3" typeLabel="0003" typeDefinition="Objects">
                  <audioBlockFormat audioBlockFormatID="AB_0003100b_00000001" rtime="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <speakerLabel>Ltr</speakerLabel>
                    <cartesian>1</cartesian>
                    <position coordinate="X">-1.0000000000</position>
                    <position coordinate="Y">-1.0000000000</position>
                    <position coordinate="Z">1.0000000000</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioChannelFormat audioChannelFormatID="AC_0003100c" audioChannelFormatName="Atmos_Obj_4" typeLabel="0003" typeDefinition="Objects">
                  <audioBlockFormat audioBlockFormatID="AB_0003100c_00000001" rtime="00:00:00.00000" duration="hh:mm:ss.fffff">
                    <speakerLabel>Rtr</speakerLabel>
                    <cartesian>1</cartesian>
                    <position coordinate="X">1.0000000000</position>
                    <position coordinate="Y">-1.0000000000</position>
                    <position coordinate="Z">1.0000000000</position>
                  </audioBlockFormat>
                </audioChannelFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011001" audioStreamFormatName="PCM_FrontLeft" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011001</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011001_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011002" audioStreamFormatName="PCM_FrontRight" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011002</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011002_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011003" audioStreamFormatName="PCM_FrontCentre" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011003</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011003_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011004" audioStreamFormatName="PCM_LowFrequencyEffects" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011004</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011004_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011005" audioStreamFormatName="PCM_SideLeft" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011005</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011005_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011006" audioStreamFormatName="PCM_SideRight" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011006</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011006_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011007" audioStreamFormatName="PCM_BackLeftMid" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011007</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011007_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00011008" audioStreamFormatName="PCM_BackRightMid" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00011008</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00011008_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_00031009" audioStreamFormatName="PCM_Atmos_Obj_1" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_00031009</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_00031009_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_0003100a" audioStreamFormatName="PCM_Atmos_Obj_2" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_0003100a</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_0003100a_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_0003100b" audioStreamFormatName="PCM_Atmos_Obj_3" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_0003100b</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_0003100b_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioStreamFormat audioStreamFormatID="AS_0003100c" audioStreamFormatName="PCM_Atmos_Obj_4" formatDefinition="PCM" formatLabel="0001">
                    <audioChannelFormatIDRef>AC_0003100c</audioChannelFormatIDRef>
                    <audioTrackFormatIDRef>AT_0003100c_01</audioTrackFormatIDRef>
                </audioStreamFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011001_01" audioTrackFormatName="PCM_FrontLeft" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011001</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011002_01" audioTrackFormatName="PCM_FrontRight" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011002</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011003_01" audioTrackFormatName="PCM_FrontCentre" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011003</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011004_01" audioTrackFormatName="PCM_LowFrequencyEffects" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011004</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011005_01" audioTrackFormatName="PCM_SideLeft" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011005</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011006_01" audioTrackFormatName="PCM_SideRight" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011006</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011007_01" audioTrackFormatName="PCM_BackLeftMid" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011007</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00011008_01" audioTrackFormatName="PCM_BackRightMid" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00011008</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_00031009_01" audioTrackFormatName="PCM_Atmos_Obj_1" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_00031009</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_0003100a_01" audioTrackFormatName="PCM_Atmos_Obj_2" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_0003100a</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_0003100b_01" audioTrackFormatName="PCM_Atmos_Obj_3" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_0003100b</audioStreamFormatIDRef>
                </audioTrackFormat>
                <audioTrackFormat audioTrackFormatID="AT_0003100c_01" audioTrackFormatName="PCM_Atmos_Obj_4" formatDefinition="PCM" formatLabel="0001">
                    <audioStreamFormatIDRef>AS_0003100c</audioStreamFormatIDRef>
                </audioTrackFormat>
)"
R"(             <audioTrackUID UID="ATU_00000001" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011001_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000002" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011002_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000003" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011003_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000004" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011004_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000005" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011005_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000006" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011006_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000007" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011007_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000008" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00011008_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_00000009" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_00031009_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_00031009</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_0000000a" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_0003100a_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_0003100a</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_0000000b" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_0003100b_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_0003100b</audioPackFormatIDRef>
                </audioTrackUID>
                <audioTrackUID UID="ATU_0000000c" bitDepth="__BITDEPTH__" sampleRate="__SAMPLERATE__">
                    <audioTrackFormatIDRef>AT_0003100c_01</audioTrackFormatIDRef>
                    <audioPackFormatIDRef>AP_0003100c</audioPackFormatIDRef>
                </audioTrackUID>
            </audioFormatExtended>
        </format>
    </coreMetadata>
</ebuCoreMain>)";
