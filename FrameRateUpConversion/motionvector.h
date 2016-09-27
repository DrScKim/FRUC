
#ifndef	__MV_VECTOR__
#define	__MV_VECTOR__



#pragma pack (push, 16)
struct VECTOR
{
	union
	{
		struct
		{
			int x;
			int y;
		};
		int coord[2];
	};
	int sad;
};

#pragma pack (pop)


#endif	// __MV_VECTOR__

