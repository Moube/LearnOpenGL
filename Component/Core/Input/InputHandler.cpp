#include "InputHandler.h"

namespace Core
{
	void InputHandler::Execute()
	{
		handler();
	}

	void InputHandler::Bind(InputExecute _handler)
	{
		handler = _handler;
	}
}