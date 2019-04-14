/* EGI player functions
 *
 * You need the EGI DLLs and the DLL version of the Small AMX to use these
 * routines.
 *
 * (c) Copyright 1998-2007, ITB CompuPhase
 * This file is provided as is (no warranties).
 */

enum FlicParam
    {
    FLIC_PARAM_ISPLAYING,  /* 1=FLIC file is currently playing */
    FLIC_PARAM_WIDTH,      /* size of the frame */
    FLIC_PARAM_HEIGHT,
    FLIC_PARAM_NUMFRAMES,  /* total number of frames */
    FLIC_PARAM_CURFRAME,   /* current frame number */
    FLIC_PARAM_CONTINUOUS, /* segment is continuous (it has a ring frame) */
    FLIC_PARAM_SPEED,      /* playback speed, set to 0 for max. speed */
    FLIC_PARAM_DESIGNSPEED,/* design speed, read from FLIC file */
    FLIC_PARAM_REALSPEED,  /* real playback speed */
    FLIC_PARAM_MODE,       /* BitBlt mode */
    FLIC_PARAM_LOOP,       /* loop frames or only play once */
    FLIC_PARAM_NUMSEGMENTS,
    FLIC_PARAM_CURSEGMENT,
    FLIC_PARAM_XPOS,       /* position of the animation in the window */
    FLIC_PARAM_YPOS,
    FLIC_PARAM_LABEL,      /* label of the current frame (-1 if none) */
    FLIC_PARAM_CHGPAL,     /* use current palette, don't create a new one */
    FLIC_PARAM_VERSION,    /* current version of EGI player */
    FLIC_PARAM_SEGLABEL,   /* the label of the current segment */
    FLIC_PARAM_DRAWFRAME,  /* draw the frames? */
    FLIC_PARAM_FRNOTIFY,   /* notify for each frame? */
    FLIC_PARAM_MASK,       /* masks present (0=no, 1=bitmap, 2=multilevel) */
    FLIC_PARAM_ID,         /* user specified id */
    FLIC_PARAM_MSGNUM,     /* value of FLIC_NOTIFY */
    FLIC_PARAM_KEYFREQ,    /* key frame interval (0 if there are no key frames) */
    FLIC_PARAM_SKIPDELTAS, /* skip all delta frames up to the next key frame */
    FLIC_PARAM_PALMODE,    /* palette mode */
    FLIC_PARAM_IDENTPAL,   /* 1:1 mapping DIB colour table to logical palette? */
    FLIC_PARAM_SENDNOTIFY, /* send notification messages? */
    FLIC_PARAM_COLORDEPTH, /* 8=256 colours, 15, 16 or 24 = RGB */
    FLIC_PARAM_NUMREGRECTS,/* number or recangles that form the region */
    FLIC_PARAM_LASTFRAME,  /* last frame to play */
    FLIC_PARAM_PLAYWAVE,   /* play audio chunks y/n */
    FLIC_PARAM_SYNCHRONIZE,/* speed is more important than quality */
    FLIC_PARAM_NEXTSEGMENT,/* segment that (by design) follows this one */
    FLIC_PARAM_REPEATS,    /* number of repeats for the segment */
    FLIC_PARAM_DESIGNWIDTH,/* design size, read from FLIC file */
    FLIC_PARAM_DESIGNHEIGHT,
    FLIC_PARAM_PRELOAD,    /* preload the next frame? */
    FLIC_PARAM_FRAMEBITS,  /* data chunks in last frame */
    FLIC_PARAM_XORG,       /* position of the origin in the frame's rectangle */
    FLIC_PARAM_YORG,
    FLIC_PARAM_TOTALFRAMES,/* total number of frames (over all segments) */
    FLIC_PARAM_SCALEOPTION,/* method of scaling (point-sampling, smooth Bresenham) */
    FLIC_PARAM_TRANSPCOLOR,/* transparent colour of the animation file (or -1) */
    }
native FlicGetParam(FlicParam:index);
native FlicSetParam(FlicParam:index,param);

enum
    {
    FLIC_ERR_NONE,
    FLIC_ERR_FOPEN,
    FLIC_ERR_FORMAT,
    FLIC_ERR_FREAD,
    FLIC_ERR_MEMORY,
    FLIC_ERR_PARAM,
    FLIC_ERR_INVALID,
    FLIC_ERR_MUTEX,
    FLIC_ERR_UNSUPPORT,
    FLIC_ERR_LOCKED,
    FLIC_ERR_WAVEOPEN,
    FLIC_ERR_FLICHANDLE,
    FLIC_ERR_GENERIC,
    }
native FlicError:FlicError();

native FlicPlay(hwnd=0);
native FlicStop();
native FlicClose();

native FlicVersion();
native FlicHandle();  /* returns the "LPFLIC" handle for this AMX */

native FlicMoveWindow(x=0, y=0, bool:delta=false);
