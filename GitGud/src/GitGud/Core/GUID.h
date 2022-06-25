#pragma once

#include <xhash>

namespace GitGud
{
	class GUID
	{
	public:
		GUID();
		GUID(uint64_t id);
		GUID(const GUID&) = default;

		operator uint64_t() const { return _guid; }

	private:
		uint64_t _guid;
	};
}

namespace std
{
	template<>
	struct hash<GitGud::GUID>
	{
		std::size_t operator()(const GitGud::GUID& guid) const 
		{
			return hash<uint64_t>()((uint64_t)guid);
		}
	};
}