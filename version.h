#define KAYA_VERSION_H_

#define KAYA_COMMERCIAL_VERSION "2022.1"

#define KAYA_MAJOR_VERSION  5
#define KAYA_MINOR_VERSION  0
#define KAYA_PATCH_VERSION  0

#define KY_STR_HELPER(x) #x
#define KY_STR_VALUE(x) KY_STR_HELPER(x)

// Non-zero value indicates a "Beta build"
#ifndef KAYA_BETA
#define KAYA_BETA 0
#endif

// Non-zero value indicates a "Release Candidate build"
#ifndef KAYA_RC
#define KAYA_RC 0
#endif

#define KY_VP_MARKET_VERSION KY_STR_VALUE(KAYA_MAJOR_VERSION) "." KY_STR_VALUE(KAYA_MINOR_VERSION)
#define KAYA_VERSION_NUM (KAYA_MAJOR_VERSION*10000 + KAYA_MINOR_VERSION*100 + KAYA_PATCH_VERSION)

#if (KAYA_PATCH_VERSION)
#define KAYA_VERSION_STR KY_VP_MARKET_VERSION " SP " KY_STR_VALUE(KAYA_PATCH_VERSION)
#elif (KAYA_RC)
#define KAYA_VERSION_STR KY_VP_MARKET_VERSION " RC " KY_STR_VALUE(KAYA_RC)
#elif (KAYA_BETA)
#define KAYA_VERSION_STR KY_VP_MARKET_VERSION " Beta " KY_STR_VALUE(KAYA_BETA)
#else
#define KAYA_VERSION_STR KY_VP_MARKET_VERSION
#endif
