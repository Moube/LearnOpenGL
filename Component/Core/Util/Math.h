#pragma once


namespace Core
{
	class Math
	{
	public:
		template<class T>
		static T Clamp(const T& _this, const T& _min, const T& _max)
		{
			T temp = _this;
			temp = _this < _min ? _min : _this;
			temp = _this > _max ? _max : _this;
			return temp;
		}
	};
}