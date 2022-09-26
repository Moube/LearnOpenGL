#include "Asset.h"

namespace Core
{
	std::string Asset::Path(std::string path)
	{
		std::string rootPath{ "../../Asset/" };
		return rootPath + path;
	}
}