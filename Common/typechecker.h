namespace ysbaek
{
	template <typename T>
	struct TypeName
	{
		static const char* Get()
		{
			return typeid(T).name();
		}
	};

	template <>
	struct TypeName<const int>
	{
		/*
		* a specialization for each type of those you want to support
		* and don't like the string returned by typeid
		*/

		static const char* Get()
		{
			return "const int";
		}
	};
}