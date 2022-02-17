/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  FREQ_PANEL                       1       /* callback function: OnFreqPanel */
#define  FREQ_PANEL_IDC_SWITCH_PANEL      2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  FREQ_PANEL_IDC_GRAPH_SPECTRU     3       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_NR_PUNCTE             4       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_CMD_BTT_FRQ           5       /* control type: command, callback function: OnFrqButton */
#define  FREQ_PANEL_IDC_POWER_PEAK        6       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_IDC_FREQ_PEAK         7       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_GRAPH_SPECTRU_FILTRAT 8       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_RING_TIP_FEREASTRA    9       /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_RING_TIP_FILTRU       10      /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_CMD_BTT_Filter        11      /* control type: command, callback function: OnFilterButton */

#define  MAIN_PANEL                       2       /* callback function: OnMainPanel */
#define  MAIN_PANEL_IDC_SWITCH_PANEL      2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  MAIN_PANEL_GRAPH_2               3       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_IDC_GRAPH_RAW_DATA    4       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_NEXT_BUTTON           5       /* control type: command, callback function: OnNextButton */
#define  MAIN_PANEL_LOAD_BUTTON           6       /* control type: command, callback function: OnLoadButtonCB */
#define  MAIN_PANEL_APLICA                7       /* control type: command, callback function: OnAplicaButton */
#define  MAIN_PANEL_PREV_BUTTON           8       /* control type: command, callback function: OnPrevButton */
#define  MAIN_PANEL_MIN_INDEX_BTN         9       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MAX_INDEX_BTN         10      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MEDIAN_BTN            11      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_DISP_BTN              12      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MAX_BTN               13      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MIN_BTN               14      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MEAN_BTN              15      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_RING                  16      /* control type: ring, callback function: (none) */
#define  MAIN_PANEL_SAMPLE_BTN            17      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NPOINTS_BTN           18      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_GRAPH_3               19      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_DIM_FER               20      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_START                 21      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_STOP                  22      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_ALPHA                 23      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_ZERO_BTN              24      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnAplicaButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFilterButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFreqPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFrqButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnMainPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnNextButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPrevButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
