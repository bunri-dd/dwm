/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 6;        /* gaps between windows */
static const unsigned int snap      = 2;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Mononoki Nerd Font:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char rofifont[]	    = "Mononoki Nerd Font:size=10";
static const char col_gray1[]       = "#262220"; /* #222222; background colour */
static const char col_gray2[]       = "#262220"; /* #444444; inactive window border colour */
static const char col_gray3[]       = "#f9f6e8"; /* #BBBBBB; font colour */
static const char col_gray4[]       = "#383838"; /* #EEEEEE; current tag and current window colour */
static const char col_cyan[]        = "#f9f0c2"; /* #005577; second bar colour and active window border colour */
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 }, /* innnactive bar */
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  }, /* active bar */
 	[SchemeHid]  = { col_cyan,  col_gray1, col_cyan  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"alacritty", "-t", "spterm", "-o", "window.dimensions.columns=120", "window.dimensions.lines=34", NULL }; // terminal scratchpad
const char *spcmd2[] = {"alacritty", "-t", "spfm", "-o", "window.dimensions.columns=144", "window.dimensions.lines=41", "-e", "ranger", NULL }; // ranger scratchpad
const char *spcmd3[] = {"keepassxc", NULL }; // keepassxc scratchpad
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
	{"keepassxc",   spcmd3},
};


/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
 	/* class         instance  title            tags mask  isfloating  isterminal  noswallow  monitor */
 	/*{ "Gimp",        NULL,     NULL,            1 << 7,    0,          0,           0,        -1 },*/
 	{ "Firefox",     NULL,     NULL,            1 << 2,    0,          0,          -1,        -1 },
 	{ "Alacritty",   NULL,     NULL,            0,         0,          1,           0,        -1 },
	{ "discord",     NULL,     NULL,            1 << 1,    0,          0,           0,        -1 },
	{ "Steam",       NULL,     NULL,            1 << 8,    0,          0,           0,        -1 },
	/*{ "Code - OSS",  NULL,     NULL,            1 << 6,    0,          0,           0,        -1 },*/
 	{ NULL,          NULL,     "Event Tester",  0,         0,          0,           1,        -1 }, /* xev */
 	{ NULL,		       NULL,		 "spterm",		    SPTAG(0),	 1,			     1,           0,        -1 },
 	{ NULL,		       NULL,		 "spfm",		      SPTAG(1),	 1,			     1,           0,        -1 },
 	{ NULL,		       NULL,	   "keepassxc",		  SPTAG(2),	 0,			     0,           0,        -1 },

};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static int attachbelow = 1;    /* 1 means attach after the currently active window */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
 	{ "|||",      col },                    // 0, first entry is default
	{ "[]=",      tile },                   // 1
	{ "< >",      NULL },                   // 2, no layout function means floating behavior
	{ "[M]",      monocle },                // 3
	{ "|M|",      centeredmaster },         // 4
	{ ">M>",      centeredfloatingmaster }, // 5
 	{ "TTT",      bstack },                 // 6
 	{ "===",      bstackhoriz },            // 7
 	{ "[@]",      spiral },                 // 8
 	{ "[\\]",      dwindle },               // 9
};

/* key definitions */
#define MODKEY Mod4Mask // windows key
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define PrintScreenDWM 0x0000ff61 // the printscreen button

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *roficmd[] = { "rofi", "-show", "drun", "-show-icons","-font", rofifont, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *cmdprintscreen[] = { "flameshot", "gui", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ 0,	PrintScreenDWM,	spawn,	{.v = cmdprintscreen } },
	{ MODKEY,                       XK_r,      spawn,          {.v = roficmd } },
	{ MODKEY|ShiftMask,             XK_r,      spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_k,      focusstackvis,  {.i = +1 } },
	{ MODKEY,                       XK_j,      focusstackvis,  {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_k,      focusstackhid,  {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_j,      focusstackhid,  {.i = -1 } },
	{ MODKEY,                       XK_n,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_m,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_o,      setmfact,       {.f = -0.01} },
	{ MODKEY,                       XK_p,      setmfact,       {.f = +0.01} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, //col
 	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[3]} }, //monocle
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[4]} }, //centeredmaster
	{ MODKEY|ShiftMask,             XK_y,      setlayout,      {.v = &layouts[5]} }, //centeredfloatingmaster
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[6]} }, //bstack
	{ MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[7]} }, //bstackhoriz
	{ MODKEY,                       XK_i,      setlayout,      {.v = &layouts[8]} }, //spiral
	{ MODKEY|ShiftMask,             XK_i,      setlayout,      {.v = &layouts[9]} }, //dwindle
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
 	{ MODKEY,                       XK_f,      togglefullscreen, {0} },
 	{ MODKEY|ShiftMask,             XK_f,      togglefakefullscreen, {0} },
	{ MODKEY|ShiftMask,             XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
//{ MODKEY,                       XK_s,      show,           {0} },
 	{ MODKEY,                       XK_s,      showall,        {0} },
 	{ MODKEY,                       XK_h,      hide,           {0} },
	{ MODKEY,            			      XK_bracketleft,  	   togglescratch,  {.ui = 0 } },
	{ MODKEY,            			      XK_bracketright,     togglescratch,  {.ui = 1 } },
	{ MODKEY,            			      XK_x,	     togglescratch,  {.ui = 2 } },
	{ MODKEY,                       XK_semicolon,      shiftview,      { .i = +1 } },
	{ MODKEY,                       XK_l,      shiftview,      { .i = -1 } },
  { MODKEY,			                  XK_F5,		 spawn,		       {.v = (const char*[]){ "mpc", "-h", "0.0.0.0", "-p", "50550", "random", NULL } } },
  { MODKEY,			                  XK_F6,		 spawn,		       {.v = (const char*[]){ "mpc", "-h", "0.0.0.0", "-p", "50550", "single", NULL } } },
  { MODKEY,			                  XK_F7,		 spawn,		       {.v = (const char*[]){ "mpc", "-h", "0.0.0.0", "-p", "50550", "repeat", NULL } } },
  { MODKEY,			                  XK_F8,		 spawn,		       {.v = (const char*[]){ "mpc", "-h", "0.0.0.0", "-p", "50550", "toggle", NULL } } },
  { MODKEY,			                  XK_F11,		 spawn,		       {.v = (const char*[]){ "mpc", "-h", "0.0.0.0", "-p", "50550", "prev", NULL } } },
  { MODKEY,			                  XK_F9,		 spawn,		       {.v = (const char*[]){ "mpc", "-h", "0.0.0.0", "-p", "50550", "volume", "-5", NULL } } },
  { MODKEY,			                  XK_F10,		 spawn,		       {.v = (const char*[]){ "mpc", "-h", "0.0.0.0", "-p", "50550", "volume", "+5", NULL } } },
  { MODKEY,			                  XK_F12,		 spawn,		       {.v = (const char*[]){ "mpc", "-h", "0.0.0.0", "-p", "50550", "next", NULL } } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
 	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
 	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkStatusText,        MODKEY,         Button1,        sigstatusbar,   {.i = 6} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,            0,              Button4,        shiftview,     { .i = -1 } },
	{ ClkTagBar,            0,              Button5,        shiftview,     { .i = +1 } },
};

