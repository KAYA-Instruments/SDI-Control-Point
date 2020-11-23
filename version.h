#define KAYA_VERSION_H_

#define KAYA_MAJOR_VERSION  3
#define KAYA_MINOR_VERSION  1
#define KAYA_PATCH_VERSION  0

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define KAYA_VERSION_STR STR(KAYA_MAJOR_VERSION) "." STR(KAYA_MINOR_VERSION) "." STR(KAYA_PATCH_VERSION)
#define KAYA_VERSION_NOM (KAYA_MAJOR_VERSION*10000 + KAYA_MINOR_VERSION*100 + KAYA_PATCH_VERSION)


// Non-zero value indicates a "Beta build"
#ifndef KAYA_BETA
#define KAYA_BETA 0
#endif

// Non-zero value indicates a "Release Candidate build"
#ifndef KAYA_RC
#define KAYA_RC 0
#endif


