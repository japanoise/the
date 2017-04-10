/***********************************************************************/
/* SINGLE.C - Processing for single input mode                         */
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

static LENGTHTYPE tmp_len;
static CHARTYPE tmp_str[MAX_FILE_NAME+50];

#ifdef THE_SINGLE_INSTANCE_ENABLED

#if defined(WIN32) && !defined(__CYGWIN32__)
#include <windows.h>

#define SHM_SIZE 4096
HANDLE mutex, event, shm;
char *shmptr;

static void display_syserror(DWORD error, const char *prefix)
{
   char buf[512];
   char msg[1024];

   FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), buf, sizeof( buf ), NULL ) ;
   sprintf( msg, "%s: %s", prefix, buf );
   if ( GetConsoleOutputCP() == GetOEMCP() )
      CharToOem( msg, msg );
   display_error( 0, msg, FALSE );
}

/***********************************************************************/
int initialise_fifo( LINE *first_file_name, LINETYPE startup_line, LENGTHTYPE startup_column, bool ro )
/***********************************************************************/
{
   LINE *current_file_name;
   char buf[MAX_FILE_NAME+1];
   char *ronly;
   int am_client, rc;

   /*
    * The best would be to create a true fifo, but Win9x doesn't support
    * fifos properly. We use shared memory instead. We allocate a 4K page
    * and use it with two semaphores. One is for signalling the server,
    * one for the access protection. This is a typical run:
    *
    * Server                                  Client
    * create shm and sems
    * releases all sems
    * do forever
    *                                         aquires the protection mutex
    *                                         gets the protection mutex
    *                                         fills the shm
    *                                         signals the signal semaphore
    *    wait for signal semaphore
    *    aquires the protection mutex
    *                                         releases the protection mutex
    *    gets the protection mutex
    *    does the wanted work
    *    resets the signal semaphore
    *    releases the protection mutex
    *    end
    *
    * This is a rough scheme. A or the client needs to know when the shm is
    * free for use. Therefore the first byte in the shared memory is used as
    * a flag. A client cyles through this endless loop until the flag is
    * cleaned:
    * 1) releases the protection mutex
    * 2) release the current time slice
    * 3) aquires the protection mutex
    * 4) gets the protection mutex
    */
   TRACE_FUNCTION("single.c:  initialise_fifo");

   _snprintf( buf, sizeof(buf), "THEshm%s", fifo_name );
   SetLastError( 0 ); /* prevent problems determining the correct error code */
//   if ( ( shm = CreateFileMapping( (HANDLE) 0xFFFFFFFF,
   if ( ( shm = CreateFileMapping( INVALID_HANDLE_VALUE,
                                   NULL,
                                   PAGE_READWRITE | SEC_COMMIT,
                                   0ul,
                                   SHM_SIZE,
                                   buf ) ) == NULL )
   {
      display_syserror( GetLastError(), "CreateFileMapping() failed" );
      TRACE_CONSTANT("CreateFileMapping() failed\n");
      TRACE_RETURN();
      return 1;
   }

   if ( GetLastError() == ERROR_ALREADY_EXISTS )
   {
      /*
       * The shared memory exists; we are a client
       * Try accessing the memory and the semaphores
       */
      if ( ( shmptr = MapViewOfFile( shm,
                                     FILE_MAP_WRITE,
                                     0ul,
                                     0ul,
                                     SHM_SIZE ) ) == NULL )
      {
         display_syserror( GetLastError(), "Client: Shared memory already exists" );
         TRACE_CONSTANT("Client: Shared memory already exists\n");
         TRACE_RETURN();
         return 1;
      }

      _snprintf( buf, sizeof(buf), "THEmtx%s", fifo_name );
      if ( ( mutex = OpenMutex( MUTEX_ALL_ACCESS, FALSE, buf ) ) == NULL )
      {
         display_syserror( GetLastError(), "Client: OpenMutex() failed" );
         TRACE_CONSTANT("Client: OpenMutex() failed\n");
         TRACE_RETURN();
         return 1;
      }

      _snprintf( buf, sizeof(buf), "THEevt%s", fifo_name );
      if ( ( event = OpenEvent( EVENT_ALL_ACCESS, FALSE, buf ) ) == NULL )
      {
         display_syserror( GetLastError(), "Client: OpenEvent() failed" );
         TRACE_CONSTANT("Client: OpenEvent() failed\n");
         TRACE_RETURN();
         return 1;
      }

      /*
       * Loop through the files we have to edit. Pass each name to the server.
       */
      current_file_name = first_file_name;
      while( current_file_name != NULL )
      {
         if ( ( rc = splitpath( current_file_name->line ) ) != RC_OK )
         {
            display_error( 10, current_file_name->line, FALSE );
         }
         else
         {
            /*
             * Try to acquire the mutex and perform the endless loop of above.
             */
            for (;;)
            {
               if ( WaitForSingleObject( mutex, INFINITE ) != WAIT_OBJECT_0 )
               {
                  display_syserror( GetLastError(), "Client: WaitForSingleObject() failed" );
                  TRACE_CONSTANT("Client: WaitForSingleObject() failed\n");
                  TRACE_RETURN();
                  return 1;
               }
               if ( *shmptr == 0 )
                  break;
               ReleaseMutex( mutex );
               /*
                * Instead of releasing we do an active wait. This is much
                * better than the release aka Sleep(0) since we allow even
                * processes with a lower priority to run and the user may
                * have fiddled with the priority.
                * We wait for THE's server to become ready.
                */
               Sleep(1);
            }

            /*
             * If started with -r, set READONLY FORCE
             */
            if ( ro )
               ronly = "#readonly force";
            else
               ronly = "";
            /*
             * If line and/or column specified on command line, use
             * them to reposition file...
             */
            if ( startup_line != 0L
            ||   startup_column != 0 )
               _snprintf( shmptr, SHM_SIZE,
                          "x %s%s#cursor goto %ld %d%s",
                          sp_path, sp_fname,
                          (startup_line) ? startup_line : 1,
                          (startup_column) ? startup_column : 1,
                          ronly );
            else
               _snprintf( shmptr, SHM_SIZE,
                          "x %s%s%s",
                          sp_path,
                          sp_fname,
                          ronly );
            TRACE_CONSTANT("Client sending:");
            TRACE_CONSTANT(shmptr);
            TRACE_CONSTANT(" to server\n");
            /*
             * Signal the server and release the mutex *after* this.
             */
            SetEvent( event );
            ReleaseMutex( mutex );
         }
         current_file_name = current_file_name->next;
      }
      close_fifo();
      /*
       * We are the client, so return with 1 to enable the caller to stop
       */
      am_client = 1;
   }
   else
   {
      /*
       * We are the server and have to create the semaphores and we have
       * to map the shared memory in our address space.
       */
      if ( ( shmptr = MapViewOfFile( shm,
                                     FILE_MAP_WRITE,
                                     0ul,
                                     0ul,
                                     SHM_SIZE ) ) == NULL )
      {
         display_syserror( GetLastError(), "Server: Shared memory already exists" );
         CloseHandle( shm );
         shm = NULL;
         TRACE_CONSTANT("Server: Shared memory already exists\n");
         TRACE_RETURN();
         return 1;
      }
      *shmptr = 0; /* Nothing to process */

      _snprintf( buf, sizeof(buf), "THEmtx%s", fifo_name );
      if ( ( mutex = CreateMutex( NULL, FALSE, buf ) ) == NULL )
      {
         display_syserror( GetLastError(), "Server: CreateMutex() failed" );
         UnmapViewOfFile( shmptr );
         CloseHandle( shm );
         shmptr = NULL;
         shm = NULL;
         TRACE_CONSTANT("Server: CreateMutex() failed\n");
         TRACE_RETURN();
         return 1;
      }

      _snprintf( buf, sizeof(buf), "THEevt%s", fifo_name );
      if ( ( event = CreateEvent( NULL, TRUE, FALSE, buf ) ) == NULL )
      {
         display_syserror( GetLastError(), "Server: CreateEvent() failed" );
         CloseHandle( mutex );
         UnmapViewOfFile( shmptr );
         CloseHandle( shm );
         mutex = NULL;
         shmptr = NULL;
         shm = NULL;
         TRACE_CONSTANT("Server: CreateEvent() failed\n");
         TRACE_RETURN();
         return 0;
      }

      single_instance_server = TRUE;
      am_client = 0;
   }
   TRACE_RETURN();
   return am_client;
}

static void process_edit_request( void )
{
   bool le_status;
   CHARTYPE le_value;
   VIEW_DETAILS *le_view;

   TRACE_FUNCTION("single.c:  process_edit_request");

   /*
    * The event semaphor was signaled showing us to check the shared
    * memory for a new file to edit.
    * Lock the shared memory and process the file.
    */
   if ( WaitForSingleObject( mutex, INFINITE ) != WAIT_OBJECT_0 )
   {
      display_syserror( GetLastError(), "" );
      TRACE_RETURN();
      return;
   }

   /*
    * save view and LINEND details to reset later
    */
   le_status = CURRENT_VIEW->linend_status;
   le_value = CURRENT_VIEW->linend_value;
   le_view = CURRENT_VIEW;

   CURRENT_VIEW->linend_status = TRUE;
   CURRENT_VIEW->linend_value = '#';
   command_line( shmptr, TRUE );

   le_view->linend_status = le_status;
   le_view->linend_value = le_value;

   /*
    * Do the cleanup to allow more clients to connect.
    */
   *shmptr = '\0';
   ResetEvent( event );
   ReleaseMutex( mutex );
   TRACE_RETURN();
   return;
}


/***********************************************************************/
int process_fifo_input( int key )
/***********************************************************************/
{
   HANDLE hEvents[2];
   DWORD src;

   TRACE_FUNCTION("single.c:  process_fifo_input");

   if ( key != -1 )
   {
      TRACE_CONSTANT( "Returning because key == -1\n");
      TRACE_RETURN();
      return key;
   }

   /*
    * Prepare everything for waiting on any event.
    */
   if ( ( hEvents[0] = (HANDLE) PDC_get_input_fd() ) == 0 )
   {
      /*
       * We have to return immediately since we don't know how to wait
       * for a key; at least we can check the event semaphore, though.
       */
      if ( WaitForSingleObject( event, 0 ) == WAIT_OBJECT_0 )
      {
         process_edit_request();
         key = 0;
      }
      TRACE_CONSTANT( "Returning because hEvents[0] == PDC_get_input_fd()\n");
      TRACE_RETURN();
      return key;
   }

   hEvents[1] = event;

   for ( ; ; )
   {
      src = WaitForMultipleObjects( 2,
                                    hEvents,
                                    FALSE,
                                    INFINITE );

      if ( src == WAIT_FAILED )
      {
         TRACE_CONSTANT( "WAIT_FAILED\n");
         display_syserror( GetLastError(), "" );
         break;
      }
      else
      {
         if ( src == WAIT_OBJECT_0 )
         {
            /*
             * A key is ready...
             * We have to increment the semaphore, because we have
             * decremented it by waiting on it. Isn't this crap!
             */
            ReleaseSemaphore( hEvents[0], 1, NULL );
            TRACE_CONSTANT( "Releasing semaphore\n");
            break;
         }
         else if ( src == WAIT_OBJECT_0 + 1 )
         {
            /*
             * We were signalled by a THE client to do edit a new file.
             */
            process_edit_request();
            key = 0;
            TRACE_CONSTANT( "We called process_edit_request()\n");
            break;
         }
      }
   }
   TRACE_RETURN();
   return key;
}

/***********************************************************************/
void close_fifo( void )
/***********************************************************************/
{
   TRACE_FUNCTION("single.c:  close_fifo");
   if ( event != NULL )
      CloseHandle( event );
   if ( mutex != NULL )
      CloseHandle( mutex );
   if ( shmptr != NULL )
      UnmapViewOfFile( shmptr );
   if ( shm != NULL )
      CloseHandle( shm );
   event = NULL;
   mutex = NULL;
   shmptr = NULL;
   shm = NULL;
   TRACE_RETURN();
   return;
}

#elif defined(HAVE_SELECT) && defined(HAVE_MKFIFO)

/*
 * Most Unixen; determine where select() is
 */
# if defined(HAVE_SYS_SELECT_H)
#   include <sys/select.h>
# elif defined(HAVE_SYS_SOCKET_H)
#   include <sys/socket.h>
# elif defined(SELECT_IN_TIME_H)
#   include <time.h>
# endif

static int fifo_fd;

/***********************************************************************/
#ifdef HAVE_PROTO
int initialise_fifo( LINE *first_file_name, LINETYPE startup_line, LENGTHTYPE startup_column, bool ro )
#else
int initialise_fifo( first_file_name, startup_line, startup_column, ro )
LINE *first_file_name;
LINETYPE startup_line;
LENGTHTYPE startup_column;
bool ro;
#endif
/***********************************************************************/
{
   LINE *current_file_name;
   int am_client = 0, rc;
   char *ronly;

   TRACE_FUNCTION("single.c:  initialise_fifo");

   if ( file_exists( fifo_name ) == THE_FILE_EXISTS )
   {
      /*
       * The FIFO exists, so we assume we are the client here...
       */
      if ( file_writable( fifo_name ) )
      {
         fifo_fd = open( (DEFCHAR *)fifo_name, O_WRONLY );
         if ( fifo_fd == (-1) )
         {
            display_error( 0, (CHARTYPE *)"Warning: Unable to run in single instance mode: open() failed", FALSE );
            display_error( 0, (CHARTYPE *)strerror( errno ), FALSE );
         }
         else
         {
            current_file_name = first_file_name;
            while( current_file_name != NULL )
            {
               if ( ( rc = splitpath( current_file_name->line ) ) != RC_OK )
               {
                  display_error( 10, current_file_name->line, FALSE );
               }
               else
               {
                  /*
                   * If started with -r, set READONLY FORCE
                   */
                  if ( ro )
                     ronly = "#readonly force";
                  else
                     ronly = "";
                  /*
                   * If line and/or column specified on command line, use
                   * them to reposition file...
                   */
                  if ( startup_line != 0L
                  ||   startup_column != 0 )
                  {
                     tmp_len = sprintf( (DEFCHAR *)tmp_str, "x %s%s#cursor goto %ld %ld%s",
                                   sp_path, sp_fname,
                                   (startup_line) ? startup_line : 1,
                                   (startup_column) ? startup_column : 1,
                                   ronly );
                  }
                  else
                  {
                     tmp_len = sprintf( (DEFCHAR *)tmp_str, "x %s%s%s", sp_path, sp_fname, ronly );
                  }
                  if ( write( fifo_fd, &tmp_len, sizeof( tmp_len ) ) == (-1) )
                  {
                     display_error( 0, (CHARTYPE *)strerror( errno ), FALSE );
                  }
                  if ( write( fifo_fd, tmp_str, tmp_len ) == (-1) )
                  {
                     display_error( 0, (CHARTYPE *)strerror( errno ), FALSE );
                  }
               }
               current_file_name = current_file_name->next;
            }
            close( fifo_fd );
            /*
             * We are the client, so return with 1 to enable the caller to stop
             */
            am_client = 1;
         }
      }
      else
      {
         display_error( 0, (CHARTYPE *)"Warning: Unable to run in single instance mode: fifo not writable", FALSE );
         display_error( 0, (CHARTYPE *)strerror( errno ), FALSE );
      }
   }
   else
   {
      /*
       * The FIFO doesn't exists, so we assume we are the server here...
       */
      if ( mkfifo( (DEFCHAR*)fifo_name, S_IWUSR | S_IRUSR ) == (-1) )
      {
         display_error( 0, (CHARTYPE *)"Warning: Unable to run in single instance mode: mkfifo() failed", FALSE );
         display_error( 0, (CHARTYPE *)strerror( errno ), FALSE );
      }
      else
      {
         fifo_fd = open( (DEFCHAR*)fifo_name, O_RDWR );
         if ( fifo_fd == -1 )
         {
            display_error( 0, (CHARTYPE *)"Warning: Unable to run in single instance mode open() failed:", FALSE );
            display_error( 0, (CHARTYPE *)strerror( errno ), FALSE );
         }
         else
         {
            single_instance_server = TRUE;
         }
      }
      /*
       * We are the server, so return with 0 to enable the caller to continue...
       */
   }
   TRACE_RETURN();
   return am_client;
}

/***********************************************************************/
#ifdef HAVE_PROTO
int process_fifo_input( int key )
#else
int process_fifo_input( key )
int key;
#endif
/***********************************************************************/
{
   int s;
   fd_set readfds;
   int curses_fd;
   bool le_status=CURRENT_VIEW->linend_status;
   CHARTYPE le_value=CURRENT_VIEW->linend_value;
   VIEW_DETAILS *le_view;

   TRACE_FUNCTION("single.c:  process_fifo_input");

   if ( key == -1 )
   {
      /*
       *
       * Add curses input and the input fifo
       */
#ifdef USE_XCURSES
      curses_fd = PDC_get_input_fd();
#else
      curses_fd = fileno( stdin );
#endif
      FD_ZERO ( &readfds );
      FD_SET ( curses_fd, &readfds );
      FD_SET ( fifo_fd, &readfds );
      if ( ( s = select ( FD_SETSIZE, &readfds, NULL, NULL, NULL ) ) < 0 )
      {
         TRACE_RETURN();
         return key;
      }
      if ( s == 0 ) /* no requests pending - should never happen! */
      {
         TRACE_RETURN();
         return key;
      }
      if ( FD_ISSET ( fifo_fd, &readfds ) )
      {
         if ( read( fifo_fd, (char *)&tmp_len, sizeof(tmp_len) ) < 0 )
         {
            TRACE_RETURN();
            return key;
         }
         /*
          * Sleep for 100 milliseconds to ensure the remainder of the
          * data is in the fifo. Yuck!
          */
         napms( 100 );
         if ( read( fifo_fd, tmp_str, tmp_len*sizeof(CHARTYPE) ) < 0 )
         {
            TRACE_RETURN();
            return key;
         }
         /*
          * Save the current linend.
          * Force LINEND to # (which is what initialise_lifo() sets).
          * Run the command
          * set LINEND back to default.
          */
         tmp_str[tmp_len] = '\0';
         le_status = CURRENT_VIEW->linend_status;
         le_value = CURRENT_VIEW->linend_value;
         le_view = CURRENT_VIEW;
         CURRENT_VIEW->linend_status = TRUE;
         CURRENT_VIEW->linend_value = '#';
         (void)command_line( tmp_str, TRUE );
         le_view->linend_status = le_status;
         le_view->linend_value = le_value;
         key = 0;
      }
   }
   TRACE_RETURN();
   return key;
}

/***********************************************************************/
#ifdef HAVE_PROTO
void close_fifo( void )
#else
void close_fifo( )
#endif
/***********************************************************************/
{
   TRACE_FUNCTION("single.c:  close_fifo");
   close( fifo_fd );
   remove_file( fifo_name );
   TRACE_RETURN();
   return;
}
#endif

#endif
