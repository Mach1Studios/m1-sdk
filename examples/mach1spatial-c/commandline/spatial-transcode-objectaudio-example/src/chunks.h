#include "bw64/bw64.hpp"
#include <vector>

bw64::ChnaChunk chnaChunkAdm(std::vector<bw64::AudioId>{
	{1, "ATU_00000001", "AT_00011001_01", "AP_00011001" },
	{ 2, "ATU_00000002", "AT_00011002_01", "AP_00011001" },
	{ 3, "ATU_00000003", "AT_00011003_01", "AP_00011001" },
	{ 4, "ATU_00000004", "AT_00011004_01", "AP_00011001" },
	{ 5, "ATU_00000005", "AT_00011005_01", "AP_00011001" },
	{ 6, "ATU_00000006", "AT_00011006_01", "AP_00011001" },
	{ 7, "ATU_00000007", "AT_00011007_01", "AP_00011001" },
	{ 8, "ATU_00000008", "AT_00011008_01", "AP_00011001" },
	{ 9, "ATU_00000009", "AT_00011009_01", "AP_00011001" },
	{ 10, "ATU_0000000a", "AT_0001100a_01", "AP_00011001" },
});

const char* axmlChunkAdmString = R"(<?xml version="1.0" encoding="UTF-8"?>
<ebuCoreMain xmlns="urn:ebu:metadata-schema:ebuCore_2016" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="urn:ebu:metadata-schema:ebuCore_2016 ebucore.xsd" xml:lang="en">
	<coreMetadata>
		<format>
			<audioFormatExtended >
				<audioProgramme audioProgrammeID="APR_1001" audioProgrammeName="Atmos_Master" start="00:00:00.00000" end="00:00:31.12533">
					<audioContentIDRef>ACO_1001</audioContentIDRef>
				</audioProgramme>
				<audioContent audioContentID="ACO_1001" audioContentName="Atmos_Master_Content">
					<audioObjectIDRef>AO_1001</audioObjectIDRef>
					<dialogue mixedContentKind="0">2</dialogue>
				</audioContent>
				<audioObject audioObjectID="AO_1001" audioObjectName="Atmos_Bed_1" start="00:00:00.00000" duration="00:00:31.12533">
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
				<audioTrackUID UID="ATU_00000001" bitDepth="24" sampleRate="48000">
					<audioTrackFormatIDRef>AT_00011001_01</audioTrackFormatIDRef>
					<audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
				</audioTrackUID>
				<audioTrackUID UID="ATU_00000002" bitDepth="24" sampleRate="48000">
					<audioTrackFormatIDRef>AT_00011002_01</audioTrackFormatIDRef>
					<audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
				</audioTrackUID>
				<audioTrackUID UID="ATU_00000003" bitDepth="24" sampleRate="48000">
					<audioTrackFormatIDRef>AT_00011003_01</audioTrackFormatIDRef>
					<audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
				</audioTrackUID>
				<audioTrackUID UID="ATU_00000004" bitDepth="24" sampleRate="48000">
					<audioTrackFormatIDRef>AT_00011004_01</audioTrackFormatIDRef>
					<audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
				</audioTrackUID>
				<audioTrackUID UID="ATU_00000005" bitDepth="24" sampleRate="48000">
					<audioTrackFormatIDRef>AT_00011005_01</audioTrackFormatIDRef>
					<audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
				</audioTrackUID>
				<audioTrackUID UID="ATU_00000006" bitDepth="24" sampleRate="48000">
					<audioTrackFormatIDRef>AT_00011006_01</audioTrackFormatIDRef>
					<audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
				</audioTrackUID>
				<audioTrackUID UID="ATU_00000007" bitDepth="24" sampleRate="48000">
					<audioTrackFormatIDRef>AT_00011007_01</audioTrackFormatIDRef>
					<audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
				</audioTrackUID>
				<audioTrackUID UID="ATU_00000008" bitDepth="24" sampleRate="48000">
					<audioTrackFormatIDRef>AT_00011008_01</audioTrackFormatIDRef>
					<audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
				</audioTrackUID>
				<audioTrackUID UID="ATU_00000009" bitDepth="24" sampleRate="48000">
					<audioTrackFormatIDRef>AT_00011009_01</audioTrackFormatIDRef>
					<audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
				</audioTrackUID>
				<audioTrackUID UID="ATU_0000000a" bitDepth="24" sampleRate="48000">
					<audioTrackFormatIDRef>AT_0001100a_01</audioTrackFormatIDRef>
					<audioPackFormatIDRef>AP_00011001</audioPackFormatIDRef>
				</audioTrackUID>
			</audioFormatExtended>
		</format>
	</coreMetadata>
</ebuCoreMain>)";

bw64::AxmlChunk axmlChunkAdm(axmlChunkAdmString);
