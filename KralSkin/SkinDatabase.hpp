#pragma once

#include <cstdint>
#include <map>
#include <utility>
#include <vector>

#include "fnv_hash.hpp"
 
class SkinDatabase {
public:
	class skin_info {
	public:
		std::string model_name;
		std::string skin_name;
		std::int32_t skin_id;
	};

	class jungle_mob_skin_info {
	public:
		const char* name;
		std::vector<std::uint32_t> name_hashes;
		std::vector<const char*> skins;
	};

	void load() noexcept;

	std::map<std::uint32_t, std::vector<skin_info>> champions_skins;
	std::vector<std::pair<std::uint32_t, const char*>> wards_skins;
	
	std::vector<const char*> minions_skins{
		"Minyon", "Yaz Minyon",
		"Proje Minyon", "Kar Minyon",
		"Draven Minyon", "Yildiz Muhafizi Minyon",
		"Makine Minyon", "Kar 2 Minyon",
		"Macera Minyon", "Mouse Minyon", "Esrarengiz Minyon"
	};

	std::vector<const char*> turret_skins{
		"Varsayilan Siparis Taret", "Varsayilan Kaos Taret",
		"Kar Siparisi Taret", "Kar Kaos Taret",
		"Egilmis Koruluk Duzeni Taret", "Egilmis Koruluk Kaos Taret",
		"URF Siparis Taret", "URF Kaos Taret",
		"[Kirik] Makine Taret", // crash
		"Zambak Tapinagi ve Lotus Taret",
		"Gizemli Duzen Taret", "Gizemli Kaos Taret",
		"Kasap Koprusu Siparis Taret", "Kasap Koprusu Kaos Taret",
		"Uluyan Ucurum Duzeni Taret", "Uluyan Ucurum Kaos Taret"
	};

	std::vector<jungle_mob_skin_info> jungle_mobs_skins{
		{
			"Baron",
			{ FNV("SRU_Baron") },
			{ "Baron", "Kar Yagisi Baron", "Sampiyonlu Baron", "Ay Senligi Baron", "MSI Baron", "Macera Baron", "Sampiyona Dogum Gunu Baron", "Mahvolmus Kral Baron" }
		},
		{
			"Mavi",
			{ FNV("SRU_Blue") },
			{ "Mavi", "Karanlik Mavi", "Havuz Partisi Mavi", "Mahvolmus Kral Mavi" }
		},
		{
			"Kirmizi",
			{ FNV("SRU_Red") },
			{ "Kirmizi", "Havuz Partisi Kirmizi", "Mahvolmus Kral Kirmizi" }
		},
		{
			"Scuttle",
			{ FNV("Sru_Crab") },
			{ "Scuttle", "Cadilar Bayrami Isik Scuttle", "Cadilar Bayrami Karanlik Scuttle", "Ruined King Scuttle" }
		},
		{
			"Krug",
			{ FNV("SRU_Krug"), FNV("SRU_KrugMini"), FNV("SRU_KrugMiniMini") },
			{ "Krug", "Karanlik Krug" }
		},
		{
			"Jilet Gagasi",
			{ FNV("SRU_Razorbeak"), FNV("SRU_RazorbeakMini") },
			{ "Jilet Gagasi", "Tavuk Ustura Gagasi" }
		}
	};
};
