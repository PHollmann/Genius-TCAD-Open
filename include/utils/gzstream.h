// ============================================================================
// gzstream, C++ iostream classes wrapping the zlib compression library.
// Copyright (C) 2001  Deepak Bandyopadhyay, Lutz Kettner
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// ============================================================================
//
// File          : gzstream.h
// Revision      : $Revision: 2501 $
// Revision_date : $Date: 2007-11-19 20:33:29 -0600 (Mon, 19 Nov 2007) $
// Author(s)     : Deepak Bandyopadhyay, Lutz Kettner
//
// Standard streambuf implementation following Nicolai Josuttis, "The
// Standard C++ Library".
// ============================================================================




#ifndef GZSTREAM_H
#define GZSTREAM_H 1

#include "config.h"

#ifdef HAVE_HDF5

// standard C++ with new header file names and std:: namespace
#include <iostream>
#include <fstream>
#include <zlib.h>



// ----------------------------------------------------------------------------
// Internal classes to implement gzstream. See below for user classes.
// ----------------------------------------------------------------------------

class gzstreambuf : public std::streambuf {
private:
  static const int bufferSize = 47+256;    // size of data buff
  // totals 512 bytes under g++ for igzstream at the end.

  gzFile           file;               // file handle for compressed file
  char             buffer[bufferSize]; // data buffer
  char             opened;             // open/close state of stream
  int              mode;               // I/O mode

  int flush_buffer();
public:
  gzstreambuf() : opened(0) {
    setp( buffer, buffer + (bufferSize-1));
    setg( buffer + 4,     // beginning of putback area
	  buffer + 4,     // read position
	  buffer + 4);    // end position
    // ASSERT: both input & output capabilities will not be used together
  }
  int is_open() { return opened; }
  gzstreambuf* open( const char* name, int open_mode);
  gzstreambuf* close();
  ~gzstreambuf() { close(); }

  virtual int      overflow( int c = EOF);
  virtual int      underflow();
  virtual int      sync();


  // Backwards compatibility for GCC 2.95.3
#if defined(__GNUC__) && (__GNUC__ < 3) && !defined(__INTEL_COMPILER)
  typedef streampos pos_type;
  typedef streamoff off_type;
#endif

  // Implementation of the seekoff function.
  // The following function only works (so far) for reading.
  // It was written by John W. Peterson.
  virtual pos_type seekoff(off_type ot,
			   std::ios::seekdir,
			   std::ios::openmode);

};

class gzstreambase : virtual public std::ios {
protected:
    gzstreambuf buf;
public:
    gzstreambase() { init(&buf); }
    gzstreambase( const char* name, int open_mode);
    ~gzstreambase();
    void open( const char* name, int open_mode);
    void close();
    gzstreambuf* rdbuf() { return &buf; }
};

// ----------------------------------------------------------------------------
// User classes. Use igzstream and ogzstream analogously to ifstream and
// ofstream respectively. They read and write files based on the gz*
// function interface of the zlib. Files are compatible with gzip compression.
// ----------------------------------------------------------------------------

class igzstream : public gzstreambase, public std::istream {
public:
    igzstream() : std::istream( &buf) {}
    igzstream( const char* name, int open_mode = std::ios::in)
        : gzstreambase( name, open_mode), std::istream( &buf) {}
    gzstreambuf* rdbuf() { return gzstreambase::rdbuf(); }
    void open( const char* name, int open_mode = std::ios::in) {
        gzstreambase::open( name, open_mode);
    }
};

class ogzstream : public gzstreambase, public std::ostream {
public:
    ogzstream() : std::ostream( &buf) {}
    ogzstream( const char* name, int mode = std::ios::out)
        : gzstreambase( name, mode), std::ostream( &buf) {}
    gzstreambuf* rdbuf() { return gzstreambase::rdbuf(); }
    void open( const char* name, int open_mode = std::ios::out) {
        gzstreambase::open( name, open_mode);
    }
};

#endif

// ============================================================================
// EOF //

#endif // HAVE_ZLIB_H
