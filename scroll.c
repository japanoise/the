/***********************************************************************/
/* SCROLL.C - SCROLL commands                                          */
/* This file contains all commands that can be assigned to function    */
/* keys or typed on the command line.                                  */
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

/***********************************************************************/
#ifdef HAVE_PROTO
short scroll_page(short direction,LINETYPE num_pages,bool scrollbar)
#else
short scroll_page(direction,num_pages,scrollbar)
short direction;
LINETYPE num_pages;
bool scrollbar;
#endif
/***********************************************************************/
{
   short y=0,x=0,save_y=0,rc;
   bool save_scroll_cursor_stay=scroll_cursor_stay;

   TRACE_FUNCTION("scroll.c:  scroll_page");
   /*
    * If scrolling backward and already on TOF, return.
    * If scrolling forward and already on EOF, return.
    */
   if ((direction == DIRECTION_BACKWARD
      && CURRENT_TOF)
   ||  (direction == DIRECTION_FORWARD
      && CURRENT_BOF))
   {
      TRACE_RETURN();
      return(RC_TOF_EOF_REACHED);
   }
   /*
    * If scrolling via the scroll bars, ALWAYS leave the cursor on the
    * screen line.
    */
   if (scrollbar)
      save_scroll_cursor_stay = TRUE;
   /*
    * Get current focus row if cursor is to stay on current focus line...
    */
   if (save_scroll_cursor_stay)
      save_y = get_row_for_focus_line(current_screen,CURRENT_VIEW->focus_line,CURRENT_VIEW->current_row);
   /*
    * Find the new current line, num_pages away...
    */
   post_process_line(CURRENT_VIEW,CURRENT_VIEW->focus_line,(LINE *)NULL,TRUE);
   CURRENT_VIEW->current_line = find_next_current_line(num_pages,direction);
   build_screen(current_screen);
   if (save_scroll_cursor_stay)
   {
      save_y = get_row_for_tof_eof(save_y,current_screen);
      CURRENT_VIEW->focus_line = CURRENT_SCREEN.sl[save_y].line_number;
      pre_process_line(CURRENT_VIEW,CURRENT_VIEW->focus_line,(LINE *)NULL);
      build_screen(current_screen);
   }
   else
   {
      CURRENT_VIEW->focus_line = calculate_focus_line(CURRENT_VIEW->focus_line, CURRENT_VIEW->current_line);
      pre_process_line(CURRENT_VIEW,CURRENT_VIEW->focus_line,(LINE *)NULL);
   }
   /*
    * If curses has started, display screen and sort out cursor position..
    */
   if (curses_started)
   {
      getyx(CURRENT_WINDOW,y,x);
      display_screen(current_screen);
      if (CURRENT_VIEW->current_window != WINDOW_COMMAND)
      {
         y = get_row_for_focus_line(current_screen,CURRENT_VIEW->focus_line, CURRENT_VIEW->current_row);
         wmove(CURRENT_WINDOW,y,x);
         if (scrollbar)
            wrefresh(CURRENT_WINDOW);
      }
   }
   if (CURRENT_TOF || CURRENT_BOF)
      rc = RC_TOF_EOF_REACHED;
   else
      rc = RC_OK;
   TRACE_RETURN();
   return rc;
}
/***********************************************************************/
#ifdef HAVE_PROTO
short scroll_line( CHARTYPE curr_screen, VIEW_DETAILS *curr_view, short direction,LINETYPE num_lines,bool scrollbar,short escreen)
#else
short scroll_line( curr_screen, curr_view, direction, num_lines, scrollbar, escreen )
CHARTYPE curr_screen;
VIEW_DETAILS *curr_view;
short direction;
LINETYPE num_lines;
bool scrollbar;
short escreen;
#endif
/***********************************************************************/
{
   short rc=RC_OK;
   unsigned short x=0,y=0,iscrollbar=scrollbar;
   bool on_file_edge=FALSE,on_screen_edge=FALSE;
   short number_focus_rows=0;
   bool leave_cursor=FALSE;
   LINETYPE new_focus_line=0L,new_current_line=0L,edge_line=0L;
   LINETYPE longy=0L,longx=0L;
   ROWTYPE yoff1=0,yoff2=0;
   chtype color_filearea = set_colour( curr_view->file_for_view->attr+ATTR_FILEAREA );
   chtype color_cursorline = set_colour( curr_view->file_for_view->attr+ATTR_CURSORLINE );

   TRACE_FUNCTION("scroll.c:  scroll_line");
   /*
    * If this function is called via scrollbar...
    * If scrolling backward and already on TOF, return.
    * If scrolling forward and already on EOF, return.
    */
   if ( scrollbar )
   {
      if ( ( direction == DIRECTION_BACKWARD && CURRENT_TOF )
      ||   ( direction == DIRECTION_FORWARD && CURRENT_BOF ) )
      {
         TRACE_RETURN();
         return(RC_TOF_EOF_REACHED);
      }
   }
   calculate_scroll_values( curr_screen, curr_view, &number_focus_rows, &new_focus_line, &new_current_line, &on_screen_edge,&on_file_edge, &leave_cursor,direction );
   switch( iscrollbar )
   {
      case FALSE:
         getyx( SCREEN_WINDOW(curr_screen), y, x );
         if ( direction == DIRECTION_FORWARD )
         {
            edge_line = curr_view->file_for_view->number_lines+1L;
            yoff1 = y - ((leave_cursor) ? 0 : 1);
            yoff2 = y + number_focus_rows;
         }
         else
         {
            edge_line = 0L;
            yoff1 = y + ((leave_cursor) ? 0 : 1);
            yoff2 = y - number_focus_rows;
         }
         /*
          * If the cursor is on the edge of the window or on the edge of the
          * file and tabbing to the command line is set, tab to the command line
          * provided the command line is ON.
          */
         if ( escreen == CURSOR_SCREEN
         &&  ( on_screen_edge || on_file_edge ) )
         {
            if ( SCREEN_WINDOW_COMMAND(curr_screen) == NULL )
            {
               getyx( SCREEN_WINDOW(curr_screen), y, x );
               if ( direction == DIRECTION_FORWARD )
                  rc = find_first_focus_line( curr_screen, &y );
               else
                  rc = find_last_focus_line( curr_screen, &y );
               if (rc == RC_OK)
               {
                  curr_view->focus_line = screen[curr_screen].sl[y].line_number;
                  pre_process_line( curr_view, curr_view->focus_line, (LINE *)NULL );
                  wmove(SCREEN_WINDOW( curr_screen), y, x );
               }
               break;
            }
            THEcursor_cmdline( curr_screen, curr_view, 1 );
            break;
         }
         /*
          * If the cursor is on the edge of the file...
          */
         if ( on_file_edge )
         {
            /*
             * ... and the current row is the edge of the file, stay there.
             */
            if ( curr_view->current_line == edge_line )
               break;
            /*
             * ... and the edge of the file is above or below the current row,
             * scroll the window.
             */
            curr_view->current_line = new_current_line;
            build_screen( curr_screen );
            display_screen( curr_screen );
            y = get_row_for_focus_line( curr_screen, curr_view->focus_line, curr_view->current_row );
            wmove( SCREEN_WINDOW( curr_screen), y, x );
            break;
         }
         /*
          * If on the edge of the window, scroll the window.
          */
         if ( on_screen_edge )
         {
            curr_view->current_line = new_current_line;
            post_process_line( curr_view, curr_view->focus_line, (LINE *)NULL, TRUE );
            curr_view->focus_line = new_focus_line;
            pre_process_line( curr_view, curr_view->focus_line, (LINE *)NULL );
            build_screen( curr_screen );
            display_screen( curr_screen );
            wmove( SCREEN_WINDOW(curr_screen), yoff1, x );
            break;
         }
         /*
          * We are in the middle of the window, so just move the cursor up or
          * down 1 line.
          */
         wmove( SCREEN_WINDOW(curr_screen), yoff2, x );
         rc = post_process_line( curr_view, curr_view->focus_line, (LINE *)NULL, TRUE );
         curr_view->focus_line = new_focus_line;
         pre_process_line( curr_view, curr_view->focus_line, (LINE *)NULL );
         build_screen( curr_screen );
         if ( ( curr_view->highlight
         &&  rc != RC_NO_LINES_CHANGED )
            || ( color_filearea != color_cursorline ) )
         {
            display_screen( curr_screen );
         }
         break;
    case TRUE:
         if (curr_view->current_window != WINDOW_COMMAND)
            get_cursor_position( &longy, &longx, &new_focus_line, &new_current_line );
         rc = advance_current_line( (direction == DIRECTION_FORWARD) ? num_lines : -num_lines );
         if ( curr_view->current_window != WINDOW_COMMAND )
         {
            THEcursor_move( curr_screen, curr_view, TRUE, TRUE, (short)longy, (short)longx );
            show_heading( curr_screen );
            if ( curr_view->id_line )
               wnoutrefresh( SCREEN_WINDOW_IDLINE(curr_screen) );
            wrefresh( SCREEN_WINDOW(curr_screen) );
         }
         break;
   }
   if ( VIEW_FOCUS_TOF(curr_view) || VIEW_FOCUS_BOF(curr_view) )
      rc = RC_TOF_EOF_REACHED;
   else
      rc = RC_OK;
   TRACE_RETURN();
   return rc;
}
