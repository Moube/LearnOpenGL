#pragma once

namespace Core
{
	class GLObject
	{
	public:
		GLObject();
		unsigned int getID() const { return id; }
	protected:
		unsigned int id;
	};
}

