#ifndef MAPSTYLE
#define MAPSTYLE

#include <string>
#include <vector>
#include <fstream>
#include <regex>

#include "../Logger/Logger.h"

class MapStyle
{
public:
	std::string path;  // ��ͼ·��
	size_t pixelX = 0; // ���ؿ��
	size_t pixelY = 0; // ���ظ߶�
	size_t rangeX = 0; // ��Χ���
	size_t rangeY = 0; // ��Χ�߶�
	size_t scaleX = 0; // ��Ƭ�������
	size_t scaleY = 0; // ��Ƭ�߶�����

	std::vector<std::vector<size_t>> map; // ��ͼ��ʽ��

	MapStyle(const std::string& path) : path(path)
	{
		std::ifstream file(path);
		if (!file.is_open())
		{
			std::string message = U8_TO_CHARPTR("��ͼ��ʽ�ļ���ʧ�ܣ�") + path;
			Logger::Instance().Log(LogLevel::ERROR, message);
			file.close();
			return;
		}

		std::string line;
		size_t lineNum = 0;

		std::regex pixelRegex(R"(pixel:(\d+)\*(\d+))");					  // ƥ�����ظ�ʽ
		std::regex rangeRegex(R"(range:(\d+)\*(\d+))");					  // ƥ�䷶Χ��ʽ
		std::regex scaleRegex(R"(scale:(\d+\.\d+|\d+)\*(\d+\.\d+|\d+))"); // ƥ�����Ÿ�ʽ

		std::smatch match;

		// ��ȡ�ļ�pixel
		if (lineNum == 0)
		{
			std::getline(file, line);
			if (std::regex_search(line, match, pixelRegex))
			{
				// ���ַ�������ȡ���ؿ�Ⱥ͸߶�
				pixelX = std::stoi(match[1].str());
				pixelY = std::stoi(match[2].str());
			}
			else
			{
				std::string message = U8_TO_CHARPTR("��ͼ��ʽ�ļ�pixel��ʽ����") + path;
				Logger::Instance().Log(LogLevel::ERROR, message);
				file.close();
				return;
			}

			lineNum++;
		}
		// ��ȡ�ļ�range
		if (lineNum == 1)
		{
			std::getline(file, line);
			if (std::regex_search(line, match, rangeRegex))
			{
				// ���ַ�������ȡ��Χ��Ⱥ͸߶�
				rangeX = std::stoi(match[1].str());
				rangeY = std::stoi(match[2].str());
			}
			else
			{
				std::string message = U8_TO_CHARPTR("��ͼ��ʽ�ļ�range��ʽ����") + path;
				Logger::Instance().Log(LogLevel::ERROR, message);
				file.close();
				return;
			}

			map.resize(rangeY); // ���ݷ�Χ�߶ȵ�����ͼ��ʽ��Ĵ�С

			lineNum++;
		}
		// ��ȡ�ļ�scale
		if (lineNum == 2)
		{ 
			std::getline(file, line);
			if (std::regex_search(line, match, scaleRegex))
			{
				// ���ַ�������ȡ���ſ�Ⱥ͸߶�
				scaleX = std::stod(match[1].str());
				scaleY = std::stod(match[2].str());
			}
			else
			{
				std::string message = U8_TO_CHARPTR("��ͼ��ʽ�ļ�scale��ʽ����") + path;
				Logger::Instance().Log(LogLevel::ERROR, message);
				file.close();
				return;
			}
			lineNum++;
		}
		// ��ȡ��ͼ��ʽ��
		if (lineNum == 3)
		{
			lineNum = 0;

			// ��ȡ��ͼ��ʽ��
			while (std::getline(file, line))
			{
				std::stringstream ss(line);
				std::string value;
				while (std::getline(ss, value, ',')) 
				{
					map[lineNum].push_back(std::stoull(value));
				}

				lineNum++;
			}

			// ����ͼ��ʽ��ĸ߶��Ƿ��뷶Χ�߶�һ��
			if (map.size() != rangeY && map[0].size() != rangeX)
			{
				std::string message = U8_TO_CHARPTR("��ͼ��ʽ�ļ���ά���ʽ����") + path;
				Logger::Instance().Log(LogLevel::ERROR, message);
				map.clear();
				pixelX = pixelY = rangeX = rangeY = scaleX = scaleY = 0;
				file.close();
				return;
			}
		}
		file.close();
	}
};

#endif // !MAPSTYLE

