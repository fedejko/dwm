/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 32;
static const int focusonwheel       = 0;
static const char *fonts[]          = { "IBMPlexMono-Regular:matrix:pixelsize=14:antialias=true:autohint=true" };
static const char dmenufont[]       =   "IBMPlexMono-Regular:matrix:pixelsize=14:antialias=true:autohint=true";
static const char col_gray1[]       = "#000000";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#808080";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#cccccc";
static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
    [SchemeSel]  = { col_gray4, col_gray1, col_cyan  },
};
static const XPoint stickyicon[]    = { {0,0}, {4,0}, {4,8}, {2,6}, {0,8}, {0,0} }; /* represents the icon as an array of vertices */
static const XPoint stickyiconbb    = {4,8};    /* defines the bottom right corner of the polygon's bounding box (speeds up scaling) */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class      instance title   tags mask switchtotag iscent isfloat monitor float x,y,w,h,px */
    { "xterm-256color", NULL, "st", 0,       0,          1,     1,      -1,     0,0,800,500,1 },
    { NULL, NULL, "Event Tester",  0,        0,          1,     1,      -1,     0,0,100,100,1 },
    { "Chromium", NULL,    NULL,   1 << 1,   1,          0,     0,      -1,     0,0,0,0,1 },
    { "Firefox",  NULL,    NULL,   1 << 2,   1,          0,     0,      -1,     0,0,0,0,1 },
    { NULL,       NULL,    "info", 0,        0,          0,     1,      -1,     1320,34,600,206,1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "centeredmaster.c"
#include "columns.c"
#include "tcl.c"
static const Layout layouts[] = {
    /* symbol   arrange function */
    { "[]=",    tile },    /* first entry is default */
    { "||=",    col },
    { "|||",    tcl },
    { "|-|",    centeredmaster },
    { "|F|",    centeredfloatingmaster },
    { "[M]",    monocle },
    { "><>",    NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,               KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,   KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|Mod1Mask,      KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,     KEY,      toggletag,      {.ui = 1 << TAG} },
#define HOLDKEY 0xffeb /* replace 0 with the keysym to activate holdbar
                        * 0xffe9, Alt_L
                        * 0xfe03, ISO_Level3_Shift (Alt_R)
                        * 0xffeb, Windows key
                        * 0xffe3, Control_L
                        * 0xffe4, Control_R
                        */
/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "xterm", NULL };
static const char *nmuttcmd[]  = { "xterm", "-e", "mutt", NULL };
static const char *roficmd[]  = { "rofi", "-show", "run", NULL };
static const char *rsscmd[]  = { "xterm", "-e", "rss", NULL };

#include "movestack.c"

static Key keys[] = {
    /* modifier         key        function        argument */
    { MODKEY,           XK_Return,       spawn,          {.v = termcmd } },
    { MODKEY,           XK_space,        spawn,          {.v = roficmd } },
    { MODKEY,           XK_l,            spawn,          SHCMD("slock") },    
    { MODKEY|Mod1Mask,  XK_b,            spawn,          SHCMD("slstoggle") },
    { MODKEY|Mod1Mask,  XK_m,            spawn,          {.v = nmuttcmd } },
    { MODKEY|Mod1Mask,  XK_n,            spawn,          {.v = rsscmd } },
    { MODKEY|Mod1Mask,  XK_o,            spawn,          SHCMD("check_mail") },
    { MODKEY|Mod1Mask,  XK_w,            spawn,          SHCMD("rofi_wttr -e \"$(weather)\"") },
    { MODKEY|ShiftMask, XK_Return,       spawn,          SHCMD("cd ~/Downloads && chrome") },
    { ControlMask,      XK_Print,        spawn,          SHCMD("scrot -s -e \'mv $f ~/screenshots/\'") },
    { MODKEY,           XK_b,            togglebar,      {0} },
    { MODKEY,           XK_Tab,          focusstack,     {.i = +1 } },
    { MODKEY|ShiftMask, XK_Tab,          focusstack,     {.i = -1 } },
    { MODKEY,           XK_Down,         movestack,      {.i = +1 } },
    { MODKEY,           XK_Up,           movestack,      {.i = -1 } },
    { MODKEY,           XK_z,            zoom,           {0} },
    { MODKEY,           XK_grave,        view,           {0} },
    { MODKEY,           XK_e,            killclient,     {0} },
    { MODKEY,           XK_s,            togglesticky,   {0} },
    { MODKEY,           XK_f,            togglefloating, {0} },
    { MODKEY,           XK_comma,        focusmon,       {.i = -1 } },
    { MODKEY,           XK_period,       focusmon,       {.i = +1 } },
    { Mod5Mask,         XK_equal,        incnmaster,     {.i = +1 } },
    { Mod5Mask,         XK_minus,        incnmaster,     {.i = -1 } },
    { Mod5Mask,         XK_bracketleft,  setmfact,       {.f = -0.02} },
    { Mod5Mask,         XK_bracketright, setmfact,       {.f = +0.02} },
    { Mod5Mask,         XK_1,            setlayout,      {.v = &layouts[0]} },
    { Mod5Mask,         XK_2,            setlayout,      {.v = &layouts[1]} },
    { Mod5Mask,         XK_3,            setlayout,      {.v = &layouts[2]} },
    { Mod5Mask,         XK_4,            setlayout,      {.v = &layouts[3]} },
    { Mod5Mask,         XK_5,            setlayout,      {.v = &layouts[4]} },    
    { Mod5Mask,         XK_Return,       setlayout,      {.v = &layouts[5]} },//monocle
    { Mod5Mask,         XK_0,            setlayout,      {.v = &layouts[6]} },//floating
    { Mod5Mask,         XK_backslash,    setlayout,      {0} },               //last
    { MODKEY,           XK_0,            view,           {.ui = ~0 } },
    { MODKEY|Mod1Mask,  XK_0,            tag,            {.ui = ~0 } },
    { MODKEY|Mod1Mask,  XK_comma,        tagmon,         {.i = -1 } },
    { MODKEY|Mod1Mask,  XK_period,       tagmon,         {.i = +1 } },
    TAGKEYS(            XK_1,                            0)
    TAGKEYS(            XK_2,                            1)
    TAGKEYS(            XK_3,                            2)
    TAGKEYS(            XK_4,                            3)
    TAGKEYS(            XK_5,                            4)
    TAGKEYS(            XK_6,                            5)
    TAGKEYS(            XK_7,                            6)
    TAGKEYS(            XK_8,                            7)
    TAGKEYS(            XK_9,                            8)
    { MODKEY|ShiftMask, XK_q,            quit,           {0} },
    { MODKEY|ShiftMask, XK_r,            quit,           {1} }, 
    { 0,                HOLDKEY,         holdbar,        {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,   0,                 Button1,    setlayout,      {0} },
    { ClkLtSymbol,   0,                 Button3,    setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,   0,                 Button2,    spawn,          {.v = termcmd } },
    { ClkRootWin,    0,                 Button2,    spawn,          {.v = termcmd } },
    { ClkRootWin,    0,                 Button3,    spawn,          {.v = roficmd } },
    { ClkStatusText, 0,                 Button2,    spawn,          SHCMD("slstoggle") },
    { ClkClientWin,  MODKEY,            Button1,    movemouse,      {0} },
    { ClkClientWin,  Mod1Mask,          Button1,    resizemouse,    {0} },
    { ClkClientWin,  MODKEY,            Button2,    togglefloating, {0} },
    { ClkClientWin,  Mod1Mask,          Button2,    togglesticky,   {0} },
    { ClkClientWin,  Mod5Mask,          Button2,    togglesticky,   {0} },
    { ClkTagBar,     0,                 Button1,    view,           {0} },
    { ClkTagBar,     0,                 Button3,    toggleview,     {0} },
    { ClkTagBar,     MODKEY,            Button1,    tag,            {0} },
    { ClkTagBar,     MODKEY,            Button3,    toggletag,      {0} },
};

