#include <string>
#include <vector>

#include "CheatManager.hpp"
#include "GUI.hpp"
#include "Memory.hpp"
#include "SkinDatabase.hpp"
#include "Utils.hpp"
#include "fnv_hash.hpp"
#include "imgui/imgui.h"

inline void footer() noexcept
{
	using namespace std::string_literals;
	static const auto buildText{ "Son Yapi: "s + __DATE__ + " - " + __TIME__ };
	ImGui::Separator();
	ImGui::textUnformattedCentered(buildText.c_str());
	ImGui::textUnformattedCentered("Copyright (C) 2022 = Kral");
}

static void changeTurretSkin(const std::int32_t skinId, const std::int32_t team) noexcept
{
	if (skinId == -1 || skinId == 8) // ignore broken arcade turret skin
		return;

	const auto turrets{ cheatManager.memory->turretList };
	const auto playerTeam{ cheatManager.memory->localPlayer->get_team() };

	for (auto i{ 0u }; i < turrets->length; ++i) {
		const auto turret{ turrets->list[i] };
		if (turret->get_team() == team) {
			if (playerTeam == team) {
				turret->get_character_data_stack()->base_skin.skin = skinId * 2;
				turret->get_character_data_stack()->update(true);
			} else {
				turret->get_character_data_stack()->base_skin.skin = skinId * 2 + 1;
				turret->get_character_data_stack()->update(true);
			}
		}
	}
}

void GUI::render() noexcept
{
	static const auto player{ cheatManager.memory->localPlayer };
	static const auto heroes{ cheatManager.memory->heroList };
	static const auto my_team{ player ? player->get_team() : 100 };

	static const auto vector_getter_skin = [](void* vec, std::int32_t idx, const char** out_text) noexcept {
		const auto& vector{ *static_cast<std::vector<SkinDatabase::skin_info>*>(vec) };
		if (idx < 0 || idx > static_cast<std::int32_t>(vector.size())) return false;
		*out_text = idx == 0 ? "Varsayilan" : vector.at(idx - 1).skin_name.c_str();
		return true;
	};

	static const auto vector_getter_ward_skin = [](void* vec, std::int32_t idx, const char** out_text) noexcept {
		const auto& vector{ *static_cast<std::vector<std::pair<std::int32_t, const char*>>*>(vec) };
		if (idx < 0 || idx > static_cast<std::int32_t>(vector.size())) return false;
		*out_text = idx == 0 ? "Varsayilan" : vector.at(idx - 1).second;
		return true;
	};

	static auto vector_getter_default = [](void* vec, std::int32_t idx, const char** out_text) noexcept {
		const auto& vector{ *static_cast<std::vector<const char*>*>(vec) };
		if (idx < 0 || idx > static_cast<std::int32_t>(vector.size())) return false;
		*out_text = idx == 0 ? "Varsayilan" : vector.at(idx - 1);
		return true;
	};

	ImGui::Begin("KralSkin", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::rainbowText();
		if (ImGui::BeginTabBar("TabBar", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_NoTooltip)) {
			if (player) {
				if (ImGui::BeginTabItem("YerelOyuncu")) {
					auto& values{ cheatManager.database->champions_skins[fnv::hash_runtime(player->get_character_data_stack()->base_skin.model.str)] };
					ImGui::Text("Oyuncu Kostum Ayarlari:");

					if (ImGui::Combo("Mevcut Kostum", &cheatManager.config->current_combo_skin_index, vector_getter_skin, static_cast<void*>(&values), values.size() + 1))
						if (cheatManager.config->current_combo_skin_index > 0)
							player->change_skin(values[cheatManager.config->current_combo_skin_index - 1].model_name.c_str(), values[cheatManager.config->current_combo_skin_index - 1].skin_id);

					if (ImGui::Combo("Mevcut Totem", &cheatManager.config->current_combo_ward_index, vector_getter_ward_skin, static_cast<void*>(&cheatManager.database->wards_skins), cheatManager.database->wards_skins.size() + 1))
						cheatManager.config->current_ward_skin_index = cheatManager.config->current_combo_ward_index == 0 ? -1 : cheatManager.database->wards_skins.at(cheatManager.config->current_combo_ward_index - 1).first;
					footer();
					ImGui::EndTabItem();
				}
			}

			if (ImGui::BeginTabItem("Diger Karakterler")) {
				ImGui::Text("Diger Karakter Kostum Ayarlari:");
				std::int32_t last_team{ 0 };
				for (auto i{ 0u }; i < heroes->length; ++i) {
					const auto hero{ heroes->list[i] };

					if (hero == player)
						continue;

					const auto champion_name_hash{ fnv::hash_runtime(hero->get_character_data_stack()->base_skin.model.str) };
					if (champion_name_hash == FNV("PracticeTool_TargetDummy"))
						continue;

					const auto hero_team{ hero->get_team() };
					const auto is_enemy{ hero_team != my_team };

					if (last_team == 0 || hero_team != last_team) {
						if (last_team != 0)
							ImGui::Separator();
						if (is_enemy)
							ImGui::Text(" Rakip Karakterler");
						else
							ImGui::Text(" Takim Karakterler");
						last_team = hero_team;
					}

					auto& config_array{ is_enemy ? cheatManager.config->current_combo_enemy_skin_index : cheatManager.config->current_combo_ally_skin_index };
					const auto config_entry{ config_array.insert({ champion_name_hash, 0 }) };

					snprintf(this->str_buffer, sizeof(this->str_buffer), cheatManager.config->heroName ? "KarakterAdi: [ %s ]##%X" : "OyuncuAdi: [ %s ]##%X", cheatManager.config->heroName ? hero->get_character_data_stack()->base_skin.model.str : hero->get_name()->c_str(), reinterpret_cast<std::uintptr_t>(hero));

					auto& values{ cheatManager.database->champions_skins[champion_name_hash] };
					if (ImGui::Combo(str_buffer, &config_entry.first->second, vector_getter_skin, static_cast<void*>(&values), values.size() + 1))
						if (config_entry.first->second > 0)
							hero->change_skin(values[config_entry.first->second - 1].model_name.c_str(), values[config_entry.first->second - 1].skin_id);
				}
				footer();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Kuresel Kostumler")) {
				ImGui::Text("Kuresel Kostum Ayarlari:");
				if (ImGui::Combo("Minyon Kostumleri:", &cheatManager.config->current_combo_minion_index, vector_getter_default, static_cast<void*>(&cheatManager.database->minions_skins), cheatManager.database->minions_skins.size() + 1))
					cheatManager.config->current_minion_skin_index = cheatManager.config->current_combo_minion_index - 1;
				ImGui::Separator();
				if (ImGui::Combo("Taret Kostumleri:", &cheatManager.config->current_combo_order_turret_index, vector_getter_default, static_cast<void*>(&cheatManager.database->turret_skins), cheatManager.database->turret_skins.size() + 1))
					changeTurretSkin(cheatManager.config->current_combo_order_turret_index - 1, 100);
				if (ImGui::Combo("Kaos Taret Kostumleri:", &cheatManager.config->current_combo_chaos_turret_index, vector_getter_default, static_cast<void*>(&cheatManager.database->turret_skins), cheatManager.database->turret_skins.size() + 1))
					changeTurretSkin(cheatManager.config->current_combo_chaos_turret_index - 1, 200);
				ImGui::Separator();
				ImGui::Text("Orman Ceteleri Kostum Ayarlari:");
				for (auto& it : cheatManager.database->jungle_mobs_skins) {
					snprintf(str_buffer, 256, "Mevcut %s Kostumler", it.name);
					const auto config_entry{ cheatManager.config->current_combo_jungle_mob_skin_index.insert({ it.name_hashes.front(), 0 }) };
					if (ImGui::Combo(str_buffer, &config_entry.first->second, vector_getter_default, static_cast<void*>(&it.skins), it.skins.size() + 1))
						for (const auto& hash : it.name_hashes)
							cheatManager.config->current_combo_jungle_mob_skin_index[hash] = config_entry.first->second;
				}
				footer();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Ekstra")) {
				ImGui::hotkey("Menu Anahtari", cheatManager.config->menuKey);
				ImGui::Checkbox(cheatManager.config->heroName ? "Kahraman Adi" : "Oyuncu Adi", &cheatManager.config->heroName);
				ImGui::Checkbox("Gokkusagi Metin", &cheatManager.config->rainbowText);
				ImGui::Checkbox("Hizli Cilt Degisimi", &cheatManager.config->quickSkinChange);
				ImGui::hoverInfo("Klavyeden atadiginiz tus ile menuyu acmadan skin degistirmenizi saglar.");

				if (cheatManager.config->quickSkinChange) {
					ImGui::Separator();
					ImGui::hotkey("Onceki Kostum Anahtari", cheatManager.config->previousSkinKey);
					ImGui::hotkey("Sonraki Kostum Anahtari", cheatManager.config->nextSkinKey);
					ImGui::Separator();
				}

				if (player)
					ImGui::InputText("Isim Degistir", player->get_name());

				if (ImGui::Button("Kostum Yok")) {
					if (player) {
						cheatManager.config->current_combo_skin_index = 1;
						player->change_skin(player->get_character_data_stack()->base_skin.model.str, 0);
					}

					for (auto& enemy : cheatManager.config->current_combo_enemy_skin_index)
						enemy.second = 1;

					for (auto& ally : cheatManager.config->current_combo_ally_skin_index)
						ally.second = 1;

					for (auto i{ 0u }; i < heroes->length; ++i) {
						const auto hero{ heroes->list[i] };
						hero->change_skin(hero->get_character_data_stack()->base_skin.model.str, 0);
					}
				} ImGui::hoverInfo("Tum Karakterlerin Kostumu varsayilan olarak ayarlar.");

				if (ImGui::Button("Rastgele Kostum")) {
					for (auto i{ 0u }; i < heroes->length; ++i) {
						const auto hero{ heroes->list[i] };
						const auto championHash{ fnv::hash_runtime(hero->get_character_data_stack()->base_skin.model.str) };
						const auto skinCount{ cheatManager.database->champions_skins[championHash].size() };
						auto& skinDatabase{ cheatManager.database->champions_skins[championHash] };
						auto& config{ (hero->get_team() != my_team) ? cheatManager.config->current_combo_enemy_skin_index : cheatManager.config->current_combo_ally_skin_index };

						if (championHash == FNV("PracticeTool_TargetDummy"))
							continue;

						if (hero == player) {
							cheatManager.config->current_combo_skin_index = random(1u, skinCount);
							hero->change_skin(skinDatabase[cheatManager.config->current_combo_skin_index - 1].model_name.c_str(), skinDatabase[cheatManager.config->current_combo_skin_index - 1].skin_id);
						} else {
							auto& data{ config[championHash] };
							data = random(1u, skinCount);
							hero->change_skin(skinDatabase[data - 1].model_name.c_str(), skinDatabase[data - 1].skin_id);
						}
					}
				} ImGui::hoverInfo("Tum Karakterlerin Kostumu rastgele degistirir.");

				if (ImGui::Button("Kapatmaya Zorla"))
					cheatManager.hooks->uninstall();
				ImGui::hoverInfo("Yeniden baglanma ekranina geri doneceksiniz.");
				ImGui::Text("FPS: %.0f FPS", ImGui::GetIO().Framerate);
				footer();
				ImGui::EndTabItem();
			}
		}
	}
	ImGui::End();
}
