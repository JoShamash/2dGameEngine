#ifndef RENDERGUISYSTEM_H
#define RENDERGUISYSTEM_H

constexpr double IM_PI = 3.14159265358979323846;

#include "../ECS/ECS.h"
#include "../Camera/Camera.h"
#include "../Logger/Logger.h"

#include <future>
#include <chrono>
#include <thread>
#include <vector>
#include <cmath>
#include <format>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_sdlrenderer2.h>
#include "../AssetStore/AssetStore.h"

#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/CampComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/HealthBarComponent.h"

class RenderGUISystem : public System
{
	// 定义一个SDL_Texture自定义的删除器
	struct SDLTextureDeleter 
	{
		void operator()(SDL_Texture* texture) const 
		{
			SDL_DestroyTexture(texture);
		}
	};

public:
	inline static const std::string NAME = std::string("RenderGUISystem") + U8_TO_CHARPTR("渲染GUI系统"); // 系统名称

	RenderGUISystem()
	{
		std::string message = U8_TO_CHARPTR("添加系统：") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief 更新渲染GUI系统，渲染所有GUI元素
	* @param renderer SDL 渲染器，用于渲染GUI元素
	*/
	void Update(std::unique_ptr<Registry>& registry, SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const Camera& camera)
	{
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		//ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;

		if (ImGui::Begin(U8_TO_CHARPTR("创建敌人")))
		{
			// 设置 标签和组名称
			if (ImGui::CollapsingHeader(U8_TO_CHARPTR("名称##tagName_groupName")))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 4.0f));
				if (ImGui::BeginTable("Name_Table", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Borders))
				{
					static std::string tempTagName;
					static std::string tempGroupName;

					ImGui::TableSetupColumn("attribute", ImGuiTableColumnFlags_WidthFixed);		// 第一列固定宽度
					ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_WidthStretch);		// 第二列自动填充

					auto inputFlag = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll;

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("标签名称"));
					ImGui::TableSetColumnIndex(1);
					if (ImGui::InputText(U8_TO_CHARPTR("##标签名称_InputText"), &tempTagName, inputFlag))
					{
						tagName = tempTagName;		// 更新标签名称
					}
					else
					{
						tempTagName = tagName;		// 恢复标签名称
					}

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("组名称"));
					ImGui::TableSetColumnIndex(1);
					if (ImGui::InputText(U8_TO_CHARPTR("##组名称_InputText"), &tempGroupName, inputFlag))
					{
						groupName = tempGroupName;	// 更新组名称
					}
					else
					{
						tempGroupName = groupName;	// 恢复组名称
					}

					ImGui::EndTable();
				}
				ImGui::PopStyleVar();
			}

			// 设置 Camp阵营组件属性
			if (ImGui::CollapsingHeader(U8_TO_CHARPTR("阵营组件属性##CampComponent")))
			{
				auto& camp = campComponent.camp;

				ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 4.0f));
				if (ImGui::BeginTable("Camp_Table", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Borders))
				{
					ImGui::TableSetupColumn("attribute", ImGuiTableColumnFlags_WidthFixed);		// 第一列固定宽度
					ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_WidthStretch);		// 第二列自动填充

					auto inputFlag = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll;

					// 显示当前阵营
					static int campSelectedIndex = 0;
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("阵营选择"));
					ImGui::TableSetColumnIndex(1);
					if (ImGui::BeginCombo(U8_TO_CHARPTR("##阵营选择_BeginCombo"), CampText(camp)))
					{
						for (int i = 0; i < static_cast<int>(Camp::Count); ++i)
						{
							bool isSelected = (campSelectedIndex == i);
							if (ImGui::Selectable(CampText(static_cast<Camp>(i)), isSelected, ImGuiComboFlags_HeightRegular))
							{
								campSelectedIndex = i;
								ImGui::SetItemDefaultFocus();
								camp = static_cast<Camp>(i); // 更新阵营
							}
						}
						ImGui::EndCombo();
					}

					ImGui::EndTable();
				}
				ImGui::PopStyleVar();
			}

			// 设置 Health生命值组件属性
			if (ImGui::CollapsingHeader(U8_TO_CHARPTR("生命值组件属性##HealthComponent")))
			{
				auto tempHp = healthComponent.hp;
				auto tempMaxHp = healthComponent.maxHp;

				auto& hp = healthComponent.hp;
				auto& maxHp = healthComponent.maxHp;

				ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 4.0f));
				if (ImGui::BeginTable("Health_Table", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Borders))
				{
					ImGui::TableSetupColumn("attribute", ImGuiTableColumnFlags_WidthFixed);		// 第一列固定宽度
					ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_WidthStretch);		// 第二列自动填充

					auto inputFlag = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll;

					// 显示当前生命值
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("当前生命值"));
					ImGui::TableSetColumnIndex(1);
					ImGui::InputDouble(U8_TO_CHARPTR("##当前生命值_InputDouble"), &hp, 1, 10, "%.0f", inputFlag);

					// 显示最大生命值
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("最大生命值"));
					ImGui::TableSetColumnIndex(1);
					ImGui::InputDouble(U8_TO_CHARPTR("##最大生命值_InputDouble"), &maxHp, 1, 10, "%.0f", inputFlag);

					ImGui::EndTable();
				}
				ImGui::PopStyleVar();

				if (hp < 0.0)
				{
					hp = tempHp;	// 限制当前生命值不能小于0
				}
				if (maxHp < hp)
				{
					maxHp = hp;		// 限制最大生命值不能小于当前生命值
				}
			}

			// 设置 HealthBar生命条组件属性
			if (ImGui::CollapsingHeader(U8_TO_CHARPTR("生命条组件属性##HealthBarComponent")))
			{
				auto& assetId = healthBarComponent.assetId;
				auto& borderColor = healthBarComponent.borderColor;
				auto& fillColor = healthBarComponent.fillColor;
				auto& backgroundColor = healthBarComponent.backgroundColor;

				const auto& fontAssets = assetStore->GetAllFontAssets();
				const auto& fontAssetIds = assetStore->GetAllFontAssetIds(SortFlag::ASCENDING);

				static std::unordered_map<std::string, std::unique_ptr<SDL_Texture, SDLTextureDeleter>> fontTextureMap; // 预览字体样式的纹理缓存
				static std::unordered_map<int, std::unique_ptr<SDL_Texture, SDLTextureDeleter>> hpTextureMap;			// 预览血条文本的纹理缓存（0%-100%）

				ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 4.0f));
				if (ImGui::BeginTable("HealthBar_Table", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Borders))
				{
					ImGui::TableSetupColumn("attribute", ImGuiTableColumnFlags_WidthFixed);		// 第一列固定宽度
					ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_WidthStretch);		// 第二列自动填充

					auto inputFlag = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll;

					// 显示当前血条/文本初始颜色
					static ImColor tempFillColor = { fillColor.r, fillColor.g, fillColor.b, fillColor.a };
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("血条/文本初始颜色"));
					ImGui::TableSetColumnIndex(1);
					if (ImGui::ColorEdit4(U8_TO_CHARPTR("##血条/文本初始颜色_ColorEdit4"), (float*)&(tempFillColor.Value)))
					{
						// 更新血条/文本初始颜色
						ImU32 colorU32 = ImGui::ColorConvertFloat4ToU32(tempFillColor.Value);
						fillColor.r = (colorU32 >> IM_COL32_R_SHIFT) & 0xFF;
						fillColor.g = (colorU32 >> IM_COL32_G_SHIFT) & 0xFF;
						fillColor.b = (colorU32 >> IM_COL32_B_SHIFT) & 0xFF;
						fillColor.a = (colorU32 >> IM_COL32_A_SHIFT) & 0xFF;

						// 清空预览字体样式的纹理缓存
						fontTextureMap.clear();
						// 清空预览血条文本的纹理缓存
						hpTextureMap.clear();
					}

					// 显示当前字体样式选择
					static int fontAssetIdsSelectIdx = 0;
					static std::string assetIdSelected = U8_TO_CHARPTR("请选择字体样式资源");
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("字体样式资源"));
					ImGui::TableSetColumnIndex(1);
					if (ImGui::BeginCombo(U8_TO_CHARPTR("##字体样式资源_BeginCombo"), assetIdSelected.c_str(), ImGuiComboFlags_HeightRegular))
					{
						for (int i = 0; i < fontAssets.size(); ++i)
						{
							bool isSelected = (fontAssetIdsSelectIdx == i);

							if (ImGui::Selectable(fontAssetIds[i].c_str(), isSelected, ImGuiComboFlags_HeightRegular))
							{
								fontAssetIdsSelectIdx = i;
								ImGui::SetItemDefaultFocus();
								assetId = fontAssetIds[i];			// 更新字体样式
								assetIdSelected = fontAssetIds[i];	// 更新选择的字体样式
								hpTextureMap.clear();				// 清空预览血条文本的纹理缓存
							}

							//如果悬停，显示字体样式预览，文本为“这是字体样式预览 This is font preview”
							if (ImGui::IsItemHovered())
							{
								// 获取选中的字体样式
								auto& hoveredFont = fontAssets.at(fontAssetIds[i]);

								if (hoveredFont)
								{
									auto text = U8_TO_CHARPTR("这是字体样式预览 This is font preview");

									if (fontTextureMap.find(fontAssetIds[i]) == fontTextureMap.end())
									{
										// 如果字体纹理不存在，则创建
										SDL_Surface* surface = TTF_RenderUTF8_Blended(hoveredFont, text, fillColor);
										if (surface)
										{
											SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
											fontTextureMap.emplace(fontAssetIds[i], texture);
										}
										SDL_FreeSurface(surface);
									}

									int texWidth = 0, texHeight = 0;
									auto hoveredTexture = fontTextureMap[fontAssetIds[i]].get();
									SDL_QueryTexture(hoveredTexture, nullptr, nullptr, &texWidth, &texHeight);
									ImVec2 textSize((float)texWidth, (float)texHeight);

									// 获取当前 Selectable 的屏幕位置
									ImVec2 itemPos = ImGui::GetItemRectMin();

									// 设置新窗口的位置：右边紧邻
									ImVec2 previewPos = ImVec2(itemPos.x + ImGui::GetItemRectSize().x + 10.0f, itemPos.y);  // 可调间距

									ImGui::SetNextWindowPos(previewPos, ImGuiCond_Always);

									// 样式：无边框、无背景、无标题
									ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
									ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
									ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

									ImGuiWindowFlags imageFlags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;
									ImGui::Begin("TextPreview", nullptr, imageFlags);
									if (hoveredTexture)
									{
										ImGui::Image((ImTextureID)hoveredTexture, textSize); // 显示图片（RGBA纹理）
									}
									else
									{
										ImGui::Text(U8_TO_CHARPTR("❌字体样式加载失败"));
									}
									ImGui::End();
									ImGui::PopStyleVar(3);
								}
							}
						}
						ImGui::EndCombo();
					}

					// 显示当前边框颜色
					static ImColor tempBorderColor = { borderColor.r, borderColor.g, borderColor.b, borderColor.a };
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("边框颜色"));
					ImGui::TableSetColumnIndex(1);
					if (ImGui::ColorEdit4(U8_TO_CHARPTR("##边框颜色_ColorEdit4"), (float*)&(tempBorderColor.Value)))
					{
						// 更新边框颜色
						ImU32 colorU32 = ImGui::ColorConvertFloat4ToU32(tempBorderColor.Value);
						borderColor.r = (colorU32 >> IM_COL32_R_SHIFT) & 0xFF;
						borderColor.g = (colorU32 >> IM_COL32_G_SHIFT) & 0xFF;
						borderColor.b = (colorU32 >> IM_COL32_B_SHIFT) & 0xFF;
						borderColor.a = (colorU32 >> IM_COL32_A_SHIFT) & 0xFF;
					}

					// 显示当前背景颜色
					static ImColor tempBackgroundColor = { backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a };
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("背景颜色"));
					ImGui::TableSetColumnIndex(1);
					if (ImGui::ColorEdit4(U8_TO_CHARPTR("##背景颜色_ColorEdit4"), (float*)&(tempBackgroundColor.Value)))
					{
						// 更新背景颜色
						ImU32 colorU32 = ImGui::ColorConvertFloat4ToU32(tempBackgroundColor.Value);
						backgroundColor.r = (colorU32 >> IM_COL32_R_SHIFT) & 0xFF;
						backgroundColor.g = (colorU32 >> IM_COL32_G_SHIFT) & 0xFF;
						backgroundColor.b = (colorU32 >> IM_COL32_B_SHIFT) & 0xFF;
						backgroundColor.a = (colorU32 >> IM_COL32_A_SHIFT) & 0xFF;
					}

					// 显示血条预览
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("血条预览"));
					ImGui::TableSetColumnIndex(1);
					if (fontAssets.find(assetId) != fontAssets.end()) // 确保字体样式存在
					{
						// 获取当前血量和最大血量（假设有这两个数据）
						auto currentHealth = healthComponent.hp;
						auto maxHealth = healthComponent.maxHp;
						int healthPercentage = static_cast<int>((currentHealth / maxHealth) * 100);
						std::string healthText = std::to_string(healthPercentage) + "%";

						// 创建血条文本纹理
						if (hpTextureMap.find(healthPercentage) == hpTextureMap.end())
						{
							// 如果血条文本纹理不存在，则创建
							SDL_Surface* surface = TTF_RenderUTF8_Blended(fontAssets.at(assetId), healthText.c_str(), fillColor);
							if (surface)
							{
								SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
								hpTextureMap.emplace(healthPercentage, texture);
							}
							SDL_FreeSurface(surface);
						}

						// 计算血条的填充宽度（假设最大宽度为 200）
						float healthBarWidth = 200.0f;
						float healthWidth = (currentHealth / maxHealth) * healthBarWidth; // 当前血量对应的宽度

						// 获取绘制区域（可调整大小）
						ImVec2 pos = ImGui::GetCursorScreenPos();
						pos.y += 8.0f;
						ImVec2 size(healthBarWidth, 5.0f);  // 设置血条的高度为5

						// 获取ImGui的DrawList
						ImDrawList* drawList = ImGui::GetWindowDrawList();

						// 绘制背景（背景颜色为 backgroundColor）
						drawList->AddRectFilled(pos, ImVec2(pos.x + healthBarWidth, pos.y + size.y), tempBackgroundColor);

						// 绘制血条（血条颜色为 fillColor）
						drawList->AddRectFilled(pos, ImVec2(pos.x + healthWidth, pos.y + size.y), tempFillColor);

						// 绘制边框（边框颜色为 borderColor）
						drawList->AddRect(pos, ImVec2(pos.x + healthBarWidth, pos.y + size.y), tempBorderColor);

						// 绘制血条百分比文本
						int texWidth = 0, texHeight = 0;
						auto hpTexture = hpTextureMap[healthPercentage].get();
						SDL_QueryTexture(hpTexture, nullptr, nullptr, &texWidth, &texHeight);
						ImVec2 textSize((float)texWidth, (float)texHeight);
						ImVec2 textPos(pos.x + healthBarWidth + 5.0f, pos.y + (size.y - textSize.y) * 0.5f); // 文本显示在血条右侧

						// 显示图片（RGBA纹理）设置位置
						ImGui::SetCursorScreenPos(textPos);
						ImGui::Image((ImTextureID)hpTexture, textSize);
					}

					ImGui::EndTable();
				}
				ImGui::PopStyleVar();

			}

			// 设置 Transform变换组件属性
			if (ImGui::CollapsingHeader(U8_TO_CHARPTR("变换组件属性##TransformComponent")))
			{
				auto tempPos = transformComponent.position;
				auto tempAngle = transformComponent.rotation;
				auto tempScale = transformComponent.scale;

				auto& pos = transformComponent.position;
				auto& angle = transformComponent.rotation;
				auto& scale = transformComponent.scale;

				ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 4.0f));
				if (ImGui::BeginTable("Transform_Table", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Borders))
				{
					ImGui::TableSetupColumn("attribute", ImGuiTableColumnFlags_WidthFixed);		// 第一列固定宽度
					ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_WidthStretch);		// 第二列自动填充

					auto inputFlag = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll;

					// 显示当前X轴位置
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("X轴位置"));
					ImGui::TableSetColumnIndex(1);
					ImGui::InputDouble(U8_TO_CHARPTR("##X轴位置_InputDouble"), &pos.x, 1, 10, "%.2f", inputFlag);

					// 显示当前Y轴位置
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("Y轴位置"));
					ImGui::TableSetColumnIndex(1);
					ImGui::InputDouble(U8_TO_CHARPTR("##Y轴位置_InputDouble"), &pos.y, 1, 10, "%.2f", inputFlag);

					// 显示当前旋转角度
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("旋转角度"));
					ImGui::TableSetColumnIndex(1);
					ImGui::InputDouble(U8_TO_CHARPTR("##旋转角度_InputDouble"), &angle, 1, 10, "%.2f", inputFlag);

					// 显示当前X轴缩放
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("X轴缩放"));
					ImGui::TableSetColumnIndex(1);
					ImGui::InputDouble(U8_TO_CHARPTR("##X轴缩放_InputDouble"), &scale.x, 0.1, 1, "%.2f", inputFlag);

					// 显示当前Y轴缩放
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("Y轴缩放"));
					ImGui::TableSetColumnIndex(1);
					ImGui::InputDouble(U8_TO_CHARPTR("##Y轴缩放_InputDouble"), &scale.y, 0.1, 1, "%.2f", inputFlag);

					ImGui::EndTable();
				}
				ImGui::PopStyleVar();

				transformComponent.rotation = fmod(angle, 360.0); // 确保角度在0-360度之间
				if (scale.x < 0.0 || scale.x > 100.0)
				{
					scale.x = tempScale.x;	// 限制缩放比例
				}
				if (scale.y < 0.0 || scale.y > 100.0)
				{
					scale.y = tempScale.y;	// 限制缩放比例
				}
			}

			// 设置 RigidBody刚体组件属性
			if (ImGui::CollapsingHeader(U8_TO_CHARPTR("刚体组件属性##rigidBodyComponent")))
			{
				auto tempSpeed = rigidBodyComponent.speed;
				auto tempVelocity = rigidBodyComponent.velocity;

				auto& speed = rigidBodyComponent.speed;
				auto& velocity = rigidBodyComponent.velocity;
				static double angle = 0.0; // 速度方向角度

				ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 4.0f));
				if (ImGui::BeginTable("RigidBody_Table", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Borders))
				{
					ImGui::TableSetupColumn("attribute", ImGuiTableColumnFlags_WidthFixed);		// 第一列固定宽度
					ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_WidthStretch);		// 第二列自动填充

					auto inputFlag = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll;

					// 显示当前速率
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("速率"));
					ImGui::TableSetColumnIndex(1);
					ImGui::InputDouble(U8_TO_CHARPTR("##速率_InputDouble"), &speed, 0.1, 1, "%.2f", inputFlag);

					// 显示当前速度方向
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("速度方向"));
					ImGui::TableSetColumnIndex(1);
					if (SliderAngleDeg(U8_TO_CHARPTR("##速度方向_SliderAngleDeg"), &angle, 0.0, 360.0, "%.2f", ImGuiSliderFlags_AlwaysClamp)) {
						// 将角度转换为弧度
						double rad = glm::radians(angle - 90);
						velocity.x = std::cos(rad);
						velocity.y = std::sin(rad);
					}

					// 显示当前不同方向的速度分量
					std::string velocityX = std::format("{:.2f}", velocity.x * speed);
					std::string velocityY = std::format("{:.2f}", velocity.y * speed);
					std::string velocityText = U8_TO_CHARPTR("X轴速度: ") + velocityX + " | " + U8_TO_CHARPTR("Y轴速度: ") + velocityY;
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), velocityText.c_str());

					ImGui::EndTable();
				}
				ImGui::PopStyleVar();
			}

			// 设置 Sprite精灵组件属性
			if (ImGui::CollapsingHeader(U8_TO_CHARPTR("精灵组件属性##SpriteComponent")))
			{
				auto& assetId = spriteComponent.assetId;
				auto& size = spriteComponent.size;
				auto& layer = spriteComponent.layer;
				auto& srcRect = spriteComponent.srcRect;

				const auto& imageAssets = assetStore->GetAllImageAssets();
				const auto& imageInfos = assetStore->GetAllImageInfos();
				const auto& imageAssetIds = assetStore->GetAllImageAssetIds(SortFlag::ASCENDING);

				ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 4.0f));
				if (ImGui::BeginTable("RigidBody_Table", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Borders))
				{
					ImGui::TableSetupColumn("attribute", ImGuiTableColumnFlags_WidthFixed);		// 第一列固定宽度
					ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_WidthStretch);		// 第二列自动填充

					// 显示当前贴图资源名称
					static int imageAssetIdsSelectIdx = 0;
					static std::string assetIdSelected = U8_TO_CHARPTR("请选择贴图资源");
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("贴图资源名称"));
					ImGui::TableSetColumnIndex(1);
					if (ImGui::BeginCombo(U8_TO_CHARPTR("##贴图资源名称_BeginCombo"), assetIdSelected.c_str(), ImGuiComboFlags_HeightRegular))
					{
						for (int i = 0; i < imageAssetIds.size(); ++i)
						{
							bool is_selected = (imageAssetIdsSelectIdx == i);
							if (ImGui::Selectable(imageAssetIds[i].c_str(), is_selected))
							{
								imageAssetIdsSelectIdx = i;
								ImGui::SetItemDefaultFocus();
								assetId = imageAssetIds[i];  // 更新选中的资源ID
								assetIdSelected = imageAssetIds[i]; // 更新选中的资源名称
							}

							// 如果悬停，显示图片
							if (ImGui::IsItemHovered())
							{
								// 获取选中的图片资产
								auto& hoveredTexture = imageAssets.at(imageAssetIds[i]);  // 获取悬停的图片资产

								if (hoveredTexture)
								{
									int texWidth = 0, texHeight = 0;
									SDL_QueryTexture(hoveredTexture, nullptr, nullptr, &texWidth, &texHeight);

									if (texWidth > 0 && texHeight > 0)
									{
										// 图片尺寸信息
										std::string sizeInfo = std::to_string(texWidth) + U8_TO_CHARPTR("×") + std::to_string(texHeight);

										ImVec2 imageSize((float)texWidth, (float)texHeight);

										// 获取当前 Selectable 的屏幕位置
										ImVec2 itemPos = ImGui::GetItemRectMin();

										// 设置新窗口的位置：右边紧邻
										ImVec2 previewPos = ImVec2(itemPos.x + ImGui::GetItemRectSize().x + 10.0f, itemPos.y);  // 可调间距

										ImGui::SetNextWindowPos(previewPos, ImGuiCond_Always);

										// 样式：无边框、无背景、无标题
										ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
										ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
										ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

										ImGuiWindowFlags imageFlags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;
										ImGui::Begin("ImagePreview", nullptr, imageFlags);
										ImGui::Text("%s", sizeInfo.c_str()); // 显示尺寸信息
										ImGui::Image((ImTextureID)hoveredTexture, imageSize); // 显示图片（RGBA纹理）
										ImGui::End();
										ImGui::PopStyleVar(3);
									}
								}
							}
						}
						ImGui::EndCombo();
					}

					// 显示当前精灵宽度和长度
					ImageInfo imageInfo;
					if (imageInfos.find(assetId) != imageInfos.end())
					{
						imageInfo = imageInfos.at(assetId);
					}
					size.x = imageInfo.width;
					size.y = imageInfo.height;
					srcRect = { 0, 0, static_cast<int>(size.x), static_cast<int>(size.y) };
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					std::string spriteWidth = U8_TO_CHARPTR("精灵宽度: ") + std::to_string(size.x);
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), spriteWidth.c_str());
					ImGui::TableSetColumnIndex(1);
					std::string spriteHeight = U8_TO_CHARPTR("精灵高度: ") + std::to_string(size.y);
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), spriteHeight.c_str());

					// 显示当前渲染层
					static int renderLayerSelectIdx = 0;
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("渲染层"));
					ImGui::TableSetColumnIndex(1);
					if (ImGui::BeginCombo(U8_TO_CHARPTR("##渲染层_BeginCombo"), RenderLayerText(static_cast<RenderLayer>(renderLayerSelectIdx)), ImGuiComboFlags_HeightRegular))
					{
						for (int i = 0; i < static_cast<int>(RenderLayer::Count); ++i)
						{
							bool is_selected = (renderLayerSelectIdx == i);
							if (ImGui::Selectable(RenderLayerText(static_cast<RenderLayer>(i)), is_selected))
							{
								renderLayerSelectIdx = i;
								ImGui::SetItemDefaultFocus();
								layer = static_cast<RenderLayer>(i);  // 更新选中的渲染层
							}
						}
						ImGui::EndCombo();
					}

					ImGui::EndTable();
				}
				ImGui::PopStyleVar();
			}

			// 设置 BoxCollider盒子碰撞组件属性
			if (ImGui::CollapsingHeader(U8_TO_CHARPTR("盒子碰撞组件属性##BoxColliderComponent")))
			{
				auto tempSize = boxColliderComponent.size;
				auto tempOffset = boxColliderComponent.offset;

				auto& size = boxColliderComponent.size;
				auto& offset = boxColliderComponent.offset;

				ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 4.0f));
				if (ImGui::BeginTable("BoxCollider_Table", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Borders))
				{
					ImGui::TableSetupColumn("attribute", ImGuiTableColumnFlags_WidthFixed);		// 第一列固定宽度
					ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_WidthStretch);		// 第二列自动填充

					auto inputFlag = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll;

					// 显示当前盒子碰撞组件宽度
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("碰撞盒宽度W"));
					ImGui::TableSetColumnIndex(1);
					ImGui::InputDouble(U8_TO_CHARPTR("##碰撞盒宽度_InputDouble"), &size.x, 1, 10, "%.2f", inputFlag);

					// 显示当前盒子碰撞组件高度
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("碰撞盒高度H"));
					ImGui::TableSetColumnIndex(1);
					ImGui::InputDouble(U8_TO_CHARPTR("##碰撞盒高度_InputDouble"), &size.y, 1, 10, "%.2f", inputFlag);

					// 显示当前盒子碰撞组件X轴偏移
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("碰撞盒X轴偏移"));
					ImGui::TableSetColumnIndex(1);
					ImGui::InputDouble(U8_TO_CHARPTR("##碰撞盒X轴偏移_InputDouble"), &offset.x, 1, 10, "%.2f", inputFlag);

					// 显示当前盒子碰撞组件Y轴偏移
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("碰撞盒Y轴偏移"));
					ImGui::TableSetColumnIndex(1);
					ImGui::InputDouble(U8_TO_CHARPTR("##碰撞盒Y轴偏移_InputDouble"), &offset.y, 1, 10, "%.2f", inputFlag);

					ImGui::EndTable();

					if (size.x < 0.0)
					{
						size.x = tempSize.x;	// 限制碰撞盒宽度
					}
					if (size.y < 0.0)
					{
						size.y = tempSize.y;	// 限制碰撞盒高度
					}
				}
				ImGui::PopStyleVar();
			}

			// 设置 ProjectileEmitter发射器组件属性
			if (ImGui::CollapsingHeader(U8_TO_CHARPTR("发射器组件属性##ProjectileEmitterComponent")))
			{
				auto tempIsEmit = projectileEmitterComponent.isEmit;
				auto tempRepeatFrequency = projectileEmitterComponent.repeatFrequency;
				auto tempProjectileDuration = projectileEmitterComponent.projectileDuration;
				auto tempHitDamage = projectileEmitterComponent.hitDamage;
				auto tempSpeed = projectileEmitterComponent.speed;

				auto& isEmit = projectileEmitterComponent.isEmit;
				auto& repeatFrequency = projectileEmitterComponent.repeatFrequency;
				auto& projectileDuration = projectileEmitterComponent.projectileDuration;
				auto& hitDamage = projectileEmitterComponent.hitDamage;
				auto& speed = projectileEmitterComponent.speed;

				ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 4.0f));
				if (ImGui::BeginTable("ProjectileEmitter_Table", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Borders))
				{
					ImGui::TableSetupColumn("attribute", ImGuiTableColumnFlags_WidthFixed);		// 第一列固定宽度
					ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_WidthStretch);		// 第二列自动填充

					auto inputFlag = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll;

					// 显示当前是否开启发射器功能
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("开启发射器功能"));
					ImGui::TableSetColumnIndex(1);
					ImGui::Checkbox(U8_TO_CHARPTR("##开启发射器功能_Checkbox"), &isEmit);

					// 显示当前发射器重复发射频率
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("重复发射频率（毫秒/每次）"));
					ImGui::TableSetColumnIndex(1);
					ImGui::InputInt(U8_TO_CHARPTR("##重复发射频率_InputInt"), &repeatFrequency, 10, 100, inputFlag);

					// 显示当前发射物持续时间
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("发射物持续时间（毫秒）"));
					ImGui::TableSetColumnIndex(1);
					ImGui::InputInt(U8_TO_CHARPTR("##发射物持续时间_InputInt"), &projectileDuration, 10, 100, inputFlag);

					// 显示当前发射物伤害值
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("发射物伤害值"));
					ImGui::TableSetColumnIndex(1);
					ImGui::InputDouble(U8_TO_CHARPTR("##发射物伤害值_InputDouble"), &hitDamage, 1, 10, "%.2f", inputFlag);

					// 显示当前发射物速度
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(U8_TO_CHARPTR("发射物速度"));
					ImGui::TableSetColumnIndex(1);
					ImGui::InputDouble(U8_TO_CHARPTR("##发射物速度_InputDouble"), &speed, 1, 10, "%.2f", inputFlag);

					ImGui::EndTable();

					if (repeatFrequency < 1)
					{
						repeatFrequency = tempRepeatFrequency;			// 限制发射器重复发射频率不能小于1ms
					}
					if (projectileDuration < 0)
					{
						projectileDuration = tempProjectileDuration;	// 限制发射物持续时间不能小于0ms
					}
					if (hitDamage < 0.0)
					{
						hitDamage = tempHitDamage;						// 限制发射物伤害值不能小于0
					}
					if (speed < 0.0)
					{
						speed = tempSpeed;								// 限制发射物速度不能小于0
					}
				}
				ImGui::PopStyleVar();
			}

			if (ImGui::Button(U8_TO_CHARPTR("创建敌人")))
			{
				CreateEntity(registry);
			}

			if (ImGui::Button(U8_TO_CHARPTR("创建多个敌人")))
			{
				// 启动多线程执行
				std::thread t([this, &registry]() {SpawnEnemiesWithDelay(registry); });
				t.detach(); // 不阻塞主线程，让其独立执行
			}
		}
		ImGui::End();

		// 在左下角显示坐标
		ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y - 20), ImGuiCond_Always);
		ImGui::SetNextWindowBgAlpha(0.6f); // 设置背景透明度

		// 样式：无边框、无背景、无标题
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGuiWindowFlags textFlags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;

		ImGui::Begin("Coordinates", nullptr, textFlags);
		ImVec2 mousePos = { ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y };
		if (mousePos.x < 0 || mousePos.y < 0)
		{
			mousePos = { 0.0f, 0.0f };
		}
		ImGui::Text(U8_TO_CHARPTR("当前坐标：(%.2f | %.2f)"), mousePos.x, mousePos.y);
		ImGui::End();

		ImGui::PopStyleVar(3);

		ImGui::ShowDemoWindow();   
		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
		
	}

private:
	void SpawnEnemiesWithDelay(std::unique_ptr<Registry>& registry) {
		for (double angle = 0; angle < 360; angle += 10) {
			// 计算方向向量
			double rad = glm::radians(angle - 90);
			auto enemyVelocityX = std::cos(rad);
			auto enemyVelocityY = std::sin(rad);
			rigidBodyComponent.velocity = { enemyVelocityX, enemyVelocityY };
			CreateEntity(registry);

			SDL_Delay(1500);
		}
	}


	// 滑块角度输入，单位：度
	bool SliderAngleDeg(const char* label, double* v_deg, double v_degrees_min = 0.0, double v_degrees_max = 360.0, const char* format = "%.0f deg", ImGuiSliderFlags flags = 0)
	{
		ImGuiIO& io = ImGui::GetIO();
		// 显示滑块，单位：度
		bool changed = ImGui::SliderScalar(label, ImGuiDataType_Double, v_deg, &v_degrees_min, &v_degrees_max, format, flags);

		// 鼠标悬停时用滚轮±1°
		if (ImGui::IsItemHovered()) {
			float wheel = io.MouseWheel;
			if (wheel != 0.0f) {
				// 默认步长：±1.0 度
				double step = 1.0;

				// Alt：慢速模式（±0.01°）
				if (io.KeyAlt && !io.KeyCtrl)
				{
					step = 0.01;
				}

				// Ctrl：快速模式（±0.01°，但连续滚动加倍）
				else if (io.KeyCtrl && !io.KeyAlt)
				{
					step = 0.1;
				}

				// Ctrl + Alt 同时按下：也可以定义其他行为（可选）
				// else if (io.KeyCtrl && io.KeyAlt)
				//     step = 0.001;

				// 应用步长
				*v_deg += (wheel > 0.0f) ? step : -step;

				// 限制范围
				*v_deg = std::clamp(*v_deg, v_degrees_min, v_degrees_max);

				changed = true;
			}
		}

		return changed;
	}

	// 虚拟化下拉框，支持大量数据
	bool ComboVirtual(const std::string label, int& current_item, const std::vector<std::string>& items, int visible_items = 10, ImGuiComboFlags flags = ImGuiComboFlags_None)
	{
		bool changed = false;

		if (ImGui::BeginCombo(label.c_str(), items[current_item].c_str(), flags))
		{
			// 计算每个条目的高度，控制下拉框的最大高度（基于 visible_items）
			float item_height = ImGui::GetTextLineHeightWithSpacing(); // 获取每个条目的高度
			float max_height = item_height * visible_items;  // 最大高度基于 visible_items

			// 设置下拉框最大显示高度
			ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, max_height));

			// 开启列表剪裁器（虚拟化）
			ImGuiListClipper clipper;
			clipper.Begin((int)items.size(), item_height);  // 第一个参数是总项数，第二个是每行的高度

			while (clipper.Step())
			{
				for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
				{
					const bool is_selected = (current_item == i);
					if (ImGui::Selectable(items[i].c_str(), is_selected)) {
						current_item = i;
						changed = true;
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
			}

			clipper.End();
			ImGui::EndCombo();
		}

		return changed;
	}


	void CreateEntity(std::unique_ptr<Registry>& registry)
	{
		Entity entity = registry->CreateEntity();
		entity.AddTag(tagName);					// 添加标签
		entity.AddGroup(groupName);				// 添加组

		entity.AddComponent<TransformComponent>(transformComponent);					// 添加变换组件
		entity.AddComponent<RigidBodyComponent>(rigidBodyComponent);					// 添加刚体组件
		entity.AddComponent<SpriteComponent>(spriteComponent);							// 添加精灵组件
		entity.AddComponent<BoxColliderComponent>(boxColliderComponent);				// 添加碰撞盒组件
		entity.AddComponent<ProjectileEmitterComponent>(projectileEmitterComponent);	// 添加子弹发射组件
		entity.AddComponent<CampComponent>(campComponent);								// 添加阵营组件
		entity.AddComponent<HealthComponent>(healthComponent);							// 添加生命值组件
		entity.AddComponent<HealthBarComponent>(healthBarComponent);					// 添加生命条组件
	}

private:
	std::string tagName;										// 标签名称
	std::string groupName;										// 组名称
	TransformComponent transformComponent;						// 变换组件
	RigidBodyComponent rigidBodyComponent;						// 刚体组件
	SpriteComponent spriteComponent;							// 精灵组件
	BoxColliderComponent boxColliderComponent;					// 碰撞盒组件
	ProjectileEmitterComponent projectileEmitterComponent;		// 子弹发射组件
	CampComponent campComponent;								// 阵营组件
	HealthComponent healthComponent;							// 生命值组件
	HealthBarComponent healthBarComponent;						// 生命条组件
};

#endif // !RENDERGUISYSTEM_H
