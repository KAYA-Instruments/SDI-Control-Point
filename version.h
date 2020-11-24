#define KAYA_VERSION_H_

#define KAYA_MAJOR_VERSION  3
#define KAYA_MINOR_VERSION  1
#define KAYA_PATCH_VERSION  0

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

// Non-zero value indicates a "Beta build"
#ifndef KAYA_BETA
#define KAYA_BETA 0
#endif

// Non-zero value indicates a "Release Candidate build"
#ifndef KAYA_RC
#define KAYA_RC 0
#endif

#define KAYA_BETA_STR(KAYA_BETA) ((0 < KAYA_BETA) ?  " BETA" : "")
#define KAYA_RC_STR(KAYA_RC) ((0 < KAYA_RC) ?  " RC" : "")

#define KAYA_VERSION_STR STR(KAYA_MAJOR_VERSION) "." STR(KAYA_MINOR_VERSION) "." STR(KAYA_PATCH_VERSION)
#define KAYA_VERSION_NUM (KAYA_MAJOR_VERSION*10000 + KAYA_MINOR_VERSION*100 + KAYA_PATCH_VERSION)
