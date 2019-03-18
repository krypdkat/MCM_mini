#ifndef DKATNETGETWORK_H
#define DKATNETGETWORK_H
#if defined(__APPLE__) || defined(__LINUX__)
#define DKNGetWork_API __attribute__((visibility("default")))
#else
#  define DKNGetWork_API __declspec(dllexport)
#endif
struct work
{
    char* work;
    char* res; //should be full json
    unsigned char* decoded_data;
    int diff;
    int work_len;
    unsigned long long nblock;
    float hr;
    unsigned char h[32];
};
#ifdef  __cplusplus
class DKNGetWork
{
public:
    class Impl;
    DKNGetWork(const char* usr, const char* pwd, const char* pool);
    ~DKNGetWork();
    void GetWork(work* w);
    void SubmitWork(work* w);
    bool CheckAlive();
    bool IsReady();
    bool HaveNewWork();
    void UpdateHR(work* w);
    Impl* mpImpl;
};
DKNGetWork_API DKNGetWork* DKNGetWorkFactory(const char* usr, const char* pwd, const char* pool);
#endif
#ifdef  __cplusplus
extern "C"
{
#endif
typedef void* C_DKNGetWork;
DKNGetWork_API C_DKNGetWork C_DKNGetWorkFactory(const char* usr, const char* pwd, const char* pool);
DKNGetWork_API void C_GetWork    (C_DKNGetWork ptr, void* w);
DKNGetWork_API void C_SubmitWork (C_DKNGetWork ptr, void* w);
DKNGetWork_API void C_Decode64   (void* w);
DKNGetWork_API int C_CheckAlive (C_DKNGetWork ptr);
DKNGetWork_API int C_IsReady    (C_DKNGetWork ptr);
DKNGetWork_API int C_HaveNewWork(C_DKNGetWork ptr);
DKNGetWork_API void C_UpdateHR(C_DKNGetWork ptr, void* w);
#ifdef  __cplusplus
}
#endif
#endif


