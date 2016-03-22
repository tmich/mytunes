#pragma once

using namespace std;

namespace mytunes
{
	class Serializer
	{
	public:
		virtual ~Serializer() {};
		virtual unsigned char * serialize() = 0;
	};
}
