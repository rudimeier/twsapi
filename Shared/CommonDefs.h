#ifndef common_defs_h_INCLUDED
#define common_defs_h_INCLUDED

namespace IB {

typedef long TickerId;
typedef long OrderId;

enum faDataType { GROUPS=1, PROFILES, ALIASES } ;

inline const char* faDataTypeStr ( faDataType pFaDataType )
{
	switch (pFaDataType) {
		case GROUPS:
			return "GROUPS" ;
			break ;
		case PROFILES:
			return "PROFILES" ;
			break ;
		case ALIASES:
			return "ALIASES" ;
			break ;
	}
	return 0 ;
}

enum MarketDataType { 
	REALTIME = 1, 
	FROZEN = 2
};

} // namespace IB
#endif /* common_defs_h_INCLUDED */
