/* See LICENSE file for copyright and license details. */
#include "push.c"
#include "gaplessgrid.c"

/* appearance */
static const char font[]            = "-*-proggysmall-*-*-*-*-*-*-*-*-*-*-*-*";
static const char normbordercolor[] = "#000000";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#aaaaaa";
static const char selbordercolor[]  = "#535d6c";
static const char selbgcolor[]      = "#535d6c";
static const char selfgcolor[]      = "#ffffff";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 8;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

#define FILEMAN "/usr/bin/rox"
#define TERM "svte"
//#define TERM "/usr/bin/urxvt"

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",      NULL,       NULL,       0,            True,        -1 },
	{ "Firefox",   NULL,       NULL,       1 << 8,       False,       -1 },
	{ "Uzbl-core", NULL,       NULL,       0,            True,        -1 },
	{ "Gvim",      NULL,       NULL,       0,            True,        -1 },
};

/* Tag walking */
static void
prevtag(const Arg *arg) {
	uint i;
	Arg a;

	for(i = 0; i < LENGTH(tags) && !(selmon->tagset[selmon->seltags] & (1 << i)); i++);
	a.ui = 1 << (i == 0 ? LENGTH(tags) - 1 : i - 1);
	view(&a);
}

static void
nexttag(const Arg *arg) {
	uint i;
	Arg a;

	for(i = 0; i < LENGTH(tags) && !(selmon->tagset[selmon->seltags] & (1 << i)); i++);
	a.ui = 1 << (i == LENGTH(tags) - 1 ? 0 : i + 1);
	view(&a);
}

/* layout(s) */
static const float mfact      = 0.65; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = False; /* True means respect size hints in tiled resizals */

/* Layout enum(s) */
enum {
	Monocle,
	Tile,
	Float,
	Gapless
};

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[M]",      monocle },
	{ "[]=",      tile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "###",      gaplessgrid },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define TERMCMD(cmd) { .v = (const char*[]){ TERM, "-e", "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] = { "dmenu_run", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { TERM, NULL };
//static const char *termcmd[]  = { TERM, "-ls", NULL };
static const char *fmcmd[]  = { FILEMAN, NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          TERMCMD("tmux") },
	{ MODKEY|ControlMask,           XK_Return, spawn,          TERMCMD("tmux attach") },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_j,      pushdown,       {0} },
	{ MODKEY|ControlMask,           XK_k,      pushup,         {0} },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,  togglefloating, {0} },
	{ ALTKEY,                       XK_Tab,    zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[Tile]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[Float]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[Monocle]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[Gapless]} },
	{ MODKEY,                       XK_Escape, view,           {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_Left,   prevtag,        {0} },
	{ MODKEY,                       XK_Right,  nexttag,        {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY,                       XK_e,      spawn,          { .v = fmcmd } },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          TERMCMD("sleep 0.01; ncmpcpp") },
	{ MODKEY|ShiftMask,             XK_u,      spawn,          TERMCMD("mutt -y") },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          TERMCMD("earthquake") },
	{ MODKEY|ControlMask,           XK_b,      spawn,          SHCMD("uzbl-browser") },
	{ MODKEY|ControlMask,           XK_c,      spawn,          SHCMD("mpc prev") },
	{ MODKEY|ControlMask,           XK_v,      spawn,          SHCMD("mpc next") },
	{ MODKEY|ControlMask,           XK_x,      spawn,          SHCMD("mpc stop") },
	{ MODKEY|ControlMask,           XK_z,      spawn,          SHCMD("mpc toggle") },
	{ MODKEY|ShiftMask,             XK_v,      spawn,          SHCMD("gvim") },
	{ MODKEY|ShiftMask,             XK_l,      spawn,          TERMCMD("lock-blank.sh") },
/*	{ MODKEY|ShiftMask,             XK_space,  spawn,          SHCMD("bmark=$(cat ${HOME}/.config/surfraw/bookmarks | cut -d\\  -f1 | dmenu) && urxvt -e sh -c \"surfraw ${bmark}\"") },*/
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
};

