#pragma once
#include <cstdint>
#include <cstdio>

// Vorbis structures (opaque for proxy)
struct ogg_sync_state;
struct ogg_stream_state;
struct vorbis_info;
struct vorbis_comment;
struct vorbis_dsp_state;
struct vorbis_block;

typedef struct {
    size_t (*read_func)(void* ptr, size_t size, size_t nmemb, void* datasource);
    int    (*seek_func)(void* datasource, int64_t offset, int whence);
    int    (*close_func)(void* datasource);
    long   (*tell_func)(void* datasource);
} ov_callbacks;

typedef struct OggVorbis_File {
    void*            datasource;
    int              seekable;
    int64_t          offset;
    int64_t          end;
    ogg_sync_state*  oy;
    int              links;
    int64_t*         offsets;
    int64_t*         dataoffsets;
    long*            serialnos;
    int64_t*         pcmlengths;
    vorbis_info*     vi;
    vorbis_comment*  vc;
    int64_t          pcm_offset;
    int              ready_state;
    long             current_serialno;
    int              current_link;
    double           bittrack;
    double           samptrack;
    ogg_stream_state* os;
    vorbis_dsp_state* vd;
    vorbis_block*    vb;
    ov_callbacks     callbacks;
} OggVorbis_File;
