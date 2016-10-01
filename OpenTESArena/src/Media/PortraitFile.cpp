#include <cassert>
#include <unordered_map>

#include "PortraitFile.h"

#include "TextureFile.h"
#include "TextureName.h"
#include "../Entities/CharacterGenderName.h"
#include "../Entities/CharacterRaceName.h"
#include "../Math/Int2.h"

namespace std
{
	// Hash specializations, since GCC doesn't support enum classes used as keys
	// in unordered_maps.
	template <>
	struct hash<CharacterGenderName>
	{
		size_t operator()(const CharacterGenderName &x) const
		{
			return static_cast<size_t>(x);
		}
	};

	template <>
	struct hash<CharacterRaceName>
	{
		size_t operator()(const CharacterRaceName &x) const
		{
			return static_cast<size_t>(x);
		}
	};

	template <>
	struct hash<std::pair<CharacterGenderName, CharacterRaceName>>
	{
		size_t operator()(const std::pair<CharacterGenderName, CharacterRaceName> &x) const
		{
			return std::hash<CharacterGenderName>()(x.first) ^
				std::hash<CharacterRaceName>()(x.second);
		}
	};

	template <>
	struct hash<std::pair<CharacterGenderName, bool>>
	{
		size_t operator()(const std::pair<CharacterGenderName, bool> &x) const
		{
			return std::hash<CharacterGenderName>()(x.first) ^ std::hash<bool>()(x.second);
		}
	};
}

namespace
{
	// Pairings of genders and races to portrait background filenames.
	const std::unordered_map<std::pair<CharacterGenderName, CharacterRaceName>,
		TextureName> BodyTextureNames =
	{
		{ { CharacterGenderName::Female, CharacterRaceName::Argonian }, TextureName::FemaleArgonianBackground },
		{ { CharacterGenderName::Female, CharacterRaceName::Breton }, TextureName::FemaleBretonBackground },
		{ { CharacterGenderName::Female, CharacterRaceName::DarkElf }, TextureName::FemaleDarkElfBackground },
		{ { CharacterGenderName::Female, CharacterRaceName::HighElf }, TextureName::FemaleHighElfBackground},
		{ { CharacterGenderName::Female, CharacterRaceName::Khajiit }, TextureName::FemaleKhajiitBackground},
		{ { CharacterGenderName::Female, CharacterRaceName::Nord }, TextureName::FemaleNordBackground},
		{ { CharacterGenderName::Female, CharacterRaceName::Redguard }, TextureName::FemaleRedguardBackground},
		{ { CharacterGenderName::Female, CharacterRaceName::WoodElf }, TextureName::FemaleWoodElfBackground },

		{ { CharacterGenderName::Male, CharacterRaceName::Argonian }, TextureName::MaleArgonianBackground },
		{ { CharacterGenderName::Male, CharacterRaceName::Breton }, TextureName::MaleBretonBackground },
		{ { CharacterGenderName::Male, CharacterRaceName::DarkElf }, TextureName::MaleDarkElfBackground },
		{ { CharacterGenderName::Male, CharacterRaceName::HighElf }, TextureName::MaleHighElfBackground },
		{ { CharacterGenderName::Male, CharacterRaceName::Khajiit }, TextureName::MaleKhajiitBackground },
		{ { CharacterGenderName::Male, CharacterRaceName::Nord }, TextureName::MaleNordBackground },
		{ { CharacterGenderName::Male, CharacterRaceName::Redguard }, TextureName::MaleRedguardBackground },
		{ { CharacterGenderName::Male, CharacterRaceName::WoodElf }, TextureName::MaleWoodElfBackground }
	};

	// Pixel offsets for shirt textures in the equipment screen.
	const std::unordered_map<std::pair<CharacterGenderName, bool>, Int2> ShirtOffsets =
	{
		{ { CharacterGenderName::Female, false }, Int2(220, 35) },
		{ { CharacterGenderName::Female, true }, Int2(220, 33) },
		{ { CharacterGenderName::Male, false }, Int2(186, 12) },
		{ { CharacterGenderName::Male, true }, Int2(215, 35) }
	};

	// Pixel offsets for pants textures in the equipment screen.
	const std::unordered_map<CharacterGenderName, Int2> PantsOffsets =
	{
		{ CharacterGenderName::Female, Int2(212, 74) },
		{ CharacterGenderName::Male, Int2(229, 82) }
	};
}

const std::string &PortraitFile::getBody(CharacterGenderName gender, CharacterRaceName race)
{
	const TextureName textureName = BodyTextureNames.at(std::make_pair(gender, race));
	const std::string &filename = TextureFile::fromName(textureName);
	return filename;
}

const std::string &PortraitFile::getShirt(CharacterGenderName gender, bool magic)
{
	const TextureName textureName = [gender, magic]()
	{
		if (gender == CharacterGenderName::Female)
		{
			return magic ? TextureName::FemaleMagicShirt : TextureName::FemaleNonMagicShirt;
		}
		else
		{
			return magic ? TextureName::MaleMagicShirt : TextureName::MaleNonMagicShirt;
		}
	}();

	const std::string &filename = TextureFile::fromName(textureName);
	return filename;
}

const std::string &PortraitFile::getPants(CharacterGenderName gender)
{
	const TextureName textureName = (gender == CharacterGenderName::Female) ?
		TextureName::FemalePants : TextureName::MalePants;
	const std::string &filename = TextureFile::fromName(textureName);
	return filename;
}

const Int2 &PortraitFile::getShirtOffset(CharacterGenderName gender, bool magic)
{
	const Int2 &offset = ShirtOffsets.at(std::make_pair(gender, magic));
	return offset;
}

const Int2 &PortraitFile::getPantsOffset(CharacterGenderName gender)
{
	const Int2 &offset = PantsOffsets.at(gender);
	return offset;
}
