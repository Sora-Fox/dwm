/* See LICENSE file for copyright and license details. */
/* clang-format off */
#include <X11/XF86keysym.h>

#define MODKEY Mod4Mask
#define ACCENT_COLOR "#5a4fcf"
/* #5a4fcf - violet-blue
 * #d65d0e - orange */
/* Uncomment the line below to enable monochrome theme */
#define MONOCHROME
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }

static const unsigned int borderpx       = 1;
static const unsigned int snap           = 32;
static const unsigned int gappih         = 0;
static const unsigned int gappiv         = 0;
static const unsigned int gappoh         = 0;
static const unsigned int gappov         = 0;
static const          int smartgaps_fact = 0;
static const          int showbar        = 1;
static const          int topbar         = 1;
static const          int statusmon      = 'A';

static       int   tagindicatortype   = INDICATOR_TOP_LEFT_SQUARE;
static       int   tiledindicatortype = INDICATOR_NONE;
static       int   floatindicatortype = INDICATOR_TOP_LEFT_SQUARE;
static const char* fonts[]            = { "Source Code Pro:size=15" };
static const char  dmenufont[]        = "Source Code Pro:size=15";

#ifndef MONOCHROME
  static char accent[]          = ACCENT_COLOR;
  static char text_active[]     = "#fbf1c7";
  static char text_normal[]     = "#ebdbb2";
#else
  static char accent[]          = "#605d77";
  static char text_active[]     = "#f1efe7";
  static char text_normal[]     = "#dedbd3";
#endif

static char background_base[] = "#282828";
static char background_dim[]  = "#424242";
static char border_base[]     = "#3a3938";
static char no_color[]        = "#000000";

static char* colors[][ColCount] = {
  [SchemeNorm]      = { text_normal, background_base, border_base, border_base },
  [SchemeSel]       = { text_active, accent,          accent,      accent      },
  [SchemeTitleNorm] = { text_normal, background_base, border_base, border_base },
  [SchemeTitleSel]  = { text_active, accent,          accent,      accent      },
  [SchemeTagsNorm]  = { text_normal, background_base, border_base, border_base },
  [SchemeTagsSel]   = { text_active, accent,          accent,      accent      },
  [SchemeHidNorm]   = { text_normal, background_dim,  no_color,    no_color    },
  [SchemeHidSel]    = { text_active, accent,          no_color,    no_color    },
  [SchemeUrg]       = { accent,      text_active,     accent,      accent      },
};

static char* tagicons[][NUMTAGS] = {
  [DEFAULT_TAGS]     = { "1", "2", "3", "4", "5", "6", "7" },
  [ALTERNATIVE_TAGS] = { "A", "B", "C", "D", "E", "F", "G" },
};

static const Rule rules[] = {
    RULE(.wintype = WTYPE "DIALOG",  .isfloating = 1 )
    RULE(.wintype = WTYPE "UTILITY", .isfloating = 1 )
    RULE(.wintype = WTYPE "TOOLBAR", .isfloating = 1 )
    RULE(.wintype = WTYPE "SPLASH",  .isfloating = 1 )
    RULE(.class = "firefox",         .tags = 1 << 1  )
    RULE(.class = "TelegramDesktop", .tags = 1 << 3  )
};

static const BarRule barrules[] = {
  { -1,        0, BAR_ALIGN_LEFT,  width_tags,       draw_tags,       click_tags,       hover_tags, "tags"       },
  { -1,        0, BAR_ALIGN_LEFT,  width_ltsymbol,   draw_ltsymbol,   click_ltsymbol,   NULL,       "layout"     },
  { statusmon, 0, BAR_ALIGN_RIGHT, width_status,     draw_status,     click_status,     NULL,       "status"     },
  { -1,        0, BAR_ALIGN_NONE,  width_awesomebar, draw_awesomebar, click_awesomebar, NULL,       "awesomebar" },
};

static const float mfact          = 0.5;
static const int   nmaster        = 1;
static const int   nstack         = 0;
static const int   resizehints    = 0;
static const int   lockfullscreen = 1;

static const Layout layouts[] = {
  { "[]=", flextile, { -1, -1, SPLIT_VERTICAL,            TOP_TO_BOTTOM, TOP_TO_BOTTOM, 0,             NULL } },
  { "><>", NULL,     {  0                                                                                   } },
  { ":::", flextile, { -1, -1, NO_SPLIT,                  GAPPLESSGRID,  GAPPLESSGRID,  0,             NULL } },
  { "TTT", flextile, { -1, -1, SPLIT_HORIZONTAL,          LEFT_TO_RIGHT, LEFT_TO_RIGHT, 0,             NULL } },
  { "[M]", flextile, { -1, -1, NO_SPLIT,                  MONOCLE,       MONOCLE,       0,             NULL } },
};

static char dmenumon[2] = "0";
static const char *dmenucmd[] = {
  "dmenu_run",
  "-m",  dmenumon,
  "-fn", dmenufont,
  "-nb", background_base,
  "-nf", text_normal,
  "-sb", accent,
  "-sf", text_active,
  NULL,
};
static const char terminal[]        = "alacritty";
static const char clipboard[]       = "diodon";
static const char screen_area[]     = "flameshot gui -c";
static const char screen_full[]     = "flameshot screen -c";
static const char sink_up[]         = "audiobrightctl.sh sink up";
static const char sink_down[]       = "audiobrightctl.sh sink down";
static const char sink_mute[]       = "audiobrightctl.sh sink mute";
static const char source_mute[]     = "audiobrightctl.sh source mute";
static const char brightness_up[]   = "audiobrightctl.sh brightness up";
static const char brightness_down[] = "audiobrightctl.sh brightness down";

static const Key keys[] = {
  { MODKEY,             XK_r,      spawn,          { .v = dmenucmd }         },
  { MODKEY,             XK_Return, spawn,          SHCMD(terminal)           },
  { MODKEY,             XK_v,      spawn,          SHCMD(clipboard)          },
  { MODKEY,             XK_f,      spawn,          SHCMD("firefox")          },
  { MODKEY,             XK_t,      spawn,          SHCMD("Telegram")         },
  { MODKEY,             XK_p,      spawn,          SHCMD("pavucontrol-qt")   },
  { MODKEY,             XK_e,      spawn,          SHCMD("emacs")            },

  { MODKEY | ShiftMask, XK_b,      togglebar,      { 0 }                     },
  { MODKEY | ShiftMask, XK_c,      killclient,     { 0 }                     },
  { MODKEY | ShiftMask, XK_q,      quit,           { 0 }                     },
  { MODKEY,             XK_0,      view,           { .ui = ~0 }              },

  { MODKEY,             XK_j,      focusstack,     { .i = +1 }               },
  { MODKEY,             XK_k,      focusstack,     { .i = -1 }               },
  { MODKEY,             XK_h,      setmfact,       { .f = -0.05 }            },
  { MODKEY,             XK_l,      setmfact,       { .f = +0.05 }            },
  { MODKEY | ShiftMask, XK_f,      togglefloating, { 0 }                     },
/*{ MODKEY,             XK_space,  cyclelayout,    { .i = -1 }               }, */
  { MODKEY | ShiftMask, XK_space,  cyclelayout,    { .i = +1 }               },

  { 0,             XF86XK_AudioRaiseVolume,  spawn, SHCMD(sink_up)           },
  { 0,             XF86XK_AudioLowerVolume,  spawn, SHCMD(sink_down)         },
  { 0,             XF86XK_AudioMute,         spawn, SHCMD(sink_mute)         },
  { 0,             XF86XK_AudioMicMute,      spawn, SHCMD(source_mute)       },
  { 0,             XF86XK_MonBrightnessUp,   spawn, SHCMD(brightness_up)     },
  { 0,             XF86XK_MonBrightnessDown, spawn, SHCMD(brightness_down)   },
  { 0,             XK_Print,                 spawn, SHCMD(screen_area)       },
  { 0 | ShiftMask, XK_Print,                 spawn, SHCMD(screen_full)       },

  TAGKEYS(XK_1, 0),
  TAGKEYS(XK_2, 1),
  TAGKEYS(XK_3, 2),
  TAGKEYS(XK_4, 3),
  TAGKEYS(XK_5, 4),
  TAGKEYS(XK_6, 5),
  TAGKEYS(XK_7, 6),
};

static const Button buttons[] = {
  { ClkLtSymbol,   0,      Button1, setlayout,      { 0 }                },
  { ClkLtSymbol,   0,      Button3, setlayout,      { .v = &layouts[2] } },
  { ClkWinTitle,   0,      Button1, togglewin,      { 0 }                },
  { ClkWinTitle,   0,      Button3, showhideclient, { 0 }                },
  { ClkClientWin,  MODKEY, Button1, movemouse,      { 0 }                },
  { ClkClientWin,  MODKEY, Button3, resizemouse,    { 0 }                },
  { ClkTagBar,     0,      Button1, view,           { 0 }                },
  { ClkTagBar,     0,      Button3, toggleview,     { 0 }                },
  { ClkTagBar,     MODKEY, Button1, tag,            { 0 }                },
  { ClkTagBar,     MODKEY, Button3, toggletag,      { 0 }                },
};

