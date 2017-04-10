/***********************************************************************/
/* NONANSI.C -                                                         */
/* This file contains all calls to non-ansi conforming routines.       */
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

#if defined(UNIX) || defined(__EMX__)
# include <pwd.h>
#endif
#include <errno.h>

/*#define DEBUG 1*/
#ifdef __EMX__            /* prevent DOS- and OS2-includes    */
#elif defined(DOS)
#  include <dos.h>
#  if !defined(GO32)
#    include <direct.h>
#  endif
#elif defined(OS2)
#  include <direct.h>
#  include <io.h>
#  ifndef S_IFMT
#    define S_IFMT 0xF000
#  endif
#endif

#if defined(WIN32) && defined(_MSC_VER)
# include <direct.h>
#endif
/*
 * Replace non-ANSI defs with ANSI ones
 */
#ifndef S_IFMT
# if defined(__S_IFMT)
#  define S_IFMT __S_IFMT
# elif defined(_S_IFMT)
#  define S_IFMT _S_IFMT
# endif
#endif

/***********************************************************************/
#ifdef HAVE_PROTO
short file_readable(CHARTYPE *filename)
#else
short file_readable(filename)
CHARTYPE *filename;
#endif
/***********************************************************************/
{
   TRACE_FUNCTION("nonansi.c: file_readable");
   if (access((DEFCHAR *)filename,R_OK) == (-1))
   {
      TRACE_RETURN();
      return(FALSE);
   }
   TRACE_RETURN();
   return(TRUE);
}
/***********************************************************************/
#ifdef HAVE_PROTO
short file_writable(CHARTYPE *filename)
#else
short file_writable(filename)
CHARTYPE *filename;
#endif
/***********************************************************************/
{
   TRACE_FUNCTION("nonansi.c: file_writable");
   if ( file_exists( filename ) != THE_FILE_EXISTS )
   {
      TRACE_RETURN();
      return(TRUE);
   }
   if (access((DEFCHAR *)filename,W_OK) == (-1))
   {
      TRACE_RETURN();
      return(FALSE);
   }
   TRACE_RETURN();
   return(TRUE);
}
/***********************************************************************/
#ifdef HAVE_PROTO
short file_exists(CHARTYPE *filename)
#else
short file_exists(filename)
CHARTYPE *filename;
#endif
/***********************************************************************/
{
   int rc;
   TRACE_FUNCTION("nonansi.c: file_exists");
   if ( access( (DEFCHAR *)filename, F_OK ) == (-1) )
   {
      rc = errno;
      switch( rc )
      {
#ifdef ENAMETOOLONG
         case ENAMETOOLONG:
            rc = THE_FILE_NAME_TOO_LONG;
            break;
#endif
         default:
            rc = THE_FILE_UNKNOWN;
            break;
      }
      TRACE_RETURN();
      return((short)rc);
   }
   else
   {
      TRACE_RETURN();
      return(THE_FILE_EXISTS);
   }
}
/***********************************************************************/
#ifdef HAVE_PROTO
short remove_file(CHARTYPE *filename)
#else
short remove_file(filename)
CHARTYPE *filename;
#endif
/***********************************************************************/
{
   TRACE_FUNCTION("nonansi.c: remove_file");
   if (filename == NULL)
   {
      TRACE_RETURN();
      return(RC_OK);
   }
#ifdef VMS
   if (delete(filename) == (-1))
#else
   if (unlink((DEFCHAR *)filename) == (-1))
#endif
   {
      TRACE_RETURN();
      return(RC_ACCESS_DENIED);
   }
   TRACE_RETURN();
   return(RC_OK);
}
/***********************************************************************/
#ifdef HAVE_PROTO
void convert_equals_in_filename(CHARTYPE *outfilename,CHARTYPE *infilename)
#else
void convert_equals_in_filename(outfilename,infilename)
CHARTYPE *outfilename,*infilename;
#endif
/***********************************************************************/
{
  /*
   * Support an = in the following circumstances:
   *    In filename      Current           Current         Substitutes
   *                   \oldp\fred.c      \oldp\fred
   * ---------------------------------------------------------------
   * 1) =\abc.def      \oldp\abc.def     \oldp\abc.def     fpath
   * 2) \apath\=.x     \apath\fred.x     \apath\fred.x     fname
   * 3) \apath\abc.=   \apath\abc.c      \apath\abc.       ftype
   * 4) =.x            \oldp\fred.x      \oldp\fred.x      fpath and fname
   * 5) abc.=          abc.c             abc.              ftype
   * 6) =              \oldp\fred.c      \oldp\fred        fpath, fname and ftype
   * 7) \apath\=       \apath\fred.c     \apath\fred       filename
   */
/*--------------------------- local data ------------------------------*/
   CHARTYPE _THE_FAR in_filename[MAX_FILE_NAME+1] ;
   CHARTYPE _THE_FAR current_filename[MAX_FILE_NAME+1] ;
   CHARTYPE *in_ftype,*in_fpath,*in_fname;
   CHARTYPE *current_ftype,*current_fpath,*current_fname;
   LINETYPE last_pos;
#if defined(DOS) || defined(OS2) || (defined(WIN32) && !defined(__CYGWIN32__)) || defined(__EMX__)
   CHARTYPE *in_fmode;
   CHARTYPE *current_fmode;
#endif
/*--------------------------- processing ------------------------------*/
   TRACE_FUNCTION("nonansi.c: convert_equals_in_filename");
   /*
    * If we don't have a current file, or there are no equivalence chars
    * just copy the incoming filename to the outgoing filename...
    */
   if ( CURRENT_VIEW == NULL
   ||   CURRENT_FILE == NULL
   ||   strzreveq( infilename, (CHARTYPE)EQUIVCHARx ) == (-1) )
   {
      strcpy( (DEFCHAR *)outfilename, (DEFCHAR *)infilename );
      TRACE_RETURN();
      return;
   }
  /*
   * Split the incoming file name into 2 or 3 pieces; fpath/filename or
   * fpath/fname/ftype.
   */
   strcpy( (DEFCHAR *)in_filename, (DEFCHAR *)strrmdup( strtrans( infilename, OSLASH, ISLASH), EQUIVCHARx, TRUE ) );
#if defined(DOS) || defined(OS2) || (defined(WIN32) && !defined(__CYGWIN32__)) || defined(__EMX__)
   last_pos = strzreveq( in_filename, (CHARTYPE)':' );
   if ( last_pos  == (-1) )
   {
      in_fmode = NULL;
      in_fpath = in_filename;
   }
   else
   {
      in_fmode = in_filename;
      in_filename[last_pos] = '\0';
      in_fpath = in_filename + last_pos + 1;
   }
#else
   in_fpath = in_filename;
#endif
   last_pos = strzreveq( in_fpath, (CHARTYPE)ISLASH );
   if ( last_pos  == (-1) )
   {
      in_fpath = NULL;
      in_fname = in_filename;
   }
   else
   {
      in_fpath[last_pos] = '\0';
      in_fname = in_fpath + last_pos + 1;
   }
   last_pos = strzreveq( in_fname, (CHARTYPE)'.' );
   if ( last_pos  == (-1) )
      in_ftype = NULL;
   else
   {
      in_ftype = in_fname + last_pos + 1;
      in_fname[last_pos] = '\0';
   }
   /*
    * Split the current filename and path into its component parts
    */
   strcpy( (DEFCHAR *)current_filename, (DEFCHAR *)CURRENT_FILE->fpath );
   strcat( (DEFCHAR *)current_filename, (DEFCHAR *)CURRENT_FILE->fname );
#if defined(DOS) || defined(OS2) || (defined(WIN32) && !defined(__CYGWIN32__)) || defined(__EMX__)
   last_pos = strzreveq( current_filename, (CHARTYPE)':' );
   if ( last_pos  == (-1) )
   {
      current_fmode = NULL;
      current_fpath = current_filename;
   }
   else
   {
      current_fmode = current_filename;
      current_filename[last_pos] = '\0';
      current_fpath = current_filename + last_pos + 1;
   }
#else
   current_fpath = current_filename;
#endif
   last_pos = strzreveq( current_fpath, (CHARTYPE)ISLASH );
   if ( last_pos  == (-1) )
   {
      current_fpath = NULL;
      current_fname = current_filename;
   }
   else
   {
      current_fpath[last_pos] = '\0';
      current_fname = current_fpath + last_pos + 1;
   }
   last_pos = strzreveq( current_fname, (CHARTYPE)'.' );
   if ( last_pos  == (-1) )
      current_ftype = NULL;
   else
   {
      current_ftype = current_fname + last_pos + 1;
      current_fname[last_pos] = '\0';
   }
   /*
    * Now its time to put the new file name together
    */
#if defined(DOS) || defined(OS2) || (defined(WIN32) && !defined(__CYGWIN32__)) || defined(__EMX__)
   if ( in_fmode && !equal( in_fmode, EQUIVCHARstr, 1 ) )
      strcpy( (DEFCHAR *)outfilename, (DEFCHAR *)in_fmode );
   else
      strcpy( (DEFCHAR *)outfilename, (DEFCHAR *)current_fmode );
   strcat( (DEFCHAR *)outfilename, ":" );
#else
   strcpy( (DEFCHAR *)outfilename, "" );
#endif
   if ( in_fpath && !equal( in_fpath, EQUIVCHARstr, 1 ) )
      strcat( (DEFCHAR *)outfilename, (DEFCHAR *)in_fpath );
   else
      strcat( (DEFCHAR *)outfilename, (DEFCHAR *)current_fpath );
   strcat( (DEFCHAR *)outfilename, (DEFCHAR *)ISTR_SLASH );
   if ( in_fname && !equal( in_fname, EQUIVCHARstr, 1 ) )
      strcat( (DEFCHAR *)outfilename, (DEFCHAR *)in_fname );
   else
      strcat( (DEFCHAR *)outfilename, (DEFCHAR *)current_fname );
   if ( in_ftype && !equal( in_ftype, EQUIVCHARstr, 1 ) )
   {
      strcat( (DEFCHAR *)outfilename, "." );
      strcat( (DEFCHAR *)outfilename, (DEFCHAR *)in_ftype );
   }
   else
   {
      if ( current_ftype )
      {
         strcat( (DEFCHAR *)outfilename, "." );
         strcat( (DEFCHAR *)outfilename, (DEFCHAR *)current_ftype );
      }
   }
   TRACE_RETURN();
   return;
}

#if defined(DOS) || defined(OS2) || (defined(WIN32) && !defined(__CYGWIN32__)) || defined(__EMX__)
/***********************************************************************/
#ifdef HAVE_PROTO
short splitpath(CHARTYPE *filename)
#else
short splitpath(filename)
CHARTYPE *filename;
#endif
/***********************************************************************/
{
   LENGTHTYPE len=0;
   CHARTYPE _THE_FAR work_filename[MAX_FILE_NAME+1] ;
   CHARTYPE _THE_FAR conv_filename[MAX_FILE_NAME+1] ;
   CHARTYPE _THE_FAR current_dir[MAX_FILE_NAME+1] ;
#ifdef __EMX__
   int new_dos_disk=0,current_dos_disk=0,temp_disk=0;
#elif defined(DOS)
# if defined(__WATCOMC__)
   unsigned int new_dos_disk=0,current_dos_disk=0;        /* 1 - A,2 - B... */
   unsigned int temp_disk=0;
# else
   short new_dos_disk=0,current_dos_disk=0;        /* 1 - A,2 - B... */
   short temp_disk=0;
# endif
#elif defined (OS2)
   ULONG logical_os2_drives=0L;
# if defined( __32BIT__) || defined(__386__)
   ULONG new_dos_disk=0L,current_dos_disk=0L,temp_disk=0L;
# else
   USHORT new_dos_disk=0,current_dos_disk=0,temp_disk=0;
# endif
#elif defined(WIN32) && !defined(__CYGWIN32__)
   unsigned int new_dos_disk=0,current_dos_disk=0;        /* 1 - A,2 - B... */
   unsigned int temp_disk=0;
#endif

   TRACE_FUNCTION("nonansi.c: splitpath");

   if ( strlen( (DEFCHAR *)filename ) > MAX_FILE_NAME )
   {
      TRACE_RETURN();
      return(RC_BAD_FILEID);
   }
   /*
    * Save the current directory.
    */
#if defined(EMX)
   if ( _getcwd2( curr_path, MAX_FILE_NAME ) == NULL )
#else
   if ( getcwd( (DEFCHAR *)curr_path, MAX_FILE_NAME ) == NULL )
#endif
   {
      TRACE_RETURN();
      return(RC_BAD_FILEID);
   }
   strcpy(sp_path,"");
   strcpy(sp_fname,"");
   convert_equals_in_filename( conv_filename, filename );
   strcpy( work_filename, conv_filename );
   /*
    * If the supplied filename is empty, set the path = cwd and filename
    * equal to blank.
    */
   if (strcmp(filename,"") == 0)
   {
#if defined(__EMX__)
      _getcwd2(sp_path,MAX_FILE_NAME);
#else
      getcwd(sp_path,MAX_FILE_NAME);
#endif
      strcpy(sp_fname,"");
   }
   /*
    * For DOS and OS/2, get current drive.
    */
#ifdef __EMX__
   current_dos_disk = (_getdrive() - 'A') + 1;
#elif defined(DOS)
# if defined(TC) || defined(GO32)
   current_dos_disk = (short)(getdisk() + 1);
# endif
# if defined(MSC)
   _dos_getdrive(&current_dos_disk);
# endif
#elif defined (OS2)
   DosQueryCurrentDisk(&current_dos_disk,&logical_os2_drives);
#else  /* WIN32 */
# if defined(__WATCOMC__)
   _dos_getdrive(&current_dos_disk);
# else /* assume MSC */
   current_dos_disk = _getdrive();
# endif
#endif
   new_dos_disk = current_dos_disk;
   /*
    * For DOS and OS/2, if a drive specified determine the drive number.
    */
   if (*(conv_filename+1) == ':')/* we assume this means a drive secification */
      new_dos_disk = (toupper(*(conv_filename)) - 'A') + 1;
   /*
    * For DOS and OS/2, change to the specified disk (if supplied and
    * different). Validate the drive number.
    */
   if (new_dos_disk != current_dos_disk)
   {
#if defined(__EMX__)
      _chdrive((char)((new_dos_disk-1)+'A'));
      temp_disk = (_getdrive() - 'A') + 1;
#elif defined(DOS)
# if defined(TC) || defined(GO32)
      setdisk((short)(new_dos_disk-1));
      temp_disk = getdisk()+1;
# else   /* assume MSC */
      _dos_setdrive(new_dos_disk,&temp_disk);
      _dos_getdrive(&temp_disk);
# endif
#elif defined(OS2)
      DosSetDefaultDisk(new_dos_disk);
      DosQueryCurrentDisk(&temp_disk,&logical_os2_drives);
#else   /* assume WIN32 */
# if defined(__WATCOMC__)
      _dos_setdrive(new_dos_disk,&temp_disk);
      _dos_getdrive(&temp_disk);
# else  /* assume MSC */
      _chdrive(new_dos_disk);
      temp_disk = _getdrive();
# endif
#endif
      if (temp_disk != new_dos_disk)  /* invalid drive */
      {
         TRACE_RETURN();
         return (RC_BAD_DRIVE);
      }
   }
   /*
    * Save the current working directory on the specified drive, or the
    * current drive if not specified.
    */
#if defined(__EMX__)
   _getcwd2(current_dir,MAX_FILE_NAME);
#else
   getcwd(current_dir,MAX_FILE_NAME);
#endif
   /*
    * If the work_filename contains a drive specifier, special handling is
    * needed.
    */
   switch(strlen(conv_filename))
   {
      case 1:
         break;
      case 2:
      /*
       * If the filename consists only of a drive specifier, copy the current
       * directory for the now new drive into work_filename.
       */
         if (*(conv_filename+1) == ':')
            strcpy(work_filename,current_dir);
         break;
      default:
         if (*(conv_filename+1) == ':'
         &&  *(conv_filename+2) != ISLASH)
         {
            strcpy(work_filename,current_dir);
            if (current_dir[strlen(current_dir)-1] != ISLASH)
               strcat(work_filename,ISTR_SLASH);
            strcat(work_filename,conv_filename+2);
         }
         break;
   }
   /*
    * First determine if the supplied filename is a directory.
    */
   if (chdir(work_filename) == 0)  /* valid directory */
   {
#if defined(__EMX__)
      _getcwd2(sp_path,MAX_FILE_NAME);
#else
      getcwd(sp_path,MAX_FILE_NAME);
#endif
      strcpy(sp_fname,"");
   }
   else          /* here if the file is not a directory */
   {
      len = strzreveq(work_filename,ISLASH);
      switch(len)
      {
         case (-1):
#if defined(__EMX__)
            _getcwd2(sp_path,MAX_FILE_NAME);
#else
            getcwd(sp_path,MAX_FILE_NAME);
#endif
            strcpy(sp_fname,work_filename);
            break;
         case 0:
            strcpy(sp_path,work_filename);
            sp_path[1] = '\0';
            strcpy(sp_fname,work_filename+1+len);
            break;
         default:
            strcpy(sp_path,work_filename);
            sp_path[len] = '\0';
            strcpy(sp_fname,work_filename+1+len);
            break;
      }
   }
   if (strlen(sp_path) == 2
   && sp_path[1] == ':')
      strcat(sp_path,ISTR_SLASH);
   /*
    * Change directory to the supplied path, if possible and store the
    * expanded path.
    * If an error, restore the current path.
    */
   if (chdir(sp_path) != 0)
   {
      if (new_dos_disk != current_dos_disk)
      {
         chdir(current_dir);
#if defined(__EMX__)
         _chdrive((char)((current_dos_disk-1)+'A'));
#elif defined(DOS)
# if defined(TC) || defined(GO32)
         setdisk((short)(current_dos_disk-1));
# else   /* assume MSC */
         _dos_setdrive(current_dos_disk,&temp_disk);
# endif
#elif defined(OS2)
         DosSetDefaultDisk(new_dos_disk);
#else    /* assume WIN32 */
# if defined(__WATCOMC__)
         _dos_setdrive(current_dos_disk,&temp_disk);
# else   /* assume MSC */
         _chdrive(current_dos_disk);
# endif
#endif
      }
      chdir(curr_path);
      TRACE_RETURN();
      return(RC_FILE_NOT_FOUND);
   }
   /*
    * We are now in a valid directory, get the fully qualified directory
    * name.
    */
#if defined(__EMX__)
   _getcwd2(sp_path,MAX_FILE_NAME);
#else
   getcwd(sp_path,MAX_FILE_NAME);
#endif
   /*
    * For DOS or OS/2, change back to the current directory of the now
    * current disk and then change back to the original disk.
    */
   if (new_dos_disk != current_dos_disk)
   {
      if (chdir(current_dir) != 0)
      {
         TRACE_RETURN();
         return(RC_FILE_NOT_FOUND);
      }
#if defined(__EMX__)
      _chdrive((char)((current_dos_disk-1)+'A'));
#elif defined(DOS)
# if defined(TC) || defined(GO32)
      setdisk((short)(current_dos_disk-1));
# else /* assume MSC */
      _dos_setdrive(current_dos_disk,&temp_disk);
# endif
#elif defined(OS2)
      DosSetDefaultDisk(current_dos_disk);
#else  /* assume WIN32 */
# if defined(__WATCOMC__)
      _dos_setdrive(current_dos_disk,&temp_disk);
# else /* assume MSC */
      _chdrive(current_dos_disk);
# endif
#endif
   }
   chdir(curr_path);
   /*
    * Append the OS directory character to the path if it doesn't already
    * end in the character.
    */
   len = strlen(sp_path);
   if (len > 0)
   {
      if (sp_path[len-1] != ISLASH)
         strcat(sp_path,(CHARTYPE *)ISTR_SLASH);
      TRACE_RETURN();
      return(RC_OK);
   }
   TRACE_RETURN();
   return(RC_OK);
}
#elif defined(__QNX__) && !defined(__QNXNTO__)
/***********************************************************************/
#ifdef HAVE_PROTO
short splitpath(CHARTYPE *filename)
#else
short splitpath(filename)
CHARTYPE *filename;
#endif
/***********************************************************************/
{
   short len=0;
   CHARTYPE _THE_FAR work_filename[MAX_FILE_NAME+1] ;
   CHARTYPE _THE_FAR conv_filename[MAX_FILE_NAME+1] ;

   TRACE_FUNCTION("nonansi.c: splitpath");

   if ( strlen( (DEFCHAR *)filename ) > MAX_FILE_NAME )
   {
      TRACE_RETURN();
      return(RC_BAD_FILEID);
   }
   /*
    * Copy the argument to a working area
    */
   if ( getcwd( (DEFCHAR *)curr_path, MAX_FILE_NAME ) == NULL )
   {
      TRACE_RETURN();
      return(RC_BAD_FILEID);
   }
   strcpy((DEFCHAR *)sp_path,"");
   strcpy((DEFCHAR *)sp_fname,"");
   convert_equals_in_filename(conv_filename,filename);
   strcpy( work_filename, conv_filename );
   /*
    * If the supplied filename is empty, set the path = cwd and filename
    * equal to blank.
    */
   if (strcmp((DEFCHAR *)filename,"") == 0)
   {
      getcwd((DEFCHAR *)sp_path,MAX_FILE_NAME);
      strcpy((DEFCHAR *)sp_fname,"");
   }
   /*
    * Check if the first character is tilde; translate HOME env variable
    * if there is one. Obviously only applicable to UNIX.
    */
   if (*(conv_filename) == '~')
   {
      if (*(conv_filename+1) == ISLASH
      ||  *(conv_filename+1) == '\0')
      {
         strcpy((DEFCHAR *)work_filename,(DEFCHAR *)getenv("HOME"));
         strcat((DEFCHAR *)work_filename,(DEFCHAR *)(conv_filename+1));
      }
      else
      {
         struct passwd *pwd;

         strcpy((DEFCHAR *)sp_path,(DEFCHAR *)conv_filename+1);
         if ((len = strzeq(sp_path,ISLASH)) != (-1))
            sp_path[len] = '\0';
         if ((pwd = getpwnam((DEFCHAR *)sp_path)) == NULL)
         {
            TRACE_RETURN();
            return(RC_BAD_FILEID);
         }
         strcpy((DEFCHAR *)work_filename,pwd->pw_dir);
         if (len != (-1))
            strcat((DEFCHAR *)work_filename,(DEFCHAR *)(conv_filename+1+len));
      }
   }
   /*
    * First determine if the supplied filename is a directory.
    */
   if (chdir(work_filename) == 0)
   {
      chdir(curr_path);
      strcpy((DEFCHAR *)sp_path,(DEFCHAR *)work_filename);
      strcpy((DEFCHAR *)sp_fname,"");
   }
   else      /* here if the file doesn't exist or is not a directory */
   {
      len = strzreveq(work_filename,ISLASH);
      switch(len)
      {
         case (-1):
            getcwd((DEFCHAR *)sp_path,MAX_FILE_NAME);
            strcpy((DEFCHAR *)sp_fname,(DEFCHAR *)work_filename);
            break;
         case 0:
            strcpy((DEFCHAR *)sp_path,(DEFCHAR *)work_filename);
            sp_path[1] = '\0';
            strcpy((DEFCHAR *)sp_fname,(DEFCHAR *)work_filename+1+len);
            break;
        default:
            strcpy((DEFCHAR *)sp_path,(DEFCHAR *)work_filename);
            sp_path[len] = '\0';
            strcpy((DEFCHAR *)sp_fname,(DEFCHAR *)work_filename+1+len);
            break;
      }
   }
   /*
    * Change directory to the supplied path, if possible and store the
    * expanded path.
    * If an error, restore the current path.
    */
   if (qnx_fullpath((DEFCHAR *)work_filename,(DEFCHAR *)sp_path) == NULL)
   {
      chdir((DEFCHAR *)curr_path);
      TRACE_RETURN();
      return(RC_FILE_NOT_FOUND);
   }
   strcpy(sp_path,work_filename);
   chdir((DEFCHAR *)curr_path);
   /*
    * Append the OS directory character to the path if it doesn't already
    * end in the character.
    */
   len = strlen((DEFCHAR *)sp_path);
   if (len > 0)
   {
      if (sp_path[len-1] != ISLASH)
         strcat((DEFCHAR *)sp_path,(DEFCHAR *)ISTR_SLASH);
   }
   TRACE_RETURN();
   return(RC_OK);
}
#elif defined(AMIGA) && defined(GCC)
/***********************************************************************/
#ifdef HAVE_PROTO
short splitpath(CHARTYPE *filename)
#else
short splitpath(filename)
CHARTYPE *filename;
#endif
/***********************************************************************/
{
   short len=0;
   CHARTYPE _THE_FAR work_filename[MAX_FILE_NAME+1] ;

   TRACE_FUNCTION("nonansi.c: splitpath");

   if ( strlen( (DEFCHAR *)filename ) > MAX_FILE_NAME )
   {
      TRACE_RETURN();
      return(RC_BAD_FILEID);
   }
   /*
    * Save the current directory.
    */
   if ( getcwd( (DEFCHAR *)curr_path, MAX_FILE_NAME ) == NULL )
   {
      TRACE_RETURN();
      return(RC_BAD_FILEID);
   }
   strcpy((DEFCHAR *)sp_path,"");
   strcpy((DEFCHAR *)sp_fname,"");
   convert_equals_in_filename(work_filename,filename);
   /*
    * If the supplied filename is empty, set the path = cwd and filename
    * equal to blank.
    */
   if (strcmp((DEFCHAR *)filename,"") == 0)
   {
      getcwd((DEFCHAR *)sp_path,MAX_FILE_NAME);
      strcpy((DEFCHAR *)sp_fname,"");
   }
   /*
    * First determine if the supplied filename is a directory.
    */
   if ( ( stat((DEFCHAR *)work_filename, &stat_buf ) == 0 )
   &&   ( is_a_dir_stat( stat_buf.st_mode ) ) )
   {
      strcpy((DEFCHAR *)sp_path,(DEFCHAR *)work_filename);
      strcpy((DEFCHAR *)sp_fname,"");
   }
   else          /* here if the file doesn't exist or is not a directory */
   {
      len = strzreveq(work_filename,ISLASH);
      switch(len)
      {
         case (-1):
            getcwd((DEFCHAR *)sp_path,MAX_FILE_NAME);
            strcpy((DEFCHAR *)sp_fname,(DEFCHAR *)work_filename);
            break;
         case 0:
            strcpy((DEFCHAR *)sp_path,(DEFCHAR *)work_filename);
            sp_path[1] = '\0';
            strcpy((DEFCHAR *)sp_fname,(DEFCHAR *)work_filename+1+len);
            break;
         default:
            strcpy((DEFCHAR *)sp_path,(DEFCHAR *)work_filename);
            sp_path[len] = '\0';
            strcpy((DEFCHAR *)sp_fname,(DEFCHAR *)work_filename+1+len);
            break;
      }
   }
   /*
    * Change directory to the supplied path, if possible and store the
    * expanded path.
    * If an error, restore the current path.
    */
   if (chdir((DEFCHAR *)sp_path) != 0)
   {
      chdir((DEFCHAR *)curr_path);
      TRACE_RETURN();
      return(RC_FILE_NOT_FOUND);
   }
   getcwd((DEFCHAR *)sp_path,MAX_FILE_NAME);
   chdir((DEFCHAR *)curr_path);
   /*
    * Append the OS directory character to the path if it doesn't already
    * end in the character.
    */
   len = strlen((DEFCHAR *)sp_path);
   if ( len > 0
   &&   sp_path[len-1] != ISLASH )
      strcat((DEFCHAR *)sp_path,(DEFCHAR *)ISTR_SLASH);
   TRACE_RETURN();
   return(RC_OK);
}
#else
/***********************************************************************/
#ifdef HAVE_PROTO
short splitpath(CHARTYPE *filename)
#else
short splitpath(filename)
CHARTYPE *filename;
#endif
/***********************************************************************/
{
   short len=0;
   CHARTYPE _THE_FAR work_filename[MAX_FILE_NAME+1] ;
   CHARTYPE _THE_FAR conv_filename[MAX_FILE_NAME+1] ;

   TRACE_FUNCTION("nonansi.c: splitpath");

   if ( strlen( (DEFCHAR *)filename ) > MAX_FILE_NAME )
   {
      TRACE_RETURN();
      return(RC_BAD_FILEID);
   }
   /*
    * Save the current directory.
    */
#if defined(EMX)
   if ( _getcwd2( curr_path, MAX_FILE_NAME ) == NULL )
#else
   if ( getcwd( (DEFCHAR *)curr_path, MAX_FILE_NAME ) == NULL )
#endif
   {
      TRACE_RETURN();
      return(RC_BAD_FILEID);
   }
   strcpy( (DEFCHAR *)sp_path, "" );
   strcpy( (DEFCHAR *)sp_fname, "" );
   convert_equals_in_filename( conv_filename, filename );
   if ( strlen( (DEFCHAR *)conv_filename ) > MAX_FILE_NAME )
   {
      TRACE_RETURN();
      return(RC_BAD_FILEID);
   }
   strcpy( (DEFCHAR *)work_filename, (DEFCHAR *)conv_filename );
   /*
    * If the supplied filename is empty, set the path = cwd and filename
    * equal to blank.
    */
   if (strcmp((DEFCHAR *)filename,"") == 0)
   {
      if ( getcwd( (DEFCHAR *)sp_path, MAX_FILE_NAME ) == NULL )
      {
         TRACE_RETURN();
         return(RC_BAD_FILEID);
      }
      strcpy((DEFCHAR *)sp_fname,"");
   }
   /*
    * Check if the first character is tilde; translate HOME env variable
    * if there is one. Obviously only applicable to UNIX.
    */
#ifndef VMS
   if (*(conv_filename) == '~')
   {
      if (*(conv_filename+1) == ISLASH
      ||  *(conv_filename+1) == '\0')
      {
         CHARTYPE *home = (CHARTYPE *)getenv("HOME");
         if ( ((home==NULL) ? 0 : strlen( (DEFCHAR *)home )) + strlen( (DEFCHAR *)conv_filename ) > MAX_FILE_NAME )
         {
            TRACE_RETURN();
            return(RC_BAD_FILEID);
         }
         strcpy( (DEFCHAR *)work_filename, (DEFCHAR *)home );
         strcat( (DEFCHAR *)work_filename, (DEFCHAR *)(conv_filename+1));
      }
      else
      {
         struct passwd *pwd;

         strcpy((DEFCHAR *)sp_path,(DEFCHAR *)conv_filename+1);
         if ((len = strzeq(sp_path,ISLASH)) != (-1))
            sp_path[len] = '\0';
         if ((pwd = getpwnam((DEFCHAR *)sp_path)) == NULL)
         {
            TRACE_RETURN();
            return(RC_BAD_FILEID);
         }
         strcpy((DEFCHAR *)work_filename,pwd->pw_dir);
         if (len != (-1))
            strcat((DEFCHAR *)work_filename,(DEFCHAR *)(conv_filename+1+len));
      }
   }
#endif
   /*
    * First determine if the supplied filename is a directory.
    */
   if ( ( stat( (DEFCHAR *)work_filename, &stat_buf ) == 0 )
   &&   ( is_a_dir_stat( stat_buf.st_mode ) ) )
   {
      strcpy((DEFCHAR *)sp_path,(DEFCHAR *)work_filename);
      strcpy((DEFCHAR *)sp_fname,"");
   }
   else          /* here if the file doesn't exist or is not a directory */
   {
      len = strzreveq(work_filename,ISLASH);
      switch(len)
      {
         case (-1):
            if ( getcwd( (DEFCHAR *)sp_path, MAX_FILE_NAME ) == NULL )
            {
               TRACE_RETURN();
               return(RC_BAD_FILEID);
            }
            strcpy((DEFCHAR *)sp_fname,(DEFCHAR *)work_filename);
            break;
         case 0:
            strcpy((DEFCHAR *)sp_path,(DEFCHAR *)work_filename);
            sp_path[1] = '\0';
            strcpy((DEFCHAR *)sp_fname,(DEFCHAR *)work_filename+1+len);
            break;
         default:
            strcpy((DEFCHAR *)sp_path,(DEFCHAR *)work_filename);
            sp_path[len] = '\0';
            strcpy((DEFCHAR *)sp_fname,(DEFCHAR *)work_filename+1+len);
            break;
      }
   }
   /*
    * Change directory to the supplied path, if possible and store the
    * expanded path.
    * If an error, restore the current path.
    */
   if (chdir((DEFCHAR *)sp_path) != 0)
   {
      chdir((DEFCHAR *)curr_path);
      TRACE_RETURN();
      return(RC_FILE_NOT_FOUND);
   }
   if ( getcwd( (DEFCHAR *)sp_path, MAX_FILE_NAME ) == NULL )
   {
      TRACE_RETURN();
      return(RC_BAD_FILEID);
   }
   chdir((DEFCHAR *)curr_path);
   /*
    * Append the OS directory character to the path if it doesn't already
    * end in the character.
    */
   len = strlen((DEFCHAR *)sp_path);
   if ( len > 0
   &&   sp_path[len-1] != ISLASH )
      strcat((DEFCHAR *)sp_path,(DEFCHAR *)ISTR_SLASH);
   TRACE_RETURN();
   return(RC_OK);
}
#endif

#ifndef HAVE_RENAME
/***********************************************************************/
#ifdef HAVE_PROTO
short rename(CHARTYPE *path1,CHARTYPE *path2)
#else
short rename(path1,path2)
CHARTYPE *path1;
CHARTYPE *path2;
#endif
/***********************************************************************/
/* Function  : Emulate missing rename() function missing from SystemV  */
/* Parameters: path1    - old filename                                 */
/*             path2    - new_filename                                 */
/* Return    : 0 on success, -1 on error                               */
/***********************************************************************/
{
   TRACE_FUNCTION("nonansi.c: rename");
   if ( link( (DEFCHAR *)path1, (DEFCHAR *)path2 ) != 0 )
   {
      TRACE_RETURN();
      return(-1);
   }
   if ( unlink( (DEFCHAR *)path1 ) != 0 )
   {
      TRACE_RETURN();
      return(-1);
   }
   TRACE_RETURN();
   return(0);
}
#endif

#if defined(OS2) || (defined(__EMX__) && !defined(MSDOS))
/***********************************************************************/
/* Function  : Determine if file system allows the supplied filename.  */
/* Parameters: PathFn - directory path and filename without ext.     */
/* Note      : replaces function LongFileNames                  */
/* Return    : 1 if file system allows this filename                 */
/***********************************************************************/
bool IsPathAndFilenameValid(CHARTYPE *PathFn)
{
   CHARTYPE _THE_FAR FullFn[CCHMAXPATH];
   CHARTYPE *buf;
   ULONG rc;

   TRACE_FUNCTION("nonansi.c: IsPathAndFilenameValid");
#ifdef __EMX__
   if (_osmode == DOS_MODE)
   {
      TRACE_RETURN();
      return(0);
   }
#endif
   if ((buf = malloc(strlen(PathFn) + 5)) == NULL)
   {
      TRACE_RETURN();
      return(0); /* Fake an error      */
   }
   strcpy(buf,PathFn);
   strcat(buf,".xxx");                    /* any extension                     */
#if defined(__32BIT__) || defined(__386__)
   rc = DosQueryPathInfo(buf,FIL_QUERYFULLNAME,FullFn,sizeof(FullFn));
#else
   rc = DosQPathInfo(buf,FIL_QUERYFULLNAME,FullFn,sizeof(FullFn),0ul);
#endif
   free(buf);
   if (rc == 0)
   {
      TRACE_RETURN();
      return(1);
   }
   TRACE_RETURN();
   return(0);
}
#endif

#ifdef USE_OLD_LONGFILENAMES /* FGC: previous ifdef OS2     */
#  if defined(__32BIT__) || defined(__386__)
#  define FSQBUFFERSIZE 64
/***********************************************************************/
bool LongFileNames(CHARTYPE *path)
/***********************************************************************/
/* Function  : Determine if file system allows long file names. (HPFS) */
/*             This is the 32-bit version.                             */
/* Parameters: path     - directory path                               */
/* Return    : 1 if file system is HPFS                                */
/***********************************************************************/
{
   ULONG nDrive=0L;
   ULONG lMap=0L;
   char _THE_FAR buffer[FSQBUFFERSIZE];
   FSQBUFFER2 *bData = (FSQBUFFER2 *) buffer;
   char bName[3];
   ULONG bDataLen=0L;

   TRACE_FUNCTION("nonansi.c: LongFileNames");
   if ((strlen (path) > 0) && path [1] == ':')
      bName[0] = path[0];
   else
   {
      DosQueryCurrentDisk(&nDrive, &lMap);
      bName[0] = (char) (nDrive + 'A' - 1);
   }
   bName[1] = ':';
   bName[2] = 0;
   bDataLen = FSQBUFFERSIZE;
   DosQueryFSAttach(bName, 0, FSAIL_QUERYNAME, bData, &bDataLen);
   TRACE_RETURN();
   return(strcmp(bData->szFSDName + bData->cbName, "HPFS") == 0);
}
#  else

/***********************************************************************/
bool LongFileNames(CHARTYPE *path)
/***********************************************************************/
/* Function  : Determine if file system allows long filenames. (HPFS)  */
/*             This is the 16-bit version.                             */
/* Parameters: path     - directory path                               */
/* Return    : 1 if file system is HPFS                                */
/***********************************************************************/
{
typedef struct _FSNAME {
        USHORT cbName;
        UCHAR  szName[1];
} FSNAME;
typedef struct _FSQINFO {
        USHORT iType;
        FSNAME Name;
        UCHAR  rgFSAData[59];
} FSQINFO;
typedef FSQINFO FAR *PFSQINFO;

   USHORT nDrive=0,cbData=0;
   ULONG lMap=0L;
   FSQINFO bData;
   BYTE bName[3];
   FSNAME *pFSName=NULL;

   TRACE_FUNCTION("nonansi.c: LongFileNames");
   if ((strlen(path) > 0) && path[1] == ':')
      bName[0] = path[0];
   else
   {
      DosQueryCurrentDisk(&nDrive, &lMap);
      bName[0] = (char)(nDrive + '@');
   }
   bName[1] = ':';
   bName[2] = 0;
   cbData = sizeof(bData);
   DosQFSAttach((PSZ)bName,0,1,(PBYTE)&bData,&cbData,0L);
   pFSName = &bData.Name;
   (CHARTYPE *)pFSName += pFSName->cbName + sizeof(pFSName->cbName)+1;
   TRACE_RETURN();
   return(strcmp((CHARTYPE *)&(pFSName->szName[0]),"HPFS") == 0);
}
#  endif
#endif

/***********************************************************************/
#ifdef HAVE_PROTO
LINE *getclipboard(LINE *now, int from_get)
#else
LINE *getclipboard(now,from_get)
LINE *now;
int from_get;
#endif
/*
 * Function  : Reads the contents of the clipboard into the file.
 * Parameters: pointer to line after which lines are to be added
 * Return    : 0 on success.
 */
{
   LINE *curr=now;
#if defined(PDC_CLIP_SUCCESS)
   LINE *clip_first_line=NULL;
   LINE *my_first_line=NULL;
   CHARTYPE *ptr=NULL;
   LINETYPE i,line_start,numlines=0,numcols=0;
   LENGTHTYPE maxlen=0;
   LENGTHTYPE length=0;
   short rc=RC_OK;
# if defined(UNIX) || defined(MAC)
   int offset = 0;
# else
   int offset = 1;
# endif
   int this_offset;
#endif

#if defined(PDC_CLIP_SUCCESS)
   TRACE_FUNCTION("nonansi.c: getclipboard");
   rc = PDC_getclipboard( (DEFCHAR **)&ptr, &length );

   switch( rc )
   {
      case PDC_CLIP_MEMORY_ERROR:
         display_error( 30, (CHARTYPE *)"", FALSE );
         rc = RC_OUT_OF_MEMORY;
         break;
      case PDC_CLIP_ACCESS_ERROR:
         display_error( 186, (CHARTYPE *)"", FALSE );
         rc = RC_INVALID_ENVIRON;
         break;
      case PDC_CLIP_EMPTY:
         display_error( 187, (CHARTYPE *)"", FALSE );
         rc = RC_INVALID_ENVIRON;
         break;
      default:
         rc = RC_OK;
         break;
   }
   if (rc != RC_OK)
   {
      TRACE_RETURN();
      return NULL;
   }

   line_start=0;
   maxlen = CURRENT_FILE->max_line_length;

   if ( from_get )
      my_first_line = CURRENT_FILE->first_line;
   else
   {
      my_first_line = clip_first_line;
      curr = NULL;
   }
   for (i=0; i<length; i++)
   {
      if ( i - line_start == max_line_length /* exceeding WIDTH */
      ||   *(ptr+i) == 0x0A ) /* CR may have been prior character */
      {
         if ( *(ptr+i) == 0x0A )
            this_offset = offset;
         else
            this_offset = 0;
         numcols = i - line_start - this_offset;
/*         *(ptr+i-offset) = '\0'; */
         if ( ( curr = add_LINE( my_first_line, curr, ptr+line_start, numcols, 0, TRUE ) ) == NULL )
         {
            curr = NULL;
            break;
         }
         if ( clip_first_line == NULL )
         {
            clip_first_line = my_first_line = curr;
         }
         if ( *(ptr+i) == 0x0A )
            line_start = i+1;
         else
            line_start = i;
         numlines++;
         if (i - line_start - this_offset > maxlen)
            maxlen = i - line_start - this_offset;
      }
   }
   if (line_start != i )
   {
      numcols = i-line_start;
      curr = add_LINE( my_first_line, curr, ptr+line_start, numcols, 0, TRUE );
      numlines++;
      if (i-line_start > maxlen)
         maxlen = i-line_start;
      if ( clip_first_line == NULL )
      {
         clip_first_line = my_first_line = curr;
      }
   }
   if ( !from_get )
   {
      /*
       * We have created our own source list of lines...
       *...we need to fix up the list; the last entry to have a NULL next pointer
       * numcols will be the length of the last line, but it is only used
       * when there is only 1 line.
       */
     box_paste_from_clipboard( clip_first_line, numlines, numcols );
     lll_free( clip_first_line );
   }

   PDC_freeclipboard( (DEFCHAR *)ptr );

   if ( from_get && curr)
   {
      CURRENT_FILE->max_line_length = maxlen;
      CURRENT_FILE->number_lines += numlines;
   }
#else
   display_error( 82, (CHARTYPE *)"CLIP:", FALSE );
   curr = NULL;
#endif
   TRACE_RETURN();
   return curr;
}
/***********************************************************************/
#ifdef HAVE_PROTO
short setclipboard(FILE_DETAILS *cf,CHARTYPE *new_fname,bool force,LINETYPE in_lines,
                LINETYPE start_line_in,LINETYPE end_line_in,LINETYPE *num_file_lines,bool append,LENGTHTYPE start_col_in, LENGTHTYPE end_col_in,bool ignore_scope,bool lines_based_on_scope,int target_type)
#else
short setclipboard(cf,new_fname,force,in_lines,start_line_in,end_line_in,num_file_lines,append,start_col_in,end_col_in,ignore_scope,lines_based_on_scope,target_type)
FILE_DETAILS *cf;
CHARTYPE *new_fname;
bool force,append;
LINETYPE in_lines,start_line_in,end_line_in;
LINETYPE *num_file_lines;
LENGTHTYPE start_col_in,end_col_in;
bool ignore_scope;
bool lines_based_on_scope;
int target_type;
#endif
/***********************************************************************/
{
#define CLIP_TYPE_LINE   1
#define CLIP_TYPE_BOX    2
#define CLIP_TYPE_STREAM 3
   LINETYPE i=0L;
   LINETYPE abs_num_lines=(in_lines < 0L ? -in_lines : in_lines);
   LINETYPE num_actual_lines=0L;
   LINETYPE my_num_file_lines=0L;
   LINETYPE current_line;
   short direction=(in_lines < 0L ? DIRECTION_BACKWARD : DIRECTION_FORWARD);
   LINE *curr=NULL;
   short rc=RC_OK;
   bool save_scope_all=CURRENT_VIEW->scope_all;
#ifdef UNIX
   CHARTYPE *eol = (CHARTYPE *)"\n";
   int eol_len=1;
#elif defined(MAC)
   CHARTYPE *eol = (CHARTYPE *)"\r";
   int eol_len=1;
#else
   CHARTYPE *eol = (CHARTYPE *)"\r\n";
   int eol_len=2;
#endif
   long clip_size=1024;
   CHARTYPE *ptr=NULL;
   long len=0,pos=0;
   int clip_type;
   LENGTHTYPE start_col=start_col_in,end_col=end_col_in;
   LINETYPE start_line=start_line_in,end_line=end_line_in;

   TRACE_FUNCTION("file.c:    setclipboard");
   /*
    * CUA block can have start position after end position, so fix this up so
    * start position is less than end position
    */
   if ( MARK_VIEW
   &&   MARK_VIEW->mark_type == M_CUA
   && ( start_line * max_line_length ) + start_col > (end_line * max_line_length ) + end_col )
   {
      start_line = end_line_in;
      start_col = end_col_in;
      end_line = start_line_in;
      end_col = start_col_in;
      /*
       * Now switch the "in" values...
       */
      start_line_in = start_line;
      end_line_in = end_line;
      start_col_in = start_col;
      end_col_in = end_col;
      /*
       * Fix lines and direction
       */
      direction = DIRECTION_FORWARD;
      in_lines = abs_num_lines;
   }
   /*
    * Determine where to start writing from in the linked list.
    */
   curr = lll_find(cf->first_line,cf->last_line,start_line,cf->number_lines);
   /*
    * Save the setting of scope_all if we are ignoring scope. ie full file
    * is being written...
    */
   if (ignore_scope)
      CURRENT_VIEW->scope_all = TRUE;
   /*
    * Allocate an initial amount of memory for the clipboard buffer.
    * It will be increased later if required.
    */
   ptr = (CHARTYPE *)(*the_malloc)(clip_size);
   if (!ptr)
   {
      display_error(30,(CHARTYPE *)"",FALSE);
      TRACE_RETURN();
      return(RC_OUT_OF_MEMORY);
   }

   rc = RC_OK;
   /*
    * We need to work out which part of the line needs to
    * be copied to the clipboard buffer, depending on what
    * type of target has been selected.
    * Effectively there are 3 different combinations of
    * lines to put in the clipboard:
    * 1) full lines - every line is the complete line
    *    BLOCK target with M_LINE type
    *    all other non-block targets
    * 2) boxes - every line is the same partial size
    *    M_BOX, M_WORD and M_COLUMN BLOCK target types
    *    plus M_STREAM and M_CUA when start_line = end_line
    * 3) streams - irregular line lengths
    *    first line starts at start_col goes to end of line
    *    last line starts at 0 and goes to end_col
    *    all other lines are complete lines
    */
   if ( target_type == TARGET_BLOCK_CURRENT )
   {
      if ( MARK_VIEW->mark_type == M_LINE )
      {
         clip_type = CLIP_TYPE_LINE;
      }
      else if ( MARK_VIEW->mark_type == M_BOX
      ||        MARK_VIEW->mark_type == M_WORD
      ||        MARK_VIEW->mark_type == M_COLUMN )
      {
         clip_type = CLIP_TYPE_BOX;
      }
      else if ( MARK_VIEW->mark_type == M_CUA
      &&        start_line == end_line )
      {
         clip_type = CLIP_TYPE_BOX;
      }
      else if ( MARK_VIEW->mark_type == M_STREAM
      &&        start_line == end_line )
      {
         clip_type = CLIP_TYPE_BOX;
      }
      else
      {
         clip_type = CLIP_TYPE_STREAM;
      }
   }
   else
   {
      clip_type = CLIP_TYPE_LINE;
   }
   /*
    * Processe each line...
    */
   for ( i = 0L, num_actual_lines = 0L; ; i++ )
   {
      if ( lines_based_on_scope )
      {
         if ( num_actual_lines == abs_num_lines )
            break;
      }
      else
      {
         if (abs_num_lines == i)
            break;
      }
      current_line = start_line+(LINETYPE)(i*direction);
      rc = processable_line( CURRENT_VIEW, current_line, curr );
      switch(rc)
      {
         case LINE_SHADOW:
              rc = 0;
              break;
         case LINE_TOF:
         case LINE_EOF:
              rc = 0;
              num_actual_lines++;
              break;
         default:
              rc = 0;
              /*
               * Depending on the type of line we need to work out
               * which columns to copy...
               */
              switch( clip_type )
              {
                 case CLIP_TYPE_LINE:
                    break;
                 case CLIP_TYPE_BOX:
                    break;
                 case CLIP_TYPE_STREAM:
                    if ( current_line == start_line )
                    {
                       start_col = start_col_in;
                       end_col = curr->length - 1;
                    }
                    else if ( current_line == end_line )
                    {
                       start_col = 0;
                       end_col = end_col_in;
                    }
                    else
                    {
                       start_col = 0;
                       end_col = curr->length - 1;
                    }
                    break;
              }
              if (start_col < curr->length )
              {
                 len = min( curr->length-start_col, (end_col - start_col) + 1 );
                 if ( pos + len > clip_size )
                 {
                    ptr = (CHARTYPE *)(*the_realloc)( (DEFCHAR *)ptr, clip_size * 2 );
                    clip_size *= 2;
                 }
                 memcpy( (DEFCHAR *)ptr+pos, (DEFCHAR *)curr->line+start_col, len );
                 pos += len;
              }
              if ( pos + 1 + eol_len > clip_size )
              {
                 ptr = (CHARTYPE *)(*the_realloc)( (DEFCHAR *)ptr, clip_size * 2 );
                 if ( !ptr )
                 {
                    display_error( 30, (CHARTYPE *)"", FALSE );
                    TRACE_RETURN();
                    return( RC_OUT_OF_MEMORY );
                 }
                 clip_size *= 2;
              }
              memcpy((DEFCHAR *)ptr+pos,(DEFCHAR *)eol,eol_len);
              pos += eol_len;
              num_actual_lines++;
              break;
      }
      /*
       * Proceed to the next record, even if the current record not in scope.
       */
      if (rc)
         break;
      if (direction == DIRECTION_BACKWARD)
         curr = curr->prev;
      else
         curr = curr->next;
      my_num_file_lines += (LINETYPE)direction;
      if (curr == NULL)
         break;
   }
   /*
    * Nul terminate the string.
    */
   *(ptr+pos) = '\0';
   /*
    * Restore the setting of scope_all if we changed it before...
    */
   if (ignore_scope)
      CURRENT_VIEW->scope_all = save_scope_all;

#if defined(PDC_CLIP_SUCCESS)
   if ( pos )
   {
      rc = PDC_setclipboard( (DEFCHAR *)ptr, pos );

      switch( rc )
      {
         case PDC_CLIP_MEMORY_ERROR:
            display_error(30,(CHARTYPE *)"",FALSE);
            TRACE_RETURN();
            return(RC_OUT_OF_MEMORY);
            break;
         case PDC_CLIP_ACCESS_ERROR:
            display_error(186,(CHARTYPE *)"",FALSE);
            rc = RC_INVALID_ENVIRON;
            break;
         default:
            rc = RC_OK;
            break;
      }
   }
#else
   display_error(82,(CHARTYPE *)"CLIP:",FALSE);
   rc = RC_INVALID_ENVIRON;
#endif

   (*the_free)( (void *)ptr);
   TRACE_RETURN();
   return(rc);
}

/*
 * These THE_curs_set() functions required as PDCurses has been changed so that curs_set(1) no longer
 * results in an underline cursor, it uses the cursor shape when the application
 * starts, so if you have a block cursor on starting THE, you would not be able to get
 * an underline cursor.
 */
#if defined(OS2)
# define curs_set THE_curs_set
static void THE_curs_set( int visibility )
{
#ifndef EMXVIDEO
   VIOMODEINFO modeInfo = {0};
   VIOCURSORINFO pvioCursorInfo;
   int pdc_font;
#endif
   int hidden = 0, start = 0, end = 0;

#ifndef EMXVIDEO
   modeInfo.cb = sizeof(modeInfo);
   VioGetMode( &modeInfo, 0 );
   pdc_font = (modeInfo.vres / modeInfo.row);
#endif

   switch( visibility )
   {
      case 0: /* invisible */
#ifdef EMXVIDEO
         start = end = 0;
#else
         start = pdc_font / 4;
         end = pdc_font;
         hidden = -1;
#endif
         break;

      case 2: /* highly visible */
         start = 2;  /* almost full-height block */
         end = pdc_font - 1;
         break;

      default: /* normal visibility */
         start = pdc_font - (pdc_font / 4);
         end = pdc_font - 1;
         break;
   }

#ifdef EMXVIDEO
   if ( !visibility )
      v_hidecursor();
   else
      v_ctype( start, end );
#else
   pvioCursorInfo.yStart = (USHORT)start;
   pvioCursorInfo.cEnd = (USHORT)end;
   pvioCursorInfo.cx = (USHORT)1;
   pvioCursorInfo.attr = hidden;
   VioSetCurType( (PVIOCURSORINFO)&pvioCursorInfo, 0 );
#endif

   /*
    * Tell curses internals we have changed visibility
    */
   SP->visibility = visibility;

   return;
}
#endif

#if defined(WIN32) && !defined(__CYGWIN32__) && !defined(USE_WINGUICURSES)
# define curs_set THE_curs_set
# undef MOUSE_MOVED
# include <windows.h>
static void THE_curs_set(int visibility)
{
   CONSOLE_CURSOR_INFO cci;
   HANDLE hConOut = GetStdHandle(STD_OUTPUT_HANDLE);

   TRACE_FUNCTION("nonansi,c: THE_curs_set");
   if ( GetConsoleCursorInfo( hConOut, &cci ) == FALSE )
   {
      TRACE_RETURN();
      return;
   }

   switch( visibility )
   {
      case 0:    /* invisible */
         cci.bVisible = FALSE;
         break;
      case 2:    /* block */
         cci.bVisible = TRUE;
         cci.dwSize = 95;
         break;
      default:  /* underline */
         cci.bVisible = TRUE;
         cci.dwSize = 25;
         break;
   }

   if ( SetConsoleCursorInfo( hConOut, &cci ) == FALSE )
   {
      TRACE_RETURN();
      return;
   }

   /*
    * Tell curses internals we have changed visibility
    */
   SP->visibility = visibility;

   TRACE_RETURN();
   return;
}
#endif

#if defined(DOS)
# define curs_set THE_curs_set
static void PDC_curs_set(int visibility)
{
   union REGS regs;
   int start, end;

   /* get scrnmode into regs.h.al */
   regs.h.ah = 0x0f;
   int86( 0x10, &regs, &regs );

   switch( visibility )
   {
      case 0:  /* invisible */
         start = 32;
         end = 0;  /* was 32 */
         break;
      case 2:  /* highly visible */
         start = 0;   /* full-height block */
         end = 7;
         break;
      default:  /* normal visibility */
         start = 6;
         end = 7;
         break;
   }

   /* if scrnmode is not set, some BIOSes hang */
   regs.h.ah = 0x01;
   regs.h.ch = (unsigned char)start;
   regs.h.cl = (unsigned char)end;
   int86( 0x10, &regs, &regs );

   /*
    * Tell curses internals we have changed visibility
    */
   SP->visibility = visibility;

   return;
}
#endif

/***********************************************************************/
#ifdef HAVE_PROTO
void draw_cursor(bool visible)
#else
void draw_cursor(visible)
bool visible;
#endif
/***********************************************************************/
{
   TRACE_FUNCTION("nonansi,c: draw_cursor");
#ifdef HAVE_CURS_SET
   if (visible)
   {
      if (INSERTMODEx)
      {
         curs_set(1);   /* First set to displayed... */
         curs_set(2);   /* ...then try to make it more visible */
      }
      else
      {
         curs_set(1);   /* underline cursor */
      }
   }
   else
   {
      curs_set(0);      /* cursor off */
   }
#endif
   TRACE_RETURN();
   return;
}
/*
 * is_a_dir_stat() used when the attributes are obtained from stat()
 */
/*********************************************************************/
#ifdef HAVE_PROTO
int is_a_dir_stat(ATTR_TYPE attrs)
#else
int is_a_dir_stat(attrs)
ATTR_TYPE attrs;
#endif
/*********************************************************************/
{
#if defined(S_IFDIR)
   ATTR_TYPE ftype=(attrs & S_IFMT);

   if (ftype == S_IFDIR)
      return(1);
#else
   if ((attrs & F_DI) == F_DI)
      return(1);
#endif
   return(0);
}

/*
 * is_a_dir_dir() used when the attributes are obtained from _findfirst() on Windows
 */
/*********************************************************************/
#ifdef HAVE_PROTO
int is_a_dir_dir(ATTR_TYPE attrs)
#else
int is_a_dir_dir(attrs)
ATTR_TYPE attrs;
#endif
/*********************************************************************/
{
#if defined(_A_SUBDIR)
   if ( ( attrs & _A_SUBDIR) == _A_SUBDIR )
      return(1);
#elif defined(S_IFDIR)
   ATTR_TYPE ftype=(attrs & S_IFMT);

   if (ftype == S_IFDIR)
      return(1);
#else
   if ((attrs & F_DI) == F_DI)
      return(1);
#endif
   return(0);
}
