
namespace Component
{
	template<class T>
	class Singleton
	{
	public:
		Singleton() {}

		static T* Instance()
		{
			static T instance;
			return &instance;
		}

	};
}