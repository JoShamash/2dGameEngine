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
	std::string path;  // 地图路径
	size_t pixelX = 0; // 像素宽度
	size_t pixelY = 0; // 像素高度
	size_t rangeX = 0; // 范围宽度
	size_t rangeY = 0; // 范围高度
	size_t scaleX = 0; // 瓦片宽度缩放
	size_t scaleY = 0; // 瓦片高度缩放

	std::vector<std::vector<size_t>> map; // 地图样式表

	MapStyle(const std::string& path) : path(path)
	{
		std::ifstream file(path);
		if (!file.is_open())
		{
			std::string message = U8_TO_CHARPTR("地图样式文件打开失败：") + path;
			Logger::Instance().Log(LogLevel::ERROR, message);
			file.close();
			return;
		}

		std::string line;
		size_t lineNum = 0;

		std::regex pixelRegex(R"(pixel:(\d+)\*(\d+))");					  // 匹配像素格式
		std::regex rangeRegex(R"(range:(\d+)\*(\d+))");					  // 匹配范围格式
		std::regex scaleRegex(R"(scale:(\d+\.\d+|\d+)\*(\d+\.\d+|\d+))"); // 匹配缩放格式

		std::smatch match;

		// 读取文件pixel
		if (lineNum == 0)
		{
			std::getline(file, line);
			if (std::regex_search(line, match, pixelRegex))
			{
				// 从字符串中提取像素宽度和高度
				pixelX = std::stoi(match[1].str());
				pixelY = std::stoi(match[2].str());
			}
			else
			{
				std::string message = U8_TO_CHARPTR("地图样式文件pixel格式错误：") + path;
				Logger::Instance().Log(LogLevel::ERROR, message);
				file.close();
				return;
			}

			lineNum++;
		}
		// 读取文件range
		if (lineNum == 1)
		{
			std::getline(file, line);
			if (std::regex_search(line, match, rangeRegex))
			{
				// 从字符串中提取范围宽度和高度
				rangeX = std::stoi(match[1].str());
				rangeY = std::stoi(match[2].str());
			}
			else
			{
				std::string message = U8_TO_CHARPTR("地图样式文件range格式错误：") + path;
				Logger::Instance().Log(LogLevel::ERROR, message);
				file.close();
				return;
			}

			map.resize(rangeY); // 根据范围高度调整地图样式表的大小

			lineNum++;
		}
		// 读取文件scale
		if (lineNum == 2)
		{ 
			std::getline(file, line);
			if (std::regex_search(line, match, scaleRegex))
			{
				// 从字符串中提取缩放宽度和高度
				scaleX = std::stod(match[1].str());
				scaleY = std::stod(match[2].str());
			}
			else
			{
				std::string message = U8_TO_CHARPTR("地图样式文件scale格式错误：") + path;
				Logger::Instance().Log(LogLevel::ERROR, message);
				file.close();
				return;
			}
			lineNum++;
		}
		// 读取地图样式表
		if (lineNum == 3)
		{
			lineNum = 0;

			// 读取地图样式表
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

			// 检查地图样式表的高度是否与范围高度一致
			if (map.size() != rangeY && map[0].size() != rangeX)
			{
				std::string message = U8_TO_CHARPTR("地图样式文件二维表格式错误：") + path;
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

