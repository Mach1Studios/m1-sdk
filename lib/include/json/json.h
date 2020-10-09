#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <streambuf>

namespace JSON {

	enum NodeType {
		ArrayElement,
		HashElement,
		Value,
		Key,
	};

	struct Node {
		std::string value = "";
		int level = 0;
		NodeType type = Value;
		Node* parent = nullptr;
	};

	std::vector<Node*> getChildren(std::vector<Node*> docOrig, std::string name) {
		int level = -1;
		std::vector<Node*> doc;

		int idx = -1;
		for (int i = 0; i < docOrig.size(); i++) {
			if (docOrig[i]->value == name && (level < 0 || docOrig[i]->level < level)) {
				idx = i;
				level = docOrig[i]->level;
			}
		}

		if (idx >= 0) {
			for (int j = idx + 1; j < docOrig.size(); j++) {
				if (docOrig[j]->level > docOrig[idx]->level) {
					doc.push_back(docOrig[j]);
				}
				else break;
			}
		}

		return doc;
	}

	std::vector<Node*> getElement(std::vector<Node*> docOrig, int level, int n) {
		std::vector<Node*> doc;

		int idx = -1;
		int cnt = 0;

		for (int i = 0; i < docOrig.size(); i++) {
			if (docOrig[i]->level == level) {
				if (n == cnt) {
					idx = i;
					break;
				}
				cnt++;
			}
		}

		if (idx >= 0) {
			for (int j = idx; j < docOrig.size(); j++) {
				if (docOrig[j]->level >= docOrig[idx]->level) {
					doc.push_back(docOrig[j]);
				}
				else break;
			}
		}

		return doc;
	}

	std::vector<Node*> parse(std::string srtJson) {
		std::vector<Node*> doc;
		std::vector<Node*> parentNodes;

		int namePosStart = -1;
		int valuePosStart = -1;
		std::string name = "";

		for (int i = 0; i < srtJson.size(); i++) {
			if (srtJson[i] == '{' || srtJson[i] == '[' || srtJson[i] == ':') {
				Node* node = new Node();

				if (srtJson[i] == '{') {
					node->type = HashElement;
					valuePosStart = -1;
				}
				else if (srtJson[i] == '[') {
					node->type = ArrayElement;
					valuePosStart = -1;
				}
				else if (srtJson[i] == ':') {
					node->type = Key;
					valuePosStart = i;
				}

				node->value = name;
				node->level = parentNodes.size();
				if (parentNodes.size() > 0) {
					node->parent = parentNodes.back();
				}
				doc.push_back(node);

				parentNodes.push_back(node);

				name = "";
			}
			else if (srtJson[i] == ',' || srtJson[i] == '\r' || srtJson[i] == '\n') {
				if (valuePosStart >= 0) {

					Node* node = new Node();
					node->type = Value;
					node->value = srtJson.substr(valuePosStart + 1, i - valuePosStart - 1);
					node->level = parentNodes.size();
					doc.push_back(node);

					valuePosStart = -1;
				}

				if (srtJson[i] == ',') {
					parentNodes.pop_back();
				}
			}
			else if (srtJson[i] == '"') {
				if (namePosStart < 0) {
					name = "";
					namePosStart = i;
				}
				else {
					name = srtJson.substr(namePosStart + 1, i - namePosStart - 1);
					namePosStart = -1;
				}
			}
			else if (srtJson[i] == '}' || srtJson[i] == ']') {
				parentNodes.pop_back();
			}
		}

		return doc;
	}
};
