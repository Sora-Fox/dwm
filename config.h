/* See LICENSE file for copyright and license details. */
// clang-format off
#include <X11/XF86keysym.h>
#define SHRINK_COLORS

#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }

static const unsigned int borderpx       = 4;
static const unsigned int snap           = 32;
static const unsigned int gappih         = 10;
static const unsigned int gappiv         = 10;
static const unsigned int gappoh         = 10;
static const unsigned int gappov         = 10;
static const int          smartgaps_fact = 0;
static const int          showbar        = 1;
static const int          topbar         = 1;
static const int          statusmon      = 'A';

static int         tagindicatortype   = INDICATOR_TOP_LEFT_SQUARE;
static int         tiledindicatortype = INDICATOR_NONE;
static int         floatindicatortype = INDICATOR_TOP_LEFT_SQUARE;
static const char* fonts[]            = { "Source Code Pro:size=16" };
static const char  dmenufont[]        = "Source Code Pro:size=16";

#if !defined(SHRINK_COLORS)

#define orange "#d65d0e";
#define beige "#fbf1c7";
#define dark_beige "#ebdbb2";
#define light_gray "#424242";
#define gray "#3c3836";
#define dark_gray "#282828";

static char c000000[]         = "#000000";

static char normfgcolor[]     = dark_beige;
static char normbgcolor[]     = dark_gray;
static char normbordercolor[] = gray;
static char normfloatcolor[]  = gray;

static char selfgcolor[]     = beige;
static char selbgcolor[]     = orange;
static char selbordercolor[] = orange;
static char selfloatcolor[]  = orange;

static char titlenormfgcolor[]     = dark_beige;
static char titlenormbgcolor[]     = dark_gray;
static char titlenormbordercolor[] = gray;
static char titlenormfloatcolor[]  = gray;

static char titleselfgcolor[]     = beige;
static char titleselbgcolor[]     = orange;
static char titleselbordercolor[] = orange;
static char titleselfloatcolor[]  = orange;

static char tagsnormfgcolor[]     = dark_beige;
static char tagsnormbgcolor[]     = dark_gray;
static char tagsnormbordercolor[] = gray;
static char tagsnormfloatcolor[]  = gray;

static char tagsselfgcolor[]     = beige;
static char tagsselbgcolor[]     = orange;
static char tagsselbordercolor[] = orange;
static char tagsselfloatcolor[]  = orange;

static char hidnormfgcolor[] = dark_beige;
static char hidselfgcolor[]  = beige;
static char hidnormbgcolor[] = light_gray;
static char hidselbgcolor[]  = orange;

static char urgfgcolor[]     = dark_gray;
static char urgbgcolor[]     = dark_beige;
static char urgbordercolor[] = orange;
static char urgfloatcolor[]  = orange;

static char* colors[][ColCount] = {
  [SchemeNorm]      = { normfgcolor,      normbgcolor,      normbordercolor,      normfloatcolor      },
  [SchemeSel]       = { selfgcolor,       selbgcolor,       selbordercolor,       selfloatcolor       },
  [SchemeTitleNorm] = { titlenormfgcolor, titlenormbgcolor, titlenormbordercolor, titlenormfloatcolor },
  [SchemeTitleSel]  = { titleselfgcolor,  titleselbgcolor,  titleselbordercolor,  titleselfloatcolor  },
  [SchemeTagsNorm]  = { tagsnormfgcolor,  tagsnormbgcolor,  tagsnormbordercolor,  tagsnormfloatcolor  },
  [SchemeTagsSel]   = { tagsselfgcolor,   tagsselbgcolor,   tagsselbordercolor,   tagsselfloatcolor   },
  [SchemeHidNorm]   = { hidnormfgcolor,   hidnormbgcolor,   c000000,              c000000             },
  [SchemeHidSel]    = { hidselfgcolor,    hidselbgcolor,    c000000,              c000000             },
  [SchemeUrg]       = { urgfgcolor,       urgbgcolor,       urgbordercolor,       urgfloatcolor       },
};

#else

static char orange[] = "#d65d0e";
static char beige[] = "#fbf1c7";
static char dark_beige[] = "#ebdbb2";
static char light_gray[] = "#424242";
static char gray[] = "#3c3836";
static char dark_gray[] = "#282828";
static char c000000[]         = "#000000";

static char* colors[][ColCount] = {
  [SchemeNorm]      = { dark_beige, dark_gray, gray, gray },
  [SchemeSel]       = {  beige, orange, orange, orange },
  [SchemeTitleNorm] = {  dark_beige, dark_gray, gray, gray},
  [SchemeTitleSel]  = {  beige, orange, orange, orange},
  [SchemeTagsNorm]  = { dark_beige, dark_gray, gray, gray },
  [SchemeTagsSel]   = {  beige, orange, orange, orange},
  [SchemeHidNorm]   = { dark_beige, light_gray, c000000, c000000 },
  [SchemeHidSel]    = {  beige, orange, c000000, c000000},
  [SchemeUrg]       = {  dark_gray, dark_beige, orange, orange},
};

#endif

static char* tagicons[][NUMTAGS] = {
  [DEFAULT_TAGS]     = { "1", "2", "3", "4", "5", "6", "7" },
  [ALTERNATIVE_TAGS] = { "A", "B", "C", "D", "E", "F", "G" },
};

static const Rule rules[] = {
    RULE(.wintype = WTYPE "DIALOG", .isfloating = 1)
    RULE(.wintype = WTYPE "UTILITY",.isfloating = 1)
    RULE(.wintype = WTYPE "TOOLBAR", .isfloating = 1)
    RULE(.wintype = WTYPE "SPLASH", .isfloating = 1)
    RULE(.class = "firefox", .tags = 1 << 1)
    RULE(.class = "TelegramDesktop", .tags = 1 << 3)
};

static const BarRule barrules[] = {
  { -1, 0, BAR_ALIGN_LEFT, width_tags, draw_tags, click_tags, hover_tags, "tags" },
  { -1, 0, BAR_ALIGN_LEFT, width_ltsymbol, draw_ltsymbol, click_ltsymbol, NULL, "layout" },
  { statusmon, 0, BAR_ALIGN_RIGHT, width_status, draw_status, click_status, NULL, "status" },
  { -1, 0, BAR_ALIGN_NONE, width_awesomebar, draw_awesomebar, click_awesomebar, NULL, "awesomebar" },
};

static const float mfact          = 0.55;
static const int   nmaster        = 1;
static const int   nstack         = 0;
static const int   resizehints    = 0;
static const int   lockfullscreen = 1;

static const Layout layouts[] = {
  { "[]=", flextile, { -1, -1, SPLIT_VERTICAL, TOP_TO_BOTTOM, TOP_TO_BOTTOM, 0, NULL } },
  { "><>", NULL,     { 0 } },
  { ":::", flextile, { -1, -1, NO_SPLIT, GAPPLESSGRID, GAPPLESSGRID, 0, NULL } },
  { "[M]", flextile, { -1, -1, NO_SPLIT, MONOCLE, MONOCLE, 0, NULL } },
  { "[D]", flextile, { -1, -1, SPLIT_VERTICAL, TOP_TO_BOTTOM, MONOCLE, 0, NULL } },
  { "TTT", flextile, { -1, -1, SPLIT_HORIZONTAL, LEFT_TO_RIGHT, LEFT_TO_RIGHT, 0, NULL } },
  { "|M|", flextile, { -1, -1, SPLIT_CENTERED_VERTICAL, LEFT_TO_RIGHT, TOP_TO_BOTTOM, TOP_TO_BOTTOM, NULL } },
  { "-M-", flextile, { -1, -1, SPLIT_CENTERED_HORIZONTAL, TOP_TO_BOTTOM, LEFT_TO_RIGHT, LEFT_TO_RIGHT, NULL } },
  { "(@)", flextile, { -1, -1, NO_SPLIT, SPIRAL, SPIRAL, 0, NULL } },
};

static char dmenumon[2] = "0";
static const char *dmenucmd[] = {
  "dmenu_run",
  "-m",  dmenumon,
  "-fn", dmenufont,
#if !defined (SHRINK_COLORS)
  "-nb", normbgcolor,
  "-nf", normfgcolor,
  "-sb", selbgcolor,
  "-sf", selfgcolor,
#else
  "-nb", dark_gray,
  "-nf", dark_beige,
  "-sb", orange,
  "-sf", beige,
#endif
  NULL,
};

static const char terminal[]       = "alacritty";
static const char clipboard[]      = "clipboard";
static const char screen_area[]    = "flameshot gui -c";
static const char screen_screen[]  = "flameshot screen -c";
static const char inc_volume[]     = "pactl set-sink-volume @DEFAULT_SINK@ +5%";
static const char dec_volume[]     = "pactl set-sink-volume @DEFAULT_SINK@ -5%";
static const char mute_volume[]    = "pactl set-sink-mute @DEFAULT_SINK@ toggle";
static const char inc_brightness[] = "brightnessctl set +5%";
static const char dec_brightness[] = "\
current_brightness=$(brightnessctl g | awk -v max=$(brightnessctl max) '{printf \"%.0f\\n\", ($1/max)*100}'); \
if [ $? -ne 0 ] || ! [[ \"$current_brightness\" =~ ^[0-9]+$ ]] || [ \"$current_brightness\" -le 5 ]; then \
    brightnessctl set 5%; \
else \
    brightnessctl set 5%-; \
fi";

static const Key keys[] = {
  { MODKEY,             XK_r,                     spawn,          { .v = dmenucmd }         },
  { MODKEY,             XK_Return,                spawn,          SHCMD(terminal)           },
  { MODKEY,             XK_v,                     spawn,          SHCMD(clipboard)          },
  { MODKEY,             XK_f,                     spawn,          SHCMD("firefox")          },
  { MODKEY,             XK_t,                     spawn,          SHCMD("telegram-desktop") },
  { MODKEY,             XK_p,                     spawn,          SHCMD("pavucontrol")      },

  { MODKEY | ShiftMask, XK_b,                     togglebar,      { 0 }                     },
  { MODKEY | ShiftMask, XK_c,                     killclient,     { 0 }                     },
  { MODKEY | ShiftMask, XK_q,                     quit,           { 0 }                     },
  { MODKEY,             XK_0,                     view,           { .ui = ~0 }              },

  { MODKEY,             XK_j,                     focusstack,     { .i = +1 }               },
  { MODKEY,             XK_k,                     focusstack,     { .i = -1 }               },
  { MODKEY,             XK_h,                     setmfact,       { .f = -0.05 }            },
  { MODKEY,             XK_l,                     setmfact,       { .f = +0.05 }            },
  { MODKEY | ShiftMask, XK_f,                     togglefloating, {0}                       },
  { MODKEY,             XK_space,                 cyclelayout,    { .i = +1 }               },
  { MODKEY | ShiftMask, XK_space,                 cyclelayout,    { .i = -1 }               },

  { 0,                  XF86XK_AudioLowerVolume,  spawn,          SHCMD(dec_volume)         },
  { 0,                  XF86XK_AudioRaiseVolume,  spawn,          SHCMD(inc_volume)         },
  { 0,                  XF86XK_AudioMute,         spawn,          SHCMD(mute_volume)        },
  { 0,                  XF86XK_MonBrightnessDown, spawn,          SHCMD(dec_brightness)     },
  { 0,                  XF86XK_MonBrightnessUp,   spawn,          SHCMD(inc_brightness)     },
  { 0,                  XK_Print,                 spawn,          SHCMD(screen_area)        },
  { 0 | ShiftMask,      XK_Print,                 spawn,          SHCMD(screen_screen)      },

  TAGKEYS(XK_1, 0),
  TAGKEYS(XK_2, 1),
  TAGKEYS(XK_3, 2),
  TAGKEYS(XK_4, 3),
  TAGKEYS(XK_5, 4),
  TAGKEYS(XK_6, 5),
  TAGKEYS(XK_7, 6),

//{ MODKEY | ShiftMask, XK_0,                     tag,            { .ui = ~0 }              },
//{ MODKEY,             XK_Tab,                   view,           { 0 }                     },
//{ MODKEY,             XK_i,                     incnmaster,     {.i = +1}                 },
//{ MODKEY,             XK_d,                     incnmaster,     {.i = -1}                 },
//{ MODKEY,             XK_Return,                zoom,           {0}                       },
//{ MODKEY | ShiftMask, XK_t,                     setlayout,      { .v = &layouts[0] }      },
//{ MODKEY | ShiftMask, XK_f,                     setlayout,      { .v = &layouts[1] }      },
//{ MODKEY | ShiftMask, XK_m,                     setlayout,      { .v = &layouts[2] }      },
//{ MODKEY,             XK_comma,                 focusmon,       { .i = -1 }               },
//{ MODKEY,             XK_period,                focusmon,       { .i = +1 }               },
//{ MODKEY | ShiftMask, XK_comma,                 tagmon,         { .i = -1 }               },
//{ MODKEY | ShiftMask, XK_period,                tagmon,         { .i = +1 }               },
};

static const Button buttons[] = {
  { ClkLtSymbol, 0, Button1, setlayout, { 0 } },
  { ClkLtSymbol, 0, Button3, setlayout, { .v = &layouts[2] } },
  { ClkWinTitle, 0, Button1, togglewin, { 0 } },
  { ClkWinTitle, 0, Button3, showhideclient, { 0 } },
  { ClkWinTitle, 0, Button2, zoom, { 0 } },
  { ClkStatusText, 0, Button2, spawn, SHCMD(terminal) },
  { ClkClientWin, MODKEY, Button1, movemouse, { 0 } },
  { ClkClientWin, MODKEY, Button2, togglefloating, { 0 } },
  { ClkClientWin, MODKEY, Button3, resizemouse, { 0 } },
  { ClkTagBar, 0, Button1, view, { 0 } },
  { ClkTagBar, 0, Button3, toggleview, { 0 } },
  { ClkTagBar, MODKEY, Button1, tag, { 0 } },
  { ClkTagBar, MODKEY, Button3, toggletag, { 0 } },
};

