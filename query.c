/***********************************************************************/
/* QUERY.C - Functions related to QUERY,STATUS and EXTRACT             */
/***********************************************************************/
/*
 * THE - The Hessling Editor. A text editor similar to VM/CMS xedit.
 * Copyright (C) 1991-2013 Mark Hessling
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to:
 *
 *    The Free Software Foundation, Inc.
 *    675 Mass Ave,
 *    Cambridge, MA 02139 USA.
 *
 *
 * If you make modifications to this software that you feel increases
 * it usefulness for the rest of the community, please email the
 * changes, enhancements, bug fixes as well as any and all ideas to me.
 * This software is going to be maintained and enhanced as deemed
 * necessary by the community.
 *
 * Mark Hessling, mark@rexx.org  http://www.rexx.org/
 */


#include <the.h>
#include <proto.h>

#include <query.h>

#ifdef HAVE_PROTO
short extract_point_settings(short,CHARTYPE *);
short extract_prefix_settings(short,CHARTYPE *,CHARTYPE);
short extract_colour_settings(short,CHARTYPE *,CHARTYPE,CHARTYPE *,bool,bool);
short extract_autocolour_settings(short,CHARTYPE *,CHARTYPE,CHARTYPE *,bool);
void get_etmode(CHARTYPE *,CHARTYPE *);
#else
short extract_point_settings();
short extract_prefix_settings();
short extract_autocolour_settings();
void get_etmode();
#endif

extern ExtractFunction extract_after_function;
extern ExtractFunction extract_alt;
extern ExtractFunction extract_alt_function;
extern ExtractFunction extract_altkey_function;
extern ExtractFunction extract_arbchar;
extern ExtractFunction extract_autocolour;
extern ExtractFunction extract_autocolor;
extern ExtractFunction extract_autosave;
extern ExtractFunction extract_autoscroll;
extern ExtractFunction extract_backup;
extern ExtractFunction extract_batch_function;
extern ExtractFunction extract_before_function;
extern ExtractFunction extract_beep;
extern ExtractFunction extract_blank_function;
extern ExtractFunction extract_block;
extern ExtractFunction extract_block_function;
extern ExtractFunction extract_bottomedge_function;
extern ExtractFunction extract_case;
extern ExtractFunction extract_clearerrorkey;
extern ExtractFunction extract_clearscreen;
extern ExtractFunction extract_clock;
extern ExtractFunction extract_command_function;
extern ExtractFunction extract_cmdarrows;
extern ExtractFunction extract_cmdline;
extern ExtractFunction extract_color;
extern ExtractFunction extract_coloring;
extern ExtractFunction extract_colour;
extern ExtractFunction extract_colouring;
extern ExtractFunction extract_column;
extern ExtractFunction extract_compat;
extern ExtractFunction extract_ctrl_function;
extern ExtractFunction extract_ctlchar;
extern ExtractFunction extract_curline;
extern ExtractFunction extract_cursor;
extern ExtractFunction extract_cursorstay;
extern ExtractFunction extract_current_function;
extern ExtractFunction extract_define;
extern ExtractFunction extract_defsort;
extern ExtractFunction extract_dir_function;
extern ExtractFunction extract_dirfileid;
extern ExtractFunction extract_dirinclude;
extern ExtractFunction extract_end_function;
extern ExtractFunction extract_display;
extern ExtractFunction extract_ecolor;
extern ExtractFunction extract_ecolour;
extern ExtractFunction extract_efileid;
extern ExtractFunction extract_eof;
extern ExtractFunction extract_eof_function;
extern ExtractFunction extract_eolout;
extern ExtractFunction extract_equivchar;
extern ExtractFunction extract_erroroutput;
extern ExtractFunction extract_etmode;
extern ExtractFunction extract_field;
extern ExtractFunction extract_fieldword;
extern ExtractFunction extract_first_function;
extern ExtractFunction extract_focuseof_function;
extern ExtractFunction extract_focustof_function;
extern ExtractFunction extract_filename;
extern ExtractFunction extract_filestatus;
extern ExtractFunction extract_filetabs;
extern ExtractFunction extract_fmode;
extern ExtractFunction extract_fname;
extern ExtractFunction extract_fpath;
extern ExtractFunction extract_ftype;
extern ExtractFunction extract_fullfname;
extern ExtractFunction extract_getenv;
extern ExtractFunction extract_hex;
extern ExtractFunction extract_header;
extern ExtractFunction extract_hexdisplay;
extern ExtractFunction extract_hexshow;
extern ExtractFunction extract_highlight;
extern ExtractFunction extract_idline;
extern ExtractFunction extract_impmacro;
extern ExtractFunction extract_impos;
extern ExtractFunction extract_inblock_function;
extern ExtractFunction extract_incommand_function;
extern ExtractFunction extract_initial_function;
extern ExtractFunction extract_inprefix_function;
extern ExtractFunction extract_inputmode;
extern ExtractFunction extract_insertmode;
extern ExtractFunction extract_insertmode_function;
extern ExtractFunction extract_interface;
extern ExtractFunction extract_lastkey;
extern ExtractFunction extract_lastmsg;
extern ExtractFunction extract_lastop;
extern ExtractFunction extract_lastrc;
extern ExtractFunction extract_leftedge_function;
extern ExtractFunction extract_length;
extern ExtractFunction extract_line;
extern ExtractFunction extract_lineflag;
extern ExtractFunction extract_linend;
extern ExtractFunction extract_lscreen;
extern ExtractFunction extract_macro;
extern ExtractFunction extract_macroext;
extern ExtractFunction extract_macropath;
extern ExtractFunction extract_margins;
extern ExtractFunction extract_modifiable_function;
extern ExtractFunction extract_monitor;
extern ExtractFunction extract_mouse;
extern ExtractFunction extract_msgline;
extern ExtractFunction extract_msgmode;
extern ExtractFunction extract_nbfile;
extern ExtractFunction extract_nbscope;
extern ExtractFunction extract_newlines;
extern ExtractFunction extract_nondisp;
extern ExtractFunction extract_number;
extern ExtractFunction extract_parser;
extern ExtractFunction extract_pagewrap;
extern ExtractFunction extract_pending;
extern ExtractFunction extract_point;
extern ExtractFunction extract_position;
extern ExtractFunction extract_prefix;
extern ExtractFunction extract_printer;
extern ExtractFunction extract_profile;
extern ExtractFunction extract_pscreen;
extern ExtractFunction extract_reprofile;
extern ExtractFunction extract_regexp;
extern ExtractFunction extract_readonly;
extern ExtractFunction extract_readv;
extern ExtractFunction extract_reserved;
extern ExtractFunction extract_rexx;
extern ExtractFunction extract_rexxhalt;
extern ExtractFunction extract_rexxoutput;
extern ExtractFunction extract_rightedge_function;
extern ExtractFunction extract_ring;
extern ExtractFunction extract_scale;
extern ExtractFunction extract_scope;
extern ExtractFunction extract_screen;
extern ExtractFunction extract_select;
extern ExtractFunction extract_shadow;
extern ExtractFunction extract_shadow_function;
extern ExtractFunction extract_shift_function;
extern ExtractFunction extract_showkey;
extern ExtractFunction extract_size;
extern ExtractFunction extract_slk;
extern ExtractFunction extract_spacechar_function;
extern ExtractFunction extract_statopt;
extern ExtractFunction extract_statusline;
extern ExtractFunction extract_stay;
extern ExtractFunction extract_synelem;
extern ExtractFunction extract_synonym;
extern ExtractFunction extract_tabkey;
extern ExtractFunction extract_tabline;
extern ExtractFunction extract_tabs;
extern ExtractFunction extract_tabsin;
extern ExtractFunction extract_tabsout;
extern ExtractFunction extract_targetsave;
extern ExtractFunction extract_terminal;
extern ExtractFunction extract_thighlight;
extern ExtractFunction extract_timecheck;
extern ExtractFunction extract_tof;
extern ExtractFunction extract_tof_function;
extern ExtractFunction extract_tofeof;
extern ExtractFunction extract_topedge_function;
extern ExtractFunction extract_trailing;
extern ExtractFunction extract_typeahead;
extern ExtractFunction extract_ui;
extern ExtractFunction extract_undoing;
extern ExtractFunction extract_untaa;
extern ExtractFunction extract_utf8;
extern ExtractFunction extract_verify;
extern ExtractFunction extract_vershift;
extern ExtractFunction extract_verone_function;
extern ExtractFunction extract_version;
extern ExtractFunction extract_width;
extern ExtractFunction extract_word;
extern ExtractFunction extract_wordwrap;
extern ExtractFunction extract_wrap;
extern ExtractFunction extract_xterminal;
extern ExtractFunction extract_zone;

CHARTYPE query_num0[3]; /* DO NOT USE THIS FOR DATA !! */
CHARTYPE query_num1[10];
CHARTYPE query_num2[10];
CHARTYPE query_num3[40];
CHARTYPE query_num4[40];
CHARTYPE query_num5[10];
CHARTYPE query_num6[10];
CHARTYPE query_num7[10];
CHARTYPE query_num8[10];
CHARTYPE query_rsrvd[MAX_FILE_NAME+1];

/*man-start*********************************************************************


========================================================================
QUERY, EXTRACT and STATUS
========================================================================

     The following lists the valid variables that can be queried and
     also shows what values are returned. For both <QUERY> and <STATUS>,
     the values are concatenated together and displayed as a single
     line. For <EXTRACT> the REXX variables that are set are defined.
     The capitalised portion of the variable is the minimum abbreviation
     recognised.

     The bracketed text at the end of the description indicates from
     which commands a valid response will be supplied.
     (Q-Query, E-Extract, M-Modify and S-Status).

ALT
     The number of alterations to the current file since the last <SAVE>
     or automatic save via <SET AUTOSAVE>. Can be set by <SET ALT>
     (QES)

     alt.0           - 2
     alt.1           - Number of alterations since last SAVE or autosave
     alt.2           - Number of alterations since last SAVE

ARBchar
     The status of <SET ARBCHAR> and the ARBCHAR characters.
     (QEMS)

     arbchar.0       - 3
     arbchar.1       - ON|OFF
     arbchar.2       - multiple match character
     arbchar.3       - single match character

AUTOCOLOR [*|mask]
AUTOCOLOUR [*|mask]
     The status of auto colouring for the supplied file mask or
     "magic number". Set by <SET AUTOCOLOR> or <SET AUTOCOLOUR>.
     The variable name is spelt the save way that the option is specified.
     (QES)

     autocolor.0     - 3
     autocolor.1     - filemask or "magic number"
     autocolor.2     - name of parser associated or "NULL"
     autocolor.3     - MAGIC (if "magic number") or empty

     If no file mask or "magic number" is supplied as a parameter
     or '*' is passed, details of all parser mappings are set as
     follows:

     autocolor.0     - number of parser mappings
     autocolor.1     - first mapping (mask parser [MAGIC])
     autocolor.n     - nth mapping

AUtosave
     The status of <SET AUTOSAVE> and/or the frequency setting.
     (QEMS)

     autosave.0      - 1
     autosave.1      - OFF|n

AUTOSCroll
     The status of <SET AUTOSCROLL> and/or the frequency setting.
     (QEMS)

     autoscroll.0    - 1
     autoscroll.1    - OFF|HALF|n

BACKup
     Indicates if a backup file is kept after editing and what the
     backup suffix is.
     (QEMS)

     backup.0        - 2
     backup.1        - ON|OFF|TEMP|KEEP|INPLACE
     backup.2        - backup suffix string

BEEP
     Indicates if the bell is sounded on display of error messages.
     Set by <SET BEEP>
     (QEMS)

     beep.0          - 1
     beep.1          - ON|OFF

BLOCK
     Returns information about the marked <block>, if any.
     (QE)

     block.0         - 6 if a marked block exists, or 1 for NONE
     block.1         - type of marked block (LINE|BOX|NONE|WORD|COLUMN|CUA)
     block.2         - line number of start of block
     block.3         - column number of start of block
     block.4         - line number of end of block
     block.5         - column number of end of block
     block.6         - file name containing marked block

CASE
     The settings related to the CASE of characters entered, searched
     for, changed and sorted. Set by <SET CASE>
     (QEMS)

     case.0          - 6
     case.1          - MIXED|UPPER|LOWER
     case.2          - RESPECT|IGNORE
     case.3          - RESPECT|IGNORE
     case.4          - RESPECT|IGNORE
     case.5          - MIXED|UPPER|LOWER
     case.6          - MIXED|UPPER|LOWER

CLEARErrorkey
     Returns the key that clears the <message line>. If no specific
     key is defined, ie ALL keys perform a reset, then * is returned.
     Set by <SET CLEARERRORKEY>.
     (QEMS)

     clearerrorkey.0 - 1
     clearerrorkey.1 - keyname|*

CLEARScreen
     Indicates if the screen is cleared on exit from THE. Set by
     <SET CLEARSCREEN>.
     (QEMS)

     clearscreen.0   - 1
     clearscreen.1   - ON|OFF

CLOCK
     Indicates if the time is displayed on the status line. Set by
     <SET CLOCK>.
     (QEMS)

     clock.0         - 1
     clock.1         - ON|OFF

CMDArrows
     Returns the settings for arrow key behaviour on <command line>.
     Set by <SET CMDARROWS>.
     (QEMS)

     cmdarrows.0     - 1
     cmdarrows.1     - RETRIEVE|TAB

CMDline
     The settings for the <command line>. Set by <SET CMDLINE>.
     (QEMS)

     If CMDLINE OFF

     cmdline.0       - 1
     cmdline.1       - OFF

     If CMDLINE ON, BOTTOM or TOP

     cmdline.0       - 3
     cmdline.1       - ON|TOP|BOTTOM
     cmdline.2 *     - line number within window of command line
     cmdline.3 *     - contents of command line

     * these values are only returned with <EXTRACT>

COLOR [*|area]
COLOUR [*|area]
     Displays the current color settings for the file.
     Set by <SET COLOR> or <SET COLOUR>. The variable name is
     spelt the save way that the option is specified.

     With the ['*'] option, (or no option), returns color settings
     for all areas of the screen.
     (QE)

     color.0         - 40
     color.1         - FILEAREA        followed by its color
     color.2         - CURLINE         followed by its color
     color.3         - BLOCK           followed by its color
     color.4         - CBLOCK          followed by its color
     color.5         - CMDLINE         followed by its color
     color.6         - IDLINE          followed by its color
     color.7         - MSGLINE         followed by its color
     color.8         - ARROW           followed by its color
     color.9         - PREFIX          followed by its color
     color.10        - CPREFIX         followed by its color
     color.11        - PENDING         followed by its color
     color.12        - SCALE           followed by its color
     color.13        - TOFEOF          followed by its color
     color.14        - CTOFEOF         followed by its color
     color.15        - TABLINE         followed by its color
     color.16        - SHADOW          followed by its color
     color.17        - STATAREA        followed by its color
     color.18        - DIVIDER         followed by its color
     color.19        - RESERVED        followed by its color
     color.20        - NONDISP         followed by its color
     color.21        - HIGHLIGHT       followed by its color
     color.22        - CHIGHLIGHT      followed by its color
     color.23        - THIGHLIGHT      followed by its color
     color.24        - SLK             followed by its color
     color.25        - GAP             followed by its color
     color.26        - CGAP            followed by its color
     color.27        - ALERT           followed by its color
     color.28        - DIALOG          followed by its color
     color.29        - BOUNDMARKER     followed by its color
     color.30        - FILETABS        followed by its color
     color.31        - FILETABSDIV     followed by its color
     color.32        - CURSORLINE      followed by its color
     color.33        - DIALOGBORDER    followed by its color
     color.34        - DIALOGEDITFIELD followed by its color
     color.35        - DIALOGBUTTON    followed by its color
     color.36        - DIALOGABUTTON   followed by its color
     color.37        - POPUPBORDER     followed by its color
     color.38        - POPUPCURLINE    followed by its color
     color.39        - POPUP           followed by its color
     color.40        - POPUPDIVIDER    followed by its color

     With the ['area'] option, returns color settings
     for the specified area of the screen.
     (QE)

     color.0         - 1
     color.1         - 'area' followed by its color

COLORING
COLOURING
     Displays the current color settings for the file. The variable
     name is spelt the same way that the option is specified.
     Set by <SET COLORING> or <SET COLOURING>.
     (QEMS)

     coloring.0      - 3
     coloring.1      - ON|OFF
     coloring.2      - AUTO|parser (if coloring.1 is ON)
     coloring.3      - parser (if coloring.1 is ON)

COLUMN
     Displays the current value of the focus column.
     (QE)

     column.0        - 1
     column.1        - Column number of focus column

COMPAT
     The settings for the compatibility mode. Set by <SET COMPAT>.
     (QEMS)

     compat.0        - 3
     compat.1        - THE|XEDIT|KEDIT|KEDITW|ISPF (compatibility LOOK)
     compat.2        - THE|XEDIT|KEDIT|KEDITW|ISPF (compatibility FEEL)
     compat.3        - THE|XEDIT|KEDIT|KEDITW|ISPF (compatibility KEYS)

CTLchar [*|char]
     The definitions of control characters that affect the display of
     <reserved line>s. Set by <SET CTLCHAR>.
     (QES)

     With the ['*'] option, (or no option), returns a list of all
     control characters that have been defined.

     ctlchar.0       - 1 if ctlchar.1 is OFF, otherwise 3
     ctlchar.1       - ON|OFF
     ctlchar.2       - The character defined as the escape character.
     ctlchar.3       - List of defined control characters, if any.

     With the ['char'] option, returns color settings for the specified
     control character.

     ctlchar.0       - 1 if ctlchar.1 is OFF, otherwise 2
     ctlchar.1       - PROTECT|NOPROTECT|OFF
     ctlchar.2       - The color defined for this control character.

CURline
     The value and position of the <current line> within the current file.
     Set by <SET CURLINE>. Also provides the contents of the <focus line>
     and an indication of the status of the <focus line> for the current
     session.
     (QEMS)

     With <EXTRACT> command:

     curline.0       - 6
     curline.1       - curline setting
     curline.2       - line number within window of current line
     curline.3       - contents of <focus line>
     curline.4       - ON|OFF (ON if line has changed or been added this session)
     curline.5       - OLD|OLD CHANGED|NEW|NEW CHANGED
     curline.6       - selection level of focus line (same as select.1)

     Note: If <SET COMPAT> (feel) is set to XEDIT, curline.3 will always
     return the contents of the <current line>.

     The setting of curline.5 is as follows:
       OLD:            The line existed in its current form in the
                       file before THE began.
       OLD CHANGED:    The line was in the file before THE started,
                       but has been changed during the current editing
                       session.
       NEW:            The line has been added to the file during
                       the current edit session and not been changed.
       NEW CHANGED:    The line has been added to the file during
                       the current editing session and has been changed.

     With <QUERY>, <MODIFY> and <STATUS> commands:

     curline.0       - 1
     curline.1       - curline setting

CURSor
     The position of the cursor within the displayed screen and file
     at the time the <EXTRACT> /CURSOR/ is issued and the position of
     the cursor at the time the <macro> was issued.
     If the cursor is not in the <filearea>, then line and column
     values will be set to -1.
     (QE)

     cursor.0        - 8
     cursor.1        - line number of cursor in screen (current)
     cursor.2        - column number of cursor in screen (current)
     cursor.3        - line number of cursor in file (current)
     cursor.4        - column number of cursor in file (current)
     cursor.5        - line number of cursor in screen (original)
     cursor.6        - column number of cursor in screen (original)
     cursor.7        - line number of cursor in file (original)
     cursor.8        - column number of cursor in file (original)

CURSORSTay
     The setting for the behaviour of the cursor after a file scroll.
     Set by <SET CURSORSTAY>.
     (QEMS)

     cursorstay.0    - 1
     cursorstay.1    - ON|OFF

DEFINE [key|KEY|MOUSE|*]
     Returns details about the commands associated with a keyboard key
     or mouse key.  The details returned are the same as those displayed
     by the <SHOWKEY> command.
     Set by <DEFINE>.

     The following forms are available:

     With no parameter or '*', all details about all key and mouse
     event definitions are returned. (Similar to <SHOWKEY> ALL)
     (E)

     define.0        - number of all definitions
     define.1        - command assigned to the 1st key or mouse
     define.n        - command assigned to the nth key or mouse

     With a parameter of 'KEY', all details about all key
     definitions are returned.
     (E)

     define.0        - number of key definitions
     define.1        - command assigned to the 1st key
     define.n        - command assigned to the nth key

     With a parameter of 'MOUSE', all details about all mouse event
     definitions are returned.
     (E)

     define.0        - number of mouse definitions
     define.1        - command assigned to the 1st mouse event
     define.n        - command assigned to the nth mouse event

     With a 'key' mnemonic specified, details about this one key are
     returned. The displayed output from a <QUERY> DEFINE is in the same
     format as for the <DEFINE> command.
     (QE)

     define.0        - 1
     define.1        - command assigned to the key

DEFSORT
     Specifies the order in which files are sorted in the DIR.DIR file.
     Set by <SET DEFSORT>.
     (QEMS)

     defsort.0       - 2
     defsort.1       - DIRECTORY|NAME|SIZE|TIME|DATE|OFF
     defsort.2       - ASCENDING|DESCENDING

DIRFILEID
     The value of the path and filename of the <focus line> in a DIR.DIR
     file.
     (E)

     dirfileid.0     - 2
     dirfileid.1     - full path of directory
     dirfileid.2     - file name at <focus line>

     If the <focus line> is <Bottom-of-File line> or <Top-of-File line> then:

     dirfileid.0     - 1
     dirfileid.1     - full path of directory

DIRInclude
     The value of the file type masks currently in place for display
     in DIR.DIR file. Set by <SET DIRINCLUDE>.
     (QEMS)

     dirinclude.0    - 1
     dirinclude.1    - list of file types shown in DIR.DIR file

DISPlay
     Indicates the range of selection levels current. Set by
     <SET DISPLAY>.
     (QEMS)

     display.0       - 2
     display.1       - display low value
     display.2       - display high value

ECOLOR [*|category]
ECOLOUR [*|category]
     Displays the current color settings for syntax highlighting.
     Set by <SET ECOLOR> or <SET ECOLOUR>. The variable name is
     spelt the save way that the option is specified.

     With the ['*'] option, (or no option), returns color settings
     for all categories.
     (QE)

     ecolor.0        - 35
     ecolor.1        - A followed by its color
     ecolor.2        - B followed by its color
     ecolor.3        - C followed by its color
     ecolor.4        - D followed by its color
     ecolor.5        - E followed by its color
     ecolor.6        - F followed by its color
     ecolor.7        - G followed by its color
     ecolor.8        - H followed by its color
     ecolor.9        - I followed by its color
     ecolor.10       - J followed by its color
     ecolor.11       - K followed by its color
     ecolor.12       - L followed by its color
     ecolor.13       - M followed by its color
     ecolor.14       - N followed by its color
     ecolor.15       - O followed by its color
     ecolor.16       - P followed by its color
     ecolor.17       - Q followed by its color
     ecolor.18       - R followed by its color
     ecolor.19       - S followed by its color
     ecolor.20       - T followed by its color
     ecolor.21       - U followed by its color
     ecolor.22       - V followed by its color
     ecolor.23       - W followed by its color
     ecolor.24       - X followed by its color
     ecolor.25       - Y followed by its color
     ecolor.26       - Z followed by its color
     ecolor.27       - 1 followed by its color
     ecolor.28       - 2 followed by its color
     ecolor.29       - 3 followed by its color
     ecolor.30       - 4 followed by its color
     ecolor.31       - 5 followed by its color
     ecolor.32       - 6 followed by its color
     ecolor.33       - 7 followed by its color
     ecolor.34       - 8 followed by its color
     ecolor.35       - 9 followed by its color

     With the ['category'] option, returns color settings
     for the specified syntax highlighting category.
     (E)

     color.0         - 1
     color.1         - ['category'] followed by its color

EFILEId
     The original full filename of the current file. See <SET FILENAME> for a
     full description of the components of a file name. EFILEID.2 differs from
     the value returned by KEDIT.
     (QE)

     efileid.0       - 2
     efileid.1       - Original full file name.
     efileid.2       - Original file name used to edit the file.

EOF
     Indicates if the <current line> is on the <Bottom-of-File line>.
     (QES)

     eof.0           - 1
     eof.1           - ON|OFF

EOLout
     Returns the value of the end-of-line character(s).
     (QEMS)

     eolout.0        - 1
     eolout.1        - LF|CRLF|CR|NONE

EQUIVChar
     Returns the equivalence character. Set by <SET EQUIVCHAR>.
     (QEMS)

     equivchar.0     - 2
     equivchar.1     - equivalence character

ERROROUTput
     Indicates if THE errors are also echoed to the calling window.
     Set by <SET ERROROUTPUT>.
     (QEMS)

     erroroutput.0    - 1
     erroroutput.1    - ON|OFF

ETMODE
     Indicates if extended display mode is set. Set by <SET ETMODE>.
     (QEMS)

     etmode.0        - 2
     etmode.1        - ON|OFF
     etmode.2        - character ranges if not all ON or OFF

FExt
     The extension of the current file. The characters following
     the trailing '.' character. Same as FType.
     (QEM)

     fext.0          - 1
     fext.1          - File extension.

FIELD
     Details about the current cursor field.
     (QE)

     field.0         - 4
     field.1         - contents of the cursor field
     field.2         - character under the cursor
     field.3         - column position in the cursor field
     field.4         - COMMAND|TEXT|PREFIX

FIELDWORD
     Details about the word closest to the cursor in the current cursor field.
     (E)

     fieldword.0     - 3
     fieldword.1     - word as defined by <SET WORD> ALPHANUM
     fieldword.2     - word as defined by <SET WORD> NONBLANK
     fieldword.3     - starting column of word

FILEName
     The full filename of the current file, including any file
     extension.
     (QEM)

     filename.0      - 1
     filename.1      - Full file name.

FILESTATUS
     Details about the status of the file being edited.
     (QEM)

     filestatus.0    - 3
     filestatus.1    - sharing mode - NONE
     filestatus.2    - access type - READONLY|READWRITE
     filestatus.3    - end of line - CR/LF/CRLF/NONE

FILETABS
     Indicates if the <filetabs> window is displayed or not.
     (QEMS)

     filetabs.0      - 1
     filetabs.1      - ON|OFF

FMode
     The file mode of the current file. Under Un*x, this will
     always return an empty string. Other platforms returns the
     first character of the file's path. ie the disk drive.
     (QEM)

     fmode.0         - 1
     fmode.1         - File mode.

FName
     The fname portion of the current file. See <SET FILENAME> for a
     full description of the components of a file name.
     (QEM)

     fname.0         - 1
     fname.1         - File name.

FPath
     The path name of the current file. This includes a trailing
     directory separator.
     (QEM)

     fpath.0         - 1
     fpath.1         - File path.

FType
     The extension of the current file. The characters following
     the trailing '.' character.
     (QEM)

     ftype.0         - 1
     ftype.1         - File extension.

FULLFName
     Indicates if the fully qualified filename is displayed on the
     <idline>.
     (QEMS)

     fullfname.0     - 1
     fullfname.1     - ON|OFF

GETENV variable
     The value of the supplied environment 'variable' or ***invalid***
     if the 'variable' does not exist. On platforms other than Unix
     the supplied variable name is uppercased before obtaining the
     environment variable value.
     (E)

     getenv.0        - 1
     getenv.1        - value of variable

HEX
     Indicates if hexadecimal values in commands are interpreted
     as hexadecimal values or not. Set by <SET HEX>.
     (QEMS)

     hex.0           - 1
     hex.1           - ON|OFF

HEADer
     Returns details about which sections of a TLD file are to be
     applied to the current view. Do not rely on the position of a
     particular header in the returned stem. Set by <SET HEADER>.
     (QE)

     header.0        - 11
     header.1        - section ON|OFF
     header.2        - section ON|OFF
     header.3        - section ON|OFF
     header.4        - section ON|OFF
     header.5        - section ON|OFF
     header.6        - section ON|OFF
     header.7        - section ON|OFF
     header.8        - section ON|OFF
     header.9        - section ON|OFF
     header.10       - section ON|OFF
     header.11       - section ON|OFF

HEXDISPlay
     Indicates if the current character is displayed on the <status line>.
     Set by <SET HEXDISPLAY>.
     (QEMS)

     hexdisplay.0    - 1
     hexdisplay.1    - ON|OFF

HEXShow
     Returns details of HEXSHOW placement. Set by <SET HEXSHOW>.
     (QEMS)

     hexshow.0       - 2
     hexshow.1       - ON|OFF
     hexshow.2       - line displaying first hexshow line

HIGHlight
     Returns details of HIGHLIGHT settings. Set by <SET HIGHLIGHT>.
     (QEMS)

     highlight.0     - 1 or 3 (if highlight.1 = SELECT)
     highlight.1     - OFF|ALTERED|TAGGED|SELECT
     highlight.2     - minimum (or only) selection level for SELECT
     highlight.3     - maximum selection level for SELECT

IDline [*]
     Indicates if the <idline> is displayed for a file. Set by
     <SET IDLINE>.
     (QEMS)

     idline.0        - 1
     idline.1        - ON|OFF

     With ['*'] option with EXTRACT, the contents of the <idline>
     is also returned.
     (E)

     idline.0        - 2
     idline.1        - ON|OFF
     idline.2        - contents of <idline>.

IMPMACro
     Indicates if implied <macro> processing is on or off. Set by
     <SET IMPMACRO>.
     (QEMS)

     impmacro.0      - 1
     impmacro.1      - ON|OFF

IMPOS
     Indicates if implied operating system command processing is on
     or off. Set by <SET IMPOS> or <SET IMPCMSCP>.
     (QEMS)

     impos.0         - 1
     impos.1         - ON|OFF

INPUTMode
     Indicates the inputmode for the current view.
     Set by <SET INPUTMODE>.
     (QEMS)

     inputmode.0     - 1
     inputmode.1     - OFF|FULL|LINE

INSERTmode
     Indicates if currently in insert mode or overstrike mode. Set by
     <SET INSERTMODE>.
     (QEMS)

     insertmode.1    - 1
     insertmode.1    - ON|OFF

LASTmsg
     Return the text of the last error message generated.
     (E)

     lastmsg.0       - 1
     lastmsg.1       - text of last message.

LASTKEY [n]
     Return information about the last key, or the nth last key pressed. Only the last
     8 keys are retained.
     (E)

     lastkey.0       - 4
     lastkey.1       - name of the key.
     lastkey.2       - character associated with the key (if applicable)
     lastkey.3       - curses mnemonic code
     lastkey.4       - shift status. see <READV> for details

LASTOP [command|*]
     Returns the last operand from selected commands.  command can be
     one of ALter, Change, CLocate COUnt, Find, Locate SCHange or TFind.
     (QE)
     EXTract /LASTOP command/ sets:

     lastop.0        - 1
     lastop.1        - command and its last operand

     If no command is supplied as a parameter or '*' is passed,
     details of all last operands are set as follows:

     lastop.0        - 9
     lastop.1        - "alter" and its last operand (not implemented)
     lastop.2        - "change" and its last operand
     lastop.3        - "clocate" and its last operand (not implemented)
     lastop.4        - "count" and its last operand (not implemented)
     lastop.5        - "find" and its last operand
     lastop.6        - "locate" and its last operand
     lastop.7        - "schange" and its last operand
     lastop.8        - "tfind" and its last operand (not implemented)
     lastop.9        - "search" and its last operand

LASTRC
     Returns the return code from last command issued from <command line>.
     (QES)

     lastrc.0        - 1
     lastrc.1        - Last return code.

LENgth
     Length of the current line.
     (QES)

     length.0        - 1
     length.1        - Length of current line.

LIne
     Line number of <focus line> in current file; or <current line>
     if <SET COMPAT> (feel) is set to XEDIT.
     (QES)

     line.0          - 1
     line.1          - Line number

LINEFLAG
     Returns information about the flags set on the <focus line>, or
     <current line> if <SET COMPAT> (feel) is set to XEDIT.
     (QEMS)

     lineflag.0      - 3
     lineflag.1      - NEW|NONEW
     lineflag.2      - CHANGE|NOCHANGE
     lineflag.3      - TAG|NOTAG

LINENd
     Indicates if multiple commands allowed on command line
     and the delimiter. Set by <SET LINEND>.
     (QEMS)

     linend.0        - 2
     linend.1        - ON|OFF
     linend.2        - delimiter

LScreen
     Displays the size of the current screen and window. Also
     shows the upper left corner of the window.
     (QE)

     lscreen.0       - 6
     lscreen.1       - height of current screen
     lscreen.2       - width of current screen
     lscreen.3       - screen line of upper left corner of screen
     lscreen.4       - screen column of upper left corner of screen
     lscreen.5       - height of display
     lscreen.6       - width of display

MACRO
     Indicates if macros are executed before commands. Set by
     <SET MACRO>.
     (QEMS)

     macro.0         - 1
     macro.1         - ON|OFF

MACROExt
     The current setting for a macro's file extension. Set by
     <SET MACROEXT>.
     (QEMS)

     macroext.0      - 1
     macroext.1      - Default file extension

MACROPath
     The path that THE looks for by default for macro files. Set by
     <SET MACROPATH>.
     (QEMS)

     macropath.0     - 1
     macropath.1     - Path for macro files.

MARgins
     The settings for left and right margins and paragraph indent.
     Set by <SET MARGINS>.
     (QEMS)

     margins.0       - 3
     margins.1       - left column
     margins.2       - right column
     margins.3       - indent value (column or offset from left margin)

MONITOR
     Indicates if the combination of monitor and the curses package
     supports colour. If the curses package supports colour, then
     monitor.1 is set to COLOR and monitor.2 can be COLOR or MONO
     depending on whether the monitor supports colour. If monitor.1
     is MONO then monitor.2 will also be set to MONO.
     (QE)

     monitor.0       - 2
     monitor.1       - COLOR|MONO
     monitor.2       - COLOR|MONO

MOUSE
     Indicates if the mouse is supported as an input device. Set by
     <SET MOUSE>.
     (QEMS)

     mouse.0         - 1
     mouse.1         - ON|OFF

MSGLine
     Returns details of where the <message line> is displayed. Set by
     <SET MSGLINE>.
     (QEMS)

     msgline.0       - 4
     msgline.1       - ON
     msgline.2       - line position of <message line>
     msgline.3       - number of message lines available
     msgline.4       - OVERLAY (returned for compatibility reasons)

MSGMode
     Indicates if messages are suppressed. Set by <SET MSGMODE>.
     (QEMS)

     msgmode.0       - 2
     msgmode.1       - ON|OFF
     msgmode.2       - LONG

NBFile
     Returns with the number of files currently in the <ring>.
     (QES)

     nbfile.0        - 1
     nbfile.1        - Number of files in ring

NBScope
     Returns with the number of lines currently in scope. If SCOPE ALL
     is in effect, this will be the same number as SIZE.
     (QE)

     nbscope.0       - 1
     nbscope.1       - Number of lines in scope
     nbscope.2       - Line number of focus line in scope

NEWLines
     Indicates if NEWLINES variable is set to LEFT or ALIGNED.
     Set by <SET NEWLINES>.
     (QEMS)

     newlines.0      - 1
     newlines.1      - ALIGNED|LEFT

NONDisp
     Returns the character that is displayed for extended characters
     that are not displayed. Set by <SET NONDISP>.
     (QEMS)

     nondisp.0       - 1
     nondisp.1       - char

NUMber
     Indicates if line numbers are displayed in the prefix area.
     Set by <SET NUMBER>.
     (QEMS)

     number.0        - 1
     number.1        - ON|OFF

PAGEWRAP
     Indicates if the scrolling the file view using the <FORWARD>
     and <BACKWARD> commands will wrap if the cursor is at the
     <Bottom-of-File line> or <Top-of-File line> respectively.
     Set by <SET PAGEWRAP>.
     (QEMS)

     pagewrap.0      - 1
     pagewrap.1      - ON|OFF

PARSER [*|parser]
     Displays the filename of the THE Language Defintion file from which
     a syntax highlighting parser was loaded. Set by <SET PARSER>.
     (QES)

     parser.0        - 2
     parser.1        - Name of parser
     parser.2        - Filename of TLD file

     If no parser is supplied as a parameter or '*' is passed,
     details of all parsers are set as follows:

     parser.0        - number of parsers currently defined
     parser.1        - name and filename of first parser
     parser.i        - name and filename of ith parser

PENDing [BLOCK] [OLDNAME] name|* [target1 [target2]
     Returns information about pending prefix commands.
     (E)

     pending.0       - 7
     pending.1       - line number in file
     pending.2       - newname - actual name entered in prefix area
     pending.3       - oldname - original name of macro after synonym resolution
     pending.4       - BLOCK or null
     pending.5       - value of first operand
     pending.6       - value of second operand
     pending.7       - value of third operand

Point [*]
     Returns the name and line number of the <focus line>, or names and
     line numbers of all lines in a file if '*' is specified.
     If <SET COMPAT> (feel) is set to XEDIT, then the name and line number
     of the <current line> is returned, rather than the name and line
     number of the <focus line>.

     (QE)

     With no arguments:

     point.0         - 0 or 1       (0 if focus line not named)
     point.1         - line number and name of line (if line is named)

     With ['*'] argument:
     (E)

     point.0         - number of named lines in the file
     point.1         - line number and name for first named line
     point.n         - line number and name for nth named line

     Only point.0 and point.1 are available using implied extract
     functions.

POSition
     Indicates if LINE/COL is displayed on <idline>. Set by
     <SET POSITION>.
     (QMS)

     position.0      - 1
     position.1      - ON|OFF

     The current/focus line/column is also returned via <EXTRACT>.
     (E)

     position.0      - 3
     position.1      - ON|OFF
     position.2      - current or focus line
     position.3      - current or focus column

PREfix [Synonym *|name]
     Indicates if prefix is displayed for the view and if displayed
     where is is displayed. See <SET PREFIX>.
     (QEMS)

     prefix.0        - 1 or 2     (1 if prefix.1 is OFF, 2 otherwise)
     prefix.1        - ON|OFF|NULLS
     prefix.2        - LEFT|RIGHT        (if prefix.1 is ON or NULLS)
     prefix.3        - width of prefix area
     prefix.4        - width of prefix gap

     With ['Synonym name'] option, the macroname associated with the
     synonym 'name' is returned. If 'name' is not a synonym then 'name'
     is returned as the macroname.
     (QE)

     prefix.0        - 1
     prefix.1        - synonym macroname

     With ['Synonym *'] option, all prefix synonyms are returned.
     (QE)

     prefix.0        - number of prefix synonyms
     prefix.1        - synonym macroname
     prefix.n        - synonym macroname

PRINTER
     Returns the value of the printer port or spooler. Set by
     <SET PRINTER>
     (QEMS)

     printer.0       - 1
     printer.1       - port or spooler name

PROFile
     Returns the value of the profile file being used.
     (QES)

     profile.0       - 1
     profile.1       - profile file name of blank if none being used

READONLY
     Indicates if the file being edited is allowed to be altered.
     Set by <SET READONLY>.
     (QEMS)

     readonly.0      - 1
     readonly.1      - ON|OFF|FORCE

REGEXP
     Return the type of regular expression syntax currently used
     for targets.
     Set by <SET REGEXP>.
     (QEMS)

     regexp.0        - 1
     regexp.1        - EMACS|AWK|POSIX_AWK|GREP|EGREP|POSIX_EGREP|SED|POSIX_BASIC
                     - |POSIX_MINIMAL_BASIC|POSIX_EXTENDED|POSIX_MINIMAL_EXTENDED

REPROFile
     Indicates if the specified (or implied) profile file is re-executed each
     time a file is added to the ring.
     Set by <SET REPROFILE>.
     (QEMS)

     reprofle.0      - 1
     reprofle.1      - ON|OFF

RESERved [*]
     Return with list of the screen rows that have been reserved.
     Set by <SET RESERVED>.
     (QES)

     reserved.0      - 0 if no reserved lines; 1 otherwise
     reserved.1      - list of screen row numbers reserved

     With ['*'] option, the line number, followed by the colour
     specification and <reserved line> contents are returned; one
     variable for each reserved line.
     (E)

     reserved.0      - the number of reserved lines
     reserved.1      - first reserved line
     reserved.2      - second reserved line
     reserved.n      - nth reserved line

REXX
     Returns the version details of the Rexx interpreter (if any) in
     the same format as PARSE VERSION.
     (QE)

     rexx.0          - 1
     rexx.1          - Version strings

REXXHalt
     Returns details on when a <REXX> macro can be halted during execution.
     Set by <SET REXXHALT>.
     (QE)

     rexxhalt.0      - 2
     rexxhalt.1      - Number of command calls or OFF
     rexxhalt.2      - Number of function calls or OFF

REXXOUTput
     Indicates if <REXX> output is captured to a file or not and the
     line number limit of lines to be displayed. Set by <SET REXXOUTPUT>.
     (QEMS)

     rexxoutput.0    - 2
     rexxoutput.1    - FILE|DISPLAY
     rexxoutput.2    - line number limit

RING
     Returns details of each file being edited.
     (QE)

     With <SET COMPAT> (feel) set to XEDIT, the values set are:

     ring.0          - number of variables returned (ring.1 + 1)
     ring.1          - number of files in the <ring>
     ring.2          - IDLINE of first file in the ring
     ring.3          - IDLINE of second file in the ring
     ring.n          - IDLINE of nth file in the ring

     With <SET COMPAT> (feel) set to THE or KEDIT, the values set are:

     ring.0          - number of files in the <ring>
     ring.1          - IDLINE of first file in the ring
     ring.2          - IDLINE of second file in the ring
     ring.n          - IDLINE of nth file in the ring

     Only ring.0 and ring.1 are available using implied extract
     functions.

SCALe
     Returns details of <scale line>. Set by <SET SCALE>.
     (QEMS)

     scale.0         - 2
     scale.1         - ON|OFF
     scale.2         - line displaying scale line

SCOPE
     Returns information about whether <shadow line>s will be affected by
     commands or not. Set by <SET SCOPE>.
     (QEMS)

     scope.0         - 1
     scope.1         - ALL|DISPLAY

SCReen
     Returns the number and orientation of THE screens. Set by
     <SET SCREEN>.
     (QEMS)

     screen.0        - 2
     screen.1        - Number of screens displayed
     screen.2        - HORIZONTAL|VERTICAL

SELect
     Returns the selection level of the <focus line> and the
     maximum selection level for the file. Set by <SET SELECT> and
     <ALL> commands. If <SET COMPAT> (feel) XEDIT is set, then the
     selection level of the <current line> is returned instead of
     the selection level of the <focus line>.
     (QES)

     select.0        - 2
     select.1        - selection level of focus line
     select.2        - maximum selection level for file

SHADow
     Returns the status of the display of shadow lines. Set by
     <SET SHADOW>.
     (QEMS)

     shadow.0        - 1
     shadow.1        - ON|OFF

SHOWkey key
     Returns the commands and parameters assigned to the 'key'
     passed as an argument.  This keyvalue is returned as
     READV.3 from a call to <READV> KEY.
     (E)

     showkey.0       - the number of commands assigned
     showkey.1       - first command/parameter assigned
     showkey.n       - last command/parameter assigned

     If no 'key' supplied as a parameter:

     showkey.0       - 1
     showkey.1       - INVALID KEY

     This item is not available as an implied extract function.

SIze
     Returns the number of lines in the current file.
     (QES)

     size.0          - 1
     size.1          - Lines in current file.

SLK [*|number]
     Returns details about Soft Label Keys.

     With no optional argument.
     (QEMS)

     slk.0           - 2
     slk.1           - ON|OFF|UNAVAILABLE
     slk.2           - value of -k switch from command line
     UNAVAILABLE is returned if the -k switch is not supplied on command line.
     ON is returned if -k switch supplied and SLK are showing.
     OFF is returned if -k switch supplied but SLK are not showing.

     With ['number'] option, details about the specific label are returned.
     (QE)

     slk.0           - 1
     slk.1           - label for SLK

     With ['*'] option, details about all labels are returned.
     (E)

     slk.0           - number of labels defined
     slk.1           - label for first SLK
     slk.n           - label of nth SLK

STATOPT [*|option]
     Displays the current status options in effect on the <status line>.
     Set by <SET STATOPT>

     With the ['*'] option, (or no option), returns settings
     for all options that are in effect. The returned settings are
     the same as the arguments specified in the <SET STATOPT> command.
     (QE)

     statopt.0       - the number of status options in effect
     statopt.1       - the first status option in effect
     statopt.2       - the second status option in effect
     statopt.n       - the nth status option in effect

     With the ['option'] option, returns settings for the specified
     option. If the option is invalid or has net been set, the status
     will be returned as OFF.
     (QE)

     statopt.0       - 1
     statopt.1       - the status option settings

STATUSLine
     Indicates if the <status line> is displayed and if so, where.
     Set by <SET STATUSLINE>.
     (QEMS)

     statusline.0    - 1
     statusline.1    - TOP|BOTTOM|OFF

STAY
     Indicates if the focus line stays where it is after a successful
     THE command or an unsuccessful <LOCATE> command.  Set by <SET STAY>.
     (QEMS)

     stay.0          - 1
     stay.1          - ON|OFF

SYNELEM [Cursor|row col|File line column]
     Returns the syntax highlighting element at the indicated position
     in the <file area>.
     UNKNOWN will be returned if syntax highlighting is not active for the
     current file.
     With ['Cursor'], the default, the syntax highlighting element at the position
     of the cursor in the <file area> will be returned. NONE will be
     returned if the cursor is not in the <file area>, or if there is
     no syntax highlighting element at the cursor position.
     With ['row col'], the syntax highlighting element at the position
     specified by ['row'] and ['col'] (relative to the top left of the
     <file area>, (1,1), will be returned.
     With ['File line column'], the syntax highlighting element at the position
     specified by ['line'] and ['column'] (relative to the line and column in the
     file will be returned. If the specified file location is not in view,
     UNKNOWN will be returned.
     NONE will be returned if there is no syntax highlighting element
     at the specified position.

     (QE)

     synelem.0       - 1
     synelem.1       - NONE|COMMENT|FUNCTION|HEADER|INCOMPLETESTRING|KEYWORD|LABEL|MARKUP|MATCH|NUMBER|POSTCOMPARE|PREPROCESSOR|STRING|UNKNOWN

SYNonym [*|name]
     Indicates if synonym processing is on or off. Set by <SET SYNONYM>
     command.
     (QEMS)

     synonym.0       - 1
     synonym.1       - ON|OFF

     With ['name'] option, details about the defined synonym are
     returned.
     (QE)

     synonym.0       - 4
     synonym.1       - synonym name
     synonym.2       - length of minimum abbreviation
     synonym.3       - definition
     synonym.4       - linend character (if specified)

     With ['*'] option, details about all defined synonyms are
     returned.
     (E)

     synonym.0       - number of synonyms defined
     synonym.1       - synonym definition of first synonym
     synonym.n       - synonym definition of nth synonym

TABKey
     Returns settings about behaviour of TAB key.
     tabkey.1 indicates behaviour while not in insert mode
     tabkey.2 indicates behaviour while in insert mode
     Set by <SET TABKEY>.
     (QEMS)

     tabkey.0        - 2
     tabkey.1        - TAB|CHARACTER
     tabkey.2        - TAB|CHARACTER

TABLine
     Returns details of if and where the <tab line> is displayed.
     Set by <SET TABLINE>.
     (QEMS)

     tabline.0       - 2
     tabline.1       - ON|OFF
     tabline.2       - line displaying tab line

TABS
     Returns settings about tab columns. Set by <SET TABS>.
     (QEMS)

     tabs.0          - 1
     tabs.1          - actual tab columns or "INCR n"

TABSIn
     Indicates if TABSIN processing is on or off and the size of the
     tabs setting. Set by <SET TABSIN>.
     (QEMS)

     tabsin.0        - 2
     tabsin.1        - ON|OFF
     tabsin.2        - size of tabs

TABSOut
     Indicates if TABSOUT processing is on or off and the size of the
     tabs setting. Set by <SET TABSOUT>.
     (QEMS)

     tabsout.0       - 2
     tabsout.1       - ON|OFF
     tabsout.2       - size of tabs

TARGETSAVE
     Indicates the type of targets saved for subsequent LOCATE commands.
     Set by <SET TARGETSAVE>.
     (QEMS)

     targetsave.0    - 1
     targetsave.1    - ALL|NONE|list of target types

TERMinal
     Identifies the terminal type currently being used.
     (QES)

     terminal.0      - 1
     terminal.1      - DOS|OS2|X11|WIN32|$TERM value under Unix

THIGHlight
     Indicates if text highlighting is on. ie a found string target is
     highlighted.
     Set by <SET THIGHLIGHT>
     (QEMS)

     thighlight.0    - 1
     thighlight.1    - ON|OFF

TIMECHECK
     Indicates the status of TIMECHECK for the current file.
     (QEMS)

     timecheck.0     - 1
     timecheck.1     - ON|OFF

TOF
     Indicates if the <current line> is on the <Top-of-File line>.
     (QES)

     tof.0           - 1
     tof.1           - ON|OFF

TOFEOF
     Indicates if the <Top-of-File line> and the <Bottom-of-File line>
     are displayed.
     (QEMS)

     tofeof.0        - 1
     tofeof.1        - ON|OFF

TRAILING
     Indicates how trailing blanks on lines are handled when the file
     is saved. Set by <SET TRAILING>
     (QEMS)

     trailing.0      - 1
     trailing.1      - ON|OFF|REMOVE|EMPTY|SINGLE

TYPEAhead
     Indicates if THE will wait until all keyboard input has been
     processed before updating the screen display. Set by
     <SET TYPEAHEAD>.
     (QEMS)

     typeahead.0     - 1
     typeahead.1     - ON|OFF

UNDOING
     Indicates if the undo facility is available or not.
     Set by <SET UNDOING>
     (QEMS)

     undoing.0       - 1
     undoing.1       - ON|OFF

UNTAA
     Indicates if "unsigned numbers are absolute".
     Set by <SET UNTAA>
     (QEMS)

     untaa.0         - 1
     untaa.1         - ON|OFF

UTF8
     Indicates if the UTF8 support is built in.
     (QES)

     utf8.0          - 1
     utf8.1          - ON|OFF

Verify
     Returns verify column settings. Set by <SET VERIFY>.
     (QEMS)

     verify.0        - 1
     verify.1        - Column pair of verify start and end columns.

VERShift
     Returns the value of the <vershift> internal variable.
     (QES)

     vershift.0      - 1
     vershift.1      - VERSHIFT value

VERSION
     Returns information about name of application (THE) and version
     information.
     (QES)

     version.0       - 5
     version.1       - THE
     version.2       - version string e.g. 1.5
     version.3       - platform version (DOS,OS2,UNIX,X11,WIN32,...)
     version.4       - version status information e.g. release date, beta
     version.5       - platform kernel (DOS,OS2,Linux,WIN32,...)

Width
     Returns maximum line width setting. Set by -w command line switch
     on starting THE.
     (QEMS)

     width.0         - 1
     width.1         - Maximum line width value.

WORD
     Specifies how THE defines a word. Set by <SET WORD>.
     (QEMS)

     word.0          - 1
     word.1          - ALPHANUM|NONBLANK

WORDWrap
     Indicates if WORDWRAP is on or off. Set by <SET WORDWRAP>.
     (QEMS)

     wordwrap.0      - 1
     wordwrap.1      - ON|OFF

WRap
     Indicates if WRAP is on or off. Set by <SET WRAP>.
     (QEMS)

     wrap.0          - 1
     wrap.1          - ON|OFF

XTERMinal
     Returns the current value of the X11 terminal program.
     Only applicable in X version. Set by <SET XTERMINAL>.
     (QEMS)

     xterminal.0     - 1
     xterminal.1     - X11 terminal program

Zone
     Returns zone column settings. Set by <SET ZONE>.
     (QEMS)

     zone.0          - 2
     zone.1          - Zone start column
     zone.2          - Zone end column



========================================================================
IMPLIED EXTRACT
========================================================================

     The above <REXX> variables set by the <EXTRACT> command may also
     be obtained by a REXX macro as an implied EXTRACT. Each variable
     above that may be set by an explicit EXTRACT command may also be
     e.g. The REXX commands:

         'EXTRACT /SIZE/CURLINE/'
         Say size.1 curline.1

     may be substituted with:

         Say size.1() curline.1()


========================================================================
BOOLEAN FUNCTIONS
========================================================================

     THE also provides other information to the REXX interpreter via
     boolean functions. These functions return either '1' or '0'
     depending on the information queried.

after()
     Returns '1' if the cursor is currently after the last non-blank
     character on the line, or if the line is blank.

altkey()
     Returns '1' if at the time the last key was pressed, the ALT
     key was also being held down.

alt()
     Returns '1' if the file being edited has changed since the
     last SAVE. i.e. if the value of alt.2 is non zero.

blank()
     Returns '1' if the line the <cursor field> is completely blank.

batch()
     Returns '1' if THE is being run in batch mode. ie THE was
     started with the -b switch.

block()
     Returns '1' if the marked <block> is within the current view.

before()
     Returns '1' if the cursor is currently before the first non-blank
     character on the line, or if the line is blank.

bottomedge()
     Returns '1' if the cursor is on the bottom edge of the <filearea>
     or <prefix area>.

command()
     Returns '1' if the <command line> is on. This is different to
     the definition in KEDIT; "Returns '1' if the cursor is on the
     command line." To get the equivalent KEDIT functionality
     use incommand();

ctrl()
     Returns '1' if at the time the last key was pressed, the CTRL
     key was also being held down.

current()
     Returns '1' if the cursor is on the <current line>.

dir()
     Returns '1' if the current file is the special DIR.DIR file.

end()
     Returns '1' if the cursor is on the last non-blank character on
     the line.

eof()
     Returns '1' if the cursor is on the <Bottom-of-File line>
     and the cursor is not on the <command line>.

first()
     Returns '1' if the cursor is in column 1 of the current window.

focuseof()
     Returns '1' if the focus line is the <Bottom-of-File line>
     whether the cursor is on it or not.

focustof()
     Returns '1' if the <focus line> is the <Top-of-File line>
     whether the cursor is on it or not.

inblock()
     Returns '1' if the cursor is in the marked <block>.

incommand()
     Returns '1' if the cursor is on the <command line>.

initial()
     Returns '1' if the function is called when THE is executing the <profile>
     against files specified on the Operating System command line. Once THE is
     ready to accept the first keystroke, not as a result of a command from within
     <profile>, this function returns 0;

inprefix()
     Returns '1' if the cursor is located in the <prefix area>.

insertmode()
     Returns '1' if THE is in INSERT mode.

leftedge()
     Returns '1' if the cursor is on the left edge of the <filearea>.

modifiable()
     Returns '1' if the cursor is located in an area that can be changed.
     i.e. not on <Top-of-File line> or <Bottom-of-File line> nor on a
     <shadow line>.

rightedge()
     Returns '1' if the cursor is on the right edge of the <filearea>.

shadow()
     Returns '1' if the cursor is on a <shadow line>.

shift()
     Returns '1' if at the time the last key was pressed, the SHIFT
     key was also being held down.

spacechar()
     Returns '1' if the cursor is on a space character.

tof()
     Returns '1' if the cursor is on the <Top-of-File line>
     and the cursor is not on the <command line>.

topedge()
     Returns '1' if the cursor is on the top edge of the <filearea>.

verone()
     Returns '1' if the column 1 of the file is being displayed in
     column 1.

========================================================================
OTHER FUNCTIONS
========================================================================

     The following functions provide features to simplify THE macros
     written in REXX.

valid_target(target[,anything])
     The first argument is the <target> to be validated. If a second,
     optional argument is supplied, the target to be validated can
     consist of a target followed by any optional characters. This can
     be useful if the arguments to a macro consist of a target followed
     by another argument. If a valid target is supplied, the remainder
     of the string passed to valid_target() is returned following the
     first line affected and the number of lines to the target.

     Returns 'ERROR' if the supplied target is invalid.
     Returns 'NOTFOUND' if the supplied target is valid, but not found.

     If a valid target, returns the first line affected by the target
     followed by the number of lines to the target, and optionally the
     remainder of the argument. e.g.

     if the focus line is 12 and valid_target() is called as

         result = valid_target(":7")      ===> result = "12 -5"

     if the focus line is 12 and valid_target is called as

         result = valid_target(":7 /fred/",junk), ===> result = "12 -5 /fred/"

run_os(command[,stdin_stem[,stdout_stem[,stderr_stem]]])
     This function allows the macro writer to call an operating system
     command and have the standard streams; 'stdin', 'stdout' and 'stderr'
     redirected to or from REXX arrays.

     The first argument is the operating system command to execute.
     The command can include any command line switches appropriate
     to the command.

     All other arguments comprise a stem name (including a trailing '.')
     which refers to the REXX arrays where 'stdin', 'stdout' and 'stderr'
     are to be redirected.

     As with all REXX arrays, the value of the 0th element (stem.0)
     contains the number of elements in the array.

     The only restriction with the names of the stem variables is
     that the stem name for the 'stdin' stem cannot be the same as the
     stem for 'stdout' or 'stderr'.

     The stem name for 'stdout' and 'stderr' can be the same; the contents
     of the resulting output stems will consist of 'stdout' and 'stderr' in
     the order that the command generates this output.

     Return values:

          0    - successful
          1005 - invalid argument or syntax
          1012 - problems with system redirection of streams
          1094 - out of memory
          1099 - error interfacing to REXX interpreter

     all other numbers, return code from operating system command

     e.g. to spell check the words "The Hessling Editr" with ispell

         in.0 = 3
         in.1 = "The"
         in.2 = "Hessling"
         in.3 = "Editr"
         rc = run_os("ispell -a","in.","out.")

     sets:

          out.0 --> 3
          out.1 --> "*"
          out.2 --> "#"
          out.3 --> "& edits edit editor"

**man-end**********************************************************************/


/***********************************************************************/
/* Keep the following items in alphabetic order of name.               */
/***********************************************************************/
QUERY_ITEM _THE_FAR query_item[] =
{
   {(CHARTYPE *)"alt",             3, 3,ITEM_ALT,           2, 2,          LVL_FILE,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_alt                    },
   {(CHARTYPE *)"arbchar",         7, 3,ITEM_ARBCHAR,       3, 3,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_arbchar                },
   {(CHARTYPE *)"autocolor",       9, 9,ITEM_AUTOCOLOR,     3, 0,          LVL_FILE,QUERY_QUERY             |QUERY_EXTRACT             ,extract_autocolor              },
   {(CHARTYPE *)"autocolour",     10,10,ITEM_AUTOCOLOUR,    3, 0,          LVL_FILE,QUERY_QUERY             |QUERY_EXTRACT             ,extract_autocolour             },
   {(CHARTYPE *)"autosave",        8, 2,ITEM_AUTOSAVE,      1, 1,          LVL_FILE,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_autosave               },
   {(CHARTYPE *)"autoscroll",     10, 6,ITEM_AUTOSCROLL,    1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_autoscroll             },
   {(CHARTYPE *)"backup",          6, 4,ITEM_BACKUP,        2, 2,          LVL_FILE,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_backup                 },
   {(CHARTYPE *)"beep",            4, 4,ITEM_BEEP,          1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_beep                   },
   {(CHARTYPE *)"block",           5, 5,ITEM_BLOCK,         0, 6,          LVL_VIEW,QUERY_QUERY             |QUERY_EXTRACT             ,extract_block                  },
   {(CHARTYPE *)"case",            4, 4,ITEM_CASE,          6, 6,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_case                   },
   {(CHARTYPE *)"clearerrorkey",  13, 6,ITEM_CLEARERRORKEY, 1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_clearerrorkey          },
   {(CHARTYPE *)"clearscreen",    11, 6,ITEM_CLEARSCREEN,   1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_clearscreen            },
   {(CHARTYPE *)"clock",           5, 5,ITEM_CLOCK,         1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_clock                  },
   {(CHARTYPE *)"cmdarrows",       9, 4,ITEM_CMDARROWS,     1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_cmdarrows              },
   {(CHARTYPE *)"cmdline",         7, 3,ITEM_CMDLINE,       2, 3,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_cmdline                },
   {(CHARTYPE *)"color",           5, 5,ITEM_COLOR,         1, ATTR_MAX,   LVL_FILE,QUERY_QUERY             |QUERY_EXTRACT             ,extract_color                  },
   {(CHARTYPE *)"coloring",        8, 8,ITEM_COLORING,      3, 3,          LVL_FILE,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_coloring               },
   {(CHARTYPE *)"colour",          6, 5,ITEM_COLOUR,        1, ATTR_MAX,   LVL_FILE,QUERY_QUERY             |QUERY_EXTRACT             ,extract_colour                 },
   {(CHARTYPE *)"colouring",       9, 9,ITEM_COLOURING,     3, 3,          LVL_FILE,QUERY_QUERY             |QUERY_EXTRACT|QUERY_MODIFY,extract_colouring              },
   {(CHARTYPE *)"column",          6, 3,ITEM_COLUMN,        1, 1,          LVL_VIEW,QUERY_QUERY             |QUERY_EXTRACT             ,extract_column                 },
   {(CHARTYPE *)"compat",          6, 6,ITEM_COMPAT,        3, 3,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_compat                 },
   {(CHARTYPE *)"ctlchar",         7, 3,ITEM_CTLCHAR,       3, 3,          LVL_GLOB,QUERY_QUERY             |QUERY_EXTRACT             ,extract_ctlchar                },
   {(CHARTYPE *)"curline",         7, 3,ITEM_CURLINE,       6, 6,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_curline                },
   {(CHARTYPE *)"cursor",          6, 4,ITEM_CURSOR,        8, 8,          LVL_VIEW,QUERY_QUERY             |QUERY_EXTRACT             ,extract_cursor                 },
   {(CHARTYPE *)"cursorstay",     10, 8,ITEM_CURSORSTAY,    1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_cursorstay             },
   {(CHARTYPE *)"define",          6, 6,ITEM_DEFINE,        1, 0,          LVL_GLOB,QUERY_QUERY             |QUERY_EXTRACT             ,extract_define                 },
   {(CHARTYPE *)"defsort",         7, 7,ITEM_DEFSORT,       2, 2,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_defsort                },
   {(CHARTYPE *)"dirfileid",       9, 9,ITEM_DIRFILEID,     2, 2,          LVL_FILE,                         QUERY_EXTRACT             ,extract_dirfileid              },
   {(CHARTYPE *)"dirinclude",     10, 4,ITEM_DIRINCLUDE,    1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_dirinclude             },
   {(CHARTYPE *)"display",         7, 4,ITEM_DISPLAY,       2, 2,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_display                },
   {(CHARTYPE *)"ecolor",          6, 6,ITEM_ECOLOR,        2, ECOLOUR_MAX,LVL_VIEW,QUERY_QUERY|             QUERY_EXTRACT             ,extract_ecolor                 },
   {(CHARTYPE *)"ecolour",         7, 7,ITEM_ECOLOUR,       2, ECOLOUR_MAX,LVL_VIEW,QUERY_QUERY|             QUERY_EXTRACT             ,extract_ecolour                },
   {(CHARTYPE *)"efileid",         7, 6,ITEM_EFILEID,       2, 2,          LVL_FILE,QUERY_QUERY|             QUERY_EXTRACT             ,extract_efileid                },
   {(CHARTYPE *)"eof",             3, 3,ITEM_EOF,           1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT             ,extract_eof                    },
   {(CHARTYPE *)"eolout",          6, 3,ITEM_EOLOUT,        1, 1,          LVL_FILE,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_eolout                 },
   {(CHARTYPE *)"equivchar",       9, 6,ITEM_EQUIVCHAR,     1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_equivchar              },
   {(CHARTYPE *)"erroroutput",    11, 8,ITEM_ERROROUTPUT,   1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_erroroutput            },
   {(CHARTYPE *)"etmode",          6, 6,ITEM_ETMODE,        2, 2,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_etmode                 },
   {(CHARTYPE *)"fext",            4, 2,ITEM_FEXT,          1, 1,          LVL_FILE,QUERY_QUERY|             QUERY_EXTRACT|QUERY_MODIFY,extract_ftype                  },
   {(CHARTYPE *)"field",           5, 5,ITEM_FIELD,         4, 4,          LVL_VIEW,QUERY_QUERY|             QUERY_EXTRACT             ,extract_field                  },
   {(CHARTYPE *)"fieldword",       9, 9,ITEM_FIELDWORD,     3, 3,          LVL_VIEW,                         QUERY_EXTRACT             ,extract_fieldword              },
   {(CHARTYPE *)"filename",        8, 5,ITEM_FILENAME,      1, 1,          LVL_FILE,QUERY_QUERY|             QUERY_EXTRACT|QUERY_MODIFY,extract_filename               },
   {(CHARTYPE *)"filestatus",     10,10,ITEM_FILESTATUS,    3, 3,          LVL_FILE,QUERY_QUERY|             QUERY_EXTRACT|QUERY_MODIFY,extract_filestatus             },
   {(CHARTYPE *)"filetabs",        8, 8,ITEM_FILETABS,      1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_filetabs               },
   {(CHARTYPE *)"fmode",           5, 2,ITEM_FMODE,         1, 1,          LVL_FILE,QUERY_QUERY|             QUERY_EXTRACT|QUERY_MODIFY,extract_fmode                  },
   {(CHARTYPE *)"fname",           5, 2,ITEM_FNAME,         1, 1,          LVL_FILE,QUERY_QUERY|             QUERY_EXTRACT|QUERY_MODIFY,extract_fname                  },
   {(CHARTYPE *)"fpath",           5, 2,ITEM_FPATH,         1, 1,          LVL_FILE,QUERY_QUERY|             QUERY_EXTRACT|QUERY_MODIFY,extract_fpath                  },
   {(CHARTYPE *)"ftype",           5, 2,ITEM_FTYPE,         1, 1,          LVL_FILE,QUERY_QUERY|             QUERY_EXTRACT|QUERY_MODIFY,extract_ftype                  },
   {(CHARTYPE *)"fullfname",       9, 6,ITEM_FULLFNAME,     1, 1,          LVL_FILE,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_fullfname              },
   {(CHARTYPE *)"getenv",          6, 6,ITEM_GETENV,        1, 1,          LVL_GLOB,                         QUERY_EXTRACT             ,extract_getenv                 },
   {(CHARTYPE *)"header",          4, 3,ITEM_HEADER,       11,11,          LVL_GLOB,QUERY_QUERY|             QUERY_EXTRACT             ,extract_header                 },
   {(CHARTYPE *)"hex",             3, 3,ITEM_HEX,           1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_hex                    },
   {(CHARTYPE *)"hexdisplay",     10, 7,ITEM_HEXDISPLAY,    1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_hexdisplay             },
   {(CHARTYPE *)"hexshow",         7, 4,ITEM_HEXSHOW,       2, 2,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_hexshow                },
   {(CHARTYPE *)"highlight",       9, 4,ITEM_HIGHLIGHT,     1, 3,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_highlight              },
   {(CHARTYPE *)"idline",          6, 2,ITEM_IDLINE,        1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_idline                 },
   {(CHARTYPE *)"impmacro",        8, 6,ITEM_IMPMACRO,      1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_impmacro               },
   {(CHARTYPE *)"impos",           5, 5,ITEM_IMPOS,         1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_impos                  },
   {(CHARTYPE *)"inputmode",       9, 6,ITEM_INPUTMODE,     1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_inputmode              },
   {(CHARTYPE *)"insertmode",     10, 6,ITEM_INSERTMODE,    1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_insertmode             },
   {(CHARTYPE *)"interface",       9, 3,ITEM_INTERFACE,     1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_interface              },
   {(CHARTYPE *)"lastkey",         7, 7,ITEM_LASTKEY,       4, 4,          LVL_GLOB,                         QUERY_EXTRACT             ,extract_lastkey                },
   {(CHARTYPE *)"lastmsg",         7, 4,ITEM_LASTMSG,       1, 1,          LVL_GLOB,                         QUERY_EXTRACT             ,extract_lastmsg                },
   {(CHARTYPE *)"lastop",          6, 6,ITEM_LASTOP,        9, 9,          LVL_GLOB,QUERY_QUERY             |QUERY_EXTRACT             ,extract_lastop                 },
   {(CHARTYPE *)"lastrc",          6, 6,ITEM_LASTRC,        1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT             ,extract_lastrc                 },
   {(CHARTYPE *)"length",          6, 3,ITEM_LENGTH,        1, 1,          LVL_FILE,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT             ,extract_length                 },
   {(CHARTYPE *)"line",            4, 2,ITEM_LINE,          1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT             ,extract_line                   },
   {(CHARTYPE *)"lineflag",        8, 8,ITEM_LINEFLAG,      3, 3,          LVL_FILE,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_lineflag               },
   {(CHARTYPE *)"linend",          6, 5,ITEM_LINEND,        2, 2,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_linend                 },
   {(CHARTYPE *)"lscreen",         7, 2,ITEM_LSCREEN,       6, 6,          LVL_GLOB,QUERY_QUERY             |QUERY_EXTRACT             ,extract_lscreen                },
   {(CHARTYPE *)"macro",           5, 5,ITEM_MACRO,         1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_macro                  },
   {(CHARTYPE *)"macroext",        8, 6,ITEM_MACROEXT,      1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_macroext               },
   {(CHARTYPE *)"macropath",       9, 6,ITEM_MACROPATH,     1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_macropath              },
   {(CHARTYPE *)"margins",         7, 3,ITEM_MARGINS,       3, 3,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_margins                },
   {(CHARTYPE *)"monitor",         7, 7,ITEM_MONITOR,       2, 2,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT             ,extract_monitor                },
   {(CHARTYPE *)"mouse",           5, 5,ITEM_MOUSE,         1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_mouse                  },
   {(CHARTYPE *)"msgline",         7, 4,ITEM_MSGLINE,       4, 4,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_msgline                },
   {(CHARTYPE *)"msgmode",         7, 4,ITEM_MSGMODE,       2, 2,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_msgmode                },
   {(CHARTYPE *)"nbfile",          6, 3,ITEM_NBFILE,        1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT             ,extract_nbfile                 },
   {(CHARTYPE *)"nbscope",         7, 3,ITEM_NBSCOPE,       2, 2,          LVL_VIEW,QUERY_QUERY             |QUERY_EXTRACT             ,extract_nbscope                },
   {(CHARTYPE *)"newlines",        8, 4,ITEM_NEWLINES,      1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_newlines               },
   {(CHARTYPE *)"nondisp",         7, 4,ITEM_NONDISP,       1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_nondisp                },
   {(CHARTYPE *)"number",          6, 3,ITEM_NUMBER,        1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_number                 },
   {(CHARTYPE *)"pagewrap",        8, 8,ITEM_PAGEWRAP,      1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_pagewrap               },
   {(CHARTYPE *)"parser",          6, 6,ITEM_PARSER,        5, 0,          LVL_FILE,QUERY_QUERY             |QUERY_EXTRACT             ,extract_parser                 },
   {(CHARTYPE *)"pending",         7, 4,ITEM_PENDING,       7, 1,          LVL_VIEW,                         QUERY_EXTRACT             ,extract_pending                },
   {(CHARTYPE *)"point",           5, 1,ITEM_POINT,         1, 1,          LVL_VIEW,QUERY_QUERY             |QUERY_EXTRACT             ,extract_point                  },
   {(CHARTYPE *)"position",        8, 3,ITEM_POSITION,      3, 3,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_position               },
   {(CHARTYPE *)"prefix",          6, 3,ITEM_PREFIX,        0, 4,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_prefix                 },
   {(CHARTYPE *)"printer",         7, 7,ITEM_PRINTER,       1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_printer                },
   {(CHARTYPE *)"profile",         4, 7,ITEM_PROFILE,       1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT             ,extract_profile                },
   {(CHARTYPE *)"pscreen",         7, 4,ITEM_PSCREEN,       2, 2,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_pscreen                },
   {(CHARTYPE *)"readonly",        8, 8,ITEM_READONLY,      1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_readonly               },
   {(CHARTYPE *)"readv",           5, 5,ITEM_READV,         4, 1,          LVL_GLOB,QUERY_READV                                        ,extract_readv                  },
   {(CHARTYPE *)"regexp",          6, 6,ITEM_REGEXP,        1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_regexp                 },
   {(CHARTYPE *)"reprofile",       9, 6,ITEM_REPROFILE,     1, 1,          LVL_FILE,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_reprofile              },
   {(CHARTYPE *)"reserved",        8, 5,ITEM_RESERVED,      1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT             ,extract_reserved               },
   {(CHARTYPE *)"rexx",            4, 4,ITEM_REXX,          1, 1,          LVL_GLOB,QUERY_QUERY                                        ,extract_rexx                   },
   {(CHARTYPE *)"rexxhalt",        5, 5,ITEM_REXXHALT,      2, 2,          LVL_GLOB,QUERY_QUERY             |QUERY_EXTRACT             ,extract_rexxhalt               },
   {(CHARTYPE *)"rexxoutput",     10, 7,ITEM_REXXOUTPUT,    2, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_rexxoutput             },
   {(CHARTYPE *)"ring",            4, 4,ITEM_RING,          1, 0,          LVL_GLOB,QUERY_QUERY             |QUERY_EXTRACT             ,extract_ring                   },
   {(CHARTYPE *)"scale",           5, 4,ITEM_SCALE,         2, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_scale                  },
   {(CHARTYPE *)"scope",           5, 5,ITEM_SCOPE,         1, 2,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_scope                  },
   {(CHARTYPE *)"screen",          6, 3,ITEM_SCREEN,        2, 2,          LVL_FILE,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_screen                 },
   {(CHARTYPE *)"select",          6, 3,ITEM_SELECT,        2, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT             ,extract_select                 },
   {(CHARTYPE *)"shadow",          6, 4,ITEM_SHADOW,        1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_shadow                 },
   {(CHARTYPE *)"showkey",         7, 4,ITEM_SHOWKEY,       1, 1,          LVL_FILE,                         QUERY_EXTRACT             ,extract_showkey                },
   {(CHARTYPE *)"size",            4, 2,ITEM_SIZE,          1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT             ,extract_size                   },
   {(CHARTYPE *)"slk",             3, 3,ITEM_SLK,           2, 0,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_slk                    },
   {(CHARTYPE *)"statopt",         7, 7,ITEM_STATOPT,       1, 1,          LVL_GLOB,QUERY_QUERY             |QUERY_EXTRACT             ,extract_statopt                },
   {(CHARTYPE *)"statusline",     10, 7,ITEM_STATUSLINE,    1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_statusline             },
   {(CHARTYPE *)"stay",            4, 4,ITEM_STAY,          1, 2,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_stay                   },
   {(CHARTYPE *)"synelem",         7, 4,ITEM_SYNELEM,       1, 0,          LVL_VIEW,QUERY_QUERY             |QUERY_EXTRACT             ,extract_synelem                },
   {(CHARTYPE *)"synonym",         7, 3,ITEM_SYNONYM,       1, 0,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_synonym                },
   {(CHARTYPE *)"tabkey",          6, 4,ITEM_TABKEY,        2, 2,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_tabkey                 },
   {(CHARTYPE *)"tabline",         7, 4,ITEM_TABLINE,       2, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_tabline                },
   {(CHARTYPE *)"tabs",            4, 4,ITEM_TABS,          1, 2,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_tabs                   },
   {(CHARTYPE *)"tabsin",          6, 5,ITEM_TABSIN,        2, 2,          LVL_FILE,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_tabsin                 },
   {(CHARTYPE *)"tabsout",         7, 5,ITEM_TABSOUT,       2, 2,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_tabsout                },
   {(CHARTYPE *)"targetsave",     10,10,ITEM_TARGETSAVE,    1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_targetsave             },
   {(CHARTYPE *)"terminal",        8, 4,ITEM_TERMINAL,      1, 1,          LVL_FILE,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT             ,extract_terminal               },
   {(CHARTYPE *)"thighlight",      5, 5,ITEM_THIGHLIGHT,    1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_thighlight             },
   {(CHARTYPE *)"timecheck",       9, 9,ITEM_TIMECHECK,     1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_timecheck              },
   {(CHARTYPE *)"tof",             3, 3,ITEM_TOF,           1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT             ,extract_tof                    },
   {(CHARTYPE *)"tofeof",          6, 6,ITEM_TOFEOF,        1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_tofeof                 },
   {(CHARTYPE *)"trailing",        8, 8,ITEM_TRAILING,      1, 1,          LVL_FILE,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_trailing               },
   {(CHARTYPE *)"typeahead",       9, 5,ITEM_TYPEAHEAD,     1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_typeahead              },
   {(CHARTYPE *)"ui",              2, 2,ITEM_UI,            1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT             ,extract_ui                     },
   {(CHARTYPE *)"undoing",         7, 7,ITEM_UNDOING,       1, 1,          LVL_FILE,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_undoing                },
   {(CHARTYPE *)"untaa",           5, 5,ITEM_UNTAA,         1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_untaa                  },
   {(CHARTYPE *)"utf8",            4, 4,ITEM_UTF8,          1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT             ,extract_utf8                   },
   {(CHARTYPE *)"verify",          6, 1,ITEM_VERIFY,        1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_verify                 },
   {(CHARTYPE *)"vershift",        8, 4,ITEM_VERSHIFT,      1, 1,          LVL_VIEW,QUERY_QUERY             |QUERY_EXTRACT             ,extract_vershift               },
   {(CHARTYPE *)"version",         7, 7,ITEM_VERSION,       5, 5,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT             ,extract_version                },
   {(CHARTYPE *)"width",           5, 1,ITEM_WIDTH,         1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_width                  },
   {(CHARTYPE *)"word",            4, 4,ITEM_WORD,          1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_word                   },
   {(CHARTYPE *)"wordwrap",        8, 5,ITEM_WORDWRAP,      1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_wordwrap               },
   {(CHARTYPE *)"wrap",            4, 2,ITEM_WRAP,          1, 1,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_wrap                   },
   {(CHARTYPE *)"xterminal",       9, 5,ITEM_XTERMINAL,     1, 1,          LVL_GLOB,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_xterminal              },
   {(CHARTYPE *)"zone",            4, 1,ITEM_ZONE,          2, 2,          LVL_VIEW,QUERY_QUERY|QUERY_STATUS|QUERY_EXTRACT|QUERY_MODIFY,extract_zone                   },
};
#define NUMBER_QUERY_ITEM ( sizeof( query_item ) / sizeof( QUERY_ITEM ) )

QUERY_ITEM _THE_FAR function_item[] =
{
   {(CHARTYPE *)"after",           5, 5,ITEM_AFTER_FUNCTION,         0, 1,LVL_VIEW,0,extract_after_function         },
   {(CHARTYPE *)"alt",             3, 3,ITEM_ALT_FUNCTION,           0, 1,LVL_FILE,0,extract_alt_function           },
   {(CHARTYPE *)"altkey",          6, 6,ITEM_ALTKEY_FUNCTION,        0, 1,LVL_GLOB,0,extract_altkey_function        },
   {(CHARTYPE *)"batch",           5, 5,ITEM_BATCH_FUNCTION,         0, 1,LVL_GLOB,0,extract_batch_function         },
   {(CHARTYPE *)"before",          6, 6,ITEM_BEFORE_FUNCTION,        0, 1,LVL_VIEW,0,extract_before_function        },
   {(CHARTYPE *)"blank",           5, 5,ITEM_BLANK_FUNCTION,         0, 1,LVL_VIEW,0,extract_blank_function         },
   {(CHARTYPE *)"block",           5, 5,ITEM_BLOCK_FUNCTION,         0, 1,LVL_VIEW,0,extract_block_function         },
   {(CHARTYPE *)"bottomedge",     10,10,ITEM_BOTTOMEDGE_FUNCTION,    0, 1,LVL_VIEW,0,extract_bottomedge_function    },
   {(CHARTYPE *)"command",         7, 7,ITEM_COMMAND_FUNCTION,       0, 1,LVL_VIEW,0,extract_command_function       },
   {(CHARTYPE *)"ctrl",            4, 4,ITEM_CTRL_FUNCTION,          0, 1,LVL_GLOB,0,extract_ctrl_function          },
   {(CHARTYPE *)"current",         7, 7,ITEM_CURRENT_FUNCTION,       0, 1,LVL_VIEW,0,extract_current_function       },
   {(CHARTYPE *)"dir",             3, 3,ITEM_DIR_FUNCTION,           0, 1,LVL_FILE,0,extract_dir_function           },
   {(CHARTYPE *)"end",             3, 3,ITEM_END_FUNCTION,           0, 1,LVL_VIEW,0,extract_end_function           },
   {(CHARTYPE *)"eof",             3, 3,ITEM_EOF_FUNCTION,           0, 1,LVL_VIEW,0,extract_eof_function           },
   {(CHARTYPE *)"first",           5, 5,ITEM_FIRST_FUNCTION,         0, 1,LVL_VIEW,0,extract_first_function         },
   {(CHARTYPE *)"focuseof",        8, 8,ITEM_FOCUSEOF_FUNCTION,      0, 1,LVL_VIEW,0,extract_focuseof_function      },
   {(CHARTYPE *)"focustof",        8, 8,ITEM_FOCUSTOF_FUNCTION,      0, 1,LVL_VIEW,0,extract_focustof_function      },
   {(CHARTYPE *)"inblock",         7, 7,ITEM_INBLOCK_FUNCTION,       0, 1,LVL_VIEW,0,extract_inblock_function       },
   {(CHARTYPE *)"incommand",       9, 9,ITEM_INCOMMAND_FUNCTION,     0, 1,LVL_VIEW,0,extract_incommand_function     },
   {(CHARTYPE *)"initial",         7, 7,ITEM_INITIAL_FUNCTION,       0, 1,LVL_GLOB,0,extract_initial_function       },
   {(CHARTYPE *)"inprefix",        8, 8,ITEM_INPREFIX_FUNCTION,      0, 1,LVL_VIEW,0,extract_inprefix_function      },
   {(CHARTYPE *)"insertmode",     10,10,ITEM_INSERTMODE_FUNCTION,    0, 1,LVL_GLOB,0,extract_inprefix_function      },
   {(CHARTYPE *)"leftedge",        8, 8,ITEM_LEFTEDGE_FUNCTION,      0, 1,LVL_VIEW,0,extract_leftedge_function      },
   {(CHARTYPE *)"modifiable",     10,10,ITEM_MODIFIABLE_FUNCTION,    0, 1,LVL_VIEW,0,extract_modifiable_function    },
   {(CHARTYPE *)"rightedge",       9, 9,ITEM_RIGHTEDGE_FUNCTION,     0, 1,LVL_VIEW,0,extract_rightedge_function     },
   {(CHARTYPE *)"run_os",          6, 6,ITEM_RUN_OS_FUNCTION,        0, 1,LVL_GLOB,0,NULL                           },
   {(CHARTYPE *)"shadow",          6, 6,ITEM_SHADOW_FUNCTION,        0, 1,LVL_VIEW,0,extract_shadow_function        },
   {(CHARTYPE *)"shift",           5, 5,ITEM_SHIFT_FUNCTION,         0, 1,LVL_GLOB,0,extract_shift_function         },
   {(CHARTYPE *)"spacechar",       9, 9,ITEM_SPACECHAR_FUNCTION,     0, 1,LVL_VIEW,0,extract_spacechar_function     },
   {(CHARTYPE *)"tof",             3, 3,ITEM_TOF_FUNCTION,           0, 1,LVL_VIEW,0,extract_tof_function           },
   {(CHARTYPE *)"topedge",         7, 7,ITEM_TOPEDGE_FUNCTION,       0, 1,LVL_VIEW,0,extract_topedge_function       },
   {(CHARTYPE *)"valid_target",   12,12,ITEM_VALID_TARGET_FUNCTION,  0, 1,LVL_VIEW,0,NULL                           },
   {(CHARTYPE *)"verone",          6, 6,ITEM_VERONE_FUNCTION,        0, 1,LVL_VIEW,0,extract_verone_function        },
};
#define NUMBER_FUNCTION_ITEM ( sizeof( function_item ) / sizeof( QUERY_ITEM ) )

CHARTYPE _THE_FAR *block_name[] = {
                          (CHARTYPE *)"",
                          (CHARTYPE *)"LINE",
                          (CHARTYPE *)"BOX",
                          (CHARTYPE *)"STREAM",
                          (CHARTYPE *)"COLUMN",
                          (CHARTYPE *)"WORD",
                          (CHARTYPE *)"CUA"
                         };

VALUE item_values[MAX_VARIABLES_RETURNED];

/***********************************************************************/
#ifdef HAVE_PROTO
THE_PPC *in_range( THE_PPC *found_ppc, THE_PPC *curr_ppc, LINETYPE first_in_range, LINETYPE last_in_range )
#else
THE_PPC *in_range( found_ppc, curr_ppc, first_in_range, last_in_range )
THE_PPC *found_ppc, *curr_ppc;
LINETYPE first_in_range, last_in_range;
#endif
/***********************************************************************/
{
   if ( found_ppc == NULL )
   {
      if ( curr_ppc->ppc_line_number >= first_in_range
      &&   curr_ppc->ppc_line_number <= last_in_range )
         found_ppc = curr_ppc;
   }
   else
   {
      if ( curr_ppc->ppc_line_number < found_ppc->ppc_line_number
      &&   curr_ppc->ppc_line_number >= first_in_range
      &&   curr_ppc->ppc_line_number <= last_in_range )
         found_ppc = curr_ppc;
   }
   return found_ppc;
}

/***********************************************************************/
#ifdef HAVE_PROTO
void set_key_values(int key, bool mouse_key)
#else
void set_key_values(key, mouse_key)
int key;
bool mouse_key;
#endif
/***********************************************************************/
{
   CHARTYPE *keyname=NULL;
   int shift=0;
   int mod_shift, mod_ctrl, mod_alt;

#if defined(PDCURSES_MOUSE_ENABLED) || defined(NCURSES_MOUSE_VERSION)
   if ( mouse_key )
   {
      int b,ba,bm,w;
      CHARTYPE scrn;
      if ( get_mouse_info( &b, &ba, &bm ) == RC_OK )
      {
         which_window_is_mouse_in( &scrn, &w );
         key = mouse_info_to_key( w, b, ba, bm );
         item_values[1].value = mouse_key_number_to_name( key, query_rsrvd, &shift );
         item_values[1].len = strlen( (DEFCHAR *)item_values[1].value );
      }
      mod_shift = SHIFT_MOUSE_SHIFT;
      mod_ctrl = SHIFT_MOUSE_CTRL;
      mod_alt = SHIFT_MOUSE_ALT;
   }
   else
#endif
   {
      keyname = get_key_name( key, &shift );
      if (keyname == NULL)
      {
         item_values[1].value = (CHARTYPE *)"";
         item_values[1].len = 0;
      }
      else
      {
         item_values[1].value = keyname;
         item_values[1].len = strlen( (DEFCHAR *)keyname );
      }
      mod_shift = SHIFT_SHIFT;
      mod_ctrl = SHIFT_CTRL;
      mod_alt = SHIFT_ALT;
   }
   if (key < 256
   &&  key >= 0)
   {
      sprintf((DEFCHAR *)query_num1,"%d",key);
      item_values[2].value = query_num1;
      item_values[2].len = strlen( (DEFCHAR *)query_num1 );
   }
   else
   {
      item_values[2].value = (CHARTYPE *)"";
      item_values[2].len = 0;
   }
   sprintf( (DEFCHAR *)query_num2, "%d", key );
   item_values[3].value = query_num2;
   item_values[3].len = strlen( (DEFCHAR *)query_num2 );
   memset( (DEFCHAR *)query_num3, '0', 8 );
   query_num3[8] = '\0';
   if ( key != -1 )
   {
      if ( INSERTMODEx )
         query_num3[0] = '1';
      if ( shift & mod_alt )
         query_num3[4] = '1';
      if ( shift & mod_ctrl )
         query_num3[5] = '1';
      if ( shift & mod_shift )
         query_num3[6] = query_num3[7] = '1';
   }
   item_values[4].value = query_num3;
   item_values[4].len = 8;
   return;
}

/***********************************************************************/
#ifdef HAVE_PROTO
short set_boolean_value(bool flag, short num)
#else
short set_boolean_value(flag, num)
bool flag;
short num;
#endif
/***********************************************************************/
{
   if (flag)
   {
      item_values[num].value = (CHARTYPE *)"1";
      item_values[num].len = 1;
   }
   else
   {
      item_values[num].value = (CHARTYPE *)"0";
      item_values[num].len = 1;
   }
   return 1;
}

/***********************************************************************/
#ifdef HAVE_PROTO
short set_on_off_value(bool flag, short num)
#else
short set_on_off_value(flag, num)
bool flag;
short num;
#endif
/***********************************************************************/
{
   if (flag)
   {
      item_values[num].value = (CHARTYPE *)"ON";
      item_values[num].len = 2;
   }
   else
   {
      item_values[num].value = (CHARTYPE *)"OFF";
      item_values[num].len = 3;
   }
   return 1;
}

/***********************************************************************/
#ifdef HAVE_PROTO
short find_query_item(CHARTYPE *item_name,int len,CHARTYPE *query_type)
#else
short find_query_item(item_name,len,query_type)
CHARTYPE *item_name;
int len;
CHARTYPE *query_type;
#endif
/***********************************************************************/
{
   int rc=0;

   TRACE_FUNCTION("query.c   :find_query_item");
   /*
    * Using the supplied abbreviation for an item, find the entry in the
    * query_item[] array. It should return with the index pointer (for
    * QUERY_EXTRACT) or an index to the first item in the function_item[]
    * rexx.c for other query types.
    */
   set_compare_exact( FALSE );
   rc = search_query_item_array( query_item,
                                 number_query_item(),
                                 sizeof( QUERY_ITEM ),
                                 (DEFCHAR *)item_name,
                                 len);
   if (rc != (-1))
      *query_type = query_item[rc].query;
   TRACE_RETURN();
   return(rc);
}
/***********************************************************************/
#ifdef HAVE_PROTO
short show_status(void)
#else
short show_status()
#endif
/***********************************************************************/
{
#define STATUS_COLS 6
   register short i=0,j=0,k=0;
   short lineno=0,colno=0;
   short number_variables=0;
   short item_width=0,column=0,column_width=0,col[STATUS_COLS];

   TRACE_FUNCTION("query.c   :show_status");
   /*
    * For each item that is displayable, display it...
    */
   column = 0;
   column_width = COLS / STATUS_COLS;
   col[0] = 0;
   for (i=1;i<STATUS_COLS;i++)
      col[i] = col[i-1]+column_width+1;
   wclear(stdscr);
   for (i=0; i<NUMBER_QUERY_ITEM; i++)
   {
      /*
       * Get only those settings that are queryable...
       */
      if (query_item[i].query & QUERY_STATUS)
      {
         number_variables = get_item_values(1,query_item[i].item_number,(CHARTYPE *)"",QUERY_STATUS,0L,NULL,0L);
         item_width = 0;
         /*
          * Obtain the total length of the setting values...
          */
         for (j=0;j<number_variables+1;j++)
            item_width += item_values[j].len+1;
         item_width--;     /* reduce by 1 for last value's blank at end */
         /*
          * If the length of the variables is > the screen width, go to next
          * line.
          */
         if (item_width+col[column] > COLS)
         {
            column = colno = 0;
            lineno++;
         }
         else
            colno = col[column];
         /*
          * Display the variables. For the first value, display in BOLD.
          */
         for (j=0;j<number_variables+1;j++)
         {
            if (j == 0)
               attrset(A_BOLD);
            mvaddstr(lineno,colno,(DEFCHAR *)item_values[j].value);
            attrset(A_NORMAL);
            colno += item_values[j].len+1;
         }
         colno--;
         /*
          * Now have to determine where to display the next set of variables...
          * If the just-displayed values go past the last column, or we are
          * already in the last column, go to the next line...
          */
         if (colno >= col[STATUS_COLS-1] || column == STATUS_COLS-1)
         {
            lineno++;
            column = 0;
         }
         else
         {
            /*
             * ...else if the the just-displayed values go past the 2nd column...
             * If the just-displayed values go past the last column, go on to the
             * next line...
             */
            for (k=0;k<STATUS_COLS;k++)
            {
               if (colno < col[k])
               {
                  column = k;
                  break;
               }
            }
         }
      }
   }
   mvaddstr(terminal_lines-2,0,HIT_ANY_KEY);
   refresh();

   TRACE_RETURN();
   return(RC_OK);
}
/***********************************************************************/
#ifdef HAVE_PROTO
short save_status(CHARTYPE *filename)
#else
short save_status(filename)
CHARTYPE *filename;
#endif
/***********************************************************************/
{
   register short i=0,j=0;
   short number_variables=0,rc=RC_OK;
   FILE *fp=NULL;

   TRACE_FUNCTION("query.c   :save_status");
   /*
    * Get the fully qualified filename from the supplied filename.
    */
   strrmdup( strtrans( filename, OSLASH,ISLASH ), ISLASH, TRUE );
   if ( ( rc = splitpath( filename ) ) != RC_OK )
   {
      display_error( 10, filename, FALSE );
      TRACE_RETURN();
      return(rc);
   }
   /*
    * splitpath() has set up sp_path  and sp_fname. Append the filename.
    */
   strcat( (DEFCHAR *)sp_path, (DEFCHAR *)sp_fname );
   /*
    * If the file already exists, display an error.
    */
   if ( file_exists( sp_path ) == THE_FILE_EXISTS )
   {
      display_error( 8, filename, FALSE );
      TRACE_RETURN();
      return(rc);
   }
   fp = fopen( (DEFCHAR *)sp_path, "w" );
   /*
    * For each item that is modifiable, get its value...
    */
   fprintf( fp, "/* This file generated by STATUS command of THE %s %s*/\n", the_version, the_release );
   for ( i = 0; i < NUMBER_QUERY_ITEM; i++ )
   {
      /*
       * Get only those settings that are modifiable...
       */
      if ( query_item[i].query & QUERY_MODIFY )
      {
         number_variables = get_item_values( 1, query_item[i].item_number, (CHARTYPE *)"", QUERY_MODIFY, 0L, NULL, 0L );
         fputs( "'set", fp );
         /*
          * Write the variables to the file...
          */
         for ( j = 0; j < number_variables + 1; j++ )
         {
            fputc( ' ', fp );
            fputs( (DEFCHAR *)item_values[j].value, fp );
         }
         fprintf( fp, "'\n" );
      }
   }
   fflush( fp );
   fclose( fp );
   TRACE_RETURN();
   return(RC_OK);
}
/***********************************************************************/
#ifdef HAVE_PROTO
short set_extract_variables(short itemno)
#else
short set_extract_variables(itemno)
short itemno;
#endif
/***********************************************************************/
{
   register short i=0;
   short rc=RC_OK,number_values=0;

   TRACE_FUNCTION("query.c:   set_extract_variables");
   number_values = atoi((DEFCHAR *)item_values[0].value);
   for (i=0;i<number_values+1;i++)
   {
      if ((rc = set_rexx_variable(query_item[itemno].name,item_values[i].value,item_values[i].len,i)) != RC_OK)
         break;
   }
   TRACE_RETURN();
   return(rc);
}

/***********************************************************************/
#ifdef HAVE_PROTO
short get_number_dynamic_items( int qitem )
#else
short get_number_dynamic_items( qitem )
int qitem;
#endif
/***********************************************************************/
{
   int number_variables=0;
   PARSER_MAPPING *mapping=NULL;
   PARSER_DETAILS *details=NULL;
   DEFINE *synonym=NULL;

   switch(qitem)
   {
      case ITEM_RING:
         number_variables = number_of_files;
         break;
      case ITEM_PARSER:
         for(details=first_parser;details!=NULL;details=details->next,number_variables++);
         break;
      case ITEM_AUTOCOLOR:
      case ITEM_AUTOCOLOUR:
         for(mapping=first_parser_mapping;mapping!=NULL;mapping=mapping->next,number_variables++);
         break;
      case ITEM_SYNONYM:
         for (synonym=first_synonym;synonym!=NULL;synonym=synonym->next,number_variables++);
      default:
         break;
   }
   return number_variables;
}

/***********************************************************************/
#ifdef HAVE_PROTO
short get_item_values(int qitem,short itemno,CHARTYPE *itemargs,CHARTYPE query_type,LINETYPE argc,CHARTYPE *arg,LINETYPE arglen)
#else
short get_item_values(qitem,itemno,itemargs,query_type,argc,arg,arglen)
int qitem;
short itemno;
CHARTYPE *itemargs;
CHARTYPE query_type;
LINETYPE argc;
CHARTYPE *arg;
LINETYPE arglen;
#endif
/***********************************************************************/
{
   short number_variables = 1;
   CHARTYPE *itemname=NULL;

   TRACE_FUNCTION("query.c:   get_item_values");
   /*
    * Only use itemno to reference query_item[] array if it is NOT a
    * boolean function...
    */
   if (qitem)
   {
      itemname = query_item[itemno].name;
      number_variables = (query_item[itemno].ext_func)(
          query_item[itemno].number_values,
          itemno,
          itemargs,
          query_type,
          argc,
          arg,
          arglen);
   }
   else
   {
      itemname = function_item[itemno].name;
      number_variables = (function_item[itemno].ext_func)(
          1,
          itemno,
          itemargs,
          query_type,
          argc,
          arg,
          arglen);
   }
   /*
    * If an "error" condition exists, do not set any values.
    */
   if (number_variables >= 0)
   {
      if (number_variables == 0)
      {
         /*
          * The number of variables needs to be set dynamically for the
          * following items. Any item that has a value of 0 for item_values
          * column of query_item, needs to be catered for here.
          */
         number_variables = get_number_dynamic_items( query_item[itemno].item_number );
      }
      switch(query_type)
      {
         case QUERY_EXTRACT:
         case QUERY_FUNCTION:
         case QUERY_READV:
            sprintf((DEFCHAR *)query_num0,"%d",number_variables);
            item_values[0].value = query_num0;
            item_values[0].len = strlen((DEFCHAR *)query_num0);
            break;
         case QUERY_STATUS:
         case QUERY_QUERY:
         case QUERY_MODIFY:
            item_values[0].value = query_item[itemno].name;
            item_values[0].len = strlen((DEFCHAR *)query_item[itemno].name);
            break;
         default:
            break;
      }
   }
   TRACE_RETURN();
   return(number_variables);
}
/***********************************************************************/
#ifdef HAVE_PROTO
short extract_point_settings(short itemno,CHARTYPE *params)
#else
short extract_point_settings(itemno,params)
short itemno;
CHARTYPE *params;
#endif
/***********************************************************************/
{
   register short i=0;
   short number_variables = query_item[itemno].number_values;
   CHARTYPE num4[15];
   LINE *curr=NULL;
   LINETYPE true_line=0L;
   short rc=RC_OK;
   THELIST *curr_name;
   int len,total_len=0;

   if ( strcmp( (DEFCHAR *)params, "" ) == 0 ) /* get name for focus line only */
   {
      true_line = (compatible_feel==COMPAT_XEDIT)?CURRENT_VIEW->current_line:get_true_line( TRUE );
      curr = lll_find( CURRENT_FILE->first_line, CURRENT_FILE->last_line, true_line, CURRENT_FILE->number_lines );
      if ( curr->first_name == NULL )  /* line not named */
         number_variables = 0;
      else
      {
         total_len = sprintf( (DEFCHAR *)query_rsrvd, "%ld", true_line );
         curr_name = curr->first_name;
         while( curr_name )
         {
            len = strlen( (DEFCHAR *)curr_name->data );
            if ( total_len + len + 1 > sizeof(query_rsrvd) )
            {
               break;
            }
            strcat( (DEFCHAR *)query_rsrvd, " " );
            strcat( (DEFCHAR *)query_rsrvd, (DEFCHAR *)curr_name->data );
            total_len += len+1;
            curr_name = curr_name->next;
         }
         item_values[1].value = query_rsrvd;
         item_values[1].len = total_len;
         number_variables = 1;
      }
   }
   else
   {
      if ( strcmp( (DEFCHAR *)params, "*" ) != 0 )     /* argument must be "*" */
      {
         display_error( 1, (CHARTYPE *)params, FALSE );
         number_variables = EXTRACT_ARG_ERROR;
      }
      else
      {
         curr = CURRENT_FILE->first_line;
         for( true_line=0,i=0; curr != NULL; true_line++ )
         {
            if ( curr->first_name != NULL )  /* line is named */
            {
               total_len = sprintf( (DEFCHAR *)query_rsrvd, "%ld", true_line );
               curr_name = curr->first_name;
               while( curr_name )
               {
                  len = strlen( (DEFCHAR *)curr_name->data );
                  if ( total_len + len + 1 > sizeof(query_rsrvd) )
                  {
                     break;
                  }
                  strcat( (DEFCHAR *)query_rsrvd, " " );
                  strcat( (DEFCHAR *)query_rsrvd, (DEFCHAR *)curr_name->data );
                  total_len += len+1;
                  curr_name = curr_name->next;
               }
               rc = set_rexx_variable( query_item[itemno].name, query_rsrvd, total_len, ++i );
               if ( rc == RC_SYSTEM_ERROR )
               {
                  display_error( 54, (CHARTYPE *)"", FALSE );
                  return( EXTRACT_ARG_ERROR );
               }
            }
            curr = curr->next;
         }
         sprintf( (DEFCHAR *)num4, "%d", i );
         rc = set_rexx_variable( query_item[itemno].name, num4, strlen( (DEFCHAR *)num4 ), 0 );
         if ( rc == RC_SYSTEM_ERROR )
         {
            display_error( 54, (CHARTYPE *)"", FALSE );
            number_variables = EXTRACT_ARG_ERROR;
         }
         else
            number_variables = EXTRACT_VARIABLES_SET;
      }
   }
   return( number_variables );
}

/***********************************************************************/
#ifdef HAVE_PROTO
short extract_prefix_settings(short itemno,CHARTYPE *params,CHARTYPE query_type)
#else
short extract_prefix_settings(itemno,params,query_type)
short itemno;
CHARTYPE *params;
CHARTYPE query_type;
#endif
/***********************************************************************/
{
   register short i=0;
#define PRE_PARAMS  3
   CHARTYPE *word[PRE_PARAMS+1];
   CHARTYPE strip[PRE_PARAMS];
   unsigned short num_params=0;
   short number_variables = query_item[itemno].number_values;
   CHARTYPE num4[15];
   LINE *curr=NULL;
   short rc=RC_OK;
   CHARTYPE *tmpbuf=NULL;

   strip[0]=STRIP_BOTH;
   strip[1]=STRIP_BOTH;
   strip[2]=STRIP_BOTH;
   num_params = param_split(params,word,PRE_PARAMS,WORD_DELIMS,TEMP_PARAM,strip,FALSE);
   switch(num_params)
   {
      /*
       * None or 1 parameters, error.
       */
      case 0:
      case 1:
         display_error(3,(CHARTYPE *)"",FALSE);
         number_variables = EXTRACT_ARG_ERROR;
         break;
      /*
       * 2  parameters, Synonym and name (or *).
       */
      case 2:
         if (!equal((CHARTYPE *)"synonym",word[0],1))
         {
            display_error(13,(CHARTYPE *)"",FALSE);
            number_variables = EXTRACT_ARG_ERROR;
            break;
         }
         break;
    default:
         /*
          * Too many parameters, error.
          */
         display_error(2,(CHARTYPE *)"",FALSE);
         number_variables = EXTRACT_ARG_ERROR;
         break;
   }
   /*
    * If the validation of parameters is successful...
    */
   if (number_variables >= 0)
   {
      if (strcmp((DEFCHAR *)word[1],"*") == 0)
      {
         /*
          * Get oldname for all synonyms...
          */
         curr = first_prefix_synonym;
         i = 0;
         while(curr != NULL)
         {
            tmpbuf = (CHARTYPE *)(*the_malloc)(sizeof(CHARTYPE)*(strlen((DEFCHAR *)curr->name)+strlen((DEFCHAR *)curr->line)+2));
            if (tmpbuf == (CHARTYPE *)NULL)
            {
               display_error(30,(CHARTYPE *)"",FALSE);
               return(EXTRACT_ARG_ERROR);
            }
            strcpy((DEFCHAR *)tmpbuf,(DEFCHAR *)curr->name);
            strcat((DEFCHAR *)tmpbuf," ");
            strcat((DEFCHAR *)tmpbuf,(DEFCHAR *)curr->line);
            if ( query_type == QUERY_EXTRACT )
               rc = set_rexx_variable(query_item[itemno].name,tmpbuf,strlen((DEFCHAR *)tmpbuf),++i);
            else
            {
               display_error( 0, tmpbuf, FALSE );
               rc = RC_OK;
            }
            (*the_free)(tmpbuf);
            if (rc == RC_SYSTEM_ERROR)
            {
               display_error(54,(CHARTYPE *)"",FALSE);
               return(EXTRACT_ARG_ERROR);
            }
            curr = curr->next;
         }
         if ( query_type == QUERY_EXTRACT )
         {
            sprintf((DEFCHAR *)num4,"%d",i);
            rc = set_rexx_variable(query_item[itemno].name,num4,strlen((DEFCHAR *)num4),0);
            if (rc == RC_SYSTEM_ERROR)
            {
               display_error(54,(CHARTYPE *)"",FALSE);
               number_variables = EXTRACT_ARG_ERROR;
            }
            else
               number_variables = EXTRACT_VARIABLES_SET;
         }
         else
         {
            number_variables = EXTRACT_VARIABLES_SET;
         }
      }
      else
      {
         /*
          * Get oldname for named synonym...
          */
         item_values[1].value = find_prefix_synonym(word[1]);
         item_values[1].len = strlen((DEFCHAR *)item_values[1].value);
         number_variables = 1;
      }
   }
   return(number_variables);
}
/***********************************************************************/
#ifdef HAVE_PROTO
void get_etmode(CHARTYPE *onoff,CHARTYPE *list)
#else
void get_etmode(onoff,list)
CHARTYPE *onoff,*list;
#endif
/***********************************************************************/
{
   bool on_flag=FALSE;
   bool off_flag=FALSE;
   bool last_state=FALSE;
   char tmp[15];
   register int i=0;

   TRACE_FUNCTION("query.c:   get_etmode");
   strcpy((DEFCHAR *)list,"");
   last_state = TRUE;
   for (i=0;i<256;i++)
   {
      if (etmode_flag[i] == TRUE)
         off_flag = TRUE;
      else
         on_flag = TRUE;
      if (last_state != etmode_flag[i])
      {
         if (last_state == FALSE)
         {
            sprintf(tmp,"%d",i-1);
            strcat((DEFCHAR *)list,tmp);
         }
         else
         {
            sprintf(tmp," %d-",i);
            strcat((DEFCHAR *)list,tmp);
         }
         last_state = etmode_flag[i];
      }
   }
   if (*(list+strlen((DEFCHAR *)list)-1) == '-')
   {
      sprintf(tmp,"%d",255);
      strcat((DEFCHAR *)list,tmp);
   }
   if (on_flag)
   {
      strcpy((DEFCHAR *)onoff,"ON");
      if (!off_flag)
         strcpy((DEFCHAR *)list,"");
   }
   else
   {
      strcpy((DEFCHAR *)onoff,"OFF");
      strcpy((DEFCHAR *)list,"");
   }
   TRACE_RETURN();
   return;
}
/***********************************************************************/
#ifdef HAVE_PROTO
short extract_colour_settings(short itemno,CHARTYPE *buffer,CHARTYPE query_type,CHARTYPE *params,bool us,bool isecolour)
#else
short extract_colour_settings(itemno,buffer,query_type,params,us,isecolour)
short itemno;
CHARTYPE *buffer;
CHARTYPE query_type;
CHARTYPE *params;
bool us,isecolour;
#endif
/***********************************************************************/
{
   short rc=RC_OK;
   register int i=0,maxnum;
   int start=0,end=0,number_variables=0,off=0;
   bool found=FALSE;
   CHARTYPE *attr_string=NULL,tmparea[2];
   ROWTYPE save_msgline_rows = CURRENT_VIEW->msgline_rows;
   bool save_msgmode_status = CURRENT_VIEW->msgmode_status;
   CHARTYPE *ptr,*area,*colour=(CHARTYPE*)"colour ",*color=(CHARTYPE*)"color ",*ecolour=(CHARTYPE*)"ecolour ",*ecolor=(CHARTYPE*)"ecolor ";
   COLOUR_ATTR *attr;

   TRACE_FUNCTION("query.c:   extract_colour_settings");
   tmparea[1] = '\0';
   maxnum = (isecolour) ? ECOLOUR_MAX : ATTR_MAX;
   attr = (isecolour) ? CURRENT_FILE->ecolour : CURRENT_FILE->attr;
   if (blank_field(params)
   ||  strcmp((DEFCHAR*)params,"*") == 0)
   {
     start = 0;
     end = maxnum;
   }
   else
   {
      if (isecolour)
      {
         if (strlen((DEFCHAR *)params) != 1)
         {
            TRACE_RETURN();
            return(EXTRACT_ARG_ERROR);
         }
         if (*params >= 'A' && *params <= 'Z')
            i = *params - 'A';
         else if (*params >= 'a' && *params <= 'z')
            i = *params - 'a';
         else if (*params >= '1' && *params <= '9')
            i = *params - '1' + 26;
         else
         {
            TRACE_RETURN();
            return(EXTRACT_ARG_ERROR);
         }
         found = TRUE;
      }
      else
      {
         for (i=0;i<maxnum;i++)
         {
            if (equal(valid_areas[i].area,params,valid_areas[i].area_min_len))
            {
               found = TRUE;
               break;
            }
         }
      }
      if (!found)
      {
         display_error(1,params,FALSE);
         TRACE_RETURN();
         return(EXTRACT_ARG_ERROR);
      }
      start = i;
      end = i+1;
   }
   if (query_type == QUERY_QUERY)
   {
      CURRENT_VIEW->msgline_rows   = min(terminal_lines-1,end-start);
      CURRENT_VIEW->msgmode_status = TRUE;
   }

   for (i=start;i<end;i++)
   {
      attr_string = get_colour_strings(attr+i);
      if (attr_string == (CHARTYPE *)NULL)
      {
         TRACE_RETURN();
         return(EXTRACT_ARG_ERROR);
      }
      if (us)
      {
         if (isecolour)
            ptr = ecolor;
         else
            ptr = color;
      }
      else
      {
         if (isecolour)
            ptr = ecolour;
         else
            ptr = colour;
      }
      if (isecolour)
      {
        if ( i > 25 )
           tmparea[0] = (CHARTYPE)(i-25)+'0';
        else
           tmparea[0] = (CHARTYPE)(i+'A');
         area = tmparea;
      }
      else
      {
         area = valid_areas[i].area;
      }
      sprintf((DEFCHAR *)query_rsrvd,"%s%s %s",
            (query_type == QUERY_QUERY) ? (DEFCHAR *)ptr : "",
            area,
            attr_string);
      (*the_free)(attr_string);

      if (query_type == QUERY_QUERY)
         display_error(0,query_rsrvd,TRUE);
      else
      {
         number_variables++;
         item_values[number_variables].len = strlen((DEFCHAR *)query_rsrvd);
         memcpy((DEFCHAR*)trec+off,(DEFCHAR*)query_rsrvd,(item_values[number_variables].len)+1);
         item_values[number_variables].value = trec+off;
         off += (item_values[number_variables].len)+1;
      }
   }
   if (query_type == QUERY_QUERY)
   {
      CURRENT_VIEW->msgline_rows   = save_msgline_rows;
      CURRENT_VIEW->msgmode_status = save_msgmode_status;
      rc = EXTRACT_VARIABLES_SET;
   }
   else
      rc = number_variables;
   TRACE_RETURN();
   return(rc);
}
/***********************************************************************/
#ifdef HAVE_PROTO
short extract_autocolour_settings(short itemno,CHARTYPE *buffer,CHARTYPE query_type,CHARTYPE *params,bool us)
#else
short extract_autocolour_settings(itemno,buffer,query_type,params,us)
short itemno;
CHARTYPE *buffer;
CHARTYPE query_type;
CHARTYPE *params;
bool us;
#endif
/***********************************************************************/
{
   short rc=RC_OK;
   register int i=0;
   int number_variables=0,off=0;
   bool found=FALSE;
   ROWTYPE save_msgline_rows = CURRENT_VIEW->msgline_rows;
   bool save_msgmode_status = CURRENT_VIEW->msgmode_status;
   CHARTYPE *ptr,*ptr_mask=NULL,*ptr_magic=NULL,*ptr_parser=NULL;
   PARSER_MAPPING *curr;

   TRACE_FUNCTION("query.c:   extract_autocolour_settings");
   ptr = (us) ? (CHARTYPE *)"autocolor " : (CHARTYPE *)"autocolour ";
   if (blank_field(params)
   ||  strcmp((DEFCHAR*)params,"*") == 0)
   {
      if (query_type == QUERY_QUERY)
      {
         for (i=0,curr=first_parser_mapping;curr!=NULL;curr=curr->next,i++);
         CURRENT_VIEW->msgline_rows   = min(terminal_lines-1,i);
         CURRENT_VIEW->msgmode_status = TRUE;
      }
      for (curr=first_parser_mapping;curr!=NULL;curr=curr->next)
      {
         sprintf((DEFCHAR *)query_rsrvd,"%s%s%s %s%s",
           (query_type == QUERY_QUERY) ? (DEFCHAR *)ptr : "",
           (curr->filemask) ? (DEFCHAR *)curr->filemask : "",
           (curr->magic_number) ? (DEFCHAR *)curr->magic_number : "",
            (DEFCHAR *)curr->parser->parser_name,
           (curr->magic_number) ? " MAGIC" : "");

         if (query_type == QUERY_QUERY)
            display_error(0,query_rsrvd,TRUE);
         else
         {
            number_variables++;
            item_values[number_variables].len = strlen((DEFCHAR *)query_rsrvd);
            memcpy((DEFCHAR*)trec+off,(DEFCHAR*)query_rsrvd,(item_values[number_variables].len)+1);
            item_values[number_variables].value = trec+off;
            off += (item_values[number_variables].len)+1;
         }
      }
   }
   else
   {
      if (query_type == QUERY_QUERY)
      {
         CURRENT_VIEW->msgline_rows   = 1;
         CURRENT_VIEW->msgmode_status = TRUE;
      }
      /*
       * Find a match for the supplied mask or magic number
       */
      for (curr=first_parser_mapping;curr!=NULL;curr=curr->next)
      {
#ifdef UNIX
         if (curr->filemask
         &&  strcmp((DEFCHAR *)params,(DEFCHAR *)curr->filemask) == 0)
         {
            ptr_mask = curr->filemask;
            ptr_magic = (CHARTYPE *)"";
            found = TRUE;
            break;
         }
         if (curr->magic_number
         &&  strcmp((DEFCHAR *)params,(DEFCHAR *)curr->magic_number) == 0)
         {
            ptr_mask = curr->magic_number;
            ptr_magic = (CHARTYPE *)"MAGIC";
            found = TRUE;
            break;
         }
#else
         if (curr->filemask
         &&  my_stricmp(params,curr->filemask) == 0)
         {
            ptr_mask = curr->filemask;
            ptr_magic = (CHARTYPE *)"";
            found = TRUE;
            break;
         }
         if (curr->magic_number
         &&  my_stricmp(params,curr->magic_number) == 0)
         {
            ptr_mask = curr->magic_number;
            ptr_magic = (CHARTYPE *)"MAGIC";
            found = TRUE;
            break;
         }
#endif
      }
      if (found)
         ptr_parser = (CHARTYPE *)curr->parser->parser_name;
      else
      {
         ptr_mask = params;
         ptr_magic = (CHARTYPE *)"";
         ptr_parser = (CHARTYPE *)"NULL";
      }
      if (query_type == QUERY_QUERY)
      {
         sprintf((DEFCHAR *)query_rsrvd,"%s%s %s%s",
            (query_type == QUERY_QUERY) ? (DEFCHAR *)ptr : "",
            ptr_mask,
            ptr_parser,
            ptr_magic);
         display_error(0,query_rsrvd,TRUE);
      }
      else
      {
         item_values[1].value = ptr_mask;
         item_values[1].len = strlen((DEFCHAR *)ptr_mask);
         item_values[2].value = ptr_parser;
         item_values[2].len = strlen((DEFCHAR *)ptr_parser);
         item_values[3].value = ptr_magic;
         item_values[3].len = strlen((DEFCHAR *)ptr_magic);
         number_variables = 3;
      }
   }

   if (query_type == QUERY_QUERY)
   {
      CURRENT_VIEW->msgline_rows   = save_msgline_rows;
      CURRENT_VIEW->msgmode_status = save_msgmode_status;
      rc = EXTRACT_VARIABLES_SET;
   }
   else
      rc = number_variables;

   TRACE_RETURN();
   return(rc);
}

/***********************************************************************/
#ifdef HAVE_PROTO
int number_function_item( void )
#else
int number_function_item()
#endif
/***********************************************************************/
{
   return NUMBER_FUNCTION_ITEM;
}

/***********************************************************************/
#ifdef HAVE_PROTO
int number_query_item( void )
#else
int number_query_item()
#endif
/***********************************************************************/
{
   return NUMBER_QUERY_ITEM;
}

/***********************************************************************/
#ifdef HAVE_PROTO
void format_options( CHARTYPE *buf )
#else
void format_options( buf )
CHARTYPE *buf;
#endif
/***********************************************************************/
{
   LINE *curr=first_option;
   int tail,col,itemno,linelen,valuelen,number_variables,max_col=0,off;

   TRACE_FUNCTION("query.c:   format_options");

   while(curr != NULL)
   {
      itemno = curr->length;
      tail = curr->flags.new_flag;
      number_variables = get_item_values(1,query_item[itemno].item_number,(CHARTYPE *)"",QUERY_EXTRACT,0L,NULL,0L);
      if (number_variables >= tail)
      {
         col = curr->select;
         linelen = curr->line?strlen((DEFCHAR *)curr->line):0;
         valuelen = (curr->save_select)?min(curr->save_select,item_values[tail].len):item_values[tail].len;
         if (curr->line)
         {
            memcpy(buf+col,(DEFCHAR *)curr->line,linelen);
         }
         memcpy(buf+col+linelen,(DEFCHAR *)item_values[tail].value,valuelen);
         off = col+linelen+valuelen;
         if (off > max_col)
            max_col = off;
      }
      curr = curr->next;
   }
   buf[max_col] = '\0';
   TRACE_RETURN();
   return;
}
