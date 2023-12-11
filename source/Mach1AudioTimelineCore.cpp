//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1AudioTimelineCore.h"
#include "Mach1GenerateCoeffs.h"
#include <Yaml.hpp>

float Mach1AudioTimelineCore::clamp(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
}

void Mach1AudioTimelineCore::ConvertRCtoXYRaw(float r, float d, float &x, float &y) {
    float abs_d = fabs(d);
    float x_tmp;
    float y_tmp;

    if (abs_d == 0.0) {
        x_tmp = 0;
        y_tmp = 0;
    } else {
        float sign = d / abs_d;
        float rotation_radian = r * PI / 180;
        float center = abs_d * sqrtf(2);
        float ratio_x_center = sin(rotation_radian);
        float ratio_y_center = cos(rotation_radian);
        x_tmp = sign * ratio_x_center * center;
        y_tmp = sign * ratio_y_center * center;
    }

    x = clamp(x_tmp, -100, 100);
    y = clamp(y_tmp, -100, 100);
}

std::vector<Mach1AudioObjectCore> *Mach1AudioTimelineCore::getAudioObjects() {
    return &audioObjects;
}

void Mach1AudioTimelineCore::parseADM(char *inXml) {
    ADMParser admParser;
    ADMParser::AudioTracks audioTracks;
    admParser.ParseString(inXml, audioTracks);

    audioObjects.clear();

    for (auto it = audioTracks.begin(); it != audioTracks.end(); it++) {

        int c = -1;
        for (int i = 0; i < audioObjects.size(); i++) {
            if (audioObjects[i].name == it->first) {
                c = i;
                break;
            }
        }

        if (c < 0) {
            Mach1AudioObjectCore audioObject;
            audioObject.name = it->first;
            audioObjects.push_back(audioObject);
            c = (int)audioObjects.size() - 1;
        }

        for (int i = 0; i < it->second.size(); i++) {
            Mach1KeyPoint p;
            p.sample = it->second[c].sample;
            p.point.x = it->second[c].x;
            p.point.y = it->second[c].y;
            p.point.z = it->second[c].z;
            audioObjects[c].points.push_back(p);
        }
    }
}

void Mach1AudioTimelineCore::parseAtmos(char *inDotAtmos, char *inDotAtmosDotMetadata) {
    ADMParser::AudioTracks audioTracks;

    Yaml::Node generalmetadata;
    Yaml::Node objectmetadata;
    int cnt = 0;

    Yaml::Parse(generalmetadata, inDotAtmos, std::strlen(inDotAtmos));
    Yaml::Parse(objectmetadata, inDotAtmosDotMetadata, std::strlen(inDotAtmosDotMetadata));

    {
        Yaml::Node &item = generalmetadata["presentations"][0]["bedInstances"][0]["channels"];
        for (auto it = item.Begin(); it != item.End(); it++) {
            string ID = (*it).second["ID"].As<string>();
            std::cout << (*it).first << ": " << (*it).second.As<string>() << std::endl;

            float Rotation = 0;
            float Diverge = 0;
            float Elevation = 0;

            string channel = (*it).second["channel"].As<string>();

            if (channel == "L") {
                Rotation = -45;
                Diverge = 100;
                Elevation = 0;
            } else if (channel == "R") {
                Rotation = 45;
                Diverge = 100;
                Elevation = 0;
            } else if (channel == "C") {
                Rotation = 0;
                Diverge = 100;
                Elevation = 0;
            } else if (channel == "LFE") {
                Rotation = 0;
                Diverge = 0;
                Elevation = 0;
            } else if (channel == "Lss") {
                Rotation = -90;
                Diverge = 100;
                Elevation = 0;
            } else if (channel == "Rss") {
                Rotation = 90;
                Diverge = 100;
                Elevation = 0;
            } else if (channel == "Lrs") {
                Rotation = -135;
                Diverge = 100;
                Elevation = 0;
            } else if (channel == "Rrs") {
                Rotation = 135;
                Diverge = 100;
                Elevation = 0;
            } else if (channel == "Lts") {
                Rotation = -90;
                Diverge = 35;
                Elevation = 0;
            } else if (channel == "Rts") {
                Rotation = 90;
                Diverge = 35;
                Elevation = 0;
            }

            float xConv, yConv;
            ConvertRCtoXYRaw(Rotation, Diverge, xConv, yConv);

            audioTracks[channel].push_back(ADMParser::KeyPoint(0, xConv, yConv, Elevation));

            cnt++;
        }
    }

    {
        Yaml::Node &item = objectmetadata["events"];
        for (auto it = item.Begin(); it != item.End(); it++) {
            string channel = (*it).second["ID"].As<string>();
            std::cout << (*it).first << ": " << (*it).second.As<string>() << std::endl;

            if (!(*it).second["pos"].IsNone()) {
                long long p = (*it).second["samplePos"].As<long long>(); // 1.0 * (*it).second["samplePos"].As<int>() / objectmetadata["sampleRate"].As<int>();
                float x = ((*it).second["pos"][0].As<float>());
                float y = ((*it).second["pos"][1].As<float>());
                float z = ((*it).second["pos"][2].As<float>());

                float Rotation = atan2(x, y) * 180 / PI;
                float Diverge = 100 * sqrtf(x * x + y * y) / sqrtf(2.0);

                Mach1Point3DCore a(x, y, z);
                Mach1Point3DCore b(x, y, 0.0);

                float Elevation = 0;
                if (a != b) {
                    // vector_angle_between
                    Elevation = (acos(Mach1Point3DCore::dot(a, b)) / (a.length() * b.length())) * 180 / PI;
                }

                float xConv, yConv;
                ConvertRCtoXYRaw(Rotation, Diverge, xConv, yConv);

                audioTracks[channel].push_back(ADMParser::KeyPoint(p, xConv, yConv, Elevation));

                cnt++;
            }
        }
    }

    cout << "Imported " << cnt << " points";
}
