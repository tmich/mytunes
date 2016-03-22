#pragma once
#include "serializer.h"
#include "mhia_serializer.h"
#include <vector>

using namespace std;

namespace mytunes
{
	class MhlaSerializer :
		public Serializer
	{
	public:
		MhlaSerializer(const vector<Album>& albums);
		~MhlaSerializer();

		unsigned char * serialize() override;
		inline size_t children() { return children_no; }
		inline size_t size() { return sz; }
	private:
		vector<MhiaSerializer> mhias;
		const char header_identifier[4]{ 'm','h','l','a' };
		size_t	sz = 0,
			header_length = 0x5C,
			children_no = 0;
	};
}